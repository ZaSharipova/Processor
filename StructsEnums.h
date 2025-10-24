#ifndef STRUCTS_ENUMS_H_
#define STRUCTS_ENUMS_H_

#include <stdio.h>
#include <stdint.h>

typedef int Stack_t;
#define STACK_VALUE_MODE "%d"
const Stack_t POISON = 1e6;

#define READ_MODE "r"
#define WRITE_MODE "w"

#define RAM_SIZE 144 * 192
#define REGS_SIZE 16

enum ParseErr_t {
    kNoError      = 0,
    kErrorParsing = -1,
    kWrongMode    = -2,
    kErrorOpening = -3,
    kErrorClosing = -4,
}; // 

enum ProcessorErr_t {
    kProcessorNullPointer  = 1 << 0,
    kNoMemory              = 1 << 1,
    kZeroNumber            = 1 << 2,
    kNumberNotWritten      = 1 << 3,
    kNoCommandFound        = 1 << 4,
    kNoArgumentWritten     = 1 << 5,

    kProcessorSuccess = 0,
    kProcessorFailure = -1,
};

enum StackErr_t {
    kErrorEmptyStack       = 1 << 16,
    kErrorStackNullPointer = 1 << 17,
    kSizeError             = 1 << 18,
    kNegativeCapacity      = 1 << 19,
    kNegativeSize          = 1 << 20,
    kWrongCanaryLeft       = 1 << 21,
    kWrongCanaryRight      = 1 << 22,
    kWrongHash             = 1 << 23,
    kNoAllocMemory        = 1 << 24,

    kStackSuccess          = 0,
    kStackFailure          = -1
};


// enum PossibleErrors {
//     kNoErrorA,
//     kErrorOpeningA,
//     kErrorClosingA,
//     kErrorParsingA,  
// };

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
    size_t code_size;
    size_t instruction_counter;
    Stack_Info stack;
    Stack_t regs[REGS_SIZE];
    Stack_Info call_array;
    Stack_t ram[RAM_SIZE];
};

struct Files {
    const char *log_file;
    // FILE *open_log_file;
    const char *in_file;
    FILE *open_in_file;
    const char *out_file;
    FILE *open_out_file;
};

enum ReallocMode {
    kIncrease,
    kDecrease,
    kNoChange,
    kIncreaseZero,
};

#define CHECK_ERROR_AND_CLOSE_FILE_RETURN(call) \
    err = (call);                               \
    if (err != kNoError) {                      \
        CloseLogFile();                         \
        return DoCloseFile(&in_out_files);   \
    }

#endif //STRUCTS_ENUMS_H_