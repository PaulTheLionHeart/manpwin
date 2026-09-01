/*
    PERTENGINE.CPP - a module to explore Perturbation

    Thanks to Claude Heiland-Allen https://fractalforums.org/programming/11/perturbation-code-for-cubic-and-higher-order-polynomials/2783
    and Shirom Makkad https://github.com/ShiromMakkad/MandelbrotPerturbation
    for their inspiration and methods of calculating perturbation fractals

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

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
CPerturbation::CPerturbation() = default;

//CPerturbation::CPerturbation(std::vector<float>& wp)
//    {}

void CPerturbation::AttachSharedTables(const std::vector<Complex>* xs, const std::vector<ExpComplex>* exs, const BLAS* bla)
    {
    XSubN = xs;
    ExpXSubN = exs;
    Bla = bla;
    }

//////////////////////////////////////////////////////////////////////
// Initialisation
//////////////////////////////////////////////////////////////////////

int CPerturbation::initialiseCalculateFrame(CDib *DibIn, int HeightIn, int threshold, BigDouble BigCentreXin, BigDouble BigCentreYin, BigDouble BigWidthIn, int decimals, int OutsideMethodIn, int InsideMethodIn,
		HWND hwndIn, int ThreadIn, double paramIn[], double potparamIn[], int PaletteShiftIn, int *PlotTypeIn, int SlopeTypeIn, double lightDirectionDegreesIn, double bumpMappingDepthIn,
		double bumpMappingStrengthIn, int PaletteStartIn, double LightHeightIn, int PertColourMethodIn,	int PalOffsetIn, double IterDivIn, bool EnableApproximationIn, BYTE _3dflagIn, double ColourSpeedIn, int NumberThreadsIn)
    {
    Complex q;
    int	    i;

    // compute your desired bitcount from the incoming 'decimals'
    mpfr_prec_t bitcount = decimals * SAFETYMARGIN;

    if (bitcount < 30)
	bitcount = 30;

    if (bitcount > SIZEOF_BF_VARS - 10)
	bitcount = SIZEOF_BF_VARS - 10;

    mpfr_set_default_prec(bitcount);

    // ensure member mpfr_t's have enough precision BEFORE assignment
    mpfr_prec_round(BigCentreX.x, bitcount, MPFR_RNDN);
    mpfr_prec_round(BigCentreY.x, bitcount, MPFR_RNDN);
    mpfr_prec_round(BigWidth.x, bitcount, MPFR_RNDN);

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
    OutsideMethod = OutsideMethodIn;
    InsideMethod = InsideMethodIn;
    hwnd = hwndIn;
    thread = ThreadIn;
    Dib = DibIn;
    PaletteShift = PaletteShiftIn;
    for (i = 0; i < NUMPERTPARAM - 1; i++)	// NUMSLOPEPARAM  - 1 because param[15] is reserved for start colour
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

    if (gManp->OutsideMethod >= TIERAZONFILTERS)
	TZfilter.InitFilter(gManp->OutsideMethod, gManp->threshold, gManp->dStrands, gManp->nFDOption, gManp->UseCurrentPalette);
 
    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Full frame calculation
//////////////////////////////////////////////////////////////////////

int CPerturbation::calculateOneFrame(double bailout, char* StatusBarInfo, int powerin, int InsideMethodIn, int OutsideMethodIn, int biomorphin, int subtypein, Complex rsrAin, bool rsrSignIn, int user_data(HWND hwnd), int xdotsIn, 
	CTrueCol &TrueCol, int *pPertProgress, bool Multi, int delay, char *PertErrorMessage, int ArithTypeIn, int MaxRefIterationIn, int SlopeDegreeIn, PlotMode mode,
	std::vector<std::pair<int, int>> *pixelOrder, std::atomic<int> *workIndex, int totalPixels, HANDLE ghMutex)
    {
    BigComplex	BigDelta;
    Complex	delta;
    ExpComplex	ExpDeltaSub0;
    Complex	DeltaSub0;

    if (Bla == nullptr || XSubN == nullptr || ExpXSubN == nullptr)
	{
	strcpy(PertErrorMessage, "Perturbation engine not initialised");
	return -4;
	}

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

    if (SlopeType == DERIVSLOPE)
	{
	Complex	    w;			// unit 2D vector
	w = { 0.0, 2.0 * lightDirectionDegrees * PI / 360.0 };
	v = w.CExp();			// setup derivative 2D light vector v
	}

    // calculate the pascal's triangle coefficients
    LoadPascal(PascalArray, power);

    int		lastChecked = -1;
    if (height <= 0)		// sanity
	{
	_snprintf_s(PertErrorMessage, MAXLINE, _TRUNCATE, "calculateOneFrame has negative height=%d", height);
	return -2;
	}

    BigDouble	BigPixelSize = BigWidth / (double)(height * 0.5);	// pixel radius so we need to divide by half height
    double	PixelSize = BigPixelSize.BigDoubleToDouble();

     // --------------------------------------------
    // SCHEDULER PATH (ALL NEW MODES)
    // --------------------------------------------
    int		iteration = 0;

    // Optional tuning (safe, simple)
    int chunk = (mode == PlotMode::Tile) ? 1024 : CHUNK_SIZE;
    if (pixelOrder->empty())
	{
	OutputDebugStringA("Pert: pixelOrder empty\n");
	return -1;
	}

    gManp->DumpStartupState("start calculateOneFrame");
    if (pixelOrder != NULL && workIndex != NULL && totalPixels > 0)
	{
	while (true)
	    {
	    // Dynamic work distribution across threads
	    int start = workIndex->fetch_add(chunk, std::memory_order_relaxed);
	    if (start >= totalPixels)
		break;

	    for (int k = 0; k < chunk; k++)
		{
		int idx = start + k;
		if (idx >= totalPixels)
		    break;

		int x, y;

		if (mode == PlotMode::Scanline)
		    {
		    // Compute coordinates directly for linear traversal
		    y = height - 1 - (idx / xdots);
		    x = idx % xdots;
		    }
		else
		    {
		    // Use precomputed traversal order (Tile, Spiral, etc.)
		    x = (*pixelOrder)[idx].first;
		    y = (*pixelOrder)[idx].second;
		    }

		if ((idx & 0x3FF) == 0)
		    {
		    gManp->UpdateProgress(workIndex, totalPixels, StatusBarInfo, NumberThreads);
		    }

		// Allow user abort without stalling threads
		if (AbortRequested())
		    return -1;

		// Fractal pixel computation(performance-critical)
		gPixelsDone.fetch_add(1, std::memory_order_relaxed);

		if (ArithType == FLOATEXP || ArithType == EXP_UNSUPPORTED)
		    {
		    ExpComplex  zExp;
		    ExpComplex  dcExp;

		    BigDelta.x = BigPixelSize * (double)(x - xdots / 2);
		    BigDelta.y = BigPixelSize * (double)(height / 2 - y);
		    BigComplex2ExpComplex(&ExpDeltaSub0, BigDelta);
		    if ((iteration = iterateFractalWithPerturbationBLAExp(ExpXSubN, MaxIteration, bailout, ExpDeltaSub0.CHalf(), Bla, /*TZfilter, */zExp, dcExp, user_data)) < 0)
			return -1;

		    if (SlopeType == DERIVSLOPE)
			{
			BigProcessDerivativeSlope(dcExp, zExp, ghMutex, MaxIteration, iteration, TrueCol, x, y);
			}
		    else
			// final colour write (one pixel only)
			ColourProcessingExp(zExp, iteration, x, y, TrueCol, /*TZfilter, */bailout);
		    }
		else
		    {
		    Complex	z(0.0, 0.0);
		    Complex	dc(0.0, 0.0);			// dZ/dc accumulator for derivative slope

		    delta.x = PixelSize * (double)(x - xdots / 2);
		    delta.y = PixelSize * (double)(height / 2 - y);
		    DeltaSub0 = delta / 2.0;
		    if ((iteration = iterateFractalWithPerturbationBLA(XSubN, MaxIteration, bailout, DeltaSub0, Bla, /*TZfilter, */z, dc, user_data)) < 0)
			return -1;
		    if (SlopeType == DERIVSLOPE && (iteration < MaxIteration || InsideMethod == 0))
			{
			// Use the same rule as legacy: keep outside slope unless inside filters are active
			ProcessDerivativeSlope(dc, z, ghMutex, MaxIteration, iteration, TrueCol, x, y);
			}
		    else
			{
			ColourProcessing(z, iteration, x, y, TrueCol, /*TZfilter, */bailout);
			}
		    }
		}
	    }
	}
    else
	{
	_snprintf_s(PertErrorMessage, MAXLINE, _TRUNCATE, "Scheduler not initialised correctly");
	return -3;
	}
    gManp->DumpStartupState("after initialiseCalculateFrame");
    return 0;
    }


