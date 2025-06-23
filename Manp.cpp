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
#include "colour.h"
#include "mpfr.h"
#include "slope.h"
#include "pixel.h"
#include "plot.h"
#include "OtherFunctions.h"

struct	workliststuff	worklist[MAXCALCWORK];
int	/*workpass, */totpasses, curpass;	// for 1/2 pass type tracing 
int	PlotType;

double	dem_toobig;
int	distest = 0, distestwidth = 71;
double	delxx, delxx2, delyy, delyy2;

int	finished = FALSE;		// all passes complete 
BYTE	orig_palette[768];		// loaded palette 

BYTE	*logtable = NULL;		// log value table for col comp 

long	iteration;			// globals for speed for now at least
WORD	type;				// M=mand, N=Newton etc 
int	subtype;		
					// Sub types:
					// Newton	B=basin, S=stripe, N=normal
					// Bifurcation	Q = quad mand, B = bifurcation
					// Cubic	B = CBIN, C = CCIN, F = CFIN, K = CKIN
					// Rational Map	3 = RAT34, 4 = RAT44

RGBTRIPLE OrbitColour = {255,255,255};	// Indexed colour for the orbit displays in Julia sets
WORD	special = 2;			// special colour, phase etc
WORD	degree;				// power
BYTE	pairflag;			// stereo pair 
int	biomorph;			// biomorph colour 
int	decomp;				// number of decomposition colours
BYTE	screenflag;			// replay saved screen 
BYTE	save_flag;			// save screen after image 
BYTE	_3dflag;			// replay saved file. 3 = 3D 
BOOL	ZoomEdge;			// Zooming process
BOOL	UseFractintPalette = FALSE;	// standard EGA palette
BYTE	exitflag;			// exit on completion
BYTE	juliaflag;			// Julia implementation of fractal
BYTE	RealTimeJuliaFlag;		// Display Julia set in real time
BYTE	calcmode;			// 'B' boundary, 'G' guess, etc
BYTE	phaseflag;			// 3 phases for type SPECIALNEWT fractals
BYTE	cycleflag;			// do colour cycling
BYTE	addflag;			// add spirals
int	logflag;			// log colour map required or in error
int	logval;				// log colour map starting value
int	AutoStereo_value;		// AutoStereo depth value
long	threshold;			// maximum iterations
int	Offset3D = 0;			// offset to threshold for 3D display
int	start, end;			// y pixel count range
BYTE	symmflag;			// symmetry ?
int	reset_period;			// periodicity checking
int	window_depth;			// data window size
int	window_width;			// modify for Julia set

double	x_rot;				// angle display plane to x axis
double	y_rot;				// angle display plane to y axis
double	z_rot;				// angle display plane to z axis
double	sclx, scly, sclz;		// scale 

double	hor;				// horizontal address
double	vert;				// vertical address
double	mandel_width;			// width of display
double	xgap;				// gap between pixels
double	ygap;				// gap between pixels

BYTE	ch;				// used for compression
int	number;				// used for compression

char	AntStatus[200];			// display the progress of ant()
char	LyapSequence[120];		// hold the AB sequence for Lyapunov fractals

double	potparam[3] = { 255.0, 820.0, 20.0 };		// potential parameters
double	param[20];			// parameters
double	f_radius, f_xcenter, f_ycenter;	// inversion radius, center
double	magnitude;
double	rqlim;				// bailout level
int	BailoutTestType = BAIL_MOD;	// type of bailout test
int	InsideMethod;			// the number of the inside filter
int	OutsideMethod;			// the number of the outside filter

int	colors = 256; 			// maximum colors available

int	oldrow = -1;
int	oldcol = -1;

jmp_buf	jump;				// jump to start next screen, jump in zoom 

extern	CTrueCol    TrueCol;		// palette info
extern	CSlope	Slope[];
extern	CDib	Dib; 
extern	WORD	delaycount;
extern	RECT 	r;
extern	int	FilterType;		// data for Tierazon filters
extern	int	ColourMethod;		// Tierazon colour methods
extern	int	nFDOption;		// Fractal Dimension option for Tierazon filters
extern	double	xx3rd, yy3rd;		// selected screen corners 

extern	BOOL	invert;			// invert fractal
extern	BOOL	ShowOrbits;		// show julia orbits for each image
extern	long    fillcolor;		// tesseral fillcolor: -1=normal 0 means don't fill     
	int	currow, curcol;

extern	BOOL	BatchFlag;
extern	BOOL	AutoExitFlag;
extern	BOOL	AutoSaveFlag;
extern	BOOL	AutoStartFlag;
extern	BOOL	CommandLineFile;	// does the command line contain a par filename?
extern	char	FormulaString[];	// used to hold the full formula
extern	char	AxesText[];		// used to convert axes and display them
extern	BYTE	PerspectiveFlag;	// display using perspective
extern	WORD	colours;

	int	symmetry;		// symmetry flag 
extern	POINT	ptSize;			// Stores DIB dimensions
extern	int	RotationAngle;		// in degrees
extern	char	LSYSFile[];
extern	char	PARFile[];
extern	char	KFRFile[];
extern	int	PaletteShift;		// fractional palette addressing

extern	int	height, xdots, ydots, width, bits_per_pixel;

// stuff for DwdDiff algorithm
extern	double	bump_transfer_factor;
extern	int	PaletteStart;
extern	double	lightDirectionDegrees;
extern	double	bumpMappingDepth;
extern	double	bumpMappingStrength;

extern	int	DrawJulia(HWND, POINTS);
extern	int	InitRTJulia(HWND);
extern	void	FindCursorRealPos(POINTS *);
extern	int	SendCopydataMessage(HWND, char *);
//extern	int	sticky_orbits(HWND);
extern	char	*FractData(void);
extern	char	*str_find_ci(char *, char *);

extern	BOOL	bTrack;			// TRUE if user is selecting a region
extern	HWND	GlobalHwnd;		// to allow passing of hwnd

//extern	BYTE	DisplayPaletteFlag;	// Display palette 
extern	BOOL	RunAnimation;		// are we in the middle of an animation run?
extern	BOOL	IsPAR;			// are we currently running a PAR file?
extern	BOOL	IsKFR;			// are we currently running a KFR file?
extern	int	CoordSystem;
extern	int	MaxDimensions;

extern	long	color;			// used for Newton root locations 
extern	int	colors; 		// maximum colors available
extern	int	file_type;
	int	blockindex;		// for solid guessing blocksize 

extern	char	lsys_type[];
extern	char	PertStatus[];
extern	char	SlopeStatus[];

extern	struct __timeb64 	FrameEnd;
extern	struct __timeb64 	FrameStart;
	char	szStatus[1200];		// status bar text
extern	DWORD	StatusColour;		// colour of status bar
extern	double	ScreenRatio;		// ratio of width / height for the screen
extern	DOUBLE	yymax;			// max value of vert
extern	Complex	RotationCentre;		// centre of rotation
extern	HWND	CallingWindowHandle;	// Is ManpWIN called by an external window via WM_COPYDATA message?
extern	int	DataFromPNGFile;	// loaded PNG file?
extern	int	NumberThreads;		// number of threads for multi-threading
extern	double 	*wpixels;		// an array of doubles holding slope modified iteration counts

// number declarations **********************************************************
extern	BYTE		BigNumFlag;		// True if bignum used
extern	MATH_TYPE	MathType;

extern	int		decimals, precision;
extern	BigDouble	BigHor, BigVert, BigWidth, Big_xgap, Big_ygap;
extern	BigComplex	zBig, cBig, qBig;
extern	BigDouble	Big_xxmax, Big_xxmin, Big_yymax, Big_yymin;
extern	BigDouble	BigCloseEnough, BigBailout;
extern	dd_real		DDBailout, DDCloseEnough, DDxgap, DDygap, DDHor, DDVert, DDWidth, DDyymax;
extern	qd_real		QDBailout, QDCloseEnough, QDxgap, QDygap, QDHor, QDVert, QDWidth, QDyymax;
// number declarations **********************************************************

int	time_to_zoom = 0;		// time to zoom in or out?
int	time_to_restart = 0;		// time to restart?
int	time_to_reinit = 0;		// time to reinitialize?
int	time_to_quit = 0;		// time to quit?
int	time_to_load = 0;		// time to load file?
int	time_to_break = 0;		// time to break out of animation?
int	NonStandardFractal = FALSE;	// does fractal use standard plotting mode?

extern	int	find_count_fp(DOUBLE, DOUBLE), DoHenon(void), user_data(HWND), 
		Lsystem(HWND, char *), dynam2dfloatsetup(void), dynam2dfloat(void),
		orbit3dfloatsetup(void), orbit3dfloatcalc(void), ifs(HWND), cellular(void), plasma(void), 
		demowalk(void), orbit2dfloat(void), FractintPar(HWND, char *), 
		Fibonacci(void), Fourier(void), RunForm(char *, int), bifurcation(void);
extern	int	NullSetup(void);				// sometimes we just don't want to do anything 
extern	int	rotate(int);

extern	void	init_log(HWND), set_palette(void), 
		setsymmetry(int, int), DisplayPalette(HWND, BOOL);

extern	int	write_png_file(HWND, char *, char *, char *); 
extern	void	ChangeView(HWND, int, int, int, int, int, int, int, int, char);
extern	void	initFibonacci(void);
extern	void	UpdateInit(void);
extern	int	UpdateClose(void);
extern	void	ConvertBignum2String(char *s, mpfr_t num);
extern	int	InitParserArithmetic(void);

	void	DisplayFractal(HWND);

int	perform_worklist(HWND), SecondaryWndProc(void);
void	ClearScreen(void);
void	DisplayStatusBarInfo(int, char *);

extern	ProcessType	OscAnimProc;
	CFract		Fractal;
	CTZfilter	TZfilter;	// Tierazon filters
	Complex		z, q, c, j;

	CPlot		Plot;		// image plotting routines 
	CPixel		Pix;		// routines for escape fractals
	CMatrix		Mat;		// matrix applications for rotation and translation
	CDDMatrix	DDMat;
	CQDMatrix	QDMat;
	CBigMatrix	BigMat;

// stuff for Pixel initialisation
	BYTE		SpecialFlag;			// tell graphing routine of special conditions
extern	double		closenuff;			// periodicity bailout
extern	COscProcess	OscProcess;
extern	int		period_level;			// 0 for no periodicity checking
extern	BOOL		UseCurrentPalette;		// do we use the ManpWIN palette? If false, generate internal filter palette
extern	double		dStrands;			// for Tierazon filters
	long		andcolor;
extern	BOOL		ExpandStarTrailColours;		// use the first 16 colours if false, else expand across the whole iteration range

	double		xxmin, xxmax, yymin, yymax;	// corners

/**************************************************************************
	Main Fractal Loop
**************************************************************************/

void	pfract_main(HWND hwnd, char *szSaveFileName)

    {
    HCURSOR  hCursor;
    char	s[360];
    int	flag;
    POINTS	CursorLocShort;
#ifdef DEBUG
    int	i;
    wsprintf(s, "time_to_restart = %d, time_to_reinit = %d", time_to_restart, time_to_reinit);
    MessageBox (hwnd, s, "Entering user_data()", MB_ICONEXCLAMATION | MB_OK);
#endif

    logflag = FALSE;						// don't use if no memory
    if (!RunAnimation)
	_ftime64(&FrameStart);					// initialise time counter

    time_to_zoom = FALSE;
    time_to_quit = FALSE;
    time_to_reinit = TRUE;
    time_to_restart = FALSE;
    for (;;) 
	{
	Sleep(1);						// reduce idle processor load
	user_data(hwnd);
	if (time_to_quit)
	     return;
	if (time_to_load)
	    {
	    time_to_load = FALSE;
	    time_to_reinit = FALSE;
	    time_to_restart = FALSE;
	    SecondaryWndProc();
	    }
	if (time_to_reinit || time_to_zoom)
	    {
	    time_to_reinit = FALSE;
	    flag = SecondaryWndProc();
	    if (flag == 0 || AutoStartFlag)
		{
		hCursor = LoadCursor(NULL, IDC_WAIT);	// Load hour-glass cursor.
		SetCursor(hCursor);
		if (logval)
		    init_log(hwnd);				// log colour distribution
		if (_3dflag)
		    Pix.init3d(xdots, ydots, x_rot, y_rot, z_rot, sclx, scly, sclz, threshold, hor, vert);				// init 3D parameters 
		if (!addflag && !DataFromPNGFile/* || type != FIBONACCI*/)
		    ClearScreen();
		hCursor = LoadCursor(NULL, IDC_ARROW);		// Load normal cursor.
		SetCursor(hCursor);			        // Reload arrow cursor.
		DisplayFractal(hwnd);
		if (DataFromPNGFile)			// if we reload a PNG file before we finish a calc, we don't want to restart a new calc.
		    DataFromPNGFile = FALSE;
		else
		    time_to_restart = TRUE;
		}
	    }
	if (time_to_restart)
	    {

#ifdef DEBUG
	    for (i = 0; i < 32; i++)
		{
		wsprintf(s, "Col[%d]: %d %d %d", i, *(PalettePtr + 3 * i), 
					    *(PalettePtr + 3 * i + 1), *(PalettePtr + 3 * i + 2));
		MessageBox (hwnd, s, "File colours", MB_ICONEXCLAMATION | MB_OK);
		}
#endif   

	    _ftime64(&FrameStart);				// initialise time counter
	    time_to_restart = FALSE;
	    number = 0;					// char count for screen
	    finished = FALSE;
	    UpdateInit();
	    if (!DataFromPNGFile)
		perform_worklist(hwnd);
	    if (time_to_quit)
		 return;
	    DisplayPalette(hwnd, TrueCol.DisplayPaletteFlag);
	    finished = TRUE;
	    DisplayFractal(hwnd);
	    if (RealTimeJuliaFlag && !juliaflag)
		{
		InitRTJulia(hwnd);
		FindCursorRealPos(&CursorLocShort);
		DrawJulia(hwnd, CursorLocShort);
		}
	    if (!bTrack)			// don't splatter scan
		{
		ChangeView(hwnd, (short)-GetScrollPos (hwnd, SB_HORZ), (short)-GetScrollPos (hwnd, SB_VERT),
						    (short)ptSize.x, (short)ptSize.y, 0, 0, (short)width, (short)height, TRUE);
		InvalidateRect(hwnd, &r, FALSE);
		}
	    if (AutoExitFlag)
		time_to_quit = TRUE;
	    if (AutoSaveFlag)
		{
		if (write_png_file(hwnd, szSaveFileName, "ManpWIN", FractData()) < 0)
		    {
		    wsprintf(s, "Error: Could not write file: <%s>", szSaveFileName);
		    MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		    MessageBeep (0);
		    AutoSaveFlag = false;
		    continue;
		    }
		}
	    if (CallingWindowHandle)				// image started by WM_COPYDATA message... return message 
	    							// to advise that the work is done.
		SendCopydataMessage(hwnd, "Send CopyData");
	    UpdateClose();
	    DisplayStatusBarInfo(COMPLETE, ((IsPAR) ? PARFile : (IsKFR) ? KFRFile : ""));			// display status bar
	    }
	}
#ifdef DEBUG
    wsprintf(s, "NonStandardFractal = %d", NonStandardFractal);
    MessageBox (hwnd, s, "Entering user_data()", MB_ICONEXCLAMATION | MB_OK);
#endif
    }

/**************************************************************************
    Get Fractal Name
**************************************************************************/

    char	*GetFractalName(void)

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
	Get Arith Type
**************************************************************************/

int 	 GetArithType()
    {
    if (BigNumFlag)
	{
	if (precision <= 30)
	    MathType = DOUBLEDOUBLE;
	else if (precision <= 60)
	    MathType = QUADDOUBLE;
	else
	    MathType = ARBITRARYPREC;
	}
    else
	MathType = DOUBLEFLOAT;
    return MathType;
    }

/**************************************************************************
	Create Fractal Name
**************************************************************************/

void	CreateFractalName(BOOL UseszAppName, char *Name)

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
	sprintf(SubData, "Fn=%s", Fractal.Fn1);
    else if (Fractal.NumFunct == 2 && type != OSCILLATORS)			// we use NumFunct to display dimensions
	sprintf(SubData, "Fn1=%s,Fn2=%s", Fractal.Fn1, Fractal.Fn2);
    else if (type == FORMULA || type == LSYSTEM || type == FRACTPAR || type == IFS)
	sprintf(SubData, "Sub=%s", lsys_type);
    else if (type == TIERAZON || type == CROSSROADS || type == ZIGZAG || type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
	sprintf(SubData, "Sub=%d", subtype);
    else if (type == CUBIC)
	{
	char	ch;
	if (param[0] == 0.0)   ch = 'B';
	if (param[0] == 1.0)   ch = 'C';
	if (param[0] == 2.0)   ch = 'F';
	if (param[0] == 3.0)   ch = 'K';
	sprintf(SubData, "Sub=C%cIN", ch);
	}
    else if (type == MALTHUS || type == TRIANGLES || type == GEOMETRY || type == CIRCLES || type == PASCALTRIANGLE)
	sprintf(SubData, "Sub=%c", subtype);
    else if (type == SCREENFORMULA)
	sprintf(SubData, "Frm=<%s>", FormulaString);
    else 
	sprintf(SubData, "Sub=%d", subtype);
    if (BigNumFlag)
	sprintf(PrecisionData, "Precision=%d", precision);
    else
	sprintf(PrecisionData, "");

    if (InsideMethod > NONE || OutsideMethod > NONE)
	{
	if (OutsideMethod > TIERAZONCOLOURS)
	    sprintf(FilterString, "TZColour=%d, ", ColourMethod);
	else if (OutsideMethod > TIERAZONFILTERS)
	    sprintf(FilterString, "TZFilter=%d, ", FilterType);
	else if (InsideMethod > NONE)
	    sprintf(FilterString, "Filter=%d, ", InsideMethod);
	else
	    sprintf(FilterString, "Filter=%d, ", OutsideMethod);
	}
    else
	sprintf(FilterString, "");
	              
    if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
	{
	wsprintf(Name, "%s: Thresh=%d, Plot=%c, Log=%d, Fract=%s, %s, Max Dim=%d, Co-ordSys=%s, Axes=%s", 
	    (UseszAppName) ? (LPSTR)szAppName : "", threshold, calcmode, logval, GetFractalName(), SubData, MaxDimensions, CoordSysInfo.CoordSys[CoordSystem], AxesText);
	}
    else if (type == PERTURBATION)
	wsprintf(Name, "%s: Thresh=%d, %sFract=(Pert)-%s, %s, NumThreads=%d, %s, Deg=%d, %s",
	(UseszAppName) ? (LPSTR)szAppName : "", threshold, FilterString, GetFractalName(), SubData, NumberThreads, ArithString, degree, PrecisionData);
    else if (type == SLOPEDERIVATIVE)
	wsprintf(Name, "%s: Thresh=%d, Fract=(Slope Der)-%s, %s, NumThreads=%d, %s, Deg=%d, %s",
	(UseszAppName) ? (LPSTR)szAppName : "", threshold, GetFractalName(), SubData, NumberThreads, ArithString, degree, PrecisionData);
    else if (type == SLOPEFORWARDDIFF)
	wsprintf(Name, "%s: Thresh=%d, Fract=(Slope Fwd)-%s, %s, NumThreads=%d, %s, Deg=%d, %s",
	(UseszAppName) ? (LPSTR)szAppName : "", threshold, GetFractalName(), SubData, NumberThreads, ArithString, degree, PrecisionData);
    else
	{
	wsprintf(Name, "%s: Thresh=%d, Plot=%c, %sLog=%d, Fract=%s, %s, Jul=%c, %s, Deg=%d, Spec=%d, 3D=%c, %s", 
	    (UseszAppName) ? (LPSTR)szAppName : "", threshold, calcmode, FilterString, logval, GetFractalName(), SubData,
	    ((juliaflag) ? 'T' : 'F'), ArithString, degree, special, ((_3dflag) ? 'T' : 'F'), PrecisionData);
	}
    }

/**************************************************************************
	Display Parameters
**************************************************************************/

void	DisplayFractal(HWND hwnd)

    {
    char    Name[6400];

    CreateFractalName(TRUE, Name);
    SetWindowText(hwnd, Name);
    }

/**************************************************************************
	Run special plotting mode fractals 
**************************************************************************/

#include"BigTrig.h"

int	SpecialFractals(HWND hwnd, CPixel *Pix)

    {
    COtherFunctions	OthFn;
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
	    if (fractalspecific[type].flags & OTHERFNINPIXEL)
		{
		OthFn.InitOtherFunctions(type, &z, &q, hwnd, wpixels, &TrueCol, &Dib, AntStatus, FrameEnd, FrameStart);
		OthFn.RunOtherFunctions(type, &z, &q, &SpecialFlag, &iteration, xdots, ydots, param, threshold, hor, vert, mandel_width, ScreenRatio, &curpass, &totpasses, user_data, hwnd, rotate);
		}
	    else
		{
		double	temp_x, temp_y;

		temp_x = ScreenRatio / (double)(xdots - 1);
		temp_y = 1.0 / (double)(ydots - 1);
		if (BigNumFlag)
		    {
		    Big_xgap = BigWidth * temp_x;
		    Big_ygap = BigWidth * temp_y;
		    }
		else
		    {
		    xgap = mandel_width * temp_x;
		    ygap = mandel_width * temp_y;
		    }
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

/**************************************************************************
	General escape-time engine 
**************************************************************************/

//#define PASSWIDTH	20

int	perform_worklist(HWND hwnd)

    {
    int		SpecialFunctionsFlag, StripWidth;
    HANDLE	ghMutex = NULL;							// manage access to shared resources such as Dib and wpixels

    StripWidth = (NumberThreads > 0) ? xdots / NumberThreads : xdots;		// width of vertical strip
    Plot.InitPlot(threshold, &TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib.BitsPerPixel, &Dib, USEPALETTE + USEWPIXELS);
    
    if (BigNumFlag)
	Pix.ManageBignumPrecision(precision);	// allow internal bignum variables to track current precision requirements

    StripWidth = xdots;								// until we implement multi-threading

    if (distest)								// setup stuff for distance estimator
	Pix.InitDistEst(&xxmin, &xxmax, &yymin, &yymax, &xx3rd, &yy3rd, &distestwidth, distest);

    // okay, we have to get the globals into the Pixel object somehow
    Pix.InitPixel0(type, special, subtype, &degree, rqlim, ExpandStarTrailColours, SpecialFlag, precision, biomorph, InsideMethod, OutsideMethod, RotationAngle, xdots, ydots, nFDOption, &Plot);
    Pix.InitPixel1(&TZfilter, &TrueCol, &OscProcess, period_level, distest, invert, phaseflag, wpixels, juliaflag, &closenuff, &BigCloseEnough, calcmode);
    Pix.InitPixel2(CoordSystem, UseCurrentPalette, reset_period, colors, hor, vert, mandel_width, BigHor, BigVert, BigWidth, &yymax, &Big_yymax);
    Pix.InitPixel3(dStrands, j, pairflag, &andcolor, _3dflag, &xgap, &ygap, &Big_xgap, &Big_ygap, &c, ScreenRatio, colours, &Fractal, BailoutTestType);
    Pix.InitPixel4(&cBig, &q, &z, &qBig, &zBig, threshold, BigNumFlag, &color, logval, &iteration, f_radius, f_xcenter, LyapSequence);
    Pix.InitPixel5(f_ycenter, &symmetry, param, potparam, decomp, logtable, &AutoStereo_value, width, hwnd, &Mat, &DDMat, &QDMat, &BigMat);
    Pix.InitPixel6(&Dib, &PlotType, &oldrow, &oldcol, &time_to_zoom, &time_to_restart, &time_to_reinit, &time_to_quit, fillcolor, &andcolor, &blockindex, &totpasses, &curpass);
    Pix.InitPixel7(&DDxgap, &DDygap, &DDHor, &DDVert, &DDWidth, &DDCloseEnough, &QDxgap, &QDygap, &QDHor, &QDVert, &QDWidth, &QDCloseEnough);
    Pix.InitPixel8(&DDyymax, &QDyymax, &MathType, RotationCentre);

    NonStandardFractal = FALSE;
    SpecialFunctionsFlag = SpecialFractals(hwnd, &Pix);				// non-"standard" fractals - no multi-threading
    if (SpecialFunctionsFlag == -1)
	return -1;
    else if (SpecialFunctionsFlag == 1)
	{
	NonStandardFractal = TRUE;
	return 1;								// finished processing non-standard plotted fractal
	}

    if (type == SCREENFORMULA || type == FORMULA)
	if (InitParserArithmetic() < 0)						// we have to load different routines for each type of arithmetic, so let's test for it and init function pointers
	    return -1;
    if (calcmode == 'F')
	Slope[0].InitRender(threshold, &TrueCol, &Dib, wpixels, PaletteShift, bump_transfer_factor, PaletteStart, lightDirectionDegrees, bumpMappingDepth, bumpMappingStrength);
    size_t SizofPix = sizeof(Pix);			// just for interest
    if (Pix.RunThread(hwnd, /*int i*/0, ghMutex, StripWidth, Slope[0], &Fractal, &Big_xxmin, &Big_xxmax, &Big_yymin, &xxmin, &xxmax, &yymin, user_data) < 0)
	return -1;
    if (juliaflag && ShowOrbits)
	Pix.plot_orbits(OrbitColour, NUM_ORBITS);
    return 0;
    }

 /*-----------------------------------------
	Generate  status info
  -----------------------------------------*/

char	*ShowTime (double time)

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
	    sprintf(TimeString, "%d.%1d sec", sec, tenths);
	else
	    sprintf(TimeString, "%d min, %d sec", min, sec);
	}
    else
	sprintf(TimeString, "%d hr, %d min", hr, min);
    return (TimeString);
    }
  
 /*-----------------------------------------
	Generate  status info
  -----------------------------------------*/

void	GenPositionStr (char *PositionStr)

    {     
    char	*s1 = NULL;
    char	*s2 = NULL;
    char	*s3 = NULL;
    double	centrex, centrey;
    BigDouble	Big_centrex, Big_centrey;

    if (type == FOURIER)					// there's no fractal dimensions here
	return;
//	wsprintf(s, "Hor = %f, Vert = %f, Width = %f", hor, vert, mandel_width); 
    if (BigNumFlag)
	{
	Big_centrex = BigHor + (BigWidth * ((double) width / (double) (2 * height)));
	Big_centrey = BigVert + (BigWidth / 2.0);
	s1 = new char[SIZEOF_BF_VARS];
	s2 = new char[SIZEOF_BF_VARS];
	s3 = new char[SIZEOF_BF_VARS];
	mpfr_sprintf(s1, "%.36Rf", Big_centrex.x);
	mpfr_sprintf(s2, "%.36Rf", Big_centrey.x);
	mpfr_sprintf(s3, "%.12Re", BigWidth.x);
	sprintf(PositionStr, "X = %s,Y = %s,Width = %s", s1, s2, s3);
	if (s1) { delete[] s1; s1 = NULL; }
	if (s2) { delete[] s2; s2 = NULL; }
	if (s3) { delete[] s3; s3 = NULL; }
	}
    else 
	{
	if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)		// don't show centre, show corner
	    sprintf(PositionStr, "X=%14.14f,Y=%14.14f,Width=%14.14f", hor, vert, mandel_width);
	else
	    {
	    centrex = hor + (mandel_width * ((double) width / (double) (2 * height)));
	    centrey = vert + (mandel_width / 2.0);
	    if (mandel_width > 0.000001)
		sprintf(PositionStr, "X=%14.14f,Y=%14.14f,Width=%14.14f", centrex, centrey, mandel_width);
	    else
		sprintf(PositionStr, "X=%14.14f,Y=%14.14f,BigWidth=%14e", centrex, centrey, mandel_width);
	    }
	}
    }
    
 /*-----------------------------------------
	Generate  status info
  -----------------------------------------*/

void	DisplayStatusBarInfo (int complete, char *text)

    {     
    char	PassStr[200];
    char	PositionStr[800];
    char	FinishedStr[200];
    char	PrecisionStr[200];		// times 3 because of x, y, width
    double	ElapsedTime;
    int		VariableSize;

    VariableSize = max(decimals + PRECISION_FACTOR, DBL_MANT_DIG);	// dec = 0 if not BigNum

    if (NonStandardFractal)
	*PassStr = '\0';
    else if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
	sprintf(PassStr, "Pass %d of %d", curpass, totpasses); 
    else
	{
	switch (calcmode)
	    {
	    case 'B':
	    case 'T':
	    case '1':
	    case 'F':
		sprintf(PassStr, "Pass 1 of 1");
		break;
	    case '2':
	    case 'G':
	    case 'S':
	    case 'V':
	    case 'H':
		sprintf(PassStr, "Pass %d of %d", curpass, totpasses); 
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
	    sprintf(szStatus, "Par File <%s> Completed in %s, %s", text, ShowTime(ElapsedTime), PositionStr);
	else if (IsKFR)
	    sprintf(szStatus, "KFR File <%s> Completed in %s, %s", text, ShowTime(ElapsedTime), PositionStr);
	else if (type == LYAPUNOV)
	    sprintf(szStatus, "Image Completed in %s, %s, Lyapunov Sequence='%s'", ShowTime(ElapsedTime), PositionStr, LyapSequence);
	else
	    sprintf(szStatus, "Image Completed in %s, %s", ShowTime (ElapsedTime), PositionStr);
	}
    else if (complete == INCOMPLETE)
	{
	sprintf(FinishedStr, ", Time %s", ShowTime (ElapsedTime));
	if (BigNumFlag)	    // now we have double double and quad double...
	    {
	    if (precision <= 30 && fractalspecific[type].flags & USEDOUBLEDOUBLE)
		sprintf(PrecisionStr, "DD Prec: %d", precision);
	    else if (precision <= 60 && fractalspecific[type].flags & USEDOUBLEDOUBLE)
		sprintf(PrecisionStr, "QD Prec: %d", precision);
	    else
		sprintf(PrecisionStr, "Arb Prec: %d", precision);
	    }
	if (OscAnimProc == MORPHING)
	    sprintf(szStatus, "%s%s", PassStr, FinishedStr);
	else if (type == PERTURBATION)
	    sprintf(szStatus, "%s%s, Arith=%s, %s", PertStatus, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr);
	else if (type == SLOPEDERIVATIVE || type == SLOPEFORWARDDIFF)
	    sprintf(szStatus, "%s%s, Arith=%s, %s", SlopeStatus, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr);
	else if (type == ANT || type == TOWER)
	    sprintf(szStatus, "Progress = %s", AntStatus);
	else if (type == LYAPUNOV)
	    sprintf(szStatus, "%s%s, Arith=%s, %s, Lyapunov Sequence='%s'", PassStr, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr, LyapSequence);
	else
	    sprintf(szStatus, "%s%s, Arith=%s, %s", PassStr, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr);
	if (str_find_ci(szStatus, "Ref[") != 0)
	    StatusColour = 0x00FFFF80;
	else
	    StatusColour = 0x0000FFFF;
	}
    else if (complete == CALCULATINGREF)
	{
	sprintf(FinishedStr, ", Time %s", ShowTime(ElapsedTime));
	sprintf(PrecisionStr, "Arb Prec: %d", precision);
	sprintf(szStatus, "%s%s, Arith=%s, %s", PertStatus, FinishedStr, ((BigNumFlag) ? PrecisionStr : "Float"), PositionStr);
	StatusColour = 0x00FFFF80;
	}
    else							// initialising
	{
	sprintf(szStatus, "Initialising fractal variables");
	StatusColour = 0x006060FF;
	}
    }
