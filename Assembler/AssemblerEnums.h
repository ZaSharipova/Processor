#ifndef ASSEMBLER_ENUMS_H_
#define ASSEMBLER_ENUMS_H_

#include <stdio.h>

#include "StructsEnums.h"
#include "SubsidiaryFunctionsAssembler.h"

#define ASSEMBLY_LISTING_FILENAME "assembly.lst"
#define ERROR_CHECK_RETURN(call) \
    err = (call); \
    if (err != kNoErrorAsm) { \
        return err; \
    }

enum AsmError {
    kNoAsmError         = 0,
    kErrorOpeningFile   = -1,
    kErrorClosingFile   = -2,
    kNoMemoryCalloc     = -3,
    kNoAvailableValue   = -4,
    kNoAvailableCommand = -5,
    kErrorZeroArgs      = -6,
    kWhitespace         = -7,
    kErrorParsingAsm    = -8,
};

enum NumOfArgs {
    kNo = -1,
    kZero = 0,
    kOne = 1,
};

enum Convert {
    kCommandNotFound = -1,
    kHlt    = 0,
    kPushR  = 1,
    kPushM  = 2,
    kPush   = 3,
    kPopR   = 4,
    kPopM   = 5,
    kPop    = 6,
    kAdd    = 7,
    kSub    = 8,
    kMul    = 9,
    kDiv    = 10,
    kSqrt   = 11,
    kOutC   = 12,
    kOut    = 13,
    kIn     = 14,
    kJmp    = 15,
    kJBE    = 16,
    kJB     = 17,
    kJAE    = 18,
    kJA     = 19,
    kJNE    = 20,
    kJE     = 21,
    kCall   = 22,
    kRet    = 23,
    kDraw   = 24,
    kSquare = 25
};

#endif //ASSEMBLER_ENUMS_H_