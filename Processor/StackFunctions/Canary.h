#ifndef CANARY_H_
#define CANARY_H_

#include <time.h>

#include "StackFunctions.h"
#include "StructsEnums.h"

ProcessorErr_t MakeCanary(Stack_Info *stk, FILE *open_log_file);

extern uint32_t canary_left;
extern uint32_t canary_right;

#endif //CANARY_H_