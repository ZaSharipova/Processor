#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <stdio.h>

#include "AssemblerEnums.h"

#define NUM_COMMANDS 29

enum Convert {
    kCommandNotFound = -1,
    kHlt    = 0,
    kPushR  = 1,
    kPushMN = 2,
    kPushM  = 3,
    kPush   = 4,
    kPopR   = 5,
    kPopMN  = 6,
    kPopM   = 7,
    kAdd    = 8,
    kSub    = 9,
    kMul    = 10,
    kDiv    = 11,
    kSqrt   = 12,
    kOutC   = 13,
    kOut    = 14,
    kIn     = 15,
    kJmp    = 16,
    kJBE    = 17,
    kJB     = 18,
    kJAE    = 19,
    kJA     = 20,
    kJNE    = 21,
    kJE     = 22,
    kCall   = 23,
    kRet    = 24,
    kDraw   = 25,
    kSquare = 26,
    kPause  = 27,
    kDumpRam= 28,
};

typedef struct {
    const char *command_name;
    size_t num_args;
    int command_num;
    size_t command_len;
    TypeOfArg type_of_arg;
#ifdef _Assembler
    uint32_t hash;
#endif
#ifdef _Processor
    ProcessorErr_t (*func)(Processor *);
#endif
} CommandsInfo;

#ifdef _Assembler
    #define LAST_FIELD(hash_val, func_ptr) hash_val
#endif
#ifdef _Processor
    #include "CalculatorCommands.h"
    #define LAST_FIELD(hash_val, func_ptr) func_ptr
#endif

extern CommandsInfo commands[];

#endif //COMMANDS_H_