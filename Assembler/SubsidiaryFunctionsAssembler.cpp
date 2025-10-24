#include "SubsidiaryFunctionsAssembler.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "AssemblerStructs.h"
#include "CalculatorCommands.h"
#include "Hash.h"

#ifdef _Assembler
    #define LAST_FIELD(hash_val, func_ptr) hash_val
#endif
#ifdef _Processor
    #define LAST_FIELD(hash_val, func_ptr) func_ptr
#endif

CommandsInfo commands[] = {
    {"HLT",    0, kHlt,    sizeof("HLT")    - 1, kNoArg,    LAST_FIELD(0, Hlt_C)},
    {"PUSHR",  1, kPushR,  sizeof("PUSHR")  - 1, kRaxArg,   LAST_FIELD(0, PushR_C)},
    {"PUSHM",  1, kPushM,  sizeof("PUSHM")  - 1, kRamArg,   LAST_FIELD(0, PushM_C)},
    {"PUSH",   1, kPush,   sizeof("PUSH")   - 1, kIntArg,   LAST_FIELD(0, Push_C)},
    {"POPR",   1, kPopR,   sizeof("POPR")   - 1, kRaxArg,   LAST_FIELD(0, PopR_C)},
    {"POPM",   1, kPopM,   sizeof("POPM")   - 1, kRamArg,   LAST_FIELD(0, PopM_C)},
    {"ADD",    0, kAdd,    sizeof("ADD")    - 1, kNoArg,    LAST_FIELD(0, Add_C)},
    {"SUB",    0, kSub,    sizeof("SUB")    - 1, kNoArg,    LAST_FIELD(0, Sub_C)},
    {"MUL",    0, kMul,    sizeof("MUL")    - 1, kNoArg,    LAST_FIELD(0, Mul_C)},
    {"DIV",    0, kDiv,    sizeof("DIV")    - 1, kNoArg,    LAST_FIELD(0, Div_C)},
    {"SQRT",   0, kSqrt,   sizeof("SQRT")   - 1, kNoArg,    LAST_FIELD(0, Sqrt_C)},
    {"OUTC",   0, kOutC,   sizeof("OUTC")   - 1, kNoArg,    LAST_FIELD(0, OutC_C)},
    {"OUT",    0, kOut,    sizeof("OUT")    - 1, kNoArg,    LAST_FIELD(0, Out_C)},
    {"IN",     0, kIn,     sizeof("IN")     - 1, kNoArg,    LAST_FIELD(0, In_C)},
    {"JMP",    1, kJmp,    sizeof("JMP")    - 1, kLabelArg, LAST_FIELD(0, Jmp_C)},
    {"JBE",    1, kJBE,    sizeof("JBE")    - 1, kLabelArg, LAST_FIELD(0, C_JBE)},
    {"JB",     1, kJB,     sizeof("JB")     - 1, kLabelArg, LAST_FIELD(0, C_JB)},
    {"JAE",    1, kJAE,    sizeof("JAE")    - 1, kLabelArg, LAST_FIELD(0, C_JAE)},
    {"JA",     1, kJA,     sizeof("JA")     - 1, kLabelArg, LAST_FIELD(0, C_JA)},
    {"JNE",    1, kJNE,    sizeof("JNE")    - 1, kLabelArg, LAST_FIELD(0, C_JNE)},
    {"JE",     1, kJE,     sizeof("JE")     - 1, kLabelArg, LAST_FIELD(0, C_JE)},
    {"CALL",   1, kCall,   sizeof("CALL")   - 1, kLabelArg, LAST_FIELD(0, Call_C)},
    {"RET",    0, kRet,    sizeof("RET")    - 1, kNoArg,    LAST_FIELD(0, Ret_C)},
    {"DRAW",   0, kDraw,   sizeof("DRAW")   - 1, kNoArg,    LAST_FIELD(0, Draw_C)},
    {"SQUARE", 0, kSquare, sizeof("SQUARE") - 1, kNoArg,    LAST_FIELD(0, Square_C)},
    {"PAUSE",  0, kPause,  sizeof("PAUSE")  - 1, kNoArg,    LAST_FIELD(0, Pause_C)},
    {NULL,     0, kCommandNotFound, 0,           kNoArg,    LAST_FIELD(0, NULL)}
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