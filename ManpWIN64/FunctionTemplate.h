#include <windows.h>
#include <functional>

#pragma once

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

template<typename TComplex, typename TReal>
void Init_Basic(
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    TComplex& sqr,
    TReal& real_imag)
    {
    if (!juliaflag)
	{
	z->x = q->x + param[0];
	z->y = q->y + param[1];
	}

    sqr = 0.0;
    real_imag = 0.0;
    }

template<typename TComplex>
void Init_Power(
    WORD type,
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    WORD* degree)
    {
    *degree = (int)param[0];
    if (*degree < 1)
	*degree = 1;

    if (type == JULIA4FP || type == JULIA4 ||
	type == MANDEL4FP || type == MANDEL4)
	*degree = 4;

    if (!juliaflag)
	{
	z->x = q->x + param[1];
	z->y = q->y + param[2];
	}
    }

template<typename TComplex>
void Init_Cubic(
    TComplex* z,
    TComplex* q,
    double* param,
    BYTE juliaflag,
    int& subtype,
    WORD& special,
    TComplex& a,
    TComplex& b,
    TComplex& a2,
    TComplex& aa3,
    TComplex& t2,
    TComplex& t3,
    TComplex& temp,
    TComplex& v)
    {
    // --- subtype ---
    switch ((int)param[0])
	{
	case 0: subtype = 'B'; break;
	case 1: subtype = 'C'; break;
	case 2: subtype = 'F'; break;
	case 3: subtype = 'K'; break;
	default: subtype = 'B'; break;
	}

    // --- special ---
    special = ((int)param[1] < 0) ? 0 : (int)param[1];

    // --- calculations ---
    if (subtype == 'B')
	{
	t3 = (*q) * TComplex(3.0);
	t2 = q->CSqr();

	a = (t2 + TComplex(1.0)) / t3;

	temp = a.CCube() * TComplex(2.0);
	b = (t2 - TComplex(2.0)) / t3 + temp;
	}
    else if (subtype == 'C' || subtype == 'F')
	{
	a = *q;
	temp = q->CCube();

	if (subtype == 'C')
	    {
	    b = temp + temp + *q;
	    }
	else
	    {
	    b = (temp - *q) * TComplex(2.0);
	    a2 = a + a;
	    }
	}
    else if (subtype == 'K')
	{
	a = TComplex(0.0);
	v = TComplex(0.0);
	b = *q;
	}

    aa3 = a.CSqr() * TComplex(3.0);

    if (!juliaflag)
	*z = -a;
    }

template<typename TComplex>
void Init_SPECIALNEWT(
    TComplex* z,
    TComplex* q,
    TComplex& a,
    TComplex& b,
    TComplex& lm5,
    TComplex& lp5)
    {
    TComplex l2 = q->CSqr();

    a = -l2 + TComplex(0.25);
    b = -l2 - TComplex(0.75);

    lm5 = (*q) - TComplex(0.5);
    lp5 = (*q) + TComplex(0.5);
    }

template<typename TComplex>
void Init_SINFRACTAL(
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag)
    {
    if (!juliaflag)
	{
	*z = TComplex(param[3]);  // real only, imag = 0
	}
    }

template<typename TComplex, typename TDegree>
void Init_TALIS(
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    TDegree* degree)
    {
    *degree = (TDegree)param[0];

    if (*degree < (TDegree)1)
	*degree = (TDegree)1;

    if (!juliaflag)
	{
	z->x = q->x + param[2];
	z->y = q->y + param[3];
	}
    }

template<typename TComplex>
void Init_POLYNOMIAL(
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag)
    {
    if (!juliaflag)
	{
	z->x = q->x + param[0];
	z->y = q->y + param[1];
	}
    }

template<typename TComplex, typename TReal>
int Init_MATEIN(
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    TComplex& oz,
    TComplex& temp,
    double& distance,
    double& absolute)
    {
    absolute = q->CSumSqr();

    if (absolute > 1.0)
	return -1;   // outside set

    if (!juliaflag)
	*z = TComplex(1.0);

    // --- pre-iteration loop ---
    for (int i = 0; i < 100; ++i)
	{
	temp = z->CInvert();
	*z = (*q) * (*z + temp);
	}

    distance = 1.0;
    oz = z->CInvert();

    return 0;
    }

template<typename TComplex, typename TDegree>
void Init_REDSHIFTRIDER(
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    TComplex& a,
    TDegree* degree)
    {
    // parameter a
    a.x = param[0];
    a.y = param[1];

    // degree
    *degree = (TDegree)(int)param[2];

    // initial z
    if (!juliaflag)
	{
	z->x = q->x + param[3];
	z->y = q->y + param[4];
	}
    }

template<typename TComplex, typename TReal>
int Init_RATIONALMAP(
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    char subtype,
    TComplex& a,
    TComplex& b,
    TComplex& alpha,
    TComplex& temp,
    TComplex& temp1,
    TComplex& temp3)
    {
    TComplex cmcc;

    if (subtype == 'A')
	{
	cmcc = *q - q->CSqr();

	temp = -(*q) + TComplex(2.0);
	a = temp / cmcc;

	temp = cmcc + TComplex(1.0);
	b = -temp / cmcc;

	// ALPHA calculation
	temp = a * (*q) * TComplex(2.0) + b;
	temp1 = b.CSqr() / a + b + b;
	temp3 = q->CSqr() * temp1 * temp;

	alpha = temp3.CInvert();
	}
    else if (subtype == 'B')
	{
	a = *q;
	b = a + TComplex(1.0);

	temp = a.CSqr() - TComplex(1.0);
	alpha = a / temp;
	}
    else
	{
	return -1;
	}

    // --- escape safety ---
    if (alpha.x == TReal(0.0) && alpha.y == TReal(0.0))
	return -1;

    // --- initial Z ---
    if (!juliaflag)
	{
	temp = -a * TComplex(2.0);
	*z = b / temp;
	}

    // --- pre-iteration ---
    int zcount = 0;

    if (subtype == 'A')
	zcount = juliaflag ? 4 : 2;
    else if (subtype == 'B')
	zcount = 3;

    for (int i = 0; i < zcount; ++i)
	{
	temp = b * (*z) + TComplex(1.0);
	temp1 = z->CSqr() * a + temp;
	*z = temp1.CInvert();
	}

    return 0;
    }

// ------------------------------------------------------------
// Template version of RunFunctions 
// ------------------------------------------------------------

template<typename TComplex, typename TReal, typename TDegree>
inline int FunctionsDispatch(
    CPixel* self,
    WORD type,
    TComplex* z,
    TComplex* q,
    TDegree* degree,
    TComplex& sqr,
    TReal& real_imag,
    TComplex& a,
    TComplex& a2,
    TComplex& aa3,
    TComplex& b,
    TComplex& v,
    TComplex& temp,
    double* param,
    BYTE* SpecialFlag,
    long* iteration,
    WORD special,
    char subtype,
    int bailout_type,
    TReal rqlim)
    {
    switch (type)
	{
	case MANDELFP:					// Mandelbrot
	case MANDEL:					// to handle fractint par files
	case JULIA:					// to handle fractint par files
	case JULIAFP:					// like he said
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;

	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;

	    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim);

	case POWER:					// Power
	case JULIA4FP:
	case JULIA4:
	case MANDEL4FP:
	case MANDEL4:
	    {
	    switch (*degree)
		{
		case 2:
		    *z = z->CSqr() + *q;
		    break;

		case 3:
		    *z = z->CCube() + *q;
		    break;

		default:
		    *z = z->CPolynomial(*degree) + *q;
		    break;
		}

	    return DoBailout<TComplex>(bailout_type, z, rqlim);
	    }

	case BURNINGSHIP:				// Burning Ship
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = fabs(z->x * z->y);
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));

	case BURNINGSHIPPOWER:				// Burning Ship to higher power
	    z->x = fabs(z->x);
	    z->y = -fabs(z->y);
	    switch (*degree)
		{
		case 2:
		    *z = z->CSqr() + *q;
		    break;

		case 3:
		    *z = z->CCube() + *q;
		    break;

		default:
		    *z = z->CPolynomial(*degree) + *q;
		    break;
		}
	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));

	case CUBIC:					// Art Matrix Cubic
	    {
	    if (subtype == 'K')				// CKIN
		{
		*z = z->CCube() + b;
		z->x += param[2];
		z->y += param[3];
		}
	    else
		{
		temp = z->CCube() + b;
		*z = temp - aa3 * *z;
		z->x += param[2];
		z->y += param[3];
		}

	    if (z->CSumSqr() > 100.0)
		return TRUE;
	    else
		{
		if (subtype == 'F')
		    {
		    if (q->CSumSqr() < 0.111111)
			{
			*iteration = special;
			*SpecialFlag = TRUE;
			return TRUE;
			}
		    v = *z + a2;
		    }
		else if (subtype == 'K')
		    v = *z - v;
		else
		    v = *z - a;

		if (v.CSumSqr() <= 0.000001)
		    {
		    *iteration = special;
		    *SpecialFlag = TRUE;
		    return TRUE;
		    }

		return FALSE;
		}
	    }
	}

    return 0;
    }

template<typename TComplex>
BOOL Iter_SPECIALNEWT(				// Art Matrix Newton
	TComplex* z,
	const TComplex* q,
	TComplex& a,
	TComplex& b,
	TComplex& lm5,
	TComplex& lp5,
	TComplex& v,
	double* param,
	BYTE& phaseflag,
	WORD& special)
    {
    if ((int)param[0] < 0)
	special = 2;
    else
	special = (int)param[0];

    TComplex z2 = z->CSqr();

    TComplex top = z2 * (*z) * 2 + a;
    TComplex bottom = z2 * 3 + b;

    *z = top / bottom;

    z->x += param[1];
    z->y += param[2];

    v = *z - TComplex(1.0);

    if (v.CSumSqr() <= 0.000001)
	{
	phaseflag = 0;
	return TRUE;
	}

    v = *z - lm5;
    if (v.CSumSqr() <= 0.000001)
	{
	phaseflag = 1;
	return TRUE;
	}

    v = *z + lp5;
    if (v.CSumSqr() <= 0.000001)
	{
	phaseflag = 2;
	return TRUE;
	}

    return FALSE;
    }

template<typename TComplex>
BOOL Iter_MATEIN(					// Art Matriuc Matein fractal
    TComplex* z,
    TComplex* q,
    TComplex& oz,
    TComplex& temp,
    double& distance,
    double absolute,
    double* param,
    double& epsilon,
    double& escape,
    BYTE& phaseflag)
    {
    epsilon = 0.01;
    escape = 10.0E20;

    *z = (*q) * (*z + oz);

    z->x += param[0];
    z->y += param[1];

    oz = z->CInvert();

    temp = -oz / *z + TComplex(1.0);

    distance = distance * absolute * temp.CSumSqr();

    if (distance <= epsilon)
	{
	phaseflag = 0;
	return TRUE;
	}

    if (distance > escape)
	{
	phaseflag = 1;
	return TRUE;
	}

    return FALSE;
    }

/**************************************************************************
    Determine count before 'Z' becomes unstable
    Z = L*SIN(Z)
    sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/
template<typename TComplex, typename TReal>
bool Iter_SINFRACTAL(					// Sine
    CPixel* self,
    TComplex* z,
    TComplex* q,
    double* param,
    int bailout_type,
    TReal rqlim)
    {
    if (param[2] == 0.0)
	*z = (*q) * z->CSin();
    else
	*z = (*q) + z->CSin();

    z->x += param[0];
    z->y += param[1];

    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
    }

template<typename TComplex>
BOOL Iter_EXP_FRACTAL(					// Exponential
    TComplex* z,
    TComplex* q,
    double* param,
    WORD* degree,
    int& subtype,
    double rqlim)
    {
    int compare;

    *degree = (int)(fabs(param[3]));

    for (int i = 0; i < *degree; i++)
	*z = z->CExp();

    if (param[4] == 0.0)
	*z = (*q) * (*z);
    else
	*z = (*q) + (*z);

    switch ((int)param[0])
	{
	case 0:
	    subtype = 'R';
	    compare = (z->x >= rqlim);
	    break;

	case 1:
	    subtype = 'I';
	    compare = (z->y >= rqlim);
	    break;

	case 2:
	    subtype = 'M';
	    compare = (z->x >= rqlim || z->x <= -rqlim ||
		z->y >= rqlim || z->y <= -rqlim);
	    break;

	default:
	    subtype = 'R';
	    compare = (z->x >= rqlim);
	    break;
	}

    z->x += param[1];
    z->y += param[2];

    return compare ? TRUE : FALSE;
    }


template<typename TComplex, typename TReal>
bool Iter_REDSHIFTRIDER(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& a,
    double* param,
    WORD* degree,
    int bailout_type,
    TReal rqlim)
    {
    TComplex term =
	((param[5] == 1.0) ? TComplex(1.0) : TComplex(-1.0))
	* z->CPolynomial(*degree);

    *z = a * (*z) * (*z) + term;
    *z = (*z) + (*q);

    return DoBailout<TComplex>(bailout_type, z, rqlim);
    }

template<typename TComplex, typename TReal>
bool Iter_TALIS(					// Talis Power    Z = Z^N/(M + Z^(N-1)) + C
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& a,
    double* param,
    WORD* degree,
    int bailout_type,
    TReal rqlim)
    {
    double m = param[1];

    a = z->CPolynomial(*degree - 1);

    *z = (a * (*z)) / (TComplex(m) + a) + (*q);

    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
    }

template<typename TComplex, typename TReal>
bool Iter_POLYNOMIAL(					// Polynomial
    CPixel* self,
    TComplex* z,
    TComplex* q,
    double* param,
    int bailout_type,
    TReal rqlim)
    {
    TComplex InitialZ = *z;
    TComplex FinalZ = TComplex(0.0);

    for (int m = 0; m < MAXPOLYDEG; m++)
	{
	if (param[2 + m] != 0.0)
	    {
	    TComplex term = InitialZ;

	    for (int k = 0; k < MAXPOLYDEG - m - 1; k++)
		term *= InitialZ;

	    FinalZ += term * param[2 + m];
	    }
	}

    *z = FinalZ + *q;

    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
    }

template<typename TComplex, typename  TReal>
bool Iter_RATIONALMAP(					// Art Matrix Rational Map
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& a,
    TComplex& b,
    double* param,
    TReal& der,
    TReal escape,
    TReal epsilon,
    long* iteration,
    long threshold,
    WORD special,
    TComplex& alpha,
    long* color)
    {
    TComplex az = a * (*z);              // AZ = A*Z

    TComplex temp = az * (*z) + TComplex(1.0);
    TComplex temp1 = b * (*z) + temp;

    *z = temp1.CInvert();               // Z = 1/(AZ*Z + B*Z + 1)

    z->x += param[2];
    z->y += param[3];

    if (z->x * z->x + z->y * z->y > escape)
	{
	int idx = *iteration % 4;
	if ((alpha.x * z->y + alpha.y * z->x) <= 0.0)
	    *color = self->GetPenP(idx);
	else
	    *color = self->GetPenN(idx);

	*iteration = *color;
	return true;
	}

    TComplex temp2 = az * TComplex(2.0) + b;
    TComplex d = z->CSqr() * temp2;

    double dist = d.CSumSqr();
    der *= dist;

    if (der < epsilon)
	{
	*iteration = threshold;
	return true;
	}

    if (*iteration >= threshold)
	{
	*iteration = special;
	return true;
	}

    return false;
    }

