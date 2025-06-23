/*
    QDFRACTINTFUNCTIONS.CPP a module for the per pixel calculations of fractals originally from Fractint Quad-double version
    
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
extern	    int QDFormPerPixel(QDComplex *zIn, QDComplex *qIn);	// norty declarations because this is in the formula parser
extern	    int QDFormula(QDComplex *zIn, QDComplex *qIn);
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
	    if (juliaflag)
		{
		*z = *q;
		tempQD.x = tQD.x;
		tempQD.y = tQD.y;
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
	    tQD = (invert) ? QDInvertz2(cQD) : cQD;
	    tempQD.x = param[0];
	    tempQD.y = param[1];
	    temp2QD.x = param[2];
	    temp2QD.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		tempQD.x = q->x;
		tempQD.y = q->y;
		}
	    else
		{
		z->x = tQD.x + param[0];
		z->y = tQD.y + param[1];
		}
	    sqrQD.x = sqr(z->x);
	    sqrQD.y = sqr(z->y);
	    temp1QD = *z;				// set temp1 to Y value
	    temp1QD.x -= 1;
	    temp1QD = *z ^ temp1QD;
	    break;


	case FNPLUSFNPIXFP:				//  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
	case FNPLUSFNPIXLONG:
	    period_level = FALSE;			// no periodicity checking (get rid of bug PHD 2009-10-16)
	    tQD = (invert) ? QDInvertz2(cQD) : cQD;
	    if (!juliaflag)
		{
		*z = tQD;
		}
	    tempQD.x = param[0];
	    tempQD.y = param[1];
	    temp2QD.x = param[2];
	    temp2QD.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    z->x += tempQD.x;				// initial pertubation of parameters set 
	    z->y += tempQD.y;
	    temp4QD = *z;
	    TrigFn.QDCMPLXtrig(&temp4QD, &temp1QD, Fn2Index);
	    temp1QD = temp1QD * temp2QD;
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
		tQD = (invert) ? QDInvertz2(cQD) : cQD;
		*z = tQD;
		}
	    tempQD.x = param[0];
	    tempQD.y = param[1];
	    temp2QD.x = param[2];
	    temp2QD.y = param[3];
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
	    tQD = (invert) ? QDInvertz2(cQD) : cQD;
	    tempQD.x = param[0];
	    tempQD.y = param[1];
	    temp2QD.x = param[2];
	    temp2QD.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		tQD.x = q->x;
		tQD.y = q->y;
		}
	    else
		{
		z->x = tQD.x + param[0];
		z->y = tQD.y + param[1];
		}
	    sqrQD.x = sqr(z->x);			// precalculated value for regular Mandelbrot
	    sqrQD.y = sqr(z->y);
	    temp1QD = *z;
	    break;
	case FORMULA:
	case SCREENFORMULA:
	    QDFormPerPixel(z, q);
	    break;



	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::QDRunFractintTrigFunctions(WORD type, QDComplex *z, QDComplex *q, BYTE *SpecialFlag, long *iteration)
    {
    CTrigFn	TrigFn;
    size_t	SizeOfTrigFn = sizeof(CTrigFn);			// info only

    switch (type)
	{
	case HYPERCMPLXFP:
	case HYPERCMPLXJFP:
	    {
	    _QDHCMPLX hold, hnew;
	    qd_real  magnitude;
	    qd_real rqlimQD = rqlim;

	    hold.x = z->x;
	    hold.y = z->y;
	    hold.z = tempQD.x;
	    hold.t = tempQD.y;

	    TrigFn.QDHComplexTrig(&hold, &hnew, Fn1Index);

	    hnew.x += qcQD;
	    hnew.y += qciQD;
	    hnew.z += qcjQD;
	    hnew.t += qckQD;

	    z->x = hnew.x;
	    z->y = hnew.y;
	    //   z.x = New.x;					// to allow filters to work
	    //   z.y = New.y;
	    tempQD.x = hnew.z;
	    tempQD.y = hnew.t;

	    // Check bailout
	    magnitude = sqr(z->x) + sqr(z->y) + sqr(tempQD.x) + sqr(tempQD.y);
	    if (magnitude > rqlimQD)
		return 1;
	    return(0);
	    }

	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	    TrigFn.QDCMPLXtrig(z, &temp2QD, Fn1Index);		// how do we make this work? #include	".\parser\mpmath.h" of course... loads globals
	    temp2QD = temp1QD * temp2QD;
	    temp2QD.x += q->x;
	    temp2QD.y += q->y;
	    *z = temp2QD;
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case FNPLUSFNPIXFP:					//  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
	case FNPLUSFNPIXLONG:
	    TrigFn.QDCMPLXtrig(z, &temp3QD, Fn1Index);
	    temp3QD += temp1QD;
	    *z = temp3QD;
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case TRIGSQRFP:						// { z=pixel: z=trig(z*z), |z|<TEST }
	case TRIGSQR:						// to handle fractint par files
	    sqrQD.x = sqr(z->x);				// precalculated value for regular Mandelbrot
	    sqrQD.y = sqr(z->y);
	    temp1QD.y = (z->x + z->x) * z->y;
	    temp1QD.x = sqrQD.x - sqrQD.y;
	    TrigFn.QDCMPLXtrig(&temp1QD, &temp3QD, Fn1Index);
	    *z = temp3QD;
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case TRIGXTRIGFP:					// z = trig0(z)*trig1(z)
	case TRIGXTRIG:						// to handle fractint par files
	    TrigFn.QDCMPLXtrig(z, &temp1QD, Fn1Index);
	    TrigFn.QDCMPLXtrig(z, z, Fn2Index);
	    temp3QD = *z * temp1QD;
	    *z = *z * temp1QD;
	    *z = temp3QD;
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case ZXTRIGPLUSZFP:
	case ZXTRIGPLUSZ:					// to handle fractint par files
	    if (tempQD.x == 1.0 && tempQD.y == 0.0 && temp2QD.y == 0.0)
		{
		if (temp2DD.x == 1.0)				// Scott variant
		    return(QDScottZXTrigPlusZfpFractal(z, q, &TrigFn));
		else if (temp2.x == -1.0)			// Skinner variant
		    return(QDSkinnerZXTrigSubZfpFractal(z, q, &TrigFn));
		}
	    return(QDZXTrigPlusZfpFractal(z, q, &TrigFn));

	case TRIGPLUSTRIGFP:					// z = trig0(z)*p1+trig1(z)*p2
	case TRIGPLUSTRIG:					// to handle fractint par files
	    TrigFn.QDCMPLXtrig(z, &temp1QD, Fn1Index);
	    temp1QD = temp2QD * temp1QD;
	    TrigFn.QDCMPLXtrig(z, z, Fn2Index);
	    *z = *z * temp2QD;
	    *z = temp1QD + *z;
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case MANDELTRIGFP:					// we split the routine to allow for different bailout routines
	case LAMBDATRIG:					// to handle fractint par file references
	case LAMBDATRIGFP:
	case MANDELTRIG:
	    {
	    qd_real  siny, cosy, tmpexp;
	    char    *Fn1;
	    qd_real QDrqlim = rqlim;

	    Fn1 = TrigFn.FunctList[Fn1Index];

	    if (strcmp(Fn1, "sin") == 0 || strcmp(Fn1, "cos") == 0)
		{
		if (fabs(z->y) >= QDrqlim) return(1);		// sin,cos */
		TrigFn.QDCMPLXtrig(z, &temp1QD, Fn1Index); 	// temp1 = trig(old)
		temp3QD = temp1QD * tQD;			// New = longparm*trig(old) 
		*z = temp3QD;
		}
	    else if (strcmp(Fn1, "exp") == 0)			// exp added so that "bows" fractal from FRACTINT.par runs properly
		{
		// found this in  "Science of Fractal Images"
		if (fabs(z->y) >= 1.0e8) return(1);
		if (fabs(z->x) >= 6.4e2) return(1);
		siny = sin(z->y);
		cosy = cos(z->y);

		if (z->x >= QDrqlim && cosy >= 0.0) return(1);
		tmpexp = exp(z->x);
		temp1QD.x = tmpexp * cosy;
		temp1QD.y = tmpexp * siny;

		//multiply by lamda
		temp3QD.x = tQD.x * temp1QD.x - tQD.y * temp1QD.y;
		temp3QD.y = tQD.y * temp1QD.x + tQD.x * temp1QD.y;
		*z = temp3QD;
		}
	    else if (strcmp(Fn1, "sinh") == 0 || strcmp(Fn1, "cosh") == 0)
		{
		if (fabs(z->x) >= QDrqlim) return(1);
		TrigFn.QDCMPLXtrig(z, &temp1QD, Fn1Index); 	// temp1 = trig(old)
		temp3QD = tQD * temp1QD;			// New = longparm*trig(old)
		*z = temp3QD;
		}
	    else
		{
		dd_real QDrqlim = rqlim;
		if (fabs(z->y) >= QDrqlim) return(1);
		TrigFn.QDCMPLXtrig(z, &temp1QD, Fn1Index); 	// tmp = trig(old)
		temp3QD = tQD * temp1QD;			// New = longparm*trig(old)
		*z = temp3QD;
		}

	    return 0;
	    }

	case FPMANTRIGPLUSEXP:					// another Scientific American biomorph type: z(n+1) = e**z(n) + trig(z(n)) + C
	    {
	    qd_real  siny, cosy, tmpexp;
	    if (fabs(z->x) >= 6.4e2) return(1);			// DOMAIN errors
	    tmpexp = exp(z->x);
	    siny = sin(z->y);
	    cosy = cos(z->y);
	    TrigFn.QDCMPLXtrig(z, &temp1QD, Fn1Index);
								// New =   trig(old) + e**old + C 
	    temp1QD.x += tmpexp * cosy + tQD.x;
	    temp1QD.y += tmpexp * siny + tQD.y;
	    *z = temp1QD;
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);
	    }

	case FPMANTRIGPLUSZSQRD:				// From Scientific American, July 1989 A Biomorph: z(n+1) = trig(z(n))+z(n)**2+C
	case FPJULTRIGPLUSZSQRD:				// to handle fractint par file references:
	case LMANTRIGPLUSZSQRD:					// to handle fractint par file references
	case LJULTRIGPLUSZSQRD:					// to handle fractint par file references
	    sqrQD.x = sqr(z->x);
	    sqrQD.y = sqr(z->y);
	    TrigFn.QDCMPLXtrig(z, &temp1QD, Fn1Index);
	    temp1QD.x += sqrQD.x - sqrQD.y + tQD.x;
	    temp1QD.y += 2.0 * z->x * z->y + tQD.y;
	    *z = temp1QD;
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);


	case LJULFNFN:
	case FPJULFNFN:
	case FPMANFNFN:						// z = trig0(z)+p1 if mod(old) < p2.x and trig1(z)+p1 if mod(old) >= p2.x
	case LMANFNFN:
	    if (z->CSumSqr() < temp2.x)
		{
		TrigFn.QDCMPLXtrig(z, z, Fn1Index);
		*z = tQD + *z;
		}
	    else
		{
		TrigFn.QDCMPLXtrig(z, z, Fn2Index);
		*z = tQD + *z;
		}
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case LLAMBDAFNFN:
	case FPLAMBDAFNFN:
	case LMANLAMFNFN:
	case FPMANLAMFNFN:					// z = trig0(z)*p1 if mod(old) < p2.x and trig1(z)*p1 if mod(old) >= p2.x
	    if (z->CSumSqr() < temp2.x)
		{
		TrigFn.QDCMPLXtrig(z, z, Fn1Index);
		*z = tQD * *z;
		}
	    else
		{
		TrigFn.QDCMPLXtrig(z, z, Fn2Index);
		*z = tQD * *z;
		}
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case SQRTRIGFP:						// SZSB(XYAXIS) { z=pixel, TEST=(p1+3): z=sin(z)*sin(z), |z|<TEST}
	case SQRTRIG:						// to handle fractint par files
	    TrigFn.QDCMPLXtrig(z, &temp1QD, Fn1Index);
	    *z = temp1QD.CSqr();
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case SQR1OVERTRIGFP:					// z = sqr(1/trig(z))
	case SQR1OVERTRIG:					// to handle fractint par files
	    TrigFn.QDCMPLXtrig(z, z, Fn1Index);
	    *z = z->CInvert();
	    *z = z->CSqr();
	    return QDFractintBailoutTest(z, rqlim, BailoutTestType);

	case FORMULA:
	case SCREENFORMULA:
	    return(QDFormula(z, q));
	}
    return 0;
    }


int	CPixel::QDZXTrigPlusZfpFractal(QDComplex *z, QDComplex *q, CTrigFn *TrigFn)	// z = (p1*z*trig(z))+p2*z
    {
    TrigFn->QDCMPLXtrig(z, &temp1QD, Fn1Index); 		// tmp  = trig(old)
    temp1QD = tempQD * temp1QD;					// tmp  = p1*trig(old)
    temp3QD = *z * temp1QD;					// tmp2 = p1*old*trig(old)
    temp1QD = *z * temp2QD;					// tmp  = p2*old
    *z = temp1QD + temp3QD;					// New  = p1*trig(old) + p2*old
    return QDFractintBailoutTest(z, rqlim, BailoutTestType);
    }

int	CPixel::QDScottZXTrigPlusZfpFractal(QDComplex *z, QDComplex *q, CTrigFn *TrigFn)    // z = (z*trig(z))+z
    {
    TrigFn->QDCMPLXtrig(z, &temp1QD, Fn1Index); 		// tmp	= trig(old)
    temp3QD = *z * temp1QD;					// New  = old*trig(old)
    *z = *z + temp3QD;						// New  = trig(old) + old
    return QDFractintBailoutTest(z, rqlim, BailoutTestType);
    }

int	CPixel::QDSkinnerZXTrigSubZfpFractal(QDComplex *z, QDComplex *q, CTrigFn *TrigFn)
    {
    // z = (z*trig(z))-z 
    TrigFn->QDCMPLXtrig(z, &temp1QD, Fn1Index); 		// tmp	= trig(old)
    temp3QD = *z * temp1QD;					// New  = old*trig(old)
    *z = tempQD - *z;						// New  = trig(old) + old
    return QDFractintBailoutTest(z, rqlim, BailoutTestType);
    }



