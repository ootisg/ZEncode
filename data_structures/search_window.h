#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include "cbuf.h"

// Search window struct implemented as typedef over cbuf
typedef struct cbuf search_window;

// Defines as wrappers for cbuf functions
#define search_window_init cbuf_init
#define search_window_destroy cbuf_destroy
#define search_window_append_char cbuf_append_char
#define search_window_append_chars cbuf_append_chars
#define search_window_get cbuf_get
#define search_window_print cbuf_print

struct match_info {
    uint32_t match_idx;
    uint32_t src_len;
    uint32_t dest_len;
    uint8_t nomatch_symbol;
};

typedef struct match_info match_info;

match_info search_window_match(search_window* buf, uint8_t* match_data, int max_len);

#endif