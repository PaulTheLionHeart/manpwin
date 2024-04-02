/*-----------------------------------------
	Paul's Fractal Generator
	Script.cpp - argument and script interpreter
  -----------------------------------------*/

#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include "big.h"
#include <math.h>
#include "manpwin.h"
#include "manp.h"
#include "anim.h"
#include "slope.h"
#include "fractype.h"
#include "fractalp.h"
#include "resource.h"
#include "colour.h"
#include "complex.h"
#include "OscProcess.h"
#include "plot.h"
#include "pixel.h"

#define	TWO_PI		6.28318530717958
#define	ENDOFSCRIPT	1

extern	int	mainview(HWND, BOOL);
extern	char	*trailing(char *);
extern	int	GetParamData(HWND, LPSTR, LPSTR, LPSTR, BOOL);
extern	void	setup_defaults(void);
extern	void	init_log(HWND), init_sym(void), /*init3d(void), */NewtonSetup(void), 
		/*init_stereo_pairs(void), */DisplayPalette(HWND, BOOL);
//extern	int	ComplexNewtonSetup(void);
extern	int	write_png_file(HWND, char *, char *, char *); 
extern	int	read_png_file(HWND hwnd, char *infile);
extern	void	ClearScreen(void);
extern	void	DisplayFractal(HWND);
extern	int	perform_worklist(HWND);
extern	void	InitTrueColourPalette(BYTE);
extern	int	user_data(HWND);
extern	char	*ShowTime (double);
//extern	short	FilePalette(HWND, char *, char *);
	int	RunFourierScript(HWND, char *, char *);
extern	void	InitFourier(char);
extern	int	FourierStep(HWND, char *, int, int, CPlot);
extern	void	LoadAnimationFrame(char *, char *, int, double, BOOL, BOOL);
extern	void	AnimateInit(int);
extern	int	AnimateClose(void);
extern	char	*FractData(void);
extern	int	ProcessFormulaString(char *);
extern	char	*str_find_ci(char *, char *);
//extern	void	LoadOscillatorParams(void);
//extern	void	LoadKnotParams(void);
//extern	void	LoadFractalMapParams(void);
//extern	void	LoadCurveParams(void);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);
extern	DLGPROC	PNGFileOpenDlg(HWND, LPSTR, LPSTR);
extern	void	DoCaption(HWND, char *);

extern	BYTE	BigNumFlag;		// True if bignum used
extern	double	hor;			/* horizontal address */
extern	double	vert;			/* vertical address */
extern	double	mandel_width;		/* width of display */
extern	BYTE	_3dflag;		/* replay saved file. 3 = 3D */
extern	int	logval;			/* log colour map starting value */
extern	WORD	type;			/* M=mand, N=Newton etc */
extern	BYTE	pairflag;		/* stereo pair */
extern	BOOL	AutoSaveFlag;
extern	int	number;				/* used for compression */
//extern	short	num_worklist;		/* resume worklist for standard engine */
//extern	int	workpass;		/* for 1/2 pass type tracing */
extern	long	threshold;
extern	int	time_to_restart;	// time to restart?
extern	int	time_to_reinit;		// time to reinitialize?
extern	int	time_to_quit;		// time to quit?
extern	int	time_to_load;		// time to load file?
extern	int	time_to_load;		// time to load file?
extern	int	time_to_break;		// time to break out of animation?
extern	int	finished;		/* all passes complete */
extern	BOOL	StartImmediately;	// immediate start of animation generation
//extern	BYTE	palette_flag;		// set palette
extern	char	MAPFile[];		// colour map file
extern	char	MPGPath[];		// path for MPEG files
extern	char	MPGFile[];		// MPEG file
extern	char	PNGName[];		// base name for PNG file sequence or MPG filename
extern	char	ANIMPNGPath[];		// path for animated PNG files and LST files
extern	char	PNGFile[];		// PNG file
extern	int	subtype;
extern	WORD	steps;			// for Fourier Analysis
extern	BYTE	cycleflag;		// do colour cycling
//extern	BYTE	*PalettePtr;		// points to true colour palette
extern	CTrueCol    TrueCol;		// palette info
extern	BOOL	WritePNGFrames;		// write frames to PNG files
extern	BOOL	WriteMemFrames;		// write frames to memory
extern	BOOL	WritePNGList;		// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;	// write frames directly to an MPEG file

extern	double	param[];
extern	char	FormulaString[];	// used to hold the full formula
extern	char	*FormulaStringPointer;	// declared in RTJulia so we can create a julia formula
extern	double	rqlim;			// bailout level
extern	int	MaxDimensions;
extern	double	iterations;
extern	int	AutoStereo_value;	// AutoStereo depth value
extern	int	PaletteShift;		// fractional palette addressing
//extern	BOOL	DisplayAxisImages;	// display all axes in a single image
extern	BOOL	DisplayAxisLabels;		// show labels for axis pairs

extern	double	lightDirectionDegrees;
extern	double	bumpMappingDepth;
extern	double	bumpMappingStrength;
extern	int	PaletteStart;

extern	int	invert;
//extern	int	symmetry;
extern	double	f_radius,f_xcenter,f_ycenter;    // inversion radius, center 

extern	PAINTSTRUCT 	ps;
extern	RECT 		r;

extern	double	x_rot;			// angle display plane to x axis
extern	double	y_rot;			// angle display plane to y axis
extern	double	z_rot;			// angle display plane to z axis
extern	double	sclx, scly, sclz;	// scale

extern	int	height, xdots, ydots, width, bits_per_pixel;

extern	ProcessType	OscAnimProc;
extern	AnimStruct	ANIM[];		// holds all the date for each animation frame
extern	CSlope		Slope[];
extern	CTrueCol	TrueCol;	// palette info

extern	CPlot		Plot;		// image plotting routines 
extern	CPixel		Pix;		// routines for escape fractals
extern	HWND		GlobalHwnd;	// This is the main windows handle
extern	char		szStatus[];	// status bar text
extern	CDib	Dib;
extern	Complex	j;

PAINTSTRUCT 	StatusBar;
RECT 		StatusBarRect;
BOOL		RunAnimation = FALSE;		// are we in the middle of an animation run?
BOOL		SuspendAnimation = FALSE;	// pause animation run?
//BYTE		ScriptPaletteFlag = FALSE;	// Display palette in animation frames
	int	StartFrame = 0;			// allow us to resume animation run
	DWORD	StatusColour = 0x0000FFFF;	// colour of status bar
//	time_t	FrameEnd = 0;
//	time_t	FrameStart = 0;
struct __timeb64 	FrameEnd;
struct __timeb64 	FrameStart;

static	double	AverageTime = 0.0;
static	double	TotalTime = 0.0;
static	double	FinalTime = 0.0;
static	double	RemainingTime = 0.0;
static	double	StartRate, EndRate;		// population growth rates for Malthus fractal
//static	BYTE	*LocalPalettePtr;		// points local palette to store script palette
static	char    szTitleName[MAXDATALINE] = "ManpWIN";

static	FILE	*fp;				// script file   
static	FILE	*fl;				// a list of files names for creating a large MPEG file
static	int	StartIter, EndIter, CurrentFrame, frames, ParamNumber;
static	BOOL	MathusRateAnimation = FALSE;
static	BOOL	IkedaAnimation = FALSE;
static	BOOL	ParamAnimation = FALSE;
static	BOOL	OscillatorAnimation = FALSE;
static	BOOL	JuliaSetAnimation = FALSE;
static	BOOL	InversionSetAnimation = FALSE;
static	double	divisor;
extern	BOOL	Return2Start;					// flag return to reverse sweep

	char	SaveFileOrig[MAXLINE];				// SaveAs filename base name
	char	FilenameList[MAXLINE];				// filename for list of animation frame filenames

	void	StatusBarAnimInfo (int, int);
extern	void	ConvertASCII2RGB(BYTE *, BYTE *, BYTE *, char *);
extern	void	UpdateInit(void);
extern	int	UpdateClose(void);
extern	void	DisplayStatusBarInfo(int, char *);

extern	COscProcess	OscProcess;

/*------------------------------------------
   Generate  MPG filename
  ------------------------------------------*/

char	*GenerateMPEGFileName (char *MPEGPath, char *infile)

    {
    char		drive[_MAX_DRIVE], dir[_MAX_DIR], name[_MAX_FNAME], ext[_MAX_EXT];
    static	char	MPGName[MAX_PATH];

    _splitpath(infile, drive, dir, name, ext);				// remove extension
    sprintf(MPGName, TEXT("%s\\%s.mpg"), MPEGPath, name);
    return MPGName;	
    }

/*------------------------------------------
   Generate  animation filename
  ------------------------------------------*/

char	*GenerateAnimFileName (char *AnimPath, char *infile)

    {
    char		drive[_MAX_DRIVE], dir[_MAX_DIR], name[_MAX_FNAME], ext[_MAX_EXT];
    static	char	AnimName[MAX_PATH];

    _splitpath(infile, drive, dir, name, ext);				// remove extension
    sprintf(AnimName, TEXT("%s\\%s"), AnimPath, name);
    return AnimName;	
    }

/*------------------------------------------
   Get the total number of frames
  ------------------------------------------*/

double	GetNumFrames(void)
    {
    return (double)frames;
    }

/*------------------------------------------
   if MPEG direct fails
  ------------------------------------------*/

void	CloseScript(void)
    {
    fclose(fp);
    }

/*------------------------------------------
   Generate  actual fractal
  ------------------------------------------*/

int	GenerateFractalFrame(HWND hwnd, char *FileName, int TotalFrames, int ThisFrame)
    {
    double	FramesCubed, iCubed;
    HCURSOR	hCursor;
    char	s[MAXLINE];
    char	buf[MAXDATALINE]; 

    BigNumFlag = FALSE;					// we are starting at the shallow end of the pool
    user_data(hwnd);
    if (time_to_quit)
	{
	AutoSaveFlag = FALSE;
	fclose(fp);
	return -1;
	}

    if (type != MALTHUS || !MathusRateAnimation)	// already have all the information we need
	if (type != FRACTALMAPS || !IkedaAnimation)	// ditto
	    if (!ParamAnimation)
		{
		if (fgets(buf, MAXDATALINE, fp) == NULL) 
		    return ENDOFSCRIPT;
		if (GetParamData(hwnd, FileName, buf, SaveFileOrig, FALSE) < 0)
		    return -1;
		FramesCubed = (double)TotalFrames * (double)TotalFrames * (double)TotalFrames;		// use cubic fn
		iCubed = (double)ThisFrame * (double)ThisFrame * (double)ThisFrame; 
		threshold = StartIter + (int)(((double)(EndIter - StartIter + 1) * iCubed) / FramesCubed);
		}
    hCursor = LoadCursor(NULL, IDC_WAIT);	// Load hour-glass cursor.
    SetCursor(hCursor);

    if (logval)
	init_log(hwnd);				// log colour distribution
    if (_3dflag)
	Pix.init3d(xdots, ydots, x_rot, y_rot, z_rot, sclx, scly, sclz, threshold, hor, vert);				// init 3D parameters 
					// init 3D parameters 
//    if (type == NEWTON || type == NEWTBASIN)
//	NewtonSetup();
//    if (type == COMPLEXNEWTON || type == COMPLEXBASIN)
//	ComplexNewtonSetup();
    if (pairflag)
	Pix.init_stereo_pairs(pairflag, &AutoStereo_value);			// init stereo pair parameters
    init_sym();					// symmetry? set up params
    ClearScreen();
    hCursor = LoadCursor(NULL, IDC_ARROW);	// Load normal cursor.
    SetCursor(hCursor);			        // Reload arrow cursor.
    DisplayFractal(hwnd);
//    num_worklist = 0;
//    number = 0;				// char count for screen
//    workpass = 0;
    finished = FALSE;
    if (perform_worklist(hwnd) < 0)
	{
	wsprintf(s, "Can't calculate fractal in script file: %s", FileName);
	MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}
    return 0;
    }

int	GenerateFractal(HWND hwnd, char *FileName, char *MoreInfo, int TotalFrames, int ThisFrame)
    {
    double *delay;
    delay = &(ANIM[ThisFrame].DelayMultiplier);
    if (OscAnimProc == MORPHING)
	return (OscProcess.MorphStep(hwnd, FileName, MoreInfo, TotalFrames, ThisFrame, delay, threshold));
    else if (type == FOURIER)
	{
	return (FourierStep(hwnd, FileName, TotalFrames, ThisFrame, Plot));
	}
    else
	return (GenerateFractalFrame(hwnd, FileName, TotalFrames, ThisFrame));
    }

/*-----------------------------------------
	Get PNG sequence from script file
  -----------------------------------------*/

int	GetPNGSeqFromScript(HWND hwnd, char *FileName)

    {
    char	buf[MAXDATALINE];
    char	s[MAXLINE];

    if ((fp = fopen(FileName, "r")) == NULL)
	{
	wsprintf(s, "Can't open script file: %s for read", FileName);
	MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	return -1;
	}

    if (fgets(buf, MAXDATALINE, fp) == NULL)				// get common params
	{
	wsprintf(s, "Can't read parameters in script file: %s", FileName);
	MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	fclose(fp);
	return -1;
	}
    else
	{
	if (GetParamData(hwnd, FileName, buf, PNGName, FALSE) < 0)
	    return -1;
	}
    fclose(fp);
    return 0;
    }

/*-----------------------------------------
   Allow palette to move in zoom animations
  -----------------------------------------*/

void	MovePalette(CTrueCol *TrueCol, int PaletteShift, int threshold)
    {
    int		i, j, AbsPalShift;
    BYTE	TempPal[3];
    long	thresh = TrueCol->FinalThreshold;

    if (PaletteShift == 0)
	return;

    AbsPalShift = abs(PaletteShift);
    for (j = 0; j < AbsPalShift; j++)
	{
	if (PaletteShift > 0)
	    {
	    memcpy(TempPal, &TrueCol->PalettePtr[(thresh - 1) * 3], 3);		// load last entry for 'ron
	    for (i = thresh - 2; i >= 0; i--)
		memcpy(&TrueCol->PalettePtr[(i + 1) * 3], &TrueCol->PalettePtr[(i) * 3], 3);
	    memcpy(TrueCol->PalettePtr, TempPal, 3);				// restore last entry to the beginning
	    }
	else
	    {
	    memcpy(TempPal, TrueCol->PalettePtr, 3);				// load first entry for 'ron
	    for (i = 0; i < thresh - 1; i++)
		memcpy(&TrueCol->PalettePtr[i * 3], &TrueCol->PalettePtr[(i + 1) * 3], 3);
	    memcpy(&TrueCol->PalettePtr[(thresh - 1) * 3], TempPal, 3);		// restore first entry at the end
	    }
	}
    }

/*-----------------------------------------
	Init script file
  -----------------------------------------*/

int	InitScript(HWND hwnd, char *FileName, int *TotalFrames)

    {
    char	buf[MAXDATALINE]; 
    char	buf1[MAXDATALINE]; 
    char	s[MAXLINE];
    int		i, j;
    char	ascii[6];
    BOOL	eof = FALSE;
    int		ch, UpdateFrames;
    BOOL	RotateAnim = TRUE;
    char	*p;

    setup_defaults();
    MathusRateAnimation = FALSE;
    IkedaAnimation = FALSE;
    OscillatorAnimation = FALSE;
    ParamAnimation = FALSE;
    JuliaSetAnimation = FALSE;
    InversionSetAnimation = FALSE;
//    EndIter =  threshold;
    if ((fp = fopen(FileName, "r")) == NULL)
	{
	wsprintf(s, "Can't open script file: %s for read", FileName);
	MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    if (fgets(buf, MAXDATALINE, fp) == NULL)				// get common params
	{
	wsprintf(s, "Can't read parameters in script file: %s", FileName);
	MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	fclose(fp);
	return -1;
	}
    else 
	{
//	if (strcmp("Default Colour Map", MAPFile) == 0)
//	    palette_flag = FALSE;
	strcpy(buf1, buf);
	if (GetParamData(hwnd, FileName, buf, SaveFileOrig, FALSE) < 0)
	    return -1;
	time_to_restart = FALSE;
	}
    StartIter = EndIter = threshold;					// get first iteration count from file
									// now get co-ords for each image
    if (fgets(buf, MAXDATALINE, fp) == NULL)				// get common params
	{
	wsprintf(s, "Can't read parameters in script file: %s", FileName);
	MessageBox (hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	fclose(fp);
	return -1;
	}
    if (strncmp(buf, "Malthus Animation:", 18) == 0)
	{
	sscanf(buf + 19, "%lf %lf %d", &StartRate, &EndRate, &frames);  
	MathusRateAnimation = TRUE;
	}
    else if (strncmp(buf, "Ikeda Animation:", 16) == 0)
	{
	sscanf(buf + 17, "%lf %lf %d", &StartRate, &EndRate, &frames);  
	IkedaAnimation = TRUE;
	}
    else if (strncmp(buf, "Parameter Animation:", 20) == 0)
	{
	sscanf(buf + 20, "%lf %lf %d %d %d", &StartRate, &EndRate, &frames, &ParamNumber, &Return2Start);  
	ParamAnimation = TRUE;
	}
    else if (strncmp(buf, "Oscillator", 10) == 0)
	{
	if (strncmp(buf, "Oscillator Morphing:", 19) == 0)
	    RotateAnim = MORPH;
	else if (strncmp(buf, "Oscillator Rotation:", 19) == 0)
	    RotateAnim = ROTATION;
	else
	    RotateAnim = EVOLVE;
	sscanf(buf + 22, "%d %d %d", &EndIter, &EndIter, &frames);  
	OscillatorAnimation = TRUE;
	if (RotateAnim == ROTATION)
	    OscAnimProc = INITANIM;
	if (fractalspecific[type].calctype() < 0)
//	if (GenerateFractal(hwnd, FileName, frames, CurrentFrame) < 0)
	    {
	    if (WritePNGFrames)
		fclose(fl);
	    fclose(fp);
	    AnimateClose();
	    return -1;
	    }
	switch (RotateAnim)
	    {
	    case ROTATION:
		OscAnimProc = RUNANIM;
		break;
	    case EVOLVE:
		OscAnimProc = EVOLUTION;
		break;
	    case MORPH:
		OscAnimProc = MORPHING;
		break;
	    }
//	OscAnimProc = (RotateAnim) ? RUNANIM : EVOLUTION;
	}
    else
	{
	p = buf;
	if (strncmp(buf, "Julia Set Animation:", 20) == 0)
	    {
	    p = buf + 20;
	    JuliaSetAnimation = TRUE;
	    }
	if (strncmp(buf, "Inversion Animation:", 20) == 0)
	    {
	    p = buf + 20;
	    InversionSetAnimation = TRUE;
	    }
	sscanf(p, "%d %d %d %d", &EndIter, &frames, (int *)&TrueCol.ScriptPaletteFlag, &PaletteShift);
	TrueCol.FinalThreshold = EndIter;
	MathusRateAnimation = FALSE;
	IkedaAnimation = FALSE;
	OscillatorAnimation = FALSE;
	ParamAnimation = FALSE;
	}
    *TotalFrames = frames;						// only needed for direct creation of MPEG
    if (fgets(buf, MAXLINE, fp) == NULL)				// no palette
	InitTrueColourPalette(FALSE);					// process anything we found in the param list, eg palette file etc
    else			
	{
	if (strncmp(buf, "Palette=", 8) == 0)				// we have a palette file
	    {
	    for (i = 0; i < EndIter; i++)
		{
		for (j = 0; j < 4; j++)
		    {
		    ch = fgetc(fp);
		    if (ch == '\n')					// newline
			ch = fgetc(fp);
		    if (ch == EOF)
			{
			eof = TRUE;
			break;
			}
		    ascii[j] = ch;
		    }
		ascii[4] = '\0';
		if (eof)
		    break;
		if (i < MAXTHRESHOLD)
		    ConvertASCII2RGB((TrueCol.PalettePtr + i * 3 + 0), (TrueCol.PalettePtr + i * 3 + 1), (TrueCol.PalettePtr + i * 3 + 2), ascii);
		}
	    }
	}

    sprintf(szStatus, "Starting Animation Run at frame [%d]", StartFrame);
    if (StartFrame > 0)
	{
	int LocalPaletteShift = (PaletteShift * StartFrame) % EndIter;		// if we are using palette shift and starting frame != 0, we need to shift palette in order to match previous frames
	MovePalette(&TrueCol, LocalPaletteShift, EndIter);
	}
    _ftime64(&FrameStart);					// initialise time counter
    AverageTime = 0.0;
    TotalTime = 0.0;
    FinalTime = 0.0;
    RemainingTime = 0.0;
    if (StartImmediately)
	StartFrame = 0;
    RunAnimation = TRUE;
    time_to_break = FALSE;
    if (type == FOURIER)
	{
//	RunFourierScript(hwnd, FileName, buf1);
//	fclose(fp);
	ClearScreen();
	InitFourier(subtype);
	}
    if (OscAnimProc == MORPHING)
	{
	ClearScreen();
/*
	switch (type)
	    {
	    case OSCILLATORS:
		LoadOscillatorParams();
		break;
	    case KNOTS:
		LoadKnotParams();
		break;
	    case FRACTALMAPS:
		LoadFractalMapParams();
		break;
	    case CURVES:
		LoadCurveParams();
		break;
	    }
*/
	if (OscProcess.InitMorphing(MaxDimensions, (long)iterations, &UpdateFrames, type, OscAnimProc, frames, &Dib/*, OscProcess.DisplayAxisImages*/) < 0)
	    MessageBox (hwnd, "Oscillator Morphing failed because of Memory Unavailable", "ManpWin", MB_ICONEXCLAMATION | MB_OK);
	StatusColour = 0x0000FFFF;				// colour of status bar
	frames = UpdateFrames;					// round the number of frames to 
	}
    return 0;
    }


/*-----------------------------------------
    Axis Status Bar generator
     -----------------------------------------*/

void	OutputAxesStatus(HWND hwnd, HDC	hdc, int HorOffset, int VertOffset, int NumRows, int NumColumns, char *text)
    {
    RECT    AxisRect;
    LOGFONT lf;
    SelectObject(hdc, CreateFontIndirect(&lf));

//    HBRUSH  hbr;         // handle to brush
//    SetBkColor(ps.hdc, 0L);
//    SetTextColor(ps.hdc, 0x00ffffff);
//    BeginPaint(hwnd, &ps);
//    hbr = CreateSolidBrush(0xffffff);
/*				    // 3D
    AxisRect.left = HorOffset + (xdots * 2) / 25;
    AxisRect.top = VertOffset + ydots / 4 - GetSystemMetrics(SM_CYMENU);// make status same size as menu
    AxisRect.right = HorOffset + (xdots * 3) / 25;
    AxisRect.bottom = VertOffset + ydots / 4;
*/
    AxisRect.left = HorOffset + (xdots * 2) / (NumColumns * 5);
    AxisRect.top = VertOffset + ydots / 10;
    AxisRect.right = AxisRect.left + xdots / (2 * NumColumns);
    AxisRect.bottom = AxisRect.top + 40;			// allow 40 pixels height for font

    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfWidth = 0;
    lf.lfItalic = 0;
    lf.lfUnderline = 0;
    lf.lfStrikeOut = 0;
    lf.lfOrientation = 0;
    lf.lfWeight = FW_BOLD;

//    FillRect(ps.hdc, &AxisRect, hbr);
//    TextOut(ps.hdc, 8, AxisRect.top + 2, szStatus, (int)strlen(szStatus));
    Dib.Text2Dib(hdc, &AxisRect, 0x00ffffff, 0L, &lf, TRANSPARENT, text);
//    DrawText(hdc, text, (int)strlen(text), &AxisRect, DT_CENTER);
//    DeleteObject(hbr);
    }

void	ShowAxisLabels(HWND hwnd)
    {
    char    labels[] = "xyzuvwpqrs";
    char    a[12];
    char    b[12];
    char    text[24];
    int	    NumAxes = OscProcess.axes;

    int NumColumns = NumAxes - 1;
    int NumRows = NumAxes / 2;
//    HDC	hdc = GetDC(hwnd);

    if (OscProcess.DisplayAxisImages)
	{
	/*
		for (int i = 0; i < 15; i++)
		    {
		    int HorOffset = (i % 5) * xdots / 5 + 5;
		    int VertOffset = (i / 5) * ydots / 3 + 5;
		    OutputAxesStatus(GlobalHwnd, HorOffset, VertOffset, ANIM[i].FrameInfo);
		    //	TextOut(ps.hdc, HorOffset, VertOffset, ANIM[i].FrameInfo, (int)strlen(ANIM[i].FrameInfo));
		    }
	*/
	BeginPaint(hwnd, &ps);
	for (int i = 0; i < NumRows * NumColumns; i++)
	    {
	    int HorOffset = (i % NumColumns) * xdots / NumColumns + 5;
	    int VertOffset = (i / NumColumns) * ydots / NumRows + 5;
	    if (OscProcess.In1[i] < NumAxes / 2)
		{
		*a = *(labels + OscProcess.In1[i]);
		*(a + 1) = '\0';
		}
	    else
		{
		*a = *(labels + OscProcess.In1[i] - NumAxes / 2);
		*(a + 1) = 'n';
		*(a + 2) = '\0';
		}
	    if (OscProcess.In2[i] < NumAxes / 2)
		{
		*b = *(labels + OscProcess.In2[i]);
		*(b + 1) = '\0';
		}
	    else
		{
		*b = *(labels + OscProcess.In2[i] - NumAxes / 2);
		*(b + 1) = 'n';
		*(b + 2) = '\0';
		}
	    sprintf(text, "%s-%s", a, b);
	    OutputAxesStatus(GlobalHwnd, ps.hdc, HorOffset, VertOffset, NumRows, NumColumns, text);
//	    OutputAxesStatus(GlobalHwnd, HorOffset, VertOffset, ANIM[i].FrameInfo);
//	    TextOut(ps.hdc, HorOffset, VertOffset, ANIM[i].FrameInfo, (int)strlen(ANIM[i].FrameInfo));
	    }
	EndPaint(hwnd, &ps);
	InvalidateRect(GlobalHwnd, &r, FALSE);
	//	DisplayAxisImages = FALSE;
	}
//    ReleaseDC(hwnd, hdc);
    }


/*-----------------------------------------
	Finish script file
  -----------------------------------------*/

void	EndScript(int ThisFrame)
    {
//    if (LocalPalettePtr)
//	delete [] LocalPalettePtr;
    StatusColour = 0x0000FF00;					// colour of status bar
    sprintf(szStatus, "Animation Run Completed %d frames in %s", ThisFrame, ShowTime ((long)TotalTime));
    AutoSaveFlag = FALSE;					// all done
    time_to_restart = FALSE;
//    time_to_restart = TRUE;
    time_to_break = FALSE;
    OscAnimProc = STANDARD;
    if (!OscProcess.DisplayAxisImages)
	return;

    Dib.ClearDib(0, 0, 0);	// set background colour
//    SetWindowText(GlobalHwnd, "Calculating Axis Images");
//    DisplayStatusBarInfo(INCOMPLETE, "Calculating Axis Images");		// display status bar

    DWORD   local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
    DWORD   ptr;
    BYTE    colour;
//    int	    HorOffset, VertOffset;

    for (int i = 0; i < frames; i++)
	{
	for (int j = 0; j < ydots; j++)
	    for (int k = 0; k < xdots; k++)
		{
		ptr = ((DWORD)(height - 1 - j) * (DWORD)(local_width + 3 - ((local_width - 1) % 4)) + (DWORD)(k * 3));
		for (int m = 0; m < 3; m++)
		    {
		    colour = *(ANIM[i].animDIB.DibPixels + ptr + m);
	    	    if (colour != 0)
			*(Dib.DibPixels + ptr + m) = colour;
		    }
		}
	}
//    ShowAxisLabels(GlobalHwnd);
//    DisplayStatusBarInfo(COMPLETE, PARFile);			// display status bar
/*
    BeginPaint(GlobalHwnd, &ps);
    for (int i = 0; i < 15; i++)
	{
	HorOffset = (i % 5) * xdots / 5 + 5;
	VertOffset = (i / 5) * ydots / 3 + 5;
	OutputAxesStatus(GlobalHwnd, HorOffset, VertOffset, ANIM[i].FrameInfo);
//	TextOut(ps.hdc, HorOffset, VertOffset, ANIM[i].FrameInfo, (int)strlen(ANIM[i].FrameInfo));
	}
    EndPaint(GlobalHwnd, &ps);
*/
    time_to_break = TRUE;
    RunAnimation = FALSE;
    AnimateClose();
    finished = TRUE;
    UpdateClose();
    if (DisplayAxisLabels)
	ShowAxisLabels(GlobalHwnd);
    DoCaption(GlobalHwnd, (type == OSCILLATORS) ? OscillatorSpecific[subtype].name : FractalMapSpecific[subtype].name);
    //    SetWindowText(GlobalHwnd, "Completed Calculating Axis Images");

//    DisplayStatusBarInfo(COMPLETE, "Completed Calculating Axis Images");		// display status bar
//    DisplayStatusBarInfo(COMPLETE, "Splat");		// display status bar
//    InvalidateRect(GlobalHwnd, &r, FALSE);

/*
    BeginPaint(GlobalHwnd, &ps);
    for (int i = 0; i < 15; i++)
	{
	HorOffset = (i % 5) * xdots / 5 + 5;
	VertOffset = (i / 5) * ydots / 3 + 5;
	OutputAxesStatus(GlobalHwnd, HorOffset, VertOffset, ANIM[i].FrameInfo);
	//	TextOut(ps.hdc, HorOffset, VertOffset, ANIM[i].FrameInfo, (int)strlen(ANIM[i].FrameInfo));
	}
    EndPaint(GlobalHwnd, &ps);

//    Sleep(500);
*/
    }

/*-----------------------------------------------------------------------------------
    These two functions taken out of RunScript() so they can be accessed within 
    MPEGWrite.cpp to update frames for variable parameter type animations
-----------------------------------------------------------------------------------*/

void	UpdateAnimParamValues(void)
    {
    static  int	count = 0;
    if (!MathusRateAnimation && !IkedaAnimation && !ParamAnimation)	// if we got here by mistake, let's get outa here
	return;
    if (MathusRateAnimation)
	{
	ParamNumber = 0;
	param[0] += divisor;
	}
    if (IkedaAnimation)
	{
	ParamNumber = 1;
	param[1] += divisor;
	}
    if (ParamAnimation)
	param[ParamNumber] += divisor;
    if (type == PERTURBATION && ParamNumber > 9)
	{
	switch (ParamNumber)
	    {
	    case 10:	// can't animate Slope Type
		break;
	    case 11:
		lightDirectionDegrees += divisor;
		break;
	    case 12:
		bumpMappingStrength += divisor;
		break;
	    case 13:
		bumpMappingDepth += divisor;
		break;
	    case 14:
		PaletteStart = (int)(StartRate + divisor * count++);
		break;
	    case 15:
		rqlim += divisor;
		break;
	    }
	}
    }

void	InitAnimParamValues(void)
    {
    if (!MathusRateAnimation && !IkedaAnimation && !ParamAnimation)	// if we got here by mistake, let's get outa here
	return;
    if (type == MALTHUS && MathusRateAnimation)
	{
	divisor = (EndRate - StartRate) / frames;
	param[0] = StartRate;
	}
    else if (type == FRACTALMAPS && IkedaAnimation)
	{
	divisor = (EndRate - StartRate) / frames;
	param[1] = StartRate;
	}
    else if (ParamAnimation)
	{
	divisor = (EndRate - StartRate) / frames;
	if (type == PERTURBATION && ParamNumber > 9)
	    {
	    switch (ParamNumber)
		{
		case 10:	// can't animate Slope Type
		    break;
		case 11:	
		    lightDirectionDegrees = StartRate;
		    break;
		case 12:	
		    bumpMappingStrength = StartRate;
		    break;
		case 13:	
		    bumpMappingDepth = StartRate;
		    break;
		case 14:	
		    PaletteStart = (int)StartRate;
		    break;
		case 15:
		    rqlim = StartRate;
		    if (rqlim < 1.0)
			rqlim = StartRate = 1.0;
		    break;
		}
	    }
	}
    param[ParamNumber] = StartRate;
    }

/*-----------------------------------------
	Build Julia Set formula string
  -----------------------------------------*/

void	BuildJuliaFormulaString(void)
    {
    char	TempFormula[1200];
    char	*ptr;

    if (param[4] != 0.0)
	return;								// we want to keep the initial conditions
    strcpy(TempFormula, FormulaString);
    ptr = str_find_ci(TempFormula, "pixel:");
    sprintf(FormulaString, "z = pixel:%s", ptr);			// Julia
    }

/*-----------------------------------------
	Parse script file
  -----------------------------------------*/

int	RunScript(HWND hwnd, char *FileName)

    {   
    char	buf[MAXDATALINE];
    char	s[MAXLINE];
    char	MoreInfo[MAXLINE];
    static	char	SaveFileName[MAXLINE];				// individual SaveAs filename
    static	char	ReadFileName[MAXLINE];
    char	ScriptName[MAX_PATH];
    char	*ptr, *pstr;
    int		status = 0;

    if (InitScript(hwnd, FileName, &frames) < 0)
	return -1;
    if (type == SCREENFORMULA && JuliaSetAnimation)			// force julia version using startup of "z = pixel:" (I can't think of a better way)
	{
	BuildJuliaFormulaString();
	if (ProcessFormulaString(FormulaString) == -1)
	    return -1;
	}
    if (WritePNGList)
	{
	sprintf(FilenameList, "%s.lst", SaveFileOrig);
	if ((fl = fopen(FilenameList, "w")) == NULL)
	    {
	    wsprintf(s, "Can't open file: %s for read", FilenameList);
	    MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    fclose(fp);
	    return -1;
	    }
	fprintf(fl, ";\n; This is a ManpWIN list file for generating an MPEG output\n");
	fprintf(fl, "; Firstly, we identify it as an MPEG generating file and give the the Dib dimensions\n");
	fprintf(fl, "#MPEG=%d %d\n", Dib.DibWidth, Dib.DibHeight);	// first line of list file is the Dib dimensions
	fprintf(fl, "; Name of the output file. May be left blank\n");
	fprintf(fl, "#MPEGName=%s\n", GenerateMPEGFileName (MPGPath, SaveFileOrig));
	fprintf(fl, "; Begin the list of full filenames\n");
	}

    CurrentFrame = 0;
    if (MathusRateAnimation || IkedaAnimation || ParamAnimation)
	InitAnimParamValues();
    else					    // no need to read any further in script file. We calculate the change in rate only.
	{
	if (fgets(buf, MAXDATALINE, fp) == NULL)
	    {
	    wsprintf(s, "Error in getting data from file: %s", FileName);
	    MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	    fclose(fp);
	    return -1;
	    }
	}
    UpdateInit();
//    if (flag == 1)							// flag returned from InitScript()
//	{
//	if (ImplementMorph(OscillatorSpecific[subtype].MaxDimensions, (int)OscillatorSpecific[subtype].iterations) < 0)
//	    {
//	    MessageBox (hwnd, "Oscillator Morphing failed because of Memory Unavailable", "ManpWin", MB_ICONEXCLAMATION | MB_OK);
//	    CloseMorphing();
//	    return -1;
//	    }
//	CloseMorphing();
//	return 1;							// MORPHING gets reset in ImplementMorph(), so make sure RunScript() knows we are morphing
//	}

// What is this needed for? It crashes PaletteShift
/*
    if (StartFrame > 0)							// we are resuming after a break
	{
	if (abs(PaletteShift) > 1)
	    {
	    if (PNGFileOpenDlg(hwnd, PNGFile, szTitleName) == 0)
		{
		if (read_png_file(hwnd, PNGFile) < 0)			// load last reliable frame to set up palette etc
		    {
		    wsprintf(s, "Error in getting start up data for frame: %d", StartFrame);
		    MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		    }
		}
	    }
	}
*/

    while(TRUE)
	{
	if (MathusRateAnimation || IkedaAnimation || ParamAnimation)
	    UpdateAnimParamValues();
	if (CurrentFrame < StartFrame/* && !StartImmediately*/)
	    {
	    if (fgets(buf, MAXDATALINE, fp) == NULL)
		{
		wsprintf(s, "Error in getting data for frame: %d", CurrentFrame);
		MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		fclose(fp);
		return -1;
		}
//	    if (WriteMemFrames)				// write frame to memory
//		{
//		sprintf(ReadFileName, "%s%05d.png", SaveFileOrig, CurrentFrame);	// try to use existing files
//		if (read_png_file(hwnd, ReadFileName) > 0)
//		    LoadAnimationFrame(buf, MoreInfo, CurrentFrame, param[ParamNumber], (MathusRateAnimation || IkedaAnimation || ParamAnimation), FALSE);
//		}
	    CurrentFrame++;
	    continue;					// skip until we catch up
	    }
	if (abs(PaletteShift))
	    MovePalette(&TrueCol, PaletteShift, EndIter);			// move palette for animations
	pstr = FileName + lstrlen(FileName) - 1;
	while ((*pstr != '\\') && (*pstr != ':') && (pstr >= FileName))
	    pstr--;
	pstr++;
	strcpy(ScriptName, pstr);

	if (OscAnimProc != MORPHING)
	    wsprintf (s, "Paul's Fractals: Frame %d of %d in script file %s, It=%d, BigNum=%s", CurrentFrame + 1, frames, ScriptName, threshold, (BigNumFlag) ? "T" : "F");
	SetWindowText (hwnd, s);			// Show formatted text in the caption bar
	ptr = SaveFileOrig;
	while (*ptr && *ptr != '.')			//strip extension
	    ptr++;
	*ptr = '\0';

//	if (LocalPalettePtr)
//	    memcpy(PalettePtr, LocalPalettePtr, EndIter * 3);
	if (type == SCREENFORMULA && JuliaSetAnimation)			// update julia values
	    {
	    param[0] = j.x;
	    param[1] = j.y;
	    }

	status = GenerateFractal(hwnd, FileName, MoreInfo, frames, CurrentFrame);
	if (status < 0)
	    {
	    if (WritePNGFrames)
		fclose(fl);
	    fclose(fp);
	    AnimateClose();
	    if (OscAnimProc != MORPHING)
		OscProcess.CloseMorphing();
	    return -1;
	    }
	else if (status == ENDOFSCRIPT)
	    {
	    if (WritePNGFrames)
		fclose(fl);
	    fclose(fp);
	    AnimateClose();
	    if (OscAnimProc != MORPHING)
		OscProcess.CloseMorphing();
	    wsprintf(s, "Error: Unexpected end of script: Frame %d of %d", CurrentFrame, frames);
	    MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	    break;
	    }

	if (status == ENDOFSCRIPT || time_to_break)
	    {
	    InvalidateRect(hwnd, &r, FALSE);
	    break;
	    }

	finished = TRUE;
	DisplayPalette(hwnd, TrueCol.ScriptPaletteFlag);
	if (WritePNGList)				// write PNG filename to *.lst file
	    fprintf(fl, "%s%05d.png\n", SaveFileOrig, CurrentFrame);
	if (WritePNGFrames)				// write frame to PNG file without loading to memory
	    {
	    sprintf(SaveFileName, "%s%05d.png", SaveFileOrig, CurrentFrame);
	    if (write_png_file(hwnd, SaveFileName, "ManpWIN", FractData()) < 0)
		{
		wsprintf(s, "Error: Could not write png file: <%s>", SaveFileName);
		MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);
		fclose(fp);
		if (WritePNGFrames)
		    fclose(fl);
		if (OscAnimProc != MORPHING)
		    OscProcess.CloseMorphing();
		return -1;
		}
	    }
	if (WriteMemFrames)				// write frame to memory
	    LoadAnimationFrame(buf, MoreInfo, CurrentFrame, param[ParamNumber], (MathusRateAnimation || IkedaAnimation || ParamAnimation), (OscAnimProc == MORPHING));
	CurrentFrame++;
	if (CurrentFrame >= frames || time_to_break || CurrentFrame >= MAXANIM)
	    {
	    InvalidateRect(hwnd, &r, FALSE);
	    break;
	    }
	StatusBarAnimInfo (CurrentFrame, frames);
	}

    frames = CurrentFrame;
    StartFrame = 0;
    EndScript(frames);

	// here begins the main memory animation run. Individual frames are run by the timer
    if (!OscProcess.DisplayAxisImages)
	{
	if (WriteMemFrames)				// write frame to memory
	    AnimateInit(frames);
	}
    if (WritePNGList)
	fclose(fl);
    fclose(fp);
    if (OscAnimProc != MORPHING)
	OscProcess.CloseMorphing();
    StatusColour = 0x0000FFFF;				// colour of status bar
    time_to_break = FALSE;
    DisplayStatusBarInfo(INFORMATION, "Full list of images for each axis pair");		// display status bar
/*
    for (int i = 0; i < 15; i++)
	{
	int	    HorOffset, VertOffset;
	HorOffset = (i % 5) * xdots / 5 + 5;
	VertOffset = (i / 5) * ydots / 3 + 5;
	OutputAxesStatus(GlobalHwnd, HorOffset, VertOffset, "SPLAT");
	//	TextOut(ps.hdc, HorOffset, VertOffset, ANIM[i].FrameInfo, (int)strlen(ANIM[i].FrameInfo));
	}
    InvalidateRect(GlobalHwnd, &r, FALSE);
*/
    return 0;
    }

void	StatusBarAnimInfo (int frame, int TotalFrames)

    {
    double	FrameTime, MaxFrameTime = 0.0;
    static	char	TotalTimeString[120];
    static	char	AverageTimeString[120];
    static	char	FrameTimeString[120];
    static	char	RemainingTimeString[120];

		// to do complex estimation of rate of increase in delay
    _ftime64(&FrameEnd);					// initialise time counter

    FrameTime = (double)(FrameEnd.time)   + (double)(FrameEnd.millitm)   / 1000.0 
	      - (double)(FrameStart.time) - (double)(FrameStart.millitm) / 1000.0;
    FrameStart.time = FrameEnd.time;
    FrameStart.millitm = FrameEnd.millitm;
    TotalTime += FrameTime;
    AverageTime = (frame - StartFrame > 0) ? TotalTime / (double)(frame - StartFrame) : FrameTime;
    // FinalTime = (frame > 0) ? (TotalTime *  TotalFrames) / (long)frame : 0L;
    //    RemainingTime = (TotalFrames - frame) * AverageTime;
    if (FrameTime > MaxFrameTime)
	MaxFrameTime = FrameTime;
    RemainingTime = (TotalFrames - frame) * MaxFrameTime;				// assume it will always be longer than the maximum frame time. This gets rid of early fast frame bias.


#ifdef	DEBUG
    sprintf(TotalTimeString, "Factor=%f, Rate=%f,SmoothTime=%f,LastSmoothTime=%f", factor, rate, SmoothTime, LastSmoothTime);
#else
    sprintf(TotalTimeString, "Total time=%s", ShowTime (TotalTime));
#endif
    sprintf(FrameTimeString, "Time for Frame[%d]of[%d]=%s", frame, TotalFrames, ShowTime (FrameTime));
    sprintf(AverageTimeString, "Ave Frame Time=%s", ShowTime (AverageTime));
//sprintf(FinalTimeString, "Total Expected Time (Poor Guess if BigNum)=%s", ShowTime (FinalTime));
    sprintf(RemainingTimeString, "Remaining Time (Est)=%s", ShowTime (RemainingTime));
    sprintf(szStatus, "%s, %s, %s, %s", TotalTimeString, FrameTimeString, AverageTimeString, RemainingTimeString);
    StatusColour = 0x0000FFFF;				// colour of status bar
//LastSmoothTime = SmoothTime;
    }

/*-----------------------------------------
	Status Bar generator
  -----------------------------------------*/

void	OutputStatusBar (HWND hwnd)
    {
    HBRUSH  hbr;         // handle to brush

/*
    if (DisplayAxisImages)
	{
	strcpy(szStatus, "Axis Display");
	StatusColour = 0x0000ff00;
	}
*/
    hbr = CreateSolidBrush(StatusColour);
    GetClientRect (hwnd, &r);
    StatusBarRect.left = r.left;
    StatusBarRect.top = r.bottom - GetSystemMetrics(SM_CYMENU);// make status same size as menu
    StatusBarRect.right = r.right; 
    StatusBarRect.bottom = r.bottom;
    FillRect(ps.hdc, &StatusBarRect, hbr);
    TextOut(ps.hdc, 8, StatusBarRect.top + 2, szStatus, (int)strlen(szStatus));    
//    DrawText(ps.hdc, szStatus, strlen(szStatus), &StatusBarRect, DT_LEFT);  
    DeleteObject(hbr);
//    if (DisplayAxisImages)
//	ShowAxisLabels(hwnd);
#ifdef SPLAT
    if (DisplayAxisImages)
	{
/*
	for (int i = 0; i < 15; i++)
	    {
	    int HorOffset = (i % 5) * xdots / 5 + 5;
	    int VertOffset = (i / 5) * ydots / 3 + 5;
	    OutputAxesStatus(GlobalHwnd, HorOffset, VertOffset, ANIM[i].FrameInfo);
	    //	TextOut(ps.hdc, HorOffset, VertOffset, ANIM[i].FrameInfo, (int)strlen(ANIM[i].FrameInfo));
	    }
*/
	for (int i = 0; i < 28; i++)
	    {
	    int HorOffset = (i % 7) * xdots / 7 + 5;
	    int VertOffset = (i / 7) * ydots / 4 + 5;
	    OutputAxesStatus(GlobalHwnd, HorOffset, VertOffset, ANIM[i].FrameInfo);
	    //	TextOut(ps.hdc, HorOffset, VertOffset, ANIM[i].FrameInfo, (int)strlen(ANIM[i].FrameInfo));
	    }
	InvalidateRect(GlobalHwnd, &r, FALSE);
//	DisplayAxisImages = FALSE;
	}
#endif
    }

/**************************************************************************
	Dialog Control for Starting Animation
**************************************************************************/

DLGPROC FAR PASCAL AnimStartDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
    {
     static	HANDLE	hCursor;
     static     char	temp;
     static     UINT	tempParam;
     static     BYTE	temp_degree;
     static	char	TempFile[MAX_PATH];
     BOOL		bTrans ;
     BOOL		TempCheck;
     HWND		hCtrl;
     char		*fileptr;
     
//     temp_degree = degree;
     switch (message)
	  {
	  case WM_INITDIALOG:
		cycleflag = FALSE;
		SetDlgItemInt(hDlg, IDC_ANIMSTART, StartFrame, TRUE);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
//		SetUpFilename(PNGName, "animpng", "Manp");
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
	        return (DLGPROC)TRUE ;
//	        return FALSE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_WRITEMPEGDIRECT:
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (WriteMPEGFrames)					// no point in writing other forms if direct to MPEG
			    {
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WritePNGFrames = WritePNGList = FALSE;
			    sprintf(MPGFile, "%s", GenerateAnimFileName (MPGPath, PNGName));
			    SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, MPGFile);
			    }
			return (DLGPROC)TRUE;

		    case IDC_WRITEPNGDIRECT:
		    case IDC_WRITEMEMDIRECT:
		    case IDC_WRITEPNGFILELIST:
			hCtrl = GetDlgItem (hDlg, (int) LOWORD(wParam));
			TempCheck = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (TempCheck)						// see above
			    {
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WriteMPEGFrames = FALSE;
			    }
//			sprintf(PNGFile, "%s", GenerateAnimFileName (ANIMPNGPath, PNGName));
//			SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGFile);
			return (DLGPROC)TRUE;
		    case IDOK:
			StartFrame = GetDlgItemInt(hDlg, IDC_ANIMSTART, &bTrans, TRUE);
			if (StartFrame < 0)
			    StartFrame = 0;
			StartImmediately = (StartFrame == 0);
			GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName, MAX_PATH);
			fileptr = PNGName + strlen(PNGName);
			while (fileptr > PNGName && *fileptr != '.')
			    fileptr--;	
			if (*fileptr == '.')						// remove any extension
			    *fileptr = '\0';
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			WritePNGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			WriteMemFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			WritePNGList = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (WriteMPEGFrames)						// generate MPEG filename
			    {
			    GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, TempFile, MAX_PATH);
			    sprintf(MPGFile, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }


