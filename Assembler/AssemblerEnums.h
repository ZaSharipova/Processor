#ifndef ASSEMBLER_ENUMS_H_
#define ASSEMBLER_ENUMS_H_

#include <stdio.h>

#include "StructsEnums.h"
#include "SubsidiaryFunctionsAssembler.h"

#define MAX_ARR_SIZE 20
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

enum Convert {
    kCommandNotFound = -1,
    kHlt    = 0,
    kPushR  = 1,
    kPushM  = 2,
    kPush   = 3,
    kPopR   = 4,
    kPopM   = 5,
    kPop    = 6,
    kAdd    = 7,
    kSub    = 8,
    kMul    = 9,
    kDiv    = 10,
    kSqrt   = 11,
    kOut    = 12,
    kIn     = 13,
    kJmp    = 14,
    kJBE    = 15,
    kJB     = 16,
    kJAE    = 17,
    kJA     = 18,
    kJNE    = 19,
    kJE     = 20,
    kCall   = 21,
    kRet    = 22
};

typedef struct {
    const char *command_name;
    size_t num_args;
    int command_num;
    size_t command_len;
} CommandsInfo;

extern const CommandsInfo commands[];

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
    char name[MAX_ARR_SIZE];
    int address;
} Label;

typedef struct {
    Label data[MAX_ARR_SIZE];
    size_t count;
} Labels;

typedef struct {
    Stack_Info data;
    Labels labels[MAX_ARR_SIZE];
    //int gotomap[MAX_ARR_SIZE];
} AssemblerInfo;

#endif //ASSEMBLER_ENUMS_H_