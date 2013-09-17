#include "utils.h"

/**
 * Decodes hex string into bytes
 */
int
decode_hex(char *hex_str, uint8_t **bytes)
{
  size_t len = strlen(hex_str);

  assert(len % 2 == 0);

  *bytes = calloc(len/2, sizeof(uint8_t));

  uint8_t prev;
  for (int i = 0; i < len; i++)
  {
    uint8_t tmp;
    if (hex_str[i] >= '0' && hex_str[i] <= '9')
    {
      tmp = hex_str[i] - '0';
    }
    else if (hex_str[i] >= 'a' && hex_str[i] <= 'f')
    {
      tmp = 10 + hex_str[i] - 'a';
    }
    else if (hex_str[i] >= 'A' && hex_str[i] <= 'F')
    {
      tmp = 10 + hex_str[i] - 'A';
    }
    else {
      /* Invalid hex string */
      free(*bytes);
      return 0;
    }

    if(i % 2 == 0){
      prev = tmp;
    }
    else {
      (*bytes)[i/2] = (prev << 4) | tmp;
    }
  }

  return len/2;
}

static char hex_map[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

char *
encode_hex(uint8_t bytes[], int len)
{
  char *hex_str = calloc(len*2, sizeof(char));
  for (int i = 0; i < len; i++)
  {
     hex_str[2*i] = hex_map[(bytes[i] & 0xF0) >> 4];
     hex_str[2*i+1] = hex_str[(bytes[i] & 0x0F)];
  }
  return hex_str;
}

static char b64_map[] = {
  'A', 'B', 'C', 'D', 'E',
  'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O',
  'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y',
  'Z', 'a', 'b', 'c', 'd',
  'e', 'f', 'g', 'h', 'i',
  'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's',
  't', 'u', 'v', 'w', 'x',
  'y', 'z', '0', '1', '2',
  '3', '4', '5', '6', '7',
  '8', '9', '+', '/'
};

char *
encode_b64(uint8_t *bytes, int len)
{

  int out_len;
  if(len % 3 == 0){
    out_len = 4 * len / 3;
  }
  else {
    out_len = 4 * (len / 3) + 4;
  }
  char *b64 = calloc(out_len, sizeof(char));

  /* Take 3 bytes at a time */
  for (int i = 0; i < len; i+=3)
  {
    char c[4];
    // 0xFC 0x00 0x00
    c[0] = b64_map[(0xFC & bytes[i]) >> 2];
    if (i+1 < len){
      // 0x03 0xF0 0x00
      c[1] = b64_map[((0x03 & bytes[i]) << 4) | ((0xF0 & bytes[i+1]) >> 4)];

      if (i + 2 < len) {
        // 0x00 0x0F 0xC0
        c[2] = b64_map[((0x0F & bytes[i+1]) << 2) | ((0xC0 & bytes[i+2]) >> 6)];
        // 0x00 0x00 0x3F
        c[3] = b64_map[0x3F & bytes[i+2]];
      }
      else {
        c[2] = b64_map[((0x0F & bytes[i+1]) << 2)];
        c[3] = '=';
      }
    }
    else {
      c[1] = b64_map[((0x03 & bytes[i]) << 4)];
      c[2] = '=';
      c[3] = '=';
    }

    // copy into output array
    b64[4*i/3] = c[0];
    b64[4*i/3+1] = c[1];
    b64[4*i/3+2] = c[2];
    b64[4*i/3+3] = c[3];
  }

  return b64;
}
