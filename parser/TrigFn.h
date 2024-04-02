//////////////////////////////////////////////////////////////////////
//
// TrigFn.h: Trig Function list.
//
//////////////////////////////////////////////////////////////////////
#pragma once

//#include <Windows.h>
//#include <time.h>
#include "..\complex.h"
//#include "Fractype.h"
#include "prototyp.h"

#undef DEBUG		// not sure how this got defined
#define rand15() rand()
#define Sqrtz(z,rz) (*(rz) = ComplexSqrtFloat((z).x, (z).y))


//////////////////////////////////////////////////////////////////////
// Class definition
//////////////////////////////////////////////////////////////////////

class CTrigFn
    {
    public:
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

	int  FindFunct(char *Str, int NumFns);

	void CMPLXtrig(Complex *zIn, Complex *zOut, int index);
	void HComplexTrig(_HCMPLX *h, _HCMPLX *out, int index);	    // extends the unary function f to *h1 


	char *FunctList[37] = {"sin", "sinh", "cos", "cosh", "sqr", "log", "exp", "abs", "conj", "real",
					"imag", "ident", "recip", "fn1", "fn2", "fn3", "fn4", "flip", "tan", "tanh", 
					"cotan", "cotanh", "cosxx", "srand", "asin", "asinh", "acos", "acosh", "atan", "atanh", 
					"sqrt", "cabs", "floor", "ceil", "trunc", "round", ""};

    private:
	void FPUsincos(double *Angle, double *Sin, double *Cos);
	void FPUsinhcosh(double *Angle, double *Sinh, double *Cosh);
	void FPUcplxlog(Complex *x, Complex *z);
	void FPUcplxexp(Complex *x, Complex *z);
	void Arcsinz(Complex z, Complex *rz);
	void Arccosz(Complex z, Complex *rz);
	void Arcsinhz(Complex z, Complex *rz);
	void Arccoshz(Complex z, Complex *rz);
	void Arctanhz(Complex z, Complex *rz);
	void Arctanz(Complex z, Complex *rz);
	Complex ComplexSqrtFloat(double x, double y);
    };


/*

struct FNCT_LIST FnctList[] = {
	{ s_sin,   &StkSin },
	{ s_sinh,  &StkSinh },
	{ s_cos,   &StkCos },
	{ s_cosh,  &StkCosh },
	{ s_sqr,   &StkSqr },
	{ s_log,   &StkLog },
	{ s_exp,   &StkExp },
	{ s_abs,   &StkAbs },
	{ s_conj,  &StkConj },
	{ s_real,  &StkReal },
	{ s_imag,  &StkImag },
	{ s_ident, &dStkIdent },
	{ s_recip, &StkRecip },
	{ s_fn1,   &StkTrig0 },   // TIW 03-30-91
	{ s_fn2,   &StkTrig1 },   // TIW 03-30-91
	{ s_fn3,   &StkTrig2 },   // TIW 03-30-91
	{ s_fn4,   &StkTrig3 },   // TIW 03-30-91
	{ s_flip,  &StkFlip },    // MCP 4-9-91
	{ s_tan,   &StkTan },     // TIW 04-22-91
	{ s_tanh,  &StkTanh },    // TIW 04-22-91
	{ s_cotan, &StkCoTan },   // TIW 04-24-91
	{ s_cotanh,&StkCoTanh },  // TIW 04-24-91
	{ s_cosxx, &StkCosXX },   // PB  04-28-91
	{ s_srand, &StkSRand },   // MCP 11-21-91
	{ s_asin,  &StkASin },    // TIW 11-26-94
	{ s_asinh, &StkASinh },   // TIW 11-26-94
	{ s_acos,  &StkACos },    // TIW 11-26-94
	{ s_acosh, &StkACosh },   // TIW 11-26-94
	{ s_atan,  &StkATan },    // TIW 11-26-94
	{ s_atanh, &StkATanh },   // TIW 11-26-94
	{ s_sqrt,  &StkSqrt },    // TIW 11-26-94
	{ s_cabs,  &StkCAbs },    // TIW 11-26-94
	{ s_floor, &StkFloor },   // TIW 06-30-96
	{ s_ceil,  &StkCeil },    // TIW 06-30-96
	{ s_trunc, &StkTrunc },   // TIW 06-30-96
	{ s_round, &StkRound },   // TIW 06-30-96
	{ "", NULL },		    // PHD 2009-10-15
    };
*/
/*
char s_sin [] =             "sin";
char s_cos [] =             "cos";
char s_cosh [] =            "cosh";
char s_sqr [] =             "sqr";
char s_log [] =             "log";
char s_exp [] =             "exp";
char s_abs [] =             "abs";
char s_conj [] =            "conj";
char s_fn1 [] =             "fn1";
char s_fn2 [] =             "fn2";
char s_fn3 [] =             "fn3";
char s_fn4 [] =             "fn4";
char s_flip [] =            "flip";
char s_floor [] =           "floor";
char s_ceil [] =            "ceil";
char s_trunc [] =           "trunc";
char s_round [] =           "round";
char s_tan [] =             "tan";
char s_tanh [] =            "tanh";
char s_cotan [] =           "cotan";
char s_cotanh [] =          "cotanh";
char s_cosxx [] =           "cosxx";
char s_srand [] =           "srand";
char s_recip [] =           "recip";
char s_ident [] =           "ident";
char s_zero [] =            "zero";
char s_one  [] =            "one";
char s_asin [] =            "asin";
char s_asinh [] =           "asinh";
char s_acos [] =            "acos";
char s_acosh [] =           "acosh";
char s_atanh [] =           "atanh";
char s_cabs [] =            "cabs";
char s_sqrt [] =            "sqrt";
char s_ismand [] =          "ismand";

*/




