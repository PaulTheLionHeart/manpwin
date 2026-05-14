/*
   OSCANIM.CPP a module to animate Chaotic Oscillators
   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	<time.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"menu.h"
//#include	"anim.h"
#include	"colour.h"
#include	"OscProcess.h"
#include	"Matrix.h"
#include	"SafeStrings.h"

//extern	HWND	GlobalHwnd;			// This is the main windows handle

//extern	void	ConvertRGB2ASCII(RGBTRIPLE, char *);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	char	*AnimData(void);
//extern	int	GenMalthusScript(HWND hwnd, char *filename);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);

extern	BOOL	WritePNGFrames;			// write frames to PNG files
extern	BOOL	WriteMemFrames;			// write frames to memory
extern	BOOL	WritePNGList;			// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;		// write frames directly to an MPEG file

extern	char	ANIMPNGPath[];			// path for animated PNG files and LST files
extern	char	MPGPath[];			// path for PNG files
extern	char	PNGPath[];			// path for MPEG files
extern	char	MPGFile[];			// MPEG file
extern	char	PNGFile[];			// PNG file
extern	char	PNGName[];			// base name for PNG file sequence
extern	char	ScriptFileName[];		// base name for script file 

extern	char	PNGName[];			// base name for PNG file sequence
static	MATRIX	m;				// transformation matrice 

//extern	void	ClearScreen(void);

static	double	xRotInc, yRotInc, zRotInc; 
static	int	frames = 100;
//	BOOL	DisplayLines = TRUE;
static	int	RotateAnim = ROTATION;
extern	BOOL	DisplayAxes;
extern	BOOL	PlotCentre;		// display circle at the centre of the oscillator
extern	int	CentrePixels;		// radius of the display circle at the centre of the oscillator in pixels
extern	BOOL	BlockAnimation;		// used for 2D or less to preventanimating non-existent dimensions
extern	double	zBias;			// allow stretching of the image in the z direction

/**************************************************************************
	Oscillator and Fractal Map script generator
**************************************************************************/

int	CManp::GenOscillatorScript(HWND hwnd, char *filename) 
    {
    int		i, k;
    char	s[120];
    FILE	*out;
    char	ascii[6];

//    x_rot = y_rot = z_rot = 0.0;
    if ((out = fopen(filename, "w")) == NULL)
	{
	SAFE_SPRINTF(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", gManp->hor, gManp->vert, gManp->mandel_width);
    if (RotateAnim == ROTATION)
	fprintf(out, "Oscillator Rotation: %d %d %d\n", threshold, threshold, frames);
    else
	fprintf(out, "Oscillator Evolution: %d %d %d\n", threshold, threshold, frames);
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
	fprintf(out, "-2%f,%f,%f,%f,%f,%f\n", 1.0, 1.0, 1.0, x_rot + i * xRotInc, y_rot + i * yRotInc, z_rot + i * zRotInc);
    fclose(out);
    return 0;
    }

/**************************************************************************
	Dialog For Setting Up Oscillator or Fractal Map Animation
**************************************************************************/

INT_PTR CALLBACK OscillatorAnimDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     char		*fileptr;
     HWND		hCtrl;
     BOOL		TempCheck;
     BOOL		bTrans;
     int		RotationAxes;
     char		zScaling[24];
     static	char	TempFile[MAX_PATH];
     static	char	TempStr[MAX_PATH];
     char		AnimType[24];
     static	struct	OscillatorSpecificStuff	*DatabasePtr; 

     switch (message)
	  {
	  case WM_INITDIALOG:
	        DatabasePtr = gManp->OscProcess.LoadDatabasePointer(gManp->type, gManp->subtype);
	        hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, gManp->TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, gManp->StartImmediately, 0L);
		RotationAxes = DatabasePtr->RotationAxes;
		hCtrl = GetDlgItem (hDlg, IDC_XAXIS);
		SendMessage(hCtrl, BM_SETCHECK, RotationAxes & XAXIS, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_YAXIS);
		SendMessage(hCtrl, BM_SETCHECK, RotationAxes & YAXIS, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_ZAXIS);
		SendMessage(hCtrl, BM_SETCHECK, RotationAxes & ZAXIS, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_ROTATIONANIM);
		SendMessage(hCtrl, BM_SETCHECK, !(RotationAxes & EVOLVE), 0L);
//		hCtrl = GetDlgItem (hDlg, IDC_PLOTLINES);
//		SendMessage(hCtrl, BM_SETCHECK, DisplayLines, 0L);
		if (gManp->type == OSCILLATORS)
		    SAFE_SPRINTF(AnimType, "Osc");
		else if (gManp->type == FRACTALMAPS || gManp->type == SPROTTMAPS)
		    SAFE_SPRINTF(AnimType, "Map");
		else if (gManp->type == SURFACES)
		    SAFE_SPRINTF(AnimType, "Surface");
		else if (gManp->type == KNOTS)
		    SAFE_SPRINTF(AnimType, "Knot");
		else if (gManp->type == CURVES)
		    SAFE_SPRINTF(AnimType, "Curve");
		SetUpFilename(ScriptFileName, "sci", AnimType);
		SetUpFilename(PNGName, "animpng", AnimType);
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
//		hCtrl = GetDlgItem (hDlg, IDC_ROTATIONANIM);
//		SendMessage(hCtrl, BM_SETCHECK, TRUE, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_SHOWAXES);
		SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_SHOWCENTRE);
		SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
		SetDlgItemInt(hDlg, IDC_RADIUS, CentrePixels, TRUE);
		SAFE_SPRINTF(zScaling, "%f", zBias);
		SetDlgItemText(hDlg, IDC_ZBIAS, zScaling);
		if (gManp->type == OSCILLATORS)
		    {
		    if (BlockAnimation)
//		    switch (subtype)	// these are NOT supported
			{
//			case 235:	// Lévy footprints on an african savanah after the rain
			    _snprintf_s(TempStr, MAX_PATH, _TRUNCATE, "Chaotic Oscillator type %d (%s) cannot configured for animation\nIt is either 2D or it is not contained within a specified volume", gManp->subtype, OscillatorSpecific[gManp->subtype].name);
			    MessageBox (hDlg, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
			    EndDialog (hDlg, FALSE);
			    return FALSE;
//			default:
//			    break;
			}
		    }
		else if (gManp->type == FRACTALMAPS)
		    {
		    switch (gManp->subtype)	// these are NOT supported
			{
			case 99:	// if we ever get this far
			    _snprintf_s(TempStr, MAX_PATH, _TRUNCATE, "Fractal Map type %d (%s) cannot be configured for animation\nIt is either 2D or it is not contained within a specified volume", gManp->subtype, FractalMapSpecific[gManp->subtype].name);
			    MessageBox (hDlg, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
			    EndDialog (hDlg, FALSE);
			    return FALSE;
//			case 32:	// initialisation of subtype failed - protect the program against crashing
//			    _snprintf_s(TempStr, MAX_PATH, _TRUNCATE, "Fractal Map type %d does not exist", subtype);
//			    MessageBox (hDlg, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
//			    EndDialog (hDlg, FALSE);
//			    return FALSE;
			default:
			    break;
			}
		    }
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

		    case IDC_ROTATIONANIM:
			hCtrl = GetDlgItem (hDlg, IDC_ROTATIONANIM);
			RotateAnim = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L) ? ROTATION : EVOLVE;
			hCtrl = GetDlgItem (hDlg, IDC_XAXIS);
			EnableWindow (hCtrl, (RotateAnim == ROTATION));
			hCtrl = GetDlgItem (hDlg, IDC_YAXIS);
			EnableWindow (hCtrl, (RotateAnim == ROTATION));
			hCtrl = GetDlgItem (hDlg, IDC_ZAXIS);
			EnableWindow (hCtrl, (RotateAnim == ROTATION));
			return TRUE;

		    case IDOK:
			GetDlgItemText(hDlg, IDC_ZBIAS, zScaling, 100);
			zBias = atof(zScaling);
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
			hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
			gManp->TrueCol.ScriptPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
//			hCtrl = GetDlgItem (hDlg, IDC_PLOTLINES);
//			DisplayLines = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
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

			hCtrl = GetDlgItem (hDlg, IDC_XAXIS);
			if (SendMessage(hCtrl, BM_GETCHECK, 0, 0L) == FALSE)
			    xRotInc = 0.0;
			else
			    xRotInc = 360.0 / (double) frames;			    // do one complete circle
			hCtrl = GetDlgItem (hDlg, IDC_YAXIS);
			if (SendMessage(hCtrl, BM_GETCHECK, 0, 0L) == FALSE)
			    yRotInc = 0.0;
			else
			    yRotInc = 360.0 / (double) frames;
			hCtrl = GetDlgItem (hDlg, IDC_ZAXIS);
			if (SendMessage(hCtrl, BM_GETCHECK, 0, 0L) == FALSE)
			    zRotInc = 0.0;
			else
			    zRotInc = 360.0 / (double) frames;
//			type = OSCILLATORS;
//			_3dflag = TRUE;
			hCtrl = GetDlgItem (hDlg, IDC_ROTATIONANIM);
			RotateAnim = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L) ? ROTATION : EVOLUTION;
			hCtrl = GetDlgItem (hDlg, IDC_SHOWAXES);
			DisplayAxes = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_SHOWCENTRE);
			PlotCentre = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			CentrePixels = GetDlgItemInt(hDlg, IDC_RADIUS, &bTrans, TRUE);
			if (CentrePixels <= 0)
			    frames = 1;
			gManp->GenOscillatorScript(hDlg, ScriptFileName);
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

