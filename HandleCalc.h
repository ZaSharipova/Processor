#ifndef HANDLE_CALC_H_
#define HANDLE_CALC_H_

#include <stdio.h>

#include "StructsEnums.h"

void Calculate(FILE *fin, FILE *fout, Stack_Info *stk);

FILE *OpenFile(const char *filename, const char *mode);
StackErr_t CloseFile(FILE *file);

#endif //HANDLE_CALC_H_