#include "parser.c"
#include "variables.h"

void interpret(Node* root);
void exLOfStatements(Node* lOfStatements);
void exStatement(Node* root);
void exCin(Node* cin);
void exCout(Node* cout);
void exLOfExp(Node* lOfExp);
int exExp(Node* exp);
int exLogAnd(Node* logAnd);
int exBor(Node* bor);
int exBand(Node* band);
int exBop(Node* bop);
int exComp(Node* comp);
int exAdd(Node* add);
int exMult(Node* mult);
int exUop(Node* uop);
int exPrim(Node* prim);
int exVar(Node* var);
void exIf(Node* ifNode);
void exWhile(Node* whileNode);
void exAssign(Node* assign);
void printVarTable();


Variable* varTable[10000] = {NULL};

int main(int argc, char *argv[]) {
    char *path;
    if (argc > 1) {
        path = argv[1];
    } else {
        path = "../TestFile.c";
    }

    Node* root = parse(path);

    interpret(root);

    printVarTable();

    return 0;
}

void interpret(Node* root) {
    Node* node = root->first_child;
    printf("%s", nodeToString(node));
    while(strcmp(node->data.rule, "listofstatements") != 0) {
        node = node->next_sib;
        if(node == NULL) {
            printf("%s", "No statements.");
            return;
        }
    }
    printf("%s", nodeToString(node));
    exLOfStatements(node);

}

void exLOfStatements(Node* lOfStatements) {
    //get to statement
    Node* current = lOfStatements->first_child;
    if(current != NULL) {
        exStatement(current);
    }
    else {
        return;
    }
    while(current->next_sib != NULL) {
        current = current->next_sib;
        exStatement(current);
    }
}

void exStatement(Node* root) {
    Node* current = root->first_child;
    if (current->type == 1) {
        char *ruleStr = current->data.rule;
        if (strcmp(ruleStr, "cin") == 0) {
            exCin(current);
        } else if (strcmp(ruleStr, "cout") == 0) {
            exCout(current);
        } else if (strcmp(ruleStr, "if") == 0) {
            exIf(current);
        } else if (strcmp(ruleStr, "while") == 0) {
            exWhile(current);
        } else if (strcmp(ruleStr, "assign") == 0) {
            exAssign(current);
        }
    }
    else {
        if(current->data.token->type == OPEN_BRACE) {
            //get to lOfStatements
            current = current->next_sib;
            exLOfStatements(current);
        }
        else if(current->data.token->type == RETURN) {
            return;
        }
    }
}

void exCin(Node* cin) {
    //gets to listofvariables1
    Node* current = cin->first_child->next_sib->next_sib;
    //gets to variable
    current = current->first_child;
    _Bool moreVars = 1;
    while(moreVars) {
        Node* firstVar = current->first_child;
        char* name = firstVar->data.token->lexeme;
        int readNum;
        int index;
        Variable* var = NULL;
        if(scanf("%d", &readNum) == 1) {
            index = checkIndex(INT, name);
            if(index == -1) {
                printf("Wrong type read in.\n");
                return;
            }
            var = createNumVar(name, readNum);
            //printf("%d\n", readNum);
        }
        else {
            char readC;
            scanf("%c", &readC);
            index = checkIndex(CHAR, name);
            if(index == -1) {
                printf("Wrong type read in.\n");
                return;
            }
            var = createCVar(name, readC);
        }
        varTable[index] = var;

        if(current->next_sib == NULL) {
            moreVars = 0;
        }
        else {
            current = current->next_sib->next_sib;
        }
        //printf("%s", varToString(varTable[index]));
    }
}

void exCout(Node* cout) {
    //get to listofexpressions
    Node* current = cout->first_child->next_sib->next_sib;
    exLOfExp(current);
}

void exLOfExp(Node* lOfExp) {
    //get to exp
    Node* current = lOfExp->first_child;
    _Bool moreExps = 1;
    while(moreExps) {
        int toPrint = exExp(current);
        if(current->next_sib == NULL) {
            moreExps = 0;
        }
        else {
            current = current->next_sib->next_sib;
        }
        printf("%d\n", toPrint);
    }
}

int exExp(Node* exp) {
    //get to logand
    Node* current = exp->first_child;
    int result;
    if(current->next_sib != NULL) {
        int result1 = exLogAnd(current);
        int result2 = exLogAnd(current->next_sib->next_sib);
        if(result1 != 0 || result2 != 0) {
            result = 1;
        }
        else {
            result = 0;
        }
    }
    else {
        result = exLogAnd(current);
    }
    return result;
}

int exLogAnd(Node* logAnd) {
    //get to bor
    Node* current = logAnd->first_child;
    int result;
    if(current->next_sib != NULL) {
        int result1 = exBor(current);
        int result2 = exBor(current->next_sib->next_sib);
        if(result1 != 0 && result2 != 0) {
            result = 1;
        }
        else {
            result = 0;
        }
    }
    else {
        result = exBor(current);
    }
    return result;
}

int exBor(Node* bor) {
    //get to band
    Node* current = bor->first_child;
    int result;
    if(current->next_sib != NULL) {
        int result1 = exBand(current);
        int result2 = exBand(current->next_sib->next_sib);
        result = (result1 | result2);
    }
    else {
        result = exBand(current);
    }
    return result;
}

int exBand(Node* band) {
    //get to bop
    Node* current = band->first_child;
    int result;
    if(current->next_sib != NULL) {
        int result1 = exBop(current);
        int result2 = exBop(current->next_sib->next_sib);
        result = (result1 & result2);
    }
    else {
        result = exBop(current);
    }
    return result;
}

int exBop(Node* bop) {
    //get to comp
    Node* current = bop->first_child;
    int result;
    if(current->next_sib != NULL) {
        int result1 = exComp(current);
        int result2 = exComp(current->next_sib->next_sib);
        if(current->next_sib->data.token->type == NOT_EQUAL) {
            if(result1 != result2) {
                result = 1;
            }
            else {
                result = 0;
            }
        }
        else {
            if(result1 == result2) {
                result = 1;
            }
            else {
                result = 0;
            }
        }
    }
    else {
        result = exComp(current);
    }
    return result;
}

int exComp(Node* comp) {
    //get to add
    Node* current = comp->first_child;
    int result = -1;
    if(current->next_sib != NULL) {
        int result1 = exAdd(current);
        int result2 = exAdd(current->next_sib->next_sib);
        if(current->next_sib->data.token->type == GREATER_THAN) {
            if(result1 > result2) {
                result = 1;
            }
            else {
                result = 0;
            }
        }
        else if(current->next_sib->data.token->type == GREATER_THAN_EQUAL){
            if(result1 >= result2) {
                result = 1;
            }
            else {
                result = 0;
            }
        }
        else if(current->next_sib->data.token->type == LESS_THAN){
            if(result1 < result2) {
                result = 1;
            }
            else {
                result = 0;
            }
        }
        else if(current->next_sib->data.token->type == LESS_THAN_EQUAL){
            if(result1 <= result2) {
                result = 1;
            }
            else {
                result = 0;
            }
        }
    }
    else {
        result = exAdd(current);
    }
    return result;
}

int exAdd(Node* add) {
    //get to mult
    Node* current = add->first_child;
    int result;
    if(current->next_sib != NULL) {
        int result1 = exMult(current);
        int result2 = exMult(current->next_sib->next_sib);
        if(current->next_sib->data.token->type == SUB_OP) {
            result = result1 - result2;
        }
        else {
            result = result1 + result2;
        }
        current = current->next_sib->next_sib;
        while(current->next_sib != NULL) {
            int resultx = exMult(current->next_sib->next_sib);
            if(current->next_sib->data.token->type == SUB_OP) {
                result = result - resultx;
            }
            else {
                result = result + resultx;
            }
            current = current->next_sib->next_sib;
        }
    }
    else {
        result = exMult(current);
    }
    return result;
}

int exMult(Node* mult) {
    //get to uop
    Node* current = mult->first_child;
    int result;
    if(current->next_sib != NULL) {
        int result1 = exUop(current);
        int result2 = exUop(current->next_sib->next_sib);
        if(current->next_sib->data.token->type == DIV_OP) {
            result = result1 / result2;
        }
        else if(current->next_sib->data.token->type == MULT_OP){
            result = result1 * result2;
        }
        else {
            result = result1 % result2;
        }
    }
    else {
        result = exUop(current);
    }
    return result;
}

int exUop(Node* uop) {
    //get to next
    Node* current = uop->first_child;
    int result = -1;
    if(strcmp(current->data.rule, "prim") == 0) {
        result = exPrim(current);
    }
    else {
        int result1 = exUop(current->next_sib);
        if(current->data.token->type == LOG_NOT) {
            result = !result1;
        }
        else if(current->data.token->type == SUB_OP) {
            result = (-result1);
        }
        else if(current->data.token->type == BIT_NOT) {
            result = (~result1);
        }
    }
    return result;
}

int exPrim(Node* prim) {
    //get to next
    Node* current = prim->first_child;
    int result;
    if(current->data.token->type == INT) {
        int a = atoi(current->data.token->lexeme);
        result = a;
    }
    else if(current->data.token->type == CHAR) {
        char c = current->data.token->lexeme[0];
        result = c;
    }
    else if(current->data.token->type == OPEN_PARENT) {
        result = exExp(current->next_sib);
    }
    else {
        result = exVar(current);
    }
    return result;
}

int exVar(Node* var) {
    //get to IDENT
    Node* current = var->first_child;
    int result = -1;
    //1 for num, 2 for char
    int numOrChar = 1;
    int index;
    char* name;
    name = current->data.token->lexeme;
    Variable* variable = NULL;
    index = checkIndex(INT, name);
    if(index == -1) {
        index = checkIndex(CHAR, name);
        numOrChar = 2;
    }
    if(index == -1) {
        printf("%s\n", "Variable doesn't exist.");
    }
    variable = varTable[index];
    if(variable == NULL) {
        printf("%s", "Variable has not been initialized.\n");
    }
    else {
        if(numOrChar == 1) {
            result = variable->value.num;
        }
        else {
            result = variable->value.c;
        }
    }
    return result;
}

void exIf(Node* ifNode) {
    //get to exp
    Node* current = ifNode->first_child->next_sib->next_sib;
    int expResult = exExp(current);
    if(expResult == 0) {
        //get to else if there
        current = current->next_sib->next_sib->next_sib;
        if(current != NULL) {
            //get to else statement
            current = current->next_sib;
            exStatement(current);
        }
        else {
            return;
        }
    }
    else {
        //get to statement in if
        current = current->next_sib->next_sib;
        exStatement(current);
    }
}

void exWhile(Node* whileNode) {
    //get to exp
    Node* current = whileNode->first_child->next_sib->next_sib;
    int expResult = exExp(current);
    if(expResult == 0) {
        return;
    }
    else {
        //get to statement
        current = current->next_sib->next_sib;
        exStatement(current);
    }
}

void exAssign(Node* assign) {
    //get to lofVariables2
    Node* lOfVar2 = assign->first_child;
    //get to exp
    Node* exp = lOfVar2->next_sib->next_sib;
    int expResult = exExp(exp);
    //gets to variable
    Node* var = lOfVar2->first_child;
    _Bool moreVars = 1;
    while(moreVars) {
        Node* firstVar = var->first_child;
        char* name = firstVar->data.token->lexeme;
        int index;
        Variable* variable = NULL;
        index = checkIndex(INT, name);
        if(index == -1) {
            index = checkIndex(CHAR, name);
            variable = createCVar(name, (char) expResult);
        }
        else {
            variable = createNumVar(name, expResult);
        }
        varTable[index] = variable;

        if(var->next_sib == NULL) {
            moreVars = 0;
        }
        else {
            var = var->next_sib->next_sib;
        }
        //printf("%s", varToString(varTable[index]));
    }
}

void printVarTable() {
    int i = 0;
    while(varTable[i] != NULL) {
        printf("%s", varToString(varTable[i++]));
    }
}