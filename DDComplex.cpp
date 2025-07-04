// Complex.cpp: interface for the Complex class.
//
//////////////////////////////////////////////////////////////////////
#include "Complex.h"
#include "DDComplex.h"

//Complex::Complex(void)
//    {
//    }

DDComplex::~DDComplex(void)
    {
    }

DDComplex DDComplex::operator=(const double & value)	// Assignment to double Operator
    {
    x = value;
    y = 0.0;
    return *this;
    }

DDComplex DDComplex::operator=(const Complex & value)	// Assignment to Complex Operator
    {
    x = value.x;
    y = value.y;
    return *this;
    }

DDComplex DDComplex::operator=(const DDComplex & Cmplx1)	// Assignment to DDComplex Operator
    {
    x = Cmplx1.x;
    y = Cmplx1.y;
    return *this;
    }

DDComplex DDComplex::operator+=(const DDComplex & Cmplx1)
    {
    x+=Cmplx1.x;
    y+=Cmplx1.y;
    return *this;
    }

DDComplex DDComplex::operator+=(double & rvalue)
    {
    x+=rvalue;
    return *this;
    }

DDComplex DDComplex::operator-=(const DDComplex & Cmplx1)
    {
    x-=Cmplx1.x;
    y-=Cmplx1.y;
    return *this;
    }

DDComplex DDComplex::operator-=(double & rvalue)
    {
    x-=rvalue;
    return *this;
    }

DDComplex DDComplex::operator*=(const DDComplex & Cmplx1)
    {
    dd_real  t;

    t = Cmplx1.x*x-Cmplx1.y*y; 
    y = Cmplx1.x*y+Cmplx1.y*x;
    x = t;
    return *this;
    }

DDComplex DDComplex::operator*=(double & rvalue)
    {
    x*=rvalue;
    y*=rvalue;
    return *this;
    }

bool DDComplex::operator==(DDComplex & rvalue)
    {
    return (x==rvalue.x && y==rvalue.y);
    }

DDComplex DDComplex::operator^(dd_real & expon)
    {
    DDComplex	temp, temp1;

    temp.x = x;
    temp.y = y;
    temp1 = temp.CLog()*expon;
    temp = temp1.CExp();
    return temp;			
    }

DDComplex DDComplex::operator^(double & expon)
    {
    DDComplex	temp, temp1;

    temp.x = x;
    temp.y = y;
    temp1 = temp.CLog()*expon;
    temp = temp1.CExp();
    return temp;
    }

DDComplex DDComplex::operator^(DDComplex & expon)
    {
    DDComplex	temp, temp1;

    temp.x = x;
    temp.y = y;
    temp1 = temp.CLog()*expon;
    temp = temp1.CExp();
    return temp;
    }

DDComplex DDComplex::operator++(void)
    {
    x+=1.0;
    return *this;
    }

DDComplex DDComplex::operator+(const DDComplex & Cmplx1)	// complex add Operator
    {
    DDComplex	temp;

    temp.x = x + Cmplx1.x;
    temp.y = y + Cmplx1.y;
    return  temp;
    }

DDComplex DDComplex::operator+(const double & Sum)	// complex add double Operator
    {
    DDComplex	temp;

    temp.x = x + Sum;
    temp.y = y;
    return  temp;
    }

DDComplex DDComplex::operator+(const dd_real & Sum)	// complex add double double Operator
    {
    DDComplex	temp;

    temp.x = x + Sum;
    temp.y = y;
    return  temp;
    }

DDComplex DDComplex::operator-(const DDComplex & Cmplx1)	// complex subtract Operator
    {
    DDComplex	temp;

    temp.x = x - Cmplx1.x;
    temp.y = y - Cmplx1.y;
    return  temp;
    }

DDComplex DDComplex::operator-(const double & Difference)	// complex subtract double Operator
    {
    DDComplex	temp;

    temp.x = x - Difference;
    temp.y = y;
    return  temp;
    }

DDComplex DDComplex::operator-(const dd_real & Difference)	// complex subtract double double Operator
    {
    DDComplex	temp;

    temp.x = x - Difference;
    temp.y = y;
    return  temp;
    }

DDComplex DDComplex::operator-(void)		    // unary minus
    {
    DDComplex	temp;

    temp.x = -x;
    temp.y = -y;
    return  temp;
    }

DDComplex DDComplex::operator*(const DDComplex & Cmplx1)	// complex multiply Operator
    {
    DDComplex	temp;

    temp.x = x * Cmplx1.x - y * Cmplx1.y;
    temp.y = y * Cmplx1.x + x * Cmplx1.y;
    return  temp;
    }

DDComplex DDComplex::operator*(const dd_real & Multiplier)		// complex multiply by double double Operator
    {
    DDComplex	temp;

    temp.x = x * Multiplier;
    temp.y = y * Multiplier;
    return  temp;
    }

DDComplex DDComplex::operator*(const double & Multiplier)	// complex multiply by double Operator
    {
    DDComplex	temp;

    temp.x = x * Multiplier;
    temp.y = y * Multiplier;
    return  temp;
    }

DDComplex DDComplex::operator/(const DDComplex & Cmplx1)	// complex divide Operator
    {
    dd_real	d;
    DDComplex	temp;

    if (Cmplx1.x == 0.0 && Cmplx1.y == 0.0)
	d = zerotol;					// prevent divide by zero
    else
	d = Cmplx1.x * Cmplx1.x + Cmplx1.y * Cmplx1.y;
    temp.x = (x * Cmplx1.x + y * Cmplx1.y) / d;
    temp.y = (y * Cmplx1.x - x * Cmplx1.y) / d;
    return  temp;
    }

DDComplex DDComplex::operator/(const double & divisor)	// complex divide by double Operator
    {
    DDComplex	temp;
    double  d;

    if (divisor == 0.0)
	d = zerotol;					// prevent divide by zero
    else
	d = divisor;
    temp.x = x / d;
    temp.y = y / d;
    return  temp;
    }

DDComplex DDComplex::operator/(const dd_real & divisor)	// complex divide by double double Operator
    {
    DDComplex	temp;
    dd_real  d;

    if (divisor == 0.0)
	d = zerotol;					// prevent divide by zero
    else
	d = divisor;
    temp.x = x / d;
    temp.y = y / d;
    return  temp;
    }

/**************************************************************************
	sumsqr(x+iy)  = x*x+y*y
***************************************************************************/

double	DDComplex::CSumSqr(void)
    {
    dd_real t;
    double  sumsqr;
    t = (x * x) + (y * y);
    sumsqr = to_double(t);
    return (sumsqr);
    }

/**************************************************************************
	sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/

DDComplex	DDComplex::CSin()
    {
    DDComplex a;
    dd_real	t1, t2, t3, t4;

    sincos(x, t1, t3);
    sincosh(y, t4, t2);
    a.x = t1 * t2;
    a.y = t3 * t4;
    return a;
    }

/**************************************************************************
	cos(x+iy)  = cos(x)cosh(y) - isin(x)sinh(y)
***************************************************************************/

DDComplex	DDComplex::CCos()
    {
    DDComplex a;
    dd_real	t1, t2, t3, t4;
    sincos(x, t3, t1);
    sincosh(y, t4, t2);
    a.x = t1 * t2;
    a.y = -t3 * t4;
    return a;
    }

/**************************************************************************
	tan(x+iy)  = (tan(x) -i tanh(-y)) / (1 +i tanh(-y) tan(x))
***************************************************************************/

DDComplex	DDComplex::CTan(void)
    {
    DDComplex a;
    dd_real  denom;

    x *= 2;
    y *= 2;
    denom = cos(x) + cosh(y);
    if (fabs(denom) <= DBL_MIN) 
	a = DBL_MAX;
    else
	{
	a.x = sin(x) / denom;
	a.y = sinh(y) / denom;
	}
    return a;
    }

/**************************************************************************
	sinh(x+iy)  = sinh(x)cos(y) + icosh(x)sin(y)
***************************************************************************/

DDComplex	DDComplex::CSinh(void)
    {
    DDComplex a;
    dd_real	t1, t2, t3, t4;

    sincos(y, t1, t3);
    sincosh(x, t4, t2);
    a.x = t4 * t3;
    a.y = t2 * t1;
    return a;
    }

/**************************************************************************
	cosh(x+iy)  = cosh(x)cos(y) - isinh(x)sin(y)
***************************************************************************/

DDComplex	DDComplex::CCosh(void)
    {
    DDComplex a;
    dd_real	t1, t2, t3, t4;

    sincos(y, t1, t3);
    sincosh(x, t4, t2);
    a.x = t2 * t3;
    a.y = -t4 * t1;
    a.x = cosh(x) * cos(y);
    a.y = -sinh(x) * sin(y);
    return a;
    }

/**************************************************************************
	tanh(x+iy)  = (tanh(x) + i tan(y)) / (1 +i tanh(x) tan(y))
***************************************************************************/

DDComplex	DDComplex::CTanh(void)
    {
    DDComplex a;
    dd_real  denom;

    x *= 2;
    y *= 2;

    denom = cosh(x) + cos(y);
    if (fabs(denom) <= DBL_MIN) 
	a = DBL_MAX;
    else
	{
	a.x = sinh(x) / denom;
	a.y = sin(y) / denom;
	}
    return a;
    }

/**************************************************************************
	Complex Exponent: e^(x+iy) = (e^x) * cos(y) + i * (e^x) * sin(y)
***************************************************************************/

DDComplex	DDComplex::CExp()
    {
    DDComplex a;
    dd_real	t1, t2, t3, t4;

    sincos(y, t1, t2);
    t3 = exp(x);
    a.x = t2 * t3;
    a.y = t1 * t3;
    return a;
    }

/**************************************************************************
	sqr(x+iy)  = x*x-y*y + i(x*y)*2
***************************************************************************/

DDComplex	DDComplex::CSqr()
    {
    DDComplex a;
    dd_real  temp;

    a.x = (x * x) - (y * y);
    temp = x * y;
    a.y = temp + temp;
    return a;
    }

/**************************************************************************
	sqrt(x+iy)  =  

DeMoivre’s Theorem states that if n is any positive real number, then
(a + bi)^n = r^n(cos nθ + i sin nθ).

In particular, if n = ½, we have
√a + bi = √r [cos θ/2 + i sin θ/2]

If we apply the half-angle formula:
cos (θ/2) = ± √[(1 + cos θ) / 2]
and
sin (θ/2) = ± √[(1 - cos θ) / 2]

This gives us a straightforward way to calculate √(a + bi)

√(a + bi) = √r [√[(1 + cos θ) / 2] ± i√[(1 - cos θ) / 2]]
√(a + bi) = √r [√[(1 + a/r)/ 2] ± i√[(1 - a/r) / 2]]

Simplifying you get:
√(a + bi) = √[(r + a) / 2] ± i √[(r - a) / 2]
***************************************************************************/

DDComplex	DDComplex::CSqrt(void)
    {
    DDComplex a;

    a.x = sqrt(sqrt(x*x+y*y)) * cos(atan2(y,x)/2);
    a.y = sqrt(sqrt(x*x+y*y)) * sin(atan2(y,x)/2);
    return a;
    }

/**************************************************************************
	Abs(x+iy)  = sqrt(x*x+y*y)
***************************************************************************/

dd_real	DDComplex::CFabs(void)
    {
    DDComplex temp;

    temp = (DDComplex(x, y));
    return sqrt(temp.CSumSqr());
    }

/**************************************************************************
	CFlip(x+iy)  = (y+ix) i.e. swap real and imaginary
***************************************************************************/

DDComplex	DDComplex::CFlip(void)
    {
    return DDComplex(y, x);
    }

/**************************************************************************
	Complex Power Function - int exponent
**************************************************************************/

void	DDComplex::CPower(DDComplex  & result, DDComplex  & base, int exp)

    {
    dd_real	xt, yt, t2;

    xt = base.x;   yt = base.y;

    if (exp & 1)
	{
	result.x = xt;
	result.y = yt;
	}
    else
	{
	result.x = 1.0;
	result.y = 0.0;
	}

    exp >>= 1;
    while (exp)
	{
	t2 = (xt + yt) * (xt - yt);
	yt = xt * yt;
	yt = yt + yt;
	xt = t2;

	if (exp & 1)
	    {
	    t2 = xt * result.x - yt * result.y;
	    result.y = result.y * xt + yt * result.x;
	    result.x = t2;
	    }
	exp >>= 1;
	}
    }

/**************************************************************************
	Complex Power Function - Complex exponent
**************************************************************************/

DDComplex	DDComplexPower(DDComplex x, DDComplex y)
    {
    DDComplex z;
    z = x ^ y;
    return z;
    }

/**************************************************************************
	Evaluate a Complex Polynomial
**************************************************************************/
                         
DDComplex	DDComplex::CPolynomial(int degree)

    {
    DDComplex	temp, temp1;

    temp1.x = x;
    temp1.y = y;
    if (degree < 0)
	degree = 0;
    CPower(temp, temp1, degree);
    return  temp;
    }

/**************************************************************************
	invert c + jd = (1 + j0) / (a + jb)
***************************************************************************/

DDComplex	DDComplex::CInvert(void)

    {
    dd_real	d;
    DDComplex	temp;

    if (x == 0.0 && y == 0.0)
	d = zerotol;						// prevent divide by zero
    else
	d = x * x + y * y;
    temp.x = x / d;
    temp.y = - y / d;
    return  temp;
    }

/**************************************************************************
	Cube c + jd = (a + jb) * (a + jb) * (a + jb) 
***************************************************************************/

DDComplex	DDComplex::CCube()

    {
    DDComplex	temp;
    dd_real	sqr_real, sqr_imag;

    sqr_real = x * x;
    sqr_imag = y * y;
    temp.x = x * (sqr_real - (sqr_imag + sqr_imag + sqr_imag));
    temp.y = y * ((sqr_real + sqr_real + sqr_real) - sqr_imag);
    return  temp;
    }

/**************************************************************************
	Evaluate a Complex Log: ln(x+i y) = 0.5*ln(x²+y²) + i atan(x/y) 
**************************************************************************/
                         
DDComplex	DDComplex::CLog()

    {
    DDComplex a, t;
    dd_real  mod,zx,zy;

    t.x = x;
    t.y = y;
    mod = t.CSumSqr();
    mod = sqrt(mod);
    zx = log(mod);
    zy = atan2(y,x);

    a.x = zx;
    a.y = zy;
    return a;
    }

#ifdef ALLOWDDTRIG
/**************************************************************************
	Evaluate a Complex Polynomial with a complex power
**************************************************************************/

// now replaced with operator z^x where z and x are both complex.

//void SinCos(double *Angle, double *Sin, double *Cos)
//    {
//    *Sin = sin(*Angle);
//    *Cos = cos(*Angle);
//    }

//Complex	CComplexPower(Complex  & Cmplx1, Complex & power)
//
//    {
//    static  Complex z;
//	    Complex t;
//	    double  e2x, siny, cosy;
//	    double  e2x;

//    t = 0;
//    t = power*CLog(Cmplx1);
//    e2x = exp(t.x);
//    SinCos(&t.y, &siny, &cosy);
//    z.x = e2x * cos(t.y);
//    z.y = e2x * sin(t.y);
//    return(z);
//    }

#endif // ALLOWDDTRIG
