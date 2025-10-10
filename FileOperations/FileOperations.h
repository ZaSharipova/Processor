#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include "AssemblerEnums.h"
#include "../Calculator/StructsEnums.h"

size_t SizeOfFile(const char *filename);
char *ReadToBuf(FILE *file, size_t filesize);
int IsBlankLine(const char *start, size_t len);
int CountLines(const char *buf);
void ParseBuf(FileInfo *file_info);

void PrintError(AsmError err_type);

#endif //FILE_OPERATIONS_H_