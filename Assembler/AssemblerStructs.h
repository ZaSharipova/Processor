#ifndef ASSEMBLER_STRUCTS_H_
#define ASSEMBLER_STRUCTS_H_

#include <stdio.h>

#include "AssemblerEnums.h"
#include "Commands.h"

#define MAX_ARR_SIZE 23
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

#endif //ASSEMBLER_STRUCTS_H_