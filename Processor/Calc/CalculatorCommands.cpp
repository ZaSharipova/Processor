#include "CalculatorCommands.h"

#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

static double SqrtFind(Stack_t n);

ProcessorErr_t StackOperation(Stack_Info *stk, Stack_t (*operation)(Stack_t, Stack_t), FILE *open_log_file) {
    assert(stk);
    assert(operation);
    assert(open_log_file);

    Stack_t second = 0, first = 0, result = 0;
    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(CheckError(stk, open_log_file));

    CHECK_STACK_RETURN(StackPop(stk, &second, open_log_file));
    CHECK_STACK_RETURN(StackPop(stk, &first,open_log_file));

    result = operation(first, second);

    return StackPush(stk, result, open_log_file);
}

ProcessorErr_t Div_C(Stack_Info *stk, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

    Stack_t number1 = 0, number2 = 0;
    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number2, open_log_file));
    CHECK_STACK_RETURN(StackPop(stk, &number1, open_log_file));
    
    if (number2 != 0) {
        return StackPush(stk, number1 / number2, open_log_file);
    }

    printf("Zero number2 entered in div.\n");
    return kZeroNumber;
}

ProcessorErr_t Sqrt_C(Stack_Info *stk, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

    Stack_t number = 0;
    ProcessorErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number, open_log_file));
    if (number >= 0) {
        return StackPush(stk, (int)sqrt(number), open_log_file);
    }

    printf("Zero or negative number entered in sqrt.\n");
    return kZeroNumber;
}

ProcessorErr_t Out_C(Stack_Info *stk, FILE *open_file, FILE *open_out_file) {
    assert(stk);
    assert(open_file);

    Stack_t number = 0;
    ProcessorErr_t err = kSuccess;

    CHECK_STACK_RETURN(StackPop(stk, &number, open_file));
    fprintf(open_out_file, "" MY_SPEC " \n", number);

    return kSuccess;
}

ProcessorErr_t Push_C(Stack_Info *stk, Stack_t arg, FILE *open_file) {
    assert(stk);
    assert(open_file);

    return StackPush(stk, arg, open_file);
}

ProcessorErr_t Pop_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number = 0;
    return StackPop(stk, &number, open_file);
}

ProcessorErr_t PushR_C(Processor *processor_info, int pos, FILE *open_file) {
    assert(processor_info);
    assert(open_file);

    Stack_t number = processor_info->regs[pos];
    return StackPush(&processor_info->stack, number, open_file);
}

ProcessorErr_t PopR_C(Processor *processor_info, int pos, FILE *open_file) {
    assert(processor_info);
    assert(open_file);

    Stack_t number = 0;
    processor_info->regs[pos] = processor_info->stack.data[processor_info->stack.size - 1];
    return StackPop(&processor_info->stack, &number, open_file);
}

ProcessorErr_t In_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number = scanf("%d", &number);
    return Push_C(stk, number, open_file);
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