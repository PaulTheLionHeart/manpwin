/*
    PIXELUTILS.CPP a module for CPixel Utilities. 
    
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
#include	"PixelTemplate.h"


#ifndef sqr
#define sqr(x) ((x)*(x))
#endif

#define OTHER	    0
#define FRACTINT    1
#define MANPWIN	    2

// constructors to enable using references to wpixels
//extern std::vector<float> wpixels;

CPixel::CPixel()
    {
    }

CPixel::CPixel(std::vector<float>& wp)
    {
    }

// ============================================================
// CPixel INITIALISATION PIPELINE
// ============================================================
//
// The following order MUST be preserved:
//
// 1. InitFractalDefinition()
//      - sets type, subtype, degree, rqlim, parameters, Fractal
//
// 2. InitViewport()
//      - sets geometry: ScreenRatio, xdots/ydots,
//        mandel_width, BigHor/BigVert/BigWidth, rotation, Julia seed
//
// 3. InitControlFlags()
//      - sets iteration and behaviour flags
//
// 4. InitArithmeticInputs()
//      - sets BigNumFlag and precision
//
// 5. InitRendering()
//      - sets wpixels, Dib, palette, output buffers
//
// 6. InitRuntimeControl()
//      - thread control, timing, worklist pointers
//
// 7. InitColourProcessing()
//      - palette control, smoothing, log tables, Lyapunov sequence
//
// 8. InitVisualEffects()
//      - star trails, orbit display
//
// 9. InitFilters()
//      - filter parameters (e.g. strands)
//
// 10. InitFwdDiff()
//      - lighting / bump mapping parameters
//
// 11. InitTransformations()
//      - coordinate system, radius, centres, distance estimator params
//
// 12. InitArithmetic()
//      - depends on:
//          • fractal type
//          • viewport (geometry)
//          • BigNumFlag + precision
//      - computes:
//          • MathType
//          • xgap / ygap (or Big equivalents)
//
// 13. InitBailout()
//      - depends on rqlim + BigNumFlag
//
// 14. GeneralInit()
//      - final safety initialisation of internal BigNum variables
//
// NOTE:
// Some parameters (e.g. rqlim, calcmode) may be modified later
// during execution (see ManpEngine.cpp).
// ============================================================

void	CPixel::InitFractalDefinition(WORD typeIn, int subtypeIn, WORD *degreeIn, double rqlimIn, long thresholdIn, int BailoutTestTypeIn, double paramIn[], double potparamIn[], CFract *FractalIn)
    {
    type = typeIn; 
    subtype = subtypeIn;
    degree = degreeIn;
    rqlim = rqlimIn;
    threshold = thresholdIn;
    BailoutTestType = BailoutTestTypeIn;
    for (int i = 0; i < NUMPERTPARAM; i++)
	param[i] = paramIn[i];
    for (int i = 0; i < 3; i++)
	potparam[i] = potparamIn[i];
    Fractal = FractalIn;
    Fn1Index = Fractal->Fn1Index;
    Fn2Index = Fractal->Fn2Index;
    }

void	CPixel::InitViewport(double horIn, double vertIn, double mandel_widthIn, BigDouble BigHorIn, BigDouble BigVertIn, BigDouble BigWidthIn, double ScreenRatioIn, 
		int xdotsIn, int ydotsIn, int RotationAngleIn, Complex RotationCentreIn, Complex jIn)
    {
    hor = horIn;
    vert = vertIn;
    mandel_width = mandel_widthIn;
    BigHor = BigHorIn;
    BigVert = BigVertIn;
    BigWidth = BigWidthIn;
    ScreenRatio = ScreenRatioIn;
    xdots = xdotsIn;
    ydots = ydotsIn;
    RotationAngle = RotationAngleIn;
    RotationCentre = RotationCentreIn;
    j = jIn;
    }

void	CPixel::InitControlFlags(BYTE calcmodeIn, BYTE juliaflagIn, BOOL invertIn, BYTE phaseflagIn, BYTE pairflagIn, BYTE _3dflagIn, int period_levelIn, //int reset_periodIn, 
		int distestIn, int InsideMethodIn, int OutsideMethodIn, int biomorphIn, int nFDOptionIn, BYTE SpecialFlagIn)
    {
    calcmode = calcmodeIn;
    juliaflag = juliaflagIn;
    invert = invertIn;
    phaseflag = phaseflagIn;
    pairflag = pairflagIn;
    _3dflag = _3dflagIn;
    period_level = period_levelIn;
    //reset_period = reset_periodIn;
    distest = distestIn;
    InsideMethod = InsideMethodIn;
    OutsideMethod = OutsideMethodIn;
    biomorph = biomorphIn;
    nFDOption = nFDOptionIn;
    SpecialFlag = SpecialFlagIn;
    }

// ============================================================
// ARITHMETIC INITIALISATION
// ============================================================
//
// IMPORTANT DEPENDENCIES:
//
// This function is NOT standalone. It depends on prior initialisation of:
//
//   - Fractal definition:
//         type (used via fractalspecific[type].flags)
//
//   - Viewport / geometry:
//         ScreenRatio, xdots, ydots
//         mandel_width OR BigWidth
//         BigHor, Big_yymax (for BigNum mode)
//
//   - Arithmetic inputs:
//         BigNumFlag, precision
//
// It computes:
//   - MathType (DOUBLE / DD / QD / Big)
//   - pixel step sizes (xgap/ygap or Big_xgap/Big_ygap)
//   - closeness thresholds
//
// DO NOT reorder this before the above initialisation steps.
// ============================================================
void	CPixel::InitArithmetic(BYTE BigNumFlagIn, int precisionIn)
    {
    BigNumFlag = BigNumFlagIn;
    precision = precisionIn;
    }

void	CPixel::InitRendering(/*std::vector <float> *wpixelsIn, */CDib *DibIn, int widthIn, int PlotTypeIn, CTrueCol *TrueColIn, int colorsIn, BOOL UseCurrentPaletteIn, int *AutoStereo_valueIn, int *symmetryIn)
    {
    // NOTE: wpixels is a reference bound in the constructor.
    // This copies pixel data into the shared buffer; it does NOT rebind the reference.
//    wpixels = wpixelsIn;
    Dib = DibIn;
    width = widthIn;
    PlotType = PlotTypeIn;
    TrueCol = TrueColIn;
    colors = colorsIn;
    UseCurrentPalette = UseCurrentPaletteIn;
    AutoStereo_value = AutoStereo_valueIn;
    symmetry = symmetryIn;
    }

void	CPixel::InitRuntimeControl(int *time_to_zoomIn, int *time_to_restartIn, int *time_to_reinitIn, int *time_to_quitIn, int *blockindexIn, int *totpassesIn, int *curpassIn)
    {
    time_to_zoom = time_to_zoomIn; 
    time_to_restart = time_to_restartIn; 
    time_to_reinit = time_to_reinitIn; 
    time_to_quit = time_to_quitIn;
    blockindex = blockindexIn; 
    totpasses = totpassesIn; 
    curpass = curpassIn;
    }

// ============================================================
// BAILOUT INITIALISATION
// ============================================================
//
// Depends on:
//   - rqlim (from InitFractalDefinition)
//   - BigNumFlag (from InitArithmetic input)
//
// Must be called AFTER both are initialised.
// ============================================================
void	CPixel::InitBailout()		    // needs rqlim + BigNumFlag
    {
    DDBailout = rqlim;
    QDBailout = rqlim;
    if (BigNumFlag)
	BigBailout = rqlim;
    }

void	CPixel::InitColourProcessing(WORD specialIn, WORD coloursIn, int decompIn, int logvalIn, BYTE *logtableIn, char *LyapSequenceIn, double ColourSpeedIn, int PaletteStartIn, int PaletteShiftIn)
    {
    special = specialIn;
    colours = coloursIn;
    decomp = decompIn;
    logval = logvalIn;
    logtable = logtableIn;
    LyapSequence = LyapSequenceIn;
    ColourSpeed = ColourSpeedIn;
    smoothing = (ColourSpeed != 0.0);
    PaletteStart = PaletteStartIn; 
    PaletteShift = PaletteShiftIn;
    }

void	CPixel::InitVisualEffects(BOOL ExpandStarTrailColoursIn, bool ShowOrbitsIn, RGBTRIPLE OrbitColourIn)
    {
    ExpandStarTrailColours = ExpandStarTrailColoursIn;
    ShowOrbits = ShowOrbitsIn;
    OrbitColour = OrbitColourIn;
    }

void	CPixel::InitOutput(HWND hwndIn, long fillcolorIn)
    {
    hwnd = hwndIn;
    fillcolor = fillcolorIn;
    }

void	CPixel::InitFilters(double dStrandsIn)
    {
    dStrands = dStrandsIn;
    }

void	CPixel::InitLightingAndBumpMapping(double bump_transfer_factorIn, double lightDirectionDegreesIn, double bumpMappingDepthIn, double bumpMappingStrengthIn)
    {
    bump_transfer_factor = bump_transfer_factorIn; 
    lightDirectionDegrees = lightDirectionDegreesIn; 
    bumpMappingDepth = bumpMappingDepthIn; 
    bumpMappingStrength = bumpMappingStrengthIn;
    }

void	CPixel::InitTransformations(int CoordSystemIn, double f_radiusIn, double f_xcenterIn, double f_ycenterIn, int distestwidthIn)
    {
    CoordSystem = CoordSystemIn;
    f_radius = f_radiusIn;
    f_xcenter = f_xcenterIn;
    f_ycenter = f_ycenterIn;
    distestwidth = distestwidthIn;
    }

void	CPixel::GeneralInit()             // final safety
    {
    // initialise a number of BigNumvariables that will be used in DD and QD fractals. This prevents NaN in conversion
    aBig = 0.0; a2Big = 0.0; aa3Big = 0.0; bBig = 0.0; l2Big = 0.0; lm5Big = 0.0; lp5Big = 0.0; ozBig = 0.0; t2Big = 0.0; t3Big = 0.0; vBig = 0.0;
    tempBig = 0.0; temp1Big = 0.0; temp2Big = 0.0; temp3Big = 0.0; sqrBig = 0.0; sqrsqrBig = 0.0; realimagBig = 0.0; RealImagSqrBig = 0.0; tBig = 0.0;
    c1Big = 0.0; c2Big = 0.0; cbBig = 0.0; caa3Big = 0.0; z1Big = 0.0; z2Big = 0.0; z3Big = 0.0; z4Big = 0.0; zdBig = 0.0; ztBig = 0.0;
    }

/**************************************************************************
    Smooth the transitions between adjascent pallete colours
**************************************************************************/

// we need to take symmetry into account when calculating smoothing.
/*
RGBTRIPLE CPixel::GetSmoothedColour(double fIter, double color_speed, CTrueCol &TrueCol, CPlot *Plot)
    {
    double  color_bias = 0.0;
    // ---------------------------------------
    // Move through palette smoothly
    // Apply palette speed (default 1.0)
    // ---------------------------------------
    double v = fIter * (color_speed)+color_bias;
    //    RGBTRIPLE* rgbPal = reinterpret_cast<RGBTRIPLE*>(TrueCol.PalettePtr);
    int paletteSize = TrueCol.ColoursInPALFile;

    // Wrap t into [0, paletteSize)
    double pos = fmod(v, TrueCol.ColoursInPALFile);
    if (pos < 0) pos += TrueCol.ColoursInPALFile;

    int i0 = (int)pos;
    int i1 = (i0 + 1) % TrueCol.ColoursInPALFile;

    // Fraction between the two colours
    double t = v - floor(v);

    double frac = pos - i0;
    RGBTRIPLE c0;
    RGBTRIPLE c1;
    Plot->GetRGB(i0, &c0);
    Plot->GetRGB(i1, &c1);

    RGBTRIPLE out;

    out.rgbtRed = BYTE(c0.rgbtRed   * (1.0 - t) + c1.rgbtRed   * t);
    out.rgbtGreen = BYTE(c0.rgbtGreen * (1.0 - t) + c1.rgbtGreen * t);
    out.rgbtBlue = BYTE(c0.rgbtBlue  * (1.0 - t) + c1.rgbtBlue  * t);
    return out;
    }
*/

/**************************************************************************
	Setup symmetry etc
**************************************************************************/

std::string DDToString(const dd_real& v)
    {
    char buf[128];
    snprintf(buf, sizeof(buf), "%.17g", to_double(v)); // temporary
    return std::string(buf);
    }

std::string QDToString(const qd_real& v)
    {
    char buf[128];
    snprintf(buf, sizeof(buf), "%.17g", to_double(v)); // temporary
    return std::string(buf);
    }


int	CPixel::InitArithmetic()
    {
    if (BigNumFlag)
	{
	BigDouble BigScreenRatio(ScreenRatio);
	BigDouble xdotsBig(xdots - 1);
	BigDouble ydotsBig(ydots - 1);

	BigDouble Bigtemp_x = BigScreenRatio / xdotsBig;
	BigDouble Bigtemp_y = BigDouble(1.0) / ydotsBig;

	Big_xgap = BigWidth * Bigtemp_x;
	Big_ygap = BigWidth * Bigtemp_y;
	BigCloseEnough = Big_ygap / 16.0;

	if (precision <= DDPRECISION)
	    {
	    MathType = DOUBLEDOUBLE;
	    if (BigWidth.BigDouble2DD(&DDWidth) < 0) return -1;
	    if (BigHor.BigDouble2DD(&DDHor) < 0) return -1;
	    if (Big_yymax.BigDouble2DD(&DDyymax) < 0) return -1;
	    if (Big_xgap.BigDouble2DD(&DDxgap) < 0) return -1;
	    if (Big_ygap.BigDouble2DD(&DDygap) < 0) return -1;
	    if (BigCloseEnough.BigDouble2DD(&DDCloseEnough) < 0) return -1;

/*
	    _snprintf_s(PositionString, SIZEOF_BF_VARS * 3, _TRUNCATE, "DD: X = %s\r\nYymax = %s\r\nWidth = %s\n", DDToString(DDHor).c_str(), DDToString(DDyymax).c_str(), DDToString(DDWidth).c_str());
	    OutputDebugStringA(PositionString);
	    _snprintf_s(PositionString, SIZEOF_BF_VARS * 3, _TRUNCATE, "DD: xgap = %s\r\nygap = %s\n", DDToString(DDxgap).c_str(), DDToString(DDygap).c_str());
	    OutputDebugStringA(PositionString);
*/
	    }
	else if (precision <= QDPRECISION || fractalspecific[type].flags & FRACTINTINPIXEL || fractalspecific[type].flags & TRIGINPIXEL)    // Bignum versions not yet available
	    {
	    MathType = QUADDOUBLE;
	    if (BigWidth.BigDouble2QD(&QDWidth) < 0) return -1;
	    if (BigHor.BigDouble2QD(&QDHor) < 0) return -1;
	    if (Big_yymax.BigDouble2QD(&QDyymax) < 0) return -1;
	    if (Big_xgap.BigDouble2QD(&QDxgap) < 0) return -1;
	    if (Big_ygap.BigDouble2QD(&QDygap) < 0) return -1;
	    if (BigCloseEnough.BigDouble2QD(&QDCloseEnough) < 0) return -1;
/*
	    _snprintf_s(PositionString, SIZEOF_BF_VARS * 3, _TRUNCATE, "QD: X = %s\r\nYymax = %s\r\nWidth = %s\n", QDToString(QDHor).c_str(), QDToString(QDyymax).c_str(), QDToString(QDWidth).c_str());
	    OutputDebugStringA(PositionString);
	    _snprintf_s(PositionString, SIZEOF_BF_VARS * 3, _TRUNCATE, "QD: xgap = %s\r\nygap = %s\n", QDToString(QDxgap).c_str(), QDToString(QDygap).c_str());
	    OutputDebugStringA(PositionString);
*/
	    }
	else
	    {
	    MathType = ARBITRARYPREC;
	    }
	}
    else
	{
	MathType = DOUBLEFLOAT;
	double	temp_x, temp_y;

	temp_x = ScreenRatio / (double)(xdots - 1);
	temp_y = 1.0 / (double)(ydots - 1);

	xgap = mandel_width * temp_x;
	ygap = mandel_width * temp_y;
	closenuff = ygap / 16.0;
	}
    return 0;
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

    tBig.x.ChangePrecision(precision);  tBig.y.ChangePrecision(precision);

    zBig.x.ChangePrecision(precision); zBig.y.ChangePrecision(precision);
    qBig.x.ChangePrecision(precision); qBig.y.ChangePrecision(precision);
    cBig.x.ChangePrecision(precision); cBig.y.ChangePrecision(precision);

    Big_xxmax.ChangePrecision(precision);
    Big_yymax.ChangePrecision(precision);
    Big_xxmin.ChangePrecision(precision);
    Big_yymin.ChangePrecision(precision);

    realimagBig.ChangePrecision(precision);
    RealImagSqrBig.ChangePrecision(precision);
    BigBailout.ChangePrecision(precision);
    Big_xgap.ChangePrecision(precision);
    Big_ygap.ChangePrecision(precision);
    BigHor.ChangePrecision(precision);
    BigVert.ChangePrecision(precision);
    BigWidth.ChangePrecision(precision);
    }
