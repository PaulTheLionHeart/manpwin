/****************************************************
    FRAMECALCULATOR.CPP a module to explore Perturbation
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
#include "FrameCalculator.h"
#include "Slope.h"
#include "Potential.h"

//////////////////////////////////////////////////////////////////////
// Initialisation
//////////////////////////////////////////////////////////////////////

int calculateFrame::initialiseCalculateFrame(CDib *DibIn, CSlope *Slope, int WidthIn, int HeightIn, int threshold, BigDouble xZoomPointin, BigDouble yZoomPointin, BigDouble BigWidthIn, int decimals, CTZfilter *TZfilter, HWND hwndIn, int ThreadIn, double *wpixelsIn, 
			double paramIn[], double potparamIn[], int PaletteShiftIn, int *PlotTypeIn, int SlopeTypeIn, double lightDirectionDegreesIn, double bumpMappingDepthIn, double bumpMappingStrengthIn, int PaletteStartIn, double LightHeightIn, int PertColourMethodIn,
			int PalOffsetIn, double IterDivIn)
    {
    Complex q;
    int	    i;

    width = WidthIn;
    height = HeightIn;
    MaxIteration = threshold;
    BigWidth = BigWidthIn;
    ZoomRadius = mpfr_get_d(BigWidth.x, MPFR_RNDN);
    BigZoomRadius = BigWidth;
    InsideMethod = TZfilter->method;
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

    xZoomPt = xZoomPointin;
    yZoomPt = yZoomPointin;

    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Full frame calculation
//////////////////////////////////////////////////////////////////////

int calculateFrame::calculateOneFrame(double bailout, char* StatusBarInfo, int powerin, int InsideMethodIn, int OutsideMethodIn, int biomorphin, int subtypein, Complex rsrAin, bool rsrSignIn,
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

    if (fabs(ZoomRadius) > DBL_MIN && precision <= 300)
	{
	ArithType = DOUBLE;
	ScalingOffset = 0;
	Scaling = 1.0;
	}
    else if (precision < 600 && (subtype == 0 || (subtype == 1 && power == 3)))	// only for Mandelbrot and Cubic so far
	{
	ArithType = DOUBLESHIFT;						// use shifted double float
	ScalingOffset = 0;
	Scaling = 1.0;
	for (i = 300; i < precision; i++)
	    {
	    ScalingOffset++;
	    Scaling = Scaling * 0.1;
	    }
	ScalingSquared = Scaling * Scaling;
	}
    else
	{
	ArithType = FLOATEXP;							// width too small for double float
	ScalingOffset = 0;
	Scaling = 1;
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
    if (ArithType == FLOATEXP)
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
	    C.x = xZoomPt;
	    C.y = yZoomPt;
	    ReferenceCoordinate = C;

	    BigCalculatedDelta = 0.0;
	    calculatedDelta = 0.0;

	    if (ReferenceZoomPoint(ReferenceCoordinate, MaxIteration, user_data, StatusBarInfo, pPertProgress, Multi) < 0)
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

	    if (ReferenceZoomPoint(ReferenceCoordinate, MaxIteration, user_data, StatusBarInfo, pPertProgress, Multi) < 0)
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

	    if (ArithType != DOUBLE)
		{
		BigDelta.x = ((BigMagnifiedRadius * (2 * x - width)) / window_radius) - BigCalculatedDelta.x;
		BigDelta.y = ((-BigMagnifiedRadius * (2 * y - height)) / window_radius) - BigCalculatedDelta.y;
		BigComplex2ExpComplex(&ExpDeltaSub0, BigDelta);
		}

	    if (ArithType == FLOATEXP)
		{
		if (BigCalculatePoint(x, y, ExpDeltaSub0, bailout, glitchPoints, user_data, TZfilter, TrueCol, ghMutex) < 0)
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

void	calculateFrame::CloseTheDamnPointers(void)
    {
    if (pointsRemaining) { delete[] pointsRemaining; pointsRemaining = NULL; }
    if (glitchPoints) { delete[] glitchPoints; glitchPoints = NULL; }
    if (XSubN) { delete[] XSubN; XSubN = NULL; }

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

int	calculateFrame::calculatePoint(int x, int y, Complex DeltaSub0, double bailout, Point *glitchPoints, int user_data(HWND hwnd), CTZfilter *TZfilter, CTrueCol *TrueCol, HANDLE ghMutex)
    {
// Get the complex number at this pixel.
// This calculates the number relative to the reference point, so we need to translate that to the center when the reference point isn't in the center.
// That's why for the first reference, calculatedRealDelta and calculatedImaginaryDelta are 0: it's calculating relative to the center.

    double	magnitude = 0.0;
    int		iteration = skippedIterations;
    double	FloatIteration;
    Complex	DeltaSubN;
    Complex	temp, temp1, temp2;
    double	log_zn, nu;
    int		SlopeIndex;
    CPlot	Plot;

    ExpComplex	ExpDC = 0.0;				// used for derivative slope when upgrading double to shifted double
    ExpComplex	ExpTemp, ExpTemp1;

    /* removed as it causes glitches
    Complex	Saved = 0.0;				// periodicity checking stuff
    double	closenuff = fabs(deltaImaginary / 16.0);
    int		reset_period = 0;
    */

    BigComplex	BigDC = 0.0;				// for deep, slope high precision

    Complex	dc = 0.0;
    CPotential	Pot;

    DeltaSubN = DeltaSub0;
    iteration = 0;
    FloatIteration = 0.0;
    bool glitched = false;

    double	BOFmagnitude;
    double	min_orbit;			// orbit value closest to origin
    long	min_index;			// iteration of min_orbit
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

    if (InsideMethod >= TIERAZONFILTERS)
	TZfilter->LoadFilterQ(DeltaSub0);		// initialise the constants used by Tierazon filters

    Plot.InitPlot(MaxIteration, TrueCol, wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);
    //Iteration loop
    do
	{
	if (user_data(NULL) < 0)
	    return -1;
	if (ArithType == DOUBLESHIFT)
	    temp1 = *(XSubN + iteration) + DeltaSubN * Scaling;
	else
	    temp1 = *(XSubN + iteration) + DeltaSubN;

#ifdef	ALLOW_SIN
	PertFunctions((XSubN + iteration), (sinZp + iteration), (cosZp + iteration), &DeltaSubN, &DeltaSub0);
#else
	PertFunctions((XSubN + iteration), &DeltaSubN, &DeltaSub0);
#endif
	iteration++;
	FloatIteration++;
	temp = *(XSubN + iteration) + DeltaSubN * Scaling;
	ZCoordinateMagnitudeSquared = temp.CSumSqr();

	if (SlopeType == DERIVSLOPE)
	    {
	    if (ArithType == DOUBLESHIFT)
		{
		ExpTemp1 = temp1;
		BigCalculateDerivativeSlope(&ExpDC, ExpTemp1);
		}
	    else
		CalculateDerivativeSlope(&dc, temp1);
	    }

	if (InsideMethod >= TIERAZONFILTERS)
	    {
	    Complex z = *(XSubN + iteration) + DeltaSubN;
	    TZfilter->DoTierazonFilter(z, (long *)&iteration);
	    }
	else if (InsideMethod == BOF60 || InsideMethod == BOF61)
	    {
	    Complex z = *(XSubN + iteration) + DeltaSubN;
	    BOFmagnitude = z.CSumSqr();
	    if (BOFmagnitude < min_orbit)
		{
		min_orbit = BOFmagnitude;
		min_index = iteration + 1L;
		}
	    }

	// periodicity checking https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#Periodicity_checking
	/* removed as it causes glitches

	if (fabs(Saved.x - temp1.x) < closenuff)
	    if (fabs(Saved.y - temp1.y) < closenuff)
		{
		iteration = MaxIteration;
		break;
		}
	reset_period++;
	if (reset_period > 20)				// code stores away a new x and y value on every 20:th iteration, thus it can detect periods that are up to 20 points long. 
	    {
	    reset_period = 0;
	    Saved = temp1;
	    }
	*/
	// This is Pauldelbrot's glitch detection method. You can see it here: http://www.fractalforums.com/announcements-and-news/pertubation-theory-glitches-improvement/.
	// As for why it looks so weird, it's because I've squared both sides of his equation and moved the |ZsubN| to the other side to be precalculated.
	// For more information, look at where the reference point is calculated.
	// I also only want to store this point once.

	if (calculateGlitches == true && glitched == false && ZCoordinateMagnitudeSquared < *(PerturbationToleranceCheck + iteration))
	    {
	    Point pt(x, y, iteration);
	    glitchPoints[GlitchPointCount] = pt;
	    GlitchPointCount++;
	    glitched = true;
	    break;
	    }
/*
	if (subtype == 57)		// Cubic Newton
	    {
	    Complex	zd = temp1 - temp;
	    double	d = zd.CSumSqr();
	    if (d < MINSIZE || d > MAXSIZE)
		break;
	    }
*/
	//use bailout radius of 256 for smooth coloring.
	} while (ZCoordinateMagnitudeSquared < bailout && iteration < MaxIteration);

    if (SlopeType == DERIVSLOPE)
	{
	if (ArithType == DOUBLESHIFT)
	    {
	    ExpTemp = *(XSubN + iteration) + DeltaSubN * Scaling;
	    BigProcessDerivativeSlope(ExpDC, ExpTemp, ghMutex, iteration, TrueCol, x, y);
	    }
	else
	    ProcessDerivativeSlope(dc, temp, ghMutex, iteration, TrueCol, x, y);
	}
    if (glitched == false) 
	{
	int	index;
	double	rqlim2 = sqrt(bailout);
	Complex	w;

	if (ArithType == DOUBLESHIFT)
	    w = *(XSubN + iteration) + DeltaSubN * Scaling;
	else
	    w = *(XSubN + iteration) + DeltaSubN;
	if (wpixels)						// it's not Null, so we must have initialsed it for fwd diff slope
	    {
	    if (iteration < MaxIteration)
		{
		log_zn = log(w.x * w.x + w.y * w.y) / SlopeDegree;
		nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		FloatIteration = FloatIteration + 2 - nu;
		}
	    else
		FloatIteration = MaxIteration;
	    SlopeIndex = (((DWORD)height - 1 - y) * (DWORD)Dib->DibWidth) + (DWORD)x + width * thread;
	    if (x >= 0 && x < Dib->DibWidth - 1 && y >= 0 && y < Dib->DibHeight - 1 && wpixels)
		*(wpixels + SlopeIndex) = FloatIteration;
	    }

	if (PertColourMethod != 0 && iteration < MaxIteration)		// Kalles colour method
	    {
	    double  iter = (double)iteration;
	    if (PertColourMethod == 1)
		iter = sqrt(iter);
	    else if (PertColourMethod == 2)
		iter = pow(iter, (double)1 / (double)3);
	    else if (PertColourMethod == 3)
		iter = log(iter);
	    iteration = (int)iter;
	    }
	if (IterDiv != 1.0 && iteration < MaxIteration)
	    iteration = (int)((double)iteration / IterDiv);
	if (PalOffset && iteration < MaxIteration)
	    iteration = (iteration + PalOffset) % TrueCol->ColoursInPALFile;

	if (biomorph >= 0)						// biomorph
	    {
	    if (iteration == MaxIteration)
		index = MaxIteration;
	    else
		{
		if (fabs(w.x) < rqlim2 || fabs(w.y) < rqlim2)
		    index = biomorph;
		else
		    index = iteration % 256;
		}
	    }
	else if (OutsideMethod == NONE && InsideMethod == NONE)		// no filter
	    {
	    if (iteration == MaxIteration)
		index = MaxIteration;
	    else
		index = iteration;
	    }
	else
	    {
	    switch (OutsideMethod)
		{
		case PERT1:						// something Shirom Makkad added
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = (int)((iteration - log2(log2(ZCoordinateMagnitudeSquared))) * 5) % 256; //Get the index of the color array that we are going to read from. 
		    break;
		case PERT2:						// something Shirom Makkad added
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = (int)(iteration - (log(0.5*(ZCoordinateMagnitudeSquared)) - log(0.5*log(256))) / log(2)) % 256;
		    break;
		case REAL:						// "real"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = iteration + (long)w.x + 7;
		    break;
		case IMAG:	    					// "imag"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = iteration + (long)w.y + 7;
		    break;
		case MULT:						// "mult"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else if (w.y)
			index = (long)((double)iteration * (w.x / w.y));
		    else
			index = iteration;
		    break;
		case SUM:						// "sum"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = iteration + (long)(w.x + w.y);
		    break;
		case ATAN:						// "atan"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = (long)fabs(atan2(w.y, w.x)*180.0 / PI);
		    break;
		default:						// make sure we set all the inside pixels unless there is an inside filter (later on in the next switch)
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			{
//			if (abs(PaletteShift) <= 1)
			index = iteration;
//			else
//			    index = (BYTE)(((long)(FloatIteration * abs(PaletteShift))) % 256);
//			index = iteration % 256;
			}
		    break;
		}
	    switch (InsideMethod)
		{
		case ZMAG:
		    if (iteration == MaxIteration)			// Zmag
			index = (int)((w.CSumSqr()) * (MaxIteration >> 1) + 1);
//		    else
//			index = iteration;
		    //			index = iteration % 256;
		    break;
		case BOF60:
		    if (iteration == MaxIteration)
			index = (int)(sqrt(min_orbit) * 75.0);
//		    else
//			index = iteration;
		    break;
		case BOF61:
		    if (iteration == MaxIteration)
			index = min_index;
//		    else
//			index = iteration;
		    break;
		case POTENTIAL:
		    magnitude = sqr(w.x) + sqr(w.y);
		    index = Pot.potential(magnitude, iteration, MaxIteration, TrueCol, 256, potparam);
		    break;
		default:
		    if (InsideMethod >= TIERAZONFILTERS)		// suite of Tierazon filters and colouring schemes
			{
			TZfilter->EndTierazonFilter(w, (long *)&iteration, TrueCol);
			index = iteration;
			}
		    break;
		}
	    }

	if (SlopeType != DERIVSLOPE)
	    {
	    if (*PlotType == FILTERPLOT)
		Plot.FilterPoint(x + width * thread, height - 1 - y, index, &(TZfilter->FilterRGB));
	    else
		Plot.PlotPoint(x + width * thread, height - 1 - y, index);
	    }
	}
    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Individual point calculation
//////////////////////////////////////////////////////////////////////

int	calculateFrame::BigCalculatePoint(int x, int y, ExpComplex ExpDeltaSub0, double bailout, Point *glitchPoints, int user_data(HWND hwnd), CTZfilter *TZfilter, CTrueCol *TrueCol, HANDLE ghMutex)
    {
    // Get the complex number at this pixel.
    // This calculates the number relative to the reference point, so we need to translate that to the center when the reference point isn't in the center.
    // That's why for the first reference, calculatedRealDelta and calculatedImaginaryDelta are 0: it's calculating relative to the center.

    ExpComplex	ExpDeltaSubN;
    ExpComplex	ExpX_N;

    ExpComplex	ExpTemp, ExpTemp1, ExpTemp2, ExpTemp3, ExpTemp4;
    floatexp	ExpBailout = bailout;
    ExpComplex	ExpDC = 0.0;

    int		iteration = skippedIterations;
    double	FloatIteration;
    double	magnitude = 0.0;

    double	log_zn, nu;
    int		SlopeIndex;

//	int		reset_period = 0;

    BigComplex	BigDC = 0.0;
    BigComplex	BigU;
    BigDouble	BigReflection;
    CPotential	Pot;
    CPlot	Plot;

    ExpDeltaSubN = ExpDeltaSub0;
    iteration = 0;
    FloatIteration = 0.0;
    bool glitched = false;

    if (InsideMethod >= TIERAZONFILTERS)
	{
	Complex	tempComplex;

	tempComplex.x = ExpDeltaSub0.x.todouble();
	tempComplex.y = ExpDeltaSub0.y.todouble();
	TZfilter->LoadFilterQ(tempComplex);		// initialise the constants used by Tierazon filters
	}

    Plot.InitPlot(MaxIteration, TrueCol, wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);
    // Iteration loop
    do
	{
	if (user_data(NULL) < 0)
	    return -1;
	ExpTemp1 = *(ExpXSubN + iteration) + ExpDeltaSubN;

	if (SlopeType == DERIVSLOPE)
	    BigCalculateDerivativeSlope(&ExpDC, ExpTemp1);
	ExpX_N = *(ExpXSubN + iteration);

	BigPertFunctions(&ExpX_N, &ExpDeltaSubN, &ExpDeltaSub0);

	iteration++;
	FloatIteration++;
	ExpTemp = ExpDeltaSubN + *(ExpXSubN + iteration);
	ExpZCoordinateMagnitudeSquared = ExpTemp.CSumSqr();

	if (InsideMethod >= TIERAZONFILTERS)
	    {
	    Complex	tempComplex;
	    ExpTemp = ExpDeltaSubN + *(ExpXSubN + iteration);
	    tempComplex.x = ExpTemp.x.todouble();
	    tempComplex.y = ExpTemp.y.todouble();
	    TZfilter->DoTierazonFilter(tempComplex, (long *)&iteration);
	    }

	// periodicity checking https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#Periodicity_checking
	/* removed as it causes glitches
	BigTemp4Real = BigSavedReal - BigTempReal;
	BigTemp4Imag = BigSavedImag - BigTempImag;
	if (BigTemp4Real.abs() < BigClosenuff)
	    {
	    if (BigTemp4Imag.abs() < BigClosenuff)
		{
		iteration = MaxIteration;
		break;
		}
	    }
	reset_period++;
	if (reset_period > 20)				// code stores away a new x and y value on every 20:th iteration, thus it can detect periods that are up to 20 points long.
	    {
	    reset_period = 0;
	    BigSavedReal = BigTempReal;
	    BigSavedImag = BigTempImag;
	    }
	*/
	// This is Pauldelbrot's glitch detection method. You can see it here: http://www.fractalforums.com/announcements-and-news/pertubation-theory-glitches-improvement/.
	// As for why it looks so weird, it's because I've squared both sides of his equation and moved the |ZsubN| to the other side to be precalculated.
	// For more information, look at where the reference point is calculated.
	// I also only want to store this point once.

	if (calculateGlitches == true && glitched == false && ExpZCoordinateMagnitudeSquared < *(ExpPerturbationToleranceCheck + iteration))
	    {
	    Point pt(x, y, iteration);
	    glitchPoints[GlitchPointCount] = pt;
	    GlitchPointCount++;
	    glitched = true;
	    break;
	    }
	//use bailout radius of 256 for smooth coloring.
	} while (ExpZCoordinateMagnitudeSquared < ExpBailout && iteration < MaxIteration);

    if (SlopeType == DERIVSLOPE)
	BigProcessDerivativeSlope(ExpDC, ExpTemp, ghMutex, iteration, TrueCol, x, y);

    if (glitched == false)
	{
	int	index;
	double	rqlim2 = sqrt(bailout);
	ExpComplex	ExpW;

	ExpW = *(ExpXSubN + iteration) + ExpDeltaSubN;

	if (wpixels)						// it's not Null, so we must have initialsed it
	    {
	    if (iteration < MaxIteration)
		{
		double	SumSqW;
		floatexp	ExpSumSqW = ExpW.x * ExpW.x + ExpW.y * ExpW.y;
		SumSqW = ExpSumSqW.todouble();
		log_zn = log(SumSqW) / SlopeDegree;
		nu = log(log_zn / log(SlopeDegree)) / log(SlopeDegree);
		FloatIteration = FloatIteration + 2 - nu;
		}
	    else
		FloatIteration = MaxIteration;
	    SlopeIndex = (((DWORD)height - 1 - y) * (DWORD)xdots) + (DWORD)x + width * thread;
	    if (x >= 0 && x < Dib->DibWidth - 1 && y >= 0 && y < Dib->DibHeight - 1 && wpixels)
		*(wpixels + SlopeIndex) = FloatIteration;
	    }

	if (biomorph >= 0)						// biomorph
	    {
	    if (iteration == MaxIteration)
		index = MaxIteration;
	    else
		{
		if (ExpW.x.abs() < rqlim2 || ExpW.y.abs() < rqlim2)
		    index = biomorph;
		else
		    index = iteration % 256;
		}
	    }
	else
	    {
	    floatexp	temp = 0.0;
	    switch (OutsideMethod)
		{
		case NONE:						// no filter
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			//			index = iteration;
//			index = iteration % 256;
			index = iteration % TrueCol->ColoursInPALFile;
		    break;
		case PERT1:						// something Shirom Makkad added
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			{
			ZCoordinateMagnitudeSquared = ExpZCoordinateMagnitudeSquared.todouble();
			index = (int)((iteration - log2(log2(ZCoordinateMagnitudeSquared))) * 5) % 256; //Get the index of the color array that we are going to read from.
			}
		    break;
		case PERT2:						// something Shirom Makkad added
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			{
			ZCoordinateMagnitudeSquared = ExpZCoordinateMagnitudeSquared.todouble();
			index = (int)(iteration - (log(0.5*(ZCoordinateMagnitudeSquared)) - log(0.5*log(256))) / log(2)) % 256;
			}
		    break;
		case ZMAG:
		    if (iteration == MaxIteration)			// Zmag
			index = (int)((ExpW.CSumSqr()) * (MaxIteration >> 1) + 1);
		    else
			index = iteration;
		    //			index = iteration % 256;
		    break;
		case REAL:						// "real"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = iteration + (int)(ExpW.x.todouble()) + 7;
		    break;
		case IMAG:	    					// "imag"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			index = iteration + (int)(ExpW.y.todouble()) + 7;
		    break;
		case MULT:						// "mult"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else if (!(temp == ExpW.y))
			{
			temp = ExpW.x / ExpW.y;
			index = (long)((double)iteration * temp.todouble());
			}
		    else
			index = iteration;
		    break;
		case SUM:						// "sum"
		    if (iteration == MaxIteration)
			index = MaxIteration;
		    else
			{
			temp = ExpW.x + ExpW.y;
			index = iteration + (int)(temp.todouble());
			}
		    break;
		case ATAN:						// "atan"
		    if (iteration == MaxIteration)			// silly bug in BigTrig
			index = MaxIteration;
		    else
			{
			Complex	w;
			w.x = ExpW.x.todouble();
			w.y = ExpW.y.todouble();
			index = (long)fabs(atan2(w.y, w.x)*180.0 / PI);
			}
		    break;
		case POTENTIAL:
		    magnitude = ExpW.CSumSqr();
		    index = Pot.potential(magnitude, iteration, MaxIteration, TrueCol, 256, potparam);
		    break;
		default:
		    if (InsideMethod >= TIERAZONFILTERS)			// suite of Tierazon filters and colouring schemes
			{
			Complex	tempComplex;

			tempComplex.x = ExpW.x.todouble();
			tempComplex.y = ExpW.y.todouble();
			TZfilter->EndTierazonFilter(tempComplex, (long *)&iteration, TrueCol);
			index = iteration;
			}
		    else						// no filter
			{
			if (iteration == MaxIteration)
			    index = MaxIteration;
			else
			    index = iteration % 256;
			}
		    break;
		}
	    }

	if (SlopeType != DERIVSLOPE)
	    {
	    if (*PlotType == FILTERPLOT)
		Plot.FilterPoint(x + width * thread, height - 1 - y, index, &(TZfilter->FilterRGB));
	    else
		Plot.PlotPoint(x + width * thread, height - 1 - y, index);
	    }
	}
    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Reference Zoom Point
//////////////////////////////////////////////////////////////////////

int calculateFrame::ReferenceZoomPoint(BigComplex& centre, int maxIteration, int user_data(HWND hwnd), char* StatusBarInfo, int *pPertProgress, bool Multi)
    {
    // Raising this number makes more calculations, but less variation between each calculation (less chance of mis-identifying a glitched point).
    BigComplex	ZTimes2, Z, SinX, CosX, ScaledZ;
    double	glitchTolerancy = 1e-6;
    BigDouble	TempReal, TempImag;
    BigDouble	zisqr, zrsqr, realimag;

    ReferenceNumber++;
//    if (subtype == 58)			// sin()
//	Z = centre + param[6];
//	Z = centre + HALF_PI;
//    else
	Z = centre;

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

	if (ArithType == FLOATEXP)
	    {
	    BigComplex2ExpComplex(&ExpC, Z);
	    *(ExpXSubN + i) = ExpC;
	    }
/*
	else if (ArithType == DOUBLESHIFT)
	    {
	    ScaledZ = Z / Scaling;
	    C = ScaledZ.CBig2Double();
	    *(XSubN + i) = C;
	    }
*/
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
//	if (user_data(hwnd) < 0)
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
//		InvalidateRect(hwnd, NULL, FALSE);
		}
	    }

	if (ArithType == FLOATEXP)
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
//	if(Z.CSumSqr() > 1000.0)
//	    return(0);

/*
	if (subtype == 58)							// sin()
	    {
	    BigDouble	a, b;
	    a = 80.0;
	    b = -80.0;

	    if (Z.x > a || Z.x < b || Z.y > a || Z.y < b)
		return(0);
	    }
*/
	}
    return 0;
    }
    
