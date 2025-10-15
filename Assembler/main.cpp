#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "HandleFileWork.h"
#include "FileOperations.h"
#include "ParseCommandLine.h"
#include "HandleLogFile.h"

int main(int argc, const char *argv[]) {
    Files in_out_files = {NULL, NULL, NULL, NULL};

    ParseErr_t read_write_error = kNoError;

    CALL_CHECK_IN_OUT_RETURN(ParseCommandLine(argv, argc, &in_out_files));
    SetLogFile(in_out_files.log_file);
    CALL_CHECK_IN_OUT_RETURN(HandleOpenFile(&in_out_files));

    int labels[10] = {0};
    for (int i = 0; i < 10; i++) {
        labels[i] = -1;
    } //
    
    FileInfo file_info = {};

    AsmError err = PrepareToAssemble(&in_out_files, &file_info, labels);
    if (err != kNoAsmError) {
        return err;
    }

    err = HandleAsm(&file_info, &in_out_files, labels);
    if (err != kNoAsmError) {
        return err;
    }

    CALL_CHECK_IN_OUT_RETURN(HandleCloseFile(&in_out_files));
    return kNoError;
}