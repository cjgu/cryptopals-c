#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "utils.h"

int
main(int argc, char *argv[])
{
  if (argc != 3){
    printf("usage: encrypt-xor <plain text> <key>\n");
    exit(-1);
  }

  int plaintext_len = strlen(argv[1]);
  uint8_t *plaintext = calloc(plaintext_len, sizeof(uint8_t));
  memcpy(plaintext, argv[1], plaintext_len);

  int key_len = strlen(argv[2]);
  assert(key_len <= plaintext_len);

  uint8_t *key;
  repeat_key(argv[2], key_len, &key, plaintext_len);

  uint8_t *crypto_text;
  xor(plaintext, key, plaintext_len, &crypto_text);

  char *output = encode_hex(crypto_text, plaintext_len);

  printf("Encrypted: %s\n", output);

  return 0;
}
