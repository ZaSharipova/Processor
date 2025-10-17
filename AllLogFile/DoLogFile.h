#ifndef DO_LOG_FILE_H
#define DO_LOG_FILE_H

#include <stdio.h>

#include "AssemblerEnums.h"

void SetLogFile(const char *filename);
FILE *GetLogFile(void);
ParseErr_t CloseLogFile(void);

#endif //DO_LOG_FILE_H_