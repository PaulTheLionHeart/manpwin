/*
   IKEDA.CPP a module for Ikeda Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"menu.h"
#include	"anim.h"
#include	"colour.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 48

extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);
extern	char	*AnimData(void);

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
//extern	BYTE	*PalettePtr;			// points to true colour palette
extern	CTrueCol    TrueCol;			// palette info
extern	BOOL	StartImmediately;		// immediate start of animation generation
//extern	BYTE	ScriptPaletteFlag;		// Display palette 

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	WORD	type;				// fractal type
extern	int	subtype;			// A - E
extern	int	curpass, totpasses;
//extern	BYTE	*PalettePtr;			// points to true colour palette
extern	double	param[];

static	double	xscale, yscale;
static	int	sides = 7;			// number of sides of polygon or star
static	BOOL	IsStar = TRUE;			// spread colours across palette

static	int	frames = 100;
static	double	StartRate = 0.65, EndRate = 1.0;
static	double  a = 1.0, b = 0.9, c = 0.4, d = 6.0;

/**************************************************************************
	Ikeda Fractal Type Images
***************************************************************************/
/*
int	DoIkeda2D(void)

    {
    int	    j, k;
    double  i, x, y, xt, temp, sintemp, costemp;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    x = 0.1;
    y = 0.1;

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    for(i = 0; i < 80000; i++)
	{
	temp = c - d / (1.0 + x * x + y * y);
	sintemp = sin(temp);
	costemp = cos(temp);

	xt = a + b * (x * costemp - y * sintemp);
	y = b * (x * sintemp + y * costemp);
	x = xt;

	j = (int)((x - hor) * xscale);
	k = (int)((vert + mandel_width - y) * yscale);
	plot (j, k, (long)fabs(atan2(y, x)*180.0/PI));
	}
    return 0;
    }
*/

/**************************************************************************
	Ikeda script generator
**************************************************************************/

int	GenIkedaScript(HWND hwnd, char *filename) 

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
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Ikeda Animation: %7.7f %7.7f %d\n", StartRate, EndRate, frames);
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
	Dialog For Setting Up Ikeda Animation
**************************************************************************/

DLGPROC FAR PASCAL IkedaAnimDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     char		*fileptr;
     HWND		hCtrl;
     BOOL		TempCheck;
     BOOL		bTrans ;
     static	char	TempFile[MAX_PATH];
     static	char	TempStr[MAX_PATH];

     switch (message)
	  {
	  case WM_INITDIALOG:
		hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, StartImmediately, 0L);
		sprintf(TempStr, "%f", StartRate);
		SetDlgItemText(hDlg, IDC_RATE_START, TempStr);
		sprintf(TempStr, "%f", EndRate);
		SetDlgItemText(hDlg, IDC_RATE_END, TempStr);

		sprintf(TempStr, "%f", a);
		SetDlgItemText(hDlg, IDC_PARAM1, TempStr);
		sprintf(TempStr, "%f", c);
		SetDlgItemText(hDlg, IDC_PARAM3, TempStr);
		sprintf(TempStr, "%f", d);
		SetDlgItemText(hDlg, IDC_PARAM4, TempStr);

		SetUpFilename(ScriptFileName, "sci", "Ikeda");
		SetUpFilename(PNGName, "animpng", "Ikeda");
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
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			GetDlgItemText(hDlg, IDC_RATE_START, TempStr, 100);
			sscanf(TempStr, "%lf", &StartRate);
			GetDlgItemText(hDlg, IDC_RATE_END, TempStr, 100);
			sscanf(TempStr, "%lf", &EndRate);

			GetDlgItemText(hDlg, IDC_PARAM1, TempStr, 100);
			sscanf(TempStr, "%lf", &param[0]);
			GetDlgItemText(hDlg, IDC_PARAM3, TempStr, 100);
			sscanf(TempStr, "%lf", &param[2]);
			GetDlgItemText(hDlg, IDC_PARAM4, TempStr, 100);
			sscanf(TempStr, "%lf", &param[3]);

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
			type = FRACTALMAPS;
			GenIkedaScript(hDlg, ScriptFileName);
			EndDialog (hDlg, TRUE);
			return(DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }

