#ifndef CALCULATOR_COMMANDS_H_
#define CALCULATOR_COMMANDS_H_

#include <stdio.h>
#include "StructsEnums.h"

#define CHECK_STACK_RETURN(call) \
    err = (call); \
    if (err != kSuccess) { \
        return err; \
    }

ProcessorErr_t StackOperation(Processor *processor_info, Stack_t (*operation)(Stack_t, Stack_t));
Stack_t Add_C(Stack_t a, Stack_t b);
Stack_t Mul_C(Stack_t a, Stack_t b);
Stack_t Sub_C(Stack_t a, Stack_t b);

ProcessorErr_t Push_C(Processor *processor_info);
ProcessorErr_t Pop_C(Processor *processor_info, Stack_t *number_pop) ;
ProcessorErr_t PopR_C(Processor *processor_info);
ProcessorErr_t Div_C(Processor *processor_info);
ProcessorErr_t Sqrt_C(Processor *processor_info);

ProcessorErr_t In_C(Processor *processor_info);
ProcessorErr_t PushR_C(Processor *processor_info);
ProcessorErr_t PopR_C(Processor *processor_info);
ProcessorErr_t Out_C(Processor *processor_info, FILE *open_out_file);

ProcessorErr_t Jmp_C(Processor *processor_info);

#endif