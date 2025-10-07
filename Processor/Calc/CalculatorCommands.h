#ifndef CALCULATOR_COMMANDS_H_
#define CALCULATOR_COMMANDS_H_

#include <stdio.h>
#include "StructsEnums.h"

#define CHECK_STACK_RETURN(call) \
    err = (call); \
    if (err != kSuccess) { \
        return err; \
    }
ProcessorErr_t StackOperation(Stack_Info *stk, Stack_t (*operation)(Stack_t, Stack_t), FILE *file);
Stack_t Add_C(Stack_t a, Stack_t b);
Stack_t Mul_C(Stack_t a, Stack_t b);
Stack_t Sub_C(Stack_t a, Stack_t b);

ProcessorErr_t Push_C(Stack_Info *stk, Stack_t arg, FILE *open_log_file);
ProcessorErr_t Pop_C(Stack_Info *stk, FILE *open_log_file);
ProcessorErr_t Div_C(Stack_Info *stk, FILE *open_log_file);
ProcessorErr_t Sqrt_C(Stack_Info *stk, FILE *open_log_file);

ProcessorErr_t In_C(Stack_Info *stk, FILE *open_log_file);
ProcessorErr_t PushR_C(Processor *processor_info, int pos, FILE *open_log_file);
ProcessorErr_t PopR_C(Processor *processor_info, int pos, FILE *open_log_file);
ProcessorErr_t Out_C(Stack_Info *stk, FILE *open_log_file, FILE *open_out_file);

#endif