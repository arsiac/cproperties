#ifndef CPROPERTIES_H
#define CPROPERTIES_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef NULL
#define NULL ((void *)0)
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

cProperties *newProperties();
void freeProperties(cProperties *cp);

void cPropertiesSet(const cProperties *cp, const char *key, const char *value);
const char *cPropertiesGet(const cProperties *cp, const char *key);
int cPropertiesContainsKey(const cProperties *cp, const char *key);
//void cPropertiesDelete(const cProperties *cp, const char *key);

void printfProperties(const cProperties *cp);
void printfNode(const cNode *node);
#endif