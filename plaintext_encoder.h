#ifndef PLAINTEXT_ENCODER_H
#define PLAINTEXT_ENCODER_H

#include <stdio.h>
#include <stdint.h>

#include "data_structures/search_window.h"

void plaintext_encode_block (FILE* f, match_info* m, uint8_t* dptr);
int plaintext_decode_block (match_info* result, FILE* f);

#endif