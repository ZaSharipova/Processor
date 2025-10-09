#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "HandleCalc.h"
#include "/Users/zarinasharipova/Assembler/FileOperations.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "ParseCommandLine.h"

int main(int argc, const char *argv[]) {
    Files in_out_files = {NULL, NULL, NULL, NULL};
    ParseErr_t read_write_error = kNoError;
    int err = kSuccess;

    CALL_CHECK_IN_OUT_RETURN(ParseCommandLine(argv, argc, &in_out_files));
    CALL_CHECK_IN_OUT_RETURN(HandleOpenFile(&in_out_files));

    INIT_PROCESSOR_INFO(processor_info);

    CHECK_ERROR_AND_CLOSE_FILE_RETURN((int)ProcessorCtor(&processor_info, 1, in_out_files.open_log_file));

    size_t code_size = 0;
    CHECK_ERROR_AND_CLOSE_FILE_RETURN(Read(in_out_files.open_in_file, &processor_info.code, &code_size));
    processor_info.code_size = code_size; //

    Calculate(in_out_files.open_out_file, &processor_info, (int)code_size, in_out_files.open_log_file);

    CHECK_ERROR_AND_CLOSE_FILE_RETURN((int)ProcessorDtor(in_out_files.open_log_file, &processor_info)); 

    return HandleCloseFile(in_out_files);
}