#include <windows.h>
#include <functional>
#include <cmath>

#pragma once

template<typename TComplex, typename TReal>
inline void MandelDerivInit(
    int subtype,
    bool juliaflag,
    TComplex* z,
    TComplex* q,
    TComplex& sqr,
    TReal& real_imag,
    WORD* degree,
    double* param
)
    {
    switch (subtype)
	{
	// =========================
	// GROUP 1 — standard init
	// =========================
	default:
	    {
	    if (!juliaflag)
		{
		z->x = q->x + param[0];
		z->y = q->y + param[1];
		}

	    sqr = TComplex(0.0);     // important: type-safe
	    real_imag = TReal(0.0);

	    break;
	    }

	// =========================
	// GROUP 2 — degree-based
	// =========================
	case 2:				// Burning Ship of Higher Degree
	case 4:				// Buffalo
	case 6:				// Mandelbar (Tricorn)
	case 7:				// Celtic
	case 55:			// SimonBrot
	case 56:			// SimonBrot2
	    {
	    *degree = (int)param[0];
	    if (*degree < 1)
		*degree = 1;

	    if (!juliaflag)
		{
		z->x = q->x + param[1];
		z->y = q->y + param[2];
		}

	    break;
	    }
	}
    }

// ---------------------------------------------------------------------
// Mandelbrot Derivative Template Dispatcher (Tierazon style)
// ---------------------------------------------------------------------
template<typename TComplex, typename TReal>
inline int MandelDerivDispatch(
    CPixel* self,
    int subtype,
    TComplex* z,
    TComplex* q,
    WORD* degree,
    TComplex& sqr,
    TReal& real_imag,
    int bailout_type,
    TReal rqlim)
    {
    TComplex sqrsqr;
    TReal RealImagSqr;
    switch (subtype)
	{
/**************************************************************************
    Perpendicular Mandelbrot
    The equation is very similar to burning ship except you absolute just
    the real factor when calculating the imaginary side.
    zi = abs(zr) * zi * -2.0 + JuliaI
    zr = zrsqr - zisqr + JuliaR

    Whereas burning ship is ...
    zi = abs(zr * zi) * 2.0 - JuliaI
    zr = zrsqr - zisqr - JuliaR
    Real -0.737,424,082,465,620,824,452,347,915,7­36,817,521,648,421,984,117,126,135,371,4
    Imaginary -0.355,631,230,436,806,548,631,431,830,9­06,449,574,310,522,006,013,120,497,532,0
    Zooms 182 magnification 6.13e54
***************************************************************************/

	case 0:						// Perpendicular Mandelbrot
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    real_imag = fabs(z->x) * z->y;

	    z->x = sqr.x - sqr.y + q->x;
	    z->y = -real_imag - real_imag + q->y;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 1:						// Burning Ship
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    real_imag = fabs(z->x * z->y);

	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 2:						// Burning Ship of Higher Degree
	    {
	    z->x = fabs(z->x);
	    z->y = -fabs(z->y);

	    *z = z->CPolynomial(*degree);
	    *z = *z + *q;

	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
	    }

/**************************************************************************
    Perpendicular BurningShip
    The equation is very similar to burning ship except you absolute just 
    the imaginary factor instead of the real factor when calculating the imaginary side.
    zi = zr * abs(zi) * -2.0 + JuliaI
    zr = zrsqr - zisqr + JuliaR

    Whereas burning ship is ...
    zi = abs(zr * zi) * 2.0 - JuliaI
    zr = zrsqr - zisqr - JuliaR
    Real -0.737,424,082,465,620,824,452,347,915,7­36,817,521,648,421,984,117,126,135,371,4
    Imaginary -0.355,631,230,436,806,548,631,431,830,9­06,449,574,310,522,006,013,120,497,532,0
    Zooms 182 magnification 6.13e54
***************************************************************************/
	case 3:						// Perpendicular Burning Ship
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    real_imag = z->x * fabs(z->y);

	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag + q->y;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 4:						// Buffalo (works according to Kalles Fraktaller)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    if (*degree == 2)
		{
		z->y = fabs(z->x) * fabs(z->y) * -2.0 - q->y;
		z->x = fabs(sqr.x - sqr.y) + q->x;
		}
	    else if (*degree == 3)
		{
		z->y = fabs(((sqr.x * 3.0) - sqr.y) * z->y) - q->y;
		z->x = fabs((sqr.x - (sqr.y * 3.0)) * z->x) + q->x;
		}
	    else // degree > 3
		{
		*z = z->CPolynomial(*degree);
		z->x = fabs(z->x) + q->x;
		z->y = fabs(z->y) - q->y;
		}

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 5:						// Perpendicular Buffalo - (according to Kalles Fraktaller)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = z->x * fabs(z->y) * -2.0 - q->y;
	    z->x = fabs(sqr.x - sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 6:						// Mandelbar (Tricorn)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    if (*degree == 2)
		{
		real_imag = z->x * z->y;
		z->x = sqr.x - sqr.y + q->x;
		z->y = -real_imag - real_imag + q->y;
		}
	    else
		{
		*z = z->CPolynomial(*degree);
		z->x = (param[3] == 1.0 ? -z->x : z->x) + q->x;
		z->y = (param[3] == 1.0 ? z->y : -z->y) + q->y;
		}

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 7:						// Celtic
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    if (*degree == 2)
		{
		z->y = z->x * z->y * -2.0 - q->y;
		z->x = -fabs(sqr.x - sqr.y) - q->x;
		}
	    else // degree > 2
		{
		*z = z->CPolynomial(*degree);
		z->x = fabs(z->x);
		*z = *z + *q;
		}

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 8:						// Mandelbar Celtic
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = z->x * z->y * -2.0 + q->y;
	    z->x = fabs(sqr.x - sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 9:						// Perpendicular Celtic
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = fabs(z->x) * z->y * -2.0 + q->y;
	    z->x = fabs(sqr.x - sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 10:					// Cubic Flying Squirrel (Buffalo Imaginary)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = fabs(((sqr.x * 3.0) - sqr.y) * z->y) - q->y;
	    z->x = ((sqr.x - (sqr.y * 3.0)) * z->x) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 11:					// Heart Mandelbrot
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = fabs(z->x) * z->y * 2.0 - q->y;
	    z->x = sqr.x - sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 12:					// Celtic Heart
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = fabs(z->x) * z->y * 2.0 - q->y;
	    z->x = fabs(sqr.x - sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 13:					// Partial Cubic Burning Ship Real
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = ((sqr.x * 3.0) - sqr.y) * z->y + q->y;
	    z->x = (sqr.x - (sqr.y * 3.0)) * fabs(z->x) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 14:					// Partial Cubic Burning Ship Imaginary
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = ((sqr.x * 3.0) - sqr.y) * fabs(z->y) + q->y;
	    z->x = (sqr.x - (sqr.y * 3.0)) * z->x + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 15:					// Partial Cubic Buffalo Real (Celtic)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = ((sqr.x * 3.0) - sqr.y) * z->y + q->y;
	    z->x = fabs((sqr.x - (sqr.y * 3.0)) * z->x) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 16:					// Cubic Quasi Burning Ship (Buffalo Burning Ship Hybrid)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -fabs(((sqr.x * 3.0) - sqr.y) * z->y) + q->y;
	    z->x = (sqr.x - (sqr.y * 3.0)) * fabs(z->x) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 17:					// Cubic Quasi Perpendicular
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -fabs((sqr.x * 3.0) - sqr.y) * z->y + q->y;
	    z->x = (sqr.x - (sqr.y * 3.0)) * fabs(z->x) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 18:					// Cubic Quasi Heart
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = fabs((sqr.x * 3.0) - sqr.y) * z->y + q->y;
	    z->x = (sqr.x - (sqr.y * 3.0)) * fabs(z->x) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

    /****************************************************************
	4th Order Fractals:
    ****************************************************************/

    /****************************************************************
	Non ABS Variations (2)
    ****************************************************************/

	case 19:					// Mandelbrot 4th Order
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * z->x * z->y * (sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 20:					// Mandelbar 4th Order
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -4.0 * z->x * z->y * (sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

    /****************************************************************
	***Straight ABS Variations (16)
    ****************************************************************/

	case 21:					// Burning Ship 4th Order
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * fabs(z->x * z->y) * (sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 22:					// Burning Ship 4th Partial Imag
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * z->x * fabs(z->y) * (sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 23:					// Burning Ship 4th Partial Real
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * fabs(z->x) * z->y * (sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 24:					// Burning Ship 4th Partial Real Mbar
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -4.0 * fabs(z->x) * z->y * (sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 25:					// Celtic Burning Ship 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * fabs(z->x * z->y) * (sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 26:					// Celtic Burning Ship 4th Partial Imag
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * z->x * fabs(z->y) * (sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 27:					// Celtic Burning Ship 4th Partial Real
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * fabs(z->x) * z->y * (sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 28:					// Celtic Burning Ship 4th Partial Real Mbar
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -4.0 * fabs(z->x) * z->y * (sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 29:					// Buffalo 4th Order
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * fabs(z->x * z->y * (sqr.x - sqr.y)) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 30:					// Buffalo 4th Partial Imag
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * fabs(z->x * z->y * (sqr.x - sqr.y)) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 31:					// Celtic (Buffalo 4th Partial Real)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * z->x * z->y * (sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 32:					// Celtic 4th Mbar
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -4.0 * z->x * z->y * (sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

   /**************************************************************************
	Quasi ABS Variations (10)
    ***************************************************************************/
	case 33:					// False Quasi Perpendicular 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -4.0 * z->x * z->y * fabs(sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 34:					// False Quasi Heart 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * z->x * z->y * fabs(sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 35:					// Celtic False Quasi Perpendicular 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -4.0 * z->x * z->y * fabs(sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 36:					// Celtic False Quasi Heart 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * z->x * z->y * fabs(sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 37:					// Imag Quasi Perpendicular / Heart 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * z->x * fabs(z->y) * fabs(sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 38:					// Real Quasi Perpendicular 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -4.0 * fabs(z->x) * z->y * fabs(sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 39:					// Real Quasi Heart 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * fabs(z->x) * z->y * fabs(sqr.x - sqr.y) + q->y;
	    z->x = sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 40:					// Celtic Imag Quasi Perpendicular / Heart 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * z->x * fabs(z->y) * fabs(sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 41:					// Celtic Real Quasi Perpendicular 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = -4.0 * fabs(z->x) * z->y * fabs(sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 42:					// Celtic Real Quasi Heart 4th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->y = 4.0 * fabs(z->x) * z->y * fabs(sqr.x - sqr.y) + q->y;
	    z->x = fabs(sqr.x * sqr.x + sqr.y * sqr.y - 6.0 * sqr.x * sqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

   /****************************************************************
	5th Order Fractals:
    ****************************************************************/
	case 43:					// Mandelbrot 5th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;

	    z->y = z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = z->x * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 44:					// Mandelbar 5th (Vertical)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;

	    z->y = -z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = z->x * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 45:					// Mandelbar 5th (horizontal)
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;

	    z->y = z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = -z->x * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 46:					// Burning Ship 5th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;

	    z->y = fabs(z->y) * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = fabs(z->x) * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 47:					// Buffalo 5th
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;

	    z->y = fabs(z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y)) + q->y;
	    z->x = fabs(z->x * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y)) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 48:					// Burning Ship 5th Partial
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;

	    z->y = z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = fabs(z->x) * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;

	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));
	    }

	case 49:					// Burning Ship 5th Partial Mbar
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;
	    z->y = -z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = fabs(z->x) * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;
	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));

	case 50:					// Celtic 5th (Buffalo 5th Partial)
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;
	    z->y = z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = fabs(z->x * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y)) + q->x;
	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));

	case 51:					// Celtic 5th Mbar
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;
	    z->y = -z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = fabs(z->x * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y)) + q->x;
	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));

	case 52:					// Quazi Burning Ship 5th (BS/Buffalo Hybrid)
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;
	    z->y = -fabs(z->y * (5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y)) + q->y;
	    z->x = fabs(z->x) * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;
	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));

	case 53:					// Quazi Perpendicular 5th
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;
	    z->y = -z->y * fabs(5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = fabs(z->x) * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;
	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));

	case 54:					// Quazi Heart 5th
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    sqrsqr.x = sqr.x * sqr.x;
	    sqrsqr.y = sqr.y * sqr.y;
	    RealImagSqr = sqr.x * sqr.y;
	    z->y = z->y * fabs(5.0 * sqrsqr.x - 10.0 * RealImagSqr + sqrsqr.y) + q->y;
	    z->x = fabs(z->x) * (sqrsqr.x - 10.0 * RealImagSqr + 5.0 * sqrsqr.y) + q->x;
	    return (BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim));

	case 55:					// SimonBrot
/**************************************************************************
	Run SimonBrot type fractals
	z^n * |z|^2 + c	    (normal)

	It would also be nice to see Simonbrot 3rd, 5th, and 7th, but the formulas for these fractals are
	z1.5 * |z|2 + pixel, z2.5 * |z|2 + pixel, and z3.5 * |z|2 + pixel. Due to the fact that the powers of
	some of the terms are fractions, this could be difficult to implement. In a roundabout way, there is
	a power zero Simonbrot on Kalles Fraktaler, because the power zero Simonbrot is actually the Burning Ship.
**************************************************************************/
	    {
	    TComplex	zabs, tempz, sqrtz;

	    zabs.x = fabs(z->x);
	    zabs.y = fabs(z->y);
	    tempz.y = z->y * zabs.x + z->x * zabs.y;
	    tempz.x = z->x * zabs.x - z->y * zabs.y;
	    sqrtz = (*degree % 2 == 1) ? z->CSqrt() : TComplex(1.0);		// use square root power if degree is odd
	    *z = tempz.CPolynomial(*degree / 2) * sqrtz + *q;
	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
	    }

	case 56:					// SimonBrot2
/**************************************************************************
	Run SimonBrot2 type fractals
	z^n * |z^2| + c	    (SimonBrot2)
**************************************************************************/
	    {
	    TComplex	zabs, tempz, sqrtz;

	    tempz = *z * *z;
	    zabs.x = fabs(tempz.x);
	    zabs.y = -fabs(tempz.y);
	    tempz = zabs;
	    sqrtz = (*degree % 2 == 1) ? z->CSqrt() : TComplex(1.0);		// use square root power if degree is odd
	    *z = z->CPolynomial(*degree / 2) * sqrtz * tempz + *q;
	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
	    }

	case 57:					// Kung Fu Panda
/**************************************************************************
	Kung Fu Panda type fractals
	z = abs(z*z)
	z = z * z + p
**************************************************************************/
	    {
	    TComplex	t1, t2;

	    t1 = *z * *z;
	    t2.x = fabs(t1.x);
	    t2.y = fabs(t1.y);
	    *z = t2 * t2 - *q;
	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
	    }

	case 58:					// HPDZ Buffalo
/**************************************************************************
	HPDZ Buffalo type fractals
	z := (((x^2 - y^2) - |x|) + i (|2xy| - |y|)) + c
	or
	w := |x| + i |y|
	z := w^2 - w + c
**************************************************************************/
	    {
	    TComplex w;

	    w.x = fabs(z->x);
	    w.y = fabs(z->y);
	    *z = w * w - w;
	    z->x += q->x;
	    z->y -= q->y;
	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
	    }

	case 59:					// SzegediButterfly 1
/**************************************************************************
	Szegedi Butterfly
	double temp = complex[0].getIm();
	double temp2 = complex[0].getRe();
	complex[0] = new Complex(temp * temp - Math.sqrt(complex[0].getAbsRe()), temp2 * temp2 - Math.sqrt(complex[0].getAbsIm())).plus_mutable(complex[1]);
	complex[0] = new Complex(temp2 * temp2 - Math.sqrt(complex[0].getAbsIm()), temp * temp - Math.sqrt(complex[0].getAbsRe())).plus_mutable(complex[1]);
**************************************************************************/
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    TReal tx = sqrt(fabs(z->x));
	    TReal ty = sqrt(fabs(z->y));

	    z->x = sqr.y - tx;
	    z->y = sqr.x - ty;
	    *z += *q;

	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
	    }

	case 60:					// SzegediButterfly 2
	    {
	    TReal temp;

	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    TReal tx = sqrt(fabs(z->y));
	    TReal ty = sqrt(fabs(z->x));

	    temp = sqr.x - tx;
	    z->y = sqr.y - ty;
	    z->x = temp;

	    *z += *q;

	    return (BailoutCore<TComplex>(bailout_type, z, nullptr, rqlim));
	    }


	default:
	    return 0;
	}
    }




