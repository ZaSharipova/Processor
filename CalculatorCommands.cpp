#include "CalculatorCommands.h"

#include <assert.h>
#include <stdio.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

static double SqrtFind(Stack_t n);

StackErr_t StackOperation(Stack_Info *stk, Stack_t (*operation)(Stack_t, Stack_t), FILE *file) {
    assert(stk);
    assert(operation);
    assert(file);

    Stack_t second = 0, first = 0, result = 0;
    StackErr_t err = kSuccess;

    CHECK_ERROR_RETURN(StackPop(stk, &second, file));
    CHECK_ERROR_RETURN(StackPop(stk, &first, file));

    result = operation(first, second);

    return StackPush(stk, result, file);
}

StackErr_t Div_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number1 = 0, number2 = 0;
    StackErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number1, open_file));
    CHECK_STACK_RETURN(StackPop(stk, &number2, open_file));
    
    if (number2 != 0) {
        return StackPush(stk, number1 * number2, open_file);
    }

    printf("Zero number2 entered in div.\n");
    return kZeroNumber;
}

StackErr_t Sqrt_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number = 0;
    StackErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number, open_file));
    if (number >= 0) {
        return StackPush(stk, (int)SqrtFind(number), open_file);
    }

    printf("Zero or negative number entered in sqrt.\n");
    return kZeroNumber;
}

StackErr_t Out_C(Stack_Info *stk, FILE *open_file, FILE *open_out_file) {
    assert(stk);
    assert(open_file);

    Stack_t number = 0;
    StackErr_t err = kSuccess;

    CHECK_STACK_RETURN(StackPop(stk, &number, open_file));
    fprintf(open_out_file, " " MY_SPEC " \n", number);

    return kSuccess;
}

StackErr_t Push_C(Stack_Info *stk, Stack_t arg, FILE *open_file) {
    assert(stk);
    assert(arg);
    assert(open_file);

    return StackPush(stk, arg, open_file);
}

StackErr_t Pop_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number = 0;
    return StackPop(stk, &number, open_file);
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