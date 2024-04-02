/*
    FRACZOOM.CPP a program to animate the Mandelbrot set.
    
    Written in Microsoft Visual C++ by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "manpwin.h"
#include "manp.h"
#include "anim.h"
#include "fractype.h"
#include "resource.h"
#include "BigDouble.h"
#include "big.h"
#include "colour.h"

extern	double	hor;			// horizontal address
extern	double	vert;			// vertical address
extern	double	mandel_width;		// final width
extern	double	ScreenRatio;		// ratio of width / height for the screen
extern	BYTE	BigNumFlag;		// True if bignum used
extern	long	threshold;
extern	int	time_to_restart;	// time to restart?

extern	WORD	special;		// special colour, phase etc
extern	int	period_level;		/* 0 for no periodicity checking */
extern	BYTE	screenflag;		/* replay saved screen */
extern	int	biomorph;		/* biomorph colour */
extern	BYTE	cycleflag;		/* do colour cycling */
extern	int	decomp;			/* number of decomposition colours */
extern	BYTE	orbit_flag;		/* display orbits? */
extern	BYTE	exitflag;		/* exit on completion */
extern	BYTE	juliaflag;		/* Julia implementation of fractal */
extern	char	floatflag;		/* floating point maths */
extern	BYTE	grayflag;		/* use grey value not colour number */
extern	BYTE	pairflag;		/* stereo pair flag and window size */
extern	BYTE	orig_palette[];		/* loaded palette */
extern	CTrueCol    TrueCol;		// palette info
extern	WORD	type;			/* M=mand, N=Newton etc */
extern	int	xdots, ydots;
extern	char	SCIPath[];		// path for SCI files
extern	double	param[];

// Big num declarations **********************************************************
extern	int	decimals, precision;
extern	BYTE	BigNumFlag;		// True if bignum used
extern	BigDouble   BigHor, BigVert, BigWidth;
extern	int getprecbf_mag(void);
// Big num declarations **********************************************************

extern	int	analyse_corner(char *);
//extern	void	calcfracinit(void);
extern	void	BasicFractData(char *, BOOL);
extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	void	ConvertBignum2String(char *s, mpfr_t num);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);

char	*AnimData(void);

double		StartWidth = 4.0;		// starting width
static	int	PaletteShift = 0;
BOOL		StartImmediately = TRUE;
static	int	StartIter = 250;
static	int	EndIter = 250;
static	int	frames = 100;
extern	char	MPGPath[];			// path for MPEG files
extern	char	MPGFile[];			// MPEG file
extern	char	ANIMPNGPath[];			// path for animated PNG files and LST files
extern	char	PNGFile[];			// PNG file
	char	PNGName[MAX_PATH] = "Manp";	// base name for PNG file sequence
	char	ScriptFileName[MAX_PATH] = "Manp";	// base name for script file 
	BOOL	WritePNGFrames = FALSE;		// write frames to PNG files
	BOOL	WriteMemFrames = TRUE;		// write frames to memory
	BOOL	WritePNGList = FALSE;		// write PNG filenames to a *lst file
	BOOL	WriteMPEGFrames = FALSE;	// write frames directly to an MPEG file

/**************************************************************************
	Main script generator
**************************************************************************/

int	GenZoomScript(HWND hwnd, char *filename) 

    {
    int		steps, i, k;
    char	ascii[6];
    double	temp, divisor, current_width, centrex, centrey;
    double	x1, y1;
    double	x, y, wi, wo;
    BOOL	WasBig = FALSE;
// local Big num declarations **********************************************************
    BigDouble	Big_centrex, Big_centrey, Big_StartWidth, Big_divisor, Big_CurrentWidth;
    BigDouble	Big_x1, Big_y1, Big_Scale, BigTemp;
// local Big num declarations **********************************************************
    char	s[120];
    char	*s1, *s2, *s3;
    FILE	*out;
    long	LocalThreshold;
    errno_t	err;

    LocalThreshold = (EndIter >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : EndIter;

    x = hor;
    y = vert;
    wi = StartWidth;
    wo = mandel_width;

    steps = frames;
    centrex = x + (wo * (ScreenRatio / 2.0));
    centrey = y + (wo / 2.0);
    temp = (log(wi / wo)) / steps;
    divisor = exp(temp);
    current_width = wi;

    if (BigNumFlag)
	{
	Big_StartWidth = StartWidth;			// get StartWidth
	Big_Scale = ScreenRatio/2.0;			// get centrex
	Big_centrex = BigWidth * Big_Scale + BigHor;
	Big_centrey = BigWidth /2.0 + BigVert;		// get centrey
	BigTemp = Big_StartWidth / BigWidth;
	BigTemp = BigTemp.BigLog();
	BigTemp = BigTemp / steps;
	mpfr_exp(Big_divisor.x, BigTemp.x, MPFR_RNDN);	// a = exp(a);
	divisor = (double)mpfr_get_d(Big_divisor.x, MPFR_RNDN);
	}
    BigNumFlag = FALSE;				// we are starting at the shallow end of the pool

    if ((out = fopen(filename, "w")) == NULL)
	{
	_get_errno(&err);
	sprintf(s, "Cannot open output file %s, Error number=%d\nDoes Folder exist?", filename, err);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%d -s\"%s\" %s\n", StartIter, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, "%d %d %d %d\n", EndIter, frames, TrueCol.ScriptPaletteFlag, PaletteShift);
//    fprintf(out, "%d %d %d %d\n", EndIter, frames, xdots, ydots);
    fprintf(out, "Palette=\n");
    for (i = 0, k = 0; i < LocalThreshold; i++, k++)
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
	mandel_width = current_width;		// to ensure getprecbf_mag() gets the correct precision
	precision = getprecbf_mag();
	if (precision < 0)			// exceeded allowable precision
	    {
	    sprintf(s, "Cannot calculate precision for %s", filename);
	    MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    return -1;
	    }

	if (precision > DBL_DIG - 3)
	    {
	    if (!BigNumFlag)
		{
		Big_CurrentWidth = current_width;
		BigNumFlag = TRUE;
		}

	    Big_x1 = Big_centrex - (Big_CurrentWidth * Big_Scale);
	    Big_y1 = Big_centrey - (Big_CurrentWidth / 2.0);
	    s1 = new char[SIZEOF_BF_VARS];
	    s2 = new char[SIZEOF_BF_VARS];
	    s3 = new char[SIZEOF_BF_VARS];
	    ConvertBignum2String(s1, Big_x1.x);
	    ConvertBignum2String(s2, Big_y1.x);
	    ConvertBignum2String(s3, Big_CurrentWidth.x);
	    *(s1 + decimals + 5) = '\0';					// no need to print too many decimals
	    *(s2 + decimals + 5) = '\0';
	    *(s3 + decimals + 5) = '\0';
	    fprintf(out, "-c%s,%s,%s\n", s1, s2, s3);
	    if (s1) { delete[] s1; s1 = NULL; }
	    if (s2) { delete[] s2; s2 = NULL; }
	    if (s3) { delete[] s3; s3 = NULL; }
	    Big_CurrentWidth = Big_CurrentWidth / Big_divisor;
	    }
	else
	    {
	    x1 = centrex - (current_width * (ScreenRatio / 2.0));
	    y1 = centrey - (current_width / 2.0);
	    fprintf(out, "-c%24.24f,%24.24f,%1.24e\n", x1, y1, current_width);
	    current_width /= divisor;
	    }
	}
    fclose(out);
    return 0;
    }

/**************************************************************************
	Fractal data to string
**************************************************************************/

char	*AnimData(void)

    {
//char	s[400];
    static	char	info[MAXDATALINE];

    *info = '\0';
    BasicFractData(info, TRUE);
    return info;
    }

/**************************************************************************
	Dialog Control for animation
**************************************************************************/

DLGPROC FAR PASCAL AnimationDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
//     static	HANDLE	hCursor;
     static     UINT	tempParam, VariableSize;
     static     BYTE	temp_special;
     static	char	*buf = NULL;
     static	char	*s = NULL;
     static	char	*s1 = NULL;
     static	char	*s2 = NULL;
     static	char	*s3 = NULL;
     static	char	TempFile[MAX_PATH];
     BOOL		bTrans ;
     BOOL		TempCheck;
     char		*fileptr;
     HWND		hCtrl;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        VariableSize = max(decimals + PRECISION_FACTOR, DBL_MANT_DIG);	// dec = 0 if not BigNum
		cycleflag = FALSE;
		s = new char[SIZEOF_BF_VARS];
		s1 = new char[SIZEOF_BF_VARS];
		s2 = new char[SIZEOF_BF_VARS];
		s3 = new char[SIZEOF_BF_VARS];
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, StartImmediately, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
//		sprintf(ScriptFileName, "%s%s\\Manp", ManpPath, SCIPath);
		SetUpFilename(ScriptFileName, "sci", "Zoom");
		SetUpFilename(PNGName, "animpng", "Zoom");
		SetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName);
//		sprintf(PNGName, "%s%s\\Manp", ManpPath, ANIMPNGPath);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
		if (BigNumFlag)
		    {
		    ConvertBignum2String(s1, BigHor.x);
		    ConvertBignum2String(s2, BigVert.x);
		    mpfr_sprintf(s3, "%.12Re", BigWidth.x);
		    }
		else
		    {
		    sprintf(s1, "%18.18f", hor);
		    sprintf(s2, "%18.18f", vert);
		    sprintf(s3, "%1.12e", mandel_width);
		    }
		SetDlgItemText(hDlg, IDC_HOR, s1);
		SetDlgItemText(hDlg, IDC_VERT, s2);
		SetDlgItemText(hDlg, IDC_WIDTH_END, s3);
	        sprintf(s, "%18.18f", StartWidth);
		SetDlgItemText(hDlg, IDC_WIDTH_START, s);
		SetDlgItemInt(hDlg, IDC_THRESHOLD_START, StartIter, TRUE);
		SetDlgItemInt(hDlg, IDC_THRESHOLD_END, threshold, TRUE);
		PaletteShift = 0;
		SetDlgItemInt(hDlg, IDC_PALETTESHIFT, PaletteShift, TRUE);
//		SetDlgItemInt(hDlg, IDC_THRESHOLD_END, EndIter, TRUE);
		SetDlgItemInt(hDlg, IDC_FRAMES, frames, TRUE);
	        return (DLGPROC)TRUE ;

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
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WriteMPEGFrames = FALSE;
			    }
//			sprintf(PNGFile, "%s", GenerateAnimFileName (ANIMPNGPath, PNGName));
//			SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGFile);
			return (DLGPROC)TRUE;

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

			StartIter = GetDlgItemInt(hDlg, IDC_THRESHOLD_START, &bTrans, TRUE);
			EndIter = GetDlgItemInt(hDlg, IDC_THRESHOLD_END, &bTrans, TRUE);
			PaletteShift = GetDlgItemInt(hDlg, IDC_PALETTESHIFT, &bTrans, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
			StartImmediately = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			WritePNGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			WriteMemFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			WritePNGList = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (StartIter <= 0 || EndIter <= 0)
			    {
			    StartIter =  threshold;
			    EndIter =  threshold;
			    }

			if (WriteMPEGFrames)						// generate MPEG filename
			    {
			    GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, TempFile, MAX_PATH);
			    sprintf(MPGFile, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }

			frames = GetDlgItemInt(hDlg, IDC_FRAMES, &bTrans, TRUE);
			if (frames <= 0)
			    frames = 20;
			if (frames > MAXANIM)
			    frames = MAXANIM;
			GetDlgItemText(hDlg, IDC_HOR, s1, VariableSize);
			GetDlgItemText(hDlg, IDC_VERT, s2, VariableSize);
			GetDlgItemText(hDlg, IDC_WIDTH_START, s, VariableSize);
			sscanf(s, "%lf", &StartWidth);
			GetDlgItemText(hDlg, IDC_WIDTH_END, s3, VariableSize);
			buf = new char[SIZEOF_BF_VARS * 3];
			sprintf(buf, "%s,%s,%s, %f, %f", s1, s2, s3, param[0], param[1]);	// ensure that real and imag perturbation is unchanged
			if (analyse_corner(buf) < 0)
			    {
			    double  AspectRatio = (double)xdots / (double)ydots;
			    BigNumFlag = FALSE;
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
			    sprintf(s, "Deep Zooming Limit (%d decimals) exceeded", SIZEOF_BF_VARS);
			    MessageBox (hDlg, s, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			    MessageBeep (0);
			    }
			else
			    GenZoomScript(hDlg, ScriptFileName);
			if (s1) delete [] s1;
			if (s2) delete [] s2;
			if (s3) delete [] s3;
			if (s) delete [] s;
			if (buf) delete [] buf;
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			if (s1) delete [] s1;
			if (s2) delete [] s2;
			if (s3) delete [] s3;
			if (s) delete [] s;
			EndDialog (hDlg, FALSE);
			time_to_restart = FALSE;
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }
