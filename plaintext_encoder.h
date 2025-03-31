#ifndef PLAINTEXT_ENCODER_H
#define PLAINTEXT_ENCODER_H

#include <stdio.h>
#include <stdint.h>

#include "data_structures/search_window.h"
#include "ZEncode_header.h"

ZEncode_header* init_plaintext_header(void* ptr, char* filename, int max_match_length, int window_size, void* options);
void plaintext_read_header(ZEncode_header* header, FILE* f);
void plaintext_write_header(FILE* f, ZEncode_header* header_info);
void plaintext_encode_block (FILE* f, match_info* m, uint8_t* dptr);
int plaintext_decode_block (match_info* result, FILE* f);

#endif