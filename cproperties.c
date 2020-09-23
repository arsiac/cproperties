#include "cproperties.h"

// 计算字符串哈希值
int stringHash(const char *str)
{
    int hash = 0, length;
    if (str)
    {
        length = strlen(str);
        for (int i = 0; i < length; i++)
            hash = 31 * hash + str[i];
    }
    return hash;
}

// 获取文件大小
long getFileSize(FILE *file)
{
    if (!file)
        return 0;

    long locationCache, size;
    locationCache = ftell(file);
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, locationCache, SEEK_SET);
    return size;
}

cNode *newNode(const int hash, const char *key, const char *value)
{
    // key不能为NULL
    if (!key)
    {
        return NULL;
    }

    int keylen, vallen;

    keylen = strlen(key);
    if (keylen <= 0)
    {
        return NULL;
    }

    vallen = value == NULL ? 0 : strlen(value);

    cNode *node = (cNode *)malloc(sizeof(cNode));
    node->hash = hash;
    node->next = NULL;

    node->key = (char *)malloc(sizeof(char) * keylen + 1);
    strcpy(node->key, key);

    if (vallen)
    {
        node->value = (char *)malloc(sizeof(char) * vallen + 1);
        strcpy(node->value, value);
    }
    else
        node->value = NULL;

    return node;
}

void freeNode(cNode *node)
{
    if (node)
    {
        if (node->key)
            free(node->key);
        if (node->value)
            free(node->value);
        free(node);
        node = NULL;
    }
}

cProperties *newProperties()
{
    cProperties *properties = (cProperties *)malloc(sizeof(cProperties));

    properties->size = CPROPERTIES_ARRAY_SIZE;
    properties->array = (cNode **)malloc(CPROPERTIES_ARRAY_SIZE * sizeof(cNode *));

    // 初始化数组
    for (int i = 0; i < CPROPERTIES_ARRAY_SIZE; i++)
        properties->array[i] = NULL;

    return properties;
}

void freeProperties(cProperties *cp)
{
    if (!cp)
    {
        return;
    }

    if (cp->array)
    {
        cNode *temp1 = NULL, *temp2 = NULL;
        for (int i = 0; i < cp->size; i++)
        {
            temp1 = cp->array[i];
            while (temp1)
            {
                temp2 = temp1;
                temp1 = temp2->next;
                freeNode(temp2);
            }
        }
        free(cp->array);
    }

    free(cp);
    cp = NULL;
}

// 用hash得到节点
cNode *getNodeFromHash(const cProperties *cp, const int hash)
{
    // 计算链表索引
    int index = hash % cp->size;

    cNode *temp = cp->array[index];
    while (temp)
        if (temp->hash == hash)
            return temp;

    return NULL;
}

// hash对应的节点是否存在
int containsKeyHash(const cProperties *cp, const int hash)
{
    return NULL != getNodeFromHash(cp, hash);
}

// key是否存在
int cPropertiesContainsKey(const cProperties *cp, const char *key)
{
    // 为NULL的key不存在
    if (!key)
        return 0;

    return containsKeyHash(cp, stringHash(key));
}

// 新增或者修改key对应的值
int cPropertiesSet(const cProperties *cp, const char *key, const char *value)
{
    // key为NULL时不进行操作
    if (!key)
        return FALSE;

    // 计算key的hash
    int hash = stringHash(key);
    if (!hash)
    {
        return FALSE;
    }

    int index = hash % cp->size;
    index = index < 0 ? -index : index;
    cNode *temp = NULL;
    // 链表表头为NULL
    if (cp->array[index] == NULL)
    {
        cp->array[index] = newNode(hash, key, value);
    }
    else
    {
        temp = cp->array[index];
        do
        {
            // 表中已有该key， 修改value
            if (temp->hash == hash)
            {
                free(temp->value);
                temp->value = NULL;
                if (value)
                {
                    int vallen = strlen(value);
                    if (vallen > 0)
                    {
                        temp->value = (char *)malloc(vallen * sizeof(char) + 1);
                        strcpy(temp->value, value);
                    }
                }
                break;
            }
            else if (!temp->next)
            {
                temp->next = newNode(hash, key, value);
                break;
            }
            else
            {
                temp = temp->next;
            }
        } while (temp);
    }

    return TRUE;
}

const char *cPropertiesGet(const cProperties *cp, const char *key)
{
    cNode *node = getNodeFromHash(cp, stringHash(key));
    if (node)
        return node->value;
    else
        return NULL;
}

int cPropertiesDelete(const cProperties *cp, const char *key)
{
    if (!key)
        return FALSE;
    int hash = stringHash(key);
    int arrayIndex = hash % cp->size;

    // 如果链表开始节点是要删除的节点
    cNode *temp = NULL;
    if (cp->array[arrayIndex]->hash == hash)
    {
        temp = cp->array[arrayIndex];
        cp->array[arrayIndex] = temp->next;
        freeNode(temp);
        return TRUE;
    }
    else
    {
        cNode *node;
        for (node = cp->array[arrayIndex], temp = node->next; node && temp; node = temp, temp = temp->next)
            if (temp->hash == hash)
            {
                node->next = temp->next;
                freeNode(temp);
                return TRUE;
            }
    }
    return FALSE;
}

void printProperties(const cProperties *cp)
{
    if (!cp || !cp->array)
    {
        puts("(null)");
        return;
    }

    cNode *temp;
    for (int i = 0; i < cp->size; i++)
    {
        if (cp->array[i])
        {
            temp = cp->array[i];
            while (temp)
            {
                printf("%s=%s hash: %d\n", temp->key, temp->value, temp->hash);
                temp = temp->next;
            }
        }
    }
}

void printNode(const cNode *node)
{
    if (!node || !node->key)
    {
        puts("(null)");
        return;
    }

    printf("key: %s\tvalue: %s\thash: %d\n", node->key, node->value, node->hash);
}

void printStructure(const cProperties *cp)
{
    if (!cp || !cp->array)
    {
        puts("(null)");
        return;
    }

    cNode *temp;
    for (int i = 0; i < cp->size; i++)
    {
        temp = cp->array[i];
        if (temp)
        {
            printf("array[%d]: ", i);
            while (temp)
            {
                if (temp->next)
                {
                    printf("%s:%s -> ", temp->key, temp->value);
                }
                else
                {
                    printf("%s:%s", temp->key, temp->value);
                }
                temp = temp->next;
            }
            putchar('\n');
        }
    }
}

cProperties *createPropertiesFromFile(const char *path)
{
    if (!path)
    {
        puts("cproperties: the file path cannot be NULL.");
        return NULL;
    }

    FILE *file = fopen(path, "rb");
    if (!file)
    {
        printf("cproperties: failed to open file.[%s]", path);
        return NULL;
    }

    long fileSize = getFileSize(file);
    if (!fileSize)
    {
        printf("cproperties: file is empty.[%s]", path);
        return NULL;
    }
    char *fileBuffer = (char *)malloc(fileSize * sizeof(char) + 1);
    if (!fileBuffer)
    {
        puts("cproperties: buffer request failed.");
        return NULL;
    }
    fread(fileBuffer, sizeof(char), fileSize, file);
    fileBuffer[fileSize] = '\0';
    fclose(file);

    cProperties *cp = newProperties();
    char tempKey[CPROPERTIES_KEY_MAX_SIZE],
        tempValue[CPROPERTIES_VALUE_MAX_SIZE];
    int bufferPoint = 0,
        lineCount = 0,
        copyCount = 0;

    enum status
    {
        start,
        annotation,
        key,
        value,
        error,
        end
    };
    enum status state = start;
    while (TRUE)
    {
        if (state == end)
        {
            break;
        }

        switch (state)
        {
        case start:
            lineCount++;
            while (fileBuffer[bufferPoint] == ' ' || fileBuffer[bufferPoint] == '\n' || fileBuffer[bufferPoint] == '\r')
            {
                bufferPoint++;
            }
            if (fileBuffer[bufferPoint] == '#')
            {
                state = annotation;
            }
            else if (fileBuffer[bufferPoint] == '=')
            {
                state = error;
            }
            else if (fileBuffer[bufferPoint] == '\0')
            {
                state = end;
            }
            else
            {
                state = key;
            }
            break;
        case annotation:
            while (fileBuffer[bufferPoint] != '\n' && fileBuffer[bufferPoint] != '\r' && fileBuffer[bufferPoint] != '\0')
            {
                bufferPoint++;
            }
            if (fileBuffer[bufferPoint] == '\n' || fileBuffer[bufferPoint] == '\r')
            {
                bufferPoint++;
                state = start;
            }
            if (fileBuffer[bufferPoint] == '\0')
            {
                state = end;
            }
            break;
        case key:
            copyCount = 0;
            while (fileBuffer[bufferPoint] != ' ' &&
                   fileBuffer[bufferPoint] != '=' &&
                   fileBuffer[bufferPoint] != '\n' &&
                   fileBuffer[bufferPoint] != '\r' &&
                   fileBuffer[bufferPoint] != '\0')
            {
                if (copyCount >= CPROPERTIES_KEY_MAX_SIZE)
                {
                    break;
                }
                tempKey[copyCount++] = fileBuffer[bufferPoint++];
            }
            tempKey[copyCount] = '\0';

            while (fileBuffer[bufferPoint] == ' ')
            {
                bufferPoint++;
            }
            if (fileBuffer[bufferPoint] != '=')
            {
                state = error;
            }
            else
            {
                bufferPoint++;
                state = value;
            }
            break;
        case value:
            while (fileBuffer[bufferPoint] == ' ')
            {
                bufferPoint++;
            }
            copyCount = 0;
            while (fileBuffer[bufferPoint] != '\n' &&
                   fileBuffer[bufferPoint] != '\r' &&
                   fileBuffer[bufferPoint] != '\0' &&
                   fileBuffer[bufferPoint] != '#')
            {
                if (copyCount >= CPROPERTIES_VALUE_MAX_SIZE)
                {
                    break;
                }
                tempValue[copyCount++] = fileBuffer[bufferPoint++];
            }
            tempValue[copyCount] = '\0';
            // printf("key: [%s], value: [%s]\n", tempKey, tempValue);
            cPropertiesSet(cp, tempKey, tempValue);
            if (fileBuffer[bufferPoint] == '\n' || fileBuffer[bufferPoint] == '\r')
            {
                bufferPoint++;
                state = start;
            }
            else if (fileBuffer[bufferPoint] == '#')
            {
                state = annotation;
            }
            else if (fileBuffer[bufferPoint] == '\0')
            {
                state = end;
            }
            break;
        case error:
            printf("error: %d\n", lineCount);
            while (fileBuffer[bufferPoint] != '\n' && fileBuffer[bufferPoint] != '\r' && fileBuffer[bufferPoint] != '\0')
            {
                bufferPoint++;
            }
            if (fileBuffer[bufferPoint] == '\n' || fileBuffer[bufferPoint] == '\r')
            {
                state = start;
            }
            else if (fileBuffer[bufferPoint] == '\0')
            {
                state = end;
            }
            break;
        default:
            break;
        }
    }
    free(fileBuffer);
    return cp;
}