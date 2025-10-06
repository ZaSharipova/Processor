#include "Canary.h"

#include <assert.h>
#include <stdio.h>

#include "StructsEnums.h"

static uint8_t RandomNonZeroByte() {
    uint8_t b = 0;
    while (b == 0) {
        b = (uint8_t)(rand() & 0xFF);
    }
    return b;
}

static uint32_t InitCanary(void) {
    srand((unsigned int)time(NULL));
    
    uint32_t result = 0;
    for (int i = 0; i < 4; ++i) {
        result <<= 8;
        result |= RandomNonZeroByte();
    }
    
    return result;
}

uint32_t canary_left = 0;
uint32_t canary_right = 0;

void InitCanaries(void) {
    srand((unsigned int)time(NULL));
    canary_left = InitCanary();
    canary_right = InitCanary();
}

StackErr_t MakeCanary(Stack_Info *stk, FILE *open_log_file) {
    assert(stk);
    assert(open_log_file);

#ifdef _CANARY
    Stack_t *stack_ptr = (Stack_t *) calloc ((size_t)stk->capacity + 2, sizeof(*stack_ptr));
    if (stack_ptr == NULL) {
        STACKDUMP(open_log_file, stk, kNoMemory);
        return kNoMemory;
    }

    stk->data = stack_ptr + 1;
    stk->data[-1] = (Stack_t)canary_left;
    stk->data[stk->capacity] = (Stack_t)canary_right;

    return kSuccess;
#endif
    return kSuccess;
}