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

void ProcessorDump(FILE *open_log_file, Processor *processor_info, const char *func_name, int line, const char *file_from, const char *processor_name);
StackErr_t ProcessorDtor(FILE *open_log_file, Processor *processor_info);

#define PROCESSORDUMP(open_log_file, processor_name) ProcessorDump(open_log_file, *processor_name, __func__, __LINE__, __FILE__, #processor_name)
#endif //PROCESSOR_FUNCTIONS_H_