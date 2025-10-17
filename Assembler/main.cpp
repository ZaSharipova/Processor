#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "HandleFileWork.h"
#include "FileOperations.h"
#include "ParseCommandLine.h"
#include "HandleLogFile.h"
#include "SubsidiaryFunctionsAssembler.h"

int main(int argc, const char *argv[]) {
    Files in_out_files = {NULL, NULL, NULL, NULL};

    ParseErr_t read_write_error = kNoError;

    CALL_CHECK_IN_OUT_RETURN(ParseCommandLine(argv, argc, &in_out_files));
    SetLogFile(in_out_files.log_file);
    CALL_CHECK_IN_OUT_RETURN(HandleOpenFile(&in_out_files));

    Labels labels = {};
    //InitLabels(labels);
    
    FileInfo file_info = {};

    AsmError err = kNoAsmError;
    CALL_CHECK_ASM_RETURN(PrepareToAssemble(&in_out_files, &file_info, &labels));

    CALL_CHECK_ASM_RETURN(DoAsm(&file_info, &in_out_files, &labels));

    read_write_error = CloseLogFile();
    if (read_write_error != kNoError) {
        CALL_CHECK_IN_OUT_RETURN(HandleCloseFile(&in_out_files));
        return read_write_error;
    }

    CALL_CHECK_IN_OUT_RETURN(HandleCloseFile(&in_out_files));
    return kNoError;
}