/*
    DDFRACTINTFUNCTIONS.CPP - a module for the per pixel calculations of fractals originally from Fractint Double-double version
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include <math.h>
#include "manp.h"
#include "fractype.h"
#include "Complex.h"
#include "pixel.h"
#include "Fract.h"
#include "BailoutTemplate.h"
#include "FractalMathsTemplate.h"
#include "FractintTrigTemplate.h"
#include "..\parser\TrigFn.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::DDInitFractintTrigFunctions(WORD type, DDComplex *z, DDComplex *q)
    {
    CTrigFn	TrigFn;

    switch (type)
	{
	case HYPERCMPLXFP:
	case HYPERCMPLXJFP:
	    tDD = TransformPointT<DDComplex, dd_real>(cDD, TransformType, f_radius, f_xcenter, f_ycenter);
	    DDInitFunctions(MANDELFP, z, q);
	    InitFractintTrigFunctionsT<DDComplex, dd_real>(type, z, q, juliaflag, cDD, qcDD, qciDD, qcjDD, qckDD, tDD, tempDD, temp1DD, temp2DD, temp4DD, sqrDD, Fn2Index, param);
	    break;

	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	case NUMFRACTAL:
	case FNPLUSFNPIXFP:				//  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
	case FNPLUSFNPIXLONG:
	case TRIGSQRFP:					// { z=pixel: z=trig(z*z), |z|<TEST }
	case TRIGSQR:					// to handle fractint par files
	case TRIGXTRIGFP:				// z = trig0(z)*trig1(z)
	case TRIGXTRIG:					// to handle fractint par files
	case ZXTRIGPLUSZFP:
	case ZXTRIGPLUSZ:				// to handle fractint par files
	case TRIGPLUSTRIGFP:				// z = trig0(z)*p1+trig1(z)*p2
	case TRIGPLUSTRIG:				// to handle fractint par files
	case SQRTRIGFP:					// SZSB(XYAXIS) { z=pixel, TEST=(p1+3): z=sin(z)*sin(z), |z|<TEST}
	case SQRTRIG:					// to handle fractint par files
	case SQR1OVERTRIGFP:				// z = sqr(1/trig(z))
	case SQR1OVERTRIG:				// to handle fractint par files
	case MANDELTRIGFP:
	case MANDELTRIG:
	case FPMANTRIGPLUSEXP:				// another Scientific American biomorph type: z(n+1) = e**z(n) + trig(z(n)) + C
	case LAMBDATRIGFP:
	case LAMBDATRIG:				// to handle fractint par file references
	case FPMANTRIGPLUSZSQRD:			// From Scientific American, July 1989 A Biomorph: z(n+1) = trig(z(n))+z(n)**2+C
	case FPJULTRIGPLUSZSQRD:			// to handle fractint par file references:
	case LMANTRIGPLUSZSQRD:				// to handle fractint par file references
	case LJULTRIGPLUSZSQRD:				// to handle fractint par file references
	case LJULFNFN:
	case FPJULFNFN:
	case FPMANFNFN:					// z = trig0(z)+p1 if mod(old) < p2.x and trig1(z)+p1 if mod(old) >= p2.x
	case LMANFNFN:
	case LMANLAMFNFN:
	case LLAMBDAFNFN:
	case FPLAMBDAFNFN:
	case FPMANLAMFNFN:				// z = trig0(z)*p1 if mod(old) < p2.x and trig1(z)*p1 if mod(old) >= p2.x
	    period_level = FALSE;			// no periodicity checking (get rid of bug PHD 2009-10-16)
	    tDD = TransformPointT<DDComplex, dd_real>(cDD, TransformType, f_radius, f_xcenter, f_ycenter);
	    InitFractintTrigFunctionsT<DDComplex, dd_real>(type, z, q, juliaflag, cDD, qcDD, qciDD, qcjDD, qckDD, tDD, tempDD, temp1DD, temp2DD, temp4DD, sqrDD, Fn2Index, param);
	    break;

//	case FORMULA:
//	case SCREENFORMULA:
//	    DDFormPerPixel(z, q);
//	    break;

	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::DDRunFractintTrigFunctions(WORD type, DDComplex *z, DDComplex *q, BYTE *SpecialFlag, long *iteration)
    {
    CTrigFn	TrigFn;
    size_t	SizeOfTrigFn = sizeof(CTrigFn);			// info only

    switch (type)
	{
	case HYPERCMPLXFP:
	case HYPERCMPLXJFP:
	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	case FNPLUSFNPIXFP:					//  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
	case FNPLUSFNPIXLONG:
	case TRIGSQRFP:						// { z=pixel: z=trig(z*z), |z|<TEST }
	case TRIGSQR:						// to handle fractint par files
	case TRIGXTRIGFP:					// z = trig0(z)*trig1(z)
	case TRIGXTRIG:						// to handle fractint par files
	case ZXTRIGPLUSZFP:
	case ZXTRIGPLUSZ:					// to handle fractint par files
	    {
	    dd_real rqlimDD = rqlim;

	    return FractintTrigIterT<DDComplex, dd_real>(type, z, q, tempDD, temp1DD, temp2DD, temp3DD, sqrDD, qcDD, qciDD, qcjDD, qckDD,
		Fn1Index, Fn2Index, rqlimDD, BailoutTestType, &DDOldZ);
	    }

	case TRIGPLUSTRIGFP:
	case TRIGPLUSTRIG:
	    {
	    // Fractint's Scott Taylor special case.
	    if (param[0] == 1.0 && param[1] == 0.0 && param[2] == 1.0 && param[3] == 0.0)
		{
		return ScottTrigPlusTrigFractalT<DDComplex, dd_real>(z, temp1DD, temp4DD, Fn1Index, Fn2Index, rqlim, BailoutTestType, &DDOldZ);
		}

	    return TrigPlusTrigFractalT<DDComplex, dd_real>(z, tempDD, temp1DD, temp2DD, Fn1Index, Fn2Index, rqlim, BailoutTestType, &DDOldZ);
	    }

	case MANDELTRIGFP:
	case LAMBDATRIG:
	case LAMBDATRIGFP:
	case MANDELTRIG:
	    return MandelTrigFractalT<DDComplex, dd_real>(z, temp1DD, temp3DD, tDD, Fn1Index, rqlim, BailoutComponentLimit);

	case FPMANTRIGPLUSEXP:
	    return ManTrigPlusExpFractalT<DDComplex, dd_real>(z, temp1DD, tDD, Fn1Index, rqlim, BailoutTestType, &DDOldZ);

	case FPMANTRIGPLUSZSQRD:
	case FPJULTRIGPLUSZSQRD:
	case LMANTRIGPLUSZSQRD:
	case LJULTRIGPLUSZSQRD:
	    return ManTrigPlusZSqrFractalT<DDComplex, dd_real>(z, temp1DD, sqrDD, tDD, Fn1Index, rqlim, BailoutTestType, &DDOldZ);

	case LJULFNFN:
	case FPJULFNFN:
	case FPMANFNFN:
	case LMANFNFN:
	    return FnFnPlusFractalT<DDComplex, dd_real>(z, tDD, temp2DD, Fn1Index, Fn2Index, rqlim, BailoutTestType, &DDOldZ);

	case LLAMBDAFNFN:
	case FPLAMBDAFNFN:
	case LMANLAMFNFN:
	case FPMANLAMFNFN:
	    return FnFnMulFractalT<DDComplex, dd_real>(z, tDD, temp2DD, Fn1Index, Fn2Index, rqlim, BailoutTestType, &DDOldZ);

	case SQRTRIGFP:
	case SQRTRIG:
	    return SqrTrigFractalT<DDComplex, dd_real>(z, temp1DD, Fn1Index, rqlim, BailoutTestType, &DDOldZ);

	case SQR1OVERTRIGFP:
	case SQR1OVERTRIG:
	    return Sqr1OverTrigFractalT<DDComplex, dd_real>(z, Fn1Index, rqlim, BailoutTestType, &DDOldZ);

	case NUMFRACTAL:
	    return NumFractalT<DDComplex, dd_real>(z, q, Fn1Index, rqlim, BailoutTestType, param, &DDOldZ);

//	case FORMULA:
//	case SCREENFORMULA:
//	    return(DDFormula(z, q));

	}
    return 0;
    }

