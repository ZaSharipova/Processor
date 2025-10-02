#include "CalculatorCommands.h"

#include <assert.h>
#include <stdio.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

double Sqrt_find(Stack_t n) {
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

StackErr_t Mul_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number1 = 0, number2 = 0;
    StackErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number1, open_file));
    CHECK_STACK_RETURN(StackPop(stk, &number2, open_file));
    
    CHECK_STACK_RETURN(StackPush(stk, number1 * number2, open_file));
    return kSuccess;
}

StackErr_t Add_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number1 = 0, number2 = 0;
    StackErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number1, open_file));
    CHECK_STACK_RETURN(StackPop(stk, &number2, open_file));
    
    CHECK_STACK_RETURN(StackPush(stk, number1 + number2, open_file));
    return kSuccess;
}

StackErr_t Div_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number1 = 0, number2 = 0;
    StackErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number1, open_file));
    CHECK_STACK_RETURN(StackPop(stk, &number2, open_file));
    
    if (number2 != 0) {
        CHECK_STACK_RETURN(StackPush(stk, number1 * number2, open_file));
        return kSuccess;
    }

    printf("Zero number2 entered in div.\n");
    return kZeroNumber;
}

StackErr_t Sub_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number1 = 0, number2 = 0;
    StackErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number1, open_file));
    CHECK_STACK_RETURN(StackPop(stk, &number2, open_file));
    
    CHECK_STACK_RETURN(StackPush(stk, number2 - number1, open_file));
    return kSuccess;
}

StackErr_t Sqrt_C(Stack_Info *stk, FILE *open_file) {
    assert(stk);
    assert(open_file);

    Stack_t number = 0;
    StackErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number, open_file));
    if (number >= 0) {
        CHECK_STACK_RETURN(StackPush(stk, (int)Sqrt_find(number), open_file));
        return kSuccess;
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
    StackErr_t err = kSuccess;
    CHECK_STACK_RETURN(StackPop(stk, &number, open_file));
    return kSuccess;
}