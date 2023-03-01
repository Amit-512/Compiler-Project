// this is used to tell us where the buffer pointer is currently on
typedef enum BuffPointerLocation
{
    BUFF_ONE,
    BUFF_TWO
} BuffPointerLocation;

// buffer implementation is based on twin buffer
typedef struct Buffer
{
    FILE *fp;
    char *buf1, *buf2;
    char *begin, *forward;
    int lineNumber;
    BuffPointerLocation begin_location, forward_location;

} Buffer;

// used for handling error
typedef enum DFAError
{
    ID_TOO_LONG,
    INVALID_COMMENT,
    UNKOWN_SYMBOL,
    RANGEOP_ERR,
    EQ_ERROR,
    COMMENT_ERROR,
    EOF_LEXING,
    INVALID_NUM
} DFAError;

// dfa error enum not written yet
typedef enum DFAStateType
{
    FINAL,
    NONFINAL,
    TRAP
} DFAStateType;

// all the dfa states
typedef enum DFAStates
{
    START_STATE,

    // final states for some symbols
    FINAL_BO,
    FINAL_BC,
    FINAL_PLUS,
    FINAL_MINUS,
    FINAL_DIV,
    FINAL_COMMA,
    FINAL_SQBO,
    FINAL_SQBC,
    FINAL_SEMICOL,

    // for comment and multiply
    STATE_TRAP,
    STATE_NE1,
    STATE_STAR,
    STATE_COMMENT_START,
    STATE_COMMENT_END1,
    FINAL_NE2,
    FINAL_MUL,
    FINAL_COMMENT_END2,

    // rangeop
    STATE_RANGEOP1,
    FINAL_RANGEOP,

    // equality
    STATE_EQ1,
    FINAL_EQ2,

    // colon and assignop
    STATE_COLON_ASSIGNOP,
    FINAL_COLON,
    FINAL_ASSIGNOP,

    // for identifiers and keywords
    STATE_ID1,
    STATE_LONG_ID,
    FINAL_ID,

    // for numbers
    STATE_NUM_RNUM1,
    STATE_NUM_RNUM2,
    STATE_RNUM3,
    STATE_RNUM4,
    STATE_RNUM1,
    STATE_RNUM2,
    FINAL_NUM,
    FINAL_RNUM,

    // for greater symbol family
    STATE_GT,
    STATE_GTGT,
    FINAL_DRIVERENDDEF,
    FINAL_ENDDEF,
    FINAL_GT,
    FINAL_GE,

    // for lesser symbol family
    STATE_LT,
    STATE_LTLT,
    FINAL_DEF,
    FINAL_LE,
    FINAL_LT,
    FINAL_DRIVERDEF,

    // state representing end of file
    FINAL_EOF

    // temporary state for use

} DFAStates;

// function definations
tokenID keywordToTokenID(char *str);
Buffer *getStream(FILE *fp);
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