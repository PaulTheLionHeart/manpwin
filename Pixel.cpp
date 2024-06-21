/*
    PIXEL.CPP a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" API and not in MFC for portability
*/

#include	<math.h>
#include	"manp.h"
#include	"colour.h"
#include	"Fract.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"filter.h"
#include	"complex.h"
#include	"dib.h"
#include	"Slope.h"
#include	"OscProcess.h"
#include	"pixel.h"
#include	"Potential.h"

#ifndef sqr
#define sqr(x) ((x)*(x))
#endif

#define OTHER	    0
#define FRACTINT    1
#define MANPWIN	    2

/**************************************************************************
	Initialise Pixel
**************************************************************************/

void	CPixel::InitPixel0(WORD typeIn, WORD specialIn, int subtypeIn, WORD *degreeIn, double rqlimIn, BOOL ExpandStarTrailColoursIn, BYTE SpecialFlagIn, int precisionIn, int biomorphIn, int InsideMethodIn, int OutsideMethodIn, 
		int RotationAngleIn, int xdotsIn, int ydotsIn, int nFDOptionIn)

    {
    type = typeIn;
    subtype = subtypeIn;
    special = specialIn;
    degree = degreeIn;
    rqlim = rqlimIn;
    SpecialFlag = SpecialFlagIn;
//    dem_delta = dem_deltaIn;
//    dem_width = dem_widthIn;
    biomorph = biomorphIn;
    InsideMethod = InsideMethodIn;
    OutsideMethod = OutsideMethodIn;
    nFDOption = nFDOptionIn;
    RotationAngle = RotationAngleIn;
    ExpandStarTrailColours = ExpandStarTrailColoursIn;
    xdots = xdotsIn;						// only the width of the strip
    ydots = ydotsIn;
    precision = precisionIn;
    }

void	CPixel::InitPixel1(CTZfilter *TZfilterIn, CTrueCol *TrueColIn, COscProcess *OscProcessIn, int period_levelIn, int distestIn, BOOL invertIn, BYTE phaseflagIn, double *wpixelsIn, BYTE juliaflagIn, double closenuffIn, BigDouble BigCloseEnoughIn, BYTE calcmodeIn)

    {
    TZfilter = TZfilterIn;
    TrueCol = TrueColIn;
    OscProcess = OscProcessIn;
    period_level = period_levelIn;
    distest = distestIn;
    invert = invertIn;
    phaseflag = phaseflagIn;
    wpixels = wpixelsIn;
    juliaflag = juliaflagIn;
    closenuff = closenuffIn;
    BigCloseEnough = BigCloseEnoughIn;
    calcmode = calcmodeIn;
    }

void	CPixel::InitPixel2(int CoordSystemIn, BOOL UseCurrentPaletteIn, int reset_periodIn, int colorsIn, double horIn, double vertIn, double mandel_widthIn, BigDouble BigHorIn, BigDouble BigVertIn, BigDouble BigWidthIn, double *yymaxIn, BigDouble *Big_yymaxIn)
    {
    CoordSystem = CoordSystemIn;
    UseCurrentPalette = UseCurrentPaletteIn;
    reset_period = reset_periodIn;
    colors = colorsIn;
    hor = horIn;
    vert = vertIn;
    mandel_width = mandel_widthIn;
    BigHor = BigHorIn;
    BigVert = BigVertIn;
    BigWidth = BigWidthIn;
    yymax = yymaxIn;
    Big_yymax = Big_yymaxIn;
    }

void	CPixel::InitPixel3(double dStrandsIn, Complex jIn, BYTE pairflagIn, long *andcolorIn, BYTE _3dflagIn, double xgapIn, double ygapIn, BigDouble Big_xgapIn, BigDouble Big_ygapIn, Complex *cIn, double ScreenRatioIn, WORD coloursIn, CFract *Fract, int BailoutTestTypeIn)
    {
//    FilterRGB = FilterRGBIn;
    dStrands = dStrandsIn;
    j = jIn;
    pairflag = pairflagIn;
    andcolor = andcolorIn;
    _3dflag = _3dflagIn;
    xgap = xgapIn;
    ygap = ygapIn;
    Big_xgap = Big_xgapIn;
    Big_ygap = Big_ygapIn;
    c = cIn;
    ScreenRatio = ScreenRatioIn;
    colours = coloursIn;
    Fn1Index = Fract->Fn1Index;
    Fn2Index = Fract->Fn2Index;
    BailoutTestType = BailoutTestTypeIn;
    }

void	CPixel::InitPixel4(BigComplex *cBigIn, Complex *qIn, Complex *zIn, BigComplex *qBigIn, BigComplex *zBigIn, long thresholdIn, BYTE BigNumFlagIn, long *colorIn, int logvalIn, long *iterationIn, double f_radiusIn, double f_xcenterIn, char *LyapSequenceIn)
    {
    cBig = cBigIn;
    q = qIn;
    z = zIn;
    qBig = qBigIn;
    zBig = zBigIn;
    threshold = thresholdIn;
    BigNumFlag = BigNumFlagIn;
    if (BigNumFlag)
	BigBailout = rqlim;
    color = colorIn;
    logval = logvalIn;
    iteration = iterationIn;
    f_radius = f_radiusIn;
    f_xcenter = f_xcenterIn;
    LyapSequence = LyapSequenceIn;
    }

void	CPixel::InitPixel5(double f_ycenterIn, int *symmetryIn, double paramIn[], double potparamIn[], int decompIn, BYTE *logtableIn, int *AutoStereo_valueIn, int widthIn, HWND hwndIn/*, struct workliststuff *worklistIn*/, CMatrix *MatIn, CBigMatrix *BigMatIn)
    {
    f_ycenter = f_ycenterIn;
    symmetry = symmetryIn;
    for (int i = 0; i < NUMPERTPARAM; i++)
	param[i] = paramIn[i];
    for (int i = 0; i < 3; i++)
	potparam[i] = potparamIn[i];
    decomp = decompIn;
    logtable = logtableIn;
    AutoStereo_value = AutoStereo_valueIn;
    width = widthIn;
    hwnd = hwndIn;
    Mat = MatIn;
    BigMat = BigMatIn;
    //    for (int i = 0; i < MAXCALCWORK; i++)
//	worklist[i] = &worklistIn[i];
    }

void	CPixel::InitPixel6(CDib *DibIn, int *PlotTypeIn, int *oldrowIn, int *oldcolIn, int *time_to_zoomIn, int *time_to_restartIn, int *time_to_reinitIn, int *time_to_quitIn, long fillcolorIn, long *andcolorIn, int *blockindexIn, int *totpassesIn, int *curpassIn)
    {
    int	flags = USEPALETTE + ((calcmode != 'F' && type != PERTURBATION && type != SLOPEFORWARDDIFF) ? USEWPIXELS : 0);
    Dib = DibIn; PlotType = PlotTypeIn; oldrow = oldrowIn; oldcol = oldcolIn; time_to_zoom = time_to_zoomIn; time_to_restart = time_to_restartIn; time_to_reinit = time_to_reinitIn; time_to_quit = time_to_quitIn; fillcolor = fillcolorIn; andcolor = andcolorIn; 
	blockindex = blockindexIn; totpasses = totpassesIn; curpass = curpassIn;
    Plot.InitPlot(threshold, TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib->BitsPerPixel, Dib, flags);
    }

/**************************************************************************
	Allow internal bignum variables to track current precision requirements
**************************************************************************/

void	CPixel::ManageBignumPrecision(int precision)
    {
    aBig.x.ChangePrecision(precision); aBig.y.ChangePrecision(precision);
    a2Big.x.ChangePrecision(precision); a2Big.y.ChangePrecision(precision);
    aa3Big.x.ChangePrecision(precision); aa3Big.y.ChangePrecision(precision);
    bBig.x.ChangePrecision(precision); bBig.y.ChangePrecision(precision);
    l2Big.x.ChangePrecision(precision); l2Big.y.ChangePrecision(precision);
    lm5Big.x.ChangePrecision(precision); lm5Big.y.ChangePrecision(precision);
    lp5Big.x.ChangePrecision(precision); lp5Big.y.ChangePrecision(precision);
    ozBig.x.ChangePrecision(precision); ozBig.y.ChangePrecision(precision);
    t2Big.x.ChangePrecision(precision); t2Big.y.ChangePrecision(precision);
    t3Big.x.ChangePrecision(precision); t3Big.y.ChangePrecision(precision);
    vBig.x.ChangePrecision(precision); vBig.y.ChangePrecision(precision);
    tempBig.x.ChangePrecision(precision); tempBig.y.ChangePrecision(precision);
    temp1Big.x.ChangePrecision(precision); temp1Big.y.ChangePrecision(precision);
    temp2Big.x.ChangePrecision(precision); temp2Big.y.ChangePrecision(precision);
    temp3Big.x.ChangePrecision(precision); temp3Big.y.ChangePrecision(precision);
    sqrBig.x.ChangePrecision(precision); sqrBig.y.ChangePrecision(precision);
    sqrsqrBig.x.ChangePrecision(precision); sqrsqrBig.y.ChangePrecision(precision);
    c1Big.x.ChangePrecision(precision); c1Big.y.ChangePrecision(precision);
    c2Big.x.ChangePrecision(precision); c2Big.y.ChangePrecision(precision);
    cbBig.x.ChangePrecision(precision); cbBig.y.ChangePrecision(precision);
    caa3Big.x.ChangePrecision(precision); caa3Big.y.ChangePrecision(precision);
    z1Big.x.ChangePrecision(precision); z1Big.y.ChangePrecision(precision);
    z2Big.x.ChangePrecision(precision); z2Big.y.ChangePrecision(precision);
    z3Big.x.ChangePrecision(precision); z3Big.y.ChangePrecision(precision);
    z4Big.x.ChangePrecision(precision); z4Big.y.ChangePrecision(precision);
    zdBig.x.ChangePrecision(precision); zdBig.y.ChangePrecision(precision);
    ztBig.x.ChangePrecision(precision); ztBig.y.ChangePrecision(precision);
    BigOldZ.x.ChangePrecision(precision); BigOldZ.y.ChangePrecision(precision);
    BigOlderZ.x.ChangePrecision(precision); BigOlderZ.y.ChangePrecision(precision);

    realimagBig.ChangePrecision(precision);
    RealImagSqrBig.ChangePrecision(precision);
    tBig.ChangePrecision(precision);
    BigBailout.ChangePrecision(precision);
    Big_xgap.ChangePrecision(precision);
    Big_ygap.ChangePrecision(precision);
    BigHor.ChangePrecision(precision);
    BigVert.ChangePrecision(precision);
    BigWidth.ChangePrecision(precision);
    }

/**************************************************************************
	Initialise fractal
**************************************************************************/

int	CPixel::init_fractal(Complex *z, Complex *q)

    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(InitFunctions(type, z, q));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(InitFractintFunctions(type, z, q));
    else if (fractalspecific[type].flags & TRIGINPIXEL)
	return(InitFractintTrigFunctions(type, z, q));
//    else if (fractalspecific[type].flags & OTHERFNINPIXEL)
//	return(InitOtherFunctions(type, z, q));
    else if (fractalspecific[type].per_pixel() < 0)
	return -1;
    return 0;
    }

/**************************************************************************
	Run fractal
**************************************************************************/

int	CPixel::run_fractal(Complex *z, Complex *q)

    {
    if (fractalspecific[type].flags & FUNCTIONINPIXEL)
	return(RunFunctions(type, z, q, &SpecialFlag, iteration));
    else if (fractalspecific[type].flags & FRACTINTINPIXEL)
	return(RunFractintFunctions(type, z, q, &SpecialFlag, iteration));
    else if (fractalspecific[type].flags & TRIGINPIXEL)
	return(RunFractintTrigFunctions(type, z, q, &SpecialFlag, iteration));
//    else if (fractalspecific[type].flags & OTHERFNINPIXEL)
//	return(RunOtherFunctions(type, z, q, &SpecialFlag, iteration));
    else
	return fractalspecific[type].calctype();
    }

/**************************************************************************
	Filter
**************************************************************************/

int	CPixel::DoFilter(int method, int hooper)

    {
    double	magnitude = 0.0;
    CPotential	Pot;

    if (colours == 256 && decomp > 0)
	*iteration = float_decomposition(z->x, z->y);
    else if (logval)
	*iteration = (BYTE) (*(logtable + (*iteration % MAXTHRESHOLD)));
    else if (biomorph >= 0)
	{
	rqlim2 = sqrt(rqlim);
	if (fabs(z->x) < rqlim2 || fabs(z->y) < rqlim2)
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
		*iteration += (long)z->x + 7;
		break;
	    case IMAG:	    			// "imag"
		*iteration += (long)z->y + 7;
		break;
	    case MULT:				// "mult"
		if (z->y)
		    *iteration = (long)((double)*iteration * (z->x/z->y));
		break;
	    case SUM:				// "sum"
		*iteration += (long)(z->x + z->y);
		break;
	    case ATAN:				// "atan"
		*iteration = (long)fabs(atan2(z->y, z->x)*180.0/PI);
		break;
	    case POTENTIAL:
		magnitude = sqr(z->x) + sqr(z->y);
		*iteration = Pot.potential(magnitude, *iteration, threshold, TrueCol, colors, potparam);
		break;
	    case PERT1:
		if (*iteration != threshold)
		    *iteration = (int)((*iteration - log2(log2(z->CSumSqr()))) * 5) % 256;				//Get the index of the color array that we are going to read from. 
	    case PERT2:
		if (*iteration != threshold)
		    *iteration = (int)(*iteration - (log(0.5*(z->CSumSqr())) - log(0.5*log(256))) / log(2)) % 256;	//Get the index of the color array that we are going to read from. 
		  
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

void	CPixel::CalcFloatIteration(double error, double *wpixels, int row, int col, Complex z, Complex OldZ, Complex OlderZ)
    {
    double	log_zn, nu, t;
    int		SlopeDegree, BailoutType;
    DWORD	index;

    if (FloatIteration < threshold)
	{
	Complex	a, b, root = { 1.0, 0.0 };

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
		log_zn = log(z.x * z.x + z.y * z.y) / SlopeDegree;
		nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		FloatIteration = FloatIteration + 1 - nu;
		break;

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
	    case CONVERGING:
		a = OldZ - OlderZ;
		b = z - OldZ;
		log_zn = log(error) - log(a.x * a.x + a.y * a.y);				// convergence method 1 (page 25 Fractal-Zoomer Algorithms.docx)
		t = log(b.x * b.x + b.y * b.y) - log(a.x * a.x + a.y * a.y);
		nu = log_zn / t;
		FloatIteration = FloatIteration + nu;
		break;
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
	    default:
		log_zn = log(z.x * z.x + z.y * z.y) / SlopeDegree;
		nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		FloatIteration = FloatIteration + 1 - nu;
		break;
	    }
	}
    if ((type == SPECIALNEWT || type == MATEIN) && special != 0)  // split colours
	{
	if (phaseflag == 1)				// second phase
	    FloatIteration += special;
	else if (phaseflag == 2)			// third phase
	    FloatIteration += (special << 1);
	}						// default first phase
    if ((long)FloatIteration >= threshold)
	FloatIteration = (double)threshold;
    else if (SpecialFlag)
	FloatIteration = (double)special;

    index = ((DWORD)row * (DWORD)width) + (DWORD)col;
    if (col >= 0 && col < xdots - 1 && row >= 0 && row < ydots - 1)
	*(wpixels + index) = FloatIteration;
    }

/**************************************************************************
	Run fractal engine per pixel
**************************************************************************/

//extern	CSlope	Slope[];



long	CPixel::dofract(HWND hwnd, int row, int col)

    {
    int	real_iteration;				// actual count for orbit deletion

    Complex	saved;
    int		savedand, savedincr;		// for periodicity checking
    int		result;
    int		hooper = 0;
    double	close;
    double	TempZ;
    double	magnitude;
    double	min_orbit;			// orbit value closest to origin
    long	min_index;			// iteration of min_orbit
    double	tantable[16];			// used for Star Trails

    if (InsideMethod == STARTRAIL)
       {
       int	i;
       for(i = 0; i < 16; i++)
	    tantable[i] = 0.0;
       }
    close = 0.01;
    magnitude = 0.0;
    min_orbit = 100000.0;

    if (period_level == 0 || InsideMethod == ZMAG || InsideMethod == STARTRAIL)
	oldcolour = 32767; 			// don't check periodicity at all
    else //if (reset_period)
	oldcolour = 240;			// don't check periodicity 1st n iterations

    SpecialFlag = FALSE;			// no special colour yet. Use for decomp etc 
    saved = 0;
    savedand = 1;				// begin checking every other cycle
    savedincr = 1;				// start checking the very first time 

    *iteration = 0L;
    FloatIteration = 0.0;
    real_iteration = 0;
    phaseflag = 0;			// assume all type 5, 9 fractals same colour 
    if (InsideMethod >= TIERAZONFILTERS)
	{
	TZfilter->InitFilter(InsideMethod, threshold, dStrands, nFDOption, UseCurrentPalette);		// initialise the constants used by Tierazon fractals
	TZfilter->LoadFilterQ(*q);
	}

#ifdef	DEBUG
sprintf(ProcessType, "Filter stuff [%c],[%d][%d]", ((TierazonFilter[FilterType].rgb) ? 'T' : 'F'), method, FilterType);
SetWindowText (hwnd, ProcessType);
#endif

    if (juliaflag)
	{
	*q = j;
	*z = (invert) ? invertz2(*c) : *c;
	}
    else
	{
	*q = (invert) ? invertz2(*c) : *c;
	*z = 0;
	}

    if (init_fractal(z, q) < 0)
	return(BLUE);

    if (distest)
	{
//	rqlim = rqlim_save;		    /* start with regular bailout */
//	if (distest != 1 || colors == 2)    /* not doing regular outside colors */
	if (distest != 1)		    /* not doing regular outside colors */
	    if (rqlim < DEM_BAILOUT)	    /* so go straight for dem bailout */
		rqlim = DEM_BAILOUT;
	deriv.x = 1;
	deriv.y = 0;
	magnitude = 0.0;
	dem_color = -1;
	}

    if (InsideMethod == BOF60 || InsideMethod == BOF61)
	{
	magnitude = 0.0;
	min_orbit = 100000.0;
	}

    if (InsideMethod == POTENTIAL)
	rqlim = (potparam[2] > 0) ? potparam[2] : 4.0;

    FloatIteration = 0.0;
    for EVER
	{
	if (calcmode == 'F')
	    {
	    OlderZ = OldZ;
	    OldZ = *z;
	    }   

	if (FloatIteration >= threshold)
//    if (iteration >= threshold)
	    break;
	(*iteration)++;
	FloatIteration++;
	if (distest)
	    {
	    double ftemp;
	 // Distance estimator for points near Mandelbrot set
	 // Original code by Phil Wilson, hacked around by PB
	 // Algorithms from Peitgen & Saupe, Science of Fractal Images, p.198
	    ftemp = 2 * (z->x * deriv.x - z->y * deriv.y) + 1;
	    deriv.y = 2 * (z->y * deriv.x + z->x * deriv.y);
//	    ftemp = 2 * (old.x * deriv.x - old.y * deriv.y) + 1;
//	    deriv.y = 2 * (old.y * deriv.x + old.x * deriv.y);
	    deriv.x = ftemp;
//	    if (sqr(deriv.x)+sqr(deriv.y) > dem_toobig)
//		break;
	 // if above exit taken, the later test vs dem_delta will place this point on the boundary, because mag(old)<bailout just now
	    result = run_fractal(z, q);
	    if (result == -1)				// divide by zero error
		return(BLUE);				// division by zero (Was Blue)
	    if (result)
		{
		if (dem_color < 0)			// note "regular" color for later
		    {
		    dem_color = *iteration;
//		    dem_new = new;
		    dem_new = *z;
		    }

		if (rqlim >= DEM_BAILOUT		// exit if past real bailout
		    || magnitude >= (rqlim = DEM_BAILOUT) // reset to real bailout
		    || magnitude == 0)			// exit if type doesn't "floatbailout"
		    break;
//		old = new;				// carry on till past real bailout
		}
	    }
	else						// the usual case
	    {
	    result = run_fractal(z, q);
	    if (result == -1)				// divide by zero error
		return(BLUE);				// division by zero (Was Blue)
	    else if (result == 1)			// escape time
		break;
	    }

	TempPt = *z;
	OscProcess->ChangeCoordSystem(&z->x, &z->y, &TempZ, TempPt.x, TempPt.y, (double)*iteration, CoordSystem);
							// result = 0 so continue iteration
	if (InsideMethod == STARTRAIL)
	    {
	    if(0 < *iteration && *iteration < 16)
		tantable[*iteration - 1] = z->y/(z->x + 0.000001);
	    }
	else if (InsideMethod == EPSCROSS)
	    {
	    hooper = 0;
	    if (fabs(z->x) < close)
		{
		hooper = 1; // close to y axis
		break;
		}
	    else if (fabs(z->y) < close)
		{
		hooper = 2; // close to x axis
		break;
		}
	    }
	else if (InsideMethod == BOF60 || InsideMethod == BOF61)
	    {
	    magnitude = z->CSumSqr();
	    if (magnitude < min_orbit)
		{
		min_orbit = magnitude;
		min_index = *iteration + 1L;
		}
	    }
	else if (InsideMethod >= TIERAZONFILTERS)
	    TZfilter->DoTierazonFilter(*z, iteration);

	if (*iteration > oldcolour)			// check periodicity
	    {
	    if ((*iteration & savedand) == 0)		// time to save a new value
		{
		saved = *z;
		if (--savedincr == 0)			// time to lengthen the periodicity?
		    {
		    savedand = (savedand << 1) + 1;	// longer periodicity
		    savedincr = 4;			// restart counter
		    }
		}
	    else		     			// check against an old save
		{
		if (fabs(saved.x - z->x) < closenuff)
		    if (fabs(saved.y - z->y) < closenuff)
			*iteration = threshold;
		}
      	    }
	}

    if (*iteration >= threshold && period_level)
	oldcolour = 0;		// check periodicity immediately next time
    else
	oldcolour = *iteration + 10;	// check when past this+10 next time

//    if (juliaflag && ShowOrbits)
//	plot_orbits(special, NUM_ORBITS);

    if (InsideMethod >= TIERAZONFILTERS)
	{
	TZfilter->EndTierazonFilter(*z, iteration, TrueCol);
	return *iteration;
	}

    else if (InsideMethod == STARTRAIL)
	{
	int i;
	double diff;
	*iteration = 0;
	for(i=1; i < 16; i++)
	    {
	    diff = tantable[0] - tantable[i];
	    if(fabs(diff) < 0.05)
		{
		*iteration = (ExpandStarTrailColours) ? (int)((long)i * (long) threshold / 16L) : i;
		break;
		}
	    }
	}

    if (SpecialFlag)
	*iteration = special;		// flag for special colour

    if (distest)
	{
	double	dist, temp;

	if (*iteration >= threshold)
	    return threshold;

	dist = sqr(z->x) + sqr(z->y);
	temp = log(dist);
	dist = dist * sqr(temp) / (sqr(deriv.x) + sqr(deriv.y));
	if (dist < dem_delta)		/* point is on the edge */
	    {
	    if (distest > 0)
		{
		*iteration = threshold;
//		return threshold + 1;
//		goto plot_pixel;	/* no further adjustments apply */
		}
	    else
		{
//		goto plot_inside;	/* show it as an inside point */
		*iteration = 0 - distest;	/* show boundary as specified color */
		if (*iteration > threshold)
		    *iteration = threshold;
//		goto plot_pixel;		/* no further adjustments apply */
		}
	    }
//	if (colors == 2)
//	    {
//	    color = !inside;		/* the only useful distest 2 color use */
//	    goto plot_pixel;		/* no further adjustments apply */
//	    }
	if (distest > 1)		/* pick color based on distance */
	    {
	    *iteration = (long)sqrt(dist / dem_width + 1);
	    *iteration &= INT_MAX;		/* oops - color can be negative */
//	    return iteration;		/* no further adjustments apply */
//	    goto plot_pixel;		/* no further adjustments apply */
	    }
//	iteration = dem_color;		/* use pixel's "regular" color */
//	new = dem_new;
//	z = dem_new;
	}

    if (*iteration < threshold)
	{
	DoFilter(InsideMethod, hooper);
	DoFilter(OutsideMethod, hooper);
	}
    else
	{
	if (InsideMethod == BOF60)
	    *iteration = (int)(sqrt(min_orbit) * 75.0);
	else if (InsideMethod == BOF61)
	    *iteration = min_index;
	else if (InsideMethod == ZMAG)
	    *iteration = (int)((z->CSumSqr()) * (threshold >> 1) + 1);
	else
	    *iteration = threshold;
	}

//    plot_pixel:

//    if (type == RATIONALMAP)
//	return(iteration);
    if ((type == NEWTON || type == NEWTBASIN) && subtype != 'N')
	*iteration = *color;						// Newton root colour

    if ((type == SPECIALNEWT || type == MATEIN) && special != 0)  // split colours
	{
	if (phaseflag == 1)				// second phase
	    *iteration += special;
	else if (phaseflag == 2)			// third phase
	    *iteration += (special << 1);
	}						// default first phase

    if (calcmode == 'F')
	{
	SlopeError = 1.0e-9;
	CalcFloatIteration(SlopeError, wpixels, row, col, *z, OldZ, OlderZ);
	}
    return(*iteration);
    }

    /**************************************************************************
	    Invert fractal
    **************************************************************************/

    Complex	CPixel::invertz2(Complex  & Cmplx1)

	{
	Complex	temp;
	double	tempsqrx;

	temp.x = Cmplx1.x;
	temp.y = Cmplx1.y;
	temp.x -= f_xcenter; temp.y -= f_ycenter;	// Normalize values to center of circle

	tempsqrx = sqr(temp.x) + sqr(temp.y);	// Get old radius
	if (fabs(tempsqrx) > FLT_MIN)
	    tempsqrx = f_radius / tempsqrx;
	else
	    tempsqrx = FLT_MAX;			// a big number, but not TOO big
	temp.x *= tempsqrx;
	temp.y *= tempsqrx;				// Perform inversion
	temp.x += f_xcenter;
	temp.y += f_ycenter;			// Renormalize
	return  temp;
	}

    /************************************************************************
	    Calculate Fractal using a "standard" mode
    ************************************************************************/

    long	CPixel::calc_frac(HWND hwnd, int row, int col, int user_data(HWND hwnd))

	{
	if (BigNumFlag)
	    return (BigCalcFrac(hwnd, row, col, user_data));

	if (NonStandardFractal)					// does fractal use standard plotting mode?
	    return(-1);

	if (pairflag)						// half size screens: only do every second row / col
	    if (row % pairflag || col % pairflag)
		if (row != (int)ydots - 1)				// must trigger for last line
		    return(threshold);
	if (RotationAngle == 0 || RotationAngle == 90 || RotationAngle == 180 || RotationAngle == 270)		// save calcs in rotating, just remap
	    {
	    if (row != *oldrow)
		{
		if (pairflag && row)					// draw row for right hand image
		    draw_right_image((short)(*oldrow));			// PHD to fix
		switch (RotationAngle)
		    {
		    case NORMAL:						// normal
			c->y = *yymax - row * ygap;
			break;
		    case 90:						// 90 degrees
			c->x = *yymax - row * xgap;
			break;
		    case 180:						// 180 degrees
			c->y = -(*yymax - row * ygap);
			break;
		    case 270:						// 270 degrees
			c->x = -(*yymax - row * xgap);
			break;
		    }
		*oldrow = row;
		}
	    if (col != *oldcol)
		{
		switch (RotationAngle)
		    {
		    case NORMAL:						// normal
			c->x = col * xgap + hor;
			break;
		    case 90:						// 90 degrees
			c->y = col * ygap + hor;
			break;
		    case 180:						// 180 degrees
			c->x = -(col * xgap + hor);
			break;
		    case 270:						// 270 degrees
			c->y = -(col * ygap + hor);
			break;
		    }

		*oldcol = col;
		}
	    }
	else
	    {
	    double  zero = 0.0;
	    Mat->DoTransformation(&c->x, &c->y, &zero, col * xgap + hor, *yymax - row * ygap, 0.0);
	    }
	if (user_data(hwnd) == -1)
	    return(-1);
	*color = (type == CIRCLESQ || type == FPCIRCLE) ? ((int)(floor(c->CSumSqr())) & 0x00ff) : dofract(hwnd, row, col);
	reset_period = 0;

	if (*color > threshold && decomp <= threshold)		// for small thresholds, we can still have higher decomp levels
	    *color = threshold;
	if (calcmode == 'B')
	    {
	    if (*color >= colours)				// don't use color 0 unless from inside
		if (colours < 16)
		    *color &= *andcolor;
		else
		    *color = ((*color - 1) % *andcolor) + 1;  // skip color zero 
	    }
/*
	if ((type == SPECIALNEWT || type == MATEIN) && special != 0)  // split colours
	    {
	    if (phaseflag == 1)				// second phase
		*color += special;
	    else if (phaseflag == 2)			// third phase
		*color += (special << 1);
	    }						// default first phase
*/

    
	if (_3dflag)
	    projection(col, row, *color);
	else if (pairflag)
	    do_stereo_pairs(col, row, *color);
	else
	    plot((WORD)col, (WORD)row, *color);
	return(*color);
	}

/**************************************************************************
	Plot orbits
**************************************************************************/

void	CPixel::plot_orbits(RGBTRIPLE colour, int count)
    {
    int	    i, r, c;

    if (colour.rgbtBlue == 0 && colour.rgbtGreen == 0 && colour.rgbtRed == 0)
	return;										// nothing to do
    *z = 0;
    for (i = 0; i < count; i++)
	{
	run_fractal(z, q);
	r = ydots - (int)((z->y - vert) / ygap);
	c = (int)((z->x - hor) / xgap);
	Plot.OutRGBpoint(c, r, colour);
	}
    }

/**************************************************************************
	Init Distance Estimation
**************************************************************************/

void	CPixel::InitDistEst(double *xxmin, double *xxmax, double *yymin, double *yymax, double *xx3rd, double *yy3rd, int *distestwidth, int distest)
    {
    double	ftemp;
//    double	dem_delta, dem_width;	// distance estimator variables 
//    double	dem_toobig;
//    int		/*distest = 0, */distestwidth = 71;
    double	delxx, delxx2, delyy, delyy2;

    *xxmin = hor;
    *xxmax = hor + mandel_width * ScreenRatio;
    *yymin = vert;
    *yymax = vert + mandel_width;

    *xx3rd = *xxmin;
    *yy3rd = *yymin;

    delxx = (*xxmax - *xx3rd) / (xdots - 1); // calculate stepsizes
    delyy = (*yymax - *yy3rd) / (ydots - 1);
    delxx2 = (*xx3rd - *xxmin) / (ydots - 1);
    delyy2 = (*yy3rd - *yymin) / (xdots - 1);

    dem_delta = sqr(delxx) + sqr(delyy2);
    if ((ftemp = sqr(delyy) + sqr(delxx2)) > dem_delta)
	dem_delta = ftemp;
    if (*distestwidth == 0)
	*distestwidth = 71;
    ftemp = *distestwidth;
    dem_delta *= sqr(ftemp) / 10000;		// multiply by thickness desired
    dem_width = (sqrt(sqr(*xxmax - *xxmin) + sqr(*xx3rd - *xxmin)) * ydots / xdots + sqrt(sqr(*yymax - *yymin) + sqr(*yy3rd - *yymin))) / distest;
    }

/**************************************************************************
    General multi-threaded escape-time engine
**************************************************************************/

//extern	void	DoCaption(HWND, char *);

int	CPixel::RunThread(HWND hwnd, int i, HANDLE ghMutex, int StripWidth, CSlope Slope, CFract *Fractal, BigDouble *Big_xxmin, BigDouble *Big_xxmax, BigDouble *Big_yymin, double *xxmin, double *xxmax, double *yymin, int user_data(HWND hwnd))
    {
    num_worklist = 0;

    if (!num_worklist)				// no worklist from saved file
	{						// default setup a new worklist
	iystart = ixstart = yystart = xxstart = yybegin = 0;
	iystop = yystop = ydots - 1;
	ixstop = xxstop = xdots - 1;
	num_worklist = 1;
	workpass = 0;
	worklist[0].xxstart = 0;
	worklist[0].yystart = worklist[0].yybegin = 0;
	worklist[0].xxstop = xdots - 1;
	worklist[0].yystop = ydots - 1;
	worklist[0].pass = worklist[0].sym = 0;
	}

    while (num_worklist > 0)
	{						// pull top entry off worklist
	ixstart = xxstart = worklist[0].xxstart;
	ixstop = xxstop = worklist[0].xxstop;
	iystart = yystart = worklist[0].yystart;
	iystop = yystop = worklist[0].yystop;
	yybegin = worklist[0].yybegin;
	workpass = worklist[0].pass;
	worksym = worklist[0].sym;
	--num_worklist;
	for (i = 0; i < num_worklist; ++i)
	    worklist[i] = worklist[i + 1];

	FindSymmetry(_3dflag, decomp, pairflag, InsideMethod, invert, CoordSystem, param, *degree, type, subtype, calcmode, RotationAngle, Fractal, hor, vert, mandel_width, BigNumFlag, Big_xxmin, Big_xxmax, Big_yymin, Big_yymax, BigHor, BigVert, BigWidth, ScreenRatio,
	    xxmin, xxmax, yymin, yymax, special, juliaflag);
	if (pairflag)
	    init_stereo_pairs(pairflag, AutoStereo_value);			// init stereo pair parameters
	// okay, we have to get the globals into the Pixel object somehow

	if (_3dflag || pairflag)
	    calcmode = '1';				// 3D can only be done on single pass
	if (calcmode == 'F')
	    if (rqlim < 1000.0 && type != KLEINIAN)
		rqlim = rqlim = 1000.0;	// short term fix?
	if (StandardCalculationMode(hwnd, Slope, user_data) < 0)
	    return -1;
	if (*time_to_quit)				// exit cleanly
	    return -1;
	}
    return 0;
    }