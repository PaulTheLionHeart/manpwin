/*
    DDFRACTINTFUNCTIONS.CPP a module for the per pixel calculations of fractals originally from Fractint Double-double version
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	"manp.h"
#include	"fractype.h"
#include	"Complex.h"
#include	"pixel.h"
#include	"Fract.h"
#include	".\parser\TrigFn.h"

// lotsa norty externs that will be resolved when we c++ise parser
extern	    int DDFormPerPixel(DDComplex *zIn, DDComplex *qIn);	// norty declarations because this is in the formula parser
extern	    int DDFormula(DDComplex *zIn, DDComplex *qIn);
// end norty externs that will be resolved when we c++ise parser


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
	    tDD = (invert) ? DDInvertz2(cDD) : cDD;

	    DDInitFunctions(MANDELFP, z, q);
	    if (juliaflag)
		{
		*z = *q;
		tempDD.x = tDD.x;
		tempDD.y = tDD.y;
		}
	    else
		{
		*z = 0.0;
		tempDD = 0.0;
		}
	    qcDD = tDD.x;
	    qciDD = tDD.y;
	    qcjDD = param[2];
	    qckDD = param[3];
	    break;

	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	    tDD = (invert) ? DDInvertz2(cDD) : cDD;
	    tempDD.x = param[0];
	    tempDD.y = param[1];
	    temp2DD.x = param[2];
	    temp2DD.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		tempDD.x = q->x;
		tempDD.y = q->y;
		}
	    else
		{
		z->x = tDD.x + param[0];
		z->y = tDD.y + param[1];
		}
	    sqrDD.x = sqr(z->x);
	    sqrDD.y = sqr(z->y);
	    temp1DD = *z;				// set temp1 to Y value
	    temp1DD.x -= 1;
	    temp1DD = *z ^ temp1DD;
	    break;


	case FNPLUSFNPIXFP:				//  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
	case FNPLUSFNPIXLONG:
	    period_level = FALSE;			// no periodicity checking (get rid of bug PHD 2009-10-16)
	    tDD = (invert) ? DDInvertz2(cDD) : cDD;
	    if (!juliaflag)
		{
		*z = tDD;
		}
	    tempDD.x = param[0];
	    tempDD.y = param[1];
	    temp2DD.x = param[2];
	    temp2DD.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    z->x += tempDD.x;				// initial pertubation of parameters set 
	    z->y += tempDD.y;
	    temp4DD = *z;
	    TrigFn.DDCMPLXtrig(&temp4DD, &temp1DD, Fn2Index);
	    temp1DD = temp1DD * temp2DD;
	    break;

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
	    period_level = FALSE;			// no periodicity checking (get rid of bug PHD 2009-10-16)
	    if (!juliaflag)
		{
		tDD = (invert) ? DDInvertz2(cDD) : cDD;
		*z = tDD;
		}
	    tempDD.x = param[0];
	    tempDD.y = param[1];
	    temp2DD.x = param[2];
	    temp2DD.y = param[3];
	    break;

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
	    tDD = (invert) ? DDInvertz2(cDD) : cDD;
	    tempDD.x = param[0];
	    tempDD.y = param[1];
	    temp2DD.x = param[2];
	    temp2DD.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		tDD.x = q->x;
		tDD.y = q->y;
		}
	    else
		{
		z->x = tDD.x + param[0];
		z->y = tDD.y + param[1];
		}
	    sqrDD.x = sqr(z->x);			// precalculated value for regular Mandelbrot
	    sqrDD.y = sqr(z->y);
	    temp1DD = *z;
	    break;
	case FORMULA:
	case SCREENFORMULA:
	    DDFormPerPixel(z, q);
	    break;



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
	    {
	    _DDHCMPLX hold, hnew;
	    dd_real  magnitude;
	    dd_real rqlimDD = rqlim;

	    hold.x = z->x;
	    hold.y = z->y;
	    hold.z = tempDD.x;
	    hold.t = tempDD.y;

	    TrigFn.DDHComplexTrig(&hold, &hnew, Fn1Index);

	    hnew.x += qcDD;
	    hnew.y += qciDD;
	    hnew.z += qcjDD;
	    hnew.t += qckDD;

	    z->x = hnew.x;
	    z->y = hnew.y;
	    tempDD.x = hnew.z;
	    tempDD.y = hnew.t;

	    // Check bailout
	    magnitude = sqr(z->x) + sqr(z->y) + sqr(tempDD.x) + sqr(tempDD.y);
	    if (magnitude > rqlimDD)
		return 1;
	    return(0);
	    }

	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	    TrigFn.DDCMPLXtrig(z, &temp2DD, Fn1Index);		// how do we make this work? #include	".\parser\mpmath.h" of course... loads globals
	    temp2DD = temp1DD * temp2DD;
	    temp2DD.x += q->x;
	    temp2DD.y += q->y;
	    *z = temp2DD;
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case FNPLUSFNPIXFP:					//  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
	case FNPLUSFNPIXLONG:
	    TrigFn.DDCMPLXtrig(z, &temp3DD, Fn1Index);
	    temp3DD += temp1DD;
	    *z = temp3DD;
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case TRIGSQRFP:						// { z=pixel: z=trig(z*z), |z|<TEST }
	case TRIGSQR:						// to handle fractint par files
	    sqrDD.x = sqr(z->x);				// precalculated value for regular Mandelbrot
	    sqrDD.y = sqr(z->y);
	    temp1DD.y = (z->x + z->x) * z->y;
	    temp1DD.x = sqrDD.x - sqrDD.y;
	    TrigFn.DDCMPLXtrig(&temp1DD, &temp3DD, Fn1Index);
	    *z = temp3DD;
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case TRIGXTRIGFP:					// z = trig0(z)*trig1(z)
	case TRIGXTRIG:						// to handle fractint par files
	    TrigFn.DDCMPLXtrig(z, &temp1DD, Fn1Index);
	    TrigFn.DDCMPLXtrig(z, z, Fn2Index);
	    temp3DD = *z * temp1DD;
	    *z = *z * temp1DD;
	    *z = temp3DD;
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case ZXTRIGPLUSZFP:
	case ZXTRIGPLUSZ:					// to handle fractint par files
	    if (tempDD.x == 1.0 && tempDD.y == 0.0 && temp2DD.y == 0.0)
		{
		if (temp2DD.x == 1.0)				// Scott variant
		    return(DDScottZXTrigPlusZfpFractal(z, q, &TrigFn));
		else if (temp2.x == -1.0)			// Skinner variant
		    return(DDSkinnerZXTrigSubZfpFractal(z, q, &TrigFn));
		}
	    return(DDZXTrigPlusZfpFractal(z, q, &TrigFn));

	case TRIGPLUSTRIGFP:					// z = trig0(z)*p1+trig1(z)*p2
	case TRIGPLUSTRIG:					// to handle fractint par files
	    TrigFn.DDCMPLXtrig(z, &temp1DD, Fn1Index);
	    temp1DD = temp2DD * temp1DD;
	    TrigFn.DDCMPLXtrig(z, z, Fn2Index);
	    *z = *z * temp2DD;
	    *z = temp1DD + *z;
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case MANDELTRIGFP:					// we split the routine to allow for different bailout routines
	case LAMBDATRIG:					// to handle fractint par file references
	case LAMBDATRIGFP:
	case MANDELTRIG:
	    {
	    dd_real  siny, cosy, tmpexp;
	    char    *Fn1;
	    dd_real DDrqlim = rqlim;

	    Fn1 = TrigFn.FunctList[Fn1Index];

	    if (strcmp(Fn1, "sin") == 0 || strcmp(Fn1, "cos") == 0)
		{
		if (fabs(z->y) >= DDrqlim) return(1);		// sin,cos */
		TrigFn.DDCMPLXtrig(z, &temp1DD, Fn1Index); 	// temp1 = trig(old)
		temp3DD = temp1DD * tDD;			// New = longparm*trig(old) 
		*z = temp3DD;
		}
	    else if (strcmp(Fn1, "exp") == 0)			// exp added so that "bows" fractal from FRACTINT.par runs properly
		{
		// found this in  "Science of Fractal Images"
		if (fabs(z->y) >= 1.0e8) return(1);
		if (fabs(z->x) >= 6.4e2) return(1);
		siny = sin(z->y);
		cosy = cos(z->y);

		if (z->x >= DDrqlim && cosy >= 0.0) return(1);
		tmpexp = exp(z->x);
		temp1DD.x = tmpexp * cosy;
		temp1DD.y = tmpexp * siny;

		//multiply by lamda
		temp3DD.x = tDD.x * temp1DD.x - tDD.y * temp1DD.y;
		temp3DD.y = tDD.y * temp1DD.x + tDD.x * temp1DD.y;
		*z = temp3DD;
		}
	    else if (strcmp(Fn1, "sinh") == 0 || strcmp(Fn1, "cosh") == 0)
		{
		if (fabs(z->x) >= DDrqlim) return(1);
		TrigFn.DDCMPLXtrig(z, &temp1DD, Fn1Index); 	// temp1 = trig(old)
		temp3DD = tDD * temp1DD;			// New = longparm*trig(old)
		*z = temp3DD;
		}
	    else
		{
		dd_real DDrqlim = rqlim;
		if (fabs(z->y) >= DDrqlim) return(1);
		TrigFn.DDCMPLXtrig(z, &temp1DD, Fn1Index); 	// tmp = trig(old)
		temp3DD = tDD * temp1DD;			// New = longparm*trig(old)
		*z = temp3DD;
		}

	    return 0;
	    }

	case FPMANTRIGPLUSEXP:					// another Scientific American biomorph type: z(n+1) = e**z(n) + trig(z(n)) + C
	    {
	    dd_real  siny, cosy, tmpexp;
	    if (fabs(z->x) >= 6.4e2) return(1);			// DOMAIN errors
	    tmpexp = exp(z->x);
	    siny = sin(z->y);
	    cosy = cos(z->y);
	    TrigFn.DDCMPLXtrig(z, &temp1DD, Fn1Index);
								// New =   trig(old) + e**old + C 
	    temp1DD.x += tmpexp * cosy + tDD.x;
	    temp1DD.y += tmpexp * siny + tDD.y;
	    *z = temp1DD;
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);
	    }

	case FPMANTRIGPLUSZSQRD:				// From Scientific American, July 1989 A Biomorph: z(n+1) = trig(z(n))+z(n)**2+C
	case FPJULTRIGPLUSZSQRD:				// to handle fractint par file references:
	case LMANTRIGPLUSZSQRD:					// to handle fractint par file references
	case LJULTRIGPLUSZSQRD:					// to handle fractint par file references
	    sqrDD.x = sqr(z->x);
	    sqrDD.y = sqr(z->y);
	    TrigFn.DDCMPLXtrig(z, &temp1DD, Fn1Index);
	    temp1DD.x += sqrDD.x - sqrDD.y + tDD.x;
	    temp1DD.y += 2.0 * z->x * z->y + tDD.y;
	    *z = temp1DD;
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);


	case LJULFNFN:
	case FPJULFNFN:
	case FPMANFNFN:						// z = trig0(z)+p1 if mod(old) < p2.x and trig1(z)+p1 if mod(old) >= p2.x
	case LMANFNFN:
	    if (z->CSumSqr() < temp2.x)
		{
		TrigFn.DDCMPLXtrig(z, z, Fn1Index);
		*z = tDD + *z;
		}
	    else
		{
		TrigFn.DDCMPLXtrig(z, z, Fn2Index);
		*z = tDD + *z;
		}
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case LLAMBDAFNFN:
	case FPLAMBDAFNFN:
	case LMANLAMFNFN:
	case FPMANLAMFNFN:					// z = trig0(z)*p1 if mod(old) < p2.x and trig1(z)*p1 if mod(old) >= p2.x
	    if (z->CSumSqr() < temp2.x)
		{
		TrigFn.DDCMPLXtrig(z, z, Fn1Index);
		*z = tDD * *z;
		}
	    else
		{
		TrigFn.DDCMPLXtrig(z, z, Fn2Index);
		*z = tDD * *z;
		}
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case SQRTRIGFP:						// SZSB(XYAXIS) { z=pixel, TEST=(p1+3): z=sin(z)*sin(z), |z|<TEST}
	case SQRTRIG:						// to handle fractint par files
	    TrigFn.DDCMPLXtrig(z, &temp1DD, Fn1Index);
	    *z = temp1DD.CSqr();
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case SQR1OVERTRIGFP:					// z = sqr(1/trig(z))
	case SQR1OVERTRIG:					// to handle fractint par files
	    TrigFn.DDCMPLXtrig(z, z, Fn1Index);
	    *z = z->CInvert();
	    *z = z->CSqr();
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);

	case FORMULA:
	case SCREENFORMULA:
	    return(DDFormula(z, q));




	}
    return 0;
    }


int	CPixel::DDZXTrigPlusZfpFractal(DDComplex *z, DDComplex *q, CTrigFn *TrigFn)	// z = (p1*z*trig(z))+p2*z
    {
    TrigFn->DDCMPLXtrig(z, &temp1DD, Fn1Index); 		// tmp  = trig(old)
    temp1DD = tempDD * temp1DD;					// tmp  = p1*trig(old)
    temp3DD = *z * temp1DD;					// tmp2 = p1*old*trig(old)
    temp1DD = *z * temp2DD;					// tmp  = p2*old
    *z = temp1DD + temp3DD;					// New  = p1*trig(old) + p2*old
    return DDFractintBailoutTest(z, rqlim, BailoutTestType);
    }

int	CPixel::DDScottZXTrigPlusZfpFractal(DDComplex *z, DDComplex *q, CTrigFn *TrigFn)    // z = (z*trig(z))+z
    {
    TrigFn->DDCMPLXtrig(z, &temp1DD, Fn1Index); 		// tmp	= trig(old)
    temp3DD = *z * temp1DD;					// New  = old*trig(old)
    *z = *z + temp3DD;						// New  = trig(old) + old
    return DDFractintBailoutTest(z, rqlim, BailoutTestType);
    }

int	CPixel::DDSkinnerZXTrigSubZfpFractal(DDComplex *z, DDComplex *q, CTrigFn *TrigFn)
    {
    // z = (z*trig(z))-z 
    TrigFn->DDCMPLXtrig(z, &temp1DD, Fn1Index); 		// tmp	= trig(old)
    temp3DD = *z * temp1DD;					// New  = old*trig(old)
    *z = tempDD - *z;						// New  = trig(old) + old
    return DDFractintBailoutTest(z, rqlim, BailoutTestType);
    }



