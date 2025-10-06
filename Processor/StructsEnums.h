#ifndef STRUCTS_ENUMS_H_
#define STRUCTS_ENUMS_H_

#include <stdio.h>
#include <stdint.h>

typedef int Stack_t;
#define MY_SPEC "%d"
const Stack_t POISON = 1e6;


#define READ_MODE "r"
#define WRITE_MODE "w"

enum ParseErr_t {
    kNoError      = 0,
    kErrorParsing = -1,
    kWrongMode    = -2,
    kErrorOpening = -3,
    kErrorClosing = -4,
};

enum StackErr_t {
    kProcessorNullPointer  = 1 < 0,
    kErrorEmptyStack       = 1 << 1,
    kErrorStackNullPointer = 1 << 2,
    kSizeError             = 1 << 3,
    kNegativeCapacity      = 1 << 4,
    kNegativeSize          = 1 << 5,
    kWrongCanaryLeft       = 1 << 6,
    kWrongCanaryRight      = 1 << 7,
    kWrongHash             = 1 << 8,
    kNoCallocMemory        = 1 << 9,

    kNoMemory              = 1 << 10,
    kZeroNumber            = 1 << 11,
    kNumberNotWritten      = 1 << 12,
    kNoCommandFound        = 1 << 13,

    kSuccess               = 0,
    kFailure               = -1,
};

enum PossibleErrors {
    kNoErrorA,
    kErrorOpeningA,
    kErrorClosingA,
    kErrorParsingA,  
};


enum NumOfArgs {
    kNo = -1,
    kZero = 0,
    kOne = 1,
};

// typedef struct {
//     char cmd[16];
//     int  arg;
//     int  has_arg;
// } Instruction;

struct Source_Location_Info {
    const char *file_name;
    const char *func_name;
    const char *var;
    size_t line;
};

struct Stack_Info {
    Stack_t *data;
    ssize_t size;
    ssize_t capacity;

#ifdef _DEBUG
    Source_Location_Info create_var_info;
#endif

#ifdef _CANARY
    uint32_t canary_left;
    uint32_t canary_right;
#endif

#ifdef _HASH
    uint32_t data_hash;
#endif
};

struct Processor {
    Stack_t *code;
    int instruction_counter;
    Stack_Info stack;
    Stack_t regs[16];
};

struct Files {
    const char *log_file;
    FILE *open_log_file;
    const char *in_file;
    FILE *open_in_file;
    const char *out_file;
    FILE *open_out_file;
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
#define POPR "POPR"
#define IN "IN"
#define HLT "HLT"

enum Convert {
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
    kHlt   = 0,
};

// typedef enum {
//     kSuccess,
//     kErrorOpening,
//     kErrorUnknown
// } StackErr_t;

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
    int instruction_counter;
} FileInfo;

enum Realloc_Mode {
    kIncrease,
    kDecrease,
    kNoChange,
    kIncreaseZero,
};

#endif //STRUCTS_ENUMS_H_