#include "SubsidiaryFunctionsAssembler.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "AssemblerEnums.h"

int CommandToEnum(const char *command) {
    assert(command);

    if (strncmp(command, PUSHR, sizeof(PUSHR) - 1) == 0) return kPushR;
    if (strncmp(command, PUSHM, sizeof(PUSHM) - 1) == 0) return kPushM;
    if (strncmp(command, POPR,  sizeof(POPR) - 1)  == 0) return kPopR;
    if (strncmp(command, POPM,  sizeof(POPM) - 1)  == 0) return kPopM;

    if (strncmp(command, PUSH,  sizeof(PUSH) - 1)  == 0) return kPush;
    if (strncmp(command, POP,   sizeof(POP) - 1)   == 0) return kPop;
    if (strncmp(command, ADD,   sizeof(ADD) - 1)   == 0) return kAdd;
    if (strncmp(command, SUB,   sizeof(SUB) - 1)   == 0) return kSub;
    if (strncmp(command, MUL,   sizeof(MUL) - 1)   == 0) return kMul;
    if (strncmp(command, DIV,   sizeof(DIV) - 1)   == 0) return kDiv;
    if (strncmp(command, SQRT,  sizeof(SQRT) - 1)  == 0) return kSqrt;
    if (strncmp(command, IN,    sizeof(IN) - 1)    == 0) return kIn;
    if (strncmp(command, OUT,   sizeof(OUT) - 1)   == 0) return kOut;
    if (strncmp(command, JBE,   sizeof(JBE) - 1)   == 0) return kJBE;
    if (strncmp(command, JB,    sizeof(JB) - 1)    == 0) return kJB;
    if (strncmp(command, JAE,   sizeof(JAE) - 1)   == 0) return kJAE;
    if (strncmp(command, JA,    sizeof(JA) - 1)    == 0) return kJA;
    if (strncmp(command, JE,    sizeof(JE) - 1)    == 0) return kJE;
    if (strncmp(command, JNE,   sizeof(JNE) - 1)   == 0) return kJNE;
    if (strncmp(command, HLT,   sizeof(HLT) - 1)   == 0) return kHlt;
    if (strncmp(command, CALL,  sizeof(CALL) -1)   == 0)   return kCall;
    if (strncmp(command, RET,   sizeof(RET) - 1)   == 0)   return kRet;

    return kCommandNotFound; 
}

const char *SkipWhitespace(const char *str) {
    assert(str);

    while (isspace((unsigned char) *str)) {
        str++;
    }

    return str;
}

int StringToInt(char *arg_str) {
    assert(arg_str);

    return (int)(arg_str[1] - 'A');
}