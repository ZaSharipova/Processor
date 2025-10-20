#include "DoFileWork.h"

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
#include "FileStructs.h"
#include "AssemblerStructs.h"
#include "DoLogFile.h"

#define MAX_ARG_LEN 16
static AsmError GetLabels(FileInfo *file_info, Labels *labels, ssize_t *size);
void AddLabel(Labels *labels, const char *name, int address);
int FindLabel(const Labels *labels, const char *name);
static void WriteCommandsOut(FILE *output, FileInfo *file_info, const Stack_Info *buf_out);
static AsmError DoScanfAndConvert(const char *line_ptr, FileInfo *file_info, Stack_Info *buf_out, Labels *labels, bool flag_push, int *num_instructions);

void AllBufRead(const Files *in_out_files, FileInfo *file_info) {
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

AsmError PrepareToAssemble(const Files *files, FileInfo *file_info, Labels *labels, ssize_t *size) {
    assert(file_info);
    assert(labels);
    assert(size);

    AllBufRead(files, file_info);

    // GetLabels(file_info, labels);
    // for (int i = 0; i < 10; i++) {
    //     printf("%s ", labels->data[i].name);
    // }
    // return kNoAsmError;
    return GetLabels(file_info, labels, size);
}

AsmError DoAsm(FileInfo *file_info, const Files *in_out_files, AssemblerInfo *Assembler) {
    assert(file_info);
    assert(in_out_files);
    assert(Assembler);

    StackCtor(&Assembler->data, 1);

    // Assembler->data.size = data_size;
    for (size_t i = 0; i < (size_t)file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];

        AsmError handle_error = DoParse(line->start_ptr, file_info, &Assembler->data, &Assembler->labels);
        if (handle_error < 0 && handle_error != kWhitespace) {
            StackDtor(&(Assembler->data));
            fprintf(GetLogFile(), "Parse error on line %lu\n", i + 1);
            return handle_error;
        }
    }

    WriteCommandsOut(in_out_files->open_out_file, file_info, &(Assembler->data));

    free(file_info->buf_ptr);
    free(file_info->text_ptr);

    return kNoAsmError;
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

static AsmError DoScanfAndConvert(const char *line_ptr, FileInfo *file_info, Stack_Info *buf_out, Labels *labels, bool flag_push, int *num_instructions) {
    assert(line_ptr);
    assert(file_info);
    assert(buf_out);
    assert(labels);
    assert(num_instructions);

    char cmd_name[MAX_ARG_LEN] = {}, 
    arg_str[MAX_ARG_LEN] = {};

    int scanf_num = 0;
    AsmError err = kNoAsmError;
    scanf_num = sscanf(SkipWhitespace(line_ptr), "%s", cmd_name);
    if (scanf_num <= 0) {
        return kErrorZeroArgs;
    }

    int command_to_enum = CommandToEnum(cmd_name);
    if (command_to_enum == kCommandNotFound) {
        printf("Command not found.\n");
        return kErrorZeroArgs;
    }

    if (flag_push) {
        CHECK_ERROR_RETURN((AsmError)StackPush(buf_out, (Stack_t)command_to_enum));
    }

    const CommandsInfo *commands_info = &commands[command_to_enum];
    int arg_val = 0;

    if (commands_info->num_args == 1) {
        line_ptr += strlen(cmd_name);
        scanf_num = sscanf(SkipWhitespace(line_ptr), "%s", arg_str);
        if (scanf_num <= 0) {
            return kErrorZeroArgs;
        }

        *num_instructions = 2;

        if (!flag_push) {
            return kNoAsmError;
        }
        ParseArgumentValue(arg_str, &arg_val, labels);

        if (flag_push) {
            CHECK_ERROR_RETURN((AsmError)StackPush(buf_out, arg_val));
        }
    }

    *num_instructions = 1;

    return kNoAsmError;
}

AsmError DoParse(const char *line, FileInfo *file_info, Stack_Info *buf_out, Labels *labels) {
    assert(line);
    assert(file_info);
    assert(buf_out);
    assert(labels);

    const char *line_ptr = SkipWhitespace(line);
    if (*line_ptr == '\0') {
        return kWhitespace;
    }

    bool flag_push_stack = true;
    int num_instructions = 0;
    AsmError err = DoScanfAndConvert(line_ptr, file_info, buf_out, labels, flag_push_stack, &num_instructions);
    if (err != kNoAsmError) {
        return err;
    }
    // *code_size = num_instructions;
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

static AsmError GetLabels(FileInfo *file_info, Labels *labels, ssize_t *size) {
    assert(file_info);
    assert(labels);
    assert(size);

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
            // printf("%s %d\n", line_ptr, i);

            bool flag_push_stack = false;
            int args_count = 0;
            AsmError scanf_err = kNoAsmError;
            Stack_Info buf_out = {};
            scanf_err = DoScanfAndConvert(line_ptr, file_info, &buf_out, labels, flag_push_stack, &args_count);

            if (scanf_err != kNoAsmError) {
                printf("Scanf error in %s.", line_ptr);
                return scanf_err;
            }


            // int args_count = sscanf(line_ptr, "%s %s", command, arg_str);
            if (args_count == 0) {
                printf("Error parse command address: %d\n", current_address);
                return kErrorZeroArgs;

            } else {
                current_address += args_count;
            }
        }
    }
    *size = current_address;
    
    return kNoAsmError;
}

void AddLabel(Labels *labels, const char *name, int address) {
    assert(labels);
    assert(name);

    for (size_t i = 0; i < labels->count; i++) {
        if (strcmp(labels->data[i].name, name) == 0) {
            labels->data[i].address = address;
        }
    }

    strcpy(labels->data[labels->count].name, name);
    labels->data[labels->count].address = address;
    labels->count++;
}

int FindLabel(const Labels *labels, const char *name) {
    assert(labels);
    assert(name);

    for (size_t i = 0; i < labels->count; i++) {
        if (strcmp(labels->data[i].name, name) == 0) {
            return labels->data[i].address;
        }
    }

    return -1;
}