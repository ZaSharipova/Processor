#ifndef STRUCTS_ENUMS_H_
#define STRUCTS_ENUMS_H_

#include <stdio.h>
#include <stdint.h>

typedef int Stack_t;
#define MY_SPEC "%d"

#define READ_MODE "r"
#define WRITE_MODE "w"

enum PossibleErrors {
    kNoError,
    kErrorOpening,
    kErrorClosing,
    kErrorParsing,
};

// struct Source_Location_Info {
//     const char *file_name;
//     const char *func_name;
//     const char *var;
//     size_t line;
// };


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

typedef struct {
    char *start_ptr;
    char *end_ptr;
    size_t size;
} LineInfo;

typedef struct {
    char *buf_ptr;      
    size_t filesize;    
    int count_lines;    
    LineInfo *text_ptr; 
} FileInfo;

enum NumOfArgs {
    kNothing = -1,
    kZero = 0,
    kOne = 1,
};

#endif //STRUCTS_ENUMS_H_