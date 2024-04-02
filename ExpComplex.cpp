// BigComplex.cpp: interface for the Complex class.
//
//////////////////////////////////////////////////////////////////////
#include "ExpComplex.h"

//Complex::Complex(void)
//    {
//    }

//extern	void	ShowBignum(BigDouble, char *);			// diagnostic tool only

ExpComplex::~ExpComplex(void)
    {
//    mpfr_clear(x.x);
//    mpfr_clear(y.x);
    }

/*
ExpComplex * ExpComplex::operator =(const BigComplex & Cmplx1)	// Assignment to a BigComplex Operator
    {
    double	value;
    long	exp;
    BigDouble	t;

    exp = mpfr_get_exp(Cmplx1.x.x);
    mpfr_set(t.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_set_exp(t.x, (mpfr_exp_t)0);
    value = mpfr_get_d(t.x, MPFR_RNDN);
    x = value;
    x.exp += exp;

    exp = mpfr_get_exp(Cmplx1.y.x);
    mpfr_set(t.x, Cmplx1.y.x, MPFR_RNDN);
    mpfr_set_exp(t.x, (mpfr_exp_t)0);
    value = mpfr_get_d(t.x, MPFR_RNDN);
    y = value;
    y.exp += exp;

    return this;
    }
*/
ExpComplex * ExpComplex::operator=(const double & value)	// Assignment to double Operator
    {
    x = value;
    y = 0.0;
    return this;
    }

ExpComplex * ExpComplex::operator=(const Complex & value)	// Assignment to Complex Operator
    {
    x = value.x;
    y = value.y;
    return this;
    }

ExpComplex * ExpComplex::operator=(const floatexp & value)	// Assignment to double Operator
    {
    x = value;
    y = 0.0;
    return this;
    }

ExpComplex * ExpComplex::operator=(const ExpComplex & Cmplx1)	// Assignment Operator
    {
    x = Cmplx1.x;
    y = Cmplx1.y;
    return this;
    }

// new stuff
ExpComplex * ExpComplex::operator+=(const ExpComplex & Cmplx1)
    {
    x+=Cmplx1.x;
    y+=Cmplx1.y;
    return this;
    }

ExpComplex * ExpComplex::operator+=(floatexp & rvalue)
    {
    x+=rvalue;
    return this;
    }

ExpComplex * ExpComplex::operator-=(const ExpComplex & Cmplx1)
    {
    x-=Cmplx1.x;
    y-=Cmplx1.y;
    return this;
    }

ExpComplex * ExpComplex::operator-=(floatexp & rvalue)
    {
    x-=rvalue;
    return this;
    }

ExpComplex * ExpComplex::operator*=(const ExpComplex & Cmplx1)
    {
    floatexp	t;
    ExpComplex	a;

    a = Cmplx1;
    t = a.x * x - a.y * y;
    y = a.x * y + a.y * x;
    x = t;
    return this;
    }

ExpComplex * ExpComplex::operator*=(floatexp & rvalue)
    {
    x = x * rvalue;
    y = y * rvalue;
    return this;
    }

ExpComplex * ExpComplex::operator++(void)
    {
    x+=1;
    return this;
    }

ExpComplex ExpComplex::operator+(const ExpComplex & Cmplx1)	// complex add Operator
    {
    ExpComplex	temp;

    temp.x = x + Cmplx1.x;
    temp.y = y + Cmplx1.y;
    return  temp;
    }

ExpComplex ExpComplex::operator+(const floatexp & Sum)	// complex add double Operator
    {
    ExpComplex	temp;

    temp.x = x + Sum;
    temp.y = y;
    return  temp;
    }

ExpComplex ExpComplex::operator-(const ExpComplex & Cmplx1)	// complex subtract Operator
    {
    ExpComplex	temp;

    temp.x = x - Cmplx1.x;
    temp.y = y - Cmplx1.y;
    return  temp;
    }

ExpComplex ExpComplex::operator-(const floatexp & Difference)	// complex subtract double Operator
    {
    ExpComplex	temp;

    temp.x = x - Difference;
    temp.y = y;
    return  temp;
    }

ExpComplex ExpComplex::operator-(void)		    // unary minus
    {
    ExpComplex	temp;

    temp.x = -x;
    temp.y = -y;
    return  temp;
    }

ExpComplex ExpComplex::operator*(const ExpComplex & Cmplx1)	// complex multiply Operator
    {
    ExpComplex	temp;

    temp.x = x * Cmplx1.x - y * Cmplx1.y;
    temp.y = y * Cmplx1.x + x * Cmplx1.y;
    return  temp;
    }

ExpComplex ExpComplex::operator*(const floatexp & Multiplier)	// complex multiply by double Operator
    {
    ExpComplex	temp;

    temp.x = x * Multiplier;
    temp.y = y * Multiplier;
    return  temp;
    }

bool ExpComplex::operator==(ExpComplex & rvalue)
    {
    return (x==rvalue.x && y==rvalue.y);
    }

/*

BigComplex BigComplex::operator^(BigDouble & expon)
    {
    BigComplex	temp;
    
    temp.x = x;
    temp.y = y;
    temp = CExp(CLog(temp)*expon);
    return temp;			
    }

BigComplex BigComplex::operator^(BigComplex & expon)
    {
    BigComplex	temp;

    temp.x = x;
    temp.y = y;
    temp = CExp(CLog(temp)*expon);
    return temp;			
    }

BigComplex BigComplex::operator++(void)
    {
    x+=1;
    return *this;
    }
// end new stuff
*/

ExpComplex ExpComplex::operator/(const ExpComplex & Cmplx1)	// complex divide Operator
    {
    ExpComplex	temp;
    floatexp	a, b, d;

    temp = Cmplx1;
    if (temp.x == temp.x && temp.y == 0.0)
	d = zerotol;					// prevent divide by zero
    else
	d = temp.x * temp.x + temp.y * temp.y;
    temp.x = (x * Cmplx1.x + y * Cmplx1.y) / d;
    temp.y = (y * Cmplx1.x - x * Cmplx1.y) / d;
    return  temp;
    }

ExpComplex ExpComplex::operator/(const floatexp & divisor)	// complex divide by double Operator
    {
    ExpComplex	temp;
    floatexp  a, d;

    a = divisor;
    if (a == 0.0)
	d = zerotol;					// prevent divide by zero
    else
	d = divisor;
    temp.x = x / d;
    temp.y = y / d;
    return  temp;
    }

/**************************************************************************
	Cube c + jd = (a + jb) * (a + jb) * (a + jb) 
***************************************************************************/

ExpComplex	ExpComplex::CCube()

    {
    ExpComplex	temp;
    floatexp	sqr_real, sqr_imag;

    sqr_real = x * x;
    sqr_imag = y * y;
    temp.x = x * (sqr_real - (sqr_imag + sqr_imag + sqr_imag));
    temp.y = y * ((sqr_real + sqr_real + sqr_real) - sqr_imag);
    return  temp;
    }


/**************************************************************************
	sumsqr(x+iy)  = x*x+y*y
***************************************************************************/

double	ExpComplex::CSumSqr()
    {
    floatexp	sqr_real, sqr_imag, temp;
    double	out;

    sqr_real = x * x;
    sqr_imag = y * y;
    temp = sqr_real + sqr_imag;
    out = temp.todouble();
    return (out);
    }

/**************************************************************************
	sqr(x+iy)  = x*x-y*y + i(x*y)*2
***************************************************************************/

ExpComplex	ExpComplex::CSqr()
    {
    ExpComplex	a;
    floatexp  temp;

    a.x = (x * x) - (y * y);
    temp = x * y;
    a.y = temp.X2();
    return a;
    }

#ifdef FUTURE
/**************************************************************************
	r = 2*n
***************************************************************************/

BigComplex	BigComplex::CDouble()
    {
    BigComplex a;
    mpfr_exp_t	exp;

    exp = mpfr_get_exp(x.x);
    mpfr_set(a.x.x, x.x, MPFR_RNDN);
    mpfr_set_exp(a.x.x, exp + 1);
    exp = mpfr_get_exp(y.x);
    mpfr_set(a.y.x, y.x, MPFR_RNDN);
    mpfr_set_exp(a.y.x, exp + 1);
    //    mpfr_add(a.x.x, x.x, x.x, MPFR_RNDN);
    //    mpfr_add(a.y.x, y.x, y.x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	r = n/2
***************************************************************************/

BigComplex	BigComplex::CHalf()
    {
    BigComplex a;
    mpfr_exp_t	exp;

    exp = mpfr_get_exp(x.x);
    mpfr_set(a.x.x, x.x, MPFR_RNDN);
    mpfr_set_exp(a.x.x, exp + 1);
    exp = mpfr_get_exp(y.x);
    mpfr_set(a.y.x, y.x, MPFR_RNDN);
    mpfr_set_exp(a.y.x, exp - 1);
    //    mpfr_div_ui(a.x.x, x.x, 2, MPFR_RNDN);
    //    mpfr_div_ui(a.y.x, y.x, 2, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	invert c + jd = (1 + j0) / (a + jb)
***************************************************************************/

BigComplex	BigComplex::CInvert(void)

    {
    BigComplex	temp;
    BigDouble	t, t1, t2, d;

    if (mpfr_sgn(x.x) == 0 && mpfr_sgn(y.x) == 0)
	mpfr_set_d(d.x, zerotol, MPFR_RNDN);					// prevent divide by zero
    else
	{
	mpfr_mul(t1.x, x.x, x.x, MPFR_RNDN);
	mpfr_mul(t2.x, y.x, y.x, MPFR_RNDN);
	mpfr_add(d.x, t1.x, t2.x, MPFR_RNDN);					// divisor
	}

    mpfr_div(temp.x.x, x.x, d.x, MPFR_RNDN);					// c = a/temp
    mpfr_div(t1.x, y.x, d.x, MPFR_RNDN);
    mpfr_neg(temp.y.x, t1.x, MPFR_RNDN);						// d = -b/temp

    return  temp;
    }

/**************************************************************************
	Evaluate a Complex Polynomial
**************************************************************************/

BigComplex	BigComplex::CPolynomial(int degree)

    {
    BigComplex	temp;
    BigDouble	t, t1, t2, t3, t4;

    if (degree < 0)
	degree = 0;

    mpfr_set(t1.x, x.x, MPFR_RNDN);	  		// BigTemp1 = xt  
    mpfr_set(t2.x, y.x, MPFR_RNDN);			// BigTemp2 = yt

    if (degree & 1)
	{
	mpfr_set(temp.x.x, t1.x, MPFR_RNDN);	// new.x = result real  
	mpfr_set(temp.y.x, t2.x, MPFR_RNDN);	// new.y = result imag
	}
    else
	{
	mpfr_set_ui(temp.x.x, 1, MPFR_RNDN);
	mpfr_set_ui(temp.y.x, 0, MPFR_RNDN);
	}

    degree >>= 1;
    while (degree)
	{
	mpfr_sub(t.x, t1.x, t2.x, MPFR_RNDN);				// (xt - yt)
	mpfr_add(t3.x, t1.x, t2.x, MPFR_RNDN);				// (xt + yt)
	mpfr_mul(t4.x, t.x, t3.x, MPFR_RNDN);				// t2 = (xt + yt) * (xt - yt)
	mpfr_set(t.x, t2.x, MPFR_RNDN);
	mpfr_mul(t3.x, t.x, t1.x, MPFR_RNDN);				// yt = xt * yt
	mpfr_add(t2.x, t3.x, t3.x, MPFR_RNDN);				// yt = yt + yt
	mpfr_set(t1.x, t4.x, MPFR_RNDN);

	if (degree & 1)
	    {
	    mpfr_mul(t.x, t1.x, temp.x.x, MPFR_RNDN);			// xt * result->x
	    mpfr_mul(t3.x, t2.x, temp.y.x, MPFR_RNDN);			// yt * result->y
	    mpfr_sub(t4.x, t.x, t3.x, MPFR_RNDN);				// t2 = xt * result->x - yt * result->y
	    mpfr_mul(t.x, t1.x, temp.y.x, MPFR_RNDN);			// xt * result->y
	    mpfr_mul(t3.x, t2.x, temp.x.x, MPFR_RNDN);			// yt * result->x
	    mpfr_add(temp.y.x, t.x, t3.x, MPFR_RNDN);			// result->y = result->y * xt + yt * result->x
	    mpfr_set(temp.x.x, t4.x, MPFR_RNDN);	    			// result->x = t2
	    }
	degree >>= 1;
	}

    return  temp;
    }

/**************************************************************************
	Sinh and Cosh Functions
	cosh(x) = (exp(x) + exp(-x)) / 2.
	sinh(x) = (exp(x) - exp(-x)) / 2
**************************************************************************/

/*
static	bf_t sinhcosh_bf(bf_t s, bf_t c, bf_t n)

    {
    bf_t	temp1, temp2, temp3, temp4;

    temp1 = big_alloc(rbflength * 10);
    temp2 = big_alloc(rbflength * 10);
    temp3 = big_alloc(rbflength * 10);
    temp4 = big_alloc(rbflength * 10);

    abs_bf(temp1, n);
    exp_bf(temp2, temp1);				// a = exp(a);
    floattobf(temp3, 0.5);
    div_bf(temp4, temp3, temp2);			// 0.5 / a
    half_bf(temp3, temp2);				// 0.5 * a
    sub_bf(temp2, temp3, temp4);			// a = 0.5*a - (0.5/a)
    if (sign_bf(n) < 0)					// if (x < 0) 
	neg_bf(s, temp2);				// s = -a;
    else
	memcpy(s, temp2, bflength+2);			// s = a;
    unsafe_add_bf(c, temp3, temp4);			// c = 0.5*a + (0.5/a)
    if (temp1) big_free(temp1);
    if (temp2) big_free(temp2);
    if (temp3) big_free(temp3);
    if (temp4) big_free(temp4);
    return s;						// return sinh I guess
    }
*/




/**************************************************************************
	sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/

BigComplex	BigComplex::CSin()
    {
    BigComplex	a;
    BigDouble	t1, t2, t3, t4;

    mpfr_sin_cos(t1.x, t3.x, x.x, MPFR_RNDN);
    mpfr_sinh_cosh(t4.x, t2.x, y.x, MPFR_RNDN);
    mpfr_mul(a.x.x, t1.x, t2.x, MPFR_RNDN);
    mpfr_mul(a.y.x, t3.x, t4.x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	cos(x+iy)  = cos(x)cosh(y) - isin(x)sinh(y)
***************************************************************************/

BigComplex	BigComplex::CCos()
    {
    BigComplex a;
    BigDouble	t1, t2, t3, t4;

    mpfr_sin_cos(t3.x, t1.x, x.x, MPFR_RNDN);
    mpfr_sinh_cosh(t4.x, t2.x, y.x, MPFR_RNDN);
    mpfr_neg(t3.x, t3.x, MPFR_RNDN);
    mpfr_mul(a.x.x, t1.x, t2.x, MPFR_RNDN);
    mpfr_mul(a.y.x, t3.x, t4.x, MPFR_RNDN);

    return a;
    }

/**************************************************************************
	tan(x+iy)  = (tan(x) -i tanh(-y)) / (1 +i tanh(-y) tan(x))
***************************************************************************/

BigComplex	BigComplex::CTan(void)
    {
    BigComplex a;
    BigDouble  denom;

    x *= 2;
    y *= 2;
    denom = x.BigCos() + y.BigCosh();
    a.x = x.BigSin() / denom;
    a.y = y.BigSinh() / denom;
    return a;
    }

/**************************************************************************
	sinh(x+iy)  = sinh(x)cos(y) + icosh(x)sin(y)
***************************************************************************/

BigComplex	BigComplex::CSinh(void)
    {
    BigComplex a;

    a.x = x.BigSinh() * y.BigCos();
    a.y = x.BigCosh() * y.BigSin();
    return a;
    }

/**************************************************************************
	cosh(x+iy)  = cosh(x)cos(y) - isinh(x)sin(y)
***************************************************************************/

BigComplex	BigComplex::CCosh(void)
    {
    BigComplex a;

    a.x = x.BigCosh() * y.BigCos();
    a.y = -x.BigSinh() * y.BigSin();
    return a;
    }

/**************************************************************************
	tanh(x+iy)  = (tanh(x) + i tan(y)) / (1 +i tanh(x) tan(y))
***************************************************************************/

BigComplex	BigComplex::CTanh(void)
    {
    BigComplex a;
    BigDouble  denom;

    x *= 2;
    y *= 2;

    denom = x.BigCosh() + y.BigCos();
    a.x = x.BigSinh() / denom;
    a.y = y.BigSin() / denom;
    return a;
    }

/**************************************************************************
	Complex Exponent: e^(x+iy) = (e^x) * cos(y) + i * (e^x) * sin(y)
***************************************************************************/

BigComplex	BigComplex::CExp()
    {
    BigComplex	a;
    BigDouble	t1, t2, t3, t4;

    mpfr_sin_cos(t1.x, t2.x, y.x, MPFR_RNDN);	// BigTemp1 = sin(z), BigTemp2 = cos(z)
    mpfr_exp(t3.x, x.x, MPFR_RNDN);		// a = exp(a);
    mpfr_mul(a.x.x, t2.x, t3.x, MPFR_RNDN);	// a.x = t2*t3
    mpfr_mul(a.y.x, t1.x, t3.x, MPFR_RNDN);	// a.y = t1*t3

    return a;
    }

/**************************************************************************
	Abs(x+iy)  = sqrt(x*x+y*y)
***************************************************************************/

BigDouble	BigComplex::CFabs()
    {
    BigDouble	sqr_real, sqr_imag, u;
    BigDouble	out;

    mpfr_mul(sqr_real.x, x.x, x.x, MPFR_RNDN);			// sqr_real = x * x;
    mpfr_mul(sqr_imag.x, y.x, y.x, MPFR_RNDN);			// sqr_imag = y * y;
    mpfr_add(u.x, sqr_real.x, sqr_imag.x, MPFR_RNDN);
    mpfr_sqrt(out.x, u.x, MPFR_RNDN);

    return (out);
    }

/**************************************************************************
	sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/

/*
//extern	bf_t	sinhcosh_bf(bf_t, bf_t, bf_t);

BigComplex	CSin(BigComplex  & Cmplx1)
    {
    BigComplex	a;
    BigDouble	t1, t2, t3, t4;

    mpfr_sin_cos(t1.x, t3.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_sinh_cosh(t4.x, t2.x, Cmplx1.y.x, MPFR_RNDN);
    mpfr_mul(a.x.x, t1.x, t2.x, MPFR_RNDN);
    mpfr_mul(a.y.x, t3.x, t4.x, MPFR_RNDN);

    return a;
    }
*/

/**************************************************************************
	cos(x+iy)  = cos(x)cosh(y) - isin(x)sinh(y)
***************************************************************************/

/*
BigComplex	CCos(BigComplex  & Cmplx1)
    {
    BigComplex a;
    BigDouble	t1, t2, t3, t4;

    mpfr_sin_cos(t3.x, t1.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_sinh_cosh(t4.x, t2.x, Cmplx1.y.x, MPFR_RNDN);
    mpfr_neg(t3.x, t3.x, MPFR_RNDN);
    mpfr_mul(a.x.x, t1.x, t2.x, MPFR_RNDN);
    mpfr_mul(a.y.x, t3.x, t4.x, MPFR_RNDN);

    return a;
    }
*/

/**************************************************************************
	Complex Exponent: e^(x+iy) = (e^x) * cos(y) + i * (e^x) * sin(y)
***************************************************************************/

/*
BigComplex	CExp(BigComplex  & Cmplx1)
    {
    BigComplex a;
    BigDouble	t1, t2, t3, t4;

    mpfr_sin_cos(t1.x, t2.x, Cmplx1.y.x, MPFR_RNDN);	// BigTemp1 = sin(z), BigTemp2 = cos(z)
    mpfr_exp(t3.x, Cmplx1.x.x, MPFR_RNDN);		// a = exp(a);
    mpfr_mul(a.x.x, t2.x, t3.x, MPFR_RNDN);	// a.x = t2*t3
    mpfr_mul(a.y.x, t1.x, t3.x, MPFR_RNDN);	// a.y = t1*t3
    return a;
    }
*/

/**************************************************************************
	sqr(x+iy)  = x*x-y*y + i(x*y)*2
***************************************************************************/

/*
BigComplex	CSqr(BigComplex  & Cmplx1)
    {
    BigComplex a;

    a.x = (Cmplx1.x * Cmplx1.x) - (Cmplx1.y * Cmplx1.y);
    a.y = Cmplx1.x * Cmplx1.y * 2.0;
    return a;
    }
*/

/**************************************************************************
	sumsqr(x+iy)  = x*x+y*y
***************************************************************************/

/*
BigDouble	BigCSumSqr(BigComplex  & Cmplx1)
    {
    return (Cmplx1.x * Cmplx1.x) + (Cmplx1.y * Cmplx1.y);
    }

double	CSumSqr(BigComplex  & Cmplx1)
    {
    BigDouble	t = ((Cmplx1.x * Cmplx1.x) + (Cmplx1.y * Cmplx1.y));
    return ((double)bftofloat(t.x));
    }
*/

/**************************************************************************
	Evaluate a Complex Polynomial
**************************************************************************/

/*
extern	void	BigComplexPower(bf_t, bf_t, bf_t, bf_t, int);

BigComplex	CPolynomial(BigComplex  & Cmplx1, int degree)

    {
    BigComplex	temp;

    if (degree < 0)
	degree = 0;
    BigComplexPower(temp.x.x, temp.y.x, Cmplx1.x.x, Cmplx1.y.x, degree);
    return  temp;
    }
*/

/**************************************************************************
	invert c + jd = (1 + j0) / (a + jb)
***************************************************************************/

/*
BigComplex	CInvert(BigComplex  & Cmplx1)

    {
    BigDouble	d;
    BigComplex	temp;

    if (is_bf_zero(Cmplx1.x.x) && is_bf_zero(Cmplx1.y.x))
	d = zerotol;						// prevent divide by zero
    else
	d = Cmplx1.x * Cmplx1.x + Cmplx1.y * Cmplx1.y;
    temp.x = Cmplx1.x / d;
    temp.y = - Cmplx1.y / d;
    return  temp;
    }
*/

/**************************************************************************
	Cube c + jd = (a + jb) * (a + jb) * (a + jb) 
***************************************************************************/

/*
BigComplex	CCube(BigComplex  & Cmplx1)

    {
    BigComplex	temp;
    BigDouble	sqr_real, sqr_imag;

    sqr_real = Cmplx1.x * Cmplx1.x;
    sqr_imag = Cmplx1.y * Cmplx1.y;
    temp.x = Cmplx1.x * (sqr_real - (sqr_imag + sqr_imag + sqr_imag));
    temp.y = Cmplx1.y * ((sqr_real + sqr_real + sqr_real) - sqr_imag);
    return  temp;
    }
*/

//#ifdef	ALLOWBIGCOMPLEXFUNCTIONS
/**************************************************************************
	Evaluate a Complex Log: ln(x+i y) = 0.5*ln(x²+y²) + i atan(x/y) 
**************************************************************************/
                         
BigComplex	BigComplex::CLog()

    {
    BigComplex a;

    mpfr_sqr(a.x.x,x.x, MPFR_RNDN);
    mpfr_sqr(a.y.x,y.x, MPFR_RNDN);
    mpfr_add(a.x.x, a.x.x, a.y.x, MPFR_RNDN);
    mpfr_log(a.x.x,a.x.x, MPFR_RNDN);
    mpfr_div_si(a.x.x, a.x.x, 2, MPFR_RNDN);
    mpfr_atan2(a.y.x,y.x,x.x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	Evaluate a Complex Polynomial with a complex power
**************************************************************************/

BigComplex	BigComplex::BigComplexPower(BigComplex & power)

    {
    BigComplex	tmp, t;

    if (x == (BigDouble)0.0 && y == (BigDouble)0.0)
	{
	t = 0.0;
	return (t);
	}
    tmp = CLog() * power;
    t = tmp.CExp();

    return(t);
    }

/**************************************************************************
    sqrt(x+iy)  =

DeMoivre’s Theorem states that if n is any positive real number, then
(a + bi)^n = r^n(cos n? + i sin n?).

In particular, if n = ½, we have
?a + bi = ?r [cos ?/2 + i sin ?/2]

If we apply the half-angle formula:
cos (?/2) = ± ?[(1 + cos ?) / 2]
and
sin (?/2) = ± ?[(1 - cos ?) / 2]

This gives us a straightforward way to calculate ?(a + bi)

?(a + bi) = ?r [?[(1 + cos ?) / 2] ± i?[(1 - cos ?) / 2]]
?(a + bi) = ?r [?[(1 + a/r)/ 2] ± i?[(1 - a/r) / 2]]

Simplifying you get:
?(a + bi) = ?[(r + a) / 2] ± i ?[(r - a) / 2]

    a.x = sqrt(sqrt(x*x + y * y)) * cos(atan2(y, x) / 2);
    a.y = sqrt(sqrt(x*x + y * y)) * sin(atan2(y, x) / 2);

***************************************************************************/

BigComplex	BigComplex::CSqrt(void)
    {
    BigComplex	a;
    BigDouble	SqrRoot, Atan, Cos, Sin; 

    SqrRoot = x * x + y * y;
    SqrRoot = SqrRoot.BigSqrt();
    SqrRoot = SqrRoot.BigSqrt();
    mpfr_atan2(Atan.x, y.x, x.x, MPFR_RNDN);
    Atan = Atan / 2;
    mpfr_sin_cos(Sin.x, Cos.x, Atan.x, MPFR_RNDN);
    a.x = SqrRoot * Cos;
    a.y = SqrRoot * Sin;

//    a.x = sqrt(sqrt(x*x + y * y)) * cos(atan2(y, x) / 2);
//    a.y = sqrt(sqrt(x*x + y * y)) * sin(atan2(y, x) / 2);
    return a;
    }



#ifdef ALLOWNEWTRIG

/**************************************************************************
	Complex Exponent: e^(x+iy) = (e^x) * cos(y) + i * (e^x) * sin(y)
***************************************************************************/

void	BigComplex::MTCExp(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4)
    {
    mpfr_sin_cos(t1.x, t2.x, in.y.x, MPFR_RNDN);	// BigTemp1 = sin(z), BigTemp2 = cos(z)
    mpfr_exp(t3.x, in.x.x, MPFR_RNDN);			// a = exp(a);
    mpfr_mul(out->x.x, t2.x, t3.x, MPFR_RNDN);		// a.x = t2*t3
    mpfr_mul(out->y.x, t1.x, t3.x, MPFR_RNDN);		// a.y = t1*t3
    }

/**************************************************************************
	sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/

void	BigComplex::MTCSin(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4)
    {
    mpfr_sin_cos(t1.x, t3.x, in.x.x, MPFR_RNDN);
    mpfr_sinh_cosh(t4.x, t2.x, in.y.x, MPFR_RNDN);
    mpfr_mul(out->x.x, t1.x, t2.x, MPFR_RNDN);
    mpfr_mul(out->y.x, t3.x, t4.x, MPFR_RNDN);
    }

/**************************************************************************
	cos(x+iy)  = cos(x)cosh(y) - isin(x)sinh(y)
***************************************************************************/

void	BigComplex::MTCCos(BigComplex *out, BigComplex in, BigDouble t1, BigDouble t2, BigDouble t3, BigDouble t4)
    {
    mpfr_sin_cos(t3.x, t1.x, in.x.x, MPFR_RNDN);
    mpfr_sinh_cosh(t4.x, t2.x, in.y.x, MPFR_RNDN);
    mpfr_neg(t3.x, t3.x, MPFR_RNDN);
    mpfr_mul(out->x.x, t1.x, t2.x, MPFR_RNDN);
    mpfr_mul(out->y.x, t3.x, t4.x, MPFR_RNDN);
    }
#endif // ALLOWNEWTRIG
#endif // FUTURE

