#ifndef DEFAULT_BINARY_ENCODER_H
#define DEFAULT_BINARY_ENCODER_H

#include <stdio.h>
#include <stdint.h>

#include "data_structures/search_window.h"
#include "ZEncode_header.h"

#define DEFAULT_BINARY_FORMAT_VERSION 1

ZEncode_header* init_default_binary_header(void* ptr, char* filename, uint64_t filesize, int max_match_length, int window_size, void* options);
void default_binary_read_header(ZEncode_header* header, FILE* f);
void default_binary_write_header(FILE* f, ZEncode_header* header_info);
void default_binary_encode_block (FILE* f, match_info* m, uint8_t* dptr);
int default_binary_decode_block (match_info* result, FILE* f);

#endif