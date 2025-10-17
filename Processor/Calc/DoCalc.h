#ifndef DO_CALC_H_
#define DO_CALC_H_

#include <stdio.h>

#include "StructsEnums.h"

ProcessorErr_t Read(FILE *fin, Stack_t *code[], size_t *code_size);
int Calculate(FILE *fout, Processor *processor_info, size_t code_size);

#endif //DO_CALC_H_