#include "CalculatorCommands.h"

#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "StructsEnums.h"
#include "StackFunctions.h"
#include "AssemblerEnums.h"

static double SqrtFind(Stack_t n);

ProcessorErr_t StackOperation(Processor *processor_info, Stack_t (*operation)(Stack_t, Stack_t)) {
    assert(processor_info);
    assert(operation);

    processor_info->instruction_counter++;
    Stack_t second = 0, first = 0, result = 0;
    ProcessorErr_t err = kSuccess;

    CHECK_ERROR_RETURN(CheckError(&processor_info->stack));

    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &second));
    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &first));

    result = operation(first, second);
    if (StackPush(&processor_info->stack, result) != kSuccess) {
        fprintf(stderr, "Error: #operation failed.\n"); //
        return kNoCommandFound;
    }

    return kSuccess;
}

ProcessorErr_t Div_C(Processor *processor_info) {
    assert(processor_info);

    processor_info->instruction_counter++;
    Stack_t number1 = 0, number2 = 0;
    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number2));
    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number1));
    
    if (number2 != 0) {
        err = StackPush(&processor_info->stack, number1 / number2);
        if (err != kSuccess) {
            fprintf(stderr, "Error: DIV failed.\n");
            return kNoCommandFound;
        }
        return kSuccess;
    }

    fprintf(stderr, "Zero number2 entered in div.\n");
    return kZeroNumber;
}

ProcessorErr_t Sqrt_C(Processor *processor_info) {
    assert(processor_info);

    processor_info->instruction_counter++;
    Stack_t number = 0;
    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number));

    if (number >= 0) {
        err = StackPush(&processor_info->stack, (int)sqrt(number));
        if (err != kSuccess) {
            fprintf(stderr, "Error: SQRT failed.\n");
            return kNoCommandFound;
        } 
        return kSuccess;
    }

    fprintf(stderr, "Zero or negative number entered in sqrt.\n");
    return kZeroNumber;
}

ProcessorErr_t Out_C(Processor *processor_info, FILE *open_out_file) {
    assert(processor_info);
    assert(open_out_file);

    processor_info->instruction_counter++;
    Stack_t number = 0;
    ProcessorErr_t err = kSuccess;

    CHECK_STACK_RETURN(StackPop(&processor_info->stack, &number));
    fprintf(open_out_file, "" MY_SPEC " \n", number);

    return kSuccess;
}

ProcessorErr_t Push_C(Processor *processor_info) {
    assert(processor_info);

    Stack_t arg = 0;
    if (processor_info->instruction_counter + 1 < processor_info->code_size) {
        arg = processor_info->code[processor_info->instruction_counter + 1];
        processor_info->instruction_counter += 2; 

        if (StackPush(&processor_info->stack, arg) != kSuccess) {
            fprintf(stderr, "Error: PUSH failed.\n");
            return kNoCommandFound;
        }

    } else {
        fprintf(stderr, "Error: Push without argument, command: %d.\n", processor_info->instruction_counter);
        return kNoArgumentWritten;
    }

    return kSuccess;
}

ProcessorErr_t Pop_C(Processor *processor_info, Stack_t *number_pop) {
    assert(processor_info);
    assert(number_pop);

    processor_info->instruction_counter++;
    if (StackPop(&processor_info->stack, number_pop) != kSuccess) {
        fprintf(stderr, "Error: POP failed.\n");
        return kNoCommandFound;
    }
    
    return kSuccess;
}

ProcessorErr_t PushR_C(Processor *processor_info) {
    assert(processor_info);

    if (processor_info->instruction_counter + 1 < processor_info->code_size) {
            int reg_code = processor_info->code[processor_info->instruction_counter + 1];
            Stack_t number = processor_info->regs[reg_code];

            ProcessorErr_t err = StackPush(&processor_info->stack, number);
            if (err != kSuccess) {
                fprintf(stderr, "Error: PUSH failed.\n");
                return err;
            }

            processor_info->instruction_counter += 2;
            return kSuccess;

    } else {
        fprintf(stderr, "Error: Push without argument. Command: %d.\n", processor_info->instruction_counter);
        return kNoArgumentWritten;
    }
}

ProcessorErr_t PopR_C(Processor *processor_info) {
    assert(processor_info);

    if (processor_info->instruction_counter + 1 < processor_info->code_size) {
        Stack_t number = 0;
        int pos = processor_info->code[processor_info->instruction_counter + 1];
        processor_info->regs[pos] = processor_info->stack.data[processor_info->stack.size - 1];

        ProcessorErr_t err = StackPop(&processor_info->stack, &number);
        if (err != kSuccess) {
            fprintf(stderr, "Error: POP failed.\n");
            return kNoCommandFound;
        }

        processor_info->instruction_counter += 2;
        return kSuccess;

    } else {
        fprintf(stderr, "Error: Pop without argument. Command: %d.\n", processor_info->instruction_counter);
        return kNoArgumentWritten;
    }
}

ProcessorErr_t Jmp_C(Processor *processor_info) {
    assert(processor_info);

    processor_info->instruction_counter = processor_info->code[processor_info->instruction_counter + 1];
    getchar();
    
    return kSuccess;
}

ProcessorErr_t In_C(Processor *processor_info) {
    assert(processor_info);

    processor_info->instruction_counter++;
    Stack_t number = 0;
    scanf("%d", &number);

    return StackPush(&processor_info->stack, number);
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