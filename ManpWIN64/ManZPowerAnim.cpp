/*
   MANZPOWERANIM.CPP a module for Fractals by Dr. M. Ecker
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	<windowsx.h>
#include	"manp.h"
#include	"manpwin.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"BigDouble.h"
#include	"anim.h"
#include	"polygon.h"
#include	"colour.h"
#include	"pixel.h"
#include	"preview.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 16
#define NUM_SLIDERS 9

#define	MANDEL_PREVIEW_WIDTH	420
#define	MANDEL_PREVIEW_HEIGHT	310
#define	MANDEL_HOR_OFFSET	520
#define	MANDEL_VERT_OFFSET	48

extern	HWND	GlobalHwnd;				// This is the main windows handle

extern	int	user_data(HWND);
extern	int	power(int base, int n);
extern	double	FloatPower(double base, int n);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);
extern	char	*AnimData(void);
extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);
extern	void	ConvertBignum2String(char *s, mpfr_t num);

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	double	param[];
extern	int	curpass, totpasses;
extern	BOOL	StartImmediately;		// immediate start of animation generation
extern	WORD	type;				// fractal type
extern	int	subtype;
extern	int	SlopeType;

extern	double	lightDirectionDegrees;
extern	double	bumpMappingDepth;
extern	double	bumpMappingStrength;
extern	int	PaletteStart;
extern	double	rqlim;
extern	long	iteration;			// globals for speed
extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts

extern	BOOL	WritePNGFrames;			// write frames to PNG files
extern	BOOL	WriteMemFrames;			// write frames to memory
extern	BOOL	WritePNGList;			// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;		// write frames directly to an MPEG file

extern	char	MPGPath[];			// path for MPEG files
extern	char	MPGFile[];			// MPEG file
extern	char	PNGName[];			// base name for PNG file sequence
extern	char	ANIMPNGPath[];			// path for animated PNG files and LST files
extern	char	PNGFile[];			// PNG file
extern	char	ScriptFileName[];		// base name for script file 

extern	char	PNGName[];			// base name for PNG file sequence

static	double	xscale, yscale;
static	double	StartRate = -1.0, EndRate = 1.0;
static	int	ParamNumber = 0;
static	int	frames = 100;
extern	BOOL	Return2Start;			// do we want to go backwards to the start?

extern	CTrueCol    TrueCol;			// palette info
extern	CFract	Fractal;			// current fractal stuff
extern	PAINTSTRUCT 	ps;
extern	CPixel	Pix;				// routines for escape fractals
extern	CPlot	Plot;				// image plotting routines 

static	CPreview	MandelScreen;
static	BOOL	IsPreview = FALSE;
static	RECT 	rect;
static	double	ParamValues[NUM_SLIDERS];
static	short	level[NUM_SLIDERS] = {0,0,0,0,0,0,0,0,0};
static	short	OldLevel8 = -300;			// use this to detect a change in zoom level
static	double	PreviewRatio;
static  Complex	centre;


// Big num declarations **********************************************************
extern	int	decimals, precision;
extern	BYTE	BigNumFlag;		// True if bignum used
extern	BigDouble   BigHor, BigVert, BigWidth;
extern	int getprecbf_mag(void);
// Big num declarations **********************************************************

/**************************************************************************
	ManZPower fractal script generator
**************************************************************************/

int	GenManZPowerScript(HWND hwnd, char *filename) 

    {
    int		i, k;
    char	s[120];
    FILE	*out;
    char	ascii[6];

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    if (BigNumFlag)
	{
	char	*s1, *s2, *s3;
	s1 = new char[SIZEOF_BF_VARS];
	s2 = new char[SIZEOF_BF_VARS];
	s3 = new char[SIZEOF_BF_VARS];
	ConvertBignum2String(s1, BigHor.x);
	ConvertBignum2String(s2, BigVert.x);
	ConvertBignum2String(s3, BigWidth.x);
	*(s1 + decimals + 5) = '\0';					// no need to print too many decimals
	*(s2 + decimals + 5) = '\0';
	*(s3 + decimals + 5) = '\0';
	fprintf(out, " -c%s,%s,%s\n", s1, s2, s3);
	if (s1) { delete[] s1; s1 = NULL; }
	if (s2) { delete[] s2; s2 = NULL; }
	if (s3) { delete[] s3; s3 = NULL; }
	}
    else
        fprintf(out, " -c%24.24f,%24.24f,%24.24g\n", hor, vert, mandel_width);
    fprintf(out, "Parameter Animation: %10.10f %10.10f %d %d %d\n", StartRate, EndRate, frames, ParamNumber, Return2Start);
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
    fclose(out);
    return 0;
    }

/**************************************************************************
	Draw preview image
***************************************************************************/

int	DrawPreview(HWND hwnd)

    {
    char	s[120];
    WORD	i, j, offset = 0;
    int		result;
    DWORD	colour;
    short	percent, display_count;
    double	PreviewGap;
    Complex	t, temp, temp1, temp2;

    if (OldLevel8 != level[8])
	{
	centre.x = hor + mandel_width * PreviewRatio / 2.0;
	centre.y = mandel_width / 2.0 + vert;
	mandel_width = log((double)level[8] + 257.0) / 2.0;
	hor = centre.x - mandel_width * PreviewRatio / 2.0;
	vert = centre.y - mandel_width / 2.0;
	}
    PreviewGap = mandel_width / (double)MANDEL_PREVIEW_HEIGHT;
    Pix.mandel_width = mandel_width;
    Pix.hor = hor;
    Pix.vert = vert;

    Pix.q->x = Pix.mandel_width * (double)(-level[4]) / (double)MANDEL_PREVIEW_HEIGHT + Pix.hor;
    Pix.q->y = Pix.mandel_width * (double)(level[5]) / ((double)MANDEL_PREVIEW_HEIGHT) + Pix.vert;
    Pix.c->x = Pix.mandel_width * (double)level[4] / (double)MANDEL_PREVIEW_HEIGHT + Pix.hor;
    Pix.c->y = Pix.mandel_width * (double)(-level[5]) / ((double)MANDEL_PREVIEW_HEIGHT) + Pix.vert;
    Pix.j.x = (double)level[6] / 51.0;
    Pix.j.y = (double)(-level[7]) / 51.0;

//    Pix.juliaflag = true;
    Plot.InitPlot(threshold, &TrueCol, wpixels, xdots, ydots, MandelScreen.PreviewWidth, MandelScreen.PreviewHeight, MandelScreen.PreviewDib.BitsPerPixel, &MandelScreen.PreviewDib, USEPALETTE);
    percent = 0;
    for (i = 0; i < MANDEL_PREVIEW_WIDTH; i++)
	{
	Pix.c->x += PreviewGap;
	Pix.c->y = -2.0;
	Pix.c->y = Pix.mandel_width * (double)(-level[5]) / ((double)MANDEL_PREVIEW_HEIGHT) + Pix.vert;
//	Pix.q->y = -2.0;
	display_count = (10 * i) / MANDEL_PREVIEW_WIDTH;
	if (display_count > percent)
	    {
	    percent = display_count;
	    sprintf(s, "%d%%", percent * 10);
	    SetWindowText(hwnd, s);			// Show formatted text in the caption bar
	    }
	for (j = 0; j < MANDEL_PREVIEW_HEIGHT; j++)
	    {
	    if (Pix.juliaflag)
		{
		*Pix.q = Pix.j;
		*Pix.z = *Pix.c;
		}
	    else
		{
		*Pix.q = *Pix.c;
		*Pix.z = 0.0;
		}

	    Pix.c->y += PreviewGap;
	    iteration = 0;
	    t = /*(invert) ? invertz2(*c) : */*Pix.c;
//	    t = 0.0;
	    temp2.x = param[2];
	    temp2.y = param[3];
	    Pix.z->x = t.x + param[0];
	    Pix.z->y = t.y + param[1];
	    for EVER
		{
		if (iteration++ >= threshold)
		    {
		    iteration = threshold;
		    break;
		    }
		iteration += 6;
		*Pix.z = *Pix.z ^ temp2;
		*Pix.z += *Pix.q;
		result = Pix.FractintBailoutTest(Pix.z);
		if (result)				// escape time
		    break;
		}
	    colour = (iteration == threshold) ? threshold : iteration / 6;
//	    colour = (iteration/* << SpeedFactor*/);
	    Plot.PlotPoint(i, (WORD)(MANDEL_PREVIEW_HEIGHT - j - 2), colour);
	    }
	}

    OldLevel8 = level[8];
    return 0;
    }

/**************************************************************************
    Draw Preview Mandelbrot Images
***************************************************************************/

int	MandelPreview(HWND hwnd)

    {
    memset(MandelScreen.PreviewDib.DibPixels, 0, (WIDTHBYTES((DWORD)MandelScreen.PreviewWidth * (DWORD)MandelScreen.PreviewDib.BitsPerPixel)) * MandelScreen.PreviewHeight);	// set background to black
    IsPreview = TRUE;
    DrawPreview(hwnd);
    ShowWindow(hwnd, SW_SHOWNORMAL);
    return 0;
    }

/**************************************************************************
	Dialog For Setting Up ManZPower Animation
**************************************************************************/

DLGPROC FAR PASCAL MandelbrotAnimDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     char		*fileptr;
     BOOL		TempCheck;
     BOOL		bTrans ;
     static	char	TempFile[MAX_PATH];
     static	char	TempStr[MAX_PATH];
     char		Bailout[120];
     HWND		hwndParent, hCtrl;
//     int		i;
     short		nCtrlID, nIndex;
     Complex	d;

     // set the dimensions and location of the windows
     MandelScreen.HorOffset = MANDEL_HOR_OFFSET;				// dimensions of preview window
     MandelScreen.VertOffset = MANDEL_VERT_OFFSET;
     MandelScreen.PreviewHeight = MANDEL_PREVIEW_HEIGHT;
     MandelScreen.PreviewWidth = MANDEL_PREVIEW_WIDTH;

     ParamValues[0] = Pix.param[0] = param[0];
     ParamValues[1] = Pix.param[1] = param[1];
     ParamValues[2] = Pix.param[2] = param[2];
     ParamValues[3] = Pix.param[3] = param[3];
     ParamValues[4] = 0.0;
     ParamValues[5] = 0.0;
     ParamValues[6] = 0.0;
     ParamValues[7] = 0.0;
     ParamValues[8] = mandel_width;
     rect.left = 0;
     rect.top = 0;
     rect.right = MANDEL_PREVIEW_WIDTH;
     rect.bottom = MANDEL_PREVIEW_HEIGHT;
     PreviewRatio = (double)MANDEL_PREVIEW_WIDTH / (double)MANDEL_PREVIEW_HEIGHT;
     Pix.type = FPMANDELZPOWER;
//     Pix.type = MARKSMANDELFP;
//     Pix.type = FPMANZTOZPLUSZPWR;
//     Pix.type = MANDELFP;


/*
     Pix.q->x = Pix.mandel_width * (double)level[4] / (double)MANDEL_PREVIEW_HEIGHT + Pix.hor;
     Pix.q->y = Pix.mandel_width * (double)(-level[5]) / ((double)MANDEL_PREVIEW_HEIGHT) + Pix.vert;
     ParamValues[4] = Pix.q->x;
     ParamValues[5] = Pix.q->y;
*/
     switch (message)
	  {
	  case WM_INITDIALOG:
		hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, StartImmediately, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_RETURN);
		SendMessage(hCtrl, BM_SETCHECK, Return2Start, 0L);
		sprintf(TempStr, "%f", StartRate);
		SetDlgItemText(hDlg, IDC_RATE_START, TempStr);
		sprintf(TempStr, "%f", EndRate);
		SetDlgItemText(hDlg, IDC_RATE_END, TempStr);
		SetDlgItemInt(hDlg, IDC_PARAM_NUM, ParamNumber, TRUE);
		for (nCtrlID = IDC_SCROLLBAR1; nCtrlID <= IDC_SCROLLBAR9; nCtrlID++)
		    {
		    nIndex = nCtrlID - IDC_SCROLLBAR1;
		    hCtrl = GetDlgItem(hDlg, nCtrlID);
		    SetScrollRange(hCtrl, SB_CTL, -255, 255, FALSE);
		    if (nIndex == 8)
			level[nIndex] = (int)(exp(ParamValues[nIndex] * 2.0) - 257.0);
		    else
			level[nIndex] = (int)(ParamValues[nIndex] * 25.5);
		    SetScrollPos(hCtrl, SB_CTL, -level[nIndex], FALSE);
		    sprintf(TempStr, "%2.2lf", ParamValues[nIndex]);
		    SetDlgItemText(hDlg, nIndex + IDC_LABEL1, TempStr);
		    }
		if (MandelScreen.InitPreview(hDlg) < 0)
		    {
		    wsprintf(TempStr, "Error: No memory for preview");
		    MessageBox(hDlg, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		    EndDialog(hDlg, FALSE);
		    }
		sprintf(Bailout, "%14.14lf", rqlim);
		SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
		hCtrl = GetDlgItem(hDlg, IDC_JULIA);
		SendMessage(hCtrl, BM_SETCHECK, Pix.juliaflag, 0L);
		InvalidateRect(hDlg, NULL, FALSE);
		return (DLGPROC)FALSE;

		SetUpFilename(ScriptFileName, "sci", "ManZ");
		SetUpFilename(PNGName, "animpng", "ManZ");
		SetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
		SetDlgItemInt(hDlg, IDC_FRAMES, frames, TRUE);
	        return (DLGPROC)TRUE ;

	  case WM_VSCROLL:

	      hCtrl = GET_WM_COMMAND_HWND(wParam, (_int64)lParam);
	      if ((nCtrlID = ((WORD)(GetWindowLong(hCtrl, GWL_ID)))) == 0)
		  {
		  wsprintf(TempStr, "Error: Scrollbar fail: <%ld>", GetLastError());
		  MessageBox(hDlg, TempStr, "Paul's Fractals", MB_ICONEXCLAMATION | MB_OK);
		  }

	      nIndex = nCtrlID - IDC_SCROLLBAR1;
	      hwndParent = GetParent(hDlg);

	      switch ((int)LOWORD(wParam))
		  {
		  case SB_PAGEDOWN:
		      level[nIndex] -= 15;        // fall through
		  case SB_LINEDOWN:
		      level[nIndex] = max(-255, level[nIndex] - 1);
		      break;
		  case SB_PAGEUP:
		      level[nIndex] += 15;        // fall through
		  case SB_LINEUP:
		      level[nIndex] = min(255, level[nIndex] + 1);
		      break;
		  case SB_TOP:
		      level[nIndex] = 255;
		      break;
		  case SB_BOTTOM:
		      level[nIndex] = -255;
		      break;
		  case SB_THUMBPOSITION:
		  case SB_THUMBTRACK:
		      level[nIndex] = -HIWORD(wParam);
		      break;
		  default:
		      return FALSE;
		  }
	      SetScrollPos(hCtrl, SB_CTL, -level[nIndex], TRUE);
/*
	      if (nIndex == 4)
		  {
		  ParamValues[nIndex] = Pix.mandel_width * (double)level[4] / (double)MANDEL_PREVIEW_HEIGHT + Pix.hor;
		  Pix.q->x = ParamValues[nIndex];
		  }
	      else if (nIndex == 5)
		  {
		  ParamValues[nIndex] = Pix.mandel_width * (double)(-level[5]) / ((double)MANDEL_PREVIEW_HEIGHT) + Pix.vert;
		  Pix.q->y = ParamValues[nIndex];
		  }
*/
	      if (nIndex == 8)
		  {
		  ParamValues[nIndex] = log((double)level[8] + 257.0) / 2.0;
//		  mandel_width = ParamValues[nIndex];
		  sprintf(TempStr, "%2.2le", ParamValues[nIndex]);
		  }
	      else
		  {
		  ParamValues[nIndex] = (double)(level[nIndex]) / 25.5;
		  param[nIndex] = Pix.param[nIndex] = ParamValues[nIndex];
		  sprintf(TempStr, "%2.2lf", ParamValues[nIndex]);
		  }
	      SetDlgItemText(hDlg, nIndex + IDC_LABEL1, TempStr);
	      InvalidateRect(hDlg, &rect, FALSE);
	      return (DLGPROC)TRUE;

	  case WM_PAINT:
	      BeginPaint(hDlg, &ps);
	      MandelPreview(hDlg);
	      MandelScreen.Preview(hDlg);
	      EndPaint(hDlg, &ps);
	      return (DLGPROC)TRUE;

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
		    case IDC_JULIA:
			hCtrl = GetDlgItem(hDlg, IDC_JULIA);
			Pix.juliaflag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			return (DLGPROC)TRUE;

		    case IDOK:
			GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName, MAX_PATH);
			fileptr = PNGName + strlen(PNGName);
			while (fileptr > PNGName && *fileptr != '.')
			    fileptr--;	
			if (*fileptr == '.')						// remove any extension
			    *fileptr = '\0';
			
			if (WriteMPEGFrames)						// generate MPEG filename
			    {
			    GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, TempFile, MAX_PATH);
			    sprintf(MPGFile, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }

			GetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName, 400);
			fileptr = ScriptFileName + strlen(ScriptFileName);
			while (fileptr > ScriptFileName && *fileptr != '.')
			    fileptr--;							// remove extension
			if (*fileptr == '.')
			    *fileptr = '\0';
			strcat(ScriptFileName, ".sci");
			hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
			StartImmediately = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_RETURN);
			Return2Start = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			GetDlgItemText(hDlg, IDC_RATE_START, TempStr, 100);
			sscanf(TempStr, "%lf", &StartRate);
			GetDlgItemText(hDlg, IDC_RATE_END, TempStr, 100);
			sscanf(TempStr, "%lf", &EndRate);

			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			WritePNGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			WriteMemFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			WritePNGList = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			frames = GetDlgItemInt(hDlg, IDC_FRAMES, &bTrans, TRUE);
			if (frames <= 0)
			    frames = 20;
			if (frames > MAXANIM)
			    frames = MAXANIM;
			GenManZPowerScript(hDlg, ScriptFileName);
			MandelScreen.PreviewDib.CloseDibPtrs();
			EndDialog (hDlg, TRUE);
			return(DLGPROC)TRUE;

		    case IDCANCEL:
			MandelScreen.PreviewDib.CloseDibPtrs();
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }

