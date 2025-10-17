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

// PossibleErrorsAsm HandleBufRead(Files in_out_files, FileInfo *file_info) {
//     assert(file_info);

//     file_info->filesize = SizeOfFile(in_out_files.in_file);
//     file_info->buf_ptr = ReadToBuf(in_out_files.open_in_file, file_info->filesize);
//     assert(file_info->buf_ptr != NULL);

//     file_info->count_lines = CountLines(file_info->buf_ptr);
//     file_info->text_ptr = (LineInfo *) calloc ((size_t)file_info->count_lines + 1, sizeof(LineInfo));
//     assert(file_info->text_ptr != NULL);

//     ParseBuf(file_info);

//     return kNoErrorAsm;
// }

const char *EnumToCommand(int command) {
    switch (command) {
        case kPush:  return PUSH;
        case kPop:   return POP;
        case kAdd:   return ADD;
        case kSub:   return SUB;
        case kMul:   return MUL;
        case kDiv:   return DIV;
        case kSqrt:  return SQRT;
        case kIn:    return IN;
        case kOut:   return OUT;
        case kPushR: return PUSHR;
        case kPopR:  return POPR;
        case kHlt:   return HLT;
        default:     return NULL;
    }
}

static const char *SkipWhitespace(const char *str) {
    assert(str);

    while (isspace((unsigned char) *str)) {
        str++;
    }
    return str;
}

static char IntToString(int arg_int) {
    return (char)(arg_int + 'A');
}

int DoParseAsm(const char *line, FileInfo *file_info, char **buf_out, int *num_args) { //
    assert(file_info);
    assert(buf_out);
    assert(num_args);

    const char *line_ptr = SkipWhitespace(line);
    if (*line_ptr == '\0')
        return kWhitespace;

    int bytes_read = 0;
    int commands_number = 0;
    sscanf(line_ptr, "%d%n", &commands_number, &bytes_read);
    line_ptr += bytes_read;

    int command = 0;
    int arg_int = 0;
    int has_arg = 0;
    int total_commands = 0;

    while (*line_ptr != '\0') {
        if (sscanf(line_ptr, "%d%n", &command, &bytes_read) != 1)
            break;

        line_ptr += bytes_read;
        line_ptr = SkipWhitespace(line_ptr);

        has_arg = 0;
        arg_int = 0;

        if (command == kPush || command == kPushR || command == kPopR) {
            if (sscanf(line_ptr, "%d%n", &arg_int, &bytes_read) == 1) {
                has_arg = 1;
                line_ptr += bytes_read;
                line_ptr = SkipWhitespace(line_ptr);
            } else {
                fprintf(stderr, "Expected argument after command %d\n", command);
                return kNoMemory;
            }
        }

        size_t pos = (size_t)file_info->instruction_counter;
        size_t new_count = pos + (size_t)(has_arg + 1);

        char *realloc_ptr = (char *)realloc(*buf_out, new_count * sizeof(char));
        if (!realloc_ptr)
            return kNoMemory;
        *buf_out = realloc_ptr;

        (*buf_out)[pos] = (char)command;
        if (has_arg)
            (*buf_out)[pos + 1] = (char)arg_int;

        file_info->instruction_counter = (int)new_count;
        total_commands++;

        const char *cmd_str = EnumToCommand(command);
        printf("Parsed: cmd=%d (%s), arg=%d, has_arg=%d, next='%s'\n",
               command, cmd_str ? cmd_str : "???", arg_int, has_arg, line_ptr);
    }

    *num_args = total_commands;
    return (total_commands > 0) ? kNoError : kWhitespace;
}



// int DoParseAsm(const char *line, FileInfo *file_info, char **buf_out, int *num_args) {
//     assert(line);
//     assert(file_info);
//     assert(buf_out);
//     assert(num_args);

//     const char *line_ptr = SkipWhitespace(line);
//     if (*line_ptr == '\0')
//         return kWhitespace;

//     int command = 0;
//     int arg_int = 0;
//     int has_arg = 0;
//     int bytes_read = 0;  // <-- сюда `sscanf` запишет, сколько символов прочитано

//     int commands_number = 0;
//     sscanf(line_ptr, "%d%n", &commands_number, &bytes_read);
//     line_ptr += bytes_read;
//     if (sscanf(line_ptr, "%d%n", &command, &bytes_read) != 1) {
//         fprintf(stderr, "Parse error: expected command at '%s'\n", line_ptr);
//         return kNoAvailableCommand;
//     }
//     line_ptr += bytes_read;
//     line_ptr = SkipWhitespace(line_ptr);

//     if (command == kPush || command == kPopR || command == kPushR) {
//         if (sscanf(line_ptr, "%d%n", &arg_int, &bytes_read) == 1) {
//             has_arg = 1;
//             line_ptr += bytes_read;
//             line_ptr = SkipWhitespace(line_ptr);
//         } else {
//             fprintf(stderr, "Parse error: expected argument for command %d\n", command);
//             return kNoMemory; //
//         }
//     }

//     size_t pos = (size_t)file_info->instruction_counter;
//     size_t new_count = pos + (size_t)(has_arg + 1);

//     char *realloc_ptr = (char *)realloc(*buf_out, new_count * sizeof(char));
//     if (!realloc_ptr)
//         return kNoMemory;

//     *buf_out = realloc_ptr;

//     const char *cmd_str = EnumToCommand(command);
//     if (!cmd_str) {
//         fprintf(stderr, "Unknown command code: %d\n", command);
//         return kNoAvailableCommand;
//     }
//     (*buf_out)[pos] = (char)command;
//     if (has_arg)
//         (*buf_out)[pos + 1] = (char)arg_int;

//     file_info->instruction_counter = (int)new_count;
//     *num_args = has_arg + 1;

//     printf("Parsed: cmd=%d (%s), arg=%d, has_arg=%d, next ptr='%s'\n",
//            command, cmd_str, arg_int, has_arg, line_ptr);

//     return kNoError;
// }



// int DoParseAsm(const char *line, FileInfo *file_info, char **buf_out, int *num_args) {
//     assert(line);
//     assert(file_info);
//     assert(buf_out);
//     assert(num_args);

//     const char *line_ptr = SkipWhitespace(line);
//     if (*line_ptr == '\0') {
//         return kWhitespace;
//     }

//     int command = 0, arg_int = 0;

//     int args_count = sscanf(line_ptr, "%d %d", command, arg_int);

//     NumOfArgs has_arg = (args_count == 2) ? kOne : kZero;
//     if (args_count != 1 && args_count != 2) {
//         return kErrorZeroArgs;
//     }

//     size_t pos = (size_t)file_info->instruction_counter;
//     size_t new_count = pos + (size_t)(has_arg + 1);

//     char *realloc_ptr = (char *) realloc (*buf_out, new_count * sizeof(char));
//     if (realloc_ptr == NULL) {
//         return kNoMemory;
//     }

//     *buf_out = realloc_ptr;
//     file_info->instruction_counter = (int)new_count;

//     char *enum_to_command = EnumToCommand(command);
//     if (enum_to_command == NULL) {
//         file_info->instruction_counter = (int)pos;
//         return kNoAvailableCommand;
//     }

//     (*buf_out)[pos] = *enum_to_command;
//     if (has_arg == kOne) {
//         (*buf_out)[pos + 1] = arg_int;
//     }

//     *num_args = has_arg + 1;
//     return kNoError;
// }

void WriteCommandsBack(FILE *output, FileInfo *file_info, char *buf_out) {
    assert(output);
    assert(file_info);
    assert(buf_out);

    for (int i = 0; i < file_info->instruction_counter; i++) {
        int command = (unsigned char)buf_out[i];
        const char *cmd_str = EnumToCommand(command);
        if (!cmd_str) {
            fprintf(output, "UNKNOWN(%d)\n", command);
            continue;
        }

        fprintf(output, "%s", cmd_str);

        if (command == kPush) {
            int arg = (unsigned char)buf_out[++i];
            fprintf(output, " %d", arg);
        } else if (command == kPushR || command == kPopR) {
            fprintf(output, " R%cX", IntToString(buf_out[++i]));
        }

        fprintf(output, "\n");
    }
}

int DoDisAsm(FileInfo *file_info, Files in_out_files) {
    assert(file_info);

    AsmError err = HandleBufRead(in_out_files, file_info);
    if (err != kNoAsmError) {
        return err;
    }

    char *buf_out = (char *) calloc ((size_t)file_info->instruction_counter, sizeof(char));
    if (buf_out == NULL) {
        return kNoMemory;
    }

    for (int i = 0; i < file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];
        size_t len = line->size + 1;

        char *line_buf = (char *) calloc (len + 1, sizeof(char));
        memcpy(line_buf, line->start_ptr, len); //
        line_buf[len] = '\0';
        
        int args_check = 0;
        int handle_error = DoParseAsm(line_buf, file_info, &buf_out, &args_check);
        if (handle_error < 0) {
            free(buf_out);
            return handle_error;
        }
        if (args_check < 0) {
            file_info->instruction_counter += args_check;
            fprintf(stderr, "Parse error on line %d: %s\n", i + 1, line_buf); //
        }

        free(line_buf);
    }
    
    WriteCommandsBack(in_out_files.open_out_file, file_info, buf_out);
    free(buf_out);

    free(file_info->buf_ptr);
    free(file_info->text_ptr);

    return 0;
}