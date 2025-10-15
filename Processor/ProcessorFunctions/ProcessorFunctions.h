#ifndef PROCESSOR_FUNCTIONS_H_
#define PROCESSOR_FUNCTIONS_H_

#include <stdio.h>

#include "StructsEnums.h"

#ifndef _DEBUG
    #define INIT_PROCESSOR_INFO(name) \
        Processor name = {};
#else 
    #define INIT_PROCESSOR_INFO(name) \
        Processor name = { \
            .stack.create_var_info.file_name = __FILE__, .stack.create_var_info.func_name = __func__, \
            .stack.create_var_info.var = #name, .stack.create_var_info.line = __LINE__} 
#endif

#define CHECK_PROCESSOR_RETURN(call) \
    err = (call); \
    if (err != kProcessorSuccess) { \
        return err; \
    }

ProcessorErr_t StackErrToProcessorErr(StackErr_t err);

ProcessorErr_t ProcessorCtor(Processor *processor_info, ssize_t capacity);
ProcessorErr_t ProcessorVerify(Processor *processor_info);
void ProcessorDump(Processor *processor_info, const char *func_name, int line, const char *file_from, const char *processor_name);
ProcessorErr_t ProcessorDtor(Processor *processor_info);

#define PROCESSORDUMP(processor_name) ProcessorDump(*processor_name, __func__, __LINE__, __FILE__, #processor_name)
#endif //PROCESSOR_FUNCTIONS_H_