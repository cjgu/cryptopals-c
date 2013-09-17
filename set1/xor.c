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
    exit(-1);
  }
  uint8_t *input1;
  int input1_len = decode_hex(argv[1], &input1);

  uint8_t *input2;
  int input2_len = decode_hex(argv[2], &input2);

  if (input1_len != input2_len){
    exit(-1);
  }

  uint8_t *output;
  xor(input1, input2, input1_len, &output);

  char *output_str = encode_hex(output, input1_len);

  printf("%s\n", output_str);

  return 0;
}
