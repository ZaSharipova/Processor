#ifndef FILE_STRUCTS_H_
#define FILE_STRUCTS_H_

#include <stdio.h>

typedef struct {
    char *start_ptr;
    char *end_ptr;
    char *start_ptr_alpha;
    char *end_ptr_alpha;
    size_t size;
} LineInfo;

typedef struct {
    char *buf_ptr;      
    size_t filesize;    
    int count_lines;    
    LineInfo *text_ptr;
} FileInfo;

#endif //FILE_STRUCTS_H_