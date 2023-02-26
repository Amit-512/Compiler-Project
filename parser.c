#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< Updated upstream

struct Node
{
    struct Node *next;
    char *lexicalElement;
=======
#include<stdbool.h>
#define rows 142
#define columns 200
enum nonTerminals{
    program,moduleDeclarations,moduleDeclaration,otherModules,driverModule,module,ret,input_plist,n1,output_plist,n2,dataType,range_arrays,type,moduleDef,statements,statement,ioStmt,boolConst,id_num_rnum,var_print,p1,simpleStmt,assignmentStmt,whichStmt,lvalueIDStmt,lvalueARRStmt,index_arr,new_index,sign,moduleReuseStmt,optional,idList,n3,actual_para_list,expression,u,new_NT,var_id_num,unary_op,arithmeticOrBooleanExpr,n7,anyTerm,n8,arithmeticExpr,n4,term,n5,factor,n_11,element_index_with_expressions,n_10 ,arrExpr,arr_n4,arrTerm,arr_n5,arrFactor,op1,logicalOp,relationalOp,declareStmt,conditionalStmt,caseStmts,n9,value,defaultP,iterativeStmt,range_for_loop,index_for_loop,new_index_for_loop,sign_for_loop
};
struct Node{
    struct Node* next;
    char* lexicalElement;
    struct Node* prev;
>>>>>>> Stashed changes
};

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
<<<<<<< Updated upstream
    struct Node *rules[142];
    for (int j = 0; j < 142; j++)
    {
        char *rule = getRule(grammer);
        char *currentLexicalElement = strtok(rule, " "); // it is the current lexical element

        struct Node *currNode = (struct Node *)malloc(sizeof(struct Node));
        struct Node *head = currNode;
        while (currentLexicalElement != NULL)
        {
            printf("%s", currentLexicalElement);

            currNode->lexicalElement = currentLexicalElement;

            currentLexicalElement = strtok(NULL, " ");

            currNode = currNode->next;
            currNode = (struct Node *)malloc(sizeof(struct Node));
        }
        rules[j] = head;
    }
    // struct Node* table[numberOfNonTerminals][noOFTerminals];
=======
    struct Node* rules[142];
    bool epsilon[142];
    for(int j=0;j<142;j++)
    {
        char* rule = getRule(grammer);
        char* currentLexicalElement = strtok(rule," ");//it is the current lexical element
        struct Node* currNode = (struct Node*)malloc(sizeof(struct Node));
        struct Node* head = currNode;
        while(currentLexicalElement!=NULL)
        {
            if(currentLexicalElement=="e"){
                // epsilon[j]=
            }
            currNode->lexicalElement = currentLexicalElement;
            currentLexicalElement = strtok(NULL," ");
            currNode->next = (struct Node*)malloc(sizeof(struct Node));
            currNode->next->prev=currNode;
            currNode = currNode->next;
            
        }
        rules[j]=head;
    }
    struct Node* temp = rules[0]->next->next;
    printf("%s",temp->lexicalElement);
    temp = rules[0]->next;
    if(temp==NULL)
    {
        printf("%d",1);
    }
    

}

void push(struct Node* curr,char* s)
{
    curr->next = (struct Node*)malloc(sizeof(struct Node));
    curr=curr->next;
    curr->lexicalElement = s;
}
void first(struct Node** rules,struct Node* nonTerminal)
{
    
    {
        char *s = rules[i]->next->lexicalElement;
        if(isTerminal(s)){
            // push s in first ki array
        }
        else if(s[0]=='e' && sizeof(s)/sizeof(char)==1){
            if(x)
            // push epsilon
        }else if(isNonTerminal(s)){
            //push every element of first(s);
            //but if this contains e then 
            // first(rules,nonTerminal->next)
            struct Node* currNonTerminal = rules[i]->next;
            while (checkforE(currNonTerminal))//if currentNonTerminal gives e then currNonTerminal
            {
                /* code */
            }
            
        }
    }
>>>>>>> Stashed changes
}