/*
   CIRCLE.CPP a module for CIRCLE Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"menu.h"
#include	"anim.h"
#include	"fractalp.h"
#include	"colour.h"
#include	"Plot.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 48

extern	HWND	GlobalHwnd;			// This is the main windows handle

extern	CTrueCol    TrueCol;			// palette info

extern	int	power(int base, int n);
extern	int	user_data(HWND);

extern	BYTE	default_palette[];		// default VGA colour palette
extern	CPlot	Plot;		// image plotting routines 
extern	CDib	Dib;				// Device Independent Bitmap

extern	long	threshold;
extern	double	mandel_width;			// width of display 
extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address 
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	WORD	type;				// fractal type
extern	int	subtype;			// A - E
extern	int	curpass, totpasses;
extern	int	xdots, ydots;

static	double	xscale, yscale;
static	int	Passes = 10;
static	BOOL	UseDefaultPalette = TRUE;	// standard EGA palette
extern	int	FilledCircle;			// 1 = filled, 2 = unfilled, 3 = 3D

/*---------------------------------------------------------------------
	Display a Circle (based on Michener's Algorithm)
  -------------------------------------------------------------------*/
/*
void	DoPlot(int x, int y, DWORD colour)
    {
    if (x >= 0 && y >= 0 && x < xdots && y < ydots)				// allow underflow testing
	plot(x, y, colour);
    }

void	DisplayCircle(int centrex, int centrey, int radius, DWORD colour)

    {
    int	x, y, d, i;

    i = x = 0;
    y = radius;
    d = 3 - 2 * radius;

    while (x <= y)							// draw 8 points per iteration...uses best symmetry
	{
	DoPlot(centrex + x, centrey + y, colour);
	DoPlot(centrex - x, centrey + y, colour);
	DoPlot(centrex + x, centrey - y, colour);
	DoPlot(centrex - x, centrey - y, colour);
	DoPlot(centrex + y, centrey + x, colour);
	DoPlot(centrex - y, centrey + x, colour);
	DoPlot(centrex + y, centrey - x, colour);
	DoPlot(centrex - y, centrey - x, colour);
	if (d < 0)
	    d = d + 4 * x + 6;					// update error term
	else
	    {
	    d = d + 4 * (x - y) + 10;
	    y--;
	    }
	x++;
	}
    }
*/

/*---------------------------------------------------------------------
	Display a filled Circle 
  -------------------------------------------------------------------*/

/*
void	DisplayFilledCircle(int centrex, int centrey, int radius, DWORD colour)

    {
    int x, y;

    for (x = -radius; x <= radius ; x++)
	{
	if (x + centrex < 0 || x + centrex >= xdots)
	    continue;
	int height = (int)sqrt((double)(radius * radius) - (double)(x * x));

	for (y = -height; y <= height; y++)
	    {
	    if (y + centrey < 0 || y + centrey >= ydots || x + centrex < 0 || x + centrex >= xdots)
		continue;
	    DoPlot(x + centrex, y + centrey, colour);
	    }
	}
    }
*/

/*---------------------------------------------------------------------
	Generate scaled circle
  -------------------------------------------------------------------*/

void	ScaledCircle(double x, double y, double radius, DWORD colour, CPlot Plot)
    {
    int	    j, k, rad;
    BYTE    rgb[3];

    j = (int)((x - hor) * xscale);
    rad = (int)(radius * xscale);
    k = (int)((vert + mandel_width - y) * yscale);
    switch (FilledCircle)
	{
	case 1: 
	    Plot.DisplayFilledCircle(j, k, rad, colour);
	    break;
	case 2: 
	    Plot.DisplayCircle(j, k, rad, colour);
	    break;
	case 3: 
	    Plot.GetRGB(colour%threshold, rgb);
	    Plot.Display3DCircle(&Dib, j, k, rad, rgb);
	    break;
	}
//    if (FillCircle)
//	DisplayFilledCircle(j, k, rad, colour);
//    else
//	DisplayCircle(j, k, rad, colour);
    }

/**************************************************************************
	Ford Froth Circle Images
***************************************************************************/

int	DoFordFroth(CPlot Plot)

    {
    int	    k, h;
    double  x, y;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    if (subtype == 'L')
	{
	x = 0;
	y = 0.5;
	ScaledCircle(x, y +	0.125, y, 1, Plot);
	ScaledCircle(x, -y + 0.125, y, 1, Plot);
	x = 1;
	ScaledCircle(x, y +	0.125, y, 1, Plot);
	ScaledCircle(x, -y + 0.125, y, 1, Plot);
	}

    for (k = 1; k < Passes * 12; k++)
	{
	for (h = 1; h < Passes * 12; h++)
	    {
	    x = (double)h / (double)k;
	    y = 1 / (2 * (double)k * (double)k);
	    if (x == 1.0)
		break;
	    ScaledCircle(x, y +	0.125, y, k, Plot);
	    ScaledCircle(x, -y + 0.125, y, k, Plot);
	    }
	}
    return 0;
    }

/**************************************************************************
	Cantor set curved I.F.S.
	Union of circle curve set with Cantor dust set
***************************************************************************/

int	DoCurvedIFS(CPlot Plot)
    {
    double	x, y, x1, y1, r;
    double	i;
    int		u, v, p; 
    double	iterations = 5000000.0 / mandel_width;

    x = 0.3 ;
    y = 0.1;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    srand((unsigned)time(NULL));
    u = 0; v = 0;							// Initial line draw points
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	p = ((rand() % 2) == 0 ? 1 : -1);
	r = sqrt(x*x + y*y);
	switch (rand() % 4) 
	    {
	    case 0:
	    case 1:
		x1 = x / r  - 1;
		y1 = p * y / r;
		break;
	    case 2:
		x1 =  x / 3.0;
		y1 =  y / 3.0;
		break;
	    case 3:
		x1 = x / 3.0 + 2.0 / 3.0;
		y1 = y / 3.0;
		break;
	    }
	x = x1;
	y = y1;
	if (i < 100)
	    continue;
	u = (int)((x - hor) * xscale);
	v = (int)((vert + mandel_width - y) * yscale);
	Plot.PlotPoint(u, v, (DWORD)i % threshold);
	}

    return 0;
    }

/*---------------------------------------------------------------------
	Circle Fractal Type Images
  -------------------------------------------------------------------*/

int	DoCircle(void)

    {
    int		gen, bot, top, count, i;
    double	dx, dy, l, frac;
    double	radius, offset, m[10240];
    DWORD	colour;

    if (UseDefaultPalette)
	memcpy(TrueCol.PalettePtr, default_palette, 256 * 3);
//	TrueCol.PalettePtr = default_palette;
    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    if (subtype == 'K' || subtype == 'L')
	return (DoFordFroth(Plot));				// Ford Froth Circle
    else if (subtype == 'M')
	return DoCurvedIFS(Plot);
    offset = xscale - yscale;
    offset = 1;
    radius = 1;
    count = 1;
    frac = 1;
    dx = 1;
    dy = 1;
    m[count] = 1;
    if (subtype != 'J')
	ScaledCircle(m[count] + dx, dy, radius, 1, Plot);

    for(gen = 0; gen < Passes; gen++)
	{
	colour = threshold * (DWORD)gen / Passes;
	bot = power(2, gen);
	top = power(2, gen+1);
	radius /= 2;
	l = radius;
	for(i = bot; i < top; i++)
	    {
	    switch (subtype)
		{
		case 'A':
		case 'B':
		    m[count+1] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1, Plot);
		    if (subtype == 'B')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 1) % 14 + 1, Plot);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+2] + dx, dy, radius, (count + 2) % 14 + 1, Plot);
		    if (subtype == 'B')
			ScaledCircle(dy + offset, m[count+2] + dx - offset, radius, (count + 2) % 14 + 1, Plot);
		    count += 2;
		    break;
		case 'C':
		case 'D':
		    m[count+1] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 12, Plot);
		    if (subtype == 'D')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 1) % 14 + 1, Plot);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1, Plot);
		    if (subtype == 'D')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 2) % 14 + 1, Plot);
		    count += 2;
		    break;
		case 'E':
		case 'F':
		    m[count+1] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1, Plot);
		    if (subtype == 'F')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 1) % 14 + 1, Plot);
		    m[count+2] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1, Plot);
		    if (subtype == 'F')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 2) % 14 + 1, Plot);
		    count += 2;
		    break;
		case 'G':
		    m[count+1] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1, Plot);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1, Plot);
		    count += 2;
		    break;
		case 'H':
		    m[count+1] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1, Plot);
		    m[count+1] = m[i] - frac * l;
		    m[count+2] = m[i] - frac * l;
		    count += 2;
		    break;
		case 'I':
		    m[count+1] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1, Plot);
		    m[count+1] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1, Plot);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1, Plot);
		    m[count+2] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1, Plot);
		    count += 2;
		    break;
		case 'J':
		    m[count+1] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1, Plot);
		    ScaledCircle(-m[count+1] + dx - (yscale - 10)/2, dy, radius, (count + 1) % 14 + 1, Plot);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1, Plot);
		    ScaledCircle(-m[count+1] + dx - (yscale - 10)/2, dy, radius, (count + 2) % 14 + 1, Plot);
		    count += 2;
		    break;
		}
	    }
	}
    return 0;
    }

/*---------------------------------------------------------------------
	Circle Fractal Type dialogue box
  -------------------------------------------------------------------*/

DLGPROC FAR PASCAL CircleDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static     char	temp;
     static     UINT	tempParam, tempParam1;
     static     WORD	temp_special;
     HWND		hCtrl;
     BOOL		bTrans;
//     char		s[24];

     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = subtype;
	        switch (subtype)
		    {
		    case 'A':
			tempParam = IDC_A;
			break;
		    case 'B':
			tempParam = IDC_B;
			break;
		    case 'C':
			tempParam = IDC_C;
			break;
		    case 'D':
			tempParam = IDC_D;
			break;
		    case 'E':
			tempParam = IDC_E;
			break;
		    case 'F':
			tempParam = IDC_F;
			break;
		    case 'G':
			tempParam = IDC_G;
			break;
		    case 'H':
			tempParam = IDC_H;
			break;
		    case 'I':
			tempParam = IDC_I;
			break;
		    case 'J':
			tempParam = IDC_J;
			break;
		    case 'K':
			tempParam = IDC_K;
			break;
		    case 'L':
			tempParam = IDC_L;
			break;
		    case 'M':
			tempParam = IDC_M;
			break;
/*
		    case 'N':
			tempParam = IDC_N;
			break;
		    case 'O':
			tempParam = IDC_O;
			break;
		    case 'P':
			tempParam = IDC_P;
			break;
		    case 'Q':
			tempParam = IDC_Q;
			break;
		    case 'R':
			tempParam = IDC_R;
			break;
		    case 'S':
			tempParam = IDC_S;
			break;
		    case 'T':
			tempParam = IDC_T;
			break;
*/
		    default:				// uninitialised
			tempParam = IDC_A;
			temp = 'A';
			break;
		    }
		CheckRadioButton(hDlg, IDC_A, IDC_M, tempParam);
		SetDlgItemInt(hDlg, IDC_PARAM1, (int)Passes, TRUE);
		hCtrl = GetDlgItem (hDlg, IDC_DEFAULTPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, UseDefaultPalette, 0L);
	        switch (FilledCircle)
		    {
		    case 1:
			tempParam1 = IDC_FILLED;
			break;
		    case 2:
			tempParam1 = IDC_UNFILLED;
			break;
		    case 3:
			tempParam1 = IDC_3D;
			break;
		    }
		CheckRadioButton(hDlg, IDC_FILLED, IDC_3D, tempParam1);
//		SetFocus(GetDlgItem(hDlg, tempParam));
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_A:
		    case IDC_B:
		    case IDC_C:
		    case IDC_D:
		    case IDC_E:
		    case IDC_F:
		    case IDC_G:
		    case IDC_H:
		    case IDC_I:
		    case IDC_J:
		    case IDC_K:
		    case IDC_L:
		    case IDC_M:
		    case IDC_N:
		    case IDC_O:
		    case IDC_P:
		    case IDC_Q:
		    case IDC_R:
		    case IDC_S:
		    case IDC_T:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_A:
				temp = 'A';
				break;
			    case IDC_B:
				temp = 'B';
				break;
			    case IDC_C:
				temp = 'C';
				break;
			    case IDC_D:
				temp = 'D';
				break;
			    case IDC_E:
				temp = 'E';
				break;
			    case IDC_F:
				temp = 'F';
				break;
			    case IDC_G:
				temp = 'G';
				break;
			    case IDC_H:
				temp = 'H';
				break;
			    case IDC_I:
				temp = 'I';
				break;
			    case IDC_J:
				temp = 'J';
				break;
			    case IDC_K:
				temp = 'K';
				break;
			    case IDC_L:
				temp = 'L';
				break;
			    case IDC_M:
				temp = 'M';
				break;
/*
			    case IDC_N:
				temp = 'N';
				break;
			    case IDC_O:
				temp = 'O';
				break;
			    case IDC_P:
				temp = 'P';
				break;
			    case IDC_Q:
				temp = 'Q';
				break;
			    case IDC_R:
				temp = 'R';
				break;
			    case IDC_S:
				temp = 'S';
				break;
			    case IDC_T:
				temp = 'T';
				break;
*/
			    }

			CheckRadioButton(hDlg, IDC_A, IDC_M, (int) LOWORD(wParam));
		        return (DLGPROC)TRUE ;

		    case IDC_FILLED:
		    case IDC_UNFILLED:
		    case IDC_3D:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_FILLED:
				FilledCircle = 1;
				break;
			    case IDC_UNFILLED:
				FilledCircle = 2;
				break;
			    case IDC_3D:
				FilledCircle = 3;
				break;
			    }
			CheckRadioButton(hDlg, IDC_FILLED, IDC_3D, (int) LOWORD(wParam));
//			InvalidateRect(hDlg, NULL, FALSE);
		        return (DLGPROC)TRUE ;

		    case IDOK:
			Passes = GetDlgItemInt(hDlg, IDC_PARAM1, &bTrans, TRUE);
			if (Passes > 12)
			    Passes = 12;
			subtype = temp;
			switch (subtype)
			    {
			    case 'K':
				fractalspecific[type].hor = -0.1;
				fractalspecific[type].vert = -0.2;
				fractalspecific[type].width = 0.65;
				break;
			    case 'L':
				fractalspecific[type].hor = -1.4;
				fractalspecific[type].vert = -0.96;
				fractalspecific[type].width = 2.2;
				break;
			    case 'M':
				fractalspecific[type].hor = -3.0;
				fractalspecific[type].vert = -1.5;
				fractalspecific[type].width = 3.0;
				break;
			    default:
				fractalspecific[type].hor = -0.25;
				fractalspecific[type].vert = -0.2;
				fractalspecific[type].width = 2.5;
				break;
			    }
			hCtrl = GetDlgItem (hDlg, IDC_DEFAULTPALETTE);
			UseDefaultPalette = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }
