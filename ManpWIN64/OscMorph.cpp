/*
   OSCMORPH.CPP a module to explore Morphing Chaotic Oscillators

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/


#include	<stdio.h>
#include	"manp.h"
#include	"colour.h"
#include	"resource.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"anim.h"
#include	"OscProcess.h"
#include	"SafeStrings.h"

extern	long	threshold;
extern	double	mandel_width;			// width of display
extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address 
extern	WORD	type;				// fractal type
extern	int	subtype;

extern	BOOL	DisplayLines;
extern	BOOL	StartImmediately;

extern	BOOL	WritePNGFrames;			// write frames to PNG files
extern	BOOL	WriteMemFrames;			// write frames to memory
extern	BOOL	WritePNGList;			// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;		// write frames directly to an MPEG file

extern	char	PNGName[];			// base name for PNG file sequence
extern	char	MPGFile[];			// MPEG file
extern	char	MPGPath[];			// path for MPEG files
extern	char	ANIMPNGPath[];			// path for animated PNG files and LST files
extern	char	PNGFile[];			// PNG file
extern	char	ScriptFileName[];		// base name for script file 

extern	char	*AnimData(void);
extern	void	ConvertRGB2ASCII(RGBTRIPLE, char *);
extern	char	*GenerateMPEGFileName(char *, char *);
extern	char	*GenerateAnimFileName(char *, char *);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);

static	int	PlotWidth = 5;			// width of plot for curve or knot
static	int	CurrentFrame, frames = 100;
static	struct	OscillatorSpecificStuff	*DatabasePtr;

//BOOL		DisplayAxisImages;			// show all possible images for axis pairs
BOOL		DisplayAxisLabels;			// show labels for axis pairs

extern	CTrueCol    TrueCol;			// palette info
extern	COscProcess	OscProcess;

/**************************************************************************
	Oscillator and Fractal Map script generator
**************************************************************************/

int	GenOscMorphScript(HWND hwnd, char *filename) 
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
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
    fprintf(out, "Oscillator Morphing: %d %d %d\n", threshold, threshold, frames);
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
    for (i = 0; i < frames; i++) 
	fprintf(out, "-2%f,%f,%f,%f,%f,%f\n", 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    fclose(out);
    return 0;
    }

/**************************************************************************
	Dialog For Setting Up Oscillator Morphing Animation
**************************************************************************/

INT_PTR CALLBACK OscMorphAnimDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     char		*fileptr;
     HWND		hCtrl;
     BOOL		TempCheck;
     BOOL		bTrans;
//     int		RotationAxes;
//     char		zScaling[24];
     static	char	TempFile[MAX_PATH];
     static	char	TempStr[MAX_PATH];
     char		AnimType[24];

     switch (message)
	  {
	  case WM_INITDIALOG:
	        DatabasePtr = OscProcess.LoadDatabasePointer(type, subtype);
	        PlotWidth = (type == KNOTS) ? 10 : 3;
		hCtrl = GetDlgItem (hDlg, IDC_PLOTWIDTH);
		SetDlgItemInt(hDlg, IDC_PLOTWIDTH, PlotWidth, TRUE);
		EnableWindow (hCtrl, (type == KNOTS || type == CURVES));

		hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, StartImmediately, 0L);
		if (type == OSCILLATORS)
		    SAFE_SPRINTF(AnimType, "Osc");
		else if (type == FRACTALMAPS || type == SPROTTMAPS)
		    SAFE_SPRINTF(AnimType, "Map");
		else if (type == SURFACES)
		    SAFE_SPRINTF(AnimType, "Surface");
		else if (type == KNOTS)
		    SAFE_SPRINTF(AnimType, "Knot");
		else if (type == CURVES)
		    SAFE_SPRINTF(AnimType, "Curve");
		SetUpFilename(ScriptFileName, "sci", AnimType);
		SetUpFilename(PNGName, "animpng", AnimType);
		SetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
		SetDlgItemText(hDlg, IDC_MORPHDELAY, "10.0");
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
		SetDlgItemInt(hDlg, IDC_FRAMES, frames, TRUE);
		hCtrl = GetDlgItem(hDlg, IDC_LINES);
		SendMessage(hCtrl, BM_SETCHECK, (DatabasePtr->flags) & 1, 0L);
		hCtrl = GetDlgItem(hDlg, IDC_DISPLAYALLAXES);
		SendMessage(hCtrl, BM_SETCHECK, OscProcess.DisplayAxisImages, 0L);
//		hCtrl = GetDlgItem(hDlg, IDC_DISPLAYAXISLABELS);
//		SendMessage(hCtrl, BM_SETCHECK, DisplayAxisLabels, 0L);
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
			_snprintf_s(PNGFile, _MAX_PATH, _TRUNCATE, "%s", GenerateAnimFileName (ANIMPNGPath, PNGName));
			SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGFile);
			return TRUE;

		    case IDOK:
//			GetDlgItemText(hDlg, IDC_ZBIAS, zScaling, 100);
//			zBias = atof(zScaling);
			GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName, MAX_PATH);
			fileptr = PNGName + strlen(PNGName);
			while (fileptr > PNGName && *fileptr != '.')
			    fileptr--;	
			if (*fileptr == '.')						// remove any extension
			    *fileptr = '\0';
			
			GetDlgItemText(hDlg, IDC_MORPHDELAY, TempStr, 20);
			OscProcess.MorphDelay = atof(TempStr);

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
			StartImmediately = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_LINES);
			DisplayLines = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			WritePNGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			WriteMemFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			WritePNGList = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem(hDlg, IDC_DISPLAYALLAXES);
			WritePNGList = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem(hDlg, IDC_DISPLAYALLAXES);
			OscProcess.DisplayAxisImages = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
//			hCtrl = GetDlgItem(hDlg, IDC_DISPLAYAXISLABELS);
//			DisplayAxisLabels = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			frames = GetDlgItemInt(hDlg, IDC_FRAMES, &bTrans, TRUE);
			if (frames <= 0)
			    frames = 20;
			if (frames > MAXANIM)
			    frames = MAXANIM;
			GenOscMorphScript(hDlg, ScriptFileName);
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

