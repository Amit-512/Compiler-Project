#ifndef _lexerDef
#define _lexerDef

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
    int bufferSize;
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

// enum data structure used for storing the
typedef enum _tokenID
{
    // not terminals removed not required for now

    // all the terminals listed below
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
    TEMP1,
    TEMP2,
    TEMP3,
    TEMP4,
    TEMP5

} tokenID;

// structure of token to store all the information
typedef struct token
{
    tokenID id;
    int lineNumber;
    char *lexeme;
} tokenInfo;

// structure to store the inforemation to store the information of previously extracted token
// not required for now
typedef struct prevtoken
{
    tokenID id;
    int lineNumber;
    char *lexeme;
} prevTokenInfo;

#endif