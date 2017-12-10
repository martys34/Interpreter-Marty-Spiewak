//
// Created by Marty on 11/15/2017.
//

#ifndef LEXICAL_ANALYZER_C_FIRSTS_H
#define LEXICAL_ANALYZER_C_FIRSTS_H

#endif //LEXICAL_ANALYZER_C_FIRSTS_H

_Bool inFirst(char* firsts[], Token token);

char* fWhile[] = {"1", "while"};
char* fLofVar2[] = {"1", "IDENT"};
char* fAssign[] = {"1", "IDENT"};
char* fPrim[] = {"4", "INT", "CONSTCH", "IDENT", "("};
char* fUop[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fMult[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fAdd[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fComp[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fBop[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fBand[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fBor[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fLogand[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fExp[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fIf[] = {"1", "if"};
char* fLofExp[] = {"7", "!", "-", "~", "INT", "CONSTCH", "IDENT", "("};
char* fCout[] = {"1", "cout"};
char* fLofVar1[] = {"1", "IDENT"};
char* fCin[] = {"1", "cin"};
char* fStatement[] = {"7", "cin", "cout", "if", "IDENT", "while", "{", "return"};
char* fType[] = {"2", "int", "char"};
char* fVariable[] = {"1", "IDENT"};
char* fDec[] = {"2", "int", "char"};
char* fLofStatements[] = {"8", "cin", "cout", "if", "IDENT", "while", "{", "return", ""};
char* fProgram[] = {"10", "int", "char", "cin", "cout", "if", "IDENT", "while", "{", "return", ""};


_Bool inFirst(char* firsts[], Token token) {
    int arrSize = atoi(firsts[0]) + 1;
    for(int i = 1; i < arrSize; i++) {
        if((strcmp(firsts[i], checkType(token.type)) == 0) || (strcmp(firsts[i], token.lexeme) == 0)) {
            return 1;
        }
    }
    return 0;
}

struct Node {
    //1 is inner, 2 is leaf
    int type;
    union {
        Token* token;
        char* rule;
    } data;
    struct Node* first_child; //Points to the first node on the next level down
    struct Node* next_sib; //Points to the next node on the same level
};

typedef struct Node Node;

Node* createTkNode(Token* tk) {
    Node* node = malloc(sizeof(Node));
    char* lexCopy;
    lexCopy = malloc(sizeof(char) * strlen(tk->lexeme));
    strcpy(lexCopy, tk->lexeme);
    Token* newTk = createToken(tk->type, lexCopy, 0);
    node->type = 2;
    node->data.token = newTk;
    node->first_child = NULL;
    node->next_sib = NULL;
    return node;
}

Node* createSNode(char* rule) {
    Node* node = malloc(sizeof(Node));
    node->type = 1;
    node->data.rule = rule;
    node->first_child = NULL;
    node->next_sib = NULL;
    return node;
}

void setChild(Node* node, Node* child) {
    if(node->first_child == NULL) {
        node->first_child = child;
    }
    else {
        Node* nextChild = node->first_child;
        while(nextChild->next_sib != NULL) {
            nextChild = nextChild->next_sib;
        }
        nextChild->next_sib = child;
    }
}

char* nodeToString(Node* node) {
    char* string = (char*) malloc(sizeof(char) * 50);
    if(node->type == 1) {
        strcpy(string, "Rule: ");
        strcat(string, node->data.rule);
        strcat(string, "\n");
    }
    else {
        strcpy(string, tokenToString(node->data.token));
    }
    return string;
}

void printTree(Node* root) {
    printf("%s", nodeToString(root));
    if(root->first_child != NULL) {
        printTree(root->first_child);
    }
    if(root->next_sib != NULL) {
        printTree(root->next_sib);
    }
}

int offset = 0;
int counter = 0;

typedef struct Symbol Symbol;

struct Symbol {
    TokenType type;
    char* name;
    int offSet;
};


Symbol* symTable[10000];

void addSymbol(TokenType tType, char* tName) {
    Symbol* s = malloc(sizeof(Symbol));
    char* nameCopy;
    nameCopy = malloc(sizeof(char) * strlen(tName));
    strcpy(nameCopy, tName);
    s->name = nameCopy;
    s->type = tType;
    s->offSet = offset++;

    symTable[counter++] = s;


}

_Bool tryAdd(TokenType tType, char* tName) {
    for(int i = 0; i < counter; i++) {
        Symbol* s = symTable[i];
        if(s->type == tType && strcmp(s->name, tName) == 0) {
            return 0;
        }
    }
    addSymbol(tType, tName);
    return 1;
}

int checkIndex(TokenType tType, char* tName) {
    for(int i = 0; i < counter; i++) {
        Symbol* s = symTable[i];
        if(s->type == tType && strcmp(s->name, tName) == 0) {
            return s->offSet;
        }
    }
    return -1;
}