#include "HandleCalc.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "CalculatorCommands.h"

void Calculate(FILE *fin, FILE *fout, Stack_Info *stk) {
    assert(fin);
    assert(fout);
    assert(stk);

    int cmd = -1;
    Stack_t arg = 0;

    while (fscanf(fin, "%d", &cmd) == 1) {
        switch (cmd) {
            case (Push):
                if (fscanf(fin, "" MY_SPEC "", &arg) != 1) {
                    fprintf(stderr, "Expected argument for PUSH.\n");
                    goto cleanup;
                }
                if (Push_C(stk, arg, stdout) != kSuccess) {
                    fprintf(stderr, "Error: PUSH failed.\n");
                    goto cleanup;
                }
                break;

            case (Pop):
                if (Pop_C(stk, stdout) != kSuccess) {
                    fprintf(stderr, "Error: POP failed.\n");
                    goto cleanup;
                }
                break;

            case (Add):
                if (StackOperation(stk, Add_C, stdout) != kSuccess) {
                    fprintf(stderr, "Error: ADD failed.\n");
                    goto cleanup;
                }
                break;

            case (Sub):
                if (StackOperation(stk, Sub_C, stdout) != kSuccess) {
                    fprintf(stderr, "Error: SUB failed.\n");
                    goto cleanup;
                }
                break;

            case (Mul):
                if (StackOperation(stk, Mul_C, stdout) != kSuccess) {
                    fprintf(stderr, "Error: MUL failed.\n");
                    goto cleanup;
                }
                break;

            case (Div):
                if (Div_C(stk, stdout) != kSuccess) {
                    fprintf(stderr, "Error: DIV failed.\n");
                    goto cleanup;
                }
                break;

            case (Sqrt):
                if (Sqrt_C(stk, stdout) != kSuccess) {
                    fprintf(stderr, "Error: SQRT failed.\n");
                    goto cleanup;
                }
                break;

            case (Out):
                if (Out_C(stk, stdout, fout) != kSuccess) {
                    fprintf(stderr, "Error: OUT failed.\n");
                    goto cleanup;
                }
                break;

            case (Hlt):
                goto cleanup;

            default:
                fprintf(stderr, "Unknown command: %d.\n", cmd);
                goto cleanup;
        }
    }

    cleanup:
        free(stk->data);
        fclose(fin); //
        fclose(fout);
}

FILE *OpenFile(const char *filename, const char *mode) {
    assert(filename);
    assert(mode);

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("fopen() failed");
        return NULL;
    }

    return file;
}

StackErr_t CloseFile(FILE *file) {
    assert(file);

    int status = fclose(file);
    if (status != 0) {
        perror("fclose() failed");
        return kErrorClosing;
    }

    return kSuccess;
}
