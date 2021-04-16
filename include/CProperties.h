#ifndef CPP_CPROPERTIES_H
#define CPP_CPROPERTIES_H

#include <cstdio>
#include <cstdlib>

namespace ac
{
    class CProperties
    {
    public:
        static int ARRAY_SIZE;
        static int PROPERTY_MAX_SIZE;

        CProperties();
        CProperties(const char *path);
        ~CProperties();

        int size();
        void clear();
        void remove(const char *key);
        void set(const char *key, const char *value);
        char *get(const char *key);

    private:
        // properties structure
        class Node;

        Node **properties;
        int propertySize;

        void initialize();
        long fileSize(FILE *file);
        void append(Node *node);
        Node *find(const char *key);
    };

    class CProperties::Node
    {
    public:
        Node();
        Node(const char *key, const char *value);
        ~Node();

        void setKey(const char *key);
        void setValue(const char *value);
        void setNext(Node *next);
        void setPrevious(Node *previous);

        char *getKey();
        char *getValue();
        Node *getNext();
        Node *getPrevious();

        int hashCode();
        bool equals(Node *other);
        bool keyEquals(const char *key);

    private:
        int keyHashCode;
        char *key;
        char *value;
        Node *next;
        Node *previous;

        int hash(const char *value);
        bool strcmp(const char *src, const char *dst);
        char *strcpy(const char *src);
    };
};

#endif