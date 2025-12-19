#ifndef HASH_H_
#define HASH_H_

#include <stdio.h>

#include "StructsEnums.h"
#include "SubsidiaryFunctionsAssembler.h"
#include "AssemblerEnums.h"
#include "DoCalc.h"

AsmError DoHash(void);
uint32_t HashData(const char *data, ssize_t size);
AsmError FillCommandsWithHash(void);
int CompareHash(const void *command_info1, const void *command_info2);
int SearchHash(uint32_t pivot);

#endif //HASH_H_