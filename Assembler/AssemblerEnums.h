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
    kAdd    = 6,
    kSub    = 7,
    kMul    = 8,
    kDiv    = 9,
    kSqrt   = 10,
    kOutC   = 11,
    kOut    = 12,
    kIn     = 13,
    kJmp    = 14,
    kJBE    = 15,
    kJB     = 16,
    kJAE    = 17,
    kJA     = 18,
    kJNE    = 19,
    kJE     = 20,
    kCall   = 21,
    kRet    = 22,
    kDraw   = 23,
    kSquare = 24,
    kPause  = 25
};

enum TypeOfArg {
    kIntArg = 0,
    kRamArg = 1,
    kRaxArg = 2,
    kNoArg  = 3,
    kLabelArg = 4,
};

#endif //ASSEMBLER_ENUMS_H_