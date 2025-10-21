#include "SubsidiaryFunctionsAssembler.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "AssemblerStructs.h"

const CommandsInfo commands[] = {
    {"HLT",   0, kHlt,   sizeof("HLT") - 1},
    {"PUSHR", 1, kPushR, sizeof("PUSHR") - 1},
    {"PUSHM", 1, kPushM, sizeof("PUSHM") - 1},
    {"PUSH",  1, kPush,  sizeof("PUSH") - 1},
    {"POPR",  1, kPopR,  sizeof("POPR") - 1},
    {"POPM",  1, kPopM,  sizeof("POPM") - 1},
    {"POP",   1, kPop,   sizeof("POP") - 1},
    {"ADD",   0, kAdd,   sizeof("ADD") - 1},
    {"SUB",   0, kSub,   sizeof("SUB") - 1},
    {"MUL",   0, kMul,   sizeof("MUL") - 1},
    {"DIV",   0, kDiv,   sizeof("DIV") - 1},
    {"SQRT",  0, kSqrt,  sizeof("SQRT") - 1},
    {"OUTC",  0, kOutC,  sizeof("OUTC") - 1},
    {"OUT",   0, kOut,   sizeof("OUT") - 1},
    {"IN",    0, kIn,    sizeof("IN") - 1},
    {"JMP",   1, kJmp,   sizeof("JMP") - 1},
    {"JBE",   1, kJBE,   sizeof("JBE") - 1},
    {"JB",    1, kJB,    sizeof("JB") - 1},
    {"JAE",   1, kJAE,   sizeof("JAE") - 1},
    {"JA",    1, kJA,    sizeof("JA") - 1},
    {"JNE",   1, kJNE,   sizeof("JNE") - 1},
    {"JE",    1, kJE,    sizeof("JE") - 1},
    {"CALL",  1, kCall,  sizeof("CALL") - 1},
    {"RET",   0, kRet,   sizeof("RET") - 1},
    {"DRAW",  0, kDraw,  sizeof("DRAW") - 1},
    {NULL,    0, kCommandNotFound, 0}
};

int CommandToEnum(const char *command) {
    assert(command);

    for (size_t i = 0; commands[i].command_name != NULL; i++) {
        if (strncmp(command, commands[i].command_name, commands[i].command_len) == 0) {
            return commands[i].command_num;
        }
    }

    return kCommandNotFound;
}

const char *EnumToCommand(int command_num) {
    return commands[command_num].command_name;
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