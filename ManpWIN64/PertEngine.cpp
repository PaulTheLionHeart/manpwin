/****************************************************
    PERTENGINE.CPP a module to explore Perturbation
    Thanks to Claude Heiland-Allen https://fractalforums.org/programming/11/perturbation-code-for-cubic-and-higher-order-polynomials/2783
    and Shirom Makkad https://github.com/ShiromMakkad/MandelbrotPerturbation
    for their inspiration and methods of calculating perturbation fractals
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include <windows.h>
#include <time.h>
#include <atomic>
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

extern	std::atomic<bool> gStopRequested;	// force early exit
extern	std::atomic<long> gPixelsDone;
extern	void	ShowBignum(BigDouble x, char *Location);

// constructors
extern	std::vector<float> wpixels;

CPerturbation::CPerturbation()
    : wpixels(::wpixels)
    {}

CPerturbation::CPerturbation(std::vector<float>& wp)
    : wpixels(wp)
    {}

void CPerturbation::AttachSharedTables(const std::vector<Complex>* xs, const std::vector<ExpComplex>* exs, const BLAS* bla)
    {
    XSubN = xs;
    ExpXSubN = exs;
    Bla = bla;
    }

//////////////////////////////////////////////////////////////////////
// Initialisation
//////////////////////////////////////////////////////////////////////

int CPerturbation::initialiseCalculateFrame(CDib *DibIn, CSlope *Slope, int xStartIn, int xEndIn, int HeightIn, int threshold, BigDouble BigCentreXin, BigDouble BigCentreYin, BigDouble BigWidthIn, int decimals, CTZfilter *TZfilter,
		HWND hwndIn, int ThreadIn, std::vector<float> &wpixelsIn, double paramIn[], double potparamIn[], int PaletteShiftIn, int *PlotTypeIn, int SlopeTypeIn, double lightDirectionDegreesIn, double bumpMappingDepthIn,
		double bumpMappingStrengthIn, int PaletteStartIn, double LightHeightIn, int PertColourMethodIn,	int PalOffsetIn, double IterDivIn, bool EnableApproximationIn, BYTE _3dflagIn, double ColourSpeedIn, int NumberThreadsIn)
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

    // compute your desired bitcount from the incoming 'decimals'
    mpfr_prec_t p = std::max<mpfr_prec_t>(30, decimals * SAFETYMARGIN);

    // ensure member mpfr_t's have enough precision BEFORE assignment
    mpfr_prec_round(BigCentreX.x, p, MPFR_RNDN);
    mpfr_prec_round(BigCentreY.x, p, MPFR_RNDN);
    mpfr_prec_round(BigWidth.x, p, MPFR_RNDN);

    // now do the copies — no truncation
    BigCentreX = BigCentreXin;
    BigCentreY = BigCentreYin;
    BigWidth = BigWidthIn;

    _3dflag = _3dflagIn;
    ColourSpeed = ColourSpeedIn;
    smoothing = (ColourSpeed != 0.0);
    NumberThreads = NumberThreadsIn;

    height = HeightIn;
    MaxIteration = threshold;
    EnableApproximation = EnableApproximationIn;
    OutsideMethod = TZfilter->method;
    hwnd = hwndIn;
    thread = ThreadIn;
    wpixels = wpixelsIn;
    Dib = DibIn;
    PaletteShift = PaletteShiftIn;
    for (i = 0; i < NUMPERTPARAM; i++)
	param[i] = paramIn[i];
    for (i = 0; i < 3; i++)
	potparam[i] = potparamIn[i];
    PlotType = PlotTypeIn;
    SlopeType = (_3dflag) ? NOSLOPE  : SlopeTypeIn;
    lightDirectionDegrees = lightDirectionDegreesIn;
    bumpMappingDepth = bumpMappingDepthIn;
    bumpMappingStrength = bumpMappingStrengthIn;
    PaletteStart = PaletteStartIn;
    LightHeight = LightHeightIn;
    PertColourMethod = PertColourMethodIn;
    PalOffset = PalOffsetIn;
    IterDiv = IterDivIn;
    gStopRequested = false; // NEW: reset flag before starting threads

    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Full frame calculation
//////////////////////////////////////////////////////////////////////

int CPerturbation::calculateOneFrame(double bailout, char* StatusBarInfo, int powerin, int InsideMethodIn, int OutsideMethodIn, int biomorphin, int subtypein, Complex rsrAin, bool rsrSignIn, int user_data(HWND hwnd), int xdotsIn, 
	CTZfilter &TZfilter, CTrueCol &TrueCol, int *pPertProgress, BYTE &ThreadComplete, bool Multi, int delay, char *PertErrorMessage, int ArithTypeIn, int MaxRefIterationIn, int SlopeDegreeIn, HANDLE ghMutex)

    {
    BigComplex	BigDelta;
    Complex	delta;
    ExpComplex	ExpDeltaSub0;
    Complex	DeltaSub0;

    xdots = xdotsIn;
    rsrA = rsrAin;
    rsrSign = rsrSignIn;
    power = powerin;
    if (power < 2)
	power = 2;
    if (power > MAXPOWER)
	power = MAXPOWER;
    PascalArray.resize(MAXPOWER + 1);

    InsideMethod = InsideMethodIn;
    OutsideMethod = OutsideMethodIn;
    subtype = subtypein;
    biomorph = biomorphin;
    SlopeDegree = SlopeDegreeIn;
    MaxRefIteration = MaxRefIterationIn;
    ArithType = ArithTypeIn;

    ThreadComplete = false;

    if (SlopeType == DERIVSLOPE)
	{
	Complex	    w;			// unit 2D vector
	w = { 0.0, 2.0 * lightDirectionDegrees * PI / 360.0 };
	v = w.CExp();			// setup derivative 2D light vector v
	}

    // calculate the pascal's triangle coefficients
    LoadPascal(PascalArray, power);

    int		lastChecked = -1;
    double	TotalPoints = (double)(width * height);

    int	    effectiveThreads = (NumberThreads > 0) ? NumberThreads : 1;
    int	    StripMult = 8;

    int	    StripWidth = xdots / (effectiveThreads * StripMult);
    if (StripWidth < 1) 
	StripWidth = 1;

    int TempXdots = ((xdots + StripWidth - 1) / StripWidth) * StripWidth;

    int	    TotalStrips = TempXdots / StripWidth;

    long TotalPixels = (long)xdots * height;

    BigDouble	BigPixelSize = BigWidth / (double)(height * 0.5);	// pixel radius so we need to divide by half height
    double	PixelSize = BigPixelSize.BigDoubleToDouble();

    CPlotmode PlotMethod;

    if (height <= 0)
	{
	_snprintf_s(PertErrorMessage, MAXLINE, _TRUNCATE, "calculateOneFrame has negative height=%d", height);
	return -2;
	}

    auto yOrder = PlotMethod.generateYOrder(height, currentYMode);

    for (size_t yi = 0; yi < yOrder.size(); ++yi)
	{
	int y = yOrder[yi];
	double progress = (double)gPixelsDone / (double)TotalPixels;
	if (int(progress * 100) != lastChecked)
	    {
	    lastChecked = int(progress * 100);
	    char	UseBigWidthStr[12];
	    if (ArithType == DOUBLE || ArithType == DOUBLE || ArithType == DBL_UNSUPPORTED)
		strcpy(UseBigWidthStr, " Duble");
	    else
		strcpy(UseBigWidthStr, " Exp");
	    _snprintf_s(StatusBarInfo, MAXLINE, _TRUNCATE, "Th=%d, (%d%%)%s", thread, lastChecked, UseBigWidthStr);
	    if (Multi)
		{
		*pPertProgress = lastChecked;
		if (delay > 0)
		    {
		    Sleep(delay);
		    InvalidateRect(hwnd, NULL, FALSE);
		    }
		}
	    }

	for (int strip = thread; strip < TotalStrips	; strip += effectiveThreads)
	    {
	    int StripStart = strip * StripWidth;
	    int StripEnd = StripStart + StripWidth;  // NOTE: NOT min(..., xdots)
	    int	iteration = 0;
	    for (int x = StripStart; x < StripEnd; x++)
		{
		if (AbortRequested())
		    return -1;

		if (x >= xdots)		// <-- the whole trick
		    continue;
		gPixelsDone.fetch_add(1, std::memory_order_relaxed);

		if (ArithType == FLOATEXP || ArithType == EXP_UNSUPPORTED)
		    {
		    ExpComplex  zExp;
		    ExpComplex  dcExp;

		    BigDelta.x = BigPixelSize * (double)(x - xdots / 2);
		    BigDelta.y = BigPixelSize * (double)(height / 2 - y);
		    BigComplex2ExpComplex(&ExpDeltaSub0, BigDelta);
		    if ((iteration = iterateFractalWithPerturbationBLAExp(ExpXSubN, MaxIteration, bailout, ExpDeltaSub0.CHalf(), Bla, TZfilter, zExp, dcExp, user_data)) < 0)
			return -1;

		    if (SlopeType == DERIVSLOPE)
			{
			BigProcessDerivativeSlope(dcExp, zExp, ghMutex, MaxIteration, iteration, TrueCol, x, y);
			}
		    else
			// final colour write (one pixel only)
			ColourProcessingExp(zExp, iteration, x, y, TrueCol, TZfilter, bailout);
		    }
		else
		    {
		    Complex	z(0.0, 0.0);
		    Complex	dc(0.0, 0.0);			// dZ/dc accumulator for derivative slope

		    delta.x = PixelSize * (double)(x - xdots / 2);;
		    delta.y = PixelSize * (double)(height / 2 - y);
		    DeltaSub0 = delta / 2.0;
		    if ((iteration = iterateFractalWithPerturbationBLA(XSubN, MaxIteration, bailout, DeltaSub0, Bla, TZfilter, z, dc, user_data)) < 0)
			return -1;
		    if (SlopeType == DERIVSLOPE && (iteration < MaxIteration || InsideMethod == 0)) 
			{
			// Use the same rule as legacy: keep outside slope unless inside filters are active
			ProcessDerivativeSlope(dc, z, ghMutex, MaxIteration, iteration, TrueCol, x, y);
			}
		    else
			{
			ColourProcessing(z, iteration, x, y, TrueCol, TZfilter, bailout);
			}
		    }
		}
	    }
	}

    ThreadComplete = true;
    return 0;
    }

