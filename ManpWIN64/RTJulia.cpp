/*
    RTJULIA.CPP - a module to view Real Time Julia set images.
    
    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include "manp.h"
#include "fractype.h"
#include "resource.h"
#include "complex.h"
#include "pixel.h"
#include "plot.h"
#include "SafeStrings.h"
#include "FilterTemplate.h"
#include "FractalMathsTemplate.h"

#define	JULIA_SIZE	4.0
#define	THUMB_SIZE	6

static	int	ThumbSize = THUMB_SIZE;
static	int	Jwidth, Jheight, TempWidth, TempHeight, TempXdots, TempYdots, TempXgap, TempYgap;
static	std::vector<float> RTIJPixels;

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
    RTIJPixels.assign((size_t)Jwidth * Jheight, 0.0f);

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
    Draw Real Time Inverse Julia using the OtherFunctions renderer.
***************************************************************************/

int CManp::DrawJuliaOther(HWND hwnd)
    {
    double SaveParam[NUMPARAM];
    int SaveType = type;
    int IJPower;
    long TempIteration = 0;
    BYTE TempSpecialFlag = 0;
    struct __timeb64 DummyTime = {};

    int SaveXdots = xdots;
    int SaveYdots = ydots;
    double SaveHor = hor;
    double SaveVert = vert;
    double SaveWidth = mandel_width;

    for (int k = 0; k < NUMPARAM; k++)
	SaveParam[k] = param[k];

    IJPower = (type == MANDELFP) ? 2 : (int)param[0];

    // Load default Inverse Julia parameters.
    for (int k = 0; k < fractalspecific[INVERSEJULIA].numparams; k++)
	param[k] = fractalspecific[INVERSEJULIA].paramvalue[k];

    param[0] = Pixel[0]->q.x;
    param[1] = Pixel[0]->q.y;
    param[6] = IJPower;
    type = INVERSEJULIA;

    // Clear the Real Time Julia thumbnail.
    for (int y = 0; y < Jheight; y++)
	{
	for (int x = 0; x < Jwidth; x++)
	    Plot.OutRGBpoint((WORD)x, (WORD)y, { 0, 0, 0 });
	}

    // Temporarily use the RT Julia window as the IJ image.
    xdots = Jwidth;
    ydots = Jheight;
    mandel_width = JULIA_SIZE;
    hor = -(JULIA_SIZE * AspectRatio) / 2.0;
    vert = -JULIA_SIZE / 2.0;

    RTIJPixels.assign((size_t)Jwidth * Jheight, 0.0f);
    wpixels.swap(RTIJPixels);

    OthFn.InitOtherFunctions((WORD)type, subtype, hwnd, &TrueCol, &Dib, AntStatus, DummyTime, DummyTime, mandel_width, hor, vert, AspectRatio, &totpasses, &curpass, nullptr, CoordSystem, xAxis, yAxis, zAxis);
    OthFn.RunOtherFunctions((WORD)type, &TempSpecialFlag, &TempIteration, xdots, ydots, param, threshold, nullptr);

    wpixels.swap(RTIJPixels);

    xdots = SaveXdots;
    ydots = SaveYdots;
    hor = SaveHor;
    vert = SaveVert;
    mandel_width = SaveWidth;

    type = SaveType;

    for (int k = 0; k < NUMPARAM; k++)
	param[k] = SaveParam[k];

    InvalidateRect(hwnd, &rect, FALSE);

    return 0;
    }

/**************************************************************************
    Draw Real Time Julia using the CPixel renderer.
***************************************************************************/

int CManp::DrawJuliaPixel(HWND hwnd, POINTS ptCurrent)
    {
    char	s[120];
    WORD	i, j;
    int		result;
    DWORD	colour;
    short	percent, display_count;

    Pixel[0]->c.x = -Jwidth * Jxgap / 2.0;

    percent = 0;
    for (i = 0; i < Jwidth; i++)
	{
	Pixel[0]->c.x += Jxgap;
	Pixel[0]->c.y = -2.0;

	display_count = (10 * i) / Jwidth;
	if (display_count > percent)
	    {
	    percent = display_count;
	    SAFE_SPRINTF(s, "%14.14f, %14.14f, %d, %d %d%%",
		Pixel[0]->q.x, Pixel[0]->q.y, ptCurrent.x, ptCurrent.y, percent * 10);
	    SetWindowText(hwnd, s);
	    }

	for (j = 0; j < Jheight >> ((type == MANDELFP && TransformType == TRANSFORM_NONE) ? 1 : 0); j++)
	    {
	    Pixel[0]->c.y += Jygap;
	    Pixel[0]->z = TransformPointT<Complex, double>(Pixel[0]->c, TransformType, f_radius, f_xcenter, f_ycenter);
	    iteration = 0;
	    Pixel[0]->InitFractal(&(Pixel[0]->z), &(Pixel[0]->q));

	    for EVER
		{
		if (iteration >= threshold)
		    break;

		iteration += SpeedFactor;
		result = Pixel[0]->RunFractal(&(Pixel[0]->z), &(Pixel[0]->q));

		if (result)				// escape time
		    break;
		}

	    colour = iteration;

	    if ((type == SPECIALNEWT || type == MATEIN) && special != 0)	// split colours
		{
		if (Pixel[0]->phaseflag == 1)			// second phase
		    colour += Pixel[0]->special;
		else if (Pixel[0]->phaseflag == 2)		// third phase
		    colour += (Pixel[0]->special << 1);
		}						// default first phase

	    Plot.PlotPoint(i, (WORD)(Jheight - j - 2), colour);

	    if (type == MANDELFP && TransformType == TRANSFORM_NONE)
		Plot.PlotPoint((WORD)(Jwidth - i - 1), j, colour);	// make the best of symmetry for speed
	    }
	}

    if (ShowRTOrbits)
	RTJuliaOrbits(OrbitColour, NUM_ORBITS);

    InvalidateRect(hwnd, &rect, FALSE);

    SAFE_SPRINTF(s, "%14.14f, %14.14f, %d, %d",
	Pixel[0]->q.x, Pixel[0]->q.y, ptCurrent.x, ptCurrent.y);

    SetWindowText(hwnd, s);

    return 0;
    }

/**************************************************************************
	Draw small Julia set
	As we are in a treansitional phase where some functions are in CPixel 
	class and others are not, it's easier to have two functions for each 
	category: DrawJuliaPixel() and DrawJuliaOther()
	Dispatch to the pixel or OtherFunctions renderer.
***************************************************************************/

int CManp::DrawJulia(HWND hwnd, POINTS ptCurrent)
    {
    BYTE TempJuliaFlag;
    int result;

    if (RealTimeJuliaFlag == RTJ_INVERSE_JULIA)
	{
	if (type != MANDELFP && type != POWER)
	    {
	    char s[100];
	    SAFE_SPRINTF(s, "Real Time Inverse Julia is not available for fractal [%d]", type);
	    MessageBox(hwnd, s, "RTJulia", MB_OK);
	    return -1;
	    }
	}
    else if ((fractalspecific[type].flags & FUNCTIONINPIXEL) == 0 &&
	(fractalspecific[type].flags & FRACTINTINPIXEL) == 0 &&
	(fractalspecific[type].flags & TRIGINPIXEL) == 0)
	{
	char s[100];
	SAFE_SPRINTF(s, "Real Time Julia is not available for fractal [%d]", type);
	MessageBox(hwnd, s, "RTJulia", MB_OK);
	return -1;
	}

    RTJuliaActive = TRUE;

    TempJuliaFlag = Pixel[0]->juliaflag;
    Pixel[0]->juliaflag = TRUE;

    Complex Point;

    Point.x = Pixel[0]->mandel_width * (double)ptCurrent.x / (double)ydots + Pixel[0]->hor;
    Point.y = Pixel[0]->mandel_width * (double)(ydots - ptCurrent.y) / (double)ydots + Pixel[0]->vert;

    if (RotationAngle == NORMAL)
	Pixel[0]->q = Point;
    else
	Pixel[0]->q = AffineTransformT<Complex, double>(Point, ViewAffineParams);

    if (type == SCREENFORMULA)
	{
	param[0] = Pixel[0]->q.x;
	param[1] = Pixel[0]->q.y;
	}

    q = Pixel[0]->q;

    if (RealTimeJuliaFlag == RTJ_INVERSE_JULIA)
	result = DrawJuliaOther(hwnd);
    else
	result = DrawJuliaPixel(hwnd, ptCurrent);

    Pixel[0]->juliaflag = TempJuliaFlag;
    RTJuliaActive = FALSE;

    return result;
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

