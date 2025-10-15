#ifndef HANDLE_LOG_FILE_H
#define HANDLE_LOG_FILE_H

#include <stdio.h>

#include "AssemblerEnums.h"

void SetLogFile(const char *filename);
FILE *GetLogFile(void);
ParseErr_t CloseLogFile(void);

#endif //HANDLE_LOG_FILE_H_