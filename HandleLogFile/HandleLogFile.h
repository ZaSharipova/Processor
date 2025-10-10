#ifndef HANDLE_LOG_FILE_H
#define HANDLE_LOG_FILE_H

#include <stdio.h>

void SetLogFile(const char *filename);
FILE *GetLogFile(void);

#endif //HANDLE_LOG_FILE_H_