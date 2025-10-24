#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "StructsEnums.h"
#include "DoFileWork.h"
#include "FileOperations.h"
#include "ParseCommandLine.h"
#include "DoLogFile.h"
#include "FileStructs.h"
#include "AssemblerStructs.h"
#include "SubsidiaryFunctionsAssembler.h"
#include "AssemblerListing.h"
#include "Hash.h"

int main(int argc, const char *argv[]) {

    Files in_out_files = {NULL, NULL, NULL, NULL};

    ParseErr_t read_write_error = kNoError;

    CALL_CHECK_IN_OUT_RETURN(ParseCommandLine(argv, argc, &in_out_files));
    SetLogFile(in_out_files.log_file);

    CALL_CHECK_IN_OUT_RETURN((ParseErr_t)DoHash());

    CALL_CHECK_IN_OUT_RETURN(DoOpenFile(&in_out_files));

    AssemblerInfo Assembler = {}; //
    FileInfo file_info = {};

    AssemblerCtor(&Assembler);
    AsmError err = kNoAsmError;
    
    CALL_CHECK_ASM_RETURN(PrepareToAssemble(&in_out_files, &file_info, &Assembler));

    CALL_CHECK_ASM_RETURN(DoAsm(&file_info, &in_out_files, &Assembler));

    read_write_error = CloseLogFile();
    if (read_write_error != kNoError) {
        CALL_CHECK_IN_OUT_RETURN(DoCloseFile(&in_out_files));
        return read_write_error;
    }


    //CALL_CHECK_IN_OUT_RETURN((ParseErr_t)PrintAllAssemblyListing(ASSEMBLY_LISTING_FILENAME, &Assembler));
    StackDtor(&Assembler.data);

    CALL_CHECK_IN_OUT_RETURN(DoCloseFile(&in_out_files));
    return kNoError;
}