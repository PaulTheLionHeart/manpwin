/*
    RTJULIA.CPP - a module to view Real Time Julia set images.
    
    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <vector>
#include "manp.h"
#include "fractype.h"
#include "resource.h"
#include "complex.h"
#include "pixel.h"
#include "plot.h"
#include "SafeStrings.h"
#include "PixelTemplate.h"

#define	JULIA_SIZE	4.0
#define	THUMB_SIZE	6

static	int	ThumbSize = THUMB_SIZE;
static	int	Jwidth, Jheight, TempWidth, TempHeight, TempXdots, TempYdots, TempXgap, TempYgap;
static	double	Jxgap;					// gap between pixels
static	double	Jygap;					// gap between pixels
static	BYTE	SpeedFactor = 1;			// multiplier to reduce calculations
static	RECT 	rect;
static	BOOL	ShowRTOrbits = TRUE;			// show julia orbits for each image

extern	int		ProcessFormulaString(char *);

/**************************************************************************
	Initialise
***************************************************************************/

int	CManp::InitRTJulia(HWND hwnd)
    {
    double	temp_x, temp_y;

    rect.left   = 0;
    rect.top    = 0;
    rect.right  = (width / ThumbSize);
    rect.bottom = (height / ThumbSize);
    Jwidth = width / ThumbSize;
    Jheight = height / ThumbSize;
    temp_x = AspectRatio / (double) (Jwidth);
    temp_y = 1.0 / (double) (Jheight);
    Jxgap = JULIA_SIZE * temp_x;
    Jygap = JULIA_SIZE * temp_y;
    if (type == SCREENFORMULA)			// force julia version using startup of "z = pixel:" (I can't think of a better way)
	{
	BuildJuliaFormulaString();
	if (ProcessFormulaString(FormulaString) == -1)
	    return -1;
	}
    return 0;
    }

/**************************************************************************
	Plot orbits
***************************************************************************/

void	CManp::RTJuliaOrbits(RGBTRIPLE colour, int count)
    {
    int	    i, row, col;
    Complex	z, q;

    if (colour.rgbtBlue == 0 && colour.rgbtGreen == 0 && colour.rgbtRed == 0)
	return;										// nothing to do
    z = 0;
    for (i = 0; i < count; i++)
	{
	gManp->Pixel[0]->RunFractal(&z, &q);
	row = Jheight - (int)((gManp->Pixel[0]->z.y + 2.0) / Jygap);
	col = (int)(z.x / Jxgap + (double)Jwidth / 2.0);
	if (col > rect.left && col < rect.right && row > rect.top && row < rect.bottom)
	    Plot.OutRGBpoint(col, row, colour);
	}
    }

/**************************************************************************
	Draw small Julia set
	As we are in a treansitional phase where some functions are in CPixel 
	class and others are not, it's easier to have two functions for each 
	category: DrawJuliaPixel() and DrawJuliaOther()
***************************************************************************/

int	CManp::DrawJulia(HWND hwnd, POINTS ptCurrent)
    {
    char	s[120];
    WORD	i, j, offset = 0;
    BYTE	TempJuliaFlag;
    int		result;
    DWORD	colour;
    short	percent, display_count;

    if ((fractalspecific[type].flags & FUNCTIONINPIXEL) == 0 && (fractalspecific[type].flags & FRACTINTINPIXEL) == 0 && (fractalspecific[type].flags & TRIGINPIXEL) == 0)
	{
	char	s[100];
	SAFE_SPRINTF(s, "Real Time Julia is not available for fractal [%d]", type);
	MessageBox(hwnd, s, "RTJulia", MB_OK);
	return -1;
	}

    RTJuliaActive = TRUE;			// block any moire requests until calcs complete
    TempJuliaFlag = gManp->Pixel[0]->juliaflag;			// force julia calc
    gManp->Pixel[0]->juliaflag = TRUE;
    switch (RotationAngle)
	{
	case 0:							// normal
	    gManp->Pixel[0]->q.x = gManp->Pixel[0]->mandel_width * (double)ptCurrent.x / (double)ydots + gManp->Pixel[0]->hor;
	    gManp->Pixel[0]->q.y = gManp->Pixel[0]->mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + gManp->Pixel[0]->vert;
	    break;
	case 90:						// 90 degrees
	    gManp->Pixel[0]->q.y = -(gManp->Pixel[0]->mandel_width * (double)ptCurrent.x / (double)ydots + gManp->Pixel[0]->hor);
	    gManp->Pixel[0]->q.x = -(gManp->Pixel[0]->mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + gManp->Pixel[0]->vert);
	    break;
	case 180:						// 180 degrees
	    gManp->Pixel[0]->q.x = -(gManp->Pixel[0]->mandel_width * (double)ptCurrent.x / (double)ydots + gManp->Pixel[0]->hor);
	    gManp->Pixel[0]->q.y = -(gManp->Pixel[0]->mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + gManp->Pixel[0]->vert);
	    break;
	case 270:						// 270 degrees
	    gManp->Pixel[0]->q.y = gManp->Pixel[0]->mandel_width * (double)ptCurrent.x / (double)ydots + gManp->Pixel[0]->hor;
	    gManp->Pixel[0]->q.x = gManp->Pixel[0]->mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + gManp->Pixel[0]->vert;
	    break;
	}

    if (type == SCREENFORMULA)
	{
	param[0] = gManp->Pixel[0]->q.x;
	param[1] = gManp->Pixel[0]->q.y;
	}
    gManp->Pixel[0]->c.x = -Jwidth * Jxgap / 2.0;
    // update "global" q (stored in manp.h) so we can print the value through IDM_FRACTLOC message if required.
    gManp->q = gManp->Pixel[0]->q;
        percent = 0;
    for (i = 0; i < Jwidth; i++)
	{
	gManp->Pixel[0]->c.x += Jxgap;
	gManp->Pixel[0]->c.y = -2.0;
	display_count = (10 * i) / Jwidth;
	if (display_count > percent)
	    {
	    percent = display_count;
	    SAFE_SPRINTF(s, "%14.14f, %14.14f, %d, %d %d%%", gManp->Pixel[0]->q.x, gManp->Pixel[0]->q.y, ptCurrent.x, ptCurrent.y, percent * 10);
	    SetWindowText(hwnd, s);			// Show formatted text in the caption bar
	    }
	for (j = 0; j < Jheight >> ((type == MANDELFP && !invert) ? 1 : 0); j++)
	    {
	    gManp->Pixel[0]->c.y += Jygap;
	    gManp->Pixel[0]->z = (invert) ? Invertz2T<Complex, double>(gManp->Pixel[0]->c, f_radius, f_xcenter, f_ycenter) : gManp->Pixel[0]->c;
	    iteration = 0;
	    gManp->Pixel[0]->InitFractal(&(gManp->Pixel[0]->z), &(gManp->Pixel[0]->q));
	    for EVER
		{
		if (iteration >= threshold)
		    break;
		iteration += SpeedFactor;
		result = gManp->Pixel[0]->RunFractal(&(gManp->Pixel[0]->z), &(gManp->Pixel[0]->q));
		if (result)				// escape time
		    break;
		}
	    colour = (iteration/* << SpeedFactor*/);
	    if ((type == SPECIALNEWT || type == MATEIN) && special != 0)  // split colours
		{
		if (gManp->Pixel[0]->phaseflag == 1)				// second phase
		    colour += gManp->Pixel[0]->special;
		else if (gManp->Pixel[0]->phaseflag == 2)			// third phase
		    colour += (gManp->Pixel[0]->special << 1);
		}						// default first phase
	    Plot.PlotPoint(i, (WORD)(Jheight - j - 2), colour);
	    if (type == MANDELFP && !invert)
		Plot.PlotPoint((WORD)(Jwidth - i - 1), j, colour);	// make the best of symmetry for speed
	    }
	}

    if (ShowRTOrbits)
	RTJuliaOrbits(OrbitColour, NUM_ORBITS);
    InvalidateRect(hwnd, &rect, FALSE);
    SAFE_SPRINTF(s, "%14.14f, %14.14f, %d, %d", gManp->Pixel[0]->q.x, gManp->Pixel[0]->q.y, ptCurrent.x, ptCurrent.y);
    SetWindowText(hwnd, s);				// Show formatted text in the caption bar
    gManp->Pixel[0]->juliaflag = TempJuliaFlag;			// restore julia to original type
    RTJuliaActive = FALSE;
    return 0;
    }

/**************************************************************************
	Dialog Control for Julia Thumbnail Size
**************************************************************************/

INT_PTR CALLBACK JuliaDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     static	HANDLE	hCursor;
     static     char	temp;
     static     UINT	tempParam;
     static     BYTE	temp_speed;
     BOOL		bTrans ;
     HWND		hCtrl;
     
     switch (message)
	  {
	  case WM_INITDIALOG:
		gManp->cycleflag = FALSE;
		SetDlgItemInt(hDlg, IDC_SPEED, (int)SpeedFactor, TRUE);
		SetFocus(GetDlgItem(hDlg, IDC_SPEED));
	        temp = ThumbSize;
	        switch (ThumbSize)
		    {
		    case 10:
			tempParam = IDC_JULIA10;
			break;
		    case 8:
			tempParam = IDC_JULIA8;
			break;
		    case 6:
			tempParam = IDC_JULIA6;
			break;
		    case 4:
			tempParam = IDC_JULIA4;
			break;
		    case 3:
			tempParam = IDC_JULIA3;
			break;
		    default:				// uninitialised
			tempParam = IDC_JULIA6;
			temp = 6;
			break;
		    }
		CheckRadioButton(hDlg, IDC_JULIA10, IDC_JULIA3, tempParam);
		hCtrl = GetDlgItem (hDlg, IDC_ORBITS);
		SendMessage(hCtrl, BM_SETCHECK, ShowRTOrbits, 0L);
	        return FALSE ;
//	        return TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_JULIA10:
		    case IDC_JULIA8:
		    case IDC_JULIA6:
		    case IDC_JULIA4:
		    case IDC_JULIA3:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_JULIA10:
				temp = 10;
				break;
			    case IDC_JULIA8:
				temp = 8;
				break;
			    case IDC_JULIA6:
				temp = 6;
				break;
			    case IDC_JULIA4:
				temp = 4;
				break;
			    case IDC_JULIA3:
				temp = 3;
				break;
			    }

			CheckRadioButton(hDlg, IDC_JULIA10, IDC_JULIA3, (int) LOWORD(wParam));
		        return TRUE ;

		    case IDC_SPEED:
			temp_speed = GetDlgItemInt(hDlg, IDC_SPEED, &bTrans, TRUE);
		        return TRUE ;

		    case IDOK:
			hCtrl = GetDlgItem (hDlg, IDC_ORBITS);
			ShowRTOrbits = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			ThumbSize = temp;
			SpeedFactor = temp_speed;
			if (SpeedFactor < 1)
			    SpeedFactor = 1;
			if (SpeedFactor > 100)
			    SpeedFactor = 100;
			gManp->InitRTJulia(hDlg);
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

