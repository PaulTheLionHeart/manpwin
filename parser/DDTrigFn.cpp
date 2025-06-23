//////////////////////////////////////////////////////////////////////
//
// DDTrigFn.cpp: Function list. Double-double version
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <time.h>
#include "complex.h"
#include "TrigFn.h"

void CTrigFn::DDHComplexTrig(_DDHCMPLX *h, _DDHCMPLX *out, int index)		// extends the unary function f to *h1
    {
    // This is the whole beauty of Hypercomplex numbers - *ANY* unary
    // complex valued function of a complex variable can easily
    // be generalized to hypercomplex numbers

    DDComplex a, b, resulta, resultb;

    // convert to duplex form
    a.x = h->x - h->t;
    a.y = h->y + h->z;
    b.x = h->x + h->t;
    b.y = h->y - h->z;

    // apply function to each part
    DDCMPLXtrig(&a, &resulta, index);
    DDCMPLXtrig(&b, &resultb, index);

    // convert back
    out->x = (resulta.x + resultb.x) / 2;
    out->y = (resulta.y + resultb.y) / 2;
    out->z = (resulta.y - resultb.y) / 2;
    out->t = (resultb.x - resulta.x) / 2;
    }

//  The following Complex double-double function routines added by Paul de Leeuw 08/06/2025.

#define DDSqrtz(z,rz) (*(rz) = z.CSqrt())

// rz=Arcsin(z)=-i*Log{i*z+sqrt(1-z*z)} 
void CTrigFn::DDArcsinz(DDComplex z, DDComplex *rz)
    {
    DDComplex tempz1, tempz2;

    tempz1 = z * z;
    tempz1.x = 1.0 - tempz1.x; tempz1.y = -tempz1.y;	// tempz1 = 1 - tempz1
    DDSqrtz(tempz1, &tempz1);
    tempz2.x = -z.y; tempz2.y = z.x;			// tempz2 = i*z
    tempz1.x += tempz2.x;  tempz1.y += tempz2.y;	// tempz1 += tempz2
    tempz1 = tempz1.CLog();
    rz->x = tempz1.y;  rz->y = -tempz1.x;		// rz = (-i)*tempz1
    }   // end. Arcsinz


    // rz=Arccos(z)=-i*Log{z+sqrt(z*z-1)}
void CTrigFn::DDArccosz(DDComplex z, DDComplex *rz)
    {
    DDComplex temp;

    temp = z * z;
    temp.x -= 1.0;					// temp = temp - 1
    DDSqrtz(temp, &temp);
    temp.x += z.x; temp.y += z.y;			// temp = z + temp
    temp = temp.CLog();
    rz->x = temp.y;  rz->y = -temp.x;			// rz = (-i)*tempz1
    }   // end. Arccosz

void CTrigFn::DDArcsinhz(DDComplex z, DDComplex *rz)
    {
    DDComplex temp;

    temp = z * z;
    temp.x += 1;					// temp = temp + 1
    DDSqrtz(temp, &temp);
    temp.x += z.x; temp.y += z.y;			// temp = z + temp
    *rz = temp.CLog();
    }  // end. Arcsinhz

    // rz=Arccosh(z)=Log(z+sqrt(z*z-1)}
void CTrigFn::DDArccoshz(DDComplex z, DDComplex *rz)
    {
    DDComplex tempz;

    tempz = z * z;
    tempz.x -= 1;					// tempz = tempz - 1
    DDSqrtz(tempz, &tempz);
    tempz.x = z.x + tempz.x; tempz.y = z.y + tempz.y;	// tempz = z + tempz
    *rz = tempz.CLog();
    }   // end. Arccoshz

    // rz=Arctanh(z)=1/2*Log{(1+z)/(1-z)}
void CTrigFn::DDArctanhz(DDComplex z, DDComplex *rz)
    {
    DDComplex temp0, temp1, temp2;

    if (z.x == 0.0) {
	rz->x = 0.0;
	rz->y = atan(z.y);
	return;
	}
    else {
	if (fabs(z.x) == 1.0 && z.y == 0.0) 
	    {
	    return;
	    }
	else if (fabs(z.x) < 1.0 && z.y == 0.0) 
	    {
	    rz->x = log((1 + z.x) / (1 - z.x)) / 2;
	    rz->y = 0.0;
	    return;
	    }
	else {
	    temp0.x = 1 + z.x; temp0.y = z.y;		// temp0 = 1 + z
	    temp1.x = 1 - z.x; temp1.y = -z.y;		// temp1 = 1 - z
	    temp2 = temp0 / temp1;
	    temp2 = temp2.CLog();
	    rz->x = 0.5*temp2.x; rz->y = 0.5*temp2.y;	// rz = .5*temp2
	    return;
	    }
	}
    }   // end. Arctanhz

    // rz=Arctan(z)=i/2*Log{(1-i*z)/(1+i*z)}
void CTrigFn::DDArctanz(DDComplex z, DDComplex *rz)
    {
    DDComplex temp0, temp1, temp2, temp3;
    if (z.x == 0.0 && z.y == 0.0)
	rz->x = rz->y = 0.0;
    else if (z.x != 0.0 && z.y == 0.0) 
	{
	rz->x = atan(z.x);
	rz->y = 0.0;
	}
    else if (z.x == 0.0 && z.y != 0.0) 
	{
	temp0.x = z.y;  temp0.y = 0.0;
	DDArctanhz(temp0, &temp0);
	rz->x = -temp0.y; rz->y = temp0.x;              // i*temp0
	}
    else if (z.x != 0.0 && z.y != 0.0) 
	{

	temp0.x = -z.y; temp0.y = z.x;                  // i*z
	temp1.x = 1 - temp0.x; temp1.y = -temp0.y;      // temp1 = 1 - temp0
	temp2.x = 1 + temp0.x; temp2.y = temp0.y;       // temp2 = 1 + temp0

	temp3 = temp1 / temp2;
	rz->x = -temp3.y*0.5; rz->y = 0.5*temp3.x;	// .5*i*temp0
	}
    }   // end. Arctanz

// start functions here
void CTrigFn::ddStkSin(DDComplex *z)
    {
    dd_real sinx, cosx, sinhy, coshy;

    sincos(z->x, sinx, cosx);
    sincosh(z->y, sinhy, coshy);
    z->x = sinx*coshy;
    z->y = cosx*sinhy;
    };

void CTrigFn::ddStkSinh(DDComplex *z)
    {
    dd_real siny, cosy, sinhx, coshx;

    sincos(z->y, siny, cosy);
    sincosh(z->x, sinhx, coshx);
    z->x = sinhx*cosy;
    z->y = coshx*siny;
    }

void CTrigFn::ddStkCos(DDComplex *z)
    {
    dd_real sinx, cosx, sinhy, coshy;

    sincos(z->x, sinx, cosx);
    sincosh(z->y, sinhy, coshy);
    z->x = cosx*coshy;
    z->y = -sinx*sinhy; /* TIW 04-25-91 sign */
    }

void CTrigFn::ddStkCosh(DDComplex *z)
    {
    dd_real siny, cosy, sinhx, coshx;

    sincos(z->y, siny, cosy);
    sincosh(z->x, sinhx, coshx);
    z->x = coshx*cosy;
    z->y = sinhx*siny;
    }

void CTrigFn::ddStkSqr(DDComplex *z)
    {
    DDComplex LastSqr;
    LastSqr.x = z->x * z->x;
    LastSqr.y = z->y * z->y;
    z->y = z->x * z->y * 2.0;
    z->x = LastSqr.x - LastSqr.y;
    }

void CTrigFn::ddStkLog(DDComplex *z)
    {
    *z = z->CLog();
    }

void CTrigFn::ddStkExp(DDComplex *z)
    {
    *z = z->CExp();
    }

void CTrigFn::ddStkAbs(DDComplex *z)
    {
    z->x = fabs(z->x);
    z->y = fabs(z->y);
    }

void CTrigFn::ddStkConj(DDComplex *z)
    {
    z->y = -z->y;
    }

void CTrigFn::ddStkReal(DDComplex *z)
    {
    z->y = 0.0;
    }

void CTrigFn::ddStkImag(DDComplex *z)
    {
    z->x = z->y;
    z->y = 0.0;
    }

void CTrigFn::ddStkIdent(DDComplex *z)	// do nothing - the function Z 
    { 
    }

void CTrigFn::ddStkRecip(DDComplex *z)
    {
    dd_real mod;
    mod = z->x * z->x + z->y * z->y;
    if (fabs(mod) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = z->x / mod;
    z->y = -z->y / mod;
    }

void CTrigFn::ddStkFlip(DDComplex *z)
    {
    dd_real t;

    t = z->x;
    z->x = z->y;
    z->y = t;
    }

void CTrigFn::ddStkTan(DDComplex *z)
    {
    dd_real sinx, cosx, sinhy, coshy, denom;
    z->x *= 2;
    z->y *= 2;
    sincos(z->x, sinx, cosx);
    sincosh(z->y, sinhy, coshy);
    denom = cosx + coshy;
    if (fabs(denom) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = sinx / denom;
    z->y = sinhy / denom;
    }

void CTrigFn::ddStkTanh(DDComplex *z)
    {
    dd_real siny, cosy, sinhx, coshx, denom;
    z->x *= 2;
    z->y *= 2;
    sincos(z->y, siny, cosy);
    sincosh(z->x, sinhx, coshx);
    denom = coshx + cosy;
    if (fabs(denom) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = sinhx / denom;
    z->y = siny / denom;
    }

void CTrigFn::ddStkCoTan(DDComplex *z)
    {
    dd_real sinx, cosx, sinhy, coshy, denom;
    z->x *= 2;
    z->y *= 2;
    sincos(z->x, sinx, cosx);
    sincosh(z->y, sinhy, coshy);
    denom = coshy - cosx;
    if (fabs(denom) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = sinx / denom;
    z->y = -sinhy / denom;
    }

void CTrigFn::ddStkCoTanh(DDComplex *z)
    {
    dd_real siny, cosy, sinhx, coshx, denom;
    z->x *= 2;
    z->y *= 2;
    sincos(z->y, siny, cosy);
    sincosh(z->x, sinhx, coshx);
    denom = coshx - cosy;
    if (fabs(denom) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = sinhx / denom;
    z->y = -siny / denom;
    }

void CTrigFn::ddStkCosXX(DDComplex *z)		// Bogus version of cos, to replicate bug which was in regular cos till v16: 
    {
    ddStkCos(z);
    z->y = -z->y;
    }

void CTrigFn::ddStkSRand(DDComplex *z)
    {
    long x, y;

    x = rand15();
    y = rand15();
    z->x = (double)x / 12000.0;
    z->y = (double)y / 12000.0;
    }

void CTrigFn::ddStkASin(DDComplex *z)
    {
    DDArcsinz(*z, z);
    }

void CTrigFn::ddStkASinh(DDComplex *z)
    {
    DDArcsinhz(*z, z);
    }

void CTrigFn::ddStkACos(DDComplex *z)
    {
    DDArccosz(*z, z);
    }

void CTrigFn::ddStkACosh(DDComplex *z)
    {
    DDArccoshz(*z, z);
    }

void CTrigFn::ddStkATan(DDComplex *z)
    {
    DDArctanz(*z, z);
    }

void CTrigFn::ddStkATanh(DDComplex *z)
    {
    DDArctanhz(*z, z);
    }

void CTrigFn::ddStkSqrt(DDComplex *z)
    {
//    *z = ComplexSqrtFloat(z->x, z->y);
    *z = z->CSqrt();
    }

void CTrigFn::ddStkCAbs(DDComplex *z)
    {
    z->x = sqrt(sqr(z->x) + sqr(z->y));
    z->y = 0.0;
    }

void CTrigFn::ddStkFloor(DDComplex *z)
    {
    z->x = floor(z->x);
    z->y = floor(z->y);
    }

void CTrigFn::ddStkCeil(DDComplex *z)
    {
    z->x = ceil(z->x);
    z->y = ceil(z->y);
    }

void CTrigFn::ddStkTrunc(DDComplex *z)
    {
    z->x = to_int(z->x);
    z->y = to_int(z->y);
    }

void CTrigFn::ddStkRound(DDComplex *z)
    {
    z->x = floor(z->x + 0.5);
    z->y = floor(z->y + 0.5);
    }

// call appropriate trig function
void CTrigFn::DDCMPLXtrig(DDComplex *zIn, DDComplex *zOut, int index)
    {
    DDComplex z;

    z = *zIn;
    switch (index)
	{ 
	case 0:
	    ddStkSin(&z);
	    break;
	case 1:
	    ddStkSinh(&z);
	    break;
	case 2:
	    ddStkCos(&z);
	    break;
	case 3:
	    ddStkCosh(&z);
	    break;
	case 4:
	    ddStkSqr(&z);
	    break;
	case 5:
	    ddStkLog(&z);
	    break;
	case 6:
	    ddStkExp(&z);
	    break;
	case 7:
	    ddStkAbs(&z);
	    break;
	case 8:
	    ddStkConj(&z);
	    break;
	case 9:
	    ddStkReal(&z);
	    break;
	case 10:
	    ddStkImag(&z);
	    break;
	case 11:
	    ddStkIdent(&z);
	    break;
	case 12:
	    ddStkRecip(&z);
	    break;
	case 13:		// Fn1 - not sure what the next for really mean but left in for consistency
	    ddStkSin(&z);
	    break;
	case 14:		// Fn2
	    ddStkSqr(&z);
	    break;
	case 15:		// Fn3
	    ddStkSinh(&z);
	    break;
	case 16:		// Fn4
	    ddStkCosh(&z);
	    break;
	case 17:
	    ddStkFlip(&z);
	    break;
	case 18:
	    ddStkTan(&z);
	    break;
	case 19:
	    ddStkTanh(&z);
	    break;
	case 20:
	    ddStkCoTan(&z);
	    break;
	case 21:
	    ddStkCoTanh(&z);
	    break;
	case 22:
	    ddStkCosXX(&z);
	    break;
	case 23:
	    ddStkSRand(&z);
	    break;
	case 24:
	    ddStkASin(&z);
	    break;
	case 25:
	    ddStkASinh(&z);
	    break;
	case 26:
	    ddStkACos(&z);
	    break;
	case 27:
	    ddStkACosh(&z);
	    break;
	case 28:
	    ddStkATan(&z);
	    break;
	case 29:
	    ddStkATanh(&z);
	    break;
	case 30:
	    ddStkSqrt(&z);
	    break;
	case 31:
	    ddStkCAbs(&z);
	    break;
	case 32:
	    ddStkFloor(&z);
	    break;
	case 33:
	    ddStkCeil(&z);
	    break;
	case 34:
	    ddStkTrunc(&z);
	    break;
	case 35:
	    ddStkRound(&z);
	    break;
	default:
	    ddStkSin(&z);
	    break;
	}
    *zOut = z;
    }


