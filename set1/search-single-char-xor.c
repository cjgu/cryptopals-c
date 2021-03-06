#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "utils.h"

int
main(int argc, char *argv[])
{
  if (argc != 2){
    printf("Usage: search-single-char-xor <cryptotext>\n");
     exit(-1);
  }

  uint8_t *crypto_text;
  int len = decode_hex(argv[1], &crypto_text);

  uint8_t *plain_text;
  uint8_t key;
  int score = search_single_char_xor_key(crypto_text, len, &plain_text, &key);

  printf("Score: %d\n", score);
  printf("Key: 0x%X\n", key);
  printf("Plaintext: %s\n", plain_text);

  return 0;
}
