#include "DoCalc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "FileOperations.h"
#include "NcursesWin.h"
#include "ProcessorStructs.h"

WINDOW *win = NULL;

#define DEFAULT_COMMAND -1

int Calculate(FILE *fout, Processor *processor_info, size_t code_size) {
    assert(fout);
    assert(processor_info);

    ProcessorErr_t err = kProcessorSuccess;
    CHECK_PROCESSOR_RETURN(ProcessorVerify(processor_info));

    int cmd = DEFAULT_COMMAND;
   // printf("AAA%zu %d\n", processor_info->instruction_counter, processor_info->code[processor_info->instruction_counter]);

    // win = GraphicsInit();
    // if (!win) {
    //     fprintf(stderr, "Error: failed to initialize ncurses window\n");
    //     return kProcessorFailure;
    // }

    while (processor_info->instruction_counter < code_size && cmd != kHlt) {
        cmd = processor_info->code[processor_info->instruction_counter];

        // printf("...%d %d- \n", cmd, processor_info->instruction_counter);
        if (cmd <= NUM_COMMANDS) {
            CHECK_ERROR_RETURN(commands_processor[cmd].func(processor_info)); //
        } else {
            printf("Error - no such command.");
            return kProcessorFailure;
        }
    }

    // endwin();
    //GraphicsDestroy(&win);
    return kProcessorSuccess;
}