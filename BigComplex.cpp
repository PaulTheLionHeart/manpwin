// BigComplex.cpp: interface for the Complex class.
//
//////////////////////////////////////////////////////////////////////
#include "BigComplex.h"
#include "Complex.h"

//Complex::Complex(void)
//    {
//    }

//extern	void	ShowBignum(BigDouble, char *);			// diagnostic tool only

BigComplex::~BigComplex(void)
    {
//    mpfr_clear(x.x);
//    mpfr_clear(y.x);
    }

BigComplex * BigComplex::operator=(const double & value)	// Assignment to double Operator
    {
    mpfr_set_d(this->x.x, value, MPFR_RNDN);
    mpfr_set_d(this->y.x, 0.0, MPFR_RNDN);
    return this;
    }

BigComplex * BigComplex::operator=(const Complex & value)	// Assignment to Complex Operator
    {
    mpfr_set_d(this->x.x, value.x, MPFR_RNDN);
    mpfr_set_d(this->y.x, value.y, MPFR_RNDN);
    return this;
    }

BigComplex * BigComplex::operator=(const BigDouble & value)	// Assignment to big double Operator
    {
    mpfr_set(this->x.x, value.x, MPFR_RNDN);
    mpfr_set_d(this->y.x, 0.0, MPFR_RNDN);
    return this;
    }

BigComplex * BigComplex::operator=(const BigComplex & Cmplx1)	// Assignment Operator
    {
    mpfr_set(this->x.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_set(this->y.x, Cmplx1.y.x, MPFR_RNDN);
    return this;
    }

// new stuff
BigComplex * BigComplex::operator =(const ExpComplex & Cmplx1)	// complex add by ExpComplex Operator
    {
    BigComplex	temp;
    double	value;
    __int64	exp;
    mpfr_exp_t	exp1;
    floatexp	t;

    exp = Cmplx1.x.exp;
    t = Cmplx1.x;
    t.exp = 0;
    value = t.val;
    temp.x = value;
    exp1 = mpfr_get_exp(temp.x.x);
    exp1 += (mpfr_exp_t)exp;
    mpfr_set_exp(temp.x.x, exp1);

    exp = Cmplx1.y.exp;
    t = Cmplx1.y;
    t.exp = 0;
    value = t.val;
    temp.y = value;
    exp1 = mpfr_get_exp(temp.y.x);
    exp1 += (mpfr_exp_t)exp;
    mpfr_set_exp(temp.y.x, exp1);
    mpfr_set(this->x.x, temp.x.x, MPFR_RNDN);
    mpfr_set(this->y.x, temp.y.x, MPFR_RNDN);

    return this;
    }


BigComplex * BigComplex::operator+=(const BigComplex & Cmplx1)
    {
    mpfr_add(this->x.x, x.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_add(this->y.x, y.x, Cmplx1.y.x, MPFR_RNDN);
//    x+=Cmplx1.x;
//    y+=Cmplx1.y;
    return this;
    }

BigComplex * BigComplex::operator+=(BigDouble & rvalue)
    {
    mpfr_add(this->x.x, x.x, rvalue.x, MPFR_RNDN);
//    x+=rvalue;
    return this;
    }

BigComplex * BigComplex::operator-=(const BigComplex & Cmplx1)
    {
    mpfr_sub(this->x.x, x.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_sub(this->y.x, y.x, Cmplx1.y.x, MPFR_RNDN);
//    x-=Cmplx1.x;
//    y-=Cmplx1.y;
    return this;
    }

BigComplex * BigComplex::operator-=(BigDouble & rvalue)
    {
    mpfr_sub(this->x.x, x.x, rvalue.x, MPFR_RNDN);
//    x-=rvalue;
    return this;
    }

BigComplex * BigComplex::operator*=(const BigComplex & Cmplx1)
    {
    BigDouble	t1, t2, t3;

    // *e = (*a * *c - *b * *d)
    // *f = (*b * *c + *a * *d)

    mpfr_mul(t1.x, Cmplx1.x.x, x.x, MPFR_RNDN);	// *a * *c
    mpfr_mul(t2.x, Cmplx1.y.x, y.x, MPFR_RNDN);	// *b * *d
    mpfr_sub(t3.x, t1.x, t2.x, MPFR_RNDN);		// *t3 = (*a * *c - *b * *d)

    mpfr_mul(t1.x, Cmplx1.y.x, x.x, MPFR_RNDN);	// *b * *c
    mpfr_mul(t2.x, Cmplx1.x.x, y.x, MPFR_RNDN);	// *a * *d
    mpfr_add(this->y.x, t1.x, t2.x, MPFR_RNDN);	// *f = (*b * *c + *a * *d)

//    copy_bf(this->x.x, t3);		// x = t3
    mpfr_set(this->x.x, t3.x, MPFR_RNDN);		// x = t3

//    t = Cmplx1.x*x-Cmplx1.y*y; 
//    y = Cmplx1.x*y+Cmplx1.y*x;
//    x = t;
    return this;
    }

BigComplex * BigComplex::operator*=(BigDouble & rvalue)
    {
    mpfr_mul(this->x.x, rvalue.x, x.x, MPFR_RNDN);	// *a * *c
    mpfr_mul(this->y.x, rvalue.x, y.x, MPFR_RNDN);	// *b * *d
//    x*=rvalue;
//    y*=rvalue;
    return this;
    }

BigComplex * BigComplex::operator++(void)	// prefix ++ operator
    {
    BigDouble	t;

    mpfr_set_ui(t.x, 1, MPFR_RNDN);
    mpfr_add(this->x.x, x.x, t.x, MPFR_RNDN);
//    x+=1;
    return this;
    }

/*
BigComplex * BigComplex::operator++(int)	// postfix ++ operator
    {
    bf_t	t;

    t = big_alloc(rbflength * 10);
    inttobf(t, 1L);
    memcpy(bftmpcpy1, x.x, bflength+2);
    unsafe_add_bf(this->x.x, bftmpcpy1, t);
    if (t) big_free(t);

//    x+=1;
    return this;
    }
*/

BigComplex BigComplex::operator+(const BigComplex & Cmplx1)	// complex add Operator
    {
    BigComplex	temp;

    mpfr_add(temp.x.x, x.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_add(temp.y.x, y.x, Cmplx1.y.x, MPFR_RNDN);
    return  temp;
    }

BigComplex BigComplex::operator+(const BigDouble & Sum)		// complex add double Operator
    {
    BigComplex	temp;

    mpfr_add(temp.x.x, x.x, Sum.x, MPFR_RNDN);
    mpfr_set(temp.y.x, y.x, MPFR_RNDN);
    return  temp;
    }

BigComplex BigComplex::operator +(const ExpComplex & Cmplx1)	// complex add by ExpComplex Operator
    {
    BigComplex	temp;
    double	value;
    __int64	exp;
    mpfr_exp_t	exp1;
    floatexp	t;

    exp = Cmplx1.x.exp;
    t = Cmplx1.x;
    t.exp = 0;
    value = t.val;
    temp.x = value;
    exp1 = mpfr_get_exp(temp.x.x);
    exp1 += (mpfr_exp_t)exp;
    mpfr_set_exp(temp.x.x, exp1);

    exp = Cmplx1.y.exp;
    t = Cmplx1.y;
    t.exp = 0;
    value = t.val;
    temp.y = value;
    exp1 = mpfr_get_exp(temp.y.x);
    exp1 += (mpfr_exp_t)exp;
    mpfr_set_exp(temp.y.x, exp1);
    mpfr_add(temp.x.x, temp.x.x, x.x, MPFR_RNDN);
    mpfr_add(temp.y.x, temp.y.x, y.x, MPFR_RNDN);

    return  temp;
    }

BigComplex BigComplex::operator-(const BigComplex & Cmplx1)	// complex subtract Operator
    {
    BigComplex	temp;

    mpfr_sub(temp.x.x, x.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_sub(temp.y.x, y.x, Cmplx1.y.x, MPFR_RNDN);
    return  temp;
    }

BigComplex BigComplex::operator-(const BigDouble & Difference)	// complex subtract double Operator
    {
    BigComplex	temp;

    mpfr_sub(temp.x.x, x.x, Difference.x, MPFR_RNDN);
    mpfr_set(temp.y.x, y.x, MPFR_RNDN);
    return  temp;
    }

BigComplex BigComplex::operator-(void)		    // unary minus
    {
    BigComplex	temp;

    mpfr_neg(temp.x.x, x.x, MPFR_RNDN);
    mpfr_neg(temp.y.x, y.x, MPFR_RNDN);
    return  temp;
    }

BigComplex BigComplex::operator*(const BigComplex & Cmplx1)	// complex multiply Operator
    {
    BigComplex	temp;
    BigDouble	t1, t2, t3;

    // *e = (*a * *c - *b * *d)
    // *f = (*b * *c + *a * *d)

    mpfr_mul(t1.x, Cmplx1.x.x, x.x, MPFR_RNDN);	// *a * *c
    mpfr_mul(t2.x, Cmplx1.y.x, y.x, MPFR_RNDN);	// *b * *d
    mpfr_sub(t3.x, t1.x, t2.x, MPFR_RNDN);		// *t3 = (*a * *c - *b * *d)

    mpfr_mul(t1.x, Cmplx1.y.x, x.x, MPFR_RNDN);	// *b * *c
    mpfr_mul(t2.x, Cmplx1.x.x, y.x, MPFR_RNDN);	// *a * *d
    mpfr_add(temp.y.x, t1.x, t2.x, MPFR_RNDN);	// *f = (*b * *c + *a * *d)

    mpfr_set(temp.x.x, t3.x, MPFR_RNDN);		// x = t3

    return  temp;
    }

BigComplex BigComplex::operator*(const BigDouble & Multiplier)	// complex multiply by double Operator
    {
    BigComplex	temp;

    mpfr_mul(temp.x.x, Multiplier.x, x.x, MPFR_RNDN);
    mpfr_mul(temp.y.x, Multiplier.x, y.x, MPFR_RNDN);
    return  temp;
    }

bool BigComplex::operator==(BigComplex & rvalue)
    {
    if (mpfr_cmp(x.x, rvalue.x.x) == 0)
	return (mpfr_cmp(y.x, rvalue.y.x) == 0);
    else
	return FALSE;
    }

BigComplex BigComplex::operator/(const BigComplex & Cmplx1)	// complex divide Operator
    {
    BigComplex	temp;
    BigDouble	t1, t2, t3, d;

    if (mpfr_sgn(Cmplx1.x.x) == 0 && mpfr_sgn(Cmplx1.y.x) == 0)
	mpfr_set_d(d.x, zerotol, MPFR_RNDN);					// prevent divide by zero
    else
	{
	mpfr_mul(t1.x, Cmplx1.x.x, Cmplx1.x.x, MPFR_RNDN);
	mpfr_mul(t2.x, Cmplx1.y.x, Cmplx1.y.x, MPFR_RNDN);
	mpfr_add(d.x, t1.x, t2.x, MPFR_RNDN);		// *f = (*b * *c + *a * *d)
//	mult_bf(a.x, Cmplx1.x.x, Cmplx1.x.x);
//	mult_bf(b.x, Cmplx1.y.x, Cmplx1.y.x);
//	d = a+b;
	}
//    temp.x = (x * Cmplx1.x + y * Cmplx1.y) / d;
//    temp.y = (y * Cmplx1.x - x * Cmplx1.y) / d;
    mpfr_mul(t1.x, x.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_mul(t2.x, y.x, Cmplx1.y.x, MPFR_RNDN);
    mpfr_add(t3.x, t1.x, t2.x, MPFR_RNDN);
    mpfr_div(temp.x.x, t3.x, d.x, MPFR_RNDN);

    mpfr_mul(t1.x, y.x, Cmplx1.x.x, MPFR_RNDN);
    mpfr_mul(t2.x, x.x, Cmplx1.y.x, MPFR_RNDN);
    mpfr_sub(t3.x, t1.x, t2.x, MPFR_RNDN);
    mpfr_div(temp.y.x, t3.x, d.x, MPFR_RNDN);

    return  temp;
    }

BigComplex BigComplex::operator/(const BigDouble & divisor)	// complex divide by double Operator
    {
    BigComplex	temp;
    BigDouble  d;

    mpfr_set(d.x, divisor.x, MPFR_RNDN);
    if (mpfr_sgn(d.x) == 0)
	mpfr_set_d(d.x, zerotol, MPFR_RNDN);					// prevent divide by zero
//    temp.x = x / d;
//    temp.y = y / d;
    mpfr_div(temp.x.x, x.x, d.x, MPFR_RNDN);
    mpfr_div(temp.y.x, y.x, d.x, MPFR_RNDN);
    return  temp;
    }

BigComplex BigComplex::operator^(BigComplex & expon)
    {
    BigComplex	temp;

    temp.x = x;
    temp.y = y;
    temp = (*this).BigComplexPower(expon);
    return temp;
    }

BigComplex BigComplex::operator^(BigDouble & expon)
    {
    BigComplex	temp1, temp2;

    temp2.x = expon;
    temp2.y = 0.0;
    temp1.x = x;
    temp1.y = y;
    temp1 = (*this).BigComplexPower(temp2);
    return temp1;
    }

BigComplex BigComplex::operator^(WORD & degree)
    {
    BigComplex	temp;

    temp.x = x;
    temp.y = y;
    temp = (*this).CPolynomial(degree);
    return temp;			
    }


/**************************************************************************
	Cube c + jd = (a + jb) * (a + jb) * (a + jb) 
***************************************************************************/

BigComplex	BigComplex::CCube()

    {
    BigComplex	temp;
    BigDouble	t, t1, t2, sqr_real, sqr_imag;

    mpfr_mul(sqr_real.x, x.x, x.x, MPFR_RNDN);			// sqr_real = x * x;
    mpfr_mul(sqr_imag.x, y.x, y.x, MPFR_RNDN);			// sqr_imag = y * y;
    mpfr_set_ui(t.x, 3, MPFR_RNDN);
//    strtobf(t, "3.0");
    mpfr_mul(t1.x, t.x, sqr_imag.x, MPFR_RNDN);			// sqr_real + sqr_real + sqr_real
    mpfr_sub(t2.x, sqr_real.x, t1.x, MPFR_RNDN);		// sqr_real - (sqr_imag + sqr_imag + sqr_imag)
    mpfr_mul(temp.x.x, x.x, t2.x, MPFR_RNDN);			// c = x * (sqr_real - (sqr_imag + sqr_imag + sqr_imag))

    mpfr_mul(t1.x, t.x, sqr_real.x, MPFR_RNDN);			// sqr_imag + sqr_imag + sqr_imag
    mpfr_sub(t2.x, t1.x, sqr_imag.x, MPFR_RNDN);		// (sqr_real + sqr_real + sqr_real) - sqr_imag
    mpfr_mul(temp.y.x, y.x, t2.x, MPFR_RNDN);			// d = y * ((sqr_real + sqr_real + sqr_real) - sqr_imag)

    return  temp;
    }

/**************************************************************************
	sumsqr(x+iy)  = x*x+y*y
***************************************************************************/

/*
BigDouble	BigCSumSqr(BigComplex  & Cmplx1)
    {
    return (Cmplx1.x * Cmplx1.x) + (Cmplx1.y * Cmplx1.y);
    }
*/

double	BigComplex::CSumSqr()
    {
    BigDouble	sqr_real, sqr_imag, temp;
    double	out;

    mpfr_mul(sqr_real.x, x.x, x.x, MPFR_RNDN);			// sqr_real = x * x;
    mpfr_mul(sqr_imag.x, y.x, y.x, MPFR_RNDN);			// sqr_imag = y * y;
    mpfr_add(temp.x, sqr_real.x, sqr_imag.x, MPFR_RNDN);
    out = (double)mpfr_get_d(temp.x, MPFR_RNDN);
    return (out);
    }

/**************************************************************************
	sqr(x+iy)  = x*x-y*y + i(x*y)*2
***************************************************************************/

BigComplex	BigComplex::CSqr()
    {
    BigDouble	t, t1, t2;
    BigComplex	a;

    mpfr_mul(t.x, x.x, y.x, MPFR_RNDN);			// temp = a * b
    mpfr_sub(t1.x, x.x, y.x, MPFR_RNDN);		// (a - b)
    mpfr_add(t2.x, x.x, y.x, MPFR_RNDN);		// (a + b)
    mpfr_mul(a.x.x, t1.x, t2.x, MPFR_RNDN);		// c = (a - b) * (a + b)
    mpfr_add(a.y.x, t.x, t.x, MPFR_RNDN);		// d = 2 * temp

    return a;
    }

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
	Convert BigComplex to Complex
***************************************************************************/

Complex	BigComplex::CBig2Double(void)
    {
    Complex a;
    a.x = x.BigDoubleToDouble();
    a.y = y.BigDoubleToDouble();
    return a;
    }

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

