#include "SubsidiaryFunctionsAssembler.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "AssemblerEnums.h"

const CommandsInfo commands[] = {
    {"HLT",   0, kHlt, 3},
    {"PUSHR", 1, kPushR, 5},
    {"PUSHM", 1, kPushM, 5},
    {"PUSH",  1, kPush, 4},
    {"POPR",  1, kPopR, 4},
    {"POPM",  1, kPopM, 4},
    {"POP",   1, kPop, 3},
    {"ADD",   0, kAdd, 3},
    {"SUB",   0, kSub, 3},
    {"MUL",   0, kMul, 3},
    {"DIV",   0, kDiv, 3},
    {"SQRT",  0, kSqrt, 4},
    {"OUT",   0, kOut, 3},
    {"IN",    0, kIn, 2},
    {"JMP",   1, kJmp, 3},
    {"JBE",   1, kJBE, 3},
    {"JB",    1, kJB, 2},
    {"JAE",   1, kJAE, 3},
    {"JA",    1, kJA, 2},
    {"JNE",   1, kJNE, 3},
    {"JE",    1, kJE, 2},
    {"CALL",  1, kCall, 4},
    {"RET",   0, kRet, 3},
    {NULL,    0, kCommandNotFound, 0}
};

int CommandToEnum(const char *command) {
    assert(command);

    for (size_t i = 0; commands[i].command_name != NULL; i++) {
        if (strncmp(command, commands[i].command_name, commands[i].command_len) == 0) {
            return commands[i].command_num;
        }
    }

    printf("12 ");
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

void InitLabels(int *labels) {
    assert(labels);

    for (int i = 0; i < 12; i++) {
        labels[i] = -1;
    }
}