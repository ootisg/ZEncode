#ifndef ZENCODE_HEADER_H
#define ZENCODE_HEADER_H

#define ZENCODE_FILE_MARKER "ZENCODE "

#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct ZEncode_header {
    char ZEncode_marker[8];
    char subtype[4];
    uint16_t header_length;
    uint16_t max_match_length;
    uint32_t window_size;
    char* original_filename;
    void* format_data;
};

typedef struct ZEncode_header ZEncode_header;

int ZEncode_check_is_encoded(char* filepath);
int ZEncode_read_common_header_fields(ZEncode_header* result, FILE* f);
ZEncode_header* default_header_init(void* ptr, char* filename, int max_match_length, int window_size, void* options);

#endif