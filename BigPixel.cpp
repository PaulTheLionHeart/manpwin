/*
    BigPixel.cpp a module for the per pixel calculations of fractals using BigNums. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	<stdio.h>
#include	"manp.h"
#include	"colour.h"
#include	"big.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"complex.h"
#include	"filter.h"
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"pixel.h"
#include	"Potential.h"

/**************************************************************************
	Initialise fractal
**************************************************************************/

int    CPixel::init_big_fractal(void)
    {
    BOOL    IsBig = FALSE;
    int	    i;

    for (i = 0; BigFractalSpecific[i].big_calctype; i++)
	if (type == BigFractalSpecific[i].type)	// check the list of "allowed" fractals.    
	    {
	    BigFractPtr = i;
	    IsBig = TRUE;
	    break;
	    }
    if (!IsBig)
	return -1;
    if (type == EXPFRACTAL)			// there's no initialisation for exp()
	return 0;
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(BigInitFunctions(type, zBig, qBig));
    else if (BigFractalSpecific[BigFractPtr].big_per_pixel() < 0)
	return -1;
    return 0;
    }

/**************************************************************************
	Run fractal
**************************************************************************/

int    CPixel::run_big_fractal(void)
    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(BigRunFunctions(type, zBig, qBig, &SpecialFlag, iteration));
    else
	return BigFractalSpecific[BigFractPtr].big_calctype();
    }

/**************************************************************************
	Filter
**************************************************************************/

int	CPixel::DoBigFilter(int method, int hooper)

    {
    double	magnitude = 0.0;
    BigDouble	temp;
    CPotential	Pot;

    if (colours == 256 && decomp > 0)
	*iteration = BigDecomposition(zBig->x, zBig->y);
    else if (logval)
	*iteration = (BYTE) (*(logtable + (*iteration % MAXTHRESHOLD)));
    else   
	{
	switch (method)
	    {
	    case EPSCROSS:
		if (hooper == 1)
		    *iteration = special;
		else if (hooper == 2)
		    *iteration = (special << 1);
		break;
						// these options by Richard Hughes modified by TW
						// Add 7 to overcome negative values on the MANDEL
	    case REAL:				// "real"
		*iteration += zBig->x.BigDoubleToInt() + 7L;
		break;
	    case IMAG:	    			// "imag"
		*iteration += zBig->y.BigDoubleToInt() + 7L;
		break;
	    case MULT:				// "mult"
		temp = 0.0;
		if (!(temp == zBig->y))
		    {
		    temp = zBig->x / zBig->y;
		    *iteration = (long)((double)(*iteration) * temp.BigDoubleToDouble());
		    }
		break;
	    case SUM:				// "sum"
		temp = zBig->x + zBig->y;
		*iteration += temp.BigDoubleToInt();
		break;
	    case ATAN:				// "atan"
		mpfr_atan2(temp.x, zBig->y.x, zBig->x.x, MPFR_RNDN);
		*iteration = (long)fabs(temp.BigDoubleToDouble()*180.0 / PI);
		break;
	    case POTENTIAL:
		magnitude = zBig->CSumSqr();
		*iteration = Pot.potential(magnitude, *iteration, threshold, TrueCol, colors, potparam);

		break;
	    }
	}
    return 0;
    }

/**************************************************************************
	Get Float Iteration per pixel
**************************************************************************/

void	CPixel::CalcBigFloatIteration(double error, double *wpixels, int row, int col, BigComplex z, BigComplex OldZ, BigComplex OlderZ)
    {
    double	log_zn, nu, t;
    int		SlopeDegree, BailoutType;
    BigDouble	BigTemp, BigTemp1, LogA, LogB;

    if (FloatIteration < threshold)
	{
	DWORD	index;
	BigComplex	a, b;

	if (type == 204)													// Tierazon
	    {
	    SlopeDegree = (TierazonSpecific[subtype].SlopeDegree == -1) ? *degree : TierazonSpecific[subtype].SlopeDegree;
	    BailoutType = TierazonSpecific[subtype].BailoutType;
	    }
	else if (type == 228)													// Mandel derivatives
	    {
	    SlopeDegree = (MandelDerivSpecific[subtype].SlopeDegree == -1) ? *degree : MandelDerivSpecific[subtype].SlopeDegree;
	    BailoutType = MandelDerivSpecific[subtype].BailoutType;
	    }
	else
	    {
	    SlopeDegree = (fractalspecific[type].SlopeDegree == -1) ? *degree : fractalspecific[type].SlopeDegree;
	    BailoutType = fractalspecific[type].BailoutType;
	    }

	switch (BailoutType)
	    {
	    case ESCAPING:
		{
		BigTemp = zBig->x * zBig->x + zBig->y * zBig->y;
//		BigTemp1 = BigTemp.BigLog();
//		log_zn = mpfr_get_d(BigTemp1.x, MPFR_RNDN) / SlopeDegree;
		double  t = mpfr_get_d(BigTemp.x, MPFR_RNDN);
		log_zn = log(t) / SlopeDegree;
		nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		FloatIteration = FloatIteration + 1 - nu;
		}
		break;
/*
	    case ESCAPING1:
		log_zn = log(sqr(rqlim)) - log(OldZ.x * OldZ.x + OldZ.y * OldZ.y);		// escape method 1 (page 24 Fractal-Zoomer Algorithms.docx)
		t = log(z.x * z.x + z.y * z.y) - log(OldZ.x * OldZ.x + OldZ.y * OldZ.y);
		nu = log_zn / t;
		FloatIteration = FloatIteration + nu;
		break;
	    case ESCAPING2:
		log_zn = log(z.x * z.x + z.y * z.y) / log(sqr(rqlim));				// escape method 2 (page 24 Fractal-Zoomer Algorithms.docx)
		t = log(z.x * z.x + z.y * z.y) / log(OldZ.x * OldZ.x + OldZ.y * OldZ.y);
		nu = log_zn / t;
		FloatIteration = FloatIteration + 1 - nu;
		break;
*/
	    case CONVERGING:
		a = OldZ - OlderZ;
		b = *zBig - OldZ;
		BigTemp = a.x * a.x + a.y * a.y;
		LogA = BigTemp.BigLog();
		log_zn = log(error) - mpfr_get_d(LogA.x, MPFR_RNDN);
//		log_zn = log(error) - log(a.x * a.x + a.y * a.y);				// convergence method 1 (page 25 Fractal-Zoomer Algorithms.docx)
		BigTemp = b.x * b.x + b.y * b.y;
		LogB = BigTemp.BigLog();
		BigTemp1 = LogB - LogA;
		t = mpfr_get_d(BigTemp1.x, MPFR_RNDN);
//		t = log(b.x * b.x + b.y * b.y) - log(a.x * a.x + a.y * a.y);
		nu = log_zn / t;
		FloatIteration = FloatIteration + nu;
		break;
/*
	    case CONVERGING1:
		a = OldZ - OlderZ;
		b = z - OldZ;
		log_zn = log(error) / log(b.x * b.x + b.y * b.y);				// convergence method 2 (page 25 Fractal-Zoomer Algorithms.docx)
		t = log(b.x * b.x + b.y * b.y) / log(a.x * a.x + a.y * a.y);
		nu = log_zn / t;
		FloatIteration = FloatIteration + nu;
		break;
	    case CONVERGINGMAG:
		a = OldZ - root;
		b = z - root;
		log_zn = log(error) - log(a.x * a.x + a.y * a.y);				// convergence method 1 (page 25 Fractal-Zoomer Algorithms.docx)
		t = log(b.x * b.x + b.y * b.y) - log(a.x * a.x + a.y * a.y);
		nu = log_zn / t;
		FloatIteration = FloatIteration + nu;
		break;
	    case CONVERGINGMAG1:
		a = OldZ - root;
		b = z - root;
		log_zn = log(error) / log(b.x * b.x + b.y * b.y);				// convergence method 2 (page 26 Fractal-Zoomer Algorithms.docx)
		t = log(b.x * b.x + b.y * b.y) - log(a.x * a.x + a.y * a.y);
		nu = log_zn / t;
		FloatIteration = FloatIteration + nu;
		break;
*/
	    default:
		BigTemp = zBig->x * zBig->x + zBig->y * zBig->y;
		BigTemp1 = BigTemp.BigLog();
		log_zn = mpfr_get_d(BigTemp1.x, MPFR_RNDN) / SlopeDegree;
		nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		FloatIteration = FloatIteration + 1 - nu;
		break;
	    }

	if ((long)FloatIteration >= threshold)
	    FloatIteration = (double)threshold;
	else if (SpecialFlag)
	    FloatIteration = (double)special;
	index = ((DWORD)row * (DWORD)width) + (DWORD)col;
	if (col >= 0 && col < xdots - 1 && row >= 0 && row < ydots - 1)
	    *(wpixels + index) = FloatIteration;
	}
    }

/**************************************************************************
	Run fractal
**************************************************************************/

//extern    void	ShowBignum(BigDouble x, char *Location);		// use for debugging

long	CPixel::DoBigFract(HWND hwnd, int row, int col)

    {
    long	real_iteration;			// actual count for orbit deletion
    BigComplex	bigTemp;
    Complex	tempComplex;
    BigDouble	magnitude;
    BigDouble	min_orbit;			// orbit value closest to origin
    long	min_index;			// iteration of min_orbit
//    double	tantable[16];			// used for Star Trails
    BigDouble	close = 0.01;

    magnitude = 0.0;
    min_orbit = 100000.0;

/*  No point, it doesn't have a fractal nature
    if (method == STARTRAIL)
	{
	int	i;
	for (i = 0; i < 16; i++)
	    tantable[i] = 0.0;
	}
*/
    int		savedand, savedincr;		// for periodicity checking
    int		result;
    BigComplex	BigSaved = 0.0;
    BigDouble	BioMorphTest = 0.5;
    double	temp;
    int		hooper = 0;

    if (period_level == 0)
	oldcolour = 32767; 			// don't check periodicity at all
    else //if (reset_period)
	oldcolour = 240;			// don't check periodicity 1st n iterations

//    BioFlag = FALSE;				// no biomorph colour yet
    SpecialFlag = FALSE;			// no special colour yet. Use for decomp etc
    savedand = 1;				// begin checking every other cycle
    savedincr = 1;				// start checking the very first time

    *iteration = 0L;
    real_iteration = 0;	
    phaseflag = 0;				// assume all type 5, 9 fractals same colour

    if (method >= TIERAZONFILTERS)
	{
	bigTemp = *qBig;
	tempComplex = bigTemp.CBig2Double();
	TZfilter->InitFilter(method, threshold, dStrands, nFDOption, UseCurrentPalette);		// initialise the constants used by Tierazon fractals
	TZfilter->LoadFilterQ(tempComplex);
	}

    if (juliaflag)
	{
	switch (orientation)
	    {
	    case 0:				// normal
		qBig->x = j.x;
		qBig->y = j.y;
    //	    q = j;
		break;
	    case 90:				// 90 degrees
		qBig->x = -j.y;
		qBig->y = -j.x;
    //	    q.x = -j.y;
    //	    q.y = -j.x;
		break;
	    case 180:				// 180 degrees
		qBig->x = -j.x;
		qBig->y = -j.y;
    //	    q = -j;
		break;
	    case 270:				// 270 degrees
		qBig->x = j.y;
		qBig->y = j.x;
    //	    q.x = j.y;
    //	    q.y = j.x;
		break;
	    }
	*zBig = (invert) ? BigInvertz2(*cBig) : *cBig;
	}
    else
	{
	*qBig = (invert) ? BigInvertz2(*cBig) : *cBig;
	*zBig = 0.0;
	}

//    ShowBignum(zBig->x, "x before init");
//    ShowBignum(zBig->y, "y before init");
    if (init_big_fractal() < 0)
	return(BLUE);
//    ShowBignum(zBig->x, "x after init");
//    ShowBignum(zBig->y, "y after init");
    if (method == BOF60 || method == BOF61)
	{
	magnitude = 0.0;
	min_orbit = 100000.0;
	}

    if (method == POTENTIAL)
	BigBailout = potparam[2];

    FloatIteration = 0.0;
    for EVER
	{
	if (calcmode == 'F')
	    {
	    BigOlderZ = BigOldZ;
	    BigOldZ = *zBig;
	    }
	if (FloatIteration >= threshold)
	//    if (iteration >= threshold)
	    break;
	(*iteration)++;
	FloatIteration++;

//	ThreadStatus[ThreadNum].curpass = curpass;
//	ThreadStatus[ThreadNum].totpasses = totpasses;
//	ShowBignum(zBig->x, "x before runfract");
//	ShowBignum(zBig->y, "y before runfract");

	result = run_big_fractal();
	if (result < 0)
	    return(BLUE);				// division by zero (Was Blue)
	else if (result == 1)				// escape time
	    break;
	if (type == RATIONALMAP)
	    return(*iteration);
	/*  No point, it doesn't have a fractal nature

	if (method == STARTRAIL)
	    {
	    if (0 < iteration && iteration < 16)
		tantable[iteration - 1] = z->y / (z->x + 0.000001);
	    }
	else 
	*/
	if (method == EPSCROSS)
	    {
	    hooper = 0;
	    if (zBig->x.BigAbs() < close)
		{
		hooper = 1; // close to y axis
		break;
		}
	    else if (zBig->y.BigAbs() < close)
		{
		hooper = 2; // close to x axis
		break;
		}
	    }
	else if (method == BOF60 || method == BOF61)
	    {
	    magnitude = zBig->CSumSqr();
	    if (magnitude < min_orbit)
		{
		min_orbit = magnitude;
		min_index = (long)*iteration + 1L;
		}
	    }

	else if (method >= TIERAZONFILTERS)
	    {
	    bigTemp = *zBig;
	    tempComplex = bigTemp.CBig2Double();
	    TZfilter->DoTierazonFilter(tempComplex, iteration);
	    }

	if (*iteration > oldcolour)			// check periodicity
	    {
	    if ((*iteration & savedand) == 0)		// time to save a new value
		{
		BigSaved = *zBig;
		if (--savedincr == 0)			// time to lengthen the periodicity?
		    {
		    savedand = (savedand << 1) + 1;	// longer periodicity
		    savedincr = 4;			// restart counter
		    }
		}
	    else		     			// check against an old save
		{
		BigDouble   xAbs = BigSaved.x - zBig->x;
		BigDouble   yAbs = BigSaved.y - zBig->y;

		if (xAbs.BigAbs() < BigCloseEnough)
		    if (yAbs.BigAbs() < BigCloseEnough)
			*iteration = threshold;
		}
	    }
	}

    if (calcmode == 'F')
	{
	SlopeError = 1.0e-9;
	CalcBigFloatIteration(SlopeError, wpixels, row, col, *zBig, BigOldZ, BigOlderZ);
	}
    if (*iteration >= threshold && period_level)
	oldcolour = 0;			// check periodicity immediately next time
    else
	oldcolour = *iteration + 10;	// check when past this+10 next time

    if (*iteration >= threshold)
	{
	BigDouble	t = min_orbit.BigSqrt() * 75.0;
	if (method == BOF60)
	    *iteration = (int)(t.BigDoubleToDouble());
	else if (method == BOF61)
	    *iteration = min_index;
	}

    if (SpecialFlag)
	return(special);		// flag for special colour
    if (*iteration < threshold)
	{
	if (biomorph >= 0)
	    {
	    if (zBig->x.BigAbs() < BioMorphTest || zBig->y.BigAbs() < BioMorphTest)
		{
		*iteration = biomorph;
//		BioFlag = TRUE;
		}
	    }
	DoBigFilter(method, hooper);

      // eliminate negative colors & wrap arounds
      
	if (*iteration < 0)
	    *iteration = 0;
	if (*iteration > threshold && decomp <= threshold)		// for small thresholds, we can still have higher decomp levels
	    *iteration = threshold;
//	else if (colours == 256 && decomp > 0)
//	    iteration = float_decomposition(z.x, z.y);
//	else if (logval)
//	    iteration = (BYTE) (*(logtable + iteration));
	}
    else
	{
	if (method == ZMAG)
	    {
	    temp = zBig->CSumSqr();
	    *iteration = (long)(temp * double(threshold >> 1) + 1.0);
	    }
	else
	    *iteration = threshold;
	}

    if (method >= TIERAZONFILTERS)
	{
	BigComplex	bigTemp;
	Complex		tempComplex;

	bigTemp = *zBig;
	tempComplex = bigTemp.CBig2Double();
	TZfilter->EndTierazonFilter(tempComplex, iteration, TrueCol);
	return *iteration;
	}

    if (type == NEWTON && subtype != 'N')
	return (*color);				// Newton root colour
    return(*iteration);
    }

    /**************************************************************************
	     Invert fractal
     **************************************************************************/

    BigComplex	CPixel::BigInvertz2(BigComplex  & Cmplx1)

	{
	BigComplex	temp;
	BigDouble	tempsqrx, BigRadius = f_radius;

	temp.x = Cmplx1.x;
	temp.y = Cmplx1.y;
	temp.x -= f_xcenter; temp.y -= f_ycenter;	// Normalize values to center of circle

	tempsqrx = temp.x.BigSqr() + temp.y.BigSqr();	// Get old radius
	if (tempsqrx.BigAbs() > (BigDouble)FLT_MIN)
	    tempsqrx = BigRadius / tempsqrx;
	else
	    tempsqrx = FLT_MAX;			// a big number, but not TOO big
	temp.x *= tempsqrx;
	temp.y *= tempsqrx;				// Perform inversion
	temp.x += f_xcenter;
	temp.y += f_ycenter;			// Renormalize
	return  temp;
	}

    /************************************************************************
	Calculate Big Fractal
************************************************************************/

long	CPixel::BigCalcFrac(HWND hwnd, int row, int col, int user_data(HWND hwnd))

    {
 //   FloatCornerstoBig(var);
    if (pairflag)		// half size screens: only do every second row / col
	if (row % pairflag || col % pairflag)
	    if (row != (int)ydots - 1)			// must trigger for last line
		return(threshold);
    if (row != *oldrow)
	{
	if (pairflag && row)		// draw row for right hand image
	    draw_right_image((short)(*oldrow));
	switch (orientation)
	    {
	    case NORMAL:					// normal
		cBig->y = *Big_yymax - Big_ygap * (double)row;
		break;
	    case 90:						// 90 degrees
		cBig->x = -(*Big_yymax - Big_xgap * (double)row);
		break;
	    case 180:						// 180 degrees
		cBig->y = -(*Big_yymax - Big_ygap * (double)row);
		break;
	    case 270:						// 270 degrees
		cBig->x = *Big_yymax - Big_xgap * (double)row;
		break;
	    }
	*oldrow = row;
	}
    if (col != *oldcol)
	{
	switch (orientation)
	    {
	    case NORMAL:					// normal
		cBig->x = Big_xgap * (double)col + BigHor;
		break;
	    case 90:						// 90 degrees
		cBig->y = -(Big_ygap * (double)col + BigHor);
		break;
	    case 180:						// 180 degrees
		cBig->x = -(Big_xgap * (double)col + BigHor);
		break;
	    case 270:						// 270 degrees
		cBig->y = Big_ygap * (double)col + BigHor;
		break;
	    }
	*oldcol = col;
	}

    if (user_data(hwnd) == -1)
	return(-1);
    *color = DoBigFract(hwnd, row, col);

    reset_period = 0;

    if (*color >= threshold)
	*color = threshold;
/*
    else if (logval && logflag == TRUE)
	color = (BYTE) (*(logtable + color));
*/

    if (calcmode == 'B')
	{
	if (*color >= colours)	/* don't use color 0 unless from inside */
	    if (colours < 16)
		*color &= *andcolor;
	    else
		*color = ((*color - 1) % *andcolor) + 1;  /* skip color zero */
	}

    if ((type == SPECIALNEWT || type == MATEIN) && special != 15)  // split colours
	{
	if (phaseflag == 1)				// second phase
	    *color += special;
	else if (phaseflag == 2)			// third phase
	    *color += (special << 1);
	}						// default first phase

    if (_3dflag)
	    projection(col, row, *color);
	else if (pairflag)
	    do_stereo_pairs(col, row, *color);
    else
	plot((WORD)col, (WORD)row, *color);
    return(*color);
    }

