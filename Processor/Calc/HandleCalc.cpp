#include "HandleCalc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "/Users/zarinasharipova/Assembler/AssemblerEnums.h"

#define JB_SIGN <
#define JBE_SIGN <=
#define JA_SIGN >
#define JAE_SIGN >=
#define JE_SIGN ==
#define JNE_SIGN !=

#define DO_JUMP_COMPARE(mode) \
    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number2, open_log_file)); \
    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number1, open_log_file)); \
    if (number1 mode number2) { \
        pointer = processor_info->code[pointer + 1]; \
    } else { \
        pointer += 2; \
    } \
    getchar(); \
    break; \

size_t Read(FILE *fin, int *instruction_counter, Stack_t *code[]) {
    assert(fin);
    assert(code);

    Stack_t cmd = -1;
    int pointer = 0;
    size_t code_size = 0;

    fscanf(fin, "%zd", &code_size);
    *instruction_counter = (int)code_size;

    *code = (Stack_t *) calloc ((size_t)code_size, sizeof(Stack_t));

    while (pointer < (int)code_size && fscanf(fin, "%d", &cmd) == 1) {
        (*code)[pointer] = cmd;
        pointer++;
    }

    return code_size;
}

int Calculate(FILE *fout, Processor *processor_info, int code_size, FILE *open_log_file) {
    assert(fout);
    assert(processor_info);
    assert(open_log_file);

    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(ProcessorVerify(processor_info, open_log_file));

    Stack_t number1 = 0, number2 = 0;
    Stack_t number_pop = 0;
    int cmd = -1;
    Stack_t pointer = 0;

    while (pointer < code_size && cmd != kHlt) {
        cmd = processor_info->code[pointer];
        // printf("%d - ", cmd);
        // for (int i = 0; i < processor_info->stack.size; i++) {
        //     printf("%d. ", processor_info->stack.data[i]);
        // }
        // printf("\n");
        switch (cmd) {
            case (kPush):
                CHECK_ERROR_RETURN(Push_C(processor_info, &pointer, open_log_file));
                break;

            case (kPop):
                CHECK_ERROR_RETURN(Pop_C(&processor_info->stack, &pointer, &number_pop, open_log_file));
                break;

            case (kAdd):
                CHECK_ERROR_RETURN(StackOperation(&processor_info->stack, Add_C, &pointer, open_log_file));
                break;

            case (kSub):
                CHECK_ERROR_RETURN(StackOperation(&processor_info->stack, Sub_C, &pointer, open_log_file));
                break;

            case (kMul):
                CHECK_ERROR_RETURN(StackOperation(&processor_info->stack, Mul_C, &pointer,  open_log_file));
                break;

            case (kDiv):
                CHECK_ERROR_RETURN(Div_C(&processor_info->stack, &pointer, open_log_file));
                break;

            case (kSqrt):
                CHECK_ERROR_RETURN(Sqrt_C(&processor_info->stack, &pointer, open_log_file));
                break;

            case (kOut):
                CHECK_ERROR_RETURN(Out_C(&processor_info->stack, &pointer, open_log_file, fout));
                break;

            case (kPushR):
                CHECK_ERROR_RETURN(PushR_C(processor_info, &pointer, open_log_file));
                break;

            case (kPopR):
                CHECK_ERROR_RETURN(PopR_C(processor_info, &pointer, open_log_file));
                break;

            case (kIn):
                CHECK_ERROR_RETURN(In_C(processor_info, &pointer, open_log_file));
                break;

            case(kJmp):
                pointer = processor_info->code[pointer + 1];
                getchar();
                break;

            case(kJB):
                DO_JUMP_COMPARE(JB_SIGN);
            case(kJBE):
                DO_JUMP_COMPARE(JBE_SIGN);
            case(kJA):
                DO_JUMP_COMPARE(JA_SIGN);
            case(kJAE):
                DO_JUMP_COMPARE(JAE_SIGN);
            case(kJE):
                DO_JUMP_COMPARE(JE_SIGN);
            case(kJNE):
                DO_JUMP_COMPARE(JNE_SIGN);

            case (kHlt):
                break;

            default:
                fprintf(stderr, "Unknown command: %d.\n", cmd);
                return -1;
        }
    }

    return ProcessorVerify(processor_info, open_log_file);
}