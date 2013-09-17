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
     exit(-1);
  }

  uint8_t *bytes;
  int len = decode_hex(argv[1], &bytes);

  char *b64 = encode_b64(bytes, len);
  free(bytes);

  printf("%s\n", b64);

  free(b64);
}

