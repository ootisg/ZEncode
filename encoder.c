#include "data_structures/search_window.h"
#include "plaintext_encoder.h"

#define MAX_FILE_SIZE 1048576

#define SLIDING_WINDOW_SIZE 32768
#define MAX_MATCH_LENGTH 1024

void encode_file(char* filepath) {

    int ADD_COMPRESSOR_DEBUG = 0;
    int PRINT_COMPRESSOR_OUTPUT = 0;

    search_window buf;
    search_window_init(&buf, SLIDING_WINDOW_SIZE);

    uint8_t fbuf[MAX_FILE_SIZE];
    FILE* f = fopen(filepath, "rb");
    int c, idx;
    idx = 0;
    while ((c = fgetc(f)) != EOF) {
        fbuf[idx++] = c;
    }
    fclose(f);

    FILE* f2 = fopen("encoded.bin", "w");

    int buf_len = idx;
    int buf_idx = 0;
    int max_len = MAX_MATCH_LENGTH;
    while(buf_idx < buf_len) {

        int bytes_left = buf_len - buf_idx;
        match_info m = search_window_match(&buf, &(fbuf[buf_idx]), bytes_left < max_len ? bytes_left : max_len);
        if(m.src_len > 0) {
            search_window_append_chars(&buf, &(fbuf[buf_idx]), m.src_len);
        } else {
            search_window_append_char(&buf, m.nomatch_symbol);
        }

        plaintext_encode_block(f2, &m, &(fbuf[buf_idx]));

        buf_idx += m.src_len > 0 ? m.src_len : 1;

    }

    fclose(f2);

}

void decode_file(char* filepath) {

    int PRINT_DECOMPRESSOR_OUTPUT = 0;

    cbuf out;
    cbuf_init(&out, SLIDING_WINDOW_SIZE);

    FILE* f = fopen(filepath, "r");
    FILE* f2 = fopen("decoded.bin", "wb");
    char sbuf[SLIDING_WINDOW_SIZE];
    match_info m;
    int last_block_result = plaintext_decode_block(&m, f);
    while(last_block_result != EOF) {
        if (m.src_len == 0) {
            cbuf_append_char(&out, (char)m.nomatch_symbol);
            if(PRINT_DECOMPRESSOR_OUTPUT) { printf("%c", (char)m.nomatch_symbol); }
            fputc((char)m.nomatch_symbol, f2);
        } else {
            int idx = m.match_idx;
            int i;
            char buf[SLIDING_WINDOW_SIZE];
            for (i = 0; i < m.src_len; i++) {
                buf[i] = cbuf_get(&out, idx - i);
            }
            for(i = 0; i < m.src_len; i++) {
                cbuf_append_char(&out, buf[i]);
                if(PRINT_DECOMPRESSOR_OUTPUT) { printf("%c", buf[i]); }
                fputc(buf[i], f2);
            }
        }
        last_block_result = plaintext_decode_block(&m, f);
    }
}

int main(int argc, char** argv) {
    int compressMode = 1;
    char* filepath = NULL;
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                compressMode = 1;
            } else if (argv[i][1] == 'd') {
                compressMode = 0;
            }
        } else {
            filepath = argv[i];
        }
    }
    if (filepath) {
        if (compressMode) {
            encode_file(filepath);
        } else {
            decode_file(filepath);
        }
    }
}