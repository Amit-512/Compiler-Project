#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define rows 142
#define TABLE_SIZE 100
#define MAX_PROBE 10
#define number_nt 71
#define number_t 59
typedef struct map
{
    char *key;
    int value;
} map;

map *hashTableNonTerminals[TABLE_SIZE];
map *hashTableTerminals[TABLE_SIZE];
// hash function using quadratic probing
int hash(char *key, int i)
{
    int hashval = 0;
    for (int j = 0; j < strlen(key); j++)
    {
        hashval += key[j];
    }
    return (hashval + i * i) % TABLE_SIZE;
}

// insert a key-value pair into the hash table
void insertNT(char *key, int value)
{
    int i = 0;
    int index = hash(key, i);
    while (hashTableNonTerminals[index] != NULL)
    {
        i++;
        if (i > MAX_PROBE)
        {
            printf("Error: Maximum probing limit reached\n");
            return;
        }
        index = hash(key, i);
    }
    map *new = (map *)malloc(sizeof(map));
    new->key = key;
    new->value = value;
    hashTableNonTerminals[index] = new;
}
void insertT(char *key, int value)
{
    int i = 0;
    int index = hash(key, i);
    while (hashTableTerminals[index] != NULL)
    {
        i++;
        if (i > MAX_PROBE)
        {
            printf("Error: Maximum probing limit reached\n");
            return;
        }
        index = hash(key, i);
    }
    map *new = (map *)malloc(sizeof(map));
    new->key = key;
    new->value = value;
    hashTableTerminals[index] = new;
}
// get the value associated with the given key from the hash table
int getNT(char *key)
{
    int i = 0;
    int index = hash(key, i);
    while (hashTableNonTerminals[index] != NULL)
    {
        if (strcmp(hashTableNonTerminals[index]->key, key) == 0)
        {
            return hashTableNonTerminals[index]->value;
        }
        i++;
        if (i > MAX_PROBE)
        {
            printf("Error: Maximum probing limit reached\n");
            return 0;
        }
        index = hash(key, i);
    }
    return 0;
}
int getT(char *key)
{
    int i = 0;
    int index = hash(key, i);
    while (hashTableTerminals[index] != NULL)
    {
        if (strcmp(hashTableTerminals[index]->key, key) == 0)
        {
            return hashTableTerminals[index]->value;
        }
        i++;
        if (i > MAX_PROBE)
        {
            printf("Error: Maximum probing limit reached\n");
            return 0;
        }
        index = hash(key, i);
    }
    return 0;
}

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
    char *data;
    struct Node *prev;
};

struct nonTerminalStruct nts[number_nt]; // array of stucts

char *Terminals[] = {"ID", "TRUE", "FALSE", "COMMENT", "AND", "OR", "INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", "USE", "WITH", "PARAMETERS", "TAKES", "INPUT", "RETURNS", "FOR", "IN", "SWITCH", "CASE", "BREAK", "DEFAULT", "WHILE", "NUM", "RNUM", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF", "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "LEXERROR", "TK_EOF"};
char *nonTerminals[] = {"program", "moduleDeclarations", "moduleDeclaration", "otherModules", "driverModule", "module", "ret", "input_plist", "n1", "output_plist", "n2", "dataType", "range_arrays", "type", "moduleDef", "statements", "statement", "ioStmt", "boolConst", "id_num_rnum", "var_print", "p1", "simpleStmt", "assignmentStmt", "whichStmt", "lvalueIDStmt", "lvalueARRStmt", "index_arr", "new_index", "sign", "moduleReuseStmt", "optional", "idList", "n3", "actual_para_list", "expression", "u", "new_NT", "var_id_num", "unary_op", "arithmeticOrBooleanExpr", "n7", "anyTerm", "n8", "arithmeticExpr", "n4", "term", "n5", "factor", "n_11", "element_index_with_expressions", "n_10 ", "arrExpr", "arr_n4", "arrTerm", "arr_n5", "arrFactor", "op1", "logicalOp", "relationalOp", "declareStmt", "conditionalStmt", "caseStmts", "n9", "value", "default", "iterativeStmt", "range_for_loop", "index_for_loop", "new_index_for_loop", "sign_for_loop"};
char *getRule(FILE *grammer)
{
    char *buff;
    buff = (char *)malloc(200 * sizeof(char));
    fgets(buff, 200, grammer);
    return buff;
}

bool isTerminal(char *data) // works
{
    return data[0] >= 65 && data[0] <= 90;
}
bool isEpsilon(char *s) // works
{
    return (strcmp(s, "e") == 0) || (strcmp(s, "e\r") == 0) || (strcmp(s, "e\n") == 0);
}

struct Node *copy(struct Node *n1, struct Node *n2)
{
    struct Node *temp = n1;
    while (n2 != NULL)
    {
        temp->next = (struct Node *)malloc(sizeof(struct Node));
        (temp->next)->data = n2->data;
        (temp->next)->prev = temp;
        temp = temp->next;
    }
    return temp;
}
void initHashTableNT(char **nonTerminals)
{
    for (int i = 0; i < number_nt; i++)
    {
        insertNT(nonTerminals[i], i);
    }
}
void initHashTableT(char **Terminals)
{
    for (int i = 0; i < number_t; i++)
    {
        insertT(Terminals[i], i);
    }
}
struct Node *init(char *s)
{
    struct Node *curr;
    curr = (struct Node *)malloc(sizeof(struct Node));
    curr->data = s;
    return curr;
}

void push(struct Node *curr, char *s)
{
    curr->next = (struct Node *)malloc(sizeof(struct Node));
    (curr->next)->data = s;
    (curr->next)->prev = curr;
}
struct Node *initcopy(struct Node *rec, struct Node *header)
{
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    header = temp;
    temp->data = rec->data;
    while (rec->next != NULL)
    {
        temp->next = (struct Node *)malloc(sizeof(struct Node));
        (temp->next)->data = (rec->next)->data;
        (temp->next)->prev = temp;
        temp = temp->next;
        rec = rec->next;
    }
    return temp;
}
struct Node *findFirst(struct Node **rules, char *nonTerminal)
{
    if (nts[getNT(nonTerminal)].first != NULL)
    {
        printf("first of %s already calculated\n", nonTerminal);
        return nts[getNT(nonTerminal)].first;
    }
    else
    {
        printf("first of %s not calculated\n", nonTerminal);
    }

    struct Node *currNode = NULL; // first
    struct Node *header = NULL;
    for (int i = 0; i < rows; i++)
    {
        if (strcmp(rules[i]->data, nonTerminal) == 0)
        {
            struct Node *temp = rules[i]->next; // first node of rhs
            int lhs = getNT(rules[i]->data);
            if (isTerminal(temp->data))
            {
                printf("terminal %s\n", temp->data);
                if (currNode == NULL)
                {
                    header = init(temp->data);
                    currNode = header;
                }
                else
                {
                    push(currNode, temp->data);
                }
            }

            else if (isEpsilon(temp->data))
            {
                printf("%s has epsilon %s\n", nts[lhs].nonTerminal, temp->data);
                nts[lhs].hasEpsilon = true;
            }
            else
            {
                printf("non terminal %s \n", temp->data);
                int tempenum;
                temp = temp->prev;
                do
                {
                    temp = temp->next;
                    if (temp == NULL)
                    {
                        break;
                    }
                    tempenum = getNT(temp->data);
                    printf("entering %s\n", temp->data);
                    struct Node *rec = findFirst(rules, temp->data);
                    nts[getNT(temp->data)].first = rec;

                    if (rec == NULL)
                    {
                        break;
                    }
                    else
                    {
                        printf("calculated first of %s as %s\n", temp->data, rec->data);
                    }
                    printf("coming out\n");
                    if (currNode == NULL)
                    {
                        printf("initcopy\n");
                        currNode = initcopy(rec, header);
                    }
                    else
                    {
                        currNode = copy(currNode, rec);
                    }

                } while (nts[tempenum].hasEpsilon);
                if (temp == NULL)
                {
                    nts[lhs].hasEpsilon = true;
                }
            }
        }
    }

    nts[getNT(nonTerminal)].first = header;
    if (header == NULL)
        return NULL;
    return header;
}
int main()
{
    printf("Hello world\n");
    initHashTableNT(nonTerminals);
    initHashTableT(Terminals);
    FILE *grammer;
    grammer = fopen("grammer.txt", "r");
    struct Node *rules[142];
    bool epsilon[142];
    for (int j = 0; j < 142; j++)
    {
        char *rule = getRule(grammer);
        rule[strcspn(rule, "\n")] = 0;
        char *currentLexicalElement = strtok(rule, " "); // it is the current lexical element
        struct Node *currNode = (struct Node *)malloc(sizeof(struct Node));
        struct Node *head = currNode;
        while (currentLexicalElement != NULL)
        {
            currNode->data = currentLexicalElement;
            currentLexicalElement = strtok(NULL, " ");
            currNode->next = (struct Node *)malloc(sizeof(struct Node));
            (currNode->next)->prev = currNode;
            currNode = currNode->next;
        }
        rules[j] = head;
    }
    for (int i = 0; i < number_nt; i++)
    {
        nts[i].nonTerminal = nonTerminals[i];
        nts[i].hasEpsilon = false;
        nts[i].completed = false;
    }
    for (int i = 0; i < number_nt; i++)
    {
        nts[i].first = findFirst(rules, nonTerminals[i]);
    }
    // We have computed first and follow

    // Fill the parsing table

    struct Node *parseTable[number_nt][number_t + 1] = {NULL};
    for (int i = 0; i < rows; i++)
    {
        struct Node *lhs = rules[i];
        bool flag = false;
        int templ = getNT(lhs->data);
        struct Node *rhs = rules[i]->next;
        if (isTerminal(rhs->data))
        {
            int tempr = getT(rhs->data);
            parseTable[templ][tempr] = rules[i];
        }
        if (isEpsilon(rhs->data))
        {
            struct Node *follow = nts[templ].follow;
            while (follow != NULL)
            {
                int tempr = getT(follow->data);
                parseTable[templ][tempr] = rules[i];
                follow = follow->next;
            }
        }
        else
        {
            int RHS = getNT(rhs->data);
            struct Node *first = nts[RHS].first;
            if (nts[RHS].hasEpsilon)
            {
                flag = true;
                while (flag && rhs->next != NULL)
                {
                    int t = getNT(rhs->data);
                    flag = nts[t].hasEpsilon;
                    struct Node *first2 = nts[t].first;
                    while (first2->next != NULL)
                    {
                        int tempr = getT(first2->data);
                        parseTable[templ][tempr] = rules[i];
                        first2 = first2->next;
                    }
                    rhs = rhs->next;
                }
                if (rhs->next == NULL && flag)
                {
                    struct Node *follow = nts[templ].follow;
                    while (follow->next != NULL)
                    {
                        int tempr = getT(follow->data);
                        parseTable[templ][tempr] = rules[i];
                    }
                }
            }
            else
            {

                while (first->next != NULL)
                {
                    int tempr = getT(first->data);
                    parseTable[templ][tempr] = rules[i];
                    first = first->next;
                }
            }
        }
    }
}