/*********************************************************************
	BigTrig.cpp - C routines for big floating point trig functions 
	Wesley Loewer's Big Numbers.     (C) 1994-95, Wesley B. Loewer

	WThis allows an alternative for multi-threading where trig 
	functions fail using MPFR
*********************************************************************/

#include <atomic>
#include "BigTrig.h"

extern	std::atomic<bool> gStopRequested;	// force early exit

// Simple rule
// working inside BigTrig function, use EnsurePrecision()
// creating temp variables, use MatchPrecision()

void CBigTrig::EnsurePrecision(const BigDouble& ref)
    {
    int p = mpfr_get_prec(ref.x);

    if (p != precision)
	{
	precision = p;
	InitConstants();
	}
    }

void CBigTrig::InitConstants()
    {
    mpfr_const_pi(pi.x, MPFR_RNDN);

    half_pi = pi.BigHalf();
    two_pi = pi.BigX2();
    quarter_pi = half_pi.BigHalf();

    one = BigDouble(1.0);
    two = BigDouble(2.0);
    inv_two = one.BigHalf();
    }

/********************************************************************/
/* sincos_bf(r)                                                     */
/********************************************************************/

void CBigTrig::sincos_bf(BigDouble *s, BigDouble *c, const BigDouble& n)
    {
    U16     fact = 2;
    int     k = 0, i, halves;
    int     signcos = 0, signsin = 0, switch_sincos = 0;
    int     sin_done = 0, cos_done = 0;
    BigDouble   bftmp1, bftmp2, zero;
    BigDouble   x;

    if (!s || !c) return;

    x = n;
    EnsurePrecision(n);

    bftmp1.MatchPrecision(x);
    bftmp2.MatchPrecision(x);
    s->MatchPrecision(x);
    c->MatchPrecision(x);

    if (x == zero)
	{
	*s = 0.0;
	*c = 1.0;
	return;
	}

    if (x < zero)
	{
	signsin = !signsin;
	x = -x;
	}

    bftmp1 = two_pi;
    int reduceCount = 0;
    while (x > bftmp1 || x == bftmp1)
	{
	if (gStopRequested.load())
	    break;

	if (++reduceCount > 1000000)
	    {
	    break;
	    }

	bftmp2 = bftmp1;
	x = x - bftmp2;
	}

    bftmp1 = pi;
    if (x > bftmp1 || x == bftmp1)
	{
	x = x - bftmp1;
	signsin = !signsin;
	signcos = !signcos;
	}

    bftmp1 = half_pi;
    if (x > bftmp1)
	{
	bftmp2 = pi;
	x = bftmp2 - x;
	signcos = !signcos;
	}

    bftmp1 = quarter_pi;
    if (x > bftmp1)
	{
	bftmp1 = half_pi;
	x = bftmp1 - x;
	switch_sincos = !switch_sincos;
	}

    // SECOND zero check (after range reduction) — FIX THIS ONE
    if (mpfr_zero_p(x.x))
	{
	*s = zero;
	*c = one;

	if (switch_sincos)
	    {
	    BigDouble tmp = *s;
	    *s = *c;
	    *c = tmp;
	    }

	if (signsin)
	    *s = -*s;
	if (signcos)
	    *c = -*c;
	return;
	}

    halves = 1;
    for (i = 0; i < halves; i++)
	x = x.BigHalf();

    *s = x;
    *c = one;
    bftmp1 = x;

    for (int i = 0; i < 200; i++)
	{
	if (gStopRequested.load())
	    break;

	// even term for cosine
	bftmp2 = bftmp1;
	bftmp1 = bftmp2 * x;
	bftmp1 = bftmp1 / fact++;

	if (!cos_done)
	    {
	    BigDouble newc;
	    newc.MatchPrecision(*c);

	    if (k)
		newc = *c + bftmp1;
	    else
		newc = *c - bftmp1;

	    cos_done = (mpfr_cmp(newc.x, c->x) == 0);

	    if (!cos_done)
		*c = newc;
	    }

	// odd term for sine
	bftmp2 = bftmp1;
	bftmp1 = bftmp2 * x;
	bftmp1 = bftmp1 / fact++;

	if (!sin_done)
	    {
	    BigDouble news;
	    news.MatchPrecision(*s);

	    if (k)
		news = *s + bftmp1;
	    else
		news = *s - bftmp1;

	    sin_done = (mpfr_cmp(news.x, s->x) == 0);

	    if (!sin_done)
		*s = news;
	    }

	k = !k;

	if (cos_done && sin_done)
	    break;
	}

    for (i = 0; i < halves; i++)
	{
	*s = *s * *c * 2;
	*c = c->BigSqr() * 2 - 1.0;
	}

    if (switch_sincos)
	{
	bftmp1 = *s;
	*s = *c;
	*c = bftmp1;
	}
    if (signsin)
	*s = -*s;
    if (signcos)
	*c = -*c;
    }

/********************************************************************/
/* sinhcosh_bf(r)                                                   */
/********************************************************************/

void CBigTrig::sinhcosh_bf(BigDouble* s, BigDouble* c, const BigDouble& x)
    {
    BigDouble ex;

    EnsurePrecision(x);
    exp_bf(&ex, x);

    BigDouble inv_ex = one / ex;

    *s = (ex - inv_ex);
    *c = (ex + inv_ex);

    *s = s->BigHalf();
    *c = c->BigHalf();
    }

/********************************************************************/
/* exp_bf(r)                                                   */
/********************************************************************/

void CBigTrig::exp_bf(BigDouble* ExpOut, const BigDouble& n)
    {
    BigDouble x;
    BigDouble term, sum;
    BigDouble one, zero;
    BigDouble tmp, tmp1;
    int halves = 0;

    EnsurePrecision(n);
    bool neg = mpfr_sgn(n.x) < 0;

    x = n;

    if (neg)
	{
	x = -x;
	}

    one.MatchPrecision(x);
    zero.MatchPrecision(x);
    term.MatchPrecision(x);
    sum.MatchPrecision(x);
    ExpOut->MatchPrecision(x);

    mpfr_set_ui(one.x, 1, MPFR_RNDN);
    mpfr_set_zero(zero.x, 0);

    if (mpfr_zero_p(x.x))
	{
	*ExpOut = one;
	return;
	}

    // Reduce argument until Taylor is working on a small value.
    // |x| <= 1 is a safe simple choice.
    while (mpfr_cmpabs(tmp1.x, one.x) > 0)
	{
	if (gStopRequested.load())
	    break;

	x = x.BigHalf();
	halves++;

	// safety guard
	if (halves > 1024)
	    {
	    break;
	    }
	}

//    delta = MakeSeriesDelta(x);

    // Taylor series for exp(x)
    sum = one;
    term = one;

    for (int fact = 1; fact < 1000; fact++)
	{
	if (gStopRequested.load())
	    break;

	term = term * x;
	term = term / BigDouble((double)fact);

	BigDouble newSum = sum + term;

	if (mpfr_cmp(newSum.x, sum.x) == 0)
	    {
	    break;
	    }

	sum = newSum;

	if (fact == 999)
	    {
#ifdef _DEBUG
	    OutputDebugStringA("exp_bf hit safety cap\n");
#endif
	    }
	}

    // Undo the halving: exp(original) = exp(reduced)^(2^halves)
    for (int i = 0; i < halves; i++)
	{
	if (gStopRequested.load())
	    break;

	sum = sum * sum;
	}

    if (neg)
	{
	*ExpOut = one / sum;
	}
    else
	{
	*ExpOut = sum;
	}
    }


/********************************************************************/
/* atan(r)                                                            */
/********************************************************************/

void CBigTrig::atan_bf(BigDouble* out, const BigDouble& n)
    {
    BigDouble x = n;
    BigDouble result;

    if (!out) return;

    EnsurePrecision(n);

    // Match precision for constants
    one.MatchPrecision(x);
    pi.MatchPrecision(x);
    half_pi.MatchPrecision(x);

    // Handle sign
    bool neg = mpfr_sgn(x.x) < 0;
    if (neg)
        x = -x;

    // Reduction for large x: atan(x) = pi/2 - atan(1/x)
    if (mpfr_cmp(x.x, one.x) > 0)
	{
        BigDouble inv = one / x;

        atan_bf(&result, inv);

        result = half_pi - result;
	}
    else
	{
        // Taylor series: atan(x) = x - x^3/3 + x^5/5 - ...
        BigDouble term = x;
        BigDouble sum = x;

        BigDouble x2 = x * x;

        bool subtract = true;

        for (int k = 3; k < 1000; k += 2)
	    {
            if (gStopRequested.load())
                break;

            term = term * x2;

            BigDouble div;
            div.MatchPrecision(x);
            mpfr_set_si(div.x, k, MPFR_RNDN);

            BigDouble delta = term / div;

            BigDouble newSum;

            if (subtract)
                newSum = sum - delta;
            else
                newSum = sum + delta;

            // Convergence check
            if (mpfr_cmp(newSum.x, sum.x) == 0)
                break;

            sum = newSum;
            subtract = !subtract;

#ifdef _DEBUG
//	    if (k > 997)
 //               OutputDebugStringA("atan_bf hit safety cap\n");
#endif
	    }

        result = sum;
	}

    // Restore sign
    if (neg)
        result = -result;

    *out = result;
    }

/********************************************************************/
/*  atan2(r,ny,nx)                                                       */
/********************************************************************/

void CBigTrig::atan2_bf(BigDouble* out, const BigDouble& y, const BigDouble& x)
    {
    BigDouble result;
    if (!out) return;

    EnsurePrecision(x);

    // Match precision
    one.MatchPrecision(x);
    pi.MatchPrecision(x);
    half_pi.MatchPrecision(x);

    // x > 0 -> atan(y/x)
    if (mpfr_sgn(x.x) > 0)
	{
	BigDouble ratio = y / x;
	atan_bf(&result, ratio);
	}
    // x < 0
    else if (mpfr_sgn(x.x) < 0)
	{
	BigDouble ratio = y / x;
	atan_bf(&result, ratio);

	if (mpfr_sgn(y.x) >= 0)
	    result = result + pi;
	else
	    result = result - pi;
	}
    // x == 0
    else
	{
	if (mpfr_sgn(y.x) > 0)
	    {
	    result = half_pi;
	    }
	else if (mpfr_sgn(y.x) < 0)
	    {
	    result = -half_pi;
	    }
	else
	    {
	    // undefined: atan2(0,0)
	    mpfr_set_zero(result.x, 0);
	    }
	}

    *out = result;
    }

/********************************************************************/
/* ln(r)                                                            */
/********************************************************************/

void CBigTrig::log_bf(BigDouble* out, const BigDouble& n)
    {
    BigDouble x = n;
    if (!out) return;

    EnsurePrecision(n);

    out->MatchPrecision(x);
    one.MatchPrecision(x);

    // Special cases
    if (mpfr_zero_p(x.x))
	{
	mpfr_set_inf(out->x, -1);
	return;
	}

    if (mpfr_sgn(x.x) < 0)
	{
	mpfr_set_nan(out->x);
	return;
	}

    // Extract exponent: x = m * 2^e
    long exp2 = mpfr_get_exp(x.x);

    BigDouble m;
    m.MatchPrecision(x);

    // m = x / 2^exp2  -> now m in [0.5, 1)
    mpfr_set(m.x, x.x, MPFR_RNDN);
    mpfr_div_2si(m.x, m.x, exp2, MPFR_RNDN);

    // log(x) = log(m) + exp2 * log(2)
    BigDouble logm;
    logm.MatchPrecision(x);

    mpfr_log(logm.x, m.x, MPFR_RNDN);

    BigDouble log2;
    log2.MatchPrecision(x);
    mpfr_const_log2(log2.x, MPFR_RNDN);

    BigDouble exp_term;
    exp_term.MatchPrecision(x);
    mpfr_mul_si(exp_term.x, log2.x, exp2, MPFR_RNDN);

    *out = logm + exp_term;
    }

/********************************************************************/
/* log10(r)                                                            */
/********************************************************************/

void CBigTrig::log10_bf(BigDouble* out, const BigDouble& n)
    {
    BigDouble ln;
    
    EnsurePrecision(n);
    ln.MatchPrecision(n);

    log_bf(&ln, n);

    BigDouble ten;
    ten.MatchPrecision(n);
    mpfr_set_ui(ten.x, 10, MPFR_RNDN);

    BigDouble ln10;
    ln10.MatchPrecision(n);
    mpfr_log(ln10.x, ten.x, MPFR_RNDN);

    *out = ln / ln10;
    }

/**************************************************************************
    sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/

void CBigTrig::CSin(BigComplex *SinVal, const BigComplex& val)
    {
    BigDouble	sin, cos, sinh, cosh;

    sin.MatchPrecision(val.y);
    cos.MatchPrecision(val.y);
    sinh.MatchPrecision(val.x);
    cosh.MatchPrecision(val.x);

    sincos_bf(&sin, &cos, val.x);
    sinhcosh_bf(&sinh, &cosh, val.y);
    SinVal->x = sin * cosh;
    SinVal->y = cos * sinh;
    }

/**************************************************************************
	cos(x+iy)  = cos(x)cosh(y) - isin(x)sinh(y)
***************************************************************************/

void CBigTrig::CCos(BigComplex *CosVal, const BigComplex& val)
    {
    BigDouble	sin, cos, sinh, cosh;

    sin.MatchPrecision(val.y);
    cos.MatchPrecision(val.y);
    sinh.MatchPrecision(val.x);
    cosh.MatchPrecision(val.x);

    sincos_bf(&sin, &cos, val.x);
    sinhcosh_bf(&sinh, &cosh, val.y);
    CosVal->x = cos * cosh;
    CosVal->y = -sin * sinh;
    }

/**************************************************************************
	Complex Exponent: e^(x+iy) = (e^x) * cos(y) + i * (e^x) * sin(y)
***************************************************************************/

void CBigTrig::CExp(BigComplex *ExpVal, const BigComplex& val)
    {
    BigDouble	sin, cos, exp;

    sin.MatchPrecision(val.y);
    cos.MatchPrecision(val.y);
    exp.MatchPrecision(val.x);

    sincos_bf(&sin, &cos, val.y);
    exp_bf(&exp, val.x);
    ExpVal->x = exp * cos;
    ExpVal->y = exp * sin;
    }

/**************************************************************************
	sinh(x+iy)  = sinh(x)cos(y) + icosh(x)sin(y)
***************************************************************************/

void CBigTrig::CSinh(BigComplex *SinhVal, const BigComplex& val)
    {
    BigDouble	sin, cos, sinh, cosh;

    sin.MatchPrecision(val.y);
    cos.MatchPrecision(val.y);
    sinh.MatchPrecision(val.x);
    cosh.MatchPrecision(val.x);

    sincos_bf(&sin, &cos, val.y);
    sinhcosh_bf(&sinh, &cosh, val.x);
    SinhVal->x = sinh * cos;
    SinhVal->y = cosh * sin;
    }

/**************************************************************************
	cosh(x+iy)  = cosh(x)cos(y) - isinh(x)sin(y)
***************************************************************************/

void CBigTrig::CCosh(BigComplex *CoshVal, const BigComplex& val)
    {
    BigDouble	sin, cos, sinh, cosh;

    sin.MatchPrecision(val.y);
    cos.MatchPrecision(val.y);
    sinh.MatchPrecision(val.x);
    cosh.MatchPrecision(val.x);

    sincos_bf(&sin, &cos, val.y);
    sinhcosh_bf(&sinh, &cosh, val.x);
    CoshVal->x = cosh * cos;
    CoshVal->y = -sinh * sin;
    }

