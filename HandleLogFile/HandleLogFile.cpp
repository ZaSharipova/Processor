#include "HandleLogFile.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "ParseCommandLine.h"

static char currentLogFile[256] = "";
static FILE *file = NULL;

void SetLogFile(const char *filename) {
    assert(filename);

    snprintf(currentLogFile, sizeof(currentLogFile), "%s", filename);
    file = OpenFile(currentLogFile, "w");
}

FILE *GetLogFile(void) {
    return file;
}

