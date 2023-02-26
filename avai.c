#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define rows 142
#define columns 200
#define number_nt 71
#define number_t 58
enum nonterminals
{
    program,
    moduleDeclarations,
    moduleDeclaration,
    otherModules,
    driverModule,
    module,
    ret,
    input_plist,
    n1,
    output_plist,
    n2,
    dataType,
    range_arrays,
    type,
    moduleDef,
    statements,
    statement,
    ioStmt,
    boolConst,
    id_num_rnum,
    var_print,
    p1,
    simpleStmt,
    assignmentStmt,
    whichStmt,
    lvalueIDStmt,
    lvalueARRStmt,
    index_arr,
    new_index,
    sign,
    moduleReuseStmt,
    optional,
    idList,
    n3,
    actual_para_list,
    expression,
    u,
    new_NT,
    var_id_num,
    unary_op,
    arithmeticOrBooleanExpr,
    n7,
    anyTerm,
    n8,
    arithmeticExpr,
    n4,
    term,
    n5,
    factor,
    n_11,
    element_index_with_expressions,
    n_10,
    arrExpr,
    arr_n4,
    arrTerm,
    arr_n5,
    arrFactor,
    op1,
    logicalOp,
    relationalOp,
    declareStmt,
    conditionalStmt,
    caseStmts,
    n9,
    value,
    defaultP,
    iterativeStmt,
    range_for_loop,
    index_for_loop,
    new_index_for_loop,
    sign_for_loop
};
typedef enum terminals
{
    // not terminals removed not required for now

    ID,
    TRUE,
    FALSE,
    COMMENT,
    AND,
    OR,
    INTEGER,
    REAL,
    BOOLEAN,
    OF,
    ARRAY,
    START,
    END,
    DECLARE,
    MODULE,
    DRIVER,
    PROGRAM,
    GET_VALUE,
    PRINT,
    USE,
    WITH,
    PARAMETERS,
    TAKES,
    INPUT,
    RETURNS,
    FOR,
    IN,
    SWITCH,
    CASE,
    BREAK,
    DEFAULT,
    WHILE,
    NUM,
    RNUM,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LT,
    LE,
    GE,
    GT,
    EQ,
    NE,
    DEF,
    ENDDEF,
    DRIVERDEF,
    DRIVERENDDEF,
    COLON,
    RANGEOP,
    SEMICOL,
    COMMA,
    ASSIGNOP,
    SQBO,
    SQBC,
    BO,
    BC,
    LEXERROR,
    TK_EOF,
};

char *nonTerminals[] = {"program", "moduleDeclarations", "moduleDeclaration", "otherModules", "driverModule", "module", "ret", "input_plist", "n1", "output_plist", "n2", "dataType", "range_arrays", "type", "moduleDef", "statements", "statement", "ioStmt", "boolConst", "id_num_rnum", "var_print", "p1", "simpleStmt", "assignmentStmt", "whichStmt", "lvalueIDStmt", "lvalueARRStmt", "index_arr", "new_index", "sign", "moduleReuseStmt", "optional", "idList", "n3", "actual_para_list", "expression", "u", "new_NT", "var_id_num", "unary_op", "arithmeticOrBooleanExpr", "n7", "anyTerm", "n8", "arithmeticExpr", "n4", "term", "n5", "factor", "n_11", "element_index_with_expressions", "n_10 ", "arrExpr", "arr_n4", "arrTerm", "arr_n5", "arrFactor", "op1", "logicalOp", "relationalOp", "declareStmt", "conditionalStmt", "caseStmts", "n9", "value", "default", "iterativeStmt", "range_for_loop", "index_for_loop", "new_index_for_loop", "sign_for_loop"};
struct nonTerminalStruct
{
    char *nonTerminal;
    bool hasEpsilon;
    struct Node *first;
    struct Node *follow;
};
struct Node
{
    struct Node *next;
    char *data;
    struct Node *prev;
};

struct nonTerminalStruct nts[number_nt];

char *getRule(FILE *grammer)
{
    char *buff;
    buff = (char *)malloc(200 * sizeof(char));
    fgets(buff, 200, grammer);
    return buff;
}

int main()
{
    FILE *grammer;
    grammer = fopen("grammer.txt", "r");
    struct Node *rules[rows];
    bool epsilon[rows];
    for (int j = 0; j < rows; j++)
    {
        char *rule = getRule(grammer);
        char *currentLexicalElement = strtok(rule, " "); // it is the current lexical element
        struct Node *currNode = (struct Node *)malloc(sizeof(struct Node));
        struct Node *head = currNode;
        while (currentLexicalElement != NULL)
        {
            if (currentLexicalElement == "e")
            {
                // epsilon[j]=
            }
            currNode->data = currentLexicalElement;
            currentLexicalElement = strtok(NULL, " ");
            currNode->next = (struct Node *)malloc(sizeof(struct Node));
            currNode->next->prev = currNode;
            currNode = currNode->next;
        }
        rules[j] = head;
    }
    // enum nonTerminals nonTerminal;
    // Computing first follow
    for (int i = 0; i < 71; i++)
    {
        nts[i].nonTerminal = nonTerminals[i];
        nts[i].first = findFirst(rules, nonTerminals[i]);
        nts[i].follow = findFollow(rules, nonTerminals[i]);
    }
    // We have computed first and follow

    // Fill the parsing table

    struct Node *parseTable[number_nt][number_t + 1] = {NULL};
    for (int i = 0; i < rows; i++)
    {
        struct Node *lhs = rules[i];
        bool flag = false;
        enum nonterminals templ = lhs->data;
        struct Node *rhs = rules[i]->next;
        if (isTerminal(rhs->data))
        {
            enum terminals tempr = rhs->data;
            parseTable[templ][tempr] = rules[i];
        }
        if (isEpsilon(rhs->data))
        {
            struct Node *follow = nts[templ].follow;
            while (follow != NULL)
            {
                enum terminals tempr = follow->data;
                parseTable[templ][tempr] = rules[i];
                follow = follow->next;
            }
        }
        else
        {
            enum nonterminals RHS = rhs->data;
            struct Node *first = nts[RHS].first;
            if (nts[RHS].hasEpsilon)
            {
                flag = true;
                while (flag && rhs->next != NULL)
                {
                    enum nonterminals t = rhs->data;
                    flag = nts[t].hasEpsilon;
                    struct Node *first2 = nts[t].first;
                    while (first2->next != NULL)
                    {
                        enum terminals tempr = first2->data;
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
                        enum terminals tempr = follow->data;
                        parseTable[templ][tempr] = rules[i];
                    }
                }
            }
            else
            {

                while (first->next != NULL)
                {
                    enum terminals tempr = first->data;
                    parseTable[templ][tempr] = rules[i];
                    first = first->next;
                }
            }
        }
    }
}
bool isTerminal(char *data)
{
    return data[0] >= 65 && data[0] <= 90;
}
bool isEpsilon(char *data)
{
    return !isTerminal(data) && sizeof(data) / sizeof(char) == 1 && data[0] == 'e';
}
void push(struct Node *curr, char *s)
{
    curr->next = (struct Node *)malloc(sizeof(struct Node));
    curr = curr->next;
    curr->data = s;
}
struct Node *findFirst(struct Node **rules, char *nonTerminal)
{
    struct Node *currNode = (struct Node *)malloc(sizeof(struct Node)); // first
    struct Node *header = currNode;
    for (int i = 0; i < rows; i++)
    {

        if (strcmp(rules[i]->data, nonTerminal) == 0)
        {
            struct Node *temp = rules[i]->next;
            enum myenum rhs = temp->data;
            enum myenum lhs = nonTerminal;
            if (isTerminal(temp->data))
            {
                push(currNode, temp->data);
            }
            else if (!isEpsilon(temp->data))
            {

                struct Node *childLL = findFirst(rules, temp->data);
                // while ()
                // {
                //     /* code */
                // }

                while (currNode->next != NULL)
                {
                    currNode = currNode->next;
                }
            }
            else
            {

                nts[lhs].hasEpsilon = true;
                currNode->next = findFirst(rules, temp->next->data);
            }
        }
    }
    return currNode;
}