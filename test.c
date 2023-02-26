#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define rows 142
#define columns 200
typedef struct Node
{
    struct Node *next;
    char *lexicalElement;
}Node;

char *getRule(FILE *grammer)
{
    char *buff;
    buff = (char *)malloc(columns * sizeof(char));
    fgets(buff, columns, grammer);
    return buff;
}
int main()
{
    FILE *grammer;
    grammer = fopen("grammer.txt", "r");
    // struct Node *temp = NULL
    Node** head;
    head = (Node *)malloc(rows * sizeof(Node));
    for (int i = 0; i < rows; ++i)
    {
        *(head + i) = NULL;
    }
    for (int i = 0; i < rows; ++i)
    {
        // To track last node of the list
        Node *prev = NULL;

        // Randomly taking 4 nodes in each
        // linked list
        char *rule = getRule(grammer);
        char *currentLexicalElement = strtok(rule, " "); // it is the current lexical element
        while (currentLexicalElement!=NULL)
        {
            // Create a new node
            Node *n = (Node *)malloc(sizeof(Node));

            // Input the random data
            n->lexicalElement = currentLexicalElement;
            n->next = NULL;

            // If the node is first
            if (*(head + i) == NULL)
            {
                *(head + i) = n;
            }
            else
            {
                prev->next = n;
            }
            prev = n;
            currentLexicalElement = strtok(NULL, " ");
        }
    }
    Node *temp2 =*(head+2);
    printf("%s",(temp2->next)->lexicalElement);
    // struct Node* table[numberOfNonTerminals][noOFTerminals];
}