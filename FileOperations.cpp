#include "FileOperations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>

#include "StructsEnums.h"

FILE *open_file(const char *filename, const char *mode) {
    assert(filename);
    assert(mode);

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("fopen() failed");
        return NULL;
    }

    return file;
}

StackErr_t close_file(FILE *file) {
    assert(file);

    int status = fclose(file);
    if (status != 0) {
        perror("fclose() failed");
        return kErrorClosing;
    }

    return kSuccess;
}

size_t size_of_file(const char *filename) {
    assert(filename);

    struct stat stbuf;
    if (stat(filename, &stbuf) != 0) {
        perror("stat() failed");
        return 0;
    }
    return (size_t)stbuf.st_size;
}

char *read_to_buf(const char *filename, FILE *file, size_t filesize) {
    assert(filename);
    assert(file);

    char *buf_in = (char *) calloc(filesize + 2, sizeof(char));
    assert(buf_in != NULL);

    size_t bytes_read = fread(buf_in, sizeof(char), filesize, file);
    if (bytes_read == 0) {
        buf_in[0] = '\n';
        buf_in[1] = '\0';
    } else {
        if (buf_in[bytes_read - 1] != '\n') {
            buf_in[bytes_read] = '\n';
            bytes_read++;
        }
        buf_in[bytes_read] = '\0';
    }
    return buf_in;
}

int is_blank_line(const char *start, size_t len) {
    assert(start);

    for (size_t i = 0; i < len; i++) {
        if (!isspace((unsigned char)start[i]))
            return 0;
    }
    return 1;
}

int count_lines(const char *buf) {
    int count = 0;
    for (size_t i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == '\n')
            count++;
    }
    return count;
}

void parse_buf(FileInfo *file_info) {
    assert(file_info);

    size_t line_idx = 0;
    char *line_start = file_info->buf_ptr;
    char *alpha_start = NULL;
    char *alpha_end = NULL;
    size_t bufsize = file_info->filesize + 1;

    for (size_t i = 0; i <= bufsize; i++) {
        char c = file_info->buf_ptr[i];
        int end_of_buffer = (i == bufsize);
        int end_of_line = (c == '\n' || end_of_buffer);

        if (!end_of_line) {
            if (isalpha((unsigned char)c)) {
                if (alpha_start == NULL) {
                    alpha_start = &(file_info->buf_ptr[i]);
                }
                alpha_end = &(file_info->buf_ptr[i]);
            }
        }

        if (end_of_line) {
            long line_len = &(file_info->buf_ptr[i]) - line_start;
            if (!is_blank_line(line_start, (size_t)line_len) && alpha_start != NULL) {
                file_info->text_ptr[line_idx].start_ptr = line_start;
                file_info->text_ptr[line_idx].end_ptr = &(file_info->buf_ptr[i - 1]);
                file_info->text_ptr[line_idx].start_ptr_alpha = alpha_start;
                file_info->text_ptr[line_idx].end_ptr_alpha = alpha_end;
                file_info->text_ptr[line_idx].size = (size_t)(&(file_info->buf_ptr[i - 1]) - line_start) + 1;
                line_idx++;
            }
            line_start = &(file_info->buf_ptr[i + 1]);
            alpha_start = NULL;
            alpha_end = NULL;
        }
    }
}