#pragma once

#include	<math.h>
#pragma once
#include <mpfr.h>
//#pragma message("MPFR version (header): " MPFR_VERSION_STRING)
#include	"../qdlib/dd_real.h"
#include	"../qdlib/qd_real.h"

#define		FALSE	0
#define		TRUE	1
#define		zerotol 1.e-50    // 1.e-14
#define		SAFETYMARGIN	4			// Multiply number of digits by a safety margin 

//#define	errorcode -1.e60

extern	int	decimals;

class BigDouble
    {
    public:
	BigDouble(void);
	~BigDouble(void);
	BigDouble(const double & value)
	    {
	    bitcount = decimals * SAFETYMARGIN;
	    mpfr_init2(x, bitcount);	
	    mpfr_set_d(x, value, MPFR_RNDN);
	    }

	BigDouble(const BigDouble & Big)		// Copy Constructor
	    {
	    bitcount = decimals * SAFETYMARGIN;
	    mpfr_init2(x, bitcount);
	    mpfr_set(x, Big.x, MPFR_RNDN);
	    }

	BigDouble & operator =(const BigDouble &);	// Assignment Operator
	BigDouble & operator =(const double &);		// Assignment to a double Operator
	BigDouble & operator+=(const BigDouble &);
	BigDouble & operator+=(double&);
	BigDouble & operator-=(const BigDouble &);
	BigDouble & operator-=(double &);
	BigDouble & operator*=(const BigDouble &);
	BigDouble & operator*=(double &);

	BigDouble BigSqr(void);				// square is faster than * because of symmetry
	BigDouble BigSqrt(void);			// square root
	BigDouble BigInvert(void);			// invert
	BigDouble BigAbs(void);				// abs()
	BigDouble BigX2(void);				// double   r = 2*n
	BigDouble BigHalf(void);			// half	    r - n/2
	BigDouble BigCos();				// cos
	BigDouble BigSin();				// sin
	BigDouble BigTan();				// tan
	BigDouble BigSinh();				// sinh
	BigDouble BigCosh();				// cosh
	BigDouble BigTanh();				// tanh
	BigDouble BigLog();				// log
	BigDouble BigLog10();				// log10
	long	  BigDoubleToInt(void);			// convert a big double to int
	double	  BigDoubleToDouble(void);		// convert a big double to double
	int	  BigDouble2DD(dd_real *out);		// convert a big double to double-double
	int	  BigDouble2QD(qd_real *out);		// convert a big double to quad-double
	void	  SafeSprintf(char* buf, size_t bufSize, char* fmt) const;
	void	  ToString(char* buf, size_t bufSize, bool scientific = false) const;

	int	  ChangePrecision(int n);		// increase precision to n digits -1 means precision is out of range
	void	  MatchPrecision(const BigDouble& other);	// match the precision of an extisting bignum

	BigDouble operator +(const BigDouble &);	// Addition Operator
	BigDouble operator +(const double &);		// add to double Operator
	BigDouble operator -(const BigDouble &);	// Subtraction Operator
	BigDouble operator -(const double &);		// subtract from double Operator
	BigDouble operator -(void);			// unary minus
	BigDouble operator *(const BigDouble &);	// Multiplication Operator
	BigDouble operator *(const double &);		// Multiply by double Operator
	BigDouble operator /(const BigDouble &);	// Division Operator
	BigDouble operator /(const double &);		// divide by double Operator

	bool	  operator==(const BigDouble &) const;
	bool	  operator<(const BigDouble &) const;
	bool	  operator>(const BigDouble &) const;
	bool	  operator==(const double &) const;
	bool	  operator<(const double &) const;
	bool	  operator>(const double &) const;
	BigDouble operator^(const double &) const;
	BigDouble operator^(BigDouble &);
//	BigDouble operator++(void);

	mpfr_t		x;					// pointer to BigNum
	mpfr_prec_t	bitcount;
    };



