/*
    FRACZOOM.CPP - a program to animate the Mandelbrot set.
    
    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "manpwin.h"
#include "manp.h"
#include "fractype.h"
#include "resource.h"
#include "BigDouble.h"
#include "big.h"
#include "colour.h"
#include "SafeStrings.h"

extern	char	SCIPath[];		// path for SCI files

// Big num declarations **********************************************************
int	decimals = 10;
// Big num declarations **********************************************************

extern	void	BasicFractData(StringBuilder& sb, BOOL);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);

char	*AnimData(void);

double		StartWidth = 4.0;		// starting width
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

int	CManp::GenZoomScript(HWND hwnd, char *filename) 
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
    std::vector<char>	s1(SIZEOF_BF_VARS);
    std::vector<char>	s2(SIZEOF_BF_VARS);
    std::vector<char>	s3(SIZEOF_BF_VARS);
    FILE	*out;
    long	LocalThreshold;
    errno_t	err;

    LocalThreshold = (EndIter >= MAXPALETTE) ? MAXPALETTE - 1 : EndIter;

    x = hor;
    y = vert;
    wi = StartWidth;
    wo = mandel_width;

    steps = frames;
    centrex = x + (wo * (AspectRatio / 2.0));
    centrey = y + (wo / 2.0);
    temp = (log(wi / wo)) / steps;
    divisor = exp(temp);
    current_width = wi;

    if (BigNumFlag)
	{
	Big_StartWidth = StartWidth;			// get StartWidth
	Big_Scale = AspectRatio /2.0;			// get centrex
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
	SAFE_SPRINTF(s, "Cannot open output file %s, Error number=%d\nDoes Folder exist?", filename, err);
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
	ConvertRGB2ASCII(TrueCol.PalettePtr[i], ascii);
	fprintf(out, "%s", ascii);
	}
    fprintf(out, "\n");
    for (i = steps; i > 0; i--) 
	{
	mandel_width = current_width;		// to ensure getprecbf_mag() gets the correct precision
	precision = getprecbf_mag();
	if (precision < 0)			// exceeded allowable precision
	    {
	    SAFE_SPRINTF(s, "Cannot calculate precision for %s", filename);
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
	    Big_x1.ToString(s1.data(), (int)s1.size(), false);
	    Big_y1.ToString(s2.data(), (int)s2.size(), false);
	    Big_CurrentWidth.SafeSprintf(s3.data(), (int)s3.size(), "%.20Re");

	    size_t TruncateAt = (size_t)decimals + 5;
	    if (TruncateAt >= s1.size())
		TruncateAt = s1.size() - 1;
	    s1[TruncateAt] = '\0';				// no need to print too many decimals
	    s2[TruncateAt] = '\0';
	    s3[TruncateAt] = '\0';

	    fprintf(out, "-c%s,%s,%s\n", s1.data(), s2.data(), s3.data());
	    Big_CurrentWidth = Big_CurrentWidth / Big_divisor;
	    }
	else
	    {
	    x1 = centrex - (current_width * (AspectRatio / 2.0));
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

    info[0] = '\0';
    StringBuilder sb(info, sizeof(info));

    BasicFractData(sb, TRUE);
    return info;
    }

/**************************************************************************
	Dialog Control for animation
**************************************************************************/

INT_PTR CALLBACK AnimationDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     static     UINT	tempParam;
     static     BYTE	temp_special;
     std::vector<char>	buf(SIZEOF_BF_VARS * 3);
     std::vector<char>	s(SIZEOF_BF_VARS);
     std::vector<char>	s1(SIZEOF_BF_VARS);
     std::vector<char>	s2(SIZEOF_BF_VARS);
     std::vector<char>	s3(SIZEOF_BF_VARS);

     static	char	TempFile[MAX_PATH];
     BOOL		bTrans ;
     BOOL		TempCheck;
     char		*fileptr;
     HWND		hCtrl;

     switch (message)
	  {
	  case WM_INITDIALOG:
		gManp->cycleflag = FALSE;
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, gManp->StartImmediately, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, gManp->TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
		SetUpFilename(ScriptFileName, "sci", "Zoom");
		SetUpFilename(PNGName, "animpng", "Zoom");
		SetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
		if (gManp->BigNumFlag)
		    {
		    gManp->BigHor.ToString(s1.data(), (int)s1.size(), false);
		    gManp->BigVert.ToString(s2.data(), (int)s2.size(), false);
		    gManp->BigWidth.SafeSprintf(s3.data(), (int)s3.size(), "%.12Re");
		    }
		else
		    {
		    _snprintf_s(s1.data(), (int)s1.size(), _TRUNCATE, "%18.18f", gManp->hor);
		    _snprintf_s(s2.data(), (int)s2.size(), _TRUNCATE, "%18.18f", gManp->vert);
		    _snprintf_s(s3.data(), (int)s3.size(), _TRUNCATE, "%1.12e", gManp->mandel_width);
		    }
		SetDlgItemText(hDlg, IDC_HOR, s1.data());
		SetDlgItemText(hDlg, IDC_VERT, s2.data());
		SetDlgItemText(hDlg, IDC_WIDTH_END, s3.data());
		_snprintf_s(s.data(), (int)s.size(), _TRUNCATE, "%18.18f", StartWidth);
		SetDlgItemText(hDlg, IDC_WIDTH_START, s.data());
		SetDlgItemInt(hDlg, IDC_THRESHOLD_START, StartIter, TRUE);
		SetDlgItemInt(hDlg, IDC_THRESHOLD_END, gManp->threshold, TRUE);
		gManp->PaletteShift = 0;						// palette movement between animation frames
		SetDlgItemInt(hDlg, IDC_PALETTESHIFT, gManp->PaletteShift, TRUE);
		SetDlgItemInt(hDlg, IDC_FRAMES, frames, TRUE);
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
			    WritePNGFrames = WriteMemFrames = WritePNGList = FALSE;
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
			strcat_s(ScriptFileName, MAX_PATH, ".sci");

			StartIter = GetDlgItemInt(hDlg, IDC_THRESHOLD_START, &bTrans, TRUE);
			EndIter = GetDlgItemInt(hDlg, IDC_THRESHOLD_END, &bTrans, TRUE);
			gManp->PaletteShift = GetDlgItemInt(hDlg, IDC_PALETTESHIFT, &bTrans, TRUE);
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			gManp->TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
			gManp->StartImmediately = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
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
			    StartIter = gManp->threshold;
			    EndIter = gManp->threshold;
			    }

			if (WriteMPEGFrames)						// generate MPEG filename
			    {
			    GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, TempFile, MAX_PATH);
			    _snprintf_s(MPGFile, _MAX_PATH, _TRUNCATE, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }

			frames = GetDlgItemInt(hDlg, IDC_FRAMES, &bTrans, TRUE);
			if (frames <= 0)
			    frames = 20;
			if (frames > MAXANIM)
			    frames = MAXANIM;
			GetDlgItemText(hDlg, IDC_HOR, s1.data(), (int)s1.size());
			GetDlgItemText(hDlg, IDC_VERT, s2.data(), (int)s2.size());
			GetDlgItemText(hDlg, IDC_WIDTH_START, s.data(), (int)s.size());
			sscanf(s.data(), "%lf", &StartWidth);
			GetDlgItemText(hDlg, IDC_WIDTH_END, s3.data(), (int)s3.size());
			_snprintf_s(buf.data(), (int)buf.size(), _TRUNCATE, "%s,%s,%s, %f, %f", s1.data(), s2.data(), s3.data(), gManp->param[0], gManp->param[1]);	// ensure that real and imag perturbation is unchanged
			if (gManp->analyse_corner(buf.data()) < 0)
			    {
			    gManp->BigNumFlag = FALSE;
			    if (gManp->AspectRatio > 1.0)	// take aspect ration into account when plotting Julia
				{
				gManp->mandel_width = 4.0;
				gManp->vert = -2.0;
				gManp->hor = gManp->vert * gManp->AspectRatio;
				}
			    else
				{
				gManp->mandel_width = 4.0 / gManp->AspectRatio;
				gManp->hor = -2.0;
				gManp->vert = gManp->hor / gManp->AspectRatio;
				}
			    SAFE_SPRINTF(s.data(), "Deep Zooming Limit (%d decimals) exceeded", (int)s.size());
			    MessageBox (hDlg, s.data(), "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			    MessageBeep (0);
			    }
			else
			    gManp->GenZoomScript(hDlg, ScriptFileName);
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			gManp->time_to_restart = FALSE;
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }
