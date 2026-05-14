#include <cmath>

#pragma once

/**************************************************************************
    Unary Negation
**************************************************************************/

template<typename TComplex>
inline void TStkNeg(TComplex* z)
    {
    z->x = -z->x;
    z->y = -z->y;
    }

/**************************************************************************
    Add
**************************************************************************/

template<typename TComplex>
inline void TStkAdd(TComplex* a, const TComplex* b)
    {
    a->x += b->x;
    a->y += b->y;
    }

/**************************************************************************
    Subtract
**************************************************************************/

template<typename TComplex>
inline void TStkSub(TComplex* a, const TComplex* b)
    {
    a->x -= b->x;
    a->y -= b->y;
    }

/**************************************************************************
    Square
**************************************************************************/

template<typename TComplex>
inline void TStkSqr(TComplex* z)
    {
    TComplex LastSqr;

    LastSqr.x = z->x * z->x;
    LastSqr.y = z->y * z->y;

    z->y = z->x * z->y * 2.0;
    z->x = LastSqr.x - LastSqr.y;
    }

/**************************************************************************
    Absolute Value
**************************************************************************/

template<typename TComplex>
inline void TStkAbs(TComplex* z)
    {
    z->x = abs(z->x);
    z->y = abs(z->y);
    }

/**************************************************************************
    Conjugate
**************************************************************************/

template<typename TComplex>
inline void TStkConj(TComplex* z)
    {
    z->y = -z->y;
    }

/**************************************************************************
    Real Part
**************************************************************************/

template<typename TComplex>
inline void TStkReal(TComplex* z)
    {
    z->y = 0.0;
    }

/**************************************************************************
    Imaginary Part
**************************************************************************/

template<typename TComplex>
inline void TStkImag(TComplex* z)
    {
    z->x = z->y;
    z->y = 0.0;
    }

/**************************************************************************
    Identity
**************************************************************************/

template<typename TComplex>
inline void TStkIdent(TComplex* z)
    {
    // intentionally empty
    }

/**************************************************************************
    Flip Real/Imaginary
**************************************************************************/

template<typename TComplex>
inline void TStkFlip(TComplex* z)
    {
    auto t = z->x;
    z->x = z->y;
    z->y = t;
    }

/**************************************************************************
    Multiply
**************************************************************************/

template<typename TComplex>
inline void TStkMul(TComplex* a, const TComplex* b)
    {
    TComplex temp;

    temp.x = a->x * b->x - a->y * b->y;
    temp.y = a->x * b->y + a->y * b->x;
    *a = temp;
    }

/**************************************************************************
    Divide
**************************************************************************/

template<typename TComplex>
inline void TStkDiv(TComplex* arg2, const TComplex* arg1)
    {
    *arg2 = *arg2 / *arg1;
    }

/**************************************************************************
    Zero
**************************************************************************/

template<typename TComplex>
inline void TStkZero(TComplex* z)
    {
    z->x = 0.0;
    z->y = 0.0;
    }

/**************************************************************************
    One
**************************************************************************/

template<typename TComplex>
inline void TStkOne(TComplex* z)
    {
    z->x = 1.0;
    z->y = 0.0;
    }
    
/**************************************************************************
    Reciprocal
**************************************************************************/

template<typename T>
inline bool TChkDenom(T denom)
    {
    return (fabs(denom) > DBL_MIN);
    }

template<typename TComplex>
inline void TStkRecip(TComplex* z)
    {
    auto mod = z->x * z->x + z->y * z->y;

    if (!TChkDenom(mod))
	return;

    auto tx = z->x;
    auto ty = z->y;

    z->x = tx / mod;
    z->y = -ty / mod;
    }

/**************************************************************************
Complex Absolute Value (modulus)
**************************************************************************/

template<typename TComplex>
inline void TStkCAbs(TComplex* z)
    {
    z->x = sqrt(z->x * z->x + z->y * z->y);
    z->y = 0.0;
    }

/**************************************************************************
Floor
**************************************************************************/

template<typename TComplex>
inline void TStkFloor(TComplex* z)
    {
    z->x = floor(z->x);
    z->y = floor(z->y);
    }

/**************************************************************************
Ceil
**************************************************************************/

template<typename TComplex>
inline void TStkCeil(TComplex* z)
    {
    z->x = ceil(z->x);
    z->y = ceil(z->y);
    }

/**************************************************************************
Truncate
**************************************************************************/
    
    //Some helpers for truncation to int
inline double TTrunc(double x)
    {
    return (int)x;
    }

inline dd_real TTrunc(dd_real x)
    {
    return (int)(to_double(x));
    }

inline qd_real TTrunc(qd_real x)
    {
    return (int)(to_double(x));
    }


template<typename TComplex>
inline void TStkTrunc(TComplex* z)
    {
    z->x = TTrunc(z->x);
    z->y = TTrunc(z->y);
    }

/**************************************************************************
Round
**************************************************************************/

//Some helpers for rounding
inline double TRound(double x)
    {
    return floor(x + 0.5);
    }

inline dd_real TRound(dd_real x)
    {
    return floor(x + 0.5);
    }

inline qd_real TRound(qd_real x)
    {
    return floor(x + 0.5);
    }

template<typename TComplex>
inline void TStkRound(TComplex* z)
    {
    z->x = TRound(z->x);
    z->y = TRound(z->y);
    }

/**************************************************************************
    Modulus Squared
**************************************************************************/

template<typename TComplex>
inline void TStkMod(TComplex* z)
    {
    z->x = (z->x * z->x) + (z->y * z->y);
    z->y = 0.0;
    }

/**************************************************************************
    Square root helper
**************************************************************************/

template<typename TComplex>
inline TComplex TComplexSqrtFloat(double x, double y);

template<>
inline Complex TComplexSqrtFloat<Complex>(double x, double y)
    {
    double mag;
    double theta;
    Complex result;

    if (x == 0.0 && y == 0.0)
	{
	result.x = 0.0;
	result.y = 0.0;
	}
    else
	{
	mag = sqrt(sqrt(x * x + y * y));
	theta = atan2(y, x) / 2.0;

	result.x = cos(theta) * mag;
	result.y = sin(theta) * mag;
	}

    return result;
    }

/**************************************************************************
    Square root
**************************************************************************/

template<typename TComplex>
inline void TStkSqrt(TComplex* z)
    {
    *z = z->CSqrt();
    }

template<>
inline void TStkSqrt<Complex>(Complex* z)
    {
    *z = TComplexSqrtFloat<Complex>(z->x, z->y);
    }

/**************************************************************************
    Comparison modules
**************************************************************************/

template<typename TComplex>
inline void TStkLT(TComplex* arg2, const TComplex* arg1)
    {
    using TReal = decltype(arg1->x);

    arg2->x = TReal(arg2->x < arg1->x);
    arg2->y = 0.0;
    }

template<typename TComplex>
inline void TStkGT(TComplex* arg2, const TComplex* arg1)
    {
    using TReal = decltype(arg1->x);

    arg2->x = TReal(arg2->x > arg1->x);
    arg2->y = 0.0;
    }

template<typename TComplex>
inline void TStkLTE(TComplex* arg2, const TComplex* arg1)
    {
    using TReal = decltype(arg1->x);

    arg2->x = TReal(arg2->x <= arg1->x);
    arg2->y = 0.0;
    }

template<typename TComplex>
inline void TStkGTE(TComplex* arg2, const TComplex* arg1)
    {
    using TReal = decltype(arg1->x);

    arg2->x = TReal(arg2->x >= arg1->x);
    arg2->y = 0.0;
    }

template<typename TComplex>
inline void TStkEQ(TComplex* arg2, const TComplex* arg1)
    {
    using TReal = decltype(arg1->x);

    arg2->x = TReal(arg2->x == arg1->x);
    arg2->y = 0.0;
    }

template<typename TComplex>
inline void TStkNE(TComplex* arg2, const TComplex* arg1)
    {
    using TReal = decltype(arg1->x);

    arg2->x = TReal(arg2->x != arg1->x);
    arg2->y = 0.0;
    }

/**************************************************************************
    Boolean operators
**************************************************************************/

template<typename T>
inline bool IsTrue(const T& v)
    {
    return v != 0;
    }

template<typename TComplex>
inline void TStkOR(TComplex* arg2, const TComplex* arg1)
    {
    using TReal = decltype(arg1->x);

    bool a = IsTrue(arg2->x);
    bool b = IsTrue(arg1->x);

    arg2->x = (a || b) ? TReal(1.0) : TReal(0.0);
    arg2->y = 0.0;
    }

template<typename TComplex>
inline void TStkAND(TComplex* arg2, const TComplex* arg1)
    {
    using TReal = decltype(arg1->x);

    bool a = IsTrue(arg2->x);
    bool b = IsTrue(arg1->x);

    arg2->x = (a && b) ? TReal(1.0) : TReal(0.0);
    arg2->y = 0.0;
    }

/**************************************************************************
    Trig operators sine and cosine and the hyperbolics 
**************************************************************************/

inline void TSinCos(
    double x,
    double& s,
    double& c)
    {
    s = sin(x);
    c = cos(x);
    }

inline void TSinhCosh(
    double x,
    double& s,
    double& c)
    {
    s = sinh(x);
    c = cosh(x);
    }

inline void TSinCos(
    const dd_real& x,
    dd_real& s,
    dd_real& c)
    {
    sincos(x, s, c);
    }

inline void TSinhCosh(
    const dd_real& x,
    dd_real& s,
    dd_real& c)
    {
    sincosh(x, s, c);
    }

inline void TSinCos(
    const qd_real& x,
    qd_real& s,
    qd_real& c)
    {
    sincos(x, s, c);
    }

inline void TSinhCosh(
    const qd_real& x,
    qd_real& s,
    qd_real& c)
    {
    sincosh(x, s, c);
    }

template<typename TComplex>
inline void TStkSin(TComplex* z)
    {
    using TReal = decltype(z->x);

    TReal sinx, cosx, sinhy, coshy;

    TSinCos(z->x, sinx, cosx);
    TSinhCosh(z->y, sinhy, coshy);

    z->x = sinx * coshy;
    z->y = cosx * sinhy;
    }

template<typename TComplex>
inline void TStkCos(TComplex* z)
    {
    using TReal = decltype(z->x);

    TReal sinx, cosx, sinhy, coshy;

    TSinCos(z->x, sinx, cosx);
    TSinhCosh(z->y, sinhy, coshy);

    z->x = cosx * coshy;
    z->y = -sinx * sinhy;
    }

template<typename TComplex>
inline void TStkSinh(TComplex* z)
    {
    using TReal = decltype(z->x);

    TReal siny, cosy, sinhx, coshx;

    TSinCos(z->y, siny, cosy);
    TSinhCosh(z->x, sinhx, coshx);

    z->x = sinhx * cosy;
    z->y = coshx * siny;
    }

template<typename TComplex>
inline void TStkCosh(TComplex* z)
    {
    using TReal = decltype(z->x);

    TReal siny, cosy, sinhx, coshx;

    TSinCos(z->y, siny, cosy);
    TSinhCosh(z->x, sinhx, coshx);

    z->x = coshx * cosy;
    z->y = sinhx * siny;
    }

template<typename TComplex>
inline void TStkCosXX(TComplex* z)
    {
    TStkCos(z);
    z->y = -z->y;
    }

/**************************************************************************
    Complex Exp Helper
**************************************************************************/

template<typename TComplex>
inline void TFPUcplxexp(TComplex *x, TComplex *z);

template<>
inline void TFPUcplxexp<Complex>(Complex *x, Complex *z)
    {
    double e2x = exp(x->x);
    double siny, cosy;

    TSinCos(x->y, siny, cosy);

    z->x = e2x * cosy;
    z->y = e2x * siny;
    }

/**************************************************************************
    Complex Exp
**************************************************************************/

template<typename TComplex>
inline void TStkExp(TComplex* z)
    {
    *z = z->CExp();
    }

/**************************************************************************
    Complex Exp specialization for Complex
**************************************************************************/

template<>
inline void TStkExp<Complex>(Complex* z)
    {
    TFPUcplxexp(z, z);
    }

/**************************************************************************
    Complex Log Helper
**************************************************************************/

template<typename TComplex>
inline void TFPUcplxlog(TComplex *x, TComplex *z);

template<>
inline void TFPUcplxlog<Complex>(Complex *x, Complex *z)
    {
    double mod, zx, zy;

    mod = sqrt(x->x * x->x + x->y * x->y);

    zx = log(mod);
    zy = atan2(x->y, x->x);

    z->x = zx;
    z->y = zy;
    }
    

/**************************************************************************
Complex Log
**************************************************************************/

template<typename TComplex>
inline void TStkLog(TComplex* z)
    {
    *z = z->CLog();
    }

/**************************************************************************
Complex Log specialization for Complex
**************************************************************************/

template<>
inline void TStkLog<Complex>(Complex* z)
    {
    TFPUcplxlog(z, z);
    }

/**************************************************************************
    Complex Power
**************************************************************************/

template<typename TComplex>
inline void TStkPwr(
    TComplex* arg2,
    const TComplex* arg1);

/**************************************************************************
    Complex specialization
**************************************************************************/

template<>
inline void TStkPwr<Complex>(
    Complex* arg2,
    const Complex* arg1)
    {
    *arg2 = ComplexPower(*arg2, *arg1);
    }

/**************************************************************************
    DD specialization
**************************************************************************/

template<>
inline void TStkPwr<DDComplex>(
    DDComplex* arg2,
    const DDComplex* arg1)
    {
    DDComplex temp = *arg1;
    *arg2 = (*arg2) ^ temp;
    }

/**************************************************************************
    QD specialization
**************************************************************************/

template<>
inline void TStkPwr<QDComplex>(
    QDComplex* arg2,
    const QDComplex* arg1)
    {
    QDComplex temp = *arg1;
    *arg2 = (*arg2) ^ temp;
    }

/**************************************************************************
    Complex Tangent
**************************************************************************/

template<typename TComplex>
inline void TStkTan(TComplex* z)
    {
    using TReal = decltype(z->x);

    TReal sinx, cosx, sinhy, coshy, denom;

    z->x *= 2;
    z->y *= 2;

    TSinCos(z->x, sinx, cosx);
    TSinhCosh(z->y, sinhy, coshy);

    denom = cosx + coshy;

    z->x = sinx / denom;
    z->y = sinhy / denom;
    }

/**************************************************************************
Complex Hyperbolic Tangent
**************************************************************************/

template<typename TComplex>
inline void TStkTanh(TComplex* z)
    {
    using TReal = decltype(z->x);

    TReal siny, cosy, sinhx, coshx, denom;

    z->x *= 2;
    z->y *= 2;

    TSinCos(z->y, siny, cosy);
    TSinhCosh(z->x, sinhx, coshx);

    denom = coshx + cosy;

    z->x = sinhx / denom;
    z->y = siny / denom;
    }

/**************************************************************************
Complex Cotangent
**************************************************************************/

template<typename TComplex>
inline void TStkCoTan(TComplex* z)
    {
    using TReal = decltype(z->x);

    TReal sinx, cosx, sinhy, coshy, denom;

    z->x *= 2;
    z->y *= 2;

    TSinCos(z->x, sinx, cosx);
    TSinhCosh(z->y, sinhy, coshy);

    denom = coshy - cosx;

    z->x = sinx / denom;
    z->y = -sinhy / denom;
    }

/**************************************************************************
Complex Hyperbolic Cotangent
**************************************************************************/

template<typename TComplex>
inline void TStkCoTanh(TComplex* z)
    {
    using TReal = decltype(z->x);

    TReal siny, cosy, sinhx, coshx, denom;

    z->x *= 2;
    z->y *= 2;

    TSinCos(z->y, siny, cosy);
    TSinhCosh(z->x, sinhx, coshx);

    denom = coshx - cosy;

    z->x = sinhx / denom;
    z->y = -siny / denom;
    }

// The following Complex function routines added by Tim Wegner November 1994.

/**************************************************************************
    ArcSinh Helper
**************************************************************************/

template<typename TComplex>
inline void TArcsinhz(TComplex z, TComplex* rz)
    {
    TComplex temp = z;

    TStkMul(&temp, &z);

    temp.x += 1;

    temp = temp.CSqrt();

    temp.x += z.x;
    temp.y += z.y;

    *rz = temp.CLog();
    }

/**************************************************************************
    ArcSinh
**************************************************************************/

template<typename TComplex>
inline void TStkASinh(TComplex* z)
    {
    TArcsinhz(*z, z);
    }

/**************************************************************************
ArcSin Helper
**************************************************************************/

template<typename TComplex>
inline void TArcsinz(TComplex z, TComplex* rz)
    {
    TComplex temp1;
    TComplex temp2;

    temp1 = z;

    TStkMul(&temp1, &z);

    temp1.x = 1 - temp1.x;
    temp1.y = -temp1.y;

    temp1 = temp1.CSqrt();

    temp2.x = -z.y;
    temp2.y = z.x;

    temp1.x += temp2.x;
    temp1.y += temp2.y;

    temp1 = temp1.CLog();

    rz->x = temp1.y;
    rz->y = -temp1.x;
    }

template<typename TComplex>
inline void TStkASin(TComplex* z)
    {
    TArcsinz(*z, z);
    }

/**************************************************************************
    ArcCos Helper
**************************************************************************/

template<typename TComplex>
inline void TArccosz(TComplex z, TComplex* rz)
    {
    TComplex temp = z;

    TStkMul(&temp, &z);

    temp.x -= 1;

    temp = temp.CSqrt();

    temp.x += z.x;
    temp.y += z.y;

    temp = temp.CLog();

    rz->x = temp.y;
    rz->y = -temp.x;
    }

template<typename TComplex>
inline void TStkACos(TComplex* z)
    {
    TArccosz(*z, z);
    }

/**************************************************************************
ArcCosh Helper
**************************************************************************/

template<typename TComplex>
inline void TArccoshz(TComplex z, TComplex* rz)
    {
    TComplex temp = z;

    TStkMul(&temp, &z);

    temp.x -= 1;

    temp = temp.CSqrt();

    temp.x += z.x;
    temp.y += z.y;

    *rz = temp.CLog();
    }

template<typename TComplex>
inline void TStkACosh(TComplex* z)
    {
    TArccoshz(*z, z);
    }

/**************************************************************************
ArcTan Helper
**************************************************************************/

template<typename TComplex>
inline void TArctanz(TComplex z, TComplex* rz)
    {
    TComplex temp0;
    TComplex temp1;
    TComplex temp2;

    temp0.x = -z.y;
    temp0.y = z.x;

    temp1.x = 1 - temp0.x;
    temp1.y = -temp0.y;

    temp2.x = 1 + temp0.x;
    temp2.y = temp0.y;

    TStkDiv(&temp1, &temp2);

    temp1 = temp1.CLog();

    rz->x = -temp1.y * 0.5;
    rz->y = temp1.x * 0.5;
    }

template<typename TComplex>
inline void TStkATan(TComplex* z)
    {
    TArctanz(*z, z);
    }

/**************************************************************************
ArcTanh Helper
**************************************************************************/

template<typename TComplex>
inline void TArctanhz(TComplex z, TComplex* rz)
    {
    TComplex temp0;
    TComplex temp1;

    temp0.x = 1 + z.x;
    temp0.y = z.y;

    temp1.x = 1 - z.x;
    temp1.y = -z.y;

    TStkDiv(&temp0, &temp1);

    temp0 = temp0.CLog();

    rz->x = temp0.x * 0.5;
    rz->y = temp0.y * 0.5;
    }

template<typename TComplex>
inline void TStkATanh(TComplex* z)
    {
    TArctanhz(*z, z);
    }

/**************************************************************************
Truth Test Helper
**************************************************************************/

template<typename TComplex>
inline bool TIsTrue(const TComplex& z)
    {
    return z.x != 0;
    }

/**************************************************************************
Jump On True
**************************************************************************/

template<typename TComplex>
inline void TStkJumpOnTrue(TComplex* z)
    {
    if (TIsTrue(*z))
	StkJump();
    else
	Ctx().jump_cursor++;
    }

/**************************************************************************
Jump On False
**************************************************************************/

template<typename TComplex>
inline void TStkJumpOnFalse(TComplex* z)
    {
    if (!TIsTrue(*z))
	StkJump();
    else
	Ctx().jump_cursor++;
    }

/**************************************************************************
SRand
**************************************************************************/

template<typename TComplex>
inline void TStkSRand(TComplex* z)
    {
    long x, y;

    x = rand15();
    y = rand15();

    z->x = (double)x / 12000.0;
    z->y = (double)y / 12000.0;
    }

/**************************************************************************
Generic Complex Trig Dispatcher
**************************************************************************/

template<typename TComplex>
inline void TCMPLXtrig(TComplex* zIn, TComplex* zOut, int index)
    {
    TComplex z = *zIn;

    switch (index)
	{
	case 0:
	    TStkSin(&z);
	    break;
	case 1:
	    TStkSinh(&z);
	    break;
	case 2:
	    TStkCos(&z);
	    break;
	case 3:
	    TStkCosh(&z);
	    break;
	case 4:
	    TStkSqr(&z);
	    break;
	case 5:
	    TStkLog(&z);
	    break;
	case 6:
	    TStkExp(&z);
	    break;
	case 7:
	    TStkAbs(&z);
	    break;
	case 8:
	    TStkConj(&z);
	    break;
	case 9:
	    TStkReal(&z);
	    break;
	case 10:
	    TStkImag(&z);
	    break;
	case 11:
	    TStkIdent(&z);
	    break;
	case 12:
	    TStkRecip(&z);
	    break;
	case 13:		// Fn1 - not sure what the next for really mean but left in for consistency
	    TStkSin(&z);
	    break;
	case 14:		// Fn2
	    TStkSqr(&z);
	    break;
	case 15:		// Fn3
	    TStkSinh(&z);
	    break;
	case 16:		// Fn4
	    TStkCosh(&z);
	    break;
	case 17:
	    TStkFlip(&z);
	    break;
	case 18:
	    TStkTan(&z);
	    break;
	case 19:
	    TStkTanh(&z);
	    break;
	case 20:
	    TStkCoTan(&z);
	    break;
	case 21:
	    TStkCoTanh(&z);
	    break;
	case 22:
	    TStkCosXX(&z);
	    break;
	case 23:
	    TStkSRand(&z);
	    break;
	case 24:
	    TStkASin(&z);
	    break;
	case 25:
	    TStkASinh(&z);
	    break;
	case 26:
	    TStkACos(&z);
	    break;
	case 27:
	    TStkACosh(&z);
	    break;
	case 28:
	    TStkATan(&z);
	    break;
	case 29:
	    TStkATanh(&z);
	    break;
	case 30:
	    TStkSqrt(&z);
	    break;
	case 31:
	    TStkCAbs(&z);
	    break;
	case 32:
	    TStkFloor(&z);
	    break;
	case 33:
	    TStkCeil(&z);
	    break;
	case 34:
	    TStkTrunc(&z);
	    break;
	case 35:
	    TStkRound(&z);
	    break;

	default:
	    TStkSin(&z);
	    break;
	}

    *zOut = z;
    }

    /**************************************************************************
    Generic HyperComplex Trig Dispatcher
**************************************************************************/

    template<typename THComplex, typename TComplex>
    inline void THComplexTrig(THComplex* h, THComplex* out, int index)
	{
	TComplex a, b, resulta, resultb;

	// convert to duplex form
	a.x = h->x - h->t;
	a.y = h->y + h->z;

	b.x = h->x + h->t;
	b.y = h->y - h->z;

	// apply function to each duplex part
	TCMPLXtrig(&a, &resulta, index);
	TCMPLXtrig(&b, &resultb, index);

	// convert back
	out->x = (resulta.x + resultb.x) / 2;
	out->y = (resulta.y + resultb.y) / 2;
	out->z = (resulta.y - resultb.y) / 2;
	out->t = (resultb.x - resulta.x) / 2;
	}


