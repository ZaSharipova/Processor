#include "HandleCalc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "AssemblerEnums.h"

#define JB_SIGN <
#define JBE_SIGN <=
#define JA_SIGN >
#define JAE_SIGN >=
#define JE_SIGN ==
#define JNE_SIGN !=

// #define CHECK_JUMP(a, b, mode) ((a) mode (b))

// void DoJump(Processor *peocessor_info, bool do_jump) {

// }

// DoJump(CHECK_JUMP(a, b >));

#define DO_JUMP_COMPARE(mode)                                                                                 \
    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number2));                                           \
    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number1));                                           \
    if (number1 mode number2) {                                                                               \
        processor_info->instruction_counter = processor_info->code[processor_info->instruction_counter + 1];  \
    } else {                                                                                                  \
        processor_info->instruction_counter += 2;                                                             \
    }                                                                                                         \
    getchar();

ProcessorErr_t Read(FILE *fin, Stack_t *code[], size_t *code_size) {
    assert(fin);
    assert(code);

    Stack_t cmd = -1;
    int pointer = 0;

    fscanf(fin, "%zd", code_size);

    *code = (Stack_t *) calloc ((size_t)code_size, sizeof(Stack_t));
    if (*code == NULL) {
        return kNoMemory;
    }

    while (pointer < (int)*code_size && fscanf(fin, "%d", &cmd) == 1) {
        (*code)[pointer] = cmd;
        pointer++;
    }

    return kSuccess;
}

int Calculate(FILE *fout, Processor *processor_info, int code_size) {
    assert(fout);
    assert(processor_info);

    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(ProcessorVerify(processor_info));

    Stack_t number1 = 0, number2 = 0;
    Stack_t number_pop = 0;
    int cmd = -1;

    while (processor_info->instruction_counter < code_size && cmd != kHlt) {
        cmd = processor_info->code[processor_info->instruction_counter];
        printf("%d - ", cmd);
        for (int i = 0; i < processor_info->stack.size; i++) {
            printf("%d. ", processor_info->stack.data[i]);
        }
        printf("\n");
        switch (cmd) {
            case (kPush):
                CHECK_ERROR_RETURN(Push_C(processor_info));
                break;

            case (kPop):
                CHECK_ERROR_RETURN(Pop_C(processor_info, &number_pop));
                break;

            case (kAdd):
                CHECK_ERROR_RETURN(StackOperation(processor_info, Add_C));
                break;

            case (kSub):
                CHECK_ERROR_RETURN(StackOperation(processor_info, Sub_C));
                break;

            case (kMul):
                CHECK_ERROR_RETURN(StackOperation(processor_info, Mul_C));
                break;

            case (kDiv):
                CHECK_ERROR_RETURN(Div_C(processor_info));
                break;

            case (kSqrt):
                CHECK_ERROR_RETURN(Sqrt_C(processor_info));
                break;

            case (kOut):
                CHECK_ERROR_RETURN(Out_C(processor_info, fout));
                break;

            case (kPushR):
                CHECK_ERROR_RETURN(PushR_C(processor_info));
                break;

            case (kPopR):
                CHECK_ERROR_RETURN(PopR_C(processor_info));
                break;

            case (kIn):
                CHECK_ERROR_RETURN(In_C(processor_info));
                break;

            case(kJmp):
                CHECK_ERROR_RETURN(Jmp_C(processor_info));
                break;

            case(kJB):
                DO_JUMP_COMPARE(JB_SIGN);
                break;  
            case(kJBE):
                DO_JUMP_COMPARE(JBE_SIGN);
                break;  
            case(kJA):
                DO_JUMP_COMPARE(JA_SIGN);
                break;  
            case(kJAE):
                DO_JUMP_COMPARE(JAE_SIGN);
                break;  
            case(kJE):
                DO_JUMP_COMPARE(JE_SIGN);
                break;  
            case(kJNE):
                DO_JUMP_COMPARE(JNE_SIGN);
                break;  

            case (kHlt):
                break;

            default:
                fprintf(stderr, "Unknown command: %d.\n", cmd);
                return -1;
        }
    }

    return ProcessorVerify(processor_info);
}