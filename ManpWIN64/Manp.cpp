/*
    MANP.CPP a program to investigate the Mandelbrot set. This version dumps
    values of count to file and implements periodic checking. For more 
    information refer to Scientific American August 1985.
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <stdio.h>
#include <string.h>
#include "manp.h"
#include "fractype.h"
#include "fractalp.h"
#include "fract.h"
#include "big.h"
#include <time.h>
#include <sys/timeb.h>
#include "complex.h"
#include "BigDouble.h"
#include "BigComplex.h"
#include "BigTrig.h"
#include "mpfr.h"
#include "slope.h"
#include "pixel.h"
#include "plot.h"
#include "OtherFunctions.h"
#include "SafeStrings.h"

CManp *gManp = nullptr;			// this is a global pointer to the manp object that will slowly absorb most of the code. For now, we make it a pointer that can be used elsewhere

extern	std::atomic<bool> gFatalErrorOccurred;		// abort from DD/QD failure

extern	char	AxesText[];		// used to convert axes and display them
extern	char	LSYSFile[];
extern	char	PARFile[];
extern	char	KFRFile[];
extern	void	FindCursorRealPos(POINTS *);
extern	int	SendCopydataMessage(HWND, char *);

extern	char	*FractData(void);
extern	char	*str_find_ci(char *, char *);

extern	char	lsys_type[];

extern	struct __timeb64 	FrameEnd;
extern	struct __timeb64 	FrameStart;
extern	HWND	CallingWindowHandle;	// Is ManpWIN called by an external window via WM_COPYDATA message?
//extern	int	DataFromPNGFile;	// loaded PNG file?


extern	int	find_count_fp(double, double), DoHenon(void), user_data(HWND),
		Lsystem(HWND, char *), dynam2dfloatsetup(void), dynam2dfloat(void),
		orbit3dfloatsetup(void), orbit3dfloatcalc(void), ifs(HWND), cellular(void), plasma(void), 
		demowalk(void), orbit2dfloat(void), FractintPar(HWND, char *), 
		Fibonacci(void), Fourier(void), RunForm(char *, int), bifurcation(void);
extern	int	NullSetup(void);				// sometimes we just don't want to do anything 
extern	int	rotate(int);

extern	void	set_palette(void), 
		setsymmetry(int, int), DisplayPalette(HWND, BOOL);

extern	int	write_png_file(HWND, char *, char *, char *); 
extern	void	ChangeView(HWND, int, int, int, int, int, int, int, int, char);
extern	void	initFibonacci(void);
extern	int	InitParserArithmetic(void);

/**************************************************************************
	CManp class constructor and destructor
**************************************************************************/

CManp::CManp()
    {
    // Safe now because YOU control when CManp is created (before CreateWindow)
    RefData = new StoreReferenceData();
    pOtherFunctions = &OthFn;
    }

CManp::~CManp()
    {
    delete RefData;
    RefData = nullptr;
    }

/**************************************************************************
	Main Fractal Loop - called from WinMain()
**************************************************************************/

void pfract_main(CManp* manp, HWND hwnd, char* szSaveFileName)
    {
    try
	{
	gManp->RunMainLoop(hwnd, szSaveFileName);
	}
    catch (const char* msg)
	{
	MessageBoxA(hwnd, msg, "ManpWIN Error", MB_OK | MB_ICONERROR);
	}
    catch (...)
	{
	MessageBoxA(hwnd, "Unknown error", "ManpWIN Error", MB_OK | MB_ICONERROR);
	}
    }

/**************************************************************************
    Get Fractal Name
**************************************************************************/

char	*CManp::GetFractalName(void)
    {
    if (type == TIERAZON)
	return (TierazonSpecific[subtype].name+5);
    else if (type == MANDELDERIVATIVES)
	return (MandelDerivSpecific[subtype].name);
    else if (type == PERTURBATION)
	return (PerturbationSpecific[subtype].name);
    else if (type == SLOPEDERIVATIVE)
	return (SlopeDerivSpecific[subtype].name);
    else if (type == SLOPEFORWARDDIFF)
	return (SlopeFwdDiffSpecific[subtype].name);
    else
	return ((*fractalspecific[type].name == '*') ? fractalspecific[type].name + 1 : fractalspecific[type].name);
    }

/**************************************************************************
	Generate  status info
**************************************************************************/

char	*CManp::ShowTime (double time)
    {   
    static	char	TimeString[120];
    int		TimeInt, tenths, sec, min, hr;

    tenths = (long)(time * 10.0) % 10;
    TimeInt = (int)time;
    sec = TimeInt % 60;
    TimeInt /= 60;
    min = TimeInt % 60;
    hr = TimeInt / 60;
    if (hr == 0)
	{
	if (min == 0)
	    SAFE_SPRINTF(TimeString, "%d.%1d sec", sec, tenths);
	else
	    SAFE_SPRINTF(TimeString, "%d min, %d sec", min, sec);
	}
    else
	SAFE_SPRINTF(TimeString, "%d hr, %d min", hr, min);
    return (TimeString);
    }
  
/**************************************************************************
	Track Progress
**************************************************************************/

void	CManp::UpdateProgress(std::atomic<int>* workIndex, int totalPixels, char* StatusBarInfo, int NumThreads)
    {
    static int lastPercent = -1;

    int done = workIndex->load();
    double progress = (double)done / (double)totalPixels;

    int percent = (int)(progress * 100.0 + 0.5);

    if (percent != lastPercent)
	{
	lastPercent = percent;
	_snprintf_s(StatusBarInfo, MAXLINE, _TRUNCATE, "Progress (%d%%), %d Threads", percent, NumThreads);
	}
    }

/**************************************************************************
	Run special plotting mode fractals 
**************************************************************************/

int	CManp::SpecialFractals(HWND hwnd/*, CPixel *Pix*/)
    {
    if (type < 0 || type >= FRACTPAR)
	{
#if defined(_DEBUG)
	char    buf[128];
	SAFE_SPRINTF(buf, "Invalid type [%d] in SpecialFractals\n", type);
	OutputDebugStringA(buf);
#endif
	return -1;
	}
    switch (type)
	{
	case DYNAMICFP:
	case BIFURCATION:
	case BIFSTEWART:
	case BIFLAMBDA:
	case BIFADSINPI:
	case BIFEQSINPI:
	case BIFMAY:
	case LBIFURCATION:
	case LBIFSTEWART:
	case LBIFLAMBDA:
	case LBIFADSINPI:
	case LBIFEQSINPI:
	case LBIFMAY:
	case QUADMAND:
	case DIFFUSION:
	case FOURIER:
	case HENON:
	case MALTHUS:
	case TRIANGLES:
	case GEOMETRY:
	case CIRCLES:
	case IKEDA:
	case CROSSROADS:
	case PASCALTRIANGLE:
	case APOLLONIUS:
	case APOLLONIUSIFS:
	case SIERPINSKIFLOWERS:
	case ZIGZAG:
	case GARGOYLE:
	case CURLICUES:
	case CHUA:
	case CELLULAR:
	case FIBONACCI:
	case PLASMA:
	case DEMOWALK:
	case FPROSSLER:
	case FPPICKOVER:
	case FPLORENZ3D:
	case FPLORENZ3D1:
	case FPLORENZ3D3:
	case FPLORENZ3D4:
	case KAM3DFP:
	case FPLORENZ:
	case FPGINGERBREAD:
	case FPMARTIN:
	case FPHOPALONG:
	case KAMFP:
	case MANDELCLOUD:
	case MOUNTAIN:
	case FFT:
	case BUDDHABROT:
	case POPCORN:
	case PERTURBATION:
	case SLOPEDERIVATIVE:
	case SLOPEFORWARDDIFF:
	case ANT:
	case TOWER:
	case LATOO:
	case CHIP:
	case ICON:
	case QUADRUPTWO:
	case THREEPLY:
	case HAILSTONE:
	    if (fractalspecific[type].flags & OTHERFNINPIXEL)
		{
		OthFn.InitOtherFunctions(type, subtype, hwnd, &TrueCol, &Dib, AntStatus, FrameEnd, FrameStart, mandel_width, hor, vert, AspectRatio, &curpass, &totpasses, user_data, CoordSystem, xAxis, yAxis, zAxis);
		OthFn.RunOtherFunctions(type, &SpecialFlag, &iteration, xdots, ydots, param, threshold, rotate);
		}
	    else
		{
		fractalspecific[type].per_pixel();
		fractalspecific[type].calctype();
		}
	    return 1;

	case OSCILLATORS:
	case FRACTALMAPS:
	case SPROTTMAPS:
	case SURFACES:
	case KNOTS:
	case CURVES:
	    if (PerspectiveFlag && OscAnimProc != EVOLUTION)
		{
		ProcessType TempProc = OscAnimProc;
		OscAnimProc = INITANIM;			// gotta run it twice to initialise rotation centre before doing perspective
		fractalspecific[type].calctype();	// initialise fractal first
		OscAnimProc = RUNANIM;
		fractalspecific[type].calctype();	// now we can run it
		OscAnimProc = TempProc;			// better restore process
		}
	    else
		{
//		init_fractal();
//		OscAnimProc = RUNANIM;
		fractalspecific[type].calctype();
		}
	    return 1;

	case IFS:
	    ifs(hwnd);				// Iterated Function System fractals
	    return 1;

	case LSYSTEM:
	    Lsystem(hwnd, LSYSFile);		// LSystem fractals
	    return 1;

	default:				// none of these fractals, then go to general plotting routines
	    return 0;
	}
    }

/*************************************************************************
	Generate  status info
**************************************************************************/

void	CManp::DisplayStatusBarInfo (int complete, char *text)
    {     
    char	PassStr[MAXLINE];
    char	PositionStr[POSITIONSIZE];				// times 3 because of x, y, width
    char	FinishedStr[MAXLINE];
    char	PrecisionStr[MAXLINE];
    double	ElapsedTime;
    int		VariableSize;

    VariableSize = max(decimals + PRECISION_FACTOR, DBL_MANT_DIG);	// dec = 0 if not BigNum
    PrecisionStr[0] = '\0';						// <-- CRITICAL: always initialize

    if (NonStandardFractal)
	*PassStr = '\0';
    else if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
	_snprintf_s(PassStr, MAXLINE, _TRUNCATE, "Pass %d of %d", curpass, totpasses);
    else
	{
	switch (calcmode)
	    {
	    case 'B':
	    case 'T':
	    case '1':
	    case 'F':
		SAFE_SPRINTF(PassStr, "Pass 1 of 1");
		break;
	    case '2':
	    case 'G':
	    case 'S':
	    case 'V':
	    case 'H':
		SAFE_SPRINTF(PassStr, "Pass %d of %d", curpass, totpasses);
		break;
	    default: 
		*PassStr = '\0';
	    }        
	}
    _ftime64(&FrameEnd);					// initialise time counter
    ElapsedTime = (double)(FrameEnd.time)   + (double)(FrameEnd.millitm)   / 1000.0 
		- (double)(FrameStart.time) - (double)(FrameStart.millitm) / 1000.0;

    GenPositionStr(PositionStr);
    if (complete == INFORMATION)				// information
	{
	if (OscProcess.DisplayAxisImages)
	    {
	    strcpy(szStatus, text);
	    StatusColour = 0x00FFFF00;				// colour of status bar
	    }
	}
    else if (complete == COMPLETE)				// image done
	{
	StatusColour = 0x0000FF00;				// colour of status bar
	if (RunAnimation)					// we completed an animation run
	    RunAnimation = FALSE;
	else if (IsPAR)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Par File <%s> Completed in %s, %s", text, ShowTime(ElapsedTime), PositionStr);
	else if (IsKFR)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "KFR File <%s> Completed in %s, %s", text, ShowTime(ElapsedTime), PositionStr);
	else if (type == LYAPUNOV)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Image Completed in %s, %s, Lyapunov Sequence='%s'", ShowTime(ElapsedTime), PositionStr, LyapSequence);
	else
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Image Completed in %s, %s", ShowTime(ElapsedTime), PositionStr);
//	    SAFE_SPRINTF(szStatus, "Image Completed in %s, %s", ShowTime (ElapsedTime), PositionStr);
	}
    else if (complete == INCOMPLETE)
	{
	SAFE_SPRINTF(FinishedStr, ", Time %s", ShowTime (ElapsedTime));
	if (BigNumFlag)	    // now we have double double and quad double...
	    {
	    if (precision <= DDPRECISION && fractalspecific[type].flags & USEDOUBLEDOUBLE)
		SAFE_SPRINTF(PrecisionStr, "DD Prec: %d", precision);
	    else if (precision <= QDPRECISION && fractalspecific[type].flags & USEDOUBLEDOUBLE)
		SAFE_SPRINTF(PrecisionStr, "QD Prec: %d", precision);
	    else
		{ 
//		if (fractalspecific[type].flags & FRACTINTINPIXEL || fractalspecific[type].flags & TRIGINPIXEL)    // Bignum versions not yet available
//		    SAFE_SPRINTF(PrecisionStr, "QD Prec: %d", precision);
//		else
		    SAFE_SPRINTF(PrecisionStr, "Arb Prec: %d", precision);
		}
	    }
	if (OscAnimProc == MORPHING)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "%s%s", PassStr, FinishedStr);
	else if (type == PERTURBATION)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "%s%s, Arith=%s, %s", PertStatus, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr);
	else if (type == SLOPEDERIVATIVE || type == SLOPEFORWARDDIFF)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "%s%s, Arith=%s, %s", SlopeStatus, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr);
	else if (type == ANT || type == TOWER)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Progress = %s", AntStatus);
	else if (type == LYAPUNOV)
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "%s%s, Arith=%s, %s, Lyapunov Sequence='%s'", PassStr, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr, LyapSequence);
	else
	    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "%s%s, Arith=%s, %s", PassStr, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr);
	if (str_find_ci(szStatus, "Ref=") != 0)
	    StatusColour = 0x00FFFF80;
	else
	    StatusColour = 0x0000FFFF;
	}
    else if (complete == CALCULATINGREF)
	{
	SAFE_SPRINTF(FinishedStr, ", Time %s", ShowTime(ElapsedTime));
	SAFE_SPRINTF(PrecisionStr, "Arb Prec: %d", precision);
	_snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "%s%s, Arith=%s, %s", PertStatus, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr);
	StatusColour = 0x00FFFF80;
	}
    else if (complete == CLOSINGTHREADS)			// so the user knows what's happening
	{
	SAFE_SPRINTF(FinishedStr, ", Time %s", ShowTime(ElapsedTime));
	_snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Waiting For Render Threads To Close..., %s", FinishedStr);
	StatusColour = 0x80FFFF80;
	}
    else							// initialising
	{
	_snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Initialising fractal variables");
	StatusColour = 0x006060FF;
	}
    }

/*************************************************************************
	Generate  status info
**************************************************************************/

void	CManp::GenPositionStr (char *PositionStr)
    {     
    double	centrex, centrey;
//    BigDouble	Big_centrex, Big_centrey;

    if (!PositionStr) return;
    PositionStr[0] = '\0';   // <-- CRITICAL: always initialize

    if (type == FOURIER)					// there's no fractal dimensions here
	return;
//	_snprintf_s(s, MAXLINE, _TRUNCATE, "Hor = %f, Vert = %f, Width = %f", hor, vert, mandel_width); 
    if (BigNumFlag)
	{
	BigDouble Big_centrex(BigHor);
	BigDouble Big_centrey(BigVert);

	BigDouble temp;
	BigDouble factor;
	BigDouble denom;

	factor = BigDouble((double)width);
	denom = BigDouble((double)(2 * height));
	factor = factor / denom;

	// Big_centrex = BigHor + BigWidth * factor
	temp = BigWidth;
	temp *= factor;
	Big_centrex += temp;

	// Big_centrey = BigVert + BigWidth / 2
	temp = BigWidth;
	temp = temp.BigHalf();
	Big_centrey += temp;


	char buf1[256], buf2[256], buf3[256];

	mpfr_snprintf(buf1, sizeof(buf1), "%.36Rf", Big_centrex.x);
	mpfr_snprintf(buf2, sizeof(buf2), "%.36Rf", Big_centrey.x);
	mpfr_snprintf(buf3, sizeof(buf3), "%.12Re", BigWidth.x);
	_snprintf_s(PositionStr, POSITIONSIZE, _TRUNCATE, "X = %s,Y = %s,Width = %s", buf1, buf2, buf3);
	}
    else 
	{
	if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)		// don't show centre, show corner
	    _snprintf_s(PositionStr, POSITIONSIZE, _TRUNCATE, "X=%14.14f,Y=%14.14f,Width=%14.14f", hor, vert, mandel_width);
	else
	    {
	    centrex = hor + (mandel_width * ((double) width / (double) (2 * height)));
	    centrey = vert + (mandel_width / 2.0);
	    if (mandel_width > 1e-13)
		_snprintf_s(PositionStr, POSITIONSIZE, _TRUNCATE, "X=%14.14f,Y=%14.14f,Width=%14.14f", centrex, centrey, mandel_width);
	    else
		_snprintf_s(PositionStr, POSITIONSIZE, _TRUNCATE, "X=%14.14f,Y=%14.14f,BigWidth=%14e", centrex, centrey, mandel_width);
	    }
	}
    }
    
/**************************************************************************
	Create Fractal Name
**************************************************************************/

void	CManp::CreateFractalName(BOOL UseszAppName, char *Name)
    {
    char		*szAppName = "Paul's Fractals";
    char		SubData[1200];
    char		PrecisionData[20];
    char		FilterString[20];
    char		ArithString[20];
    int			Arith = GetArithType();
    switch (Arith)
	{
	case DOUBLEFLOAT:
	    strcpy(ArithString, "Arith=Float");
	    break;
	case DOUBLEDOUBLE:
	    strcpy(ArithString, "Arith=DD");
	    break;
	case QUADDOUBLE:
	    strcpy(ArithString, "Arith=QD");
	    break;
	default:
	    strcpy(ArithString, "Arith=Bignum");
	    break;
	}

    if (Fractal.NumFunct == 1)
	SAFE_SPRINTF(SubData, "Fn=%s", Fractal.Fn1);
    else if (Fractal.NumFunct == 2 && type != OSCILLATORS)			// we use NumFunct to display dimensions
	SAFE_SPRINTF(SubData, "Fn1=%s,Fn2=%s", Fractal.Fn1, Fractal.Fn2);
    else if (type == FORMULA || type == LSYSTEM || type == FRACTPAR || type == IFS)
	SAFE_SPRINTF(SubData, "Sub=%s", lsys_type);
    else if (type == TIERAZON || type == CROSSROADS || type == ZIGZAG || type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
	SAFE_SPRINTF(SubData, "Sub=%d", subtype);
    else if (type == CUBIC)
	{
	char	ch;
	if (param[0] == 0.0)   ch = 'B';
	if (param[0] == 1.0)   ch = 'C';
	if (param[0] == 2.0)   ch = 'F';
	if (param[0] == 3.0)   ch = 'K';
	SAFE_SPRINTF(SubData, "Sub=C%cIN", ch);
	}
    else if (type == MALTHUS || type == TRIANGLES || type == GEOMETRY || type == CIRCLES || type == PASCALTRIANGLE)
	SAFE_SPRINTF(SubData, "Sub=%c", subtype);
    else if (type == SCREENFORMULA)
	SAFE_SPRINTF(SubData, "Frm=<%s>", FormulaString);
    else 
	SAFE_SPRINTF(SubData, "Sub=%d", subtype);
    if (BigNumFlag)
	SAFE_SPRINTF(PrecisionData, "Precision=%d", precision);
    else
	SAFE_SPRINTF(PrecisionData, "");

    if (InsideMethod > NONE || OutsideMethod > NONE)
	{
	if (OutsideMethod > TIERAZONCOLOURS)
	    SAFE_SPRINTF(FilterString, "TZColour=%d, ", ColourMethod);
	else if (OutsideMethod > TIERAZONFILTERS)
	    SAFE_SPRINTF(FilterString, "TZFilter=%d, ", FilterType);
	else if (InsideMethod > NONE)
	    SAFE_SPRINTF(FilterString, "Filter=%d, ", InsideMethod);
	else
	    SAFE_SPRINTF(FilterString, "Filter=%d, ", OutsideMethod);
	}
    else
	SAFE_SPRINTF(FilterString, "");
	              
    if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
	{
	_snprintf_s(Name, 6400, _TRUNCATE, "%s: Thresh=%d, Plot=%c, Log=%d, Fract=%s, %s, Max Dim=%d, Co-ordSys=%s, Axes=%s",
	    (UseszAppName) ? (LPSTR)szAppName : "", threshold, calcmode, logval, GetFractalName(), SubData, MaxDimensions, CoordSysInfo.CoordSys[CoordSystem], AxesText);
	}
    else if (type == PERTURBATION)
	_snprintf_s(Name, 6400, _TRUNCATE, "%s: Thresh=%d, %sFract=%s-%s, %s, NumThreads=%d, %s, Deg=%d, %s",
	(UseszAppName) ? (LPSTR)szAppName : "", threshold, FilterString, (EnableApproximation) ? "(Pert-BLA)" : "(Pert)", GetFractalName(), SubData, NumberThreads, ArithString, degree, PrecisionData);
    else if (type == SLOPEDERIVATIVE)
	_snprintf_s(Name, 6400, _TRUNCATE, "%s: Thresh=%d, Fract=(Slope Der)-%s, %s, NumThreads=%d, %s, Deg=%d, %s",
	(UseszAppName) ? (LPSTR)szAppName : "", threshold, GetFractalName(), SubData, NumberThreads, ArithString, degree, PrecisionData);
    else if (type == SLOPEFORWARDDIFF)
	_snprintf_s(Name, 6400, _TRUNCATE, "%s: Thresh=%d, Fract=(Slope Fwd)-%s, %s, NumThreads=%d, %s, Deg=%d, %s",
	(UseszAppName) ? (LPSTR)szAppName : "", threshold, GetFractalName(), SubData, NumberThreads, ArithString, degree, PrecisionData);
    else
	{
	_snprintf_s(Name, 6400, _TRUNCATE, "%s: Thresh=%d, Plot=%c, NumThreads=%d, %sLog=%d, Fract=%s, %s, Jul=%c, %s, Deg=%d, Spec=%d, 3D=%c, %s",
	    (UseszAppName) ? (LPSTR)szAppName : "", threshold, calcmode, NumberThreads, FilterString, logval, GetFractalName(), SubData,
	    ((juliaflag) ? 'T' : 'F'), ArithString, degree, special, ((_3dflag) ? 'T' : 'F'), PrecisionData);
	}
    }

/**************************************************************************
	Get Arith Type
**************************************************************************/

int 	 CManp::GetArithType()
    {
    int	flags = fractalspecific[type].flags;
    if (BigNumFlag)
	{
	if (precision <= DDPRECISION)
	    MathType = DOUBLEDOUBLE;
	else if (precision <= QDPRECISION || fractalspecific[type].flags & FRACTINTINPIXEL || fractalspecific[type].flags & TRIGINPIXEL)    // Bignum versions not yet available
	    MathType = QUADDOUBLE;
	else
	    MathType = ARBITRARYPREC;
	}
    else
	MathType = DOUBLEFLOAT;
    return MathType;
    }

/**************************************************************************
	Initialise Special Colour for Art Matrix Cubic
**************************************************************************/

void CManp::SetupSpecialColourIndex(CTrueCol &TrueCol, long threshold, RGBTRIPLE SpecialColour, RGBTRIPLE &oldColour, int &SPECIALINDEX)
    {
    int paletteSize = (int)TrueCol.PalettePtr.size();

    if (threshold - 1 < paletteSize)
	SPECIALINDEX = threshold - 1;
    else
	SPECIALINDEX = paletteSize - 1;  // fallback
    oldColour = TrueCol.PalettePtr[SPECIALINDEX];  // save
    if (SPECIALINDEX >= 0 && SPECIALINDEX < MAXPALETTE)
	TrueCol.PalettePtr[SPECIALINDEX] = SpecialColour;
    }

/**************************************************************************
	Initialise Manp engine
**************************************************************************/

int	CManp::InitPixelFractal()
    {
    int		i;
    static	bool	FirstTime = true;

    if (NumberThreads > MAXTHREADS)
	NumberThreads = MAXTHREADS;

    if (FirstTime)
	{
	for (i = 0; i < NumberThreads; i++)
	    pPixelDataArray[i] = nullptr;			// Ensure address is not reset before being allocated.
	FirstTime = false;
	}

    for (i = 0; i < NumberThreads; i++)
	Pixel[i]->EndPixel = false;				// We're not exiting yet

    gFatalErrorOccurred.store(false);
    gStopRequested.store(false, std::memory_order_relaxed);	// NEW: reset flag before starting threads
    CurrentRenderMode = RENDER_PIXEL;
    RGBTRIPLE   SpecialColour;
    SpecialColour.rgbtRed = (BYTE)param[1];
    SpecialColour.rgbtGreen = (BYTE)param[2];
    SpecialColour.rgbtBlue = (BYTE)param[3];

    if (type == CUBIC)
	SetupSpecialColourIndex(TrueCol, threshold, SpecialColour, oldColour, SPECIALINDEX);
    return 0;
    }

/**************************************************************************
	Setup bailout values to default
**************************************************************************/

void    CManp::SetupBailoutDefaults()
    {
    DDBailout = rqlim;
    QDBailout = rqlim;
    BigBailout = rqlim;
    }

/**************************************************************************
	Display Parameters
**************************************************************************/

void	CManp::DisplayFractal(HWND hwnd)
    {
    char    Name[6400];

    CreateFractalName(TRUE, Name);
    SetWindowText(hwnd, Name);
    }

/**************************************************************************
	How about the rest of them
**************************************************************************/

void CManp::WaitForAllThreadsToFinish()
    {
    if (!RunAnimation && !DisplayAnimation && CurrentRenderMode != NOMULTITHREAD)
	{
	DisplayStatusBarInfo(CLOSINGTHREADS, "");
	OutputStatusBar(GlobalHwnd);
	}

    auto WaitAndClose = [](std::vector<HANDLE>& threads)
	{
	for (size_t i = 0; i < threads.size(); i++)
	    {
	    if (threads[i] != NULL)
		{
		WaitForSingleObject(threads[i], INFINITE);
		CloseHandle(threads[i]);
		threads[i] = NULL;
		}
	    }
	};

    switch (CurrentRenderMode)
	{
	case RENDER_PIXEL:
	    WaitAndClose(hPixelThread);
	    break;

	case RENDER_PERT:
	    WaitAndClose(hThread);
	    break;

	case RENDER_SLOPE:
	    WaitAndClose(hSlopeThread);
	    break;

	case NOMULTITHREAD:
	default:
	    break;
	}

    CurrentRenderMode = NOMULTITHREAD;
    }

/**************************************************************************
	Get all the variables into the pixel objects
**************************************************************************/

void CManp::InitPixelObjects(int threadCount, HWND hwnd)
    {
    for (int i = 0; i < threadCount; i++)
	{
	if (BigNumFlag)
	    Pixel[i]->ManageBignumPrecision(decimals);

	Pixel[i]->InitFractalDefinition(type, subtype, &degree, rqlim, threshold, BailoutTestType, param, potparam, &Fractal);

	Pixel[i]->InitControlFlags(calcmode, juliaflag, invert, phaseflag, pairflag, _3dflag,
	    period_level, /*reset_period, */distest, InsideMethod, OutsideMethod,
	    biomorph, nFDOption, SpecialFlag);

	Pixel[i]->InitViewport(hor, vert, mandel_width, BigHor, BigVert, BigWidth,
	    AspectRatio, xdots, ydots, RotationAngle, RotationCentre, j);

	Pixel[i]->InitArithmetic(BigNumFlag, precision);

	Pixel[i]->InitRendering(/*wpixels, */&Dib, width, PlotType, &TrueCol,
	    colors, UseCurrentPalette, &AutoStereo_value, &symmetry);

	Pixel[i]->GeneralInit();
	Pixel[i]->InitBailout();

	Pixel[i]->InitRuntimeControl(&time_to_zoom, &time_to_restart,
	    &time_to_reinit, &time_to_quit,
	    &blockindex, &totpasses, &curpass);

	Pixel[i]->InitColourProcessing(special, colours, decomp, logval,
	    logtable, LyapSequence, ColourSpeed,
	    PaletteStart, PaletteShift);

	Pixel[i]->InitVisualEffects(ExpandStarTrailColours, ShowOrbits, OrbitColour);

	Pixel[i]->InitOutput(hwnd, fillcolor);
	Pixel[i]->InitFilters(dStrands);

	Pixel[i]->InitTransformations(CoordSystem, f_radius, f_xcenter, f_ycenter, distestwidth);

	Pixel[i]->InitLightingAndBumpMapping(bump_transfer_factor,
	    lightDirectionDegrees,
	    bumpMappingDepth,
	    bumpMappingStrength);
	}
    }

void	CManp::CreatePixelObjects(int threadCount)
    {
    Pixel.clear();
    Pixel.reserve(threadCount);
    for (int i = 0; i < threadCount; ++i)
	{
	Pixel.emplace_back(std::make_unique<CPixel>());
	}
    }

/**************************************************************************
	General escape-time engine - sets everything up
**************************************************************************/

int	CManp::RunEscapeTimeEngine(HWND hwnd)
    {
    int		SpecialFunctionsFlag;
    int		ActualNumThreads = NumberThreads;				// make sure formula parser only uses one thread as it isn't set up for multi-thread yet

    int threadCount = NumberThreads;

    if (_3dflag && type != PERTURBATION)
	threadCount = 1;

    if (threadCount < 1)
	threadCount = 1;
    if (threadCount > MAXTHREADS)
	threadCount = MAXTHREADS;

    WaitForAllThreadsToFinish();
    CreatePixelObjects(threadCount);
    InitThreadArrays(threadCount);
    InitPixelObjects(threadCount, hwnd);					// okay, we have to get the globals into the Pixel object somehow

    //#define debug   1
    #ifdef debug
    size_t	PlotSize, PixelSize, SlopeSize, Others;

    PlotSize = sizeof(CPlot);
    PixelSize = sizeof(CPixel);
    SlopeSize = sizeof(CSlope);
    Others = sizeof(COtherFunctions);
    #endif


    if (Pixel.size() > 0 && Pixel[0] != nullptr)
	{
	if (logval)
	    init_log(hwnd);							// log colour distribution
	if (_3dflag)
	    Pixel[0]->init3d(xdots, ydots, x_rot, y_rot, z_rot, sclx, scly, sclz, threshold, hor, vert);				// init 3D parameters 
	InitPixelFractal();							// Initialise Manp engine
	}
    else
	{
	OutputDebugStringA("Pixel objects not initialised\n");
	NumberThreads = ActualNumThreads;
	return -1;
	}

   if (type < 0 || type >= FRACTPAR)
	{
#if defined(_DEBUG)
	char    buf[128];
	SAFE_SPRINTF(buf, "Invalid type [%d] in RunEscapeTimeEngine before SpecialFractals()\n", type);
	OutputDebugStringA(buf);
#endif
	NumberThreads = ActualNumThreads;
	return -1;
	}

    NonStandardFractal = FALSE;
    SpecialFunctionsFlag = SpecialFractals(hwnd/*, Pixel[0].get()*/);		// non-"standard" fractals - no multi-threading
    if (SpecialFunctionsFlag == -1)
	{
	NumberThreads = ActualNumThreads;					// for types other than formula
	return -1;
	}
    else if (SpecialFunctionsFlag == 1)
	{
	NonStandardFractal = TRUE;
	gManp->CurrentRenderMode = NOMULTITHREAD;				// no threads so make sure we don't wait for them to finish
	NumberThreads = ActualNumThreads;					// for types other than formula
	return 1;								// finished processing non-standard plotted fractal
	}

    if (type == SCREENFORMULA || type == FORMULA)
	{
	if (InitParserArithmetic() < 0)						// we have to load different routines for each type of arithmetic, so let's test for it and init function pointers
	    {
	    NumberThreads = ActualNumThreads;					// for types other than formula
	    return -1;
	    }
	// CRITICAL STEP
	for (int i = 0; i < threadCount; ++i)
	    Pixel[i]->m_parser.InitContextFromGlobals();
	}
    DoPixelFractal(hwnd, distestwidth, threadCount);
    NumberThreads = ActualNumThreads;						// for types other than formula
    return 0;
    }

/**************************************************************************
	Handles loading a new fractal setup and schedules reinitialisation
**************************************************************************/

void CManp::HandleLoadPhase(HWND hwnd)
    {
    time_to_load = FALSE;
    time_to_reinit = TRUE;
    time_to_restart = FALSE;
    }

/**************************************************************************
	Handles the restart phase of the fractal lifecycle:
	runs the worklist, updates display, handles autosave and completion
**************************************************************************/

void CManp::HandleRestartPhase(HWND hwnd, char* szSaveFileName)
    {
    _ftime64(&FrameStart);
    time_to_restart = FALSE;
    finished = FALSE;

    UpdateInit();
    SetupBailoutDefaults();

    if (!DataFromPNGFile)
	RunEscapeTimeEngine(hwnd);

    if (time_to_quit)
        return;

    DisplayPalette(hwnd, TrueCol.DisplayPaletteFlag);

    finished = TRUE;
    DisplayFractal(hwnd);

    if (RealTimeJuliaFlag && !juliaflag)
	{
        POINTS CursorLocShort;
        InitRTJulia(hwnd);
        FindCursorRealPos(&CursorLocShort);
        DrawJulia(hwnd, CursorLocShort);
	}

    if (!bTrack)
	{
        ChangeView(hwnd,
            (short)-GetScrollPos(hwnd, SB_HORZ),
            (short)-GetScrollPos(hwnd, SB_VERT),
            (short)ptSize.x, (short)ptSize.y,
            0, 0,
            (short)width, (short)height,
            TRUE);

        InvalidateRect(hwnd, &r, FALSE);
	}

    if (AutoExitFlag)
        time_to_quit = TRUE;

    if (AutoSaveFlag)
	{
        char s[360];
        if (write_png_file(hwnd, szSaveFileName, "ManpWIN", FractData()) < 0)
	    {
            _snprintf_s(s, 360, _TRUNCATE,
                "Error: Could not write file: <%s>", szSaveFileName);
            MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
            MessageBeep(0);
            AutoSaveFlag = false;
	    }
	}

    if (CallingWindowHandle)
        SendCopydataMessage(hwnd, "Send CopyData");

    UpdateClose();

    DisplayStatusBarInfo(COMPLETE,
        ((IsPAR) ? PARFile : (IsKFR) ? KFRFile : ""));
    }

/**************************************************************************
	State machine that drives the fractal lifecycle (load, init, render, restart)
**************************************************************************/

void CManp::HandleReinitPhase(HWND hwnd, char* szSaveFileName)
    {
    time_to_reinit = FALSE;

    HCURSOR hCursor = LoadCursor(NULL, IDC_WAIT);
    SetCursor(hCursor);

    if (!gManp->addflag && !DataFromPNGFile)
	ClearScreen();

    hCursor = LoadCursor(NULL, IDC_ARROW);
    SetCursor(hCursor);

    DisplayFractal(hwnd);

    if (DataFromPNGFile)
	DataFromPNGFile = FALSE;
    else
	time_to_restart = TRUE;
    }

/**************************************************************************
	State machine that drives the fractal lifecycle
	init -> render -> display -> restart -> animation -> exit
**************************************************************************/

void CManp::HandleFractalStateMachine(HWND hwnd, char* szSaveFileName)
    {
    if (time_to_quit)
        return;

    if (time_to_load)
	{
	HandleLoadPhase(hwnd);
	}

    if (time_to_reinit || time_to_zoom)
	{
	time_to_zoom = false;
	HandleReinitPhase(hwnd, szSaveFileName);
	}

    if (time_to_restart)
	{
	HandleRestartPhase(hwnd, szSaveFileName);
	}
    }

/**************************************************************************
    Initialise metadata before we inititialise a new fractal or we load a fractal from a file or image
**************************************************************************/

void CManp::RebuildFractalMetadata(WORD type, int subtype)
    {
    Fractal.NumParam = 0;
    for (int i = 0; i < NUMPARAM; i++)					// make sure to clear out old data
	Fractal.ParamName[i] = "";
    switch (type)
	{
	case PERTURBATION:
	    Fractal.NumParam = PerturbationSpecific[subtype].numparams;
	    for (int i = 0; i < NUMPERTPARAM; i++)
		Fractal.ParamName[i] = PerturbationSpecific[subtype].paramname[i];
	    break;
	case MANDELDERIVATIVES:
	    Fractal.NumParam = fractalspecific[type].numparams;
	    for (int i = 0; i < NUMALTERNATIVEPARAM; i++)
		Fractal.ParamName[i] = fractalspecific[type].paramname[i];	// setup defaults first and then override
	    switch (subtype)
		{
		case 2:
		case 4:
		case 7:
		    Fractal.NumParam = 3;
		    Fractal.ParamName[0] = "Polynomial Degree (>= 2)";
		    Fractal.ParamName[1] = "Real Perturbation of Z(0)";
		    Fractal.ParamName[2] = "Imaginary Perturbation of Z(0)";
		    break;
		case 6:
		    Fractal.NumParam = 4;
		    Fractal.ParamName[0] = "Polynomial Degree (>= 2)";
		    Fractal.ParamName[1] = "Real Perturbation of Z(0)";
		    Fractal.ParamName[2] = "Imaginary Perturbation of Z(0)";
		    Fractal.ParamName[3] = "Rotated? (Only for degree > 2, 1=yes 0=no)";
		    break;
		case 55:
		case 56:
		    Fractal.NumParam = 3;
		    Fractal.ParamName[0] = (subtype == 55) ? "SimonBrot Order (>= 2)" : "SimonBrot2 Order (>= 2)";
		    Fractal.ParamName[1] = "Real Perturbation of Z(0)";
		    Fractal.ParamName[2] = "Imaginary Perturbation of Z(0)";
		    break;
		}
	    break;
	case SLOPEDERIVATIVE:
	    Fractal.NumParam = SlopeDerivSpecific[subtype].numparams;
	    Fractal.ParamName[0] = "Light Source Angle (degrees)";
	    Fractal.ParamName[1] = "Light Source Height";
	    Fractal.ParamName[2] = "Start Palette 0 - Max Iterations";

	    switch (subtype)
		{
		case 3:
		    Fractal.ParamName[3] = "0=multiply, else=plus";
		    Fractal.ParamName[4] = "Start value for z";
		    break;
		case 2:
		case 6:
		case 10:
		case 12:
		    Fractal.ParamName[3] = "Polynomial Degree (>= 0)";
		    break;
		case 7:
		    Fractal.ParamName[3] = "First Polynomial Degree (>= 0)";
		    Fractal.ParamName[4] = "Second Polynomial Degree (>= 0)";
		    Fractal.ParamName[5] = "Third Polynomial Degree (>= 0)";
		    break;
		case 13:
		    Fractal.ParamName[3] = "0=CBIN, 1=CCIN, 2=CFIN, 3=CKIN";
		    Fractal.ParamName[4] = "Special Colour Red Component";
		    Fractal.ParamName[5] = "Special Colour Green Component";
		    Fractal.ParamName[6] = "Special Colour Blue Component";
		    break;
		}
	    break;
	case SLOPEFORWARDDIFF:
	    Fractal.NumParam = SlopeFwdDiffSpecific[subtype].numparams;
	    Fractal.ParamName[0] = "Transfer Factor";
	    Fractal.ParamName[1] = "Start Palette 0 - Max Iterations";
	    Fractal.ParamName[2] = "Light Direction (Degrees)";
	    Fractal.ParamName[3] = "Mapping Depth";
	    Fractal.ParamName[4] = "Mapping Strength";
	    Fractal.ParamName[5] = "Palette Shift";

	    switch (subtype)
		{
		case 4:
		    Fractal.ParamName[5] = "0=multiply, else=plus";
		    Fractal.ParamName[6] = "Start value for z";
		    break;
		case 3:
		case 7:
		case 11:
		case 13:
		    Fractal.ParamName[5] = "Polynomial Degree (>= 0)";
		    break;
		case 8:
		    Fractal.ParamName[5] = "First Polynomial Degree (>= 0)";
		    Fractal.ParamName[6] = "Second Polynomial Degree (>= 0)";
		    Fractal.ParamName[7] = "Third Polynomial Degree (>= 0)";
		    break;
		case 15:
		    Fractal.ParamName[5] = "0=CBIN, 1=CCIN, 2=CFIN, 3=CKIN";
		    Fractal.ParamName[6] = "Special Colour Red Component";
		    Fractal.ParamName[7] = "Special Colour Green Component";
		    Fractal.ParamName[8] = "Special Colour Blue Component";

		    break;
		case 16:
		    Fractal.ParamName[5] = "Polynomial Degree (>= 2)";
		    break;
		}
	    break;
	case TIERAZON:						// Tierazon fractals
	    Fractal.NumParam = fractalspecific[type].numparams;
	    for (int i = 0; i < NUMALTERNATIVEPARAM; i++)
		Fractal.ParamName[i] = fractalspecific[type].paramname[i];	// setup defaults first and then override
	    switch (subtype)
		{
		case 10:					// z=z*z*z+c
		case 11:					// z=z*z*z*z+c
		case 12:					// z=z*z*z*z*z+c
		case 13:					// z=z*z*z*z*z*z+c
		case 26:					// Newton/Mandel, 5th order Newton Mset
		case 27:					// Newton/Mandel, 7th order Newton Mset
		case 28:					// Newton/Mandel, 9th order Newton Mset
		case 29:					// Newton/Mandel, 13th order Newton Mset
		case 30:					// Newton/Mandel, 8th order Newton Mset
		case 31:					// Newton/Mandel, Newton Diamond
		case 32:					// Newton/Mandel, Newton Pentagon
		case 33:					// Newton/Mandel, Newton Hexagon
		case 34:					// Newton/Mandel, Newton Octagon
		case 35:					// Newton/Mandel, 9th order Newton Flower
		case 52:					// Newton/Mandel, 8th order Newton flower
		case 53:					// Newton/Mandel, 6th order Newton Mset
		case 54:					// More Newton Msets, 15th order Newton Mset flower
		case 55:					// More Newton Msets, 4th order Newton's apple
		case 56:					// More Newton Msets, 25th order Newton Mset flower
		case 57:					// More Newton Msets, 38th order Newton Mset flower
		case 58:					// More Newton Msets, 50th order Newton Mset flower
		case 59:					// More Newton Msets, 5th order Newton Mset
		case 84:					// Quartets, t=0; z1=z; z=z*z*z*z-t*t*t*t+c; t=z1
		case 85:					// Quartets, z2=z; z=(z^4)+c; c=z2
		case 96:					// Quartets, z1=z; z=z*z*z*z+z2/2+c;; z2=z1
		case 106:					// More Fractals, 5th order N/Mset
		case 120:					// Flarium 07, Polynomial: z = (z*z+c)^(cn+c)
		case 132:					// Flarium 27, Polynomial: z=z*z*(cn+z)/(cn+z+c)+c
		case 165:					// Flarium 67-69, Newton Variations: z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c
		case 172:					// Flarium 112-116, Polynomials: z=z^n*c+z*c; Dragon curve variations
	//	case 177:					// Flarium 145, Polynomial: z=z^2+c [Jaenisch method]
		    Fractal.NumParam = 3;
		    Fractal.ParamName[0] = "Polynomial Degree (>= 2)";
		    Fractal.ParamName[1] = "Real Perturbation of Z(0)";
		    Fractal.ParamName[2] = "Imaginary Perturbation of Z(0)";
		    break;
		}

	    break;
	default:
	    Fractal.NumParam = fractalspecific[type].numparams;
	    for (int i = 0; i < NUMPARAM; i++)
		Fractal.ParamName[i] = fractalspecific[type].paramname[i];  
	}
    }

/**************************************************************************
    Initialise thread arrays
**************************************************************************/

void CManp::InitThreadArrays(int threadCount)
    {
    if (threadCount < 1)
	threadCount = 1;

    hPixelThread.assign(threadCount, nullptr);
    pPixelDataArray.assign(threadCount, nullptr);
    PixelThreadComplete.assign(threadCount, 0);
    PixelProgress.assign(threadCount, 0);
    }

/**************************************************************************
    Main engine loop : processes input and drives the fractal state machine
**************************************************************************/

void CManp::RunMainLoop(HWND hwnd, char* saveFile)
    {
    time_to_load = false;
    time_to_reinit = true;
    time_to_restart = true;   // force first render
    time_to_zoom = false;
    time_to_quit = false;

    // Bridge application state -> math layer.
    // Copy formatting precision from CManp into BigDouble so low-level
    // math code (BigDouble/BigComplex) does not depend on gManp.
//    BigDouble::decimals = decimals;
    for (;;)
	{
	Sleep(1);
	user_data(hwnd);

	if (time_to_quit)
	    return;

	HandleFractalStateMachine(hwnd, saveFile);
	}
    }

/**************************************************************************
    A handy little debugger
**************************************************************************/

void CManp::DumpStartupState(const char *where)
    {
#ifdef _DEBUG111

    char buf[2048];
    char widthbuf[256];

    BigWidth.SafeSprintf(widthbuf, sizeof(widthbuf), (char *)"%.32Rg");

    SAFE_SPRINTF(buf,
	"\n[PERT STARTUP] %s\n"
	"subtype=%d power=%d threshold=%d MaxRefIteration=%d\n"
	"xdots=%d ydots=%d precision=%d\n"
	"EnableApproximation=%d\n"
	"BigWidth=%s\n"
	"XSubN.size=%llu ExpXSubN.size=%llu\n",

	where,
	subtype,
	degree,
	threshold,
	MaxRefIteration,
	xdots,
	ydots,
	precision,
	EnableApproximation ? 1 : 0,
	widthbuf,
	(unsigned long long)XSubN.size(),
	(unsigned long long)ExpXSubN.size()
    );

    OutputDebugStringA(buf);

#endif
    }




