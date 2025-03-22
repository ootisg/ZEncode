#include "search_window.h"

match_info search_window_match(search_window* buf, uint8_t* match_data, int max_len) {

    match_info curr;
    curr.match_idx = 0;
    curr.src_len = 0;
    curr.nomatch_symbol = 0;

    int m_idx, m_len;
    for(m_idx = 0; m_idx < buf->len; m_idx++) {

        for(m_len = 0; m_len < max_len && m_idx - m_len >= 0; m_len++) {
            if(match_data[m_len] != search_window_get(buf, m_idx - m_len)) { break; }
        }

        if (m_len > curr.src_len) {
            curr.src_len = m_len;
            curr.match_idx = m_idx;
        }

    }

    if(curr.src_len == 0) {
        curr.nomatch_symbol = match_data[0];
    }

    return curr;

}