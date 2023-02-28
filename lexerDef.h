#ifndef _lexerDef
#define _lexerDef

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