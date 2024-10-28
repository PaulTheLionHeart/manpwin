/**************************************************************************
	MANPDLG.CPP
	Manp Dialog Code
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <windows.h>
#include <fcntl.h>
#define  VIEWMAIN
#include "manpwin.h"
#include "manp.h"
#include "colour.h"
#include "fractype.h"
#include "fractalp.h"
//#include "default.c"
#include "resource.h"
#include "menu.h"
#include "Fract.h"
#include "BigDouble.h"
#include "big.h"
#include "pixel.h"
#include ".\parser\cmplx.h"
#include ".\parser\TrigFn.h"
//#define	MAXTHRESHOLD	10000

extern	double	hor;			// horizontal address
extern	double	vert;			// vertical address
extern	double	mandel_width;		// final width
extern	double	ScreenRatio;		// ratio of width / height for the screen
extern	long	threshold;		// maximum iterations
extern	int	Offset3D;		// offset to threshold for 3D display
extern	BYTE	calcmode;		// 'B' boundary, 'G' guess, etc
extern	BYTE	oldcalcmode;		// store values during 3D transformations, filters etc
extern	BYTE	cycleflag;		// do colour cycling
extern	int	logval, logflag;	// log colour map starting value and flag for use of log palette
extern	WORD	type;			// M=mand, N=Newton etc
extern	int	subtype;		
extern	char	LyapSequence[];		// hold the AB sequence for Lyapunov fractals

/*			    Sub types:
Newton			    B=basin, S=stripe, N=normal
Bifurcation		    Q = quad mand, B = bifurcation
Cubic			    B = CBIN, C = CCIN, F = CFIN, K = CKIN
Rational Map		    3 = RAT34, 4 = RAT44
*/
extern	int	CoordSystem;
extern	WORD	special;		// special colour, phase etc
//extern	BYTE	degree;			// power, degree
//extern	BYTE	_3dflag;		// replay saved file. 3 = 3D
extern	RGBTRIPLE OrbitColour;		// Indexed colour for the orbit displays in Julia sets
extern	int	biomorph;		// biomorph colour
extern	int	decomp;			// number of decomposition colours
extern	int	blockindex;		// for solid guessing blocksize
extern	BYTE	pairflag;		// stereo pair
extern	int	AutoStereo_value;	// AutoStereo depth value
extern	double	param[];
extern	double	potparam[];
extern	BOOL	RGBFilter;		// If true, we use the plotting routine for RGB filters (no plotting of iteration)
extern	BOOL	UseCurrentPalette;	// do we use the ManpWIN palette? If false, generate internal filter palette
extern	double	x_rot;			// angle display plane to x axis
extern	double	y_rot;			// angle display plane to y axis
extern	double	z_rot;			// angle display plane to z axis
extern	double	sclx, scly, sclz;	// scale
extern	int	HenonPoints;
extern	int	InsideMethod;		// the number of the inside filter
extern	int	OutsideMethod;		// the number of the outside filter
extern	int	RotationAngle;		// in degrees
extern	Complex	RotationCentre;		// centre of rotation
extern	int	BailoutTestType;	// type of bailout test

BOOL	ExpandStarTrailColours = TRUE;	// use the first 16 colours if false, else expand across the whole iteration range
extern	double	dStrands;		// for Tierazon filters
	long    fillcolor = -1;		// tesseral fillcolor: -1=normal 0 means don't fill
extern	BOOL	invert;			// invert fractal
extern	double	f_radius, f_xcenter, f_ycenter;	// inversion radius, center 
extern	char	*InitCond[];		// initial conditions for direct formula parsing 
extern	char	*DirectFormula[];	// formulae for direct formula parsing 
extern	DWORD	BackgroundColour;	// set background colour for IFS and L-System fractals
extern	int	distest, distestwidth;	// distance estimation
extern	int	PlotType;
extern	Complex	j;
extern	BYTE	juliaflag;		// Julia implementation of fractal 

// stuff for DwdDiff algorithm
extern	double	bump_transfer_factor;
extern	int	PaletteStart;
extern	double	lightDirectionDegrees;
extern	double	bumpMappingDepth;
extern	double	bumpMappingStrength;
extern	double	SlopeError;			// used as error value for Newton type Fwd Diff bailout calcs

// Big num declarations **********************************************************
//extern	int	dec;
extern	BYTE	BigNumFlag;		// True if bignum used
//extern	bf_t	BigTemp, BigTemp1, BigTemp2, BigTemp3, BigTemp4;
extern	BigDouble   BigHor, BigVert, BigWidth;
// Big num declarations **********************************************************

#define	MAXFUNCTIONS	60

extern	WORD	type;			// fractal type

extern	void	init_log(HWND);
extern	int	analyse_corner(char *);
extern	int	FindInitCond(char *Str);
extern	int	FindFormula(char *Str);
extern	void	ConvertBignum2String(char *s, mpfr_t num);
extern	CMatrix	Mat;			// transformation and rotation matrix

//extern	void	SaveUndo(void);
//extern	int	atox(char);
extern	void	InitFract(int);

extern	CFract	Fractal;
extern	CTrueCol    TrueCol;
//extern	CPixel	Pixel;		// routines for escape fractals

/**************************************************************************
	Dialog Control for 3D Parameters
**************************************************************************/

DLGPROC FAR PASCAL Param3D (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
//     HWND		hCtrl ;
//     static	HANDLE	hCursor;
//     short		nCtrlID, filterSum, i, divisor; 
     char		s[100];
     BOOL		bTrans ;
     static	int	temp_xrot, temp_yrot, temp_zrot; 
     static     char	temp_xscale[20], temp_yscale[20], temp_zscale[20];
     static     char	temp;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        sprintf(s, "%2.4f", sclx);
		SetDlgItemText(hDlg, IDC_XSCALE, s);
	        sprintf(s, "%2.4f", scly);
		SetDlgItemText(hDlg, IDC_YSCALE, s);
	        sprintf(s, "%2.4f", sclz);
		SetDlgItemText(hDlg, IDC_ZSCALE, s);
		SetDlgItemInt(hDlg, IDC_XROTATE, (int)x_rot, TRUE);
		SetDlgItemInt(hDlg, IDC_YROTATE, (int)y_rot, TRUE);
		SetDlgItemInt(hDlg, IDC_ZROTATE, (int)z_rot, TRUE);
	        return (DLGPROC) TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_XSCALE:
			GetDlgItemText(hDlg, IDC_XSCALE, temp_xscale, 30);
		        return (DLGPROC) TRUE ;

		    case IDC_YSCALE:
			GetDlgItemText(hDlg, IDC_YSCALE, temp_yscale, 30);
		        return (DLGPROC) TRUE ;

		    case IDC_ZSCALE:
			GetDlgItemText(hDlg, IDC_ZSCALE, temp_zscale, 30);
		        return (DLGPROC) TRUE ;

		    case IDC_XROTATE:
			temp_xrot = GetDlgItemInt(hDlg, IDC_XROTATE, &bTrans, TRUE);
		        return (DLGPROC) TRUE ;

		    case IDC_YROTATE:
			temp_yrot = GetDlgItemInt(hDlg, IDC_YROTATE, &bTrans, TRUE);
		        return (DLGPROC) TRUE ;

		    case IDC_ZROTATE:
			temp_zrot = GetDlgItemInt(hDlg, IDC_ZROTATE, &bTrans, TRUE);
		        return (DLGPROC) TRUE ;

		    case IDOK:
			sscanf(temp_xscale, "%lf", &sclx);
			sscanf(temp_yscale, "%lf", &scly);
			sscanf(temp_zscale, "%lf", &sclz);
			x_rot = (double)temp_xrot;
			y_rot = (double)temp_yrot;
			z_rot = (double)temp_zrot;
			cycleflag = FALSE;
			EndDialog (hDlg, TRUE);
			return (DLGPROC) TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }

#ifdef TESTFWDDIFF

/**************************************************************************
	Dialog Control for Fractal Parameters (test version for mFwd Diff testing)
**************************************************************************/

DLGPROC FAR PASCAL FractalTestDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)
    {
//    HWND		hCtrl;
    //     static	HANDLE	hCursor;
    //     short		nCtrlID, filterSum, i, divisor; 
    char		s[100];
    BOOL		bTrans;
    static	int	temp_logval, temp_inside, temp_biomorph,
	temp_decomp, temp_3Dthreshold;
    static	long	temp_threshold;
    static	short	temp_blockindex;
    static     char	temp;
    static     short	tempMethod;
    static     UINT	tempParam;
    static     WORD	temp_special;
    static     WORD	temp_OrbitColour;
    static     WORD	temp_orientation;
    static     long	temp_fillcolor;

    switch (message)
	{
	case WM_INITDIALOG:
	    sprintf(s, "%5.2f", bump_transfer_factor);
	    SetDlgItemText(hDlg, IDM_TRANSFACTOR, s);
	    SetDlgItemInt(hDlg, IDM_STARTPAL, PaletteStart, TRUE);
	    sprintf(s, "%5.2f", lightDirectionDegrees);
	    SetDlgItemText(hDlg, IDM_LIGHTDIR, s);
	    sprintf(s, "%5.2f", bumpMappingDepth);
	    SetDlgItemText(hDlg, IDM_MAPDEPTH, s);
	    sprintf(s, "%5.2f", bumpMappingStrength);
	    SetDlgItemText(hDlg, IDM_MAPSTRENGTH, s);

	    SetDlgItemInt(hDlg, IDM_TYPE, type, TRUE);
	    SetDlgItemInt(hDlg, IDM_SUBTYPE, subtype, TRUE);
	    SetDlgItemInt(hDlg, IDM_BAILOUTTYPE, fractalspecific[type].BailoutType, TRUE);
	    SetDlgItemInt(hDlg, IDM_SLOPEDEGREE, fractalspecific[type].SlopeDegree, TRUE);
	    sprintf(s, "%5.2g", SlopeError);
	    SetDlgItemText(hDlg, IDM_SLOPEERROR, s);

	    if (type == 204)													// Tierazon
		{
		SetDlgItemInt(hDlg, IDM_BAILOUTTYPE, TierazonSpecific[subtype].BailoutType, TRUE);
		SetDlgItemInt(hDlg, IDM_SLOPEDEGREE, TierazonSpecific[subtype].SlopeDegree, TRUE);
		}
	    else if (type == 228)													// Mandel derivatives
		{
		SetDlgItemInt(hDlg, IDM_BAILOUTTYPE, MandelDerivSpecific[subtype].BailoutType, TRUE);
		SetDlgItemInt(hDlg, IDM_SLOPEDEGREE, MandelDerivSpecific[subtype].SlopeDegree, TRUE);
		}
	    else
		{
		SetDlgItemInt(hDlg, IDM_BAILOUTTYPE, fractalspecific[type].BailoutType, TRUE);
		SetDlgItemInt(hDlg, IDM_SLOPEDEGREE, fractalspecific[type].SlopeDegree, TRUE);
		}


	    temp = calcmode;
	    switch (calcmode)
		{
		case 'G':
		    tempParam = IDC_GUESS;
		    break;
		case 'B':
		    tempParam = IDC_BOUNDARY;
		    break;
		case 'T':
		    tempParam = IDC_TESSERAL;
		    break;
		case '1':
		    tempParam = IDC_1PASS;
		    break;
		case '2':
		    tempParam = IDC_2PASS;
		    break;
		case '3':
		    tempParam = IDC_3PASS;
		    break;
		case 'H':
		    tempParam = IDC_HORBLINDS;
		    break;
		case 'V':
		    tempParam = IDC_VERTBLINDS;
		    break;
		case 'S':
		    tempParam = IDC_SPIRAL;
		    break;
		case 'F':
		    tempParam = IDC_FWDDIFF;
		    break;
		}
	    CheckRadioButton(hDlg, IDC_GUESS, IDC_FWDDIFF, tempParam);

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		//	        switch (wParam)
		{
		case IDC_GUESS:
		case IDC_BOUNDARY:
		case IDC_TESSERAL:
		case IDC_1PASS:
		case IDC_2PASS:
		case IDC_3PASS:
		case IDC_HORBLINDS:
		case IDC_VERTBLINDS:
		case IDC_SPIRAL:
		case IDC_FWDDIFF:
		    switch ((int)LOWORD(wParam))
			{
			case IDC_GUESS:
			    temp = 'G';
			    break;
			case IDC_BOUNDARY:
			    temp = 'B';
			    break;
			case IDC_TESSERAL:
			    temp = 'T';
			    break;
			case IDC_1PASS:
			    temp = '1';
			    break;
			case IDC_2PASS:
			    temp = '2';
			    break;
			case IDC_3PASS:
			    temp = '3';
			    break;
			case IDC_HORBLINDS:
			    temp = 'H';
			    break;
			case IDC_VERTBLINDS:
			    temp = 'V';
			    break;
			case IDC_SPIRAL:
			    temp = 'S';
			    break;
			case IDC_FWDDIFF:
			    temp = 'F';
			    break;
			}
		    CheckRadioButton(hDlg, IDC_GUESS, IDC_FWDDIFF, (int)LOWORD(wParam));
		    return (DLGPROC)TRUE;

		case IDOK:
		    calcmode = temp;

		    GetDlgItemText(hDlg, IDM_TRANSFACTOR, s, 30);
		    sscanf(s, "%lf", &bump_transfer_factor);
		    PaletteStart = GetDlgItemInt(hDlg, IDM_STARTPAL, &bTrans, TRUE);
		    GetDlgItemText(hDlg, IDM_LIGHTDIR, s, 30);
		    sscanf(s, "%lf", &lightDirectionDegrees);
		    GetDlgItemText(hDlg, IDM_MAPDEPTH, s, 30);
		    sscanf(s, "%lf", &bumpMappingDepth);
		    GetDlgItemText(hDlg, IDM_MAPSTRENGTH, s, 30);
		    sscanf(s, "%lf", &bumpMappingStrength);

		    GetDlgItemText(hDlg, IDM_SLOPEERROR, s, 30);
		    sscanf(s, "%lg", &SlopeError);
		    fractalspecific[type].BailoutType = GetDlgItemInt(hDlg, IDM_BAILOUTTYPE, &bTrans, TRUE);
		    fractalspecific[type].SlopeDegree = GetDlgItemInt(hDlg, IDM_SLOPEDEGREE, &bTrans, TRUE);
		    if (type == 204)													// Tierazon
			{
			TierazonSpecific[subtype].BailoutType = GetDlgItemInt(hDlg, IDM_BAILOUTTYPE, &bTrans, TRUE);
			TierazonSpecific[subtype].SlopeDegree = GetDlgItemInt(hDlg, IDM_SLOPEDEGREE, &bTrans, TRUE);
			}
		    else if (type == 228)													// Mandel derivatives
			{
			MandelDerivSpecific[subtype].BailoutType = GetDlgItemInt(hDlg, IDM_BAILOUTTYPE, &bTrans, TRUE);
			MandelDerivSpecific[subtype].SlopeDegree = GetDlgItemInt(hDlg, IDM_SLOPEDEGREE, &bTrans, TRUE);
			}
		    else
			{
			fractalspecific[type].BailoutType = GetDlgItemInt(hDlg, IDM_BAILOUTTYPE, &bTrans, TRUE);
			fractalspecific[type].SlopeDegree = GetDlgItemInt(hDlg, IDM_SLOPEDEGREE, &bTrans, TRUE);
			}

		    EndDialog(hDlg, TRUE);
		    return (DLGPROC)TRUE;

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    break;
	}
    return FALSE;
    }

#else

/**************************************************************************
	Dialog Control for Fractal Parameters
**************************************************************************/

DLGPROC FAR PASCAL FractalDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     HWND		hCtrl ;
//     static	HANDLE	hCursor;
//     short		nCtrlID, filterSum, i, divisor; 
     char		s[100];
     BOOL		bTrans ;
     static	int	temp_logval, temp_inside, temp_biomorph, temp_decomp, temp_3Dthreshold, OrigRotationAngle;
     static	long	temp_threshold;
     static	short	temp_blockindex;
     static     char	temp;
     static     short	tempInsideMethod;
     static     short	tempOutsideMethod;
     static     UINT	tempParam;
     static     WORD	temp_special;
     static     long	temp_OrbitColour;
     static     WORD	temp_RotationAngle;
     static     long	temp_fillcolor;
     static     int	TempBailoutTest;

     switch (message)
	  {
	  case WM_INITDIALOG:
//		dStrands = 0.08;
		SetDlgItemInt(hDlg, IDM_THRESHOLD, threshold, TRUE);
		sprintf(s, "%02X%02X%02X", OrbitColour.rgbtRed, OrbitColour.rgbtGreen, OrbitColour.rgbtBlue);
		SetDlgItemText(hDlg, IDC_ORBITCOL, s);
		SetDlgItemInt(hDlg, IDM_ROTDEG, RotationAngle, TRUE);
		SetDlgItemInt(hDlg, IDM_THRESH_OFF, Offset3D, TRUE);
		SetDlgItemInt(hDlg, IDM_BLKINDEX, blockindex, TRUE);
		SetDlgItemInt(hDlg, IDM_LOGVAL, logval, TRUE);
		SetDlgItemInt(hDlg, IDM_TESSFILLCOL, fillcolor, TRUE);
		SetDlgItemInt(hDlg, IDM_DECOMP, decomp, TRUE);
		SetDlgItemInt(hDlg, IDC_SPECIAL, special, TRUE);
		SetDlgItemInt(hDlg, IDM_BIOMORPH, biomorph, TRUE);
		SetDlgItemInt(hDlg, IDM_MAXCOL, (int)potparam[0], TRUE);
		SetDlgItemInt(hDlg, IDM_SLOPE, (int)potparam[1], TRUE);
		SetDlgItemInt(hDlg, IDM_BAILOUT, (int)potparam[2], TRUE);
		SetDlgItemInt(hDlg, IDC_DISTEST, distest, TRUE);
		SetDlgItemInt(hDlg, IDC_DISTESTWIDTH, distestwidth, TRUE);
	        sprintf(s, "%7.7f", f_radius);
		SetDlgItemText(hDlg, IDM_RADIUS, s);
	        sprintf(s, "%7.7f", f_xcenter);
		SetDlgItemText(hDlg, IDM_CENTREX, s);
	        sprintf(s, "%7.7f", f_ycenter);
		SetDlgItemText(hDlg, IDM_CENTREY, s);
	        sprintf(s, "%2.2f", dStrands);
		SetDlgItemText(hDlg, IDM_STALKS, s);
		sprintf(s, "%06X", BackgroundColour);
		SetDlgItemText(hDlg, IDC_BKGROUNDCOLOUR, s);

		sprintf(s, "%5.2f", bump_transfer_factor);
		SetDlgItemText(hDlg, IDM_TRANSFACTOR, s);
		SetDlgItemInt(hDlg, IDM_STARTPAL, PaletteStart, TRUE);
		sprintf(s, "%5.2f", lightDirectionDegrees);
		SetDlgItemText(hDlg, IDM_LIGHTDIR, s);
		sprintf(s, "%5.2f", bumpMappingDepth);
		SetDlgItemText(hDlg, IDM_MAPDEPTH, s);
		sprintf(s, "%5.2f", bumpMappingStrength);
		SetDlgItemText(hDlg, IDM_MAPSTRENGTH, s);
		
		TempBailoutTest = BailoutTestType;
		switch (TempBailoutTest)
		    {
		    case BAIL_MOD:
			tempParam = IDC_BAIL_MOD;
			break;
		    case BAIL_REAL:
			tempParam = IDC_BAIL_REAL;
			break;
		    case BAIL_IMAG:
			tempParam = IDC_BAIL_IMAG;
			break;
		    case BAIL_OR:
			tempParam = IDC_BAIL_OR;
			break;
		    case BAIL_AND:
			tempParam = IDC_BAIL_AND;
			break;
		    case MANH:
			tempParam = IDC_MANH;
			break;
		    case MANR:
			tempParam = IDC_MANR;
			break;
		    }

		CheckRadioButton(hDlg, IDC_BAIL_MOD, IDC_MANR, tempParam);

		tempInsideMethod = InsideMethod;
	        switch (tempInsideMethod)
		    {
		    case NONE:
			tempParam = IDC_NO_INSIDE;
			break;
		    case ZMAG:
			tempParam = IDC_ZMAG;
			break;
		    case BOF60:
			tempParam = IDC_BOF60;
			break;
		    case BOF61:
			tempParam = IDC_BOF61;
			break;
//		    case ATAN:
//			tempParam = IDC_ATAN;
//			break;
		    }
		CheckRadioButton(hDlg, IDC_NO_INSIDE, IDC_BOF61, tempParam);

		tempOutsideMethod = OutsideMethod;
		// can't access tierazon filter/colour methods, so set the radio button to TIERAZONFILTERS or TIERAZONCOLOURS
		if (OutsideMethod > TIERAZONFILTERS)
		    tempOutsideMethod = (OutsideMethod > TIERAZONCOLOURS) ? TIERAZONCOLOURS : TIERAZONFILTERS;
		switch (tempOutsideMethod)
		    {
		    case NONE:
			tempParam = IDC_NO_OUTSIDE;
			break;
		    case REAL:
			tempParam = IDC_REAL1;
			break;
		    case IMAG:
			tempParam = IDC_IMAG1;
			break;
		    case PERT1:
			tempParam = IDC_PERT1;
			break;
		    case PERT2:
			tempParam = IDC_PERT2;
			break;
		    case MULT:
			tempParam = IDC_MULT;
			break;
		    case SUM:
			tempParam = IDC_SUM;
			break;
		    case ATAN:
			tempParam = IDC_ATAN;
			break;
		    case POTENTIAL:
			tempParam = IDC_POTENTIAL;
			break;
		    case EPSCROSS:
			tempParam = IDC_EPSCR;
			break;
		    case STARTRAIL:
			tempParam = IDC_STARTRAILS;
			break;
		    case TIERAZONFILTERS:
			tempParam = IDC_TIERAZONFILTERS;
			break;
		    case TIERAZONCOLOURS:
			tempParam = IDC_TIERAZONCOLOURS;
			break;
		    }

		CheckRadioButton(hDlg, IDC_NO_OUTSIDE, IDC_TIERAZONCOLOURS, tempParam);
		CheckRadioButton(hDlg, IDC_CARTESIAN, IDC_CONICAL, IDC_CARTESIAN + CoordSystem);

		temp = calcmode;
	        switch (calcmode)
		    {
		    case 'G':
			tempParam = IDC_GUESS;
			break;
		    case 'B':
			tempParam = IDC_BOUNDARY;
			break;
		    case 'T':
			tempParam = IDC_TESSERAL;
			break;
		    case '1':
			tempParam = IDC_1PASS;
			break;
		    case '2':
			tempParam = IDC_2PASS;
			break;
//		    case '3':
//			tempParam = IDC_3PASS;
//			break;
		    case 'H':
			tempParam = IDC_HORBLINDS;
			break;
		    case 'V':
			tempParam = IDC_VERTBLINDS;
			break;
		    case 'S':
			tempParam = IDC_SPIRAL;
			break;
		    case 'F':
			tempParam = IDC_FWDDIFF;
			break;
		    }
		CheckRadioButton(hDlg, IDC_GUESS, IDC_FWDDIFF, tempParam);

		temp_RotationAngle = RotationAngle % 360;
	        switch (RotationAngle)
		    {
		    case NORMAL:
			tempParam = IDC_NORMAL;
			break;
		    case 90:
			tempParam = IDC_90DEGREES;
			break;
		    case 180:
			tempParam = IDC_180DEGREES;
			break;
		    case 270:
			tempParam = IDC_270DEGREES;
			break;
		    default:
			tempParam = IDM_ROTDEG;
			temp_RotationAngle = GetDlgItemInt(hDlg, IDM_ROTDEG, &bTrans, TRUE);
		    }
		CheckRadioButton(hDlg, IDC_NORMAL, IDC_OTHER_DEGREES, tempParam);

		hCtrl = GetDlgItem (hDlg, IDC_STRETCHPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, TrueCol.Stretch, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_EXPANDSTARCOLS);
		SendMessage(hCtrl, BM_SETCHECK, ExpandStarTrailColours, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_USEPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, UseCurrentPalette, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_INVERTFRACT);
		SendMessage(hCtrl, BM_SETCHECK, invert, 0L);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return (DLGPROC) TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
/*
		    case IDM_THRESHOLD:
			temp_threshold = GetDlgItemInt(hDlg, IDM_THRESHOLD, &bTrans, TRUE);
			if (temp_threshold > MAXTHRESHOLD)
			    temp_threshold = MAXTHRESHOLD;
			if (temp_threshold < 1)
			    temp_threshold = 1;
		        return TRUE ;
*/

		    case IDC_CARTESIAN:
			CoordSystem = CARTESIAN;
			break;
		    case IDC_SPHERICAL:
			CoordSystem = SPHERICAL;
			break;
		    case IDC_CYLINDRICAL:
			CoordSystem = CYLINDRICAL;
			break;
		    case IDC_PARABOLIC:
			CoordSystem = PARABOLIC;
			break;
		    case IDC_PARABOLOIDAL:
			CoordSystem = PARABOLOIDAL;
			break;
		    case IDC_ELLIPTICAL:
			CoordSystem = ELLIPTICAL;
			break;
		    case IDC_BIPOLAR:
			CoordSystem = BIPOLAR;
			break;
		    case IDC_TOROIDAL:
			CoordSystem = TOROIDAL;
			break;
		    case IDC_PROLATE_SPHEROIDAL:
			CoordSystem = PROLATE_SPHERE;
			break;
		    case IDC_OBLATE_SPHEROIDAL:
			CoordSystem = OBLATE_SPHERE;
			break;
		    case IDC_BISPHERICAL:
			CoordSystem = BISPHERICAL;
			break;
		    case IDC_CONICAL:
			CoordSystem = CONICAL;
			break;
		    case IDC_USEPALETTE:
			hCtrl = GetDlgItem (hDlg, IDC_USEPALETTE) ;
			UseCurrentPalette = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return (DLGPROC) TRUE ;

		    case IDC_EXPANDSTARCOLS:
			hCtrl = GetDlgItem (hDlg, IDC_EXPANDSTARCOLS) ;
			ExpandStarTrailColours = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return (DLGPROC) TRUE ;

		    case IDC_STRETCHPALETTE:
			hCtrl = GetDlgItem (hDlg, IDC_STRETCHPALETTE) ;
			TrueCol.Stretch = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return (DLGPROC) TRUE ;

		    case IDC_INVERTFRACT:
			hCtrl = GetDlgItem (hDlg, IDC_INVERTFRACT) ;
			invert = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return (DLGPROC) TRUE ;

		    case IDM_THRESH_OFF:
			temp_3Dthreshold = GetDlgItemInt(hDlg, IDM_THRESH_OFF, &bTrans, TRUE);
			if (temp_3Dthreshold > MAXTHRESHOLD)
			    temp_3Dthreshold = MAXTHRESHOLD;
			if (temp_3Dthreshold < 0)
			    temp_3Dthreshold = 0;
		        return (DLGPROC) TRUE ;

		    case IDM_LOGVAL:
			temp_logval = GetDlgItemInt(hDlg, IDM_LOGVAL, &bTrans, TRUE);
		        return (DLGPROC) TRUE ;

		    case IDM_BLKINDEX:
			temp_blockindex = GetDlgItemInt(hDlg, IDM_BLKINDEX, &bTrans, TRUE);
			if (temp_blockindex < 1)
			    temp_blockindex = 1;
			else if (temp_blockindex > 6)
			    temp_blockindex = 6;
		        return (DLGPROC) TRUE ;

		    case IDM_TESSFILLCOL:
			temp_fillcolor = GetDlgItemInt(hDlg, IDM_TESSFILLCOL, &bTrans, TRUE);
		        return (DLGPROC) TRUE ;

		    case IDM_BIOMORPH:
			temp_biomorph = GetDlgItemInt(hDlg, IDM_BIOMORPH, &bTrans, TRUE);
		        return (DLGPROC) TRUE ;

		    case IDM_DECOMP:
			temp_decomp = GetDlgItemInt(hDlg, IDM_DECOMP, &bTrans, TRUE);
			return (DLGPROC)TRUE;

		    case IDC_SPECIAL:
			temp_special = GetDlgItemInt(hDlg, IDC_SPECIAL, &bTrans, TRUE);
			return (DLGPROC)TRUE;

		    case IDC_ORBITCOL:
			GetDlgItemText(hDlg, IDC_ORBITCOL, s, 30);
			sscanf(s, "%06X", &temp_OrbitColour);
		        return (DLGPROC) TRUE ;

		    case IDC_GUESS:
		    case IDC_BOUNDARY:
		    case IDC_TESSERAL:
		    case IDC_1PASS:
		    case IDC_2PASS:
		    case IDC_3PASS:
		    case IDC_HORBLINDS:
		    case IDC_VERTBLINDS:
		    case IDC_SPIRAL:
		    case IDC_FWDDIFF:
			switch ((int) LOWORD(wParam))
			    {
			    case IDC_GUESS:
				temp = 'G';
				break;
			    case IDC_BOUNDARY:
				temp = 'B';
				break;
			    case IDC_TESSERAL:
				temp = 'T';
				break;
			    case IDC_1PASS:
				temp = '1';
				break;
			    case IDC_2PASS:
				temp = '2';
				break;
			    case IDC_3PASS:
				temp = '3';
				break;
			    case IDC_HORBLINDS:
				temp = 'H';
				break;
			    case IDC_VERTBLINDS:
				temp = 'V';
				break;
			    case IDC_SPIRAL:
				temp = 'S';
				break;
			    case IDC_FWDDIFF:
				temp = 'F';
				break;
			    }
			CheckRadioButton(hDlg, IDC_GUESS, IDC_FWDDIFF, (int) LOWORD(wParam));
		        return (DLGPROC) TRUE ;

		    case IDC_BAIL_MOD:
		    case IDC_BAIL_REAL:
		    case IDC_BAIL_IMAG:
		    case IDC_BAIL_OR:
		    case IDC_BAIL_AND:
		    case IDC_MANH:
		    case IDC_MANR:
			switch ((int)LOWORD(wParam))
			    {
			    case IDC_BAIL_MOD:
				TempBailoutTest = BAIL_MOD;
				break;
			    case IDC_BAIL_REAL:
				TempBailoutTest = BAIL_REAL;
				break;
			    case IDC_BAIL_IMAG:
				TempBailoutTest = BAIL_IMAG;
				break;
			    case IDC_BAIL_OR:
				TempBailoutTest = BAIL_OR;
				break;
			    case IDC_BAIL_AND:
				TempBailoutTest = BAIL_AND;
				break;
			    case IDC_MANH:
				TempBailoutTest = MANH;
				break;
			    case IDC_MANR:
				TempBailoutTest = MANR;
				break;
			    }

			CheckRadioButton(hDlg, IDC_BAIL_MOD, IDC_MANR, (int)LOWORD(wParam));
			return (DLGPROC)TRUE;

		    case IDC_NO_INSIDE:
		    case IDC_BOF60:
		    case IDC_BOF61:
		    case IDC_ZMAG:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_NO_INSIDE:
				tempInsideMethod = NONE;
				break;
			    case IDC_ZMAG:
				tempInsideMethod = ZMAG;
				break;
			    case IDC_BOF60:
				tempInsideMethod = BOF60;
				break;
			    case IDC_BOF61:
				tempInsideMethod = BOF61;
				break;
			    }
			CheckRadioButton(hDlg, IDC_NO_INSIDE, IDC_BOF61, (int) LOWORD(wParam));
		        return (DLGPROC) TRUE ;

		    case IDC_NO_OUTSIDE:
		    case IDC_REAL1:
		    case IDC_IMAG1:
		    case IDC_MULT:
		    case IDC_ATAN:
		    case IDC_SUM:
		    case IDC_PERT1:
		    case IDC_PERT2:
		    case IDC_EPSCR:
		    case IDC_POTENTIAL:
		    case IDC_STARTRAILS:
		    case IDC_TIERAZONFILTERS:
		    case IDC_TIERAZONCOLOURS:
			switch ((int)LOWORD(wParam))
			    {
			    case IDC_NO_OUTSIDE:
				tempOutsideMethod = NONE;
				break;
			    case IDC_REAL1:
				tempOutsideMethod = REAL;
				break;
			    case IDC_IMAG1:
				tempOutsideMethod = IMAG;
				break;
			    case IDC_PERT1:
				tempOutsideMethod = PERT1;
				break;
			    case IDC_PERT2:
				tempOutsideMethod = PERT2;
				break;
			    case IDC_MULT:
				tempOutsideMethod = MULT;
				break;
			    case IDC_SUM:
				tempOutsideMethod = SUM;
				break;
			    case IDC_ATAN:
				tempOutsideMethod = ATAN;
				break;
			    case IDC_EPSCR:
				tempOutsideMethod = EPSCROSS;
				break;
			    case IDC_POTENTIAL:
				tempOutsideMethod = POTENTIAL;
				break;
			    case IDC_STARTRAILS:
				tempOutsideMethod = STARTRAIL;
				break;
			    case IDC_TIERAZONFILTERS:
				tempOutsideMethod = TIERAZONFILTERS;
				break;
			    case IDC_TIERAZONCOLOURS:
				tempOutsideMethod = TIERAZONCOLOURS;
				break;
			    }
			CheckRadioButton(hDlg, IDC_NO_OUTSIDE, IDC_TIERAZONCOLOURS, (int)LOWORD(wParam));
			return (DLGPROC)TRUE;

		    case IDC_NORMAL:
		    case IDC_90DEGREES:
		    case IDC_180DEGREES:
		    case IDC_270DEGREES:
		    case IDC_OTHER_DEGREES:
			switch ((int) LOWORD(wParam))
			    {
			    case IDC_NORMAL:
				temp_RotationAngle = NORMAL;
				break;
			    case IDC_90DEGREES:
				temp_RotationAngle = 90;
				break;
			    case IDC_180DEGREES:
				temp_RotationAngle = 180;
				break;
			    case IDC_270DEGREES:
				temp_RotationAngle = 270;
				break;
			    default:
				temp_RotationAngle = GetDlgItemInt(hDlg, IDM_ROTDEG, &bTrans, TRUE);
			    }
			SetDlgItemInt(hDlg, IDM_ROTDEG, temp_RotationAngle, TRUE);
			CheckRadioButton(hDlg, IDC_NORMAL, IDC_OTHER_DEGREES, (int) LOWORD(wParam));
			return (DLGPROC)TRUE;
		    case IDM_ROTDEG:
			temp_RotationAngle = GetDlgItemInt(hDlg, IDM_ROTDEG, &bTrans, TRUE);
			CheckRadioButton(hDlg, IDC_NORMAL, IDC_OTHER_DEGREES, IDC_OTHER_DEGREES);
			return (DLGPROC) TRUE ;

		    case IDOK:
			if (logval == 0 && temp_logval != 0)
			     init_log(hDlg);			// don't forget to initialise loglist
			logval = temp_logval; 
			logflag = (logval != 0);
			biomorph = temp_biomorph; 
			decomp = temp_decomp; 
			blockindex = temp_blockindex;
			fillcolor = temp_fillcolor;
			temp_threshold = GetDlgItemInt(hDlg, IDM_THRESHOLD, &bTrans, TRUE);
//			if (temp_threshold >= MAXTHRESHOLD || temp_threshold < 1)
//			    temp_threshold = MAXTHRESHOLD - 1;
			if (temp_threshold < 1 || temp_threshold > MAXTHRESHOLD)
			    temp_threshold = MAXTHRESHOLD;
			threshold = (int)temp_threshold;
			Offset3D = temp_3Dthreshold;
//			if (Offset3D >= MAXTHRESHOLD)
//			    Offset3D = MAXTHRESHOLD;
//			inside_colour = temp_inside/* & 0x00ff*/;	// 0 - 255
//			calcmode = temp;
			if (temp_special < 0)
			    special = 0;
//			else if (temp_special > 255)
//			    special = 255;
			else
			    special = temp_special;
			BailoutTestType = TempBailoutTest;
			InsideMethod = tempInsideMethod;
			OutsideMethod = tempOutsideMethod;
			OrbitColour.rgbtBlue = temp_OrbitColour & 0xff;
			OrbitColour.rgbtGreen = (temp_OrbitColour >> 8) & 0xff;
			OrbitColour.rgbtRed = (temp_OrbitColour >> 16) & 0xff;
			if (InsideMethod == POTENTIAL)
			    {
			    potparam[0] = GetDlgItemInt(hDlg, IDM_MAXCOL, &bTrans, TRUE);
			    potparam[1] = GetDlgItemInt(hDlg, IDM_SLOPE, &bTrans, TRUE);
			    potparam[2] = GetDlgItemInt(hDlg, IDM_BAILOUT, &bTrans, TRUE);
			    }
			if (InsideMethod >= TIERAZONFILTERS)
			    {
			    GetDlgItemText(hDlg, IDM_STALKS, s, 30);
			    calcmode = '2';
			    sscanf(s, "%lf", &dStrands);
			    }
			else
			    {
			    RGBFilter = FALSE;			// don't use the plotting routine for RGB filters
			    PlotType = NOSYM;
			    calcmode = temp;
			    }
			GetDlgItemText(hDlg, IDM_RADIUS, s, 30);
			sscanf(s, "%lf", &f_radius);
			GetDlgItemText(hDlg, IDM_CENTREX, s, 30);
			sscanf(s, "%lf", &f_xcenter);
			GetDlgItemText(hDlg, IDM_CENTREY, s, 30);
			sscanf(s, "%lf", &f_ycenter);
			GetDlgItemText(hDlg, IDC_BKGROUNDCOLOUR, s, 10);
			sscanf(s, "%X", &BackgroundColour);
			cycleflag = FALSE;
			RotationAngle = temp_RotationAngle % 360;
			if (temp_RotationAngle < 0)
			    RotationAngle = -(-temp_RotationAngle % 360);
			else
			    RotationAngle = temp_RotationAngle % 360;
			if (RotationAngle != NORMAL && RotationAngle != 90 && RotationAngle != 180 && RotationAngle != 270)
			    {
			    z_rot = (double)RotationAngle;
			    RotationCentre.x = hor + (mandel_width * ScreenRatio) / 2;
			    RotationCentre.y = vert + mandel_width / 2;
			    if (OrigRotationAngle != RotationAngle)
				Mat.InitTransformation(RotationCentre.x, RotationCentre.y, 0.0, 0.0, 0.0, z_rot);
			    }
			OrigRotationAngle = RotationAngle;
			distest = GetDlgItemInt(hDlg, IDC_DISTEST, &bTrans, TRUE);
			distestwidth = GetDlgItemInt(hDlg, IDC_DISTESTWIDTH, &bTrans, TRUE);

			GetDlgItemText(hDlg, IDM_TRANSFACTOR, s, 30);
			sscanf(s, "%lf", &bump_transfer_factor);
			PaletteStart = GetDlgItemInt(hDlg, IDM_STARTPAL, &bTrans, TRUE);
			GetDlgItemText(hDlg, IDM_LIGHTDIR, s, 30);
			sscanf(s, "%lf", &lightDirectionDegrees);
			GetDlgItemText(hDlg, IDM_MAPDEPTH, s, 30);
			sscanf(s, "%lf", &bumpMappingDepth);
			GetDlgItemText(hDlg, IDM_MAPSTRENGTH, s, 30);
			sscanf(s, "%lf", &bumpMappingStrength);

			EndDialog (hDlg, TRUE);
			return (DLGPROC) TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }
#endif // TESTFWDDIFF

/**************************************************************************
	Dialog Control for Fractal Type
**************************************************************************/

DLGPROC FAR PASCAL FractTypeDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
//     static	HANDLE	hCursor;
     static     UINT	temp;
     static     UINT	tempParam;
//     static     UINT	TempSubtype;
     
     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = type;
//		TempSubtype = subtype;
	        switch (type)
		    {
		    case MANDELFP:
			tempParam = IDC_MANDEL;
			break;
		    case NEWTON:
			tempParam = IDC_NEWTON;
			break;
		    case HENON:
			tempParam = IDC_HENON;
			break;
		    case CUBIC:
			tempParam = IDC_CUBIC;
			break;
		    case POWER:
			tempParam = IDC_POWER;
			break;
		    case CIRCLESQ:
			tempParam = IDC_CIRCLE;
			break;
		    case SPECIALNEWT:
			tempParam = IDC_NEWTSPECIAL;
			break;
		    case RATIONALMAP:
			tempParam = IDC_RATMAP;
			break;
		    case SINFRACTAL:
			tempParam = IDC_SIN;
			break;
		    case EXPFRACTAL:
			tempParam = IDC_EXP;
			break;
		    case MATEIN:
			tempParam = IDC_MATEIN;
			break;
		    case BIFURCATION:
			tempParam = IDC_VERHULST;
			break;
		    case BIFLAMBDA:
			tempParam = IDC_LAMBDA;
			break;
		    case BIFADSINPI:
			tempParam = IDC_ADDSINPI;
			break;
		    case BIFEQSINPI:
			tempParam = IDC_SETSINPI;
			break;
		    case BIFMAY:
			tempParam = IDC_MAY;
			break;
		    case BIFSTEWART:
			tempParam = IDC_STEWART;
			break;
		    case QUADMAND:
			tempParam = IDC_QUADMAND;
			break;
		    case FIBONACCI:
			tempParam = IDC_FIB;
			break;
		    case BARNSLEYM1:
			tempParam = IDC_BARNSLEYM1;
			break;
		    case BARNSLEYM2:
			tempParam = IDC_BARNSLEYM2;
			break;
		    case BARNSLEYM3:
			tempParam = IDC_BARNSLEYM3;
			break;
		    case CELLULAR:
			tempParam = IDC_CELLULAR;
			break;
		    case FROTH:
			tempParam = IDC_FROTH;
			break;
		    case FPGINGERBREAD:
			tempParam = IDC_GINGER;
			break;
		    case FPHOPALONG:
			tempParam = IDC_HOPALONG;
			break;
		    case IFS:
			tempParam = IDC_IFS;
			break;
		    case KAMFP:
			tempParam = IDC_KAM;
			break;
		    case KAM3DFP:
			tempParam = IDC_KAM3D;
			break;
		    case MANOWARFP:
			tempParam = IDC_MANOWAR;
			break;
		    case FPLORENZ:
			tempParam = IDC_LORENZ;
			break;
		    case FPLORENZ3D:
			tempParam = IDC_LORENZ3D1;
			break;
		    case FPLORENZ3D1:
			tempParam = IDC_LORENZ3D2;
			break;
		    case FPLORENZ3D3:
			tempParam = IDC_LORENZ3D3;
			break;
		    case FPLORENZ3D4:
			tempParam = IDC_LORENZ3D4;
			break;
		    case LSYSTEM:
			tempParam = IDC_LSYSTEM;
			break;
		    case FORMULA:
			tempParam = IDC_FORMULA;
			break;
		    case FRACTPAR:
			tempParam = IDC_FRACTPAR;
			break;
		    case LYAPUNOV:
			tempParam = IDC_LYAPUNOV;
			break;
		    case MANDELCLOUD:
			tempParam = IDC_MANDELCLOUD;
			break;
		    case MANDELLAMBDAFP:
			tempParam = IDC_MANDELAMBDA;
			break;
		    case FPMARTIN:
			tempParam = IDC_MARTIN;
			break;
		    case FPPICKOVER:
			tempParam = IDC_PICKOVER;
			break;
		    case PLASMA:
			tempParam = IDC_PLASMA;
			break;
		    case FPROSSLER:
			tempParam = IDC_ROSSLER;
			break;
		    case SIERPINSKI:
			tempParam = IDC_SIERPINSKI;
			break;
		    case SPIDERFP:
			tempParam = IDC_SPIDERFP;
			break;
		    case TEST:
			tempParam = IDC_TEST;
			break;
		    case UNITYFP:
			tempParam = IDC_UNITY;
			break;
		    case DEMOWALK:
			tempParam = IDC_WALK;
			break;
		    case NEWTONPOLYGON:
			tempParam = IDC_GEO_NEWTON;
			break;
		    case COMPLEXNEWTON:
//			tempParam = IDC_CMPLXNEWTON;
//			TempSubtype = 'N';			// B=basin, N=normal
//			break;
		    case COMPLEXBASIN:
			tempParam = IDC_CMPLXNEWTON;
//			TempSubtype = 'B';			// B=basin, N=normal
			break;
		    case FOURIER:
			tempParam = IDC_FOURIER;
			break;
		    case SQRTRIGFP:
			tempParam = IDC_SQRFN;
			break;
		    case SQR1OVERTRIGFP:
			tempParam = IDC_SQR1OVERFN;
			break;
		    case TRIGPLUSTRIGFP:
			tempParam = IDC_FNPLUSFN;
			break;
		    case TRIGXTRIGFP:
			tempParam = IDC_FNXFN;
			break;
		    case TRIGSQRFP:
			tempParam = IDC_FNZXZ;
			break;
		    case MANDELTRIGFP:
			tempParam = IDC_MANDELFN;
			break;
		    case DIFFUSION:
			tempParam = IDC_DIFFUSION;
			break;
		    case TETRATEFP:
			tempParam = IDC_TETRATE;
			break;
		    case MANDPHOENIXFP:
			tempParam = IDC_MANDELPHOENIX;
			break;
		    case PHOENIXFP:
			tempParam = IDC_PHOENIX;
			break;
		    case MANDPHOENIXFPCPLX:
			tempParam = IDC_MANDPHOENIXCMPLX;
			break;
		    case ZXTRIGPLUSZFP:
			tempParam = IDC_FNZPLUSZ;
			break;
		    case FNPLUSFNPIXFP:
			tempParam = IDC_FNZPLUSFNPIX;
			break;
		    case FPMANTRIGPLUSEXP:
			tempParam = IDC_MANFNPLUSEXP;
			break;
		    case FPMANTRIGPLUSZSQRD:
			tempParam = IDC_MANFNPLUSZSQRD;
			break;
		    case FPMANFNFN:
			tempParam = IDC_MANDELFNORFN;
			break;
		    case FPMANLAMFNFN:
			tempParam = IDC_MANLAMFNORFN;
			break;
		    case ESCHER:
			tempParam = IDC_ESCHER;
			break;
		    case VL:
			tempParam = IDC_VL;
			break;
		    case HYPERCMPLXFP:
			tempParam = IDC_HYPERCOMPLEX;
			break;
		    case NEWTONAPPLE:
			tempParam = IDC_NEWTONAPPLE;
			break;
		    case NEWTONFLOWER:
			tempParam = IDC_NEWTONFLOWER;
			break;
//		    case MAGNET1:
//			tempParam = IDC_MAGNET1;
//			break;
//		    case MAGNET2:
//			tempParam = IDC_MAGNET2;
//			break;
		    case TEDDY:
			tempParam = IDC_TEDDY;
			break;
		    case NEWTONMSET:
			tempParam = IDC_NEWTONMSET;
			break;
		    case NEWTONJULIANOVA:
			tempParam = IDC_NEWTONJULIANOVA;
			break;
		    case TALIS:
			tempParam = IDC_TALIS;
			break;
		    case NEWTONCROSS:
			tempParam = IDC_NEWTONCROSS;
			break;
		    case QUARTET1:
			tempParam = IDC_QUARTET1;
			break;
		    case QUARTET2:
		 	tempParam = IDC_QUARTET2;
			break;
		    case QUARTET3:
			tempParam = IDC_QUARTET3;
			break;
		    case RAMONSIN:
			tempParam = IDC_RAMONSIN;
			break;
		    case RAMONCOS:
			tempParam = IDC_RAMONCOS;
			break;
		    case FORMULA05:
			tempParam = IDC_FORMULA05;
			break;
//		    case SHARON:
//			tempParam = IDC_SHARON;
//			break;
		    case QUAD:
			tempParam = IDC_QUAD;
			break;
		    case NEWTONVARIATION:
			tempParam = IDC_NEWTONVARIATION;
			break;
		    case MOUNTAIN:
			tempParam = IDC_MOUNTAIN;
			break;
		    case FFT:
			tempParam = IDC_FFT;
			break;
		    case TIERAZON:
			tempParam = IDC_TIERAZON;
			break;
//		    case MARCUSMANDEL:
//			tempParam = IDC_MARCUSMANDEL;
//			break;
		    case QUATFP:
			tempParam = IDC_QUAT;
			break;
		    case FPMANZTOZPLUSZPWR:
			tempParam = IDC_MANZZPWR;
			break;
		    case FPMANDELZPOWER:
			tempParam = IDC_MANZPOWER;
			break;
		    case COMPLEXMARKSMAND:
			tempParam = IDC_CMPLXMARKSMAND;
			break;
		    case MARKSMANDELPWRFP:
			tempParam = IDC_MARKSMANDELPWR;
			break;
		    case MARKSMANDELFP:
			tempParam = IDC_MARKSMANDEL;
			break;
		    case SCREENFORMULA:
			tempParam = IDC_DIRECTFORMULA;
			break;
		    case NOVA:
			tempParam = IDC_NOVA;
			break;
		    case MALTHUS:
			tempParam = IDC_MALTHUS;
			break;
		    case TRIANGLES:
			tempParam = IDC_TRIANGLES;
			break;
		    case GEOMETRY:
			tempParam = IDC_GEOMETRICSHAPES;
			break;
		    case CIRCLES:
			tempParam = IDC_CIRCLES;
			break;
//		    case IKEDA:
//			tempParam = IDC_IKEDA;
//			break;
		    case CROSSROADS:
			tempParam = IDC_CROSSROADS;
			break;
		    case PASCALTRIANGLE:
			tempParam = IDC_PASCAL;
			break;
		    case APOLLONIUS:
			tempParam = IDC_APOLLONIUS;
			break;
		    case APOLLONIUSIFS:
			tempParam = IDC_APOLLONIUSIFS;
			break;
		    case SIERPINSKIFLOWERS:
			tempParam = IDC_SIERPINSKIFLOWERS;
			break;
		    case ZIGZAG:
			tempParam = IDC_ZIGZAG;
			break;
		    case GARGOYLE:
			tempParam = IDC_GARGOYLE;
			break;
		    case CURLICUES:
			tempParam = IDC_CURLICUES;
			break;
		    case CHUA:
			tempParam = IDC_CHUA;
			break;
		    case BURNINGSHIP:
			tempParam = IDC_BURNINGSHIP;
			break;
		    case BURNINGSHIPPOWER:
			tempParam = IDC_BURNINGSHIPPOWER;
			break;
		    case THORN:
			tempParam = IDC_THORN;
			break;
		    case POPCORN:
			tempParam = IDC_POPCORN;
			break;
		    case REDSHIFTRIDER:
			tempParam = IDC_REDSHIFTRIDER;
			break;
		    case MANDELDERIVATIVES:
			tempParam = IDC_MANDELDERIVATIVES;
			break;
//		    case MANDALA:
//			tempParam = IDC_MANDALA;
//			break;
//		    case PERPBUFFALO:
//			tempParam = IDC_PERPBUFFALO;
//			break;
//		    case MARCUSIFS:
//			tempParam = IDC_MARCUSIFS;
//			break;
//		    case CELTIC:
//			tempParam = IDC_CELTIC;
//			break;
//		    case MANDELBARCELTIC:
//			tempParam = IDC_MANDELBARCELTIC;
//			break;
//		    case PERPCELTIC:
//			tempParam = IDC_PERPCELTIC;
//			break;
		    case BUDDHABROT:
			tempParam = IDC_BUDDHABROT;
			break;
//		    case PERPBURNINGSHIP:
//			tempParam = IDC_PERPBURNINGSHIP;
//			break;
//		    case CUBICFLYINGSQUIRREL:
//			tempParam = IDC_CUBICFLYINGSQUIRREL;
//			break;
		    case OSCILLATORS:
			tempParam = IDC_OSCILLATORS;
			break;
		    case FRACTALMAPS:
			tempParam = IDC_FRACTALMAPS;
			break;
		    case SPROTTMAPS:
			tempParam = IDC_SPROTTMAPS;
			break;
		    case SURFACES:
			tempParam = IDC_SURFACES;
			break;
		    case KNOTS:
			tempParam = IDC_KNOTS;
			break;
		    case CURVES:
			tempParam = IDC_CURVES;
			break;
		    case TETRATION:
			tempParam = IDC_TETRATION;
			break;
		    case PERTURBATION:
			tempParam = IDC_PERTURBATION;
			break;
		    case KLEINIAN:
			tempParam = IDC_KLEINIAN;
			break;
		    case SLOPEDERIVATIVE:
			tempParam = IDC_SLOPEDERIVATIVE;
			break;
		    case SLOPEFORWARDDIFF:
			tempParam = IDC_SLOPEFWDDIFF;
			break;
		    case POLYNOMIAL:
			tempParam = IDC_POLYNOMIAL;
			break;
		    case ANT:
			tempParam = IDC_ANT;
			break;
		    case TOWER:
			tempParam = IDC_TOWER;
			break;
		    case LATOO:
			tempParam = IDC_LATOO;
			break;
		    case CHIP:
			tempParam = IDC_CHIP;
			break;
		    case QUADRUPTWO:
			tempParam = IDC_QUADRUPTWO;
			break;
		    case THREEPLY:
			tempParam = IDC_THREEPLY;
			break;
		    case ICON:
			tempParam = IDC_ICONS;
			break;
		    case DYNAMICFP:
			tempParam = IDC_DYNAMIC;
			break;
		    case FPPOPCORN:
			tempParam = IDC_FRACTINTPOPCORN;
			break;

		    }
		CheckRadioButton(hDlg, IDC_MANDEL, IDC_TEST, tempParam);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return FALSE ;
//	        return (DLGPROC) TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_MANDEL:
			temp = MANDELFP;
			break;
		    case IDC_NEWTON:
			temp = NEWTON;
			break;
		    case IDC_HENON:
			temp = HENON;
			break;
		    case IDC_CUBIC:
			temp = CUBIC;
			break;
		    case IDC_POWER:
			temp = POWER;
			break;
		    case IDC_CIRCLE:
			temp = CIRCLESQ;
			break;
		    case IDC_NEWTSPECIAL:
			temp = SPECIALNEWT;
			break;
		    case IDC_RATMAP:
			temp = RATIONALMAP;
			break;
		    case IDC_SIN:
			temp = SINFRACTAL;
			break;
		    case IDC_EXP:
			temp = EXPFRACTAL;
			break;
		    case IDC_MATEIN:
			temp = MATEIN;
			break;
		    case IDC_VERHULST:
			temp = BIFURCATION;
			break;
		    case IDC_LAMBDA:
			temp = BIFLAMBDA;
			break;
		    case IDC_ADDSINPI:
			temp = BIFADSINPI;
			break;
		    case IDC_SETSINPI:
			temp = BIFEQSINPI;
			break;
		    case IDC_MAY:
			temp = BIFMAY;
			break;
		    case IDC_STEWART:
			temp = BIFSTEWART;
			break;
		    case IDC_QUADMAND:
			temp = QUADMAND;
			break;
		    case IDC_FIB:
			temp = FIBONACCI;
			break;
		    case IDC_BARNSLEYM1:
			temp = BARNSLEYM1;
			break;
		    case IDC_BARNSLEYM2:
			temp = BARNSLEYM2;
			break;
		    case IDC_BARNSLEYM3:
			temp = BARNSLEYM3;
			break;
		    case IDC_CELLULAR:
			temp = CELLULAR;
			break;
		    case IDC_FROTH:
			temp = FROTH;
			break;
		    case IDC_GINGER:
			temp = FPGINGERBREAD;
			break;
		    case IDC_HOPALONG:
			temp = FPHOPALONG;
			break;
		    case IDC_IFS:
			temp = IFS;
			break;
		    case IDC_KAM:
			temp = KAMFP;
			break;
		    case IDC_KAM3D:
			temp = KAM3DFP;
			break;
		    case IDC_MANOWAR:
			temp = MANOWARFP;
			break;
		    case IDC_LORENZ:
			temp = FPLORENZ;
			break;
		    case IDC_LORENZ3D1:
			temp = FPLORENZ3D;
			break;
		    case IDC_LORENZ3D2:
			temp = FPLORENZ3D1;
			break;
		    case IDC_LORENZ3D3:
			temp = FPLORENZ3D3;
			break;
		    case IDC_LORENZ3D4:
			temp = FPLORENZ3D4;
			break;
		    case IDC_LSYSTEM:
			temp = LSYSTEM;
			break;
		    case IDC_FORMULA:
			temp = FORMULA;
			break;
		    case IDC_FRACTPAR:
			temp = FRACTPAR;
			break;
		    case IDC_LYAPUNOV:
			temp = LYAPUNOV;
			break;
		    case IDC_MANDELCLOUD:
			temp = MANDELCLOUD;
			break;
		    case IDC_MANDELAMBDA:
			temp = MANDELLAMBDAFP;
			break;
		    case IDC_MARTIN:
			temp = FPMARTIN;
			break;
		    case IDC_PICKOVER:
			temp = FPPICKOVER;
			break;
		    case IDC_PLASMA:
			temp = PLASMA;
			break;
		    case IDC_ROSSLER:
			temp = FPROSSLER;
			break;
		    case IDC_SIERPINSKI:
			temp = SIERPINSKI;
			break;
		    case IDC_SPIDERFP:
			temp = SPIDERFP;
			break;
		    case IDC_TEST:
			temp = TEST;
			break;
		    case IDC_UNITY:
			temp = UNITYFP;
			break;
		    case IDC_WALK:
			temp = DEMOWALK;
			break;
		    case IDC_GEO_NEWTON:
			temp = NEWTONPOLYGON;
			break;
		    case IDC_CMPLXNEWTON:
			temp = COMPLEXNEWTON;
			break;
		    case IDC_FOURIER:
			temp = FOURIER;
			break;
		    case IDC_SQRFN:
			temp = SQRTRIGFP;
			break;
		    case IDC_SQR1OVERFN:
			temp = SQR1OVERTRIGFP;
			break;
		    case IDC_FNPLUSFN:
			temp = TRIGPLUSTRIGFP;
			break;
		    case IDC_FNXFN:
			temp = TRIGXTRIGFP;
			break;
		    case IDC_FNZXZ:
			temp = TRIGSQRFP;
			break;
		    case IDC_MANDELFN:
			temp = MANDELTRIGFP;
			break;
		    case IDC_TETRATE:
			temp = TETRATEFP;
			break;
		    case IDC_DIFFUSION:
			temp = DIFFUSION;
			break;
		    case IDC_MANDELPHOENIX:
			temp = MANDPHOENIXFP;
			break;
		    case IDC_PHOENIX:
			temp = PHOENIXFP;
			break;
		    case IDC_MANDPHOENIXCMPLX:
			temp = MANDPHOENIXFPCPLX;
			break;
		    case IDC_FNZPLUSZ:
			temp = ZXTRIGPLUSZFP;
			break;
		    case IDC_FNZPLUSFNPIX:
			temp = FNPLUSFNPIXFP;
			break;
		    case IDC_MANFNPLUSEXP:
			temp = FPMANTRIGPLUSEXP;
			break;
		    case IDC_MANFNPLUSZSQRD:
			temp = FPMANTRIGPLUSZSQRD;
			break;
		    case IDC_MANDELFNORFN:
			temp = FPMANFNFN;
			break;
		    case IDC_MANLAMFNORFN:
			temp = FPMANLAMFNFN;
			break;
		    case IDC_ESCHER:
			temp = ESCHER;
			break;
		    case IDC_VL:
			temp = VL;
			break;
		    case IDC_HYPERCOMPLEX:
			temp = HYPERCMPLXFP;
			break;
		    case IDC_NEWTONAPPLE:
			temp = NEWTONAPPLE;
			break;
		    case IDC_NEWTONFLOWER:
			temp = NEWTONFLOWER;
			break;
//		    case IDC_MAGNET1:
//			temp = MAGNET1;
//			break;
//		    case IDC_MAGNET2:
//			temp = MAGNET2;
//			break;
		    case IDC_TEDDY:
			temp = TEDDY;
			break;
		    case IDC_NEWTONMSET:
			temp = NEWTONMSET;
			break;
		    case IDC_NEWTONJULIANOVA:
			temp = NEWTONJULIANOVA;
			break;
		    case IDC_TALIS:
			temp = TALIS;
			break;
		    case IDC_NEWTONCROSS:
			temp = NEWTONCROSS;
			break;
		    case IDC_QUARTET1:
			temp = QUARTET1;
			break;
		    case IDC_QUARTET2:
			temp = QUARTET2;
			break;
		    case IDC_QUARTET3:
			temp = QUARTET3;
			break;
		    case IDC_RAMONSIN:
			temp = RAMONSIN;
			break;
		    case IDC_RAMONCOS:
			temp = RAMONCOS;
			break;
		    case IDC_FORMULA05:
			temp = FORMULA05;
			break;
//		    case IDC_SHARON:
//			temp = SHARON;
//			break;
		    case IDC_QUAD:
			temp = QUAD;
			break;
		    case IDC_NEWTONVARIATION:
			temp = NEWTONVARIATION;
			break;
		    case IDC_MOUNTAIN:
			temp = MOUNTAIN;
			break;
		    case IDC_FFT:
			temp = FFT;
			break;
		    case IDC_TIERAZON:
			temp = TIERAZON;
			break;
//		    case IDC_MARCUSMANDEL:
//			temp = MARCUSMANDEL;
//			break;
		    case IDC_QUAT:
			temp = QUATFP;
			break;
		    case IDC_MANZZPWR:
			temp = FPMANZTOZPLUSZPWR;
			break;
		    case IDC_MANZPOWER:
			temp = FPMANDELZPOWER;
			break;
		    case IDC_CMPLXMARKSMAND:
			temp = COMPLEXMARKSMAND;
			break;
		    case IDC_MARKSMANDELPWR:
			temp = MARKSMANDELPWRFP;
			break;
		    case IDC_MARKSMANDEL:
			temp = MARKSMANDELFP;
			break;
		    case IDC_DIRECTFORMULA:
			temp = SCREENFORMULA;
			break;
		    case IDC_NOVA:
			temp = NOVA;
			break;
		    case IDC_MALTHUS:
			temp = MALTHUS;
			break;
		    case IDC_TRIANGLES:
			temp = TRIANGLES;
			break;
		    case IDC_GEOMETRICSHAPES:
			temp = GEOMETRY;
			break;
		    case IDC_CIRCLES:
			temp = CIRCLES;
			break;
//		    case IDC_IKEDA:
//			temp = IKEDA;
//			break;
		    case IDC_CROSSROADS:
			temp = CROSSROADS;
			break;
		    case IDC_PASCAL:
			temp = PASCALTRIANGLE;
			break;
		    case IDC_APOLLONIUS:
			temp = APOLLONIUS;
			break;
		    case IDC_APOLLONIUSIFS:
			temp = APOLLONIUSIFS;
			break;
		    case IDC_SIERPINSKIFLOWERS:
			temp = SIERPINSKIFLOWERS;
			break;
		    case IDC_ZIGZAG:
			temp = ZIGZAG;
			break;
		    case IDC_GARGOYLE:
			temp = GARGOYLE;
			break;
		    case IDC_CURLICUES:
			temp = CURLICUES;
			break;
		    case IDC_CHUA:
			temp = CHUA;
			break;
		    case IDC_BURNINGSHIP:
			temp = BURNINGSHIP;
			break;
		    case IDC_BURNINGSHIPPOWER:
			temp = BURNINGSHIPPOWER;
			break;
		    case IDC_THORN:
			temp = THORN;
			break;
		    case IDC_POPCORN:
			temp = POPCORN;
			break;
		    case IDC_REDSHIFTRIDER:
			temp = REDSHIFTRIDER;
			break;
//		    case IDC_MANDALA:
//			temp = MANDALA;
//			break;
//		    case IDC_PERPBURNINGSHIP:
//			temp = PERPBURNINGSHIP;
//			break;
		    case IDC_MANDELDERIVATIVES:
			temp = MANDELDERIVATIVES;
			break;
//		    case IDC_PERPBUFFALO:
//			temp = PERPBUFFALO;
//			break;
//		    case IDC_MARCUSIFS:
//			temp = MARCUSIFS;
//			break;
//		    case IDC_CELTIC:
//			temp = CELTIC;
//			break;
//		    case IDC_MANDELBARCELTIC:
//			temp = MANDELBARCELTIC;
//			break;
//		    case IDC_PERPCELTIC:
//			temp = PERPCELTIC;
//			break;
//		    case IDC_CUBICFLYINGSQUIRREL:
//			temp = CUBICFLYINGSQUIRREL;
//			break;
		    case IDC_BUDDHABROT:
			temp = BUDDHABROT;
			break;
		    case IDC_OSCILLATORS:
			temp = OSCILLATORS;
			break;
		    case IDC_FRACTALMAPS:
			temp = FRACTALMAPS;
			break;
		    case IDC_SPROTTMAPS:
			temp = SPROTTMAPS;
			break;
		    case IDC_SURFACES:
			temp = SURFACES;
			break;
		    case IDC_KNOTS:
			temp = KNOTS;
			break;
		    case IDC_CURVES:
			temp = CURVES;
			break;
		    case IDC_TETRATION:
			temp = TETRATION;
			break;
		    case IDC_PERTURBATION:
			temp = PERTURBATION;
			break;
		    case IDC_KLEINIAN:
			temp = KLEINIAN;
			break;
		    case IDC_SLOPEDERIVATIVE:
			temp = SLOPEDERIVATIVE;
			break;
		    case IDC_SLOPEFWDDIFF:
			temp = SLOPEFORWARDDIFF;
			break;
		    case IDC_POLYNOMIAL:
			temp = POLYNOMIAL;
			break;
		    case IDC_ANT:
			temp = ANT;
			break;
		    case IDC_TOWER:
			temp = TOWER;
			break;
		    case IDC_LATOO:
			temp = LATOO;
			break;
		    case IDC_CHIP:
			temp = CHIP;
			break;
		    case IDC_QUADRUPTWO:
			temp = QUADRUPTWO;
			break;
		    case IDC_THREEPLY:
			temp = THREEPLY;
			break;
		    case IDC_ICONS:
			temp = ICON;
			break;
		    case IDC_DYNAMIC:
			temp = DYNAMICFP;
			break;
		    case IDC_FRACTINTPOPCORN:
			temp = FPPOPCORN;
			break;
			CheckRadioButton(hDlg, IDC_MANDEL, IDC_TEST, (int) LOWORD(wParam));
		        return (DLGPROC) TRUE ;

		    case IDOK:
			type = temp;
//			subtype = TempSubtype;
			cycleflag = FALSE;
			EndDialog (hDlg, TRUE);
			return (DLGPROC) TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }

/**************************************************************************
	Dialog Control for Walk Type
**************************************************************************/

DLGPROC FAR PASCAL WalkDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
//     static	HANDLE	hCursor;
     char		s[100];
//     BOOL		bTrans ;
     static	UINT	tempPoints; 
     static     char	temp[20];
//     static     char	temp;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        sprintf(s, "%3.2f", param[0]);
		SetDlgItemText(hDlg, IDC_WALK_STEPSIZE, s);
	        return (DLGPROC) TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_WALK_STEPSIZE:
			GetDlgItemText(hDlg, IDC_WALK_STEPSIZE, temp, 20);
		        return (DLGPROC) TRUE ;

		    case IDOK:
			sscanf(temp, "%lf", &param[0]);
			if (param[0] != 999.0)
			    if (param[0] < 0.1 || param[0] > 100.0)
				param[0] = 1.0;
			EndDialog (hDlg, TRUE);
			return (DLGPROC) TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }


/*-----------------------------------------
	About View
  -----------------------------------------*/

//#pragma argsused
DLGPROC FAR PASCAL AboutDlgProc (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     switch (message)
	  {
	  case WM_INITDIALOG:
#ifdef _WIN64
	      SetDlgItemText(hDlg, IDC_PROGRAMNAME, "ManpWIN 64 bit");
#else
	      SetDlgItemText(hDlg, IDC_PROGRAMNAME, "ManpWIN");
#endif // _WIN64
	       return (DLGPROC) TRUE;

	  case WM_COMMAND:
	       switch (wParam)
		    {
		    case IDOK:
			EndDialog (hDlg, TRUE);
			return (DLGPROC) TRUE;
		    }
	       break;
	  }
     return FALSE;
     }

/**************************************************************************
	Dialog Control for Special Newton Fractal Type
**************************************************************************/

void	Centre2Edge(void) 

    {
    double	centrex, centrey;
    // local Big num declarations **********************************************************
    BigDouble	Big_centrex, Big_centrey, Big_Scale;
    // local Big num declarations **********************************************************

    if (BigNumFlag)
	{
	Big_centrex = BigHor;
	Big_centrey = BigVert;
	BigHor = Big_centrex - (BigWidth * (ScreenRatio / 2.0));
	BigVert = Big_centrey - BigWidth / 2.0;
	}
    else
	{
	centrex = hor;
	centrey = vert;
	hor = centrex - (mandel_width * (ScreenRatio / 2.0));
	vert = centrey - (mandel_width / 2.0);
	}
    }

/**************************************************************************
	Get corner and width
**************************************************************************/

extern	void	ConvertString2Bignum(mpfr_t, char *);
extern	int	getprecbf_mag(void);
extern	int	ChangeBigPrecision(int decimals);
extern	int	decimals, precision;

int	GetCorner(char *s1, char *s2, char *s3)

    {
    sscanf(s1, "%lf", &hor);
    sscanf(s2, "%lf", &vert);
    sscanf(s3, "%lf", &mandel_width);

    if (mandel_width < DBL_MIN)						// we can do a BigNum calculation here to allow deeper zooming
	{
	ConvertString2Bignum(BigWidth.x, s3);
	BigNumFlag = TRUE;
	}

    precision = getprecbf_mag();
    if (precision < 0)							// exceeded allowable precision
	return -1;
    if (precision > DBL_DIG - 3)
	{
	decimals = precision + PRECISION_FACTOR;
	if (ChangeBigPrecision(decimals) < 0)				// increase precision of Big numbers	
	    return -1;							// too many decimals for library

	BigNumFlag = TRUE;
	ConvertString2Bignum(BigHor.x, s1);
	ConvertString2Bignum(BigVert.x, s2);
	ConvertString2Bignum(BigWidth.x, s3);
	if (mpfr_sgn(BigWidth.x) == 0)					// no naughty division
	    mpfr_set_d(BigWidth.x, 1.0, MPFR_RNDN);
	}
    else
	{
	BigNumFlag = FALSE;
	if (mandel_width < DBL_MIN)
	    mandel_width = 1.0;
	}
    return 0;
    }

/**************************************************************************
	Dialog Box for loading co-ordinates
**************************************************************************/

extern double   rqlim;

extern	char	*GetFractalName(void);

DLGPROC FAR PASCAL CoordDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
//     static	HANDLE	hCursor;
     static     UINT	tempParam;
     static     BOOL	CentreCoord = FALSE;
//     static     char	*buf;
     static	long	temp_threshold;
     char		*s1;
     char		*s2;
     char		*s3;
     BOOL		bTrans;
     HWND		hCtrl;
     char		Bailout[120];
     char		t[120];
     char		JuliaReal[120];
     char		JuliaImag[120];
     int		i, k;
     char		s[10][100];
//     buf = new char [SIZEOF_BF_VARS * 3];		// leave full size because we have no idea how big they are going to be before hand
     s1 = new char[SIZEOF_BF_VARS];
     s2 = new char[SIZEOF_BF_VARS];
     s3 = new char[SIZEOF_BF_VARS];

     switch (message)
	  {
	  case WM_INITDIALOG:
	      if (Fractal.FractName == NULL)
		  InitFract(type);
	      sprintf(JuliaReal, "%lf", j.x);
	      sprintf(JuliaImag, "%lf", j.y);
	      sprintf(Bailout, "%lf", rqlim);
	      SetDlgItemText(hDlg, IDC_JULIAREAL, JuliaReal);
	      SetDlgItemText(hDlg, IDC_JULIAIMAG, JuliaImag);
	      SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	      SetDlgItemText(hDlg, ID_FRACNAME, GetFractalName());
	      SetDlgItemInt(hDlg, IDC_THRESHOLD, threshold, TRUE);
	      hCtrl = GetDlgItem(hDlg, IDC_CENTRE);
	      SendMessage(hCtrl, BM_SETCHECK, CentreCoord, 0L);
	      hCtrl = GetDlgItem(hDlg, IDC_ISJULIA);
	      SendMessage(hCtrl, BM_SETCHECK, juliaflag, 0L);
	      if (BigNumFlag)
		  {
		  ConvertBignum2String(s1, BigHor.x);
		  ConvertBignum2String(s2, BigVert.x);
		  mpfr_sprintf(s3, "%.20Re", BigWidth.x);
//		  ConvertBignum2String(s3, BigWidth.x);
		  }
	      else
		  {
		  sprintf(s1, "%18.18f", hor);
		  sprintf(s2, "%18.18f", vert);
		  sprintf(s3, "%18.18f", mandel_width);
		  }
	      
	      SetDlgItemText(hDlg, IDC_XCOORD, s1);
	      SetDlgItemText(hDlg, IDC_YCOORD, s2);
	      SetDlgItemText(hDlg, IDC_WIDTH, s3);
	      if (Fractal.NumFunct > 0)
		  {
		  SetDlgItemText(hDlg, ID_FRACPARAM1, Fractal.Fn1);
		  hCtrl = GetDlgItem (hDlg, ID_FRACPARAM1);
 		  EnableWindow (hCtrl, FALSE);
		  SetDlgItemText(hDlg, ID_FRACPARTX1, "First Function");
		  if (Fractal.NumFunct > 1)
		      {
		      SetDlgItemText(hDlg, ID_FRACPARAM2, Fractal.Fn2);
		      hCtrl = GetDlgItem (hDlg, ID_FRACPARAM2);
 		      EnableWindow (hCtrl, FALSE);
		      SetDlgItemText(hDlg, ID_FRACPARTX2, "Second Function");
		      }
		  }
	      for (i = Fractal.NumFunct, k = 0; i < Fractal.NumFunct + Fractal.NumParam && i < 10; i++, k++) 
		  {
		  sprintf(s[k], "%f", *Fractal.ParamValue[k]);
		  SetDlgItemText(hDlg, ID_FRACPARTX1 + i, fractalspecific[type].paramname[k]);
		  SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[k]);
		  hCtrl = GetDlgItem (hDlg, ID_FRACPARAM1 + i);
 		  EnableWindow (hCtrl, TRUE);
		  }
	      for (i = Fractal.NumFunct + Fractal.NumParam; i < 10; i++) 
		  SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");

	      return (DLGPROC) TRUE ;

	  case WM_COMMAND:
	      switch ((int) LOWORD(wParam))
		    {
		    case IDOK:
//			SaveUndo();
			hCtrl = GetDlgItem(hDlg, IDC_CENTRE);
			CentreCoord = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem(hDlg, IDC_ISJULIA);
			juliaflag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			GetDlgItemText(hDlg, IDC_XCOORD, s1, SIZEOF_BF_VARS);
			GetDlgItemText(hDlg, IDC_YCOORD, s2, SIZEOF_BF_VARS);
			GetDlgItemText(hDlg, IDC_WIDTH, s3, SIZEOF_BF_VARS);
//			sprintf(buf, "%s,%s,%s", s1, s2, s3);
//			if (analyse_corner(buf) < 0)
			if (GetCorner(s1, s2, s3) < 0)
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
			    param[0] = 0.0;
			    param[1] = 0.0;
			    sprintf(t, "Deep Zooming Limit (%d decimals) exceeded", SIZEOF_BF_VARS);	
			    MessageBox (hDlg, t, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			    MessageBeep (0);
			    }
			GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
			GetDlgItemText(hDlg, IDC_JULIAREAL, JuliaReal, 100);
			GetDlgItemText(hDlg, IDC_JULIAIMAG, JuliaImag, 100);
			rqlim = atof(Bailout);
			j.x = atof(JuliaReal);
			j.y = atof(JuliaImag);
			temp_threshold = GetDlgItemInt(hDlg, IDC_THRESHOLD, &bTrans, TRUE);
			if (temp_threshold < 1 || temp_threshold > MAXTHRESHOLD)
			    temp_threshold = MAXTHRESHOLD;
			threshold = (int)temp_threshold;
			for (i = Fractal.NumFunct, k = 0; i < Fractal.NumFunct + Fractal.NumParam && i < 10; i++, k++)
			    {
			    GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[k], 100);
			    *Fractal.ParamValue[k] = atof(s[k]);
			    }
			if (CentreCoord)
			    Centre2Edge();
			cycleflag = FALSE;
//			if (buf) { delete[] buf; buf = NULL; }
			if (s1) { delete[] s1; s1 = NULL; }
			if (s2) { delete[] s2; s2 = NULL; }
			if (s3) { delete[] s3; s3 = NULL; }
			EndDialog (hDlg, TRUE);
			return (DLGPROC) TRUE;

		    case IDCANCEL:
//			if (buf) { delete[] buf; buf = NULL; }
			if (s1) { delete[] s1; s1 = NULL; }
			if (s2) { delete[] s2; s2 = NULL; }
			if (s3) { delete[] s3; s3 = NULL; }
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
//      if (buf) { delete[] buf; buf = NULL; }
      if (s1) { delete[] s1; s1 = NULL; }
      if (s2) { delete[] s2; s2 = NULL; }
      if (s3) { delete[] s3; s3 = NULL; }
      return FALSE ;
      }

/**************************************************************************
	Dialog Box for loading parameters
**************************************************************************/

DLGPROC FAR PASCAL ParamDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     char		s1[DBL_MANT_DIG];
     char		s2[DBL_MANT_DIG];
     char		s3[DBL_MANT_DIG];
     char		s4[DBL_MANT_DIG];
     char		s5[DBL_MANT_DIG];

     switch (message)
	  {
	  case WM_INITDIALOG:
		sprintf(s1, "%18.18f", param[0]);
		sprintf(s2, "%18.18f", param[1]);
		sprintf(s3, "%18.18f", param[2]);
		sprintf(s4, "%18.18f", param[3]);
		sprintf(s5, "%18.18f", param[4]);

		SetDlgItemText(hDlg, IDC_PARAM0, s1);
		SetDlgItemText(hDlg, IDC_PARAM1, s2);
		SetDlgItemText(hDlg, IDC_PARAM2, s3);
		SetDlgItemText(hDlg, IDC_PARAM3, s4);
		SetDlgItemText(hDlg, IDC_PARAM4, s5);
	        return (DLGPROC) TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDOK:
//			SaveUndo();
			GetDlgItemText(hDlg, IDC_PARAM0, s1, DBL_MANT_DIG);
			GetDlgItemText(hDlg, IDC_PARAM1, s2, DBL_MANT_DIG);
			GetDlgItemText(hDlg, IDC_PARAM2, s3, DBL_MANT_DIG);
			GetDlgItemText(hDlg, IDC_PARAM3, s4, DBL_MANT_DIG);
			GetDlgItemText(hDlg, IDC_PARAM4, s5, DBL_MANT_DIG);
			param[0] = atof(s1);
			param[1] = atof(s2);
			param[2] = atof(s3);
			param[3] = atof(s4);
			param[4] = atof(s5);

			cycleflag = FALSE;
			EndDialog (hDlg, TRUE);
			return (DLGPROC) TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }


/**************************************************************************
	Dialog Box for loading functions
**************************************************************************/

DLGPROC FAR PASCAL SelectFracParams(HWND hDlg, UINT message, UINT wParam, LONG lParam)

    {
    int i, j;
    static	int	index1 = -1, index2 = -1;
    static	int	numtrig;
    char		s[10][100];
    char		Bailout[120];
    CTrigFn		TrigFn;

    switch (message) 
	{
        case WM_INITDIALOG:
	    sprintf(Bailout, "%14.14lf", rqlim);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	    numtrig = Fractal.TotalFunctions;
            SetDlgItemText(hDlg, ID_FRACNAME, 	GetFractalName());
//            SetDlgItemText(hDlg, ID_FRACNAME, 	fractalspecific[type].name);
	    if (Fractal.NumFunct > 0)
		{
		for (i = 0; i < numtrig; ++i)
		    SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) Fractal.fnptr[i]);
		if (index1 == -1 || *Fractal.Fn1 != '\0')		// index not loaded, but have a default
		    index1 = TrigFn.FindFunct(Fractal.Fn1, numtrig);
		if (Fractal.NumFunct == 2)
		    {
		    for (i = 0; i < numtrig; ++i)
			SendDlgItemMessage(hDlg, IDC_FNLIST2, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) Fractal.fnptr[i]);
		    if (index2 == -1 || *Fractal.Fn2 != '\0')		// index not loaded, but have a default
			index2 = TrigFn.FindFunct(Fractal.Fn2, numtrig);
		    }
		else
		    {
		    SendDlgItemMessage(hDlg, IDC_FNLIST2, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) "N/A");
		    index2 = -1;
		    }
		}
	    else
		{
		SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) "N/A");
		index1 = -1;
		SendDlgItemMessage(hDlg, IDC_FNLIST2, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) "N/A");
		index2 = -1;
		}

            SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_SETCURSEL, (WPARAM)Fractal.fnptr, 0L);
	    if (index1 >= 0 && index1 < numtrig)
		SetDlgItemText(hDlg, ID_FRACPARAM1, Fractal.fnptr[index1]);
	    SetDlgItemText(hDlg, ID_FRACPARTX1, "First Function");

	    if (Fractal.NumFunct == 2)
		{
		if (index2 >= 0 && index2 < numtrig)
		    SetDlgItemText(hDlg, ID_FRACPARAM2, Fractal.fnptr[index2]);
		SetDlgItemText(hDlg, ID_FRACPARTX2, "Second Function");
		}
            for (i = Fractal.NumFunct, j = 0; i < Fractal.NumFunct + Fractal.NumParam && i < 10; i++, j++) 
		{
		sprintf(s[j], "%f", *Fractal.ParamValue[j]);
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, Fractal.ParamName[j]);
		SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[j]);
		}
            for (i = Fractal.NumFunct + Fractal.NumParam; i < 10; i++) 
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
            return ((DLGPROC) TRUE);

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
//	    switch (wParam)
		{
                case IDOK:
//                    index1 = SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_GETCURSEL, 0, 0L);
//                    index2 = SendDlgItemMessage(hDlg, IDC_FNLIST2, LB_GETCURSEL, 0, 0L);
                    if (Fractal.NumFunct >= 1)
			{
			if (index1 == LB_ERR) 
			    {
			    MessageBox(hDlg, "No Choice selected", "Select From a List", MB_OK | MB_ICONEXCLAMATION);
			    break;
			    }
			Fractal.Fn1Index = index1; 
			Fractal.Fn1 = TrigFn.FunctList[index1];
			if (Fractal.NumFunct == 2)
			    {
			    if (index2 == LB_ERR) 
				{
				MessageBox(hDlg, "No Choice selected", "Select From a List", MB_OK | MB_ICONEXCLAMATION);
				break;
				}
			    Fractal.Fn2Index = index2;
			    Fractal.Fn2 = TrigFn.FunctList[index2];
			    }
			}
		    GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
		    rqlim = atof(Bailout);
		    for (i = Fractal.NumFunct, j = 0; i < Fractal.NumFunct + Fractal.NumParam && i < 10; i++, j++) 
			{
			GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[j], 100);
			*Fractal.ParamValue[j] = atof(s[j]);
			}
		    cycleflag = FALSE;
                    EndDialog(hDlg, TRUE);
                    return ((DLGPROC) TRUE);
                  
                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    return (FALSE);
                    
                case IDC_FNLIST1:
                    switch (HIWORD(wParam) & 0x0003) 
//                    switch (LOWORD(lParam)) 
			{
                        case LBN_SELCHANGE:
//                            index1 = SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_GETCURSEL, 0, 0L);
//                            if (index1 == LB_ERR)
//                                break;
//                            break;
                         
			case LBN_DBLCLK:
			    index1 = (int)SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_GETCURSEL, 0, 0L);
			    if (index1 == LB_ERR) 
				{
				MessageBox(hDlg, "No Choice selected",
				    "Select From a List", MB_OK | MB_ICONEXCLAMATION);
				break;
				}
			    Fractal.Fn1Index = index1;
			    SetDlgItemText(hDlg, ID_FRACPARAM1, Fractal.fnptr[index1]);
//			    EndDialog(hDlg, TRUE);
			    return ((DLGPROC) TRUE);
                  
			}
		    return ((DLGPROC) TRUE);
                case IDC_FNLIST2:
                    switch (HIWORD(wParam) & 0x0003) 
//                    switch (LOWORD(lParam)) 
			{
                        case LBN_SELCHANGE:
 //                           index2 = SendDlgItemMessage(hDlg, IDC_FNLIST2, LB_GETCURSEL, 0, 0L);
 //                           if (index2 == LB_ERR)
 //                               break;
 //                           break;
                         
                       case LBN_DBLCLK:
//			    goto okay;
			    index2 = (int)SendDlgItemMessage(hDlg, IDC_FNLIST2, LB_GETCURSEL, 0, 0L);
			    if (index2 == LB_ERR) 
				{
				MessageBox(hDlg, "No Choice selected",
				    "Select From a List", MB_OK | MB_ICONEXCLAMATION);
				break;
				}
			    Fractal.Fn2Index = index2;
			    SetDlgItemText(hDlg, ID_FRACPARAM2, Fractal.fnptr[index2]);
//			    EndDialog(hDlg, TRUE);
			    return ((DLGPROC) TRUE);
                  
			}
//		    return ((DLGPROC) TRUE);
	    return ((DLGPROC) TRUE);
	    }
	}
    return (FALSE);
    }

/**************************************************************************
	Dialog Box for loading functions
**************************************************************************/

DLGPROC FAR PASCAL LyapDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)

    {
    int i, j;
    static	int	index1 = -1, index2 = -1;
    static	int	numtrig;
    char		s[10][100];
    char		Bailout[120];
    CTrigFn		TrigFn;

    switch (message) 
	{
        case WM_INITDIALOG:
	    sprintf(Bailout, "%14.14lf", rqlim);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
//            SetDlgItemText(hDlg, ID_FRACNAME, 	fractalspecific[type].name);
            for (j = 0; j < Fractal.NumParam; j++) 
		{
		sprintf(s[j], "%f", *Fractal.ParamValue[j]);
		SetDlgItemText(hDlg, ID_FRACPARTX1 + j, Fractal.ParamName[j]);
		SetDlgItemText(hDlg, ID_FRACPARAM1 + j, s[j]);
		}
            for (i = Fractal.NumFunct + Fractal.NumParam; i < 10; i++) 
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
	    SetDlgItemText(hDlg, IDC_LYAPSEQ, LyapSequence);	// we cheat and use Fractal.Fn1 to store the Lyapunov sequence
	    return ((DLGPROC) TRUE);

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
//	    switch (wParam)
		{
                case IDOK:
		    GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
		    rqlim = atof(Bailout);
		    for (j = 0; j < Fractal.NumParam; j++) 
			{
			GetDlgItemText(hDlg, ID_FRACPARAM1 + j, s[j], 100);
			*Fractal.ParamValue[j] = atof(s[j]);
			}
		    GetDlgItemText(hDlg, IDC_LYAPSEQ, LyapSequence, 100);	// we cheat and use Fractal.Fn1 to store the Lyapunov sequence
		    EndDialog(hDlg, TRUE);
                    return ((DLGPROC) TRUE);
                  
                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    return (FALSE);
                    
//		    return ((DLGPROC) TRUE);
	    return ((DLGPROC) TRUE);
	    }
	}
    return (FALSE);
    }

/**************************************************************************
	Dialog Box for screen formulae
**************************************************************************/

#define	MAXFORMULASTRINGLENGTH		3600	

char	FormulaString[MAXFORMULASTRINGLENGTH] = "\0";		// used to hold the full formula
extern	char	*str_find_ci(char *, char *);

void	AnalyseFormulaString(char *Startup, char *OrigStartup, char *Formula, char *Bailout, char *FormulaString)

    {
    char    *p, *s, *t, *u;
    size_t  length = strlen(FormulaString);

    char    *TempStr = new char [length + 1];
    
    strcpy(TempStr, FormulaString);			// protect original string
    p = TempStr;

    u = str_find_ci(TempStr, ";");			// is it a comment?
    if (u)
	*u = '\0';					// remove comment
    s = str_find_ci(TempStr, ":");			// possible initial condition
    if (s)						// initial condition is given in the string and it's not part of a comment
	{
	*(s - 1) = '\0';				// replace the ':' with a NULL
	strcpy(Startup, p);
	p = s;
	}
    else 
	strcpy(Startup, OrigStartup);

    if ((t = str_find_ci(p, "|")) != 0)			// bailout is given in the string
	{ 
	*(t - 2) = '\0';				// t - 2 so we keep the '|'
	strcpy(Formula, p);
	p = t - 1;
	strcpy(Bailout, p);
	}
    else
	{
	strcpy(Formula, p);
	strcpy(Bailout, "|z| < 4.0");
	}
    
    if (TempStr) { delete[] TempStr; TempStr = NULL; }
    }

void	CreateFormulaString(char *Startup, char *Formula, char *Bailout, char *FormulaString)

    {
    char    *p, *q;
    char    temp[MAXFORMULASTRINGLENGTH];

    sprintf(temp, "%s:%s,%s", Startup, Formula, Bailout);
    p = temp;
    q = FormulaString;

    while (*p)
	{
	if (*p == '\n')
	    {
	    *q = ',';
	    p++; q++;
	    }
	else if(*p == '\r')
	    p++;			// remove linefeeds
	else
	    { 
	    *q = *p; 
	    p++; q++;
	    }
	}
    *q = '\0';
    }

DLGPROC FAR PASCAL ScrnFormDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)

    {
    int			i;
    static	int	index = -1;
    static	int	index1 = -1;
    static	char	Formula[1200] = "z = z*z + c + p1";
    static	char	Bailout[120] = "|z| < 4.0";
    static	char	Startup[1200] = "z=c=pixel";
    static	int	OldIndex1;		// used to check if formula has been edited. If it doesn't change, then it is either a repeat or a change ion the curremy formula

    switch (message) 
	{
        case WM_INITDIALOG:
	    for (i = 0; InitCond[i]; i++)
		SendDlgItemMessage(hDlg, IDC_INITIAL, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) InitCond[i]);
	    if (index == -1 || *Startup != '\0')		// index not loaded, but have a default
		index = FindInitCond(Startup);
            SendDlgItemMessage(hDlg, IDC_INITIAL, (UINT)LB_SETCURSEL, (WPARAM)((index == -1) ? 1 : index), 0L);

	    for (i = 0; DirectFormula[i]; i++)
		SendDlgItemMessage(hDlg, IDC_FORMULA, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) DirectFormula[i]);
	    if (index1 == -1 || *Formula != '\0')		// index not loaded, but have a default
		if (OldIndex1 != index1)			// it won't be found if the formula has been edited
		    index1 = FindFormula(Formula);
            SendDlgItemMessage(hDlg, IDC_FORMULA, (UINT)LB_SETCURSEL, (WPARAM)((index1 == -1) ? 0 : index1), 0L);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	    SetDlgItemText(hDlg, IDC_FORMULAVALUE, Formula);
	    SetDlgItemText(hDlg, IDC_INITIALVALUE, Startup);
	    OldIndex1 = index1;
            return ((DLGPROC) TRUE);

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
//	    switch (wParam)
		{
                case IDOK:
		    GetDlgItemText(hDlg, IDC_FORMULAVALUE, Formula, 1200);
		    GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
		    GetDlgItemText(hDlg, IDC_INITIALVALUE, Startup, 1200);
		    CreateFormulaString(Startup, Formula, Bailout, FormulaString);
		    EndDialog(hDlg, TRUE);
                    return ((DLGPROC) TRUE);
                  
                case IDCANCEL:
                    EndDialog(hDlg, FALSE);
                    return (FALSE);

                case IDC_FORMULA:
		    index1 = (int)SendDlgItemMessage(hDlg, IDC_FORMULA, LB_GETCURSEL, 0, 0L);
		    if (index1 == LB_ERR) 
			{
			MessageBox(hDlg, "No Choice selected","Select From a List", MB_OK | MB_ICONEXCLAMATION);
			break;
			}
						    index = (int)SendDlgItemMessage(hDlg, IDC_INITIAL, LB_GETCURSEL, 0, 0L);
		    if (index < 0) index = 1;
		    if (*DirectFormula[index1] != '\0')
			if (OldIndex1 != index1)			// don't overwrite an edited formula
			    AnalyseFormulaString(Startup, InitCond[index], Formula, Bailout, DirectFormula[index1]);
		    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
		    SetDlgItemText(hDlg, IDC_FORMULAVALUE, Formula);
		    SetDlgItemText(hDlg, IDC_INITIALVALUE, Startup);
		    return ((DLGPROC) TRUE);
                case IDC_INITIAL:
		    index = (int)SendDlgItemMessage(hDlg, IDC_INITIAL, LB_GETCURSEL, 0, 0L);
		    if (index == LB_ERR) 
			{
			MessageBox(hDlg, "No Choice selected","Select From a List", MB_OK | MB_ICONEXCLAMATION);
			break;
			}
		    SetDlgItemText(hDlg, IDC_INITIALVALUE, InitCond[index]);
		    strcpy(Startup, InitCond[index]);
//		    return ((DLGPROC) TRUE);
	    return ((DLGPROC) TRUE);
	    }
	}
    return (FALSE);
    }

/**************************************************************************
    Dialog Control for Stereo Pairs
**************************************************************************/

DLGPROC FAR PASCAL StereoPairDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)
    {
    static	UINT	tempPoints;
    static     char	temp[20];
    static     UINT	tempParam;
    BOOL		bTrans;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemInt(hDlg, IDC_PAIRFLAG, pairflag, TRUE);
	    SetDlgItemInt(hDlg, IDC_STEREODEPTH, AutoStereo_value, TRUE);
	    return (DLGPROC)TRUE;
	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDOK:
		    pairflag = GetDlgItemInt(hDlg, IDC_PAIRFLAG, &bTrans, TRUE);
		    AutoStereo_value = GetDlgItemInt(hDlg, IDC_STEREODEPTH, &bTrans, TRUE);
		    oldcalcmode = calcmode;
		    calcmode = '1';
		    EndDialog(hDlg, TRUE);
		    return (DLGPROC)TRUE;
			case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    break;
	}
    return FALSE;
    }

