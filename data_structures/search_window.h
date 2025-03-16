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
    uint32_t match_len;
    uint8_t nomatch_symbol;
};

typedef struct match_info match_info;

match_info search_window_match(search_window* buf, uint8_t* match_data, int max_len) {

    match_info curr;
    curr.match_idx = 0;
    curr.match_len = 0;
    curr.nomatch_symbol = 0;

    int m_idx, m_len;
    for(m_idx = 0; m_idx < buf->len; m_idx++) {

        for(m_len = 0; m_len < max_len && m_idx - m_len >= 0; m_len++) {
            if(match_data[m_len] != search_window_get(buf, m_idx - m_len)) { break; }
        }

        if (m_len > curr.match_len) {
            curr.match_len = m_len;
            curr.match_idx = m_idx;
        }

    }

    if(curr.match_len == 0) {
        curr.nomatch_symbol = match_data[0];
    }

    return curr;

}

#endif