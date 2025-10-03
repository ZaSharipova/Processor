#ifndef CALCULATOR_COMMANDS_H_
#define CALCULATOR_COMMANDS_H_

#include <stdio.h>
#include "StructsEnums.h"

#define CHECK_STACK_RETURN(call) \
    err = (call); \
    if (err != kSuccess) { \
        return err; \
    }
StackErr_t StackOperation(Stack_Info *stk, Stack_t (*operation)(Stack_t, Stack_t), FILE *file);
Stack_t Add_C(Stack_t a, Stack_t b);
Stack_t Mul_C(Stack_t a, Stack_t b);
Stack_t Sub_C(Stack_t a, Stack_t b);

StackErr_t Push_C(Stack_Info *stk, Stack_t arg, FILE *log);
StackErr_t Pop_C(Stack_Info *stk, FILE *open_log_file);
StackErr_t Div_C(Stack_Info *stk, FILE *open_log_file);
StackErr_t Sqrt_C(Stack_Info *stk, FILE *open_log_file) ;
StackErr_t Out_C(Stack_Info *stk, FILE *open_log_file, FILE *open_out_file);

#endif