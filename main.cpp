#include <stdio.h>
#include <stdlib.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "HandleCalc.h"
#include "StackFunctions.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "%s. commands input failed.\n", argv[0]);
        return kErrorParsing;
    }

    StackErr_t err = kSuccess;

    const char *infilename = argv[1];
    FILE *fin = OpenFile(infilename, READ_MODE);
    if (!fin) {
        return kErrorOpening;
    }

    const char *outfilename = argv[2];
    FILE *fout = OpenFile(outfilename, WRITE_MODE);
    if (!fout) {
        return kErrorOpening;
    }

    Stack_Info stk = {};
    CHECK_ERROR_RETURN(StackCtor(&stk, 1, stdout));

    printf("%d\n", 66 * 66 - 4 * 15 * 5);
    Calculate(fin, fout, &stk);
}