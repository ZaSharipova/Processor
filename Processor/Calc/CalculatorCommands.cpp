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
const int DIVZERO = -INT_MAX; //

#define PRINT_DOCALC_ERROR(operation) fprintf(GetLogFile(), "Error: %s failed.\n", #operation); //

ProcessorErr_t DoCalc(Processor *processor_info, Stack_t (*operation)(Stack_t, Stack_t)) {
    assert(processor_info);
    assert(operation);

    processor_info->instruction_counter++;
    Stack_t second = 0, first = 0, result = 0;
    ProcessorErr_t err = kProcessorSuccess;

    CHECK_ERROR_RETURN(StackErrToProcessorErr(CheckError(&processor_info->stack)));

    CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, &second)));
    CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, &first)));

    result = operation(first, second);
    if (StackPush(&processor_info->stack, result) != kStackSuccess) {
        PRINT_DOCALC_ERROR(operation);
        return kNoCommandFound;
    }

    return kProcessorSuccess;
}

// ProcessorErr_t Div_C(Processor *processor_info) {
//     assert(processor_info);

//     processor_info->instruction_counter++;
//     Stack_t number1 = 0, number2 = 0;
//     ProcessorErr_t err = kSuccess;
//     CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number2));
//     CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number1));
    
//     if (number2 != 0) {
//         err = StackPush(&processor_info->stack, number1 / number2);
//         if (err != kSuccess) {
//             fprintf(stderr, "Error: DIV failed.\n");
//             return kNoCommandFound;
//         }
//         return kSuccess;
//     }

//     fprintf(stderr, "Zero number2 entered in div.\n");
//     return kZeroNumber;
// }

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

ProcessorErr_t Out_C(Processor *processor_info, FILE *open_out_file) {
    assert(processor_info);
    assert(open_out_file);

    processor_info->instruction_counter++;
    Stack_t number = 0;
    ProcessorErr_t err = kProcessorSuccess;

    CHECK_PROCESSOR_RETURN((ProcessorErr_t)StackPop(&processor_info->stack, &number));
    fprintf(open_out_file, "" STACK_VALUE_MODE " \n", number);
    printf("Answer is already in a file.\n");

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

ProcessorErr_t PopTwoNumbers(Processor *processor_info, Stack_t *number1, Stack_t *number2) {
    assert(processor_info);
    assert(number1);
    assert(number2);
    ProcessorErr_t err = kProcessorSuccess;
    CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, number2)));
    CHECK_PROCESSOR_RETURN(StackErrToProcessorErr(StackPop(&processor_info->stack, number1)));

    return kProcessorSuccess;
}

void DoJump(Processor *processor_info, bool do_jump) {
    assert(processor_info);

    if (do_jump) {
        processor_info->instruction_counter = (size_t)processor_info->code[processor_info->instruction_counter + 1]; 

    } else {
        processor_info->instruction_counter += 2;
    }
    //getchar();
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

ProcessorErr_t ROut_C(Processor *processor_info) {
    assert(processor_info);

    for (size_t i = 0; i < RAM_SIZE; i++) {
        if (processor_info->ram[i] == 0) {
            printf("..");
        } else {
            printf("++");
        }
        if ((i + 1) % (size_t) sqrt(RAM_SIZE) == 0)
            printf("\n");

    }

    processor_info->instruction_counter ++;
    return kProcessorSuccess;
}

Stack_t Add_C(Stack_t a, Stack_t b) { 
    return a + b; 
}

Stack_t Sub_C(Stack_t a, Stack_t b) { 
    return a - b; 

}

Stack_t Mul_C(Stack_t a, Stack_t b) { 
    return a * b; 
}

Stack_t Div_C(Stack_t a, Stack_t b) {
    if (b == 0) {
        fprintf(GetLogFile(), "Zero number2 entered in div.\n");
        return DIVZERO;

    } else {
        return a / b;
    }

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