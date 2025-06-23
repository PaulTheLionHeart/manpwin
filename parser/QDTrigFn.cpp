//////////////////////////////////////////////////////////////////////
//
// QDTrigFn.cpp: Function list. Quad-double version
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <time.h>
#include "complex.h"
#include "TrigFn.h"

void CTrigFn::QDHComplexTrig(_QDHCMPLX *h, _QDHCMPLX *out, int index)		// extends the unary function f to *h1
    {
    // This is the whole beauty of Hypercomplex numbers - *ANY* unary
    // complex valued function of a complex variable can easily
    // be generalized to hypercomplex numbers

    QDComplex a, b, resulta, resultb;

    // convert to duplex form
    a.x = h->x - h->t;
    a.y = h->y + h->z;
    b.x = h->x + h->t;
    b.y = h->y - h->z;

    // apply function to each part
    QDCMPLXtrig(&a, &resulta, index);
    QDCMPLXtrig(&b, &resultb, index);

    // convert back
    out->x = (resulta.x + resultb.x) / 2;
    out->y = (resulta.y + resultb.y) / 2;
    out->z = (resulta.y - resultb.y) / 2;
    out->t = (resultb.x - resulta.x) / 2;
    }

//  The following Complex quad-double function routines added by Paul de Leeuw 12/06/2025.

#define QDSqrtz(z,rz) (*(rz) = z.CSqrt())

// rz=Arcsin(z)=-i*Log{i*z+sqrt(1-z*z)} 
void CTrigFn::QDArcsinz(QDComplex z, QDComplex *rz)
    {
    QDComplex tempz1, tempz2;

    tempz1 = z * z;
    tempz1.x = 1.0 - tempz1.x; tempz1.y = -tempz1.y;	// tempz1 = 1 - tempz1
    QDSqrtz(tempz1, &tempz1);
    tempz2.x = -z.y; tempz2.y = z.x;			// tempz2 = i*z
    tempz1.x += tempz2.x;  tempz1.y += tempz2.y;	// tempz1 += tempz2
    tempz1 = tempz1.CLog();
    rz->x = tempz1.y;  rz->y = -tempz1.x;		// rz = (-i)*tempz1
    }   // end. Arcsinz


    // rz=Arccos(z)=-i*Log{z+sqrt(z*z-1)}
void CTrigFn::QDArccosz(QDComplex z, QDComplex *rz)
    {
    QDComplex temp;

    temp = z * z;
    temp.x -= 1.0;					// temp = temp - 1
    QDSqrtz(temp, &temp);
    temp.x += z.x; temp.y += z.y;			// temp = z + temp
    temp = temp.CLog();
    rz->x = temp.y;  rz->y = -temp.x;			// rz = (-i)*tempz1
    }   // end. Arccosz

void CTrigFn::QDArcsinhz(QDComplex z, QDComplex *rz)
    {
    QDComplex temp;

    temp = z * z;
    temp.x += 1;					// temp = temp + 1
    QDSqrtz(temp, &temp);
    temp.x += z.x; temp.y += z.y;			// temp = z + temp
    *rz = temp.CLog();
    }  // end. Arcsinhz

    // rz=Arccosh(z)=Log(z+sqrt(z*z-1)}
void CTrigFn::QDArccoshz(QDComplex z, QDComplex *rz)
    {
    QDComplex tempz;

    tempz = z * z;
    tempz.x -= 1;					// tempz = tempz - 1
    QDSqrtz(tempz, &tempz);
    tempz.x = z.x + tempz.x; tempz.y = z.y + tempz.y;	// tempz = z + tempz
    *rz = tempz.CLog();
    }   // end. Arccoshz

    // rz=Arctanh(z)=1/2*Log{(1+z)/(1-z)}
void CTrigFn::QDArctanhz(QDComplex z, QDComplex *rz)
    {
    QDComplex temp0, temp1, temp2;

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
void CTrigFn::QDArctanz(QDComplex z, QDComplex *rz)
    {
    QDComplex temp0, temp1, temp2, temp3;
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
	QDArctanhz(temp0, &temp0);
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
void CTrigFn::qdStkSin(QDComplex *z)
    {
    qd_real sinx, cosx, sinhy, coshy;

    sincos(z->x, sinx, cosx);
    sincosh(z->y, sinhy, coshy);
    z->x = sinx*coshy;
    z->y = cosx*sinhy;
    };

void CTrigFn::qdStkSinh(QDComplex *z)
    {
    qd_real siny, cosy, sinhx, coshx;

    sincos(z->y, siny, cosy);
    sincosh(z->x, sinhx, coshx);
    z->x = sinhx*cosy;
    z->y = coshx*siny;
    }

void CTrigFn::qdStkCos(QDComplex *z)
    {
    qd_real sinx, cosx, sinhy, coshy;

    sincos(z->x, sinx, cosx);
    sincosh(z->y, sinhy, coshy);
    z->x = cosx*coshy;
    z->y = -sinx*sinhy; // TIW 04-25-91 sign
    }

void CTrigFn::qdStkCosh(QDComplex *z)
    {
    qd_real siny, cosy, sinhx, coshx;

    sincos(z->y, siny, cosy);
    sincosh(z->x, sinhx, coshx);
    z->x = coshx*cosy;
    z->y = sinhx*siny;
    }

void CTrigFn::qdStkSqr(QDComplex *z)
    {
    QDComplex LastSqr;
    LastSqr.x = z->x * z->x;
    LastSqr.y = z->y * z->y;
    z->y = z->x * z->y * 2.0;
    z->x = LastSqr.x - LastSqr.y;
    }

void CTrigFn::qdStkLog(QDComplex *z)
    {
    *z = z->CLog();
    }

void CTrigFn::qdStkExp(QDComplex *z)
    {
    *z = z->CExp();
    }

void CTrigFn::qdStkAbs(QDComplex *z)
    {
    z->x = fabs(z->x);
    z->y = fabs(z->y);
    }

void CTrigFn::qdStkConj(QDComplex *z)
    {
    z->y = -z->y;
    }

void CTrigFn::qdStkReal(QDComplex *z)
    {
    z->y = 0.0;
    }

void CTrigFn::qdStkImag(QDComplex *z)
    {
    z->x = z->y;
    z->y = 0.0;
    }

void CTrigFn::qdStkIdent(QDComplex *z)	// do nothing - the function Z 
    { 
    }

void CTrigFn::qdStkRecip(QDComplex *z)
    {
    qd_real mod;
    mod = z->x * z->x + z->y * z->y;
    if (fabs(mod) <= DBL_MIN)
	return;			// leave z unchanged
    z->x = z->x / mod;
    z->y = -z->y / mod;
    }

void CTrigFn::qdStkFlip(QDComplex *z)
    {
    qd_real t;

    t = z->x;
    z->x = z->y;
    z->y = t;
    }

void CTrigFn::qdStkTan(QDComplex *z)
    {
    qd_real sinx, cosx, sinhy, coshy, denom;
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

void CTrigFn::qdStkTanh(QDComplex *z)
    {
    qd_real siny, cosy, sinhx, coshx, denom;
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

void CTrigFn::qdStkCoTan(QDComplex *z)
    {
    qd_real sinx, cosx, sinhy, coshy, denom;
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

void CTrigFn::qdStkCoTanh(QDComplex *z)
    {
    qd_real siny, cosy, sinhx, coshx, denom;
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

void CTrigFn::qdStkCosXX(QDComplex *z)		// Bogus version of cos, to replicate bug which was in regular cos till v16: 
    {
    qdStkCos(z);
    z->y = -z->y;
    }

void CTrigFn::qdStkSRand(QDComplex *z)
    {
    long x, y;

    x = rand15();
    y = rand15();
    z->x = (double)x / 12000.0;
    z->y = (double)y / 12000.0;
    }

void CTrigFn::qdStkASin(QDComplex *z)
    {
    QDArcsinz(*z, z);
    }

void CTrigFn::qdStkASinh(QDComplex *z)
    {
    QDArcsinhz(*z, z);
    }

void CTrigFn::qdStkACos(QDComplex *z)
    {
    QDArccosz(*z, z);
    }

void CTrigFn::qdStkACosh(QDComplex *z)
    {
    QDArccoshz(*z, z);
    }

void CTrigFn::qdStkATan(QDComplex *z)
    {
    QDArctanz(*z, z);
    }

void CTrigFn::qdStkATanh(QDComplex *z)
    {
    QDArctanhz(*z, z);
    }

void CTrigFn::qdStkSqrt(QDComplex *z)
    {
//    *z = ComplexSqrtFloat(z->x, z->y);
    *z = z->CSqrt();
    }

void CTrigFn::qdStkCAbs(QDComplex *z)
    {
    z->x = sqrt(sqr(z->x) + sqr(z->y));
    z->y = 0.0;
    }

void CTrigFn::qdStkFloor(QDComplex *z)
    {
    z->x = floor(z->x);
    z->y = floor(z->y);
    }

void CTrigFn::qdStkCeil(QDComplex *z)
    {
    z->x = ceil(z->x);
    z->y = ceil(z->y);
    }

void CTrigFn::qdStkTrunc(QDComplex *z)
    {
    z->x = to_int(z->x);
    z->y = to_int(z->y);
    }

void CTrigFn::qdStkRound(QDComplex *z)
    {
    z->x = floor(z->x + 0.5);
    z->y = floor(z->y + 0.5);
    }

// call appropriate trig function
void CTrigFn::QDCMPLXtrig(QDComplex *zIn, QDComplex *zOut, int index)
    {
    QDComplex z;

    z = *zIn;
    switch (index)
	{ 
	case 0:
	    qdStkSin(&z);
	    break;
	case 1:
	    qdStkSinh(&z);
	    break;
	case 2:
	    qdStkCos(&z);
	    break;
	case 3:
	    qdStkCosh(&z);
	    break;
	case 4:
	    qdStkSqr(&z);
	    break;
	case 5:
	    qdStkLog(&z);
	    break;
	case 6:
	    qdStkExp(&z);
	    break;
	case 7:
	    qdStkAbs(&z);
	    break;
	case 8:
	    qdStkConj(&z);
	    break;
	case 9:
	    qdStkReal(&z);
	    break;
	case 10:
	    qdStkImag(&z);
	    break;
	case 11:
	    qdStkIdent(&z);
	    break;
	case 12:
	    qdStkRecip(&z);
	    break;
	case 13:		// Fn1 - not sure what the next for really mean but left in for consistency
	    qdStkSin(&z);
	    break;
	case 14:		// Fn2
	    qdStkSqr(&z);
	    break;
	case 15:		// Fn3
	    qdStkSinh(&z);
	    break;
	case 16:		// Fn4
	    qdStkCosh(&z);
	    break;
	case 17:
	    qdStkFlip(&z);
	    break;
	case 18:
	    qdStkTan(&z);
	    break;
	case 19:
	    qdStkTanh(&z);
	    break;
	case 20:
	    qdStkCoTan(&z);
	    break;
	case 21:
	    qdStkCoTanh(&z);
	    break;
	case 22:
	    qdStkCosXX(&z);
	    break;
	case 23:
	    qdStkSRand(&z);
	    break;
	case 24:
	    qdStkASin(&z);
	    break;
	case 25:
	    qdStkASinh(&z);
	    break;
	case 26:
	    qdStkACos(&z);
	    break;
	case 27:
	    qdStkACosh(&z);
	    break;
	case 28:
	    qdStkATan(&z);
	    break;
	case 29:
	    qdStkATanh(&z);
	    break;
	case 30:
	    qdStkSqrt(&z);
	    break;
	case 31:
	    qdStkCAbs(&z);
	    break;
	case 32:
	    qdStkFloor(&z);
	    break;
	case 33:
	    qdStkCeil(&z);
	    break;
	case 34:
	    qdStkTrunc(&z);
	    break;
	case 35:
	    qdStkRound(&z);
	    break;
	default:
	    qdStkSin(&z);
	    break;
	}
    *zOut = z;
    }





