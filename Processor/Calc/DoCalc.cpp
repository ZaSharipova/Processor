#include "DoCalc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "AssemblerEnums.h"
#include "FileOperations.h"
#include "SubsidiaryFunctionsAssembler.h"

#define DEFAULT_COMMAND -1
#define NUM_COMMANDS 24

int Calculate(FILE *fout, Processor *processor_info, size_t code_size) {
    assert(fout);
    assert(processor_info);

    ProcessorErr_t err = kProcessorSuccess;
    CHECK_PROCESSOR_RETURN(ProcessorVerify(processor_info));

    int cmd = DEFAULT_COMMAND;

    while (processor_info->instruction_counter < code_size && cmd != kHlt) {
        cmd = processor_info->code[processor_info->instruction_counter];

        //printf("...%d %d- \n", cmd, processor_info->instruction_counter);
        if (cmd <= NUM_COMMANDS) {
            CHECK_ERROR_RETURN(commands[cmd].func(processor_info));
        } else {
            printf("Error - no such command.");
            return kProcessorFailure;
        }
    }

    return kProcessorSuccess;
}