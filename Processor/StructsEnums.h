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

enum ProcessorErr_t {
    kProcessorNullPointer  = 1 << 0,
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
    kNoArgumentWritten     = 1 << 14,

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
    int code_size;
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

// typedef enum {
//     kSuccess,
//     kErrorOpening,
//     kErrorUnknown
// } ProcessorErr_t;

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

enum ReallocMode {
    kIncrease,
    kDecrease,
    kNoChange,
    kIncreaseZero,
};

#define CHECK_ERROR_AND_CLOSE_FILE_RETURN(call) \
    err = (call); \
    if (err != kNoError) { \
        return HandleCloseFile(in_out_files); \
    }

#endif //STRUCTS_ENUMS_H_