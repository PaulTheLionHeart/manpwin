#include <windows.h>
#include <functional>
#include <cmath>

#pragma once

// =============================
// Trig dispatch helpers
// =============================
template<typename TComplex>
struct TrigCaller;

// specialisations...

// =============================
// Fractint trig init template
// =============================

template<>
struct TrigCaller<Complex>
    {
    static void Call(CTrigFn& trig, Complex* in, Complex* out, int fn)
	{
	trig.CMPLXtrig(in, out, fn);
	}
    };

template<>
struct TrigCaller<DDComplex>
    {
    static void Call(CTrigFn& trig, DDComplex* in, DDComplex* out, int fn)
	{
	trig.DDCMPLXtrig(in, out, fn);
	}
    };

template<>
struct TrigCaller<QDComplex>
    {
    static void Call(CTrigFn& trig, QDComplex* in, QDComplex* out, int fn)
	{
	trig.QDCMPLXtrig(in, out, fn);
	}
    };

// =====================================
// Fractint Trig Fractals standard init
// =====================================

template<typename TComplex, typename TReal>
void InitFractintTrigFunctionsT(WORD type, TComplex *z, TComplex *q,    // explicitly pass what you need:
    bool juliaflag, bool invert, TComplex c,
    TReal& qc, TReal& qci, TReal& qcj, TReal& qck,			// These are for quaternions and hypercomplex
    TComplex& t, TComplex& temp, TComplex& temp1, 
    TComplex& temp2, TComplex& temp4, TComplex& sqr, int Fn2Index, double* param)
    {
    CTrigFn TrigFn;
    switch (type)
	{
	case HYPERCMPLXFP:
	case HYPERCMPLXJFP:
	    {
	    if (juliaflag)
		{
		*z = *q;
		temp.x = t.x;
		temp.y = t.y;
		}
	    else
		{
		*z = 0.0;
		temp = 0.0;
		}

	    qc = t.x;
	    qci = t.y;
	    qcj = param[2];
	    qck = param[3];
	    break;
	    }

	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	case NUMFRACTAL:
	    {
	    // t already computed before switch

	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];

	    if (juliaflag)
		{
		temp.x = q->x;
		temp.y = q->y;
		}
	    else
		{
		z->x = t.x + param[0];
		z->y = t.y + param[1];
		}

	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    temp1 = *z;
	    temp1.x -= 1.0;

	    temp1 = (*z) ^ temp1;

	    break;
	    }

	case FNPLUSFNPIXFP:
	case FNPLUSFNPIXLONG:
	    {
	    // t already computed outside switch

	    if (!juliaflag)
		{
		*z = t;
		}

	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];

	    z->x += temp.x;
	    z->y += temp.y;

	    temp4 = *z;

	    TrigCaller<TComplex>::Call(TrigFn, &temp4, &temp1, Fn2Index);

	    temp1 = temp1 * temp2;

	    break;
	    }

	case TRIGSQRFP:
	case TRIGSQR:
	case TRIGXTRIGFP:
	case TRIGXTRIG:
	case ZXTRIGPLUSZFP:
	case ZXTRIGPLUSZ:
	case TRIGPLUSTRIGFP:
	case TRIGPLUSTRIG:
	case SQRTRIGFP:
	case SQRTRIG:
	case SQR1OVERTRIGFP:
	case SQR1OVERTRIG:
	    {
	    if (!juliaflag)
		{
		*z = t;
		}

	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];

	    break;
	    }

	case MANDELTRIGFP:
	case MANDELTRIG:
	case FPMANTRIGPLUSEXP:
	case LAMBDATRIGFP:
	case LAMBDATRIG:
	case FPMANTRIGPLUSZSQRD:
	case FPJULTRIGPLUSZSQRD:
	case LMANTRIGPLUSZSQRD:
	case LJULTRIGPLUSZSQRD:
	case LJULFNFN:
	case FPJULFNFN:
	case FPMANFNFN:
	case LMANFNFN:
	case LMANLAMFNFN:
	case LLAMBDAFNFN:
	case FPLAMBDAFNFN:
	case FPMANLAMFNFN:
	    {
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];

	    if (juliaflag)
		{
		t.x = q->x;
		t.y = q->y;
		}
	    else
		{
		z->x = t.x + param[0];
		z->y = t.y + param[1];
		}

	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    temp1 = *z;

	    break;
	    }
	}
    }

// =============================
// Variable helpers
// =============================

template<typename TComplex>
struct HyperComplexType;

// specialisations...

// =============================
// Fractint trig HyperComplex template
// =============================

template<>
struct HyperComplexType<Complex>
    {
    using type = _HCMPLX;
    };

template<>
struct HyperComplexType<DDComplex>
    {
    using type = _DDHCMPLX;
    };

template<>
struct HyperComplexType<QDComplex>
    {
    using type = _QDHCMPLX;
    };

// =============================
// Trig function helpers
// =============================

template<typename TComplex>
struct HyperTrigCaller;

// specialisations...

// =============================
// Fractint trig HyperComplex function template
// =============================

template<>
struct HyperTrigCaller<Complex>
    {
    static void Call(CTrigFn& trig, _HCMPLX* in, _HCMPLX* out, int fn)
	{
	trig.HComplexTrig(in, out, fn);
	}
    };

template<>
struct HyperTrigCaller<DDComplex>
    {
    static void Call(CTrigFn& trig, _DDHCMPLX* in, _DDHCMPLX* out, int fn)
	{
	trig.DDHComplexTrig(in, out, fn);
	}
    };

template<>
struct HyperTrigCaller<QDComplex>
    {
    static void Call(CTrigFn& trig, _QDHCMPLX* in, _QDHCMPLX* out, int fn)
	{
	trig.QDHComplexTrig(in, out, fn);
	}
    };

// =====================================
// Fractint Trig Fractals per iteration
// =====================================

template<typename TComplex, typename TReal>
int FractintTrigIterT(
    int type,
    TComplex* z,
    TComplex* q,
    TComplex& temp,
    TComplex& temp1,
    TComplex& temp2,
    TComplex& temp3,
    TComplex& sqr,
    TReal& qc, TReal& qci, TReal& qcj, TReal& qck,
    int Fn1Index,
    int Fn2Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    switch (type)
	{
	case HYPERCMPLXFP:
	case HYPERCMPLXJFP:
	    {
	    using THyper = typename HyperComplexType<TComplex>::type;
	    THyper hold, hnew;	    
	    
	    TReal magnitude;

	    hold.x = z->x;
	    hold.y = z->y;
	    hold.z = temp1.x;
	    hold.t = temp1.y;

	    HyperTrigCaller<TComplex>::Call(TrigFn, &hold, &hnew, Fn1Index);

	    hnew.x += qc;
	    hnew.y += qci;
	    hnew.z += qcj;
	    hnew.t += qck;

	    z->x = hnew.x;
	    z->y = hnew.y;
	    temp1.x = hnew.z;
	    temp1.y = hnew.t;

	    magnitude =
		z->x * z->x +
		z->y * z->y +
		temp1.x * temp1.x +
		temp1.y * temp1.y;

	    return (magnitude > rqlim);
	    }

	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	    {
	    TrigCaller<TComplex>::Call(TrigFn, z, &temp2, Fn1Index);

	    temp2 = temp1 * temp2;
	    temp2.x += q->x;
	    temp2.y += q->y;

	    *z = temp2;

	    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
	    }

	case FNPLUSFNPIXFP:
	case FNPLUSFNPIXLONG:
	    {
	    TrigCaller<TComplex>::Call(TrigFn, z, &temp3, Fn1Index);

	    temp3 += temp1;
	    *z = temp3;

	    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
	    }

	case TRIGSQRFP:
	case TRIGSQR:
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    temp1.y = (z->x + z->x) * z->y;
	    temp1.x = sqr.x - sqr.y;

	    TrigCaller<TComplex>::Call(TrigFn, &temp1, &temp3, Fn1Index);

	    *z = temp3;

	    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
	    }

	case TRIGXTRIGFP:
	case TRIGXTRIG:
	    {
	    TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);
	    TrigCaller<TComplex>::Call(TrigFn, z, z, Fn2Index);

	    temp3 = (*z) * temp1;
	    *z = temp3;

	    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
	    }

	case ZXTRIGPLUSZFP:
	case ZXTRIGPLUSZ:
	    {
	    if (temp.x == TReal(1) && temp.y == TReal(0) && temp2.y == TReal(0))
		{
		if (temp2.x == TReal(1))
		    return ScottZXTrigPlusZfpFractalT<TComplex, TReal>(
			z, temp1, temp3, Fn1Index, rqlim, BailoutTestType);

		else if (temp2.x == TReal(-1))
		    return SkinnerZXTrigSubZfpFractalT<TComplex, TReal>(
			z, temp, temp1, temp3, Fn1Index, rqlim, BailoutTestType);
		}

	    return ZXTrigPlusZfpFractalT<TComplex, TReal>(
		z, q, temp, temp1, temp2, temp3, Fn1Index, rqlim, BailoutTestType);
	    }


	}
    return 0;
    }

template<typename TComplex, typename TReal>
int TrigPlusTrigFractalT(
    TComplex* z,
    TComplex& temp1,
    TComplex& temp2,
    int Fn1Index,
    int Fn2Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    // trig0(z)
    TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);
    temp1 = temp2 * temp1;

    // trig1(z)
    TrigCaller<TComplex>::Call(TrigFn, z, z, Fn2Index);

    *z = (*z) * temp2;
    *z = temp1 + *z;

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

template<typename TComplex, typename TReal>
int MandelTrigFractalT(
    TComplex* z,
    TComplex& temp1,
    TComplex& temp3,
    TComplex& t,
    int Fn1Index,
    TReal rqlim)
    {
    CTrigFn TrigFn;

    const char* Fn1 = TrigFn.FunctList[Fn1Index];

    if (strcmp(Fn1, "sin") == 0 || strcmp(Fn1, "cos") == 0)
	{
	if (fabs(z->y) >= rqlim)
	    return 1;

	TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);

	temp3 = temp1 * t;
	*z = temp3;
	}
    else if (strcmp(Fn1, "exp") == 0)
	{
	if (fabs(z->y) >= TReal(1.0e8)) return 1;
	if (fabs(z->x) >= TReal(6.4e2)) return 1;

	TReal siny = sin(z->y);
	TReal cosy = cos(z->y);

	if (z->x >= rqlim && cosy >= TReal(0))
	    return 1;

	TReal tmpexp = exp(z->x);

	temp1.x = tmpexp * cosy;
	temp1.y = tmpexp * siny;

	temp3.x = t.x * temp1.x - t.y * temp1.y;
	temp3.y = t.y * temp1.x + t.x * temp1.y;

	*z = temp3;
	}
    else if (strcmp(Fn1, "sinh") == 0 || strcmp(Fn1, "cosh") == 0)
	{
	if (fabs(z->x) >= rqlim)
	    return 1;

	TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);

	temp3 = t * temp1;
	*z = temp3;
	}
    else
	{
	if (fabs(z->y) >= rqlim)
	    return 1;

	TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);

	temp3 = t * temp1;
	*z = temp3;
	}

    return 0;
    }

template<typename TComplex, typename TReal>
int ManTrigPlusExpFractalT(
    TComplex* z,
    TComplex& temp1,
    TComplex& t,
    int Fn1Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    if (fabs(z->x) >= TReal(6.4e2))
	return 1;

    TReal tmpexp = exp(z->x);
    TReal siny = sin(z->y);
    TReal cosy = cos(z->y);

    TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);

    temp1.x += tmpexp * cosy + t.x;
    temp1.y += tmpexp * siny + t.y;

    *z = temp1;

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

template<typename TComplex, typename TReal>
int ManTrigPlusZSqrFractalT(
    TComplex* z,
    TComplex& temp1,
    TComplex& sqr,
    TComplex& t,
    int Fn1Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);

    temp1.x += sqr.x - sqr.y + t.x;
    temp1.y += TReal(2) * z->x * z->y + t.y;

    *z = temp1;

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

template<typename TComplex, typename TReal>
int FnFnPlusFractalT(
    TComplex* z,
    TComplex& t,
    TComplex& temp2,
    int Fn1Index,
    int Fn2Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    if (z->CSumSqr() < temp2.x)
	{
	TrigCaller<TComplex>::Call(TrigFn, z, z, Fn1Index);
	*z = t + *z;
	}
    else
	{
	TrigCaller<TComplex>::Call(TrigFn, z, z, Fn2Index);
	*z = t + *z;
	}

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

template<typename TComplex, typename TReal>
int FnFnMulFractalT(
    TComplex* z,
    TComplex& t,
    TComplex& temp2,
    int Fn1Index,
    int Fn2Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    if (z->CSumSqr() < temp2.x)
	{
	TrigCaller<TComplex>::Call(TrigFn, z, z, Fn1Index);
	*z = t * *z;
	}
    else
	{
	TrigCaller<TComplex>::Call(TrigFn, z, z, Fn2Index);
	*z = t * *z;
	}

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

template<typename TComplex, typename TReal>
int SqrTrigFractalT(
    TComplex* z,
    TComplex& temp1,
    int Fn1Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);
    *z = temp1.CSqr();

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

template<typename TComplex, typename TReal>
int Sqr1OverTrigFractalT(
    TComplex* z,
    int Fn1Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    TrigCaller<TComplex>::Call(TrigFn, z, z, Fn1Index);
    *z = z->CInvert();
    *z = z->CSqr();

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

template<typename TComplex, typename TReal>
int NumFractalT(
    TComplex* z,
    TComplex* q,
    int Fn1Index,
    TReal rqlim,
    int BailoutTestType,
    double* param)
    {
    CTrigFn TrigFn;

    TComplex base;
    base.x = param[2];
    base.y = param[3];

    *z = base ^ *z;
    TrigCaller<TComplex>::Call(TrigFn, z, z, Fn1Index);

    *z += *q;
    z->x += param[0];
    z->y += param[1];

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }
       	  
// =====================================
// Template the base function
// =====================================

// General ZXTrigPlusZ
template<typename TComplex, typename TReal>
int ZXTrigPlusZfpFractalT(
    TComplex* z,
    TComplex* q,
    TComplex& temp,
    TComplex& temp1,
    TComplex& temp2,
    TComplex& temp3,
    int Fn1Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);

    temp1 = temp * temp1;
    temp3 = (*z) * temp1;

    temp1 = (*z) * temp2;

    *z = temp1 + temp3;

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

// Scott variant
template<typename TComplex, typename TReal>
int ScottZXTrigPlusZfpFractalT(
    TComplex* z,
    TComplex& temp1,
    TComplex& temp3,
    int Fn1Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);

    temp3 = (*z) * temp1;
    *z = (*z) + temp3;

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }

// Skinner variant
template<typename TComplex, typename TReal>
int SkinnerZXTrigSubZfpFractalT(
    TComplex* z,
    TComplex& temp,
    TComplex& temp1,
    TComplex& temp3,
    int Fn1Index,
    TReal rqlim,
    int BailoutTestType)
    {
    CTrigFn TrigFn;

    TrigCaller<TComplex>::Call(TrigFn, z, &temp1, Fn1Index);

    temp3 = (*z) * temp1;

    *z = temp - (*z);

    return DoBailout<TComplex, TReal>(BailoutTestType, z, rqlim);
    }



