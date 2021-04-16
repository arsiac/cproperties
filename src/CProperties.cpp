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
}

ac::CProperties::~CProperties()
{
    clear();
    free(properties);
}

/* Private */
void ac::CProperties::initialize()
{
    properties = (Node **)malloc(ARRAY_SIZE * sizeof(Node *));

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        properties[i] = nullptr;
    }

    propertySize = 0;
}

long ac::CProperties::fileSize(FILE *file)
{
    if (!file)
    {
        return 0;
    }
    long locationCache, size;
    locationCache = ftell(file);
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, locationCache, SEEK_SET);
    return size;
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
            free(current);
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
        free(key);
    }

    if (value != nullptr)
    {
        free(value);
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

    char *dest = (char *)malloc((length + 1) * sizeof(char));

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
        free(this->key);
    }
    this->key = strcpy(key);
}

void ac::CProperties::Node::setValue(const char *value)
{
    if (this->value != nullptr)
    {
        free(this->value);
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