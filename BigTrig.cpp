/*********************************************************************
	BigTrig.cpp - C routines for big floating point trig functions 
	Wesley Loewer's Big Numbers.     (C) 1994-95, Wesley B. Loewer

	WThis allows an alternative for multi-threading where trig 
	functions fail using MPFR
*********************************************************************/

#include "BigTrig.h"

/********************************************************************/
/* sincos_bf(r)                                                     */
/********************************************************************/

void	CBigTrig::sincos_bf(BigDouble *s, BigDouble *c, BigDouble n)
    {
    U16		fact = 2;
    int		k = 0, i, halves;
    int		signcos = 0, signsin = 0, switch_sincos = 0;
    int		sin_done = 0, cos_done = 0;

    mpfr_const_pi(Pi.x, MPFR_RNDN);

    // assure range 0 <= x < pi/4 
    if (n == zero)
	{
	*s = 0.0;	// sin(0) = 0 
	*c = 1.0;	// cos(0) = 1 
	return;
	}

    if (n < zero)
	{
	signsin = !signsin; // sin(-x) = -sin(x), odd; cos(-x) = cos(x), even 
	n = -n;
	}

    // n >= 0 
    bftmp1 = Pi * 2;	// 2*pi 

    // this could be done with remainders, but it would probably be slower 
    while (n > bftmp1 || n == bftmp1)	// while n >= 2*pi 
	{
	bftmp2 = bftmp1;
	n = n - bftmp2;
	}

    // 0 <= n < 2*pi 
    bftmp1 = Pi;
    if (n > bftmp1 || n == bftmp1)	// if n >= pi
	{
	n = n - bftmp1;
	signsin = !signsin;
	signcos = !signcos;
	}

    // 0 <= n < pi 
    bftmp1 = Pi / 2;
    if (n > bftmp1)			// if n > pi/2
	{
	bftmp2 = Pi;
	n = bftmp2 - n;
	signcos = !signcos;
	}

    // 0 <= n < pi/2
    bftmp1 = Pi / 4;
    if (n > bftmp1)			// if n > pi/2
	{
	bftmp2 = n;
	bftmp1 = Pi / 2;			// pi/2
	n = bftmp1 - n;			// pi/2 - n;
	switch_sincos = !switch_sincos;
	}
 
 // 0 <= n < pi/4
     // this looks redundant, but n could now be zero when it wasn't before 
    if (n == zero)
	{
	*s = 0.0;	// sin(0) = 0 
	*c = 1.0;	// cos(0) = 1 
	return;
	}

    // at this point, the double angle trig identities could be used as many times as desired to reduce the range to pi/8, pi/16, etc...  
    // Each time the range is cut in half, the number of iterations required is reduced by "quite a bit."  
    // It's just a matter of testing to see what gives the optimal results. 

    // halves = bflength / 10;  /// this is experimental 
    halves = 1;
    for (i = 0; i < halves; i++)
	n = n / 2;

    // use Taylor Series (very slow convergence) 
    *s = n;		// start with s=n 
    *c = 1.0;		// start with c=1
    bftmp1 = n;		// the current x^n/n! 

    for (int i = 0; i < 40; i++)	// just for safety
	{
	// even terms for cosine 
	bftmp2 = bftmp1;
	bftmp1 = bftmp2 * n;
	bftmp1 = bftmp1 / fact++;

	if (!cos_done)
	    {
	    //	    cos_done = (abs(bftmp1 - *c) < delta); // too small to register
	    cos_done = (bftmp1.BigAbs() < delta); // too small to register
	    if (!cos_done)
		{
		if (k)			// alternate between adding and subtracting
		    *c = *c + bftmp1;
		else
		    *c = *c - bftmp1;
		}
	    }

	// odd terms for sine
	bftmp2 = bftmp1;
	bftmp1 = bftmp2 * n;
	bftmp1 = bftmp1 / fact++;

	if (!sin_done)
	    {
	    sin_done = (bftmp1.BigAbs() < delta); // too small to register
	    if (!sin_done)
		{
		if (k)				// alternate between adding and subtracting
		    *s = *s + bftmp1;
		else
		    *s = *s - bftmp1;
		}
	    }
	k = !k; // toggle 
	if (cos_done && sin_done)
	    break;
	} 

    // now need to undo what was done by cutting angles in half
    for (i = 0; i < halves; i++)
	{
	*s = *s * *c * 2;			// sin(2x) = 2*sin(x)*cos(x)
	*c = c->BigSqr() * 2 - 1.0;		// cos(2x) = 2 * cos(x)*cos(x) - 1
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

void	CBigTrig::CBigTrig::sinhcosh_bf(BigDouble *s, BigDouble *c, BigDouble n)
    {
    U16		fact = 2;
    int		i, sinh_done = 0, cosh_done = 0;

    // use Taylor Series (very slow convergence) 
    *s = n;		// start with s=n 
    *c = 1.0;		// start with c=1
    bftmp1 = n;		// the current x^n/n! 
    int precision = bftmp1.bitcount / 10;

    for (i = 0; i < 40; i++)	// just for safety
	{
	// even terms for cosh 
	bftmp2 = bftmp1;
	bftmp1 = bftmp2 * n;
	bftmp1 = bftmp1 / fact++;
	if (!cosh_done)
	    {
	    cosh_done = (bftmp1.BigAbs() < delta);	// too small to register
	    if (!cosh_done)
		*c = *c + bftmp1;
	    }

	// odd terms for sinh
	bftmp2 = bftmp1;
	bftmp1 = bftmp2 * n;
	bftmp1 = bftmp1 / fact++;
	if (!sinh_done)
	    {
	    sinh_done = (bftmp1.BigAbs() < delta);	// too small to register
	    if (!sinh_done)
		*s = *s + bftmp1;
	    }
	if (cosh_done && sinh_done)
	    break;
	} 
    }

/********************************************************************/
/* exp_bf(r)                                                   */
/********************************************************************/

void	CBigTrig::exp_bf(BigDouble *ExpOut, BigDouble n)
    {
    U16		fact = 1;
    int		i, exp_done = 0;
    bool	positive = true;

    if (n < zero)
	{
	n = -n;
	positive = false;
	}
    // use Taylor Series (very slow convergence) 
    *ExpOut = 1.0;		// start with c=1
    bftmp1 = 1.0;		// the current x^n/n! 
    for (i = 0; i < 40; i++)	// just for safety
	{
	bftmp2 = bftmp1;
	bftmp1 = bftmp2 * n;
	bftmp1 = bftmp1 / fact++;
	if (!exp_done)
	    {
	    exp_done = (bftmp1.BigAbs() < delta);	// too small to register
	    if (!exp_done)
		*ExpOut = *ExpOut + bftmp1;
	    }
	if (exp_done)
	    break;
	}
    if (!positive)
	*ExpOut = one / *ExpOut;
    }

/********************************************************************/
/* ln(r)                                                            */
/********************************************************************/

/*
void	CBigTrig::ln_bf(BigDouble *r, BigDouble n)
    {
    int		i, ln_done = 0;
    double	f;
    BigDouble	bftmp1, bftmp2, bftmp3, bftmp4, bftmp5, bftmp6, zero = 0.0, OldR;

    BigDouble   delta = 10e-10;

    // use Newton's recursive method for zeroing in on ln(n): r=r+n*exp(-r)-1
    if (n < zero || n == zero)
	{				// error, return largest neg value 
	*r = -10e+300;
	return;
	}

    f = n.BigDoubleToDouble();
    f = logl(f);			// approximate ln(x) 
    // no need to check overflow
    // appears to be ok, do ln

    // With Newton's Method, there is no need to calculate all the digits every time.  The precision approximately doubles each iteration.   

    *r = f;		// start with approximate ln and get negative: -r
    bftmp5 = -*r;	// -r 
//    OldR = -*r;
    for (i = 0; i < 20; i++)	// safety net, this shouldn't ever be needed
	{
	exp_bf(&bftmp6, *r);				// exp(-r)
	bftmp2 = bftmp6 * n;				// n*exp(-r)
	bftmp2 = bftmp2 - 1.0;				// n*exp(-r) - 1
	*r = *r - bftmp2;				// -r - n*exp(-r) - 1
	if (!ln_done)
	    {
//	    bftmp4 = *r - bftmp5;
	    double tmp3 = (double)mpfr_get_d(bftmp3.x, MPFR_RNDN);
	    double tmp4 = (double)mpfr_get_d(bftmp4.x, MPFR_RNDN);
	    double D = (double)mpfr_get_d(delta.x, MPFR_RNDN);
	    double R = (double)mpfr_get_d((*r).x, MPFR_RNDN);


	    ln_done = ((*r - bftmp5).BigAbs() < delta);	// too small to register
	    bftmp5 = *r;				// -r
	    }
	if (ln_done)
	    break;
	}

    *r = -*r;				// -(-r) 
    }
*/

/**************************************************************************
    sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/

void	CBigTrig::CSin(BigComplex *SinVal, BigComplex val)
    {
    sincos_bf(&sin, &cos, val.x);
    sinhcosh_bf(&sinh, &cosh, val.y);
    SinVal->x = sin * cosh;
    SinVal->y = cos * sinh;
    }

/**************************************************************************
	cos(x+iy)  = cos(x)cosh(y) - isin(x)sinh(y)
***************************************************************************/

void	CBigTrig::CCos(BigComplex *CosVal, BigComplex val)
    {
    sincos_bf(&sin, &cos, val.x);
    sinhcosh_bf(&sinh, &cosh, val.y);
    CosVal->x = cos * cosh;
    CosVal->y = -sin * sinh;
    }

/**************************************************************************
	Complex Exponent: e^(x+iy) = (e^x) * cos(y) + i * (e^x) * sin(y)
***************************************************************************/

void	CBigTrig::CExp(BigComplex *ExpVal, BigComplex val)
    {
    sincos_bf(&sin, &cos, val.y);
    exp_bf(&exp, val.x);
    ExpVal->x = exp * cos;
    ExpVal->y = exp * sin;
    }

/**************************************************************************
	sinh(x+iy)  = sinh(x)cos(y) + icosh(x)sin(y)
***************************************************************************/

void	CBigTrig::CSinh(BigComplex *SinhVal, BigComplex val)
    {
    sincos_bf(&sin, &cos, val.y);
    sinhcosh_bf(&sinh, &cosh, val.x);
    SinhVal->x = sinh * cos;
    SinhVal->y = cosh * sin;
    }

/**************************************************************************
	cosh(x+iy)  = cosh(x)cos(y) - isinh(x)sin(y)
***************************************************************************/

void	CBigTrig::CCosh(BigComplex *CoshVal, BigComplex val)
    {
    sincos_bf(&sin, &cos, val.y);
    sinhcosh_bf(&sinh, &cosh, val.x);
    CoshVal->x = cosh * cos;
    CoshVal->y = -sinh * sin;
    }

