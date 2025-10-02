#ifndef STRUCTS_ENUMS_H_
#define STRUCTS_ENUMS_H_

#include <stdio.h>
#include <stdint.h>

typedef int Stack_t;
#define MY_SPEC "%d"
const Stack_t POISON = 1e6;

enum StackErr_t {
    kErrorEmptyStack       = 1 << 0,
    kErrorStackNullPointer = 1 << 1,
    kSizeError             = 1 << 2,
    kNegativeCapacity      = 1 << 3,
    kNegativeSize          = 1 << 4,
    kWrongCanaryLeft       = 1 << 5,
    kWrongCanaryRight      = 1 << 6,
    kWrongHash             = 1 << 7,
    kNoCallocMemory        = 1 << 8,

    kNoMemory              = 1 << 9,
    kZeroNumber            = 1 << 10,
    kNumberNotWritten      = 1 << 11,
    kNoCommandFound        = 1 << 12,

    kErrorOpening          = 1 << 13,
    kSuccess               = 0,
};

typedef struct {
    char cmd[16];
    int  arg;
    int  has_arg;
} Instruction;

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
    Stack_t *real_data;
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
#define HLT "HLT"

enum Convert {
    Push = 1,
    Pop = 2,
    Add = 3,
    Sub = 4,
    Mul = 5,
    Div = 6,
    Sqrt = 7,
    Out = 8,
    Hlt = 0,
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
} FileInfo;

enum Realloc_Mode {
    kIncrease,
    kDecrease,
    kNoChange,
    kIncreaseZero,
};

#endif //STRUCTS_ENUMS_H_