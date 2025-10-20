#ifndef ASSEMBLER_LISTING_H_
#define ASSEMBLER_LISTING_H_

#include "AssemblerStructs.h"

void PrintAssemblyListing(FILE *file, const AssemblerInfo *assembler);
void PrintCompactListing(FILE *file, const AssemblerInfo *assembler);
void PrintHexListing(FILE *file, const AssemblerInfo *assembler);
void PrintLabelsInfo(FILE *file, const Labels *labels);
void GenerateFullListing(FILE *listing_file, const AssemblerInfo *assembler);

AsmError PrintAllAssemblyListing(const char *filename, const AssemblerInfo *assembler);

#endif //ASSEMBLER_LISTING_H_