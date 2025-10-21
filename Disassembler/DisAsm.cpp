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

AsmError DoPrepareToDisasm(FILE *input, Labels *labels) {
    assert(input);
    assert(labels);

    size_t instruction_num = 0;
    int command = 0, arg_int = 0;

    rewind(input);
    if (fscanf(input, "%zu", &instruction_num) != 1) {
        return kErrorZeroArgs;
    }

    size_t instruction_counter = 0;

    while (instruction_counter < instruction_num) {
        if (fscanf(input, "%d", &command) != 1)
            return kErrorZeroArgs;

        instruction_counter++;

        if (commands[command].num_args == 1) {
            if (fscanf(input, "%d", &arg_int) != 1) {
                return kErrorZeroArgs;
            }

            instruction_counter++;

            if (command >= kJmp && command <= kCall) {
                AddLabel(labels, arg_int);
            }
        }
    }

    rewind(input);
    return kNoAsmError;
}

AsmError WriteCommandsBack(FILE *output, FILE *input, const Labels *labels) {
    assert(output);
    assert(input);
    assert(labels);

    int command = 0, arg = 0;
    size_t instruction_num = 0;
    size_t instruction_counter = 0;

    fscanf(input, "%zu", &instruction_num);

    while (instruction_counter < instruction_num) {
        size_t cur_pos = instruction_counter;

        const char *label = FindLabelByAddr(labels, (int)cur_pos);
        if (label && cur_pos != 0) {
            fprintf(output, ":%s\n", label);
        }

        if (fscanf(input, "%d", &command) != 1) {
            return kErrorZeroArgs;
        }

        instruction_counter++;

        const char *command_str = EnumToCommand(command);
        if (!command_str) {
            fprintf(output, "UNKNOWN(%d)\n", command);
            continue;
        }

        fprintf(output, "%s", command_str);

        if (commands[command].num_args == 1) {
            if (fscanf(input, "%d", &arg) != 1) {
                return kErrorZeroArgs;
            }

            instruction_counter++;

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

AsmError DoDisAsm(FileInfo *file_info, const Files *in_out_files) {
    assert(file_info);
    assert(in_out_files);

    Labels labels = {};
    AsmError err = kNoAsmError;

    CHECK_ERROR_RETURN(DoPrepareToDisasm(in_out_files->open_in_file, &labels));

    return WriteCommandsBack(in_out_files->open_out_file, in_out_files->open_in_file, &labels);
}