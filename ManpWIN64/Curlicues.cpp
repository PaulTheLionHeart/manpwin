/*
   CURLICUES.CPP a module for Fractals by Dr. M. Ecker
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
//#include	"menu.h"
#include	"anim.h"
#include	"polygon.h"
#include	"colour.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 16

extern	HWND	GlobalHwnd;				// This is the main windows handle

extern	int	user_data(HWND);
extern	void(*plot)(WORD, WORD, DWORD);

extern	long	threshold;
extern	double	mandel_width;			// width of display
extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	double	param[];
extern	int	curpass, totpasses;

static	double	xscale, yscale;
static	int	Passes = 1800000;

/**************************************************************************
	Curlicues Fractal Type Images
***************************************************************************/

int	DoCurlicues(void)

    {
    int	    i;
    double  d, skew; 
    double  x, y;
    int	    x1, y1;
    double  t, temp;
    DWORD   colour;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    Passes = (int)param[0];
    d = param[1];
    skew = param[2];
    totpasses = 20;
    x = 0;
    y = 0;
    for (i = 1; i < Passes; i++)
	{
	curpass = (int)((double)i * (double)totpasses / (double)Passes);
	colour = threshold * (DWORD)i / Passes;
	temp = log((double)i) * d;
	t = exp(temp);
	x += sin(d * t);
	y += cos(d * t) + skew * sin(d * t);
	if (user_data(GlobalHwnd) == -1)		// user pressed a key?
	    return -1;
	x1 = (int)((x - hor) * xscale);
	y1 = (int)((vert + mandel_width - y) * yscale);
	plot(x1, y1, colour);
	}
    return 0;
    }

