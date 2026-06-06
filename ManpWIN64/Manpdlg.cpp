/*
    MANPDLG.CPP - Manp Dialog Code

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <windows.h>
#include <windowsx.h>
#include <fcntl.h>
#define  VIEWMAIN
#include "manpwin.h"
#include "manp.h"
#include "colour.h"
#include "fractype.h"
#include "fractalp.h"
#include "OtherFunctions.h"
#include "resource.h"
#include "menu.h"
#include "Fract.h"
#include "BigDouble.h"
#include "big.h"
#include "pixel.h"
#include "SafeStrings.h"
#include "..\parser\cmplx.h"
#include "..\parser\TrigFn.h"

extern	int	HenonPoints;

// Big num declarations **********************************************************
extern	int	decimals;
// Big num declarations **********************************************************

#define	MAXFUNCTIONS	60

/**************************************************************************
	Dialog Control for 3D Parameters
**************************************************************************/

INT_PTR CALLBACK Param3D (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
		SAFE_SPRINTF(s, "%2.4f", gManp->sclx);
		SetDlgItemText(hDlg, IDC_XSCALE, s);
		SAFE_SPRINTF(s, "%2.4f", gManp->scly);
		SetDlgItemText(hDlg, IDC_YSCALE, s);
		SAFE_SPRINTF(s, "%2.4f", gManp->sclz);
		SetDlgItemText(hDlg, IDC_ZSCALE, s);
		SetDlgItemInt(hDlg, IDC_XROTATE, (int)gManp->x_rot, TRUE);
		SetDlgItemInt(hDlg, IDC_YROTATE, (int)gManp->y_rot, TRUE);
		SetDlgItemInt(hDlg, IDC_ZROTATE, (int)gManp->z_rot, TRUE);
	        return  TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_XSCALE:
			GetDlgItemText(hDlg, IDC_XSCALE, temp_xscale, 30);
		        return  TRUE ;

		    case IDC_YSCALE:
			GetDlgItemText(hDlg, IDC_YSCALE, temp_yscale, 30);
		        return  TRUE ;

		    case IDC_ZSCALE:
			GetDlgItemText(hDlg, IDC_ZSCALE, temp_zscale, 30);
		        return  TRUE ;

		    case IDC_XROTATE:
			temp_xrot = GetDlgItemInt(hDlg, IDC_XROTATE, &bTrans, TRUE);
		        return  TRUE ;

		    case IDC_YROTATE:
			temp_yrot = GetDlgItemInt(hDlg, IDC_YROTATE, &bTrans, TRUE);
		        return  TRUE ;

		    case IDC_ZROTATE:
			temp_zrot = GetDlgItemInt(hDlg, IDC_ZROTATE, &bTrans, TRUE);
		        return  TRUE ;

		    case IDOK:
			sscanf(temp_xscale, "%lf", &gManp->sclx);
			sscanf(temp_yscale, "%lf", &gManp->scly);
			sscanf(temp_zscale, "%lf", &gManp->sclz);
			gManp->x_rot = (double)temp_xrot;
			gManp->y_rot = (double)temp_yrot;
			gManp->z_rot = (double)temp_zrot;
			gManp->cycleflag = FALSE;
			EndDialog (hDlg, TRUE);
			return  TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }

/**************************************************************************
	Dialog Control for Fractal Parameters
**************************************************************************/

INT_PTR CALLBACK FractalDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     HWND		hCtrl ;
//     static	HANDLE	hCursor;
//     short		nCtrlID, filterSum, i, divisor; 
     char		s[100];
     BOOL		bTrans ;
     CMatrix		Mat;			// transformation and rotation matrix
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
		SetDlgItemInt(hDlg, IDM_THRESHOLD, gManp->threshold, TRUE);
		SAFE_SPRINTF(s, "%02X%02X%02X", gManp->OrbitColour.rgbtRed, gManp->OrbitColour.rgbtGreen, gManp->OrbitColour.rgbtBlue);
		SetDlgItemText(hDlg, IDC_ORBITCOL, s);
		SetDlgItemInt(hDlg, IDM_ROTDEG, gManp->RotationAngle, TRUE);
		SetDlgItemInt(hDlg, IDM_THRESH_OFF, gManp->Offset3D, TRUE);
		SetDlgItemInt(hDlg, IDM_BLKINDEX, gManp->blockindex, TRUE);
		SetDlgItemInt(hDlg, IDM_LOGVAL, gManp->logval, TRUE);
		SetDlgItemInt(hDlg, IDM_TESSFILLCOL, gManp->fillcolor, TRUE);
		SetDlgItemInt(hDlg, IDM_DECOMP, gManp->decomp, TRUE);
		SetDlgItemInt(hDlg, IDC_SPECIAL, gManp->special, TRUE);
		SetDlgItemInt(hDlg, IDM_BIOMORPH, gManp->biomorph, TRUE);
		SetDlgItemInt(hDlg, IDM_MAXCOL, (int)gManp->potparam[0], TRUE);
		SetDlgItemInt(hDlg, IDM_SLOPE, (int)gManp->potparam[1], TRUE);
		SetDlgItemInt(hDlg, IDM_BAILOUT, (int)gManp->potparam[2], TRUE);
		SetDlgItemInt(hDlg, IDC_DISTEST, gManp->distest, TRUE);
		SetDlgItemInt(hDlg, IDC_DISTESTWIDTH, gManp->distestwidth, TRUE);
		SAFE_SPRINTF(s, "%7.7f", gManp->f_radius);
		SetDlgItemText(hDlg, IDM_RADIUS, s);
		SAFE_SPRINTF(s, "%7.7f", gManp->f_xcenter);
		SetDlgItemText(hDlg, IDM_CENTREX, s);
		SAFE_SPRINTF(s, "%7.7f", gManp->f_ycenter);
		SetDlgItemText(hDlg, IDM_CENTREY, s);
		SAFE_SPRINTF(s, "%2.2f", gManp->dStrands);
		SetDlgItemText(hDlg, IDM_STALKS, s);
		SAFE_SPRINTF(s, "%06X", gManp->BackgroundColour);
		SetDlgItemText(hDlg, IDC_BKGROUNDCOLOUR, s);

		SAFE_SPRINTF(s, "%5.2f", gManp->ColourSpeed);
		SetDlgItemText(hDlg, IDM_COLOURSPEED, s);
		SAFE_SPRINTF(s, "%5.2f", gManp->bump_transfer_factor);
		SetDlgItemText(hDlg, IDM_TRANSFACTOR, s);
		SetDlgItemInt(hDlg, IDM_STARTPAL, gManp->PaletteStart, TRUE);
		SAFE_SPRINTF(s, "%5.2f", gManp->lightDirectionDegrees);
		SetDlgItemText(hDlg, IDM_LIGHTDIR, s);
		SAFE_SPRINTF(s, "%5.2f", gManp->bumpMappingDepth);
		SetDlgItemText(hDlg, IDM_MAPDEPTH, s);
		SAFE_SPRINTF(s, "%5.2f", gManp->bumpMappingStrength);
		SetDlgItemText(hDlg, IDM_MAPSTRENGTH, s);
		
		TempBailoutTest = gManp->BailoutTestType;
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

		tempInsideMethod = gManp->InsideMethod;
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

		tempOutsideMethod = gManp->OutsideMethod;
		// can't access tierazon filter/colour methods, so set the radio button to TIERAZONFILTERS or TIERAZONCOLOURS
		if (gManp->OutsideMethod > TIERAZONFILTERS)
		    tempOutsideMethod = (gManp->OutsideMethod > TIERAZONCOLOURS) ? TIERAZONCOLOURS : TIERAZONFILTERS;
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
		CheckRadioButton(hDlg, IDC_CARTESIAN, IDC_CONICAL, IDC_CARTESIAN + gManp->CoordSystem);

		temp = gManp->calcmode;
	        switch (gManp->calcmode)
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

		temp_RotationAngle = gManp->RotationAngle % 360;
	        switch (gManp->RotationAngle)
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
		SendMessage(hCtrl, BM_SETCHECK, gManp->TrueCol.Stretch, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_EXPANDSTARCOLS);
		SendMessage(hCtrl, BM_SETCHECK, gManp->ExpandStarTrailColours, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_USEPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, gManp->UseCurrentPalette, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_INVERTFRACT);
		SendMessage(hCtrl, BM_SETCHECK, gManp->invert, 0L);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return  TRUE ;

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
			gManp->CoordSystem = CARTESIAN;
			break;
		    case IDC_SPHERICAL:
			gManp->CoordSystem = SPHERICAL;
			break;
		    case IDC_CYLINDRICAL:
			gManp->CoordSystem = CYLINDRICAL;
			break;
		    case IDC_PARABOLIC:
			gManp->CoordSystem = PARABOLIC;
			break;
		    case IDC_PARABOLOIDAL:
			gManp->CoordSystem = PARABOLOIDAL;
			break;
		    case IDC_ELLIPTICAL:
			gManp->CoordSystem = ELLIPTICAL;
			break;
		    case IDC_BIPOLAR:
			gManp->CoordSystem = BIPOLAR;
			break;
		    case IDC_TOROIDAL:
			gManp->CoordSystem = TOROIDAL;
			break;
		    case IDC_PROLATE_SPHEROIDAL:
			gManp->CoordSystem = PROLATE_SPHERE;
			break;
		    case IDC_OBLATE_SPHEROIDAL:
			gManp->CoordSystem = OBLATE_SPHERE;
			break;
		    case IDC_BISPHERICAL:
			gManp->CoordSystem = BISPHERICAL;
			break;
		    case IDC_CONICAL:
			gManp->CoordSystem = CONICAL;
			break;
		    case IDC_USEPALETTE:
			hCtrl = GetDlgItem (hDlg, IDC_USEPALETTE) ;
			gManp->UseCurrentPalette = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return  TRUE ;

		    case IDC_EXPANDSTARCOLS:
			hCtrl = GetDlgItem (hDlg, IDC_EXPANDSTARCOLS) ;
			gManp->ExpandStarTrailColours = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return  TRUE ;

		    case IDC_STRETCHPALETTE:
			hCtrl = GetDlgItem (hDlg, IDC_STRETCHPALETTE) ;
			gManp->TrueCol.Stretch = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return  TRUE ;

		    case IDC_INVERTFRACT:
			hCtrl = GetDlgItem (hDlg, IDC_INVERTFRACT) ;
			gManp->invert = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return  TRUE ;

		    case IDM_THRESH_OFF:
			temp_3Dthreshold = GetDlgItemInt(hDlg, IDM_THRESH_OFF, &bTrans, TRUE);
			if (temp_3Dthreshold > MAXTHRESHOLD)
			    temp_3Dthreshold = MAXTHRESHOLD;
			if (temp_3Dthreshold < 0)
			    temp_3Dthreshold = 0;
		        return  TRUE ;

		    case IDM_LOGVAL:
			temp_logval = GetDlgItemInt(hDlg, IDM_LOGVAL, &bTrans, TRUE);
		        return  TRUE ;

		    case IDM_BLKINDEX:
			temp_blockindex = GetDlgItemInt(hDlg, IDM_BLKINDEX, &bTrans, TRUE);
			if (temp_blockindex < 1)
			    temp_blockindex = 1;
			else if (temp_blockindex > 6)
			    temp_blockindex = 6;
		        return  TRUE ;

		    case IDM_TESSFILLCOL:
			temp_fillcolor = GetDlgItemInt(hDlg, IDM_TESSFILLCOL, &bTrans, TRUE);
		        return  TRUE ;

		    case IDM_BIOMORPH:
			temp_biomorph = GetDlgItemInt(hDlg, IDM_BIOMORPH, &bTrans, TRUE);
		        return  TRUE ;

		    case IDM_DECOMP:
			temp_decomp = GetDlgItemInt(hDlg, IDM_DECOMP, &bTrans, TRUE);
			return TRUE;

		    case IDC_SPECIAL:
			temp_special = GetDlgItemInt(hDlg, IDC_SPECIAL, &bTrans, TRUE);
			return TRUE;

		    case IDC_ORBITCOL:
			GetDlgItemText(hDlg, IDC_ORBITCOL, s, 30);
			sscanf(s, "%06X", &temp_OrbitColour);
		        return  TRUE ;

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
		        return  TRUE ;

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
			return TRUE;

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
		        return  TRUE ;

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
			return TRUE;

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
			return TRUE;
		    case IDM_ROTDEG:
			temp_RotationAngle = GetDlgItemInt(hDlg, IDM_ROTDEG, &bTrans, TRUE);
			CheckRadioButton(hDlg, IDC_NORMAL, IDC_OTHER_DEGREES, IDC_OTHER_DEGREES);
			return  TRUE ;

		    case IDOK:
			if (gManp->logval == 0 && temp_logval != 0)
			     gManp->init_log(hDlg);			// don't forget to initialise loglist
			gManp->logval = temp_logval;
			gManp->logflag = (gManp->logval != 0);
			gManp->biomorph = temp_biomorph;
			gManp->decomp = temp_decomp;
			gManp->blockindex = temp_blockindex;
			gManp->fillcolor = temp_fillcolor;
			temp_threshold = GetDlgItemInt(hDlg, IDM_THRESHOLD, &bTrans, TRUE);
//			if (temp_threshold >= MAXTHRESHOLD || temp_threshold < 1)
//			    temp_threshold = MAXTHRESHOLD - 1;
			if (temp_threshold < 1 || temp_threshold > MAXTHRESHOLD)
			    temp_threshold = MAXTHRESHOLD;
			gManp->threshold = (int)temp_threshold;
			gManp->Offset3D = temp_3Dthreshold;
//			if (Offset3D >= MAXTHRESHOLD)
//			    Offset3D = MAXTHRESHOLD;
//			inside_colour = temp_inside/* & 0x00ff*/;	// 0 - 255
//			calcmode = temp;
			if (temp_special < 0)
			    gManp->special = 0;
//			else if (temp_special > 255)
//			    special = 255;
			else
			    gManp->special = temp_special;
			gManp->BailoutTestType = TempBailoutTest;
			gManp->InsideMethod = tempInsideMethod;
			gManp->OutsideMethod = tempOutsideMethod;
			gManp->OrbitColour.rgbtBlue = temp_OrbitColour & 0xff;
			gManp->OrbitColour.rgbtGreen = (temp_OrbitColour >> 8) & 0xff;
			gManp->OrbitColour.rgbtRed = (temp_OrbitColour >> 16) & 0xff;
			if (gManp->InsideMethod == POTENTIAL)
			    {
			    gManp->potparam[0] = GetDlgItemInt(hDlg, IDM_MAXCOL, &bTrans, TRUE);
			    gManp->potparam[1] = GetDlgItemInt(hDlg, IDM_SLOPE, &bTrans, TRUE);
			    gManp->potparam[2] = GetDlgItemInt(hDlg, IDM_BAILOUT, &bTrans, TRUE);
			    }
			if (gManp->OutsideMethod >= TIERAZONFILTERS)
			    {
			    GetDlgItemText(hDlg, IDM_STALKS, s, 30);
			    gManp->calcmode = '2';
			    sscanf(s, "%lf", &gManp->dStrands);
			    }
			else
			    {
			    gManp->RGBFilter = FALSE;			// don't use the plotting routine for RGB filters
			    gManp->PlotType = NOSYM;
			    gManp->calcmode = temp;
			    }
			GetDlgItemText(hDlg, IDM_RADIUS, s, 30);
			sscanf(s, "%lf", &gManp->f_radius);
			GetDlgItemText(hDlg, IDM_CENTREX, s, 30);
			sscanf(s, "%lf", &gManp->f_xcenter);
			GetDlgItemText(hDlg, IDM_CENTREY, s, 30);
			sscanf(s, "%lf", &gManp->f_ycenter);
			GetDlgItemText(hDlg, IDC_BKGROUNDCOLOUR, s, 10);
			sscanf(s, "%X", &gManp->BackgroundColour);
			gManp->cycleflag = FALSE;
			gManp->RotationAngle = temp_RotationAngle % 360;
			if (temp_RotationAngle < 0)
			    gManp->RotationAngle = -(-temp_RotationAngle % 360);
			else
			    gManp->RotationAngle = temp_RotationAngle % 360;
			if (gManp->RotationAngle != NORMAL && gManp->RotationAngle != 90 && gManp->RotationAngle != 180 && gManp->RotationAngle != 270)
			    {
			    gManp->z_rot = (double)gManp->RotationAngle;
			    gManp->RotationCentre.x = gManp->hor + (gManp->mandel_width * gManp->AspectRatio) / 2;
			    gManp->RotationCentre.y = gManp->vert + gManp->mandel_width / 2;
			    if (OrigRotationAngle != gManp->RotationAngle)
				Mat.InitTransformation(gManp->RotationCentre.x, gManp->RotationCentre.y, 0.0, 0.0, 0.0, gManp->z_rot);
			    }
			OrigRotationAngle = gManp->RotationAngle;
			gManp->distest = GetDlgItemInt(hDlg, IDC_DISTEST, &bTrans, TRUE);
			gManp->distestwidth = GetDlgItemInt(hDlg, IDC_DISTESTWIDTH, &bTrans, TRUE);

			GetDlgItemText(hDlg, IDM_COLOURSPEED, s, 30);
			sscanf(s, "%lf", &gManp->ColourSpeed);
			GetDlgItemText(hDlg, IDM_TRANSFACTOR, s, 30);
			sscanf(s, "%lf", &gManp->bump_transfer_factor);
			gManp->PaletteStart = GetDlgItemInt(hDlg, IDM_STARTPAL, &bTrans, TRUE);
			GetDlgItemText(hDlg, IDM_LIGHTDIR, s, 30);
			sscanf(s, "%lf", &gManp->lightDirectionDegrees);
			GetDlgItemText(hDlg, IDM_MAPDEPTH, s, 30);
			sscanf(s, "%lf", &gManp->bumpMappingDepth);
			GetDlgItemText(hDlg, IDM_MAPSTRENGTH, s, 30);
			sscanf(s, "%lf", &gManp->bumpMappingStrength);

			EndDialog (hDlg, TRUE);
			return  TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }

/**************************************************************************
	Dialog Control for Fractal Type
**************************************************************************/

INT_PTR CALLBACK FractTypeDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
//     static	HANDLE	hCursor;
     static     UINT	temp;
     static     UINT	tempParam;
//     static     UINT	TempSubtype;
     
     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = gManp->type;
//		TempSubtype = subtype;
	        switch (gManp->type)
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
		    case NUMFRACTAL:
			tempParam = IDC_NUMFRACTAL;
			break;
		    case HAILSTONE:
			tempParam = IDC_HAILSTONE;
			break;

		    }
		CheckRadioButton(hDlg, IDC_MANDEL, IDC_TEST, tempParam);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return FALSE ;
//	        return  TRUE ;

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
		        return  TRUE ;
		    case IDC_NUMFRACTAL:
			temp = NUMFRACTAL;
			break;
		    case IDC_HAILSTONE:
			temp = HAILSTONE;
			break;

		    case IDOK:
			gManp->type = temp;
//			subtype = TempSubtype;
			gManp->cycleflag = FALSE;
			EndDialog (hDlg, TRUE);
			return  TRUE;

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
INT_PTR CALLBACK AboutDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     switch (message)
	  {
	  case WM_INITDIALOG:
#ifdef _WIN64
	      SetDlgItemText(hDlg, IDC_PROGRAMNAME, "ManpWIN 64 bit");
#else
	      SetDlgItemText(hDlg, IDC_PROGRAMNAME, "ManpWIN");
#endif // _WIN64
	       return  TRUE;

	  case WM_COMMAND:
	       switch (wParam)
		    {
		    case IDOK:
			EndDialog (hDlg, TRUE);
			return  TRUE;
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

    if (gManp->BigNumFlag)
	{
	Big_centrex = gManp->BigHor;
	Big_centrey = gManp->BigVert;
	gManp->BigHor = Big_centrex - (gManp->BigWidth * (gManp->AspectRatio / 2.0));
	gManp->BigVert = Big_centrey - gManp->BigWidth / 2.0;
	}
    else
	{
	centrex = gManp->hor;
	centrey = gManp->vert;
	gManp->hor = centrex - (gManp->mandel_width * (gManp->AspectRatio / 2.0));
	gManp->vert = centrey - (gManp->mandel_width / 2.0);
	}
    }

/**************************************************************************
	Get corner and width
**************************************************************************/

int	GetCorner(char *s1, char *s2, char *s3)
    {
    sscanf(s1, "%lf", &gManp->hor);
    sscanf(s2, "%lf", &gManp->vert);
    sscanf(s3, "%lf", &gManp->mandel_width);

    if (gManp->mandel_width < DBL_MIN)						// we can do a BigNum calculation here to allow deeper zooming
	{
	gManp->ConvertString2Bignum(gManp->BigWidth.x, s3);
	gManp->BigNumFlag = TRUE;
	}

    gManp->precision = gManp->getprecbf_mag();
    if (gManp->precision < 0)							// exceeded allowable precision
	return -1;
    if (gManp->precision > DBL_DIG - 3)
	{
	decimals = gManp->precision + PRECISION_FACTOR;
	if (gManp->ChangeBigPrecision(decimals) < 0)				// increase precision of Big numbers	
	    return -1;							// too many decimals for library

	gManp->BigNumFlag = TRUE;
	gManp->ConvertString2Bignum(gManp->BigHor.x, s1);
	gManp->ConvertString2Bignum(gManp->BigVert.x, s2);
	gManp->ConvertString2Bignum(gManp->BigWidth.x, s3);
	if (mpfr_sgn(gManp->BigWidth.x) == 0)					// no naughty division
	    mpfr_set_d(gManp->BigWidth.x, 1.0, MPFR_RNDN);
	}
    else
	{
	gManp->BigNumFlag = FALSE;
	if (gManp->mandel_width < DBL_MIN)
	    gManp->mandel_width = 1.0;
	}
    return 0;
    }

/**************************************************************************
	Dialog Box for loading co-ordinates
**************************************************************************/

INT_PTR CALLBACK CoordDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     static     UINT	tempParam;
     static     BOOL	CentreCoord = FALSE;
     static	long	temp_threshold;
     BOOL		bTrans;
     HWND		hCtrl;
     char		Bailout[120];
     char		t[120];
     char		JuliaReal[120];
     char		JuliaImag[120];
     int		i, k;
     char		s[10][100];
     std::vector<char>	s1(SIZEOF_BF_VARS);
     std::vector<char>	s2(SIZEOF_BF_VARS);
     std::vector<char>	s3(SIZEOF_BF_VARS);

     switch (message)
	  {
	  case WM_INITDIALOG:
	      if (gManp->Fractal.FractName == NULL)
		  gManp->InitFract(gManp->type);
	      SAFE_SPRINTF(JuliaReal, "%lf", gManp->j.x);
	      SAFE_SPRINTF(JuliaImag, "%lf", gManp->j.y);
	      SAFE_SPRINTF(Bailout, "%lf", gManp->rqlim);
	      SetDlgItemText(hDlg, IDC_JULIAREAL, JuliaReal);
	      SetDlgItemText(hDlg, IDC_JULIAIMAG, JuliaImag);
	      SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	      SetDlgItemText(hDlg, ID_FRACNAME, gManp->GetFractalName());
	      SetDlgItemInt(hDlg, IDC_THRESHOLD, gManp->threshold, TRUE);
	      hCtrl = GetDlgItem(hDlg, IDC_CENTRE);
	      SendMessage(hCtrl, BM_SETCHECK, CentreCoord, 0L);
	      hCtrl = GetDlgItem(hDlg, IDC_ISJULIA);
	      SendMessage(hCtrl, BM_SETCHECK, gManp->juliaflag, 0L);
	      if (gManp->BigNumFlag)
		  {
		  gManp->BigHor.ToString(s1.data(), SIZEOF_BF_VARS, false);
		  gManp->BigVert.ToString(s2.data(), SIZEOF_BF_VARS, false);
		  gManp->BigWidth.SafeSprintf(s3.data(), SIZEOF_BF_VARS, "%.20Re");
		  }
	      else
		  {
		  _snprintf_s(s1.data(), SIZEOF_BF_VARS, _TRUNCATE, "%18.18f", gManp->hor);
		  _snprintf_s(s2.data(), SIZEOF_BF_VARS, _TRUNCATE, "%18.18f", gManp->vert);
		  _snprintf_s(s3.data(), SIZEOF_BF_VARS, _TRUNCATE, "%18.18f", gManp->mandel_width);
		  }
	      
	      SetDlgItemText(hDlg, IDC_XCOORD, s1.data());
	      SetDlgItemText(hDlg, IDC_YCOORD, s2.data());
	      SetDlgItemText(hDlg, IDC_WIDTH, s3.data());
	      if (gManp->Fractal.NumFunct > 0)
		  {
		  SetDlgItemText(hDlg, ID_FRACPARAM1, gManp->Fractal.Fn1);
		  hCtrl = GetDlgItem (hDlg, ID_FRACPARAM1);
 		  EnableWindow (hCtrl, FALSE);
		  SetDlgItemText(hDlg, ID_FRACPARTX1, "First Function");
		  if (gManp->Fractal.NumFunct > 1)
		      {
		      SetDlgItemText(hDlg, ID_FRACPARAM2, gManp->Fractal.Fn2);
		      hCtrl = GetDlgItem (hDlg, ID_FRACPARAM2);
 		      EnableWindow (hCtrl, FALSE);
		      SetDlgItemText(hDlg, ID_FRACPARTX2, "Second Function");
		      }
		  }
	      for (i = gManp->Fractal.NumFunct, k = 0; i < gManp->Fractal.NumFunct + gManp->Fractal.NumParam && i < 10; i++, k++)
		  {
		  SAFE_SPRINTF(s[k], "%f", *gManp->Fractal.ParamValue[k]);
		  SetDlgItemText(hDlg, ID_FRACPARTX1 + i, fractalspecific[gManp->type].paramname[k]);
		  SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[k]);
		  hCtrl = GetDlgItem (hDlg, ID_FRACPARAM1 + i);
 		  EnableWindow (hCtrl, TRUE);
		  }
	      for (i = gManp->Fractal.NumFunct + gManp->Fractal.NumParam; i < 10; i++)
		  SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");

	      return  TRUE ;

	  case WM_COMMAND:
	      switch ((int) LOWORD(wParam))
		    {
		    case IDOK:
			hCtrl = GetDlgItem(hDlg, IDC_CENTRE);
			CentreCoord = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem(hDlg, IDC_ISJULIA);
			gManp->juliaflag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			GetDlgItemText(hDlg, IDC_XCOORD, s1.data(), SIZEOF_BF_VARS);
			GetDlgItemText(hDlg, IDC_YCOORD, s2.data(), SIZEOF_BF_VARS);
			GetDlgItemText(hDlg, IDC_WIDTH, s3.data(), SIZEOF_BF_VARS);
			if (GetCorner(s1.data(), s2.data(), s3.data()) < 0)
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
			    gManp->param[0] = 0.0;
			    gManp->param[1] = 0.0;
			    SAFE_SPRINTF(t, "Deep Zooming Limit (%d decimals) exceeded", SIZEOF_BF_VARS);
			    MessageBox (hDlg, t, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
			    MessageBeep (0);
			    }
			GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
			GetDlgItemText(hDlg, IDC_JULIAREAL, JuliaReal, 100);
			GetDlgItemText(hDlg, IDC_JULIAIMAG, JuliaImag, 100);
			gManp->rqlim = atof(Bailout);
			gManp->j.x = atof(JuliaReal);
			gManp->j.y = atof(JuliaImag);
			temp_threshold = GetDlgItemInt(hDlg, IDC_THRESHOLD, &bTrans, TRUE);
			if (temp_threshold < 1 || temp_threshold > MAXTHRESHOLD)
			    temp_threshold = MAXTHRESHOLD;
			gManp->threshold = (int)temp_threshold;
			for (i = gManp->Fractal.NumFunct, k = 0; i < gManp->Fractal.NumFunct + gManp->Fractal.NumParam && i < 10; i++, k++)
			    {
			    GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[k], 100);
			    *gManp->Fractal.ParamValue[k] = atof(s[k]);
			    }
			if (CentreCoord)
			    Centre2Edge();
			gManp->cycleflag = FALSE;
			EndDialog (hDlg, TRUE);
			return  TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }

/**************************************************************************
	Dialog Box for loading parameters
**************************************************************************/

INT_PTR CALLBACK ParamDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     char		s1[DBL_MANT_DIG];
     char		s2[DBL_MANT_DIG];
     char		s3[DBL_MANT_DIG];
     char		s4[DBL_MANT_DIG];
     char		s5[DBL_MANT_DIG];

     switch (message)
	  {
	  case WM_INITDIALOG:
		_snprintf_s(s1, DBL_MANT_DIG, _TRUNCATE, "%18.18f", gManp->param[0]);
		_snprintf_s(s2, DBL_MANT_DIG, _TRUNCATE, "%18.18f", gManp->param[1]);
		_snprintf_s(s3, DBL_MANT_DIG, _TRUNCATE, "%18.18f", gManp->param[2]);
		_snprintf_s(s4, DBL_MANT_DIG, _TRUNCATE, "%18.18f", gManp->param[3]);
		_snprintf_s(s5, DBL_MANT_DIG, _TRUNCATE, "%18.18f", gManp->param[4]);

		SetDlgItemText(hDlg, IDC_PARAM0, s1);
		SetDlgItemText(hDlg, IDC_PARAM1, s2);
		SetDlgItemText(hDlg, IDC_PARAM2, s3);
		SetDlgItemText(hDlg, IDC_PARAM3, s4);
		SetDlgItemText(hDlg, IDC_PARAM4, s5);
	        return  TRUE ;

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
			gManp->param[0] = atof(s1);
			gManp->param[1] = atof(s2);
			gManp->param[2] = atof(s3);
			gManp->param[3] = atof(s4);
			gManp->param[4] = atof(s5);

			gManp->cycleflag = FALSE;
			EndDialog (hDlg, TRUE);
			return  TRUE;

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

INT_PTR CALLBACK SelectFracParams(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

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
	    SAFE_SPRINTF(Bailout, "%14.14lf", gManp->rqlim);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);
	    numtrig = gManp->Fractal.TotalFunctions;
            SetDlgItemText(hDlg, ID_FRACNAME, gManp->GetFractalName());
//            SetDlgItemText(hDlg, ID_FRACNAME, 	fractalspecific[type].name);
	    if (gManp->Fractal.NumFunct > 0)
		{
		for (i = 0; i < numtrig; ++i)
		    SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR)gManp->Fractal.fnptr[i]);
		if (index1 == -1 || *gManp->Fractal.Fn1 != '\0')		// index not loaded, but have a default
		    index1 = TrigFn.FindFunct(gManp->Fractal.Fn1, numtrig);
		if (gManp->Fractal.NumFunct == 2)
		    {
		    for (i = 0; i < numtrig; ++i)
			SendDlgItemMessage(hDlg, IDC_FNLIST2, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR)gManp->Fractal.fnptr[i]);
		    if (index2 == -1 || *gManp->Fractal.Fn2 != '\0')		// index not loaded, but have a default
			index2 = TrigFn.FindFunct(gManp->Fractal.Fn2, numtrig);
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

            SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_SETCURSEL, (WPARAM)gManp->Fractal.fnptr, 0L);
	    if (index1 >= 0 && index1 < numtrig)
		SetDlgItemText(hDlg, ID_FRACPARAM1, gManp->Fractal.fnptr[index1]);
	    SetDlgItemText(hDlg, ID_FRACPARTX1, "First Function");

	    if (gManp->Fractal.NumFunct == 2)
		{
		if (index2 >= 0 && index2 < numtrig)
		    SetDlgItemText(hDlg, ID_FRACPARAM2, gManp->Fractal.fnptr[index2]);
		SetDlgItemText(hDlg, ID_FRACPARTX2, "Second Function");
		}
            for (i = gManp->Fractal.NumFunct, j = 0; i < gManp->Fractal.NumFunct + gManp->Fractal.NumParam && i < 10; i++, j++)
		{
		SAFE_SPRINTF(s[j], "%f", *gManp->Fractal.ParamValue[j]);
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, gManp->Fractal.ParamName[j]);
		SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[j]);
		}
            for (i = gManp->Fractal.NumFunct + gManp->Fractal.NumParam; i < 10; i++)
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");
            return ( TRUE);

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
//	    switch (wParam)
		{
                case IDOK:
//                    index1 = SendDlgItemMessage(hDlg, IDC_FNLIST1, LB_GETCURSEL, 0, 0L);
//                    index2 = SendDlgItemMessage(hDlg, IDC_FNLIST2, LB_GETCURSEL, 0, 0L);
                    if (gManp->Fractal.NumFunct >= 1)
			{
			if (index1 == LB_ERR) 
			    {
			    MessageBox(hDlg, "No Choice selected", "Select From a List", MB_OK | MB_ICONEXCLAMATION);
			    break;
			    }
			gManp->Fractal.Fn1Index = index1;
			gManp->Fractal.Fn1 = TrigFn.FunctList[index1];
			if (gManp->Fractal.NumFunct == 2)
			    {
			    if (index2 == LB_ERR) 
				{
				MessageBox(hDlg, "No Choice selected", "Select From a List", MB_OK | MB_ICONEXCLAMATION);
				break;
				}
			    gManp->Fractal.Fn2Index = index2;
			    gManp->Fractal.Fn2 = TrigFn.FunctList[index2];
			    }
			}
		    GetDlgItemText(hDlg, IDC_BAILOUT, Bailout, 100);
		    gManp->rqlim = atof(Bailout);
		    for (i = gManp->Fractal.NumFunct, j = 0; i < gManp->Fractal.NumFunct + gManp->Fractal.NumParam && i < 10; i++, j++)
			{
			GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[j], 100);
			*gManp->Fractal.ParamValue[j] = atof(s[j]);
			}
		    gManp->cycleflag = FALSE;
                    EndDialog(hDlg, TRUE);
                    return ( TRUE);
                  
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
			    gManp->Fractal.Fn1Index = index1;
			    SetDlgItemText(hDlg, ID_FRACPARAM1, gManp->Fractal.fnptr[index1]);
//			    EndDialog(hDlg, TRUE);
			    return ( TRUE);
                  
			}
		    return ( TRUE);
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
			    gManp->Fractal.Fn2Index = index2;
			    SetDlgItemText(hDlg, ID_FRACPARAM2, gManp->Fractal.fnptr[index2]);
//			    EndDialog(hDlg, TRUE);
			    return ( TRUE);
                  
			}
//		    return ( TRUE);
	    return ( TRUE);
	    }
	}
    return (FALSE);
    }

/**************************************************************************
    Dialog Control for Stereo Pairs
**************************************************************************/

INT_PTR CALLBACK StereoPairDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static	UINT	tempPoints;
    static     char	temp[20];
    static     UINT	tempParam;
    BOOL		bTrans;

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemInt(hDlg, IDC_PAIRFLAG, gManp->pairflag, TRUE);
	    SetDlgItemInt(hDlg, IDC_STEREODEPTH, gManp->AutoStereo_value, TRUE);
	    return TRUE;
	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDOK:
		    gManp->pairflag = GetDlgItemInt(hDlg, IDC_PAIRFLAG, &bTrans, TRUE);
		    gManp->AutoStereo_value = GetDlgItemInt(hDlg, IDC_STEREODEPTH, &bTrans, TRUE);
		    gManp->oldcalcmode = gManp->calcmode;
		    gManp->calcmode = '1';
		    EndDialog(hDlg, TRUE);
		    return TRUE;
			case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    break;
	}
    return FALSE;
    }





