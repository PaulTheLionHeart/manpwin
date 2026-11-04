/*
    FunctionTemplate.H - Template for handling ManpWIN native Functions for all arithmetic types.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <functional>
#include "DDTrig.h"
#include "QDTrig.h"

#pragma once

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

/**************************************************************************
    Initialise basic Mandelbrot / Julia style functions.

    Mandelbrot:
	z = pixel + initial perturbation

    Julia:
	preserve the supplied starting value

    sqr and real_imag are cleared ready for the iteration loop.
**************************************************************************/

template<typename TComplex, typename TReal>
void Init_Basic(TComplex* z, TComplex* q, double* param, bool juliaflag, TComplex& sqr, TReal& real_imag)
    {
    if (!juliaflag)
	{
	z->x = q->x + param[0];
	z->y = q->y + param[1];
	}

    sqr = 0.0;
    real_imag = 0.0;
    }

/**************************************************************************
    Initialise integer-power Mandelbrot / Julia functions.

    param[0] specifies the power, with a minimum value of 1.
    The fourth-power fractal types force the degree to 4.

    Mandelbrot:
	z = pixel + initial perturbation

    Julia:
	preserve the supplied starting value.
**************************************************************************/

template<typename TComplex>
void Init_Power(WORD type, TComplex* z, TComplex* q, double* param, bool juliaflag, WORD* degree)
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

/**************************************************************************
    Initialise the Art Matrix Cubic fractals.

    The selected subtype determines the coefficients used by the cubic
    recurrence:

	B = basic cubic
	C = cubic variant
	F = Fatou-style variant
	K = CKIN variant

    The required coefficients and working values are precomputed here
    so the iteration routine contains only the recurrence itself.
**************************************************************************/

template<typename TComplex>
void Init_Cubic(TComplex* z, TComplex* q, double* param, BYTE juliaflag, int& subtype, TComplex& a, TComplex& b, TComplex& a2, TComplex& aa3, TComplex& t2, TComplex& t3, TComplex& temp, TComplex& v)
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

/**************************************************************************
    Initialise the Art Matrix Special Newton fractal.

    Precompute the constants used by the Newton iteration from the
    pixel coordinate q, including the three convergence targets.
**************************************************************************/

template<typename TComplex>
void Init_SPECIALNEWT(TComplex* z, TComplex* q, TComplex& a, TComplex& b, TComplex& lm5, TComplex& lp5)
    {
    TComplex l2 = q->CSqr();

    a = -l2 + TComplex(0.25);
    b = -l2 - TComplex(0.75);

    lm5 = (*q) - TComplex(0.5);
    lp5 = (*q) + TComplex(0.5);
    }

/**************************************************************************
    Initialise the sine fractal.

    Mandelbrot:
	z is initialised from param[3] on the real axis.

    Julia:
	preserve the supplied starting value.
**************************************************************************/

template<typename TComplex>
void Init_SINFRACTAL(TComplex* z, TComplex* q, double* param, bool juliaflag)
    {
    if (!juliaflag)
	{
	*z = TComplex(param[3]);  // real only, imag = 0
	}
    }

/**************************************************************************
    Initialise the Talis power fractal.

    param[0] specifies the integer degree, with a minimum value of 1.

    Mandelbrot:
	z = pixel + initial perturbation

    Julia:
	preserve the supplied starting value.
**************************************************************************/

template<typename TComplex, typename TDegree>
void Init_TALIS(TComplex* z, TComplex* q, double* param, bool juliaflag, TDegree* degree)
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

/**************************************************************************
    Initialise the polynomial fractal.

    Mandelbrot:
	z = pixel + initial perturbation

    Julia:
	preserve the supplied starting value.

    Polynomial coefficients are read directly from param[] during
    iteration.
**************************************************************************/

template<typename TComplex>
void Init_POLYNOMIAL(TComplex* z, TComplex* q, double* param, bool juliaflag)
    {
    if (!juliaflag)
	{
	z->x = q->x + param[0];
	z->y = q->y + param[1];
	}
    }

/**************************************************************************
    Initialise the Art Matrix Matein fractal.

    Reject pixels outside the unit circle, then perform the original
    100-step pre-iteration used to establish the starting orbit.

    oz stores the inverse of the resulting z and distance begins at 1
    for the convergence / escape calculation used during iteration.
**************************************************************************/

template<typename TComplex, typename TReal>
int Init_MATEIN(TComplex* z, TComplex* q, double* param, bool juliaflag, TComplex& oz, TComplex& temp, double& distance, double& absolute)
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

/**************************************************************************
    Initialise the Red Shift Rider fractal.

    param[0], param[1] define the complex coefficient a.
    param[2] defines the integer degree.

    Mandelbrot:
	z = pixel + initial perturbation

    Julia:
	preserve the supplied starting value.
**************************************************************************/

template<typename TComplex, typename TDegree>
void Init_REDSHIFTRIDER(TComplex* z, TComplex* q, double* param, bool juliaflag, TComplex& a, TDegree* degree)
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

/**************************************************************************
    Initialise the Art Matrix Rational Map.

    Subtype A and subtype B use different coefficient constructions.
    The required constants a, b and alpha are calculated here before
    the orbit begins.

    The routine also performs the subtype-specific pre-iterations used
    to establish the initial z value.

    Returns -1 when the selected subtype or resulting coefficients are
    invalid, otherwise returns 0.
**************************************************************************/

template<typename TComplex, typename TReal>
int Init_RATIONALMAP(TComplex* z, TComplex* q, double* param, bool juliaflag, char subtype, TComplex& a, TComplex& b, TComplex& alpha, TComplex& temp, TComplex& temp1, TComplex& temp3)
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

/**************************************************************************
    Initialise Kleinian fractal.

    Original implementation by Paul de Leeuw,
    based on work by hrkalona.

    Copyright (C) 2019 hrkalona.

    param[0] = u
    param[1] = v1
    param[2] = K
    param[3] = M
    param[4] = real initial perturbation
    param[5] = imaginary initial perturbation
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Kleinian(CPixel* self, TComplex* z, TComplex* q,
    double* param, bool juliaflag,
    TReal& u, TReal& v1, TReal& K, TReal& M,
    TComplex& t)
    {
    u = TReal(param[0]);
    v1 = TReal(param[1]);
    K = TReal(param[2]);
    M = TReal(param[3]);

    t.x = u;
    t.y = v1;

    if (!juliaflag)
	{
	z->x = q->x + TReal(param[4]);
	z->y = q->y + TReal(param[5]);
	}
    }

// ------------------------------------------------------------
// Template version of RunFunctions 
// ------------------------------------------------------------

/**************************************************************************
    Dispatch the common native ManpWIN fractal iterations.

    Handles the closely related Mandelbrot, Julia, Power, Burning Ship
    and Cubic families using the arithmetic type selected by the caller.

    Returns the bailout / completion state for the current iteration.
**************************************************************************/

template<typename TComplex, typename TReal, typename TDegree>
inline int FunctionsDispatch(CPixel* self, WORD type, TComplex* z, TComplex* q, TDegree* degree, TComplex& sqr, TReal& real_imag, TComplex& a, TComplex& a2, TComplex& aa3,
	TComplex& b, TComplex& v, TComplex& temp, double* param, BYTE* SpecialFlag, long* iteration, int &SPECIALINDEX, char subtype, int bailout_type, TReal rqlim, TComplex* oldZ)
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

	    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim, oldZ);

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

	    return DoBailout<TComplex>(bailout_type, z, rqlim, oldZ);
	    }

	case BURNINGSHIP:				// Burning Ship
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = fabs(z->x * z->y);
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim, oldZ));

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
	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim, oldZ));

	case CUBIC:					// Art Matrix Cubic
	    {
	    if (subtype == 'K')				// CKIN
		{
		*z = z->CCube() + b;
		z->x += param[4];
		z->y += param[5];
		}
	    else
		{
		temp = z->CCube() + b;
		*z = temp - aa3 * *z;
		z->x += param[4];
		z->y += param[5];
		}

	    if (z->CSumSqr() > 100.0)
		return TRUE;
	    else
		{
		if (subtype == 'F')
		    {
		    if (q->CSumSqr() < 0.111111)
			{
			*iteration = SPECIALINDEX;
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
		    *iteration = SPECIALINDEX;
		    *SpecialFlag = TRUE;
		    return TRUE;
		    }

		return FALSE;
		}
	    }
	}

    return 0;
    }

/**************************************************************************
    Iterate the Art Matrix Special Newton fractal.

    Apply the Newton recurrence and test the resulting point against
    each of the three convergence targets.

    phaseflag identifies which target was reached.
**************************************************************************/

template<typename TComplex>				// Art Matrix Newton
BOOL Iter_SPECIALNEWT(TComplex* z, const TComplex* q, TComplex& a, TComplex& b, TComplex& lm5, TComplex& lp5, TComplex& v, double* param, BYTE& phaseflag, WORD& special)
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

/**************************************************************************
    Iterate the Art Matrix Matein fractal.

    Update z and its inverse, then accumulate the orbit distance.

    phaseflag = 0 when the orbit converges below epsilon.
    phaseflag = 1 when the accumulated distance exceeds escape.
**************************************************************************/

template<typename TComplex>					// Art Matriuc Matein fractal
BOOL Iter_MATEIN(TComplex* z, TComplex* q, TComplex& oz, TComplex& temp, double& distance, double absolute, double* param, double& epsilon, double& escape, BYTE& phaseflag)
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

template<typename TComplex, typename TReal>					// Sine
bool Iter_SINFRACTAL(CPixel* self, TComplex* z, TComplex* q, double* param, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    if (param[2] == 0.0)
	*z = (*q) * z->CSin();
    else
	*z = (*q) + z->CSin();

    z->x += param[0];
    z->y += param[1];

    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim, oldZ));
    }

/**************************************************************************
    Iterate the exponential fractal.

    Apply the complex exponential repeatedly according to the selected
    degree, then either multiply by or add the complex parameter q.

    The selected subtype determines whether bailout is tested against
    the real component, imaginary component, or both components.
**************************************************************************/

template<typename TComplex>					// Exponential
BOOL Iter_EXP_FRACTAL(TComplex* z, TComplex* q, double* param, WORD* degree, int& subtype, double rqlim)
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

/**************************************************************************
    Iterate the Red Shift Rider fractal.

    z(n+1) = a*z(n)^2 +/- z(n)^degree + c

    param[5] selects the sign of the power term.
    Standard ManpWIN bailout processing is applied to the result.
**************************************************************************/

template<typename TComplex, typename TReal>
bool Iter_REDSHIFTRIDER(CPixel* self, TComplex* z, TComplex* q, TComplex& a, double* param, WORD* degree, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex term =
	((param[5] == 1.0) ? TComplex(1.0) : TComplex(-1.0))
	* z->CPolynomial(*degree);

    *z = a * (*z) * (*z) + term;
    *z = (*z) + (*q);

    return DoBailout<TComplex>(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Iterate the Talis power fractal.

		    z^N
	z' = ---------------- + c
	     M + z^(N-1)

    The previously calculated z^(N-1) is reused to form the numerator.
**************************************************************************/

template<typename TComplex, typename TReal>					// Talis Power    Z = Z^N/(M + Z^(N-1)) + C
bool Iter_TALIS(CPixel* self, TComplex* z, TComplex* q, TComplex& a, double* param, WORD* degree, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    double m = param[1];

    a = z->CPolynomial(*degree - 1);

    *z = (a * (*z)) / (TComplex(m) + a) + (*q);

    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim, oldZ));
    }

/**************************************************************************
    Iterate the polynomial fractal.

    Evaluate the polynomial defined by param[2...] at the current z,
    then add the complex parameter q.

    Terms whose coefficient is zero are skipped.
**************************************************************************/

template<typename TComplex, typename TReal>					// Polynomial
bool Iter_POLYNOMIAL(CPixel* self, TComplex* z, TComplex* q, double* param, int bailout_type, TReal rqlim, TComplex* oldZ)
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

    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim, oldZ));
    }

/**************************************************************************
    Iterate the Art Matrix Rational Map.

    Apply the rational-map recurrence, update the derivative estimate,
    and test for escape or convergence.

    Escaping points are assigned one of the positive or negative colour
    sequences according to their position relative to alpha.

    Converged points and points reaching the iteration limit are given
    their corresponding special values.
**************************************************************************/

template<typename TComplex, typename  TReal>					// Art Matrix Rational Map
bool Iter_RATIONALMAP(CPixel* self, TComplex* z, TComplex* q, TComplex& a, TComplex& b, double* param, TReal& der, TReal escape, TReal epsilon, long* iteration, long threshold, WORD special, TComplex& alpha, long* color)
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

/**************************************************************************
    Kleinian exponential helper.

    Standard arithmetic types use exp() directly.
**************************************************************************/

template <typename TReal>
inline void KleinianExp(TReal* out, TReal& x)
    {
    *out = exp(x);
    }

/**************************************************************************
    Kleinian exponential helper for BigDouble.
    Uses native MPFR exponential.
**************************************************************************/

inline void KleinianExp(BigDouble* out, BigDouble& x)
    {
    if (!out)
	return;

    out->MatchPrecision(x);

    mpfr_exp(out->x, x.x, MPFR_RNDN);
    }

/**************************************************************************
    Wrap a Kleinian real value into the interval [0, 2).

    This avoids fmod(), which is not available uniformly for all
    arithmetic types.
**************************************************************************/

template <typename TReal>
inline void KleinianMod2(TReal& value)
    {
    TReal zero = TReal(0.0);
    TReal two = TReal(2.0);

    while (value < zero)
	value += two;

    while (value >= two)
	value -= two;
    }

/**************************************************************************
    Apply one Kleinian transformation.

    Original implementation by Paul de Leeuw,
    based on work by hrkalona.

    The point is first reduced to the fundamental strip, then one of
    two Mobius transformations is selected according to its position
    relative to the boundary curve.
**************************************************************************/

template <typename TComplex, typename TReal>
inline TComplex KleinianFunctionT(TComplex* z, TReal& u, TReal& v1, TReal& K, TReal& M, TComplex& t)
    {
    TReal re = z->x;
    TReal im = z->y;

    TComplex i;
    i.x = TReal(0.0);
    i.y = TReal(1.0);

    TReal offset = v1 * im / u;

    TReal wrapped = re + offset + TReal(1.0);

    KleinianMod2(wrapped);

    re = wrapped - TReal(1.0) - offset;

    TReal f;

    if (re >= -v1 * TReal(0.5))
	f = TReal(1.0);
    else
	f = TReal(-1.0);

    TReal absValue = re + v1 * TReal(0.5);

    if (absValue < TReal(0.0))
	absValue = -absValue;

    TReal exponent = -M * absValue;
    TReal expValue;

    KleinianExp(&expValue, exponent);

    TReal line3_im = u * TReal(0.5) + f * K * u * (TReal(1.0) - expValue);

    z->x = re;
    z->y = im;

    if (im < line3_im)
	*z = (t * *z - i) / (*z * -i);
    else
	*z = i / (*z * i + t);

    return *z;
    }

/**************************************************************************
    Kleinian iteration.

    Bail out when the point leaves the vertical strip, or when an
    iterated point enters the 2-cycle test region.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Kleinian(CPixel* self, TComplex* z, TReal& u, TReal& v1, TReal& K, TReal& M, TComplex& t, long* iteration, TReal rqlim)
    {
    if (z->y < TReal(0.0) || z->y > u)
	return TRUE;

    TReal magnitude = z->CSumSqr();

    if (*iteration != 0 && magnitude < rqlim)
	return TRUE;

    *z = KleinianFunctionT<TComplex, TReal>(z, u, v1, K, M, t);

    return FALSE;
    }


