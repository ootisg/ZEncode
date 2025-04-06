#include "data_structures/search_window.h"
#include "plaintext_encoder.h"
#include "default_binary_encoder.h"
#include "ZEncode_header.h"

#include <stdlib.h>
#include <libgen.h>

#define MAX_FILE_SIZE 1073741824

#define SLIDING_WINDOW_SIZE 32768
#define MAX_MATCH_LENGTH 1024

int encode_file(char* in_file, char* out_file) {

    int ADD_COMPRESSOR_DEBUG = 0;
    int PRINT_COMPRESSOR_OUTPUT = 0;

    FILE* f = fopen(in_file, "rb");
    fseek(f, 0, SEEK_END);
    uint64_t src_filesize = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (src_filesize > MAX_FILE_SIZE) {
        printf("Error: input file size %lu exceeds maximum (%lu)\n", src_filesize, (uint64_t)MAX_FILE_SIZE);
        return 1;
    }

    ZEncode_header header;
    char* basepath = basename(in_file);
    init_default_binary_header(&header, basepath, src_filesize, MAX_MATCH_LENGTH, SLIDING_WINDOW_SIZE, (void*)0);

    search_window buf;
    search_window_init(&buf, SLIDING_WINDOW_SIZE);

    uint8_t* fbuf = malloc(src_filesize);
    if (!f) {
        return 1;
    }
    int c, idx;
    idx = 0;
    while ((c = fgetc(f)) != EOF) {
        fbuf[idx++] = c;
    }
    fclose(f);

    FILE* f2 = fopen(out_file, "w");

    default_binary_write_header(f, &header);

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

        default_binary_encode_block(f2, &m, &(fbuf[buf_idx]));

        buf_idx += m.src_len > 0 ? m.src_len : 1;

    }

    fclose(f2);
    free(fbuf);
    return 0;

}

int decode_file(ZEncode_header* file_info, char* in_file, char* out_file) {

    int PRINT_DECOMPRESSOR_OUTPUT = 0;

    cbuf out;

    FILE* f = fopen(in_file, "r");
    if (!f) {
        return 1;
    }
    ZEncode_header header;
    ZEncode_read_common_header_fields(file_info, f);
    default_binary_read_header(file_info, f);

    cbuf_init(&out, SLIDING_WINDOW_SIZE);
    FILE* f2 = fopen(out_file ? out_file : file_info->original_filename, "wb");

    char sbuf[SLIDING_WINDOW_SIZE];
    match_info m;
    int last_block_result = default_binary_decode_block(&m, f);
    while(last_block_result != 0) {
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
        last_block_result = default_binary_decode_block(&m, f);
    }

    return 0;

}

void print_help() {
    printf("Usage: ZEncode [flags] filepath\n");
    printf("Flags list:\n");
    printf("    -c specifies compression mode\n");
    printf("    -d specifies decompression mode\n");
    printf("    -o specifies the output filepath. Usage: ZEncode -o output_path input_path\n");
}

int main(int argc, char** argv) {
    int compressMode = -1;
    char* filepath = NULL;
    char* outpath = NULL;
    char* last = NULL;
    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                compressMode = 1;
            } else if (argv[i][1] == 'd') {
                compressMode = 0;
            } else if (argv[i][1] == 'o') {
                last = "-o";
            } else if (!strcmp(argv[i], "--help")) {
                print_help();
                return 0;
            }
        } else {
            if (last) {
                if (!strcmp(last, "-o")) {
                    outpath = argv[i];
                }
                last = NULL;
            } else {
                filepath = argv[i];
            }
        }
    }
    if (!filepath) {
        printf("Usage: ZEncode [flags] filepath, or ZEncode --help for more info\n");
    }
    if (compressMode == -1) {
        compressMode = !ZEncode_check_is_encoded(filepath);
        if (!compressMode) {
            printf("Auto-detected as encoded file\n");
        }
    }
    if (compressMode) {
        printf("Encoding file %s...\n", filepath);
        int err = encode_file(filepath, outpath ? outpath : "encoded.bin");
        if (!err) {
            printf("File encoded! Result saved as %s\n", outpath ? outpath : "encoded.bin");
        } else {
            printf("Error: file could not be opened\n");
        }
    } else {
        printf("Decoding file %s...\n", filepath);
        ZEncode_header header;
        int err = decode_file(&header, filepath, outpath);
        if (!err) {
            printf("File decoded! Result saved as %s\n", outpath ? outpath : header.original_filename);
        } else {
            printf("Error: file could not be opened\n");
        }
    }
}