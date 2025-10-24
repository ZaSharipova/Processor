#ifndef SUBSIDIARY_FUNCTIONS_ASSEMBLER_H_
#define SUBSIDIARY_FUNCTIONS_ASSEMBLER_H_

#include <stdio.h>

#define CALL_CHECK_ASM_RETURN(call) \
    err = (call); \
    if (err != kNoAsmError) { \
        DoCloseFile(&in_out_files); \
        CloseLogFile(); \
        return err; \
    }

int CompareCommandHash(const void *key, const void *element);
int CommandToEnumPos(const char *command);
const char *EnumToCommand(int command_num);
const char *SkipWhitespace(const char *str);
int StringToInt(char *arg_str);

void InitLabels(int *labels);

#endif //SUBSIDIARY_FUNCTIONS_ASSEMBLER_H_