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

cbuf* cbuf_init(void* ptr, int size) {
    cbuf* buf = (cbuf*)ptr;
    buf->sptr = malloc(size * sizeof(char));
    buf->size = size;
    buf->len = 0;
    buf->idx = 0;
    return buf;
}

void cbuf_destroy(cbuf* buf) {
    free(buf->sptr);
}

void cbuf_append_char(cbuf* buf, char data) {
    buf->idx = (buf->idx - 1) % buf->size;
    buf->sptr[buf->idx] = data;
    buf->len = buf->len + 1 > buf->size ? buf->size : buf->len + 1;
}

void cbuf_append_chars(cbuf* buf, char* data, int data_len) {
    int i;
    for (i = 0; i < data_len; i++) {
        cbuf_append_char(buf, data[i]);
    }
}

char cbuf_get(cbuf* buf, int idx) {
    return buf->sptr[(buf->idx + idx) % buf->size];
}

void cbuf_print(cbuf* buf) {
    int i;
    for (i = 0; i < buf->len; i++) {
        printf("%c", cbuf_get(buf, i));
    }
    printf("\n");
}

#endif