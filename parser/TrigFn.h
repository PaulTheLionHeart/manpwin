//////////////////////////////////////////////////////////////////////
//
// TrigFn.h: Trig Function list.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "..\complex.h"
#include "prototyp.h"

#undef DEBUG		// not sure how this got defined
#define rand15() rand()

//////////////////////////////////////////////////////////////////////
// Class definition
//////////////////////////////////////////////////////////////////////

class CTrigFn
    {
    public:
	// float routines
	void dStkSin(Complex *z);
	void dStkSinh(Complex *z);
	void dStkCos(Complex *z);
	void dStkCosh(Complex *z);
	void dStkSqr(Complex *z);
	void dStkLog(Complex *z);
	void dStkExp(Complex *z);
	void dStkAbs(Complex *z);
	void dStkConj(Complex *z);
	void dStkReal(Complex *z);
	void dStkImag(Complex *z);
	void dStkIdent(Complex *z);
	void dStkRecip(Complex *z);
	void dStkFlip(Complex *z);
	void dStkTan(Complex *z);
	void dStkTanh(Complex *z);

	void dStkCoTan(Complex *z);
	void dStkCoTanh(Complex *z);
	void dStkCosXX(Complex *z);
	void dStkSRand(Complex *z);
	void dStkASin(Complex *z);
	void dStkASinh(Complex *z);
	void dStkACos(Complex *z);
	void dStkACosh(Complex *z);

	void dStkATan(Complex *z);
	void dStkATanh(Complex *z);
	void dStkSqrt(Complex *z);
	void dStkCAbs(Complex *z);

	void dStkFloor(Complex *z);
	void dStkCeil(Complex *z);
	void dStkTrunc(Complex *z);
	void dStkRound(Complex *z);

	// double double routines
	void ddStkSin(DDComplex *z);
	void ddStkSinh(DDComplex *z);
	void ddStkCos(DDComplex *z);
	void ddStkCosh(DDComplex *z);
	void ddStkSqr(DDComplex *z);
	void ddStkLog(DDComplex *z);
	void ddStkExp(DDComplex *z);
	void ddStkAbs(DDComplex *z);
	void ddStkConj(DDComplex *z);
	void ddStkReal(DDComplex *z);
	void ddStkImag(DDComplex *z);
	void ddStkIdent(DDComplex *z);
	void ddStkRecip(DDComplex *z);
	void ddStkFlip(DDComplex *z);
	void ddStkTan(DDComplex *z);
	void ddStkTanh(DDComplex *z);

	void ddStkCoTan(DDComplex *z);
	void ddStkCoTanh(DDComplex *z);
	void ddStkCosXX(DDComplex *z);
	void ddStkSRand(DDComplex *z);
	void ddStkASin(DDComplex *z);
	void ddStkASinh(DDComplex *z);
	void ddStkACos(DDComplex *z);
	void ddStkACosh(DDComplex *z);

	void ddStkATan(DDComplex *z);
	void ddStkATanh(DDComplex *z);
	void ddStkSqrt(DDComplex *z);
	void ddStkCAbs(DDComplex *z);

	void ddStkFloor(DDComplex *z);
	void ddStkCeil(DDComplex *z);
	void ddStkTrunc(DDComplex *z);
	void ddStkRound(DDComplex *z);

	// quad double routines
	void qdStkSin(QDComplex *z);
	void qdStkSinh(QDComplex *z);
	void qdStkCos(QDComplex *z);
	void qdStkCosh(QDComplex *z);
	void qdStkSqr(QDComplex *z);
	void qdStkLog(QDComplex *z);
	void qdStkExp(QDComplex *z);
	void qdStkAbs(QDComplex *z);
	void qdStkConj(QDComplex *z);
	void qdStkReal(QDComplex *z);
	void qdStkImag(QDComplex *z);
	void qdStkIdent(QDComplex *z);
	void qdStkRecip(QDComplex *z);
	void qdStkFlip(QDComplex *z);
	void qdStkTan(QDComplex *z);
	void qdStkTanh(QDComplex *z);

	void qdStkCoTan(QDComplex *z);
	void qdStkCoTanh(QDComplex *z);
	void qdStkCosXX(QDComplex *z);
	void qdStkSRand(QDComplex *z);
	void qdStkASin(QDComplex *z);
	void qdStkASinh(QDComplex *z);
	void qdStkACos(QDComplex *z);
	void qdStkACosh(QDComplex *z);

	void qdStkATan(QDComplex *z);
	void qdStkATanh(QDComplex *z);
	void qdStkSqrt(QDComplex *z);
	void qdStkCAbs(QDComplex *z);

	void qdStkFloor(QDComplex *z);
	void qdStkCeil(QDComplex *z);
	void qdStkTrunc(QDComplex *z);
	void qdStkRound(QDComplex *z);

	void CMPLXtrig(Complex *zIn, Complex *zOut, int index);
	void DDCMPLXtrig(DDComplex *zIn, DDComplex *zOut, int index);
	void QDCMPLXtrig(QDComplex *zIn, QDComplex *zOut, int index);
	void HComplexTrig(_HCMPLX *h, _HCMPLX *out, int index);	    // extends the unary function f to *h1 
	void DDHComplexTrig(_DDHCMPLX *h, _DDHCMPLX *out, int index);
	void QDHComplexTrig(_QDHCMPLX *h, _QDHCMPLX *out, int index); 

	// float routines
	void Arcsinz(Complex z, Complex *rz);
	void Arccosz(Complex z, Complex *rz);
	void Arcsinhz(Complex z, Complex *rz);
	void Arccoshz(Complex z, Complex *rz);
	void Arctanhz(Complex z, Complex *rz);
	void Arctanz(Complex z, Complex *rz);
	Complex ComplexSqrtFloat(double x, double y);
	int  FindFunct(char *Str, int NumFns);

	// double double routines
	void DDArcsinz(DDComplex z, DDComplex *rz);
	void DDArccosz(DDComplex z, DDComplex *rz);
	void DDArcsinhz(DDComplex z, DDComplex *rz);
	void DDArccoshz(DDComplex z, DDComplex *rz);
	void DDArctanhz(DDComplex z, DDComplex *rz);
	void DDArctanz(DDComplex z, DDComplex *rz);

	// quad double routines
	void QDArcsinz(QDComplex z, QDComplex *rz);
	void QDArccosz(QDComplex z, QDComplex *rz);
	void QDArcsinhz(QDComplex z, QDComplex *rz);
	void QDArccoshz(QDComplex z, QDComplex *rz);
	void QDArctanhz(QDComplex z, QDComplex *rz);
	void QDArctanz(QDComplex z, QDComplex *rz);

	char *FunctList[37] = {"sin", "sinh", "cos", "cosh", "sqr", "log", "exp", "abs", "conj", "real",
					"imag", "ident", "recip", "fn1", "fn2", "fn3", "fn4", "flip", "tan", "tanh", 
					"cotan", "cotanh", "cosxx", "srand", "asin", "asinh", "acos", "acosh", "atan", "atanh", 
					"sqrt", "cabs", "floor", "ceil", "trunc", "round", ""};

    private:
	void FPUsincos(double *Angle, double *Sin, double *Cos);
	void FPUsinhcosh(double *Angle, double *Sinh, double *Cosh);
	void FPUcplxlog(Complex *x, Complex *z);
	void FPUcplxexp(Complex *x, Complex *z);
    };




