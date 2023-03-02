#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexerDef.h"
#include "lexer.h"
#include "parserDef.h"
#include "parser.h"
#define rows 142
#define TABLE_SIZE 10000
#define MAX_PROBE 100

map *hashTableNonTerminals[TABLE_SIZE];
map *hashTableTerminals[TABLE_SIZE];

// Stack Functions

treeNode *createTreeNode(char *s)
{
    treeNode *node = (treeNode *)malloc(sizeof(treeNode));
    node->child = NULL;
    node->left = NULL;
    node->parent = NULL;
    node->right = NULL;
    if (isTerminal(s) || isEpsilon(s))
    {
        node->isComputed = true;
    }
    else
    {
        node->isComputed = false;
    }
    node->data = s;
    return node;
}
void printTree(treeNode *Tree)
{
    if (Tree->child != NULL)
    {
        printTree(Tree->child);
    }
    printf(" %s ----> ", Tree->data);
    if (Tree->child != NULL)
    {
        treeNode *right_siblings = Tree->child->right;
        while (right_siblings != NULL)
        {
            printTree(right_siblings);
            right_siblings = right_siblings->right;
        }
    }
}
char *peek(struct Node *top)
{
    struct Node *n = top;
    return n->data;
}
int isEmpty(struct Node *top)
{
    if (top == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int isFull(struct Node *top)
{
    struct Node *p = (struct Node *)malloc(sizeof(struct Node));
    if (p == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

struct Node *stackPush(struct Node *top, char *x)
{
    if (isFull(top))
    {
        printf("Stack Overflow\n");
    }
    else
    {
        struct Node *n = (struct Node *)malloc(sizeof(struct Node));
        n->data = x;
        n->next = top;
        top = n;
        return top;
    }
}
char *pop(struct Node **top)
{
    if (isEmpty(*top))
    {
        printf("Stack Underflow\n");
    }
    else
    {
        struct Node *n = *top;
        *top = (*top)->next;
        char *x;
        strcpy(x, n->data);
        free(n);
        return x;
    }
}

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

            printf("Error: 1Maximum probing limit reached\n");
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

// insert a key-value pair into the hash table

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

struct nonTerminalStruct nts[number_nt]; // array of stucts

char *nonTerminals[] = {"program", "moduleDeclarations", "moduleDeclaration", "otherModules", "driverModule", "module", "ret", "input_plist", "n1", "output_plist", "n2", "dataType", "range_arrays", "type", "moduleDef", "statements", "statement", "ioStmt", "boolConst", "var_print", "p1", "simpleStmt", "assignmentStmt", "whichStmt", "lvalueIDStmt", "lvalueARRStmt", "index_arr", "new_index", "sign", "moduleReuseStmt", "optional", "idList", "n3", "actual_para_list", "expression", "uop", "new_NT", "var_id_num", "unary_op", "arithmeticOrBooleanExpr", "n7", "anyTerm", "n8", "arithmeticExpr", "n4", "term", "n5", "factor", "n_11", "element_index_with_expressions", "n_10", "arrExpr", "arr_n4", "arrTerm", "arr_n5", "arrFactor", "op1", "logicalOp", "relationalOp", "declareStmt", "conditionalStmt", "caseStmts", "n9", "value", "default", "iterativeStmt", "range_for_loop", "index_for_loop", "new_index_for_loop", "sign_for_loop", "n_12", "n_13", "list_var", "op2"};
char *Terminals[] = {"ID", "TRUE", "FALSE", "COMMENT", "AND", "OR", "INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", "USE", "WITH", "PARAMETERS", "TAKES", "INPUT", "RETURNS", "FOR", "IN", "SWITCH", "CASE", "BREAK", "DEFAULT", "WHILE", "NUM", "RNUM", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF", "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "LEXERROR", "TK_EOF"};

char *getRule(FILE *grammar)
{
    char *buff;
    buff = (char *)malloc(200 * sizeof(char));
    fgets(buff, 200, grammar);
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
void printLL(struct Node *header)
{
    int cnt = 0;

    while (header != NULL)
    {
        cnt++;
        // printf("%s ", header->data);
        header = header->next;
    }

    // printf("  count = %d\n", cnt);
}
// appends the second arg to the end of first and return the tail pointer
struct Node *appendLinkedList(struct Node *n1, struct Node *n2)
{
    while (n2 != NULL)
    {
        struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
        temp->next = NULL;
        temp->prev = NULL;
        // printf("%s ", n2->data);
        temp->data = n2->data;
        temp->prev = n1;
        n1->next = temp;
        n1 = n1->next;
        n2 = n2->next;
    }

    return n1;
}

// inserts all the non terminals in the hash table
void insertNonTerminals(char **nonTerminals)
{
    for (int i = 0; i < number_nt; i++)
    {
        insertNT(nonTerminals[i], i);
    }
}

void insertTerminals(char **Terminals)
{
    for (int i = 0; i < number_t; i++)
    {
        insertT(Terminals[i], i);
    }
}

// creates a new string node
struct Node *createNode(char *s)
{
    struct Node *curr;
    curr = (struct Node *)malloc(sizeof(struct Node));
    curr->data = s;
    curr->next = NULL;
    curr->prev = NULL;
    return curr;
}

// pushes a node to the list
void push(struct Node *curr, char *s)
{
    curr->next = (struct Node *)malloc(sizeof(struct Node));
    (curr->next)->data = s;
    (curr->next)->prev = curr;
    (curr->next)->next = NULL;
}

// find the first of the given non terminal
struct Node *findFirst(struct Node **rules, char *nonTerminal)
{

    if (nts[getNT(nonTerminal)].first != NULL)
    {

        return nts[getNT(nonTerminal)].first;
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
                    tempenum = getNT(temp->data);
                    struct Node *rec = findFirst(rules, temp->data);
                    nts[getNT(temp->data)].first = rec;

                    if (rec == NULL)
                    {
                        break;
                    }
                    if (currNode == NULL)
                    {
                        header = (struct Node *)malloc(sizeof(struct Node));
                        header->next = NULL;
                        header->prev = NULL;
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

    nts[getNT(nonTerminal)].first = header;
    if (header == NULL)
        return NULL;
    return header;
}

// needs to be changes // might be plagiarised
struct Node *removeDuplicatesFromLL(struct Node *head)
{
    struct Node *current = head;
    struct Node *temp = NULL;

    while (current != NULL)
    {
        temp = current->next;

        while (temp != NULL)
        {
            if (strcmp(current->data, temp->data) == 0)
            {
                temp->prev->next = temp->next;
                if (temp->next != NULL)
                {
                    temp->next->prev = temp->prev;
                }
                free(temp);
                temp = current->next;
            }
            else
            {
                temp = temp->next;
            }
        }

        current = current->next;
    }

    return head;
}

struct Node *cloneLinkedList(struct Node *list)
{
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    temp->data = list->data;
    temp->prev = NULL;
    temp->next = NULL;
    struct Node *head = temp;

    if (list->next != NULL)
        appendLinkedList(temp, list->next);

    return head;
}

struct Node *findFollow(struct Node **rules, char *nonTerminal)
{
    // printf("seardhing for %s--->\n", nonTerminal);
    if (nts[getNT(nonTerminal)].completed)
    {
        return nts[getNT(nonTerminal)].follow;
    }

    struct Node *currNode = NULL; // first
    struct Node *header = NULL;   // to be returned

    for (int i = 0; i < rows; i++)
    {

        struct Node *rhs = rules[i]->next; // iterator in the rhs of rules i
        struct Node *firstRhs = rhs;       // this always points to the first string of rhs
        struct Node *lhs = rules[i];       // points to the lhs node of the rule
        // printf("searching for %s\n",nonTerminal);
        while (rhs != NULL)
        {
            // rules[i] points to the lhs node of the rules
            // rhs
            // printf("for row = %d comparing this-->%s to nt\n", i, rhs->data);

            if (strcmp(rhs->data, nonTerminal) == 0)
            {
                // printf("found match\n");
                if (rhs->next == NULL)
                {
                    // printf("right is null\n");
                    fflush(stdout);

                    if (strcmp(lhs->data, rhs->data))
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
                else if (isTerminal(rhs->next->data))
                {
                    // printf("rihgt is terminal ---> %s\n", rhs->next->data);
                    struct Node *temp = createNode(rhs->next->data);

                    if (currNode == NULL)
                    {
                        currNode = temp;
                        header = currNode;
                    }
                    else
                    {
                        currNode = appendLinkedList(currNode, temp);
                    }
                    // printf("%s", currNode->data);
                }
                else
                {
                    // printf("found a nt\n");
                    if (nts[getNT(rhs->next->data)].hasEpsilon)
                    {
                        // printf("has an eps\n");
                        struct Node *currNT = rhs->next;

                        while (currNT != NULL && !isTerminal(currNT->data) && nts[getNT(currNT->data)].hasEpsilon)
                        {

                            // struct Node *temp = nts[getNT(currNT->data)].first;

                            struct Node *temp = cloneLinkedList(nts[getNT(currNT->data)].first);

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
                            struct Node *temp = createNode(currNT->data);

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
                            struct Node *temp = cloneLinkedList(nts[getNT(currNT->data)].first);

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
                        struct Node *temp = cloneLinkedList(nts[getNT(rhs->next->data)].first);

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

    nts[getNT(nonTerminal)].completed = true;
    nts[getNT(nonTerminal)].follow = header;

    return header;
}
// init function of the parser

void makeLLOfGrammar(FILE *grammar, struct Node **rules)
{
    for (int j = 0; j < rows; j++)
    {
        char *rule = getRule(grammar);

        rule[strcspn(rule, "\n")] = 0;
        rule[strcspn(rule, "\r")] = 0;

        char *currentLexicalElement = strtok(rule, " "); // it is the current lexical element
        struct Node *currNode = (struct Node *)malloc(sizeof(struct Node));
        currNode->next = NULL;
        currNode->prev = NULL;
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
            currNode->next->next = NULL;
            (currNode->next)->prev = currNode;
            currNode = currNode->next;
        }
        rules[j] = head;
        // free(currNode);
    }
}

void calculateFollowSet(struct Node **rules)
{
    for (int i = 1; i < number_nt; i++)
    {
        nts[i].follow = findFollow(rules, nonTerminals[i]);

        nts[i].follow = removeDuplicatesFromLL(nts[i].follow);
        // struct Node *temp = nts[i].follow;
        // printf("follow of %d hehe %s ------->", i, nonTerminals[i]);
        // while (temp != NULL)
        // {
        //     // printf("%s,", temp->data);
        //     temp = temp->next;
        // }
        // printf("\n");
    }
}

void calculateFirstSet(struct Node **rules)
{
    for (int i = 0; i < number_nt; i++)
    {
        nts[i].first = findFirst(rules, nonTerminals[i]);
        nts[i].first = removeDuplicatesFromLL(nts[i].first);

        struct Node *temp = nts[i].first;
        printf("first of %d %d %s ------->", i, nts[i].hasEpsilon, nonTerminals[i]);
        while (temp != NULL)
        {
            printf("%s,", temp->data);
            temp = temp->next;
        }

        printf("\n");
    }
}
void printParseTable(struct Node *parseTable[number_nt][number_t])
{
    FILE *fp = fopen("output.csv", "w");

    for (int i = 0; i < number_t; i++)
    {
        fprintf(fp, ",%s", Terminals[i]);
    }

    fprintf(fp, "\n");

    for (int row = 0; row < number_nt; row++)
    {
        fprintf(fp, "%s", nonTerminals[row]);

        for (int col = 0; col < number_t; col++)
        {
            fprintf(fp, ",");

            struct Node *curr = parseTable[row][col];
            while (curr != NULL)
            {
                fprintf(fp, "%s ", curr->data);
                curr = curr->next;
            }
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}
void fillParserTable(struct Node *parseTable[number_nt][number_t], struct Node **rules)
{
    for (int i = 0; i < rows; i++)
    {
        struct Node *lhs = rules[i];
        int templ = getNT(lhs->data);
        struct Node *rhs = rules[i]->next;
        // printf("%s\n", rhs->data);
        if (isTerminal(rhs->data))
        {
            int tempr = getT(rhs->data);
            if (parseTable[templ][tempr] != NULL)
            {
                printf("Grammar is not LL(1) %s %s", nonTerminals[templ], Terminals[tempr]);
                exit(0);
            }
            parseTable[templ][tempr] = rules[i];
        }
        else if (isEpsilon(rhs->data))
        {
            struct Node *follow = nts[templ].follow;
            while (follow != NULL)
            {
                int tempr = getT(follow->data);
                if (parseTable[templ][tempr] != NULL)
                {
                    printf("Grammar is not LL(1) %s %s", nonTerminals[templ], Terminals[tempr]);
                    exit(0);
                }
                parseTable[templ][tempr] = rules[i];
                follow = follow->next;
            }
        }
        else
        {
            int RHS = getNT(rhs->data);
            struct Node *first = nts[RHS].first;

            while (!isTerminal(rhs->data) && nts[RHS].hasEpsilon)
            {
                struct Node *temp = nts[RHS].first;

                while (temp != NULL)
                {
                    if (parseTable[templ][getT(temp->data)] != NULL)
                    {
                        printf("Grammar is not LL(1) %s %s", nonTerminals[templ], temp->data);
                        exit(0);
                    }
                    parseTable[templ][getT(temp->data)] = rules[i];
                    temp = temp->next;
                }

                rhs = rhs->next;

                if (rhs == NULL)
                    break;

                RHS = getNT(rhs->data);
            }

            if (rhs == NULL)
            {
                struct Node *temp = nts[templ].follow;

                while (temp != NULL)
                {
                    if (parseTable[templ][getT(temp->data)] != NULL)
                    {
                        printf("Grammar is not LL(1) %s %s", nonTerminals[templ], temp->data);
                        exit(0);
                    }
                    parseTable[templ][getT(temp->data)] = rules[i];
                    temp = temp->next;
                }
            }
            else if (!isTerminal(rhs->data))
            {
                struct Node *temp = nts[getNT(rhs->data)].first;
                while (temp != NULL)
                {
                    if (strcmp(lhs->data, "arithmeticExpr") == 0)
                    {
                        printf("Helllooo %s\n\n\n\n\n\n", temp->data);
                    }
                    if (parseTable[templ][getT(temp->data)] != NULL)
                    {
                        printf("Grammar is not LL(1) %s %s", nonTerminals[templ], temp->data);
                        exit(0);
                    }
                    parseTable[templ][getT(temp->data)] = rules[i];
                    temp = temp->next;
                }
            }
            else
            {
                if (parseTable[templ][getT(rhs->data)] != NULL)
                {
                    printf("Grammar is not LL(1) %s %s", nonTerminals[templ], rhs->data);
                    exit(0);
                }
                parseTable[templ][getT(rhs->data)] = rules[i];
            }
        }
    }
}

void parser(struct Node *parseTable[number_nt][number_t], struct Node **rules, int buffSize)
{
    FILE *fp = fopen("sc.txt", "r");
    if (fp == NULL)
    {
        printf("File Not Found\n");
        return;
    }
    struct Node *stack = NULL;
    stack = stackPush(stack, "TK_EOF");
    stack = stackPush(stack, "program");
    Buffer *buff = getStream(fp, buffSize);
    tokenInfo *input = getNextToken(buff);

    treeNode *tree = createTreeNode("program");
    treeNode *currNode = tree;

    while (isEmpty(stack) == 0)
    {
        if (isTerminal(peek(stack)))
        {
            printf("found a terminal on stack top--> %s\n", peek(stack));
            if (!strcmp(Terminals[input->id], peek(stack)))
            {
                printf("found a match --> %s\n", peek(stack));
                pop(&stack);
                while(currNode->right==NULL&&currNode->parent!=NULL)
                {
                    currNode=currNode->parent;
                    
                }
                if(currNode->right!=NULL)
                currNode=currNode->right;
                input = getNextToken(buff);
                continue;
            }
            else
            {
                printf("Error! Terminals mismatch %d %s %s\n", input->lineNumber, peek(stack), Terminals[input->id]);
                exit(0);
            }
        }
        else if (!isEpsilon(peek(stack)))
        {
            printf("found a non terminal on stack top--> %s\n", peek(stack));
            printf("Input is %s\n", Terminals[input->id]);
            struct Node *Rule = parseTable[getNT(peek(stack))][input->id];
            if (Rule == NULL)
            {
                printf("Rule not Found %d %s %s\n", input->lineNumber, peek(stack), Terminals[input->id]);
                exit(0);
            }
            else
            {
                printf("%s\n", peek(stack));
                pop(&stack);
                printf("%s\n", peek(stack));
                Rule = Rule->next;
                if (isEpsilon(Rule->data))
                {
                    treeNode* node = createTreeNode(Rule->data);
                    currNode->child=node;
                    node->parent = currNode;
                    while(currNode->right==NULL&&currNode->parent!=NULL)
                    {
                        currNode=currNode->parent;
                    }
                    if(currNode->right!=NULL)
                    currNode=currNode->right;
                    continue;
                }
                struct Node *tempStack = NULL;

                treeNode *parentNode = currNode;

                int child = 0;

                while (Rule != NULL)
                {

                    tempStack = stackPush(tempStack, Rule->data);

                    if (child == 0)
                    {
                        treeNode *node = createTreeNode(Rule->data);
                        currNode->child = node;
                        currNode = currNode->child;
                        currNode->parent = parentNode;
                    }
                    else
                    {
                        treeNode *node = createTreeNode(Rule->data);
                        currNode->right = node;
                        node->left = currNode;
                        currNode = currNode->right;
                        currNode->parent = parentNode;
                    }
                    child++;

                    Rule = Rule->next;
                }
                currNode = currNode->parent->child;
                // printf("%s\n", peek(tempStack));
                while (isEmpty(tempStack) == 0)
                {
                    // char *temp = peek(tempStack);
                    stack = stackPush(stack, peek(tempStack));
                    pop(&tempStack);
                }
            }
            
        }
    

       
    }

    printf("Input source code is syntactically correct..........\n\n");
    
    printTree(tree);
}
void initParser(int buffSize)
{
    insertNonTerminals(nonTerminals);
    insertTerminals(Terminals);
    FILE *grammar;

    grammar = fopen("grammar.txt", "r");

    struct Node *rules[rows];

    makeLLOfGrammar(grammar, rules);

    // filling nts
    for (int i = 0; i < number_nt; i++)
    {
        nts[i].nonTerminal = nonTerminals[i];
        nts[i].hasEpsilon = false;
        nts[i].completed = false;
    }
    calculateFirstSet(rules);
    nts[0].follow = createNode("TK_EOF");
    nts[0].completed = true;
    calculateFollowSet(rules);

    for (int i = 0; i < number_nt; i++)
    {
        struct Node *temp = nts[i].first;
        printf("first of %d %d %s ------->", i, nts[i].hasEpsilon, nonTerminals[i]);
        while (temp != NULL)
        {
            printf("%s,", temp->data);
            temp = temp->next;
        }

        printf("\n");
    }

    for (int i = 0; i < number_nt; i++)
    {
        struct Node *temp = nts[i].follow;
        printf("follow of %d %d %s ------->", i, nts[i].hasEpsilon, nonTerminals[i]);
        while (temp != NULL)
        {
            printf("%s,", temp->data);
            temp = temp->next;
        }

        printf("\n");
    }

    struct Node *parseTable[number_nt][number_t] = {NULL};
    // Filling Parse Table
    fillParserTable(parseTable, rules);
    printParseTable(parseTable);
    parser(parseTable, rules, buffSize);
}

int main()
{
    initParser(128);

}
//  bool allComputed = true;
//         while (currNode->left != NULL)
//         {
//             if (currNode->isComputed == false)
//             {
//                 allComputed = false;
//             }
//             currNode = currNode->left;
//         }
//         if (currNode->isComputed == false)
//         {
//             allComputed = false;
//         }
//         if (!allComputed)
//         {
//             while (currNode->isComputed == true)
//             {
//                 currNode = currNode->right;
//             }
//         }
//         while (allComputed)
//         {
//             if (currNode->parent == NULL)
//             {
//                 break;
//             }
//             currNode = currNode->parent;
//             currNode->isComputed = true;
//             while (currNode->right != NULL && currNode->isComputed)
//             {
//                 currNode = currNode->right;
//             }
//             if (currNode->right == NULL && currNode->isComputed)
//             {
//                 continue;
//             }
//             else
//             {
//                 break;
//             }
        // }