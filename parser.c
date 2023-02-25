#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    struct Node* next;
    char* lexicalElement;
};

char* getRule(FILE* grammer)
{
    char* buff;
    buff = (char*)malloc(200 * sizeof(char));
    fgets(buff,200,grammer);
    return buff;
}

int main()
{
    FILE* grammer;
    grammer = fopen("grammer.txt", "r");
    struct Node* rules[142];
    for(int j=0;j<142;j++)
    {
        char* rule = getRule(grammer);
        char* currentLexicalElement = strtok(rule," ");//it is the current lexical element
        struct Node* currNode;
        struct Node* head = currNode;
        while(currentLexicalElement!=NULL)
        {
            printf("%s",currentLexicalElement);
            currNode->lexicalElement = currentLexicalElement;
            currentLexicalElement = strtok(NULL," ");
            currNode=currNode->next;
            currNode = (struct Node*)malloc(sizeof(struct Node));
            
        }
        rules[j]=head;
    }
    struct Node* table[numberOfNonTerminals][noOFTerminals];
    


}