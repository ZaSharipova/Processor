#include "SubsidiaryFunctionsAssembler.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>

#include "AssemblerStructs.h"
#include "Hash.h"

int CommandToEnumPos(const char *command) {
    assert(command);

    uint32_t command_hash = HashData(command, (ssize_t)strlen(command));

    CommandsInfo *result = (CommandsInfo *)bsearch(&command_hash, commands, 30, sizeof(CommandsInfo), CompareCommandHash);
    
    if (result != NULL) {
        return (int)(result - commands);
    }

    return -1;
}

int CompareCommandHash(const void *key, const void *element) {
    uint32_t target_hash = *(const uint32_t *)key;
    const CommandsInfo *cmd = (const CommandsInfo *)element;

    if (target_hash < cmd->hash) {
        return -1;

    } else if (target_hash > cmd->hash) {
        return 1;

    } else {
        return 0;
    }
}

const char *EnumToCommand(int command_num) {
    return commands[command_num].command_name;
}

const char *SkipWhitespace(const char *str) {
    assert(str);

    while (isspace((unsigned char) *str)) {
        str++;
    }

    return str;
}

int StringToInt(char *arg_str) {
    assert(arg_str);

    int return_number = (int)(arg_str[1] - 'A');
    if (return_number >= 0 && return_number < REGS_SIZE) {
        return return_number;
    }

    return -1;
}

void InitLabels(int *labels) {
    assert(labels);

    for (int i = 0; i < 12; i++) {
        labels[i] = -1;
    }
}