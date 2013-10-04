#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "utils.h"

static void
test_hamming_distance_zero(void **state)
{
  uint8_t a[] = {0x00, 0x00};
  uint8_t b[] = {0x00, 0x00};
  int dist = hamming_distance(a, b, 2);

  assert_int_equal(dist, 0);
}

static void
test_hamming_distance_one(void **state)
{
  uint8_t a[] = {0x00, 0x00};
  uint8_t b[] = {0x00, 0x01};
  int dist = hamming_distance(a, b, 2);

  assert_int_equal(dist, 1);
}

static void
test_hamming_distance_complex(void **state)
{
  uint8_t a[] = {0xFF, 0x30};
  uint8_t b[] = {0xF0, 0x01};
  int dist = hamming_distance(a, b, 2);

  assert_int_equal(dist, 7);
}

static void
test_hamming_distance_string(void **state)
{
  uint8_t a[] = "this is a test";
  uint8_t b[] = "wokka wokka!!!";
  int dist = hamming_distance(a, b, 14);

  assert_int_equal(dist, 37);
}

static void
test_decode_b64_simple(void **state)
{
  uint8_t * b;
  int c = decode_b64("AAAA", &b);

  assert_int_equal(c, 3);
  for (int i = 0; i < 3; i++){
    assert_int_equal(b[i], 0x00);
  }
  free(b);
}

static void
test_decode_b64_padding(void **state)
{
  uint8_t * b;
  int c = decode_b64("AA==", &b);

  assert_int_equal(c, 1);
  for (int i = 0; i < c; i++){
    assert_int_equal(b[i], 0x00);
  }
  free(b);
}

static void
test_decode_b64_complex(void **state)
{
  uint8_t * b;
  int c = decode_b64("YW55IGNhcm5hbCBwbGVhc3VyZS4=", &b);
  char * actual = "any carnal pleasure.";

  assert_int_equal(c, strlen(actual));
  for (int i = 0; i < c; i++){
    assert_int_equal(b[i], actual[i]);
  }
  free(b);
}

static void
test_encode_b64_complex(void **state)
{
  char* in = "any carnal pleasure.";
  char * b = encode_b64(in, strlen(in));
  char * actual = "YW55IGNhcm5hbCBwbGVhc3VyZS4=";

  for (int i = 0; i < strlen(b); i++){
    assert_int_equal(b[i], actual[i]);
  }
  free(b);
}

int main(void)
{
  const UnitTest tests[] = {
    unit_test(test_hamming_distance_zero),
    unit_test(test_hamming_distance_one),
    unit_test(test_hamming_distance_complex),
    unit_test(test_hamming_distance_string),
    unit_test(test_decode_b64_simple),
    unit_test(test_decode_b64_padding),
    unit_test(test_decode_b64_complex),
    unit_test(test_encode_b64_complex),
  };

  return run_tests(tests);
}
