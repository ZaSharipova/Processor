#include "HandleFileWork.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>

#include "StructsEnums.h"
#include "FileOperations.h"

PossibleErrors HandleBufRead(const char *filename, FileInfo *file_info) {
    assert(filename);
    assert(file_info);

    FILE *file = OpenFile(filename, READ_MODE);
    if (file == NULL) {
        return kErrorOpening;
    }

    file_info->filesize = SizeOfFile(filename);
    file_info->buf_ptr = ReadToBuf(filename, file, file_info->filesize);
    assert(file_info->buf_ptr != NULL);

    PossibleErrors err = CloseFile(file);
    if (err != kNoError) {
        PrintError(err);
        return err;
    }

    file_info->count_lines = CountLines(file_info->buf_ptr);
    file_info->text_ptr = (LineInfo *) calloc ((size_t)file_info->count_lines + 1, sizeof(LineInfo));
    assert(file_info->text_ptr != NULL);

    ParseBuf(file_info);

    return kNoError;
}

int CommandToEnum(const char *command) {
    assert(command);

    if (strcmp(command, PUSH) == 0) return Push;
    if (strcmp(command, POP)  == 0) return Pop;
    if (strcmp(command, ADD)  == 0) return Add;
    if (strcmp(command, SUB)  == 0) return Sub;
    if (strcmp(command, MUL)  == 0) return Mul;
    if (strcmp(command, DIV)  == 0) return Div;
    if (strcmp(command, SQRT) == 0) return Sqrt;
    if (strcmp(command, OUT)  == 0) return Out;
    if (strcmp(command, HLT)  == 0) return Hlt;

    return -1; 
}

const char *EnumToCommand(int command) {
    switch (command) {
        case Push: return PUSH;
        case Pop:  return POP;
        case Add:  return ADD;
        case Sub:  return SUB;
        case Mul:  return MUL;
        case Div:  return DIV;
        case Sqrt: return SQRT;
        case Out:  return OUT;
        case Hlt:  return HLT;
        default:   return NULL;
    }
}

void WriteCommand(FILE *output, int command, int has_arg, int arg) {
    assert(output);

    if (has_arg) {
        fprintf(output, "%d %d\n", command, arg);

    } else {
        fprintf(output, "%d\n", command);
    }
}

void WriteCommandBack(FILE *output, int command, int has_arg, int arg) {
    assert(output);

    const char *char_command = EnumToCommand(command);
    if (has_arg) {
        fprintf(output, "%s %d\n", char_command, arg);

    } else {
        fprintf(output, "%s\n", char_command);
    }
}

int HandleParseAndWriteInstruction(FILE *output, const char *line) {
    assert(output);
    assert(line);

    char *cmd = (char *) calloc (16, sizeof(char));
    int arg = 0;
    NumOfArgs has_arg = kZero;

    int num_parsed = sscanf(line, "%15s %d", cmd, &arg);
    if (num_parsed == 1) {
        has_arg = kZero; //
    }

    else if (num_parsed == 2) {
        has_arg = kOne;

    } else {
        return -1;
    }

    int command_to_enum = CommandToEnum(cmd);
    if (command_to_enum == -1) {
        return -1;
    }

#ifdef _DISASM
    WriteCommandBack(output, command_to_enum, has_arg, arg);
#else
    WriteCommand(output, command_to_enum, has_arg, arg);
#endif
    free(cmd);

    return 0;
}

int HandleAsm(char **argv, const char *input_filename, const char *output_filename, FileInfo *file_info) {
    assert(argv);
    assert(input_filename);
    assert(output_filename);

    PossibleErrors err = HandleBufRead(input_filename, file_info);
    if (err != kNoError) {
        return err;
    }

    FILE *out_file = OpenFile(output_filename, WRITE_MODE);
    if (out_file == NULL) {
        free(file_info->buf_ptr);
        free(file_info->text_ptr);

        PrintError(kErrorOpening);
        return kErrorOpening;
    }

    for (int i = 0; i < file_info->count_lines; i++) {
        LineInfo *line = &file_info->text_ptr[i];
        size_t len = line->size + 1;

        char *line_buf = (char *) calloc (len + 1, sizeof(char));
        memcpy(line_buf, line->start_ptr, len);
        line_buf[len] = '\0';

        if (HandleParseAndWriteInstruction(out_file, line_buf) != 0) {
            fprintf(stderr, "Parse error on line %d: %s.\n", i + 1, line_buf); //
        }

        free(line_buf);
    }

    err = CloseFile(out_file);
    if (err != kNoError) {
        PrintError(err);
        return err;
    }

    free(file_info->buf_ptr);
    free(file_info->text_ptr);

    return 0;
}