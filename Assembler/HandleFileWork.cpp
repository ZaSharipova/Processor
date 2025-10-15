#include "HandleFileWork.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
#include <errno.h>

#include "StructsEnums.h"
#include "FileOperations.h"
#include "ParseCommandLine.h"
#include "AssemblerEnums.h"
#include "SubsidiaryFunctionsAssembler.h"
#include "StackFunctions.h"
#include "HandleLogFile.h"

static AsmError GetLabels(FileInfo *file_info, int *labels);

static void AllBufRead(const Files *in_out_files, FileInfo *file_info) { //
    assert(in_out_files);
    assert(file_info);

    file_info->filesize = SizeOfFile(in_out_files->in_file);
    assert(file_info->filesize > 0);

    file_info->buf_ptr = ReadToBuf(in_out_files->open_in_file, file_info->filesize);
    assert(file_info->buf_ptr != NULL);

    file_info->count_lines = CountLines(file_info->buf_ptr);
    file_info->text_ptr = (LineInfo *) calloc ((size_t)file_info->count_lines + 1, sizeof(LineInfo));
    assert(file_info->text_ptr != NULL);

    ParseBuf(file_info);
}

static AsmError ParseArgumentValue(char *arg_str, int *out_val, int *labels) {
    assert(arg_str);
    assert(out_val);
    assert(labels);

    if (*arg_str == ':') {
        arg_str++;

        int label_num = 0;
        if (sscanf(arg_str, "%d", &label_num) != 1) {
            return kErrorParsingAsm;
        }

        *out_val = labels[label_num];
        return kNoAsmError;
    }

    char *end_ptr = NULL;
    errno = 0;
    long val = strtol(arg_str, &end_ptr, 0);
    if (end_ptr != arg_str && *end_ptr == '\0' && errno == 0) {
        *out_val = (int)val;
        return kNoAsmError;
    }

    *out_val = StringToInt(arg_str);
    return kNoAsmError;
}

AsmError HandleParse(const char *line, FileInfo *file_info, Stack_Info *buf_out, int *labels) {
    assert(line);
    assert(file_info);
    assert(buf_out);
    assert(labels);

    const char *line_ptr = SkipWhitespace(line);
    if (*line_ptr == '\0') {
        return kWhitespace;
    }

    char command[16] = {};
    char arg_str[64] = {};

    int args_count = sscanf(line_ptr, "%15s %63s", command, arg_str);

    // fprintf(GetLogFile(), "DEBUG: line='%s' args_count=%d command='%s' arg='%s'\n",
    //         line_ptr, args_count, command, arg_str);


    NumOfArgs has_arg = (args_count == 2) ? kOne : kZero;
    if (args_count != 1 && args_count != 2) {
        return kErrorZeroArgs;
    }

    int command_to_enum = CommandToEnum(command);
    if (command_to_enum == kCommandNotFound) {
        return kNoAvailableCommand;
    }

    Stack_t arg_int = 0;
    if (has_arg == kOne) {
        ParseArgumentValue(arg_str, &arg_int, labels);
    }

    AsmError err = kNoAsmError;
    CHECK_ERROR_RETURN((AsmError)StackPush(buf_out, (Stack_t)command_to_enum)); //
    if (has_arg == kOne) {
        CHECK_ERROR_RETURN((AsmError)StackPush(buf_out, arg_int));
    }

    return kNoAsmError;
}

void HandleWriteCommands(FILE *output, FileInfo *file_info, const Stack_Info *buf_out) {
    assert(output);
    assert(file_info);
    assert(buf_out);

    fprintf(output, "%zd ", buf_out->size);

    for (int i = 0; i < buf_out->size; i++) {
        fprintf(output, "%d ", buf_out->data[i]);
    }

}

AsmError HandleAsm(FileInfo *file_info, const Files *in_out_files, int *labels) {
    assert(file_info);
    assert(in_out_files);
    assert(labels);

    // AsmError err = 0;
    INIT_STACK(buf_out);
    StackCtor(&buf_out, 1);

    for (size_t i = 0; i < (size_t)file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];

        AsmError handle_error = HandleParse(line->start_ptr, file_info, &buf_out, labels);
        if (handle_error < 0 && handle_error != kWhitespace) {
            StackDtor(&buf_out);
            fprintf(GetLogFile(), "Parse error on line %lu\n", i + 1);
            return handle_error;
        }
    }

    HandleWriteCommands(in_out_files->open_out_file, file_info, &buf_out);
    StackDtor(&buf_out);

    free(file_info->buf_ptr);
    free(file_info->text_ptr);

    return kNoAsmError;
}

AsmError PrepareToAssemble(const Files *files, FileInfo *file_info, int *labels) {
    assert(file_info);
    assert(labels);

    AllBufRead(files, file_info);

    AsmError err = GetLabels(file_info, labels);
    if (err != kNoAsmError) {
        return err;
    }
    // for (int i = 0; i < 10; i++) {
    //     printf("%d ", labels[i]);
    // }
    // printf("\n\n");
    return kNoAsmError;
}

static AsmError GetLabels(FileInfo *file_info, int *labels) {
    assert(file_info);
    assert(labels);

    int current_address = 0;
    for (int i = 0; i < file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];

        //int args_check = 0;

        int label_num = 0;
        //printf("LINE_START %s \n\n", line->start_ptr);
        if ((line->start_ptr)[0] == ':') {
            int num_counter = sscanf(line->start_ptr + 1, "%d", &label_num);
            //printf("\n %d LABEL_NUM \n", label_num);
            if (num_counter == 1) {
                //printf("%d LABEL_NUM", label_num);
                labels[label_num] = current_address;
                *line->start_ptr = '\0';

            } else {
                printf("error parsing METKI in command %d", current_address);
                return kNoAvailableValue;
            }

        } else {
            const char *line_ptr = line->start_ptr;

            char command[16] = {};
            char arg_str[64] = {};

            int args_count = sscanf(line_ptr, "%15s %63s", command, arg_str);
            if (args_count == 0) {
                printf("Error parse %s command address: %d\n", command, current_address);
                return kErrorZeroArgs;

            } else {
                current_address += args_count;
            }
        }
    }
    
    return kNoAsmError;
}