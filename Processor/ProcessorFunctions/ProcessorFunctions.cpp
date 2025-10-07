#include "ProcessorFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"
#include "StackFunctions.h"

ProcessorErr_t ProcessorCtor(Processor *processor_info, ssize_t capacity, FILE *open_log_file) {
    assert(processor_info);
    assert(open_log_file);

    processor_info->instruction_counter = 0;

    return StackCtor(&processor_info->stack, capacity, open_log_file);
}

ProcessorErr_t ProcessorVerify(Processor *processor_info, FILE *open_log_file) {
    assert(processor_info);
    assert(open_log_file);

    unsigned int error = 0;
    if (processor_info == NULL) {
        error |= kProcessorNullPointer;
        PROCESSORDUMP(open_log_file, &processor_info);
        return kProcessorNullPointer;
    }

    error |= (unsigned int)CheckError(&(processor_info->stack), open_log_file);
    return (ProcessorErr_t)error;
}

void ProcessorDump(FILE *open_log_file, Processor *processor_info, const char *func_name, int line, const char *file_from, const char *processor_name) {
    assert(processor_info);
    assert(open_log_file);
    assert(func_name);
    assert(file_from);
    assert(processor_name);

    fprintf(open_log_file, "%s%s%s ", RED(open_log_file), "Processor null pointer.", RESET(open_log_file));
    fprintf(open_log_file, "\nfrom %s, function %s: line %d\n", file_from, func_name, line);

    fprintf(open_log_file, "Processor name: %s\n", processor_name);
}

ProcessorErr_t ProcessorDtor(FILE *open_log_file, Processor *processor_info) {
    assert(processor_info);
    assert(open_log_file);

    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(ProcessorVerify(processor_info, open_log_file));

    StackDtor(&processor_info->stack, open_log_file);
    processor_info = NULL;

    return kSuccess;
}