/*
    Script.cpp - a module to interpret animation scripts.

    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include <atomic>
#include "big.h"
#include <math.h>
#include "manpwin.h"
#include "manp.h"
//#include "anim.h"
#include "slope.h"
#include "fractype.h"
#include "fractalp.h"
#include "resource.h"
#include "colour.h"
#include "complex.h"
#include "OscProcess.h"
#include "plot.h"
#include "pixel.h"
#include "SafeStrings.h"

#define	TWO_PI		6.28318530717958
#define	ENDOFSCRIPT	1

extern	int	mainview(HWND, BOOL);
extern	char	*trailing(char *);
extern	int	GetParamData(HWND, LPSTR, LPSTR, LPSTR, BOOL);
extern	void	DisplayPalette(HWND, BOOL);
extern	int	write_png_file(HWND, char *, char *, char *); 
extern	int	read_png_file(HWND hwnd, char *infile);
extern	void	InitTrueColourPalette(BYTE);
extern	int	user_data(HWND);
	int	RunFourierScript(HWND, char *, char *);
extern	void	InitFourier(char);
extern	int	FourierStep(HWND, char *, int, int, CPlot);
extern	char	*FractData(void);
extern	int	ProcessFormulaString(char *);
extern	char	*str_find_ci(char *, char *);

extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);
extern	DLGPROC	PNGFileOpenDlg(HWND, LPSTR, LPSTR);

// translate params into variables
extern	void	ApplyPerturbationParams(void);
extern	void	ApplySlopeDerivParams();
extern	void	ApplySlopeFwdDiffParams();

extern	int	number;			// used for compression

extern	std::atomic<bool> gStopRequested; // force early exit

extern	char	MAPFile[];		// colour map file
extern	char	MPGPath[];		// path for MPEG files
extern	char	MPGFile[];		// MPEG file
extern	char	PNGName[];		// base name for PNG file sequence or MPG filename
extern	char	ANIMPNGPath[];		// path for animated PNG files and LST files
extern	char	PNGFile[];		// PNG file
extern	WORD	steps;			// for Fourier Analysis
extern	BOOL	WritePNGFrames;		// write frames to PNG files
extern	BOOL	WriteMemFrames;		// write frames to memory
extern	BOOL	WritePNGList;		// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;	// write frames directly to an MPEG file

extern	BOOL	DisplayAxisLabels;		// show labels for axis pairs

struct __timeb64 	FrameEnd;
struct __timeb64 	FrameStart;

static	double	AverageTime = 0.0;
static	double	TotalTime = 0.0;
static	double	FinalTime = 0.0;
static	double	RemainingTime = 0.0;
static	double	StartRate, EndRate;		// population growth rates for Malthus fractal
static	char    szTitleName[MAXDATALINE] = "ManpWIN";

static	FILE	*fp;				// script file   
static	FILE	*fl;				// a list of files names for creating a large MPEG file
static	int	StartIter, EndIter, CurrentFrame, frames, ParamNumber;
static	BOOL	ParamAnimation = FALSE;
static	BOOL	OscillatorAnimation = FALSE;
static	BOOL	JuliaSetAnimation = FALSE;
static	BOOL	InversionSetAnimation = FALSE;
static	double	divisor;

	char	SaveFileOrig[MAXLINE];				// SaveAs filename base name
	char	FilenameList[MAXLINE];				// filename for list of animation frame filenames

bool	gStopAnimation;						// tell fourier to stop animating

/*------------------------------------------
   Generate  MPG filename
  ------------------------------------------*/

char	*GenerateMPEGFileName (char *MPEGPath, char *infile)
    {
    char		drive[_MAX_DRIVE], dir[_MAX_DIR], name[_MAX_FNAME], ext[_MAX_EXT];
    static	char	MPGName[MAX_PATH];

    _splitpath(infile, drive, dir, name, ext);				// remove extension
    SAFE_SPRINTF(MPGName, TEXT("%s\\%s.mpg"), MPEGPath, name);
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
    SAFE_SPRINTF(AnimName, TEXT("%s\\%s"), AnimPath, name);
    return AnimName;	
    }

/*------------------------------------------
   Get the total number of frames
  ------------------------------------------*/

double	CManp::GetNumFrames(void)
    {
    return (double)frames;
    }

/*------------------------------------------
   if MPEG direct fails
  ------------------------------------------*/

void	CManp::CloseScript(void)
    {
    fclose(fp);
    }

/*------------------------------------------
   Generate  actual fractal
  ------------------------------------------*/

int	CManp::GenerateFractalFrame(HWND hwnd, char *FileName, int TotalFrames, int ThisFrame)
    {
    double	FramesCubed, iCubed;
    HCURSOR	hCursor;
    char	s[MAXLINE];
    char	buf[MAXDATALINE]; 

    user_data(hwnd);
    if (time_to_quit)
	{
	AutoSaveFlag = FALSE;
	fclose(fp);
	return -1;
	}

    if (!ParamAnimation)
	{
	BigNumFlag = FALSE;					// we are starting at the shallow end of the pool
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
	Pixel[0]->init3d(xdots, ydots, x_rot, y_rot, z_rot, sclx, scly, sclz, threshold, hor, vert);				// init 3D parameters 
    if (pairflag)
	Pixel[0]->init_stereo_pairs(pairflag, &AutoStereo_value);			// init stereo pair parameters
    Pixel[0]->InitArithmetic();
    ClearScreen();
    hCursor = LoadCursor(NULL, IDC_ARROW);	// Load normal cursor.
    SetCursor(hCursor);			        // Reload arrow cursor.
    DisplayFractal(hwnd);
    finished = FALSE;
    if (RunEscapeTimeEngine(hwnd) < 0)
	{
	_snprintf_s(s, MAXLINE, _TRUNCATE, "Can't calculate fractal in script file: %s", FileName);
	MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    return 0;
    }

int	CManp::GenerateFractal(HWND hwnd, char *FileName, char *MoreInfo, int TotalFrames, int ThisFrame, double &delay)
    {
    if (OscAnimProc == MORPHING)
	return (OscProcess.MorphStep(hwnd, FileName, MoreInfo, TotalFrames, ThisFrame, &delay, threshold));
    else if (type == FOURIER)
	{
	FourierStep(hwnd, FileName, TotalFrames, ThisFrame, Plot);
	if (gStopAnimation)
	    {
	    AnimateClose();
	    }
	return 0;   // or a benign code
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
	_snprintf_s(s, MAXLINE, _TRUNCATE, "Can't open script file: %s for read", FileName);
	MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	return -1;
	}

    if (fgets(buf, MAXDATALINE, fp) == NULL)				// get common params
	{
	_snprintf_s(s, MAXLINE, _TRUNCATE, "Can't read parameters in script file: %s", FileName);
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
	Init script file
  -----------------------------------------*/

int	CManp::InitScript(HWND hwnd, char *FileName, int *TotalFrames)
    {
    std::vector<char> buf(MAXDATALINE);
    std::vector<char> buf1(MAXDATALINE);
    char	s[MAXLINE];
    int		i, j;
    char	ascii[6];
    BOOL	eof = FALSE;
    int		ch, UpdateFrames;
    BOOL	RotateAnim = TRUE;
    char	*p;
    long	PalIter = 0L;

    setup_defaults();
    OscillatorAnimation = FALSE;
    ParamAnimation = FALSE;
    JuliaSetAnimation = FALSE;
    InversionSetAnimation = FALSE;
//    EndIter =  threshold;
    if ((fp = fopen(FileName, "r")) == NULL)
	{
	_snprintf_s(s, MAXLINE, _TRUNCATE, "Can't open script file: %s for read", FileName);
	MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    if (fgets(buf.data(), MAXDATALINE, fp) == NULL)				// get common params
	{
	_snprintf_s(s, MAXLINE, _TRUNCATE, "Can't read parameters in script file: %s", FileName);
	MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	fclose(fp);
	return -1;
	}
    else 
	{
	strcpy(buf1.data(), buf.data());
	if (GetParamData(hwnd, FileName, buf.data(), SaveFileOrig, FALSE) < 0)
	    return -1;
	time_to_restart = FALSE;
	}
    StartIter = EndIter = threshold;						// get first iteration count from file
										// now get co-ords for each image
    if (fgets(buf.data(), MAXDATALINE, fp) == NULL)				// get common params
	{
	_snprintf_s(s, MAXLINE, _TRUNCATE, "Can't read parameters in script file: %s", FileName);
	MessageBox (hwnd, s, "View", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	fclose(fp);
	return -1;
	}
    if (strncmp(buf.data(), "Parameter Animation:", 20) == 0)
	{
	sscanf(buf.data() + 20, "%lf %lf %d %d %d", &StartRate, &EndRate, &frames, &ParamNumber, &Return2Start);
	ParamAnimation = TRUE;
	}
    else if (strncmp(buf.data(), "Oscillator", 10) == 0)
	{
	if (strncmp(buf.data(), "Oscillator Morphing:", 19) == 0)
	    RotateAnim = MORPH;
	else if (strncmp(buf.data(), "Oscillator Rotation:", 19) == 0)
	    RotateAnim = ROTATION;
	else
	    RotateAnim = EVOLVE;
	sscanf(buf.data() + 22, "%d %d %d", &EndIter, &EndIter, &frames);
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
	}
    else
	{
	p = buf.data();
	if (strncmp(buf.data(), "Julia Set Animation:", 20) == 0)
	    {
	    p = buf.data() + 20;
	    JuliaSetAnimation = TRUE;
	    }
	if (strncmp(buf.data(), "Inversion Animation:", 20) == 0)
	    {
	    p = buf.data() + 20;
	    InversionSetAnimation = TRUE;
	    }
	sscanf(p, "%d %d %d %d", &EndIter, &frames, (int *)&TrueCol.ScriptPaletteFlag, &PaletteShift);
	PalIter = EndIter;
	TrueCol.FinalThreshold = EndIter;

//	PalIter = (EndIter > MAXPALETTE) ? MAXPALETTE - 1 : EndIter;
//	TrueCol.FinalThreshold = PalIter;
	OscillatorAnimation = FALSE;
	ParamAnimation = FALSE;
	}
    *TotalFrames = frames;						// only needed for direct creation of MPEG
    InitAnimationFrameStructure(frames);

    if (fgets(buf.data(), MAXLINE, fp) == NULL)				// no palette
	InitTrueColourPalette(FALSE);					// process anything we found in the param list, eg palette file etc
    else			
	{
	if (strncmp(buf.data(), "Palette=", 8) == 0)				// we have a palette file
	    {
	    long PaletteEntries = min(EndIter, MAXPALETTE - 1);
	    for (i = 0; i < PaletteEntries; i++)
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
		if (eof)
		    break;
		ascii[4] = '\0';
		ConvertASCII2RGB(TrueCol.PalettePtr[i], ascii);
		}
	    }
	}

    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Starting Animation Run at frame [%d]", StartFrame);
    if (StartFrame > 0)
	{
	int LocalPaletteShift = (PaletteShift * StartFrame) % EndIter;		// if we are using palette shift and starting frame != 0, we need to shift palette in order to match previous frames
	TrueCol.MovePalette(&TrueCol, LocalPaletteShift, EndIter);
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
	ClearScreen();
	InitFourier(subtype);
	}
    if (OscAnimProc == MORPHING)
	{
	ClearScreen();
	if (OscProcess.InitMorphing(MaxDimensions, (long)iterations, &UpdateFrames, type, OscAnimProc, frames, &Dib) < 0)
	    MessageBox (hwnd, "Oscillator Morphing failed because of Memory Unavailable", "ManpWin", MB_ICONEXCLAMATION | MB_OK);
	StatusColour = 0x0000FFFF;			// colour of status bar
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

    AxisRect.left = HorOffset + (gManp->xdots * 2) / (NumColumns * 5);
    AxisRect.top = VertOffset + gManp->ydots / 10;
    AxisRect.right = AxisRect.left + gManp->xdots / (2 * NumColumns);
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
    gManp->Dib.Text2Dib(hdc, &AxisRect, 0x00ffffff, 0L, &lf, TRANSPARENT, text);
    }

void	ShowAxisLabels(HWND hwnd)
    {
    char    labels[] = "xyzuvwpqrs";
    char    a[12];
    char    b[12];
    char    text[24];
    int	    NumAxes = gManp->OscProcess.axes;

    int NumColumns = NumAxes - 1;
    int NumRows = NumAxes / 2;

    if (gManp->OscProcess.DisplayAxisImages)
	{
	BeginPaint(hwnd, &gManp->ps);
	for (int i = 0; i < NumRows * NumColumns; i++)
	    {
	    int HorOffset = (i % NumColumns) * gManp->xdots / NumColumns + 5;
	    int VertOffset = (i / NumColumns) * gManp->ydots / NumRows + 5;
	    if (gManp->OscProcess.In1[i] < NumAxes / 2)
		{
		*a = *(labels + gManp->OscProcess.In1[i]);
		*(a + 1) = '\0';
		}
	    else
		{
		*a = *(labels + gManp->OscProcess.In1[i] - NumAxes / 2);
		*(a + 1) = 'n';
		*(a + 2) = '\0';
		}
	    if (gManp->OscProcess.In2[i] < NumAxes / 2)
		{
		*b = *(labels + gManp->OscProcess.In2[i]);
		*(b + 1) = '\0';
		}
	    else
		{
		*b = *(labels + gManp->OscProcess.In2[i] - NumAxes / 2);
		*(b + 1) = 'n';
		*(b + 2) = '\0';
		}
	    SAFE_SPRINTF(text, "%s-%s", a, b);
	    OutputAxesStatus(gManp->GlobalHwnd, gManp->ps.hdc, HorOffset, VertOffset, NumRows, NumColumns, text);
	    }
	EndPaint(hwnd, &gManp->ps);
	InvalidateRect(gManp->GlobalHwnd, &gManp->r, FALSE);
	}
    }

/*-----------------------------------------
	Finish script file
  -----------------------------------------*/

void	CManp::EndScript(int ThisFrame)
    {
    StatusColour = 0x0000FF00;					// colour of status bar
    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "Animation Run Completed %d frames in %s", ThisFrame, ShowTime ((long)TotalTime));
    AutoSaveFlag = FALSE;					// all done
    time_to_restart = FALSE;
    time_to_break = FALSE;
    OscAnimProc = STANDARD;
    if (!OscProcess.DisplayAxisImages)
	return;

    Dib.ClearDib(0, 0, 0);	// set background colour

    for (int i = 0; i < frames; i++)
	{
	const AnimStruct& A = ANIM[i];

	// Optional safety (helps when scripts load mixed-size frames)
	if (A.pixels.empty() || A.width <= 0 || A.height <= 0 || A.bitsPerPixel != 24)
	    continue;

	// If Dib is supposed to match animation frame size, you can enforce it:
	// if (Dib.DibWidth != A.width || Dib.DibHeight != A.height || Dib.BitsPerPixel != A.bitsPerPixel) continue;

	for (int j = 0; j < A.height; j++)
	    {
	    int y = A.height - 1 - j; // preserve your original bottom-up logic

	    const uint8_t* srcRow = A.pixels.data() + y * A.pitch;
	    uint8_t* dstRow = Dib.DibPixels.data() + y * Dib.WidthBytes;

	    for (int k = 0; k < A.width; k++)
		{
		const uint8_t* srcPix = srcRow + k * 3;
		uint8_t* dstPix = dstRow + k * 3;

		// Copy only non-zero components (your original rule)
		if (srcPix[0] != 0) dstPix[0] = srcPix[0];
		if (srcPix[1] != 0) dstPix[1] = srcPix[1];
		if (srcPix[2] != 0) dstPix[2] = srcPix[2];
		}
	    }
	}
    
    time_to_break = TRUE;
    RunAnimation = FALSE;
    AnimateClose();
    finished = TRUE;
    UpdateClose();
    if (DisplayAxisLabels)
	ShowAxisLabels(GlobalHwnd);
    DoCaption(GlobalHwnd, (type == OSCILLATORS) ? OscillatorSpecific[subtype].name : FractalMapSpecific[subtype].name);
    }

/*-----------------------------------------------------------------------------------
    These two functions taken out of RunScript() so they can be accessed within 
    MPEGWrite.cpp to update frames for variable parameter type animations
-----------------------------------------------------------------------------------*/

void CManp::ApplyAnimatedParam(WORD type, int subtype, int ParamNumber)
    {
    switch (type)
	{
	case PERTURBATION:
	    switch (ParamNumber)
		{
		case 0:
		    ColourSpeed = param[0];
		    break;
		case 1:
		    lightDirectionDegrees = param[1];
		    break;
		case 2:
		    bumpMappingDepth = param[2];
		    break;
		case 3:
		    bumpMappingStrength = param[3];
		    break;
		case 4:
		    bump_transfer_factor = param[4];
		    break;
		case 5:
		    PaletteStart = (int)param[5];
		    break;

		case 6:
		    if (subtype == 57)
			LightHeight = param[6];
		    else
			PertColourMethod = (int)param[6];
		    break;

		case 7:
		    if (subtype != 57)
			IterDiv = param[7];
		    break;

		case 8:
		    if (subtype != 57)
			PalOffset = (int)param[8];
		    break;

		case 9:
		    if (subtype == 0 || subtype == 1 || subtype == 59 || subtype == 60 || subtype == 61 || subtype == 62 || subtype == 63)
			LightHeight = param[9];
		    break;
		}
	    break;

	case SLOPEDERIVATIVE:
	    switch (ParamNumber)
		{
		case 2:
		    PaletteStart = (int)param[2];
		    break;
		case 3:
		    ColourSpeed = param[3];
		    break;
		}
	    break;

	case SLOPEFORWARDDIFF:
	    switch (ParamNumber)
		{
		case 0:
		    bump_transfer_factor = param[0];
		    break;
		case 1:
		    PaletteStart = (int)param[1];
		    break;
		case 2:
		    lightDirectionDegrees = param[2];
		    break;
		case 3:
		    bumpMappingDepth = param[3];
		    break;
		case 4:
		    bumpMappingStrength = param[4];
		    break;
		case 5:
		    ColourSpeed = param[5];
		    break;
		}
	    break;
	default:
	    if (ParamNumber == 10)
		{
		rqlim = param[ParamNumber];

		switch (MathType)
		    {
		    case DOUBLEDOUBLE:
			DDBailout = rqlim;
			break;

		    case QUADDOUBLE:
			QDBailout = rqlim;
			break;

		    case ARBITRARYPREC:
			BigBailout = rqlim;
			break;
		    }
		}
	    break;
	}
    }

void CManp::InitAnimParamValues(void)
    {
    if (!ParamAnimation)
	return;

    divisor = (EndRate - StartRate) / frames;

    // Bailout is a pseudo-parameter used only by parameter animation.
   //
   // Ordinary Pixel fractals use ParamNumber 10 for bailout.
   // Perturbation and Slope use ParamNumber 15.
   //
   // Neither case represents the corresponding gManp->param[] entry.
   // In particular, gManp->param[15] is reserved for PrePaletteColour.
    if (IsBailoutAnimParam())
	{
	rqlim = StartRate;

	if (rqlim < 1.0)
	    rqlim = StartRate = 1.0;

	switch (MathType)
	    {
	    case DOUBLEDOUBLE:
		DDBailout = rqlim;
		break;

	    case QUADDOUBLE:
		QDBailout = rqlim;
		break;

	    case ARBITRARYPREC:
		BigBailout = rqlim;
		break;
	    }

	return;
	}

    // Genuine fractal parameter.
    param[ParamNumber] = StartRate;

    // param[15] always carries the base fractal's pre-palette colour.
    // It is not the bailout value, even though ParamNumber 15 is used
    // as the bailout pseudo-parameter for Perturbation/Slope animation.
//    PrePaletteColour = (DWORD)param[15];

    // Initialise the corresponding live runtime state for renderers
    // that mirror some values from param[].
    if (type == PERTURBATION)
	ApplyPerturbationParams();
    else if (type == SLOPEDERIVATIVE)
	ApplySlopeDerivParams();
    else if (type == SLOPEFORWARDDIFF)
	ApplySlopeFwdDiffParams();
    }

/**************************************************************************
    Update the parameter being animated for the next frame.

    Most animation parameters are genuine entries in param[].
    For Perturbation and the two slope renderers, some of those param[]
    values are also mirrored into named runtime variables such as
    PaletteStart, ColourSpeed and lightDirectionDegrees.

    Bailout is a pseudo-parameter used only by parameter animation:
	Ordinary Pixel fractals: ParamNumber 10
	Perturbation/Slope:      ParamNumber 15

    These do not correspond to the matching gManp->param[] entries.
    In particular, gManp->param[15] is reserved for PrePaletteColour.
**************************************************************************/

void CManp::UpdateAnimParamValues(void)
    {
    if (!ParamAnimation)
	return;

    // Bailout is not a genuine param[] entry, so update rqlim directly.
    if (IsBailoutAnimParam())
	{
	rqlim += divisor;

	switch (MathType)
	    {
	    case DOUBLEDOUBLE:
		DDBailout = rqlim;
		break;

	    case QUADDOUBLE:
		QDBailout = rqlim;
		break;

	    case ARBITRARYPREC:
		BigBailout = rqlim;
		break;
	    }

	return;
	}

    // Genuine fractal parameter: advance only the parameter selected
    // for animation.
    param[ParamNumber] += divisor;

    // Some renderers mirror selected param[] values into named runtime
    // variables. Update only the mirror belonging to this parameter.
    ApplyAnimatedParam(type, subtype, ParamNumber);
    }

bool CManp::IsBailoutAnimParam(void)
    {
    // Perturbation and Slope use animation slot 15 as the
    // Bailout pseudo-parameter.  It is NOT gManp->param[15];
    // param[15] is reserved for PrePaletteColour.
    if (type == PERTURBATION ||	type == SLOPEDERIVATIVE || type == SLOPEFORWARDDIFF)
	return ParamNumber == 15;

    // These fractal families use param[0..19] as two genuine
    // groups of parameters:
    //   0..9   constants
    //   10..19 variables
    // Therefore ParamNumber 10 is a real parameter and must
    // never be interpreted as Bailout.
    if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
	return false;

    // Ordinary Pixel-style fractals have up to 10 real params
    // (0..9).  Animation slot 10 is therefore available as the
    // Bailout pseudo-parameter.
    return ParamNumber == 10;
    }

/*-----------------------------------------
	Build Julia Set formula string
  -----------------------------------------*/

void	CManp::BuildJuliaFormulaString(void)
    {
    char	TempFormula[1200];
    char	*ptr;

    if (param[4] != 0.0)
	return;								// we want to keep the initial conditions
    strcpy(TempFormula, FormulaString);
    ptr = str_find_ci(TempFormula, "pixel:");
    _snprintf_s(FormulaString, MAXFORMULASTRINGLENGTH, _TRUNCATE, "z = pixel:%s", ptr);			// Julia
    }

/*-----------------------------------------
	Parse script file
  -----------------------------------------*/

int	CManp::RunScript(HWND hwnd, char *FileName)
    {   
    char	buf[MAXDATALINE];
    char	s[MAXLINE];
    char	MoreInfo[MAXLINE];
    static	char	SaveFileName[MAXLINE];				// individual SaveAs filename
    static	char	ReadFileName[MAXLINE];
    char	ScriptName[MAX_PATH];
    char	*ptr, *pstr;
    int		status = 0;
    double	delay = 1.0;

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
	SAFE_SPRINTF(FilenameList, "%s.lst", SaveFileOrig);
	if ((fl = fopen(FilenameList, "w")) == NULL)
	    {
	    _snprintf_s(s, MAXLINE, _TRUNCATE, "Can't open file: %s for read", FilenameList);
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
    if (ParamAnimation)
	InitAnimParamValues();
    else					    // no need to read any further in script file. We calculate the change in rate only.
	{
	if (fgets(buf, MAXDATALINE, fp) == NULL)
	    {
	    _snprintf_s(s, MAXLINE, _TRUNCATE, "Script: Error in getting data from file: %s", FileName);
	    MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	    fclose(fp);
	    return -1;
	    }
	}

    UpdateInit();
    while(TRUE)
	{
	if (ParamAnimation)
	    UpdateAnimParamValues();
	if (CurrentFrame < StartFrame/* && !StartImmediately*/)
	    {
	    if (fgets(buf, MAXDATALINE, fp) == NULL)
		{
		_snprintf_s(s, MAXLINE, _TRUNCATE, "Error in getting data for frame: %d", CurrentFrame);
		MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		fclose(fp);
		return -1;
		}
	    CurrentFrame++;
	    continue;					// skip until we catch up
	    }
	if (abs(PaletteShift))
	    TrueCol.MovePalette(&TrueCol, PaletteShift, EndIter);			// move palette for animations
	pstr = FileName + lstrlen(FileName) - 1;
	while ((*pstr != '\\') && (*pstr != ':') && (pstr >= FileName))
	    pstr--;
	pstr++;
	strcpy(ScriptName, pstr);

	if (OscAnimProc == MORPHING)
	    SetWindowText(hwnd, MoreInfo);				// Show morphing text in the caption bar
	else
	    {
	    _snprintf_s(s, MAXLINE, _TRUNCATE, "Paul's Fractals: Frame %d of %d in script file %s, It=%d, BigNum=%s", CurrentFrame + 1, frames, ScriptName, gManp->threshold, (gManp->BigNumFlag) ? "T" : "F");
	    SetWindowText (hwnd, s);					// Show formatted text in the caption bar
	    }
	ptr = SaveFileOrig;
	while (*ptr && *ptr != '.')					//strip extension
	    ptr++;
	*ptr = '\0';

	if (type == SCREENFORMULA && JuliaSetAnimation)			// update julia values
	    {
	    param[0] = j.x;
	    param[1] = j.y;
	    }

	status = GenerateFractal(hwnd, FileName, MoreInfo, frames, CurrentFrame, delay);
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
	    _snprintf_s(s, MAXLINE, _TRUNCATE, "Error: Unexpected end of script: Frame %d of %d", CurrentFrame, frames);
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
	    SAFE_SPRINTF(SaveFileName, "%s%05d.png", SaveFileOrig, CurrentFrame);
	    if (write_png_file(hwnd, SaveFileName, "ManpWIN", FractData()) < 0)
		{
		_snprintf_s(s, MAXLINE, _TRUNCATE, "Error: Could not write png file: <%s>", SaveFileName);
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
	    {
	    if (IsBailoutAnimParam())
		LoadAnimationFrame(buf, MoreInfo, CurrentFrame, rqlim, ParamAnimation, (OscAnimProc == MORPHING), delay);
	    else
		LoadAnimationFrame(buf, MoreInfo, CurrentFrame, param[ParamNumber], ParamAnimation, (OscAnimProc == MORPHING), delay);
	    }
	CurrentFrame++;
	if (CurrentFrame >= frames || time_to_break || CurrentFrame >= MAXANIM)
	    {
	    InvalidateRect(hwnd, &r, FALSE);
	    break;
	    }
	StatusBarAnimInfo (CurrentFrame, frames);
	}

//    frames = CurrentFrame;
    frames = (int)ANIM.size();
    EndScript(frames);
    AnimFirstFrame = StartFrame;
    AnimLastFrame = (int)ANIM.size() - 1;
    CurrentFrame = AnimFirstFrame;

	// here begins the main memory animation run. Individual frames are run by the timer
    if (!OscProcess.DisplayAxisImages)
	{
	if (WriteMemFrames)				// write frame to memory
	    {
//	    CurrentFrame = 0;				// <-- REQUIRED
	    AnimateInit(frames);
	    }
	}
    if (WritePNGList)
	fclose(fl);
    fclose(fp);
    if (OscAnimProc != MORPHING)
	OscProcess.CloseMorphing();
    StatusColour = 0x0000FFFF;				// colour of status bar
    time_to_break = FALSE;
    DisplayStatusBarInfo(INFORMATION, "Full list of images for each axis pair");		// display status bar
    return 0;
    }

void	CManp::StatusBarAnimInfo (int frame, int TotalFrames)
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
    AverageTime = (frame - gManp->StartFrame > 0) ? TotalTime / (double)(frame - gManp->StartFrame) : FrameTime;
        if (FrameTime > MaxFrameTime)
	MaxFrameTime = FrameTime;
    RemainingTime = (TotalFrames - frame) * MaxFrameTime;				// assume it will always be longer than the maximum frame time. This gets rid of early fast frame bias.


#ifdef	DEBUG
    SAFE_SPRINTF(TotalTimeString, "Factor=%f, Rate=%f,SmoothTime=%f,LastSmoothTime=%f", factor, rate, SmoothTime, LastSmoothTime);
#else
    SAFE_SPRINTF(TotalTimeString, "Total time=%s", ShowTime (TotalTime));
#endif
    SAFE_SPRINTF(FrameTimeString, "Time for Frame[%d]of[%d]=%s", frame, TotalFrames, ShowTime (FrameTime));
    SAFE_SPRINTF(AverageTimeString, "Ave Frame Time=%s", ShowTime (AverageTime));
    SAFE_SPRINTF(RemainingTimeString, "Remaining Time (Est)=%s", ShowTime (RemainingTime));
    _snprintf_s(szStatus, STATUSSIZE, _TRUNCATE, "%s, %s, %s, %s", TotalTimeString, FrameTimeString, AverageTimeString, RemainingTimeString);
    StatusColour = 0x0000FFFF;				// colour of status bar
    }

/*-----------------------------------------
	Status Bar generator
  -----------------------------------------*/

void	CManp::OutputStatusBar (HWND hwnd)
    {
    HBRUSH  hbr;         // handle to brush

    hbr = CreateSolidBrush(gManp->StatusColour);
    GetClientRect (hwnd, &gManp->r);
    gManp->StatusBarRect.left = gManp->r.left;
    gManp->StatusBarRect.top = gManp->r.bottom - GetSystemMetrics(SM_CYMENU);// make status same size as menu
    gManp->StatusBarRect.right = gManp->r.right;
    gManp->StatusBarRect.bottom = gManp->r.bottom;
    FillRect(gManp->ps.hdc, &gManp->StatusBarRect, hbr);
    TextOut(gManp->ps.hdc, 8, gManp->StatusBarRect.top + 2, gManp->szStatus, (int)strlen(gManp->szStatus));
    DeleteObject(hbr);
    }

/**************************************************************************
	Dialog Control for Starting Animation
**************************************************************************/

INT_PTR CALLBACK AnimStartDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
	        gManp->cycleflag = FALSE;
		SetDlgItemInt(hDlg, IDC_ANIMSTART, gManp->StartFrame, TRUE);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
	        return TRUE ;

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
			    _snprintf_s(MPGFile, _MAX_PATH, _TRUNCATE, "%s", GenerateAnimFileName (MPGPath, PNGName));
			    SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, MPGFile);
			    }
			return TRUE;

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
			return TRUE;
		    case IDOK:
			gManp->StartFrame = GetDlgItemInt(hDlg, IDC_ANIMSTART, &bTrans, TRUE);
			if (gManp->StartFrame < 0)
			    gManp->StartFrame = 0;
			gManp->StartImmediately = (gManp->StartFrame == 0);
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
			    _snprintf_s(MPGFile, _MAX_PATH, _TRUNCATE, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }


