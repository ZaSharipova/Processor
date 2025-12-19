#include "Hash.h"

#include <stdio.h>
#include <assert.h>

#include "StructsEnums.h"
#include "AssemblerStructs.h"
#include "AssemblerEnums.h"
#include "Commands.h"

AsmError DoHash(void) {
    AsmError err = FillCommandsWithHash();
    if (err != kNoAsmError) {
        fprintf(stderr, "Error filling hash.");
        return err;
    }

    qsort(commands, 29, sizeof(CommandsInfo), CompareHash);
    return kNoAsmError;
}

uint32_t HashData(const char *data, ssize_t size) {
    assert(data);

    uint32_t hash = 5381;
    for (ssize_t i = 0; i < size; i++) {
        hash = hash * 33 + (uint32_t)data[i];
    }

    return hash;
}

AsmError FillCommandsWithHash(void) {
    for (int i = 0; i < NUM_COMMANDS; i++) {
        commands[i].hash = HashData(commands[i].command_name, (ssize_t)commands[i].command_len);
    }

    return kNoAsmError;
}

int CompareHash(const void *command_info1, const void *command_info2) {
    assert(command_info1);
    assert(command_info2);

    const CommandsInfo *command1 = (const CommandsInfo *)command_info1;
    const CommandsInfo *command2 = (const CommandsInfo *)command_info2;

    if (command1->hash < command2->hash) {
        return -1;
    } else if (command1->hash > command2->hash) {
        return 1;
    } else {
        return 0;
    }
}

int SearchHash(uint32_t pivot) {
    int left = 0, right = NUM_COMMANDS - 1;

    while (right - left > 0) {
        int mid = right - left;
        uint32_t compare_num = commands[mid].hash;

        if (compare_num < pivot) {
            left = mid + 1;

        } else if (compare_num == pivot) {
            right = mid - 1;

        } else {
            return mid;
        }

    }
    return -1;
}