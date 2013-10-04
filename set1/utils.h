#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int decode_hex(char *hex_str, uint8_t **bytes);
char * encode_hex(uint8_t bytes[], int len);

int decode_b64(char *b64_str, uint8_t **bytes);
char * encode_b64(uint8_t *bytes, int len);

void xor(uint8_t *in1, uint8_t *in2, int len, uint8_t **out);

int score_plain_text(uint8_t *plain_text, int len);

void fill_key(uint8_t key_char, uint8_t *key, int len);

int search_single_char_xor_key(uint8_t *crypto_text, int len, uint8_t **plain_text);

void repeat_key(char *key, int key_len, uint8_t **repeated_key, int repeated_key_len);

int hamming_distance(uint8_t *buf_1, uint8_t *buf_2, int len);

int load_file(char *file_path, uint8_t **buffer);
