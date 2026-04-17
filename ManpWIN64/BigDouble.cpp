#include "BigDouble.h"
//#include "mpir.h"
#include "big.h"
#include "mpfr.h"
#include <mutex>
#include "BigTrig.h"

static std::mutex g_mpfr_printf_mutex;

BigDouble::BigDouble(void)
    {
    bitcount = decimals * SAFETYMARGIN;
    if (bitcount < MPFR_PREC_MIN) bitcount = MPFR_PREC_MIN;
    if (bitcount >= MPFR_PREC_MAX) bitcount = MPFR_PREC_MAX - 1;
    mpfr_init2(x, bitcount);
    mpfr_set_d(x, 0.0, MPFR_RNDN);
    }

BigDouble::~BigDouble(void)
    {
//    bitcount = mpfr_get_default_prec();
//    mpfr_set_prec_raw(x, bitcount);
    mpfr_clear(x);
    }

BigDouble & BigDouble::operator=(const BigDouble & Big)
    {
    if (this != &Big)
	{
	mpfr_prec_round(x, mpfr_get_prec(Big.x), MPFR_RNDN);
	mpfr_set(x, Big.x, MPFR_RNDN);
	bitcount = mpfr_get_prec(Big.x);
	}
    return *this;
    }
BigDouble & BigDouble::operator=(const double & value)	// Assignment to double Operator
    {
    mpfr_set_d(x, value, MPFR_RNDN);
    return  *this;
    }

BigDouble& BigDouble::operator+=(const BigDouble& rvalue)
    {
    mpfr_add(x, x, rvalue.x, MPFR_RNDN);
    return *this;
    }

BigDouble& BigDouble::operator+=(const double& rvalue)
    {
    mpfr_add_d(x, x, rvalue, MPFR_RNDN);
    return *this;
    }

BigDouble& BigDouble::operator-=(const BigDouble& rvalue)
    {
    mpfr_sub(x, x, rvalue.x, MPFR_RNDN);
    return *this;
    }


BigDouble & BigDouble::operator-=(const double & rvalue)
    {
    BigDouble temp;
    mpfr_set_d(temp.x, rvalue, MPFR_RNDN);
    mpfr_sub(x, x, temp.x, MPFR_RNDN);
    return *this;
    }

BigDouble & BigDouble::operator*=(const BigDouble & Big)
    {
    mpfr_mul(x, x, Big.x, MPFR_RNDN);
    return *this;
    }

BigDouble& BigDouble::operator*=(const double& rvalue)
    {
    mpfr_mul_d(x, x, rvalue, MPFR_RNDN);
    return *this;
    }


int BigDouble::ChangePrecision(int n)
    {
    mpfr_prec_t newprec = (mpfr_prec_t)(n * SAFETYMARGIN);

    if (newprec >= MPFR_PREC_MAX)
	newprec = MPFR_PREC_MAX - 1;
    if (newprec < MPFR_PREC_MIN)
	newprec = MPFR_PREC_MIN;

    mpfr_prec_round(x, newprec, MPFR_RNDN);
    bitcount = newprec;

    return 0;
    }
/*
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
*/

BigDouble BigDouble::operator+(const BigDouble & Big) const	// add Operator
    {
    BigDouble	temp;
    mpfr_add(temp.x, x, Big.x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator+(const double & Sum) const	//  add double Operator
    {
    BigDouble	temp;
    mpfr_set_d(temp.x, Sum, MPFR_RNDN);
    mpfr_add(temp.x, temp.x, x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator-(const BigDouble & Big) const	// complex subtract Operator
    {
    BigDouble	temp;

    mpfr_sub(temp.x, x, Big.x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator-(const double & Difference) const	// complex subtract double Operator
    {
    BigDouble	temp1, temp2;
    mpfr_set_d(temp2.x, Difference, MPFR_RNDN);
    mpfr_sub(temp1.x, x, temp2.x, MPFR_RNDN);
    return  temp1;
    }

BigDouble BigDouble::operator-(void) const			// unary minus
    {
    BigDouble	temp;
//    bf_t	t = big_alloc(rbflength * 10);

//    clear_bf(t);						// t = 0
//    sub_bf(temp.x, t, x);					// temp = 0 - t
//    if (t)
//	big_free(t);
    mpfr_neg(temp.x, x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator*(const BigDouble & mult) const	// complex multiply Operator
    {
    BigDouble	temp;
    mpfr_mul(temp.x, x, mult.x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator*(const double & Multiplier) const	// complex multiply by double Operator
    {
    BigDouble	temp, tmpcpy1;

    mpfr_set_d(tmpcpy1.x, Multiplier, MPFR_RNDN);
    mpfr_mul(temp.x, x, tmpcpy1.x, MPFR_RNDN);
    return  temp;
    }

BigDouble BigDouble::operator/(const BigDouble & Div) const	// complex divide Operator
    {
    BigDouble   temp1, temp2;
    mpfr_set(temp2.x, Div.x, MPFR_RNDN);
    if (mpfr_sgn(temp2.x) == 0)
	mpfr_set_d(temp2.x, zerotol, MPFR_RNDN);
    mpfr_div(temp1.x, x, Div.x, MPFR_RNDN);			// c = a/temp
    return  temp1;
    }

BigDouble BigDouble::operator/(const double & divisor) const	// bignum divide by double Operator
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
bool BigDouble::operator==(const BigDouble & Big) const
    {
    return (mpfr_cmp(x, Big.x) == 0);
    }

bool BigDouble::operator<(const BigDouble & Big) const
    {
    return (mpfr_cmp(x, Big.x) < 0);
    }

bool BigDouble::operator>(const BigDouble & Big) const
    {
    return (mpfr_cmp(x, Big.x) > 0);
    }

bool BigDouble::operator==(const double & Small) const
    {
    BigDouble tmpcpy1;
    mpfr_set_d(tmpcpy1.x, Small, MPFR_RNDN);
    return (mpfr_cmp(x, tmpcpy1.x) == 0);
    }

bool BigDouble::operator<(const double & Small) const
    {
    BigDouble tmpcpy1;
    mpfr_set_d(tmpcpy1.x, Small, MPFR_RNDN);
    return (mpfr_cmp(x, tmpcpy1.x) < 0);
    }

bool BigDouble::operator>(const double & Small) const
    {
    BigDouble tmpcpy1;
    mpfr_set_d(tmpcpy1.x, Small, MPFR_RNDN);
    return (mpfr_cmp(x, tmpcpy1.x) > 0);
    }

bool BigDouble::operator>=(const BigDouble& other) const
    {
    return mpfr_cmp(x, other.x) >= 0;
    }

bool BigDouble::operator<=(const BigDouble& other) const
    {
    return mpfr_cmp(x, other.x) <= 0;
    }

bool BigDouble::operator!=(const BigDouble& other) const
    {
    return mpfr_cmp(x, other.x) != 0;
    }

BigDouble BigDouble::operator^(double expon) const
    {
    BigDouble result, exp;
    exp = expon;
    mpfr_pow(result.x, x, exp.x, MPFR_RNDN);
    return result;
    }

BigDouble BigDouble::operator^(const BigDouble& expon) const
    {
    BigDouble result;
    mpfr_pow(result.x, x, expon.x, MPFR_RNDN);
    return result;
    }

BigDouble& BigDouble::operator++()
    {
    mpfr_add_ui(x, x, 1, MPFR_RNDN);
    return *this;
    }

BigDouble BigDouble::operator++(int)
    {
    BigDouble temp = *this;
    mpfr_add_ui(x, x, 1, MPFR_RNDN);
    return temp;
    }

/**************************************************************************
	How about some safe routines for converting BigDouble to strings
***************************************************************************/

void BigDouble::SafeSprintf(char* buf, size_t bufSize, char* fmt) const
    {
    if (!buf || bufSize == 0)
	return;

    std::lock_guard<std::mutex> lock(g_mpfr_printf_mutex);

    // mpfr_snprintf guarantees null termination if size > 0
    int written = mpfr_snprintf(buf, bufSize, fmt, x);

    // Extra paranoia: ensure termination
    buf[bufSize - 1] = '\0';

    // Optional: handle truncation if you want diagnostics
    // if (written < 0 || (size_t)written >= bufSize) { ... }
    }

void BigDouble::ToString(char* buf, size_t bufSize, bool scientific) const
    {
    if (!buf || bufSize == 0)
	return;

    char fmt[32];
    // bits -> decimal digits
    int digits10 = (int)ceil(mpfr_get_prec(x) * 0.3010299956639812) + 2;

    if (scientific)
	snprintf(fmt, sizeof(fmt), "%%.%dRe", digits10);
    else
	snprintf(fmt, sizeof(fmt), "%%.%dRf", digits10 + PRECISION_FACTOR);

    mpfr_snprintf(buf, bufSize, fmt, x);

    // Hard safety net
    buf[bufSize - 1] = '\0';
    }

/**************************************************************************
	Ensure we have the correct precision
***************************************************************************/

void BigDouble::MatchPrecision(const BigDouble& other)
    {
    if (this == nullptr)
	{
	OutputDebugStringA("MatchPrecision: this is NULL\n");
	return;
	}

    if (mpfr_get_prec(x) == 0)   // or your own flag
	{
	mpfr_init2(x, mpfr_get_prec(other.x));
	}
    else if (mpfr_get_prec(x) != mpfr_get_prec(other.x))
	{
	mpfr_set_prec(x, mpfr_get_prec(other.x));
	}
    }

/**************************************************************************
	Abs(x)  = (x > 0) ? x : -x
***************************************************************************/

BigDouble   BigDouble::BigAbs() const

    {
    BigDouble	a;

    mpfr_abs(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	invert(x)  = 1/x
***************************************************************************/

BigDouble   BigDouble::BigInvert() const	// invert

    {
    BigDouble	a;

    mpfr_ui_div(a.x, 1, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	BigSqr(x)  = x^2
***************************************************************************/

BigDouble   BigDouble::BigSqr()	const	    // square

    {
    BigDouble	a;

    mpfr_mul(a.x, x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	r = 2*n
***************************************************************************/

BigDouble	BigDouble::BigX2() const
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

BigDouble	BigDouble::BigHalf() const
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

BigDouble	BigDouble::BigSqrt() const
    {
    BigDouble	out;

    mpfr_sqrt(out.x, x, MPFR_RNDN);
    return (out);
    }

/*********************************************************************
  l = floor(b), floor rounds down
  Converts a bigfloat to a double 
  note: a bf value of 2.999... will be return a value of 2, not 3  
*********************************************************************/

/*********************************************************************
  Set pi to the curren tprecision
*********************************************************************/

void BigDouble::SetPi()
    {
    mpfr_const_pi(x, MPFR_RNDN);
    }

/*********************************************************************
    Multiply BigNum by int
*********************************************************************/

void BigDouble::MulInt(BigDouble& result, long n) const
    {
    // Ensure result has correct precision
    if (mpfr_get_prec(result.x) != mpfr_get_prec(this->x))
	{
	mpfr_prec_round(result.x, mpfr_get_prec(this->x), MPFR_RNDN);
	result.bitcount = mpfr_get_prec(this->x);
	}
    // Multiply
    mpfr_mul_si(result.x, this->x, n, MPFR_RNDN);
    }

void BigDouble::MulInt(BigDouble& result, int n) const
    {
    // Ensure result has correct precision
    if (mpfr_get_prec(result.x) != mpfr_get_prec(this->x))
	{
	mpfr_prec_round(result.x, mpfr_get_prec(this->x), MPFR_RNDN);
	result.bitcount = mpfr_get_prec(this->x);
	}

    // Multiply
    mpfr_mul_si(result.x, this->x, n, MPFR_RNDN);
    }

/*********************************************************************
    A few nice conversions
*********************************************************************/

double	  BigDouble::BigDoubleToDouble() const	// convert a big double to double

    {
    double    a;

    a = mpfr_get_d(x, MPFR_RNDN);
    return a;
    }

/***********************************************************************
	Convert bignum to double double
***********************************************************************/

int BigDouble::BigDouble2DD(dd_real *out)
    {
    if (!out)
	return -1;

    dd_real x1, x2;
    BigDouble t1, t2;
    double y1, y2;

    y1 = BigDoubleToDouble();
    t1 = y1;
    t2 = *this - t1;

    y2 = t2.BigDoubleToDouble();

    x1 = y1;
    x2 = y2;

    *out = dd_real(y1, y2);

    if (isnan(out->x[0]) || isnan(out->x[1]))
	{
	*out = 0.0;
	return -1;
	}

    return 0;
    }

/***********************************************************************
	Convert bignum to quad double
***********************************************************************/

int BigDouble::BigDouble2QD(qd_real *out)
    {
    if (!out) return -1;

    char buf[256];
    mpfr_snprintf(buf, sizeof(buf), "%.100Re", x);

    *out = qd_real(buf);

    if (isnan(out->x[0])) {
	*out = 0.0;
	return -1;
	}

    return 0;
    }
/*
int BigDouble::BigDouble2QD(qd_real *out)
    {
    if (!out)
	return -1;

    qd_real x1, x2, x3, x4;
    BigDouble t1, t2, t3, t4, t5, t6;
    double y1, y2, y3, y4;

    y1 = BigDoubleToDouble();
    t1 = y1;
    t2 = *this - t1;

    y2 = t2.BigDoubleToDouble();
    t3 = y2;
    t4 = t2 - t3;

    y3 = t4.BigDoubleToDouble();
    t5 = y3;
    t6 = t4 - t5;

    y4 = t6.BigDoubleToDouble();

    x1 = y1;
    x2 = y2;
    x3 = y3;
    x4 = y4;

    *out = qd_real(y1, y2, y3, y4);

    if (isnan(out->x[0]) || isnan(out->x[1]) ||
	isnan(out->x[2]) || isnan(out->x[3]))
	{
	*out = 0.0;
	return -1;
	}

    return 0;
    }
*/

/***********************************************************************
	Convert double double to bignum
***********************************************************************/

int	BigDouble::DD2BigDouble(const dd_real& in)
    {
    if (isnan(in.x[0]))
	{
	mpfr_set_d(x, 0.0, MPFR_RNDN);
	return -1;
	}
    // dd_real = hi + lo
    double hi = in.x[0];
    double lo = in.x[1];

    // Set value = hi
    mpfr_set_d(x, hi, MPFR_RNDN);

    // Add low part
    mpfr_add_d(x, x, lo, MPFR_RNDN);

    return 0;
    }

/***********************************************************************
	Convert quad double to bignum
***********************************************************************/

int	BigDouble::QD2BigDouble(const qd_real& in)
    {
    if (isnan(in.x[0]))
	{
	mpfr_set_d(x, 0.0, MPFR_RNDN);
	return -1;
	}
    // Start with highest component
    mpfr_set_d(x, in.x[0], MPFR_RNDN);

    // Add remaining components
    mpfr_add_d(x, x, in.x[1], MPFR_RNDN);
    mpfr_add_d(x, x, in.x[2], MPFR_RNDN);
    mpfr_add_d(x, x, in.x[3], MPFR_RNDN);

    return 0;
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

BigDouble   BigDouble::BigSin()	const	// sin

    {
    BigDouble	a;

    mpfr_sin(a.x, x, MPFR_RNDN);
    return a;
    }

BigDouble BigDouble::Sin() const
    {
    BigDouble sine, dummy;
    CBigTrig trig;
    trig.sincos_bf(&sine, &dummy, *this); // or your sin-only path
    return sine;
    }

/**************************************************************************
	cos(x)
***************************************************************************/

BigDouble   BigDouble::BigCos()	const	// cos

    {
    BigDouble	a;

    mpfr_cos(a.x, x, MPFR_RNDN);
    return a;
    }

BigDouble BigDouble::Cos() const
    {
    BigDouble cosine, dummy;
    CBigTrig trig;
    trig.sincos_bf(&dummy, &cosine, *this); // or your sin-only path
    return cosine;
    }

/**************************************************************************
	tan(x)
***************************************************************************/

BigDouble   BigDouble::BigTan()	 const		// tan

    {
    BigDouble	a;

    mpfr_tan(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	sinh(x)
***************************************************************************/

BigDouble   BigDouble::BigSinh() const		// sinh

    {
    BigDouble	a;

    mpfr_sinh(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	cosh(x)
***************************************************************************/

BigDouble   BigDouble::BigCosh() const		// cosh

    {
    BigDouble	a;

    mpfr_cosh(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	tanh(x)
***************************************************************************/

BigDouble   BigDouble::BigTanh() const		// tanh

    {
    BigDouble	a;

    mpfr_tanh(a.x, x, MPFR_RNDN);
    return a;
    }

/**************************************************************************
	log(x)
***************************************************************************/

BigDouble   BigDouble::BigLog() const

    {
    BigDouble	a;

    mpfr_log(a.x, x, MPFR_RNDN);
//    ln_bf(a.x, x);
    return a;
    }

/**************************************************************************
	log10(x) = ln(x)/ln(10)
***************************************************************************/

BigDouble   BigDouble::BigLog10() const

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

// ---------------------------------------------------------------------------
// BigDouble mixed arithmetic with built-in types (double + BigDouble)
//
// Why this exists:
// ----------------
// C++ does NOT automatically reverse operators. Even though we implement
// member operators such as:
//
//     BigDouble::operator+(double)
//     BigDouble::operator-(double)
//     BigDouble::operator*(double)
//     BigDouble::operator/(double)
//
// those only cover:
//
//     BigDouble + double
//
// The Tierazon template engine (and other generic math code) also produces:
//
//     double + BigDouble
//
// e.g.
//     z->x = param[0] + z->x;
//     z->x = param[0] - z->x;
//     z->x = param[0] * z->x;
//     z->x = param[0] / z->x;
//
// Since 'double' is a built-in type, we cannot attach member functions to it.
// Therefore, we must provide NON-MEMBER operator overloads for symmetry.
//
// Thread safety:
// --------------
// These functions are thread-safe because:
//   - they use only local variables (no shared/static state)
//   - MPFR operates on per-instance data (each BigDouble owns its mpfr_t)
//
// Summary:
// --------
// These overloads allow BigDouble to behave like a native numeric type in
// templated code, supporting full bidirectional arithmetic with doubles.
//
// Supported operations:
// ---------------------
//     double + BigDouble
//     double - BigDouble
//     double * BigDouble
//     double / BigDouble
// ---------------------------------------------------------------------------

BigDouble operator-(double lhs, const BigDouble& rhs)
    {
    BigDouble result;
    mpfr_set_d(result.x, lhs, MPFR_RNDN);
    mpfr_sub(result.x, result.x, rhs.x, MPFR_RNDN);
    return result;
    }

// double + BigDouble
BigDouble operator+(double lhs, const BigDouble& rhs)
    {
    BigDouble result;
    mpfr_set_d(result.x, lhs, MPFR_RNDN);
    mpfr_add(result.x, result.x, rhs.x, MPFR_RNDN);
    return result;
    }

// double * BigDouble
BigDouble operator*(double lhs, const BigDouble& rhs)
    {
    BigDouble result;
    mpfr_set_d(result.x, lhs, MPFR_RNDN);
    mpfr_mul(result.x, result.x, rhs.x, MPFR_RNDN);
    return result;
    }

// double / BigDouble
BigDouble operator/(double lhs, const BigDouble& rhs)
    {
    BigDouble result;
    mpfr_set_d(result.x, lhs, MPFR_RNDN);
    mpfr_div(result.x, result.x, rhs.x, MPFR_RNDN);
    return result;
    }

