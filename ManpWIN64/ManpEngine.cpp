/*
    MANPENGINE.CPP a program to investigate the Mandelbrot set. This version dumps
    values of count to file and implements periodic checking. For more 
    information refer to Scientific American August 1985.
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "pixel.h"

/**************************************************************************
	General escape-time engine 
**************************************************************************/

int	CPixel::PerformWorklist(int NumberThreadsIn, int ThreadNumber, BYTE *ThreadComplete, HANDLE *ghMutexIn, int user_data(HWND hwnd))
    {
    int		StripWidth;

    ghMutex = ghMutexIn;
    NumberThreads = NumberThreadsIn;
    multiThreaded = (NumberThreads > 1);
    int	flags = USEPALETTE + ((calcmode != 'F' && type != PERTURBATION && type != SLOPEFORWARDDIFF) ? USEWPIXELS : 0);
    Plot.InitPlot(threshold, TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib->BitsPerPixel, Dib, flags);
    *ThreadComplete = false;
    if (RotationAngle != NORMAL && RotationAngle != 90 && RotationAngle != 180 && RotationAngle != 270)
	{
	double  z_rot = (double)RotationAngle;
	if (*MathType == DOUBLEFLOAT)
	    Mat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, z_rot);
	else if (*MathType == DOUBLEDOUBLE)
	    DDMat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, z_rot);
	else if (QUADDOUBLE)
	    QDMat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, z_rot);
	else
	    BigMat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, z_rot);
	}

    StripWidth = (NumberThreads > 0) ? xdots / NumberThreads : xdots;		// width of vertical strip
    if (BigNumFlag)
	ManageBignumPrecision(precision);					// allow internal bignum variables to track current precision requirements
    if (distest)								// setup stuff for distance estimator
	InitDistEst(&xxmin, &xxmax, &yymin, &yymax, &xx3rd, &yy3rd, &distestwidth, distest);

    if (calcmode == 'F')
	Slope.InitRender(threshold, TrueCol, Dib, wpixels, PaletteShift, bump_transfer_factor, PaletteStart, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength);
    if (RunThread(hwnd, ThreadNumber, StripWidth, &Slope, Fractal, user_data) < 0)
	return -1;
    if (juliaflag && ShowOrbits)
	plot_orbits(OrbitColour, NUM_ORBITS);
    *ThreadComplete = true;
    return 0;
    }

/**************************************************************************
    General multi-threaded escape-time engine
**************************************************************************/

int	CPixel::RunThread(HWND hwnd, int ThreadNumberIn, int StripWidthIn, CSlope *Slope, CFract *Fractal, int user_data(HWND hwnd))
    {
    int	    i;

    num_worklist = 0;
    ThreadNumber = ThreadNumberIn;
    // Thread owns pixels in [xStart, xEnd)
    // Any plot outside this range must be serialized.
    // This is required for symmetry, double-pass modes,
    // and full-screen Julia rendering.

    if (NumberThreads <= 1)
	{
	xStart = 0;
	xEnd = xdots;
	}
    else
	{
	xStart = (ThreadNumber * xdots) / NumberThreads;
	xEnd = ((ThreadNumber + 1) * xdots) / NumberThreads;
	}

    StripWidth = StripWidthIn;
    if (!num_worklist)					// no worklist from saved file
	{						// default setup a new worklist
	iystart = ixstart = yystart = xxstart = yybegin = 0;
	ixstart = xxstart = xStart;
	iystop = yystop = ydots - 1;
	ixstop = xxstop = xEnd - 1;
	num_worklist = 1;
	workpass = 0;
	worklist[0].xxstart = xStart;
	worklist[0].xxstop = xEnd - 1;
	worklist[0].yystart = worklist[0].yybegin = 0;
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

	FindSymmetry(_3dflag, decomp, pairflag, OutsideMethod, invert, CoordSystem, param, *degree, type, subtype, calcmode, RotationAngle, Fractal, hor, vert, mandel_width, BigNumFlag, &Big_xxmin, &Big_xxmax, &Big_yymin,
	    &Big_yymax, BigHor, BigVert, BigWidth, ScreenRatio, &xxmin, &xxmax, &yymin, &yymax, special, juliaflag);

	if (pairflag)
	    init_stereo_pairs(pairflag, AutoStereo_value);			// init stereo pair parameters

	if (InitArithmetic() < 0)			// set up variables depending on maths type
	    return -1;
	if (_3dflag || pairflag)
	    calcmode = '1';				// 3D can only be done on single pass
	if (calcmode == 'F')
	    if (rqlim < 1000.0 && type != KLEINIAN)
		rqlim = 1000.0;	// short term fix?
	if (StandardCalculationMode(hwnd, user_data) < 0)
	    return -1;
	if (*time_to_quit)				// exit cleanly
	    return -1;
	}
    return 0;
    }

/**************************************************************************
    End Threads
**************************************************************************/

void	CPixel::PixelIsExiting(void)
    {
    EndPixel = true;		// advise thread to ternminate
    }

