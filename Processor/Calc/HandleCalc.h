#ifndef HANDLE_CALC_H_
#define HANDLE_CALC_H_

#include <stdio.h>

#include "StructsEnums.h"

size_t Read(FILE *fin, int *instruction_counter, Stack_t *code[]);
int Calculate(FILE *fout, Processor *processor_info, int code_size, FILE *open_log_file);

#endif //HANDLE_CALC_H_