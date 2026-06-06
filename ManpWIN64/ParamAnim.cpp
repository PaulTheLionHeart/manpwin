/*
   CURLICUES.CPP - a module for Fractals by Dr. M. Ecker
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"BigDouble.h"
#include	"polygon.h"
#include	"colour.h"
#include	"colour.h"
#include	"OscProcess.h"
#include	"SafeStrings.h"

#define MAXPOINTS 16

extern	int	user_data(HWND);
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

static	double	xscale, yscale;
static	double	StartRate = -1.0, EndRate = 1.0;
static	char	StartRateStr[MAX_PATH];		// these are used because there may be very small differences if using higher precision
static	char	EndRateStr[MAX_PATH];

static	int	ParamNumber = 0;
static	int	frames = 100;

/**************************************************************************
	Parameter script generator
**************************************************************************/

int	CManp::GenParameterScript(HWND hwnd, char *filename, int NumVariables) 
    {
    int		i, k;
    char	s[120];
    FILE	*out;
    char	ascii[6];

    if ((out = fopen(filename, "w")) == NULL)
	{
	SAFE_SPRINTF(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    if (BigNumFlag)
	{
	char s1[SIZEOF_BF_VARS]{};
	char s2[SIZEOF_BF_VARS]{};
	char s3[SIZEOF_BF_VARS]{};
	BigHor.ToString(s1, SIZEOF_BF_VARS, false);
	BigVert.ToString(s2, SIZEOF_BF_VARS, false);
	BigWidth.SafeSprintf(s3, SIZEOF_BF_VARS, "%.20Re");
	*(s1 + decimals + 5) = '\0';					// no need to print too many decimals
	*(s2 + decimals + 5) = '\0';
	*(s3 + decimals + 5) = '\0';
	fprintf(out, " -c%s,%s,%s\n", s1, s2, s3);
	}
    else
        fprintf(out, " -c%24.24f,%24.24f,%24.24g\n", gManp->hor, gManp->vert, gManp->mandel_width);
    fprintf(out, "Parameter Animation: %10.10f %10.10f %d %d %d\n", StartRate, EndRate, frames, ParamNumber, Return2Start);
    fprintf(out, "Palette=\n");
    for (i = 0, k = 0; i < threshold; i++, k++)
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
    fclose(out);
    return 0;
    }

/**************************************************************************
	Dialog For Setting Up Parameter Animation
**************************************************************************/

INT_PTR CALLBACK ParamAnimDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     char		*fileptr;
     HWND		hCtrl;
     BOOL		TempCheck;
     BOOL		bTrans ;
     char		s[11][100];
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
		SendMessage(hCtrl, BM_SETCHECK, gManp->TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, gManp->StartImmediately, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_RETURN);
		SendMessage(hCtrl, BM_SETCHECK, gManp->Return2Start, 0L);
		SAFE_SPRINTF(TempStr, "%.12f", StartRate);
		SetDlgItemText(hDlg, IDC_RATE_START, TempStr);
		SAFE_SPRINTF(TempStr, "%.12f", EndRate);
		SetDlgItemText(hDlg, IDC_RATE_END, TempStr);
		SetDlgItemInt(hDlg, IDC_PARAM_NUM, ParamNumber, TRUE);
		switch (gManp->type)
		    {
		    case OSCILLATORS:
		    case FRACTALMAPS:
		    case SPROTTMAPS:
		    case SURFACES:
		    case KNOTS:
		    case CURVES:
		        DatabasePtr = gManp->OscProcess.LoadDatabasePointer(gManp->type, gManp->subtype);
			if (gManp->type == OSCILLATORS)
			    SetWindowText (hDlg, "Select Chaotic Oscillator Parameters");
			else if (gManp->type == FRACTALMAPS)
			    SetWindowText(hDlg, "Select Fractal Map Parameters");
			else if (gManp->type == SPROTTMAPS)
			    SetWindowText(hDlg, "Select Sprott Fractal Map Parameters");
			else  if (gManp->type == SURFACES)
			    SetWindowText (hDlg, "Select Surface Parameters");
			else  if (gManp->type == KNOTS)
			    SetWindowText (hDlg, "Select Knot Parameters");
			else  if (gManp->type == CURVES)
			    SetWindowText (hDlg, "Select Knot Parameters");

			NumParams = DatabasePtr->numparams;
			for (i = 0; i < NumParams && i < NUMPARAM; i++)
			    {
			    SAFE_SPRINTF(s[i], "%g", DatabasePtr->paramvalue[i]);
			    SetDlgItemText(hDlg, ID_FRACPARTX1 + i, DatabasePtr->paramname[i]);
			    SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i]);
			    }
			for (i = NumParams; i < NUMPARAM; i++)
			    SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
			NumVariables = DatabasePtr->numvariables;
			for (i = 0; i < NumVariables && i < NUMPARAM; i++)
			    {
			    SAFE_SPRINTF(s[i], "%f", DatabasePtr->variablevalue[i]);
			    SetDlgItemText(hDlg, ID_FRACVARTX01 + i, DatabasePtr->variablename[i]);
			    SetDlgItemText(hDlg, ID_FRACVAR01 + i, s[i]);
			    }
			for (i = NumVariables; i < NUMPARAM; i++)
			    SetDlgItemText(hDlg, ID_FRACVARTX01 + i, "     N/A");
			break;
		    default:				// all other fractals
			if (gManp->type == PERTURBATION)
			    {
			    NumParams = PerturbationSpecific[gManp->subtype].numparams;
			    SetDlgItemInt(hDlg, ID_FRACVAR01, gManp->SlopeType, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR02, (UINT)gManp->lightDirectionDegrees, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR03, (UINT)gManp->bumpMappingStrength, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR04, (UINT)gManp->bumpMappingDepth, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR05, gManp->PaletteStart, TRUE);
			    SetDlgItemInt(hDlg, ID_FRACVAR06, (int)gManp->rqlim, TRUE);
			    }
			else if (gManp->type == SLOPEFORWARDDIFF)
			    NumParams = SlopeFwdDiffSpecific[gManp->subtype].numparams;
			else if (gManp->type == SLOPEDERIVATIVE)
			    NumParams = SlopeDerivSpecific[gManp->subtype].numparams;
			else 
			    NumParams = fractalspecific[gManp->type].numparams;
			for (i = 0; i < NumParams && i < NUMPARAM; i++)
			    {
			    SAFE_SPRINTF(s[i], "%g", *gManp->Fractal.ParamValue[i]);
			    SetDlgItemText(hDlg, ID_FRACPARTX1 + i, gManp->Fractal.ParamName[i]);
			    SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i]);
			    }
			for (i = NumParams; i < NUMPARAM; i++)
			    SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
			SAFE_SPRINTF(s[10], "%f", gManp->rqlim);
			SetDlgItemText(hDlg, ID_FRACPARAM11, s[10]);
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
//			_snprintf_s(PNGFile, _MAX_PATH, _TRUNCATE, "%s", GenerateAnimFileName (ANIMPNGPath, PNGName));
//			SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGFile);
			return TRUE;

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
			    _snprintf_s(MPGFile, _MAX_PATH, _TRUNCATE, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }

			GetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName, 400);
			fileptr = ScriptFileName + strlen(ScriptFileName);
			while (fileptr > ScriptFileName && *fileptr != '.')
			    fileptr--;							// remove extension
			if (*fileptr == '.')
			    *fileptr = '\0';
			strcat_s(ScriptFileName, MAX_PATH, ".sci");
			hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
			gManp->StartImmediately = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_RETURN);
			gManp->Return2Start = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			gManp->TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			GetDlgItemText(hDlg, IDC_RATE_START, StartRateStr, MAX_PATH);
			sscanf(StartRateStr, "%lf", &StartRate);
			GetDlgItemText(hDlg, IDC_RATE_END, EndRateStr, MAX_PATH);
			sscanf(EndRateStr, "%lf", &EndRate);
			for (i = 0; i < NumParams && i < 10; i++)	// pick up any changes to the initial parameter values
			    {
			    GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[i], 100);
			    gManp->param[i] = atof(s[i]);
			    }
			GetDlgItemText(hDlg, ID_FRACPARAM11, s[10], 100);
			gManp->rqlim = atof(s[10]);
			if (gManp->type == OSCILLATORS || gManp->type == FRACTALMAPS || gManp->type == SPROTTMAPS || gManp->type == SURFACES || gManp->type == KNOTS || gManp->type == CURVES)
			    {
			    for (i = 0; i < NumVariables && i < 10; i++)	// pick up any changes to the initial variable values
				{
				GetDlgItemText(hDlg, ID_FRACVAR01 + i, s[i], 100);
				gManp->param[i + 10] = atof(s[i]);
				}
			    }

			ParamNumber = GetDlgItemInt(hDlg, IDC_PARAM_NUM, &bTrans, TRUE);
			if ((ParamNumber < 0 || ParamNumber > 10) && gManp->type != PERTURBATION)
			    ParamNumber = 0;
			if (gManp->type == PERTURBATION)
			    {
			    gManp->SlopeType = GetDlgItemInt(hDlg, ID_FRACVAR01, &bTrans, TRUE);
			    gManp->lightDirectionDegrees = (double)GetDlgItemInt(hDlg, ID_FRACVAR02, &bTrans, TRUE);
			    gManp->bumpMappingStrength = (double)GetDlgItemInt(hDlg, ID_FRACVAR03, &bTrans, TRUE);
			    gManp->bumpMappingDepth = (double)GetDlgItemInt(hDlg, ID_FRACVAR04, &bTrans, TRUE);
			    gManp->PaletteStart = GetDlgItemInt(hDlg, ID_FRACVAR05, &bTrans, TRUE);
			    gManp->rqlim = GetDlgItemInt(hDlg, ID_FRACVAR06, &bTrans, TRUE);
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
			gManp->GenParameterScript(hDlg, ScriptFileName, NumVariables);
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

