#include <string.h>
#include "big.h"
#include "BigDouble.h"
#include "BigComplex.h"

#pragma once

class CBigTrig
    {
    public:
	// Precision control
	void SetPrecision(const BigDouble& ref);

	// Real functions
	void exp_bf(BigDouble* result, const BigDouble& x);
	void sincos_bf(BigDouble *s, BigDouble *c, const BigDouble& n);
	void sinhcosh_bf(BigDouble* s, BigDouble* c, const BigDouble& x);
	void atan_bf(BigDouble* out, const BigDouble& n);
	void atan2_bf(BigDouble* out, const BigDouble& y, const BigDouble& x);
	void log_bf(BigDouble* out, const BigDouble& n);
	void log10_bf(BigDouble* out, const BigDouble& n);

	// Complex (we'll expand later)
	void CSin(BigComplex* result, const BigComplex& z);
	void CCos(BigComplex* result, const BigComplex& z);
	void CExp(BigComplex* result, const BigComplex& z);
	void CSinh(BigComplex* SinhVal, const BigComplex& val);
	void CCosh(BigComplex* CoshVal, const BigComplex& val);

    private:
	void EnsurePrecision(const BigDouble& ref);

	// Precision
	int precision = 0;

	// Constants
	BigDouble pi;
	BigDouble half_pi;
	BigDouble two_pi;
	BigDouble quarter_pi;

	BigDouble one, two, zero;
	BigDouble inv_two;	// 0.5

	// Helpers
	void InitConstants();
    };



