#include "HandleFileWork.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>

#include "StructsEnums.h"
#include "FileOperations.h"

StackErr_t handle_buf_read(const char *filename, FileInfo *file_info) {
    assert(filename);
    assert(file_info);

    FILE *file = open_file(filename, "r");
    if (file == NULL) {
        perror("fopen() failed");
        return kErrorOpening;
    }

    file_info->filesize = size_of_file(filename);
    file_info->buf_ptr = read_to_buf(filename, file, file_info->filesize);
    assert(file_info->buf_ptr != NULL);

    StackErr_t err = close_file(file);
    if (err != kSuccess) return err;

    file_info->count_lines = count_lines(file_info->buf_ptr);
    file_info->text_ptr = (LineInfo *) calloc ((size_t)file_info->count_lines + 1, sizeof(LineInfo));
    assert(file_info->text_ptr != NULL);

    parse_buf(file_info);

    return kSuccess;
}

int command_to_enum(const char *cmd) {
    assert(cmd);

    if (strcmp(cmd, PUSH) == 0) return Push;
    if (strcmp(cmd, POP)  == 0) return Pop;
    if (strcmp(cmd, ADD)  == 0) return Add;
    if (strcmp(cmd, SUB)  == 0) return Sub;
    if (strcmp(cmd, MUL)  == 0) return Mul;
    if (strcmp(cmd, DIV)  == 0) return Div;
    if (strcmp(cmd, SQRT) == 0) return Sqrt;
    if (strcmp(cmd, OUT)  == 0) return Out;
    if (strcmp(cmd, HLT)  == 0) return Hlt;

    return -1; 
}

void write_command(FILE *output, int command, int has_arg, int arg) {
    assert(output);

    if (has_arg) {
        fprintf(output, "%d %d\n", command, arg);
    } else {
        fprintf(output, "%d\n", command);
    }
}

int parse_and_write_instruction(FILE *output, const char *line) {
    assert(output);
    assert(line);

    char *cmd = (char *) calloc (16, sizeof(char));
    int arg = 0, has_arg = 0;

    int num_parsed = sscanf(line, "%15s %d", cmd, &arg);
    if (num_parsed == 1) {
        has_arg = 0;
    }

    else if (num_parsed == 2) {
        has_arg = 1;

    } else {
        return -1;
    }
    int command_enum = command_to_enum(cmd);
    if (command_enum == -1) {
        return -1;
    }

    write_command(output, command_enum, has_arg, arg);
    free(cmd);

    return 0;
}

int handle_asm(char **argv, const char *input_filename, const char *output_filename, FileInfo *file_info) {
    assert(argv);
    assert(input_filename);
    assert(output_filename);

    if (handle_buf_read(input_filename, file_info) != kSuccess) {
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }

    FILE *out_file = open_file(output_filename, "w");
    if (out_file == NULL) {
        free(file_info->buf_ptr);
        free(file_info->text_ptr);
        return 1;
    }

    for (int i = 0; i < file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];
        size_t len = line->size + 1;

        char *line_buf = (char *) calloc(len + 1, sizeof(char));
        memcpy(line_buf, line->start_ptr, len);
        line_buf[len] = '\0';

        if (parse_and_write_instruction(out_file, line_buf) != 0) {
            fprintf(stderr, "Parse error on line %d: %s\n", i + 1, line_buf);
        }
        free(line_buf);
    }

    fclose(out_file);
    free(file_info->buf_ptr);
    free(file_info->text_ptr);

    return 0;
}