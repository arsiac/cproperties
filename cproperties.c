#include "cproperties.h"

// 计算字符串哈希值
int stringHash(const char *str)
{
    int hash = 0, length;
    if (str)
    {
        length = strlen(str);
        if (length > 0)
            for (int i = 0; i < length; i++) 
                hash = 31 * hash + str[i];
    }
    return hash;
}

cNode *newNode(const int hash, const char *key, const char *value)
{
    // key不能为NULL 
    if (!key) return NULL;

    cNode *node = (cNode *)malloc(sizeof(cNode));
    int keylen, vallen;

    keylen = strlen(key);
    vallen = value == NULL ? 0 : strlen(value);

    node->key = (char *)malloc(sizeof(char) * keylen + 1);
    strcpy(node->key, key);

    if (vallen)
    {
        node->value = (char *)malloc(sizeof(char) * vallen + 1);
        strcpy(node->value, value);
    }
    else node->value = NULL;

    node->hash = hash;

    node->next = NULL;

    return node;
}

void freeNode(cNode *node)
{
    if (node)
    {
        if (node->key) free(node->key);
        if (node->value) free(node->value);
        free(node);
        node = NULL;
    }
}

cProperties *newProperties()
{
    cProperties *properties = (cProperties *)malloc(sizeof(cProperties));

    properties->size = CPROPERTIES_ARRAY_SIZE;
    properties->array = (cNode **)malloc(CPROPERTIES_ARRAY_SIZE * sizeof(cNode*));

    // 初始化数组
    for (int i=0; i<CPROPERTIES_ARRAY_SIZE; i++)
        properties->array[i] = NULL;

    return properties;
}

void freeProperties(cProperties *cp)
{
    if (!cp) return;
    
    if (cp->array)
    {
        cNode *temp1 = NULL, *temp2 = NULL;
        for (int i=0; i<cp->size; i++)
        {
            temp1 = cp->array[i];
            while(temp1)
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
    while(temp)
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
    if (!key) return 0;

    return containsKeyHash(cp, stringHash(key));
}

// 新增或者修改key对应的值
void cPropertiesSet(const cProperties *cp, const char *key, const char *value)
{
    // key为NULL时不进行操作
    if (!key) return;
    
    // 计算key的hash
    int hash = stringHash(key);

    // 尝试寻找key所在的节点
    cNode *node = getNodeFromHash(cp, hash);

    // key的节点已经存在则修改value
    if (node)
    {
        // 释放原来的value内存
        if (node->value) free(node->value);

        // value不为NULL且长度大于0才分配内存存储value
        if (value)
        {
            int length = strlen(value);
            if (length > 0)
            {
                node->value = (char *)malloc(length * sizeof(char) + 1);
                strcpy(node->value, value);
            }
            else 
                node->value = NULL;
        }
        else 
            node->value = NULL;
        
    }
    // key不在已知节点中则插入新节点
    else
    {
        node = newNode(hash, key, value);
        int arrayIndex = hash % cp->size;
        cNode *temp = cp->array[arrayIndex];
        if (temp)
            while(temp)
            {
                if (!temp->next)
                {
                    temp->next = node;
                    break;
                }
                temp = temp->next;
            }
        else
            cp->array[arrayIndex] = node;
        
    }
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
    if (!key) return FALSE;
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

void printfProperties(const cProperties *cp)
{
    if (!cp || !cp->array) 
    {
        puts("(null)");
        return;
    }

    cNode *temp;
    for (int i=0; i<cp->size; i++)
    {
        if (cp->array[i])
        {
            temp = cp->array[i];
            while(temp)
            {
                printf("%s=%s hash: %d\n", temp->key, temp->value, temp->hash);
                temp = temp->next;
            }
        }
    }
}


void printfNode(const cNode *node)
{
    if (!node || !node->key)
    {
        puts("(null)");
        return;
    }

    printf("key: %s\tvalue: %s\thash: %d\n", node->key, node->value, node->hash);
}