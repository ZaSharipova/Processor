#ifndef CANARY_H_
#define CANARY_H_

#include <time.h>

#include "StackFunctions.h"
#include "StructsEnums.h"

StackErr_t MakeCanary(Stack_Info *stk);
void InitCanaries(void);

extern uint32_t canary_left;
extern uint32_t canary_right;

#endif //CANARY_H_