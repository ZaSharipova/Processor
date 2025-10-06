#ifndef ASSEMBLER_ENUMS_H_
#define ASSEMBLER_ENUMS_H_

enum AsmError {
    kNoAsmError            = 0,
    kErrorOpeningFile   = -1,
    kErrorClosingFile   = -2,
    kNoMemoryCalloc     = -3,
    kNoAvailableValue   = -4,
    kNoAvailableCommand = -5,
    kErrorZeroArgs      = -6,
    kWhitespace         = -7,
};
#endif //ASSEMBLER_ENUMS_H_