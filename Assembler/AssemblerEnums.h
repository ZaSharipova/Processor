#ifndef ASSEMBLER_ENUMS_H_
#define ASSEMBLER_ENUMS_H_

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
};

enum PossibleErrorsAsm {
    kNoErrorAsm,
    kErrorOpeningAsm,
    kErrorClosingAsm,
    kErrorParsingAsm,  
};

#define PUSH "PUSH"
#define POP "POP"
#define ADD "ADD"
#define SUB "SUB"
#define MUL "MUL"
#define DIV "DIV"
#define SQRT "SQRT"
#define OUT "OUT"
#define PUSHR "PUSHR"
#define POPR "POPR"
#define IN "IN"
#define HLT "HLT"
#define JMP "JMP"
#define JB "JB"
#define JBE "JBE"
#define JA "JA"
#define JAE "JAE"
#define JE "JE"
#define JNE "JNE"

enum Convert {
    kPush  = 1,
    kPop   = 2,
    kAdd   = 3,
    kSub   = 4,
    kMul   = 5,
    kDiv   = 6,
    kSqrt  = 7,
    kOut   = 8,
    kPushR = 9,
    kPopR  = 10,
    kIn    = 11,
    kJmp   = 12,
    kJB    = 13,
    kJBE   = 14,
    kJA    = 15,
    kJAE   = 16,
    kJE    = 17,
    kJNE   = 18,
    kHlt   = 0,

};

#endif //ASSEMBLER_ENUMS_H_