#include "DisAsm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
#include <errno.h>

#include "StructsEnums.h"
#include "AssemblerEnums.h"
#include "DoFileWork.h"
#include "FileOperations.h"
#include "SubsidiaryFunctionsAssembler.h"
#include "StackFunctions.h"
#include "ProcessorFunctions.h"
#include "DoLogFile.h"

static const char *FindLabelByAddr(const Labels *labels, int address) {
    assert(labels);

    for (size_t i = 0; i < labels->count; i++) {
        if (labels->data[i].address == address) {
            return labels->data[i].name;
        }
    }

    return NULL;
}

static void AddLabel(Labels *labels, int address) {
    assert(labels);

    char buf[MAX_ARR_SIZE] = "---";

    snprintf(buf, sizeof(buf), "%zu", labels->count + 1);
    labels->data[labels->count].address = address;
    strcpy(labels->data[labels->count].name, buf);

    labels->count++;
}

AsmError DoPrepareToDisasm(FILE *input, Labels *labels, Stack_t *code[], int code_size) {
    assert(input);
    assert(labels);
    assert(code);

    int command = 0, arg_int = 0;

    int instruction_counter = 0;

    while (instruction_counter < code_size) {
        command = (*code)[instruction_counter];

        instruction_counter++;

        if (commands[command].num_args == 1) {
            arg_int = (*code)[instruction_counter];

            instruction_counter++;

            if (command >= kJmp && command <= kCall) {
                AddLabel(labels, arg_int);
            }
        }
    }

    return kNoAsmError;
}

AsmError WriteCommandsBack(FILE *output, FILE *input, const Labels *labels, Stack_t *code[], int code_size) {
    assert(output);
    assert(input);
    assert(labels);
    assert(code);

    int command = 0, arg = 0;
    int instruction_counter = 0;

    while (instruction_counter < code_size) {
        int cur_pos = instruction_counter;

        const char *label = FindLabelByAddr(labels, (int)cur_pos);
        if (label && cur_pos != 0) {
            fprintf(output, ":%s\n", label);
        }

        command = (*code)[instruction_counter++];

        const char *command_str = EnumToCommand(command);
        if (!command_str) {
            fprintf(output, "UNKNOWN(%d)\n", command);
            continue;
        }

        fprintf(output, "%s", command_str);

        if (commands[command].num_args == 1) {
            arg = (*code)[instruction_counter++];

            if (command == kPushR || command == kPopR) {
                fprintf(output, " R%cX", (char)(arg + 'A'));

            } else if (command == kPushM || command == kPopM) {
                fprintf(output, " [R%cX]", (char)(arg + 'A'));

            } else if (command >= kJmp && command <= kCall) {
                label = FindLabelByAddr(labels, arg);
                if (label) {
                    fprintf(output, " :%s", label);
                } else {
                    fprintf(output, " :%d", arg);
                }
            }

            else {
                fprintf(output, " %d", arg);
            }
        }

        fprintf(output, "\n");
    }

    return kNoAsmError;
}

AsmError DoDisAsm(FileInfo *file_info, const Files *in_out_files, Stack_t *code[], int code_size) {
    assert(file_info);
    assert(in_out_files);
    assert(code);

    Labels labels = {};
    AsmError err = kNoAsmError;

    CHECK_ERROR_RETURN(DoPrepareToDisasm(in_out_files->open_in_file, &labels, code, code_size));

    return WriteCommandsBack(in_out_files->open_out_file, in_out_files->open_in_file, &labels, code, code_size);
}