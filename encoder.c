#include "data_structures/search_window.h"

char* dbug_serialize_char(char c) {
    char* str = malloc(32 * sizeof(char));
    if (c < 0x20 || c >= 0x7F) {
        sprintf(str, "[0x%02x]", (uint8_t)c);
    } else {
        sprintf(str, "%c", c);
    }
    return str;
}

void encode_file(char* filepath) {

    search_window buf;
    search_window_init(&buf, 1024);

    uint8_t fbuf[4096];
    FILE* f = fopen(filepath, "rb");
    int c, idx;
    idx = 0;
    while ((c = fgetc(f)) != EOF) {
        fbuf[idx++] = c;
    }
    fclose(f);

    int buf_len = idx;
    int buf_idx = 0;
    int max_len = 32;
    while(buf_idx < buf_len) {

        int bytes_left = buf_len - buf_idx - 1;
        match_info m = search_window_match(&buf, &(fbuf[buf_idx]), bytes_left < max_len ? bytes_left : max_len);
        if(m.match_len > 0) {
            search_window_append_chars(&buf, &(fbuf[buf_idx]), m.match_len);
        } else {
            search_window_append_char(&buf, m.nomatch_symbol);
        }

        int add_dbug = 1;
        printf("%d %d", m.match_idx, m.match_len);
        if(m.match_len == 0) {
            printf(" %d", m.nomatch_symbol);
        }
        if (add_dbug) {
            printf(" ");
            if(m.match_len == 0) {
                printf("%s", dbug_serialize_char(m.nomatch_symbol));
            } else {
                int i;
                for(i = 0; i < m.match_len; i++) {
                    printf("%s", dbug_serialize_char(fbuf[buf_idx + i]));
                }
            }
        }
        printf("\n");

        buf_idx += m.match_len > 0 ? m.match_len : 1;

    }

}

void decode_file(char* filepath) {

}

int main(int argc, char** argv) {
    encode_file("encoder.c");
}