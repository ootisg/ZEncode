#ifndef CBUF_H
#define CBUF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct cbuf {
    uint8_t* sptr;
    uint32_t size;
    uint32_t len;
    uint32_t idx;
};

typedef struct cbuf cbuf;

cbuf* cbuf_init(void* ptr, int size);
void cbuf_destroy(cbuf* buf);
void cbuf_append_byte(cbuf* buf, uint8_t data);
void cbuf_append_bytes(cbuf* buf, uint8_t* data, int data_len);
uint8_t cbuf_get(cbuf* buf, int idx);
void cbuf_print(cbuf* buf);

#endif