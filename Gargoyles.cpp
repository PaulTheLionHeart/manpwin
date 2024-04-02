/*
   GARGOYLES.CPP a module for Numeric Gargoyles
   Thanks to "The Loom of God" by Clifford A. Pickover pp 266-267.

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

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 16

extern	HWND	GlobalHwnd;				// This is the main windows handle

extern	int	user_data(HWND);
extern	void	(*plot)(WORD, WORD, DWORD);	/* function pointer */
extern	void	genline(int, int, int, int, const DWORD);
extern	int	power(int base, int n);
extern	double	FloatPower(double base, int n);
extern	void	DisplayCircle(int, int, int, DWORD);
extern	void	DisplayFilledCircle(int, int, int, DWORD);

extern	int	time_to_reinit;			// time to start fractal again??

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	double	param[];
extern	WORD	type;				// fractal type
extern	int	subtype;			// A - V
extern	int	curpass, totpasses;
//extern	BYTE	*PalettePtr;			// points to true colour palette

static	double	xscale, yscale;

/**************************************************************************
	Numerica Gargoyles Type Images
***************************************************************************/

int	DoGargoyles(void)

    {
    double  r;						// random number between 0 and 1
    BYTE    *c, *ch;					// pointers to arrays for holding 0 and 1 vakues
    DWORD   colour;
    int	    i, j, sum;
    long    t;
    int	    rule[10];

    t = (long)param[0];
    srand((unsigned)time(NULL));			// randomize things
    for (i = 0; i < 10; i++)
	{
	rule[i] = (t % 10) ? 1 : 0;
	t /= 10;
	}

    if ((c = new BYTE[xdots * ydots]) == NULL)
	return -1;
    if ((ch = new BYTE[xdots * ydots]) == NULL)
	{
	delete [] c;
	return -1;
	}

    for (i = 0; i < ydots; i++)
	for (j = 0; j < xdots; j++)
	    {
	    r = (double)rand()/32767.0;
	    *(c + i * xdots + j) = (r < 0.5) ? 1 : 0;	// seed space with random 0s and 1s
	    }

    for (;;)	// perform simulation based on a twisted majority rules to form gargoyle objects in 2D
	{
	for (i = 1; i < ydots - 1; i++)
	    {
	    if (user_data(GlobalHwnd) == -1)		// user pressed a key?
		return -1;
	    for (j = 1; j < xdots - 1; j++)
		{					// compute the sum of neighbouring cells
		sum = 
		    *(c + (i+1) * xdots + j+1) +
		    *(c + (i-1) * xdots + j-1) +
		    *(c + (i-1) * xdots + j+1) +
		    *(c + (i+1) * xdots + j-1) +
		    *(c + (i+1) * xdots + j+0) +
		    *(c + (i-1) * xdots + j+0) +
		    *(c + (i+0) * xdots + j+1) +
		    *(c + (i+0) * xdots + j-1) +
		    *(c + (i+0) * xdots + j+0);

		*(ch + i * xdots + j) = (rule[sum]) ? 1 : 0;
//		*(ch + i * xdots + j) = (sum >= 6 || sum == 4) ? 1 : 0;	// notice the "twist" in the rules that destabilises creachure boundaries
		colour = threshold * sum / 9;		// 9 is the max possible value of sum
		plot (j, i, colour);
		}
	    }
	memcpy(c, ch, xdots * ydots);
	}
    delete [] c;
    delete [] ch;
    return 0;
    }

