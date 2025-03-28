#include "plaintext_encoder.h"
#include "ZEncode_header.h"

char* dbug_serialize_char(char c) {
    char* str = malloc(32 * sizeof(char));
    if (c < 0x20 || c >= 0x7F) {
        sprintf(str, "[0x%02x]", (uint8_t)c);
    } else {
        sprintf(str, "%c", c);
    }
    return str;
}

ZEncode_header* init_plaintext_header(void* ptr, char* filename, int max_match_length, int window_size, void* options) {
    ZEncode_header* header = default_header_init(ptr, fliename, max_match_length, window_size, options);
    memcpy(&(header->subtype), "LZ77", 4);
}

void plaintext_read_header(FILE* f, ZEncode_header* header) {
    char buf[32];
    fscanf(f, "%s", buf);
    fscanf(f, "%s", buf);

}

void plaintext_write_header(FILE* f, ZEnocde_header* header_info) {
    char strbuf[16];
    memcpy(strbuf, header_info->ZEncode_marker, 8);
    memcpy(&(strbuf[8]), header_info->subtype, 4);
    strbuf[12] = '\0';
    fprintf(f, "s\n", strbuf);
    fprintf(f, "Encodes %s\n", header_info->original_filename);
    fprintf(f, "Window Size %d\n", header_info->window_size);
    fprintf(f, "Max match length %d\n", header_info->max_match_length);
}

void plaintext_encode_block (FILE* f, match_info* m, uint8_t* dptr) {

    int ADD_COMPRESSOR_DEBUG = 0;
    int PRINT_COMPRESSOR_OUTPUT = 0;
    char sbuf[32];

    // Output tuple offset, length
    sprintf(sbuf, "%d %d", m->match_idx, m->src_len);
    if(PRINT_COMPRESSOR_OUTPUT) { printf(sbuf, "%s"); }
    fputs(sbuf, f);

    // Case for literals
    if(m->src_len == 0) {
        sprintf(sbuf, " %d", m->nomatch_symbol);
        if(PRINT_COMPRESSOR_OUTPUT) { printf(sbuf, "%s"); }
        fputs(sbuf, f);
    }

    // Optional debug output (file will not be decodeable)
    if (ADD_COMPRESSOR_DEBUG) {
        fputc(' ', f);
        if (PRINT_COMPRESSOR_OUTPUT) { printf(" "); }
        if(m->src_len == 0) {
            sprintf(sbuf, "%s", dbug_serialize_char(m->nomatch_symbol));
            if(PRINT_COMPRESSOR_OUTPUT) { printf(sbuf, "%s"); }
            fputs(sbuf, f);
        } else {
            int i;
            for(i = 0; i < m->src_len; i++) {
                sprintf(sbuf, "%s", dbug_serialize_char(dptr[i]));
                if(PRINT_COMPRESSOR_OUTPUT) { printf(sbuf, "%s"); }
                fputs(sbuf, f);
            }
        }
    }

    if(PRINT_COMPRESSOR_OUTPUT) { printf("\n"); }
    fputc('\n', f);

}

int plaintext_decode_block (match_info* result, FILE* f) {

    // Read block from file and decode into match_info struct
    int last_matches;
    last_matches = fscanf(f, "%d", &(result->match_idx));
    last_matches = fscanf(f, "%d", &(result->src_len));
    int symbol;
    if (result->src_len == 0) {
        last_matches = fscanf(f, "%d", &symbol);
        result->nomatch_symbol = (char)symbol;
    }
    if (last_matches == EOF) { return EOF; }  // Early return for EOF
    return 0;

}