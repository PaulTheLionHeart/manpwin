#pragma once

#include	<math.h>
#include	"mpfr.h"

#define		FALSE	0
#define		TRUE	1
#define		zerotol 1.e-50    // 1.e-14
#define		SAFETYMARGIN	5			// Multiply number of digits by a safety margin 

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

	BigDouble * operator =(const BigDouble &);	// Assignment Operator
	BigDouble * operator =(const double &);		// Assignment to a double Operator
	BigDouble * operator+=(const BigDouble &);
	BigDouble * operator+=(double&);
	BigDouble * operator-=(const BigDouble &);
	BigDouble * operator-=(double &);
	BigDouble * operator*=(const BigDouble &);
	BigDouble * operator*=(double &);

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

	int	  ChangePrecision(int n);		// increase precision to n digits -1 means precision is out of range

	BigDouble operator +(const BigDouble &);	// Addition Operator
	BigDouble operator +(const double &);		// add to double Operator
	BigDouble operator -(const BigDouble &);	// Subtraction Operator
	BigDouble operator -(const double &);		// subtract from double Operator
	BigDouble operator -(void);			// unary minus
	BigDouble operator *(const BigDouble &);	// Multiplication Operator
	BigDouble operator *(const double &);		// Multiply by double Operator
	BigDouble operator /(const BigDouble &);	// Division Operator
	BigDouble operator /(const double &);		// divide by double Operator

	bool	  operator==(BigDouble &);
	bool	  operator<(BigDouble &);
	bool	  operator>(BigDouble &);
	bool	  operator==(double &);
	bool	  operator<(double &);
	bool	  operator>(double &);
	BigDouble operator^(double &);
	BigDouble operator^(BigDouble &);
//	BigDouble operator++(void);

	mpfr_t		x;					// pointer to BigNum
	mpfr_prec_t	bitcount;
    };



