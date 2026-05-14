//////////////////////////////////////////////////////////////////////
//
// TrigFn.h: Trig Function list.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "..\ManpWIN64\complex.h"
#include "prototyp.h"

#undef DEBUG		// not sure how this got defined
#define rand15() rand()

//////////////////////////////////////////////////////////////////////
// Class definition
//////////////////////////////////////////////////////////////////////

class CTrigFn
    {
    public:
	void CMPLXtrig(Complex *zIn, Complex *zOut, int index);
	void DDCMPLXtrig(DDComplex *zIn, DDComplex *zOut, int index);
	void QDCMPLXtrig(QDComplex *zIn, QDComplex *zOut, int index);
	void HComplexTrig(_HCMPLX *h, _HCMPLX *out, int index);	    // extends the unary function f to *h1 
	void DDHComplexTrig(_DDHCMPLX *h, _DDHCMPLX *out, int index);
	void QDHComplexTrig(_QDHCMPLX *h, _QDHCMPLX *out, int index); 

	int  FindFunct(const char *Str, int NumFns);

	const char *FunctList[37] = {"sin", "sinh", "cos", "cosh", "sqr", "log", "exp", "abs", "conj", "real",
					"imag", "ident", "recip", "fn1", "fn2", "fn3", "fn4", "flip", "tan", "tanh", 
					"cotan", "cotanh", "cosxx", "srand", "asin", "asinh", "acos", "acosh", "atan", "atanh", 
					"sqrt", "cabs", "floor", "ceil", "trunc", "round", ""};
    };




