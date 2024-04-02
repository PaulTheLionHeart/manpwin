// BigComplex.h: interface for the Complex Bignum class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include	<math.h>
#include	"Complex.h"
#include	"floatexp.h"
//#include	"BigComplex.h"

#define		FALSE	0
#define		TRUE	1
#define		zerotol 1.e-50
#define		BYTE	unsigned char

class ExpComplex
    {
    public:
	ExpComplex(void)	{ }
	ExpComplex(const floatexp & real, const floatexp & imaginary)
	    {
	    x = real;
	    y = imaginary;
	    }

	ExpComplex(const ExpComplex & Cmplx1)// Copy Constructor
	    {
//	    mpfr_set(x.x, Cmplx1.x.x, MPFR_RNDN);
//	    mpfr_set(y.x, Cmplx1.y.x, MPFR_RNDN);
	    x = Cmplx1.x;
	    y = Cmplx1.y;
	    }

	ExpComplex(const floatexp & value)
	    {
	    x = value;
	    y = 0;
	    }
		
	~ExpComplex(void);

#ifdef	JONATHAN1
	inline	ExpComplex * operator =(const ExpComplex &);	// Assignment Operator
	inline	ExpComplex * operator =(const floatexp &);	// Assignment to a big double Operator
	ExpComplex * operator =(const Complex &);	// Assignment to a complex Operator
	ExpComplex * operator =(const double &);	// Assignment to a double Operator
	ExpComplex * operator+=(const ExpComplex &);
	ExpComplex * operator+=(floatexp &);
	ExpComplex * operator-=(const ExpComplex &);
	ExpComplex * operator-=(floatexp &);
	ExpComplex * operator*=(const ExpComplex &);
	ExpComplex * operator*=(floatexp &);
	ExpComplex * operator++(void);			// prefix ++ operator
//	ExpComplex * operator++(int);			// postfix ++ operator
#else
	ExpComplex * operator =(const ExpComplex &);	// Assignment Operator
	ExpComplex * operator =(const floatexp &);	// Assignment to a double Operator
	ExpComplex * operator =(const Complex &);	// Assignment Operator
	ExpComplex * operator =(const double &);	// Assignment to a double Operator
//	ExpComplex * operator =(const BigComplex &);	// Assignment to a BigComplex Operator
	ExpComplex * operator+=(const ExpComplex &);
	ExpComplex * operator+=(floatexp&);
	ExpComplex * operator-=(const ExpComplex &);
	ExpComplex * operator-=(floatexp &);
	ExpComplex * operator*=(const ExpComplex &);
	ExpComplex * operator*=(floatexp &);
	ExpComplex * operator++(void);
#endif
	bool	   operator==(ExpComplex &);
//	ExpComplex operator^(floatexp &);
//	ExpComplex operator^(ExpComplex &);
//	ExpComplex operator^(BYTE &);
	ExpComplex operator +(const ExpComplex &);	// Addition Operator
	ExpComplex operator +(const floatexp &);	// complex add by double Operator
	ExpComplex operator -(const ExpComplex &);	// Subtraction Operator
	ExpComplex operator -(const floatexp &);	// complex subtract by double Operator
	ExpComplex operator -(void);			// unary minus
	ExpComplex operator *(const ExpComplex &);	// Multiplication Operator
	ExpComplex operator *(const floatexp &);	// complex multiply by double Operator
	ExpComplex operator /(const ExpComplex &);	// Division Operator
	ExpComplex operator /(const floatexp &);	// complex divide by double Operator
	double	   CSumSqr();				// real squared + imaginary squared
//	floatexp   CFabs(void);				// abs
	ExpComplex CSqr(void);				// square
	ExpComplex CCube(void);				// cube
/*
	BigComplex CInvert(void);			// invert
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
	BigComplex BigComplexPower(BigComplex &);	// a^b
	BigComplex CSqrt(void);				// square root

//	void	   MTCExp(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4);
//	void	   MTCSin(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4);
//	void	   MTCCos(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4);


//	inline friend BigComplex operator*(BigDouble real, BigComplex num)
//		{return BigComplex(num.x*real, num.y*real);}
//	inline friend BigComplex operator+(BigDouble real, BigComplex num)
//		{return BigComplex(num.x+real, num.y);}
//	inline friend BigComplex operator-(BigDouble real, BigComplex num)
//		{return BigComplex(real-num.x, -num.y);}
*/
	floatexp	x, y;
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
