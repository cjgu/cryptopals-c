#include <assert.h>
#include <stdio.h>

#include "utils.h"

/**
 * Decodes hex string into bytes
 */
int
decode_hex(char *hex_str, uint8_t **bytes)
{
  size_t len = strlen(hex_str);

  assert(len % 2 == 0);

  (*bytes) = calloc(len/2, sizeof(uint8_t));

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
      return -1;
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
encode_hex(uint8_t *bytes, int len)
{
  char *hex_str = calloc(len*2, sizeof(char));
  for (int i = 0; i < len; i++)
  {
     hex_str[2*i] = hex_map[(bytes[i] & 0xF0) >> 4];
     hex_str[2*i+1] = hex_map[(bytes[i] & 0x0F)];
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

int
decode_b64(char *b64_str, uint8_t **bytes)
{

  int len = strlen(b64_str);

  assert(len % 4 == 0);

  int buf_len =  3 * len/4;
  (*bytes) = calloc(buf_len, sizeof(uint8_t));
  uint8_t * buf = *bytes;
  int buf_pos = 0;

  for (int i = 0; i < len; i+=4)
  {
    uint8_t c[4];

    for (int j = 0; j < 4; j++) {
      char cur = b64_str[i+j];

      if (cur >= 'A' && cur <= 'Z')
      {
        c[j] = cur - 'A';
      }
      else if (cur >= 'a' && cur <= 'z')
      {
        c[j] = cur - 'a' + 26;
      }
      else if (cur >= '0' && cur <= '9')
      {
        c[j] = cur - '0' + 26*2;
      }
      else if (cur == '+')
      {
        c[j] = 62;
      }
      else if (cur == '/')
      {
        c[j] = 63;
      }
      else if (cur == '=')
      {
        c[j] = 0xFF;
      }
      else {
        printf("unknown char at %d: 0x%x\n", i+j, cur);
        assert(0);
      }
    }

    uint8_t u[3];
    int byte_count = 0;
    u[0] = (c[0] << 2) | ((c[1] & 0x30) >> 4);
    byte_count++;
    if (c[2] != 0xFF)
    {
      u[1] = ((c[1] & 0x0F) << 4) | ((c[2] >> 2) & 0xF);
      byte_count++;

      if (c[3] != 0xFF)
      {
        u[2] = ((c[2] & 0x03) << 6) | (0x3F & c[3]);
        byte_count++;
      }
    }

    memcpy((void *)&buf[buf_pos], (void *)&u, byte_count);
    buf_pos += byte_count;
  }

  return buf_pos;
}

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


void
xor(uint8_t *in1, uint8_t *in2, int len, uint8_t **out)
{
  *out = calloc(len, sizeof(uint8_t));

  for (int i = 0; i < len; i++)
  {
    (*out)[i] = in1[i] ^ in2[i];
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
  for(int i = '0'; i <= '9'; i++)
    sum += freqs[i];
  for(int i = 'A'; i <= 'Z'; i++)
    sum += freqs[i];
  sum += freqs['\''];
  sum += freqs[' '];
  sum += freqs['\n'];
  sum += freqs['-'];
  sum += freqs[':'];

  return sum;
}

/*
 * replace with memset :)
 */
void
fill_key(uint8_t key_char, uint8_t *key, int len)
{
  for (int i = 0; i < len; i++)
  {
    key[i] = key_char;
  }
}

void
repeat_key(uint8_t *key, int key_len, uint8_t **repeated_key, int repeated_key_len)
{
  assert(key_len <= repeated_key_len);
  (*repeated_key) = calloc(repeated_key_len, sizeof(uint8_t));

  for(int i = 0; i < repeated_key_len; i+= key_len)
  {
    int n =  (i + key_len) <= repeated_key_len ? key_len : repeated_key_len - i;

    memcpy((*repeated_key)+i, key, n);
  }

}

/*
 * Returns highest scoring plaintext
 */
int
search_single_char_xor_key(uint8_t *crypto_text, int len, uint8_t **plain_text, uint8_t *max_score_key)
{
  uint8_t *key = calloc(len, sizeof(uint8_t));

  uint8_t *plain_texts[0xFF];
  int *plain_text_scores = calloc(0xFF, sizeof(int));

  for (uint8_t key_char = 0; key_char < 0xFF; key_char++)
  {
    fill_key(key_char, key, len);

    xor(crypto_text, key, len, &plain_texts[key_char]);

    plain_text_scores[key_char] = score_plain_text(plain_texts[key_char], len);
  }

  // Find highest scoring plaintext
  uint8_t max_index = 0;
  int max = 0;
  for (uint8_t i = 0; i < 0xFF; i++)
  {
    if(plain_text_scores[i] >= max){
      max = plain_text_scores[i];
      max_index = i;
    }
  }

  (*plain_text) = plain_texts[max_index];
  for (uint8_t i = 'A'; i <= 'Z'; i++){
    if (i != max_index){
      free(plain_texts[i]);
    }
  }

  int highest_score = plain_text_scores[max_index];
  *max_score_key = max_index;
  free(plain_text_scores);
  free(key);

  return highest_score;
}

int
hamming_distance(uint8_t *buf_1, uint8_t *buf_2, int len)
{
  uint8_t *res;
  xor(buf_1, buf_2, len, &res);

  int dist = 0;

  // Count the number of set bits per byte
  for (int i = 0; i < len; i++)
  {
    uint8_t val = res[i];

    while(val)
    {
      ++dist;
      val &= val - 1;
    }
  }

  free(res);

  return dist;
}


int
load_file(char *file_path, uint8_t **buffer)
{
  ssize_t linelen = 0;
  size_t linecap = 0;
  char *line = NULL;

  int buf_len = 1024;
  (*buffer) = malloc(buf_len*sizeof(uint8_t));
  uint8_t *buf = (*buffer);
  int pos = 0;

  FILE *fd;

  fd = fopen(file_path, "r");

  while ((linelen = getline(&line, &linecap, fd)) > 0)
  {
    if (line[linelen-1] == '\n'){
      line[linelen-1] = '\0';
    }
    uint8_t *crypto_text = NULL;
    int len = decode_b64(line,  &crypto_text);
    if (len <= 0){
      printf("Invalid hex string in '%s'\n", line);
      free(line);
      fclose(fd);
      return -1;
    }

    if (len + pos > buf_len)
    {
      // realloc
      (*buffer) = realloc(*buffer, buf_len + 1024);
      buf_len += 1024;
    }

    memcpy(&buf[pos], crypto_text, len);
    pos += len;

    free(crypto_text);
    crypto_text = 0;
    free(line);
    line = NULL;
  }
  fclose(fd);
  return pos;
}


uint8_t *
break_repeating_key(int key_size, uint8_t *crypto_text, int len)
{
  // Transpose blocks of length key_size
  uint8_t **blocks = calloc(key_size, sizeof(uint8_t *));
  int *blocks_len = calloc(key_size, sizeof(int));
  for (int i = 0; i < key_size; i++)
  {
    blocks[i] = calloc(len/key_size+1, sizeof(uint8_t));
    int pos = 0;
    for (int j = i; j < len; j+=key_size, pos++)
    {
      blocks[i][pos] = crypto_text[j];
    }
    blocks_len[i] = pos;
  }

  uint8_t *key = calloc(key_size, sizeof(uint8_t));
  for (int i = 0; i < key_size; i++)
  {
    uint8_t *plaintext;
    int score = search_single_char_xor_key(blocks[i], blocks_len[i], &plaintext, &key[i]);
    //printf("Key %d: 0x%X = '%c' score: %d\n", i, key[i], key[i], score);

  }

  for (int i = 0; i < key_size; i++)
  {
    free(blocks[i]);
  }
  free(blocks);
  free(blocks_len);

  return key;
}

void print_buffer(uint8_t *buffer, int len)
{
  for (int i = 0; i < len; i++){
    putchar(buffer[i]);
  }
  putchar('\n');
}
