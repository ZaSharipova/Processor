#include "ParseCommandLine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "StructsEnums.h"
#include "/Users/zarinasharipova/Assembler/FileOperations.h"
#include "HandleLogFile.h"

static ParseErr_t OpenFileOrDefault(const char *filename, const char *mode, FILE **file_ptr, FILE *default_file);

ParseErr_t ParseCommandLine(const char **argv, int argc, Files *in_out_files) {
    assert(argv);
    assert(in_out_files);

    int pos = 1;

    if (argc == 2) {
        fprintf(stderr, "Please, enter normally next time.\n");
        return kErrorParsing;
    }

    if (argc > 2) {
        while (pos + 1 < argc) {
            if (strcmp(argv[pos], LOG_MODE) == 0) {
                in_out_files->log_file = argv[pos + 1];
                pos += 2;

            } else if (strcmp(argv[pos], IN_MODE) == 0) {
                in_out_files->in_file = argv[pos + 1];
                pos += 2;

            } else if (strcmp(argv[pos], OUT_MODE) == 0) {
                in_out_files->out_file = argv[pos + 1];
                pos += 2;

            } else {
                fprintf(stderr, "Wrong mode %s.\n", argv[pos]);
                return kWrongMode;
            }
        }
    } 

    return kNoError;
}

ParseErr_t HandleOpenFile(Files *in_out_files) {
    assert(in_out_files);

    ParseErr_t read_write_error = kNoError; 
    // read_write_error = (OpenFileOrDefault(in_out_files->log_file, WRITE_MODE, stdout));
    // if (read_write_error != kNoError) {
    //     return read_write_error;
    // }

    read_write_error = (OpenFileOrDefault(in_out_files->in_file, READ_MODE, &in_out_files->open_in_file, stdin));
    if (read_write_error != kNoError) {
        //read_write_error = CloseFile(in_out_files->open_log_file);
        return read_write_error;
    }

    read_write_error = OpenFileOrDefault(in_out_files->out_file, WRITE_MODE, &in_out_files->open_out_file, stdout);
    if (read_write_error != kNoError) {
        CALL_CHECK_IN_OUT_RETURN(CloseFile(in_out_files->open_in_file));
        CALL_CHECK_IN_OUT_RETURN(CloseFile(in_out_files->open_out_file));
        return read_write_error;
    }

    return kNoError;
}

ParseErr_t HandleCloseFile(Files in_out_files) {
    ParseErr_t read_write_error = kNoError;

    if (in_out_files.log_file != NULL) {
        CALL_CHECK_IN_OUT_RETURN(CloseFile(GetLogFile()));
    }

    if (in_out_files.open_in_file != stdin) {
        CALL_CHECK_IN_OUT_RETURN(CloseFile(in_out_files.open_in_file));
    }

    if (in_out_files.open_out_file != stdout) {
        CALL_CHECK_IN_OUT_RETURN(CloseFile(in_out_files.open_out_file));
    }

   return kNoError;
}

static ParseErr_t OpenFileOrDefault(const char *filename, const char *mode, FILE **file_ptr, FILE *default_file) {
    assert(mode);
    assert(file_ptr);
    assert(default_file);

    if (filename != NULL) {
        *file_ptr = OpenFile(filename, mode);
        if (*file_ptr == NULL) {
            return kErrorOpening;
        }
    } else {
        *file_ptr = default_file;
    }
    return kNoError;
}

FILE *OpenFile(const char *filename, const char *mode) {
    assert(filename);
    assert(mode);

    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("fopen() failed");
        return NULL;
    }

    return file;
}

ParseErr_t CloseFile(FILE *file) {
    assert(file);

    int status = fclose(file);
    if (status != 0) {
        perror("fclose() failed");
        return kErrorClosing;
    }

    return kNoError;
}
