#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "lexer.h"
#include <ctype.h>
#include <string.h>

// defining the size of buffer for each buffer in twin buffere
// #define buffSize 128

// function to check if the character is digit
int _isdigit(char c)
{
    if ((int)(c - '0') >= 0 && (int)(c - '0') <= 9)
    {
        return 1;
    }

    return 0;
}

// function to check if the character is an alphabet or underscore
int _isalphabet(char c)
{
    if (c >= 'a' && c <= 'z' || c == '_' || c >= 'A' || c <= 'Z')
    {
        return 1;
    }

    return 0;
}

// function to remove comments and write the source code to a new file
void removeComments(char *testcaseFile, char *cleanFile)
{
    // if told to keep the error
    FILE *fp1, *fp2;

    fp1 = fopen(testcaseFile, "r");

    if (fp1 == NULL)
    {
        printf("Cannot open file %s \n", testcaseFile);
        exit(0);
    }

    fp2 = fopen(cleanFile, "w");

    if (fp1 == NULL)
    {
        printf("Cannot open file %s \n", cleanFile);
        exit(0);
    }

    char c = fgetc(fp1);

    while (c != EOF)
    {
        if (c == '*')
        {
            c = fgetc(fp1);

            if (c == '*')
            {
                c = fgetc(fp1);

                while (1)
                {
                    while (c != '*' && c != EOF)
                    {
                        if (c == '\n')
                            fputc(c, fp2);

                        c = fgetc(fp1);
                    }

                    if (c == '*')
                    {
                        c = fgetc(fp1);
                        if (c == '*')
                        {
                            // comment done
                            c = fgetc(fp1);
                            break;
                        }
                        else
                        {
                            // comment still going on
                            // do nothing for now
                        }
                    }

                    if (c == EOF)
                    {
                        // file has ended, comment end not found
                        // need to handle the case when file ended but comment didnt

                        printf("Comment error in the source code\n");
                        break;
                    }
                }
            }
            else
            {
                fputc('*', fp2);
            }
        }

        if (c == EOF)
        { // useful when one * comes just before eof
            break;
        }

        fputc(c, fp2);
        c = fgetc(fp1);
    }

    fclose(fp1);
    fclose(fp2);
}

// this function returns tkid of a string if it is a keyword else returns -1
tokenID keywordToTokenID(char *str)
{
    if (strcmp(str, "program") == 0)
        return PROGRAM;

    if (strcmp(str, "get_value") == 0)
        return GET_VALUE;

    if (strcmp(str, "print") == 0)
        return PRINT;

    if (strcmp(str, "use") == 0)
        return USE;

    if (strcmp(str, "with") == 0)
        return WITH;

    if (strcmp(str, "OR") == 0)
        return OR;

    if (strcmp(str, "AND") == 0)
        return AND;

    if (strcmp(str, "true") == 0)
        return TRUE;

    if (strcmp(str, "false") == 0)
        return FALSE;

    if (strcmp(str, "for") == 0)
        return FOR;

    if (strcmp(str, "in") == 0)
        return IN;

    if (strcmp(str, "while") == 0)
        return WHILE;

    if (strcmp(str, "takes") == 0)
        return TAKES;

    if (strcmp(str, "input") == 0)
        return INPUT;

    if (strcmp(str, "returns") == 0)
        return RETURNS;

    if (strcmp(str, "integer") == 0)
        return INTEGER;

    if (strcmp(str, "real") == 0)
        return REAL;

    if (strcmp(str, "boolean") == 0)
        return BOOLEAN;

    if (strcmp(str, "of") == 0)
        return OF;

    if (strcmp(str, "array") == 0)
        return ARRAY;

    if (strcmp(str, "start") == 0)
        return START;

    if (strcmp(str, "end") == 0)
        return END;

    if (strcmp(str, "declare") == 0)
        return DECLARE;

    if (strcmp(str, "module") == 0)
        return MODULE;

    if (strcmp(str, "driver") == 0)
        return DRIVER;

    if (strcmp(str, "parameters") == 0)
        return PARAMETERS;

    if (strcmp(str, "switch") == 0)
        return SWITCH;

    if (strcmp(str, "case") == 0)
        return CASE;

    if (strcmp(str, "break") == 0)
        return BREAK;

    if (strcmp(str, "default") == 0)
        return DEFAULT;

    tokenID toret = (tokenID)-1;

    return toret;
}

// this function initailises the twin buffer for extracting tokens
Buffer *getStream(FILE *fp, int bufferSize)
{
    // creating a buffer
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));

    buffer->bufferSize = bufferSize;

    if (fp == NULL)
    {
        return NULL;
    }

    // initializing the file pointer
    buffer->fp = fp;

    // set the line number to 1 initailly
    buffer->lineNumber = 1;

    // allocating memory to each buffer pointer
    buffer->buf1 = (char *)malloc(sizeof(char) * (buffer->bufferSize + 1));

    // int temp;

    // initializing buffer1
    int temp;
    temp = fread(buffer->buf1, sizeof(char), buffer->bufferSize, buffer->fp);
    buffer->buf1[temp] = EOF;

    // setting both the pointers
    buffer->begin = buffer->buf1;
    buffer->forward = buffer->buf1;

    buffer->buf2 = (char *)malloc(sizeof(char) * (buffer->bufferSize + 1));

    // initailzing buffer2
    temp = fread(buffer->buf2, sizeof(char), buffer->bufferSize, buffer->fp);
    buffer->buf2[temp] = EOF;

    // setting locations of each pointer
    buffer->begin_location = BUFF_ONE;
    buffer->forward_location = BUFF_ONE;

    return buffer;
}

// this function returns the line number of a lexeme when called upon
int getLineNumber(Buffer *buffer)
{
    return buffer->lineNumber;
}

// function to extract a lexeme between forward and begin pointer
char *extractLexeme(Buffer *buffer)
{

    // cases:
    // 1. forward and begin pointer in same buffer
    // 2. forward and begin pointer in different buffer
    //    only possible when beign in first and forward in second
    // 3. when forward points to eof

    int i, len;
    char *retLexeme; // will contain the lexeme to be returned
    char *iter;      // used for iterating from begin to forward

    if (*buffer->forward == EOF)
    {

        if (buffer->begin_location != buffer->forward_location) // both the pointers in the same buffer
        {
            int temp1, temp2;

            int len = (buffer->forward - buffer->buf2 + 1) + (buffer->buf1 + buffer->bufferSize - buffer->begin);

            retLexeme = (char *)malloc(sizeof(char) * (len + 1));

            retLexeme[len] = '\0';

            iter = buffer->begin;
            if (*iter == EOF)
                return NULL;

            for (i = 0; *iter != EOF; i++, iter++)
            {
                retLexeme[i] = *iter;
            }

            iter = buffer->buf2;
            for (; i < len; i++, iter++)
            {
                retLexeme[i] = *iter;
            }
        }
        else
        {
            iter = buffer->begin;
            if (*iter == EOF)
                return NULL;

            while (*iter != EOF)
                iter++;

            retLexeme = (char *)malloc(sizeof(char) * (iter - buffer->begin + 1));
            iter = buffer->begin;
            for (i = 0; *iter != EOF; i++)
            {
                retLexeme[i] = *iter;
                iter++;
            }

            retLexeme[i] = '\0';
            return retLexeme;
        }
    }
    else if (buffer->begin_location == BUFF_ONE && buffer->forward_location == BUFF_TWO)
    {

        len = (buffer->buf1 + buffer->bufferSize - buffer->begin) + (buffer->forward - buffer->buf2 + 1);
        retLexeme = (char *)malloc(sizeof(char) * (len + 1));

        retLexeme[len] = '\0';

        iter = buffer->begin;
        for (i = 0; *iter != EOF; i++, iter++)
        {
            retLexeme[i] = *iter;
        }

        iter = buffer->buf2;
        for (; i < len; i++, iter++)
        {
            retLexeme[i] = *iter;
        }
    }
    else if ((buffer->begin_location == BUFF_ONE && buffer->forward_location == BUFF_ONE) || (buffer->begin_location == BUFF_TWO && buffer->forward_location == BUFF_TWO))
    {

        len = buffer->forward - buffer->begin + 1;

        retLexeme = (char *)malloc(sizeof(char) * (len + 1));

        iter = buffer->begin;

        retLexeme[len] = '\0';

        for (int i = 0; i < len; i++, iter++)
        {
            retLexeme[i] = *iter;
        }
    }
    else
    {
        // nothing for now
    }

    return retLexeme;
}

// this function does what it says it does
// retract the forward pointer by one position
void retract(Buffer *buffer)
{
    // if forward pointer is in first buffer
    if (buffer->forward_location == BUFF_ONE)
    {
        if (buffer->forward != buffer->buf1)
        {
            buffer->forward--;
        }
        else
        {
            // case when it points to first position in first buffer
            printf("Retract not possible\n");
        }

        if (*buffer->forward == '\n')
            buffer->lineNumber--;
    }
    else if (buffer->forward_location == BUFF_TWO)
    {
        // if(forward is in second buffer and points to starting of the buffer)
        if (buffer->forward != buffer->buf2)
        {
            // points to previous
            buffer->forward--;
        }
        else
        {
            buffer->forward = &buffer->buf1[buffer->bufferSize - 1];
            buffer->forward_location = BUFF_ONE;
        }

        if (*buffer->forward == '\n')
            buffer->lineNumber--;
    }
    else
    {
        if (buffer->forward != buffer->buf2)
        {
            // points to previous
            buffer->forward--;
        }
        else
        {
            buffer->forward = &buffer->buf1[buffer->bufferSize - 1];
            buffer->forward_location = BUFF_ONE;
        }

        if (*buffer->forward == '\n')
            buffer->lineNumber--;
    }

    // if the forward now points to \n the reduce the line number
}

// resets the beign pointer
void resetBeginPointer(Buffer *buffer)
{
    char *temp; // not useful for now; everything works fine

    if (buffer->begin_location != buffer->forward_location)
    {
        buffer->begin_location = BUFF_TWO;
    }

    buffer->begin = buffer->forward;
}

// this function handles the error which are caught while making tokens
tokenInfo *handleLexError(DFAError err, Buffer *buffer)
{
    // retract(buffer);

    // initializing the token

    tokenInfo *tk = (tokenInfo *)malloc(sizeof(tokenInfo));
    tk->lineNumber = getLineNumber(buffer);
    tk->id = LEXERROR;

    int len = 0;

    if (err == COMMENT_ERROR)
    {
        len = strlen("Comment not valid");
        tk->lexeme = (char *)malloc(sizeof(char) * (len + 1));
        strcpy(tk->lexeme, "Comment not valid");
    }
    else if (err == INVALID_NUM)
    {
        len = strlen("Number not valid");
        retract(buffer);
        tk->lexeme = (char *)malloc(sizeof(char) * (len + 1));
        strcpy(tk->lexeme, "Number not valid");
    }
    else if (err == ID_TOO_LONG)
    {
        len = strlen("Too long identifier");
        retract(buffer);
        tk->lexeme = (char *)malloc(sizeof(char) * (len + 1));
        strcpy(tk->lexeme, "Too long identifier");
    }
    else if (err == EQ_ERROR)
    {
        len = strlen("Expected = operator");
        retract(buffer);
        tk->lexeme = (char *)malloc(sizeof(char) * (len + 1));
        strcpy(tk->lexeme, "Expected '='");
    }
    else if (err == RANGEOP_ERR)
    {
        // abc.abc
        len = strlen("Expected '.'");
        retract(buffer);
        tk->lexeme = (char *)malloc(sizeof(char) * (len + 1));
        strcpy(tk->lexeme, "Expected '.'");
    }
    else if (err == UNKOWN_SYMBOL)
    {
        len = strlen("Unknown Symbol");
        tk->lexeme = (char *)malloc(sizeof(char) * (len + 1));
        strcpy(tk->lexeme, "Unknown Symbol");
    }

    getNextChar(buffer);
    resetBeginPointer(buffer);

    return tk;
}

// function to get next char // to be used for simulating the dfa
char getNextChar(Buffer *buffer)
{
    char c;

    c = *buffer->forward; // character pointed by the forward pointer

    char ctemp = c; // not useful for now

    if (c == '\n') // if it is \n increase the line number
        buffer->lineNumber++;

    if (c == EOF) // if it is eof then return eof
        return EOF;

    buffer->forward++; // increment the forward pointer

    // handling cases for different characters that forward may point now

    // now after incrementing if the forward is in first buffer and points to eof.
    if (buffer->forward_location == BUFF_TWO && *buffer->forward == EOF)
    {
        // case when we have exhauseted our buffer
        if (buffer->begin_location == BUFF_TWO && buffer->forward - buffer->buf2 == buffer->bufferSize)
        {
            // make buff2 buff1 and refill buf2
            // this shows swap
            char *temp = buffer->buf1;
            buffer->buf1 = buffer->buf2;
            buffer->buf2 = temp;

            // refill the second buffer
            int siz = fread(buffer->buf2, sizeof(char), buffer->bufferSize, buffer->fp);
            buffer->forward = buffer->buf2;
            buffer->buf2[siz] = EOF;

            // begin is in buffer one now
            buffer->begin_location = BUFF_ONE;
        }
        else
        {
            // nothing for now
        }
    }
    else if (buffer->forward_location == BUFF_ONE && *buffer->forward == EOF)
    {
        // this signifies that forward has reached the end of buffer1 so it needs to be placed in the second buffer
        if (buffer->forward - buffer->buf1 == buffer->bufferSize)
        {
            buffer->forward_location = BUFF_TWO;
            buffer->forward = buffer->buf2;
        }
        else
        {
            // nothing to be done for now
        }
    }

    return c;
}

// function to get tokens along with along with errors
tokenInfo *getNextTokenWithErrors(Buffer *buffer)
{
    // loop will keep on running until a valid token is found
    while (1)
    {
        tokenInfo *tok = getNextTok(buffer);

        // is it is a comment token then do not pass it to lexer
        if (tok->id != COMMENT)
        {
            return tok;
        }
    }
}

// this is the function which is called by parses to get the next token
tokenInfo *getNextToken(Buffer *buffer)
{
    // loop will keep on running until a valid token is found
    while (1)
    {
        tokenInfo *tok = getNextTok(buffer);

        // is it is a comment token then do not pass it to lexer
        if (tok->id != COMMENT && tok->id != LEXERROR)
        {
            return tok;
        }
    }
}

// this function initializes a token
tokenInfo *initializeToken(tokenID tk_id, Buffer *buffer)
{
    tokenInfo *ret = (tokenInfo *)malloc(sizeof(tokenInfo));
    ret->id = tk_id;
    ret->lexeme = extractLexeme(buffer);
    ret->lineNumber = getLineNumber(buffer);

    return ret;
}

// this function simulates a dfa to get the tokens.
tokenInfo *getNextTok(Buffer *buffer)
{
    DFAStates dfa = START_STATE;
    DFAStateType stateType = NONFINAL;
    DFAError error;

    int id_len = 0; // used to keep check of the length of identifiers

    char c;
    tokenInfo *tk = NULL; // to be returned

    while (1)
    {

        switch (dfa)
        {
        case START_STATE:
            c = getNextChar(buffer);
            stateType = NONFINAL;

            if (c == '\n' || c == ' ' || c == '\t')
            {
                dfa = START_STATE;
                resetBeginPointer(buffer);
            }
            else if (c == EOF)
            {
                dfa = FINAL_EOF;
            }
            else if (c == '*')
            {
                dfa = STATE_STAR;
            }
            else if (c == '>')
            {
                dfa = STATE_GT;
            }
            else if (c == '<')
            {
                dfa = STATE_LT;
            }
            else if (c == ':')
                dfa = STATE_COLON_ASSIGNOP;
            else if (c == '.')
                dfa = STATE_RANGEOP1;
            else if (c == ',')
                dfa = FINAL_COMMA;
            else if (c == '!')
                dfa = STATE_NE1;
            else if (c == '=')
                dfa = STATE_EQ1;
            else if (c == ')')
                dfa = FINAL_BC;
            else if (c == '(')
                dfa = FINAL_BO;
            else if (c == ']')
                dfa = FINAL_SQBC;
            else if (c == '[')
                dfa = FINAL_SQBO;
            else if (c == ';')
                dfa = FINAL_SEMICOL;
            else if (c == '+')
                dfa = FINAL_PLUS;
            else if (c == '-')
                dfa = FINAL_MINUS;
            else if (c == '/')
                dfa = FINAL_DIV;
            else if (isdigit(c))
            {
                dfa = STATE_NUM_RNUM1;
            }
            else if (isalpha(c) || c == '_')
            {
                dfa = STATE_ID1;
                id_len++;
            }
            else
            {
                retract(buffer);
                resetBeginPointer(buffer);
                error = UNKOWN_SYMBOL;
                dfa = STATE_TRAP;
            }

            break;

        case FINAL_PLUS:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(PLUS, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_MINUS:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(MINUS, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_DIV:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(DIV, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_SQBC:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(SQBC, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_SQBO:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(SQBO, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_BC:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(BC, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_BO:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(BO, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_ID1:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (isalpha(c) || isdigit(c) || c == '_')
            {
                id_len++;

                if (id_len <= 20)
                {
                    dfa = STATE_ID1;
                }
                else
                {
                    dfa = STATE_LONG_ID;
                }
            }
            else
            {
                if (c != EOF)
                {
                    retract(buffer);
                }

                dfa = FINAL_ID;
            }

            break;

        case FINAL_ID:
            stateType = FINAL;

            retract(buffer);

            tk = (tokenInfo *)malloc(sizeof(tokenInfo));

            tk = initializeToken(ID, buffer);

            if ((tk->id = keywordToTokenID(tk->lexeme)) == -1)
                tk->id = ID;

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_LONG_ID:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (!(isalpha(c) || isdigit(c) || c == '_'))
            {
                error = ID_TOO_LONG;
                dfa = STATE_TRAP;
                if (c != EOF)
                    retract(buffer);
                resetBeginPointer(buffer);
            }

            break;

        case STATE_RANGEOP1:
            stateType = NONFINAL;
            c = getNextChar(buffer);
            if (c != '.')
            {
                if (c == EOF)
                {
                }
                else
                {
                    retract(buffer);
                }

                error = RANGEOP_ERR;
                dfa = STATE_TRAP;
                resetBeginPointer(buffer);
            }
            else
            {
                dfa = FINAL_RANGEOP;
            }
            break;

        case FINAL_RANGEOP:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(RANGEOP, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_SEMICOL:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(SEMICOL, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_COMMA:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(COMMA, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_COLON_ASSIGNOP:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == '=')
                dfa = FINAL_ASSIGNOP;
            else
            {
                if (c != EOF)
                    retract(buffer);
                dfa = FINAL_COLON;
            }

            break;

        case FINAL_ASSIGNOP:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(ASSIGNOP, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_COLON:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(COLON, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_EQ1:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == '=')
                dfa = FINAL_EQ2;
            else
            {
                if (c != EOF)
                    retract(buffer);
                error = EQ_ERROR;
                dfa = STATE_TRAP;
                resetBeginPointer(buffer);
            }

            break;

        case FINAL_EQ2:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(EQ, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_NE1:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == '=')
                dfa = FINAL_NE2;
            else
            {
                if (c != EOF)
                    retract(buffer);
                error = EQ_ERROR;
                dfa = STATE_TRAP;
                resetBeginPointer(buffer);
            }

            break;

        case FINAL_NE2:
            retract(buffer);
            stateType = FINAL;

            tk = initializeToken(NE, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_GT:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == '=')
            {
                dfa = FINAL_GE;
            }
            else if (c == '>')
            {

                dfa = STATE_GTGT;
            }
            else
            {
                dfa = FINAL_GT;

                if (c == EOF)
                {
                }
                else
                {
                    retract(buffer);
                }
            }

            break;

        case STATE_GTGT:
            c = getNextChar(buffer);
            stateType = NONFINAL;

            if (c == '>')
                dfa = FINAL_DRIVERENDDEF;
            else
            {
                dfa = FINAL_ENDDEF;

                if (c != EOF)
                    retract(buffer);
            }

            break;

        case FINAL_DRIVERENDDEF:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(DRIVERENDDEF, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_ENDDEF:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(ENDDEF, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_GE:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(GE, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_GT:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(GT, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_LT:
            stateType = NONFINAL;
            c = getNextChar(buffer);
            if (c == '=')
                dfa = FINAL_LE;
            else if (c == '<')
                dfa = STATE_LTLT;
            else
            {
                dfa = FINAL_LT;
                if (c != EOF)
                    retract(buffer);
            }

            break;

        case STATE_LTLT:
            stateType = NONFINAL;
            c = getNextChar(buffer);
            if (c == '<')
                dfa = FINAL_DRIVERDEF;
            else
            {
                if (c != EOF)
                    retract(buffer);
                dfa = FINAL_DEF;
            }

            break;

        case FINAL_DRIVERDEF:
            retract(buffer);

            stateType = FINAL;
            tk = initializeToken(DRIVERDEF, buffer);
            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_DEF:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(DEF, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_LT:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(LT, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_LE:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(LE, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_STAR:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c != '*')
            {
                if (c != EOF)
                    retract(buffer);
                dfa = FINAL_MUL;
            }
            else
            {
                dfa = STATE_COMMENT_START;
            }

            break;

        case STATE_COMMENT_START:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == '*')
                dfa = STATE_COMMENT_END1;
            else if (c == EOF)
            {
                error = COMMENT_ERROR;
                dfa = STATE_TRAP;
            }
            else
                resetBeginPointer(buffer);

            break;

        case STATE_COMMENT_END1:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == EOF)
            {
                error = COMMENT_ERROR;
                dfa = STATE_TRAP;
            }
            else if (c == '*')
                dfa = FINAL_COMMENT_END2;
            else
            {
                dfa = STATE_COMMENT_START;
                resetBeginPointer(buffer);
            }

            break;

        case FINAL_COMMENT_END2:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(COMMENT, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_MUL:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(MUL, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case STATE_NUM_RNUM1:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == '.')
                dfa = STATE_NUM_RNUM2;
            else if (!isdigit(c))
            {
                if (c != EOF)
                    retract(buffer);
                dfa = FINAL_NUM;
            }

            break;

        case STATE_NUM_RNUM2:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == '.')
            {
                retract(buffer);
                retract(buffer);
                dfa = FINAL_NUM;
            }
            else if (isdigit(c))
                dfa = STATE_RNUM1;
            else
            {
                error = INVALID_NUM;
                dfa = STATE_TRAP;
                if (c != EOF)
                {
                    retract(buffer);
                }
                resetBeginPointer(buffer);
            }

            break;

        case STATE_RNUM1:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == 'e' || c == 'E')
            {
                dfa = STATE_RNUM2;
            }
            else if (isdigit(c))
                ;
            else
            {
                if (c != EOF)
                    retract(buffer);
                dfa = FINAL_RNUM;
            }

            break;

        case STATE_RNUM2:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (c == '+' || c == '-')
                dfa = STATE_RNUM3;
            else if (isdigit(c))
            {
                dfa = STATE_RNUM4;
            }
            else
            {
                error = INVALID_NUM;
                dfa = STATE_TRAP;
                if (c != EOF)
                    retract(buffer);
                resetBeginPointer(buffer);
            }
            break;

        case STATE_RNUM3:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (isdigit(c))
                dfa = STATE_RNUM4;
            else
            {
                error = INVALID_NUM;
                dfa = STATE_TRAP;
                retract(buffer);
                resetBeginPointer(buffer);
            }

            break;

        case STATE_RNUM4:
            stateType = NONFINAL;
            c = getNextChar(buffer);

            if (isdigit(c))
                ;
            else
            {
                if (c != EOF)
                    retract(buffer);
                dfa = FINAL_RNUM;
            }

            break;

        case FINAL_NUM:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(NUM, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_RNUM:
            retract(buffer);

            stateType = FINAL;

            tk = initializeToken(RNUM, buffer);

            getNextChar(buffer);
            resetBeginPointer(buffer);

            break;

        case FINAL_EOF:
            stateType = FINAL;

            tk = (tokenInfo *)malloc(sizeof(tokenInfo));
            tk->id = TK_EOF;
            tk->lexeme = (char *)malloc(sizeof(char));
            *tk->lexeme = EOF;
            tk->lineNumber = getLineNumber(buffer);

            break;

        case STATE_TRAP:
            stateType = TRAP;
            tk = handleLexError(error, buffer);
            break;

        default:
            // do nothing for now
            break;
        }

        if (stateType == FINAL || stateType == TRAP)
        {
            break;
        }
    }

    if (c == EOF && stateType == NONFINAL)
    {
        error = EOF_LEXING;
        tk = handleLexError(error, buffer);
    }

    return tk;
}

// this function destroys the buffer by dellocating the memory occupied by it
void eraseLexer(Buffer *buffer)
{
    fclose(buffer->fp);
    free(buffer->buf1);
    free(buffer->buf2);
    free(buffer);
}

char *terminals[] = {"ID", "TRUE", "FALSE", "COMMENT", "AND", "OR", "INTEGER", "REAL", "BOOLEAN", "OF", "ARRAY", "START", "END", "DECLARE", "MODULE", "DRIVER", "PROGRAM", "GET_VALUE", "PRINT", "USE", "WITH", "PARAMETERS", "TAKES", "INPUT", "RETURNS", "FOR", "IN", "SWITCH", "CASE", "BREAK", "DEFAULT", "WHILE", "NUM", "RNUM", "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GE", "GT", "EQ", "NE", "DEF", "ENDDEF", "DRIVERDEF", "DRIVERENDDEF", "COLON", "RANGEOP", "SEMICOL", "COMMA", "ASSIGNOP", "SQBO", "SQBC", "BO", "BC", "LEXERROR", "TK_EOF"};

void printAllTokens(char *fileName,int buffSize)
{
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL)
    {
        printf("file didnt open\n");
    }

    Buffer *buff = getStream(fp, buffSize);

    tokenInfo *tk = getNextTokenWithErrors(buff);

    while (tk->id != TK_EOF)
    {
        printf("%d        %s        %s\n", tk->lineNumber, tk->lexeme, terminals[tk->id]);
        tk = getNextTokenWithErrors(buff);
    }
}