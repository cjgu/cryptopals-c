#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

int decode_hex(char *hex_str, uint8_t **bytes);
char * encode_hex(uint8_t bytes[], int len);

char * encode_b64(uint8_t *bytes, int len);

void xor(uint8_t *in1, uint8_t *in2, int len, uint8_t **out);

int score_plain_text(uint8_t *plain_text, int len);

void fill_key(uint8_t key_char, uint8_t *key, int len);

uint8_t *search_single_char_xor_key(uint8_t *crypto_text, int len);
