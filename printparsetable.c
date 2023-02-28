#include <stdio.h>

typedef struct Node
{
    struct Node *next;
    char *data;
    struct Node *prev;
} Node;

void printParseTable(Node ***parseTable, char**terminals, char**nonterminals, int t_count, int nt_count){
    FILE *fp = fopen("output.csv", "w");

    for(int i=0; i<t_count; i++){
        fprintf(fp, ",%s", terminals[i]);
    }

    fprintf(fp, "\n");

    for(int row=0; row<nt_count; row++){
        fprintf(fp, "%s", nonterminals[row]);

        for(int col=0; col<t_count; col++){
            fprintf(fp, ",");

            Node* curr = parseTable[row][col];
            while (curr != NULL)
            {
                fprintf(fp, "%s..", curr->data);
            }
        }

        fprintf(fp, '\n');
    }

    fclose(fp);

}

int main(){

}