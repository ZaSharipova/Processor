#ifndef PARSE_COMMAND_LINE_H_
#define PARSE_COMMAND_LINE_H_

#include <stdio.h>

#include "StructsEnums.h"

#define LOG_MODE "--log"
#define IN_MODE "--in"
#define OUT_MODE "--out"

#define CALL_CHECK_IN_OUT_RETURN(call) \
    read_write_error = (call); \
    if (read_write_error != kNoError) { \
        return read_write_error; \
    }

ParseErr_t ParseCommandLine(const char **argv, int argc, Files *in_out_files);

#endif //PARSE_COMMAND_LINE_H_