#include "tests.h"
#include <string.h>

#define K3 3
#define K2 2
#define KM1 -1
#define KM3 -3
#define K29 29

// See full documentation in header file
int
test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "abc";
  encode (in, K3);
  return 0 != strcmp (in, "def");
}

// See full documentation in header file
int
test_encode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "yz!,cd+";
  encode (in, K2);
  return 0 != strcmp (in, "ab!,ef+");
}

// See full documentation in header file
int
test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "p!,bcd+";
  encode (in, KM1);
  return 0 != strcmp (in, "o!,abc+");
}

// See full documentation in header file
int
test_encode_cyclic_lower_case_negative_k ()
{
  char in[] = "abc";
  encode (in, KM3);
  return 0 != strcmp (in, "xyz");
}

// See full documentation in header file
int
test_encode_cyclic_upper_case_positive_k ()
{
  char in[] = "ABC";
  encode (in, K29);
  return 0 != strcmp (in, "DEF");
}

// See full documentation in header file
int
test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "def";
  decode (in, K3);
  return 0 != strcmp (in, "abc");
}

// See full documentation in header file
int
test_decode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "ab!,ef+";
  decode (in, K2);
  return 0 != strcmp (in, "yz!,cd+");
}

// See full documentation in header file
int
test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = "o!,abc+";
  decode (in, KM1);
  return 0 != strcmp (in, "p!,bcd+");
}

// See full documentation in header file
int
test_decode_cyclic_lower_case_negative_k ()
{
  char in[] = "xyzd";
  decode (in, KM3);
  return 0 != strcmp (in, "abcg");
}

// See full documentation in header file
int
test_decode_cyclic_upper_case_positive_k ()
{
  char in[] = "DEF";
  decode (in, K29);
  return 0 != strcmp (in, "ABC");
}
