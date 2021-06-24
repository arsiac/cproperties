#ifndef CPP_PROPERTIES_H
#define CPP_PROPERTIES_H

#include <cstdio>
class Properties
{
public:
    static int ARRAY_SIZE;
    static int PROPERTY_MAX_SIZE;

    Properties();
    explicit Properties(const char *path);
    ~Properties();

    void load(const char *path);
    void save(const char *path);
    int size() const;
    void clear();
    void remove(const char *key);
    void set(const char *key, const char *value);
    char *get(const char *key);

private:
    class Node;

    Node **properties;
    int propertySize;

    static size_t fileSize(FILE *file);
    static char *readFile(FILE *file);
    static bool isBlank(char c);
    void analyze(const char *str);
    void append(Node *node);
    Node *find(const char *key);
};

class Properties::Node
{
public:
    Node();
    Node(const char *key, const char *value);
    ~Node();

    void setKey(const char *k);
    void setValue(const char *v);
    void setNext(Node *next);
    void setPrevious(Node *previous);

    char *getKey();
    char *getValue();
    Node *getNext();
    Node *getPrevious();

    int hashCode() const;
    bool equals(Node *other);
    bool keyEquals(const char *k);

private:
    int keyHashCode;
    char *key;
    char *value;
    Node *next;
    Node *previous;

    static int hash(const char *value);
    static bool stringCompare(const char *src, const char *dst);
    static char *stringCopy(const char *src);
};

#endif