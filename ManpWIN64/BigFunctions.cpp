/*
    BIGFUNCTIONS.CPP a module for the per pixel calculations of Bignum fractals.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <math.h>
#include "manp.h"
#include "fractype.h"
#include "Complex.h"
#include "big.h"
#include "BigDouble.h"
#include "BigComplex.h"
#include "pixel.h"
#include "FunctionTemplate.h"
#include "BailoutTemplate.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::BigInitFunctions(WORD type, BigComplex *zBig, BigComplex *qBig)
    {
     switch (type)
	{
	case MANDELFP:					// Mandelbrot
	case MANDEL:					// to handle fractint par files
	case JULIA:					// to handle fractint par files
	case BURNINGSHIP:				// Burning Ship
	    Init_Basic<BigComplex>(zBig, qBig, param, juliaflag, sqrBig, realimagBig);
	    break;

	case BURNINGSHIPPOWER:				// Burning Ship to higher power
	case POWER:					// Power
	    Init_Power<BigComplex>(type, zBig, qBig, param, juliaflag, degree);
	    break;

	case CUBIC:					// Art Matrix Cubic
	    Init_Cubic<BigComplex>(zBig, qBig, param, juliaflag, subtype, special, aBig, bBig, a2Big, aa3Big, t2Big, t3Big, tempBig, vBig);
	    break;

	case SPECIALNEWT:				// Art Matrix Newton
	    Init_SPECIALNEWT<BigComplex>(zBig, qBig, aBig, bBig, lm5Big, lp5Big);
	    break;
	    
	case MATEIN:					// Art Matriuc Matein fractal
	    {
	    period_level = FALSE;			// no periodicity checking
	    int ret = Init_MATEIN<BigComplex, BigDouble>(zBig, qBig, param, juliaflag, ozBig, tempBig, distance, absolute);
	    if (ret < 0)
		return ret;   // propagate rejection
	    break;
	    }

	case EXPFRACTAL:				// there's no initialisation for exp()
	    break;

	case SINFRACTAL:				// Sine
	    Init_SINFRACTAL<BigComplex>(zBig, qBig, param, juliaflag);
	    break;

	case REDSHIFTRIDER:				// RedShiftRider    a*z^2 +/- z^n + c
	    Init_REDSHIFTRIDER<BigComplex>(zBig, qBig, param, juliaflag, aBig, degree);
	    break;

	case TALIS:					// Talis Power    Z = Z^N/(M + Z^(N-1)) + C
	    Init_TALIS<BigComplex>(zBig, qBig, param, juliaflag, degree);
	    break;

	case POLYNOMIAL:
	    Init_POLYNOMIAL<BigComplex>(zBig, qBig, param, juliaflag);
	    for (int i = 0; i < MAXPOLYDEG; i++)			// find highest order of polynomial for use with fwd diff calcs
		{
		if (param[2 + i] != 0.0)
		    {
		    fractalspecific[type].SlopeDegree = MAXPOLYDEG - i;
		    break;
		    }
		}
	    break;

	case RATIONALMAP:				// Art Matrix Rational Map 
	    {
	    // -----------------------------------
	    // 1. CONTROL LOGIC (CPixel responsibility)
	    // -----------------------------------

	    switch ((int)param[0])
		{
		case 0: subtype = 'A'; break;
		case 1: subtype = 'B'; break;
		default: subtype = 'A'; break;
		}
	    special = ((int)param[1] < 0) ? 0 : (int)param[1];

	    // threshold / palette logic stays here
	    if (threshold != OldThreshold)
		{
		OldThreshold = threshold;
		int gap = threshold / 16;			// split the colour map into 16 equal parts
		for (int i = 0; i < 4; i++)
		    {
		    penp[i] = penpref[i] * gap;
		    penn[i] = pennref[i] * gap;
		    }
		}

	    // -----------------------------------
	    // 2. PURE MATH (TEMPLATE CALL)
	    // -----------------------------------

	    int ret = Init_RATIONALMAP<BigComplex, BigDouble>(zBig, qBig, param, juliaflag, subtype, aBig, bBig, alphaBig, tempBig, temp1Big, temp3Big);

	    if (ret < 0)
		return ret;

	    // -----------------------------------
	    // 3. POST-MATH STATE (CPixel responsibility)
	    // -----------------------------------

	    // escape + epsilon
	    if (alphaBig.x != 0.0 || alphaBig.y != 0.0)
		escape = 16.0 / alphaBig.CSumSqr();
	    else
		return(FALSE);				// no naughty division
	    epsilonBig = 0.000001 / escapeBig;		// EPSILN = 0.000001/ESCAPE 

	    derBig = 1.0;					// DER = 1.0 
	    }
	    break;

	case MANDELDERIVATIVES:				// a group of Mandelbrot Derivatives
	    BigInitManDerFunctions(subtype, zBig, qBig);
	    break;

	case TIERAZON:					// a group of Mandelbrot Derivatives
	    BigInitTierazonFunctions(subtype, zBig, qBig);
	    break;
	case NEWTONAPPLE:				// a specific Tierazon fractal
	    BigInitTierazonFunctions(55, zBig, qBig);
	    break;
	case NEWTONFLOWER:				// a specific Tierazon fractal
	    BigInitTierazonFunctions(35, zBig, qBig);
	    break;
	case NEWTONMSET:				// a specific Tierazon fractal
	    BigInitTierazonFunctions(52, zBig, qBig);
	    break;
	case NEWTONPOLYGON:				// a specific Tierazon fractal
	    BigInitTierazonFunctions(31, zBig, qBig);
	    break;
	case NEWTONCROSS:				// a specific Tierazon fractal
	    BigInitTierazonFunctions(59, zBig, qBig);
	    break;
	case NEWTONJULIANOVA:				// a specific Tierazon fractal
	    BigInitTierazonFunctions(2, zBig, qBig);
	    break;
	case NEWTONVARIATION:				// a specific Tierazon fractal
	    BigInitTierazonFunctions(87, zBig, qBig);
	    break;
	case QUARTET1:					// a specific Tierazon fractal
	    BigInitTierazonFunctions(83, zBig, qBig);
	    break;
	case QUARTET2:					// a specific Tierazon fractal
	    BigInitTierazonFunctions(85, zBig, qBig);
	    break;
	case QUARTET3:					// a specific Tierazon fractal
	    BigInitTierazonFunctions(96, zBig, qBig);
	    break;
	case NOVA:
	    BigInitTierazonFunctions(2, zBig, qBig);
	    break;
	case QUAD:					// a specific Tierazon fractal
	    BigInitTierazonFunctions(90, zBig, qBig);
	    break;
	case RAMONSIN:					// a specific Tierazon fractal
	    BigInitTierazonFunctions(116, zBig, qBig);
	    break;
	case RAMONCOS:					// a specific Tierazon fractal
	    BigInitTierazonFunctions(117, zBig, qBig);
	    break;
	case FORMULA05:					// a specific Tierazon fractal
	    BigInitTierazonFunctions(5, zBig, qBig);
	    break;
	case TEDDY:					// a specific Tierazon fractal
	    BigInitTierazonFunctions(104, zBig, qBig);
	    break;
	}
    return 0;
    }


/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::BigRunFunctions(WORD type, BigComplex *zBig, BigComplex *qBig, BYTE *SpecialFlag, long *iteration)
    {
    switch (type)
	{
	case MANDELFP:					// Mandelbrot
	case MANDEL:					// to handle fractint par files
	case JULIA:					// to handle fractint par files
	case JULIAFP:					// like he said
	    {
	    BigDouble	t, realimagBig;
	    BigComplex	sqrBig;

	    // use direct function calls for Mandelbrot for speed. Approx 10 - 15 % faster.
	    mpfr_mul(realimagBig.x, zBig->x.x, zBig->y.x, MPFR_RNDN);
	    mpfr_mul(sqrBig.x.x, zBig->x.x, zBig->x.x, MPFR_RNDN);
	    mpfr_mul(sqrBig.y.x, zBig->y.x, zBig->y.x, MPFR_RNDN);
	    mpfr_sub(t.x, sqrBig.x.x, sqrBig.y.x, MPFR_RNDN);
	    mpfr_add(zBig->x.x, qBig->x.x, t.x, MPFR_RNDN);
	    mpfr_add(t.x, realimagBig.x, qBig->y.x, MPFR_RNDN);
	    mpfr_add(zBig->y.x, realimagBig.x, t.x, MPFR_RNDN);
	    return BailoutCore(BailoutTestType, zBig, &sqrBig, rqlim);
	    }

	case POWER:					// Power
	case JULIA4FP:
	case JULIA4:
	case MANDEL4FP:
	case MANDEL4:
	case BURNINGSHIP:				// Burning Ship
	case BURNINGSHIPPOWER:				// Burning Ship to higher power
	case CUBIC:					// Art Matrix Cubic
	    return FunctionsDispatch<BigComplex, BigDouble, WORD>(this, type, zBig, qBig, degree, sqrBig, realimagBig,
		aBig, a2Big, aa3Big, bBig, vBig, tempBig, param, SpecialFlag, iteration, special, subtype, BailoutTestType, rqlim);

	case SPECIALNEWT:				// Art Matrix Newton
	    return Iter_SPECIALNEWT<BigComplex>(zBig, qBig, aBig, bBig, lm5Big, lp5Big, vBig, param, phaseflag, special);

	case MATEIN:					// Art Matriuc Matein fractal
	    return Iter_MATEIN<BigComplex>(zBig, qBig, ozBig, tempBig, distance, absolute, param, epsilon, escape, phaseflag);
	
	case SINFRACTAL:				// Sine
	    return Iter_SINFRACTAL<BigComplex>(this, zBig, qBig, param, BailoutTestType, rqlim);

	case EXPFRACTAL:				// Exponential
	    return Iter_EXP_FRACTAL<BigComplex>(zBig, qBig, param, degree, subtype, rqlim);

	case REDSHIFTRIDER:				// RedShiftRider    a*z^2 +/- z^n + c
	    return Iter_REDSHIFTRIDER<BigComplex, BigDouble>(this, zBig, qBig, aBig, param, degree, BailoutTestType, rqlim);

	case TALIS:					// Talis Power    Z = Z^N/(M + Z^(N-1)) + C
	    return Iter_TALIS<BigComplex>(this, zBig, qBig, aBig, param, degree, BailoutTestType, rqlim);

	case POLYNOMIAL:				// Polynomial
	    return Iter_POLYNOMIAL<BigComplex>(this, zBig, qBig, param, BailoutTestType, rqlim);

	case RATIONALMAP:				// Art Matrix Rational Map
	    return Iter_RATIONALMAP<BigComplex, BigDouble>(this, zBig, qBig, aBig, bBig, param, derBig, escapeBig, epsilonBig,
		iteration, threshold, special, alphaBig, &color);

	case MANDELDERIVATIVES:				// a group of Mandelbrot Derivatives
	    return (BigRunManDerFunctions(subtype, zBig, qBig, SpecialFlag, iteration));

	case NEWTONAPPLE:				// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(55, zBig, qBig, SpecialFlag, iteration));

	case NEWTONFLOWER:				// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(35, zBig, qBig, SpecialFlag, iteration));

	case NEWTONMSET:				// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(52, zBig, qBig, SpecialFlag, iteration));

	case NEWTONPOLYGON:				// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(31, zBig, qBig, SpecialFlag, iteration));

	case NEWTONCROSS:				// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(59, zBig, qBig, SpecialFlag, iteration));

	case NEWTONJULIANOVA:				// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(2, zBig, qBig, SpecialFlag, iteration));

	case NEWTONVARIATION:				// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(87, zBig, qBig, SpecialFlag, iteration));

	case QUARTET1:					// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(83, zBig, qBig, SpecialFlag, iteration));

	case QUARTET2:					// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(85, zBig, qBig, SpecialFlag, iteration));

	case QUARTET3:					// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(96, zBig, qBig, SpecialFlag, iteration));

	case NOVA:
	    return (BigRunTierazonFunctions(2, zBig, qBig, SpecialFlag, iteration));

	case QUAD:					// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(90, zBig, qBig, SpecialFlag, iteration));

	case RAMONSIN:					// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(116, zBig, qBig, SpecialFlag, iteration));

	case RAMONCOS:					// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(117, zBig, qBig, SpecialFlag, iteration));

	case FORMULA05:					// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(5, zBig, qBig, SpecialFlag, iteration));

	case TEDDY:					// a specific Tierazon fractal
	    return (BigRunTierazonFunctions(104, zBig, qBig, SpecialFlag, iteration));

	case TIERAZON:					// a group of Tierazon Fractals
	    return (BigRunTierazonFunctions(subtype, zBig, qBig, SpecialFlag, iteration));
	}
    return 0;
    }

