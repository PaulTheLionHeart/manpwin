/*
   CURLICUES.CPP a module for Fractals by Dr. M. Ecker
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"fractalp.h"
//#include	"menu.h"
#include	"anim.h"
#include	"polygon.h"
#include	"colour.h"
#include	"colour.h"
#include	"OscProcess.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 16

extern	HWND	GlobalHwnd;				// This is the main windows handle

extern	int	user_data(HWND);
extern	int	power(int base, int n);
extern	double	FloatPower(double base, int n);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);
extern	char	*AnimData(void);
extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);

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
	BOOL	Return2Start = FALSE;		// do we want to go backwards to the start?

extern	CTrueCol    TrueCol;			// palette info
extern	CFract	Fractal;			// current fractal stuff
extern	COscProcess	OscProcess;

/**************************************************************************
	Parameter script generator
**************************************************************************/

int	GenParameterScript(HWND hwnd, char *filename) 

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
	Dialog For Setting Up Parameter Animation
**************************************************************************/

DLGPROC FAR PASCAL ParamAnimDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     char		*fileptr;
     HWND		hCtrl;
     BOOL		TempCheck;
     BOOL		bTrans ;
     char		s[10][100];
     static	char	TempFile[MAX_PATH];
     static	char	TempStr[MAX_PATH];
     static	struct	OscillatorSpecificStuff	*DatabasePtr; 
     int		i;
     static	int	NumParams;
     static	int	NumVariables;

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
		switch (type)
		    {
		    case OSCILLATORS:
		    case FRACTALMAPS:
		    case SPROTTMAPS:
		    case SURFACES:
		    case KNOTS:
		    case CURVES:
		        DatabasePtr = OscProcess.LoadDatabasePointer(type, subtype);
			if (type == OSCILLATORS)
			    SetWindowText (hDlg, "Select Chaotic Oscillator Parameters");
			else if (type == FRACTALMAPS)
			    SetWindowText(hDlg, "Select Fractal Map Parameters");
			else if (type == SPROTTMAPS)
			    SetWindowText(hDlg, "Select Sprott Fractal Map Parameters");
			else  if (type == SURFACES)
			    SetWindowText (hDlg, "Select Surface Parameters");
			else  if (type == KNOTS)
			    SetWindowText (hDlg, "Select Knot Parameters");
			else  if (type == CURVES)
			    SetWindowText (hDlg, "Select Knot Parameters");

			NumParams = DatabasePtr->numparams;
			for (i = 0; i < NumParams && i < NUMPARAM; i++)
			    {
			    sprintf(s[i], "%g", DatabasePtr->paramvalue[i]);
			    SetDlgItemText(hDlg, ID_FRACPARTX1 + i, DatabasePtr->paramname[i]);
			    SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i]);
			    }
			for (i = NumParams; i < NUMPARAM; i++)
			    SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
			NumVariables = DatabasePtr->numvariables;
			for (i = 0; i < NumVariables && i < NUMPARAM; i++)
			    {
			    sprintf(s[i], "%f", DatabasePtr->variablevalue[i]);
			    SetDlgItemText(hDlg, ID_FRACVARTX01 + i, DatabasePtr->variablename[i]);
			    SetDlgItemText(hDlg, ID_FRACVAR01 + i, s[i]);
			    }
			for (i = NumVariables; i < NUMPARAM; i++)
			    SetDlgItemText(hDlg, ID_FRACVARTX01 + i, "     N/A");
			break;
		    default:				// all other fractals
			if (type == PERTURBATION)
			    {
			    NumParams = PerturbationSpecific[subtype].numparams;
			    SetDlgItemInt(hDlg, ID_FRACVAR01, SlopeType, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR02, (UINT)lightDirectionDegrees, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR03, (UINT)bumpMappingStrength, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR04, (UINT)bumpMappingDepth, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR05, PaletteStart, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR06, (int)rqlim, TRUE);
			    }
			else if (type == SLOPEFORWARDDIFF)
			    NumParams = SlopeFwdDiffSpecific[subtype].numparams;
			else if (type == SLOPEDERIVATIVE)
			    NumParams = SlopeDerivSpecific[subtype].numparams;
			else 
			    NumParams = fractalspecific[type].numparams;
			for (i = 0; i < NumParams && i < NUMPARAM; i++)
			    {
			    sprintf(s[i], "%g", *Fractal.ParamValue[i]);
			    SetDlgItemText(hDlg, ID_FRACPARTX1 + i, Fractal.ParamName[i]);
			    SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i]);
			    }
			for (i = NumParams; i < NUMPARAM; i++)
			    SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
		    }

		SetUpFilename(ScriptFileName, "sci", "Param");
		SetUpFilename(PNGName, "animpng", "Param");
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
			hCtrl = GetDlgItem (hDlg, IDC_RETURN);
			Return2Start = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			GetDlgItemText(hDlg, IDC_RATE_START, TempStr, 100);
			sscanf(TempStr, "%lf", &StartRate);
			GetDlgItemText(hDlg, IDC_RATE_END, TempStr, 100);
			sscanf(TempStr, "%lf", &EndRate);
			for (i = 0; i < NumParams && i < 10; i++)	// pick up any changes to the initial parameter values
			    {
			    GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i], 100);
			    param[i] = atof(s[i]);
			    }
			if (type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
			    {
			    for (i = 0; i < NumVariables && i < 10; i++)	// pick up any changes to the initial variable values
				{
				GetDlgItemText(hDlg, ID_FRACVAR01 + i, s[i], 100);
				param[i + 10] = atof(s[i]);
				}
			    }

			ParamNumber = GetDlgItemInt(hDlg, IDC_PARAM_NUM, &bTrans, TRUE);
			if (type == PERTURBATION)
			    {
			    SlopeType = GetDlgItemInt(hDlg, ID_FRACVAR01, &bTrans, TRUE);
			    lightDirectionDegrees = (double)GetDlgItemInt(hDlg, ID_FRACVAR02, &bTrans, TRUE);
			    bumpMappingStrength = (double)GetDlgItemInt(hDlg, ID_FRACVAR03, &bTrans, TRUE);
			    bumpMappingDepth = (double)GetDlgItemInt(hDlg, ID_FRACVAR04, &bTrans, TRUE);
			    PaletteStart = GetDlgItemInt(hDlg, ID_FRACVAR05, &bTrans, TRUE);
			    rqlim = GetDlgItemInt(hDlg, ID_FRACVAR06, &bTrans, TRUE);
			    }

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
			GenParameterScript(hDlg, ScriptFileName);
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

