#include "HandleCalc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "/Users/zarinasharipova/Assembler/AssemblerEnums.h"

size_t Read(FILE *fin, Stack_t *code[]) {
    assert(fin);
    assert(code);

    Stack_t cmd = -1;
    int pos = 0;
    size_t code_size = 0;

    fscanf(fin, "%zd", &code_size);
    *code = (Stack_t *) calloc ((size_t)code_size, sizeof(Stack_t));

    while (pos < (int)code_size && fscanf(fin, "%d", &cmd) == 1) {
        (*code)[pos] = cmd;
        pos++;
    }

    return code_size;
}

int Calculate(FILE *fout, Processor *processor_info, int code_size, FILE *open_log_file) {
    assert(fout);
    assert(processor_info);
    assert(open_log_file);

    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(ProcessorVerify(processor_info, open_log_file));

    Stack_t number = 0;
    int cmd = -1;
    Stack_t arg = 0;
    Stack_t pos = 0;

    while (pos < code_size && cmd != kHlt) {
        for (int i = 0; i < processor_info->stack.size; i++) {
            printf("%d ", processor_info->stack.data[i]);
        }
        printf("\n");
        cmd = processor_info->code[pos];

        switch (cmd) {
            case (kPush):
                if (pos + 1 < code_size) {
                    arg = processor_info->code[pos + 1];
                    pos += 2; 
                    if (Push_C(&processor_info->stack, arg, open_log_file) != kSuccess) {
                        fprintf(stderr, "Error: PUSH failed.\n");
                        return -1;
                    }

                } else {
                    fprintf(stderr, "Error: Push without argument.\n");
                    return -1;
                }
                break;

            case (kPop):
                pos++;
                if (Pop_C(&processor_info->stack, open_log_file) != kSuccess) {
                    fprintf(stderr, "Error: POP failed.\n");
                    return -1;
                }
                break;

            case (kAdd):
                pos++;
                if (StackOperation(&processor_info->stack, Add_C, open_log_file) != kSuccess) {
                    fprintf(stderr, "Error: ADD failed.\n");
                    return -1;
                }
                break;

            case (kSub):
                pos++;
                if (StackOperation(&processor_info->stack, Sub_C, open_log_file) != kSuccess) {
                    fprintf(stderr, "Error: SUB failed.\n");
                    return -1;
                }

                break;

            case (kMul):
                pos++;
                if (StackOperation(&processor_info->stack, Mul_C, open_log_file) != kSuccess) {
                    fprintf(stderr, "Error: MUL failed.\n");
                    return -1;
                }

                break;

            case (kDiv):
                pos++;
                if (Div_C(&processor_info->stack, open_log_file) != kSuccess) {
                    fprintf(stderr, "Error: DIV failed.\n");
                    return -1;
                }
                break;

            case (kSqrt):
                pos++;
                if (Sqrt_C(&processor_info->stack, open_log_file) != kSuccess) {
                    fprintf(stderr, "Error: SQRT failed.\n");
                    return -1;
                }
                break;

            case (kOut):
                pos++;
                if (Out_C(&processor_info->stack, open_log_file, fout) != kSuccess) {
                    fprintf(stderr, "Error: OUT failed.\n");
                    return -1;
                }
                break;

            case (kPushR):
                if (pos + 1 < code_size) {
                    arg = processor_info->code[pos + 1];
                    pos += 2; 
                    if (PushR_C(processor_info, arg, open_log_file) != kSuccess) {
                        fprintf(stderr, "Error: PUSH failed.\n");
                        return -1;
                    }
                } else {
                    fprintf(stderr, "Error: Push without argument.\n");
                    return -1;
                }
                break;

            case (kPopR):
                if (pos + 1 < code_size) {
                    arg = processor_info->code[pos + 1];
                    pos += 2;
                    if (PopR_C(processor_info, arg, open_log_file) != kSuccess) {
                        fprintf(stderr, "Error: POP failed.\n");
                        return -1;
                    }
                } else {
                    fprintf(stderr, "Error: Pop without argument.\n");
                    return -1;
                }
                break;

            case (kIn):
                pos++;
                scanf("%d", &number);
                StackPush(&processor_info->stack, number, open_log_file);
                break;

            case (kHlt):
                break;

            default:
                fprintf(stderr, "Unknown command: %d.\n", cmd);
                return -1;
        }
    }

    return ProcessorVerify(processor_info, open_log_file);
}