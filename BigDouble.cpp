#include "BigDouble.h"
//#include "mpir.h"
#include "mpfr.h"

BigDouble::BigDouble(void)
    {
    bitcount = decimals * SAFETYMARGIN;
    mpfr_set_default_prec(decimals * SAFETYMARGIN);
    mpfr_init2(x, decimals * SAFETYMARGIN);
    }

BigDouble::~BigDouble(void)
    {
//    bitcount = mpfr_get_default_prec();
//    mpfr_set_prec_raw(x, bitcount);
    mpfr_clear(x);
    }

BigDouble * BigDouble::operator=(const BigDouble & Big)	// Jonathan Osuch's assignment operator PHD 2011-07-18
    {							// removes one instance of copy constructor and memcpy()
    mpfr_set(x, Big.x, MPFR_RNDN);
    return this;
    }

BigDouble * BigDouble::operator=(const double & value)	// Assignment to double Operator
    {
    mpfr_set_d(x, value, MPFR_RNDN);
    return  this;
    }

BigDouble * BigDouble::operator+=(const BigDouble & Big)
    {
    mpfr_add(x, x, Big.x, MPFR_RNDN);
    return this;
    }

BigDouble * BigDouble::operator+=(double & rvalue)
    {
    BigDouble temp;
    mpfr_set_d(temp.x, rvalue, MPFR_RNDN);
    mpfr_add(x, x, temp.x, MPFR_RNDN);
    return this;
    }

BigDouble * BigDouble::operator-=(const BigDouble & Big)
    {
    mpfr_sub(x, x, Big.x, MPFR_RNDN);
    return this;
    }

BigDouble * BigDouble::operator-=(double & rvalue)
    {
    BigDouble temp;
    mpfr_set_d(temp.x, rvalue, MPFR_RNDN);
    mpfr_sub(x, x, temp.x, MPFR_RNDN);
    return this;
    }

BigDouble * BigDouble::operator*=(const BigDouble & Big)
    {
    mpfr_mul(x, x, Big.x, MPFR_RNDN);
    return this;
    }

BigDouble * BigDouble::operator*=(double & rvalue)
    {
    BigDouble temp;
    mpfr_set_d(temp.x, rvalue, MPFR_RNDN);
    mpfr_mul(x, x, temp.x, MPFR_RNDN);
    return this;
    }

int	BigDouble::ChangePrecision(int n)		// increase precision to n digits
    {
    mpfr_t  temp;					// need to store variable before

    mpfr_init2(temp, decimals * SAFETYMARGIN);		// set to new precision
    mpfr_set(temp, x, MPFR_RNDN);			// copy in old value
    bitcount = n * SAFETYMARGIN;
    if (bitcount >= MPFR_PREC_MAX)
	bitcount = MPFR_PREC_MAX - 1;
    if(bitcount < MPFR_PREC_MIN)
	bitcount = MPFR_PREC_MIN;
    mpfr_set_prec(x, bitcount);				// sets new precision and initialises to NaN
    mpfr_swap(temp, x);					// swap values to reinit variable
    mpfr_clear(temp);
    return 0;
    }

BigDouble BigDouble::operator+(const BigDouble & Big)	// add Operator
    {
    BigDouble	temp;
    mpfr_add(temp.x, x, Big.x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator+(const double & Sum)	//  add double Operator
    {
    BigDouble	temp;
    mpfr_set_d(temp.x, Sum, MPFR_RNDN);
    mpfr_add(temp.x, temp.x, x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator-(const BigDouble & Big)	// complex subtract Operator
    {
    BigDouble	temp;

    mpfr_sub(temp.x, x, Big.x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator-(const double & Difference)	// complex subtract double Operator
    {
    BigDouble	temp1, temp2;
    mpfr_set_d(temp2.x, Difference, MPFR_RNDN);
    mpfr_sub(temp1.x, x, temp2.x, MPFR_RNDN);
    return  temp1;
    }

BigDouble BigDouble::operator-(void)		    // unary minus
    {
    BigDouble	temp;
//    bf_t	t = big_alloc(rbflength * 10);

//    clear_bf(t);				    // t = 0
//    sub_bf(temp.x, t, x);			    // temp = 0 - t
//    if (t)
//	big_free(t);
    mpfr_neg(temp.x, x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator*(const BigDouble & mult)	// complex multiply Operator
    {
    BigDouble	temp;
    mpfr_mul(temp.x, x, mult.x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator*(const double & Multiplier)	// complex multiply by double Operator
    {
    BigDouble	temp, tmpcpy1;

    mpfr_set_d(tmpcpy1.x, Multiplier, MPFR_RNDN);
    mpfr_mul(temp.x, x, tmpcpy1.x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator/(const BigDouble & Div)	// complex divide Operator
    {
    BigDouble   temp1, temp2;
    mpfr_set(temp2.x, Div.x, MPFR_RNDN);
    if (mpfr_sgn(temp2.x) == 0)
	mpfr_set_d(temp2.x, zerotol, MPFR_RNDN);
    mpfr_div(temp1.x, x, Div.x, MPFR_RNDN);				// c = a/temp
    return  temp1;
    }

BigDouble BigDouble::operator/(const double & divisor)	// bignum divide by double Operator
    {
    BigDouble	temp, tmpcpy1, tmpcpy2;
    mpfr_set(tmpcpy2.x, x, MPFR_RNDN);
    if (divisor == 0.0)
	mpfr_set_d(tmpcpy1.x, zerotol, MPFR_RNDN);
    else
	mpfr_set_d(tmpcpy1.x, divisor, MPFR_RNDN);
    mpfr_div(temp.x, tmpcpy2.x, tmpcpy1.x, MPFR_RNDN);			// c = a/temp
    return  temp;
    }

// new stuff
bool BigDouble::operator==(BigDouble & Big)
    {
    return (mpfr_cmp(x, Big.x) == 0);
    }

bool BigDouble::operator<(BigDouble & Big)
    {
    return (mpfr_cmp(x, Big.x) < 0);
    }

bool BigDouble::operator>(BigDouble & Big)
    {
    return (mpfr_cmp(x, Big.x) > 0);
    }

bool BigDouble::operator==(double & Small)
    {
    BigDouble tmpcpy1;
    mpfr_set_d(tmpcpy1.x, Small, MPFR_RNDN);
    return (mpfr_cmp(x, tmpcpy1.x) == 0);
    }

bool BigDouble::operator<(double & Small)
    {
    BigDouble tmpcpy1;
    mpfr_set_d(tmpcpy1.x, Small, MPFR_RNDN);
    return (mpfr_cmp(x, tmpcpy1.x) < 0);
    }

bool BigDouble::operator>(double & Small)
    {
    BigDouble tmpcpy1;
    mpfr_set_d(tmpcpy1.x, Small, MPFR_RNDN);
    return (mpfr_cmp(x, tmpcpy1.x) > 0);
    }

/*
BigDouble BigDouble::operator^(double & expon)
    {
    Complex	temp;
    
    temp.x = x;
    temp.y = y;
    temp = CExp(CLog(temp)*expon);
    return temp;			
    }

BigDouble BigDouble::operator^(BigDouble & expon)
    {
    Complex	temp;

    temp.x = x;
    temp.y = y;
    temp = CExp(CLog(temp)*expon);
    return temp;			
    }

BigDouble BigDouble::operator++(void)
    {
    x+=1;
    return *this;
    }
*/

/**************************************************************************
	Abs(x)  = (x > 0) ? x : -x
***************************************************************************/

BigDouble   BigDouble::BigAbs()

    {
    BigDouble	a;

    mpfr_abs(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	invert(x)  = 1/x
***************************************************************************/

BigDouble   BigDouble::BigInvert()		// invert

    {
    BigDouble	a;

    mpfr_ui_div(a.x, 1, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	BigSqr(x)  = x^2
***************************************************************************/

BigDouble   BigDouble::BigSqr()		// square

    {
    BigDouble	a;

    mpfr_mul(a.x, x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	r = 2*n
***************************************************************************/

BigDouble	BigDouble::BigX2()
    {
    BigDouble	a;
    mpfr_exp_t	exp;

    exp = mpfr_get_exp(x);
    mpfr_set(a.x, x, MPFR_RNDN);
    mpfr_set_exp(a.x, exp + 1);
    //    mpfr_add(a.x, x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	r = n/2
***************************************************************************/

BigDouble	BigDouble::BigHalf()
    {
    BigDouble a;
    mpfr_exp_t	exp;

    exp = mpfr_get_exp(x);
    mpfr_set(a.x, x, MPFR_RNDN);
    mpfr_set_exp(a.x, exp - 1);
    //    mpfr_div_ui(a.x, x, 2, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	r = sqrt(n)
***************************************************************************/

BigDouble	BigDouble::BigSqrt()
    {
    BigDouble	out;

    mpfr_sqrt(out.x, x, MPFR_RNDN);
    return (out);
    }

/*********************************************************************
  l = floor(b), floor rounds down
  Converts a bigfloat to a long 
  note: a bf value of 2.999... will be return a value of 2, not 3  
*********************************************************************/

double	  BigDouble::BigDoubleToDouble()	// convert a big double to double

    {
    double    a;

    a = mpfr_get_d(x, MPFR_RNDN);
    return a;
    }

/*********************************************************************
  l = floor(b), floor rounds down
  Converts a bigfloat to a long 
  note: a bf value of 2.999... will be return a value of 2, not 3  
*********************************************************************/

long	  BigDouble::BigDoubleToInt()	// convert a big double to int

    {
    long    a;

    a = (int)mpfr_get_d(x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	sin(x)
***************************************************************************/

BigDouble   BigDouble::BigSin()		// sin

    {
    BigDouble	a;

    mpfr_sin(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	cos(x)
***************************************************************************/

BigDouble   BigDouble::BigCos()		// cos

    {
    BigDouble	a;

    mpfr_cos(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	tan(x)
***************************************************************************/

BigDouble   BigDouble::BigTan()		// tan

    {
    BigDouble	a;

    mpfr_tan(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	sinh(x)
***************************************************************************/

BigDouble   BigDouble::BigSinh()		// sinh

    {
    BigDouble	a;

    mpfr_sinh(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	cosh(x)
***************************************************************************/

BigDouble   BigDouble::BigCosh()		// cosh

    {
    BigDouble	a;

    mpfr_cosh(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	tanh(x)
***************************************************************************/

BigDouble   BigDouble::BigTanh()		// tanh

    {
    BigDouble	a;

    mpfr_tanh(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	log(x)
***************************************************************************/

BigDouble   BigDouble::BigLog()

    {
    BigDouble	a;

    mpfr_log(a.x, x, MPFR_RNDN);
//    ln_bf(a.x, x);
    return a;
    }

/**************************************************************************
	log10(x) = ln(x)/ln(10)
***************************************************************************/

BigDouble   BigDouble::BigLog10()

    {
    BigDouble	a;

    mpfr_log10(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	Abs(x)  = (x > 0) ? x : -x
***************************************************************************/

/*
BigDouble   BigAbs(BigDouble & Num)

    {
    BigDouble	a;
//    BigDouble	b = 0.0;

    abs_bf(a.x, Num.x);
//    a = (x > b) ? Num : -Num;
    return a;
    }
*/

/**************************************************************************
	invert(x)  = 1/x
***************************************************************************/

/*
BigDouble   BigInvert(BigDouble  & Num)		// invert

    {
    BigDouble	a;
    BigDouble	b = 1.0;

    div_bf(a.x, b.x, Num.x);
    return a;
    }
*/

