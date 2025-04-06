#include "ZEncode_header.h"

ZEncode_header* default_header_init(void* ptr, uint16_t header_length, uint16_t format_version, char* filename, uint64_t filesize, int max_match_length, int window_size, void* options) {
    ZEncode_header* header = (ZEncode_header*)ptr;
    memcpy(&(header->ZEncode_marker), ZENCODE_FILE_MARKER, 8);
    memcpy(&(header->subtype), "NULL", 4);
    header->format_version = format_version;
    header->max_match_length = max_match_length;
    header->original_filesize = filesize;
    header->original_filename = filename;
    header->window_size = window_size;
    header->header_length = 14 + strlen(filename) + 1;
    return header;
}

int ZEncode_check_is_encoded(char* filepath) {
    FILE* f = fopen(filepath, "rb");
    if (!f) {
        return 0;
    }
    char marker_bytes[strlen(ZENCODE_FILE_MARKER)];
    int bytes_read = fread(&marker_bytes, 1, strlen(ZENCODE_FILE_MARKER), f);
    int result = 0;
    if (bytes_read != strlen(ZENCODE_FILE_MARKER)) {
        result = 0;
    } else if (memcmp(&marker_bytes, ZENCODE_FILE_MARKER, strlen(ZENCODE_FILE_MARKER))) {
        result = 0;
    } else {
        result = 1;
    }
    fclose(f);
    return result;
}

int ZEncode_read_common_header_fields(ZEncode_header* result, FILE* f) {
    int bytes_read = 0;
    bytes_read += fread(&(result->ZEncode_marker), 1, 8, f);
    bytes_read += fread(&(result->subtype), 1, 4, f);
    if (bytes_read != 12) {
        return 0;
    } else {
        return 1;
    }
}