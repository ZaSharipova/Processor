#include "AssemblerListing.h"

#include <stdio.h>
#include <assert.h>

#include "AssemblerStructs.h"
#include "FileOperations.h"

void PrintAssemblyListing(FILE *file, const AssemblerInfo *assembler) {
    assert(file);
    assert(assembler);

    fprintf(file, "=== ASSEMBLER LISTING ===\n");
    fprintf(file, "Address  Command  Argument\n");
    fprintf(file, "--------------------------\n");
    
    int ip = 0;
    while (ip < assembler->data.size) {
        for (size_t i = 0; i < assembler->labels.count; i++) {
            if (assembler->labels.data[i].address == ip) {
                fprintf(file, "\n%s:\n", assembler->labels.data[i].name);
                break;
            }
        }
        
        int cmd = assembler->data.data[ip];
        const CommandsInfo *cmd_info = &commands[cmd];
        
        fprintf(file, "%06d  %-8s", ip, cmd_info->command_name);
        
        if (cmd_info->num_args == 1 && ip + 1 < assembler->data.size) {
            int arg = assembler->data.data[ip + 1];
            fprintf(file, " %d", arg);
            ip += 2;
        } else {
            ip++;
        }
        
        fprintf(file, "\n");
    }
    fprintf(file, "--------------------------\n");
    fprintf(file, "Total instructions: %d\n", ip);
}

void PrintCompactListing(FILE *file, const AssemblerInfo *assembler) {
    assert(file);
    assert(assembler);
    
    fprintf(file, "Addr Cmd Arg\n");
    fprintf(file, "---- --- ---\n");
    
    int ip = 0;
    while (ip < assembler->data.size) {
        int cmd = assembler->data.data[ip];
        const CommandsInfo *cmd_info = &commands[cmd];
        
        fprintf(file, "%04d %3d", ip, cmd);
        
        if (cmd_info->num_args == 1 && ip + 1 < assembler->data.size) {
            int arg = assembler->data.data[ip + 1];
            fprintf(file, " %3d", arg);
            ip += 2;
        } else {
            fprintf(file, "  - ");
            ip++;
        }
        
        fprintf(file, " ; %s\n", cmd_info->command_name);
    }
}

void PrintHexListing(FILE *file, const AssemblerInfo *assembler) {
    assert(file);
    assert(assembler);
    
    fprintf(file, "Address  Hex     Command  Argument\n");
    fprintf(file, "----------------------------------\n");
    
    int ip = 0;
    while (ip < assembler->data.size) {
        int cmd = assembler->data.data[ip];
        const CommandsInfo *cmd_info = &commands[cmd];
        
        fprintf(file, "%06d  0x%02X    %-8s", ip, cmd, cmd_info->command_name);
        
        if (cmd_info->num_args == 1 && ip + 1 < assembler->data.size) {
            int arg = assembler->data.data[ip + 1];
            fprintf(file, " 0x%04X (%d)", arg, arg);
            ip += 2;
        } else {
            ip++;
        }
        
        fprintf(file, "\n");
    }
}

void PrintLabelsInfo(FILE *file, const Labels *labels) {
    assert(file);
    assert(labels);
    
    fprintf(file, "=== LABELS ===\n");
    fprintf(file, "Name            Address\n");
    fprintf(file, "-----------------------\n");
    
    for (size_t i = 0; i < labels->count; i++) {
        fprintf(file, "%-15s %06d\n", 
               labels->data[i].name, 
               labels->data[i].address);
    }
}

void GenerateFullListing(FILE *listing_file, const AssemblerInfo *assembler) {
    assert(listing_file);
    assert(assembler);
    
    fprintf(listing_file, ";; Assembler Listing\n");
    fprintf(listing_file, ";; Generated automatically\n\n");
    
    fprintf(listing_file, ";; Labels:\n");
    for (size_t i = 0; i < assembler->labels.count; i++) {
        fprintf(listing_file, "%-15s EQU %d\n", 
                assembler->labels.data[i].name, 
                assembler->labels.data[i].address);
    }
    fprintf(listing_file, "\n");
    
    fprintf(listing_file, ";; Code:\n");
    
    int ip = 0;
    while (ip < assembler->data.size) {
        for (size_t i = 0; i < assembler->labels.count; i++) {
            if (assembler->labels.data[i].address == ip) {
                fprintf(listing_file, "\n%s:\n", assembler->labels.data[i].name);
                break;
            }
        }
        
        int cmd = assembler->data.data[ip];
        const CommandsInfo *cmd_info = &commands[cmd];
        
        fprintf(listing_file, "    %-8s", cmd_info->command_name);
        
        if (cmd_info->num_args == 1 && ip + 1 < assembler->data.size) {
            int arg = assembler->data.data[ip + 1];
            fprintf(listing_file, " %d", arg);
            ip += 2;
        } else {
            ip++;
        }
        
        fprintf(listing_file, "\n");
    }
    
    printf("Listing saved to %s\n","assemly.lst");
}

AsmError PrintAllAssemblyListing(const char *filename, const AssemblerInfo *assembler) {
    assert(filename);
    assert(assembler);

    FILE *listing_file = fopen(filename, "w");
    if (!listing_file) {
        printf("Error: Cannot create listing file %s\n", "asembly.lst");
        return kErrorOpeningFile;
    }

    PrintAssemblyListing(listing_file, assembler);
    fprintf(listing_file, "\n");
    PrintCompactListing(listing_file, assembler);
    fprintf(listing_file, "\n");
    PrintLabelsInfo(listing_file, &assembler->labels);

    GenerateFullListing(listing_file, assembler);

    return (AsmError)CloseFile(listing_file);
}