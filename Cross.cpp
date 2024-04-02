/*
   CROSS.CPP a module for Cross Fractals
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
#include	"OscProcess.h"
#include	"Plot.h"

//#define MIN(a,b) (a <= b ? a : b)
//#define MAX(a,b) (a >= b ? a : b)
//#define MAXPOINTS 48

extern	HWND	GlobalHwnd;				// This is the main windows handle

extern	int	user_data(HWND);
extern	void	DisplayFractal(HWND);

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	WORD	type;				// fractal type
extern	int	subtype;			// 0 - 8
extern	int	curpass, totpasses;
//extern	BYTE	*PalettePtr;			// points to true colour palette
extern	double	param[];
extern	int	curpass, totpasses;
extern	int	CoordSystem;
extern	int	xdots, ydots;

static	double	xscale, yscale;
static	double  a = 2.23, b = 0.76, c = 0.3;
static	double	Div[24] = {3.0, 3.0, 1.3, 1.423, 1.3, 1.3, 1.3, 1.2, 1.3, 1.0, 1.3};

extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts

extern	CTrueCol    TrueCol;			// palette info
extern	CDib    Dib;				// Device Independent Bitmap
extern	COscProcess	OscProcess;
extern	CPlot	Plot;		// image plotting routines 

/**************************************************************************
	Cross Roads Fractal Type Images
***************************************************************************/

int	DoCrossRoads(void)

    {
    int		j, k;
    double	x, y, xx, xold, yy, temp, exponent; 
    long	i, num;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    subtype = 0;
//    x = 0.365;
//    y = -0.129;
    x = 1;
    y = 1;

    a = param[0];
    b = param[1];
    c = param[2];
    exponent = param[3];
    subtype = (int)param[4];
    num = (int)param[5];
    totpasses = 10;
    DisplayFractal(GlobalHwnd);

    for(i = 0; i < num; i++)
	{
	if (user_data(GlobalHwnd) == -1)		// user pressed a key?
	    return -1;
	curpass = i * 10 / num;
	switch (subtype)
	    {
	    case 0:				// cross roads
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * x + c);
		else
		    temp = pow(fabs(b * x + c), exponent);
		xx = y - (x / fabs(x)) / temp;
		yy = a - x;
		break;
	    case 1:				// Almost Lyapunov
		xold = x;
		yy = y + x;
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * xold + c);
		else
		    temp = pow(fabs(b * xold + c), exponent);
		xx = yy - (xold / fabs(xold)) / temp;
		yy = a - xold;
		break;
	    case 2:				// Clockwork
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(y)) / temp;
		yy = a + c - x;
		break;
	    case 3:				// Convergence of henons
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(y)) / temp;
		yy = a + c - x - y;
		break;
	    case 4:				// Leaking
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * x + c);
		else
		    temp = pow(fabs(b * x + c), exponent);
		xx = y - (x + y / fabs(x)) / temp;
		yy = a - x;
		break;
	    case 5:				// Lord of the Rings
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(y)) / temp;
		yy = a + c - x - y;
		break;
	    case 6:				// Poltergiest
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + c);
		else
		    temp = pow(fabs(b * y + c), exponent);
		xx = y - (x + y / fabs(x)) / temp;
		yy = a - x;
		break;
	    case 7:				// Resonance
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(y)) / temp;
		yy = a + c - x;
		break;
	    case 8:				// Pit
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(x)) / temp;
		yy = a - x;
		break;
	    case 9:				// Conflict
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y) + x;
		else
		    temp = pow((fabs(b * y) + x), exponent);
		xx = y - (x - y / fabs(y)) / temp;
		yy = a + c - x;
		break;
	    case 10:				// Confusion
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(x)) / temp;
		yy = a + c - x;
		break;


	    default:				// cross roads
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * x + c);
		else
		    temp = pow(fabs(b * x + c), exponent);
		xx = y - (x / fabs(x)) / temp;
		yy = a - x;
		subtype = 0;
		break;
	    }
	x = xx / Div[subtype];
	y = yy;
	OscProcess.ChangeCoordSystem(&x, &y, &temp, x, y, fabs(atan2(y, x)*180.0/PI), CoordSystem);
	if (subtype == 9)			// Conflict
	    {
	    j = (int)((x - hor) * xscale * 1000) % xdots;
	    k = (int)((vert + mandel_width - y) * yscale * 1000) % ydots;
	    }
	else
	    {
	    j = (int)((x - hor) * xscale);
	    k = (int)((vert + mandel_width - y) * yscale);
	    }
	Plot.PlotPoint(j, k, (long)fabs(atan2(y, x)*180.0/PI));
	}
    return 0;
    }

