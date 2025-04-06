#include "default_binary_encoder.h"

ZEncode_header* init_default_binary_header(void* ptr, char* filename, uint64_t filesize, int max_match_length, int window_size, void* options) {
    ZEncode_header* header = default_header_init(ptr, 0, DEFAULT_BINARY_FORMAT_VERSION, filename, filesize, max_match_length, window_size, options);
    memcpy(&(header->subtype), "DFLT", 4);
}

int ctr = 0;

void default_binary_read_header(ZEncode_header* header, FILE* f) {

    uint16_t format_version, header_length;
    uint32_t max_match_length, window_size;
    uint64_t original_filesize;
    uint8_t filename_len;
    fread(&format_version, sizeof(uint16_t), 1, f);
    fread(&header_length, sizeof(uint16_t), 1, f);
    fread(&max_match_length, sizeof(uint32_t), 1, f);
    fread(&window_size, sizeof(uint32_t), 1, f);
    fread(&original_filesize, sizeof(uint64_t), 1, f);

    fread(&filename_len, sizeof(uint8_t), 1, f);
    char* original_filename = malloc(filename_len + 1);
    fread(original_filename, sizeof(char), filename_len, f);
    original_filename[filename_len] = '\0';

    init_default_binary_header(header, original_filename, original_filesize, max_match_length, window_size, NULL);
    printf("%d, %d, %d, %d, %lu, %s\n", header->format_version, header->header_length, header->max_match_length, header->window_size, header->original_filesize, header->original_filename);

}

void default_binary_write_header(FILE* f, ZEncode_header* header_info) {
    uint8_t filename_len = strlen(header_info->original_filename);
    fwrite(header_info->ZEncode_marker, 1, 8, f);
    fwrite(header_info->subtype, 1, 4, f);
    fwrite(&(header_info->format_version), 2, 1, f);
    fwrite(&(header_info->header_length), 2, 1, f);
    fwrite(&(header_info->max_match_length), 4, 1, f);
    fwrite(&(header_info->window_size), 4, 1, f);
    fwrite(&(header_info->original_filesize), 8, 1, f);
    fwrite(&filename_len, 1, 1, f);
    fwrite(header_info->original_filename, 1, filename_len, f);
    printf("%d, %d, %d, %d, %lu, %s\n", header_info->format_version, header_info->header_length, header_info->max_match_length, header_info->window_size, header_info->original_filesize, header_info->original_filename);
}

void default_binary_encode_block (FILE* f, match_info* m, uint8_t* dptr) {
    if (m->src_len == 0) {
        uint8_t b1 = 0x80;
        fwrite(&b1, 1, 1, f);
        fwrite(&(m->nomatch_symbol), 1, 1, f);
    } else {
        uint8_t b1 = 0x00;
        fwrite(&b1, 1, 1, f);
        fwrite(&(m->src_len), sizeof(uint16_t), 1, f);
        fwrite(&(m->match_idx), sizeof(uint16_t), 1, f);
    }
    ctr++;
}

int default_binary_decode_block (match_info* result, FILE* f) {
    int bytes_read = 0;
    uint8_t b1;
    bytes_read += fread(&b1, 1, 1, f);
    if (b1 & 0x80) {
        uint8_t num_bytes = b1 & 0x7F;
        uint8_t dblock[num_bytes];
        bytes_read += fread(dblock, 1, 1, f); // TODO actually read correct num of bytes
        result->src_len = 0;
        result->nomatch_symbol = dblock[0]; // TODO return multiple symbols
    } else {
        bytes_read += fread(&(result->src_len), sizeof(uint16_t), 1, f);
        bytes_read += fread(&(result->match_idx), sizeof(uint16_t), 1, f);
    }
    ctr++;
    return bytes_read;
}