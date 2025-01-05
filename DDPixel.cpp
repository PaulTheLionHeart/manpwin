/*
    DDPixel.cpp a module for the per pixel calculations of fractals using Double doubles. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	<stdio.h>
#include	"DDComplex.h"
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
	Convert Bignum variables to double doubles
**************************************************************************/

int    CPixel::ConvertBignumVariables2DD()
    {
    if (BigDouble2DD(&zDD.x, &zBig->x) < 0) return -1;
    if (BigDouble2DD(&zDD.y, &zBig->y) < 0) return -1;
    if (BigDouble2DD(&cDD.x, &cBig->x) < 0) return -1;
    if (BigDouble2DD(&cDD.y, &cBig->y) < 0) return -1;
    if (BigDouble2DD(&qDD.x, &qBig->x) < 0) return -1;
    if (BigDouble2DD(&qDD.y, &qBig->y) < 0) return -1;
    if (BigDouble2DD(&DDCloseEnough, &BigCloseEnough) < 0) return -1;
    return 0;
    }

/**************************************************************************
	Filter
**************************************************************************/

int	CPixel::DoDDFilter(int method, int hooper, DDComplex *z)

    {
    double	magnitude = 0.0;
    dd_real	temp, BioMorphTest;
    CPotential	Pot;

    if (colours == 256 && decomp > 0)
	*iteration = DDDecomposition(z->x, z->y);
    else if (logval)
	*iteration = (BYTE) (*(logtable + (*iteration % MAXTHRESHOLD)));
    else if (biomorph >= 0)
	{
	rqlim2 = sqrt(rqlim);
//	BioMorphTest = rqlim2;
	BioMorphTest = 0.5;
	if (fabs(z->x) < BioMorphTest || fabs(z->y) < BioMorphTest)
	    *iteration = biomorph;
	}
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
		*iteration += (long)to_double(z->x) + 7;
		break;
	    case IMAG:	    			// "imag"
		*iteration += (long)to_double(z->y) + 7;
		break;
	    case MULT:				// "mult"
		if (z->y != 0.0)
		    *iteration = (long)((double)*iteration * to_double((z->x/z->y)));
		break;
	    case SUM:				// "sum"
		*iteration += (long)to_double((z->x + z->y));
		break;
	    case ATAN:				// "atan"
		*iteration = (long)to_double(abs(atan2(z->y, z->x)*180.0/PI));
		break;
	    case POTENTIAL:
		magnitude = to_double((sqr(z->x) + sqr(z->y)));
		*iteration = Pot.potential(magnitude, *iteration, threshold, TrueCol, colors, potparam);
		break;
	    case PERT1:
		if (*iteration != threshold)
		    *iteration = (int)((*iteration - log2(log2(z->CSumSqr()))) * 5) % 256;				//Get the index of the color array that we are going to read from. 
		break;
	    case PERT2:
		if (*iteration != threshold)
		    *iteration = (int)(*iteration - (log(0.5*(z->CSumSqr())) - log(0.5*log(256))) / log(2)) % 256;	//Get the index of the color array that we are going to read from. 
		break;
	    }

	// eliminate negative colors & wrap arounds
	if (*iteration < 0)
	    *iteration = 0;
	if (*iteration > threshold && decomp <= threshold)		// for small thresholds, we can still have higher decomp levels
	    *iteration = threshold;
	}
    return 0;
    }

/**************************************************************************
	Get Float Iteration per pixel
**************************************************************************/

void	CPixel::CalcDDFloatIteration(double error, double *wpixels, int row, int col, DDComplex z, DDComplex OldZ, DDComplex OlderZ, double FloatIteration, WORD type, int subtype, WORD *degree, BYTE SpecialFlag, WORD special, int width)
    {
    double	log_zn, nu, t;
    int		SlopeDegree, BailoutType;
    dd_real	DDTemp, DDTemp1, LogA, LogB;

    if (FloatIteration < threshold)
	{
	DWORD	index;
	DDComplex	a, b;

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
		DDTemp = zDD.x * zDD.x + zDD.y * zDD.y;
//		BigTemp1 = BigTemp.BigLog();
//		log_zn = mpfr_get_d(BigTemp1.x, MPFR_RNDN) / SlopeDegree;
		double  t = to_double(DDTemp);
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
		b = zDD - OldZ;
		DDTemp = a.x * a.x + a.y * a.y;
		LogA = log(DDTemp);
		log_zn = log(error) - to_double(LogA);
//		log_zn = log(error) - log(a.x * a.x + a.y * a.y);				// convergence method 1 (page 25 Fractal-Zoomer Algorithms.docx)
		DDTemp = b.x * b.x + b.y * b.y;
		LogB = log(DDTemp);
		DDTemp1 = LogB - LogA;
		t = to_double(DDTemp1);
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
		DDTemp = zDD.x * zDD.x + zDD.y * zDD.y;
//		BigTemp1 = BigTemp.BigLog();
//		log_zn = mpfr_get_d(BigTemp1.x, MPFR_RNDN) / SlopeDegree;
		double  t = to_double(DDTemp);
		log_zn = log(t) / SlopeDegree;
		nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		FloatIteration = FloatIteration + 1 - nu;
/*
		BigTemp = zBig->x * zBig->x + zBig->y * zBig->y;
		BigTemp1 = BigTemp.BigLog();
		log_zn = mpfr_get_d(BigTemp1.x, MPFR_RNDN) / SlopeDegree;
		nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		FloatIteration = FloatIteration + 1 - nu;
*/
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

long	CPixel::DoDDFract(HWND hwnd, int row, int col)

    {
    long	real_iteration;			// actual count for orbit deletion
    BigComplex	bigTemp;
    DDComplex	DDTemp;
    Complex	tempComplex;
    dd_real	magnitude;
    dd_real	min_orbit;			// orbit value closest to origin
    long	min_index;			// iteration of min_orbit
//    double	tantable[16];			// used for Star Trails
    dd_real	close = 0.01;

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
    DDComplex	DDSaved = 0.0;
    int		hooper = 0;

    if (period_level == 0)
	oldcolour = 32767; 			// don't check periodicity at all
    else //if (reset_period)
	oldcolour = 240;			// don't check periodicity 1st n iterations

    SpecialFlag = FALSE;			// no special colour yet. Use for decomp etc
    savedand = 1;				// begin checking every other cycle
    savedincr = 1;				// start checking the very first time

    *iteration = 0L;
    real_iteration = 0;	
    phaseflag = 0;				// assume all type 5, 9 fractals same colour

    if (OutsideMethod >= TIERAZONFILTERS)
	{
	DDTemp = qDD;
	tempComplex.x = to_double(DDTemp.x);
	tempComplex.y = to_double(DDTemp.y);
	TZfilter->InitFilter(OutsideMethod, threshold, dStrands, nFDOption, UseCurrentPalette);		// initialise the constants used by Tierazon fractals
	TZfilter->LoadFilterQ(tempComplex);
	}

    if (juliaflag)
	{
	qBig->x = j.x;
	qBig->y = j.y;
	*zBig = (invert) ? BigInvertz2(*cBig) : *cBig;
	}
    else
	{
	*qBig = (invert) ? BigInvertz2(*cBig) : *cBig;
	*zBig = 0.0;
	}

    if (ConvertBignumVariables2DD() < 0)
	return (-1L);
    
    DDInitFunctions(type, &zDD, &qDD);
    if (InsideMethod == BOF60 || InsideMethod == BOF61)
	{
	magnitude = 0.0;
	min_orbit = 100000.0;
	}

    if (InsideMethod == POTENTIAL)
	BigBailout = potparam[2];

    FloatIteration = 0.0;
    for EVER
	{
	if (calcmode == 'F')
	    {
	    DDOlderZ = DDOldZ;
	    DDOldZ = zDD;
	    }
	if (FloatIteration >= threshold)
	    break;
	(*iteration)++;
	FloatIteration++;

	result = run_DD_fractal(type, &zDD, &qDD, &SpecialFlag, iteration);
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
	if (InsideMethod == EPSCROSS)
	    {
	    hooper = 0;
	    if (abs(zDD.x) < close)
		{
		hooper = 1; // close to y axis
		break;
		}
	    else if (abs(zDD.y) < close)
		{
		hooper = 2; // close to x axis
		break;
		}
	    }
	else if (InsideMethod == BOF60 || InsideMethod == BOF61)
	    {
	    magnitude = zDD.CSumSqr();
	    if (magnitude < min_orbit)
		{
		min_orbit = magnitude;
		min_index = (long)*iteration + 1L;
		}
	    }

	else if (OutsideMethod >= TIERAZONFILTERS)
	    {
	    DDTemp = zDD;
	    tempComplex.x = to_double(DDTemp.x);
	    tempComplex.y = to_double(DDTemp.y);
	    TZfilter->DoTierazonFilter(tempComplex, iteration);
	    }

	if (*iteration > oldcolour)			// check periodicity
	    {
	    if ((*iteration & savedand) == 0)		// time to save a new value
		{
		DDSaved = zDD;
		if (--savedincr == 0)			// time to lengthen the periodicity?
		    {
		    savedand = (savedand << 1) + 1;	// longer periodicity
		    savedincr = 4;			// restart counter
		    }
		}
	    else		     			// check against an old save
		{
		dd_real   xAbs = DDSaved.x - zDD.x;
		dd_real   yAbs = DDSaved.y - zDD.y;

		if (abs(xAbs) < DDCloseEnough)
		    if (abs(yAbs) < DDCloseEnough)
			*iteration = threshold;
		}
	    }
	}

    if (*iteration >= threshold && period_level)
	oldcolour = 0;			// check periodicity immediately next time
    else
	oldcolour = *iteration + 10;	// check when past this+10 next time

    if (OutsideMethod >= TIERAZONFILTERS)
	{
	DDComplex	DDTemp;
	Complex		tempComplex;

	DDTemp = zDD;
	tempComplex.x = to_double(DDTemp.x);
	tempComplex.y = to_double(DDTemp.y);
	TZfilter->EndTierazonFilter(tempComplex, iteration, TrueCol);
	return *iteration;
	}

    if (SpecialFlag)
	return(special);		// flag for special colour

    if (*iteration < threshold)
	{
//	DoDDFilter(InsideMethod, hooper);
	DoDDFilter(OutsideMethod, hooper, &zDD);
	}
    else
	{
	dd_real	t = sqrt(min_orbit) * 75.0;
	if (InsideMethod == BOF60)
	    *iteration = (int)to_double(t);
	else if (InsideMethod == BOF61)
	    *iteration = min_index;
	else if (InsideMethod == ZMAG)
	    *iteration = (int)((zDD.CSumSqr()) * (threshold >> 1) + 1);
	else
	    *iteration = threshold;
	if (*iteration < 1L)
	    *iteration = threshold;
	}

    if (type == NEWTON && subtype != 'N')
	return (*color);				// Newton root colour

    if ((type == SPECIALNEWT || type == MATEIN) && special != 0)  // split colours
	{
	if (phaseflag == 1)				// second phase
	    *iteration += special;
	else if (phaseflag == 2)			// third phase
	    *iteration += (special << 1);
	if (*iteration > threshold)
	    *iteration -= threshold;
	}						// default first phase

    if (calcmode == 'F')
	{
	SlopeError = 1.0e-9;
	CalcDDFloatIteration(SlopeError, wpixels, row, col, zDD, DDOldZ, DDOlderZ, FloatIteration, type, subtype, degree, SpecialFlag, special, width);
	}

    return(*iteration);
    }

#ifdef DOUBLEDOUBLE

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

#endif // DOUBLEDOUBLE

int	CPixel::BigDouble2DD(dd_real *out, BigDouble *in)
    {
    dd_real	x1, x2;
    BigDouble	t1, t2;
    double	y1, y2;

    y1 = in->BigDoubleToDouble();	// truncate to a double
    t1 = y1;
    t2 = *in - t1;			// subtract truncated bit to get the remainder
    y2 = t2.BigDoubleToDouble();	// remainder as a float
    x1 = y1;
    x2 = y2;
    *out = x1 + x2;
    if (isnan(out->x[0]) || isnan(out->x[1]))
	{
	*out = 0.0;
	return -1;
	}
    return 0;
    }

