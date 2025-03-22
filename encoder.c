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

    int ADD_COMPRESSOR_DEBUG = 0;
    int PRINT_COMPRESSOR_OUTPUT = 0;

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

    FILE* f2 = fopen("encoded.bin", "w");

    int buf_len = idx;
    int buf_idx = 0;
    int max_len = 32;
    while(buf_idx < buf_len) {

        int bytes_left = buf_len - buf_idx;
        match_info m = search_window_match(&buf, &(fbuf[buf_idx]), bytes_left < max_len ? bytes_left : max_len);
        if(m.match_len > 0) {
            search_window_append_chars(&buf, &(fbuf[buf_idx]), m.match_len);
        } else {
            search_window_append_char(&buf, m.nomatch_symbol);
        }

        char sbuf[1024];
        sprintf(sbuf, "%d %d", m.match_idx, m.match_len);
        if(PRINT_COMPRESSOR_OUTPUT) { printf(sbuf, "%s"); }
        fputs(sbuf, f2);
        if(m.match_len == 0) {
            sprintf(sbuf, " %d", m.nomatch_symbol);
            if(PRINT_COMPRESSOR_OUTPUT) { printf(sbuf, "%s"); }
            fputs(sbuf, f2);
        }
        if (ADD_COMPRESSOR_DEBUG) {
            printf(" ");
            if(m.match_len == 0) {
                sprintf(sbuf, "%s", dbug_serialize_char(m.nomatch_symbol));
                if(PRINT_COMPRESSOR_OUTPUT) { printf(sbuf, "%s"); }
                fputs(sbuf, f2);
            } else {
                int i;
                for(i = 0; i < m.match_len; i++) {
                    sprintf(sbuf, "%s", dbug_serialize_char(fbuf[buf_idx + i]));
                    if(PRINT_COMPRESSOR_OUTPUT) { printf(sbuf, "%s"); }
                    fputs(sbuf, f2);
                }
            }
        }
        if(PRINT_COMPRESSOR_OUTPUT) { printf("\n"); }
        fputc('\n', f2);

        buf_idx += m.match_len > 0 ? m.match_len : 1;

    }

    fclose(f2);

}

void decode_file(char* filepath) {

    int PRINT_DECOMPRESSOR_OUTPUT = 1;

    cbuf out;
    cbuf_init(&out, 1024);

    FILE* f = fopen(filepath, "r");
    FILE* f2 = fopen("decoded.bin", "wb");
    char sbuf[1024];
    int last_macthes = 0;
    int offs, len, symbol;
    while(last_macthes != EOF) {
        last_macthes = fscanf(f, "%d", &offs);
        last_macthes = fscanf(f, "%d", &len);
        if (last_macthes == EOF) { return; }  // Early return for EOF
        if (len == 0) { last_macthes = fscanf(f, "%d", &symbol); }
        if (len == 0) {
            cbuf_append_char(&out, (char)symbol);
            if(PRINT_DECOMPRESSOR_OUTPUT) { printf("%c", (char)symbol); }
            fputc((char)symbol, f2);
        } else {
            int idx = offs;
            int i;
            char buf[1024];
            for (i = 0; i < len; i++) {
                buf[i] = cbuf_get(&out, idx - i);
            }
            for(i = 0; i < len; i++) {
                cbuf_append_char(&out, buf[i]);
                if(PRINT_DECOMPRESSOR_OUTPUT) { printf("%c", buf[i]); }
                fputc(buf[i], f2);
            }
        }
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