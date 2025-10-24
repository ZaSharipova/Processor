#include "SubsidiaryFunctionsAssembler.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "AssemblerStructs.h"
#include "CalculatorCommands.h"
#include "Hash.h"

#ifdef _Processor
    #define COND(func) func
#else
    #define COND(func) NULL
#endif

CommandsInfo commands[] = {
    {"HLT",   0,  kHlt,    sizeof("HLT")    - 1, kNoArg,  0},
    {"PUSHR", 1,  kPushR,  sizeof("PUSHR")  - 1, kRaxArg, 0},
    {"PUSHM", 1,  kPushM,  sizeof("PUSHM")  - 1, kRamArg, 0},
    {"PUSH",  1,  kPush,   sizeof("PUSH")   - 1, kIntArg, 0},
    {"POPR",  1,  kPopR,   sizeof("POPR")   - 1, kRaxArg, 0},
    {"POPM",  1,  kPopM,   sizeof("POPM")   - 1, kRamArg, 0},
    {"ADD",   0,  kAdd,    sizeof("ADD")    - 1, kNoArg,  0},
    {"SUB",   0,  kSub,    sizeof("SUB")    - 1, kNoArg,  0},
    {"MUL",   0,  kMul,    sizeof("MUL")    - 1, kNoArg,  0},
    {"DIV",   0,  kDiv,    sizeof("DIV")    - 1, kNoArg,  0},
    {"SQRT",  0,  kSqrt,   sizeof("SQRT")   - 1, kNoArg,  0},
    {"OUTC",  0,  kOutC,   sizeof("OUTC")   - 1, kNoArg,  0},
    {"OUT",   0,  kOut,    sizeof("OUT")    - 1, kNoArg,  0},
    {"IN",    0,  kIn,     sizeof("IN")     - 1, kNoArg,  0},
    {"JMP",   1,  kJmp,    sizeof("JMP")    - 1, kLabelArg, 0},
    {"JBE",   1,  kJBE,    sizeof("JBE")    - 1, kLabelArg, 0},
    {"JB",    1,  kJB,     sizeof("JB")     - 1, kLabelArg, 0},
    {"JAE",   1,  kJAE,    sizeof("JAE")    - 1, kLabelArg, 0},
    {"JA",    1,  kJA,     sizeof("JA")     - 1, kLabelArg, 0},
    {"JNE",   1,  kJNE,    sizeof("JNE")    - 1, kLabelArg, 0},
    {"JE",    1,  kJE,     sizeof("JE")     - 1, kLabelArg, 0},
    {"CALL",  1,  kCall,   sizeof("CALL")   - 1, kLabelArg, 0},
    {"RET",   0,  kRet,    sizeof("RET")    - 1, kNoArg, 0},
    {"DRAW",  0,  kDraw,   sizeof("DRAW")   - 1, kNoArg, 0},
    {"SQUARE", 0, kSquare, sizeof("SQUARE") - 1, kNoArg, 0},
    {"PAUSE", 0, kPause,  sizeof("PAUSE")  - 1,  kNoArg, 0},
    {NULL,    0, kCommandNotFound, 0,            kNoArg, 0}
};

int CommandToEnumPos(const char *command) {
    assert(command);

    uint32_t command_hash = HashData(command, strlen(command));

    CommandsInfo *result = (CommandsInfo *)bsearch(&command_hash, commands, 27, sizeof(CommandsInfo), CompareCommandHash);
    
    if (result != NULL) {
        return (int)(result - commands);
    }

    return -1;
}

int CompareCommandHash(const void *key, const void *element) {
    uint32_t target_hash = *(const uint32_t *)key;
    const CommandsInfo *cmd = (const CommandsInfo *)element;

    if (target_hash < cmd->hash)
        return -1;
    else if (target_hash > cmd->hash)
        return 1;
    else
        return 0;
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

    int return_number = (int)(arg_str[1] - 'A');
    if (return_number >= 0 && return_number < REGS_SIZE) {
        return return_number;
    }

    return -1;
}

void InitLabels(int *labels) {
    assert(labels);

    for (int i = 0; i < 12; i++) {
        labels[i] = -1;
    }
}