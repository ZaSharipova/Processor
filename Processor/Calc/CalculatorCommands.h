#ifndef CALCULATOR_COMMANDS_H_
#define CALCULATOR_COMMANDS_H_

#include <stdio.h>

#include "StructsEnums.h"

#define CHECK_STACK_RETURN(call) \
    err = (call); \
    if (err != kStackSuccess) { \
        return err; \
    }

// ProcessorErr_t DoCalc(Processor *processor_info, Stack_t (*operation)(Stack_t, Stack_t));
// Stack_t Add_C(Stack_t a, Stack_t b);
// Stack_t Mul_C(Stack_t a, Stack_t b);
// Stack_t Sub_C(Stack_t a, Stack_t b);
// Stack_t Div_C(Stack_t a, Stack_t b);

ProcessorErr_t Push_C(Processor *processor_info);
ProcessorErr_t Pop_C(Processor *processor_info, Stack_t *number_pop) ;
ProcessorErr_t PopR_C(Processor *processor_info);
// ProcessorErr_t Div_C(Processor *processor_info);
ProcessorErr_t Sqrt_C(Processor *processor_info);

ProcessorErr_t In_C(Processor *processor_info);
ProcessorErr_t PushR_C(Processor *processor_info);
ProcessorErr_t PopR_C(Processor *processor_info);
ProcessorErr_t Out_C(Processor *processor_info);

ProcessorErr_t Call_C(Processor *processor_info);
ProcessorErr_t Ret_C(Processor *processor_info);

ProcessorErr_t PushM_C(Processor *processor_info);
ProcessorErr_t PopM_C(Processor *processor_info);

ProcessorErr_t Draw_C(Processor *processor_info);
ProcessorErr_t OutC_C(Processor *processor_info);

ProcessorErr_t Square_C(Processor *processor_info);

ProcessorErr_t Hlt_C(Processor *processor_info);

// ProcessorErr_t PopTwoNumbers(Processor *processor_info, Stack_t *number1, Stack_t *number2);
// void DoJump(Processor *processor_info, bool do_jump);

ProcessorErr_t Jmp_C(Processor *processor_info);

ProcessorErr_t Pause_C(Processor *processor_info);

ProcessorErr_t Add_C(Processor *processor_info);
ProcessorErr_t Sub_C(Processor *processor_info);
ProcessorErr_t Mul_C(Processor *processor_info);
ProcessorErr_t Div_C(Processor *processor_info);

ProcessorErr_t C_JB(Processor *processor_info);
ProcessorErr_t C_JBE(Processor *processor_info);
ProcessorErr_t C_JA(Processor *processor_info);
ProcessorErr_t C_JAE(Processor *processor_info);
ProcessorErr_t C_JE(Processor *processor_info);
ProcessorErr_t C_JNE(Processor *processor_info);

#endif