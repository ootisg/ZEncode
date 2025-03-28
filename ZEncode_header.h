#ifndef ZENCODE_HEADER_H
#define ZENCODE_HEADER_H

#define ZENCODE_FILE_MARKER "ZENCODE "

struct ZEnocde_header {
    char ZEncode_marker[8];
    char subtype[4];
    uint16_t header_length;
    uint16_t max_match_length;
    uint32_t window_size;
    char* original_filename;
    void* format_data;
};

typedef struct ZEncode_header ZEncode_header;

ZEncode_header* default_header_init(void* ptr, char* filename, int max_match_length, int window_size, void* options) {
    ZEncode_header* header = (ZEncode_header*)ptr;
    memcpy(&(header->ZEncode_marker), ZENCODE_FILE_MARKER, 8);
    memcpy(&(header->subtype), "NULL", 4);
    header->max_match_length = max_match_length;
    header->original_filename = filename;
    header->window_size = window_size;
    header->header_length = 14 + strlen(filename) + 1;
    return header;
}

#endif