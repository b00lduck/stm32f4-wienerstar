//#pragma GCC optimize ("O0")

#include "main.h"
#include <float.h>
#include <math.h>
#include "tools/isspace.h"
#include "tools/isdigit.h"

#define EOK		0
#define ERANGE 	1

/**
 *
 */
uint8_t my_strtof(const char *str, char **endptr, float* ret) {

  float number;
  int exponent;
  int negative;
  char *p = (char*) str;
  float p10;
  int n;
  int num_digits;
  int num_decimals;

  // Skip leading whitespace
  while (isspace8(*p)) p++;

  // Handle optional sign
  negative = 0;
  switch (*p) {
    case '-': negative = 1; // Fall through to increment position
    case '+': p++;
  }

  number = 0.;
  exponent = 0;
  num_digits = 0;
  num_decimals = 0;

  // Process string of digits
  while (isdigit8(*p)) {
    number = number * 10. + (*p - '0');
    p++;
    num_digits++;
  }

  // Process decimal part
  if (*p == '.') {
    p++;

    while (isdigit8(*p)) {
      number = number * 10. + (*p - '0');
      p++;
      num_digits++;
      num_decimals++;
    }

    exponent -= num_decimals;
  }

  if (num_digits == 0) {
	  *ret = 0.0f;
	  return ERANGE;
  }

  // Correct for sign
  if (negative) number = -number;

  // Process an exponent string
  if (*p == 'e' || *p == 'E') {
    // Handle optional sign
    negative = 0;
    switch (*++p) {
      case '-': negative = 1;   // Fall through to increment pos
      case '+': p++;
    }

    // Process string of digits
    n = 0;
    while (isdigit8(*p)) {
      n = n * 10 + (*p - '0');
      p++;
    }

    if (negative)
      exponent -= n;
    else
      exponent += n;
  }

  if (exponent < FLT_MIN_EXP  || exponent > FLT_MAX_EXP) {
	  *ret = HUGE_VAL;
	  return ERANGE;
  }

  // Scale the result
  p10 = 10.;
  n = exponent;
  if (n < 0) n = -n;
  while (n) {
    if (n & 1) {
      if (exponent < 0)
        number /= p10;
      else
        number *= p10;
    }
    n >>= 1;
    p10 *= p10;
  }

  if (number == HUGE_VAL) {
	  return ERANGE;
  }

  if (endptr) *endptr = p;

  *ret = number;
  return EOK;
}
