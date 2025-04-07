#include "cbuf.h"

cbuf* cbuf_init(void* ptr, int size) {
    cbuf* buf = (cbuf*)ptr;
    buf->sptr = malloc(size * sizeof(uint8_t));
    buf->size = size;
    buf->len = 0;
    buf->idx = 0;
    return buf;
}

void cbuf_destroy(cbuf* buf) {
    free(buf->sptr);
}

void cbuf_append_byte(cbuf* buf, uint8_t data) {
    buf->idx = (buf->idx - 1) % buf->size;
    buf->sptr[buf->idx] = data;
    buf->len = buf->len + 1 > buf->size ? buf->size : buf->len + 1;
}

void cbuf_append_bytes(cbuf* buf, uint8_t* data, int data_len) {
    int i;
    for (i = 0; i < data_len; i++) {
        cbuf_append_byte(buf, data[i]);
    }
}

uint8_t cbuf_get(cbuf* buf, int idx) {
    return buf->sptr[(buf->idx + idx) % buf->size];
}

void cbuf_print(cbuf* buf) {
    int i;
    for (i = 0; i < buf->len; i++) {
        printf("%c", cbuf_get(buf, i));
    }
    printf("\n");
}