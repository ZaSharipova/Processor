#include "CalculatorCommands.h"

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include "StructsEnums.h"
#include "StackFunctions.h"
#include "AssemblerEnums.h"
#include "ProcessorFunctions.h"
#include "DoLogFile.h"

static double SqrtFind(Stack_t n);

#define PRINT_DOCALC_ERROR(operation) fprintf(GetLogFile(), "Error: %s failed.\n", #operation); //

#define GENERATE_ARIPHM_FUNC(name, op)                                                              \
    ProcessorErr_t name##_C(Processor *processor_info) {                                            \
        assert(processor_info);                                                                     \
                                                                                                    \
        processor_info->instruction_counter++;                                                      \
        Stack_t second = 0, first = 0;                                                              \
        ProcessorErr_t err = kProcessorSuccess;                                                     \
        CHECK_ERROR_RETURN(StackErrToProcessorErr(CheckError(&processor_info->stack)));             \
                                                                                                    \
        CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, &second)));  \
        CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, &first)));   \
        Stack_t result = first op second;                                                           \
        if (StackPush(&processor_info->stack, result) != kStackSuccess) {                           \
            PRINT_DOCALC_ERROR(operation);                                                          \
            return kNoCommandFound;                                                                 \
        }                                                                                           \
                                                                                                    \
        return kProcessorSuccess;                                                                   \
    }

//todo проверять деление на 0

GENERATE_ARIPHM_FUNC(Add, +);
GENERATE_ARIPHM_FUNC(Sub, -);
GENERATE_ARIPHM_FUNC(Mul, *);
GENERATE_ARIPHM_FUNC(Div, /);

#undef GENERATE_ARIPHM_FUNC

#define GENERATE_J_FUNC(name, op)                                                                                        \
    ProcessorErr_t C_J##name(Processor *processor_info) {                                                                \
        assert(processor_info);                                                                                          \
                                                                                                                         \
        ProcessorErr_t err = kProcessorSuccess;                                                                          \
        Stack_t number1 = 0, number2 = 0;                                                                                \
        CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, &number2)));                      \
        CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, &number1)));                      \
                                                                                                                         \
        if (number1 op number2) {                                                                                        \
            processor_info->instruction_counter = (size_t)processor_info->code[processor_info->instruction_counter + 1]; \   
        } else {                                                                                                         \
            processor_info->instruction_counter += 2;                                                                    \
        }                                                                                                                \
        return kProcessorSuccess;                                                                                        \
    }

GENERATE_J_FUNC(B, <);
GENERATE_J_FUNC(BE, <=);
GENERATE_J_FUNC(A, >);
GENERATE_J_FUNC(AE, >=);
GENERATE_J_FUNC(E, ==);
GENERATE_J_FUNC(NE, !=);

#undef GENERATE_J_FUNC

ProcessorErr_t Sqrt_C(Processor *processor_info) {
    assert(processor_info);

    processor_info->instruction_counter++;
    Stack_t number = 0;
    ProcessorErr_t err = kProcessorSuccess;
    CHECK_PROCESSOR_RETURN((ProcessorErr_t)StackPop(&processor_info->stack, &number));

    if (number >= 0) {
        err = (ProcessorErr_t)StackPush(&processor_info->stack, (int)round(SqrtFind((number))));
        if (err != kProcessorSuccess) {
            fprintf(GetLogFile(), "Error: SQRT failed.\n");
            return kNoCommandFound;
        } 
        return kProcessorSuccess;
    }

    fprintf(GetLogFile(), "Zero or negative number entered in sqrt.\n");
    return kZeroNumber;
}

ProcessorErr_t Out_C(Processor *processor_info) {
    assert(processor_info);

    processor_info->instruction_counter++;
    Stack_t number = 0;
    ProcessorErr_t err = kProcessorSuccess;

    CHECK_PROCESSOR_RETURN((ProcessorErr_t)StackPop(&processor_info->stack, &number));
    printf("" STACK_VALUE_MODE " \n", number);
    //printf("Answer is already in a file.\n");

    return kProcessorSuccess;
}

ProcessorErr_t Push_C(Processor *processor_info) {
    assert(processor_info);

    Stack_t arg = 0;
    if (processor_info->instruction_counter + 1 < processor_info->code_size) {
        arg = processor_info->code[processor_info->instruction_counter + 1];
        processor_info->instruction_counter += 2; 

        if (StackPush(&processor_info->stack, arg) != kStackSuccess) {
            fprintf(GetLogFile(), "Error: PUSH failed.\n");
            return kNoCommandFound;
        }

    } else {
        fprintf(GetLogFile(), "Error: PUSH without argument, command: %zu.\n", processor_info->instruction_counter);
        return kNoArgumentWritten;
    }

    return kProcessorSuccess;
}

ProcessorErr_t Pop_C(Processor *processor_info, Stack_t *number_pop) {
    assert(processor_info);
    assert(number_pop);

    processor_info->instruction_counter++;
    if (StackPop(&processor_info->stack, number_pop) != kStackSuccess) {
        fprintf(GetLogFile(), "Error: POP failed.\n");

        return kNoCommandFound;
    }
    
    return kProcessorSuccess;
}

ProcessorErr_t PushR_C(Processor *processor_info) {
    assert(processor_info);

    if (processor_info->instruction_counter + 1 < processor_info->code_size) {
            int reg_code = processor_info->code[processor_info->instruction_counter + 1];
            Stack_t number = processor_info->regs[reg_code];

            StackErr_t err = StackPush(&processor_info->stack, number);
            if (err != kStackSuccess) {
                fprintf(GetLogFile(), "Error: PUSH failed.\n");
                return (ProcessorErr_t)err;
            }

            processor_info->instruction_counter += 2;
            return kProcessorSuccess;

    } else {
        fprintf(GetLogFile(), "Error: PUSHR without argument. Command: %zu.\n", processor_info->instruction_counter);
        return kNoArgumentWritten;
    }
}

ProcessorErr_t PopR_C(Processor *processor_info) {
    assert(processor_info);

    if (processor_info->instruction_counter + 1 < processor_info->code_size) {
        Stack_t number = 0;
        int pos = processor_info->code[processor_info->instruction_counter + 1];
        processor_info->regs[pos] = processor_info->stack.data[processor_info->stack.size - 1];

        StackErr_t err = StackPop(&processor_info->stack, &number);
        if (err != kStackSuccess) {
            fprintf(GetLogFile(), "Error: POPR failed.\n");
            return kNoCommandFound;
        }

        processor_info->instruction_counter += 2;
        return kProcessorSuccess;

    } else {
        fprintf(GetLogFile(), "Error: POP without argument. Command: %zu.\n", processor_info->instruction_counter);
        return kNoArgumentWritten;
    }
}

ProcessorErr_t Jmp_C(Processor *processor_info) {
    assert(processor_info);

    processor_info->instruction_counter = (size_t)processor_info->code[processor_info->instruction_counter + 1];
    //getchar();
    
    return kProcessorSuccess;
}

ProcessorErr_t In_C(Processor *processor_info) {
    assert(processor_info);

    processor_info->instruction_counter++;

    Stack_t number = 0;
    int err = scanf("%d", &number);
    if (err <= 0) {
        fprintf(GetLogFile(), "Scanf error.\n");
        return kProcessorFailure;
    }

    return (ProcessorErr_t)StackPush(&processor_info->stack, number);
}

ProcessorErr_t Call_C(Processor *processor_info) {
    assert(processor_info);

    ProcessorErr_t err = kProcessorSuccess;
    CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPush(&processor_info->call_array, (Stack_t)processor_info->instruction_counter + 2)));
    processor_info->instruction_counter = (size_t)processor_info->code[processor_info->instruction_counter + 1];

    return kProcessorSuccess;
}

ProcessorErr_t Ret_C(Processor *processor_info) {
    assert(processor_info);

    ProcessorErr_t err = kProcessorSuccess;
    Stack_t number_pop = 0;
    CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->call_array, &number_pop)));
    processor_info->instruction_counter = (size_t)number_pop;
    return kProcessorSuccess;
}

ProcessorErr_t PushM_C(Processor *processor_info) {
    assert(processor_info);

    ProcessorErr_t err = kProcessorSuccess;
    processor_info->instruction_counter++;

    Stack_t reg_index = (int)processor_info->code[processor_info->instruction_counter];
    Stack_t ram_index = processor_info->regs[reg_index];
    CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPush(&processor_info->stack, processor_info->ram[ram_index])));

    processor_info->instruction_counter++;
    return kProcessorSuccess;
}

ProcessorErr_t PopM_C(Processor *processor_info) {
    assert(processor_info);

    ProcessorErr_t err = kProcessorSuccess;
    processor_info->instruction_counter++;

    Stack_t reg_index = processor_info->code[processor_info->instruction_counter];
    Stack_t ram_index = processor_info->regs[reg_index];
    Stack_t value = 0;

    CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, &value)));
    processor_info->ram[ram_index] = value;

    processor_info->instruction_counter++;
    return kProcessorSuccess;
}

ProcessorErr_t Draw_C(Processor *processor_info) {
    assert(processor_info);

    for (size_t i = 0; i < RAM_SIZE; i++) {
        if (processor_info->ram[i] == 0) {
            printf("..");
        } else {
            printf("++");
        }

        if ((i + 1) % (size_t) sqrt(RAM_SIZE) == 0) {
            printf("\n");
        }
    }

    processor_info->instruction_counter ++;
    return kProcessorSuccess;
}

ProcessorErr_t OutC_C(Processor *processor_info) {
    assert(processor_info);

    
    processor_info->instruction_counter++;
    Stack_t number = 0;
    ProcessorErr_t err = kProcessorSuccess;

    CHECK_PROCESSOR_RETURN((ProcessorErr_t)StackPop(&processor_info->stack, &number));
    printf("%c", number);
    //printf("Answer is already in a file.\n");

    return kProcessorSuccess;
}

ProcessorErr_t Square_C(Processor *processor_info) {
    assert(processor_info);

    ProcessorErr_t err = kProcessorSuccess;
    Stack_t number = 0;
    CHECK_PROCESSOR_RETURN((ProcessorErr_t)StackPop(&processor_info->stack, &number));

    CHECK_PROCESSOR_RETURN((ProcessorErr_t)StackPush(&processor_info->stack, number * number));

    processor_info->instruction_counter++;
    return kProcessorSuccess;
}

ProcessorErr_t Hlt_C(Processor *processor_info) {
    assert(processor_info);

    return kProcessorSuccess;
}

static double SqrtFind(Stack_t n) {
    double left = 0;
    double right = (double)n;

    while (right - left > 1e-9) {
        
        double mid = (left + right) / 2;
        if (mid * mid  > n) {
            right = mid;

        } else {
            left = mid;
        }
    }

    return left;
}