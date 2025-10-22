#ifndef DIS_ASM_H_
#define DIS_ASM_H_

#include "StructsEnums.h"
#include "DoFileWork.h"
#include "AssemblerEnums.h"

// AsmError DoPrepareToDisasm(FileInfo *file_info, Labels *labels, Files *in_out_files);
// AsmError PrepareToDisasm(const char *line, FileInfo *file_info, Labels *labels, bool flag_do_what);
// int DoParseAsm(const char *line, FileInfo *file_info,
//                unsigned char **buf_out, int *instr_count, int *buf_capacity, int *buf_size);
// void WriteCommandsBack(FILE *output, FileInfo *file_info, const unsigned char *buf_out, int instr_count, int buf_size) ;
// int DoDisAsm(FileInfo *file_info, const Files *in_out_files) ;


AsmError DoPrepareToDisasm(FILE *input, Labels *labels, Stack_t *code[], int code_size);
AsmError WriteCommandsBack(FILE *output, FILE *input, const Labels *labels, Stack_t *code[], int code_size);
AsmError DoDisAsm(FileInfo *file_info, const Files *in_out_files, Stack_t *code[], int code_size);

// PossibleErrorsAsm HandleBufRead(Files in_out_files, FileInfo *file_info);

#endif //DIS_ASM_H_