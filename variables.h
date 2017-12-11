//
// Created by Marty Spiewak on 12/9/17.
//

#include <stdlib.h>

#ifndef INTERPRETER_MARTY_SPIEWAK_VARIABLES_H
#define INTERPRETER_MARTY_SPIEWAK_VARIABLES_H

#endif //INTERPRETER_MARTY_SPIEWAK_VARIABLES_H

struct Variable {
    //1 is num, 2 is char
    int type;
    char* name;
    union {
        int num;
        char c;
    } value;
};

typedef struct Variable Variable;

Variable* createNumVar(char* vName, int vNum) {
    Variable* var = malloc(sizeof(Variable));
    char* nameCopy;
    nameCopy = malloc(sizeof(char) * strlen(vName));
    strcpy(nameCopy, vName);
    var->name = nameCopy;
    var->value.num = vNum;
    var->type = 1;
    return var;
}

Variable* createCVar(char* vName, char vC) {
    Variable* var = malloc(sizeof(Variable));
    char* nameCopy;
    nameCopy = malloc(sizeof(char) * strlen(vName));
    strcpy(nameCopy, vName);
    var->name = nameCopy;
    var->value.c = vC;
    var->type = 2;
    return var;
}

char* varToString(Variable* v) {
    char* string = (char*) malloc(sizeof(char) * 50);

    strcpy(string, "Name: ");
    strcat(string, v->name);
    strcat(string, ", Value: ");
    if(v->type == 1) {
        sprintf(string, "%s %d",string, v->value.num);
    }
    else {
        sprintf(string, "%s %c",string, v->value.c);
    }
    strcat(string, ".\n");

    return string;
}

