/*
    QDFRACTINTFUNCTIONS.CPP a module for the per pixel calculations of fractals originally from Fractint Quad-double version
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <math.h>
#include "manp.h"
#include "fractype.h"
#include "Complex.h"
#include "pixel.h"
#include "Fract.h"
#include "BailoutTemplate.h"
#include "FractintTrigTemplate.h"
#include "..\parser\TrigFn.h"

// lotsa norty externs that will be resolved when we c++ise parser
//extern	    int QDFormPerPixel(QDComplex *zIn, QDComplex *qIn);	// norty declarations because this is in the formula parser
//extern	    int QDFormula(QDComplex *zIn, QDComplex *qIn);
// end norty externs that will be resolved when we c++ise parser


/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::QDInitFractintTrigFunctions(WORD type, QDComplex *z, QDComplex *q)
    {
    CTrigFn	TrigFn;

    switch (type)
	{
	case HYPERCMPLXFP:
	case HYPERCMPLXJFP:
	    tQD = (invert) ? QDInvertz2(cQD) : cQD;
	    QDInitFunctions(MANDELFP, z, q);
	    InitFractintTrigFunctionsT<QDComplex, qd_real>(type, z, q, juliaflag, invert, cQD, qcQD, qciQD, qcjQD, qckQD, tQD, tempQD, temp1QD, temp2QD, temp4QD, sqrQD, Fn2Index, param);
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
	    tQD = (invert) ? QDInvertz2(cQD) : cQD;
	    InitFractintTrigFunctionsT<QDComplex, qd_real>(type, z, q, juliaflag, invert, cQD, qcQD, qciQD, qcjQD, qckQD, tQD, tempQD, temp1QD, temp2QD, temp4QD, sqrQD, Fn2Index, param);
	    break;

//	case FORMULA:
//	case SCREENFORMULA:
//	    QDFormPerPixel(z, q);
//	    break;

	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::QDRunFractintTrigFunctions(WORD type, QDComplex *z, QDComplex *q, BYTE *SpecialFlag, long *iteration)
    {
    CTrigFn	TrigFn;
//    size_t	SizeOfTrigFn = sizeof(CTrigFn);			// info only

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
	qd_real rqlimQD = rqlim;

	return FractintTrigIterT<QDComplex, qd_real>(type, z, q, tempQD, temp1QD, temp2QD, temp3QD, sqrQD, qcQD, qciQD, qcjQD, qckQD,
	    Fn1Index, Fn2Index, rqlimQD, BailoutTestType);
	}

	case TRIGPLUSTRIGFP:
	case TRIGPLUSTRIG:
	    return TrigPlusTrigFractalT<QDComplex, qd_real>(z, temp1QD, temp2QD, Fn1Index, Fn2Index, rqlim, BailoutTestType);

	case MANDELTRIGFP:
	case LAMBDATRIG:
	case LAMBDATRIGFP:
	case MANDELTRIG:
	    return MandelTrigFractalT<QDComplex, qd_real>(z, temp1QD, temp3QD, tQD, Fn1Index, rqlim);

	case FPMANTRIGPLUSEXP:
	    return ManTrigPlusExpFractalT<QDComplex, qd_real>(z, temp1QD, tQD, Fn1Index, rqlim, BailoutTestType);

	case FPMANTRIGPLUSZSQRD:
	case FPJULTRIGPLUSZSQRD:
	case LMANTRIGPLUSZSQRD:
	case LJULTRIGPLUSZSQRD:
	    return ManTrigPlusZSqrFractalT<QDComplex, qd_real>(z, temp1QD, sqrQD, tQD, Fn1Index, rqlim, BailoutTestType);

	case LJULFNFN:
	case FPJULFNFN:
	case FPMANFNFN:
	case LMANFNFN:
	    return FnFnPlusFractalT<QDComplex, qd_real>(z, tQD, temp2QD, Fn1Index, Fn2Index, rqlim, BailoutTestType);

	case LLAMBDAFNFN:
	case FPLAMBDAFNFN:
	case LMANLAMFNFN:
	case FPMANLAMFNFN:
	    return FnFnMulFractalT<QDComplex, qd_real>(z, tQD, temp2QD, Fn1Index, Fn2Index, rqlim, BailoutTestType);

	case SQRTRIGFP:
	case SQRTRIG:
	    return SqrTrigFractalT<QDComplex, qd_real>(z, temp1QD, Fn1Index, rqlim, BailoutTestType);

	case SQR1OVERTRIGFP:
	case SQR1OVERTRIG:
	    return Sqr1OverTrigFractalT<QDComplex, qd_real>(z, Fn1Index, rqlim, BailoutTestType);

	case NUMFRACTAL:
	    return NumFractalT<QDComplex, qd_real>(z, q, Fn1Index, rqlim, BailoutTestType, param);




//	case FORMULA:
//	case SCREENFORMULA:
//	    return(QDFormula(z, q));
	}
    return 0;
    }


