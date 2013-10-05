#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "utils.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("usage: break-repeating-key-xor <cryptotext file path>\n");
    exit(-1);
  }


  uint8_t *buffer;
  int buf_len = load_file(argv[1], &buffer);

  // Figure ut KEYSIZE
  float min_norm_dist = 100;
  int min_key_size = 0;
  for (int key_size = 2; key_size < 40; key_size++)
  {
    int dist = hamming_distance(&buffer[0], &buffer[key_size], key_size);
    int dist2 = hamming_distance(&buffer[key_size*2], &buffer[key_size*3], key_size);
    int dist3 = hamming_distance(&buffer[0], &buffer[key_size*3], key_size);
    int dist4 = hamming_distance(&buffer[0], &buffer[key_size*2], key_size);
    float norm_dist1 = ((float)dist)/key_size;
    float norm_dist2 = ((float)dist2)/key_size;
    float norm_dist3 = ((float)dist3)/key_size;
    float norm_dist4 = ((float)dist4)/key_size;
    float norm_avg_dist = (norm_dist1 + norm_dist2 + norm_dist3 + norm_dist4)/4;
    if (norm_avg_dist < min_norm_dist) {
      min_norm_dist = norm_avg_dist;
      min_key_size = key_size;
    }
  }

  printf("Min dist key size: %d\n", min_key_size);
  int key_size = min_key_size;
  uint8_t * key = break_repeating_key(key_size, buffer, buf_len);
  uint8_t *rep_key;
  repeat_key(key, key_size, &rep_key, buf_len);

  uint8_t * plaintext;
  xor(buffer, rep_key, buf_len, &plaintext);

  printf("Key: '%s':\nPlaintext:\n", key);
  print_buffer(plaintext, buf_len);
  free(key); key=NULL;
  free(rep_key); rep_key = NULL;
  free(plaintext); plaintext = NULL;

  free(buffer);
  return 0;
}

