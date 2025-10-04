#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "HandleCalc.h"
#include "StackFunctions.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "%s, not enough arguments.\n", argv[0]);
        return kErrorParsing;
    }

    const char *infilename = argv[1];
    const char *outfilename = argv[2];

#ifdef _ASM
    FileInfo file_info = {};

    return HandleAsm(argv, infilename, outfilename, &file_info);
#else


    StackErr_t err = kSuccess;

    infilename = argv[1];
    FILE *fin = OpenFile(infilename, READ_MODE);
    if (!fin) {
        return kErrorOpening;
    }

    outfilename = argv[2];
    FILE *fout = OpenFile(outfilename, WRITE_MODE);
    if (!fout) {
        return kErrorOpening;
    }

    Processor processor_info = {};
    CHECK_ERROR_RETURN(StackCtor(&processor_info.stack, 1, stdout));

    printf("%d\n", 66 * 66 - 4 * 15 * 5);
    Stack_t code_size = Read(fin, &processor_info.code);

    Calculate(fout, &processor_info, (int)code_size);
#endif

    return 0;
}