#include <stdio.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../Calculator/StructsEnums.h"
#include "/Users/zarinasharipova/Assembler/HandleFileWork.h"
#include "../Calculator/Parse/ParseCommandLine.h"
#include "DisAsm.h"
#include "HandleLogFile.h"

int main(int argc, const char *argv[]) {
    Files in_out_files = {NULL, NULL, NULL, NULL, NULL};
    ParseErr_t read_write_error = kNoError;
    int err = 0;

    CALL_CHECK_IN_OUT_RETURN(ParseCommandLine(argv, argc, &in_out_files));
    CALL_CHECK_IN_OUT_RETURN(HandleOpenFile(&in_out_files));

    FileInfo file_info = {};

    return HandleDisAsm(&file_info, in_out_files);

    CALL_CHECK_IN_OUT_RETURN(HandleCloseFile(in_out_files));
    return 0;
}