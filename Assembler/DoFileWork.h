#ifndef HANDLE_FILE_WORK_H_
#define HANDLE_FILE_WORK_H_

#include <stdio.h>

#include "AssemblerEnums.h"
#include "StructsEnums.h"
#include "StackFunctions.h"
#include "AssemblerStructs.h"
#include "FileStructs.h"

AsmError AssemblerCtor(AssemblerInfo *Assembler);

void AllBufRead(const Files *in_out_files, FileInfo *file_info);
AsmError DoParse(const char *line, FileInfo *file_info, AssemblerInfo *assembler);
AsmError DoAsm(FileInfo *file_info, const Files *in_out_files, AssemblerInfo *assembler);

AsmError PrepareToAssemble(const Files *files, FileInfo *file_info, AssemblerInfo *assembler);

AsmError PreAssemble(FileInfo *file_info, int *labels);

void AddLabel(Labels *labels, const char *name, int address);
int FindLabel(const Labels *labels, const char *name);

void PrintAssemblerListing(FILE *listing_file, const AssemblerInfo *assembler);
void PrintReadableCode(const Stack_Info *code);

Stack_t GetStackElem(const Stack_Info *buf_out, size_t pos);

#endif //HANDLE_FILE_WORK_H_