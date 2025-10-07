#include "HandleFileWork.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
#include <errno.h>

#include "../Calculator/StructsEnums.h"
#include "FileOperations.h"
#include "../Calculator/Parse/ParseCommandLine.h"
#include "AssemblerEnums.h"

PossibleErrorsAsm HandleBufRead(Files in_out_files, FileInfo *file_info) {
    assert(file_info);

    file_info->filesize = SizeOfFile(in_out_files.in_file);
    file_info->buf_ptr = ReadToBuf(in_out_files.open_in_file, file_info->filesize);
    assert(file_info->buf_ptr != NULL);

    file_info->count_lines = CountLines(file_info->buf_ptr);
    file_info->text_ptr = (LineInfo *) calloc ((size_t)file_info->count_lines + 1, sizeof(LineInfo));
    assert(file_info->text_ptr != NULL);

    ParseBuf(file_info);

    return kNoErrorAsm;
}

int CommandToEnum(const char *command) {
    assert(command);

    if (strcmp(command, PUSH)  == 0) return kPush;
    if (strcmp(command, POP)   == 0) return kPop;
    if (strcmp(command, ADD)   == 0) return kAdd;
    if (strcmp(command, SUB)   == 0) return kSub;
    if (strcmp(command, MUL)   == 0) return kMul;
    if (strcmp(command, DIV)   == 0) return kDiv;
    if (strcmp(command, SQRT)  == 0) return kSqrt;
    if (strcmp(command, IN)    == 0) return kIn;
    if (strcmp(command, OUT)   == 0) return kOut;
    if (strcmp(command, PUSHR) == 0) return kPushR;
    if (strcmp(command, POPR)  == 0) return kPopR;
    if (strcmp(command, HLT)   == 0) return kHlt;

    return -1; 
}

void WriteCommand(FILE *output, int command, int has_arg, int arg) {
    assert(output);

    if (has_arg) {
        fprintf(output, "%d %d\n", command, arg);

    } else {
        fprintf(output, "%d\n", command);
    }
}

static const char *SkipWhitespace(const char *str) {
    assert(str);

    while (isspace((unsigned char) *str)) {
        str++;
    }
    return str;
}

static int StringToInt(char *arg_str) {
    assert(arg_str);

    return (int)(arg_str[1] - 'A');
}

static int ParseArgumentValue(char *arg_str, int *out_val) {
    assert(arg_str);
    assert(out_val);

    char *end_ptr = NULL;
    errno = 0;
    long val = strtol(arg_str, &end_ptr, 0);
    if (end_ptr != arg_str && *end_ptr == '\0' && errno == 0) {
        *out_val = (int)val;
        return kNoError;
    }

    *out_val = StringToInt(arg_str);
    return kNoError;
}

int HandleParse(const char *line, FileInfo *file_info, int **buf_out, int *num_args) {
    assert(line);
    assert(file_info);
    assert(buf_out);
    assert(num_args);

    const char *line_ptr = SkipWhitespace(line);
    if (*line_ptr == '\0') {
        return kWhitespace;
    }

    char *command = (char *) calloc (16, sizeof(char));
    if (command == NULL) {
        return kNoMemory;
    }
    char *arg_str = (char *) calloc (64, sizeof(char));
    if (arg_str == NULL) {
        free(command);
        return kNoMemory;
    }

    int args_count = sscanf(line_ptr, "%15s %63s", command, arg_str);

    printf("%s %s\n", command, arg_str);
    NumOfArgs has_arg = (args_count == 2) ? kOne : kZero;
    if (args_count != 1 && args_count != 2) {
        free(command);
        free(arg_str);
        return kErrorZeroArgs;
    }

    size_t pos = (size_t)file_info->instruction_counter;
    size_t new_count = pos + (size_t)(has_arg + 1);

    int *realloc_ptr = (int *) realloc (*buf_out, new_count * sizeof(int));
    if (realloc_ptr == NULL) {
        free(command);
        free(arg_str);
        return kNoMemory;
    }

    *buf_out = realloc_ptr;
    file_info->instruction_counter = (int)new_count;

    int arg_int = 0;
    if (has_arg == kOne) {
        ParseArgumentValue(arg_str, &arg_int);
    }

    int command_to_enum = CommandToEnum(command);
    if (command_to_enum == -1) {
        file_info->instruction_counter = (int)pos;
        return kNoAvailableCommand;
    }

    (*buf_out)[pos] = command_to_enum;
    if (has_arg == kOne) {
        (*buf_out)[pos + 1] = arg_int;
    }

    free(command);
    free(arg_str);
    *num_args = has_arg + 1;
    return kNoError;
}

void HandleWriteCommands(FILE *output, FileInfo *file_info, int *buf_out) {
    assert(output);
    assert(file_info);
    assert(buf_out);

    fprintf(output, "%d\n", file_info->instruction_counter);

    for (int i = 0; i < file_info->instruction_counter; i++) {
        fprintf(output, "%d ", buf_out[i]);
    }
}

int HandleAsm(FileInfo *file_info, Files in_out_files) {
    assert(file_info);

    PossibleErrorsAsm err = kNoErrorAsm;
    ERROR_CHECK_RETURN(HandleBufRead(in_out_files, file_info));

    int *buf_out = (int *) calloc ((size_t)file_info->instruction_counter, sizeof(int));
    if (buf_out == NULL) {
        return kNoMemory;
    }

    for (int i = 0; i < file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];
        
        int args_check = 0;
        int handle_error = HandleParse(line->start_ptr, file_info, &buf_out, &args_check);
        if (handle_error < 0) {
            free(buf_out);
            fprintf(stderr, "Parse error on line %d\n", i + 1);
            return handle_error;
        }

    }
    
    HandleWriteCommands(in_out_files.open_out_file, file_info, buf_out);
    free(buf_out);

    free(file_info->buf_ptr);
    free(file_info->text_ptr);

    return kNoError;
}