/*
    ANIINVERT.CPP a program to animate the Inversion.
    
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
extern	int	invert;			/* Inversion implementation of fractal */
extern	char	floatflag;		/* floating point maths */
extern	BYTE	pairflag;		/* stereo pair flag and window size */
extern	BYTE	orig_palette[];		/* loaded palette */
extern	WORD	type;			/* M=mand, N=Newton etc */
extern	int	subtype;		
extern	char	SCIPath[];		// path for SCI files

extern	BOOL	WritePNGFrames;		// write frames to PNG files
extern	BOOL	WriteMemFrames;		// write frames to memory
extern	BOOL	WritePNGList;		// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;	// write frames directly to an MPEG file

extern	char	*AnimData(void);
extern	BOOL	StartImmediately;
static	int	frames = 100;
static	double	ScaleFactor = 1.0;
static	int	PaletteShift = 0;
static	int	InvertAnimType = 0;	// 0 = linear
					// 1 = circular
					// 2 = cardoid

extern	char	PNGName[];		// base name for PNG file sequence
extern	char	ScriptFileName[];	// base name for script file 
extern	char	MPGPath[];		// path for MPEG files
extern	char	MPGFile[];		// MPEG file
extern	char	ANIMPNGPath[];		// path for animated PNG files and LST files
extern	char	PNGFile[];		// PNG file

static	double	StartX = -2.0;		// starting constants
static	double	StartY = 0.0;
static	double	EndX = 2.0;
static	double	EndY = 0.0;

static	double	CentreX = -1.0;
static	double	CentreY = 0.0;
static	double	radius = 0.25;

static	double	Magnitude = 0.49;		// 0.5 sits neatly inside the Mandelbrot set. 0.49 gives great orbits
static	double	InvertRadius = 1.0;		// inversion radius

extern	double	xgap;			// gap between pixels
extern	double	ygap;			// gap between pixels

extern	int	xdots, ydots;

extern	CDib	Dib;			// Device Independent Bitmap

extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	int	displayCurve(HWND hwnd, int JuliaAnimType);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);

/**************************************************************************
	Cardiod Inversion script generator
**************************************************************************/

int	CardioidInversionScript(HWND hwnd, char *filename) 

    {
    int		steps, i, k;
    double	Divisor;
    char	ascii[6];
    Complex	a;

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
    invert = TRUE;

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Inversion Animation: %ld %d\n", threshold, frames);
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
	    a.x = (0.25 - cos(Divisor * (double)i) * (1.0 + cos(Divisor * (double)i)) * Magnitude);
	    a.y = (sin(Divisor * (double)i) * (1.0 + cos(Divisor * (double)i)) * Magnitude);
	    fprintf(out, "-x%6.6f,%24.24f,%24.24f,\n", InvertRadius, a.x, a.y);
	    }
	}

    fclose(out);
    return 0;
    }

/**************************************************************************
	Circle Inversion script generator
**************************************************************************/

int	CircleInversionScript(HWND hwnd, char *filename) 

    {
    int		steps, i, k;
    double	Divisor;
    char	ascii[6];
    Complex	a;

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
    invert = TRUE;

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Inversion Animation: %ld %d %d %d\n", threshold, frames, TrueCol.ScriptPaletteFlag, PaletteShift);
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
	    a.x = cos(Divisor * (double)i) * radius + CentreX;
	    a.y = sin(Divisor * (double)i) * radius + CentreY;
	    fprintf(out, "-x%6.6f,%24.24f,%24.24f,\n", InvertRadius, a.x, a.y);
	    }
	}
    fclose(out);
    return 0;
    }

/**************************************************************************
	General Linear Inversion script generator
**************************************************************************/

int	GenInversionScript(HWND hwnd, char *filename) 

    {
    int		steps, i, k;
    double	DivisorX, DivisorY;
    char	ascii[6];
    Complex	a;

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
    invert = TRUE;

    a.x = StartX;
    a.y = StartY;

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Inversion Animation: %ld %d %d %d\n", threshold, frames, TrueCol.ScriptPaletteFlag, PaletteShift);
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
	    fprintf(out, "-x%6.6f,%24.24f,%24.24f,\n", InvertRadius, a.x, a.y);
	    a.x += DivisorX;
	    a.y += DivisorY;
	    }
	}
    fclose(out);
    return 0;
    }

/**************************************************************************
	Dialog For Setting Up Inversion Animation
**************************************************************************/

DLGPROC FAR PASCAL InversionAnimDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
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
		    Magnitude = 0.93 / ((double)degree + 2);

		SetWindowText (hDlg, "Inversion Animation Setup");
		CheckRadioButton(hDlg, IDC_LINEARJUL, IDC_CARDIOID, IDC_LINEARJUL + InvertAnimType);
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
		SetUpFilename(ScriptFileName, "sci", "Invert");
		SetUpFilename(PNGName, "animpng", "Invert");
		SetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
		SetDlgItemInt(hDlg, IDC_THRESHOLD_START, threshold, TRUE);
		SetDlgItemInt(hDlg, IDC_FRAMES, frames, TRUE);
		sprintf(s, "%f", ScaleFactor);
		SetDlgItemText(hDlg, IDC_JULIAWIDTH, s);
		hCtrl = GetDlgItem (hDlg, IDC_ORBITS1);
		ShowWindow(hCtrl, SW_HIDE);
		hCtrl = GetDlgItem (hDlg, IDC_INVERTRADIUS);
		ShowWindow(hCtrl, SW_SHOWNORMAL);
		sprintf(s, "%f", InvertRadius);
		SetDlgItemText(hDlg, IDC_INVERTRADIUS, s);
		hCtrl = GetDlgItem (hDlg, IDC_INVERTTEXT);
		ShowWindow(hCtrl, SW_SHOWNORMAL);
		SetDlgItemText(hDlg, IDC_INVERTTEXT, "Inv Radius");
		SetDlgItemInt(hDlg, IDC_PALETTESHIFT1, PaletteShift, TRUE);
		switch (InvertAnimType)
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
			InvertAnimType = LOWORD(wParam) - IDC_LINEARJUL;
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
			InvertAnimType = LOWORD(wParam) - IDC_LINEARJUL;
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
			InvertAnimType = LOWORD(wParam) - IDC_LINEARJUL;
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
			switch (InvertAnimType)
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
			displayCurve(PixelHwnd, InvertAnimType);			// write to main window
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

//			hCtrl = GetDlgItem (hDlg, IDC_ORBITS1);
//			ShowOrbits = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);

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
			GetDlgItemText(hDlg, IDC_INVERTRADIUS, s, 200);
			sscanf(s, "%lf", &InvertRadius);
			PaletteShift = GetDlgItemInt(hDlg, IDC_PALETTESHIFT1, &bTrans, TRUE);

			if (frames <= 0)
			    frames = 20;
			if (WriteMPEGFrames)						// generate MPEG filename
			    {
			    GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, TempFile, MAX_PATH);
			    sprintf(MPGFile, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }

			switch (InvertAnimType)
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
				GenInversionScript(hDlg, ScriptFileName);
				break;
			    case 1:
				GetDlgItemText(hDlg, IDC_START_X, s, 200);
				sscanf(s, "%lf", &CentreX);
				GetDlgItemText(hDlg, IDC_START_Y, s, 200);
				sscanf(s, "%lf", &CentreY);
				GetDlgItemText(hDlg, IDC_END_Y, s, 200);
				sscanf(s, "%lf", &radius);
				CircleInversionScript(hDlg, ScriptFileName);
				break;
			    case 2:
				GetDlgItemText(hDlg, IDC_END_Y, s, 200);
				sscanf(s, "%lf", &Magnitude);
				CardioidInversionScript(hDlg, ScriptFileName);
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


