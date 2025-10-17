#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include "AssemblerEnums.h"
#include "StructsEnums.h"

size_t SizeOfFile(const char *filename);
char *ReadToBuf(FILE *file, size_t filesize);
int IsBlankLine(const char *start, size_t len);
int CountLines(const char *buf);
void ParseBuf(FileInfo *file_info);

void PrintError(AsmError err_type);

ParseErr_t DoOpenFile(Files *in_out_files);
ParseErr_t DoCloseFile(const Files *in_out_files); // handle

FILE *OpenFile(const char *filename, const char *mode);
ParseErr_t CloseFile(FILE *file);

#endif //FILE_OPERATIONS_H_