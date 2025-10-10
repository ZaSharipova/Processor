#include "StackFunctions.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#include "Canary.h"
#include "StructsEnums.h"
#include "HandleLogFile.h"

static ReallocMode CheckSize(ssize_t size, ssize_t *capacity);

const char *GetErrorString[NUMBER_OF_ERRORS] = {
    "Processor null pointer",
    "Empty stack",
    "Stack null pointer",
    "Size error",
    "Negative Capacity",
    "Negative Size",
    "Wrong canary left value",
    "Wrong canary right value",
    "Hash mismatch. Stack corrupted",
    "No calloc memory",
    "No memory available",
    "Zero number beeing written",
    "Number not written",
    "Command not found",
};

ProcessorErr_t StackCtor(Stack_Info *stk, ssize_t capacity) {
    assert(stk);

    ProcessorErr_t err = kSuccess;

    stk->capacity = capacity;
    if (capacity < 0) {
        STACKDUMP(stk, kNegativeCapacity);
        return kNegativeCapacity;
    }

    stk->size = 0;

#ifdef _CANARY
    CHECK_ERROR_RETURN(MakeCanary(stk));
#endif

#ifndef _CANARY
    stk->data = (Stack_t *) calloc ((size_t)capacity, sizeof(*stk->data));
    if (!stk->data) {
        STACKDUMP(stk, kNoMemory);
        return kNoMemory;
    }
#endif

#ifdef _HASH
    Update_Data_Hash(stk);
#endif

    CHECK_ERROR_RETURN(CheckError(stk));
    return kSuccess;
}

ProcessorErr_t StackPush(Stack_Info *stk, Stack_t value) {
    assert(stk);

    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(CheckError(stk));

    ReallocMode realloc_type = CheckSize(stk->size, &stk->capacity);
    if (realloc_type != kNoChange) {
        CHECK_ERROR_RETURN(StackRealloc(stk, realloc_type));
    }

    stk->data[stk->size++] = value;

#ifdef _HASH
    Update_Data_Hash(stk);
#endif

    CHECK_ERROR_RETURN(CheckError(stk));
    return kSuccess;
}

ProcessorErr_t StackPop(Stack_Info *stk, Stack_t *value) {
    assert(stk);
    assert(value);

    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(CheckError(stk));

    if (stk->size == 0) {
        STACKDUMP(stk, kErrorEmptyStack);
        return kErrorEmptyStack;
    }

    *value = stk->data[--stk->size];
    stk->data[stk->size] = POISON;

#ifdef _HASH
    Update_Data_Hash(stk);
#endif

    CHECK_ERROR_RETURN(CheckError(stk));

    ReallocMode realloc_type = CheckSize(stk->size, &stk->capacity);
    if (realloc_type != kNoChange) {
        CHECK_ERROR_RETURN(StackRealloc(stk, realloc_type));
    }

    CHECK_ERROR_RETURN(CheckError(stk));

    return kSuccess;
}

void FillPoison(Stack_Info *stk) {
    assert(stk);

    ssize_t pos = stk->size;
    while (pos < stk->capacity) {
        stk->data[pos] = POISON; 
        pos++;
    }
}

ProcessorErr_t StackTop(Stack_Info stk, Stack_t *value) {
    assert(value);

    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(CheckError(&stk));
    
    if (stk.size == 0) {
        STACKDUMP(&stk, kErrorEmptyStack);
        return kErrorEmptyStack;
    }

    *value = stk.data[stk.size - 1];
    CHECK_ERROR_RETURN(CheckError(&stk));
    return err;
}

static ReallocMode CheckSize(ssize_t size, ssize_t *capacity) {
    assert(capacity);

    if (size * INCREASE_VALUE > *capacity) {
        return kIncrease;

    } else if (size * DECREASE_VALUE < *capacity && size != 0 && *capacity >= 4) {
        return kDecrease;
    }

    if (*capacity == 0) {
        return kIncreaseZero;
    }

    return kNoChange;
}

ProcessorErr_t StackRealloc(Stack_Info *stk, ReallocMode realloc_type) {
    assert(stk);

    if (realloc_type == kIncrease) {
        stk->capacity *= INCREASE_VALUE;
    } else if (realloc_type == kDecrease) {
        stk->capacity /= DECREASE_VALUE;
    } else {
        stk->capacity = 3;
    }

#ifdef _CANARY
    size_t new_elems = (size_t)stk->capacity + 2;
    Stack_t *ptr = stk->data - 1;
#else
    size_t new_elems = (size_t)stk->capacity;
    Stack_t *ptr = stk->data;
#endif

    Stack_t *realloc_ptr = (Stack_t *) realloc (ptr, ((size_t)new_elems) * sizeof(*realloc_ptr));
    if (realloc_ptr == NULL) {
        STACKDUMP(stk, kNoMemory);
        return kNoMemory;
    }

#ifdef _CANARY
    stk->data = realloc_ptr + 1;
    realloc_ptr[0] = (Stack_t)canary_left; 
    realloc_ptr[stk->capacity + 1] = (Stack_t)canary_right;
#else
    stk->data = realloc_ptr;
#endif

    FillPoison(stk);
    
    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(CheckError(stk));

    return err;
}

ProcessorErr_t StackDtor(Stack_Info *stk) {
    assert(stk);

    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(CheckError(stk));

    stk->size = -1;
    stk->capacity = -1;

#ifdef _CANARY
        stk->create_var_info = {NULL, NULL, 0};
        stk->data_hash = 0;
#endif

#ifdef _HASH
        stk->data_hash = 0;
#endif

#ifndef _DEBUG
        free(stk->data - 1);
#else 
        free(stk->data);
#endif

    return err;
}

void StackDump(Stack_Info stk, const char *func_name, int line, const char *file_from, unsigned int error, const char *stk_name) {
    assert(func_name);
    assert(file_from);
    assert(stk_name);

    unsigned int bit = 1;
    FILE *open_log_file = GetLogFile();
    fprintf(open_log_file, "errors: ");
    for (int i = 0; i < NUMBER_OF_ERRORS; i++) {
        if (error & bit) {
            fprintf(open_log_file, "%s%s%s ", RED(open_log_file), GetErrorString[i], RESET(open_log_file));
        }
        bit <<= 1;
    }

    fprintf(open_log_file, "\nfrom %s, function %s: line %d\n", file_from, func_name, line);

    fprintf(open_log_file, "Stack name: %s\n", stk_name);

#ifdef _DEBUG
        fprintf(open_log_file, "Stack made: %s, function %s: line %zd\n", stk.create_var_info.file_name, \
        stk.create_var_info.func_name, stk.create_var_info.line);
#endif

    fprintf(open_log_file, "STACK[%p] {\n", stk.data);
    fprintf(open_log_file, "  size = %zd\n", stk.size);
    fprintf(open_log_file, "  capacity = %zd\n", stk.capacity);
    fprintf(open_log_file, "  data[%p] {\n", stk.data);

    int pos = 0;
    while(pos < stk.size) {
        fprintf(open_log_file, "  *[%d] = " MY_SPEC "\n", pos, stk.data[pos]);
        pos++;
    }

    while (pos < stk.capacity) {
        fprintf(open_log_file, "   [%d] " MY_SPEC " = POISON\n", pos, stk.data[pos]);
        pos++;
    }

    fprintf(open_log_file, "  }\n");
    fprintf(open_log_file, "}\n");

#ifdef _CANARY
    if (stk.capacity >= 0) {
        fprintf(open_log_file "canary_left = %u (real[%d])\n", stk.data[-1], 0);
        fprintf(open_log_file, "canary_right = %u (real[%zd])\n", 
            stk.data[stk.capacity], stk.capacity + 1);
    }
#endif

#ifdef _HASH
    fprintf(open_log_file, "hash = %u", stk.data_hash);
#endif

}

ProcessorErr_t CheckError(Stack_Info *stk) {
    assert(stk);

    unsigned int error = 0;

    if (stk == NULL) {
        error |= kErrorEmptyStack;
        return (ProcessorErr_t)error;
    }

    if (stk->data == NULL) {
        error |= kErrorStackNullPointer;
    }

    if (stk->size > stk->capacity) {
        error |= kSizeError;
    }

    if (stk->size < 0) {
        error |= kNegativeSize;
    }
    
    if (stk->capacity < 0) {
        error |= kNegativeCapacity;
    }

#ifdef _CANARY
    if (canary_left != (uint32_t)stk->data[-1]) {
       error |= kWrongCanaryLeft;
    }

    if (canary_right != (uint32_t)stk->data[stk->capacity]) {
        error |= kWrongCanaryRight;
    }

#endif

#ifdef _HASH
    if (!Check_Data_Hash(stk)) {
        error |= kWrongHash;
    }
#endif

    if (error != 0) {
        STACKDUMP(stk, error);
        return kFailure;
    }

    return kSuccess;

}
