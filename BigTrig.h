#include <string.h>
#include "big.h"
#include "BigDouble.h"
#include "BigComplex.h"

#pragma once
class CBigTrig
    {
    public:
	void	sincos_bf(BigDouble *s, BigDouble *c, BigDouble n);
	void	sinhcosh_bf(BigDouble *s, BigDouble *c, BigDouble n);
	void	exp_bf(BigDouble *exp, BigDouble n);
//	void	ln_bf(BigDouble *r, BigDouble n);

	void	CSin(BigComplex *Sinx, BigComplex x);
	void	CCos(BigComplex *Cosx, BigComplex x);
	void	CExp(BigComplex *CosVal, BigComplex val);
	void	CSinh(BigComplex *SinhVal, BigComplex val);
	void	CCosh(BigComplex *CoshVal, BigComplex val);

    private:
	BigDouble	bftmp1, bftmp2, zero = 0.0;
	BigDouble	Pi;
	BigDouble	delta = 10e-25;
	BigDouble	one = 1.0;
	BigDouble	sin, cos, sinh, cosh, exp;
    };

