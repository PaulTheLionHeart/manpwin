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
#include <atomic>
#include <htmlhelp.h>	// PHD 24/10/2007 tp allow *.CHM files
#include "fractype.h"
#include "Fract.h"
#include "Dib.h"
#include "resource.h"
#include "menu.h"
#include "manp.h"
#include "fractalp.h"
#include "big.h"
#include "Undo.h"
#include "complex.h"
#include "BigDouble.h"
#include "colour.h"
#include "OscProcess.h"
#include "Plot.h"
#include "PertEngine.h"
#include "SafeStrings.h"
#include "OtherFunctions.h"

extern	std::atomic<bool> gStopRequested;
extern	void	HardStopNow(HWND hwnd, const char* reason);


LRESULT CALLBACK PASCAL WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PASCAL	MenuCommand(HWND, UINT, WPARAM, LPARAM);
BOOL	InitNewFractal(HWND hwnd);
BOOL	UpdateFractal(HWND hwnd);
void	pfract_main(CManp *, HWND, char *);
//void	init(HWND);
void	DisplayLocation(HWND, POINTS);

void	ChangeView(HWND, int, int, int, int, int, int, int, int, char);
int	CopyArgument(HWND, char *, char *);
void	FindCursorRealPos(POINTS *);

extern	BOOL	wintext_initialize(HANDLE, HWND, LPSTR);
extern	void	output_batch(double, double, double, HWND, LPSTR);
//extern	int	ZoomIn(HWND, RECT *);			// zoom functions in Zoom.c
//extern	int	ZoomOut(HWND, RECT *);
extern	short	FilePalette(HWND, char *, char *);
extern	short	FileReadColours(HWND, char *, char *);
extern	int	read_png_file(HWND, char *);
extern	void	GetParam(HWND, LPSTR, LPSTR);  
extern	void	draw3D(HWND);  
extern	void	initFibonacci(void);
extern	int	load_par(HWND, char *);
extern	void	DisplayPalette(HWND, BOOL);
//extern	void	DisplayFractal(HWND);
extern	int	rotate(int);
extern	int	CopyPictureToClipboard(HWND);
extern	int	mainview(HWND, BOOL);
extern	void	ClosePtrs(void);
extern	int	FractintPar(HWND, char *);
extern	int	ReadConfig(HWND);
//extern	void	OutputStatusBar(HWND);
extern	int	SaveConfig(HWND);
extern	void	rotate_vga_palette(int, int);
extern	int	fpFormulaSetup(char *);
extern	void	ProcessKeys(HWND, WPARAM);
extern	char	*trailing(char *instr);
extern	HWND	DoHtmlHelp(HWND, char *, UINT);
//extern	int	DoAnimation(void);
//extern	int	AnimateClose(void);
extern	int	ProcessFormulaString(char *);
extern	int	MPEGWrite(char *);
extern	int	DisplayRGB(POINTS);
extern	int	EditRGB(HINSTANCE, POINTS);
extern	void	DrawSpots(void);
extern	int	TogglePerturbation(WORD *type, int *subtype);
extern	void	InitMultipliers(void);
extern	int	ReadKallesFile(HWND hwnd, char *filename);

//extern	int	xdots, ydots, width, height;

//extern	BOOL	DisplayAnimation;		// allow system to know that we are currently displaying an animation

static	char    argumentFileName[MAX_PATH+MAXDATALINE];
static	HINSTANCE  hInst;
static	HCURSOR  hCursor;
static	DLGPROC *lpfnAboutDlgProc;
static	char	szFileName[MAX_PATH];
static	char    szTitleName[MAXDATALINE] = "ManpWIN";
static	char	szSaveFileName[MAX_PATH];

//extern	int	SlopeType;
//extern	bool	EnableApproximation;		// use BLA on perturbation
//extern	int	PalOffset;			// begin palette here
//extern	double	IterDiv;			// divide ieration by this amount
//extern	double	ColourSpeed;			// used for colour smoothing
//extern	int	PertColourMethod;		// some ideas from Kalles
//extern	int	RotationAngle;			// in degrees
//extern	Complex	RotationCentre;			// centre of rotation
//extern	char	LyapSequence[];			// hold the AB sequence for Lyapunov fractals

static	int	HorOffset, VertOffset;

static	BOOL	WasFractPar = FALSE;		// did we run a FRACTINT PAR file last time?
//	BOOL	IsPAR = FALSE;			// are we currently running a PAR file?
//	BOOL	IsKFR = FALSE;			// are we currently running a KFR file?

static	BOOL	DisplayLoc = FALSE;		// display the current cursor location in the caption bar
//	BYTE	oldcalcmode;			// store values during 3D transformations

//BOOL	bTrack = FALSE;				// TRUE if user is selecting a region
RECT	Rect;
int	Shape = SL_BLOCK;			// Shape to use for the selection rectangle

HWND		CallingWindowHandle = 0L;	// Is ManpWIN called by an external window via WM_COPYDATA message?
int		ReplyUsingDIB;			// TRUE for DIB/WM_COPYDATA and FALSE for clipboard
//void	SetupView(HWND, BOOL);
//void	InitFract(int);

	  // Functions in VIEWFILE.C
extern	void	ViewFileInitialize(HWND);
extern	INT_PTR CALLBACK 	PARFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	PNGFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	SCIFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	LSTFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	LsysFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	FractintParFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	IFSFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	FormulaFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	ColFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	MAPFileOpenDlg(HWND, LPSTR);
extern	INT_PTR CALLBACK 	KFRFileOpenDlg(HWND, LPSTR);

// Functions in SAVEFILE.C
extern	void	SaveFileInitialize(HWND, HINSTANCE);
extern	INT_PTR CALLBACK 	SavePNGOpenDlg(HWND, LPSTR, LPSTR);
extern	INT_PTR CALLBACK 	SaveColFileOpenDlg(HWND, LPSTR, LPSTR);
extern	INT_PTR CALLBACK 	SaveMAPFileOpenDlg(HWND, LPSTR, LPSTR);
extern	INT_PTR CALLBACK 	SaveParOpenDlg(HWND, LPSTR, LPSTR);
extern	INT_PTR CALLBACK	SaveKfrOpenDlg(HWND, LPSTR, LPSTR);
extern	INT_PTR CALLBACK 	SaveParImageOpenDlg(HWND, LPSTR, LPSTR);
extern	INT_PTR CALLBACK 	SaveMPGOpenDlg(HWND, LPSTR, LPSTR);
extern	INT_PTR CALLBACK	SaveSVGOpenDlg(HWND, LPSTR, LPSTR);
extern	void	SaveFile(HWND, LPSTR, LPSTR);

extern	void	SetScrollRanges(HWND);
//extern	void	setup_defaults(void);
extern	void	InitTrueColourPalette(BYTE);
extern	int	StartSelection(HWND, POINTS, LPRECT, int);
extern	int	UpdateSelection(HWND, POINTS, LPRECT, int);
extern	int	EndSelection(POINTS, LPRECT);
extern	int	ClearSelection(HWND, LPRECT, int);
extern	int	load_lsystems(HWND, char *);
extern	int	Lsystem(HWND, char *);
//extern	int	calcfracinit(void);
extern	int	get_IFS_names(HWND, char *);		// get the IFS fractal names
extern	int	ifsload(HWND, char *);
//extern	int	RunScript(HWND, char *);
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
//extern	int	DoUpdate(void);

extern	INT_PTR CALLBACK ImageSizeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK RTJuliaLocDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK PlotTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK FractTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK FractalDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK FractalTestDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK ColourDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK EditPalDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK ExpTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK PowerTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK LSystemDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK NewtonGeneral(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SpecNewton(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectFractal(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectIFS(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK RatTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK NewtonTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK HenonTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK PlasmaDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK FrothDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK CellularDlg(HWND, UINT, WPARAM, LPARAM);

extern	INT_PTR CALLBACK FibTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK WalkDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK StereoPairDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK Param3D(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK JuliaDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK InsideDlg(HWND, UINT, WPARAM, LPARAM);
extern	int		 GetParamData(HWND, LPSTR, LPSTR, LPSTR, BOOL);
extern	INT_PTR CALLBACK AnimationDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK JuliaAnimDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK InversionAnimDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK AnimStartDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK CoordDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK ParamDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SpecialAnimDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK FourierTypeDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK FourierAnimDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK NewtonCmplxDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectFracParams(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectTierazonDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectSlopeDerivDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectSlopeFwdDiffDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectMarcusDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectMandelDerivDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectPertDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK ThreadingDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectOscillatorDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectColourDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectFilterDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK SelectFDOptionDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK ScrnFormDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK ParamAnimDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK OscillatorAnimDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK OscMorphAnimDlg(HWND, UINT, WPARAM, LPARAM);
extern	INT_PTR CALLBACK StatusInfoDlg(HWND, UINT, WPARAM, LPARAM);

extern	short	save_colour(HWND, char *, char *);
extern	short	save_palette(HWND, char *, char *);

extern	BOOL	AutoStereoActive;		// is autostereo currently running?
extern  int	max_vscroll, max_hscroll;
extern  int	display_width, display_height;
//extern	int	/*row, col, */curpass, totpasses;
extern	double	HenonA, HenonXStart, HenonYStart;
extern	int	HenonPoints;
//extern	double	param[];
//extern	char	FormulaString[];		// used to hold the full formula
extern	char	statname[];			// MPEG stat file. If statname[] == '-', stats sent to stdout.
extern	BOOL	WriteMPEGFrames;		// write frames directly to an MPEG file

extern	char	lptr[][100];
extern	int	lsys_ptr;
extern	char	lsys_type[];

extern	int	ifs_type;
extern	BOOL	DisplayAxisLabels;		// show labels for axis pairs

	BOOL	GetPixelColourFlag;		// get the colour of the pixel when mouse button pressed
//extern	BOOL	StartImmediately;		// immediate start of animation generation
//extern	BOOL	RunAnimation;			// are we in the middle of an animation run?
extern	char	ScriptFileName[];		// base name for PNG file animation sequence

//extern	HWND	GlobalHwnd;			// to allow passing of hwnd to find_file_item()

//extern	PAINTSTRUCT 	ps;
//extern	HDC		hdcMem;			// load picture into memory
//extern	RECT 		r;
extern	HPALETTE 	hpal;
extern	HANDLE  	hLogPal;       		// Temporary Handle

//extern	int	xAxis, yAxis, zAxis;		// numerical values for axes for chaotic oscillators

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

//extern	int	time_to_zoom;			// time to zoom in or out?
//extern	int	time_to_restart;		// time to restart?
//extern	int	time_to_reinit;			// time to reinitialize?
//extern	int	time_to_quit;			// time to quit?
//extern	int	time_to_load;			// time to load file?
//extern	int	time_to_break;			// time to break out of animation?
//extern	int	DataFromPNGFile;		// loaded PNG file?
//extern	BOOL	bTrack;				// TRUE if user is selecting a region
//extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	char	MAPFile[];			// colour map file
//extern	double	rqlim, rqlim2;

//extern	BOOL	RTJuliaActive;			// block any moire requests until calcs complete

//extern	int	AnimateSuspend(void);
//extern	int	AnimateResume(void);
//extern	BOOL	SuspendAnimation;		// pause animation run?

extern	BOOL	DisplayAxes;			// display axes for oscillator etc
extern	BOOL	PlotCentre;			// display circle at the centre of the oscillator

short	vscroll_count = 0;			// vertical scroll count
short	hscroll_count = 0;			// horizontal scroll count 
//BOOL	Reopen_flag = FALSE;			// re-open file
//BOOL	StartPicture = TRUE;			// starting picture
HANDLE	hAccTable;				// handle to accelerator table
char	AccTableAddr[120];			// name of accelerator table       
//HWND	PixelHwnd;				// pointer to handle for pixel updating
HWND	hWndCopy;				// Copy of hwnd
int     file_type = FILE_PNG;			// got to start somewhere

// Styles of app. window
DWORD	dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | 
                  WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;

//POINT	ptSize;					// Stores DIB dimensions
//static	Complex q = 0.0;

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

    gManp = new CManp();
    gManp->setup_defaults();   // THIS is now the source of truth

    hwnd = CreateWindow ("ManpWin", "Paul's Fractals",
     			  dwStyle, 
			  160, 120, 				// nice initial location
			  500, 300, 				// nice initial size
			  NULL, NULL, hInstance, NULL);

    _snprintf_s(argumentFileName, MAX_PATH + MAXDATALINE, _TRUNCATE, lpszCmdLine, hwnd);	// in case we want to analyse arguments

    UpdateWindow (hwnd);
    gManp->type = MANDELFP;
    gManp->GlobalHwnd = hwnd;				// Used in putcolor. Saves a lot of param passing.
//    PixelHwnd = hwnd;
    gManp->init(hwnd);				// init the main Fractint and screen code 
    initFibonacci();				// just the starting values
#ifdef	DEBUG
    strcpy(szFileName, "Test Filename");
#endif
    strcpy(szTitleName, "ManpWin");
    if (*argumentFileName != '\0')		// command line filename
	CopyArgument(hwnd, szFileName, argumentFileName);	// ensure correct filename copied
    ShowWindow (hwnd, SW_MAXIMIZE);
    pfract_main(gManp, hwnd, szSaveFileName);	// fire up the main Fractint code
    DestroyWindow(hWndCopy);			// stop everything when it returns
    delete gManp;
    gManp = nullptr;
    return(FALSE);				// we done when 'pfract_main' returns
    }

/*-----------------------------------------
	Just a quicky
  -----------------------------------------*/

void	CManp::DoCaption (HWND hwnd, char *szTitleName)
     {
     char szCaption [184];

     _snprintf_s(szCaption, 184, _TRUNCATE, "%s", (LPSTR) (szTitleName));
     SetWindowText (hwnd, szCaption);
     }

/*-----------------------------------------
	Let's Process a few messages
  -----------------------------------------*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		gManp->ZoomEdge = TRUE;
		gManp->UseFractintPalette = FALSE;
		ReadConfig(hwnd);				// sticky info
		ViewFileInitialize (hwnd);			// Initialise file reading routines
		SaveFileInitialize (hwnd, hInst);		// Initialise file writing routines
		vscroll_count = 0;				// no scrolling yet
		hscroll_count = 0;
		szFileName[0] = '\0';				// can't reopen if not there
		SetScrollPos(hwnd, SB_VERT, 0, FALSE);
		SetScrollPos(hwnd, SB_HORZ, 0, FALSE);

#ifdef DEBUG
		_snprintf_s(TempString, 480, _TRUNCATE, "About to enter mainview() in WM_CREATE! argument = <%s>", argumentFileName);
		MessageBox (hwnd, TempString, szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

	       if (*argumentFileName != '\0')				// command line filename
		   {
		   _snprintf_s(TempString, 480, _TRUNCATE, "View File %s", szFileName);
		   gManp->DoCaption(hwnd, TempString);
		   CopyArgument(hwnd, szFileName, argumentFileName);	// ensure correct filename copied

#ifdef DEBUG
		   _snprintf_s(TempString, 480, _TRUNCATE, "About to enter mainview() in WM_CREATE! file = <%s>", szFileName);
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
			 // SVG export only available for Hailstone (sequence visualization, not pixel-based fractals)
			 EnableMenuItem((HMENU)(_int64)wParam, IDM_SAVE_SVG, (gManp->type == HAILSTONE) ? MF_ENABLED : MF_GRAYED);
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
			 // Hailstone toggle menu items - only enabled when viewing Hailstone fractal
			 EnableMenuItem((HMENU)(_int64)wParam, IDM_HAILSTONE_TOGGLE_AXES, (gManp->type == HAILSTONE) ? MF_ENABLED : MF_GRAYED);
			 EnableMenuItem((HMENU)(_int64)wParam, IDM_HAILSTONE_TOGGLE_LABELS, (gManp->type == HAILSTONE) ? MF_ENABLED : MF_GRAYED);
			 EnableMenuItem((HMENU)(_int64)wParam, IDM_HAILSTONE_TOGGLE_DOTS, (gManp->type == HAILSTONE) ? MF_ENABLED : MF_GRAYED);
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

	        CheckMenuItem((HMENU)(_int64)wParam, IDC_ADDSPIRAL, (gManp->addflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_3D, (gManp->_3dflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_JULIA, (gManp->juliaflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_REALTIMEJULIA, (gManp->RealTimeJuliaFlag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_CYCLE, (gManp->cycleflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_STEREO, (gManp->pairflag ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_GENMPEGSTATS, ((statname[0] != '-') ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_EDGE, (gManp->ZoomEdge ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_USEDEFAULTPALETTE, (gManp->UseFractintPalette ? MF_CHECKED : MF_UNCHECKED));
		CheckMenuItem((HMENU)(_int64)wParam, IDM_DISPLAYLOC, (DisplayLoc ? MF_CHECKED : MF_UNCHECKED));
//		CheckMenuItem((HMENU)wParam, IDM_SHOWSTATUSBAR, (ShowStatusBar ? MF_CHECKED : MF_UNCHECKED));
		// Hailstone toggle menu checkmarks
//		extern COtherFunctions* g_pOtherFunctions;
		if (g_pOtherFunctions && gManp->type == HAILSTONE)
		    {
		    CheckMenuItem((HMENU)(_int64)wParam, IDM_HAILSTONE_TOGGLE_AXES, (g_pOtherFunctions->ShowAxes ? MF_CHECKED : MF_UNCHECKED));
		    CheckMenuItem((HMENU)(_int64)wParam, IDM_HAILSTONE_TOGGLE_LABELS, (g_pOtherFunctions->ShowPointLabels ? MF_CHECKED : MF_UNCHECKED));
		    CheckMenuItem((HMENU)(_int64)wParam, IDM_HAILSTONE_TOGGLE_DOTS, (g_pOtherFunctions->ShowDots ? MF_CHECKED : MF_UNCHECKED));
		    }

		break;
		
	  case WM_COMMAND:
                // Process menu commands
                return MenuCommand (hwnd, message, wParam, lParam);
                break;

	  case WM_TIMER:
	       if (wParam == ANIMTIMER)
		   gManp->DoAnimation();
	       else if (wParam == UPDATETIMER)
		   gManp->DoUpdate();
	       return 0;

	  case WM_MOVE:
		ShowWindow (hwnd, SW_SHOWNORMAL);
		break;

	  case WM_SIZE:
		InvalidateRect(hwnd, &gManp->r, FALSE);
		SetScrollRanges(hwnd);
		GetClientRect(hwnd, &gManp->r);
		gManp->xdots = gManp->width;
		gManp->ydots = gManp->height;
		gManp->AspectRatio = (double)gManp->xdots / (double)gManp->ydots;
		return 0;

	  case WM_LBUTTONDOWN:				// message: left mouse button pressed
	  case WM_RBUTTONDOWN:				// message: right mouse button pressed
	      gManp->cycleflag = FALSE;
		if (gManp->OscProcess.DisplayAxisImages)
		    {
		    gManp->OscProcess.ChooseOsc(MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y);
		    break;
		    }
		if (GetPixelColourFlag)
		    {
		    EditRGB(hInst, MAKEPOINTS(lParam));
		    GetPixelColourFlag = FALSE;
		    gManp->DisplayFractal(hwnd);
		    break;
		    }
		if (GetKeyState(VK_SHIFT) < 0)	// find julia from this point
		    {
		    gManp->bTrack = FALSE;
		    if (gManp->juliaflag)
			{
			gManp->hor = gManp->oldhor;		// restore values after julia transformations
			gManp->vert = gManp->oldvert;
			gManp->mandel_width = gManp->oldwidth;
			gManp->juliaflag = FALSE;
			}
		    else
			{
			gManp->j.x = gManp->hor + gManp->mandel_width * (double)(MAKEPOINTS(lParam).x) / (double)gManp->ydots;
			gManp->j.y = gManp->vert + gManp->mandel_width * (double)(MAKEPOINTS(lParam).y) / (double)gManp->ydots;
			gManp->oldhor = gManp->hor;		// store values during julia transformations
			gManp->oldvert = gManp->vert;
			gManp->oldwidth = gManp->mandel_width;
			gManp->hor = -3.555555555555556;
			gManp->vert = -2.0;
			gManp->mandel_width = 4.0;
			gManp->juliaflag = TRUE;
			}
		    gManp->time_to_zoom = TRUE;
		    }
		else
		    {					// Start selection of region
		    gManp->bTrack = TRUE;
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
		if (gManp->bTrack)
		    UpdateSelection(hwnd, MAKEPOINTS(lParam), &Rect, Shape);
		else if (gManp->RealTimeJuliaFlag && !gManp->juliaflag)
		    {
		    if (!gManp->RTJuliaActive)					// give last request time to complete
			{
			if (gManp->DrawJulia(hwnd, MAKEPOINTS(lParam)) < 0)
			    gManp->RealTimeJuliaFlag = FALSE;
			}
		    }
		break;

	  case WM_LBUTTONUP:				// message: left mouse button released

		if (gManp->bTrack)
		    {
		    EndSelection(MAKEPOINTS(lParam), &Rect);
		    ClearSelection(hwnd, &Rect, Shape);
		    if (gManp->ZoomIn(hwnd, &Rect))
			{
			HardStopNow(hwnd, "zoom-in");
			gManp->time_to_zoom = TRUE;
			}
		    }
		return 0;

	  case WM_RBUTTONUP:				// message: left mouse button released

		if (gManp->bTrack)
		    {
		    EndSelection(MAKEPOINTS(lParam), &Rect);
		    ClearSelection(hwnd, &Rect, Shape);
		    if (gManp->ZoomOut(hwnd, &Rect))
			{
			HardStopNow(hwnd, "zoom-out");
			gManp->time_to_zoom = TRUE;
			}
		    }
		return 0;

	  case WM_COPYDATA:
#ifdef	DEBUG
	      SAFE_SPRINTF(TempString, "hWnd=%8.8X, wParam=%8.8X, lParam=%8.8X\ncopydata = <%s>", hwnd, wParam, lParam,
		  ((PCOPYDATASTRUCT)lParam) -> lpData);
		MessageBox (hwnd, TempString, "Received Copydata message", MB_ICONEXCLAMATION | MB_OK);
#endif
		gManp->setup_defaults();
		GetParamData(hwnd, "null", (char *)((PCOPYDATASTRUCT)(_int64)lParam) -> lpData, szSaveFileName, FALSE);
			// 2 for Device Context, 1 for DIB/WM_COPYDATA and 0 for clipboard
		ReplyUsingDIB = (int)(((PCOPYDATASTRUCT)(_int64)lParam) -> dwData);
		CallingWindowHandle = (HWND)(_int64)wParam;
		InitTrueColourPalette(FALSE);

		gManp->time_to_reinit = TRUE;
		break;

	  case WM_CLOSE:
		if (MessageBox (hwnd, "Do you want to Quit?", "ManpWin", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)
		    {
		    gManp->time_to_quit = TRUE;
		    SendMessage (hwnd, WM_DESTROY, 0, 0L);
		    }
		else
		    return 0;

		gManp->time_to_quit = TRUE;
		hWndCopy = hwnd;
		return 0;

	  case WM_PAINT:
		ChangeView(hwnd, -GetScrollPos (hwnd, SB_HORZ), -GetScrollPos (hwnd, SB_VERT), gManp->ptSize.x, gManp->ptSize.y, 0, 0, gManp->width, gManp->height, TRUE);
		return 0;
        

          case WM_VSCROLL:
            						// Calculate new vertical scroll position 
            	GetScrollRange (hwnd, SB_VERT, &iMin, &iMax);
            	iPos = GetScrollPos (hwnd, SB_VERT);
            	GetClientRect (hwnd, &gManp->r);

	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
            	    {
                    case SB_LINEDOWN:
                        dn = gManp->r.bottom / 16 + 1;
                        break;

                    case SB_LINEUP:
                        dn = -gManp->r.bottom / 16 + 1;
                        break;

                    case SB_PAGEDOWN:
                        dn = gManp->r.bottom / 2  + 1;
                        break;
                        
                    case SB_PAGEUP:
                        dn = -gManp->r.bottom / 2  + 1;
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
		InvalidateRect(hwnd, &gManp->r, FALSE);
                break;

          case WM_HSCROLL:
            						// Calculate new horizontal scroll position
                GetScrollRange (hwnd, SB_HORZ, &iMin, &iMax);
                iPos = GetScrollPos (hwnd, SB_HORZ);
                GetClientRect (hwnd, &gManp->r);

	        switch ((int) LOWORD(wParam))
                    {
                    case SB_LINEDOWN:
                        dn = gManp->r.right / 16 + 1;
                        break;

                    case SB_LINEUP:
                        dn = -gManp->r.right / 16 + 1;
                        break;

                    case SB_PAGEDOWN:
                        dn = gManp->r.right / 2  + 1;
                        break;

                    case SB_PAGEUP:
                        dn = -gManp->r.right / 2  + 1;
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
		InvalidateRect(hwnd, &gManp->r, TRUE);
                break;

        case WM_KEYDOWN:					// Handle any keyboard messages
	    ProcessKeys(hwnd, wParam);
	    break;

	case WM_DESTROY :
		if (gManp->RunAnimation)
		    {
		    gManp->time_to_break = TRUE;
		    gManp->RunAnimation = FALSE;
		    gManp->AnimateClose();
		    }
		if (gManp->type == SLOPEDERIVATIVE || gManp->type == SLOPEFORWARDDIFF)
		    {
		    EndSlope();					// advise the threads to close
		    Sleep(250);					// give them a quarter second to do so
		    }
		ClosePtrs();
		HardStopNow(hwnd, "app exit");
		SaveConfig(hwnd);
		HtmlHelp(hwnd, NULL, HH_CLOSE_ALL, (DWORD)NULL);
		PostQuitMessage (0);
	       	return 0;
	  }
	  return DefWindowProc(hwnd, message, wParam, lParam);
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

LRESULT CALLBACK PASCAL	MenuCommand (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
    int		status;
    char	TempStr[MAX_PATH];
    POINTS	CursorLocShort;

    if (gManp->DisplayAnimation)			// we are currently displaying an animation
	{
	gManp->time_to_break = TRUE;
	gManp->AnimateClose();
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
	    DialogBox (hInst, "AboutBox", hwnd, AboutDlgProc);
	    return 0;

	case IDM_PATHS:
	    DialogBox (hInst, "CreateDirDlg", hwnd, CreateDirDlg);
	    return 0;

	case IDM_GET_PIXEL_COLOUR:
	    GetPixelColourFlag = TRUE;	// get the colour of the pixel when mouse button pressed
	    return 0;

	case IDM_SETINSIDE:
	    if (DialogBox (hInst, "InsideDlg", hwnd, InsideDlg))
		gManp->DisplayFractal(hwnd);
	    return 0;

 				  // Messages from Fractals menu
	case IDM_FRACTLOC:                
	    if (gManp->RealTimeJuliaFlag)
		DialogBox (hInst, "RTJuliaLocDlg", hwnd, RTJuliaLocDlg);
	    else
		DialogBox(hInst, "StatusInfoDlg", hwnd, StatusInfoDlg);
	    break;

	case IDM_CYCLE:
	    gManp->cycleflag = !gManp->cycleflag;
	    if (gManp->cycleflag)
		rotate((int)lParam);
	    break;

	case IDM_STEP:
	    rotate_vga_palette((int)lParam, 0);
	    break;

	case IDM_RDS:
	    draw3D(hwnd);
	    InvalidateRect(hwnd, &gManp->r, FALSE);
	    return 0;

	case IDM_RDS_SPOTS:
	    if (AutoStereoActive)
		DrawSpots();
	    InvalidateRect(hwnd, &gManp->r, FALSE);
	    return 0;

	case IDM_SETUPANIMATION:
	case IDM_JULIA_ANIM:
	case IDM_INVERSION_ANIM:
	case IDM_FOURIERANIM:
	case IDM_PARAM_ANIM:
	case IDM_OSCILLATOR_ANIMATION:
	case IDM_MORPHOSC:
	    if (gManp->TrueCol.PalEditFlag)						// palette has changed
		{
		if (MessageBox (hwnd, "Palette has changed! Do you wish to save first?", "ManpWIN", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		    {
		    if (SaveMAPFileOpenDlg (hwnd, szSaveFileName, szTitleName) == 0)
			{
			save_palette(hwnd, szSaveFileName, "Save Colour Parameters");
			strcpy(MAPFile, szSaveFileName);				// palette file to reflect the current map
			}
		    }
		gManp->TrueCol.PalEditFlag = FALSE;
		}
	    status = 0;
	    switch (wParam)
		{
		case IDM_SETUPANIMATION:
		    status = (int)DialogBox (hInst, "AnimationDlg", hwnd, AnimationDlg);
		    break;
		case IDM_PARAM_ANIM:
		    if (gManp->type == OSCILLATORS || gManp->type == FRACTALMAPS || gManp->type == SPROTTMAPS || gManp->type == SURFACES || gManp->type == KNOTS || gManp->type == CURVES)
			status = (int)DialogBox (hInst, "OscParamAnimDlg", hwnd, ParamAnimDlg);
		    else if (gManp->type == PERTURBATION)
			status = (int)DialogBox(hInst, "PertParamAnimDlg", hwnd, ParamAnimDlg);
		    else
			status = (int)DialogBox(hInst, "ParamAnimDlg", hwnd, ParamAnimDlg);
		    break;
		case IDM_MORPHOSC:
		    if (gManp->type != OSCILLATORS && gManp->type != FRACTALMAPS && gManp->type != SPROTTMAPS/*&& type != SURFACES*/ && gManp->type != KNOTS && gManp->type != CURVES)
			{
			MessageBox(hwnd, "This Fractal is not an Oscillator, Fractal Map, Curve or Knot", "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			break;
			}
		    status = (int)DialogBox(hInst, "OscMorphAnimDlg", hwnd, OscMorphAnimDlg);
		    break;
		case IDM_OSCILLATOR_ANIMATION:
		    switch (gManp->type)
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
			    status = (int)DialogBox (hInst, "OscillatorAnimDlg", hwnd, OscillatorAnimDlg);
			    break;
			case IFS:
			    if (ifs_type == 1)
				{
				status = (int)DialogBox (hInst, "OscillatorAnimDlg", hwnd, OscillatorAnimDlg);
				break;
				}	// else fall through
			default:
			    _snprintf_s(TempStr, MAX_PATH, _TRUNCATE, "Fractal type %d (%s) cannot be configured for animation!\nChange the fractal type first", gManp->type, fractalspecific[gManp->type].name);
			    MessageBox (hwnd, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
			    status = 0;
			    break;
			}
		    break;
		case IDM_JULIA_ANIM:
		    if (gManp->type == PERTURBATION || gManp->type == OSCILLATORS || gManp->type == FRACTALMAPS || gManp->type == CURVES || gManp->type == KNOTS || gManp->type == SPROTTMAPS || 
				gManp->type == SURFACES || gManp->type == BUDDHABROT || gManp->type == CURLICUES || gManp->type == MANDELCLOUD || gManp->type == DYNAMICFP)
			{
			_snprintf_s(TempStr, MAX_PATH, _TRUNCATE, "Fractal type %d (%s) cannot be configured for Julia animation!\nChange the fractal type first", gManp->type, fractalspecific[gManp->type].name);
			MessageBox(hwnd, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
			}
		    else
			status = (int)DialogBox (hInst, "JuliaAnimDlg", hwnd, JuliaAnimDlg);
		    break;
		case IDM_INVERSION_ANIM:
		    status = (int)DialogBox (hInst, "JuliaAnimDlg", hwnd, InversionAnimDlg);
		    break;
		case IDM_FOURIERANIM:
		    if (DialogBox (hInst, "FourierTypeDlg", hwnd, FourierTypeDlg) > 0)
			status = (int)DialogBox (hInst, "FourierAnimDlg", hwnd, FourierAnimDlg);
		    break;
		}
	    if (status > 0)
		if (gManp->StartImmediately)
		    {
		    if (WriteMPEGFrames)
			{
			if (MPEGWrite(MPGFile) == -1)
			    return (-1);
			}
		    else
			{
			if (gManp->RunScript(hwnd, ScriptFileName) == -1)
			    return (-1);
			}
		    }

	    return (1);

	case IDM_STOPANIM:
	    gManp->AnimateClose();
	    gManp->time_to_break = TRUE;
	    return 0;

	case IDM_PAUSEANIM:
	    gManp->SuspendAnimation = !gManp->SuspendAnimation;
	    if (gManp->SuspendAnimation)
		gManp->AnimateSuspend();
	    else
		gManp->AnimateResume();
	    return 0;


	case IDM_SETUPTHREADS:
	    if (DialogBox(hInst, "ThreadingDlg", hwnd, ThreadingDlg) == TRUE)
		{
		gManp->DisplayStatusBarInfo(INITIALISING, "");		// display status bar
		HardStopNow(hwnd, "Setup Num Threads");
		gManp->time_to_reinit = TRUE;
		}
	    else
		return 0;
	    break;

	case IDM_COLOUR_PAL:
	    if (DialogBox (hInst, "ColourDlg", hwnd, ColourDlg) == TRUE)
		{
		InvalidateRect(hwnd, &gManp->r, FALSE);
		InitTrueColourPalette(TRUE);
		gManp->Plot.RefreshScreen();
		DisplayPalette(hwnd, gManp->TrueCol.DisplayPaletteFlag);
		gManp->DisplayFractal(hwnd);
		}
	    return 0;

	case IDM_EDITPAL:
	    if (DialogBox (hInst, "EditPalDlg", hwnd, EditPalDlg) == TRUE)
		{
		InvalidateRect(hwnd, &gManp->r, FALSE);
		InitTrueColourPalette(TRUE);
		gManp->Plot.RefreshScreen();
		DisplayPalette(hwnd, gManp->TrueCol.DisplayPaletteFlag);
		gManp->DisplayFractal(hwnd);
		}
	    return 0;

	case IDM_EDGE:
	    gManp->ZoomEdge = !gManp->ZoomEdge;
	    return 0;

	case IDM_USEDEFAULTPALETTE:
	    gManp->UseFractintPalette =	!gManp->UseFractintPalette;
	    gManp->TrueCol.IsMAPFile = gManp->UseFractintPalette;
	    InitTrueColourPalette(FALSE);
	    gManp->time_to_reinit = TRUE;
	    return 0;

	case IDM_GENMPEGSTATS:
	    statname[0] = (statname[0] == '-') ? '\0' : '-';	// swap state
	    return 0;

	case IDM_DISPLAYLOC:
	    DisplayLoc = !DisplayLoc;
	    if (!DisplayLoc)
		gManp->DisplayFractal(hwnd);		// restore status
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

	case IDM_SAVEPAR:							// par file
	    if (gManp->TrueCol.PalEditFlag)					// palette has changed
		{
		if (MessageBox (hwnd, "Palette has changed! Do you wish to save first?", "ManpWIN", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		    {
		    if (SaveMAPFileOpenDlg (hwnd, szSaveFileName, szTitleName) == 0)
			{
			save_palette(hwnd, szSaveFileName, "Save Colour Parameters");
			strcpy(MAPFile, szSaveFileName);			// palette file to reflect the current map
			}
		    }
		gManp->TrueCol.PalEditFlag = FALSE;
		}

	    if (SaveParOpenDlg (hwnd, szSaveFileName, szTitleName) == 0)
		SaveFile (hwnd, szSaveFileName, szTitleName);                    
	    return 0;

	case IDM_SAVE_KALLES:							// kfr file
	    if (gManp->TrueCol.PalEditFlag)					// palette has changed
		{
		if (MessageBox(hwnd, "Palette has changed! Do you wish to save first?", "ManpWIN", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		    {
		    if (SaveMAPFileOpenDlg(hwnd, szSaveFileName, szTitleName) == 0)
			{
			save_palette(hwnd, szSaveFileName, "Save Colour Parameters");
			strcpy(MAPFile, szSaveFileName);			// palette file to reflect the current map
			}
		    }
		gManp->TrueCol.PalEditFlag = FALSE;
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

	case IDM_SAVE_SVG:							// SVG vector file
	    if (SaveSVGOpenDlg(hwnd, szSaveFileName, szTitleName) == 0)
		SaveFile(hwnd, szSaveFileName, szTitleName);
	    return 0;

	case IDM_OPEN_PAR:
	    if (PARFileOpenDlg(hwnd, szTitleName) == 0)
		{
		gManp->DisplayStatusBarInfo(INITIALISING, "");				// display status bar
		gManp->setup_defaults();
		GetParam(hwnd, PARFile, szSaveFileName);
		gManp->TrueCol.IsMAPFile = false;
		HardStopNow(hwnd, "open .PAR");
		gManp->time_to_reinit = TRUE;
		}
	    else
		return 0;
	    break;

	case IDM_OPEN_KALLES:
	    if (KFRFileOpenDlg(hwnd, szTitleName) == 0)
		{
		gManp->DisplayStatusBarInfo(INITIALISING, "");				// display status bar
		gManp->setup_defaults();
		ReadKallesFile(hwnd, KFRFile);
		HardStopNow(hwnd, "open .KFR");
		gManp->time_to_reinit = TRUE;
		}
	    else
		return 0;
	    break;

	case IDM_OPEN_PNG:
	    if (PNGFileOpenDlg(hwnd, szTitleName) == 0)
		{
		gManp->setup_defaults();
		if (read_png_file(hwnd, PNGFile) >= 0)
		    gManp->DataFromPNGFile = TRUE;
		}
	    else
		return 0;
	    gManp->TrueCol.IsMAPFile = false;
	    HardStopNow(hwnd, "open .PNG");
	    gManp->time_to_reinit = TRUE;
	    break;

	case IDM_OPEN_SCI: 
	    if (SCIFileOpenDlg(hwnd, szTitleName) == 0)
		{
		GetPNGSeqFromScript(hwnd, SCIFile);
		if (DialogBox (hInst, "AnimStartDlg", hwnd, AnimStartDlg))
		    {
		    if (WriteMPEGFrames)
			{
			if (MPEGWrite(MPGFile) == -1)
			    {
			    gManp->time_to_reinit = FALSE;
			    return (-1);
			    }
			}
		    else
			{
			if (gManp->RunScript(hwnd, SCIFile) == -1)
			    {
			    gManp->time_to_reinit = FALSE;
			    return (-1);
			    }
			}
		    }
		}
	    else
		return 0;
	    gManp->time_to_reinit = TRUE;
	    break;

	case IDM_OPEN_LST: 
	    if (LSTFileOpenDlg(hwnd, szTitleName) == 0)
		{
		if (MPEGWrite(MPGFile) < 0)
		    return 0;
		}
	    else
		return 0;
	    gManp->time_to_reinit = TRUE;
	    break;

	case IDM_OPEN_TRUE_MAP: 
	    if (ColFileOpenDlg (hwnd, szTitleName) == 0)
		{
		FileReadColours(hwnd, COLFile, "Paul's Fractals: Get Colour Parameters");
		InvalidateRect(hwnd, &gManp->r, FALSE);
		InitTrueColourPalette(FALSE);
		gManp->Plot.RefreshScreen();
		gManp->time_to_reinit = TRUE;
		}
	    else
		return 0;
	    break;
	
	case IDM_OPEN_PAL_MAP: 
	    if (MAPFileOpenDlg (hwnd, szTitleName) == 0)
		FilePalette(hwnd, MAPFile, "Paul's Fractals: Get Colour Map");
	    else
		return 0;
	    gManp->TrueCol.IsMAPFile = true;
	    HardStopNow(hwnd, "MAP");
//	    time_to_reinit = (type == SLOPEDERIVATIVE || type == SLOPEFORWARDDIFF);
	    gManp->time_to_reinit = true;
	    break;

	case IDM_EXIT:
	    if (MessageBox (hwnd, "Do you want to Quit?", "ManpWin", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)
		{
		gManp->time_to_quit = TRUE;
		SendMessage (hwnd, WM_DESTROY, 0, 0L);
		if (gManp->DisplayAnimation)
		    gManp->AnimateClose();
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
	case IDM_HAILSTONE_TOGGLE_AXES:
	case IDM_HAILSTONE_TOGGLE_LABELS:
	case IDM_HAILSTONE_TOGGLE_DOTS:
	    switch (wParam)
		{
		case IDM_3DPARAM:
		    if (DialogBox (hInst, "Param3D", hwnd, Param3D) == FALSE)
			return 0;
		    else
			gManp->time_to_reinit = TRUE;
		    break;

		case IDM_3D:
		    gManp->_3dflag = !gManp->_3dflag;
		    if (gManp->_3dflag)
			{
			gManp->oldcalcmode = gManp->calcmode;
			gManp->pairflag = FALSE;
			}
		    else
			gManp->calcmode = gManp->oldcalcmode;		// restore after 3D
		    gManp->time_to_reinit = TRUE;
		    break;

	    case IDM_SET_JULIA_SIZE:
		DialogBox (hInst, "JuliaDlg", hwnd, JuliaDlg);
		break;

	    case IDM_JULIA:
		gManp->juliaflag = !gManp->juliaflag;
		HardStopNow(hwnd, "Julia");
		gManp->ToggleJulia(hwnd, CursorLocShort);
		break;

	    case IDM_STEREO:
		if (gManp->pairflag)
		    {
		    gManp->pairflag = FALSE;
		    gManp->calcmode = gManp->oldcalcmode;		// restore after stereo pair
		    }
		else
		    {
		    gManp->pairflag = 10;
		    gManp->oldcalcmode = gManp->calcmode;
		    gManp->_3dflag = FALSE;
		    }

		HardStopNow(hwnd, "Stereo");
		gManp->time_to_reinit = TRUE;
		break;

	    case IDM_REALTIMEJULIA:
		gManp->RealTimeJuliaFlag = !gManp->RealTimeJuliaFlag;
		if (gManp->RealTimeJuliaFlag)
		    {
		    if (!gManp->juliaflag)		// doesn't make a lot of sense if we are in julia already
			{
			gManp->InitRTJulia(hwnd);
			FindCursorRealPos(&CursorLocShort);
			if (gManp->DrawJulia(hwnd, CursorLocShort) < 0)
			    gManp->RealTimeJuliaFlag = FALSE;
			}
		    return 1;			// ensure that we don't splatter initial RT Julia set
		    }
		else
		    {
		    HardStopNow(hwnd, "RTJulia");
		    gManp->time_to_restart = TRUE;
		    }
		break;

	    case IDM_STEREOPAIRSETUP:
		    if (DialogBox(hInst, "StereoPairDlg", hwnd, StereoPairDlg) == FALSE)
			return 0;
		    else
			gManp->time_to_reinit = TRUE;
		    break;

	    case IDM_HAILSTONE_TOGGLE_AXES:
		g_pOtherFunctions->ShowAxes = !g_pOtherFunctions->ShowAxes;
		gManp->time_to_reinit = TRUE;
		break;

	    case IDM_HAILSTONE_TOGGLE_LABELS:
		g_pOtherFunctions->ShowPointLabels = !g_pOtherFunctions->ShowPointLabels;
		gManp->time_to_reinit = TRUE;
		break;

	    case IDM_HAILSTONE_TOGGLE_DOTS:
		g_pOtherFunctions->ShowDots = !g_pOtherFunctions->ShowDots;
		gManp->time_to_reinit = TRUE;
		break;

	    case IDM_TOGGLE_DISPLAY_PALETTE:
		gManp->TrueCol.DisplayPaletteFlag = !gManp->TrueCol.DisplayPaletteFlag;
/*
		Secondaryhwnd = hwnd;
		Secondarymessage = message;
		SecondarywParam = wParam;
		SecondarylParam = lParam;
*/
		DisplayPalette(hwnd, gManp->TrueCol.DisplayPaletteFlag);
		gManp->DisplayFractal(hwnd);
		return 0;								// don't break as we don't want to reinit

//	    case IDM_IMAGE_SIZE:
//		time_to_restart = TRUE;
//		break;

	    case IDM_REDO:
		gManp->LoadUndo(FALSE);
		HardStopNow(hwnd, "redo");
		gManp->time_to_zoom = TRUE;
		gManp->time_to_restart = TRUE;
		break;

	    case IDM_UNDO:
		gManp->LoadUndo(TRUE);
		HardStopNow(hwnd, "undo");
		gManp->time_to_zoom = TRUE;
		gManp->time_to_restart = TRUE;
		break;

	    case IDM_UPDATE_FRACTAL:
		{
		BOOL result;
		if (result = UpdateFractal(hwnd) ==  TRUE)
		    {
		    HardStopNow(hwnd, "update fractal");
		    gManp->time_to_reinit = TRUE;
		    }
		break;
		}

	    case IDC_FRACTYPE:
		{
		struct	UNDO	RestoreFract;

		gManp->BigNumFlag = FALSE;
//		Dib.ClearDib(0L);
		if (InitNewFractal(hwnd) == FALSE)
		    {
//		    Redo(&RestoreFract);
		    return 0;
		    }
		else
		    {
		    gManp->Undo(&RestoreFract);
		    HardStopNow(hwnd, "fractal type changed");
		    gManp->time_to_reinit = TRUE;
		    gManp->calcfracinit();
		    }
		break;
		}

	    case IDM_TOGGLEPERT:
		{
		WORD    OldType = gManp->type;
		int	OldSubtype = gManp->subtype;
		if (TogglePerturbation(&gManp->type, &gManp->subtype) < 0)
		    {
		    gManp->type = OldType;
		    gManp->subtype = OldSubtype;
		    MessageBox(hwnd, "There is no equivelent method to toggle into", "Perturbation", MB_ICONEXCLAMATION | MB_OK);
		    }
		else
		    {
		    gManp->time_to_reinit = TRUE;
		    HardStopNow(hwnd, "toggle perturbation");
		    }
		break;
		}
	    case IDM_FRACOPTIONS:
		{

#ifdef TESTFWDDIFF
		if (DialogBox(hInst, "FractalTestDlg", hwnd, FractalTestDlg) == FALSE)
#else
		if (DialogBox(hInst, "FractalDlg", hwnd, FractalDlg) == FALSE)
#endif // TESTFWDDIFF

		    return 0;
		else
		    {
		    if (gManp->OutsideMethod == TIERAZONFILTERS)
			if (DialogBox (hInst, "SelectTierazonDlg", hwnd, SelectFilterDlg) == FALSE)
			    return 0;
		    if (gManp->OutsideMethod == TIERAZONCOLOURS)
			if (DialogBox (hInst, "SelectTierazonDlg", hwnd, SelectColourDlg) == FALSE)
			    return 0;
		    gManp->TrueCol.FillPalette(REPEAT, gManp->TrueCol.PalettePtr, gManp->threshold);
		    HardStopNow(hwnd, "Fractal options changed");
		    gManp->time_to_reinit = TRUE;
		    }
		break;
		}

	    case IDM_FDOPTIONS:
		if (DialogBox (hInst, "SelectFDOptionDlg", hwnd, SelectFDOptionDlg) == FALSE)
		    return 0;
		else
		    {
		    HardStopNow(hwnd, "FD Filter options change");
		    gManp->time_to_reinit = TRUE;
		    }
		break;

	    case IDM_FRACTALCOORDS:
		if (DialogBox (hInst, "CoordDlg", hwnd, CoordDlg) == FALSE)
		    return 0;
		else
		    {
		    HardStopNow(hwnd, "Fractal co-ordinate change change");
		    gManp->time_to_reinit = TRUE;
		    }
		break;
	    case IDM_IMAGE_SIZE:
		if (DialogBox (hInst, "ImageSizeDlg", hwnd, ImageSizeDlg) == FALSE)
		    return 0;
		else
		    {
		    gManp->time_to_reinit = TRUE;
		    HardStopNow(hwnd, "image size change");
//		    ClosePtrs();				// ready for next screen
		    mainview(hwnd, FALSE);			// all screen specific stuff
		    }
		break;
	    }
	return 0;
	}
    return 0;
    }

/*-----------------------------------------
	Handle View Options
  -----------------------------------------*/

void	ChangeView(HWND hwnd, int xdest, int ydest, int widthdest, int heightdest, int xsrc, int ysrc, int widthsrc, int heightsrc, char stretchflag)
    { 
    HPALETTE hpalT;

    BeginPaint(hwnd, &gManp->ps);
    hpalT = SelectPalette (gManp->ps.hdc, hpal, FALSE);
    RealizePalette(gManp->ps.hdc);
    SetMapMode(gManp->ps.hdc,MM_TEXT);
    SetBkMode(gManp->ps.hdc,TRANSPARENT);
    SetStretchBltMode (gManp->ps.hdc, STRETCH_DELETESCANS) ;	// correct stretch mode for zoom etc
    StretchDIBits (gManp->ps.hdc, xdest, ydest, widthdest, heightdest, xsrc, ysrc, widthsrc, heightsrc,
			       (LPSTR)gManp->Dib.DibPixels.data(), (LPBITMAPINFO)gManp->Dib.pDibInf, DIB_PAL_COLORS, SRCCOPY);
    SelectPalette(gManp->ps.hdc,hpalT,FALSE);
//    if (ShowStatusBar)
	gManp->OutputStatusBar(hwnd);
    EndPaint(hwnd, &gManp->ps);
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
    InvalidateRect(hwnd, &gManp->r, FALSE);
    gManp->caption = GetSystemMetrics(SM_CYCAPTION) + xFrame * 2 + GetSystemMetrics(SM_CYMENU) * 2;
    gManp->scroll_width = yFrame * 2;
    display_width = gManp->ptSize.x + gManp->scroll_width;
    display_height = gManp->ptSize.y + gManp->caption;					// include caption
    display_width = (display_width < gManp->screenx) ? display_width : gManp->screenx;	// large pictures
    display_height = (display_height < gManp->screeny) ? display_height : gManp->screeny;
    MoveWindow (hwnd, (gManp->screenx - display_width) / 2, (gManp->screeny - display_height) / 2, display_width, display_height, TRUE);
    HorOffset = (gManp->screenx - display_width) / 2 + yFrame;
    VertOffset = (gManp->screeny - display_height) / 2 + GetSystemMetrics(SM_CYCAPTION) + yFrame + GetSystemMetrics(SM_CYMENU);
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

void	CManp::InitFract(int type)
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

    gManp->Fractal.InitData();				// setup for next fractal
    gManp->TrueCol.IsMAPFile = gManp->UseFractintPalette;
    InitTrueColourPalette(FALSE);
    gManp->setup_defaults();
    gManp->IsPAR = FALSE;
    gManp->IsKFR = FALSE;
    gManp->OscProcess.DisplayAxisImages = FALSE;
    DisplayAxisLabels = FALSE;
    gManp->OscProcess.AxisDisplay = OFF;
    gManp->TrueCol.IsMAPFile = false;

    if (WasFractPar)				// was our last time through this dialogue box a FRACTINT PAR?
	gManp->type = FRACTPAR;				// allow the dialogue box to remember where we were last time
    if (DialogBox (hInst, "FractTypeDlg", hwnd, FractTypeDlg) == TRUE)
	{
	gManp->cycleflag = FALSE;
	WasFractPar = FALSE;
	gManp->RebuildFractalMetadata(gManp->type, gManp->subtype);		// load all the metadata for parameters
	gManp->juliaflag = (fractalspecific[gManp->type].juliaflag == JULIAFP);
	if (gManp->type != OldType)
	    {
	    gManp->xAxis = 0;
	    gManp->yAxis = 1;
	    gManp->zAxis = 2;
	    }
	OldType = gManp->type;
	if (gManp->type == FPPICKOVER || gManp->type == FPLORENZ3D || gManp->type == FPLORENZ3D1 || gManp->type == FPLORENZ3D3
	    || gManp->type == FPLORENZ3D4 || gManp->type == FPROSSLER || gManp->type == IFS)
	    {
	    gManp->x_rot = 60.0;
	    gManp->y_rot = 30.0;
	    gManp->z_rot = 0.0;
	    }
	else if (gManp->type == KAM3DFP)
	    {
	    gManp->x_rot = 30.0;
	    gManp->y_rot = 15.0;
	    gManp->z_rot = 0.0;
	    }
	else if (gManp->type == SURFACES)
	    {
	    gManp->x_rot = -20.0;
	    gManp->y_rot = 15.0;
	    gManp->z_rot = 0.0;
	    }
	else 
	    {
	    gManp->x_rot = 10.0;
	    gManp->y_rot = 5.0;
	    gManp->z_rot = 0.0;
	    }

	if (gManp->type == KAM3DFP || gManp->type == KAMFP || gManp->type == IFS)
	    gManp->threshold = 1000L;
	switch (gManp->type)					// further subtypes?
	    {
	    case LSYSTEM:					// LSystem Fractal
		if (LsysFileOpenDlg (hwnd, szTitleName) < 0)
		    return FALSE;
		if (load_lsystems(hwnd, LSYSFile) >= 0)
		    {
		    gManp->type = LSYSTEM;
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    }
		else
		    return FALSE;
		if (DialogBox (hInst, "SelectFractal", hwnd, SelectFractal))
		    DialogBox (hInst, "LSystemDlg", hwnd, LSystemDlg);
		else
		    return FALSE;
		break;                 
	    case FORMULA:					// Formula Fractal
	    case FFORMULA:
		gManp->type = FORMULA;
		if (FormulaFileOpenDlg (hwnd, szTitleName) < 0)
		    return FALSE;
		if (get_formula_names(hwnd, FRMFile) < 0)
		    return FALSE;
		if (DialogBox (hInst, "SelectFractal", hwnd, SelectFractal))
		    {
		    gManp->InitFract(gManp->type);
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    if (DialogBox (hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
			return FALSE;
		    if (!fpFormulaSetup(FRMFile))
			return FALSE;
		    }
		else
		    return FALSE;
		break;                 
	    case SCREENFORMULA:					// On screen Formula Fractal
		if (DialogBox (hInst, "ScrnFormDlg", hwnd, ScrnFormDlg))
		    {
		    gManp->InitFract(gManp->type);
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    if (DialogBox (hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
			return FALSE;
		    if (ProcessFormulaString(gManp->FormulaString) == -1)
			return FALSE;
		    }
		else
		    return FALSE;
		break;                 
	    case TIERAZON:					// Generic Tierazon Fractal
		gManp->type = TIERAZON;
		setup_Tierazon();				// count number of Tierazon fractals in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, SelectTierazonDlg) == FALSE)
		    return FALSE;
		else
		    {
		    gManp->InitFract(gManp->type);
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    LoadTierazonParams();			// get parameters  and other subtype specific stuff from Tierazon database 
		    if (DialogBox (hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;                 
	    case MANDELDERIVATIVES:				// Generic Mandelbrot Derivatives Fractal
		gManp->type = MANDELDERIVATIVES;
		setup_MandelDeriv();				// count number of Madelbrot derivatives in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, SelectMandelDerivDlg) == FALSE)
		    return FALSE;
		else
		    {
		    gManp->InitFract(gManp->type);
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    LoadMandelDerivParams();			// get parameters  and other subtype specific stuff from Mandelbrot Derivatives database 
		    if (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;
	    case SLOPEDERIVATIVE:				// Generic Mandelbrot Derivatives Fractal
		gManp->type = SLOPEDERIVATIVE;
		setup_SlopeDeriv();				// count number of Madelbrot derivatives in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, SelectSlopeDerivDlg) == FALSE)
		    return FALSE;
		else
		    {
		    gManp->InitFract(gManp->type);
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    LoadSlopeDerivParams();			// get parameters  and other subtype specific stuff from Slope Derivative database 
		    if (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;
	    case SLOPEFORWARDDIFF:				// Generic Mandelbrot Forward Differencing Fractal
		gManp->type = SLOPEFORWARDDIFF;
		setup_SlopeFwdDiff();				// count number of Madelbrot derivatives in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, SelectSlopeFwdDiffDlg) == FALSE)
		    return FALSE;
		else
		    {
		    gManp->InitFract(gManp->type);
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    LoadSlopeFwdDiffParams();			// get parameters  and other subtype specific stuff from Forward Differencing database 
		    if (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;
	    case PERTURBATION:					// fractals calculated using perturbation
		gManp->type = PERTURBATION;
		setup_Perturbation();				// count number of perturbation fractals in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, SelectPertDlg) == FALSE)
		    return FALSE;
		else
		    {
		    gManp->InitFract(gManp->type);
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    LoadPerturbationParams();			// get parameters and other subtype specific stuff from Perturbation database 
		    if (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
			return FALSE;
		    }
		break;
	    case OSCILLATORS:					// Generic Oscillator Fractal
		gManp->type = OSCILLATORS;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_Oscillator();				// count number of Oscillator fractals in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Oscillator database 
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    if (DialogBox (hInst, OscillatorSpecific[gManp->subtype].DialogueName, hwnd, OscillatorSpecific[gManp->subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;                 
	    case FRACTALMAPS:					// Generic Fractal Map
		gManp->type = FRACTALMAPS;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_FractalMaps();				// count number of fractal maps in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Fractal Map database 
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    if (DialogBox(hInst, FractalMapSpecific[gManp->subtype].DialogueName, hwnd, FractalMapSpecific[gManp->subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;
	    case SPROTTMAPS:					// Generic Fractal Map
		gManp->type = SPROTTMAPS;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_SprottMaps();				// count number of fractal maps in database
		if (DialogBox(hInst, "SelectTierazonDlg", hwnd, SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Fractal Map database 
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    if (DialogBox(hInst, SprottMapSpecific[gManp->subtype].DialogueName, hwnd, SprottMapSpecific[gManp->subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;
	    case SURFACES:					// Generic Fractal Map
		gManp->type = SURFACES;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_Surface();				// count number of surfaces in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Surface database 
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    if (DialogBox (hInst, SurfaceSpecific[gManp->subtype].DialogueName, hwnd, SurfaceSpecific[gManp->subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;                 
	    case KNOTS:					// Generic Fractal Map
		gManp->type = KNOTS;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_Knot();				// count number of surfaces in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Knot database 
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    if (DialogBox (hInst, SurfaceSpecific[gManp->subtype].DialogueName, hwnd, KnotSpecific[gManp->subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;                 
	    case CURVES:					// Generic Fractal Map
		gManp->type = CURVES;
		DisplayAxes = FALSE;
		PlotCentre = FALSE;
		setup_Curve();				// count number of surfaces in database
		if (DialogBox (hInst, "SelectTierazonDlg", hwnd, SelectOscillatorDlg) == FALSE)
		    return FALSE;
		else
		    {
		    LoadParams();			// get parameters  and other subtype specific stuff from Curves database 
		    gManp->time_to_reinit = FALSE;
		    gManp->time_to_restart = TRUE;
		    gManp->time_to_load = FALSE;
		    if (DialogBox (hInst, SurfaceSpecific[gManp->subtype].DialogueName, hwnd, CurveSpecific[gManp->subtype].DialogueType) == FALSE)
			return FALSE;
		    }
		InitTrueColourPalette(FALSE);
		break;                 
	    case FRACTPAR:					// Fractint Par File
		if (FractintParFileOpenDlg (hwnd, szTitleName) < 0)
		    return FALSE;
		gManp->setup_defaults();
		if (load_par(hwnd, FracPARFile) < 0)
		    return FALSE;
		WasFractPar = TRUE;
		if (!DialogBox (hInst, "SelectFractal", hwnd, SelectFractal))
		    return FALSE;
		if (FractintPar(hwnd, FracPARFile) < 0)	// Fractint Par fractals 
		    return FALSE;
		gManp->time_to_reinit = FALSE;
		gManp->time_to_restart = TRUE;
		gManp->time_to_load = FALSE;
		break;                 
	    case IFS:					// Iterated File Systems Fractal
		if (IFSFileOpenDlg (hwnd, szTitleName) < 0)
		    return FALSE;
		if (get_IFS_names(hwnd, IFSFile) < 0)	    // get the IFS fractal names
		    return FALSE;
		if (DialogBox (hInst, "SelectFractal", hwnd, SelectFractal))
		    {
		    if (ifsload(hwnd, IFSFile) >= 0)
			{
			gManp->time_to_reinit = FALSE;
			gManp->time_to_restart = TRUE;
			gManp->time_to_load = FALSE;
//			hor = -6.0;
			gManp->hor = (ifs_type == 0) ? -12.0 : -6.0;
//			vert = (ifs_type == 0) ? -8.0 : -4.0;
			gManp->vert = -4.0;
			gManp->mandel_width = (ifs_type == 0) ? 16.0 : 8.0;
			}
		    }
		else
		    return FALSE;
		break;      
	    case BUDDHABROT:
		InitMultipliers();
		gManp->InitFract(gManp->type);
		if (DialogBox(hInst, fractalspecific[BUDDHABROT].DialogueName, hwnd, fractalspecific[BUDDHABROT].DialogueType) == FALSE)
		    return FALSE;
		InitTrueColourPalette(FALSE);
		gManp->hor = fractalspecific[BUDDHABROT].hor;
		gManp->vert = fractalspecific[BUDDHABROT].vert;
		gManp->mandel_width = fractalspecific[BUDDHABROT].width;
		break;
	    case NEWTON:
	    case NEWTBASIN:
		gManp->juliaflag = (fractalspecific[gManp->type].juliaflag);		// default view is of julia Newton
		// fall through (no break;)
	    default:
		gManp->InitFract(gManp->type);
		if (DialogBox (hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
		    return FALSE;
		InitTrueColourPalette(FALSE);
		gManp->hor = fractalspecific[gManp->type].hor;
		gManp->vert = fractalspecific[gManp->type].vert;
		gManp->mandel_width = fractalspecific[gManp->type].width;
		break;                           
	    }
	return TRUE;
	}           
    return FALSE;    
    }
     
/*-----------------------------------------------------------
    Toggle between Mandelbrot and Julia implementation 
  -----------------------------------------------------------*/

void	CManp::ToggleJulia(HWND hwnd, POINTS &CursorLocShort)
    {
    double	temp_1 = 0.0;
    double	temp_2 = 0.0;
    if (juliaflag)
	{
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
		j.x = hor + (mandel_width * (double)xdots / (double)ydots) / 2.0;
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
	    oldhor = j.x - (oldwidth * (double)xdots / (double)ydots) / 2.0;
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
    }

/*-----------------------------------------------------------
    Update Fractal info when existing fractal selected
  -----------------------------------------------------------*/

BOOL    UpdateFractal(HWND hwnd)
    {
    gManp->RebuildFractalMetadata(gManp->type, gManp->subtype);		// load all the metadata for parameters
    switch (gManp->type)					// further subtypes?
	{
	case LSYSTEM:					// LSystem Fractal
	    DialogBox(hInst, "LSystemDlg", hwnd, LSystemDlg);
	    break;
	case FORMULA:					// Formula Fractal
	    gManp->time_to_reinit = FALSE;
	    gManp->time_to_restart = TRUE;
	    gManp->time_to_load = FALSE;
	    if (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
		return FALSE;
	    return (fpFormulaSetup(FRMFile));
	case SCREENFORMULA:					// On screen Formula Fractal
	    if (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == FALSE)
		return FALSE;
	    return !(ProcessFormulaString(gManp->FormulaString) == -1);
	case TIERAZON:					// Generic Tierazon Fractal
	    return (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == TRUE);
	case OSCILLATORS:					// Generic Oscillator Fractal
	    gManp->MaxDimensions = OscillatorSpecific[gManp->subtype].MaxDimensions;
	    return (DialogBox(hInst, OscillatorSpecific[gManp->subtype].DialogueName, hwnd, OscillatorSpecific[gManp->subtype].DialogueType) == TRUE);
	case FRACTALMAPS:					// Generic Fractal Map
	    gManp->MaxDimensions = FractalMapSpecific[gManp->subtype].MaxDimensions;
	    return (DialogBox(hInst, FractalMapSpecific[gManp->subtype].DialogueName, hwnd, FractalMapSpecific[gManp->subtype].DialogueType) == TRUE);
	case SPROTTMAPS:					// Generic Sprott Fractal Map
	    gManp->MaxDimensions = SprottMapSpecific[gManp->subtype].MaxDimensions;
	    return (DialogBox(hInst, SprottMapSpecific[gManp->subtype].DialogueName, hwnd, SprottMapSpecific[gManp->subtype].DialogueType) == TRUE);
	case SURFACES:					// Generic Fractal Map
	    gManp->MaxDimensions = SurfaceSpecific[gManp->subtype].MaxDimensions;
	    return (DialogBox(hInst, SurfaceSpecific[gManp->subtype].DialogueName, hwnd, SurfaceSpecific[gManp->subtype].DialogueType) == TRUE);
	case KNOTS:					// Generic Fractal Map
	    gManp->MaxDimensions = KnotSpecific[gManp->subtype].MaxDimensions;
	    return (DialogBox(hInst, SurfaceSpecific[gManp->subtype].DialogueName, hwnd, KnotSpecific[gManp->subtype].DialogueType) == TRUE);
	case CURVES:					// Generic Fractal Map
	    gManp->MaxDimensions = CurveSpecific[gManp->subtype].MaxDimensions;
	    return (DialogBox(hInst, SurfaceSpecific[gManp->subtype].DialogueName, hwnd, CurveSpecific[gManp->subtype].DialogueType) == TRUE);
	case PERTURBATION:					// Generic Perturbation Fractal
	    return (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == TRUE);
	default:
	    return (DialogBox(hInst, fractalspecific[gManp->type].DialogueName, hwnd, fractalspecific[gManp->type].DialogueType) == TRUE);
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

    gManp->BatchFlag = FALSE;
    gManp->AutoExitFlag = FALSE;
    gManp->AutoSaveFlag = FALSE;

    if (*CommandStr == '\0')		// nothing to parse
	return 0;
    strcpy(ParameterString, CommandStr);	// preserve a copy so strtok() doesn't splatter it.
    if (*CommandStr == '-' || *CommandStr == '/')	// must be a command line with no par file
	{
	*szSaveFileName = '\0';		// initialise
	gManp->CommandLineFile = FALSE;
	gManp->AutoStartFlag = TRUE;
	gManp->BatchFlag = TRUE;
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
	gManp->CommandLineFile = TRUE;
	gManp->AutoStartFlag = TRUE;
	token = strtok(CommandStr, seps);
	while (token != NULL)
	    {
	    if (toupper(*token) == 'S')		// outputfilename 
		{
		gManp->AutoSaveFlag = TRUE;
		SAFE_SPRINTF(szSaveFileName, "%s", token + 1);
		}
	    else if (toupper(*token) == 'E')	// Exit after completion
		gManp->AutoExitFlag = TRUE;
	    token = strtok(NULL, seps);
	    }

	gManp->BatchFlag = TRUE;
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

INT_PTR CALLBACK RTJuliaLocDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
     static	char	PixelLoc[250]; 
     static     char	JuliaLoc[250];
     static	POINT	CursorLocLong;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        gManp->cycleflag = FALSE;
		GetCursorPos(&CursorLocLong);
		SAFE_SPRINTF(PixelLoc, "%ld, %ld", CursorLocLong.x, CursorLocLong.y);
		SAFE_SPRINTF(JuliaLoc, "%14.14f, %14.14f", gManp->q.x, gManp->q.y);
		SetDlgItemText(hDlg, IDC_PIXEL, PixelLoc);
		SetDlgItemText(hDlg, IDC_JULIALOC, JuliaLoc);
	        return TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDOK:
		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }

 void	DisplayLocation(HWND hwnd, POINTS ptCurrent)
    {
    double  x, y;
    char    s[MAXLINE];

    x = ((double)ptCurrent.x/(double)gManp->xdots) * gManp->mandel_width * gManp->AspectRatio + gManp->hor;
    y = ((double)(gManp->ydots - ptCurrent.y)/(double)gManp->ydots) * gManp->mandel_width + gManp->vert;
    SAFE_SPRINTF(s, "x=<%lf>,y=<%lf>", x, y);
    SetWindowText (hwnd, s);
    }

/**************************************************************************
    Dialog Box for Displaying Status
**************************************************************************/

#define	SHORTFIELDLENGTH	48
#define	GENERALFIELDLENGTH	100
#define	TEMPLENGTH		200
#define	PARAMLENGTH		600
#define	SUBTYPELENGTH		500
#define	FRACTALTYPELENGTH	1200

 INT_PTR CALLBACK StatusInfoDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    char	PassStr[GENERALFIELDLENGTH];
    char	FinishedStr[GENERALFIELDLENGTH];
    char	PrecisionStr[GENERALFIELDLENGTH];
    char	ParamStr[PARAMLENGTH];
    char	TempStr[TEMPLENGTH];
    char	FilterString[GENERALFIELDLENGTH];
    char	FractalType[FRACTALTYPELENGTH];
    char	SubData[SUBTYPELENGTH];
    char	ColourData[SHORTFIELDLENGTH];
    char	*PositionString = nullptr;
    char	*StatusString = nullptr;
    char	*s1 = nullptr;
    char	*s2 = nullptr;
    char	*s3 = nullptr;
    double	centrex, centrey;
    BigDouble	Big_centrex, Big_centrey;

    PositionString = new char[SIZEOF_BF_VARS * 3];
    if (PositionString == NULL)
	return TRUE;
    StatusString = new char[SIZEOF_BF_VARS * 4];			// times 3 because of x, y, width & additional part is for other info
    if (StatusString == NULL)
	return TRUE;

    StatusString[0] = '\0';
    StringBuilder sb(StatusString, SIZEOF_BF_VARS * 4);
    if (gManp->BigNumFlag)
	{
	Big_centrex = gManp->BigHor + (gManp->BigWidth * (gManp->AspectRatio / 2.0));
	Big_centrey = gManp->BigVert + (gManp->BigWidth / 2.0);
	s1 = new char[SIZEOF_BF_VARS];
	s2 = new char[SIZEOF_BF_VARS];
	s3 = new char[SIZEOF_BF_VARS];
	gManp->BigHor.ToString(s1, SIZEOF_BF_VARS, false);
	gManp->BigVert.ToString(s2, SIZEOF_BF_VARS, false);
	gManp->BigWidth.SafeSprintf(s3, SIZEOF_BF_VARS, "%.20Re");
	//	ConvertBignum2String(s1, Big_centrex.x);
	//	ConvertBignum2String(s2, Big_centrey.x);
	//	mpfr_sprintf(s3, "%.20Re", BigWidth.x);
	//	ConvertBignum2String(s3, BigWidth.x);
	_snprintf_s(PositionString, SIZEOF_BF_VARS * 3, _TRUNCATE, "X = %s\r\nY = %s\r\nWidth = %s", s1, s2, s3);
	if (s1) delete[] s1;
	if (s2) delete[] s2;
	if (s3) delete[] s3;
	}
    else
	{
	centrex = gManp->hor + (gManp->mandel_width * (gManp->AspectRatio / 2.0));
	centrey = gManp->vert + (gManp->mandel_width / 2.0);
	if (gManp->mandel_width > 0.000001)
	    _snprintf_s(PositionString, SIZEOF_BF_VARS * 3, _TRUNCATE, "X = %14.14f\r\nY = %14.14f\r\nWidth = %14.14f", centrex, centrey, gManp->mandel_width);
	else
	    _snprintf_s(PositionString, SIZEOF_BF_VARS * 3, _TRUNCATE, "X = %14.14f\r\nY = %14.14f\r\nWidth = %14e", centrex, centrey, gManp->mandel_width);
	}
    switch (gManp->calcmode)
	{
	case 'B':
	case 'T':
	case '1':
	    _snprintf_s(PassStr, GENERALFIELDLENGTH, _TRUNCATE, "Pass 1 of 1");
	    break;
	case '2':
	case 'G':
	case 'V':
	case 'H':
	    _snprintf_s(PassStr, GENERALFIELDLENGTH, _TRUNCATE, "Pass %d of %d", gManp->curpass, gManp->totpasses);
	    break;
	    //	default:
	    //	    SAFE_SPRINTF(PassStr, "Pass %d of %d", curpass, totpasses); 
	    //	    break;
	}
    _snprintf_s(FinishedStr, GENERALFIELDLENGTH, _TRUNCATE, "%s", (gManp->finished) ? ", Image Complete" : " ");
    if (gManp->BigNumFlag)	    // now we have double double and quad double...
	{
	if (gManp->precision <= DDPRECISION && fractalspecific[gManp->type].flags & USEDOUBLEDOUBLE)
	    _snprintf_s(PrecisionStr, GENERALFIELDLENGTH, _TRUNCATE, "DD Prec: %d", gManp->precision);
	else if (gManp->precision <= QDPRECISION && fractalspecific[gManp->type].flags & USEDOUBLEDOUBLE)
	    _snprintf_s(PrecisionStr, GENERALFIELDLENGTH, _TRUNCATE, "QD Prec: %d", gManp->precision);
	else
	    {
	    if (fractalspecific[gManp->type].flags & FRACTINTINPIXEL || fractalspecific[gManp->type].flags & TRIGINPIXEL)    // Bignum versions not yet available
		_snprintf_s(PrecisionStr, GENERALFIELDLENGTH, _TRUNCATE, "QD Prec: %d", gManp->precision);
	    else
		_snprintf_s(PrecisionStr, GENERALFIELDLENGTH, _TRUNCATE, "Arb Prec: %d", gManp->precision);
	    }
	}
    else
	_snprintf_s(PrecisionStr, GENERALFIELDLENGTH, _TRUNCATE, "Floating Point: %d", gManp->precision);

    gManp->RebuildFractalMetadata(gManp->type, gManp->subtype);		// load all the metadata for parameters
    if (gManp->type == PERTURBATION)
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Fractal: %s-%s, Subtype = %d", (gManp->EnableApproximation) ? "(Pert-BLA)" : "(Pert)", gManp->GetFractalName(), gManp->subtype);
    else if (gManp->type == SLOPEDERIVATIVE)
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Fractal: (Slope Der)-%s, Subtype = %d", gManp->GetFractalName(), gManp->subtype);
    else if (gManp->type == SLOPEFORWARDDIFF)
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Fractal: (Slope Fwd)-%s, Subtype = %d", gManp->GetFractalName(), gManp->subtype);
    else if (gManp->type == SCREENFORMULA || gManp->type == FORMULA || gManp->type == FFORMULA)
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Formula = <%s>\r\n", gManp->FormulaString);
    else if (gManp->type == LSYSTEM || gManp->type == FRACTPAR || gManp->type == IFS)
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Fractal: %s, Sub=%s", gManp->GetFractalName(), lsys_type);
    else if (gManp->type == TIERAZON || gManp->type == CROSSROADS || gManp->type == ZIGZAG || gManp->type == OSCILLATORS || gManp->type == FRACTALMAPS || gManp->type == SPROTTMAPS || gManp->type == SURFACES || gManp->type == KNOTS || gManp->type == CURVES)
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Fractal: %s, Sub=%d", gManp->GetFractalName(), gManp->subtype);
    else if (gManp->type == CUBIC)
	{
	char	ch = 0x20;
	if (gManp->subtype == 0)   ch = 'B';
	if (gManp->subtype == 1)   ch = 'C';
	if (gManp->subtype == 2)   ch = 'F';
	if (gManp->subtype == 3)   ch = 'K';
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Fractal: %s, Sub=C%cIN", gManp->GetFractalName(), ch);
	}
    else if (gManp->type == MALTHUS || gManp->type == TRIANGLES || gManp->type == GEOMETRY || gManp->type == CIRCLES || gManp->type == PASCALTRIANGLE)
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Fractal: %s, Sub=%c", gManp->GetFractalName(), gManp->subtype);
    else
	_snprintf_s(FractalType, FRACTALTYPELENGTH, _TRUNCATE, "Fractal: %s, Subtype = %d", gManp->GetFractalName(), gManp->subtype);

    SAFE_SPRINTF(ParamStr, "Params: ");
    for (int i = 0; i < NUMPARAM; i++)
	{
	if ((fabs(gManp->param[i]) < 0.00001 || fabs(gManp->param[i]) > 100000.0) && gManp->param[i] != 0.0)
	    _snprintf_s(TempStr, TEMPLENGTH, _TRUNCATE, "\r\n%d: %-12.9e  \t  %s", i, gManp->param[i], (gManp->Fractal.ParamName[i]) ? gManp->Fractal.ParamName[i] : "N/A");
	else
	    _snprintf_s(TempStr, TEMPLENGTH, _TRUNCATE, "\r\n%d: %-12.9f  \t  %s", i, gManp->param[i], (gManp->Fractal.ParamName[i]) ? gManp->Fractal.ParamName[i] : "N/A");
	SAFE_APPEND(ParamStr, PARAMLENGTH, TempStr);
	}
    sb.append("%s\r\nPixel [%d][%d], %s%s\r\n%s\r\nThreshold: %ld\r\nArith=%s\r\n%s\r\nBailout: %f\r\nImage Size: [%d][%d] ",
	FractalType, gManp->col, gManp->row, PassStr, FinishedStr, PositionString, gManp->threshold, PrecisionStr, ParamStr, gManp->rqlim, gManp->xdots, gManp->ydots);
    if (gManp->Fractal.NumFunct == 1)
	_snprintf_s(SubData, SUBTYPELENGTH, _TRUNCATE, "\r\nFn = %s", gManp->Fractal.Fn1);
    else if (gManp->Fractal.NumFunct == 2 && gManp->type != OSCILLATORS)			// we use NumFunct to display dimensions
	_snprintf_s(SubData, SUBTYPELENGTH, _TRUNCATE, "\r\nFn1 = %s,Fn2 = %s", gManp->Fractal.Fn1, gManp->Fractal.Fn2);
    else
	_snprintf_s(SubData, SUBTYPELENGTH, _TRUNCATE, " ");
    if (gManp->RotationAngle != 0)
	{
	_snprintf_s(TempStr, TEMPLENGTH, _TRUNCATE, "\r\nRotation Angle: %d\r\nRotation Centre: %lf, %lf\r\nMagnification: %le", gManp->RotationAngle, gManp->RotationCentre.x, gManp->RotationCentre.y, (gManp->BigNumFlag) ? 1.0 / (double)gManp->precision : 2.0 / gManp->mandel_width);
	sb.append("%s", TempStr);
	}

    if (gManp->juliaflag)
	{
	_snprintf_s(TempStr, TEMPLENGTH, _TRUNCATE, "\r\nJulia: %lf, %lf", gManp->j.x, gManp->j.y);
	sb.append("%s", TempStr);
	}

    if (gManp->type == PERTURBATION || gManp->type == SLOPEDERIVATIVE || gManp->type == SLOPEFORWARDDIFF)
	{
	sb.append("\r\nPerturbation and Slope Parameters-");
	if (gManp->type == SLOPEDERIVATIVE) gManp->SlopeType = DERIVSLOPE;
	if (gManp->type == SLOPEFORWARDDIFF) gManp->SlopeType = FWDDIFFSLOPE;
	sb.append("\r\nSlope Type: %d", gManp->SlopeType);
	if (gManp->type == PERTURBATION && &gManp->EnableApproximation)
	    sb.append("\r\nPerturbation uses BiLinear Approximation");
	sb.append("\r\nSlope Shadow Angle: %lf", gManp->lightDirectionDegrees);
	sb.append("\r\nSlope Shadow Strength: %lf", gManp->bumpMappingStrength);
	sb.append("\r\nSlope Shadow Depth: %lf", gManp->bumpMappingDepth);
	sb.append("\r\nPalette Start: %d", gManp->PaletteStart);
	sb.append("\r\nSlope Light Height: %lf", gManp->LightHeight);
	sb.append("\r\nColour Offset: %d", gManp->PalOffset);
	sb.append("\r\nIteration Divisor: %lf", gManp->IterDiv);
	sb.append("\r\nPert Colour Method: %d", gManp->PertColourMethod);
	}
    if (gManp->IsPAR)
	{
	sb.append("\r\nPAR file=");
	sb.append("%s", PARFile);
	}
    else if (gManp->IsKFR)
	{
	sb.append("\r\nKFR file=");
	sb.append("%s", KFRFile);
	}
    else if (WasFractPar)
	{
	sb.append("\r\nFractPAR file=");
	sb.append("%s", FracPARFile);
	sb.append("\r\nItemName=");
	sb.append("%s", lptr[lsys_ptr]);
	}
    else if (gManp->type == IFS)
	{
	sb.append("\r\nIFS file=");
	sb.append("%s", IFSFile);
	sb.append("\r\nItemName=");
	sb.append("%s", lptr[lsys_ptr]);
	}
    else if (gManp->type == LSYSTEM)
	{
	sb.append("\r\nL System file=");
	sb.append("%s", LSYSFile);
	sb.append("\r\nItemName=");
	sb.append("%s", lptr[lsys_ptr]);
	}
    else if (gManp->type == FORMULA)
	{
	sb.append("\r\nFormula file=");
	sb.append("%s", FRMFile);
	sb.append("\r\nItemName=");
	sb.append("%s", lptr[lsys_ptr]);
	}
    //    DialogBox(hInst, "StatusInfoDlg", hwnd, StatusInfoDlg);
//    MessageBox (hwnd, StatusString, fractalspecific[type].name, MB_ICONEXCLAMATION | MB_OK);
    if (gManp->TrueCol.IsMAPFile)
	{
	sb.append("\r\nMAP file=");
	sb.append("%s", MAPFile);
	}
    if (gManp->InsideMethod > NONE || gManp->OutsideMethod > NONE)
	{
	if (gManp->OutsideMethod > TIERAZONCOLOURS)
	    _snprintf_s(FilterString, GENERALFIELDLENGTH, _TRUNCATE, "\r\nTZColour = %d, ", gManp->ColourMethod);
	else if (gManp->OutsideMethod > TIERAZONFILTERS)
	    _snprintf_s(FilterString, GENERALFIELDLENGTH, _TRUNCATE, "\r\nTZFilter = %d, Fractal Dimension Options = %d, ", gManp->FilterType, gManp->nFDOption);
	else if (gManp->InsideMethod > NONE)
	    _snprintf_s(FilterString, GENERALFIELDLENGTH, _TRUNCATE, "\r\nFilter=%d, ", gManp->InsideMethod);
	else
	    _snprintf_s(FilterString, GENERALFIELDLENGTH, _TRUNCATE, "\r\nFilter=%d, ", gManp->OutsideMethod);
	sb.append("%s", FilterString);
	}
    if (gManp->ColourSpeed != 0.0)		// used for colour smoothing
	{
	_snprintf_s(ColourData, SHORTFIELDLENGTH, _TRUNCATE, "\r\nColour Speed for Smoothing=%lf, ", gManp->ColourSpeed);
	sb.append("%s", ColourData);
	}

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemText(hDlg, IDC_STATUSINFO, StatusString);
	    return TRUE;

	case WM_COMMAND:
	    switch (wParam)
		{
		case IDOK:
		    if (StatusString)
			delete[] StatusString;
		    if (PositionString)
			delete[] PositionString;
		    EndDialog(hDlg, TRUE);
		    return TRUE;
		case IDCANCEL:
		    if (StatusString)
			delete[] StatusString;
		    if (PositionString)
			delete[] PositionString;
		    EndDialog(hDlg, TRUE);
		    return TRUE;
		}
	    break;
	}
    return FALSE;
    }

