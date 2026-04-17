#include <string.h>
#include "QDComplex.h"

#pragma once
class CQDTrig
    {
    public:
	void	sincos_qd(qd_real *s, qd_real *c, qd_real n);
	void	sinhcosh_qd(qd_real *s, qd_real *c, const qd_real &n);
/*
	void	exp_bf(qd_real *exp, qd_real n);
//	void	ln_bf(qd_real *r, qd_real n);

	void	CSin(QDComplex *Sinx, QDComplex x);
	void	CCos(QDComplex *Cosx, QDComplex x);
	void	CExp(QDComplex *CosVal, QDComplex val);
	void	CSinh(QDComplex *SinhVal, QDComplex val);
	void	CCosh(QDComplex *CoshVal, QDComplex val);

    private:
	static bool NeedsMpfrTrig(const qd_real& x);
*/
    };

