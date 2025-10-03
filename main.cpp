#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>

#include "StructsEnums.h"
#include "HandleFileWork.h"

int main(int argc, char *argv[]) {
    assert(argv);

    if (argc < 3) {
        fprintf(stderr, "%s, not enough arguments.\n", argv[0]);
        return kErrorParsing;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    FileInfo file_info = {};

    return HandleAsm(argv, input_filename, output_filename, &file_info);

}