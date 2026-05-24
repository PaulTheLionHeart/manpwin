#include <ctype.h>
#include <windows.h>
#include <setjmp.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "manpwin.h"
#include "colour.h"
#include "pixel.h"
#include "anim.h"
#include "pertengine.h"
#include "OtherFunctions.h"

#pragma once

#define g_pOtherFunctions (&gManp->OthFn)
#define	BITSPERPIXEL	24
class CManp
    {
    public:
	CManp();
	~CManp();
	void	RunMainLoop(HWND hwnd, char* saveFile);
	int	RunEscapeTimeEngine(HWND hwnd);
	void	DisplayFractal(HWND hwnd);
	void	DumpStartupState(const char *where);
	void	UpdateInit(void);
	int	UpdateClose(void);
	int	DoUpdate(void);
	DWORD	PixelThreadEntry(LPVOID lpParam);
	void	CreateFractalName(BOOL UseszAppName, char *Name);
	void	DisplayStatusBarInfo(int complete, char *text);
	char	*GetFractalName(void);
	char	*ShowTime(double time);
	void	UpdateProgress(std::atomic<int>* workIndex, int totalPixels, char* StatusBarInfo, int NumThreads);
//	void	WaitForPixelThreadsToFinish();
	void	WaitForAllThreadsToFinish();
	void	InitThreadArrays(int n);
	void	ClearScreen(void);
	int	CardioidInversionScript(HWND hwnd, char *filename);
	int	CircleInversionScript(HWND hwnd, char *filename);
	int	GenInversionScript(HWND hwnd, char *filename);
	int	CardioidJuliaScript(HWND hwnd, char *filename, int PaletteShift);
	int	CircleJuliaScript(HWND hwnd, char *filename, int PaletteShift);
	int	GenJuliaScript(HWND hwnd, char *filename, int PaletteShift);
	int	GenFourierScript(HWND hwnd, char *filename);
	int	displayCurve(HWND hwnd, int JuliaAnimType);
	int	ZoomOut(HWND hwnd, RECT *Rect);
	int	ZoomIn(HWND hwnd, RECT *Rect);
	void	setup_defaults(void);
	void	BigCornerstoFloat(void);
	void	FloatCornerstoBig(void);
	int	getprecbf_mag(void);
	BOOL	IsBignumFractal(int type);
	int	calcfracinit(void);
	int	GenZoomScript(HWND hwnd, char *filename);
	void	InitUndo(void);
	void	Redo(struct UNDO *u1);
	void	Undo(struct UNDO *u1);
	void	SaveUndo(BOOL current);
	void	LoadUndo(BOOL UndoFlag);
	int	ChangeBigPrecision(int dec);
	void	ConvertString2Bignum(mpfr_t num, char *s);
	int	GenOscillatorScript(HWND hwnd, char *filename);
	int	GenOscMorphScript(HWND hwnd, char *filename);
	int	GenParameterScript(HWND hwnd, char *filename, int NumVariables);
	int	InitRTJulia(HWND hwnd);
	void	RTJuliaOrbits(RGBTRIPLE colour, int count);
	int	DrawJulia(HWND hwnd, POINTS ptCurrent);
	void	init_log(HWND hwnd);
	void	init(HWND hwnd);
	int	analyse_corner(char *s);
	void	AnalysePalette(char *s);
	void	AnalyseDistEst(char *s);
	void	analyse_3d(char *s);
	void	AnalyseRotation(char *s);
	void	AnalyseMethod(char *s);
	void	analyse_inside(char *s);
	void	get_julia_loc(char *s);
	int	ProcessLocation(char *real, char *imag, char *zoom);
	int	FindKallesType(int power, int FractType);
	int	ConvertToKallesType(int subtype, WORD degree, int *power, int *FractalType);
	int	ProcessColours(char *value);
	void	GetParamsList(char *s);
	void	GetBailout(char *s);
	int	ProcessInvert(char *s);
	void	AnalyseFwdDiff(char *s);
	int	ProcessCorners(char *s, BOOL CentreFlag);
	int	ProcessParams(char *s);
	int	AnalyseFunction(char *Trigdata);
	int	ProcessPotential(char *s);
	int	Processfilters(char *s, BOOL IsInside);
	int	ProcessBailoutTest(char *value);
	bool	DecodeFractintColourChar(char c, BYTE& out);
	int	ParseColours(char *value);
	int	FindType(HWND hwnd, char *FractType, char *FractName, bool *IsFrm, double TempRqlim);
	void	DoCaption(HWND hwnd, char *szTitleName);
	void	ToggleJulia(HWND hwnd, POINTS &CursorLocShort);
	void	UpdateAnimParamValues(void);
	void	InitAnimParamValues(void);
	void	BuildJuliaFormulaString(void);
	void	InitFract(int type);
	void	RebuildFractalMetadata(WORD type, int subtype);
	void	ErrorHandler(LPTSTR lpszFunction);

	// script stuff
	int	GenerateFractalFrame(HWND hwnd, char *FileName, int TotalFrames, int ThisFrame);
	int	GenerateFractal(HWND hwnd, char *FileName, char *MoreInfo, int TotalFrames, int ThisFrame, double &delay);
	int	InitScript(HWND hwnd, char *FileName, int *TotalFrames);
	int	RunScript(HWND hwnd, char *FileName);
	void	EndScript(int ThisFrame);
	void	CloseScript(void);
	double	GetNumFrames(void);
	int	SaveIndividualFrames(void);

	// animation stuff
	void	StatusBarAnimInfo(int frame, int TotalFrames);
	void	OutputStatusBar(HWND hwnd);
	void	ConvertRGB2ASCII(RGBTRIPLE rgb, char *buf);
	void	ConvertASCII2RGB(RGBTRIPLE &rgb, char *buf);
	void	InitAnimationFrameStructure(int Frames);
	void	LoadAnimationFrame(char *buf, char *FrameInfo, int Frame, double ParamValue, BOOL IsParamAnim, BOOL IsMorphAnim, double &delay);
	void	AnimateInit(int Frames);
	int	DoAnimation(void);
	int	AnimateClose(void);
	int	AnimateResume(void);
	void	IncreaseAnimSpeed(void);
	void	DecreaseAnimSpeed(void);
	void	ReverseAnimDirection(void);
	void	StepFrame(void);
	int	AnimateSuspend(void);

	std::vector<std::unique_ptr<CPixel>> Pixel;
	std::vector<std::unique_ptr<CPerturbation>> PertCalculator;
	std::vector<std::unique_ptr<CSlope>> Slope;
	std::vector<int> SlopeProgress;

	std::vector<BYTE> PixelThreadComplete;		// Restore to private once we get everything in CManp)
	std::vector<BYTE> PertThreadComplete;
	std::vector<BYTE> SlopeThreadComplete;
	std::vector<float> wpixels {};			// an array of doubles holding slope modified iteration counts

	ProcessType	OscAnimProc = STANDARD;
	COscProcess	OscProcess;
	COtherFunctions OthFn;
	COtherFunctions* pOtherFunctions = nullptr;
	CFract		Fractal;
	CTZfilter	TZfilter;			// Tierazon filters
	CTrueCol	TrueCol;			// palette info
	CPlot		Plot;				// image plotting routines 
	BLAS		Bla;				// Bilinear Approximation
	CDib		Dib;				// Device Independent Bitmap
	StoreReferenceData* RefData = nullptr;

	HWND		GlobalHwnd;			// to allow passing of hwnd 

	WORD		type;				// M=mand, N=Newton etc 
	int		subtype = 0;
	// Sub types:
	// Newton	B=basin, S=stripe, N=normal
	// Bifurcation	Q = quad mand, B = bifurcation
	// Cubic	B = CBIN, C = CCIN, F = CFIN, K = CKIN
	// Rational Map	3 = RAT34, 4 = RAT44

	double		hor;
	double		vert;
	double		mandel_width;
	BigDouble	BigHor;
	BigDouble	BigVert;
	BigDouble	BigWidth;
	BigDouble	Big_oldhor, Big_oldvert, Big_oldwidth;
	BigDouble	BigCloseEnough, Big_xxmax, Big_yymax, Big_xxmin, Big_yymin, Big_xgap, Big_ygap;
	BigComplex	zBig, cBig, qBig;

	Complex		j;
	BigDouble	BigBailout;
	dd_real		DDBailout;
	qd_real		QDBailout;
//	double		closenuff;			// periodicity bailout
	double		xxmin, xxmax, yymin, yymax;	// selected screen corners 
	double		xx3rd, yy3rd;

	int		xdots, ydots, width, height;	// image dimensions

	enum MATH_TYPE	MathType = DOUBLEFLOAT;
	int		precision = 0;
//	int		decimals = 10;
	BYTE		BigNumFlag = FALSE;		// True if bignum used
	int		OldPrecision = 0;
	int		dec;

	double		AspectRatio;
	BYTE		RealTimeJuliaFlag = 0;		// Display Julia set in real time
	int		PaletteStart = 0;
//	int		totpasses, curpass;		// for 1/2 pass type tracing 
	long		iteration;
	double		potparam[3] = { 255.0, 820.0, 20.0 };		// potential parameters
	double		param[20];			// parameters
	int		PlotType;
	int		distest = 0, distestwidth = 71;
	int		finished = FALSE;		// all passes complete 
	BYTE		*logtable = NULL;		// log value table for col comp 
//	BYTE		orig_palette[768];		// loaded palette 
	WORD		special = 2;			// special colour, phase etc
	WORD		degree;				// power
	RGBTRIPLE OrbitColour = { 255,255,255 };	// Indexed colour for the orbit displays in Julia sets
	BYTE		pairflag = 0;			// stereo pair 
	int		biomorph;			// biomorph colour 
	int		decomp;				// number of decomposition colours
	BYTE		save_flag = 0;			// save screen after image 
	BOOL		ZoomEdge;			// Zooming process
	BYTE		_3dflag;			// replay saved file. 3 = 3D 
	BYTE		exitflag;			// exit on completion
	BOOL		UseFractintPalette;		// standard VGA palette
	int		logval;				// log colour map starting value
	int		logflag;			// log colour map required or in error
	int		screenx, screeny;
	int		caption;			// size of windows caption and scroll bars
	int		scroll_width;			// size of horizontal scroll bars
	char		AntStatus[200];			// display the progress of ant()
	char		LyapSequence[120];		// hold the AB sequence for Lyapunov fractals
	double		rqlim;				// bailout level
	int		period_level;			// 0 for no periodicity checking
	BYTE		phaseflag;			// 3 phases for type SPECIALNEWT fractals
	BYTE		cycleflag;			// do colour cycling
	BYTE		addflag;			// add spirals
	int		AutoStereo_value;		// AutoStereo depth value
	int		blockindex;			// for solid guessing blocksize 
	int		CoordSystem = CARTESIAN;
	int		oldrow = -1;
	int		oldcol = -1;
	int		BailoutTestType = BAIL_MOD;	// type of bailout test
	int		InsideMethod;			// the number of the inside filter
	int		OutsideMethod;			// the number of the outside filter
	int		nFDOption = 4;			// Fractal Dimension option for Tierazon filters
	double		dStrands;
	BOOL		UseCurrentPalette = TRUE;	// do we use the ManpWIN palette? If false, generate internal filter palette
	BOOL		RGBFilter;			// If true, we use the plotting routine for RGB filters (no plotting of iteration)
	int		FilterType = 1;			// data for Tierazon filters
	int		ColourMethod = 1;		// Tierazon colour methods
	RECT 		r;
	PAINTSTRUCT 	ps;
	BOOL		bTrack = FALSE;			// TRUE if user is selecting a region
	BOOL		invert = FALSE;			// invert fractal
	RECT 		WARect;				// this is the usable screen taking taskbar into account
	long		fillcolor = -1;			// tesseral fillcolor: -1=normal 0 means don't fill
	int		colors = 256; 			// maximum colors available
	long		color;				// used for Lorenz and Newton root locations 
	BOOL		BatchFlag;
	BOOL		AutoExitFlag = FALSE;
	BOOL		AutoSaveFlag;
	BOOL		AutoStartFlag = FALSE;
	BOOL		CommandLineFile = FALSE;	// does the command line contain a par filename?
	bool		EnableApproximation = true;	// use BLA on perturbation
	BYTE		calcmode;			// 'B' boundary, 'G' guess, etc
	BYTE		juliaflag = 0;			// Julia implementation of fractal
	BYTE		oldcalcmode;			// store values during 3D transformations etc
	int		Offset3D = 0;			// offset to threshold for 3D display
	BYTE		symmflag;			// symmetry ?
	int		symmetry;			// applicable symmetry logic
	int		bitshift;			// bit shift for fudge
	int             row, col;
	double		x_rot;				// angle display plane to x axis
	double		y_rot;				// angle display plane to y axis
	double		z_rot;				// angle display plane to z axis
	double		sclx, scly, sclz;		// scale 
	double		f_radius, f_xcenter, f_ycenter;	// inversion radius, centre
	double		iterations = 20000;		// oscillators, curves, knots etc
	long		threshold;			// maximum iterations
	WORD		colours;			// colours in the file
	BYTE		SpecialFlag;			// tell graphing routine of special conditions
	bool		IsPAR = false;			// are we currently running a PAR file?
	bool		IsKFR = false;			// are we currently running a KFR file?
	int		MaxDimensions;
	DWORD		StatusColour = 0x0000FFFF;	// colour of status bar
	int		StartFrame = 0;			// allow us to resume animation run
	bool		RunAnimation = false;		// are we in the middle of an animation run?
	bool		SuspendAnimation = false;	// pause animation run?
	bool		StartImmediately = true;	// immediate start of animation generation
	RECT 		StatusBarRect;
	char		szStatus[STATUSSIZE] = { 0 };	// status bar text
	BOOL		DisplayAnimation = FALSE;	// allow system to know that we are currently displaying an animation (BOOL because it is represented by an int and works best in scanf())
	int		RotationAngle = 0;		// rotate image in degrees	
	Complex		RotationCentre = 0.0;		// centre of rotation
	int		PalOffset = 0;			// Start Palette here
	int		SlopeType;
	double		IterDiv = 1.0;			// divide iteration by this amount
	double		ColourSpeed = 0.0;		// used for colour smoothing
	int		NonStandardFractal = false;	// does fractal use standard plotting mode?
	int		PertColourMethod = 0;		// Kalles colour method
	BYTE		PerspectiveFlag = false;	// display using perspective
	int		ArithType = DOUBLE;
	int		MaxRefIteration = 0;
	RECT		Rect;
	char		FormulaString[MAXFORMULASTRINGLENGTH] = "\0";		// used to hold the full formula
	BYTE		grayflag = 0;			// flag to use grey value rather than color number
	bool		RTJuliaActive = false;		// block any more requests until calcs complete
	bool		ExpandStarTrailColours = true;	// use the first 16 colours if false, else expand across the whole iteration range
	DWORD		BackgroundColour = 0L;		// set background colour for IFS and L-System fractals
	int		SlopeDegree = 2;
	POINT		ptSize;				// Stores DIB dimensions
	int		NumberThreads = 1;		// set the default to 1. Multi=threading is for experimentation only
	char		PertStatus[MAXLINE];
	char		PertErrorMessage[MAXLINE];
	char		SlopeStatus[MAXLINE];
	char		SlopeErrorMessage[MAXLINE];
	int		CurrentRenderMode = NOMULTITHREAD;	// to decide how to exit threads cleanly
	bool		UseMutex = false;		// trade off speed for accuracy
	WORD		UpdateDelay = 1000;		// delay in milliseconds
	int		totpasses, curpass;		// for 1/2 pass type tracing as well as oscillators etc
	bool		RunMPEG = false;		// are we in the middle of generating an MPEG file?
	int		xAxis = 0, yAxis = 1, zAxis = 2;// numerical values for axes. assume normal x = 0, y = 1 and z = 2
	RGBTRIPLE	OscBackGround = { 0,0,0 };
	int		DataFromPNGFile = FALSE;	// loaded PNG file?
	Complex		q = 0.0;

	// animation stuff
	std::vector<AnimStruct> ANIM;
	UINT		AnimTimerID = 0;
	bool		AnimTimerRunning = false;
	double		CurrentDelay = 20.0;		// delay in milliseconds
	bool		AnimationForward = true;
	int		TotalFrames = 0;
	int		AnimFirstFrame = 0;
	int		AnimLastFrame = -1;
	int		CurrentFrame = 0;		// used to increment to the next frame when the timer ticks
	BOOL		Return2Start = FALSE;		// do we want to go backwards to the start?


	std::vector<ExpComplex>	ExpXSubN{ 0 };		// references
	std::vector<Complex>	XSubN{ 0 };

	// thread handles (note that hPixelThread is private below as it is only called within CManp)
	std::vector<HANDLE>	hSlopeThread;
	std::vector<HANDLE>	hThread;
	std::vector<pPertThreadData> pDataArray;
	std::vector<pSlopeThreadData> pSlopeDataArray;
	PertThreadData		PixelDataArrayZero;
	SlopeThreadData		SlopeDataArrayZero;
	std::vector<int>	PertProgress;



	// stuff for DwdDiff algorithm
	double		bump_transfer_factor = 1.0;
	double		lightDirectionDegrees = 45.0;
	double		bumpMappingDepth = 50.0;
	double		bumpMappingStrength = 75.0;
	double		LightHeight = 2.0;		// height of light for slope calculation
	int		PaletteShift = 0;		// fractional palette addressing
	bool		OldPertFormat;			// used to get param values for old format perturbation par files

		// these are used to restore Mandelbrot after Julia - logically tied to current fractal - view persistent between calls
	double		oldhor = 0.0;
	double		oldvert = 0.0;
	double		oldwidth = 0.0;

	bool		ShowOrbits = true;		// show julia orbits for each image

	// control flags
	int		time_to_zoom = 0;		// time to zoom in or out?
	int		time_to_restart = 0;		// time to restart?
	int		time_to_reinit = 0;		// time to reinitialize?
	int		time_to_quit = 0;		// time to quit?
	int		time_to_load = 0;		// time to load file?
	int		time_to_break = 0;		// time to break out of animation?

	int		ThreadCreationDelay = 5;	// not sure if these are required
	int		ThreadCompletionDelay = 2;
	int		ThreadEndingDelay = 2;
	int		ThreadPertDelay = 0;		// if > 0, show pixel update in real time. Otherwise stay with screen update every second (faster)

    private:
	void	HandleFractalStateMachine(HWND hwnd, char* saveFile);
	void	HandleRestartPhase(HWND hwnd, char* saveFile);
	void	HandleReinitPhase(HWND hwnd, char* szSaveFileName);
	void	HandleLoadPhase(HWND hwnd);
	void	SetupBailoutDefaults();
	int	InitPixelFractal();
	void	SetupSpecialColourIndex(CTrueCol &TrueCol, long threshold, RGBTRIPLE SpecialColour, RGBTRIPLE &oldColour, int &SPECIALINDEX);
	int	DoPixelFractal(HWND hwnd, int distestwidth, int threadCount);
	void	RestoreSpecialColourSlot(CTrueCol &TrueCol, int SPECIALINDEX);
	int 	GetArithType();
	void	GenPositionStr(char *PositionStr);
	int	SpecialFractals(HWND hwnd/*, CPixel *Pix*/);
	void	InitPixelObjects(int threadCount, HWND hwnd);
	void	CreatePixelObjects(int threadCount);


	RGBTRIPLE   oldColour;				// used to hold original value of palette entry "stolen" for special colour used for Art Matrix Cubic
	int	SPECIALINDEX;				// points to the Special Colour in the palette - used for Art Matrix Cubic
	std::vector<int>            PixelProgress;
	std::vector<HANDLE>         hPixelThread;
	std::vector<pPixelThreadData>   pPixelDataArray;



    };

extern CManp *gManp;		// allow access to manp object in other files until we get them all moved inside

