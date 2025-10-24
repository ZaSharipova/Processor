#ifndef ASSEMBLER_STRUCTS_H_
#define ASSEMBLER_STRUCTS_H_

#include <stdio.h>

#include "AssemblerEnums.h"

#define MAX_ARR_SIZE 20
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
    int commands_counter;
    Labels labels;
} AssemblerInfo;

typedef struct {
    const char *command_name;
    size_t num_args;
    int command_num;
    size_t command_len;
    TypeOfArg type_of_arg;
    uint32_t hash;
} CommandsInfo;

extern CommandsInfo commands[];

#endif //ASSEMBLER_STRUCTS_H_