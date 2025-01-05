// A cludge to get around these routines not being found during link. 
// Also a few routines needed for conversion
// PHD 20241221

#include  <Windows.h>
#include  "qdlib/qd_real.h"
#include  "qdlib/fpu.h"
#include  "BigDouble.h"
#include  "big.h"

extern	HWND	GlobalHwnd;				// This is the main windows handle


void	BigDouble2DD(dd_real *out, BigDouble in)
    {
    char	*s1 = NULL;
    s1 = new char[SIZEOF_BF_VARS];
    mpfr_sprintf(s1, "%.36Rf", in.x);
    *out = s1;
    if (s1) { delete[] s1; s1 = NULL; }
    }

/////////////////////////////////////////////////////////////////////
// A cludge to get around these routines not being found during link. 
/////////////////////////////////////////////////////////////////////

    /* This routine is called whenever a fatal error occurs. */
void dd_real::error(const char *msg) { 
  if (msg) { 
  /*cerr << "ERROR " << msg << endl;*/ 
      MessageBox(GlobalHwnd, msg, "Double-Double Aritmetic", MB_ICONEXCLAMATION | MB_OK);

  }
}


/* Computes the n-th power of a double-double number. 
   NOTE:  0^0 causes an error.                         */
dd_real npwr(const dd_real &a, int n) {
  
  if (n == 0) {
    if (a.is_zero()) {
      dd_real::error("(dd_real::npwr): Invalid argument.");
      return dd_real::_nan;
    }
    return 1.0;
  }

  dd_real r = a;
  dd_real s = 1.0;
  int N = std::abs(n);

  if (N > 1) {
    /* Use binary exponentiation */
    while (N > 0) {
      if (N % 2 == 1) {
        s *= r;
      }
      N /= 2;
      if (N > 0)
        r = sqr(r);
    }
  } else {
    s = r;
  }

  /* Compute the reciprocal if n is negative. */
  if (n < 0)
    return (1.0 / s);
  
  return s;
}


/* Reads in a double-double number from the string s. */
int dd_real::read(const char *s, dd_real &a) {
  const char *p = s;
  char ch;
  int sign = 0;
  int point = -1;
  int nd = 0;
  int e = 0;
  bool done = false;
  dd_real r = 0.0;
  int nread;
  
  /* Skip any leading spaces */
  while (*p == ' ')
    p++;

  while (!done && (ch = *p) != '\0') {
    if (ch >= '0' && ch <= '9') {
      int d = ch - '0';
      r *= 10.0;
      r += static_cast<double>(d);
      nd++;
    } else {

      switch (ch) {

      case '.':
        if (point >= 0)
          return -1;        
        point = nd;
        break;

      case '-':
      case '+':
        if (sign != 0 || nd > 0)
          return -1;
        sign = (ch == '-') ? -1 : 1;
        break;

      case 'E':
      case 'e':
        nread = std::sscanf(p+1, "%d", &e);
        done = true;
        if (nread != 1)
          return -1;
        break;

      default:
        return -1;
      }
    }
    
    p++;
  }

  if (point >= 0) {
    e -= (nd - point);
  }

  if (e != 0) {
    r *= (dd_real(10.0) ^ e);
  }

  a = (sign == -1) ? -r : r;
  return 0;
}

const dd_real dd_real::_nan = dd_real(qd::_d_nan, qd::_d_nan);

/* Constructor.  Reads a double-double number from the string s
   and constructs a double-double number.                         */
dd_real::dd_real(const char *s) {
    if (dd_real::read(s, *this)) {
	dd_real::error("(dd_real::dd_real): INPUT ERROR.");
	*this = dd_real::_nan;
	}
    }

dd_real &dd_real::operator=(const char *s) {
    if (dd_real::read(s, *this)) {
	dd_real::error("(dd_real::operator=): INPUT ERROR.");
	*this = dd_real::_nan;
	}
    return *this;
    }

