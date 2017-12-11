#include "tokenizer.c"
#include "firsts.h"

Token* nextT;
Node* root;

void program();
void next();
void declaration(Node* prev);
void listOfStatements(Node* prev);
TokenType type(Node* prev);
void variable(Node* prev);
void statement(Node* prev);
void listOfVariables2(Node* prev);
void listOfVariables1(Node* prev);
void cin(Node* prev);
void cout(Node* prev);
void listOfExpressions(Node* prev);
void exp(Node* prev);
void logAnd(Node* prev);
void bOr(Node* prev);
void bAnd(Node* prev);
void bOp(Node* prev);
void comp(Node* prev);
void add(Node* prev);
void mult(Node* prev);
void uOp(Node* prev);
void prim(Node* prev);
void ifMeth(Node* prev);
void assign(Node* prev);
void whileMeth(Node* prev);
void listOfVariables2Dec(Node* prev, TokenType symType);
void variableDec(Node* prev, TokenType symType);



Node* parse(char* path) {

    loadFile(path);

    //tokenizerTest();

    root = createSNode("program");
    program();
    //printTree(root);

    return root;
}

void program() {
    next();

    while((inFirst(fDec, *nextT) == 1)) {
        declaration(root);
    }
    while((inFirst(fLofStatements, *nextT) == 1)) {
        listOfStatements(root);
    }
}

void declaration(Node* prev) {
    Node* n = createSNode("declaration");
    setChild(prev, n);

    TokenType symType = type(n);

    listOfVariables2Dec(n, symType);

    if(nextT->type == SEMI_COLON) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
    }
    else {
        Node* t = createSNode("Error in declaration");
        setChild(n ,t);
        next();
    }

}

void listOfVariables2Dec(Node* prev, TokenType symType) {
    Node* n = createSNode("listOfVariables2");
    setChild(prev, n);
    variableDec(n, symType);
    while (nextT->type == COMMA) {
        Node *t = createTkNode(nextT);
        setChild(n, t);
        next();
        variableDec(n, symType);
    }
}

void variableDec(Node* prev, TokenType symType) {
    Node* n = createSNode("variable");
    setChild(prev, n);
    if(inFirst(fVariable, *nextT) == 1) {
        if(tryAdd(symType, nextT->lexeme)) {
            Node *t = createTkNode(nextT);
            setChild(n, t);
            next();
        }
        else {
            Node *t = createSNode("Error symbol already exists");
            setChild(n, t);
            next();
        }
    }
}

void listOfStatements(Node* prev) {
    Node* n = createSNode("listofstatements");
    setChild(prev, n);
    while(inFirst(fStatement, *nextT) == 1) {
        statement(n);
    }
}

TokenType type(Node* prev) {
    Node* n = createSNode("type");
    setChild(prev, n);
    if(inFirst(fType, *nextT) == 1) {
        TokenType symType = nextT->type;
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        return symType;
    }
    else {
        Node* t = createSNode("Error in type");
        setChild(n ,t);
        next();
    }
}

void variable(Node* prev) {
    Node* n = createSNode("variable");
    setChild(prev, n);
    if(inFirst(fVariable, *nextT) == 1) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
    }
}

void statement(Node* prev) {
    Node* n = createSNode("statement");
    setChild(prev, n);
    if(nextT->type == OPEN_BRACE) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        listOfStatements(n);
        if(nextT->type == CLOSE_BRACE) {
            Node *s = createTkNode(nextT);
            setChild(n, s);
            next();
        }
    }
    else if(nextT->type == RETURN) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
    }
    else if(inFirst(fCin, *nextT) == 1) {
        cin(n);
    }
    else if(inFirst(fCout, *nextT) == 1) {
        cout(n);
    }
    else if(inFirst(fIf, *nextT) == 1) {
        ifMeth(n);
    }
    else if(inFirst(fAssign, *nextT) == 1) {
        assign(n);
    }
    else if(inFirst(fWhile, *nextT) == 1) {
        whileMeth(n);
    }
}

void listOfVariables2(Node* prev) {
    Node* n = createSNode("listOfVariables2");
    setChild(prev, n);
    variable(n);
    while (nextT->type == COMMA) {
        Node *t = createTkNode(nextT);
        setChild(n, t);
        next();
        variable(n);
    }
}

void listOfVariables1(Node* prev) {
    Node* n = createSNode("listOfVariables1");
    setChild(prev, n);
    if(inFirst(fVariable, *nextT) == 1) {
        variable(n);
    }
    while(nextT->type == BIT_RIGHT) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        if(inFirst(fVariable, *nextT) == 1) {
            variable(n);
        }
    }
}

void cin(Node* prev) {
    Node* n = createSNode("cin");
    setChild(prev, n);
    Node* cinNode = createTkNode(nextT);
    setChild(n, cinNode);
    next();
    if(nextT->type == BIT_RIGHT) {
        Node* bRNode = createTkNode(nextT);
        setChild(n, bRNode);
        next();
    }
    else {
        Node* error = createSNode("Error in CIN");
        setChild(n, error);
        next();
    }
    listOfVariables1(n);
    if(nextT->type == SEMI_COLON) {
        Node* sem = createTkNode(nextT);
        setChild(n, sem);
        next();
    }
}

void cout(Node* prev) {
    Node* n = createSNode("cout");
    setChild(prev, n);
    Node* coutNode = createTkNode(nextT);
    setChild(n, coutNode);
    next();
    if(nextT->type == BIT_LEFT) {
        Node* bLNode = createTkNode(nextT);
        setChild(n, bLNode);
        next();
    }
    listOfExpressions(n);
    if(nextT->type == SEMI_COLON) {
        Node* sem = createTkNode(nextT);
        setChild(n, sem);
        next();
    }
}

void listOfExpressions(Node* prev) {
    Node* n = createSNode("listOfExpressions");
    setChild(prev, n);
    exp(n);
    while(nextT->type == BIT_LEFT) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        exp(n);
    }
}

void exp(Node* prev) {
    Node* n = createSNode("exp");
    setChild(prev, n);
    logAnd(n);
    while(nextT->type == LOG_OR) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        logAnd(n);
    }
}

void logAnd(Node* prev) {
    Node* n = createSNode("logand");
    setChild(prev, n);
    bOr(n);
    while(nextT->type == LOG_AND) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        bOr(n);
    }
}

void bOr(Node* prev) {
    Node* n = createSNode("bor");
    setChild(prev, n);
    bAnd(n);
    while(nextT->type == BIT_OR) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        bAnd(n);
    }
}

void bAnd(Node* prev) {
    Node* n = createSNode("band");
    setChild(prev, n);
    bOp(n);
    while(nextT->type == BIT_AND) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        bOp(n);
    }
}

void bOp(Node* prev) {
    Node* n = createSNode("bop");
    setChild(prev, n);
    comp(n);
    while(nextT->type == EQUAL || nextT->type == NOT_EQUAL) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        comp(n);
    }
}

void comp(Node* prev) {
    Node* n = createSNode("comp");
    setChild(prev, n);
    add(n);
    while(nextT->type == GREATER_THAN || nextT->type == GREATER_THAN_EQUAL ||
            nextT->type == LESS_THAN || nextT->type == LESS_THAN_EQUAL) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        add(n);
    }
}

void add(Node* prev) {
    Node* n = createSNode("add");
    setChild(prev, n);
    mult(n);
    while(nextT->type == ADD_OP || nextT->type == SUB_OP) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        mult(n);
    }
}

void mult(Node* prev) {
    Node* n = createSNode("mult");
    setChild(prev, n);
    inFirst(fMult, *nextT);
    uOp(n);

    while(nextT->type == DIV_OP || nextT->type == MULT_OP ||
            nextT->type == MOD_OP) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        uOp(n);
    }
}

void uOp(Node* prev) {
    Node* n = createSNode("uop");
    setChild(prev, n);
    if(nextT->type == BIT_NOT || nextT->type == SUB_OP ||
            nextT->type == LOG_NOT) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        uOp(n);
    }
    prim(n);
}

void prim(Node* prev) {
    Node* n = createSNode("prim");
    setChild(prev, n);
    if(nextT->type == INT || nextT->type == CHAR) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
    }
    else if(inFirst(fVariable, *nextT) == 1) {
        variable(n);
    }
    else if(nextT->type == OPEN_PARENT) {
        Node* t = createTkNode(nextT);
        setChild(n, t);
        next();
        exp(n);
        if(nextT->type == CLOSE_PARENT) {
            Node* s = createTkNode(nextT);
            setChild(n, s);
            next();
        }
        else {
            Node* e = createSNode("Error no close parent");
            setChild(n, e);
            next();
        }
    }
}

void ifMeth(Node* prev) {
    Node* n = createSNode("if");
    setChild(prev, n);
    Node* ifNod = createTkNode(nextT);
    setChild(n, ifNod);
    next();
    Node* opPar = createTkNode(nextT);
    setChild(n, opPar);
    next();
    exp(n);
    Node* clPar = createTkNode(nextT);
    setChild(n, clPar);
    statement(n);
    if(nextT->type == ELSE) {
        Node* elNod = createTkNode(nextT);
        setChild(n, elNod);
        next();
        statement(n);
    }
}

void assign(Node* prev) {
    Node* n = createSNode("assign");
    setChild(prev, n);
    listOfVariables2(n);
    Node* eq = createTkNode(nextT);
    setChild(n, eq);
    next();
    exp(n);
    Node* sem = createTkNode(nextT);
    setChild(n, sem);
    next();
}

void whileMeth(Node* prev) {
    Node* n = createSNode("while");
    setChild(prev, n);
    Node* wNode = createTkNode(nextT);
    setChild(n, wNode);
    next();
    Node* opPar = createTkNode(nextT);
    setChild(n, opPar);
    next();
    exp(n);
    Node* clPar = createTkNode(nextT);
    setChild(n, clPar);
    next();
    statement(n);
}


void next() {
    nextT = nextToken();
}
