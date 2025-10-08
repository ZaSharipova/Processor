#ifndef HANDLE_FILE_WORK_H_
#define HANDLE_FILE_WORK_H_

#include <stdio.h>

#include "AssemblerEnums.h"
#include "/Users/zarinasharipova/Calculator/StructsEnums.h"

#define CHECK_ERROR_RETURN(call, err) \
    if (err != kNoError) { \
        free(command); \
        free(arg_str); \
        return err; \
    }

PossibleErrorsAsm HandleBufRead(Files *in_out_files, FileInfo *file_info);
void WriteCommand(FILE *output, int command, int has_arg, int arg);
int HandleParse(const char *line, FileInfo *file_info, int **buf_out, int *num_args);
int HandleAsm(FileInfo *file_info, Files in_out_files);
void HandleWriteCommands(FILE *output, FileInfo *file_info, int *buf_out);

#endif //HANDLE_FILE_WORK_H_