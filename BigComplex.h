// BigComplex.h: interface for the Complex Bignum class.
//
//////////////////////////////////////////////////////////////////////

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
	BigComplex & operator+=(BigDouble&);
	BigComplex & operator-=(const BigComplex &);
	BigComplex & operator-=(BigDouble &);
	BigComplex & operator*=(const BigComplex &);
	BigComplex & operator*=(BigDouble &);
	BigComplex & operator++(void);			// prefix ++ operator
//	BigComplex & operator++(int);			// postfix ++ operator

	bool	   operator==(BigComplex &);
	BigComplex operator^(BigDouble &);
	BigComplex operator^(BigComplex &);
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
	BigComplex CSin(void);				// sine of a complex number
	BigComplex CCos(void);				// cosine
	BigComplex CTan();				// tangent
	BigComplex CSinh();				// hyperbolic sine of a complex number
	BigComplex CCosh();				// hyperbolic cosine
	BigComplex CTanh();				// hyperbolic tangent
	BigComplex CExp(void);				// exponent
	BigComplex CDouble(void);			// double   r = 2*n
	BigComplex CHalf(void);				// half	    r = n/2
	BigComplex CLog(void);				// log
	BigDouble  CFabs(void);				// abs
	BigComplex BigComplexPower(BigComplex &);	// a^b
	BigComplex CSqrt(void);				// square root
	Complex CBig2Double(void);		// convert BigComplex to Complex

//	void	   MTCExp(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4);
//	void	   MTCSin(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4);
//	void	   MTCCos(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4);


//	inline friend BigComplex operator*(BigDouble real, BigComplex num)
//		{return BigComplex(num.x*real, num.y*real);}
//	inline friend BigComplex operator+(BigDouble real, BigComplex num)
//		{return BigComplex(num.x+real, num.y);}
//	inline friend BigComplex operator-(BigDouble real, BigComplex num)
//		{return BigComplex(real-num.x, -num.y);}

	BigDouble	x, y;
    };

//extern	BigComplex	CSin(BigComplex  &);		// sine of a complex number
//extern	BigComplex	CCos(BigComplex  &);		// cosine
//extern	BigComplex	CSqr(BigComplex  &);		// square
//extern	BigComplex	CCube(BigComplex  &);		// cube
//extern	BigDouble	BigCSumSqr(BigComplex  &);	// real squared + imaginary squared
//extern	BigDouble	BigCFabs(BigComplex  &);	// abs
//extern	double		CSumSqr(BigComplex  &);		// real squared + imaginary squared
//extern	double		CFabs(BigComplex  &);		// abs
//extern	BigComplex	CPolynomial(BigComplex  &, int);// take a complex number to an integer power
//extern	BigComplex	CInvert(BigComplex  &);		// invert
//extern	BigComplex	CExp(BigComplex  &);		// exponent
//extern	BigComplex	CLog(BigComplex  &);		// log
//extern	Complex	CComplexPower(Complex  &, Complex &);	// take a complex number to a complex power
// now replaced with operator z^x where z and x are both complex.
