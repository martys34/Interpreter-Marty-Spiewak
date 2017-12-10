#include <stdio.h>
//#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include "structures.h"

char testKey[50];
char tk[10000];


FILE *file;
Stack* stack;
char* keywordStrings[] = {
        "auto",
        "alignof",
        "break",
        "case",
        "char",
        "const",
        "continue",
        "default",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "goto",
        "if",
        "int",
        "long",
        "register",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "unsigned",
        "void",
        "volatile",
        "while",
};

TokenType keywords[] = {
        AUTO,
        ALIGNOF,
        BREAK,
        CASE,
        CHAR,
        CONST,
        CONTINUE,
        DEFAULT,
        DO,
        DOUBLE,
        ELSE,
        ENUM,
        EXTERN,
        FLOAT,
        FOR,
        GOTO,
        IF,
        INT,
        LONG,
        REGISTER,
        RETURN,
        SHORT,
        SIGNED,
        SIZEOF,
        STATIC,
        STRUCT,
        SWITCH,
        TYPEDEF,
        UNION,
        UNSIGNED,
        VOID,
        VOLATILE,
        WHILE,
        NOPE
};

char hexSet[] = {
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F'
};

char suffixSet[] = {
        'u',
        'U',
        'l',
        'L',
        'f',
        'F'
};

char getNextChar();
char readChar();
Token * nextToken();
void ungetChar(char c);
_Bool checkHex(char c);
_Bool checkSuffix(char c);
TokenType checkKeyword(char* lex);

//int main(int argc, char *argv[]) {
void loadFile(char* path) {
//    char* path;
//    if(argc > 1) {
//        path = argv[1];
//    }
//    else {
//        path = "../TestFile.c";
//    }

    file = fopen(path, "r");
    //if opening in linux environment
    if (file == NULL) {
        path = "TestFile.c";
        file = fopen(path, "r");
    }
    if (file != NULL) {
        stack = createStack();
        printf("%s", "File has been loaded.\n");
    }

}

void tokenizerTest() {

        Token *token = nextToken();
        while (token->type != END) {
            printf("%s", tokenToString(token));
            memset(tk, 0, sizeof(tk));
            memset(testKey, 0, sizeof(testKey));
            token = nextToken();

        }
        printf("%s", tokenToString(token));

}

Token* nextToken() {
    memset(tk, 0, sizeof(tk));
    memset(testKey, 0, sizeof(testKey));
    char c;
    //index of token
    int i, state;

    //skip whitespace
    while (isspace(c = getNextChar()) || c == '\r') ;

    //if at end of input stream
    if (c == (char) (-1)) {
        return createToken(END, "EOF", 3);
    }

    i = 0;
    tk[i++] = c;

    //if an integer or float
    if (isdigit(c) || c == '.') {
        if(c == 0) {
            char second = getNextChar();
            switch(second) {
                case 'x' :
                case 'X' :
                    state = 9;
                    break;
                default:
                    state = 11;
            }
        }
        if (c == '.') {
            char second = getNextChar();
            char third = getNextChar();
            if(second == '.') {
                if(third == '.') {
                    tk[1] = second;
                    tk[2] = third;
                    return createToken(VARIADIC, tk ,i + 2);
                }
                ungetChar(third);
                tk[1] = second;
                return createToken(BAD_TOKEN, tk, i + 1);
            }
            ungetChar(third);
            ungetChar(second);
            state = 4;
            c = getNextChar();
            if (!isdigit(c)) {
                ungetChar(c);
                return createToken(PERIOD, ".", 1);
            }
            ungetChar(c);
        } else state = 2;
        c = getNextChar();
        while (state > 0) {
            switch (state) {
                case 2:
                    if (isdigit(c)) state = 2;
                    else if (c == 'e' || c == 'E') state = 5;
                    else if (c == '.') state = 3;
                    else if(checkSuffix(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }

                    else if(!isalpha(c)){
                        state = -2;
                    }
                    else {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    break;
                case 3:
                    if (isdigit(c)) state = 8;
                    else if(checkSuffix(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else if(!isalpha(c)){
                        state = -3;
                    }
                    else {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    break;
                case 4:
                    if (isdigit(c)) state = 3;
                    else if(checkSuffix(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else state = -4;
                    break;
                case 5:
                    if (isdigit(c)) state = 6;
                    else if (c == '-' || c == '+') state = 7;
                    else if(checkSuffix(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else state = -5;
                    break;
                case 6:
                    if (isdigit(c)) state = 6;
                    else if(checkSuffix(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else state = -6;
                    break;

                case 7:
                    if (isdigit(c)) state = 6;
                    else if(checkSuffix(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else state = -7;
                    break;
                case 8:
                    if (isdigit(c)) state = 8;
                    else if (c == 'e' || c == 'E') state = 5;
                    else if(checkSuffix(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else if(!isalpha(c)){
                        state = -2;
                    }
                    else {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }

                    break;

                case 9 :
                    if(isdigit(c) || checkHex(c)) {
                        state = 9;
                    }
                    else if(checkSuffix(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    break;

                case 10:
                    if(!checkSuffix(c)) {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }

                    break;

                    int x;
                case 11:
                    x = c - '0';
                    if(x <= 7) {
                        state = 11;
                    }
                    else {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    break;
                    //seen one L
                case 12:
                    if(!checkSuffix(c) || c == 'l') {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    if(c == 'u' || c == 'U') {
                        state = 15;
                        break;
                    }
                    else {
                        state = 16;
                        break;
                    }
                    //seen one l
                case 13:
                    if(!checkSuffix(c) || c == 'L') {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    if(c == 'u' || c == 'U') {
                        //seen a u after l
                        state = 15;
                        break;
                    }
                    else {
                        state = 16;
                        break;
                    }
                    //seen a u or U before L
                case 14:
                    if(c == 'l') {
                        state = 17;
                    }
                    else if(c == 'L') {
                        state = 18;
                    }
                    else {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    break;
                case 15:
                    if(!isalpha(c)){
                        state = -2;
                    }
                    else {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    break;
                    //seen two L's or two l's
                case 16:
                    if(c == 'u' || c == 'U') {
                        state = 15;
                    }
                    else {
                        state = -15;
                    }
                    break;
                    //seen u/U then l
                case 17:
                    if(c == 'l') {
                        state = 15;
                    }
                    else {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    break;
                    //seen u/u then L
                case 18:
                    if(c == 'L') {
                        state = 15;
                    }
                    else {
                        tk[i] = c;
                        return createToken(BAD_TOKEN, tk, i + 1);
                    }
                    break;


            }
            if (state > 0) {
                tk[i++] = c;
                tk[i] = '\0';
                c = getNextChar();
            }
        }
        ungetChar(c);
        if (state == -3) ungetChar('.');
        switch (-state) {
            case 2:
                return createToken(INT, tk, i);
            case 3:
                return createToken(INT, tk, i - 1);
            case 6:
            case 8:
                return createToken(FLOAT, tk, i);
            case 4:
                break;
            case 15:
                return createToken(INT, tk, i);

            default:
                return createToken(BAD_TOKEN, tk, i);
        }
    }

    //ident starting with #
    if (c == '#') {
        i=0;
        c = getNextChar();
        while (isalpha(c) || isdigit(c)) {
            tk[i++] = c;
            c = getNextChar();
        }
        ungetChar(c);
        return createToken(IDENT, tk, i);
    }

    //identifier
    if (isalpha(c) || c == '_' || c == '$') {
        c = getNextChar();
        while (isdigit(c) || isalpha(c) || c == '_' || c == '$') {
            tk[i++] = c;
            c = getNextChar();
        }
        for(int x = 0; x < i; x++) {
            testKey[x] = tk[x];
        }
        TokenType type = checkKeyword(testKey);

        if(type == NOPE) {
            if(strcmp(tk, "cin") == 0) {
                type = CIN;
            }
            else if(strcmp(tk, "cout") == 0) {
                type = COUT;
            }
            else {
                type = IDENT;
            }
        }
        ungetChar(c);
        return createToken(type, tk, i);
    }

    //identifier starting with a @
    if ( c == '@') {
        while (isdigit(c = getNextChar()) || c == ';') {
            tk[i++] = c;
        }
        ungetChar(c);
        return createToken(IDENT,tk,i);
    }

    //OLDCOMMENT
    if (tk[0] == '/' && (c = getNextChar()) == '*') {
        tk[1] = '*';

        while ((c = getNextChar()) != '*' && (c = getNextChar()) != '/') {

        }
        return createToken(LONGCOMMENT, tk, i + 1);
    }
    if (tk[0] == '/') ungetChar(c);

    //LINECOMMENT
    if (tk[0] == '/' && (c = getNextChar()) == '/') {
        tk[1] = '/';

        while ((c = getNextChar()) != '\n'  && c != (char) -1) {

        }
        return createToken(LINECOMMENT, tk, i + 1);
    }
    if (tk[0] == '/') {
        ungetChar(c);
    }

    //CONSTSTRING
    if (tk[0] == '"') {
        _Bool badString = 0;
        while ((c = getNextChar()) != '"' || (tk[i - 1] == '\\' && tk[i - 2] != '\\')) {
            if(c == '"' || c == -1) {
                badString = 1;
            }
            if (c == (char) (-1)) {
                break;
            }
            tk[i++] = c;
        }
        if(c == '"') {
            badString = 0;
        }
        if(badString) {
            return createToken(BAD_TOKEN, tk, i);
        }
        char next = getNextChar();
        if(!isalpha(next)){
            ungetChar(next);
            tk[i++] = c;
            return createToken(STRING, tk, i);
        }
        else {
            tk[i] = c;
            tk[i+1] = next;
            return createToken(BAD_TOKEN, tk, i + 2);
        }
    }

    //CONSTCHAR

    if (tk[0] == '\'') {
        _Bool badChar = 0;
        while ((c = getNextChar()) != '\'' || (tk[i - 1] == '\\' && tk[i - 2] != '\\')) {
            if(c == '\'' || c == -1) {
                badChar = 0;
            }
            if (c == (char) (-1)) {
                break;
            }
            tk[i++] = c;
        }
        if(c == '\'') {
            badChar = 0;
        }
        if(badChar) {
            return createToken(BAD_TOKEN, tk, i);
        }
        char next = getNextChar();
        if(!isalpha(next)){
            ungetChar(next);
            tk[i++] = c;
            return createToken(CHAR, tk, i);
        }
        else {
            tk[i] = c;
            tk[i+1] = next;
            return createToken(BAD_TOKEN, tk, i + 2);
        }
    }

    //punctuation

    char first = tk[0];
    char second;
    char third;

//    tk[0] = first;

    switch(first) {

        case '(' :
            return createToken(OPEN_PARENT, tk, i);

        case ')' :
            return createToken(CLOSE_PARENT, tk, i);

        case '{' :
            return createToken(OPEN_BRACE, tk, i);

        case '}' :
            return createToken(CLOSE_BRACE, tk, i);

        case '[' :
            return createToken(OPEN_BRACKET, tk, i);

        case ']' :
            return createToken(CLOSE_BRACKET, tk, i);

        case ',' :
            return createToken(COMMA, tk, i);

        case ':' :
            second = getNextChar();
            if(second == '>') {
                tk[1] = second;
                return createToken(CLOSE_BRACKET, tk, i + 1);
            }
            ungetChar(second);
            return createToken(COLON, tk, i);

        case ';' :
            return createToken(SEMI_COLON, tk, i);

        case '+' :
            second = getNextChar();
            if(second == '+') {
                tk[1] = second;
                return createToken(INCREMENT, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                return createToken(ASSIGN_SUM, tk, i + 1);
            }
            ungetChar(second);
            return createToken(ADD_OP, tk, i);

        case '-' :
            second = getNextChar();
            if(second == '>') {
                tk[1] = second;
                return createToken(ARROW, tk, i + 1);
            }
            if(second == '-') {
                tk[1] = second;
                return createToken(DECREMENT, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                return createToken(ASSIGN_DIF, tk, i + 1);
            }
            ungetChar(second);
            return createToken(SUB_OP, tk, i);

        case '!' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return createToken(NOT_EQUAL, tk, i + 1);

            }
            ungetChar(second);
            return createToken(LOG_NOT, tk, i);

        case '~' :
            return createToken(BIT_NOT, tk, i);

        case '*' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return createToken(ASSIGN_PROD, tk, i + 1);
            }
            ungetChar(second);
            return createToken(MULT_OP, tk, i);

        case '/' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return createToken(ASSIGN_QUOT, tk, i + 1);
            }
            ungetChar(second);
            return createToken(DIV_OP, tk, i);

        case '%' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return createToken(ASSIGN_MOD, tk, i + 1);
            }
            if(second == '>') {
                tk[1] = second;
                return createToken(CLOSE_BRACE, tk, i + 1);
            }
            ungetChar(second);
            return createToken(MOD_OP, tk, i);

        case '<' :
            second = getNextChar();
            third = getNextChar();
            if(second == ':') {
                tk[1] = second;
                ungetChar(third);
                return createToken(OPEN_BRACKET, tk, i + 1);
            }
            if(second == '%') {
                tk[1] = second;
                ungetChar(third);
                return createToken(OPEN_BRACE, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                ungetChar(third);
                return createToken(LESS_THAN_EQUAL, tk, i + 1);
            }
            if(second == '<') {
                if(third == '=') {
                    tk[1] = second;
                    tk[2] = third;
                    return createToken(ASSIGN_LEFT, tk, i + 2);
                }
                ungetChar(third);
                tk[1] = second;
                return createToken(BIT_LEFT, tk, i + 1);
            }
            ungetChar(third);
            ungetChar(second);
            return createToken(LESS_THAN, tk, i);

        case '>' :
            second = getNextChar();
            third = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return createToken(GREATER_THAN_EQUAL, tk, i + 1);
            }
            if(second == '>') {
                if(third == '=') {
                    tk[1] = second;
                    tk[2] = third;
                    return createToken(ASSIGN_RIGHT, tk, i + 2);
                }
                ungetChar(third);
                tk[1] = second;
                return createToken(BIT_RIGHT, tk, i + 1);
            }
            ungetChar(third);
            ungetChar(second);
            return createToken(GREATER_THAN, tk, i);

        case '&' :
            second = getNextChar();
            if(second == '&') {
                tk[1] = second;
                return createToken(LOG_AND, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                return createToken(ASSIGN_AND, tk, i + 1);
            }
            ungetChar(second);
            return createToken(BIT_AND, tk, i);

        case '^' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return createToken(ASSIGN_XOR, tk, i + 1);
            }
            ungetChar(second);
            return createToken(BIT_XOR, tk, i);

        case '|' :
            second = getNextChar();
            if(second == '|') {
                tk[1] = second;
                return createToken(LOG_OR, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                return createToken(ASSIGN_OR, tk, i + 1);
            }
            ungetChar(second);
            return createToken(BIT_OR, tk, i);

        case '?' :
            return createToken(TERNARY, tk, i);

        case '=' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return createToken(EQUAL, tk, i + 1);
            }
            ungetChar(second);
            return createToken(ASSIGN, tk, i);

        default:
            break;
    }

    return createToken(BAD_TOKEN, tk, i);
}


_Bool checkHex(char c) {
    int arraySize = sizeof(hexSet) / sizeof(char);
    for(int i = 0; i < arraySize; i++) {
        if(hexSet[i] == c) {
            return 1;
        }
    }
    return 0;
}

_Bool checkSuffix(char c) {
    int arraySize = sizeof(suffixSet) / sizeof(char);
    for(int i = 0; i < arraySize; i++) {
        if(suffixSet[i] == c) {
            return 1;
        }
    }
    return 0;
}

TokenType checkKeyword(char* lex) {
    int arraySize = sizeof(keywordStrings) / sizeof(keywordStrings[0]);
    TokenType t;
    int index;
    for(int i = 0; i < arraySize; i++) {
        if(strcmp(keywordStrings[i], lex) == 0) {
            index = i;
            t = keywords[index];
            return t;
        }
    }
    t = keywords[arraySize];
    return t;
}

char getNextChar() {
    char c = pop(stack);
    if(c == -1) {
        return readChar();
    }
    else {
        return c;
    }
}

char readChar() {
    int a;
    if((a = fgetc(file)) != EOF) {
        char c = (char) a;
        return c;
    }
    return -1;
}

void ungetChar(char c) {
    push(stack, c);
}