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
     printf("Usage: ...\n");
     exit(-1);
  }

  FILE *fd = fopen(argv[1], "r");

  if (fd == NULL){
    printf("Cannot open file\n");
    exit(-1);
  }

  int max_score = -1;
  uint8_t *max_plain_text = NULL;
  int line_no =-1;

  ssize_t linelen = 0;
  size_t linecap = 0;
  char *line = NULL;
  while ((linelen = getline(&line, &linecap, fd)) > 0)
  {
    line_no++;

    if (line[linelen-1] == '\n'){
      line[linelen-1] = '\0';
    }

    uint8_t *crypto_text;
    int len = decode_hex(line, &crypto_text);

    uint8_t *plaintext;
    uint8_t key;
    int score = search_single_char_xor_key(crypto_text, len, &plaintext, &key);

    if (score > max_score)
    {
      max_score = score;
      free(max_plain_text);
      max_plain_text = plaintext;
    }
    else {
      free(plaintext);
      plaintext = NULL;
    }

    free(crypto_text);
    crypto_text = NULL;
    free(line);
    line = NULL;
  }

  printf("Max score %d\n", max_score);
  printf("Plaintext: %s\n", max_plain_text);

  return 0;
}


