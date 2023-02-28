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
    char *data;
    struct Node *prev;
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

struct Node *copy(struct Node *n1, struct Node *n2)
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
void initHashTable(char **nonTerminals)
{
    for (int i = 0; i < 71; i++)
    {
        insert(nonTerminals[i], i);
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
struct Node *initcopy(struct Node *rec)
{
    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    struct Node *header = temp;
    temp->data = rec->data;
    while (rec->next != NULL)
    {
        temp->next = (struct Node *)malloc(sizeof(struct Node));
        (temp->next)->data = (rec->next)->data;
        (temp->next)->prev = temp;
        temp = temp->next;
        rec = rec->next;
    }
    return header;
}
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
                    tempenum = get(temp->data);
                    struct Node *rec = findFirst(rules, temp->data);
                    nts[get(temp->data)].first = rec;

                    if (rec == NULL)
                    {
                        break;
                    }
                    if (currNode == NULL)
                    {
                        header = initcopy(rec);
                        currNode = header;
                        while (currNode->next != NULL)
                        {
                            currNode = currNode->next;
                        }
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

    nts[get(nonTerminal)].first = header;
    return header;
}

struct Node* findFollow(struct Node **rules, char *nonTerminal)
{ 
    struct Node* currNode=NULL;
    struct Node* header=NULL;
    for(int i=0;i<rows;i++)
    {
        
        struct Node* temp=rules[i]->next;
        while(temp!=NULL)
        {
            if(strcmp(temp->data,nonTerminal)==0)
            {
                printf("found %s in %d\n",nonTerminal,i);
                fflush(stdout);
                if(temp->next==NULL)
                {
                    printf("Next is NULL ");
                    if(strcmp(rules[i]->data,nonTerminal)==0)
                    {
                        printf("hence we ignor it ");
                    }
                    else
                    {
                        printf("hence we find follow of %s ",rules[i]->data);
                        struct Node* rec=findFollow(rules,rules[i]->data);
                        
                        if(header==NULL)
                        {
                            header=initcopy(rec);
                            currNode=header;
                            while(currNode->next!=NULL)
                            {
                                currNode=currNode->next;
                            }
                        }
                        else
                        {
                            currNode = copy(currNode,rec);
                        }
                    }
                }
                else
                {
                    printf("Next is not NULL ");
                    if(isTerminal(temp->next->data))
                    {
                        printf("Next is terminal");
                        if(header==NULL)
                        {
                            printf("new follow list");
                            header=init(temp->next->data);
                            currNode=header;
                        }
                        else
                        {
                            printf("new follow item");
                            push(currNode,temp->next->data);
                        }
                    }
                    else
                    {
                        printf("Next is non terminal ");
                        struct Node* rec=findFirst(rules,temp->next->data);
                        
                        if(header==NULL)
                        {
                            printf("new follow list ");
                            header=initcopy(rec);
                            currNode=header;
                            while(currNode->next!=NULL)
                            {
                                currNode=currNode->next;
                            }
                        }
                        else
                        {
                            printf("add follow of %s to follow list ",temp->next->data);
                            currNode = copy(currNode,rec);
                        }
                        bool flag = nts[get(temp->next->data)].hasEpsilon;
                        struct Node* aise=temp->next;
                        while(flag)
                        {
                            printf("%s is nullable ",aise->data);
                            aise = aise->next;
                            if(aise!=NULL)
                            printf("entered while with aise = %s",aise->data);
                            if(aise==NULL)
                            {
                                printf("aise is null");
                                struct Node* followlhs = findFollow(rules,rules[i]->data);
                                currNode = copy(currNode,followlhs);
                                break;
                            }
                            else if(isTerminal(aise->data))
                            {
                                push(currNode,aise->data);
                                currNode = currNode->next;
                                break;
                            }
                            else
                            {
                                struct Node* rec=findFirst(rules,aise->data);
                                
                                currNode = copy(currNode,rec);
                            }
                            flag = nts[get(aise->data)].hasEpsilon; 
                            if(!flag)
                            {
                                printf("not nullable %s",aise->data);
                            }
                        }
                    }
                }
                printf("\n");
            }
            temp=temp->next;
        }
    }
    return nts[get(nonTerminal)].follow=header;
}


int main()

{
    initHashTable(nonTerminals);
    FILE *grammer;
    grammer = fopen("grammer.txt", "r");
    struct Node *rules[rows];
    bool epsilon[rows];
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
            currNode->next = (struct Node *)malloc(sizeof(struct Node));
            (currNode->next)->prev = currNode;
            currNode = currNode->next;
        }
        free(currNode);
        rules[j] = head;
    }
    char *s = ((rules[52]->next)->next)->data;
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
    
    nts[0].follow = init("$");
    nts[1].follow = findFollow(rules, nonTerminals[1]);
    // for (int i = 1; i < 71; i++)
    // {
    //     nts[i].follow = findFollow(rules, nonTerminals[i]);
    // }
    // for(int i=0;i<71;i++)
    // {
    //     struct Node* temp=nts[i].follow;
    //     printf("follow of %s is \n",nts[i].nonTerminal);
    //     fflush(stdout);
    //     while(temp!=NULL)
    //     {
    //         printf("%s \n",temp->data);
    //         fflush(stdout);
    //         temp=temp->next;
    //     }   
    //     printf("\n");
    //     fflush(stdout);
    // }
}