#ifndef HANDLE_FILE_WORK_H_
#define HANDLE_FILE_WORK_H_

#include <stdio.h>

#include "AssemblerEnums.h"
#include "StructsEnums.h"

#define CHECK_ERROR_RETURN(call, err) \
    if (err != kNoError) { \
        free(command); \
        free(arg_str); \
        return err; \
    }

AsmError HandleBufRead(Files in_out_files, FileInfo *file_info);
void WriteCommand(FILE *output, int command, int has_arg, int arg);
int HandleParse(const char *line, FileInfo *file_info, int **buf_out, int *num_args, int *labels);
int HandleAsm(FileInfo *file_info, Files in_out_files, int *labels);
void HandleWriteCommands(FILE *output, FileInfo *file_info, int *buf_out);

AsmError HandlePreAssemble(Files files, FileInfo *file_info, int *labels);

AsmError PreAssemble(FileInfo *file_info, int *labels);
#endif //HANDLE_FILE_WORK_H_