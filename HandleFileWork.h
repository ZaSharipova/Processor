#ifndef HANDLE_FILE_WORK_H_
#define HANDLE_FILE_WORK_H_

#include <stdio.h>

#include "StructsEnums.h"

FILE *open_file(const char *filename, const char *mode);
StackErr_t close_file(FILE *file);
size_t size_of_file(const char *filename);
char *read_to_buf(const char *filename, FILE *file, size_t filesize);
int is_blank_line(const char *start, size_t len);
int count_lines(const char *buf);
void parse_buf(FileInfo *file_info);
StackErr_t handle_buf_read(const char *filename, FileInfo *file_info);
void write_command(FILE *output, int command, int has_arg, int arg);
int parse_and_write_instruction(FILE *output, const char *line);
int handle_asm(char **argv, const char *input_filename, const char *output_filename, FileInfo *file_info);

#endif //HANDLE_FILE_WORK_H_