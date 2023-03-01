#ifndef _parserDef
#define _parserDef

#include <stdbool.h>

typedef struct map
{
    char *key;
    int value;
} map;

typedef struct treeNode
{
    struct treeNode *child;
    struct treeNode *parent;
    struct treeNode *left;
    struct treeNode *right;
    char *data;
} treeNode;

struct nonTerminalStruct
{
    char *nonTerminal;
    bool hasEpsilon;
    struct Node *first;
    struct Node *follow;
    bool completed; // means it has completed calculation of its first
};

struct Node
{
    struct Node *next;
    struct Node *prev;
    char *data;
};

#endif