#include "ProcessorFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"
#include "StackFunctions.h"
#include "DoLogFile.h"

ProcessorErr_t StackErrToProcessorErr(StackErr_t err) {
    switch (err) {
        case kStackSuccess: return kProcessorSuccess;
        case kNoAllocMemory: return kNoMemory;
        case kErrorEmptyStack:
        case kErrorStackNullPointer: return kProcessorNullPointer;
        case kStackFailure: return kProcessorFailure;
        case kSizeError:
        case kNegativeSize:
        case kNegativeCapacity: 
            return kZeroNumber;
        case kWrongCanaryLeft:
        case kWrongCanaryRight:
        case kWrongHash: return kProcessorFailure;

        default: return kProcessorFailure;
    }
}

ProcessorErr_t ProcessorCtor(Processor *processor_info, ssize_t capacity) {
    assert(processor_info);

    processor_info->instruction_counter = 0;

    StackCtor(&processor_info->call_array, capacity);

    for (size_t i = 0; i < REGS_SIZE; i++) {
        processor_info->regs[i] = 0;
    }

    for (size_t i = 0; i < RAM_SIZE; i++) {
        processor_info->ram[i] = 0;
    }

    return (ProcessorErr_t)StackCtor(&processor_info->stack, capacity);
}

// const char *ProcessorErrorString[] = {
//     "Processor: null pointer",
//     "Processor: no memory",
//     "Processor: zero number written",
//     "Processor: number not written",
//     "Processor: command not found",
//     "Processor: argument not written"
// };

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

    ProcessorErr_t err = kProcessorSuccess;
    CHECK_ERROR_RETURN(ProcessorVerify(processor_info));

    free(processor_info->code);
    StackDtor(&processor_info->call_array);
    StackDtor(&processor_info->stack);
    processor_info = NULL;

    return kProcessorSuccess;
}