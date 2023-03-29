#include "cipher.h"

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.

#define ALPHABET_SIZE 26

int
is_upper (char c)
{
  return ('A' <= c && c <= 'Z');
}

int
is_lower (char c)
{
  return ('a' <= c && c <= 'z');
}

int
is_alpha (char c)
{
  return (is_upper (c) || is_lower (c));
}

// See full documentation in header file
void
encode (char s[], int k)
{
  // prevent k < 0
  while (k < 0)
    {
      k = (0 - k) % ALPHABET_SIZE;
      k = ALPHABET_SIZE - k;
    }

  if (k % ALPHABET_SIZE == 0)
    {
      return;
    }

  int i = 0;
  while (s[i] != '\0')
    {
      if (is_upper (s[i]))
        {
          s[i] = 'A' + (s[i] - 'A' + k) % ALPHABET_SIZE;
        }
      else if (is_lower (s[i]))
        {
          s[i] = 'a' + (s[i] - 'a' + k) % ALPHABET_SIZE;
        }
      i++;
    }
}

// See full documentation in header file
void
decode (char s[], int k)
{
  encode (s, 0 - k);
}
