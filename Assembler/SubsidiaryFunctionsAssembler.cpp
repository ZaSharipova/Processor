#include "SubsidiaryFunctionsAssembler.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "AssemblerStructs.h"
#include "CalculatorCommands.h"

const CommandsInfo commands[] = {
    {"HLT",   0,  kHlt,    sizeof("HLT")    - 1,  Hlt_C},
    {"PUSHR", 1,  kPushR,  sizeof("PUSHR")  - 1,  PushR_C},
    {"PUSHM", 1,  kPushM,  sizeof("PUSHM")  - 1,  PushM_C},
    {"PUSH",  1,  kPush,   sizeof("PUSH")   - 1,  Push_C},
    {"POPR",  1,  kPopR,   sizeof("POPR")   - 1,  PopR_C},
    {"POPM",  1,  kPopM,   sizeof("POPM")   - 1,  PopM_C},
    {"ADD",   0,  kAdd,    sizeof("ADD")    - 1,  Add_C},
    {"SUB",   0,  kSub,    sizeof("SUB")    - 1,  Sub_C},
    {"MUL",   0,  kMul,    sizeof("MUL")    - 1,  Mul_C},
    {"DIV",   0,  kDiv,    sizeof("DIV")    - 1,  Div_C},
    {"SQRT",  0,  kSqrt,   sizeof("SQRT")   - 1,  Sqrt_C},
    {"OUTC",  0,  kOutC,   sizeof("OUTC")   - 1,  OutC_C},
    {"OUT",   0,  kOut,    sizeof("OUT")    - 1,  Out_C},
    {"IN",    0,  kIn,     sizeof("IN")     - 1,  In_C},
    {"JMP",   1,  kJmp,    sizeof("JMP")    - 1,  Jmp_C},
    {"JBE",   1,  kJBE,    sizeof("JBE")    - 1,  C_JBE},
    {"JB",    1,  kJB,     sizeof("JB")     - 1,  C_JB},
    {"JAE",   1,  kJAE,    sizeof("JAE")    - 1,  C_JAE},
    {"JA",    1,  kJA,     sizeof("JA")     - 1,  C_JA},
    {"JNE",   1,  kJNE,    sizeof("JNE")    - 1,  C_JNE},
    {"JE",    1,  kJE,     sizeof("JE")     - 1,  C_JE},
    {"CALL",  1,  kCall,   sizeof("CALL")   - 1,  Call_C},
    {"RET",   0,  kRet,    sizeof("RET")    - 1,  Ret_C},
    {"DRAW",  0,  kDraw,   sizeof("DRAW")   - 1,  Draw_C},
    {"SQUARE", 0, kSquare, sizeof("SQUARE") - 1,  Square_C},
    {NULL,    0, kCommandNotFound, 0,             NULL}
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