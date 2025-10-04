#ifndef HANDLE_FILE_WORK_H_
#define HANDLE_FILE_WORK_H_

#include <stdio.h>

#include "StructsEnums.h"

PossibleErrors HandleBufRead(const char *filename, FileInfo *file_info);
void WriteCommand(FILE *output, int command, int has_arg, int arg);
void WriteCommandBack(FILE *output, const char *command, int has_arg, int arg);
int HandleParseAndWriteInstruction(FILE *output, const char *line);
int HandleAsm(char **argv, const char *input_filename, const char *output_filename, FileInfo *file_info);

#endif //HANDLE_FILE_WORK_H_