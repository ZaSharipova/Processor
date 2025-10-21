#include "FileOperations.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>

#include "AssemblerEnums.h"
#include "StructsEnums.h"
#include "FileOperations.h"
#include "DoLogFile.h"
#include "ParseCommandLine.h"
#include "FileStructs.h"

#define DEFAULT_COMMAND -1

static ParseErr_t OpenFileOrDefault(const char *filename, const char *mode, FILE **file_ptr, FILE *default_file);

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

                char *semicolon_is = strchr(ptr, ';');
                if (semicolon_is) {
                    *semicolon_is = '\0';
                }

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

ParseErr_t DoOpenFile(Files *in_out_files) {
    assert(in_out_files);

    ParseErr_t read_write_error = kNoError; 
    // read_write_error = (OpenFileOrDefault(in_out_files->log_file, WRITE_MODE, stdout));
    // if (read_write_error != kNoError) {
    //     return read_write_error;
    // }

    read_write_error = (OpenFileOrDefault(in_out_files->in_file, READ_MODE, &in_out_files->open_in_file, stdin));
    if (read_write_error != kNoError) {
        //read_write_error = CloseFile(in_out_files->open_log_file);
        return read_write_error;
    }

    read_write_error = OpenFileOrDefault(in_out_files->out_file, WRITE_MODE, &in_out_files->open_out_file, stdout);
    if (read_write_error != kNoError) {
        CALL_CHECK_IN_OUT_RETURN(CloseFile(in_out_files->open_in_file));
        CALL_CHECK_IN_OUT_RETURN(CloseFile(in_out_files->open_out_file));
        return read_write_error;
    }

    return kNoError;
}

ParseErr_t DoCloseFile(const Files *in_out_files) {
    assert(in_out_files);

    ParseErr_t read_write_error = kNoError;

    // if (in_out_files.log_file != NULL) {
    //     CALL_CHECK_IN_OUT_RETURN(CloseFile(GetLogFile()));
    // }

    if (in_out_files->open_in_file != stdin) {
        CALL_CHECK_IN_OUT_RETURN(CloseFile(in_out_files->open_in_file));
    }

    if (in_out_files->open_out_file != stdout) {
        CALL_CHECK_IN_OUT_RETURN(CloseFile(in_out_files->open_out_file));
    }

   return kNoError;
}

static ParseErr_t OpenFileOrDefault(const char *filename, const char *mode, FILE **file_ptr, FILE *default_file) {
    assert(mode);
    assert(file_ptr);
    assert(default_file);

    if (filename != NULL) {
        *file_ptr = OpenFile(filename, mode);
        if (*file_ptr == NULL) {
            return kErrorOpening;
        }
    } else {
        *file_ptr = default_file;
    }
    return kNoError;
}

FILE *OpenFile(const char *filename, const char *mode) {
    assert(filename);
    assert(mode);

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("fopen() failed");
        return NULL;
    }

    return file;
}

ParseErr_t CloseFile(FILE *file) {
    assert(file);

    int status = fclose(file);
    if (status != 0) {
        perror("fclose() failed");
        return kErrorClosing;
    }

    return kNoError;
}

ProcessorErr_t Read(FILE *fin, Stack_t *code[], size_t *code_size) {
    assert(fin);
    assert(code);
    assert(code_size);

    Stack_t cmd = DEFAULT_COMMAND;
    size_t pointer = 0;

    fscanf(fin, "%zd", code_size);

    *code = (Stack_t *) calloc (*code_size, sizeof(Stack_t));
    if (*code == NULL) {
        return kNoMemory;
    }

    while (pointer < *code_size && fscanf(fin, "%d", &cmd) == 1) {
        (*code)[pointer] = cmd;

        pointer++;
    }
    // for (size_t i = 0; i < *code_size; ++i) {
    //     fprintf(stderr, "DEBUG code[%zu] = %d\n", i, (int)(*code)[i]);
    // }


    return kProcessorSuccess;
}