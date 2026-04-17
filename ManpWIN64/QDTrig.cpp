/*********************************************************************
	DDTrig.cpp - C routines for big floating point trig functions 
	Wesley Loewer's Big Numbers.     (C) 1994-95, Wesley B. Loewer

	WThis allows an alternative for multi-threading where trig 
	functions fail using double-double arithmetic
*********************************************************************/

#include <Windows.h>
#include <atomic>
#include "QDTrig.h"

extern std::atomic<bool> gStopRequested;

/********************************************************************/
/* sincos_bf(r)                                                     */
/********************************************************************/

void CQDTrig::sincos_qd(qd_real* s, qd_real* c, qd_real n)
    {

//    OutputDebugStringA(("QD sin input: " + to_string(n) + "\n").c_str());


    // --- Step 1: reduce to [-pi, pi]
//    qd_real x = fmod(n, qd_real::_2pi);
    qd_real k = floor(n / qd_real::_2pi/* + qd_real(0.5)*/);
    qd_real x = n - k * qd_real::_2pi;

    if (x > qd_real::_pi)  x -= qd_real::_2pi;
    if (x < -qd_real::_pi) x += qd_real::_2pi;


    if (abs(x) > 10)
	{
	OutputDebugStringA("QD reduction FAILED\n");
	}

    // --- Step 2: reduce to [-pi/2, pi/2]
    bool flip_cos = false;

    if (x > qd_real::_pi2)
	{
	x = qd_real::_pi - x;
	flip_cos = true;
	}
    else if (x < -qd_real::_pi2)
	{
	x = -qd_real::_pi - x;
	flip_cos = true;
	}

    // --- Step 3: reduce further to [-pi/8, pi/8]
    qd_real xr = x * qd_real(0.125);   // x / 8

    // --- Step 4: Taylor on very small angle
    qd_real xr2 = xr * xr;

    *s = xr;
    *c = 1.0;

    qd_real sin_term = xr;
    qd_real cos_term = 1.0;

    for (int i = 1; i <= 12; i++)   // fewer terms now!
	{
	// cosine
	qd_real denom_cos = qd_real((2 * i - 1) * (2 * i));
	cos_term *= -xr2;
	cos_term /= denom_cos;
	*c += cos_term;

	// sine
	qd_real denom_sin = qd_real((2 * i) * (2 * i + 1));
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
	qd_real s2 = 2.0 * *s * *c;
	qd_real c2 = 2.0 * *c * *c - 1.0;
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

void CQDTrig::sinhcosh_qd(qd_real* s, qd_real* c, const qd_real& n)
    {
    qd_real zero = qd_real(0.0);
    qd_real one = qd_real(1.0);
    qd_real half = qd_real(0.5);

    if (n == zero)
	{
	*s = zero;
	*c = one;
	return;
	}

    qd_real exp_pos, exp_neg;

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


