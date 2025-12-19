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
#include "Commands.h"

#define MAX_ARG_LEN 16
static AsmError GetLabels(FileInfo *file_info, AssemblerInfo *Assembler);
void AddLabel(Labels *labels, const char *name, int address);
int FindLabel(const Labels *labels, const char *name);
static void WriteCommandsOut(FILE *output, FileInfo *file_info, const Stack_Info *buf_out);
static AsmError DoScanfAndConvert(const char *line_ptr, FileInfo *file_info, AssemblerInfo *Assembler, bool flag_push, int *num_instructions);
static AsmError ParseLabelArgument(const char *arg_str, int *out_val, Labels *labels, bool do_parse);

static AsmError DoScanfAndConvert(const char *line_ptr, FileInfo *file_info, AssemblerInfo *assembler, bool flag_push, int *num_instructions);
static AsmError GetLabels(FileInfo *file_info, AssemblerInfo *assembler);
AsmError DoParse(const char *line, FileInfo *file_info, AssemblerInfo *assembler);

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

AsmError PrepareToAssemble(const Files *files, FileInfo *file_info, AssemblerInfo *assembler) {
    assert(files);
    assert(file_info);
    assert(assembler);

    AllBufRead(files, file_info);

    return GetLabels(file_info, assembler);
}

AsmError DoAsm(FileInfo *file_info, const Files *in_out_files, AssemblerInfo *assembler) {
    assert(file_info);
    assert(in_out_files);
    assert(assembler);

    for (size_t i = 0; i < (size_t)file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];

        AsmError handle_error = DoParse(line->start_ptr, file_info, assembler);
        if (handle_error < 0 && handle_error != kWhitespace) {
            StackDtor(&(assembler->data));
            // printf("%s %d", line->start_ptr, i);
            fprintf(GetLogFile(), "Parse error on line %lu\n", i + 1);
            return handle_error;
        }
    }

    WriteCommandsOut(in_out_files->open_out_file, file_info, &(assembler->data));

    free(file_info->buf_ptr);
    free(file_info->text_ptr);

    return kNoAsmError;
}

static AsmError ParseLabelArgument(const char *line_ptr, int *out_val, Labels *labels, bool do_parse) {
    assert(line_ptr);
    assert(out_val);
    assert(labels);

    char arg_str[MAX_ARG_LEN] = {};
    if (sscanf(SkipWhitespace(line_ptr), "%s", arg_str) != 1) {
        fprintf(stderr, "Error: failed to read label argument\n");
        return kErrorParsingAsm;
    }

    if (do_parse && arg_str[0] == ':') {
        const char *label_name = arg_str + 1;
        int address = FindLabel(labels, label_name);
        if (address == -1) {
            fprintf(stderr, "Error: label '%s' not found!\n", label_name);
            return kErrorParsingAsm;
        }

        *out_val = address;
        return kNoAsmError;
    } else if (do_parse){
        fprintf(stderr, "Error: expected ':' before label name: %s\n", arg_str);
        return kErrorParsingAsm;
    }
    return kNoAsmError;
}


static AsmError ParseRValue(TypeOfArg arg_type, const char *line_ptr, int *out_val) {
    assert(line_ptr);
    assert(out_val);

    char arg_str[MAX_ARG_LEN] = {};

    int sscanf_err = sscanf(SkipWhitespace(line_ptr), "%s", arg_str);
    if (sscanf_err <= 0) {
        fprintf(stderr, "Error parsing commands.\n");
        return kErrorParsingAsm;
    }

    char *ptr = arg_str;
    char inner[32] = "";
    if (arg_type == kRamArg && (arg_str[0] != '[' || arg_str[strlen(arg_str) - 1] != ']')) {
        fprintf(stderr, "Error: memory argument must be in brackets, got '%s'\n", arg_str);
        return kErrorParsingAsm;
    }
    if (arg_type == kRamArg) {
        size_t arg_len = strlen(arg_str);
        strncpy(inner, arg_str + 1, arg_len - 2);
        inner[arg_len - 2] = '\0';

        char *endptr = NULL;
        long address = strtol(inner, &endptr, 10);

        if (*endptr == '\0') {
            *out_val = (int)address;
            return kNoAsmError;

        }
    } else {
        strcpy(inner, ptr);
    }

    int to_int_result =  StringToInt(inner);
    if (to_int_result < 0) {
        return kErrorParsingAsm;
    }
    *out_val = to_int_result;

    return kNoAsmError;

}

static AsmError ParseArgumentValue(AssemblerInfo *assembler, TypeOfArg arg_type, const char *line_ptr, int *out_val, bool do_parse) {
    assert(assembler);
    assert(line_ptr);
    assert(out_val);
    
    int sscanf_err = kNoAsmError;

    switch (arg_type) {
        case (kRamArg):
        case (kRaxArg):
            ParseRValue(arg_type, line_ptr, out_val);
            break;

        case (kIntArg):
            sscanf_err = sscanf(SkipWhitespace(line_ptr), "%d", out_val);
            if (sscanf_err <= 0) {
                return kErrorParsingAsm;
            }
            break;

        case (kLabelArg):
            ParseLabelArgument(line_ptr, out_val, &assembler->labels, do_parse);
            break;

        case (kNoArg):
            break;

        default:
            printf("Error, no such command.");
    }

    return kNoAsmError;
}

static AsmError DoScanfAndConvert(const char *line_ptr, FileInfo *file_info, AssemblerInfo *assembler, bool flag_push, int *num_instructions) {
    assert(line_ptr);
    assert(file_info);
    assert(assembler);
    assert(num_instructions);

    char cmd_name[MAX_ARG_LEN] = {}, 
    arg_str[MAX_ARG_LEN] = {};

    line_ptr = SkipWhitespace(line_ptr);
    int scanf_num = 0;
    AsmError err = kNoAsmError;

    scanf_num = sscanf(line_ptr, "%s", cmd_name);
    if (scanf_num <= 0) {
        return kErrorZeroArgs;
    }

    char *ptr = (char *)SkipWhitespace(line_ptr + strlen(cmd_name));
   
    int command_to_enum_pos = CommandToEnumPos(cmd_name);

    if (command_to_enum_pos == -1) {
        printf("Command not found.\n");
        return kErrorZeroArgs;
    }

    if (flag_push) {
        CHECK_ERROR_RETURN((AsmError)StackPush(&assembler->data, (Stack_t)commands[command_to_enum_pos].command_num));
    }

    const CommandsInfo *commands_info = &commands[command_to_enum_pos];

    int out_val = 0;

    switch(commands_info->num_args) {
    case (0):
        *num_instructions = 1;
        break;
    
    case(1):
        ParseArgumentValue(assembler, (commands_info->type_of_arg), ptr, &out_val, flag_push);
        *num_instructions = 2;

        if (flag_push) {
            CHECK_ERROR_RETURN((AsmError)StackPush(&assembler->data, out_val));
        }
        break;

    default:
        break;
    }

    return kNoAsmError;
}

AsmError DoParse(const char *line, FileInfo *file_info, AssemblerInfo *assembler) {
    assert(line);
    assert(file_info);
    assert(assembler);

    const char *line_ptr = SkipWhitespace(line);
    if (*line_ptr == '\0') {
        return kWhitespace;
    }

    bool flag_push_stack = true;
    int num_instructions = 0;
    AsmError err = DoScanfAndConvert(line_ptr, file_info, assembler, flag_push_stack, &num_instructions);
    if (err != kNoAsmError) {
        return err;
    }

    return kNoAsmError;
}

Stack_t GetStackElem(const Stack_Info *buf_out, size_t pos) {
    assert(buf_out);

    return buf_out->data[pos];
}

static void WriteCommandsOut(FILE *output, FileInfo *file_info, const Stack_Info *buf_out) {
    assert(output);
    assert(file_info);
    assert(buf_out);

    fprintf(output, "%zd ", buf_out->size);
    //getelem
    for (size_t i = 0; i < (size_t)buf_out->size; i++) {
        fprintf(output, "%d ", GetStackElem(buf_out, i));
    }
}

static AsmError GetLabels(FileInfo *file_info, AssemblerInfo *assembler) {
    assert(file_info);
    assert(assembler);

    int current_address = 0;
    for (int i = 0; i < file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];

        if ((line->start_ptr)[0] == ':') {

            char *label_name = line->start_ptr + 1;
            sscanf(line->start_ptr + 1, "%s", label_name);
            AddLabel(&assembler->labels, label_name, current_address);

            *line->start_ptr = '\0';

        } else {
            const char *line_ptr = line->start_ptr;

            bool flag_push_stack = false;
            int args_count = 0;
            AsmError scanf_err = kNoAsmError;
            scanf_err = DoScanfAndConvert(line_ptr, file_info, assembler, flag_push_stack, &args_count);

            if (scanf_err != kNoAsmError) {
                printf("Scanf error in %s.", line_ptr);
                return scanf_err;
            }

            if (args_count == 0) {
                printf("Error parse command address: %d\n", current_address);
                return kErrorZeroArgs;

            } else {
                current_address += args_count;
            }
        }
    }
    
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

AsmError AssemblerCtor(AssemblerInfo *assembler) {
    assert(assembler);

    assembler->commands_counter = 0;

    for (size_t i = 0; i < MAX_ARR_SIZE; i++) {
        assembler->labels.data[i].address = 0;
        assembler->labels.data[i].name[0] = '\0';
    }

    assembler->labels.count = 0; //
    return (AsmError)StackCtor(&assembler->data, 1);
}