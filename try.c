#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define rows 141
#define TABLE_SIZE 10000
#define MAX_PROBE 100

typedef struct map
{
    char *key;
    int value;
} map;

map *hashtable[TABLE_SIZE];

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
void insert(char *key, int value)
{
    int i = 0;
    int index = hash(key, i);
    while (hashtable[index] != NULL)
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
    hashtable[index] = new;
}

// get the value associated with the given key from the hash table
int get(char *key)
{
    int i = 0;
    int index = hash(key, i);
    while (hashtable[index] != NULL)
    {
        if (strcmp(hashtable[index]->key, key) == 0)
        {
            return hashtable[index]->value;
        }
        i++;
        if (i > MAX_PROBE)
        {
            printf("Error: Maximum probing limit reached\n");
            return 0;
        }
        index = hash(key, i);
    }
    return -1;
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
    struct Node *prev;
    char *data;
};

struct nonTerminalStruct nts[71]; // array of stucts

char *nonTerminals[] = {"program", "moduleDeclarations", "moduleDeclaration", "otherModules", "driverModule", "module", "ret", "input_plist", "n1", "output_plist", "n2", "dataType", "range_arrays", "type", "moduleDef", "statements", "statement", "ioStmt", "boolConst", "id_num_rnum", "var_print", "p1", "simpleStmt", "assignmentStmt", "whichStmt", "lvalueIDStmt", "lvalueARRStmt", "index_arr", "new_index", "sign", "moduleReuseStmt", "optional", "idList", "n3", "actual_para_list", "expression", "u", "new_NT", "var_id_num", "unary_op", "arithmeticOrBooleanExpr", "n7", "anyTerm", "n8", "arithmeticExpr", "n4", "term", "n5", "factor", "n_11", "element_index_with_expressions", "n10", "arrExpr", "arr_n4", "arrTerm", "arr_n5", "arrFactor", "op1", "logicalOp", "relationalOp", "declareStmt", "conditionalStmt", "caseStmts", "n9", "value", "default", "iterativeStmt", "range_for_loop", "index_for_loop", "new_index_for_loop", "sign_for_loop"};

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

// appends the second arg to the end of first and return the tail pointer
struct Node *appendLinkedList(struct Node *n1, struct Node *n2)
{

    while (n2 != NULL)
    {
        struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
        temp->data = n2->data;
        n1->next = temp;
        n1 = n1->next;
        n2 = n2->next;
    }
    return n1;
}

// inserts all the non terminals in the hash table
void insertNonTerminals(char **nonTerminals)
{
    for (int i = 0; i < 71; i++)
    {
        insert(nonTerminals[i], i);
    }
}

// creates a new string node
struct Node *createNode(char *s)
{
    struct Node *curr;
    curr = (struct Node *)malloc(sizeof(struct Node));
    curr->data = s;
    return curr;
}

// pushes a node to the list
void push(struct Node *curr, char *s)
{
    curr->next = (struct Node *)malloc(sizeof(struct Node));
    (curr->next)->data = s;
    (curr->next)->prev = curr;
}

// find the first of the given non terminal
struct Node *findFirst(struct Node **rules, char *nonTerminal)
{

    if (nts[get(nonTerminal)].first != NULL)
    {

        return nts[get(nonTerminal)].first;
    }

    struct Node *currNode = NULL; // first
    struct Node *header = NULL;
    for (int i = 0; i < rows; i++)
    {
        if (strcmp(rules[i]->data, nonTerminal) == 0)
        {
            struct Node *temp = rules[i]->next; // first node of rhs
            int lhs = get(rules[i]->data);
            if (isTerminal(temp->data))
            {

                if (currNode == NULL)
                {
                    header = createNode(temp->data);
                    currNode = header;
                }
                else
                {
                    push(currNode, temp->data);
                    currNode = currNode->next;
                }
            }

            else if (isEpsilon(temp->data))
            {
                nts[lhs].hasEpsilon = true;
            }
            else
            {
                int tempenum;
                temp = temp->prev;
                do
                {

                    temp = temp->next;
                    if (temp == NULL)
                    {
                        break;
                    }
                    if (isTerminal(temp->data))
                    {
                        push(currNode, temp->data);
                        currNode = currNode->next;
                        break;
                    }
                    tempenum = get(temp->data);
                    struct Node *rec = findFirst(rules, temp->data);
                    nts[get(temp->data)].first = rec;

                    if (rec == NULL)
                    {
                        break;
                    }
                    if (currNode == NULL)
                    {
                        header = (struct Node *)malloc(sizeof(struct Node));
                        header->data = rec->data;
                        currNode = appendLinkedList(header, rec->next);

                        while (currNode->next != NULL)
                        {
                            currNode = currNode->next;
                        }
                    }
                    else
                    {
                        currNode = appendLinkedList(currNode, rec);
                    }

                } while (nts[tempenum].hasEpsilon);
                if (temp == NULL)
                {
                    nts[lhs].hasEpsilon = true;
                }
            }
        }
    }

    nts[get(nonTerminal)].first = header;
    if (header == NULL)
        return NULL;
    return header;
}

struct Node *findFollow(struct Node **rules, char *nonTerminal)
{
    printf("seardhing for %s--->\n", nonTerminal);
    if (nts[get(nonTerminal)].completed)
    {
        return nts[get(nonTerminal)].follow;
    }

    struct Node *currNode = NULL; // first
    struct Node *header = NULL;   // to be returned

    for (int i = 0; i < rows; i++)
    {

        struct Node *rhs = rules[i]->next; // iterator in the rhs of rules i
        struct Node *firstRhs = rhs;       // this always points to the first string of rhs
        struct Node *lhs = rules[i];       // points to the lhs node of the rule

        while (rhs != NULL)
        {
            // rules[i] points to the lhs node of the rules
            // rhs
            printf("for row = %d %s\n", i, rhs->data);

            if (strcmp(rhs->data, nonTerminal) == 0)
            {

                printf("here\n");

                if (isTerminal(rhs->next->data))
                {
                    struct Node *temp = createNode(rhs->data);

                    if (currNode == NULL)
                    {
                        currNode = temp;
                        header = currNode;
                    }
                    else
                    {
                        currNode = appendLinkedList(currNode, temp);
                    }
                }
                else if (rhs->next == NULL)
                {
                    if (rhs->data != lhs->data)
                    {

                        struct Node *temp = findFollow(rules, rules[i]->data);

                        if (currNode == NULL)
                        {
                            currNode = temp;
                            header = currNode;
                        }
                        else
                        {
                            currNode = appendLinkedList(currNode, temp);
                        }
                    }
                }
                else
                {
                    if (nts[get(rhs->next->data)].hasEpsilon)
                    {
                        struct Node *currNT = rhs->next;

                        while (currNT != NULL && !isTerminal(currNT->data) && nts[get(currNT->data)].hasEpsilon)
                        {
                            struct Node *temp = nts[get(currNT->data)].first;

                            if (currNode == NULL)
                            {
                                currNode = temp;
                                header = currNode;
                            }
                            else
                            {
                                currNode = appendLinkedList(currNode, temp);
                            }

                            currNT = currNT->next;
                        }

                        if (currNT == NULL)
                        {
                            if (lhs->data != nonTerminal)
                            {
                                struct Node *temp = findFollow(rules, lhs->data);

                                if (currNode == NULL)
                                {
                                    currNode = temp;
                                    header = currNode;
                                }
                                else
                                {
                                    currNode = appendLinkedList(currNode, temp);
                                }
                            }
                        }
                        else if (isTerminal(currNT->data))
                        {
                            struct Node *temp = createNode(rhs->data);

                            if (currNode == NULL)
                            {
                                currNode = temp;
                                header = currNode;
                            }
                            else
                            {
                                currNode = appendLinkedList(currNode, temp);
                            }
                        }
                        else
                        {
                            struct Node *temp = nts[get(currNT->data)].first;

                            if (currNode == NULL)
                            {
                                currNode = temp;
                                header = currNode;
                            }
                            else
                            {
                                currNode = appendLinkedList(currNode, temp);
                            }
                        }
                    }
                    else
                    {
                        struct Node *temp = nts[get(rhs->next->data)].first;

                        if (currNode == NULL)
                        {
                            currNode = temp;
                            header = currNode;
                        }
                        else
                        {
                            currNode = appendLinkedList(currNode, temp);
                        }
                    }
                }
            }

            rhs = rhs->next;
        }
    }

    nts[get(nonTerminal)].completed = true;
    nts[get(nonTerminal)].follow = header;

    return header;
}

// Remove duplicate elements
void removeDuplicateElements(struct Node *start)
{
    struct Node *ptr1, *ptr2, *dup;
    ptr1 = start;
    while (ptr1 != NULL && ptr1->next != NULL)
    {
        ptr2 = ptr1;
        while (ptr2->next != NULL)
        {
            if (ptr1->data == ptr2->next->data)
            {
                dup = ptr2->next;
                ptr2->next = ptr2->next->next;
                free(dup);
            }
            else
                ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
}
// init function of the parser
void initParser()
{
    insertNonTerminals(nonTerminals);

    FILE *grammer;

    grammer = fopen("grammar.txt", "r");

    struct Node *rules[rows];

    for (int j = 0; j < rows; j++)
    {
        char *rule = getRule(grammer);

        rule[strcspn(rule, "\n")] = 0;
        rule[strcspn(rule, "\r")] = 0;

        char *currentLexicalElement = strtok(rule, " "); // it is the current lexical element
        struct Node *currNode = (struct Node *)malloc(sizeof(struct Node));
        struct Node *head = currNode;
        while (currentLexicalElement != NULL)
        {
            currNode->data = currentLexicalElement;
            currentLexicalElement = strtok(NULL, " ");

            if (currentLexicalElement == NULL)
            {
                currNode->next = NULL;
                break;
            }

            currNode->next = (struct Node *)malloc(sizeof(struct Node));
            (currNode->next)->prev = currNode;
            currNode = currNode->next;
        }
        rules[j] = head;
        // free(currNode);
    }
    // enum nonTerminals nonTerminal;
    for (int i = 0; i < 71; i++)
    {
        nts[i].nonTerminal = nonTerminals[i];
        nts[i].hasEpsilon = false;
        nts[i].completed = false;
    }
    for (int i = 0; i < 71; i++)
    {
        nts[i].first = findFirst(rules, nonTerminals[i]);
    }

    nts[0].follow = createNode("$");
    for (int i = 1; i < 71; i++)
    {
        nts[i].follow = findFollow(rules, nonTerminals[i]);
    }

    // for (int i = 0; i < rows; i++)
    // {
    //     struct Node *temp = rules[i];

    //     while (temp != NULL)
    //     {
    //         printf("%s ", temp->data);
    //         temp = temp->next;
    //     }

    //     printf("\n");
    // }
}

int main()
{
    initParser();

    for (int i = 0; i < 71; i++)
    {
        struct Node *temp = nts[get(nonTerminals[i])].follow;

        printf("first of %s n %d is ", nonTerminals[i], i);

        while (temp != NULL)
        {
            printf("%s ", temp->data);
            temp = temp->next;
        }
        if (nts[i].hasEpsilon)
        {
            printf(" hehe ");
        }
        printf("\n");
    }

    // for (int i = 0; i < rows; i++)
    // {
    //     struct Node *temp = rules[i];

    //     while (temp != NULL)
    //     {
    //         printf("%s", temp->data);
    //         temp = temp->next;
    //     }
    // }
}