#ifndef HANDLE_FILE_WORK_H_
#define HANDLE_FILE_WORK_H_

#include <stdio.h>

#include "AssemblerEnums.h"
#include "StructsEnums.h"
#include "StackFunctions.h"

// #define CHECK_ERROR_RETURN(call, err) \
//     if (err != kNoError) { \
//         free(command); \
//         free(arg_str); \
//         return err; \
//     }

// void AllBufRead(const Files *in_out_files, FileInfo *file_info);
// void WriteCommand(FILE *output, int command, int has_arg, int arg);
AsmError HandleParse(const char *line, FileInfo *file_info, Stack_Info *buf_out, int *labels);
AsmError HandleAsm(FileInfo *file_info, const Files *in_out_files, int *labels);
void HandleWriteCommands(FILE *output, FileInfo *file_info, const Stack_Info *buf_out);

AsmError PrepareToAssemble(const Files *files, FileInfo *file_info, int *labels);

AsmError PreAssemble(FileInfo *file_info, int *labels);

#endif //HANDLE_FILE_WORK_H_