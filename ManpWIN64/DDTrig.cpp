/*********************************************************************
	DDTrig.cpp - C routines for big floating point trig functions 
	Wesley Loewer's Big Numbers.     (C) 1994-95, Wesley B. Loewer

	WThis allows an alternative for multi-threading where trig 
	functions fail using double-double arithmetic
*********************************************************************/

#include <Windows.h>
#include <atomic>
#include "DDTrig.h"

extern	std::atomic<bool> gStopRequested;	// force early exit

/********************************************************************/
/* sincos_bf(r)                                                     */
/********************************************************************/

void CDDTrig::sincos_dd(dd_real* s, dd_real* c, dd_real n)
    {

    //    OutputDebugStringA(("QD sin input: " + to_string(n) + "\n").c_str());


	// --- Step 1: reduce to [-pi, pi]
    //    qd_real x = fmod(n, qd_real::_2pi);
    dd_real k = floor(n / dd_real::_2pi/* + qd_real(0.5)*/);
    dd_real x = n - k * dd_real::_2pi;

    if (x > dd_real::_pi)  x -= dd_real::_2pi;
    if (x < -dd_real::_pi) x += dd_real::_2pi;


    if (abs(x) > 10)
	{
	OutputDebugStringA("QD reduction FAILED\n");
	}

    // --- Step 2: reduce to [-pi/2, pi/2]
    bool flip_cos = false;

    if (x > dd_real::_pi2)
	{
	x = dd_real::_pi - x;
	flip_cos = true;
	}
    else if (x < -dd_real::_pi2)
	{
	x = -dd_real::_pi - x;
	flip_cos = true;
	}

    // --- Step 3: reduce further to [-pi/8, pi/8]
    dd_real xr = x * dd_real(0.125);   // x / 8

    // --- Step 4: Taylor on very small angle
    dd_real xr2 = xr * xr;

    *s = xr;
    *c = 1.0;

    dd_real sin_term = xr;
    dd_real cos_term = 1.0;

    for (int i = 1; i <= 12; i++)   // fewer terms now!
	{
	// cosine
	dd_real denom_cos = dd_real((2 * i - 1) * (2 * i));
	cos_term *= -xr2;
	cos_term /= denom_cos;
	*c += cos_term;

	// sine
	dd_real denom_sin = dd_real((2 * i) * (2 * i + 1));
	sin_term *= -xr2;
	sin_term /= denom_sin;
	*s += sin_term;
	/*
	if (abs(*s) < qd_real("1e-50") && abs(*c - 1) < qd_real("1e-50"))
	    OutputDebugStringA("QD trig collapsed to identity\n");
	*/
	}

    // --- Step 5: rebuild angle (x = xr * 8)
    // 3x double-angle steps

    for (int i = 0; i < 3; i++)
	{
	dd_real s2 = 2.0 * *s * *c;
	dd_real c2 = 2.0 * *c * *c - 1.0;
	*s = s2;
	*c = c2;
	}

    // --- Step 6: restore quadrant
    if (flip_cos)
	*c = -*c;
    }

/********************************************************************/
/* sinhcosh_bf(r)                                                   */
/********************************************************************/

void CDDTrig::sinhcosh_dd(dd_real* s, dd_real* c, dd_real n)
    {
    dd_real zero = dd_real(0.0);
    dd_real one = dd_real(1.0);
    dd_real half = dd_real(0.5);

    if (n == zero)
	{
	*s = zero;
	*c = one;
	return;
	}

    dd_real exp_pos, exp_neg;

    // interrupt before heavy work
    if (gStopRequested.load())
	return;

    exp_pos = exp(n);

    // interrupt between calls
    if (gStopRequested.load())
	return;

    exp_neg = exp(-n);

    // optional final check
    if (gStopRequested.load())
	return;

    // sinh = (e^x - e^-x)/2
    *s = (exp_pos - exp_neg) * half;

    // cosh = (e^x + e^-x)/2
    *c = (exp_pos + exp_neg) * half;
    }

