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

// all the dfa states
typedef enum DFAStates
{
    START_STATE,

    FINAL_BO,
    FINAL_BC,
    FINAL_PLUS,
    FINAL_MINUS,
    FINAL_DIV,
    FINAL_COMMA,
    FINAL_SQBO,
    FINAL_SQBC,
    FINAL_SEMICOL,

    STATE_TRAP,
    STATE_NE1,
    FINAL_NE2,
    FINAL_MUL,
    STATE_STAR,
    STATE_COMMENT_START,
    STATE_COMMENT_END1,
    FINAL_COMMENT_END2,

    STATE_RANGEOP1,
    FINAL_RANGEOP,

    STATE_EQ1,
    FINAL_EQ2,

    STATE_COLON_ASSIGNOP,
    FINAL_COLON,
    FINAL_ASSIGNOP,

    // COULD CHANGE THIS
    STATE_ID1,
    STATE_ID2,
    STATE_ID3,
    STATE_ID4,
    STATE_ID5,
    STATE_ID6,
    STATE_ID7,
    STATE_ID8,
    STATE_ID9,
    STATE_ID10,
    STATE_ID11,
    STATE_ID12,
    STATE_ID13,
    STATE_ID14,
    STATE_ID15,
    STATE_ID16,
    STATE_ID17,
    STATE_ID18,
    STATE_ID19,
    STATE_ID20,
    FINAL_ID,
    STATE_LONG_ID,

    STATE_NUM_RNUM1,
    STATE_NUM_RNUM2,
    STATE_RNUM3,
    STATE_RNUM4,
    STATE_RNUM1,
    STATE_RNUM2,
    FINAL_NUM,
    FINAL_RNUM,

    STATE_GT,
    STATE_GTGT,
    FINAL_GT,
    FINAL_ENDDEF,
    FINAL_DRIVERENDDEF,
    FINAL_GE,

    STATE_LT,
    STATE_LTLT,
    FINAL_LT,
    FINAL_DRIVERDEF,
    FINAL_DEF,
    FINAL_LE,

    FINAL_EOF

} DFAStates;

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
