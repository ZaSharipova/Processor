#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "HandleCalc.h"
#include "../Assembler/HandleFileWork.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "ParseCommandLine.h"

int main(int argc, const char *argv[]) {
    Files in_out_files = {NULL, NULL, NULL, NULL};
    ParseErr_t read_write_error = kNoError;
    int err = 0;

    CALL_CHECK_IN_OUT_RETURN(ParseCommandLine(argv, argc, &in_out_files));
    CALL_CHECK_IN_OUT_RETURN(HandleOpenFile(&in_out_files));

    INIT_PROCESSOR_INFO(processor_info);

    CHECK_ERROR_RETURN((int)StackCtor(&processor_info.stack, 1, in_out_files.open_log_file)); // для processor

    printf("%d\n", 66 * 66 - 4 * 15 * 5);
    Stack_t code_size = Read(in_out_files.open_in_file, &processor_info.code);

    Calculate(in_out_files.open_out_file, &processor_info, (int)code_size, in_out_files.open_log_file);

    CHECK_ERROR_RETURN((int)ProcessorDtor(in_out_files.open_log_file, &processor_info)); 

    CALL_CHECK_IN_OUT_RETURN(HandleCloseFile(in_out_files));
    return 0;
}