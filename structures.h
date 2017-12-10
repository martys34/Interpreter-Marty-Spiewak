//
// Created by Marty on 11/8/2017.
//

//#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#ifndef LEXICAL_ANALYZER_C_STRUCTURES_C_H
#define LEXICAL_ANALYZER_C_STRUCTURES_C_H

#endif //LEXICAL_ANALYZER_C_STRUCTURES_C_H

enum TokenType {
    AUTO,
    DOUBLE,
    INT,
    STRUCT,
    CONST,
    FLOAT,
    SHORT,
    UNSIGNED,
    BREAK,
    ELSE,
    LONG,
    SWITCH,
    CONTINUE,
    FOR,
    SIGNED,
    VOID,
    CASE,
    ENUM,
    REGISTER,
    TYPEDEF,
    DEFAULT,
    GOTO,
    SIZEOF,
    VOLATILE,
    CHAR,
    STRING,
    EXTERN,
    RETURN,
    UNION,
    DO,
    IF,
    STATIC,
    WHILE,
    IDENT,
    VARIADIC,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_PARENT,
    CLOSE_PARENT,
    OPEN_BRACE,
    CLOSE_BRACE,
    ADD_OP,
    SUB_OP,
    MULT_OP,
    DIV_OP,
    MOD_OP,
    INCREMENT,
    DECREMENT,
    PERIOD,
    ARROW,
    LOG_NOT,
    BIT_NOT,
    ALIGNOF,
    BIT_LEFT,
    BIT_RIGHT,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_EQUAL,
    GREATER_THAN_EQUAL,
    EQUAL,
    NOT_EQUAL,
    BIT_AND,
    BIT_XOR,
    BIT_OR,
    LOG_AND,
    LOG_OR,
    TERNARY,
    ASSIGN,
    ASSIGN_SUM,
    ASSIGN_DIF,
    ASSIGN_PROD,
    ASSIGN_QUOT,
    ASSIGN_MOD,
    ASSIGN_LEFT,
    ASSIGN_RIGHT,
    ASSIGN_AND,
    ASSIGN_OR,
    ASSIGN_XOR,
    COMMA,
    COLON,
    SEMI_COLON,
    END,
    LONGCOMMENT,
    LINECOMMENT,
    BAD_TOKEN,
    NOPE,
    CIN,
    COUT
};

typedef enum TokenType TokenType;

char* enumString[] = {
        "AUTO",
        "DOUBLE",
        "INT",
        "STRUCT",
        "CONST",
        "FLOAT",
        "SHORT",
        "UNSIGNED",
        "BREAK",
        "ELSE",
        "LONG",
        "SWITCH",
        "CONTINUE",
        "FOR",
        "SIGNED",
        "VOID",
        "CASE",
        "ENUM",
        "REGISTER",
        "TYPEDEF",
        "DEFAULT",
        "GOTO",
        "SIZEOF",
        "VOLATILE",
        "CHAR",
        "STRING",
        "EXTERN",
        "RETURN",
        "UNION",
        "DO",
        "IF",
        "STATIC",
        "WHILE",
        "IDENT",
        "VARIADIC",
        "OPEN_BRACKET",
        "CLOSE_BRACKET",
        "OPEN_PARENT",
        "CLOSE_PARENT",
        "OPEN_BRACE",
        "CLOSE_BRACE",
        "ADD_OP",
        "SUB_OP",
        "MULT_OP",
        "DIV_OP",
        "MOD_OP",
        "INCREMENT",
        "DECREMENT",
        "PERIOD",
        "ARROW",
        "LOG_NOT",
        "BIT_NOT",
        "ALIGNOF",
        "BIT_LEFT",
        "BIT_RIGHT",
        "LESS_THAN",
        "GREATER_THAN",
        "LESS_THAN_EQUAL",
        "GREATER_THAN_EQUAL",
        "EQUAL",
        "NOT_EQUAL",
        "BIT_AND",
        "BIT_XOR",
        "BIT_OR",
        "LOG_AND",
        "LOG_OR",
        "TERNARY",
        "ASSIGN",
        "ASSIGN_SUM",
        "ASSIGN_DIF",
        "ASSIGN_PROD",
        "ASSIGN_QUOT",
        "ASSIGN_MOD",
        "ASSIGN_LEFT",
        "ASSIGN_RIGHT",
        "ASSIGN_AND",
        "ASSIGN_OR",
        "ASSIGN_XOR",
        "COMMA",
        "COLON",
        "SEMI_COLON",
        "END",
        "LONGCOMMENT",
        "LINECOMMENT",
        "BAD_TOKEN",
        "NOPE",
        "CIN",
        "COUT"
};

struct Token{
    char* lexeme;
    TokenType type;
};

typedef struct Token Token;

char* tokenToString(Token* token);

Token* createToken(TokenType t, char* lex, int end){
    Token* token = malloc(sizeof(Token));
    char* lexCopy;
    lexCopy = malloc(sizeof(char) * strlen(lex));
    strcpy(lexCopy, lex);
    token->lexeme = lexCopy;
    token->type = t;
    return token;
}

char* tokenToString(Token* token){
    char* string = (char*) malloc(sizeof(char) * 50);
    strcpy(string, "Token: ");
    strcat(string, enumString[token->type]);
    strcat(string, ", Lexeme: ");
    strcat(string, token->lexeme);
    strcat(string, "\n");
    return string;}

struct Stack{
    int top;
    char lexeme[10000];
};

typedef struct Stack Stack;

Stack* createStack() {
    Stack* s = malloc(sizeof(Stack));
    s->top = -1;
    return s;
}

void push(Stack* s, char c) {
    s->top++;
    s->lexeme[s->top] = c;
}

char pop(Stack* s) {
    if(s->top == -1) {
        return -1;
    }
    char c = s->lexeme[s->top--];
    return c;
}

char* checkType(TokenType tkType) {
    char* type = enumString[tkType];
    return type;
}