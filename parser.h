#ifdef _parser
#define _parser

int hash(char *key, int i);
void insertNT(char *key, int value);
void insertT(char *key, int value);
int getNT(char *key);
int getT(char *key);
char *getRule(FILE *grammar);
bool isTerminal(char *data);
bool isEpsilon(char *s);
void printLL(struct Node *header);
struct Node *appendLinkedList(struct Node *n1, struct Node *n2);
void insertNonTerminals(char **nonTerminals);
void insertTerminals(char **Terminals);
struct Node *createNode(char *s);
void push(struct Node *curr, char *s);
struct Node *findFirst(struct Node **rules, char *nonTerminal);
struct Node *removeDuplicatesFromLL(struct Node *head);
struct Node *cloneLinkedList(struct Node *list);
struct Node *findFollow(struct Node **rules, char *nonTerminal);
void makeLLOfGrammar(FILE *grammar, struct Node **rules);
void caluculateFollowSet(struct Node **rules);
void calculateFirstSet(struct Node **rules);
void printParseTable(struct Node *parseTable[number_nt][number_t]);
void fillParserTable(struct Node *parseTable[number_nt][number_t], struct Node **rules);
void initParser();

#endif