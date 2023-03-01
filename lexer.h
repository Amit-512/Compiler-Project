#ifndef _lexer
#define _lexer
// function definations
tokenID keywordToTokenID(char *str);
Buffer *getStream(FILE *fp, int bufferSize);
int getLineNumber(Buffer *buffer);
void resetBeginPointer(Buffer *buffer);
tokenInfo *handleLexError(DFAError err, Buffer *buffer);
void eraseLexer(Buffer *buffer);
tokenInfo *getNextToken(Buffer *buffer);
tokenInfo *initializeToken(tokenID tk_id, Buffer *buffer);
tokenInfo *getNextTok(Buffer *buffer);
int _isdigit(char c);
int _isalphabet(char c);
char getNextChar(Buffer *buffer);
void retract(Buffer *buffer);
char *extractLexeme(Buffer *buffer);
void removeComments(char *testcaseFile, char *cleanFile);
tokenInfo *getNextTokenWithErrors(Buffer *buffer);
void printAllTokens(char *fileName,int buffSize);
void cleanUpForBeginPointer(Buffer *buffer);
#endif