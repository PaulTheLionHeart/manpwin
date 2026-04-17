/*
    FRACTINTFUNCTIONS.CPP a module for the per pixel calculations of fractals originally from Fractint 
    
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
//extern	int	FormPerPixelFloat(Complex *z, Complex *q);
//extern	int	FormulaFloat(Complex *z, Complex *q);
// end norty externs that will be resolved when we c++ise parser


/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::InitFractintTrigFunctions(WORD type, Complex *z, Complex *q)
    {
    CTrigFn	TrigFn;

    switch (type)
	{
	case HYPERCMPLXFP:
	case HYPERCMPLXJFP:
	    t = (invert) ? invertz2(c) : c;
	    InitFunctions(MANDELFP, z, q);
	    InitFractintTrigFunctionsT<Complex, double>(type, z, q, juliaflag, invert, c, qc, qci, qcj, qck, t, temp, temp1, temp2, temp4, sqr, Fn2Index, param);
	    break;
/*


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
*/
/*
	    t = (invert) ? invertz2(c) : c;
	    period_level = FALSE;			// no periodicity checking (get rid of bug PHD 2009-10-16)
	    InitFractintTrigFunctionsT<Complex>(type, z, q, juliaflag, invert, c, qc, qci, qcj, qck, t, temp, temp1, temp2, temp4, sqr, Fn2Index, param);
	    break;
*/
/*
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
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
	    sqr.x = sqr(z->x);
	    sqr.y = sqr(z->y);
	    temp1 = *z;					// set temp1 to Y value
	    temp1.x -= 1;
//	    CMPLXpwr(z, temp1, temp1);
	    temp1 = *z ^ temp1;
	    break;
*/

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
	    t = (invert) ? invertz2(c) : c;
	    InitFractintTrigFunctionsT<Complex, double>(type, z, q, juliaflag, invert, c, qc, qci, qcj, qck, t, temp, temp1, temp2, temp4, sqr, Fn2Index, param);
	    break;

//	case FORMULA:
//	case SCREENFORMULA:
//	    FormPerPixelFloat(z, q);
//	    break;

	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::RunFractintTrigFunctions(WORD type, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration)
    {
    CTrigFn	TrigFn;
    size_t	SizeOfTrigFn = sizeof(CTrigFn);		// info only

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
	    return FractintTrigIterT<Complex, double>(type, z, q, temp, temp1, temp2, temp3, sqr, qc, qci, qcj, qck,
		Fn1Index, Fn2Index, rqlim, BailoutTestType);

	case TRIGPLUSTRIGFP:
	case TRIGPLUSTRIG:
	    return TrigPlusTrigFractalT<Complex, double>(z, temp1, temp2, Fn1Index, Fn2Index, rqlim, BailoutTestType);

	case MANDELTRIGFP:
	case LAMBDATRIG:
	case LAMBDATRIGFP:
	case MANDELTRIG:
	    return MandelTrigFractalT<Complex, double>(z, temp1, temp3, t, Fn1Index, rqlim);

	case FPMANTRIGPLUSEXP:
	    return ManTrigPlusExpFractalT<Complex, double>(z, temp1, t, Fn1Index, rqlim, BailoutTestType);

	case FPMANTRIGPLUSZSQRD:
	case FPJULTRIGPLUSZSQRD:
	case LMANTRIGPLUSZSQRD:
	case LJULTRIGPLUSZSQRD:
	    return ManTrigPlusZSqrFractalT<Complex, double>(z, temp1, sqr, t, Fn1Index, rqlim, BailoutTestType);

	case LJULFNFN:
	case FPJULFNFN:
	case FPMANFNFN:
	case LMANFNFN:
	    return FnFnPlusFractalT<Complex, double>(z, t, temp2, Fn1Index, Fn2Index, rqlim, BailoutTestType);

	case LLAMBDAFNFN:
	case FPLAMBDAFNFN:
	case LMANLAMFNFN:
	case FPMANLAMFNFN:
	    return FnFnMulFractalT<Complex, double>(z, t, temp2, Fn1Index, Fn2Index, rqlim, BailoutTestType);

	case SQRTRIGFP:
	case SQRTRIG:
	    return SqrTrigFractalT<Complex, double>(z, temp1, Fn1Index, rqlim, BailoutTestType);

	case SQR1OVERTRIGFP:
	case SQR1OVERTRIG:
	    return Sqr1OverTrigFractalT<Complex, double>(z, Fn1Index, rqlim, BailoutTestType);

	case NUMFRACTAL:
	    return NumFractalT<Complex, double>(z, q, Fn1Index, rqlim, BailoutTestType, param);
	    
//	case FORMULA:
//	case SCREENFORMULA:
//	    return(FormulaFloat(z, q));

	}
    return 0;
    }



