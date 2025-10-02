#include <stdio.h>
#include <stdlib.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"
#include "HandleCalc.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "%s. commands failed,\n", argv[0]);
        return 1;
    }

    const char *infilename = argv[1];
    FILE *fin = fopen(infilename, "r");
    if (!fin) {
        perror("Error opening input file,");
        return 1;
    }

    const char *outfilename = argv[2];
    FILE *fout = fopen(outfilename, "w");
    if (!fout) {
        perror("Error opening output file.");
        fclose(fin);
        return 1;
    }

    Stack_Info stk = {};
    stk.capacity = 1;
    stk.size = 0;
    stk.data = (Stack_t *) calloc ((size_t)stk.capacity, sizeof(Stack_t));
    if (!stk.data) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(fin);
        fclose(fout);
        return 1;
    }

    Calculate(fin, fout, &stk);
}