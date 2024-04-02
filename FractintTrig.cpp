/*
    FRACTINTFUNCTIONS.CPP a module for the per pixel calculations of fractals originally from Fractint 
    
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
extern	int	form_per_pixel1(Complex *z, Complex *q);
extern	int	Formula1(Complex *z, Complex *q);
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
	    t = (invert) ? invertz2(*c) : *c;

	    InitFunctions(MANDELFP, z, q);
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

	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	    t = (invert) ? invertz2(*c) : *c;
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


	case FNPLUSFNPIXFP:				//  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
	case FNPLUSFNPIXLONG:
	    period_level = FALSE;			// no periodicity checking (get rid of bug PHD 2009-10-16)
	    t = (invert) ? invertz2(*c) : *c;
	    if (!juliaflag)
		{
		*z = t;
		}
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    z->x += temp.x;				// initial pertubation of parameters set 
	    z->y += temp.y;
	    temp4 = *z;
	    TrigFn.CMPLXtrig(&temp4, &temp1, Fn2Index);
	    temp1 = temp1 * temp2;
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
		t = (invert) ? invertz2(*c) : *c;
		*z = t;
		}
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];
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
	case FPMANFNFN:					// z = trig0(z)+p1 if mod(old) < p2.x and trig1(z)+p1 if mod(old) >= p2.x
	case LMANFNFN:
	case FPMANLAMFNFN:				// z = trig0(z)*p1 if mod(old) < p2.x and trig1(z)*p1 if mod(old) >= p2.x
	    t = (invert) ? invertz2(*c) : *c;
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
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
	    sqr.x = sqr(z->x);				// precalculated value for regular Mandelbrot
	    sqr.y = sqr(z->y);
	    temp1 = *z;
	    break;
	case SCREENFORMULA:
	    form_per_pixel1(z, q);
	    break;





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
	    {
	    _HCMPLX hold, hnew;
	    double  magnitude;

	    hold.x = z->x;
	    hold.y = z->y;
	    hold.z = temp.x;
	    hold.t = temp.y;

	    TrigFn.HComplexTrig(&hold, &hnew, Fn1Index);

	    hnew.x += qc;
	    hnew.y += qci;
	    hnew.z += qcj;
	    hnew.t += qck;

	    z->x = hnew.x;
	    z->y = hnew.y;
	    //   z.x = New.x;			// to allow filters to work
	    //   z.y = New.y;
	    temp.x = hnew.z;
	    temp.y = hnew.t;

	    // Check bailout
	    magnitude = sqr(z->x) + sqr(z->y) + sqr(temp.x) + sqr(temp.y);
	    if (magnitude > rqlim) 
		return 1;
	    return(0);
	    }

	case MARKSMANDELPWRFP:
	case MARKSMANDELPWR:
	    TrigFn.CMPLXtrig(z, &temp2, Fn1Index);	// how do we make this work? #include	".\parser\mpmath.h" of course... loads globals
	    temp2 = temp1 * temp2;
	    temp2.x += q->x;
	    temp2.y += q->y;
	    *z = temp2;
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case FNPLUSFNPIXFP:				//  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
	case FNPLUSFNPIXLONG:
	    TrigFn.CMPLXtrig(z, &temp3, Fn1Index);
	    temp3 += temp1;
	    *z = temp3;
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case TRIGSQRFP:					// { z=pixel: z=trig(z*z), |z|<TEST }
	case TRIGSQR:					// to handle fractint par files
	    sqr.x = sqr(z->x);				// precalculated value for regular Mandelbrot
	    sqr.y = sqr(z->y);
	    temp1.y = (z->x + z->x) * z->y;
	    temp1.x = sqr.x - sqr.y;
	    TrigFn.CMPLXtrig(&temp1, &temp3, Fn1Index);
	    *z = temp3;
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case TRIGXTRIGFP:				// z = trig0(z)*trig1(z)
	case TRIGXTRIG:					// to handle fractint par files
	    TrigFn.CMPLXtrig(z, &temp1, Fn1Index);
	    TrigFn.CMPLXtrig(z, z, Fn2Index);
	    temp3 = *z * temp1;
	    *z = *z * temp1;
	    *z = temp3;
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case ZXTRIGPLUSZFP:
	case ZXTRIGPLUSZ:				// to handle fractint par files
	    if (temp.x == 1.0 && temp.y == 0.0 && temp2.y == 0.0)
		{
		if (temp2.x == 1.0)			// Scott variant
		    return(ScottZXTrigPlusZfpFractal(z, q, &TrigFn));
		else if (temp2.x == -1.0)		// Skinner variant
		    return(SkinnerZXTrigSubZfpFractal(z, q, &TrigFn));
		}
	    return(ZXTrigPlusZfpFractal(z, q, &TrigFn));

	case TRIGPLUSTRIGFP:				// z = trig0(z)*p1+trig1(z)*p2
	case TRIGPLUSTRIG:				// to handle fractint par files
	    TrigFn.CMPLXtrig(z, &temp1, Fn1Index);
	    temp1 = temp2 * temp1;
	    TrigFn.CMPLXtrig(z, z, Fn2Index);
	    *z = *z * temp2;
	    *z = temp1 + *z;
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case MANDELTRIGFP:						// we split the routine to allow for different bailout routines
	case LAMBDATRIG:						// to handle fractint par file references
	case LAMBDATRIGFP:
	case MANDELTRIG:
	    {
	    double  siny, cosy, tmpexp;
	    char    *Fn1;

	    Fn1 = TrigFn.FunctList[Fn1Index];

	    if (strcmp(Fn1, "sin") == 0 || strcmp(Fn1, "cos") == 0)
		{
		if (fabs(z->y) >= rqlim) return(1);			// sin,cos */
		TrigFn.CMPLXtrig(z, &temp1, Fn1Index); 					// temp1 = trig(old)
		temp3 = temp1 * t;					// New = longparm*trig(old) 
		*z = temp3;
		}
	    else if (strcmp(Fn1, "exp") == 0)				// exp added so that "bows" fractal from FRACTINT.par runs properly
		{
		// found this in  "Science of Fractal Images"
		if (fabs(z->y) >= 1.0e8) return(1);
		if (fabs(z->x) >= 6.4e2) return(1);
		siny = sin(z->y);
		cosy = cos(z->y);

		if (z->x >= rqlim && cosy >= 0.0) return(1);
		tmpexp = exp(z->x);
		temp1.x = tmpexp * cosy;
		temp1.y = tmpexp * siny;

		//multiply by lamda
		temp3.x = t.x * temp1.x - t.y * temp1.y;
		temp3.y = t.y * temp1.x + t.x * temp1.y;
		*z = temp3;
		}
	    else if (strcmp(Fn1, "sinh") == 0 || strcmp(Fn1, "cosh") == 0)
		{
		if (fabs(z->x) >= rqlim) return(1);
		TrigFn.CMPLXtrig(z, &temp1, Fn1Index); 					// temp1 = trig(old)
		temp3 = t * temp1;					// New = longparm*trig(old)
		*z = temp3;
		}
	    else
		{
		if (fabs(z->y) >= rqlim) return(1);
		TrigFn.CMPLXtrig(z, &temp1, Fn1Index); 					// tmp = trig(old)
		temp3 = t * temp1;					// New = longparm*trig(old)
		*z = temp3;
		}

	    return 0;
	    }

	case FPMANTRIGPLUSEXP:				// another Scientific American biomorph type: z(n+1) = e**z(n) + trig(z(n)) + C
	    {
	    double  siny, cosy, tmpexp;
	    if (fabs(z->x) >= 6.4e2) return(1);		// DOMAIN errors
	    tmpexp = exp(z->x);
	    siny = sin(z->y);
	    cosy = cos(z->y);
	    TrigFn.CMPLXtrig(z, &temp1, Fn1Index);
							// New =   trig(old) + e**old + C 
	    temp1.x += tmpexp * cosy + t.x;
	    temp1.y += tmpexp * siny + t.y;
	    *z = temp1;
	    d = z->CSumSqr();
	    return (d >= rqlim);
	    }

	case FPMANTRIGPLUSZSQRD:			// From Scientific American, July 1989 A Biomorph: z(n+1) = trig(z(n))+z(n)**2+C
	case FPJULTRIGPLUSZSQRD:			// to handle fractint par file references:
	case LMANTRIGPLUSZSQRD:				// to handle fractint par file references
	case LJULTRIGPLUSZSQRD:				// to handle fractint par file references
	    sqr.x = sqr(z->x);
	    sqr.y = sqr(z->y);
	    TrigFn.CMPLXtrig(z, &temp1, Fn1Index);
	    temp1.x += sqr.x - sqr.y + t.x;
	    temp1.y += 2.0 * z->x * z->y + t.y;
	    *z = temp1;
	    d = z->CSumSqr();
	    return (d >= rqlim);


	case FPMANFNFN:					// z = trig0(z)+p1 if mod(old) < p2.x and trig1(z)+p1 if mod(old) >= p2.x
	case LMANFNFN:
	    if (z->CSumSqr() < temp2.x)
		{
		TrigFn.CMPLXtrig(z, z, Fn1Index);
		*z = t + *z;
		}
	    else
		{
		TrigFn.CMPLXtrig(z, z, Fn2Index);
		*z = t + *z;
		}
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case FPMANLAMFNFN:				// z = trig0(z)*p1 if mod(old) < p2.x and trig1(z)*p1 if mod(old) >= p2.x
	    if (z->CSumSqr() < temp2.x)
		{
		TrigFn.CMPLXtrig(z, z, Fn1Index);
		*z = t * *z;
		}
	    else
		{
		TrigFn.CMPLXtrig(z, z, Fn2Index);
		*z = t * *z;
		}
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case SQRTRIGFP:					// SZSB(XYAXIS) { z=pixel, TEST=(p1+3): z=sin(z)*sin(z), |z|<TEST}
	case SQRTRIG:					// to handle fractint par files
	    TrigFn.CMPLXtrig(z, &temp1, Fn1Index);
	    *z = temp1.CSqr();
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case SQR1OVERTRIGFP:				// z = sqr(1/trig(z))
	case SQR1OVERTRIG:				// to handle fractint par files
	    TrigFn.CMPLXtrig(z, z, Fn1Index);
	    *z = z->CInvert();
	    *z = z->CSqr();
//	    CMPLXrecip(*z, *z);
//	    CMPLXsqr(*z, *z);
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case SCREENFORMULA:
	    return(Formula1(z, q));




	}
    return 0;
    }


int	CPixel::ZXTrigPlusZfpFractal(Complex *z, Complex *q, CTrigFn *TrigFn)	// z = (p1*z*trig(z))+p2*z
    {
    TrigFn->CMPLXtrig(z, &temp1, Fn1Index); 	// tmp  = trig(old)
    temp1 = temp * temp1;		// tmp  = p1*trig(old)
    temp3 = *z * temp1;			// tmp2 = p1*old*trig(old)
    temp1 = *z * temp2;			// tmp  = p2*old
    *z = temp1 + temp3;			// New  = p1*trig(old) + p2*old
    d = z->CSumSqr();
    return (d >= rqlim);
    }

int	CPixel::ScottZXTrigPlusZfpFractal(Complex *z, Complex *q, CTrigFn *TrigFn)    // z = (z*trig(z))+z
    {
    TrigFn->CMPLXtrig(z, &temp1, Fn1Index); 	// tmp	= trig(old)
    temp3 = *z * temp1;			// New  = old*trig(old)
    *z = *z + temp3;			// New  = trig(old) + old
    d = z->CSumSqr();
    return (d >= rqlim);
    }

int	CPixel::SkinnerZXTrigSubZfpFractal(Complex *z, Complex *q, CTrigFn *TrigFn)
    {
    // z = (z*trig(z))-z 
    TrigFn->CMPLXtrig(z, &temp1, Fn1Index); 	// tmp	= trig(old)
    temp3 = *z * temp1;			// New  = old*trig(old)
    *z = temp - *z;			// New  = trig(old) + old
    d = z->CSumSqr();
    return (d >= rqlim);
    }



