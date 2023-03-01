#ifndef _parserDef
#define _parserDef


typedef struct map
{
    char *key;
    int value;
} map;

typedef struct treeNode
{
    treeNode *child;
    treeNode *parent;
    treeNode *left;
    treeNode *right;
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