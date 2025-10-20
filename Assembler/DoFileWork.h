#ifndef HANDLE_FILE_WORK_H_
#define HANDLE_FILE_WORK_H_

#include <stdio.h>

#include "AssemblerEnums.h"
#include "StructsEnums.h"
#include "StackFunctions.h"
#include "AssemblerStructs.h"
#include "FileStructs.h"

// #define CHECK_ERROR_RETURN(call, err) \
//     if (err != kNoError) { \
//         free(command); \
//         free(arg_str); \
//         return err; \
//     }

void AllBufRead(const Files *in_out_files, FileInfo *file_info);
// void WriteCommand(FILE *output, int command, int has_arg, int arg);
AsmError DoParse(const char *line, FileInfo *file_info, Stack_Info *buf_out, Labels *labels, ssize_t *code_size);
AsmError DoAsm(FileInfo *file_info, const Files *in_out_files, AssemblerInfo *Assembler);
// void WriteCommandsOut(FILE *output, FileInfo *file_info, const Stack_Info *buf_out);

AsmError PrepareToAssemble(const Files *files, FileInfo *file_info, Labels *labels);

AsmError PreAssemble(FileInfo *file_info, int *labels);

void AddLabel(Labels *labels, const char *name, int address);
int FindLabel(const Labels *labels, const char *name);

void PrintAssemblerListing(FILE *listing_file, const AssemblerInfo *Assembler);
void PrintReadableCode(const Stack_Info *code);
#endif //HANDLE_FILE_WORK_H_