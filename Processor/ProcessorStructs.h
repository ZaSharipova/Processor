#ifndef PROCESSOR_STRUCTS_H_
#define PROCESSOR_STRUCTS_H_

#include "AssemblerEnums.h"
#include "CalculatorCommands.h"

struct CommandsProcessorInfo {
    int command_num;
    ProcessorErr_t (*func)(Processor *);
};

CommandsProcessorInfo commands_processor[] = {
    {kHlt,   Hlt_C},
    {kPushR, PushR_C},
    {kPushM, PushM_C},
    {kPush,  Push_C},
    {kPopR,  PopR_C},
    {kPopM,  PopM_C},
    {kAdd,   Add_C},
    {kSub,   Sub_C},
    {kMul,   Mul_C},
    {kDiv,   Div_C},
    {kSqrt,  Sqrt_C},
    {kOutC,  OutC_C},
    {kOut,   Out_C},
    {kIn,    In_C},
    {kJmp,   Jmp_C},
    {kJBE,   C_JBE},
    {kJB,    C_JB},
    {kJAE,   C_JAE},
    {kJA,    C_JA},
    {kJNE,   C_JNE},
    {kJE,    C_JE},
    {kCall,  Call_C},
    {kRet,   Ret_C},
    {kDraw,  Draw_C},
    {kSquare, Square_C},
    {kPause, Pause_C},
    {kCommandNotFound, NULL}
};


#endif //PROCESSOR_STRUCTS_H_