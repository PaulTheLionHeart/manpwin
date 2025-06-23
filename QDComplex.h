// QDComplex.h: interface for the QDComplex class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include	<math.h>
#include	"Complex.h"
#include	"qdlib/qd_real.h"

#define		FALSE	0
#define		TRUE	1
#define		zerotol 1.e-50
#define		DBL_MIN 2.2250738585072014e-308 // min positive value
#define		DBL_MAX 1.7976931348623158e+308 // max value

class QDComplex
    {
    public:
	QDComplex(void)	{ }
	QDComplex(const dd_real & real, const dd_real & imaginary)
	    {
	    x = real;
	    y = imaginary;
	    }

	QDComplex(const QDComplex & Cmplx1)// Copy Constructor
	    {
	    x = Cmplx1.x;
	    y = Cmplx1.y;
	    }

	QDComplex(const double & value)
	    {
	    x = value;
	    y = 0.0;
	    }
		
	~QDComplex(void);

	QDComplex operator =(const QDComplex &);// Assignment Operator
	QDComplex operator =(const double &);	// Assignment to a double Operator
	QDComplex operator =(const Complex &);	// Assignment to a complex Operator
	QDComplex operator+=(const QDComplex &);
	QDComplex operator+=(double&);
	QDComplex operator-=(const QDComplex &);
	QDComplex operator-=(double &);
	QDComplex operator*=(const QDComplex &);
	QDComplex operator*=(double &);
	bool	  operator==(QDComplex &);
	QDComplex operator^(double &);
	QDComplex operator^(qd_real &);
	QDComplex operator^(QDComplex &);
	QDComplex operator++(void);
	QDComplex operator +(const QDComplex &);// Addition Operator
	QDComplex operator +(const qd_real &);	// complex add by quad double Operator
	QDComplex operator +(const double &);	// complex add by double Operator
	QDComplex operator -(const QDComplex &);// Subtraction Operator
	QDComplex operator -(const double &);	// complex subtract by double Operator
	QDComplex operator -(const qd_real &);	// complex subtract by quad double Operator
	QDComplex operator -(void);		// unary minus
	QDComplex operator *(const QDComplex &);// Multiplication Operator
	QDComplex operator *(const double &);	// complex multiply by double Operator
	QDComplex operator *(const qd_real &);	// complex multiply by quad double Operator
	QDComplex operator /(const QDComplex &);// Division Operator
	QDComplex operator /(const double &);	// complex divide by double Operator
	QDComplex operator /(const qd_real &);	// complex divide by quad double Operator
/*
	inline friend QDComplex operator*(double real, QDComplex num)
		{return QDComplex(num.x*real, num.y*real);}
	inline friend QDComplex operator+(double real, QDComplex num)
		{return QDComplex(num.x+real, num.y);}
	inline friend QDComplex operator-(double real, QDComplex num)
		{return QDComplex(real-num.x, -num.y);}
*/
	QDComplex	CPolynomial(int);	// take a complex number to an integer power
	QDComplex	CInvert();		// invert
	QDComplex	CCube();		// cube
	QDComplex	CExp(void);		// exponent
	QDComplex	CSin(void);		// sine of a complex number
	QDComplex	CSqr();			// square
	QDComplex	CSqrt();		// square root
	QDComplex	CCos();			// cosine
	QDComplex	CLog();			// log
	QDComplex	CFlip();		// swap real and imaginary
	QDComplex	CTan();			// tangent
	QDComplex	CSinh();		// hyperbolic sine of a complex number
	QDComplex	CCosh();		// hyperbolic cosine
	QDComplex	CTanh();		// hyperbolic tangent
	qd_real		CFabs();		// abs
	void		CPower(QDComplex  & result, QDComplex  & base, int exp);
	QDComplex	QDComplexPower(QDComplex x, QDComplex y);
	double		CSumSqr();		// real squared + imaginary squared

	qd_real	x, y;
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


