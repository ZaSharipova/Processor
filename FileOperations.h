#ifndef FILE_OPERATIONS_H_
#define FILE_OPERATIONS_H_

#include "StructsEnums.h"

FILE *open_file(const char *filename, const char *mode);
StackErr_t close_file(FILE *file);
size_t size_of_file(const char *filename);
char *read_to_buf(const char *filename, FILE *file, size_t filesize);
int is_blank_line(const char *start, size_t len);
int count_lines(const char *buf);
void parse_buf(FileInfo *file_info);

#endif //FILE_OPERATIONS_H_