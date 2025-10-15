#ifndef SUBSIDIARY_FUNCTIONS_ASSEMBLER_H_
#define SUBSIDIARY_FUNCTIONS_ASSEMBLER_H_

#include <stdio.h>

int CommandToEnum(const char *command);
const char *SkipWhitespace(const char *str);
int StringToInt(char *arg_str);

#endif //SUBSIDIARY_FUNCTIONS_ASSEMBLER_H_