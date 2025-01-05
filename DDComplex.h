// DDComplex.h: interface for the DDComplex class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include	<math.h>
#include	"Complex.h"
#include	"qdlib/dd_real.h"

#define		FALSE	0
#define		TRUE	1
#define		zerotol 1.e-50
#define		DBL_MIN 2.2250738585072014e-308 // min positive value
#define		DBL_MAX 1.7976931348623158e+308 // max value

class DDComplex
    {
    public:
	DDComplex(void)	{ }
	DDComplex(const dd_real & real, const dd_real & imaginary)
	    {
	    x = real;
	    y = imaginary;
	    }

	DDComplex(const DDComplex & Cmplx1)// Copy Constructor
	    {
	    x = Cmplx1.x;
	    y = Cmplx1.y;
	    }

	DDComplex(const double & value)
	    {
	    x = value;
	    y = 0.0;
	    }
		
	~DDComplex(void);

	DDComplex operator =(const DDComplex &);// Assignment Operator
	DDComplex operator =(const double &);	// Assignment to a double Operator
	DDComplex operator+=(const DDComplex &);
	DDComplex operator+=(double&);
	DDComplex operator-=(const DDComplex &);
	DDComplex operator-=(double &);
	DDComplex operator*=(const DDComplex &);
	DDComplex operator*=(double &);
	bool	  operator==(DDComplex &);
	DDComplex operator^(double &);
	DDComplex operator^(dd_real &);
	DDComplex operator^(DDComplex &);
	DDComplex operator++(void);
	DDComplex operator +(const DDComplex &);// Addition Operator
	DDComplex operator +(const double &);	// complex add by double Operator
	DDComplex operator +(const dd_real &);	// complex add by double double Operator
	DDComplex operator -(const DDComplex &);// Subtraction Operator
	DDComplex operator -(const double &);	// complex subtract by double Operator
	DDComplex operator -(const dd_real &);	// complex subtract by double Operator
	DDComplex operator -(void);		// unary minus
	DDComplex operator *(const DDComplex &);// Multiplication Operator
	DDComplex operator *(const double &);	// complex multiply by double Operator
	DDComplex operator *(const dd_real &);	// complex multiply by double double Operator
	DDComplex operator /(const DDComplex &);// Division Operator
	DDComplex operator /(const double &);	// complex divide by double Operator
	DDComplex operator /(const dd_real &);	// complex divide by double double Operator
	inline friend DDComplex operator*(double real, DDComplex num)
		{return DDComplex(num.x*real, num.y*real);}
	inline friend DDComplex operator+(double real, DDComplex num)
		{return DDComplex(num.x+real, num.y);}
	inline friend DDComplex operator-(double real, DDComplex num)
		{return DDComplex(real-num.x, -num.y);}


	DDComplex	CPolynomial(int);	// take a complex number to an integer power
	DDComplex	CInvert();		// invert
	DDComplex	CCube();		// cube
	DDComplex	CExp(void);		// exponent
	DDComplex	CSin(void);		// sine of a complex number
	DDComplex	CSqr(void);		// square
	DDComplex	CCos(void);		// cosine
	DDComplex	CLog();			// log
	dd_real		CFabs();		// abs
	DDComplex	CSqrt();		// square root
	DDComplex	CSinh();		// hyperbolic sine of a complex number
	DDComplex	CFlip();		// swap real and imaginary
	DDComplex	CTan();			// tangent
	DDComplex	CCosh();		// hyperbolic cosine
	DDComplex	CTanh();		// hyperbolic tangent
	void	CPower(DDComplex  & result, DDComplex  & base, int exp);
	double	CSumSqr();		// real squared + imaginary squared

	dd_real	x, y;
    };

//extern	Complex	CSin(Complex  &);		// sine of a complex number
//extern	Complex	CCos(Complex  &);		// cosine
//extern	Complex	CTan(Complex  &);		// tangent
//extern	Complex	CSinh(Complex  &);		// hyperbolic sine of a complex number
//extern	Complex	CCosh(Complex  &);		// hyperbolic cosine
//extern	Complex	CTanh(Complex  &);		// hyperbolic tangent
//extern	Complex	CSqr(Complex  &);		// square
//extern	Complex	CSqrt(Complex  &);		// square root
//extern	Complex	CCube(Complex  &);		// cube
//extern	double	CSumSqr(Complex  &);		// real squared + imaginary squared
//extern	double	CFabs(Complex  &);		// abs
//extern	Complex	CPolynomial(Complex  &, int);	// take a complex number to an integer power
//extern	Complex	CInvert(Complex  &);		// invert
//extern	Complex	CExp(Complex  &);		// exponent
//extern	Complex	CLog(Complex  &);		// log
//extern	Complex	CFlip(Complex  &);		// swap real and imaginary
//extern	Complex	CComplexPower(Complex  &, Complex &);	// take a complex number to a complex power
// now replaced with operator z^x where z and x are both complex.


