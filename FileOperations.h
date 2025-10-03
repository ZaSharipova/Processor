#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include "StructsEnums.h"

FILE *OpenFile(const char *filename, const char *mode);
PossibleErrors CloseFile(FILE *file);
size_t SizeOfFile(const char *filename);
char *ReadToBuf(const char *filename, FILE *file, size_t filesize);
int IsBlankLine(const char *start, size_t len);
int CountLines(const char *buf);
void ParseBuf(FileInfo *file_info);

void PrintError(PossibleErrors err_type);

#endif //FILE_OPERATIONS_H_