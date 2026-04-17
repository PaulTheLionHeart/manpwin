#include <string.h>
#include "DDComplex.h"

#pragma once
class CDDTrig
    {
    public:
	void	sincos_dd(dd_real *s, dd_real *c, dd_real n);
	void	sinhcosh_dd(dd_real *s, dd_real *c, dd_real n);
/*
	void	exp_bf(dd_real *exp, dd_real n);
//	void	ln_bf(dd_real *r, dd_real n);

	void	CSin(DDComplex *Sinx, DDComplex x);
	void	CCos(DDComplex *Cosx, DDComplex x);
	void	CExp(DDComplex *CosVal, DDComplex val);
	void	CSinh(DDComplex *SinhVal, DDComplex val);
	void	CCosh(DDComplex *CoshVal, DDComplex val);
*/
//    private:
//	static bool NeedsMpfrTrig(const dd_real& x);
    };

