// #include "DisAsm.h"

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
// #include <assert.h>
// #include <sys/stat.h>
// #include <errno.h>

// #include "StructsEnums.h"
// #include "AssemblerEnums.h"
// #include "DoFileWork.h"
// #include "FileOperations.h"
// #include "SubsidiaryFunctionsAssembler.h"
// #include "DoFileWork.h"
// #include "StackFunctions.h"
// #include "ProcessorFunctions.h"
// #include "DoLogFile.h"

// AsmError DoPrepareToDisasm(FileInfo *file_info, Labels *labels, FILE *open_in_file, size_t *instruction_num) {
//     assert(file_info);
//     assert(open_in_file);
//     assert(labels);

//     Stack_t command_num = 0, arg_int = 0;
//     size_t instruction_counter = 0;
//     fscanf(open_in_file, "%d", instruction_num);

//     for (int i = 0; i < instruction_num; i++) {
//         fscanf(open_in_file, "%d", &command_num);

//         instruction_counter ++;
//         const char *command_name = commands[command_num].command_name;

//         if (commands[command_num].num_args == 1) {
//             fscanf(open_in_file, "%d", &arg_int);

//             instruction_counter ++;
//             if (command_num >= 14 && command_num <= 21) {
//                 AddLabel(labels, command_name, instruction_counter);
//             }
//         }
//     }

//     return kNoAsmError;
// }

// AsmError PrepareToDisasm(const char *line, FileInfo *file_info, Labels *labels, bool flag_do_what) {
//     assert(line);
//     assert(file_info);

//     size_t instruction_num = 0;
//     Stack_t command_num = 0, arg_int = 0;
//     fscanf(in_out_files->open_in_file, "%d", instruction_num);
//     size_t instruction_counter = 0;

//     for (int i = 0; i < instruction_num; i++) {
//         scanf("%d", &command_num);

//         instruction_counter ++;
//         const char *command_name = commands[command_num].command_name;
        
//         if (commands[command_num].num_args == 1) {
//             scanf("%d", &arg_int);

//             instruction_counter ++;
            
//         }
//     }

//     return kNoAsmError;
// }

// static char IntToString(int arg_int) {
//     return (char)(arg_int + 'A');
// }

// void WriteCommandsBack(FILE *output, FileInfo *file_info, const unsigned char *buf_out, int instr_count, int buf_size) {
//     assert(output);
//     assert(file_info);
//     assert(buf_out);

//     // buf_out содержит последовательность команд и аргументов; но у нас instr_count даёт кол-во инструкций,
//     // поэтому при выводе нужно корректно шагать по байтам. Здесь пройдём по байтам, распознавая команду и
//     // при наличии аргумента читаем следующий байт.
//     int i = 0;
//     int printed_instr = 0;
//     while (i < buf_size && printed_instr < instr_count) {
//         int command = (int) buf_out[i++];
//         const char *cmd_str = EnumToCommand(command);
//         if (!cmd_str) {
//             fprintf(output, "UNKNOWN(%d)\n", command);
//             printed_instr++;
//             continue;
//         }

//         fprintf(output, "%s", cmd_str);

//         if (command == kPush) {
//             if (i < buf_size) {
//                 int arg = (int) buf_out[i++];
//                 fprintf(output, " %d", arg);
//             } else {
//                 fprintf(output, " <missing-arg>");
//             }
//         } else if (command == kPushR || command == kPopR) {
//             if (i < buf_size) {
//                 int reg_idx = (int) buf_out[i++];
//                 fprintf(output, " R%cX", IntToString(reg_idx));
//             } else {
//                 fprintf(output, " <missing-reg>");
//             }
//         }

//         fprintf(output, "\n");
//         printed_instr++;
//     }
// }

// int DoDisAsm(FileInfo *file_info, const Files *in_out_files) {
//     assert(file_info);
//     assert(in_out_files);

//     unsigned char *buf_out = NULL;
//     int buf_capacity = 0;
//     int buf_size = 0;
//     int instr_count = 0;

//     for (int i = 0; i < file_info->count_lines; i++) {
//         LineInfo *line = &file_info->text_ptr[i];
//         size_t len = line->size;
//         // выделяем буфер под строку (+1 для '\0')
//         char *line_buf = (char *) calloc (len + 1, sizeof(char));
//         if (!line_buf) {
//             free(buf_out);
//             return kNoMemory;
//         }
//         memcpy(line_buf, line->start_ptr, len);
//         line_buf[len] = '\0';

//         int handle_error = DoParseAsm(line_buf, file_info, &buf_out, &instr_count, &buf_capacity, &buf_size);
//         if (handle_error < 0) {
//             free(line_buf);
//             free(buf_out);
//             return handle_error;
//         }

//         free(line_buf);
//     }

//     // заполним поле instruction_counter у file_info если нужно
//     //file_info->instruction_counter = instr_count;

//     WriteCommandsBack(in_out_files->open_out_file, file_info, buf_out, instr_count, buf_size);

//     free(buf_out);

//     free(file_info->buf_ptr);
//     free(file_info->text_ptr);

//     return 0;
// }

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
