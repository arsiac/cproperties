#include "CProperties.h"

/***********************
 *     CProperties     *
 ***********************/

/* Static */
int ac::CProperties::ARRAY_SIZE = 128;
int ac::CProperties::PROPERTY_MAX_SIZE = 512;

/* Construct */
ac::CProperties::CProperties()
{
    initialize();
}

ac::CProperties::CProperties(const char *path)
{
    initialize();
    load(path);
}

ac::CProperties::~CProperties()
{
    clear();
    delete[] properties;
}

/* Private */
void ac::CProperties::initialize()
{
    if (properties != nullptr)
    {
        clear();
        delete[] properties;
    }

    properties = new Node *[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        properties[i] = nullptr;
    }

    propertySize = 0;
}

size_t ac::CProperties::fileSize(FILE *file)
{
    if (file == nullptr)
    {
        return (size_t)0;
    }

    long locationCache;
    size_t size;

    locationCache = ftell(file);
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, locationCache, SEEK_SET);
    return size;
}

char *ac::CProperties::readFile(FILE *file)
{
    if (file == nullptr)
    {
        return nullptr;
    }

    // get file size
    size_t bufferSize = fileSize(file);

    if (bufferSize == 0)
    {
        return nullptr;
    }

    // set cursor to file header
    fseek(file, (long)0, SEEK_SET);

    // create buffer
    char *fileBuffer = new char[bufferSize + 1];
    fread(fileBuffer, sizeof(char), bufferSize, file);

    // end of buffer
    fileBuffer[bufferSize] = '\0';

    return fileBuffer;
}

bool ac::CProperties::isBlank(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

void ac::CProperties::analyze(const char *str)
{
    // check
    if (str == nullptr)
    {
        puts("analyze: null pointer -> str");
        return;
    }

    if (properties == nullptr)
    {
        initialize();
    }

    enum status
    {
        ready,
        key,
        devision,
        value,
        annotation,
        error,
        end
    };
    status state = ready;
    unsigned long count = 0;
    int propertyCount = 0;
    char *property = new char[PROPERTY_MAX_SIZE + 1];
    Node node;

    // start analyze
    while (state != end)
    {
        switch (state)
        {
        case ready:
            if (!isBlank(str[count]))
            {
                if (str[count] == '#')
                {
                    state = annotation;
                }
                else if (str[count] == '\0')
                {
                    state = end;
                }
                else
                {
                    state = key;
                    propertyCount = 0;
                    property[propertyCount++] = str[count];
                }
            }
            break;

        case key:
            if (str[count] != '=')
            {
                if (str[count] == '\0' || str[count] == '\n')
                {
                    state = error;
                }
                else
                {
                    property[propertyCount++] = str[count];
                }
            }
            else
            {
                state = devision;

                // end of key
                property[propertyCount] = '\0';

                // remove blank
                while (isBlank(property[--propertyCount]))
                {
                    property[propertyCount] = '\0';
                }
                node.setKey(property);
            }
            break;

        case devision:
            if (!isBlank(str[count]))
            {
                state = value;
                propertyCount = 0;
                property[propertyCount++] = str[count];
            }

            break;

        case value:
            if (str[count] != '\n' && str[count] != '\r' && str[count] != '\0' && str[count] != '#')
            {
                property[propertyCount++] = str[count];
            }
            else
            {
                if (str[count] == '\0')
                {
                    state = end;
                }
                else if (str[count] == '#')
                {
                    state = annotation;
                }
                else
                {
                    // CRLF
                    if (str[count] == '\r' && str[count + 1] == '\n')
                    {
                        count++;
                    }

                    state = ready;
                }

                // end of value
                property[propertyCount] = '\0';

                // remove blank
                while (isBlank(property[--propertyCount]))
                {
                    property[propertyCount] = '\0';
                }
                node.setValue(property);
                set(node.getKey(), node.getValue());
            }
            break;

        case annotation:
            if (str[count] == '\n' || str[count] == '\r')
            {
                // CRLF
                if (str[count] == '\r' && str[count + 1] == '\n')
                {
                    count++;
                }
                state = ready;
            }
            break;

        case error:
            property[propertyCount] = '\0';
            printf("analyze: syntax error -> [%s]\n", property);
            state = ready;
            break;

        default:
            printf("analyze: unknown status -> %d\n", state);
            break;
        }
        count++;
    }

    delete[] property;
}

void ac::CProperties::append(Node *node)
{
    // node == null
    if (node == nullptr)
    {
        return;
    }

    int index = node->hashCode() % ARRAY_SIZE;
    Node *linkRoot = properties[index];

    // first node is nullptr
    if (linkRoot == nullptr)
    {
        properties[index] = node;
    }
    // add node to the tail
    else
    {
        Node *inode = linkRoot;
        while (inode->getNext() != nullptr)
        {
            inode = inode->getNext();
        }
        inode->setNext(node);
        node->setPrevious(inode);
    }

    propertySize++;
}

ac::CProperties::Node *ac::CProperties::find(const char *key)
{
    Node node(key, nullptr);
    int index = node.hashCode() % ARRAY_SIZE;

    Node *linkNode = properties[index];
    while (linkNode != nullptr)
    {
        // find the node
        if (linkNode->hashCode() == node.hashCode() &&
            linkNode->keyEquals(node.getKey()))
        {
            return linkNode;
        }
        else
        {
            linkNode = linkNode->getNext();
        }
    }

    // not found
    return nullptr;
}

/* Public */
void ac::CProperties::load(const char *path)
{
    if (path == nullptr)
    {
        puts("load: file path is null pointer.");
        return;
    }

    FILE *file = fopen(path, "rb");
    if (file == nullptr)
    {
        printf("load: open file failed.[%s]\n", path);
        return;
    }

    char *buffer = readFile(file);
    fclose(file);
    if (buffer == nullptr)
    {
        printf("load: read file failed.[%s]\n", path);
        return;
    }

    analyze(buffer);
}

void ac::CProperties::save(const char *path)
{
    if (path == nullptr)
    {
        puts("save: file path is null pointer.");
        return;
    }

    FILE *file = fopen(path, "w+");
    if (file == nullptr)
    {
        printf("save: open/create file failed.[%s]\n", path);
        return;
    }

    Node *node = nullptr;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        node = properties[i];
        while (node != nullptr)
        {
            fprintf(file, "%s=%s\n", node->getKey(), node->getValue());
            node = node->getNext();
        }
    }
    fclose(file);
}

int ac::CProperties::size()
{
    return propertySize;
}

void ac::CProperties::clear()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        Node *current = properties[i],
             *next;

        while (current)
        {
            next = current ? current->getNext() : nullptr;
            delete current;
            current = next;
        }
    }
}

void ac::CProperties::remove(const char *key)
{
    Node *target = find(key);

    // not found
    if (target == nullptr)
    {
        return;
    }

    // first node
    if (target->getPrevious() == nullptr)
    {
        properties[target->hashCode() % ARRAY_SIZE] = target->getNext();
    }
    else
    {
        target->getPrevious()->setNext(target->getNext());
    }
    delete target;
}

void ac::CProperties::set(const char *key, const char *value)
{
    Node *target = find(key);

    // insert
    if (target == nullptr)
    {
        append(new Node(key, value));
    }
    // alter
    else
    {
        target->setValue(value);
    }
}

char *ac::CProperties::get(const char *key)
{
    Node *target = find(key);
    return target == nullptr ? nullptr : target->getValue();
}

/***************************
 *    CProperties::Node    *
 ***************************/

/* Construct */
ac::CProperties::Node::Node()
{
    this->keyHashCode = 0;
    this->key = nullptr;
    this->value = nullptr;
    this->next = nullptr;
    this->previous = nullptr;
}

ac::CProperties::Node::Node(const char *key, const char *value)
{
    this->keyHashCode = hash(key);
    this->key = strcpy(key);
    this->value = strcpy(value);
    this->next = nullptr;
    this->previous = nullptr;
}

ac::CProperties::Node::~Node()
{
    if (key != nullptr)
    {
        delete[] key;
    }

    if (value != nullptr)
    {
        delete[] value;
    }
}

/* Private */
int ac::CProperties::Node::hash(const char *value)
{
    int result = 0;

    if (value != nullptr)
    {
        int count = 0;
        char character;
        // '\0' -> 0
        while (0 != (character = value[count++]))
        {
            result = result * 31 + character;
        }
    }

    return result < 0 ? -result : result;
}

bool ac::CProperties::Node::strcmp(const char *src, const char *dst)
{
    int ret = 0;
    while (!(ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
    {
        ++src, ++dst;
    }

    if (ret < 0)
    {
        ret = -1;
    }
    else if (ret > 0)
    {
        ret = 1;
    }
    return ret;
}

char *ac::CProperties::Node::strcpy(const char *src)
{
    if (src == nullptr)
    {
        return nullptr;
    }

    int length = 0;
    const char *str = src;
    while (*str != '\0')
    {
        length++, str++;
    }

    char *dest = new char[length + 1];

    dest[length] = '\0';
    for (int i = 0; i < length; i++)
    {
        dest[i] = src[i];
    }

    return dest;
}

/* Public */
void ac::CProperties::Node::setKey(const char *key)
{
    if (this->key != nullptr)
    {
        delete[] this->key;
    }
    this->key = strcpy(key);
}

void ac::CProperties::Node::setValue(const char *value)
{
    if (this->value != nullptr)
    {
        delete[] this->value;
    }
    this->value = strcpy(value);
}

void ac::CProperties::Node::setNext(ac::CProperties::Node *node)
{
    if (this->next != nullptr)
    {
        delete this->next;
    }
    this->next = node;
}

void ac::CProperties::Node::setPrevious(ac::CProperties::Node *node)
{
    if (this->previous != nullptr)
    {
        delete this->previous;
    }
    this->previous = previous;
}

char *ac::CProperties::Node::getKey()
{
    return key;
}

char *ac::CProperties::Node::getValue()
{
    return value;
}

ac::CProperties::Node *ac::CProperties::Node::getNext()
{
    return next;
}

ac::CProperties::Node *ac::CProperties::Node::getPrevious()
{
    return previous;
}

int ac::CProperties::Node::hashCode()
{
    return keyHashCode;
}

bool ac::CProperties::Node::equals(Node *other)
{
    return this->hashCode() == other->hashCode() &&
           0 == strcmp(this->getKey(), other->getKey()) &&
           0 == strcmp(this->getValue(), other->getValue());
}

bool ac::CProperties::Node::keyEquals(const char *key)
{
    return 0 == strcmp(this->getKey(), key);
}