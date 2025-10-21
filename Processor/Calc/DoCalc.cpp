#include "DoCalc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "AssemblerEnums.h"
#include "FileOperations.h"

#define JB_SIGN <
#define JBE_SIGN <=
#define JA_SIGN >
#define JAE_SIGN >=
#define JE_SIGN ==
#define JNE_SIGN !=

#define CHECK_JUMP(a, b, mode) ((a) mode (b))
#define DEFAULT_COMMAND -1

// #define DO_JUMP_COMPARE(mode)                                                                                         \
//     CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number2));                                                   \
//     CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number1));                                                   \
//     if (number1 mode number2) {                                                                                       \
//         processor_info->instruction_counter = (size_t)processor_info->code[processor_info->instruction_counter + 1];  \
//     } else {                                                                                                          \
//         processor_info->instruction_counter += 2;                                                                     \
//     }                                                                                                                 \
//     getchar();

int Calculate(FILE *fout, Processor *processor_info, size_t code_size) { //
    assert(fout);
    assert(processor_info);

    ProcessorErr_t err = kProcessorSuccess;
    CHECK_PROCESSOR_RETURN(ProcessorVerify(processor_info));

    Stack_t number1 = 0, number2 = 0;
    Stack_t number_pop = 0;
    int cmd = DEFAULT_COMMAND;

    while (processor_info->instruction_counter < code_size && cmd != kHlt) {
        cmd = processor_info->code[processor_info->instruction_counter];
        // for (int i = 0; i < 10000; i++) {
        //     if (processor_info->ram[i] != 0) {
        //         printf("URA-%d ", i);
        //     }
        // }

        // printf("...%d %d- \n", cmd, processor_info->instruction_counter);
        // for (int i = 0; i < processor_info->stack.size; i++) {
        //     printf("%d. ", processor_info->stack.data[i]);
        // }
        // printf("s%zu ", processor_info->instruction_counter);
        // printf("\n");
        switch (cmd) {
            case (kPush):
                CHECK_ERROR_RETURN(Push_C(processor_info));
                break;

            case (kPop):
                CHECK_ERROR_RETURN(Pop_C(processor_info, &number_pop)); 
                break;

            case (kAdd):
                CHECK_ERROR_RETURN(DoCalc(processor_info, Add_C));
                break;

            case (kSub):
                CHECK_ERROR_RETURN(DoCalc(processor_info, Sub_C));
                break;

            case (kMul):
                CHECK_ERROR_RETURN(DoCalc(processor_info, Mul_C));
                break;

            case (kDiv):
                CHECK_ERROR_RETURN(DoCalc(processor_info, Div_C));
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

            case (kPushM):
                CHECK_ERROR_RETURN(PushM_C(processor_info)); 
                break;
            case (kPopM):
                CHECK_ERROR_RETURN(PopM_C(processor_info));
                break;

            case (kIn):
                CHECK_ERROR_RETURN(In_C(processor_info));
                break;

            case(kJmp):
                CHECK_ERROR_RETURN(Jmp_C(processor_info));
                break;

            case(kJB):
                CHECK_ERROR_RETURN(PopTwoNumbers(processor_info, &number1, &number2));
                DoJump(processor_info, CHECK_JUMP(number1, number2, JB_SIGN));
                break;  

            case(kJBE):
                CHECK_ERROR_RETURN(PopTwoNumbers(processor_info, &number1, &number2));
                DoJump(processor_info, CHECK_JUMP(number1, number2, JBE_SIGN));
                break;  

            case(kJA):
                CHECK_ERROR_RETURN(PopTwoNumbers(processor_info, &number1, &number2));
                DoJump(processor_info, CHECK_JUMP(number1, number2, JA_SIGN));
                break;  

            case(kJAE):
                CHECK_ERROR_RETURN(PopTwoNumbers(processor_info, &number1, &number2));
                DoJump(processor_info, CHECK_JUMP(number1, number2, JAE_SIGN));
                break;  

            case(kJE):
                CHECK_ERROR_RETURN(PopTwoNumbers(processor_info, &number1, &number2));
                DoJump(processor_info, CHECK_JUMP(number1, number2, JE_SIGN));
                break;  

            case(kJNE):
                CHECK_ERROR_RETURN(PopTwoNumbers(processor_info, &number1, &number2));
                DoJump(processor_info, CHECK_JUMP(number1, number2, JNE_SIGN));
                break;  

            case (kCall):
                CHECK_PROCESSOR_RETURN(Call_C(processor_info));
                break;

            case(kRet):
                CHECK_PROCESSOR_RETURN(Ret_C(processor_info));
                break;

            case(kOutC):
                CHECK_PROCESSOR_RETURN(OutC_C(fout, processor_info));
                break;

            case(kDraw):
                CHECK_PROCESSOR_RETURN(Draw_C(processor_info));
                break;

            case(kSquare):
                CHECK_PROCESSOR_RETURN(Square_C(processor_info));
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

// void f(int n) {
//     DO_JUMP(n);
// }

// DO_JUMP(n) #n ##n