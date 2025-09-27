/****************************************************
    PERTENGINE.CPP a module to explore Perturbation
    Thanks to Claude Heiland-Allen https://fractalforums.org/programming/11/perturbation-code-for-cubic-and-higher-order-polynomials/2783
    and Shirom Makkad https://github.com/ShiromMakkad/MandelbrotPerturbation
    for their inspiration and methods of calculating perturbation fractals
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include <windows.h>
#include <time.h>
#include "Big.h"
#include "Dib.h"
#include "filter.h"
#include "Manp.h"
#include "Manpwin.h"
#include "mpfr.h"
#include "Complex.h"
#include "PertEngine.h"
#include "Slope.h"
#include "Potential.h"

//////////////////////////////////////////////////////////////////////
// Initialisation
//////////////////////////////////////////////////////////////////////

int CPerturbation::initialiseCalculateFrame(CDib *DibIn, CSlope *Slope, int WidthIn, int HeightIn, int threshold, BigDouble xZoomPointin, BigDouble yZoomPointin, BigDouble BigWidthIn, int decimals, CTZfilter *TZfilter,
		HWND hwndIn, int ThreadIn, double *wpixelsIn, double paramIn[], double potparamIn[], int PaletteShiftIn, int *PlotTypeIn, int SlopeTypeIn, double lightDirectionDegreesIn, double bumpMappingDepthIn, 
		double bumpMappingStrengthIn, int PaletteStartIn, double LightHeightIn, int PertColourMethodIn,	int PalOffsetIn, double IterDivIn, bool EnableApproximationIn)
    {
    Complex q;
    int	    i;

    int bitcount = decimals * SAFETYMARGIN;
    if (bitcount < 30)
	bitcount = 30;
    if (bitcount > SIZEOF_BF_VARS - 10)
	bitcount = SIZEOF_BF_VARS - 10;
    precision = decimals - PRECISION_FACTOR;

    mpfr_set_default_prec(bitcount);
    mpfr_init2(xZoomPt, bitcount);		// set to new precision
    mpfr_init2(yZoomPt, bitcount);
    mpfr_init2(BigWidth.x, bitcount);
    //    mpfr_init(xZoomPt);
//    mpfr_init(yZoomPt);
    mpfr_set(xZoomPt, xZoomPointin.x, MPFR_RNDN);
    mpfr_set(yZoomPt, yZoomPointin.x, MPFR_RNDN);

    width = WidthIn;
    height = HeightIn;
    MaxIteration = threshold;
    BigWidth = BigWidthIn;
    EnableApproximation = EnableApproximationIn;
    if (EnableApproximation)
	{
	BigWidth = BigWidth.BigX2();	// BLA runs at twice the radius as normal perturbation
	}
    ZoomRadius = mpfr_get_d(BigWidth.x, MPFR_RNDN);
    BigZoomRadius = BigWidth;
    OutsideMethod = TZfilter->method;
    hwnd = hwndIn;
    thread = ThreadIn;
    ReferenceNumber = 0;
    wpixels = wpixelsIn;
    Dib = DibIn;
    PaletteShift = PaletteShiftIn;
    for (i = 0; i < NUMPERTPARAM; i++)
	param[i] = paramIn[i];
    for (i = 0; i < 3; i++)
	potparam[i] = potparamIn[i];
    PlotType = PlotTypeIn;
    SlopeType = SlopeTypeIn;
    lightDirectionDegrees = lightDirectionDegreesIn;
    bumpMappingDepth = bumpMappingDepthIn;
    bumpMappingStrength = bumpMappingStrengthIn;
    PaletteStart = PaletteStartIn;
    LightHeight = LightHeightIn;
    PertColourMethod = PertColourMethodIn;
    PalOffset = PalOffsetIn;
    IterDiv = IterDivIn;

    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Full frame calculation
//////////////////////////////////////////////////////////////////////

int CPerturbation::calculateOneFrame(double bailout, char* StatusBarInfo, int powerin, int InsideMethodIn, int OutsideMethodIn, int biomorphin, int subtypein, Complex rsrAin, bool rsrSignIn,
							int user_data(HWND hwnd), int xdotsIn, CTZfilter *TZfilter, CTrueCol *TrueCol, int *pPertProgress, bool *ThreadComplete, bool Multi, int delay, char *PertErrorMessage, HANDLE ghMutex)

    {
    int i, x, y;
    BigComplex	C, ReferenceCoordinate;
    BigComplex	BigDelta;
    Complex	delta;
    ExpComplex	ExpDeltaSub0;
    Complex	DeltaSub0;

    xdots = xdotsIn;
    referencePoints = 0;
    GlitchPointCount = 0L;
    RemainingPointCount = 0L;

    rsrA = rsrAin;
    rsrSign = rsrSignIn;
    power = powerin;
    if (power < 2)
	power = 2;
    if (power > MAXPOWER)
	power = MAXPOWER;
    InsideMethod = InsideMethodIn;
    OutsideMethod = OutsideMethodIn;
    subtype = subtypein;
    biomorph = biomorphin;

    // okay, we are going to do some complicated arithmetic calculations for the various ways of calculating fractals
    if (EnableApproximation)
	{
	if (fabs(ZoomRadius) > DBL_MIN && precision <= 300)
	    {
	    if (subtype == 0 || subtype == 1)
		ArithType = BLA_DOUBLE;
	    else
		ArithType = BLA_DBL_UNSUPPORTED;	// use Use BLA perturbation code but don't process via BLA table
	    }
	else
	    {
	    if ((subtype == 0 || subtype == 1))		// only for Mandelbrot and power so far
		ArithType = BLA_FLOATEXP;
	    else
		ArithType = BLA_EXP_UNSUPPORTED;	// use Use BLA perturbation code but don't process via BLA table
	    }
	}
    else
	{
	if (fabs(ZoomRadius) > DBL_MIN && precision <= 300)
	    {
	    ArithType = DOUBLE;
	    ScalingOffset = 0;
	    Scaling = 1.0;
	    }
	else if (precision < 600 && (subtype == 0 || (subtype == 1 && power == 3)))	// only for Mandelbrot and Cubic so far
	    {
	    if (EnableApproximation)
		ArithType = FLOATEXP;						// shifted double float not supported for BLA
	    else
		{
		ArithType = DOUBLESHIFT;					// use shifted double float
		ScalingOffset = 0;
		Scaling = 1.0;
		for (i = 300; i < precision; i++)
		    {
		    ScalingOffset++;
		    Scaling = Scaling * 0.1;
		    }
		ScalingSquared = Scaling * Scaling;
		}
	    }
	else
	    {
	    ArithType = FLOATEXP;							// width too small for double float
	    ScalingOffset = 0;
	    Scaling = 1;
	    }
	}
     // get memory for all point arrays
    pointsRemaining = new Point[width * height];
    if (pointsRemaining == NULL)
	{
	sprintf(PertErrorMessage, "Memory for pointsRemaining failed in thread %d", thread);
	return -2;
	}
    glitchPoints = new Point[width * height];
    if (glitchPoints == NULL)
	{
	if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
	sprintf(PertErrorMessage, "Memory for glitchPoints failed in thread %d", thread);
	return -2;
	}
    // get memory for Z and 2*Z arrays
    if (ArithType == BLA_FLOATEXP || ArithType == BLA_EXP_UNSUPPORTED || ArithType == FLOATEXP)
	{
	ExpXSubN = new ExpComplex[MaxIteration + 1];
	if (ExpXSubN == NULL)
	    {
	    if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
	    if (glitchPoints) { delete[] glitchPoints; glitchPoints = NULL; }
	    sprintf(PertErrorMessage, "Memory for XSubN failed in thread %d", thread);
	    return -2;
	    }
	ExpPerturbationToleranceCheck = new floatexp[MaxIteration + 1];
	if (ExpPerturbationToleranceCheck == NULL)
	    {
	    if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
	    if (glitchPoints) { delete[] glitchPoints; glitchPoints = NULL; }
	    if (ExpXSubN) { delete[] ExpXSubN; ExpXSubN = NULL; }
	    sprintf(PertErrorMessage, "Memory for XSubNTimes2 failed in thread %d", thread);
	    return -2;
	    }
	}
    else
	{
	// get memory for Perturbation Tolerance Check array
	PerturbationToleranceCheck = new double[MaxIteration * 2];
	if (PerturbationToleranceCheck == NULL)
	    {
	    if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
	    if (glitchPoints) { delete[] glitchPoints; glitchPoints = NULL; }
	    sprintf(PertErrorMessage, "Memory for PerturbationToleranceCheck failed in thread %d", thread);
	    return -2;
	    }
	// get memory for Z array
	XSubN = new Complex[MaxIteration + 1];
	if (XSubN == NULL)
	    {
	    if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
	    if (glitchPoints) { delete[] glitchPoints; glitchPoints = NULL; }
	    if (PerturbationToleranceCheck) { delete[] PerturbationToleranceCheck; PerturbationToleranceCheck = NULL; }
	    sprintf(PertErrorMessage, "Memory for XSubN failed in thread %d", thread);
	    return -2;
	    }
#ifdef	ALLOW_SIN
	if (subtype == 58)	// sin()
	    {
	    sinZp = new Complex[MaxIteration + 1];
	    if (sinZp == NULL)
		{
		if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
		if (glitchPoints) { delete[] glitchPoints; glitchPoints = NULL; }
		if (PerturbationToleranceCheck) { delete[] PerturbationToleranceCheck; PerturbationToleranceCheck = NULL; }
		if (XSubN) { delete[] XSubN; XSubN = NULL; }
		sprintf(PertErrorMessage, "Memory for XSubN failed in thread %d", thread);
		return -2;
		}
	    cosZp = new Complex[MaxIteration + 1];
	    if (cosZp == NULL)
		{
		if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
		if (glitchPoints) { delete[] glitchPoints; glitchPoints = NULL; }
		if (PerturbationToleranceCheck) { delete[] PerturbationToleranceCheck; PerturbationToleranceCheck = NULL; }
		if (XSubN) { delete[] XSubN; XSubN = NULL; }
		if (sinZp) { delete[] sinZp; sinZp = NULL; }
		sprintf(PertErrorMessage, "Memory for XSubNTimes2 failed in thread %d", thread);
		return -2;
		}
	    }
#endif
	}

    if (SlopeType == DERIVSLOPE)
	{
	Complex	    w;			// unit 2D vector
	w = { 0.0, 2.0 * lightDirectionDegrees * PI / 360.0 };
	v = w.CExp();			// setup derivative 2D light vector v
	}

    // calculate the pascal's triangle coefficients
    LoadPascal(PascalArray, power);

    //Fill the list of points with all points in the image.
    for (long y = 0; y < height; y++) 
	{
	for (long x = 0; x < width; x++) 
	    {
	    Point pt(x, height - 1 - y);
	    *(pointsRemaining + y * width + x) = pt;
	    RemainingPointCount++;
	    }
	}

    double magnifiedRadius = ZoomRadius;
    BigDouble	BigMagnifiedRadius = BigZoomRadius;    
    int window_radius = (width < height) ? width : height;
    *ThreadComplete = false;

    while (RemainingPointCount > (width * height) * (percentGlitchTolerance / 100))
	{
	referencePoints++;
	if (user_data(NULL) < 0)
	    {
	    sprintf(PertErrorMessage, "User Activity at RemainingPointCount > (width * height) in thread %d", thread);
	    CloseTheDamnPointers();
	    return -1;
	    }

	//Determine the reference point to calculate
	//Check whether this is the first time running the loop. 
	if (referencePoints == 1) 
	    {
	    mpfr_init_set(C.x.x, xZoomPt, MPFR_RNDN);
	    mpfr_init_set(C.y.x, yZoomPt, MPFR_RNDN);
	    ReferenceCoordinate = C;

	    BigCalculatedDelta = 0.0;
	    calculatedDelta = 0.0;

	    if (ReferenceZoomPoint(ReferenceCoordinate, MaxIteration, user_data, StatusBarInfo, pPertProgress, bailout, Multi) < 0)
		{
		sprintf(PertErrorMessage, "User Activity at ReferenceZoomPoint(1) in thread %d", thread);
		CloseTheDamnPointers();
		return -1;
		}
	    }
	else 
	    {
	    if (calculateGlitches == false) 
		break;

	    int referencePointIndex = 0;
	    int Randomise;

	    srand((unsigned)time(NULL));		// Seed the random-number generator with current time 
	    Randomise = rand();
	    referencePointIndex = (int)((double)Randomise / (RAND_MAX + 1) * RemainingPointCount);


	    // We need to store this offset because the formula we use to convert pixels into a complex point does so relative to the center of the image.
	    // We need to offset that calculation when our reference point isn't in the center. The actual offsetting is done in calculate point.

	    Point   pt = *(pointsRemaining + referencePointIndex);
	    if (ArithType == FLOATEXP || ArithType == DOUBLESHIFT)
		{ 
		//Get the complex point at the chosen reference point
		BigDelta.x = ((BigMagnifiedRadius * (2 * pt.getX() - width)) / window_radius);
		BigDelta.y = ((-BigMagnifiedRadius * (2 * pt.getY() - height)) / window_radius);
		BigCalculatedDelta = BigDelta;
		ReferenceCoordinate = C + BigDelta;
		}
	    else
		{
		//Get the complex point at the chosen reference point
		delta.x = ((magnifiedRadius * (2 * pt.getX() - width)) / window_radius);
		delta.y = ((-magnifiedRadius * (2 * pt.getY() - height)) / window_radius);

		calculatedDelta = delta;

		ReferenceCoordinate.x = C.x + delta.x;
		ReferenceCoordinate.y = C.y + delta.y;
		}

	    if (ReferenceZoomPoint(ReferenceCoordinate, MaxIteration, user_data, StatusBarInfo, pPertProgress, bailout, Multi) < 0)
		{
		sprintf(PertErrorMessage, "User Activity at ReferenceZoomPoint(2) in thread %d", thread);
		CloseTheDamnPointers();
		return -1;
		}
	    }

	int lastChecked = -1;
	GlitchPointCount = 0;
	for (i = 0; i < RemainingPointCount; i++)
	    {
	    Point   pt = *(pointsRemaining + i);
	    x = pt.getX();
	    y = pt.getY();

	    if (EnableApproximation)
		{
		int	    iteration;

		BigDouble pixelSize = BigWidth.BigDoubleToDouble() / width;;
		BigComplex c;
		c.x = ReferenceCoordinate.x + pixelSize * (double)(x - width / 2.0L);
		c.y = ReferenceCoordinate.y - pixelSize * (double)(y - height / 2.0L);

		if (ArithType == BLA_FLOATEXP || ArithType == BLA_EXP_UNSUPPORTED)
		    {
		    ExpComplex  zExp;
		    BigDelta.x = ((BigMagnifiedRadius * (2 * x - width)) / window_radius) - BigCalculatedDelta.x;
		    BigDelta.y = ((-BigMagnifiedRadius * (2 * y - height)) / window_radius) - BigCalculatedDelta.y;
		    BigComplex2ExpComplex(&ExpDeltaSub0, BigDelta);
		    if ((iteration = iterateFractalWithPerturbationBLAExp(ExpXSubN, MaxIteration, bailout, ExpDeltaSub0.CHalf(), &Bla, TZfilter, &zExp, user_data)) < 0)
			return -1;

		    ColourProcessingExp(zExp, iteration, x, y, TrueCol, TZfilter, bailout);
		    }
		else
		    {
		    Complex	z;
		    delta.x = ((magnifiedRadius * (2 * x - width)) / window_radius) - calculatedDelta.x;
		    delta.y = ((-magnifiedRadius * (2 * y - height)) / window_radius) - calculatedDelta.y;
		    DeltaSub0 = delta / 2.0;
		    if ((iteration = iterateFractalWithPerturbationBLA(XSubN, MaxIteration, bailout, DeltaSub0, &Bla, TZfilter, &z, user_data)) < 0)
			return -1;
		    ColourProcessing(z, iteration, x, y, TrueCol, TZfilter, bailout);
		    }
		}
	    else
		{
		if (ArithType != DOUBLE)
		    {
		    BigDelta.x = ((BigMagnifiedRadius * (2 * x - width)) / window_radius) - BigCalculatedDelta.x;
		    BigDelta.y = ((-BigMagnifiedRadius * (2 * y - height)) / window_radius) - BigCalculatedDelta.y;
		    BigComplex2ExpComplex(&ExpDeltaSub0, BigDelta);
		    }

		if (ArithType == FLOATEXP)
		    {
		    if (CalculatePointExp(x, y, ExpDeltaSub0, bailout, glitchPoints, user_data, TZfilter, TrueCol, ghMutex) < 0)
			{
			sprintf(PertErrorMessage, "User Activity from within calculatePoint() in thread %d", thread);
			CloseTheDamnPointers();
			return -1;
			}
		    }
		else
		    {
		    if (ArithType == DOUBLE)
			{
			delta.x = ((magnifiedRadius * (2 * x - width)) / window_radius) - calculatedDelta.x;
			delta.y = ((-magnifiedRadius * (2 * y - height)) / window_radius) - calculatedDelta.y;
			DeltaSub0 = delta;
			}
		    else
			{
			DeltaSub0.x = ExpDeltaSub0.x.todouble(ScalingOffset);
			DeltaSub0.y = ExpDeltaSub0.y.todouble(ScalingOffset);
			}
		    if (calculatePoint(x, y, DeltaSub0, bailout, glitchPoints, user_data, TZfilter, TrueCol, ghMutex) < 0)
			{
			sprintf(PertErrorMessage, "User Activity from within calculatePoint() in thread %d", thread);
			CloseTheDamnPointers();
			return -1;
			}
		    }
		if (user_data(NULL) < 0)
		    {
		    sprintf(PertErrorMessage, "User Activity after calculatePoint() in thread %d", thread);
		    CloseTheDamnPointers();
		    return -1;
		    }
		}


	    //Everything else in this loop is just for updating the progress counter. 
	    double progress = (double)i / RemainingPointCount;
	    if (int(progress * 100) != lastChecked) 
		{
		char	UseBigWidthStr[12];
		if (ArithType == DOUBLE)
		    strcpy(UseBigWidthStr, " Duble");
		else if (ArithType == DOUBLESHIFT)
		    strcpy(UseBigWidthStr, " Shift");
		else
		    strcpy(UseBigWidthStr, " Exp");
		lastChecked = int(progress * 100);
		sprintf(StatusBarInfo, "Th=%d, Pass=%d, (%d%%)%s", thread, referencePoints, int(progress * 100), UseBigWidthStr);
		if (Multi)
		    {
		    *pPertProgress = int(progress * 100);
		    if (delay > 0)
			{
			Sleep(delay);
			InvalidateRect(hwnd, NULL, FALSE);
			}
		    }
		}
	    }

	//These points are glitched, so we need to mark them for recalculation. We need to recalculate them using Pauldelbrot's glitch fixing method (see calculate point).
	memcpy(pointsRemaining, glitchPoints, sizeof(Point) * GlitchPointCount);
	RemainingPointCount = GlitchPointCount;
	}

    *ThreadComplete = true;
    CloseTheDamnPointers();
    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Cleanup
//////////////////////////////////////////////////////////////////////

void	CPerturbation::CloseTheDamnPointers(void)
    {
    if (xZoomPt->_mpfr_d) mpfr_clear(xZoomPt);
    if (yZoomPt->_mpfr_d) mpfr_clear(yZoomPt);
    if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
    if (glitchPoints) { delete[] glitchPoints; glitchPoints = NULL; }
    if (XSubN) { delete[] XSubN; XSubN = NULL; }
    if (EnableApproximation) Bla.clear();

    if (ArithType == FLOATEXP)
	{
	if (ExpXSubN) { delete[] ExpXSubN; ExpXSubN = NULL; }
	if (ExpPerturbationToleranceCheck) { delete[] ExpPerturbationToleranceCheck; ExpPerturbationToleranceCheck = NULL; }
	}
    else
	{
	if (PerturbationToleranceCheck) { delete[] PerturbationToleranceCheck; PerturbationToleranceCheck = NULL; }
	if (XSubN) { delete[] XSubN; XSubN = NULL; }
#ifdef	ALLOW_SIN
	if (subtype == 58)	// sin()
	    {
	    if (sinZp) { delete[] sinZp; sinZp = NULL; }
	    if (cosZp) { delete[] cosZp; cosZp = NULL; }
	    }
#endif
	}
    }

//////////////////////////////////////////////////////////////////////
// Individual point calculation
//////////////////////////////////////////////////////////////////////

int	CPerturbation::calculatePoint(int x, int y, Complex DeltaSub0, double bailout, Point *glitchPoints, int user_data(HWND hwnd), CTZfilter *TZfilter, CTrueCol *TrueCol, HANDLE ghMutex)
    {
// Get the complex number at this pixel.
// This calculates the number relative to the reference point, so we need to translate that to the center when the reference point isn't in the center.
// That's why for the first reference, calculatedRealDelta and calculatedImaginaryDelta are 0: it's calculating relative to the center.

    double	magnitude = 0.0;
    int		iteration = skippedIterations;
    Complex	DeltaSubN;
    Complex	temp, temp1, temp2;
//    CPlot	Plot;

    ExpComplex	ExpDC = 0.0;				// used for derivative slope when upgrading double to shifted double
    ExpComplex	ExpTemp, ExpTemp1;
    BigComplex	BigDC = 0.0;				// for deep, slope high precision
    Complex	dc = 0.0;

    DeltaSubN = 0.0;
    iteration = 0;
    bool glitched = false;

    dc = 0.0;
    if (InsideMethod == BOF60 || InsideMethod == BOF61)
	{
	BOFmagnitude = 0.0;
	min_orbit = 100000.0;
	}

#ifdef ALLOW_ARTMATRIXCUBIC
    if (subtype == 58)				// Art Matrix cubic
	{
	t3 = DeltaSub0 * 3;			// T3 = 3*T
	t2 = DeltaSub0.CSqr();			// T2 = T*T
	a = (t2 + 1) / t3;			// A  = (T2 + 1)/T3
						// B  = 2*A*A*A + (T2 - 2)/T3    
	temp = a.CCube() * 2;			// 2*A*A*A
	b = (t2 - 2) / t3 + temp;		// B  = 2*A*A*A + (T2 - 2)/T3
	aa3 = a.CSqr() * 3;			// AA3 = A*A*3
	*(XSubN + 0) + DeltaSubN = -a;		// Z = -A
	}
#endif

    if (OutsideMethod >= TIERAZONFILTERS)
	TZfilter->LoadFilterQ(DeltaSub0);		// initialise the constants used by Tierazon filters

    //Iteration loop
    if ((iteration = iterateFractalWithPerturbation(XSubN, &DeltaSub0, &DeltaSubN, MaxIteration, bailout, x, y, user_data, TZfilter, &ExpDC, &dc, &glitched)) < 0)
	return -1;

    if (SlopeType == DERIVSLOPE && (iteration < MaxIteration || InsideMethod == 0))	// complicated if statement - we want to keep outside slope if using inside filters
	{
	if (ArithType == DOUBLESHIFT)
	    {
	    ExpTemp = *(XSubN + iteration) + DeltaSubN * Scaling;
	    BigProcessDerivativeSlope(ExpDC, ExpTemp, ghMutex, iteration, TrueCol, x, y);
	    }
	else
	    ProcessDerivativeSlope(dc, *(XSubN + iteration) + DeltaSubN * Scaling, ghMutex, iteration, TrueCol, x, y);
	}
    if (glitched == false) 
	{
	double	rqlim2 = sqrt(bailout);
	Complex	w;

	if (ArithType == DOUBLESHIFT)
	    w = *(XSubN + iteration) + DeltaSubN * Scaling;
	else
	    w = *(XSubN + iteration) + DeltaSubN;

	ColourProcessing(w, iteration, x, y, TrueCol, TZfilter, bailout);
	}
    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Individual point calculation - FloatExp version
//////////////////////////////////////////////////////////////////////

int	CPerturbation::CalculatePointExp(int x, int y, ExpComplex ExpDeltaSub0, double bailout, Point *glitchPoints, int user_data(HWND hwnd), CTZfilter *TZfilter, CTrueCol *TrueCol, HANDLE ghMutex)
    {
    // Get the complex number at this pixel.
    // This calculates the number relative to the reference point, so we need to translate that to the center when the reference point isn't in the center.
    // That's why for the first reference, calculatedRealDelta and calculatedImaginaryDelta are 0: it's calculating relative to the center.

    ExpComplex	ExpDeltaSubN;

    ExpComplex	ExpTemp, ExpTemp1, ExpTemp2, ExpTemp3, ExpTemp4;
    ExpComplex	ExpDC = 0.0;

    int		iteration = skippedIterations;
    double	magnitude = 0.0;

    BigComplex	BigDC = 0.0;
    BigComplex	BigU;
    BigDouble	BigReflection;

    ExpDeltaSubN = ExpDeltaSub0;
    iteration = 0;
    bool glitched = false;

    if (OutsideMethod >= TIERAZONFILTERS)
	{
	Complex	tempComplex;

	tempComplex.x = ExpDeltaSub0.x.todouble();
	tempComplex.y = ExpDeltaSub0.y.todouble();
	TZfilter->LoadFilterQ(tempComplex);		// initialise the constants used by Tierazon filters
	}

    if ((iteration = iterateFractalWithPerturbationExp(ExpXSubN, &ExpDeltaSub0, &ExpDeltaSubN, MaxIteration, bailout, x, y, user_data, TZfilter, &ExpDC, &glitched)) < 0)
	return -1;

    if (SlopeType == DERIVSLOPE)
	BigProcessDerivativeSlope(ExpDC, ExpTemp, ghMutex, iteration, TrueCol, x, y);

    if (glitched == false)
	{
//	int	index;
	double	rqlim2 = sqrt(bailout);
	ExpComplex	ExpW;

	ExpW = *(ExpXSubN + iteration) + ExpDeltaSubN;
	ColourProcessingExp(ExpW, iteration, x, y, TrueCol, TZfilter, bailout);
	}
    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Reference Zoom Point
//////////////////////////////////////////////////////////////////////

int CPerturbation::ReferenceZoomPoint(BigComplex& centre, int maxIteration, int user_data(HWND hwnd), char* StatusBarInfo, int *pPertProgress, double bailout, bool Multi)
    {
    // Raising this number makes more calculations, but less variation between each calculation (less chance of mis-identifying a glitched point).
    BigComplex	ZTimes2, Z, SinX, CosX, ScaledZ;
    double	glitchTolerancy = 1e-6;
    BigDouble	TempReal, TempImag;
    BigDouble	zisqr, zrsqr, realimag;
    bool	WeHaveMaxRefIteration = false;
    BigDouble	BigBailout = bailout;
    BigDouble	BigCoordinateMagnitudeSquared;

    ReferenceNumber++;
    Z = 0.0;

#ifdef ALLOW_ARTMATRIXCUBIC
    if (subtype == 58)
	{
	BigComplex	tempBig, qBig;
	qBig = centre;
	t3Big = qBig * 3.0;			// T3 = 3*T
	t2Big = qBig.CSqr();			// T2 = T*T
	aBig = (t2Big + 1.0) / t3Big;		// A  = (T2 + 1)/T3
						// B  = 2*A*A*A + (T2 - 2)/T3    
	tempBig = aBig.CCube();			// A*A*A
	tempBig = tempBig.CDouble();		// 2*A*A*A
	bBig = (t2Big - 2.0) / t3Big + tempBig;	// B  = 2*A*A*A + (T2 - 2)/T3
	aa3Big = aBig.CSqr()*3.0;		// AA3 = A*A*3
	}
#endif

    for (int i = 0; i <= maxIteration; i++)
	{
	Complex	C;
	ExpComplex	ExpC;
	// pre multiply by two
	ZTimes2 = Z.CDouble();

	// The reason we are storing the same value times two is that we can precalculate this value here because multiplying this value by two is needed many times in the program.
	// Also, for some reason, we can't multiply complex numbers by anything greater than 1 using std::complex, so we have to multiply the individual terms each time.
	// This is expensive to do above, so we are just doing it here.

	// Norm is the squared version of abs and 0.000001 is 10^-3 squared.
	// The reason we are storing this into an array is that we need to check the magnitude against this value to see if the value is glitched. 
	// We are leaving it squared because otherwise we'd need to do a square root operation, which is expensive, so we'll just compare this to the squared magnitude.

	if (ArithType == BLA_FLOATEXP || ArithType == BLA_EXP_UNSUPPORTED || ArithType == FLOATEXP)
	    {
	    BigComplex2ExpComplex(&ExpC, Z);
	    *(ExpXSubN + i) = ExpC;
	    }
	else
	    {
	    C = Z.CBig2Double();
	    *(XSubN + i) = C;
	    }

#ifdef	ALLOW_SIN
	if (subtype == 58)							// sin()
	    {
	    Complex	xSin, xCos;
	    if (user_data(NULL) < 0)
		{
		mpfr_clear(TempReal);
		mpfr_clear(TempImag);
		return -1;
		}
	    BigTrig.CSin(&SinX, Z);
	    BigTrig.CCos(&CosX, Z);

	    xSin = SinX.CBig2Double();
	    xCos = CosX.CBig2Double();

	    *(sinZp + i) = xSin;
	    *(cosZp + i) = xCos;
	    }
#endif
	if (user_data(NULL) < 0)
	    return -1;
	//Everything else in this loop is just for updating the progress counter. 
	int lastChecked = -1;
	double progress = (double)i / maxIteration;
	if (int(progress * 100) != lastChecked)
	    {
	    lastChecked = int(progress * 100);
	    sprintf(StatusBarInfo, "Th=%d, Ref[%d]=(%d%%)", thread, ReferenceNumber, int(progress * 100));
	    if (Multi)
		{
		*pPertProgress = int(progress * 100);
		}
	    }

	if (ArithType == BLA_FLOATEXP || ArithType == BLA_EXP_UNSUPPORTED || ArithType == FLOATEXP)
	    {
	    BigComplex	BigTemp = Z * glitchTolerancy;
	    BigDouble	BigTolerancy = BigTemp.x * BigTemp.x + BigTemp.y * BigTemp.y;
	    Bignum2FloatExp((ExpPerturbationToleranceCheck + i), BigTolerancy);
	    }
	else
	    {
	    TempReal = Z.x * glitchTolerancy;
	    TempImag = Z.y * glitchTolerancy;
	    Complex tolerancy{ TempReal.BigDoubleToDouble(), TempImag.BigDoubleToDouble() };
	    *(PerturbationToleranceCheck + i) = (tolerancy.CSumSqr());
	    }

	// Calculate the set
	RefFunctions(&centre, &Z, &ZTimes2);
	if (EnableApproximation && !WeHaveMaxRefIteration)			// only needed for BLA
	    {
	    BigCoordinateMagnitudeSquared = Z.CSumSqr();			// no point in further testing once we have MaxRefIteration
	    if ((BigCoordinateMagnitudeSquared) > BigBailout && !WeHaveMaxRefIteration)
		{
		MaxRefIteration = i;	// needed for BLA
		WeHaveMaxRefIteration = true;
		break;			// the rest of the table isn't needed for BLA
		}
	    }
	}

    if (!WeHaveMaxRefIteration)
	MaxRefIteration = maxIteration;	// MaxRefIteration not found in main reference loop

    if (EnableApproximation)
	{
	Bla.clear();
	int M = MaxRefIteration; // the period
	int image_size = MIN(width, height);
	if (ArithType == BLA_FLOATEXP)
	    {
	    // set up parameters for BLA table
	    ExpComplex temp_size_image_size = ExpComplex(ZoomRadius / (double)image_size, ZoomRadius / (double)image_size);
	    floatexp blaSize = blaSize.hypotExp(temp_size_image_size.x * width * 0.5, temp_size_image_size.y * height * 0.5);
	    Bla.initExp(M, ExpXSubN, blaSize, power, subtype);
	    }
	else if (ArithType == BLA_DOUBLE)
	    {
	    Complex temp_size_image_size = Complex(ZoomRadius / (double)image_size, ZoomRadius / (double)image_size);
	    double blaSize = hypot(temp_size_image_size.x * width * 0.5, temp_size_image_size.y * height * 0.5);
	    Bla.init(M, XSubN, blaSize, power, subtype);
	    }
	}
    return 0;
    }
    
