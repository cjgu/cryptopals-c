#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>


#include "utils.h"

void
fill_key(uint8_t key_char, uint8_t *key, int len)
{
  for (int i = 0; i < len; i++)
  {
    key[i] = key_char;
  }
}


int
score_plain_text(uint8_t *plain_text, int len)
{
  int *freqs = calloc(0xFF, sizeof(int));

  for(int i = 0; i < len; i++){
    freqs[plain_text[i]]++;
  }
  int sum = 0;
  for(int i = 'a'; i <= 'z'; i++)
    sum += freqs[i];
  for(int i = 'A'; i <= 'Z'; i++)
    sum += freqs[i];
  sum += freqs[' '];

  return sum;
}

int
main(int argc, char *argv[])
{
  if (argc != 2){
     exit(-1);
  }

  uint8_t *crypto_text;
  int len = decode_hex(argv[1], &crypto_text);

  uint8_t *key = calloc(len, sizeof(uint8_t));

  uint8_t *plain_texts[0xFF];
  int *plain_text_scores = calloc(0xFF, sizeof(int));

  for (uint8_t key_char = 'A'; key_char <= 'Z'; key_char++)
  {
    fill_key(key_char, key, len);

    xor(crypto_text, key, len, &plain_texts[key_char]);

    plain_text_scores[key_char] = score_plain_text(plain_texts[key_char], len);

    //printf("Key: 0x%X Score %d Plaintext: %s\n", key_char, plain_text_scores[key_char], plain_texts[key_char]);
  }

  // Find highest scoring plaintext
  uint8_t max_index = 0;
  int max = 0;
  for (uint8_t i; i < 0xFF; i++)
  {
    if(plain_text_scores[i] > max){
      max = plain_text_scores[i];
      max_index = i;
    }
  }

  printf("Highest scoring plaintext: '%s'. Key 0x%X=%c\n", plain_texts[max_index], max_index, max_index);

  for (uint8_t i = 'A'; i <= 'Z'; i++){
    free(plain_texts[i]);
  }
  return 0;
}
