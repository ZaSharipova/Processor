#include "HandleLogFile.h"

#include <stdio.h>
#include <assert.h>

#include "FileOperations.h"
#include "StructsEnums.h"

static FILE *opened_file = NULL;

void SetLogFile(const char *filename) {
    assert(filename);

    opened_file = OpenFile(filename, WRITE_MODE);
    if (opened_file == NULL) {
        printf("Error opening logfile, so all the mistakes will be seen in STDOUT.\n");
        opened_file = stdout;
    }
}

FILE *GetLogFile(void) {
    return opened_file;
}

ParseErr_t CloseLogFile(void) { //
    if (opened_file != stdout) {
        return CloseFile(opened_file);
    }
    return kNoError;
}