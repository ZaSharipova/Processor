#include "ProcessorFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"
#include "StackFunctions.h"
#include "HandleLogFile.h"

ProcessorErr_t ProcessorCtor(Processor *processor_info, ssize_t capacity) {
    assert(processor_info);

    processor_info->instruction_counter = 0;

    return StackCtor(&processor_info->stack, capacity);
}

ProcessorErr_t ProcessorVerify(Processor *processor_info) {
    assert(processor_info);

    unsigned int error = 0;
    if (processor_info == NULL) {
        error |= kProcessorNullPointer;
        PROCESSORDUMP(&processor_info);
        return kProcessorNullPointer;
    }

    error |= (unsigned int)CheckError(&(processor_info->stack));
    return (ProcessorErr_t)error;
}

void ProcessorDump(Processor *processor_info, const char *func_name, int line, const char *file_from, const char *processor_name) {
    assert(processor_info);
    assert(func_name);
    assert(file_from);
    assert(processor_name);

    FILE *open_log_file = GetLogFile();

    fprintf(open_log_file, "%s%s%s ", RED(open_log_file), "Processor null pointer.", RESET(open_log_file));
    fprintf(open_log_file, "\nfrom %s, function %s: line %d\n", file_from, func_name, line);

    fprintf(open_log_file, "Processor name: %s\n", processor_name);
}

ProcessorErr_t ProcessorDtor(Processor *processor_info) {
    assert(processor_info);

    ProcessorErr_t err = kSuccess;
    CHECK_ERROR_RETURN(ProcessorVerify(processor_info));

    StackDtor(&processor_info->stack);
    processor_info = NULL;

    return kSuccess;
}