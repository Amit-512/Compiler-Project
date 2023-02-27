#ifndef _lexerDef
#define _lexerDef

typedef enum _tokenID
{   
    // not terminals removed not required for now

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
} tokenID;

typedef struct token
{
    tokenID id;
    int lineNumber;
    char *lexeme;
} tokenInfo;

#endif