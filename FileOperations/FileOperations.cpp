#include "FileOperations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>

#include "AssemblerEnums.h"

size_t SizeOfFile(const char *filename) {
    assert(filename);

    struct stat stbuf = {};
    if (stat(filename, &stbuf) != 0) {
        perror("stat() failed");
        return 0;
    }

    return (size_t)stbuf.st_size;
}

char *ReadToBuf(FILE *file, size_t filesize) {
    assert(file);

    char *buf_in = (char *) calloc (filesize + 2, sizeof(char));
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

int IsBlankLine(const char *start, size_t len) {
    assert(start);

    for (size_t i = 0; i < len; i++) {
        if (!isspace((unsigned char)start[i]))
            return 0;
    }

    return 1;
}

int CountLines(const char *buf) {
    int count = 0;
    for (size_t i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == '\n')
            count++;
    }

    return count;
}

void ParseBuf(FileInfo *file_info) {
    assert(file_info);

    size_t line_idx = 0;
    char *line_start = file_info->buf_ptr;
    size_t bufsize = file_info->filesize + 1;

    for (size_t i = 0; i <= bufsize; i++) {
        char c = file_info->buf_ptr[i];
        int end_of_buffer = (i == bufsize);
        int end_of_line = (c == '\n' || end_of_buffer);

        if (end_of_line) {
            long line_len = &(file_info->buf_ptr[i]) - line_start;

            if (line_len > 0) {
                char *ptr = line_start;
                while (isspace((unsigned char)*ptr)) ptr++;

                if (*ptr != '\0') {
                    file_info->text_ptr[line_idx].start_ptr = line_start;
                    file_info->text_ptr[line_idx].end_ptr = &(file_info->buf_ptr[i - 1]);
                    file_info->text_ptr[line_idx].size = (size_t)(&(file_info->buf_ptr[i - 1]) - line_start) + 1;
                    line_idx++;
                }
            }

            if (!end_of_buffer) {
                file_info->buf_ptr[i] = '\0';
            }

            line_start = &(file_info->buf_ptr[i + 1]);
        }
    }

    file_info->count_lines = (int)line_idx;
}


void PrintError(AsmError err_type) {
    switch (err_type) {
    case (kNoAsmError):
        break;
    
    case (kErrorOpeningFile):
        fprintf(stderr, "Error while opening file.");
        break;

    case (kErrorClosingFile):
        fprintf(stderr, "Error while closing file.");
        break;

    case (kErrorParsingAsm):
        fprintf(stderr, "Error while parsing file.");
        break;

    case (kNoMemoryCalloc):
        fprintf(stderr, "Error memory error in Calloc.");
        break;

    case (kNoAvailableValue):
        fprintf(stderr, "No available value.");
        break;

    case (kNoAvailableCommand):
        fprintf(stderr, "Not available command entered.");

    case (kErrorZeroArgs):
        fprintf(stderr, "No args beeing entered.");
        break;

    case (kWhitespace):
        fprintf(stderr, "Empty line entered.");
        break;

    default:
        fprintf(stderr, "Unlnown command.");
    }
}