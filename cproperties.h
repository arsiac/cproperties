#ifndef CPROPERTIES_H
#define CPROPERTIES_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0 
#endif

#define CPROPERTIES_ARRAY_SIZE 128

typedef struct __Node {
    int hash;
    char *key;
    char *value;
    struct __Node *next;
} cNode;

typedef struct {
    cNode **array;
    int size;
} cProperties;

cNode *newNode(const int hash, const char *key, const char *value);
void freeNode(cNode *node);
cProperties *newProperties();
void freeProperties(cProperties *cp);

void cPropertiesSet(const cProperties *cp, const char *key, const char *value);
const char *cPropertiesGet(const cProperties *cp, const char *key);
int cPropertiesContainsKey(const cProperties *cp, const char *key);
int cPropertiesDelete(const cProperties *cp, const char *key);

void printfProperties(const cProperties *cp);
void printfNode(const cNode *node);
#endif