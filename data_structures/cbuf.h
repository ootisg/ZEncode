#ifndef CBUF_H
#define CBUF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct cbuf {
    char* sptr;
    uint32_t size;
    uint32_t len;
    uint32_t idx;
};

typedef struct cbuf cbuf;

cbuf* cbuf_init(void* ptr, int size);
void cbuf_destroy(cbuf* buf);
void cbuf_append_char(cbuf* buf, char data);
void cbuf_append_chars(cbuf* buf, char* data, int data_len);
char cbuf_get(cbuf* buf, int idx);
void cbuf_print(cbuf* buf);

#endif