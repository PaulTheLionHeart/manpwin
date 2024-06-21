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
#include	"anim.h"
#include	"colour.h"
#include	"OscProcess.h"
#include	"Matrix.h"

extern	HWND	GlobalHwnd;			// This is the main windows handle

extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);
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
extern	CTrueCol    TrueCol;			// palette info
//extern	BYTE	*PalettePtr;			// points to true colour palette
extern	BOOL	StartImmediately;		// immediate start of animation generation
//extern	BYTE	ScriptPaletteFlag;		// Display palette 

extern	long	threshold;
extern	double	mandel_width;			// width of display
extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address

extern	WORD	type;				// fractal type
extern	int	subtype;

extern	double	x_rot;				// angle display plane to x axis 
extern	double	y_rot;				// angle display plane to y axis 
extern	double	z_rot;				// angle display plane to z axis 
static	MATRIX	m;				// transformation matrice 
//extern	BYTE	_3dflag;		// replay saved file 3D

extern	void	ClearScreen(void);
//extern	CPixel	Pix;			// routines for projection and 3D transforms

//extern	void	init3d(void);
//extern	void	identity(MATRIXPTR);
//extern	void	trans(double, double, double, MATRIXPTR);
//extern	int	vmult(double *, MATRIXPTR, double *);
//extern	void	xrot(double, MATRIXPTR);
//extern	void	yrot(double, MATRIXPTR);
//extern	void	zrot(double, MATRIXPTR);

static	double	xRotInc, yRotInc, zRotInc; 
static	int	frames = 100;
//	BOOL	DisplayLines = TRUE;
static	int	RotateAnim = ROTATION;
extern	BOOL	DisplayAxes;
extern	BOOL	PlotCentre;		// display circle at the centre of the oscillator
extern	int	CentrePixels;		// radius of the display circle at the centre of the oscillator in pixels
extern	BOOL	BlockAnimation;		// used for 2D or less to preventanimating non-existent dimensions
extern	double	zBias;			// allow stretching of the image in the z direction

extern	ProcessType	OscAnimProc;
extern	COscProcess	OscProcess;

/**************************************************************************
	Initialise transformation Matrix
***************************************************************************/

/*
void	InitTransformation(double tx, double ty, double tz)

    {
    double	xval, yval, zval;
    CMatrix	Mat;

    if (z_rot > 360.0)  z_rot -= 360.0;
    if (y_rot > 360.0)  y_rot -= 360.0;
    if (x_rot > 360.0)  x_rot -= 360.0;

    xval = x_rot / 57.29577;					// rot values - convert degrees to radians
    yval = y_rot / 57.29577;
    zval = z_rot / 57.29577;
    Mat.identity(m);						// start with identity 
    Mat.trans(-tx, -ty, -tz, m);				// translate so origin is in centre, so that we rotate through the centre
    Mat.xrot(xval, m);						// rotate in each axis
    Mat.yrot(yval, m);
    Mat.zrot(zval, m);
    Mat.trans(tx, ty, tz, m);					// translate back
    }
*/

/**************************************************************************
	Implement transformation Matrix
***************************************************************************/

/*
void	DoTransformation(double *x1, double *y1, double *z1, double x, double y, double z)

    {
    VECTOR	w;
    CMatrix	Mat;

    w[0] = x;
    w[1] = y;
    w[2] = z;
    Mat.vmult(w, m, w);
    *x1 = w[0];
    *y1 = w[1];
    *z1 = w[2];
    }
*/

/**************************************************************************
	Oscillator and Fractal Map script generator
**************************************************************************/

int	GenOscillatorScript(HWND hwnd, char *filename) 

    {
    int		i, k;
    char	s[120];
    FILE	*out;
    char	ascii[6];

//    x_rot = y_rot = z_rot = 0.0;
    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%ld -s\"%s\" %s", threshold, PNGName, AnimData());	// add quotes to filename to trap spaces in path
    fprintf(out, " -c%24.24f,%24.24f,%24.24f\n", hor, vert, mandel_width);
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
	ConvertRGB2ASCII(*(TrueCol.PalettePtr + i * 3 + 0), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3 + 2), ascii);
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

DLGPROC FAR PASCAL OscillatorAnimDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
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
	        DatabasePtr = OscProcess.LoadDatabasePointer(type, subtype);
	        hCtrl = GetDlgItem (hDlg, IDC_SHOWPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, TrueCol.ScriptPaletteFlag, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, StartImmediately, 0L);
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
		if (type == OSCILLATORS)
		    sprintf(AnimType, "Osc");
		else if (type == FRACTALMAPS || type == SPROTTMAPS)
		    sprintf(AnimType, "Map");
		else if (type == SURFACES)
		    sprintf(AnimType, "Surface");
		else if (type == KNOTS)
		    sprintf(AnimType, "Knot");
		else if (type == CURVES)
		    sprintf(AnimType, "Curve");
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
		sprintf(zScaling, "%f", zBias);
		SetDlgItemText(hDlg, IDC_ZBIAS, zScaling);
		if (type == OSCILLATORS)
		    {
		    if (BlockAnimation)
//		    switch (subtype)	// these are NOT supported
			{
//			case 235:	// Lévy footprints on an african savanah after the rain
			    wsprintf(TempStr, "Chaotic Oscillator type %d (%s) cannot configured for animation\nIt is either 2D or it is not contained within a specified volume", subtype, OscillatorSpecific[subtype].name);
			    MessageBox (hDlg, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
			    EndDialog (hDlg, FALSE);
			    return FALSE;
//			default:
//			    break;
			}
		    }
		else if (type == FRACTALMAPS)
		    {
		    switch (subtype)	// these are NOT supported
			{
			case 99:	// if we ever get this far
			    wsprintf(TempStr, "Fractal Map type %d (%s) cannot be configured for animation\nIt is either 2D or it is not contained within a specified volume", subtype, FractalMapSpecific[subtype].name);
			    MessageBox (hDlg, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
			    EndDialog (hDlg, FALSE);
			    return FALSE;
//			case 32:	// initialisation of subtype failed - protect the program against crashing
//			    wsprintf(TempStr, "Fractal Map type %d does not exist", subtype);
//			    MessageBox (hDlg, TempStr, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
//			    EndDialog (hDlg, FALSE);
//			    return FALSE;
			default:
			    break;
			}
		    }
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

		    case IDC_ROTATIONANIM:
			hCtrl = GetDlgItem (hDlg, IDC_ROTATIONANIM);
			RotateAnim = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L) ? ROTATION : EVOLVE;
			hCtrl = GetDlgItem (hDlg, IDC_XAXIS);
			EnableWindow (hCtrl, (RotateAnim == ROTATION));
			hCtrl = GetDlgItem (hDlg, IDC_YAXIS);
			EnableWindow (hCtrl, (RotateAnim == ROTATION));
			hCtrl = GetDlgItem (hDlg, IDC_ZAXIS);
			EnableWindow (hCtrl, (RotateAnim == ROTATION));
			return (DLGPROC)TRUE;

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
/*
			RotationAxes = (type == OSCILLATORS) ? OscillatorSpecific[subtype].RotationAxes : FractalMapSpecific[subtype].RotationAxes;
			hCtrl = GetDlgItem (hDlg, IDC_XAXIS);
			if (SendMessage(hCtrl, BM_GETCHECK, 0, 0L) == FALSE)
			    xRotInc = 0.0;
			else
			    {
			    if ((RotationAxes & 0x07) == XAXIS)			// only XAXIS specified
				xRotInc = 360.0 / (double) frames;		// do one complete circle
			    else
				xRotInc = 360.0 * 1.37 / (double) frames;	// change the period of the rotation
			    }
			hCtrl = GetDlgItem (hDlg, IDC_YAXIS);
			if (SendMessage(hCtrl, BM_GETCHECK, 0, 0L) == FALSE)
			    yRotInc = 0.0;
			else
			    {
			    if ((RotationAxes & 0x07) == YAXIS)			// only YAXIS specified
				yRotInc = 360.0 / (double) frames;
			    else
				yRotInc = 360.0 * 1.59 / (double) frames;	// change the period of the rotation
			    }
			hCtrl = GetDlgItem (hDlg, IDC_ZAXIS);
			if (SendMessage(hCtrl, BM_GETCHECK, 0, 0L) == FALSE)
			    zRotInc = 0.0;
			else
			    {
			    if ((RotationAxes & 0x07) == YAXIS)			// only YAXIS specified
				zRotInc = 360.0 / (double) frames;
			    else
				zRotInc = 360.0 * 1.75 / (double) frames;	// change the period of the rotation
			    }
*/
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
			GenOscillatorScript(hDlg, ScriptFileName);
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

