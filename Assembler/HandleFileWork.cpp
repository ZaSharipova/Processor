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
    if (strcmp(command, JB)    == 0) return kJB;
    if (strcmp(command, JBE)   == 0) return kJBE;
    if (strcmp(command, JA)    == 0) return kJA;
    if (strcmp(command, JAE)   == 0) return kJAE;
    if (strcmp(command, JE)    == 0) return kJE;
    if (strcmp(command, JNE)   == 0) return kJNE;
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

static int ParseArgumentValue(char *arg_str, int *out_val, int *labels) {
    assert(arg_str);
    assert(out_val);
    assert(labels);

    if (*arg_str == ':') {
        arg_str++;

        int label_num = 0;
        if (sscanf(arg_str, "%d", &label_num) != 1) {
            return -1;
        }

        *out_val = labels[label_num];
        return 0;
    }

    char *end_ptr = NULL;
    errno = 0;
    long val = strtol(arg_str, &end_ptr, 0);
    if (end_ptr != arg_str && *end_ptr == '\0' && errno == 0) {
        *out_val = (int)val;
        return 0;
    }

    *out_val = StringToInt(arg_str);
    return 0;
}


int HandleParse(const char *line, FileInfo *file_info, int **buf_out, int *num_args, int *labels) {
    assert(line);
    assert(file_info);
    assert(buf_out);
    assert(num_args);
    assert(labels);

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

    //printf("%s %s\n", command, arg_str);
    NumOfArgs has_arg = (args_count == 2) ? kOne : kZero;
    if (args_count != 1 && args_count != 2) {
        free(command);
        free(arg_str); //
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
    // if (has_arg == kOne) {
    //     ParseArgumentValue(arg_str, &arg_int, labels);
    // }

    int command_to_enum = CommandToEnum(command);
    if (command_to_enum == -1) {
        file_info->instruction_counter = (int)pos;
        printf("%s", arg_str);
        return kNoAvailableCommand;
    }

    if (has_arg == kOne) {
        ParseArgumentValue(arg_str, &arg_int, labels);
    }

    (*buf_out)[pos] = command_to_enum;
    if (has_arg == kOne) (*buf_out)[pos + 1] = arg_int;

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


int HandleAsm(FileInfo *file_info, Files in_out_files, int *labels) {
    assert(file_info);
    assert(labels);

    int *buf_out = (int *) calloc((size_t)file_info->instruction_counter, sizeof(int));
    if (buf_out == NULL) {
        return kNoMemory;
    }

    for (int i = 0; i < file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];

        int args_check = 0;
        int handle_error = HandleParse(line->start_ptr, file_info, &buf_out, &args_check, labels);
        if (handle_error < 0 && handle_error != kWhitespace) {
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

// int HandleAsm(FileInfo *file_info, Files in_out_files, int *labels) {
//     assert(file_info);
//     assert(labels);
//     assert(labels);

//     PossibleErrorsAsm err = kNoErrorAsm;
//     ERROR_CHECK_RETURN(HandleBufRead(in_out_files, file_info));

//     int *buf_out = (int *) calloc ((size_t)file_info->instruction_counter, sizeof(int));
//     if (buf_out == NULL) {
//         return kNoMemory;
//     }

//     for (int i = 0; i < file_info->count_lines; i++) {
//         LineInfo *line = &file_info->text_ptr[i];
        
//         int args_check = 0;
//         int handle_error = HandleParse(line->start_ptr, file_info, &buf_out, &args_check, labels);
//         if (handle_error < 0) {
//             free(buf_out);
//             fprintf(stderr, "Parse error on line %d\n", i + 1);
//             return handle_error;
//         }

//     }
    
//     HandleWriteCommands(in_out_files.open_out_file, file_info, buf_out);
//     free(buf_out);

//     free(file_info->buf_ptr);
//     free(file_info->text_ptr);

//     return kNoError;
// }

PossibleErrorsAsm HandlePreAssemble(Files files, FileInfo *file_info, int *labels) {
    assert(file_info);
    assert(labels);

    file_info->filesize = SizeOfFile(files.in_file);
    file_info->buf_ptr = ReadToBuf(files.open_in_file, file_info->filesize);
    assert(file_info->buf_ptr != NULL);

    file_info->count_lines = CountLines(file_info->buf_ptr);
    file_info->text_ptr = (LineInfo *) calloc((size_t)file_info->count_lines + 1, sizeof(LineInfo));
    assert(file_info->text_ptr != NULL);
    ParseBuf(file_info);

    AsmError err = PreAssemble(file_info, labels);
    if (err != kNoAsmError) {
        return (PossibleErrorsAsm) err;
    }
    // for (int i = 0; i < 10; i++) {
    //     printf("%d ", labels[i]);
    // }
    // printf("\n\n");
    return kNoErrorAsm;
}

AsmError PreAssemble(FileInfo *file_info, int *labels) {
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

            const char *line_ptr = SkipWhitespace(line->start_ptr);
            if (*line_ptr == '\0') {
                return kWhitespace;
            }

            char *command = (char *) calloc (16, sizeof(char));
            if (command == NULL) {
                return kNoMemoryCalloc;
            }

            char *arg_str = (char *) calloc (64, sizeof(char));
            if (arg_str == NULL) {
                free(command);
                return kNoMemoryCalloc;
            }

            int args_count = sscanf(line_ptr, "%15s %63s", command, arg_str);
            if (args_count == 0) {
                printf("Error parse %s command address: %d\n", command, current_address);
                free(command);
                free(arg_str);
                return kErrorZeroArgs;

            } else {
                current_address += args_count;
                //printf("%d ", current_address);
                free(command);
                free(arg_str);
            }
        }
    }
    
    return kNoAsmError;
}

// AsmError PreAssemble(char *buf_in, int *labels) {
//     assert(buf_in);
//     assert(labels);

//     int current_address = 0;
//     char *ptr = buf_in;
//     char *line_start = ptr;
//     int flag_change_current_address = 1;

//     while (*ptr != '\0') {
//         flag_change_current_address = 1;
//         while (*ptr && isspace((unsigned char)*ptr)) {
//             ptr++;
//             line_start = ptr;
//             flag_change_current_address = 0;
//         }
//         //if (*ptr == '\0') break;

//         if (*ptr == ';') {
//             while (*ptr && *ptr != '\n') ptr++;
//             if (*ptr == '\n') {
//                 ptr++;
//                 line_start = ptr;
//             }
//             flag_change_current_address = 0;
//         }

//         char *line_end = ptr;
//         while (*line_end && *line_end != '\n') {
//             line_end++; // находим указатель на конец строки
//         }

//         if (*ptr == ':') {
//             int label_num = 0;
//             if (sscanf(ptr + 1, "%d", &label_num) == 1) {
//                 if (line_start[0] == ':') {
//                     labels[label_num] = current_address;
//                     flag_change_current_address = 0;
//                 } else {
//                     current_address++;
//                     flag_change_current_address = 0;
//                 }
//             } else {
//                 printf("Syntax error!!!!\n");
//                 flag_change_current_address = 0;
//                 //return
//             }

//             ptr = line_end;
//             if (*ptr == '\n') {
//                 ptr++;
//                 line_start = ptr;
//             }
//             //continue;
//         }

//         if (flag_change_current_address == 1) {
//             current_address += 2;
//         }

//         ptr = line_end;
//         if (*ptr == '\n') {
//             ptr++;
//             line_start = ptr;
//         }
//         printf("%d ", current_address);
//     }

//     for (int i = 0; i < 10; i++) {
//         printf("%d ", labels[i]);
//     }
//     printf("\n\n");

//     return 0;
// }