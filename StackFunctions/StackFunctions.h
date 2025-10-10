#ifndef STACK_FUNCTIONS_H_
#define STACK_FUNCTIONS_H_

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "StructsEnums.h"

#define RED_COLOR "\033[1;31m"
#define RESET_COLOR "\033[0m"
#define RED(stream)   (isatty(fileno(stream)) ? RED_COLOR : "")
#define RESET(stream) (isatty(fileno(stream)) ? RESET_COLOR : "")

#define NUMBER_OF_ERRORS 14
#define POISON 1e6
#define INCREASE_VALUE 2
#define DECREASE_VALUE 4

#define CHECK_ERROR_RETURN(call) \
    err = (call); \
    if (err != 0) { \
        return err; \
    }

#define STACKDUMP(stk_name, err) StackDump(*stk_name, __func__, __LINE__, __FILE__, err, #stk_name)

#ifndef _DEBUG
    #define INIT_STACK(name) \
        Stack_Info name = {};
#else 
    #define INIT_STACK(name) \
        Stack_Info name = { \
            .create_var_info.file_name = __FILE__, .create_var_info.func_name = __func__, \
            .create_var_info.var = #name, .create_var_info.line = __LINE__} 
#endif

ProcessorErr_t StackCtor(Stack_Info *stk, ssize_t capacity);
ProcessorErr_t StackPush(Stack_Info *stk, Stack_t value);
ProcessorErr_t StackPop(Stack_Info *stk, Stack_t *value);
ProcessorErr_t StackRealloc(Stack_Info *stk, ReallocMode realloc_type);
ProcessorErr_t StackDtor(Stack_Info *stk);
void StackDump(Stack_Info stk, const char *func_name, int line, const char *file_from, unsigned int error, const char *stk_name);
ProcessorErr_t CheckError(Stack_Info *stk);


#endif //STACK_FUNCTIONS_H_