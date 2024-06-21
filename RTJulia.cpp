/*
    RTJULIA.CPP a module to view Real Time Julia set images.
    
    Written in Microsoft Visual C++ by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "manp.h"
#include "fractype.h"
#include "resource.h"
#include "complex.h"
#include "pixel.h"
#include "plot.h"

#define	JULIA_SIZE	4.0
#define	THUMB_SIZE	6

static	int	ThumbSize = THUMB_SIZE;
static	int	Jwidth, Jheight, TempWidth, TempHeight, TempXdots, TempYdots, TempXgap, TempYgap;
static	double	Jxgap;					// gap between pixels
static	double	Jygap;					// gap between pixels
static	BYTE	SpeedFactor = 1;			// multiplier to reduce calculations
static	RECT 	rect;
	BOOL	RTJuliaActive = FALSE;			// block any more requests until calcs complete
static	BOOL	ShowRTOrbits = TRUE;			// show julia orbits for each image

extern	PAINTSTRUCT 	ps;
extern	WORD		type;				// M=mand, J=Julia 1,2,4->
extern	BYTE		juliaflag;
extern	int		RotationAngle;			// in degrees
extern	double		param[];
extern	BOOL		invert;				// invert fractal
extern	RGBTRIPLE	OrbitColour;			// Indexed colour for the orbit displays in Julia sets
extern	long		threshold;
extern	BYTE		phaseflag;			// 3 phases for type SPECIALNEWT fractals
extern	WORD		special;			// special colour for CBIN fractals

extern	double		hor;				// horizontal address
extern	double		vert;				// vertical address
extern	double		mandel_width;			// width of display
extern	int	height, xdots, ydots, width, bits_per_pixel;
extern	double		ScreenRatio;			// ratio of width / height for the screen

extern	long		iteration;			// globals for speed
//extern	BYTE		SpecialFlag;			// tell graphing routine of special conditions

extern	char		FormulaString[];		// used to hold the full formula

extern	int		ProcessFormulaString(char *);
extern	void		BuildJuliaFormulaString(void);

extern	BYTE		cycleflag;			// do colour cycling

extern	Complex	z, q, c;
extern	CPixel	Pix;		// routines for escape fractals
extern	CPlot	Plot;		// image plotting routines 

/**************************************************************************
	Initialise
***************************************************************************/

int	InitRTJulia(HWND hwnd)

    {
    double	temp_x, temp_y;

    rect.left   = 0;
    rect.top    = 0;
    rect.right  = (width / ThumbSize);
    rect.bottom = (height / ThumbSize);
    Jwidth = width / ThumbSize;
    Jheight = height / ThumbSize;
    temp_x = ScreenRatio / (double) (Jwidth);
    temp_y = 1.0 / (double) (Jheight);
    Jxgap = JULIA_SIZE * temp_x;
    Jygap = JULIA_SIZE * temp_y;
    if (type == SCREENFORMULA)			// force julia version using startup of "z = pixel:" (I can't think of a better way)
	{
	BuildJuliaFormulaString();
//	sprintf(FormulaString, "z = pixel:%s,|z| <= %f", FormulaStringPointer, rqlim);			// Julia
//	sprintf(FormulaString, "c = z = 1 / pixel:%s + p1,|z| <= %f", FormulaStringPointer, rqlim);	// Invert
	if (ProcessFormulaString(FormulaString) == -1)
	    return -1;
//	ProcessFormulaString("z = pixel:z = z*z + c + p1,|z| <= 4");
	}
    return 0;
    }

/**************************************************************************
	Plot orbits
***************************************************************************/

void	RTJuliaOrbits(RGBTRIPLE colour, int count)

    {
    int	    i, row, col;

    if (colour.rgbtBlue == 0 && colour.rgbtGreen == 0 && colour.rgbtRed == 0)
	return;										// nothing to do
    z = 0;
    for (i = 0; i < count; i++)
	{
	Pix.run_fractal(&z, &q);
	row = Jheight - (int)((Pix.z->y + 2.0) / Jygap);
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

int	DrawJulia(HWND hwnd, POINTS ptCurrent)

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
	sprintf(s, "Real Time Julia is not available for fractal [%d]", type);
	MessageBox(hwnd, s, "RTJulia", MB_OK);
	return -1;
	}

    RTJuliaActive = TRUE;			// block any moire requests until calcs complete
    TempJuliaFlag = Pix.juliaflag;			// force julia calc
    Pix.juliaflag = TRUE;
    switch (RotationAngle)
	{
	case 0:							// normal
	    Pix.q->x = Pix.mandel_width * (double)ptCurrent.x / (double)ydots + Pix.hor;
	    Pix.q->y = Pix.mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + Pix.vert;
	    break;
	case 90:						// 90 degrees
	    Pix.q->y = -(Pix.mandel_width * (double)ptCurrent.x / (double)ydots + Pix.hor);
	    Pix.q->x = -(Pix.mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + Pix.vert);
	    break;
	case 180:						// 180 degrees
	    Pix.q->x = -(Pix.mandel_width * (double)ptCurrent.x / (double)ydots + Pix.hor);
	    Pix.q->y = -(Pix.mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + Pix.vert);
	    break;
	case 270:						// 270 degrees
	    Pix.q->y = Pix.mandel_width * (double)ptCurrent.x / (double)ydots + Pix.hor;
	    Pix.q->x = Pix.mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + Pix.vert;
	    break;
	}

    if (type == SCREENFORMULA)
	{
	param[0] = Pix.q->x;
	param[1] = Pix.q->y;
	}
    Pix.c->x = -Jwidth * Jxgap / 2.0;

    percent = 0;
    for (i = 0; i < Jwidth; i++)
	{
	Pix.c->x += Jxgap;
	Pix.c->y = -2.0;
	display_count = (10 * i) / Jwidth;
	if (display_count > percent)
	    {
	    percent = display_count;
	    sprintf(s, "%14.14f, %14.14f, %d, %d %d%%", Pix.q->x, Pix.q->y, ptCurrent.x, ptCurrent.y, percent * 10);
	    SetWindowText(hwnd, s);			// Show formatted text in the caption bar
	    }
	for (j = 0; j < Jheight >> ((type == MANDELFP && !invert) ? 1 : 0); j++)
	    {
	    Pix.c->y += Jygap;
	    *Pix.z = (invert) ? Pix.invertz2(*Pix.c) : *Pix.c;
	    iteration = 0;
	    Pix.init_fractal(Pix.z, Pix.q);
	    for EVER
		{
		if (iteration >= threshold)
		    break;
		iteration += SpeedFactor;
		result = Pix.run_fractal(Pix.z, Pix.q);
		if (result)				// escape time
		    break;
		}
	    colour = (iteration/* << SpeedFactor*/);
	    if ((type == SPECIALNEWT || type == MATEIN) && special != 0)  // split colours
		{
		if (Pix.phaseflag == 1)				// second phase
		    colour += Pix.special;
		else if (Pix.phaseflag == 2)			// third phase
		    colour += (Pix.special << 1);
		}						// default first phase
	    Plot.PlotPoint(i, (WORD)(Jheight - j - 2), colour);
	    if (type == MANDELFP && !invert)
		Plot.PlotPoint((WORD)(Jwidth - i - 1), j, colour);	// make the best of symmetry for speed
	    }
	}

    if (ShowRTOrbits)
	RTJuliaOrbits(OrbitColour, NUM_ORBITS);
    InvalidateRect(hwnd, &rect, FALSE);
    sprintf(s, "%14.14f, %14.14f, %d, %d", Pix.q->x, Pix.q->y, ptCurrent.x, ptCurrent.y);
    SetWindowText(hwnd, s);				// Show formatted text in the caption bar
    Pix.juliaflag = TempJuliaFlag;			// restore julia to original type
    RTJuliaActive = FALSE;
    return 0;
    }

/**************************************************************************
	Dialog Control for Julia Thumbnail Size
**************************************************************************/

DLGPROC FAR PASCAL JuliaDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
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
		cycleflag = FALSE;
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
		        return (DLGPROC)TRUE ;

		    case IDC_SPEED:
			temp_speed = GetDlgItemInt(hDlg, IDC_SPEED, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDOK:
			hCtrl = GetDlgItem (hDlg, IDC_ORBITS);
			ShowRTOrbits = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			ThumbSize = temp;
			SpeedFactor = temp_speed;
			if (SpeedFactor < 1)
			    SpeedFactor = 1;
			if (SpeedFactor > 100)
			    SpeedFactor = 100;
			InitRTJulia(hDlg);
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

