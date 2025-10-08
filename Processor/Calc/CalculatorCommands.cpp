#include "CalculatorCommands.h"

#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "StructsEnums.h"
#include "StackFunctions.h"
#include "/Users/zarinasharipova/Assembler/AssemblerEnums.h"

static double SqrtFind(Stack_t n);

ProcessorErr_t StackOperation(Stack_Info *stk, Stack_t (*operation)(Stack_t, Stack_t), int *pointer, FILE *open_log_file) {
    assert(stk);
    assert(operation);
    assert(pointer);
    assert(open_log_file);

    (*pointer)++;
    Stack_t second = 0, first = 0, result = 0;
    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(CheckError(stk, open_log_file));

    CHECK_STACK_RETURN(StackPop(stk, &second, open_log_file));
    CHECK_STACK_RETURN(StackPop(stk, &first,open_log_file));

    result = operation(first, second);
    if (StackPush(stk, result, open_log_file) != kSuccess) {
        fprintf(stderr, "Error: #operation failed.\n"); //
        return kNoCommandFound;
    }

    return kSuccess;
}

ProcessorErr_t Div_C(Stack_Info *stk, int *pointer, FILE *open_log_file) {
    assert(stk);
    assert(pointer);
    assert(open_log_file);

    (*pointer)++;
    Stack_t number1 = 0, number2 = 0;
    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number2, open_log_file));
    CHECK_STACK_RETURN(StackPop(stk, &number1, open_log_file));
    
    if (number2 != 0) {
        err = StackPush(stk, number1 / number2, open_log_file);
        if (err != kSuccess) {
            fprintf(stderr, "Error: DIV failed.\n");
            return kNoCommandFound;
        }
        return kSuccess;
    }

    fprintf(stderr, "Zero number2 entered in div.\n");
    return kZeroNumber;
}

ProcessorErr_t Sqrt_C(Stack_Info *stk, int *pointer, FILE *open_log_file) {
    assert(stk);
    assert(pointer);
    assert(open_log_file);

    (*pointer)++;
    Stack_t number = 0;
    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number, open_log_file));
    if (number >= 0) {
        err = StackPush(stk, (int)sqrt(number), open_log_file);
        if (err != kSuccess) {
            fprintf(stderr, "Error: SQRT failed.\n");
            return kNoCommandFound;
        } 
        return kSuccess;
    }

    fprintf(stderr, "Zero or negative number entered in sqrt.\n");
    return kZeroNumber;
}

ProcessorErr_t Out_C(Stack_Info *stk, int *pointer, FILE *open_file, FILE *open_out_file) {
    assert(stk);
    assert(pointer);
    assert(open_file);
    assert(open_out_file);

    (*pointer)++;
    Stack_t number = 0;
    ProcessorErr_t err = kSuccess;

    CHECK_STACK_RETURN(StackPop(stk, &number, open_file));
    fprintf(open_out_file, "" MY_SPEC " \n", number);

    return kSuccess;
}

ProcessorErr_t Push_C(Processor *processor_info, int *pointer, FILE *open_log_file) {
    assert(processor_info);
    assert(pointer);
    assert(open_log_file);

    Stack_t arg = 0;
    if (*pointer + 1 < processor_info->instruction_counter) {
        arg = processor_info->code[*pointer + 1];
        (*pointer) += 2; 

        if (StackPush(&processor_info->stack, arg, open_log_file) != kSuccess) {
            fprintf(stderr, "Error: PUSH failed.\n");
            return kNoCommandFound;
        }

    } else {
        fprintf(stderr, "Error: Push without argument.\n");
        return kNoArgumentWritten;
    }

    return kSuccess;
}

ProcessorErr_t Pop_C(Stack_Info *stk, int *pointer, Stack_t *number_pop, FILE *open_log_file) {
    assert(stk);
    assert(pointer);
    assert(open_log_file);

    (*pointer)++;
    if (StackPop(stk, number_pop, open_log_file) != kSuccess) {
        fprintf(stderr, "Error: POP failed.\n");
        return kNoCommandFound;
    }
    
    return kSuccess;
}

ProcessorErr_t PushR_C(Processor *processor_info, int *pointer, FILE *open_file) {
    assert(processor_info);
    assert(pointer);
    assert(open_file);

    if (*pointer + 1 < processor_info->instruction_counter) {
            int reg_code = processor_info->code[*pointer + 1];
            Stack_t number = processor_info->regs[reg_code];
            ProcessorErr_t err = StackPush(&processor_info->stack, number, open_file);
            if (err != kSuccess) {
                fprintf(stderr, "Error: PUSH failed.\n");
                return err;
            }
            *pointer += 2;
            return kSuccess;

    } else {
        fprintf(stderr, "Error: Push without argument.\n");
        return kNoArgumentWritten;
    }
}

ProcessorErr_t PopR_C(Processor *processor_info, int *pointer, FILE *open_file) {
    assert(processor_info);
    assert(pointer);
    assert(open_file);

    if (*pointer + 1 < processor_info->instruction_counter) {
        Stack_t number = 0;
        int pos = processor_info->code[*pointer + 1];
        processor_info->regs[pos] = processor_info->stack.data[processor_info->stack.size - 1];
        ProcessorErr_t err = StackPop(&processor_info->stack, &number, open_file);
        if (err != kSuccess) {
            fprintf(stderr, "Error: POP failed.\n");
            return kNoCommandFound;
        }
        (*pointer) += 2;
        return kSuccess;
    } else {
        fprintf(stderr, "Error: Pop without argument.\n");
        return kNoArgumentWritten;
    }
}

ProcessorErr_t In_C(Processor *processor_info, int *pointer, FILE *open_log_file) {
    assert(processor_info);
    assert(pointer);
    assert(open_log_file);

    (*pointer)++;
    Stack_t number = 0;
    scanf("%d", &number);
    return StackPush(&processor_info->stack, number, open_log_file);
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