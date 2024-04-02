//////////////////////////////////////////////////////////////////////
//
// TrigFn.cpp: Function list.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <time.h>
#include "complex.h"
#include "TrigFn.h"

void CTrigFn::FPUsincos(double *Angle, double *Sin, double *Cos)
    {
    *Sin = sin(*Angle);
    *Cos = cos(*Angle);
    }

void CTrigFn::FPUsinhcosh(double *Angle, double *Sinh, double *Cosh)
    {
    *Sinh = sinh(*Angle);
    *Cosh = cosh(*Angle);
    }

void CTrigFn::FPUcplxlog(Complex *x, Complex *z)
    {
    double mod, zx, zy;
    mod = sqrt(x->x*x->x + x->y*x->y);
    zx = log(mod);
    zy = atan2(x->y, x->x);

    z->x = zx;
    z->y = zy;
    }

void CTrigFn::FPUcplxexp(Complex *x, Complex *z)
    {
    double e2x, siny, cosy;

    e2x = exp(x->x);
    FPUsincos(&x->y, &siny, &cosy);
    z->x = e2x * cosy;
    z->y = e2x * siny;
    }
  
void CTrigFn::HComplexTrig(_HCMPLX *h, _HCMPLX *out, int index)		/* extends the unary function f to *h1 */
    {
    /* This is the whole beauty of Hypercomplex numbers - *ANY* unary
    complex valued function of a complex variable can easily
    be generalized to hypercomplex numbers */

    Complex a, b, resulta, resultb;

    /* convert to duplex form */
    a.x = h->x - h->t;
    a.y = h->y + h->z;
    b.x = h->x + h->t;
    b.y = h->y - h->z;

    /* apply function to each part */
    CMPLXtrig(&a, &resulta, index);
    CMPLXtrig(&b, &resultb, index);

    /* convert back */
    out->x = (resulta.x + resultb.x) / 2;
    out->y = (resulta.y + resultb.y) / 2;
    out->z = (resulta.y - resultb.y) / 2;
    out->t = (resultb.x - resulta.x) / 2;
    }

// The following Complex function routines added by Tim Wegner November 1994.

void CTrigFn::Arcsinz(Complex z, Complex *rz)		    /* rz=Arcsin(z)=-i*Log{i*z+sqrt(1-z*z)} */
    {
    Complex tempz1, tempz2;

    FPUcplxmul(&z, &z, &tempz1);
    tempz1.x = 1 - tempz1.x; tempz1.y = -tempz1.y;  /* tempz1 = 1 - tempz1 */
    Sqrtz(tempz1, &tempz1);

    tempz2.x = -z.y; tempz2.y = z.x;                /* tempz2 = i*z  */
    tempz1.x += tempz2.x;  tempz1.y += tempz2.y;    /* tempz1 += tempz2 */
    FPUcplxlog(&tempz1, &tempz1);
    rz->x = tempz1.y;  rz->y = -tempz1.x;           /* rz = (-i)*tempz1 */
    }   /* end. Arcsinz */

void CTrigFn::Arccosz(Complex z, Complex *rz)		    /* rz=Arccos(z)=-i*Log{z+sqrt(z*z-1)} */

    {
    Complex temp;

    FPUcplxmul(&z, &z, &temp);
    temp.x -= 1;				    /* temp = temp - 1 */
    Sqrtz(temp, &temp);

    temp.x += z.x; temp.y += z.y;		    /* temp = z + temp */

    FPUcplxlog(&temp, &temp);
    rz->x = temp.y;  rz->y = -temp.x;		    /* rz = (-i)*tempz1 */
    }   /* end. Arccosz */

void CTrigFn::Arcsinhz(Complex z, Complex *rz)
    {
    Complex temp;

    FPUcplxmul(&z, &z, &temp);
    temp.x += 1;                                 /* temp = temp + 1 */
    Sqrtz(temp, &temp);
    temp.x += z.x; temp.y += z.y;                /* temp = z + temp */
    FPUcplxlog(&temp, rz);
    }  /* end. Arcsinhz */

       /* rz=Arccosh(z)=Log(z+sqrt(z*z-1)} */
void CTrigFn::Arccoshz(Complex z, Complex *rz)
    {
    Complex tempz;
    FPUcplxmul(&z, &z, &tempz);
    tempz.x -= 1;                              /* tempz = tempz - 1 */
    Sqrtz(tempz, &tempz);
    tempz.x = z.x + tempz.x; tempz.y = z.y + tempz.y;  /* tempz = z + tempz */
    FPUcplxlog(&tempz, rz);
    }   /* end. Arccoshz */

	/* rz=Arctanh(z)=1/2*Log{(1+z)/(1-z)} */
void CTrigFn::Arctanhz(Complex z, Complex *rz)
    {
    Complex temp0, temp1, temp2;

    if (z.x == 0.0) {
	rz->x = 0;
	rz->y = atan(z.y);
	return;
	}
    else {
	if (fabs(z.x) == 1.0 && z.y == 0.0) {
	    return;
	    }
	else if (fabs(z.x) < 1.0 && z.y == 0.0) {
	    rz->x = log((1 + z.x) / (1 - z.x)) / 2;
	    rz->y = 0;
	    return;
	    }
	else {
	    temp0.x = 1 + z.x; temp0.y = z.y;             /* temp0 = 1 + z */
	    temp1.x = 1 - z.x; temp1.y = -z.y;            /* temp1 = 1 - z */
	    FPUcplxdiv(&temp0, &temp1, &temp2);
	    FPUcplxlog(&temp2, &temp2);
	    rz->x = .5*temp2.x; rz->y = .5*temp2.y;       /* rz = .5*temp2 */
	    return;
	    }
	}
    }   /* end. Arctanhz */

	/* rz=Arctan(z)=i/2*Log{(1-i*z)/(1+i*z)} */
void CTrigFn::Arctanz(Complex z, Complex *rz)
    {
    Complex temp0, temp1, temp2, temp3;
    if (z.x == 0.0 && z.y == 0.0)
	rz->x = rz->y = 0;
    else if (z.x != 0.0 && z.y == 0.0) {
	rz->x = atan(z.x);
	rz->y = 0;
	}
    else if (z.x == 0.0 && z.y != 0.0) {
	temp0.x = z.y;  temp0.y = 0.0;
	Arctanhz(temp0, &temp0);
	rz->x = -temp0.y; rz->y = temp0.x;              /* i*temp0 */
	}
    else if (z.x != 0.0 && z.y != 0.0) {

	temp0.x = -z.y; temp0.y = z.x;                  /* i*z */
	temp1.x = 1 - temp0.x; temp1.y = -temp0.y;      /* temp1 = 1 - temp0 */
	temp2.x = 1 + temp0.x; temp2.y = temp0.y;       /* temp2 = 1 + temp0 */

	FPUcplxdiv(&temp1, &temp2, &temp3);
	FPUcplxlog(&temp3, &temp3);
	rz->x = -temp3.y*.5; rz->y = .5*temp3.x;           /* .5*i*temp0 */
	}
    }   /* end. Arctanz */

Complex CTrigFn::ComplexSqrtFloat(double x, double y)
    {
    double mag;
    double theta;
    Complex  result;

    if (x == 0.0 && y == 0.0)
	result.x = result.y = 0.0;
    else
	{
	mag = sqrt(sqrt(x*x + y*y));
	theta = atan2(y, x) / 2;
	FPUsincos(&theta, &result.y, &result.x);
	result.x *= mag;
	result.y *= mag;
	}
    return result;
    }



// start functions here
void CTrigFn::dStkSin(Complex *z)
    {
    double sinx, cosx, sinhy, coshy;

    FPUsincos(&z->x, &sinx, &cosx);
    FPUsinhcosh(&z->y, &sinhy, &coshy);
    z->x = sinx*coshy;
    z->y = cosx*sinhy;
    };

void CTrigFn::dStkSinh(Complex *z)
    {
    double siny, cosy, sinhx, coshx;

    FPUsincos(&z->y, &siny, &cosy);
    FPUsinhcosh(&z->x, &sinhx, &coshx);
    z->x = sinhx*cosy;
    z->y = coshx*siny;
    }

void CTrigFn::dStkCos(Complex *z)
    {
    double sinx, cosx, sinhy, coshy;

    FPUsincos(&z->x, &sinx, &cosx);
    FPUsinhcosh(&z->y, &sinhy, &coshy);
    z->x = cosx*coshy;
    z->y = -sinx*sinhy; /* TIW 04-25-91 sign */
    }

void CTrigFn::dStkCosh(Complex *z)
    {
    double siny, cosy, sinhx, coshx;

    FPUsincos(&z->y, &siny, &cosy);
    FPUsinhcosh(&z->x, &sinhx, &coshx);
    z->x = coshx*cosy;
    z->y = sinhx*siny;
    }

void CTrigFn::dStkSqr(Complex *z)   
    {
    Complex LastSqr;
    LastSqr.x = z->x * z->x;
    LastSqr.y = z->y * z->y;
    z->y = z->x * z->y * 2.0;
    z->x = LastSqr.x - LastSqr.y;
    }

void CTrigFn::dStkLog(Complex *z)
    {
    FPUcplxlog(z, z);
    }

void CTrigFn::dStkExp(Complex *z)
    {
    FPUcplxexp(z, z);
    }

void CTrigFn::dStkAbs(Complex *z) 
    {
    z->x = fabs(z->x);
    z->y = fabs(z->y);
    }

void CTrigFn::dStkConj(Complex *z)
    {
    z->y = -z->y;
    }

void CTrigFn::dStkReal(Complex *z)
    {
    z->y = 0.0;
    }

void CTrigFn::dStkImag(Complex *z)
    {
    z->x = z->y;
    z->y = 0.0;
    }

void CTrigFn::dStkIdent(Complex *z)	// do nothing - the function Z 
    { 
    }

void CTrigFn::dStkRecip(Complex *z)
    {
    double mod;
    mod = z->x * z->x + z->y * z->y;
    if (fabs(mod) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = z->x / mod;
    z->y = -z->y / mod;
    }

void CTrigFn::dStkFlip(Complex *z)
    {
    double t;

    t = z->x;
    z->x = z->y;
    z->y = t;
    }

void CTrigFn::dStkTan(Complex *z)
    {
    double sinx, cosx, sinhy, coshy, denom;
    z->x *= 2;
    z->y *= 2;
    FPUsincos(&z->x, &sinx, &cosx);
    FPUsinhcosh(&z->y, &sinhy, &coshy);
    denom = cosx + coshy;
    if (fabs(denom) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = sinx / denom;
    z->y = sinhy / denom;
    }

void CTrigFn::dStkTanh(Complex *z) 
    {
    double siny, cosy, sinhx, coshx, denom;
    z->x *= 2;
    z->y *= 2;
    FPUsincos(&z->y, &siny, &cosy);
    FPUsinhcosh(&z->x, &sinhx, &coshx);
    denom = coshx + cosy;
    if (fabs(denom) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = sinhx / denom;
    z->y = siny / denom;
    }

void CTrigFn::dStkCoTan(Complex *z)
    {
    double sinx, cosx, sinhy, coshy, denom;
    z->x *= 2;
    z->y *= 2;
    FPUsincos(&z->x, &sinx, &cosx);
    FPUsinhcosh(&z->y, &sinhy, &coshy);
    denom = coshy - cosx;
    if (fabs(denom) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = sinx / denom;
    z->y = -sinhy / denom;
    }

void CTrigFn::dStkCoTanh(Complex *z)
    {
    double siny, cosy, sinhx, coshx, denom;
    z->x *= 2;
    z->y *= 2;
    FPUsincos(&z->y, &siny, &cosy);
    FPUsinhcosh(&z->x, &sinhx, &coshx);
    denom = coshx - cosy;
    if (fabs(denom) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = sinhx / denom;
    z->y = -siny / denom;
    }

void CTrigFn::dStkCosXX(Complex *z)		// Bogus version of cos, to replicate bug which was in regular cos till v16: 
    {
    dStkCos(z);
    z->y = -z->y;
    }

void CTrigFn::dStkSRand(Complex *z)
    {
    long x, y;

    x = rand15();
    y = rand15();
    z->x = (double)x / 12000.0;
    z->y = (double)y / 12000.0;
    }

void CTrigFn::dStkASin(Complex *z)
    {
    Arcsinz(*z, z);
    }

void CTrigFn::dStkASinh(Complex *z)
    {
    Arcsinhz(*z, z);
    }

void CTrigFn::dStkACos(Complex *z)
    {
    Arccosz(*z, z);
    }

void CTrigFn::dStkACosh(Complex *z)
    {
    Arccoshz(*z, z);
    }

void CTrigFn::dStkATan(Complex *z)
    {
    Arctanz(*z, z);
    }

void CTrigFn::dStkATanh(Complex *z)
    {
    Arctanhz(*z, z);
    }

void CTrigFn::dStkSqrt(Complex *z)
    {
    *z = ComplexSqrtFloat(z->x, z->y);
    }

void CTrigFn::dStkCAbs(Complex *z)
    {
    z->x = sqrt(sqr(z->x) + sqr(z->y));
    z->y = 0.0;
    }

void CTrigFn::dStkFloor(Complex *z)
    {
    z->x = floor(z->x);
    z->y = floor(z->y);
    }

void CTrigFn::dStkCeil(Complex *z)
    {
    z->x = ceil(z->x);
    z->y = ceil(z->y);
    }

void CTrigFn::dStkTrunc(Complex *z)
    {
    z->x = (int)(z->x);
    z->y = (int)(z->y);
    }

void CTrigFn::dStkRound(Complex *z)
    {
    z->x = floor(z->x + .5);
    z->y = floor(z->y + .5);
    }







// call appropriate trig function
void CTrigFn::CMPLXtrig(Complex *zIn, Complex *zOut, int index)
    {
    Complex z;

    z = *zIn;
    switch (index)
	{ 
	case 0:
	    dStkSin(&z);
	    break;
	case 1:
	    dStkSinh(&z);
	    break;
	case 2:
	    dStkCos(&z);
	    break;
	case 3:
	    dStkCosh(&z);
	    break;
	case 4:
	    dStkSqr(&z);
	    break;
	case 5:
	    dStkLog(&z);
	    break;
	case 6:
	    dStkExp(&z);
	    break;
	case 7:
	    dStkAbs(&z);
	    break;
	case 8:
	    dStkConj(&z);
	    break;
	case 9:
	    dStkReal(&z);
	    break;
	case 10:
	    dStkImag(&z);
	    break;
	case 11:
	    dStkIdent(&z);
	    break;
	case 12:
	    dStkRecip(&z);
	    break;
	case 13:		// Fn1 - not sure what the next for really mean but left in for consistency
	    dStkSin(&z);
	    break;
	case 14:		// Fn2
	    dStkSqr(&z);
	    break;
	case 15:		// Fn3
	    dStkSinh(&z);
	    break;
	case 16:		// Fn4
	    dStkCosh(&z);
	    break;
	case 17:
	    dStkFlip(&z);
	    break;
	case 18:
	    dStkTan(&z);
	    break;
	case 19:
	    dStkTanh(&z);
	    break;
	case 20:
	    dStkCoTan(&z);
	    break;
	case 21:
	    dStkCoTanh(&z);
	    break;
	case 22:
	    dStkCosXX(&z);
	    break;
	case 23:
	    dStkSRand(&z);
	    break;
	case 24:
	    dStkASin(&z);
	    break;
	case 25:
	    dStkASinh(&z);
	    break;
	case 26:
	    dStkACos(&z);
	    break;
	case 27:
	    dStkACosh(&z);
	    break;
	case 28:
	    dStkATan(&z);
	    break;
	case 29:
	    dStkATanh(&z);
	    break;
	case 30:
	    dStkSqrt(&z);
	    break;
	case 31:
	    dStkCAbs(&z);
	    break;
	case 32:
	    dStkFloor(&z);
	    break;
	case 33:
	    dStkCeil(&z);
	    break;
	case 34:
	    dStkTrunc(&z);
	    break;
	case 35:
	    dStkRound(&z);
	    break;
	default:
	    dStkSin(&z);
	    break;
	}
    *zOut = z;
    }

int CTrigFn::FindFunct(char *Str, int NumFns)
    {
    int		n;
    size_t	size;

    size = strlen(Str);
    for (n = 0; n < NumFns; n++)
	{
	if (!_strnicmp(FunctList[n], Str, size))
	    return n;
	}
    return -1;			// not found
    }



/*

char s_sin [] =             "sin";
char s_sinh [] =            "sinh";
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
