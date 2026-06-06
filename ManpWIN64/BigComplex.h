/*
    BigComplex.h: interface for the Complex Bignum class.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#pragma once

#include	<math.h>
#include	"BigDouble.h"
#include	"Complex.h"
#include	"ExpComplex.h"

#define		FALSE	0
#define		TRUE	1
#define		zerotol 1.e-50
#define		BYTE	unsigned char

class BigComplex
    {
    public:
	BigComplex(void)	{ }
	BigComplex(const BigDouble & real, const BigDouble & imaginary)
	    {
	    x = real;
	    y = imaginary;
	    }

	BigComplex(const BigComplex & Cmplx1)// Copy Constructor
	    {
	    mpfr_set(x.x, Cmplx1.x.x, MPFR_RNDN);
	    mpfr_set(y.x, Cmplx1.y.x, MPFR_RNDN);
//	    x = Cmplx1.x;
//	    y = Cmplx1.y;
	    }

	BigComplex(const BigDouble & value)
	    {
	    x = value;
	    y = 0;
	    }
		
	~BigComplex(void);


	BigComplex & operator =(const BigComplex &);	// Assignment Operator
	BigComplex & operator =(const BigDouble &);	// Assignment to a big double Operator
	BigComplex & operator =(const Complex &);	// Assignment to a complex Operator
	BigComplex & operator =(const double &);	// Assignment to a double Operator
	BigComplex & operator =(const ExpComplex &);	// Assignment to a ExpComplex Operator
	BigComplex & operator+=(const BigComplex &);
	BigComplex & operator+=(const BigDouble&);
	BigComplex & operator+=(double rhs);
	BigComplex & operator-=(const BigComplex &);
	BigComplex & operator-=(const BigDouble &);
	BigComplex & operator*=(const BigComplex &);
	BigComplex & operator*=(const BigDouble &);
	BigComplex & operator++(void);			// prefix ++ operator
//	BigComplex & operator++(int);			// postfix ++ operator

	bool	   operator==(const BigComplex &) const;
	BigComplex operator^(const BigDouble &) const;
	BigComplex operator^(const BigComplex &) const;
	BigComplex operator^(WORD &);
	BigComplex operator +(const BigComplex &);	// Addition Operator
	BigComplex operator +(const BigDouble &);	// complex add by double Operator
	BigComplex operator +(const ExpComplex &);	// complex add by ExpComplex Operator
	BigComplex operator -(const BigComplex &);	// Subtraction Operator
	BigComplex operator -(const BigDouble &);	// complex subtract by double Operator
	BigComplex operator -(void);			// unary minus
	BigComplex operator *(const BigComplex &);	// Multiplication Operator
	BigComplex operator *(const BigDouble &);	// complex multiply by double Operator
	BigComplex operator /(const BigComplex &);	// Division Operator
	BigComplex operator /(const BigDouble &);	// complex divide by double Operator

	BigComplex CSqr(void);				// square
	BigComplex CCube(void);				// cube
	BigComplex CInvert(void);			// invert
	double	   CSumSqr(void);			// real squared + imaginary squared
	BigComplex CPolynomial(int);			// take a complex number to an integer power
	BigComplex CSin(void) const;			// sine of a complex number
	BigComplex CCos(void) const;			// cosine
	BigComplex CTan();				// tangent
	BigComplex CSinh() const;			// hyperbolic sine of a complex number
	BigComplex CCosh() const;			// hyperbolic cosine
	BigComplex CTanh();				// hyperbolic tangent
	BigComplex CExp(void) const;			// exponent
	BigComplex CDouble(void);			// double   r = 2*n
	BigComplex CHalf(void);				// half	    r = n/2
	BigComplex CLog() const;			// log
	BigDouble  CFabs(void);				// abs
	BigComplex BigComplexPower(const BigComplex &) const;	// a^b
	BigComplex CSqrt(void);				// square root
	Complex    CBig2Double(void);		// convert BigComplex to Complex

	BigDouble	x, y;
    };



