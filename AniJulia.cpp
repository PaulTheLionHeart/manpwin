/*
    ANIJULIA.CPP a program to animate the Julia sets.
    
    Written in Microsoft Visual C++ by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "resource.h"
#include "manpwin.h"
#include "manp.h"
#include "Dib.h"
#include "fractype.h"
#include "complex.h"
#include "colour.h"
#include "plot.h"

extern	HWND	PixelHwnd;		// pointer to handle for pixel updating

extern	CTrueCol    TrueCol;		// palette info

extern	double	hor;			// horizontal address
extern	double	vert;			// vertical address
extern	double	mandel_width;		// final width
extern	BYTE	BigNumFlag;		// True if bignum used
extern	long	threshold;
extern	WORD	special;		// special colour, phase etc
extern	WORD	degree;			// power
extern	BYTE	screenflag;		/* replay saved screen */
extern	int	biomorph;		/* biomorph colour */
extern	BYTE	cycleflag;		/* do colour cycling */
extern	int	decomp;			/* number of decomposition colours */
extern	BYTE	orbit_flag;		/* display orbits? */
extern	BYTE	exitflag;		/* exit on completion */
extern	BYTE	juliaflag;		/* Julia implementation of fractal */
extern	char	floatflag;		/* floating point maths */
extern	BYTE	pairflag;		/* stereo pair flag and window size */
extern	BYTE	orig_palette[];		/* loaded palette */
extern	WORD	type;			/* M=mand, N=Newton etc */
extern	int	subtype;		
extern	int	width, height, xdots, ydots, bits_per_pixel;

extern	char	SCIPath[];		// path for SCI files

extern	BOOL	WritePNGFrames;		// write frames to PNG files
extern	BOOL	WriteMemFrames;		// write frames to memory
extern	BOOL	WritePNGList;		// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;	// write frames directly to an MPEG file

extern	char	*AnimData(void);
extern	BOOL	StartImmediately;
static	int	frames = 100;
	BOOL	ShowOrbits = TRUE;	// show julia orbits for each image
static	double	ScaleFactor = 1.0;
static	int	JuliaAnimType = 0;	// 0 = linear
					// 1 = circular
					// 2 = cardoid
extern	char	PNGName[];		// base name for PNG file sequence
extern	char	ScriptFileName[];	// base name for script file 
extern	char	MPGPath[];		// path for MPEG files
extern	char	MPGFile[];		// MPEG file
extern	char	ANIMPNGPath[];		// path for animated PNG files and LST files
extern	char	PNGFile[];		// PNG file

static	double	StartX	= -0.75;	// starting constants
static	double	StartY	= -1.0;
static	double	EndX	= -0.75;
static	double	EndY	= 1.0;

static	double	CentreX	= -1.0;
static	double	CentreY	= 0.0;
static	double	radius	= 0.25;

static	double	Magnitude = 0.248;	// 0.5 sits neatly inside the Mandelbrot set. 0.49 gives great orbits
static	int	PaletteShift = 0;

extern	double	xgap;			// gap between pixels
extern	double	ygap;			// gap between pixels

extern	CPlot	Plot;			// image plotting routines 
extern	Complex	j;

extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);

/**************************************************************************
	Cardiod Julia script generator
x = a(2cos(t) - cos(2t)), 
y = a(2sin(t) - sin(2t)).
	Nephroid
x = a(3cos(t) - cos(3t)), 
y = a(3sin(t) - sin(3t)) 
**************************************************************************/

int	CardioidJuliaScript(HWND hwnd, char *filename, int PaletteShift)

    {
    int		steps, i, k;
    double	Divisor;
    char	ascii[6];

    char	s[120];
    FILE	*out;
    double  AspectRatio = (double)xdots / (double)ydots;

    steps = frames;
    Divisor = TWO_PI / (steps - 1);
    BigNumFlag = FALSE;
    if (AspectRatio > 1.0)	// take aspect ration into account when plotting Julia
	{
	mandel_width = 4.0 * ScaleFactor;
	vert = -2.0 * ScaleFactor;
	hor = vert * AspectRatio;
	}
    else
	{
	mandel_width = 4.0  * ScaleFactor / AspectRatio;
	hor = -2.0 * ScaleFactor;
	vert = hor / AspectRatio;
	}

    BigNumFlag = FALSE;							// we are starting at the shallow end of the pool
    juliaflag = TRUE;

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Julia Set Animation: %ld %d %d %d\n", threshold, frames, TrueCol.ScriptPaletteFlag, PaletteShift);
    fprintf(out, "Palette=\n");
    for (i = 0, k = 0; i < threshold; i++, k++)
	{
	if (k == 20)							// group into lumps of 20
	    {
	    k = 0;
	    fprintf(out, "\n");
	    }
	ConvertRGB2ASCII(*(TrueCol.PalettePtr + i * 3 + 0), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3 + 2), ascii);
	fprintf(out, "%s", ascii);
	}
    fprintf(out, "\n");

    for (i = 0; i < steps; i++) 
	{
	    {
//	    j.x = (0.25 - cos(Divisor * (double)i) * (1.0 + cos(Divisor * (double)i)) * Magnitude);
//	    j.y = (sin(Divisor * (double)i) * (1.0 + cos(Divisor * (double)i)) * Magnitude);
	    j.x = Magnitude * ((double)degree * cos(Divisor * (double)i) - (cos((double)degree * Divisor * (double)i)));
	    j.y = Magnitude * ((double)degree * sin(Divisor * (double)i) - (sin((double)degree * Divisor * (double)i)));
	    fprintf(out, "-j%24.24f,%24.24f,\n", j.x, j.y);
	    }
	}

    fclose(out);
    return 0;
    }

/**************************************************************************
	Circle Julia script generator
**************************************************************************/

int	CircleJuliaScript(HWND hwnd, char *filename, int PaletteShift)

    {
    int		steps, i, k;
    double	Divisor;
    char	ascii[6];

    char	s[120];
    FILE	*out;
    double	AspectRatio = (double)xdots / (double)ydots;

    steps = frames;
    Divisor = TWO_PI / (steps - 1);
    if (AspectRatio > 1.0)	// take aspect ration into account when plotting Julia
	{
	mandel_width = 4.0 * ScaleFactor;
	vert = -2.0 * ScaleFactor;
	hor = vert * AspectRatio;
	}
    else
	{
	mandel_width = 4.0  * ScaleFactor / AspectRatio;
	hor = -2.0 * ScaleFactor;
	vert = hor / AspectRatio;
	}

    BigNumFlag = FALSE;							// we are starting at the shallow end of the pool
    juliaflag = TRUE;

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Julia Set Animation: %ld %d %d %d\n", threshold, frames, TrueCol.ScriptPaletteFlag, PaletteShift);
    fprintf(out, "Palette=\n");
    for (i = 0, k = 0; i < threshold; i++, k++)
	{
	if (k == 20)							// group into lumps of 20
	    {
	    k = 0;
	    fprintf(out, "\n");
	    }
	ConvertRGB2ASCII(*(TrueCol.PalettePtr + i * 3 + 0), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3 + 2), ascii);
	fprintf(out, "%s", ascii);
	}
    fprintf(out, "\n");

    for (i = 0; i < steps; i++) 
	{
	    {
	    j.x = cos(Divisor * (double)i) * radius + CentreX;
	    j.y = sin(Divisor * (double)i) * radius + CentreY;
	    fprintf(out, "-j%24.24f,%24.24f,\n", j.x, j.y);
	    }
	}
    fclose(out);
    return 0;
    }

/**************************************************************************
	General Linear Julia script generator
**************************************************************************/

int	GenJuliaScript(HWND hwnd, char *filename, int PaletteShift)

    {
    int		steps, i, k;
    double	DivisorX, DivisorY;
    char	ascii[6];

    char	s[120];
    FILE	*out;
    double	AspectRatio = (double)xdots / (double)ydots;

    steps = frames;
    DivisorX = (EndX - StartX) / steps;
    DivisorY = (EndY - StartY) / steps;
    if (AspectRatio > 1.0)	// take aspect ration into account when plotting Julia
	{
	mandel_width = 4.0 * ScaleFactor;
	vert = -2.0 * ScaleFactor;
	hor = vert * AspectRatio;
	}
    else
	{
	mandel_width = 4.0  * ScaleFactor / AspectRatio;
	hor = -2.0 * ScaleFactor;
	vert = hor / AspectRatio;
	}
    
    BigNumFlag = FALSE;							// we are starting at the shallow end of the pool
    juliaflag = TRUE;

    j.x = StartX;
    j.y = StartY;

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Julia Set Animation: %ld %d %d %d\n", threshold, frames, TrueCol.ScriptPaletteFlag, PaletteShift);
    fprintf(out, "Palette=\n");
    for (i = 0, k = 0; i < threshold; i++, k++)
	{
	if (k == 20)							// group into lumps of 20
	    {
	    k = 0;
	    fprintf(out, "\n");
	    }
	ConvertRGB2ASCII(*(TrueCol.PalettePtr + i * 3 + 0), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3 + 2), ascii);
	fprintf(out, "%s", ascii);
	}
    fprintf(out, "\n");

    for (i = steps; i > 0; i--) 
	{
	    {
	    fprintf(out, "-j%24.24f,%24.24f,\n", j.x, j.y);
	    j.x += DivisorX;
	    j.y += DivisorY;
	    }
	}
    fclose(out);
    return 0;
    }

/**************************************************************************
	General Linear Julia script generator
**************************************************************************/

#define	STEPS	10000

int	displayCurve(HWND hwnd, int JuliaAnimType) 

    {
    int	i;
    double	Divisor, DivisorX, DivisorY;
    RGBTRIPLE	rgb = {255, 255, 255};

    BigNumFlag = FALSE;					// we are starting at the shallow end of the pool
    juliaflag = FALSE;

    if (JuliaAnimType == 0)				// linear
	{
	DivisorX = (EndX - StartX) / STEPS;
	DivisorY = (EndY - StartY) / STEPS;
	j.x = StartX;
	j.y = StartY;

	for (i = 0; i < STEPS; i++) 
	    {
	    Plot.OutRGBpoint((WORD)((j.x - hor) / xgap), (WORD)((double)ydots - (j.y - vert) / ygap), rgb);
	    j.x += DivisorX;
	    j.y += DivisorY;
	    }
	}
    else
	{
	Divisor = TWO_PI / STEPS;
	for (i = 0; i < STEPS; i++) 
	    {
	    if (JuliaAnimType == 1)			// circular
		{
		j.x = cos(Divisor * (double)i) * radius + CentreX;
		j.y = sin(Divisor * (double)i) * radius + CentreY;
		}
	    else					// cardioid
		{
		j.x = Magnitude * ((double)degree * cos(Divisor * (double)i) - (cos((double)degree * Divisor * (double)i)));
		j.y = Magnitude * ((double)degree * sin(Divisor * (double)i) - (sin((double)degree * Divisor * (double)i)));
//		j.x = (0.25 - cos(Divisor * (double)i) * (1.0 + cos(Divisor * (double)i)) * Magnitude);
//		j.y = (sin(Divisor * (double)i) * (1.0 + cos(Divisor * (double)i)) * Magnitude);
		}
	    Plot.OutRGBpoint((WORD)((j.x - hor) / xgap), (WORD)((double)ydots - (j.y - vert) / ygap), rgb);
	    }
	}

    InvalidateRect(hwnd, NULL, FALSE);
    return 0;
    }

/**************************************************************************
	Dialog For Setting Up Julia Animation
**************************************************************************/

DLGPROC FAR PASCAL JuliaAnimDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static     UINT	tempParam;
     static     BYTE	temp;
     static     BYTE	temp_special;
     static     char	buf[1024]; 
     BOOL		TempCheck;
     static	char	TempFile[MAX_PATH];
     char		s[500];
     BOOL		bTrans ;
     char		*fileptr;
     HWND		hCtrl;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        if (type != POWER)
		    {
		    degree = 2;
		    Magnitude = 0.249;
		    }
		else
		    Magnitude = 0.93 / ((double) degree + 2);

	        SetWindowText (hDlg, "Julia Animation Setup");
		CheckRadioButton(hDlg, IDC_LINEARJUL, IDC_CARDIOID, IDC_LINEARJUL + JuliaAnimType);
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, StartImmediately, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT1);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		SetUpFilename(ScriptFileName, "sci", "Julia");
		SetUpFilename(PNGName, "animpng", "Julia");
		SetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
		SetDlgItemInt(hDlg, IDC_THRESHOLD_START, threshold, TRUE);
		SetDlgItemInt(hDlg, IDC_FRAMES, frames, TRUE);
		sprintf(s, "%f", ScaleFactor);
		SetDlgItemText(hDlg, IDC_JULIAWIDTH, s);
		hCtrl = GetDlgItem (hDlg, IDC_ORBITS1);
		ShowWindow(hCtrl, SW_SHOWNORMAL);
		SendMessage(hCtrl, BM_SETCHECK, ShowOrbits, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_INVERTRADIUS);
		ShowWindow(hCtrl, SW_HIDE);
		hCtrl = GetDlgItem (hDlg, IDC_INVERTTEXT);
		ShowWindow(hCtrl, SW_HIDE);
		hCtrl = GetDlgItem (hDlg, IDC_ORBITS1);
		SetDlgItemInt(hDlg, IDC_PALETTESHIFT1, PaletteShift, TRUE);
		switch (JuliaAnimType)
		    {
		    case 0:						// Linear
			sprintf(s, "%18.18f", StartX);
			SetDlgItemText(hDlg, IDC_START_X, s);
			sprintf(s, "%18.18f", StartY);
			SetDlgItemText(hDlg, IDC_START_Y, s);
			sprintf(s, "%18.18f", EndX);
			SetDlgItemText(hDlg, IDC_END_X, s);
			sprintf(s, "%18.18f", EndY);
			SetDlgItemText(hDlg, IDC_END_Y, s);
			SetDlgItemText(hDlg, IDC_LABEL01, "Start X       ");
			SetDlgItemText(hDlg, IDC_LABEL02, "Start Y       ");
			SetDlgItemText(hDlg, IDC_LABEL03, "End X         ");
			SetDlgItemText(hDlg, IDC_LABEL04, "End Y         ");
			hCtrl = GetDlgItem (hDlg, IDC_START_X);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_START_Y);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_END_X);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_END_Y);
			EnableWindow (hCtrl, TRUE);
			break;
		    case 1:						// Circular
			sprintf(s, "%18.18f", CentreX);
			SetDlgItemText(hDlg, IDC_START_X, s);
			sprintf(s, "%18.18f", CentreY);
			SetDlgItemText(hDlg, IDC_START_Y, s);
			sprintf(s, "     ");
			SetDlgItemText(hDlg, IDC_END_X, s);
			sprintf(s, "%18.18f", radius);
			SetDlgItemText(hDlg, IDC_END_Y, s);
			SetDlgItemText(hDlg, IDC_LABEL01, "Centre X      ");
			SetDlgItemText(hDlg, IDC_LABEL02, "Centre Y      ");
			SetDlgItemText(hDlg, IDC_LABEL03, "              ");
			SetDlgItemText(hDlg, IDC_LABEL04, "Radius        ");
			hCtrl = GetDlgItem (hDlg, IDC_START_X);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_START_Y);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_END_X);
			EnableWindow (hCtrl, FALSE);
			hCtrl = GetDlgItem (hDlg, IDC_END_Y);
			EnableWindow (hCtrl, TRUE);
			break;
		    case 2:						// Cardioid
			sprintf(s, "     ");
			SetDlgItemText(hDlg, IDC_START_X, s);
			sprintf(s, "     ");
			SetDlgItemText(hDlg, IDC_START_Y, s);
			sprintf(s, "     ");
			SetDlgItemText(hDlg, IDC_END_X, s);
			sprintf(s, "%18.18f", Magnitude);
			SetDlgItemText(hDlg, IDC_END_Y, s);
			SetDlgItemText(hDlg, IDC_LABEL01, "              ");
			SetDlgItemText(hDlg, IDC_LABEL02, "              ");
			SetDlgItemText(hDlg, IDC_LABEL03, "              ");
			SetDlgItemText(hDlg, IDC_LABEL04, "Scale         ");
			hCtrl = GetDlgItem (hDlg, IDC_START_X);
			EnableWindow (hCtrl, FALSE);
			hCtrl = GetDlgItem (hDlg, IDC_START_Y);
			EnableWindow (hCtrl, FALSE);
			hCtrl = GetDlgItem (hDlg, IDC_END_X);
			EnableWindow (hCtrl, FALSE);
			hCtrl = GetDlgItem (hDlg, IDC_END_Y);
			EnableWindow (hCtrl, TRUE);
			break;
		    }

	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_WRITEMPEGDIRECT1:
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT1);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (WriteMPEGFrames)					// no point in writing other forms if direct to MPEG
			    {
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WritePNGFrames = WriteMemFrames = WritePNGList = FALSE;
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
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT1);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WriteMPEGFrames = FALSE;
			    }
//			sprintf(PNGFile, "%s", GenerateAnimFileName (ANIMPNGPath, PNGName));
//			SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGFile);
			return (DLGPROC)TRUE;

		    case IDC_LINEARJUL:
			JuliaAnimType = LOWORD(wParam) - IDC_LINEARJUL;
			CheckRadioButton(hDlg, IDC_LINEARJUL, IDC_CARDIOID, (int) LOWORD(wParam));
			sprintf(s, "%18.18f", StartX);
			SetDlgItemText(hDlg, IDC_START_X, s);
			sprintf(s, "%18.18f", StartY);
			SetDlgItemText(hDlg, IDC_START_Y, s);
			sprintf(s, "%18.18f", EndX);
			SetDlgItemText(hDlg, IDC_END_X, s);
			sprintf(s, "%18.18f", EndY);
			SetDlgItemText(hDlg, IDC_END_Y, s);
			SetDlgItemText(hDlg, IDC_LABEL01, "Start X       ");
			SetDlgItemText(hDlg, IDC_LABEL02, "Start Y       ");
			SetDlgItemText(hDlg, IDC_LABEL03, "End X         ");
			SetDlgItemText(hDlg, IDC_LABEL04, "End Y         ");
			hCtrl = GetDlgItem (hDlg, IDC_START_X);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_START_Y);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_END_X);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_END_Y);
			EnableWindow (hCtrl, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_CIRCULAR:
			JuliaAnimType = LOWORD(wParam) - IDC_LINEARJUL;
			CheckRadioButton(hDlg, IDC_LINEARJUL, IDC_CARDIOID, (int) LOWORD(wParam));
			sprintf(s, "%18.18f", CentreX);
			SetDlgItemText(hDlg, IDC_START_X, s);
			sprintf(s, "%18.18f", CentreY);
			SetDlgItemText(hDlg, IDC_START_Y, s);
			sprintf(s, "     ");
			SetDlgItemText(hDlg, IDC_END_X, s);
			sprintf(s, "%18.18f", radius);
			SetDlgItemText(hDlg, IDC_END_Y, s);
			SetDlgItemText(hDlg, IDC_LABEL01, "Centre X      ");
			SetDlgItemText(hDlg, IDC_LABEL02, "Centre Y      ");
			SetDlgItemText(hDlg, IDC_LABEL03, "              ");
			SetDlgItemText(hDlg, IDC_LABEL04, "Radius        ");
			hCtrl = GetDlgItem (hDlg, IDC_START_X);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_START_Y);
			EnableWindow (hCtrl, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_END_X);
			EnableWindow (hCtrl, FALSE);
			hCtrl = GetDlgItem (hDlg, IDC_END_Y);
			EnableWindow (hCtrl, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_CARDIOID:
			JuliaAnimType = LOWORD(wParam) - IDC_LINEARJUL;
			CheckRadioButton(hDlg, IDC_LINEARJUL, IDC_CARDIOID, (int) LOWORD(wParam));
			sprintf(s, "     ");
			SetDlgItemText(hDlg, IDC_START_X, s);
			sprintf(s, "     ");
			SetDlgItemText(hDlg, IDC_START_Y, s);
			sprintf(s, "     ");
			SetDlgItemText(hDlg, IDC_END_X, s);
			sprintf(s, "%18.18f", Magnitude);
			SetDlgItemText(hDlg, IDC_END_Y, s);
			SetDlgItemText(hDlg, IDC_LABEL01, "              ");
			SetDlgItemText(hDlg, IDC_LABEL02, "              ");
			SetDlgItemText(hDlg, IDC_LABEL03, "              ");
			SetDlgItemText(hDlg, IDC_LABEL04, "Scale         ");
			hCtrl = GetDlgItem (hDlg, IDC_START_X);
			EnableWindow (hCtrl, FALSE);
			hCtrl = GetDlgItem (hDlg, IDC_START_Y);
			EnableWindow (hCtrl, FALSE);
			hCtrl = GetDlgItem (hDlg, IDC_END_X);
			EnableWindow (hCtrl, FALSE);
			hCtrl = GetDlgItem (hDlg, IDC_END_Y);
			EnableWindow (hCtrl, TRUE);
		        return (DLGPROC)TRUE ;
			    
		    case IDC_TESTCURVE:
			switch (JuliaAnimType)
			    {
			    case 0:
				GetDlgItemText(hDlg, IDC_START_X, s, 200);
				sscanf(s, "%lf", &StartX);
				GetDlgItemText(hDlg, IDC_START_Y, s, 200);
				sscanf(s, "%lf", &StartY);
				GetDlgItemText(hDlg, IDC_END_X, s, 200);
				sscanf(s, "%lf", &EndX);
				GetDlgItemText(hDlg, IDC_END_Y, s, 200);
				sscanf(s, "%lf", &EndY);
				break;
			    case 1:
				GetDlgItemText(hDlg, IDC_START_X, s, 200);
				sscanf(s, "%lf", &CentreX);
				GetDlgItemText(hDlg, IDC_START_Y, s, 200);
				sscanf(s, "%lf", &CentreY);
				GetDlgItemText(hDlg, IDC_END_Y, s, 200);
				sscanf(s, "%lf", &radius);
				break;
			    case 2:
				GetDlgItemText(hDlg, IDC_END_Y, s, 200);
				sscanf(s, "%lf", &Magnitude);
				break;
			    }
			displayCurve(PixelHwnd, JuliaAnimType);				// write to main window
		        return (DLGPROC)TRUE ;

		    case IDOK:
			GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName, MAX_PATH);
			fileptr = PNGName + strlen(PNGName);
			while (fileptr > PNGName && *fileptr != '.')
			    fileptr--;	
			if (*fileptr == '.')						// remove any extension
			    *fileptr = '\0';
			
			GetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName, MAX_PATH);
			fileptr = ScriptFileName + strlen(ScriptFileName);
			while (fileptr > ScriptFileName && *fileptr != '.')
			    fileptr--;							// remove extension
			if (*fileptr == '.')
			    *fileptr = '\0';
			strcat(ScriptFileName, ".sci");

			hCtrl = GetDlgItem (hDlg, IDC_ORBITS1);
			ShowOrbits = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);

			threshold = GetDlgItemInt(hDlg, IDC_THRESHOLD_START, &bTrans, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
			StartImmediately = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			WritePNGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			WriteMemFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			WritePNGList = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			frames = GetDlgItemInt(hDlg, IDC_FRAMES, &bTrans, TRUE);
			GetDlgItemText(hDlg, IDC_JULIAWIDTH, s, 200);
			sscanf(s, "%lf", &ScaleFactor);
			PaletteShift = GetDlgItemInt(hDlg, IDC_PALETTESHIFT1, &bTrans, TRUE);

			if (frames <= 0)
			    frames = 20;
			if (WriteMPEGFrames)						// generate MPEG filename
			    {
			    GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, TempFile, MAX_PATH);
			    sprintf(MPGFile, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }

			switch (JuliaAnimType)
			    {
			    case 0:
				GetDlgItemText(hDlg, IDC_START_X, s, 200);
				sscanf(s, "%lf", &StartX);
				GetDlgItemText(hDlg, IDC_START_Y, s, 200);
				sscanf(s, "%lf", &StartY);
				GetDlgItemText(hDlg, IDC_END_X, s, 200);
				sscanf(s, "%lf", &EndX);
				GetDlgItemText(hDlg, IDC_END_Y, s, 200);
				sscanf(s, "%lf", &EndY);
				GenJuliaScript(hDlg, ScriptFileName, PaletteShift);
				break;
			    case 1:
				GetDlgItemText(hDlg, IDC_START_X, s, 200);
				sscanf(s, "%lf", &CentreX);
				GetDlgItemText(hDlg, IDC_START_Y, s, 200);
				sscanf(s, "%lf", &CentreY);
				GetDlgItemText(hDlg, IDC_END_Y, s, 200);
				sscanf(s, "%lf", &radius);
				CircleJuliaScript(hDlg, ScriptFileName, PaletteShift);
				break;
			    case 2:
				GetDlgItemText(hDlg, IDC_END_Y, s, 200);
				sscanf(s, "%lf", &Magnitude);
				CardioidJuliaScript(hDlg, ScriptFileName, PaletteShift);
				break;
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


