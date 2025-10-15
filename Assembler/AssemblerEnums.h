#ifndef ASSEMBLER_ENUMS_H_
#define ASSEMBLER_ENUMS_H_

#include <stdio.h>

#include "StructsEnums.h"

#define ERROR_CHECK_RETURN(call) \
    err = (call); \
    if (err != kNoErrorAsm) { \
        return err; \
    }

enum AsmError {
    kNoAsmError         = 0,
    kErrorOpeningFile   = -1,
    kErrorClosingFile   = -2,
    kNoMemoryCalloc     = -3,
    kNoAvailableValue   = -4,
    kNoAvailableCommand = -5,
    kErrorZeroArgs      = -6,
    kWhitespace         = -7,
    kErrorParsingAsm    = -8,
};

enum NumOfArgs {
    kNo = -1,
    kZero = 0,
    kOne = 1,
};

#define PUSH "PUSH"
#define POP "POP"
#define ADD "ADD"
#define SUB "SUB"
#define MUL "MUL"
#define DIV "DIV"
#define SQRT "SQRT"
#define OUT "OUT"
#define PUSHR "PUSHR"
#define POPR  "POPR"
#define IN    "IN"
#define HLT   "HLT"
#define JMP   "JMP"
#define JB    "JB"
#define JBE   "JBE"
#define JA    "JA"
#define JAE   "JAE"
#define JE    "JE"
#define JNE   "JNE"
#define CALL "CALL"
#define RET "RET"
#define PUSHM "PUSHM"
#define POPM "POPM"

// typedef struct {
//     int command_code;
//     int arg_count;
// } CommandInfo;

// const CommandInfo Commands[] = {
//     {kHlt,   0},
//     {kPush,  1},
//     {kPop,   1},
//     {kAdd,   0},
//     {kSub,   0},
//     {kMul,   0},
//     {kDiv,   0},
//     {kSqrt,  0},
//     {kOut,   0},
//     {kPushR, 1},
//     {kPopR,  1},
//     {kIn,    0},
//     {kJmp,   1},
//     {kJB,    1},
//     {kJBE,   1},
//     {kJA,    1},
//     {kJAE,   1},
//     {kJE,    1},
//     {kJNE,   1}
// };

enum Convert {
    kCommandNotFound = -1,
    kHlt   = 0,
    kPush  = 1,
    kPop   = 2,
    kAdd   = 3,
    kSub   = 4,
    kMul   = 5,
    kDiv   = 6,
    kSqrt  = 7,
    kOut   = 8,
    kPushR = 9,
    kPopR  = 10,
    kIn    = 11,
    kJmp   = 12,
    kJB    = 13,
    kJBE   = 14,
    kJA    = 15,
    kJAE   = 16,
    kJE    = 17,
    kJNE   = 18,
    kCall  = 19,
    kRet   = 20,
    kPushM = 21,
    kPopM  = 22,
};

typedef struct {
    char *start_ptr;
    char *end_ptr;
    char *start_ptr_alpha;
    char *end_ptr_alpha;
    size_t size;
} LineInfo;

typedef struct {
    char *buf_ptr;      
    size_t filesize;    
    int count_lines;    
    LineInfo *text_ptr;
} FileInfo;


typedef struct {
    Stack_Info stack;
    int labels[10];
    int gotomap[10];
} AssemblerInfo;

#endif //ASSEMBLER_ENUMS_H_