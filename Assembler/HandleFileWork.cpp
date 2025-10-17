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

#define MAX_ARG_LEN 16

static AsmError GetLabels(FileInfo *file_info, Labels *labels);
static void AddLabel(Labels *labels, const char *name, int address);
static int FindLabel(const Labels *labels, const char *name);
static void WriteCommandsOut(FILE *output, FileInfo *file_info, const Stack_Info *buf_out);

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

static AsmError ParseArgumentValue(char *arg_str, int *out_val, Labels *labels) {
    assert(arg_str);
    assert(out_val);
    assert(labels);

    if (*arg_str == ':') {
        arg_str++;

        int address = FindLabel(labels, arg_str);
        if (address == -1) {
            fprintf(stderr, "Error: label '%s' not found!\n", arg_str);
            return kErrorParsingAsm;
        }

        *out_val = address;
        return kNoAsmError;
    }

    char *end_ptr = NULL;
    errno = 0;
    long val = strtol(arg_str, &end_ptr, 0);
    if (end_ptr != arg_str && *end_ptr == '\0' && errno == 0) {
        *out_val = (int)val;
        return kNoAsmError;
    }

    if (*arg_str == '[') {
        arg_str++;
    }
    *out_val = StringToInt(arg_str);
    return kNoAsmError;
}

// AsmError DoParse(const char *line, FileInfo *file_info, Stack_Info *buf_out, Labels *labels) {
//     assert(line);
//     assert(file_info);
//     assert(buf_out);
//     assert(labels);

//     const char *line_ptr = SkipWhitespace(line);
//     if (*line_ptr == '\0') {
//         return kWhitespace;
//     }

//     char command[16] = {};
//     char arg_str[64] = {};

//     int args_count = sscanf(line_ptr, "%15s %63s", command, arg_str);

//     // fprintf(GetLogFile(), "DEBUG: line='%s' args_count=%d command='%s' arg='%s'\n",
//     //         line_ptr, args_count, command, arg_str);


//     NumOfArgs has_arg = (args_count == 2) ? kOne : kZero;
//     if (args_count != 1 && args_count != 2) {
//         return kErrorZeroArgs;
//     }

//     int command_to_enum = CommandToEnum(command);
//     if (command_to_enum == kCommandNotFound) {
//         return kNoAvailableCommand;
//     }

//     Stack_t arg_int = 0;
//     if (has_arg == kOne) {
//         ParseArgumentValue(arg_str, &arg_int, labels);
//     }

//     AsmError err = kNoAsmError;
//     CHECK_ERROR_RETURN((AsmError)StackPush(buf_out, (Stack_t)command_to_enum)); //
//     if (has_arg == kOne) {
//         CHECK_ERROR_RETURN((AsmError)StackPush(buf_out, arg_int));
//     }

//     return kNoAsmError;
// }

AsmError DoParse(const char *line, FileInfo *file_info, Stack_Info *buf_out, Labels *labels) {
    assert(line);
    assert(file_info);
    assert(buf_out);
    assert(labels);

    char cmd_name[MAX_ARG_LEN] = {}, 
    arg_str[MAX_ARG_LEN] = {};

    const char *line_ptr = SkipWhitespace(line);
    if (*line_ptr == '\0') {
        return kWhitespace;
    }

    AsmError err = kNoAsmError;
    sscanf(line_ptr, "%s", cmd_name);
    int command_to_enum = CommandToEnum(cmd_name);
    if (command_to_enum == kCommandNotFound) {
        return kNoAvailableCommand;
    }

    CHECK_ERROR_RETURN((AsmError)StackPush(buf_out, (Stack_t)command_to_enum));
    const CommandsInfo *commands_info = &commands[command_to_enum];
    int arg_val = 0;

    if (commands_info->num_args == 1) {
        line_ptr += strlen(cmd_name);
        sscanf(SkipWhitespace(line_ptr), "%s", arg_str);

        ParseArgumentValue(arg_str, &arg_val, labels);
        CHECK_ERROR_RETURN((AsmError)StackPush(buf_out, arg_val));
    }

    return kNoAsmError;
}

static void WriteCommandsOut(FILE *output, FileInfo *file_info, const Stack_Info *buf_out) {
    assert(output);
    assert(file_info);
    assert(buf_out);

    fprintf(output, "%zd ", buf_out->size);

    for (int i = 0; i < buf_out->size; i++) {
        fprintf(output, "%d ", buf_out->data[i]);
    }
}

AsmError DoAsm(FileInfo *file_info, const Files *in_out_files, Labels *labels) {
    assert(file_info);
    assert(in_out_files);
    assert(labels);

    // AsmError err = 0;
    INIT_STACK(buf_out);
    StackCtor(&buf_out, 1);

    for (size_t i = 0; i < (size_t)file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];

        AsmError handle_error = DoParse(line->start_ptr, file_info, &buf_out, labels);
        if (handle_error < 0 && handle_error != kWhitespace) {
            StackDtor(&buf_out);
            fprintf(GetLogFile(), "Parse error on line %lu\n", i + 1);
            return handle_error;
        }
    }

    WriteCommandsOut(in_out_files->open_out_file, file_info, &buf_out);

    StackDtor(&buf_out);

    free(file_info->buf_ptr);
    free(file_info->text_ptr);

    return kNoAsmError;
}

AsmError PrepareToAssemble(const Files *files, FileInfo *file_info, Labels *labels) {
    assert(file_info);
    assert(labels);

    AllBufRead(files, file_info);

    return GetLabels(file_info, labels);
}

static AsmError GetLabels(FileInfo *file_info, Labels *labels) {
    assert(file_info);
    assert(labels);

    int current_address = 0;
    for (int i = 0; i < file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];

        if ((line->start_ptr)[0] == ':') {

            char *label_name = line->start_ptr + 1;
            sscanf(line->start_ptr + 1, "%s", label_name);
            AddLabel(labels, label_name, current_address);

            *line->start_ptr = '\0';

        } else {
            const char *line_ptr = line->start_ptr;

            char command[MAX_ARG_LEN] = {};
            char arg_str[MAX_ARG_LEN] = {};

            int args_count = sscanf(line_ptr, "%s %s", command, arg_str);
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

static void AddLabel(Labels *labels, const char *name, int address) {
    assert(labels);
    assert(name);

    for (size_t i = 0; i < labels->count; i++) {
        if (strcmp(labels->data[i].name, name) == 0) {
            labels->data[i].address = address;
        }
    }

    strcpy(labels->data[labels->count].name, name); //
    labels->data[labels->count].address = address;
    labels->count++;
}

static int FindLabel(const Labels *labels, const char *name) {
    assert(labels);
    assert(name);

    for (size_t i = 0; i < labels->count; i++) {
        if (strcmp(labels->data[i].name, name) == 0) {
            return labels->data[i].address;
        }
    }

    return -1;
}
