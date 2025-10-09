#ifndef HANDLE_CALC_H_
#define HANDLE_CALC_H_

#include <stdio.h>

#include "StructsEnums.h"

ProcessorErr_t Read(FILE *fin, Stack_t *code[], size_t *code_size);
int Calculate(FILE *fout, Processor *processor_info, int code_size, FILE *open_log_file);

#endif //HANDLE_CALC_H_