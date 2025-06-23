/*-----------------------------------------
	Paul's Fractal Generator
  -----------------------------------------*/

#include <windows.h>
#include "manpwin.h"
#include "zoom.h"
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <htmlhelp.h>	// PHD 24/10/2007 tp allow *.CHM files
#include "fractype.h"
#include "Fract.h"
#include "Dib.h"
#include "resource.h"
#include "menu.h"
#include "fractalp.h"
#include "big.h"
#include "Undo.h"
#include "complex.h"
#include "BigDouble.h"
#include "colour.h"
#include "OscProcess.h"
#include "Plot.h"
#include "FrameCalculator.h"

extern	CDib	Dib;				// Device Independent Bitmap
extern	CFract	Fractal;			// Fractal specific stuff
extern	COscProcess	OscProcess;
extern	CPlot	Plot;				// image plotting routines 

WNDPROC	FAR PASCAL _export WndProc (HWND, UINT, UINT, LONG);
DLGPROC	FAR PASCAL _export AboutDlgProc (HWND, UINT, UINT, LONG);
WNDPROC	MenuCommand (HWND, UINT, UINT, LONG);
BOOL	InitNewFractal(HWND hwnd);
BOOL	UpdateFractal(HWND hwnd);
void	pfract_main(HWND, char *);
void	init(HWND);
void	DisplayLocation(HWND, POINTS);

void	ChangeView(HWND, int, int, int, int, int, int, int, int, char);
int	CopyArgument(HWND, char *, char *);
void	FindCursorRealPos(POINTS *);

extern	BOOL	wintext_initialize(HANDLE, HWND, LPSTR);
extern	void	output_batch(double, double, double, HWND, LPSTR);
extern	int	ZoomIn(HWND, RECT *);			// zoom functions in Zoom.c
extern	int	ZoomOut(HWND, RECT *);
extern	short	FilePalette(HWND, char *, char *);
extern	short	FileReadColours(HWND, char *, char *);
extern	int	read_png_file(HWND, char *);
extern	void	GetParam(HWND, LPSTR, LPSTR);  
extern	void	draw3D(HWND);  
extern	void	initFibonacci(void);
extern	int	load_par(HWND, char *);
extern	void	DisplayPalette(HWND, BOOL);
extern	void	DisplayFractal(HWND);
extern	int	rotate(int);
extern	int	CopyPictureToClipboard(HWND);
extern	int	mainview(HWND, BOOL);
extern	void	ClosePtrs(void);
extern	int	FractintPar(HWND, char *);
extern	int	DrawJulia(HWND, POINTS);
extern	int	InitRTJulia(HWND);
extern	void	LoadUndo(BOOL);
extern	void	InitUndo(void);
extern	void	Undo(struct UNDO *);
extern	void	Redo(struct UNDO *);
extern	int	ReadConfig(HWND);
extern	void	OutputStatusBar(HWND);
extern	int	SaveConfig(HWND);
extern	void	rotate_vga_palette(int, int);
extern	int	fpFormulaSetup(char *);
extern	void	ProcessKeys(HWND, UINT);
extern	char	*trailing(char *instr);
extern	HWND	DoHtmlHelp(HWND, char *, UINT);
extern	int	DoAnimation(void);
extern	int	AnimateClose(void);
extern	int	ProcessFormulaString(char *);
extern	int	MPEGWrite(char *);
extern	int	DisplayRGB(POINTS);
extern	int	EditRGB(HINSTANCE, POINTS);
extern	void	DrawSpots(void);
extern	void	ConvertBignum2String(char *s, mpfr_t num);
extern	int	TogglePerturbation(WORD *type, int *subtype);
extern	void	InitMultipliers(void);
extern	int	ReadKallesFile(HWND hwnd, char *filename);
extern	char	*GetFractalName(void);

extern	int	xdots, ydots, width, height;

extern	BOOL	DisplayAnimation;		// allow system to know that we are currently displaying an animation

static	char    argumentFileName[MAX_PATH+MAXDATALINE];
static	HINSTANCE  hInst;
static	HCURSOR  hCursor;
static	DLGPROC *lpfnAboutDlgProc;
static	char	szFileName[MAX_PATH];
static	char    szTitleName[MAXDATALINE] = "ManpWIN";
static	char	szSaveFileName[MAX_PATH];

extern	int	PaletteStart;
extern	double	lightDirectionDegrees;
extern	double	bumpMappingDepth;
extern	double	bumpMappingStrength;
extern	int	SlopeType;
extern	double	LightHeight;
extern	int	PalOffset;			// begin palette here
extern	double	IterDiv;			// divide ieration by this amount
extern	int	PertColourMethod;		// some ideas from Kalles
extern	int	RotationAngle;			// in degrees
extern	Complex	RotationCentre;			// centre of rotation
extern	char	LyapSequence[];			// hold the AB sequence for Lyapunov fractals

static	int	HorOffset, VertOffset;

static	BOOL	WasFractPar = FALSE;		// did we run a FRACTINT PAR file last time?
	BOOL	IsPAR = FALSE;			// are we currently running a PAR file?
	BOOL	IsKFR = FALSE;			// are we currently running a KFR file?

static	double	oldhor = 0.0;			// store values during julia transformations
static	double	oldvert = 0.0;			// init to 0 to restore reasonable Mandel values
static	double	oldwidth = 0.0;			// calculated from current Julia values
static	double	temp_1 = 0.0;
static	double	temp_2 = 0.0;

static	BOOL	DisplayLoc = FALSE;		// display the current cursor location in the caption bar
	BYTE	oldcalcmode;			// store values during 3D transformations

BOOL	bTrack = FALSE;				// TRUE if user is selecting a region
RECT	Rect;
int	Shape = SL_BLOCK;			// Shape to use for the selection rectangle

BOOL		BatchFlag;
BOOL		AutoExitFlag = FALSE;
BOOL		AutoSaveFlag;
BOOL		AutoStartFlag = FALSE;
BOOL		CommandLineFile = FALSE;	// does the command line contain a par filename?
//static	char	Directory[160];
HWND		CallingWindowHandle = 0L;	// Is ManpWIN called by an external window via WM_COPYDATA message?
int		ReplyUsingDIB;			// TRUE for DIB/WM_COPYDATA and FALSE for clipboard

//void	SetupView(HWND, BOOL);
void	InitFract(int);

	  // Functions in VIEWFILE.C
extern	void	ViewFileInitialize (HWND);
extern	DLGPROC	PARFileOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	PNGFileOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	SCIFileOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	LSTFileOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	SaveMPGOpenDlg (HWND, LPSTR, LPSTR);
extern	DLGPROC	LsysFileOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	FractintParFileOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	IFSFileOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	FormulaFileOpenDlg(HWND, LPSTR, LPSTR);

	  // Functions in SAVEFILE.C
extern	void	SaveFileInitialize (HWND, HINSTANCE);
extern	DLGPROC	SavePNGOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	SaveParOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC SaveKfrOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	SaveParImageOpenDlg(HWND, LPSTR, LPSTR);
extern	void	SaveFile(HWND, LPSTR, LPSTR);
extern	DLGPROC	SaveColFileOpenDlg (HWND, LPSTR, LPSTR);
extern	DLGPROC	SaveMAPFileOpenDlg (HWND, LPSTR, LPSTR);
extern	DLGPROC	ColFileOpenDlg (HWND, LPSTR, LPSTR);
extern	DLGPROC	MAPFileOpenDlg(HWND, LPSTR, LPSTR);
extern	DLGPROC	KFRFileOpenDlg(HWND, LPSTR, LPSTR);

extern	void	SetScrollRanges(HWND);
extern	void	setup_defaults(void);
extern	void	InitTrueColourPalette(BYTE);
extern	int	StartSelection(HWND, POINTS, LPRECT, int);
extern	int	UpdateSelection(HWND, POINTS, LPRECT, int);
extern	int	EndSelection(POINTS, LPRECT);
extern	int	ClearSelection(HWND, LPRECT, int);
extern	int	load_lsystems(HWND, char *);
extern	int	Lsystem(HWND, char *);
extern	int	calcfracinit(void);
extern	int	get_IFS_names(HWND, char *);		// get the IFS fractal names
extern	int	ifsload(HWND, char *);
extern	int	RunScript(HWND, char *);
extern	int	GetPNGSeqFromScript(HWND hwnd, char *FileName);

extern	int	get_formula_names(HWND, char *);	// get the fractal formula names
extern	int	setup_Tierazon(void);			// count how many Tierazon fractals there are
extern	int	setup_MandelDeriv(void);		// count how many Mandelbrot Derivatives there are
extern	int	setup_Perturbation(void);		// count how many Perturbation fractals there are
extern	int	setup_SlopeDeriv(void);			// count how many Slope Deriv fractals there are
extern	int	setup_SlopeFwdDiff(void);		// count how many Slope fractals there are
extern	int	setup_Oscillator(void);			// count how many Oscillator fractals there are
extern	int	setup_FractalMaps(void);		// count how many Fractal map fractals there are
extern	int	setup_SprottMaps(void);			// count how many Sprott map fractals there are
extern	int	setup_Surface(void);			// count how many Surface fractals there are
extern	int	setup_Knot(void);			// count how many Knot fractals there are
extern	int	setup_Curve(void);			// count how many Curve fractals there are
extern	void	LoadTierazonParams(void);		// get params from Tierazon database 
extern	void	LoadMandelDerivParams(void);		// get params from Mandelbrot Derivatives database 
extern	void	LoadSlopeDerivParams();			// get params from Slope database 
extern	void	LoadSlopeFwdDiffParams();
extern	int	EndSlope(void);				// end slope threads before exiting

extern	void	LoadPerturbationParams(void);		// get params from Perturbation database
extern	void	LoadParams(void);			// get params from  database
extern	int	DoUpdate(void);

extern	DLGPROC FAR PASCAL	ImageSizeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	RTJuliaLocDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	PlotTypeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	FractTypeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	FractalDlg(HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	FractalTestDlg(HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	ColourDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	EditPalDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	ExpTypeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	PowerTypeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	LSystemDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	NewtonGeneral (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	SpecNewton (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	SelectFractal (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	SelectIFS (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	RatTypeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	NewtonTypeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	HenonTypeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	PlasmaDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	FrothDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	CellularDlg (HWND, UINT, UINT, LONG) ;

extern	DLGPROC FAR PASCAL	FibTypeDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	WalkDlg (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL	StereoPairDlg(HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	Param3D (HWND, UINT, UINT, LONG) ;
extern	DLGPROC FAR PASCAL 	JuliaDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL 	InsideDlg (HWND, UINT, UINT, LONG);
extern	int			GetParamData(HWND, LPSTR, LPSTR, LPSTR, BOOL);
extern	DLGPROC FAR PASCAL	AnimationDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	JuliaAnimDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	InversionAnimDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	AnimStartDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	CoordDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	ParamDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	SpecialAnimDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	FourierTypeDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	FourierAnimDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	NewtonCmplxDlg (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	SelectFracParams (HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	SelectTierazonDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectSlopeDerivDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectSlopeFwdDiffDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectMarcusDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectMandelDerivDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectPertDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	ThreadingDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectOscillatorDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectColourDlg (HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectFilterDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	SelectFDOptionDlg(HWND, UINT, WPARAM, LPARAM);
extern	DLGPROC FAR PASCAL	ScrnFormDlg(HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	ParamAnimDlg(HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	OscillatorAnimDlg(HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	OscMorphAnimDlg(HWND, UINT, UINT, LONG);
extern	DLGPROC FAR PASCAL	StatusInfoDlg(HWND, UINT, UINT, LONG);

extern	short	save_colour(HWND, char *, char *);
extern	short	save_palette(HWND, char *, char *);
extern	void	DisplayStatusBarInfo(int, char *);

extern	BOOL	AutoStereoActive;		// is autostereo currently running?
extern  int	screenx;
extern  int	screeny;
extern  int	caption;			// size of windows caption and scroll bars
extern  int	scroll_width;			// size of horizontal scroll bars
extern  int	max_vscroll, max_hscroll;
extern  int	display_width, display_height;
extern	int	row, col, curpass, totpasses;
extern	double	HenonA, HenonXStart, HenonYStart;
extern	int	HenonPoints;
extern	double	param[];
extern	char	FormulaString[];		// used to hold the full formula
extern	char	statname[];			// MPEG stat file. If statname[] == '-', stats sent to stdout.
extern	BOOL	WriteMPEGFrames;		// write frames directly to an MPEG file

extern	char	lptr[][100];
extern	int	lsys_ptr;
extern	char	lsys_type[];

extern	BYTE	_3dflag;			// replay saved file. 3 = 3D
extern	BOOL	ZoomEdge;			// Zooming process
extern	BOOL	UseFractintPalette;		// standard EGA palette
extern	CTrueCol    TrueCol;			// palette info
extern	int	InsideMethod;			// inside filters
extern	int	OutsideMethod;			// outside filters
//extern	int	biomorph;			// biomorph colour
//extern	int	PlotType;
extern	int	FilterType;			// data for Tierazon filters
extern	int	ColourMethod;			// Tierazon colour methods
extern	int	nFDOption;			// Fractal Dimension option for Tierazon filters
extern	BYTE	cycleflag;			// do colour cycling
extern	int	finished;			// all passes complete
extern	WORD	type;				// M=mand, N=Newton etc
extern	int	subtype;
extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address
extern	double	mandel_width;			// width of display
extern	long	threshold;			// maximum iterations
extern	BYTE	juliaflag;			// Julia implementation of fractal
extern	BYTE	RealTimeJuliaFlag;		// Display Julia set in real time
extern	int	ifs_type;
extern	BYTE	calcmode;			// trace type B, G, 1, 2
extern	WORD	special;			// special colour, phase etc
extern	BYTE	pairflag;			// stereo pair
extern	BYTE	addflag;			// add to spirals?
extern	int	MaxDimensions;			// Oscillator etc
extern	BOOL	DisplayAxisLabels;		// show labels for axis pairs

	BOOL	GetPixelColourFlag;		// get the colour of the pixel when mouse button pressed
extern	BOOL	StartImmediately;		// immediate start of animation generation
extern	BOOL	RunAnimation;			// are we in the middle of an animation run?
extern	char	ScriptFileName[];		// base name for PNG file animation sequence

extern	HWND	GlobalHwnd;			// to allow passing of hwnd to find_file_item()

extern	PAINTSTRUCT 	ps;
extern	HDC		hdcMem;			// load picture into memory
extern	RECT 		r;
extern	HPALETTE 	hpal;
extern	HANDLE  	hLogPal;       		// Temporary Handle
extern	BYTE	default_palette[];		// default VGA colour palette

extern	double	x_rot;				// angle display plane to x axis
extern	double	y_rot;				// angle display plane to y axis
extern	double	z_rot;				// angle display plane to z axis

extern	int	xAxis, yAxis, zAxis;		// numerical values for axes for chaotic oscillators

extern	char	PNGPath[];			// path for PNG files
extern	char	COLPath[];			// path for COL files
extern	char	MAPPath[];			// path for MAP files
//extern	char	SCIPath[];		// path for SCI files
extern	char	PARPath[];			// path for PAR files
extern	char	KFRPath[];			// path for KFR files

extern	char	FracPARFile[];			// Fractint PAR file
extern	char	PARFile[];			// PAR file
extern	char	KFRFile[];			// KFR file
extern	char	IFSFile[];			// IFS file
extern	char	LSYSFile[];			// LSYS file
extern	char	FRMFile[];			// Formula file
extern	char	PNGFile[];			// PNG file
extern	char	COLFile[];			// COL file
extern	char	SCIFile[];			// SCI file
extern	char	MPGFile[];			// MPG file
extern	char	LSTFile[];			// list file for PNG animation frames

extern	int	time_to_zoom;			// time to zoom in or out?
extern	int	time_to_restart;		// time to restart?
extern	int	time_to_reinit;			// time to reinitialize?
extern	int	time_to_quit;			// time to quit?
extern	int	time_to_load;			// time to load file?
extern	int	time_to_break;			// time to break out of animation?
extern	int	DataFromPNGFile;		// loaded PNG file?
extern	BOOL	bTrack;				// TRUE if user is selecting a region
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	char	MAPFile[];			// colour map file
extern	double	rqlim, rqlim2;

extern	BOOL	RTJuliaActive;			// block any moire requests until calcs complete

extern	int	AnimateSuspend(void);
extern	int	AnimateResume(void);
extern	BOOL	SuspendAnimation;		// pause animation run?

extern	BOOL	DisplayAxes;			// display axes for oscillator etc
extern	BOOL	PlotCentre;			// display circle at the centre of the oscillator

// Big num declarations **********************************************************
extern	BYTE	BigNumFlag;		// True if bignum used
extern	int	decimals, precision;
	BigDouble   Big_oldhor, Big_oldvert, Big_oldwidth;
extern	BigDouble   BigHor, BigVert, BigWidth;
// Big num declarations **********************************************************

short	vscroll_count = 0;			// vertical scroll count
short	hscroll_count = 0;			// horizontal scroll count 
BOOL	Reopen_flag = FALSE;			// re-open file
BOOL	StartPicture = TRUE;			// starting picture
HANDLE	hAccTable;				// handle to accelerator table
char	AccTableAddr[120];			// name of accelerator table       
HWND	PixelHwnd;				// pointer to handle for pixel updating
HWND	hWndCopy;				// Copy of hwnd
int     file_type = FILE_PNG;			// got to start somewhere

HWND	Secondaryhwnd;
UINT	Secondarymessage;
UINT	SecondarywParam;
LONG	SecondarylParam;

// Styles of app. window
DWORD	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | 
                  WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;

POINT	ptSize;					// Stores DIB dimensions
extern	Complex	j, q;

/*-----------------------------------------
	Main Windows Entry Point
  -----------------------------------------*/

//#pragma argsused
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)

    {
    HWND	hwnd;
    WNDCLASS	wndclass;

    if (!hPrevInstance)
	{
	wndclass.style	 	= CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc	= (WNDPROC)WndProc;
	wndclass.cbClsExtra		= 0;
	wndclass.cbWndExtra		= 0;
	wndclass.hInstance		= hInstance;
	wndclass.hIcon		= LoadIcon (hInstance, "ManpWin");
	wndclass.hCursor		= LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName	= "ManpWin";
	wndclass.lpszClassName	= "ManpWin";

	if (!RegisterClass (&wndclass))
	    return FALSE;
	}

    hwnd = CreateWindow ("ManpWin", "Paul's Fractals",
     			  dwStyle, 
			  160, 120, 				// nice initial location
			  500, 300, 				// nice initial size
			  NULL, NULL, hInstance, NULL);
    GlobalHwnd = hwnd;

    wsprintf(argumentFileName, lpszCmdLine, hwnd);	// in case we want to analyse arguments

    UpdateWindow (hwnd);
    type = MANDELFP;
    setup_defaults();				// all MANP specific setups
    PixelHwnd = hwnd;				// Used in putcolor. Saves a lot of param passing.
    init(hwnd);					// init the main Fractint and screen code 
    initFibonacci();				// just the starting values
#ifdef	DEBUG
    strcpy(szFileName, "Test Filename");
#endif
    strcpy(szTitleName, "ManpWin");
    if (*argumentFileName != '\0')			// command line filename
	CopyArgument(hwnd, szFileName, argumentFileName);	// ensure correct filename copied
    ShowWindow (hwnd, SW_MAXIMIZE);
    pfract_main(hwnd, szSaveFileName);		// fire up the main Fractint code
    DestroyWindow(hWndCopy);			// stop everything when it returns
    return(FALSE);				// we done when 'pfract_main' returns
    }

/*-----------------------------------------
	Just a quicky
  -----------------------------------------*/

void	DoCaption (HWND hwnd, char *szTitleName)
     {
     char szCaption [184];

     wsprintf (szCaption, "%s", (LPSTR) (szTitleName));
     SetWindowText (hwnd, szCaption);
     }

/*-----------------------------------------
	Let's Process a few messages
  -----------------------------------------*/

WNDPROC FAR PASCAL _export WndProc (HWND hwnd, UINT message, UINT wParam, LONG lParam)
     {
     static FARPROC lpfnMonoDlgProc;
     static char    TempString  [480];
     int            iMax;
     int            iMin;
     int            iPos;
     int            dn;
                   
     switch (message)
	  {
	  case WM_CREATE:
		hInst = ((LPCREATESTRUCT)(_int64)lParam)->hInstance;
		lpfnAboutDlgProc = MakeProcInstance ((DLGPROC *)AboutDlgProc, hInst);
		ZoomEdge = TRUE;
		UseFractintPalette = FALSE;
		ReadConfig(hwnd);				// sticky info
		ViewFileInitialize (hwnd);			// Initialise file reading routines
		SaveFileInitialize (hwnd, hInst);		// Initialise file writing routines
		vscroll_count = 0;				// no scrolling yet
		hscroll_count = 0;
		szFileName[0] = '\0';				// can't reopen if not there
		SetScrollPos(hwnd, SB_VERT, 0, FALSE);
		SetScrollPos(hwnd, SB_HORZ, 0, FALSE);

#ifdef DEBUG
wsprintf(TempString, "About to enter mainview() in WM_CREATE! argument = <%s>", argumentFileName);
MessageBox (hwnd, TempString, szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

	       if (*argumentFileName != '\0')				// command line filename
		   {
		   wsprintf(TempString, "View File %s", szFileName);
		   DoCaption(hwnd, TempString);
		   CopyArgument(hwnd, szFileName, argumentFileName);	// ensure correct filename copied

#ifdef DEBUG
wsprintf(TempString, "About to enter mainview() in WM_CREATE! file = <%s>", szFileName);
MessageBox (hwnd, TempString, szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

		   SendMessage (hwnd, WM_COMMAND, IDM_OPEN_PAR, 0L);	// let the user try
		   *argumentFileName = '\0';              // only look at command line file once
		   }
		return 0;

	  case WM_INITMENUPOPUP:
		switch (lParam)
		    {
		    case 0:	   // File menu
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_NEW, MF_GRAYED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_OPEN_PAR, MF_ENABLED);
			 EnableMenuItem((HMENU)(_int64)wParam, IDM_OPEN_PNG, MF_ENABLED);
			 EnableMenuItem((HMENU)(_int64)wParam, IDM_OPEN_KALLES, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_SAVEPAR, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_SAVEIMAGE, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_SAVE_PAR_AND_IMAGE, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_EXIT, MF_ENABLED);
			 break;
		    case 1:	   // Edit Dialogs menu
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_COPY, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_UNDO, MF_ENABLED);
			 break;
		    case 2:	   // Fractal Dialogs menu
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_FRACOPTIONS, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDC_FRACTYPE, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_3DPARAM, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_FRACTLOC, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_FRACTALCOORDS, MF_ENABLED);
			 break;
		    case 3:	   // Fractal Options menu
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_3D, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_JULIA, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_RDS, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_STEREO, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDC_ADDSPIRAL, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_GENMPEGSTATS, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_EDGE, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_DISPLAYLOC, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_USEDEFAULTPALETTE, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_IMAGE_SIZE, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_START, MF_ENABLED);
			 EnableMenuItem((HMENU)(_int64)wParam, IDM_STEREOPAIRSETUP, MF_ENABLED);
			 break;
		    case 4:	   // Animation Dialogs menu
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_SETUPANIMATION, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_JULIA_ANIM, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_STOPANIM, MF_ENABLED);
			 break;
		    case 5:	   // Colour Options menu
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_OPEN_PAL_MAP, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_OPEN_TRUE_MAP, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_SAVE_TRUE_MAP, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_SAVE_PAL_MAP, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_CYCLE, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_COLOUR_PAL, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_SETINSIDE, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_EDITPAL, MF_ENABLED);
			 break;
		    case 6:	   // Help menu
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_HELP, MF_ENABLED);
			 EnableMenuItem ((HMENU)(_int64)wParam, IDM_ABOUT, MF_ENABLED);
			 break;
		    }
		return 0;

	  case WM_INITMENU:
            // check/uncheck menu items depending on state  of related flags

	        CheckMenuItem((HMENU)(_int64)wParam, IDC_ADDSPIRAL, (addflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_3D, (_3dflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_JULIA, (juliaflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_REALTIMEJULIA, (RealTimeJuliaFlag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_CYCLE, (cycleflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_STEREO, (pairflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_GENMPEGSTATS, ((statname[0] != '-') ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_EDGE, (ZoomEdge ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_USEDEFAULTPALETTE, (UseFractintPalette ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_DISPLAYLOC, (DisplayLoc ? MF_CHECKED : MF_UNCHECKED));
//		CheckMenuItem((HMENU)wParam, IDM_SHOWSTATUSBAR, (ShowStatusBar ? MF_CHECKED : MF_UNCHECKED));

		break;
		
	  case WM_COMMAND:
                // Process menu commands
                return MenuCommand (hwnd, message, wParam, lParam);
                break;

	  case WM_TIMER:
	       if (wParam == ANIMTIMER)
		    DoAnimation();
	       else if (wParam == UPDATETIMER)
		    DoUpdate();
	       return 0;

	  case WM_MOVE:
		ShowWindow (hwnd, SW_SHOWNORMAL);
		break;

	  case WM_SIZE:
		InvalidateRect(hwnd, &r, FALSE);
		SetScrollRanges(hwnd);
		GetClientRect(hwnd, &r);
		xdots = width;
		ydots = height;
		return 0;

	  case WM_LBUTTONDOWN:				// message: left mouse button pressed
	  case WM_RBUTTONDOWN:				// message: right mouse button pressed
		cycleflag = FALSE;
		if (OscProcess.DisplayAxisImages)
		    {
		    OscProcess.ChooseOsc(MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y);
		    break;
		    }
		if (GetPixelColourFlag)
		    {
		    EditRGB(hInst, MAKEPOINTS(lParam));
		    GetPixelColourFlag = FALSE;
		    DisplayFractal(hwnd);
		    break;
		    }
		if (GetKeyState(VK_SHIFT) < 0)	// find julia from this point
		    {
		    bTrack = FALSE;
		    if (juliaflag)
			{
			hor = oldhor;		// restore values after julia transformations
			vert = oldvert;
			mandel_width = oldwidth;
			juliaflag = FALSE;
			}
		    else
			{
			j.x = hor + mandel_width * (double)(MAKEPOINTS(lParam).x) / (double)ydots;    
			j.y = vert + mandel_width * (double)(MAKEPOINTS(lParam).y) / (double)ydots;    
			oldhor = hor;		// store values during julia transformations
			oldvert = vert;
			oldwidth = mandel_width;
			hor = -3.555555555555556;
			vert = -2.0;
			mandel_width = 4.0;
			juliaflag = TRUE;
			}
		    time_to_zoom = TRUE;
		    }
		else
		    {					// Start selection of region
		    bTrack = TRUE;
		    SetRectEmpty(&Rect);
		    StartSelection(hwnd, MAKEPOINTS(lParam), &Rect, 
					(wParam & MK_SHIFT) ? (SL_EXTEND | Shape) : Shape);
		    }
		break;

	  case WM_MOUSEMOVE:				// message: mouse movement

		if (GetPixelColourFlag)
		    {
		    DisplayRGB(MAKEPOINTS(lParam));		// display RGB and location
		    break;
		    }
		if (DisplayLoc)
		    DisplayLocation(hwnd, MAKEPOINTS(lParam));
							// update the selection region
		if (bTrack)
		    UpdateSelection(hwnd, MAKEPOINTS(lParam), &Rect, Shape);
		else if (RealTimeJuliaFlag && !juliaflag)
		    {
		    if (!RTJuliaActive)					// give last request time to complete
			{
			if (DrawJulia(hwnd, MAKEPOINTS(lParam)) < 0)
			    RealTimeJuliaFlag = FALSE;
			}
		    }
		break;

	  case WM_LBUTTONUP:				// message: left mouse button released

		if (bTrack) 
		    {
		    EndSelection(MAKEPOINTS(lParam), &Rect);
		    ClearSelection(hwnd, &Rect, Shape);
		    if (ZoomIn(hwnd, &Rect))
			time_to_zoom = TRUE;
		    }
		return 0;

	  case WM_RBUTTONUP:				// message: left mouse button released

		if (bTrack) 
		    {
		    EndSelection(MAKEPOINTS(lParam), &Rect);
		    ClearSelection(hwnd, &Rect, Shape);
		    if (ZoomOut(hwnd, &Rect))
			time_to_zoom = TRUE;
		    }
		return 0;

	  case WM_COPYDATA:
#ifdef	DEBUG
		sprintf(TempString, "hWnd=%8.8X, wParam=%8.8X, lParam=%8.8X\ncopydata = <%s>", hwnd, wParam, lParam, 
		  ((PCOPYDATASTRUCT)lParam) -> lpData);
		MessageBox (hwnd, TempString, "Received Copydata message", MB_ICONEXCLAMATION | MB_OK);
#endif
		setup_defaults();
		GetParamData(hwnd, "null", (char *)((PCOPYDATASTRUCT)(_int64)lParam) -> lpData, szSaveFileName, FALSE);
			// 2 for Device Context, 1 for DIB/WM_COPYDATA and 0 for clipboard
		ReplyUsingDIB = (int)(((PCOPYDATASTRUCT)(_int64)lParam) -> dwData);
		CallingWindowHandle = (HWND)(_int64)wParam;
		InitTrueColourPalette(FALSE);

		time_to_reinit = TRUE;
		Secondaryhwnd = hwnd;
		Secondarymessage = message;
		SecondarywParam = wParam;
		SecondarylParam = lParam;
		break;

	  case WM_CLOSE:
		if (MessageBox (hwnd, "Do you want to Quit?", "ManpWin", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)
		    {
		    time_to_quit = TRUE;
		    SendMessage (hwnd, WM_DESTROY, 0, 0L);
		    }
		else
		    return 0;

		time_to_quit = TRUE;
		hWndCopy = hwnd;
		return 0;

	  case WM_PAINT:
		ChangeView(hwnd, -GetScrollPos (hwnd, SB_HORZ), -GetScrollPos (hwnd, SB_VERT), ptSize.x, ptSize.y, 0, 0, width, height, TRUE);
		return 0;
        

          case WM_VSCROLL:
            						// Calculate new vertical scroll position 
            	GetScrollRange (hwnd, SB_VERT, &iMin, &iMax);
            	iPos = GetScrollPos (hwnd, SB_VERT);
            	GetClientRect (hwnd, &r);

	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
            	    {
                    case SB_LINEDOWN:
                        dn =  r.bottom / 16 + 1;
                        break;

                    case SB_LINEUP:
                        dn = -r.bottom / 16 + 1;
                        break;

                    case SB_PAGEDOWN:
                        dn =  r.bottom / 2  + 1;
                        break;
                        
                    case SB_PAGEUP:
                        dn = -r.bottom / 2  + 1;
                        break;

                    case SB_THUMBTRACK:
                    case SB_THUMBPOSITION:
#ifdef	WIN95								// 32 bit code
                        dn = HIWORD(wParam)-iPos;
#else									// 16 bit code
                        dn = LOWORD(lParam)-iPos;
#endif
                        break;

                    default:               dn = 0;
	            }
							// Limit scrolling to current scroll range
                if (dn = BOUND (iPos + dn, iMin, iMax) - iPos) 
                    {
                    ScrollWindow (hwnd, 0, -dn, NULL, NULL);
                    SetScrollPos (hwnd, SB_VERT, iPos + dn, TRUE);
                    }
		InvalidateRect(hwnd, &r, FALSE);
                break;

          case WM_HSCROLL:
            						// Calculate new horizontal scroll position
                GetScrollRange (hwnd, SB_HORZ, &iMin, &iMax);
                iPos = GetScrollPos (hwnd, SB_HORZ);
                GetClientRect (hwnd, &r);

	        switch ((int) LOWORD(wParam))
                    {
                    case SB_LINEDOWN:
                        dn =  r.right / 16 + 1;
                        break;

                    case SB_LINEUP:
                        dn = -r.right / 16 + 1;
                        break;

                    case SB_PAGEDOWN:
                        dn =  r.right / 2  + 1;
                        break;

                    case SB_PAGEUP:
                        dn = -r.right / 2  + 1;
                        break;

                    case SB_THUMBTRACK:
                    case SB_THUMBPOSITION:
#ifdef	WIN95								// 32 bit code
                        dn = HIWORD (wParam) -iPos;
#else									// 16 bit code
                        dn = LOWORD (lParam) - iPos;
#endif
                        break;

                    default:
                        dn = 0;
	            }
            						// Limit scrolling to current scroll range
                if (dn = BOUND (iPos + dn, iMin, iMax) - iPos) 
                    {
                    ScrollWindow (hwnd, -dn, 0, NULL, NULL);
                    SetScrollPos (hwnd, SB_HORZ, iPos + dn, TRUE);
                    }
		InvalidateRect(hwnd, &r, TRUE);
                break;

        case WM_KEYDOWN:					// Handle any keyboard messages
	    ProcessKeys(hwnd, wParam);
	    break;

	case WM_DESTROY :
		if (RunAnimation)
		    {
		    time_to_break = TRUE;
		    RunAnimation = FALSE;
		    AnimateClose();
		    }
		if (type == SLOPEDERIVATIVE || SLOPEFORWARDDIFF)
		    {
		    EndSlope();					// advise the threads to close
		    Sleep(250);					// give them a quarter second to do so
		    }
		ClosePtrs();
		SaveConfig(hwnd);
		HtmlHelp(hwnd, NULL, HH_CLOSE_ALL, (DWORD)NULL);
		PostQuitMessage (0);
	       	return 0;
	  }
     return (WNDPROC)DefWindowProc (hwnd, message, wParam, lParam);
     }	

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : MenuCommand ( HWND hwnd, WORD wParam)                      *
 *                                                                          *
 *  PURPOSE    : Processes menu commands.                                   *
 *                                                                          *
 *  RETURNS    : TRUE  - if command could be processed.                     *
 *               FALSE - otherwise                                          *
 *                                                                          *
 ****************************************************************************/

WNDPROC MenuCommand (HWND hwnd, UINT message, UINT wParam, LONG lParam)

    {
    int		status;
    char	TempStr[MAX_PATH];

    if (DisplayAnimation)			// we are currently displaying an animation
	{
	time_to_break = TRUE;
	AnimateClose();
	}

    switch (wParam)
	{
	case IDM_HELP:				// Messages from Help menu

						// Load hour-glass cursor.
       	    hCursor = LoadCursor(NULL, IDC_WAIT);
       	    SetCursor(hCursor);
	    DoHtmlHelp(hwnd, "ManpWIN.chm", HH_DISPLAY_TOPIC);

			    // Reload arrow cursor.
       	    SetCursor(hCursor);
	    return 0;

	case IDM_ABOUT:
	    DialogBox (hInst, "AboutBox", hwnd, (DLGPROC)lpfnAboutDlgProc);
	    return 0;

	case IDM_PATHS:
	    DialogBox (hInst, "CreateDirDlg", hwnd, (DLGPROC)CreateDirDlg);
	    return 0;

	case IDM_GET_PIXEL_COLOUR:
	    GetPixelColourFlag = TRUE;	// get the colour of the pixel when mouse button pressed
	    return 0;

	case IDM_SETINSIDE:
	    if (DialogBox (hInst, "InsideDlg", hwnd, (DLGPROC)InsideDlg))
		DisplayFractal(hwnd);
	    return 0;

 				  // Messages from Fractals menu
	case IDM_FRACTLOC:                
	    if (RealTimeJuliaFlag)
		DialogBox (hInst, "RTJuliaLocDlg", hwnd, (DLGPROC)RTJuliaLocDlg);
	    else
		DialogBox(hInst, "StatusInfoDlg", hwnd, (DLGPROC)StatusInfoDlg);
	    break;

	case IDM_CYCLE:
	    cycleflag = !cycleflag;       
	    if (cycleflag)
		rotate((int)lParam);
	    break;

	case IDM_STEP:
	    rotate_vga_palette((int)lParam, 0);
	    break;

	case IDM_RDS:
	    draw3D(hwnd);
	    InvalidateRect(hwnd, &r, FALSE);
	    return 0;

	case IDM_RDS_SPOTS:
	    if (AutoStereoActive)
		DrawSpots();
	    InvalidateRect(hwnd, &r, FALSE);
	    return 0;

	case IDM_SETUPANIMATION:
	case IDM_JULIA_ANIM:
	case IDM_INVERSION_ANIM:
	case IDM_FOURIERANIM:
	case IDM_PARAM_ANIM:
	case IDM_OSCILLATOR_ANIMATION:
	case IDM_MORPHOSC:
	    if (TrueCol.PalEditFlag)						// palette has changed
		{
		if (MessageBox (hwnd, "Palette has changed! Do you wish to save first?", "ManpWIN", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		    {
		    if (SaveMAPFileOpenDlg (hwnd, szSaveFileName, szTitleName) == 0)
			{
			save_palette(hwnd, szSaveFileName, "Save Colour Parameters");
			strcpy(MAPFile, szSaveFileName);				// palette file to reflect the current map
			}
		    }
		TrueCol.PalEditFlag = FALSE;
		}
	    status = 0;
	    switch (wParam)
		{
		case IDM_SETUPANIMATION:
		    status = (int)DialogBox (hInst, "AnimationDlg", hwnd, (DLGPROC)AnimationDlg);
		    break;
		case IDM_PARAM_ANIM:
		    if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
			status = (int)DialogBox (hInst, "OscParamAnimDlg", hwnd, (DLGPROC)ParamAnimDlg);
		    else if (type == PERTURBATION)
			status = (int)DialogBox(hInst, "PertParamAnimDlg", hwnd, (DLGPROC)ParamAnimDlg);
		    else
			status = (int)DialogBox(hInst, "ParamAnimDlg", hwnd, (DLGPROC)ParamAnimDlg);
		    break;
		case IDM_MORPHOSC:
		    if (type != OSCILLATORS && type != FRACTALMAPS && type != SPROTTMAPS/*&& type != SURFACES*/ && type != KNOTS && type != CURVES)
			{
			MessageBox(hwnd, "This Fractal is not an Oscillator, Fractal Map, Curve or Knot", "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			break;
			}
		    status = (int)DialogBox(hInst, "OscMorphAnimDlg", hwnd, (DLGPROC)OscMorphAnimDlg);
		    break;
		case IDM_OSCILLATOR_ANIMATION:
		    switch (type)
			{
			case OSCILLATORS:
			case FRACTALMAPS:
			case SPROTTMAPS:
			case SURFACES:
			case KNOTS:
			case CURVES:
			case FPPICKOVER:
			case FPLORENZ3D:
			case FPLORENZ3D1:
			case FPLORENZ3D3:
			case FPLORENZ3D4:
			case KAM3DFP:
			case FPROSSLER:
			case MALTHUS:
			    status = (int)DialogBox (hInst, "OscillatorAnimDlg", hwnd, (DLGPROC)OscillatorAnimDlg);
			    break;
			case IFS:
			    if (ifs_type == 1)
				{
				status = (int)DialogBox (hInst, "OscillatorAnimDlg", hwnd, (DLGPROC)OscillatorAnimDlg);
				break;
				}	// else fall through
			default:
			    wsprintf(TempStr, "Fractal type %d (%s) cannot be configured for animation!\nChange the fractal type first", type, fractalspecific[type].name);
			    MessageBox (hwnd, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
			    status = 0;
			    break;
			}
		    break;
		case IDM_JULIA_ANIM:
		    if (type == PERTURBATION || type == OSCILLATORS || type == FRACTALMAPS || type == CURVES || type == KNOTS || type == SPROTTMAPS || type == SURFACES || type == BUDDHABROT || type == CURLICUES || type == MANDELCLOUD || type == DYNAMICFP)
			{
			wsprintf(TempStr, "Fractal type %d (%s) cannot be configured for Julia animation!\nChange the fractal type first", type, fractalspecific[type].name);
			MessageBox(hwnd, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
			}
		    else
			status = (int)DialogBox (hInst, "JuliaAnimDlg", hwnd, (DLGPROC)JuliaAnimDlg);
		    break;
		case IDM_INVERSION_ANIM:
		    status = (int)DialogBox (hInst, "JuliaAnimDlg", hwnd, (DLGPROC)InversionAnimDlg);
		    break;
		case IDM_FOURIERANIM:
		    if (DialogBox (hInst, "FourierTypeDlg", hwnd, (DLGPROC)FourierTypeDlg) > 0)
			status = (int)DialogBox (hInst, "FourierAnimDlg", hwnd, (DLGPROC)FourierAnimDlg);
		    break;
		}
	    if (status > 0)
		if (StartImmediately)
		    {
		    if (WriteMPEGFrames)
			{
			if (MPEGWrite(MPGFile) == -1)
			    return (WNDPROC)(-1);
			}
		    else
			{
			if (RunScript(hwnd, ScriptFileName) == -1)
			    return (WNDPROC)(-1);
			}
		    }

	    return (WNDPROC)(1);

	case IDM_STOPANIM:
	    AnimateClose();
	    time_to_break = TRUE;
	    return 0;

	case IDM_PAUSEANIM:
	    SuspendAnimation = !SuspendAnimation;
	    if (SuspendAnimation)
		AnimateSuspend();
	    else
		AnimateResume();
	    return 0;


	case IDM_SETUPTHREADS:
	    if (DialogBox(hInst, "ThreadingDlg", hwnd, (DLGPROC)ThreadingDlg) == TRUE)
		{
		DisplayStatusBarInfo(INITIALISING, "");		// display status bar
		time_to_reinit = TRUE;
		}
	    else
		return 0;
	    break;

	case IDM_COLOUR_PAL:
	    if (DialogBox (hInst, "ColourDlg", hwnd, (DLGPROC)ColourDlg) == TRUE)
		{
		InvalidateRect(hwnd, &r, FALSE);
		InitTrueColourPalette(TRUE);
		Plot.RefreshScreen();
		DisplayPalette(hwnd, TrueCol.DisplayPaletteFlag);
		DisplayFractal(hwnd);
		}
	    return 0;

	case IDM_EDITPAL:
	    if (DialogBox (hInst, "EditPalDlg", hwnd, (DLGPROC)EditPalDlg) == TRUE)
		{
		InvalidateRect(hwnd, &r, FALSE);
		InitTrueColourPalette(TRUE);
		Plot.RefreshScreen();
		DisplayPalette(hwnd, TrueCol.DisplayPaletteFlag);
		DisplayFractal(hwnd);
		}
	    return 0;

	case IDM_EDGE:
	    ZoomEdge = !ZoomEdge;
	    return 0;

	case IDM_USEDEFAULTPALETTE:
	    UseFractintPalette = !UseFractintPalette;
	    InitTrueColourPalette(FALSE);
	    time_to_reinit = TRUE;
	    return 0;

	case IDM_GENMPEGSTATS:
	    statname[0] = (statname[0] == '-') ? '\0' : '-';	// swap state
	    return 0;

	case IDM_DISPLAYLOC:
	    DisplayLoc = !DisplayLoc;
	    if (!DisplayLoc)
		DisplayFractal(hwnd);		// restore status
	    return 0;

	case IDM_COPY: 
	    if (CopyPictureToClipboard(hwnd) == -1) 
		{
		MessageBox (hwnd, "Error copying image to the clipboard!",
					 "ManpWin", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);
		}
	    return 0;

	case IDM_SAVE_PAR_AND_IMAGE:						// par file and image file
	    if (SaveParImageOpenDlg(hwnd, szSaveFileName, szTitleName) == 0)
		SaveFile(hwnd, szSaveFileName, szTitleName);			// par file saved in dialogue box, we save image here
	    return 0;

	case IDM_SAVEPAR:						// par file
	    if (TrueCol.PalEditFlag)					// palette has changed
		{
		if (MessageBox (hwnd, "Palette has changed! Do you wish to save first?", "ManpWIN", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		    {
		    if (SaveMAPFileOpenDlg (hwnd, szSaveFileName, szTitleName) == 0)
			{
			save_palette(hwnd, szSaveFileName, "Save Colour Parameters");
			strcpy(MAPFile, szSaveFileName);				// palette file to reflect the current map
			}
		    }
		TrueCol.PalEditFlag = FALSE;
		}

	    if (SaveParOpenDlg (hwnd, szSaveFileName, szTitleName) == 0)
		SaveFile (hwnd, szSaveFileName, szTitleName);                    
	    return 0;

	case IDM_SAVE_KALLES:						// kfr file
	    if (TrueCol.PalEditFlag)					// palette has changed
		{
		if (MessageBox(hwnd, "Palette has changed! Do you wish to save first?", "ManpWIN", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		    {
		    if (SaveMAPFileOpenDlg(hwnd, szSaveFileName, szTitleName) == 0)
			{
			save_palette(hwnd, szSaveFileName, "Save Colour Parameters");
			strcpy(MAPFile, szSaveFileName);				// palette file to reflect the current map
			}
		    }
		TrueCol.PalEditFlag = FALSE;
		}

	    if (SaveKfrOpenDlg(hwnd, szSaveFileName, szTitleName) == 0)
		SaveFile(hwnd, szSaveFileName, szTitleName);
	    return 0;

	case IDM_SAVEIMAGE:						// image file
	    if (SavePNGOpenDlg (hwnd, szSaveFileName, szTitleName) == 0)
		SaveFile (hwnd, szSaveFileName, szTitleName);                    
	    return 0;

	case IDM_SAVE_TRUE_MAP:
	    if (SaveColFileOpenDlg (hwnd, szSaveFileName, szTitleName) == 0)
		save_colour(hwnd, szSaveFileName, "Save Colour Parameters");
	    return 0;

	case IDM_SAVE_PAL_MAP:
	    if (SaveMAPFileOpenDlg(hwnd, szSaveFileName, szTitleName) == 0)
		{
		save_palette(hwnd, szSaveFileName, "Save Colour Parameters");
		strcpy(MAPFile, szSaveFileName);				// palette file to reflect the current map
		}

	    return 0;

	case IDM_OPEN_PAR:
	    if (PARFileOpenDlg(hwnd, PARFile, szTitleName) == 0)
		{
		DisplayStatusBarInfo(INITIALISING, "");				// display status bar
		setup_defaults();
		GetParam(hwnd, PARFile, szSaveFileName);
		TrueCol.IsMAPFile = false;
		time_to_reinit = TRUE;
		}
	    else
		return 0;
	    break;

	case IDM_OPEN_KALLES:
	    if (KFRFileOpenDlg(hwnd, KFRFile, szTitleName) == 0)
		{
		DisplayStatusBarInfo(INITIALISING, "");				// display status bar
		setup_defaults();
		ReadKallesFile(hwnd, KFRFile);
		time_to_reinit = TRUE;
		}
	    else
		return 0;
	    break;

	case IDM_OPEN_PNG:
	    if (PNGFileOpenDlg(hwnd, PNGFile, szTitleName) == 0)
		{
		setup_defaults();
		if (read_png_file(hwnd, PNGFile) >= 0)
		    DataFromPNGFile = TRUE;
		}
	    else
		return 0;
	    TrueCol.IsMAPFile = false;
	    time_to_reinit = TRUE;
	    break;

	case IDM_OPEN_SCI: 
	    if (SCIFileOpenDlg(hwnd, ScriptFileName, szTitleName) == 0)
		{
		GetPNGSeqFromScript(hwnd, ScriptFileName);
		if (DialogBox (hInst, "AnimStartDlg", hwnd, (DLGPROC)AnimStartDlg))
		    {
		    if (WriteMPEGFrames)
			{
			if (MPEGWrite(MPGFile) == -1)
			    {
			    time_to_reinit = FALSE;
			    return (WNDPROC)(-1);
			    }
			}
		    else
			{
			if (RunScript(hwnd, ScriptFileName) == -1)
			    {
			    time_to_reinit = FALSE;
			    return (WNDPROC)(-1);
			    }
			}
		    }
		}
	    else
		return 0;
	    time_to_reinit = TRUE;
	    break;

	case IDM_OPEN_LST: 
	    if (LSTFileOpenDlg(hwnd, LSTFile, szTitleName) == 0)
		{
		if (MPEGWrite(MPGFile) < 0)
		    return 0;
		}
	    else
		return 0;
	    time_to_reinit = TRUE;
	    break;

	case IDM_OPEN_TRUE_MAP: 
	    if (ColFileOpenDlg (hwnd, COLFile, szTitleName) == 0)
		{
		FileReadColours(hwnd, COLFile, "Paul's Fractals: Get Colour Parameters");
		InvalidateRect(hwnd, &r, FALSE);
		InitTrueColourPalette(FALSE);
		Plot.RefreshScreen();
		}
	    else
		return 0;
	    break;
	
	case IDM_OPEN_PAL_MAP: 
	    if (MAPFileOpenDlg (hwnd, MAPFile, szTitleName) == 0)
		FilePalette(hwnd, MAPFile, "Paul's Fractals: Get Colour Map");
	    else
		return 0;
	    TrueCol.IsMAPFile = true;
	    time_to_reinit = (type == SLOPEDERIVATIVE || type == SLOPEFORWARDDIFF);
	    break;

	case IDM_EXIT:
	    if (MessageBox (hwnd, "Do you want to Quit?", "ManpWin", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)
		{
		time_to_quit = TRUE;
		SendMessage (hwnd, WM_DESTROY, 0, 0L);
		if (DisplayAnimation)
		    AnimateClose();
		}
	    hWndCopy = hwnd;
	    // the main routine will actually call 'DestroyWindow()'
	    break;

	// Note that there is no 'break' as secondary process is still req
	case IDM_UNDO:
	case IDM_REDO:
	case IDM_NEW:
	case IDM_REOPEN:
	case IDC_FRACTYPE:
	case IDM_UPDATE_FRACTAL:
	case IDM_FRACOPTIONS:
	case IDM_TOGGLEPERT:
	case IDM_FDOPTIONS:
	case IDM_3D:
	case IDM_3DPARAM:
	case IDM_STEREOPAIRSETUP:
	case IDM_JULIA:
	case IDM_REALTIMEJULIA:
	case IDM_SET_JULIA_SIZE:
	case IDM_START:
	case IDM_STEREO:
	case IDC_ADDSPIRAL:
	case IDM_IMAGE_SIZE:
	case IDM_FRACTALCOORDS:
	case IDM_TIERAZON_COLOUR:
	case IDM_TOGGLE_DISPLAY_PALETTE:
	switch (wParam)
	    {
	    case IDM_3DPARAM:
		if (DialogBox (hInst, "Param3D", hwnd, (DLGPROC)Param3D) == FALSE)
		    return 0;
		else
		    break;

	    case IDM_STEREOPAIRSETUP:
		if (DialogBox(hInst, "StereoPairDlg", hwnd, (DLGPROC)StereoPairDlg) == FALSE)
		    return 0;
		else
		    break;

	    case IDM_TOGGLE_DISPLAY_PALETTE:
		TrueCol.DisplayPaletteFlag = !TrueCol.DisplayPaletteFlag;
		Secondaryhwnd = hwnd;
		Secondarymessage = message;
		SecondarywParam = wParam;
		SecondarylParam = lParam;
		DisplayPalette(hwnd, TrueCol.DisplayPaletteFlag);
		DisplayFractal(hwnd);
		return 0;								// don't break as we don't want to reinit

	    case IDM_UPDATE_FRACTAL:
		UpdateFractal(hwnd);
		break;

	    case IDC_FRACTYPE:
		{
		struct	UNDO	RestoreFract;

		Undo(&RestoreFract);
		BigNumFlag = FALSE;
		Dib.ClearDib(0L);
		if (InitNewFractal(hwnd) == FALSE)
		    {
		    Redo(&RestoreFract);
		    return 0;
		    }
		else
		    calcfracinit();
		break;
		}

	    case IDM_TOGGLEPERT:
		{
		WORD    OldType = type;
		int	OldSubtype = subtype;
		if (TogglePerturbation(&type, &subtype) < 0)
		    {
		    type = OldType;
		    subtype = OldSubtype;
		    MessageBox(hwnd, "There is no equivelent method to toggle into", "Perturbation", MB_ICONEXCLAMATION | MB_OK);
		    }
		break;
		}
	    case IDM_FRACOPTIONS:
		{

#ifdef TESTFWDDIFF
		if (DialogBox(hInst, "FractalTestDlg", hwnd, (DLGPROC)FractalTestDlg) == FALSE)
#else
		if (DialogBox(hInst, "FractalDlg", hwnd, (DLGPROC)FractalDlg) == FALSE)
#endif // TESTFWDDIFF

		    return 0;
		else
		    {
		    if (OutsideMethod == TIERAZONFILTERS)
			if (DialogBox (hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectFilterDlg) == FALSE)
			    return 0;
		    if (OutsideMethod == TIERAZONCOLOURS)
			if (DialogBox (hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectColourDlg) == FALSE)
			    return 0;
		    TrueCol.FillPalette(REPEAT, TrueCol.PalettePtr, threshold);
		    }
		break;
		}

	    case IDM_FDOPTIONS:
		if (DialogBox (hInst, "SelectFDOptionDlg", hwnd, (DLGPROC)SelectFDOptionDlg) == FALSE)
		    return 0;
		break;

	    case IDM_FRACTALCOORDS:
		if (DialogBox (hInst, "CoordDlg", hwnd, (DLGPROC)CoordDlg) == FALSE)
		    return 0;
		else
		    break;
	    case IDM_IMAGE_SIZE:
		time_to_reinit = TRUE;
		if (DialogBox (hInst, "ImageSizeDlg", hwnd, (DLGPROC)ImageSizeDlg) == FALSE)
		    return 0;
		else
		    {
		    ClosePtrs();				// ready for next screen
		    mainview(hwnd, FALSE);			// all screen specific stuff
		    }
		break;
	    }
	Secondaryhwnd = hwnd;
	Secondarymessage = message;
	SecondarywParam = wParam;
	SecondarylParam = lParam;
	time_to_reinit = TRUE;
	return 0;
	}
    Secondaryhwnd = hwnd;
    Secondarymessage = message;
    SecondarywParam = wParam;
    SecondarylParam = lParam;
    return 0;
    }

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : MenuCommand ( HWND hwnd, WORD wParam)                      *
 *                                                                          *
 *  PURPOSE    : Processes menu commands.                                   *
 *                                                                          *
 *  RETURNS    : TRUE  - if command could be processed.                     *
 *               FALSE - otherwise                                          *
 *                                                                          *
 ****************************************************************************/

int	SecondaryWndProc (void)

    {
    static	HWND	hwndPlot;
    HWND	hwnd;
    UINT	message;
    UINT	wParam;
    LONG	lParam;
    POINTS	CursorLocShort;

    hwnd = Secondaryhwnd;
    message = Secondarymessage;
    wParam = SecondarywParam;
    lParam = SecondarylParam;

    if (time_to_zoom)
	{
	time_to_zoom = FALSE;
	return 0;			// no commands to process
	}
    switch (wParam)
	{
				  // Messages from File menu
	case IDM_NEW:
	case IDM_REOPEN:
	    MessageBox (hwnd, "This File Menu Option Not Yet Implemented!",
					 "ManpWin", MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    break;
 				  // Messages from Fractals menu
	case IDM_UPDATE_FRACTAL:
	case IDC_FRACTYPE:
	case IDM_FRACOPTIONS:
	case IDM_3DPARAM:
	case IDM_STEREOPAIRSETUP:
	    time_to_restart = TRUE;
	    break;

				// Messages from Options menu
	case IDC_ADDSPIRAL:
	    addflag = !addflag;
	    time_to_restart = TRUE;
	    break;

	case IDM_3D:
	    _3dflag = !_3dflag;
	    if (_3dflag)
		{
		oldcalcmode = calcmode;
		pairflag = FALSE;
		}
	    else   
		calcmode = oldcalcmode;		// restore after 3D
	    time_to_restart = TRUE;
	    break;

	case IDM_IMAGE_SIZE:
	    time_to_restart = TRUE;
	    break;

	case IDM_STEREO:
	    if (pairflag)
		{
		pairflag = FALSE;
		calcmode = oldcalcmode;		// restore after stereo pair
		}
	    else       
		{
		pairflag = 10;
		oldcalcmode = calcmode;        
		_3dflag = FALSE;
		}
	    time_to_restart = TRUE;
	    break;

	case IDM_REALTIMEJULIA:
	    RealTimeJuliaFlag = !RealTimeJuliaFlag;
	    if (RealTimeJuliaFlag)
		{
		if (!juliaflag)		// doesn't make a lot of sense if we are in julia already
		    {
		    InitRTJulia(hwnd);
		    FindCursorRealPos(&CursorLocShort);
		    if (DrawJulia(hwnd, CursorLocShort) < 0)
			RealTimeJuliaFlag = FALSE;
		    }
		return 1;			// ensure that we don't splatter initial RT Julia set
		}
	    else
		time_to_restart = TRUE;
	    break;


	case IDM_REDO:
	    LoadUndo(FALSE);
	    time_to_zoom = TRUE;
	    time_to_restart = TRUE;
	    break;
    	
	case IDM_UNDO:
	    LoadUndo(TRUE);
	    time_to_zoom = TRUE;
	    time_to_restart = TRUE;
	    break;

	case IDM_SET_JULIA_SIZE:
	    DialogBox (hInst, "JuliaDlg", hwnd, (DLGPROC)JuliaDlg);
	    break;

	case IDM_JULIA:
	    juliaflag = !juliaflag;
	    if (juliaflag)
		{
		double  AspectRatio = (double)xdots / (double)ydots;
		if (BigNumFlag)
		    {
		    temp_1 = (double)mpfr_get_d(BigHor.x, MPFR_RNDN);
		    temp_2 = (double)mpfr_get_d(BigWidth.x, MPFR_RNDN);
		    j.x = temp_1 + (temp_2 * (double)xdots / (double)ydots) / 2.0;
		    temp_1 = (double)mpfr_get_d(BigVert.x, MPFR_RNDN);
		    j.y = temp_1 + temp_2 / 2.0;
		    Big_oldhor = BigHor;
		    Big_oldvert = BigVert;
		    Big_oldwidth = BigWidth;
		    if (AspectRatio > 1.0)	// take aspect ration into account when plotting Julia
			{
			BigWidth = 4.0;
			BigVert = -2.0;
			BigHor = BigVert * AspectRatio;
			}
		    else
			{
			BigWidth = 4.0 / AspectRatio;
			BigHor = -2.0;
			BigVert = BigHor / AspectRatio;
			}
		    }
		else
		    {
		    if (RealTimeJuliaFlag)
			j = q;
		    else
			{
			j.x = hor + (mandel_width * (double) xdots / (double) ydots) / 2.0;
			j.y = vert + mandel_width / 2.0;    
			}
		    oldhor = hor;		// store values during julia transformations
		    oldvert = vert;
		    oldwidth = mandel_width;
		    if (AspectRatio > 1.0)	// take aspect ration into account when plotting Julia
			{
			mandel_width = 4.0;
			vert = -2.0;
			hor = vert * AspectRatio;
			}
		    else
			{
			mandel_width = 4.0 / AspectRatio;
			hor = -2.0;
			vert = hor / AspectRatio;
			}
		    }
		}
	    else
		{
		if (oldwidth == 0.0)	// we have not done any Mandelbrot yet
		    {
		    oldwidth = 4.0;
		    oldvert = j.y - oldwidth / 2.0;
		    oldhor = j.x - (oldwidth * (double) xdots / (double) ydots) / 2.0;
		    }
		if (BigNumFlag)
		    {
		    BigHor = Big_oldhor;
		    BigVert = Big_oldvert;
		    BigWidth = Big_oldwidth;
		    }
		else
		    {
		    hor = oldhor;
		    vert = oldvert;
		    mandel_width = oldwidth;
		    }
		if (RealTimeJuliaFlag)
		    {
		    InitRTJulia(hwnd);
		    FindCursorRealPos(&CursorLocShort);
		    if (DrawJulia(hwnd, CursorLocShort) < 0)
			RealTimeJuliaFlag = FALSE;
		    }
		}
	    time_to_restart = TRUE;
	    break;

 			      // Messages from Start menu
	case IDM_START:
	    time_to_restart = TRUE;
	    break;
	}     

    if (time_to_restart)
	AutoStereoActive = FALSE;
    return 0;
    }

/*-----------------------------------------
	Handle View Options
  -----------------------------------------*/

void	ChangeView(HWND hwnd, int xdest, int ydest, int widthdest, int heightdest, int xsrc, int ysrc, int widthsrc, int heightsrc, char stretchflag)
    { 
    HPALETTE hpalT;

    BeginPaint(hwnd, &ps);
    hpalT = SelectPalette (ps.hdc, hpal, FALSE);
    RealizePalette(ps.hdc);
    SetMapMode(ps.hdc,MM_TEXT);
    SetBkMode(ps.hdc,TRANSPARENT);
    SetStretchBltMode (ps.hdc, STRETCH_DELETESCANS) ;	// correct stretch mode for zoom etc
    StretchDIBits (ps.hdc, xdest, ydest, widthdest, heightdest, xsrc, ysrc, widthsrc, heightsrc,
			       (LPSTR)Dib.DibPixels, (LPBITMAPINFO)Dib.pDibInf, DIB_PAL_COLORS, SRCCOPY);
    SelectPalette(ps.hdc,hpalT,FALSE);
//    if (ShowStatusBar)
	OutputStatusBar(hwnd);
    EndPaint(hwnd, &ps);
    }

/*-----------------------------------------
	Setup View Options
  -----------------------------------------*/

void	SetupView(HWND hwnd)
    {
    static	BOOL	first = TRUE;				// start window maximised
    int		xFrame, yFrame;

    xFrame = GetSystemMetrics(SM_CXFRAME) * 2;			// 2017 version of development environment reports only half the size of 2008 version. Not sure why
    yFrame = GetSystemMetrics(SM_CYFRAME) * 2;
    InvalidateRect(hwnd, &r, FALSE);
    caption = GetSystemMetrics(SM_CYCAPTION) + xFrame * 2 + GetSystemMetrics(SM_CYMENU) * 2;
    scroll_width = yFrame * 2;
    display_width = ptSize.x + scroll_width;
    display_height = ptSize.y + caption;					// include caption
    display_width = (display_width < screenx) ? display_width : screenx;	// large pictures
    display_height = (display_height < screeny) ? display_height : screeny;
    MoveWindow (hwnd, (screenx - display_width) / 2, (screeny - display_height) / 2,
							    display_width, display_height, TRUE);

    HorOffset = (screenx - display_width) / 2 + yFrame;
    VertOffset = (screeny - display_height) / 2 + GetSystemMetrics(SM_CYCAPTION) + yFrame + GetSystemMetrics(SM_CYMENU);
    if (first)
	{
	ShowWindow (hwnd, SW_MAXIMIZE);
	first = FALSE;
	}
    }

/*-----------------------------------------------------------
	Setup Fractal defaults when new fractal selected
  -----------------------------------------------------------*/

void	FindCursorRealPos(POINTS *ptReal)

    {
    static	POINT	CursorLocLong;

    GetCursorPos(&CursorLocLong);
    ptReal->x = (short)(CursorLocLong.x - HorOffset);
    ptReal->y = (short)(CursorLocLong.y - VertOffset);
    }

/*-----------------------------------------------------------
	Setup Fractal defaults when new fractal selected
  -----------------------------------------------------------*/

void	InitFract(int type)

    {
    int	i;

    Fractal.FractName = fractalspecific[type].name;
    Fractal.NumFunct = fractalspecific[type].numfn;
    Fractal.NumParam = fractalspecific[type].numparams;
    Fractal.Fn1 = fractalspecific[type].fn1;
    Fractal.Fn2 = fractalspecific[type].fn2;
    *Fractal.rqlim = fractalspecific[type].rqlim;
    for (i = 0; i < fractalspecific[type].numparams; i++)
	{
	*Fractal.ParamValue[i] = fractalspecific[type].paramvalue[i];
	Fractal.ParamName[i] = fractalspecific[type].paramname[i];
	}
    if (type == LYAPUNOV)
	strcpy(LyapSequence, Fractal.Fn1);			// load up default sequence
    }

/*-----------------------------------------------------------
	Setup Fractal defaults when new fractal selected
  -----------------------------------------------------------*/

    BOOL	InitNewFractal(HWND hwnd)

    {
    static  int	OldType;

    Fractal.InitData();				// setup for next fractal
    InitTrueColourPalette(FALSE);
    setup_defaults();
    IsPAR = FALSE;
    IsKFR = FALSE;
    OscProcess.DisplayAxisImages = FALSE;
    DisplayAxisLabels = FALSE;
    OscProcess.AxisDisplay = OFF;
    TrueCol.IsMAPFile = false;

    if (WasFractPar)				// was our last time through this dialogue box a FRACTINT PAR?
	type = FRACTPAR;				// allow the dialogue box to remember where we were last time
    if (DialogBox (hInst, "FractTypeDlg", hwnd, (DLGPROC)FractTypeDlg) == TRUE)
	{
	cycleflag = FALSE;
	WasFractPar = FALSE;
	juliaflag = (fractalspecific[type].juliaflag == JULIAFP);
	if (type != OldType)
	    {
	    xAxis = 0;
	    yAxis = 1;
	    zAxis = 2;
	    }
	OldType = type;
	if (type == FPPICKOVER || type == FPLORENZ3D || type == FPLORENZ3D1 || type == FPLORENZ3D3 
	    || type == FPLORENZ3D4 || type == FPROSSLER || type == IFS)
	    {
	    x_rot = 60.0;
	    y_rot = 30.0;
	    z_rot = 0.0;
	    }
	else if (type == KAM3DFP) 
	    {
	    x_rot = 30.0;
	    y_rot = 15.0;
	    z_rot = 0.0;
	    }
	else if (type == SURFACES) 
	    {
	    x_rot = -20.0;
	    y_rot = 15.0;
	    z_rot = 0.0;
	    }
	else 
	    {
	    x_rot = 10.0;
	    y_rot = 5.0;
	    z_rot = 0.0;
	    }

	if (type == KAM3DFP || type == KAMFP || type == IFS)
	    threshold = 1000L;
	switch (type)					// further subtypes?
	    {
	    case LSYSTEM:					// LSystem Fractal
		if (LsysFileOpenDlg (hwnd, LSYSFile, szTitleName) < 0)
		    return FALSE;
		if (load_lsystems(hwnd, LSYSFile) >= 0)
		    {
		    type = LSYSTEM;
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    }
		else
		    return FALSE;
		if (DialogBox (hInst, "SelectFractal", hwnd, (DLGPROC)SelectFractal))
		    DialogBox (hInst, "LSystemDlg", hwnd, (DLGPROC)LSystemDlg);
		else
		    return FALSE;
		break;                 
	    case FORMULA:					// Formula Fractal
	    case FFORMULA:
		type = FORMULA;
		if (FormulaFileOpenDlg (hwnd, FRMFile, szTitleName) < 0)
		    return FALSE;
		if (get_formula_names(hwnd, FRMFile) < 0)
		    return FALSE;
		if (DialogBox (hInst, "SelectFractal", hwnd, (DLGPROC)SelectFractal))
		    {
		    InitFract(type);
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    if (DialogBox (hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
			return FALSE;
		    if (!fpFormulaSetup(FRMFile))
			return FALSE;
		    }
		else
		    return FALSE;
		break;                 
	    case SCREENFORMULA:					// On screen Formula Fractal
		if (DialogBox (hInst, "ScrnFormDlg", hwnd, (DLGPROC)ScrnFormDlg))
		    {
		    InitFract(type);
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    if (DialogBox (hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
			return FALSE;
		    if (ProcessFormulaString(FormulaString) == -1)
			return FALSE;
		    }
		else
		    return FALSE;
		break;                 
	    case TIERAZON:					// Generic Tierazon Fractal
		type = TIERAZON;
		setup_Tierazon();				// count number of Tierazon fractals in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectTierazonDlg) == FALSE)
		    return FALSE;
		else
		    {
		    InitFract(type);
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    LoadTierazonParams();			// get parameters  and other subtype specific stuff from Tierazon database 
		    if (DialogBox (hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;                 
	    case MANDELDERIVATIVES:				// Generic Mandelbrot Derivatives Fractal
		type = MANDELDERIVATIVES;
		setup_MandelDeriv();				// count number of Madelbrot derivatives in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectMandelDerivDlg) == FALSE)
		    return FALSE;
		else
		    {
		    InitFract(type);
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    LoadMandelDerivParams();			// get parameters  and other subtype specific stuff from Mandelbrot Derivatives database 
		    if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;
	    case SLOPEDERIVATIVE:				// Generic Mandelbrot Derivatives Fractal
		type = SLOPEDERIVATIVE;
		setup_SlopeDeriv();				// count number of Madelbrot derivatives in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectSlopeDerivDlg) == FALSE)
		    return FALSE;
		else
		    {
		    InitFract(type);
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    LoadSlopeDerivParams();			// get parameters  and other subtype specific stuff from Slope Derivative database 
		    if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;
	    case SLOPEFORWARDDIFF:				// Generic Mandelbrot Forward Differencing Fractal
		type = SLOPEFORWARDDIFF;
		setup_SlopeFwdDiff();				// count number of Madelbrot derivatives in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectSlopeFwdDiffDlg) == FALSE)
		    return FALSE;
		else
		    {
		    InitFract(type);
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    LoadSlopeFwdDiffParams();			// get parameters  and other subtype specific stuff from Forward Differencing database 
		    if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;
	    case PERTURBATION:					// fractals calculated using perturbation
		type = PERTURBATION;
		setup_Perturbation();				// count number of perturbation fractals in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectPertDlg) == FALSE)
		    return FALSE;
		else
		    {
		    InitFract(type);
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    LoadPerturbationParams();			// get parameters and other subtype specific stuff from Perturbation database 
		    if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;
	    case OSCILLATORS:					// Generic Oscillator Fractal
		type = OSCILLATORS;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_Oscillator();				// count number of Oscillator fractals in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Oscillator database 
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    if (DialogBox (hInst, OscillatorSpecific[subtype].DialogueName, hwnd, OscillatorSpecific[subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;                 
	    case FRACTALMAPS:					// Generic Fractal Map
		type = FRACTALMAPS;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_FractalMaps();				// count number of fractal maps in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Fractal Map database 
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    if (DialogBox(hInst, FractalMapSpecific[subtype].DialogueName, hwnd, FractalMapSpecific[subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;
	    case SPROTTMAPS:					// Generic Fractal Map
		type = SPROTTMAPS;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_SprottMaps();				// count number of fractal maps in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Fractal Map database 
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    if (DialogBox(hInst, SprottMapSpecific[subtype].DialogueName, hwnd, SprottMapSpecific[subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;
	    case SURFACES:					// Generic Fractal Map
		type = SURFACES;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_Surface();				// count number of surfaces in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Surface database 
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    if (DialogBox (hInst, SurfaceSpecific[subtype].DialogueName, hwnd, SurfaceSpecific[subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;                 
	    case KNOTS:					// Generic Fractal Map
		type = KNOTS;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_Knot();				// count number of surfaces in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Knot database 
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    if (DialogBox (hInst, SurfaceSpecific[subtype].DialogueName, hwnd, KnotSpecific[subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;                 
	    case CURVES:					// Generic Fractal Map
		type = CURVES;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_Curve();				// count number of surfaces in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, (DLGPROC)SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Curves database 
		    time_to_reinit = FALSE;
		    time_to_restart = TRUE;
		    time_to_load = FALSE;
		    if (DialogBox (hInst, SurfaceSpecific[subtype].DialogueName, hwnd, CurveSpecific[subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;                 
	    case FRACTPAR:					// Fractint Par File
		if (FractintParFileOpenDlg (hwnd, FracPARFile, szTitleName) < 0)
		    return FALSE;
		setup_defaults();
		if (load_par(hwnd, FracPARFile) < 0)
		    return FALSE;
		WasFractPar = TRUE;
		if (!DialogBox (hInst, "SelectFractal", hwnd, (DLGPROC)SelectFractal))
		    return FALSE;
		if (FractintPar(hwnd, FracPARFile) < 0)	// Fractint Par fractals 
		    return FALSE;
		time_to_reinit = FALSE;
		time_to_restart = TRUE;
		time_to_load = FALSE;
		break;                 
	    case IFS:					// Iterated File Systems Fractal
		if (IFSFileOpenDlg (hwnd, IFSFile, szTitleName) < 0)
		    return FALSE;
		if (get_IFS_names(hwnd, IFSFile) < 0)	    // get the IFS fractal names
		    return FALSE;
		if (DialogBox (hInst, "SelectFractal", hwnd, (DLGPROC)SelectFractal))
		    {
		    if (ifsload(hwnd, IFSFile) >= 0)
			{
			time_to_reinit = FALSE;
			time_to_restart = TRUE;
			time_to_load = FALSE;
//			hor = -6.0;
			hor = (ifs_type == 0) ? -12.0 : -6.0;
//			vert = (ifs_type == 0) ? -8.0 : -4.0;
			vert = -4.0;
			mandel_width = (ifs_type == 0) ? 16.0 : 8.0;
			}
		    }
		else
		    return FALSE;
		break;      
	    case BUDDHABROT:
		InitMultipliers();
		InitFract(type);
		if (DialogBox(hInst, fractalspecific[BUDDHABROT].DialogueName, hwnd, fractalspecific[BUDDHABROT].DialogueType) == FALSE)
		    return FALSE;
		InitTrueColourPalette(FALSE);
		hor = fractalspecific[BUDDHABROT].hor;
		vert = fractalspecific[BUDDHABROT].vert;
		mandel_width = fractalspecific[BUDDHABROT].width;
		break;
	    case NEWTON:
	    case NEWTBASIN:
		juliaflag = (fractalspecific[type].juliaflag);		// default view is of julia Newton
		// fall through (no break;)
	    default:
		InitFract(type);
		if (DialogBox (hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
		    return FALSE;
		InitTrueColourPalette(FALSE);
		hor = fractalspecific[type].hor;
		vert = fractalspecific[type].vert;
		mandel_width = fractalspecific[type].width;
		break;                           
	    }
	return TRUE;
	}           
    return FALSE;    
    }
     
/*-----------------------------------------------------------
    Update Fractal info when existing fractal selected
  -----------------------------------------------------------*/

    BOOL	UpdateFractal(HWND hwnd)

	{
	switch (type)					// further subtypes?
	    {
	    case LSYSTEM:					// LSystem Fractal
		DialogBox(hInst, "LSystemDlg", hwnd, (DLGPROC)LSystemDlg);
		break;
	    case FORMULA:					// Formula Fractal
		time_to_reinit = FALSE;
		time_to_restart = TRUE;
		time_to_load = FALSE;
		if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
		    return FALSE;
		if (!fpFormulaSetup(FRMFile))
		    return FALSE;
		break;
	    case SCREENFORMULA:					// On screen Formula Fractal
		if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
		    return FALSE;
		if (ProcessFormulaString(FormulaString) == -1)
		    return FALSE;
		break;
	    case TIERAZON:					// Generic Tierazon Fractal
		if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
		    return FALSE;
		break;
	    case OSCILLATORS:					// Generic Oscillator Fractal
		MaxDimensions = OscillatorSpecific[subtype].MaxDimensions;
		if (DialogBox(hInst, OscillatorSpecific[subtype].DialogueName, hwnd, OscillatorSpecific[subtype].DialogueType) == FALSE)
		    return FALSE;
		break;
	    case FRACTALMAPS:					// Generic Fractal Map
		MaxDimensions = FractalMapSpecific[subtype].MaxDimensions;
		if (DialogBox(hInst, FractalMapSpecific[subtype].DialogueName, hwnd, FractalMapSpecific[subtype].DialogueType) == FALSE)
		    return FALSE;
		break;
	    case SPROTTMAPS:					// Generic Sprott Fractal Map
		MaxDimensions = SprottMapSpecific[subtype].MaxDimensions;
		if (DialogBox(hInst, SprottMapSpecific[subtype].DialogueName, hwnd, SprottMapSpecific[subtype].DialogueType) == FALSE)
		    return FALSE;
		break;
	    case SURFACES:					// Generic Fractal Map
		MaxDimensions = SurfaceSpecific[subtype].MaxDimensions;
		if (DialogBox(hInst, SurfaceSpecific[subtype].DialogueName, hwnd, SurfaceSpecific[subtype].DialogueType) == FALSE)
		    return FALSE;
		break;
	    case KNOTS:					// Generic Fractal Map
		MaxDimensions = KnotSpecific[subtype].MaxDimensions;
		if (DialogBox(hInst, SurfaceSpecific[subtype].DialogueName, hwnd, KnotSpecific[subtype].DialogueType) == FALSE)
		    return FALSE;
		break;
	    case CURVES:					// Generic Fractal Map
		MaxDimensions = CurveSpecific[subtype].MaxDimensions;
		if (DialogBox(hInst, SurfaceSpecific[subtype].DialogueName, hwnd, CurveSpecific[subtype].DialogueType) == FALSE)
		    return FALSE;
		break;
	    case PERTURBATION:					// Generic Perturbation Fractal
		Fractal.NumParam = PerturbationSpecific[subtype].numparams;	// we need to know how many params to load
		if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
		    return FALSE;
		break;
	    default:
		if (DialogBox(hInst, fractalspecific[type].DialogueName, hwnd, fractalspecific[type].DialogueType) == FALSE)
		    return FALSE;
		break;
	    }
	return FALSE;
	}

    /*-----------------------------------------
	Parse Command Line
  -----------------------------------------*/

int	CopyArgument(HWND hwnd, char *FileName, char *CommandStr)

    {
    char	*p, *q;
    char	*token;
    char	seps[]   = "-/";
    char	ParameterString[MAXDATALINE];

    BatchFlag = FALSE;
    AutoExitFlag = FALSE;
    AutoSaveFlag = FALSE;

    if (*CommandStr == '\0')		// nothing to parse
	return 0;
    strcpy(ParameterString, CommandStr);	// preserve a copy so strtok() doesn't splatter it.
    if (*CommandStr == '-' || *CommandStr == '/')	// must be a command line with no par file
	{
	*szSaveFileName = '\0';		// initialise
	CommandLineFile = FALSE;
	AutoStartFlag = TRUE;
	BatchFlag = TRUE;
	GetParamData(hwnd, "null", ParameterString, szSaveFileName, FALSE);  
	InitTrueColourPalette(FALSE);
	}
    else					// must be a par file
	{
	p = CommandStr;
	q = FileName;
	while (*p)
	    {
	    *q = *p;
	    if (*p == '/' || *p == '-')
		{
		*q = '\0';
		break;
		}
	    if (*p == '\0')
		break;
	    p++;
	    q++;
	    }
	trailing(FileName);
	CommandLineFile = TRUE;
	AutoStartFlag = TRUE;
	token = strtok(CommandStr, seps);
	while (token != NULL)
	    {
	    if (toupper(*token) == 'S')		// outputfilename 
		{
		AutoSaveFlag = TRUE;
		sprintf(szSaveFileName, "%s", token + 1);
		}
	    else if (toupper(*token) == 'E')	// Exit after completion
		AutoExitFlag = TRUE;
	    token = strtok(NULL, seps);
	    }

	BatchFlag = TRUE;
	GetParam(hwnd, FileName, szSaveFileName);
	InitTrueColourPalette(FALSE);
	}

    return 0;
    }

 /*-----------------------------------------
	Copy correct filename and strip 
	quotes if required
  -----------------------------------------*/

/**************************************************************************
	Dialog Control for Image Size
**************************************************************************/

DLGPROC FAR PASCAL RTJuliaLocDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static	char	PixelLoc[250]; 
     static     char	JuliaLoc[250];
     static	POINT	CursorLocLong;

     switch (message)
	  {
	  case WM_INITDIALOG:
		cycleflag = FALSE;
		GetCursorPos(&CursorLocLong);
		sprintf(PixelLoc, "%ld, %ld", CursorLocLong.x, CursorLocLong.y);
		sprintf(JuliaLoc, "%14.14f, %14.14f", q.x, q.y);
		SetDlgItemText(hDlg, IDC_PIXEL, PixelLoc);
		SetDlgItemText(hDlg, IDC_JULIALOC, JuliaLoc);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDOK:
		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
	  break;
	  }
      return (DLGPROC)FALSE ;
      }

 void	DisplayLocation(HWND hwnd, POINTS ptCurrent)
    {
    double  x, y;
    char    s[MAXLINE];

    x = ((double)ptCurrent.x/(double)xdots) * mandel_width * ScreenRatio + hor;
    y = ((double)(ydots - ptCurrent.y)/(double)ydots) * mandel_width + vert;
    sprintf(s, "x=<%lf>,y=<%lf>", x, y);
    SetWindowText (hwnd, s);
    }

/**************************************************************************
    Dialog Box for Displaying Status
**************************************************************************/

DLGPROC FAR PASCAL StatusInfoDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)
    {
    char	PassStr[100];
    char	FinishedStr[100];
    char	PrecisionStr[100];
    char	ParamStr[250];
    char	TempStr[200];
    char	FilterString[80];
    char	FractalType[1200];
    char	SubData[500];
    char	*PositionStr = NULL;
    char	*StatusString = NULL;
    char	*s1 = NULL;
    char	*s2 = NULL;
    char	*s3 = NULL;
    double	centrex, centrey;
    BigDouble	Big_centrex, Big_centrey;

    PositionStr = new char[SIZEOF_BF_VARS * 3];
    if (PositionStr == NULL)
	return(DLGPROC)TRUE;
    StatusString = new char[SIZEOF_BF_VARS * 4];			// times 3 because of x, y, width & additional part is for other info
    if (StatusString == NULL)
	return(DLGPROC)TRUE;

    if (BigNumFlag)
	{
	Big_centrex = BigHor + (BigWidth * (ScreenRatio / 2.0));
	Big_centrey = BigVert + (BigWidth / 2.0);
	s1 = new char[SIZEOF_BF_VARS];
	s2 = new char[SIZEOF_BF_VARS];
	s3 = new char[SIZEOF_BF_VARS];
	ConvertBignum2String(s1, Big_centrex.x);
	ConvertBignum2String(s2, Big_centrey.x);
	mpfr_sprintf(s3, "%.20Re", BigWidth.x);
//	ConvertBignum2String(s3, BigWidth.x);
	sprintf(PositionStr, "X = %s\r\nY = %s\r\nWidth = %s", s1, s2, s3);
	if (s1) delete[] s1;
	if (s2) delete[] s2;
	if (s3) delete[] s3;
	}
    else
	{
	centrex = hor + (mandel_width * (ScreenRatio / 2.0));
	centrey = vert + (mandel_width / 2.0);
	if (mandel_width > 0.000001)
	    sprintf(PositionStr, "X = %14.14f\r\nY = %14.14f\r\nWidth = %14.14f", centrex, centrey, mandel_width);
	else
	    sprintf(PositionStr, "X = %14.14f\r\nY = %14.14f\r\nWidth = %14e", centrex, centrey, mandel_width);
	}
    switch (calcmode)
	{
	case 'B':
	case 'T':
	case '1':
	    sprintf(PassStr, "Pass 1 of 1");
	    break;
	case '2':
	case 'G':
	case 'V':
	case 'H':
	    sprintf(PassStr, "Pass %d of %d", curpass, totpasses);
	    break;
	    //	default:
	    //	    sprintf(PassStr, "Pass %d of %d", curpass, totpasses); 
	    //	    break;
	}
    sprintf(FinishedStr, "%s", (finished) ? ", Image Complete" : " ");
    if (BigNumFlag)	    // now we have double double and quad double...
	{
	if (precision <= 30 && fractalspecific[type].flags & USEDOUBLEDOUBLE)
	    sprintf(PrecisionStr, "DD Prec: %d", precision);
	else if (precision <= 60 && fractalspecific[type].flags & USEDOUBLEDOUBLE)
	    sprintf(PrecisionStr, "QD Prec: %d", precision);
	else
	    sprintf(PrecisionStr, "Arb Prec: %d", precision);
	}
    else
	sprintf(PrecisionStr, "Floating Point: %d", precision);

    if (type == PERTURBATION)
	wsprintf(FractalType, "Fractal: (Pert)-%s, Subtype = %d", GetFractalName(), subtype);
    else if (type == SLOPEDERIVATIVE)
	wsprintf(FractalType, "Fractal: (Slope Der)-%s, Subtype = %d", GetFractalName(), subtype);
    else if (type == SLOPEFORWARDDIFF)
	wsprintf(FractalType, "Fractal: (Slope Fwd)-%s, Subtype = %d", GetFractalName(), subtype);
    else if (type == SCREENFORMULA || type == FORMULA || type == FFORMULA)
	sprintf(FractalType, "Formula = <%s>\r\n", FormulaString);
    else if (type == LSYSTEM || type == FRACTPAR || type == IFS)
	sprintf(FractalType, "Fractal: %s, Sub=%s", GetFractalName(), lsys_type);
    else if (type == TIERAZON || type == CROSSROADS || type == ZIGZAG || type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
	sprintf(FractalType, "Fractal: %s, Sub=%d", GetFractalName(), subtype);
    else if (type == CUBIC)
	{
	char	ch;
	if (subtype == 0)   ch = 'B';
	if (subtype == 1)   ch = 'C';
	if (subtype == 2)   ch = 'F';
	if (subtype == 3)   ch = 'K';
	sprintf(FractalType, "Fractal: %s, Sub=C%cIN", GetFractalName(), ch);
	}
    else if (type == MALTHUS || type == TRIANGLES || type == GEOMETRY || type == CIRCLES || type == PASCALTRIANGLE)
	sprintf(FractalType, "Fractal: %s, Sub=%c", GetFractalName(), subtype);
    else
	wsprintf(FractalType, "Fractal: %s, Subtype = %d", GetFractalName(), subtype);

    sprintf(ParamStr, "Params: ");
    for (int i = 0; i < NUMPARAM; i++)
	{
	if ((fabs(param[i]) < 0.00001 || fabs(param[i]) > 100000.0) && param[i] != 0.0)
	    sprintf(TempStr, "\r\n%d: %-12.9e", i, param[i]);
	else
	    sprintf(TempStr, "\r\n%d: %-12.9f", i, param[i]);
	strcat(ParamStr, TempStr);
	}
    sprintf(StatusString, "%s\r\nPixel [%d][%d], %s%s\r\n%s\r\nThreshold: %ld\r\nArith=%s\r\n%s\r\nBailout: %f\r\nImage Size: [%d][%d]",
	FractalType, col, row, PassStr, FinishedStr, PositionStr, threshold, PrecisionStr, ParamStr, rqlim, xdots, ydots);
    if (Fractal.NumFunct == 1)
	sprintf(SubData, "\r\nFn = %s", Fractal.Fn1);
    else if (Fractal.NumFunct == 2 && type != OSCILLATORS)			// we use NumFunct to display dimensions
	sprintf(SubData, "\r\nFn1 = %s,Fn2 = %s", Fractal.Fn1, Fractal.Fn2);
    strcat(StatusString, SubData);
    if (RotationAngle != 0)
	{
	sprintf(TempStr, "\r\nRotation Angle: %d\r\nRotation Centre: %lf, %lf\r\nMagnification: %le", RotationAngle, RotationCentre.x, RotationCentre.y, (BigNumFlag) ? 1.0 / (double)precision : 2.0 / mandel_width);
	strcat(StatusString, TempStr);
	}

    if (juliaflag)
	{
	sprintf(TempStr, "\r\nJulia: %lf, %lf", j.x, j.y);
	strcat(StatusString, TempStr);
	}

    if (type == PERTURBATION || type == SLOPEDERIVATIVE || type == SLOPEFORWARDDIFF)
	{
	strcat(StatusString, "\r\nPerturbation and Slope Parameters-");
	if (type == SLOPEDERIVATIVE) SlopeType = DERIVSLOPE;
	if (type == SLOPEFORWARDDIFF) SlopeType = FWDDIFFSLOPE;
	sprintf(TempStr, "\r\nSlope Type: %d", SlopeType);
	strcat(StatusString, TempStr);
	sprintf(TempStr, "\r\nSlope Shadow Angle: %lf", lightDirectionDegrees);
	strcat(StatusString, TempStr);
	sprintf(TempStr, "\r\nSlope Shadow Strength: %lf", bumpMappingStrength);
	strcat(StatusString, TempStr);
	sprintf(TempStr, "\r\nSlope Shadow Depth: %lf", bumpMappingDepth);
	strcat(StatusString, TempStr);
	sprintf(TempStr, "\r\nPalette Start: %d", PaletteStart);
	strcat(StatusString, TempStr);
	sprintf(TempStr, "\r\nSlope Light Height: %lf", LightHeight);
	strcat(StatusString, TempStr);
	sprintf(TempStr, "\r\nColour Offset: %d", PalOffset);
	strcat(StatusString, TempStr);
	sprintf(TempStr, "\r\nIteration Divisor: %lf", IterDiv);
	strcat(StatusString, TempStr);
	sprintf(TempStr, "\r\nPert Colour Method: %d", PertColourMethod);
	strcat(StatusString, TempStr);
	}
    if (IsPAR)
	{
	strcat(StatusString, "\r\nPAR file=");
	strcat(StatusString, PARFile);
	}
    else if (IsKFR)
	{
	strcat(StatusString, "\r\nKFR file=");
	strcat(StatusString, KFRFile);
	}
    else if (WasFractPar)
	{
	strcat(StatusString, "\r\nFractPAR file=");
	strcat(StatusString, FracPARFile);
	strcat(StatusString, "\r\nItemName=");
	strcat(StatusString, lptr[lsys_ptr]);
	}
    else if (type == IFS)
	{
	strcat(StatusString, "\r\nIFS file=");
	strcat(StatusString, IFSFile);
	strcat(StatusString, "\r\nItemName=");
	strcat(StatusString, lptr[lsys_ptr]);
	}
    else if (type == LSYSTEM)
	{
	strcat(StatusString, "\r\nL System file=");
	strcat(StatusString, LSYSFile);
	strcat(StatusString, "\r\nItemName=");
	strcat(StatusString, lptr[lsys_ptr]);
	}
    else if (type == FORMULA)
	{
	strcat(StatusString, "\r\nFormula file=");
	strcat(StatusString, FRMFile);
	strcat(StatusString, "\r\nItemName=");
	strcat(StatusString, lptr[lsys_ptr]);
	}
    //    DialogBox(hInst, "StatusInfoDlg", hwnd, (DLGPROC)StatusInfoDlg);
//    MessageBox (hwnd, StatusString, fractalspecific[type].name, MB_ICONEXCLAMATION | MB_OK);
    if (TrueCol.IsMAPFile)
	{
	strcat(StatusString, "\r\nMAP file=");
	strcat(StatusString, MAPFile);
	}
    if (InsideMethod > NONE || OutsideMethod > NONE)
	{
	if (OutsideMethod > TIERAZONCOLOURS)
	    sprintf(FilterString, "\r\nTZColour = %d, ", ColourMethod);
	else if (OutsideMethod > TIERAZONFILTERS)
	    sprintf(FilterString, "\r\nTZFilter = %d, Fractal Dimension Options = %d, ", FilterType, nFDOption);
	else if (InsideMethod > NONE)
	    sprintf(FilterString, "Filter=%d, ", InsideMethod);
	else
	    sprintf(FilterString, "Filter=%d, ", OutsideMethod);
	strcat(StatusString, FilterString);
	}

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemText(hDlg, IDC_STATUSINFO, StatusString);
	    return (DLGPROC)TRUE;

	case WM_COMMAND:
	    switch (wParam)
		{
		case IDOK:
		    if (StatusString)
			delete[] StatusString;
		    if (PositionStr)
			delete[] PositionStr;
		    EndDialog(hDlg, TRUE);
		    return (DLGPROC)TRUE;
		case IDCANCEL:
		    if (StatusString)
			delete[] StatusString;
		    if (PositionStr)
			delete[] PositionStr;
		    EndDialog(hDlg, TRUE);
		    return (DLGPROC)TRUE;
		}
	    break;
	}
    return FALSE;
    }

