/*
   CHUAFLIGHT.CPP a module Automatic Chua Flight Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	<time.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"menu.h"
#include	"anim.h"

extern	HWND	GlobalHwnd;			// This is the main windows handle

extern	int	user_data(HWND);
extern	void	genline(int, int, int, int, const DWORD);

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	double	param[];
extern	WORD	type;				// fractal type
extern	int	subtype;			// A - E
extern	int	curpass, totpasses;

static	double	xscale, yscale;
static	double	scale = 1;
static	int	iterations = 5000;

/**************************************************************************
	Automatic Chua Flight Fractal Type Images
***************************************************************************/

int	DoChuaFlight(void)

    {
    double	a, b, c;
    int		f, g;

    int		i;
    double	x, y, x1, y1, z;
    int		ix, iy;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    iterations = (int)param[0];
//    scale = (int)param[1];
    if (param[1] == 0)
	{
	srand((unsigned)time(NULL));					// Seed the random-number generator with current time 
	a = (double)rand()/(RAND_MAX);
	b = (double)rand()/(RAND_MAX);
	c = (double)rand()/(RAND_MAX);
	}
    else
	{
	a = param[1];
	b = param[2];
	c = param[3];
	}
    x = c * 20; y = b * 185; z = a; //REM THESE ARE THE INITIAL VALUES 'X' GOVERN THE SIZE OF THE IMAGES, here we re-use the initial variables

// REM THESE TWO AUXILIARY VARIABLES BELOW 'f' AND 'g' WERE THE 'BIG DEAL' THAT WAS DISCOVERED BY A PROGRAMER IN ANP
// REM THEY MADE POSSIBLE TO USE LINE ()-() INSTEAD OF THE COMPLEX INSTRUCTION LINE - STEP()

    f = xdots/2;
    g = ydots/2;
    totpasses = 10;
    for (i = 0; i < iterations * 10; i++)
	{
	if (user_data(GlobalHwnd) == -1)		// user pressed a key?
	    return -1;
	curpass = i / iterations;
	x1 = a * (y - x) + a * tan(x);			// three Chua's equations
	y1 = x1 - y + (z / 10);
	z = -(b * y1 - c / z);

	if (param[1] == 0)
	    {
	    genline((int)f, (int)g, (int)(x1 * scale + f), (int)(y1 * scale + g), ((DWORD)fabs(x1 * y1)) % threshold);
	    f = f + (int)(x1 * scale);				// new position of point
	    g = g + (int)(y1 * scale);
	    }
	else
	    {
	    ix = (int)((x - hor) * xscale);
	    iy = (int)((vert + mandel_width - y) * yscale);
	    f = (int)((x1 - hor) * xscale);
	    g = (int)((vert + mandel_width - y1) * yscale);
	    genline(f, g, ix, iy, ((DWORD)fabs(x * y)) % threshold);
	    }

	x = x1;
	y = y1;
	}

    return 0;
    }

