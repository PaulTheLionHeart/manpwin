// Nebulabrot / Buddhabrot generator.
// Brought to you by Wikipedia...
// Written by User:Evercat
//
// Released under the GNU Free Documentation License
// or the GNU Public License, whichever you prefer:
// November 23, 2004
//
// This code is lame and confusing. I apologise.
// As I like to point out, my C is self-taught.
//
// Note: some folk mention possible improvements on the talk page:
// http://en.wikipedia.org/wiki/User_talk:Evercat/Buddhabrot.c
 
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Dib.h"
#include "ManpWIN.h"
#include "Manp.h"
#include "Complex.h"
#include "Plot.h"

////////////////////////////////////////////////////////////////////////////////////
 
#define SOURCE_COLUMNS (xdots * 10)			// These lines control the number of source values iterated.
#define SOURCE_ROWS (ydots * 10)			// Try WIDTH and HEIGHT * 10
 
#define RED_MULTIPLIER 0.09				// Multiply the number of hits in a pixel by these values
#define GREEN_MULTIPLIER 0.11				// (needs to be lower the greater the number of source
#define BLUE_MULTIPLIER 0.18				// values and iterations, else image will be too bright)
 
#define COLOUR_OFFSET -230				// This value is added to the number of hits before a pixel is drawn.

#define OPTIMISE		// Don't bother iterating some values obviously in the set.
 
#define reduce(foo) (foo)	// Macro to reduce colors, can use sqrt(n), log(n), etc, or just (n)
 
////////////////////////////////////////////////////////////////////////////////////
 
void	drawpath(double x, double y, double target_startx, double target_starty, double pixel_width);
int	drawbmp (void);
 
extern	long	threshold;
extern	double	mandel_width;		// width of display 
extern	double	hor;			// horizontal address 
extern	double	vert;			// vertical address 
extern	double	ScreenRatio;		// ratio of width / height for the screen
extern	int	xdots, ydots, height, bits_per_pixel;

extern	CPlot	Plot;		// image plotting routines 

extern	int	curpass, totpasses;
extern	CDib	Dib;			// Device Independent Bitmap
extern	HWND	GlobalHwnd;		// This is the main windows handle
extern	Complex	z, q, j;
extern	double	param[];
extern	long	threshold;

extern	int	user_data(HWND);

////////////////////////////////////////////////////////////////////////////////////
 
static	long	*redcount;
static	long	*greencount;
static	long	*bluecount;
 
static	double	red_multiplier = RED_MULTIPLIER;
static	double	green_multiplier = GREEN_MULTIPLIER;
static	double	blue_multiplier = BLUE_MULTIPLIER;
static	double	Brightness;

/**************************************************************************
	Plot pixel colour, at co-ordinates i, j
**************************************************************************/

void	InitMultipliers(void)
    {
    red_multiplier = RED_MULTIPLIER;
    green_multiplier = GREEN_MULTIPLIER;
    blue_multiplier = BLUE_MULTIPLIER;
    }

/**************************************************************************
	Plot pixel colour, at co-ordinates i, j
**************************************************************************/

void	RGBPoint(WORD x, WORD y, BYTE r, BYTE g, BYTE b)

    {
    long	i;
    long	local_width;

    if (x < 0 || x >= Dib.DibWidth || y < 0 || y >= Dib.DibHeight)
	return;
    local_width = WIDTHBYTES((DWORD)Dib.DibWidth * (DWORD)bits_per_pixel);
    i = ((long) (height - 1 - y) * (long) (local_width + 3 - ((local_width - 1) % 4)) + (long)(x * 3));
    *(Dib.DibPixels + i + 0) = b;
    *(Dib.DibPixels + i + 1) = g;
    *(Dib.DibPixels + i + 2) = r;
    }

/**************************************************************************
	The BuddhaBrot engine
**************************************************************************/

////////////////////////////////////////////////////////////////////////////////////
 
int DoBuddhaBrot (void) 
    {
    int	    n;
    double  x, y;				// Source coordinates of particle being tracked
    int	    source_column, source_row;		// Source grid location
    double  r, s, nextr, nexts;			// Values as particle is iterated through the Mandelbrot function
    double  x_jump, y_jump;			// Distances between particles in the source grid
    double  target_startx, target_starty;	// Top-left coordinates of drawn area
    double  target_endx, target_endy;		// Bottom-right coordinates of drawn area
    double  pixel_width;			// Actual distance represented by a pixel in the drawn area
 
    Brightness = param[0];
    threshold = (long)param[1];
    red_multiplier *= Brightness;
    green_multiplier *= Brightness;
    blue_multiplier *= Brightness;
    redcount = new long [xdots*ydots];
    if (redcount == NULL)
	throw "Can't allocate array memory.";	// PHD 2014-11-10 C++ error handling
    greencount = new long [xdots*ydots];
    if (greencount == NULL)
	throw "Can't allocate array memory.";	// PHD 2014-11-10 C++ error handling
    bluecount = new long [xdots*ydots];
    if (bluecount == NULL)
	throw "Can't allocate array memory.";	// PHD 2014-11-10 C++ error handling

    memset(redcount, 0, xdots*ydots*sizeof(long));
    memset(greencount, 0, xdots*ydots*sizeof(long));
    memset(bluecount, 0, xdots*ydots*sizeof(long));
 
    totpasses = 10;

    target_startx = hor;
    target_endx = hor + mandel_width * ScreenRatio;

    target_starty = vert;
    target_endy = vert + mandel_width;

    pixel_width = (target_endx - target_startx) / xdots;
 
    x_jump = (mandel_width * ScreenRatio) / SOURCE_COLUMNS;
    y_jump = mandel_width / SOURCE_ROWS;

	// Main bit...
 
    y = vert + mandel_width;
    for (source_row = SOURCE_ROWS - 1; source_row >= 0; source_row--, y -= y_jump)
	{
	curpass = totpasses - (source_row * totpasses / SOURCE_ROWS);
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	x = hor;
	for (source_column = 0; source_column < SOURCE_COLUMNS; source_column++, x += x_jump)
	    {
#ifdef OPTIMISE			// exclude the main body of the manelbrot
	    if 
		(
		   (x >  -1.2 && x <=  -1.1 && y >  -0.1 && y < 0.1)
		|| (x >  -1.1 && x <=  -0.9 && y >  -0.2 && y < 0.2)
		|| (x >  -0.9 && x <=  -0.8 && y >  -0.1 && y < 0.1)
		|| (x > -0.69 && x <= -0.61 && y >  -0.2 && y < 0.2)
		|| (x > -0.61 && x <=  -0.5 && y > -0.37 && y < 0.37)
		|| (x >  -0.5 && x <= -0.39 && y > -0.48 && y < 0.48)
		|| (x > -0.39 && x <=  0.14 && y > -0.55 && y < 0.55)
		|| (x >  0.14 && x <   0.29 && y > -0.42 && y < -0.07)
		|| (x >  0.14 && x <   0.29 && y >  0.07 && y < 0.42)
		) continue;
#endif
	    r = 0; s = 0;
	    for (n = 0; n <= threshold; n++)
		{
		nextr = ((r * r) - (s * s)) + x;
		nexts = (2 * r * s) + y;
		r = nextr;
		s = nexts;
		if (n == threshold)
		    {
		    // drawpath(x, y, target_startx, target_starty, pixel_width);
		    break;
		    } 
		else if ((r * r) + (s * s) > 4) 
		    {
		    Plot.PlotPoint((WORD)source_column/10, (WORD)(ydots - source_row/10 + 1), (DWORD)n);
		    drawpath(x, y, target_startx, target_starty, pixel_width);
		    break;
		    }
		}
	    }
	}
 
    drawbmp();
 
    delete [] redcount;			// give back array memory
    delete [] greencount;
    delete [] bluecount;
    return 0;
    }
 
 /**************************************************************************
	Calculate the array of pixel values
**************************************************************************/

void drawpath (double x, double y, double target_startx, double target_starty, double pixel_width)
    {
    double r, s, nextr, nexts;
    int n;
    int xpixel, ypixel;

    r = 0; s = 0;
    for (n = 0; n <= threshold; n++)
	{
	nextr = ((r * r) - (s * s)) + x;
	nexts = (2 * r * s) + y;
	r = nextr;
	s = nexts;
 
	if ((r * r) + (s * s) > 4) return;

	xpixel = (int)((r - target_startx) / pixel_width);
	ypixel = (int)((s - target_starty) / pixel_width);
	if (xpixel > 0 && xpixel < xdots && ypixel > 0 && ypixel < ydots)
	    {
	    if (n <= threshold)
		{
		redcount[ypixel*xdots+xpixel] += 1;
		greencount[ypixel*xdots+xpixel] += 1;
		bluecount[ypixel*xdots+xpixel] += 1;
		}
	    }
	}
    return;
    }

/**************************************************************************
	Normalise and copy array into the Dib
**************************************************************************/

int drawbmp (void) 
    {
    int x, y;
    int red, green, blue;
 
    for (y = 0; y < ydots; y++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	for (x = 0; x < xdots; x++)
	    {
	    red = (int)(reduce(redcount[(ydots - y - 1)*xdots+x] + COLOUR_OFFSET) * red_multiplier);
	    green = (int)(reduce(greencount[(ydots - y - 1)*xdots+x] + COLOUR_OFFSET) * green_multiplier);
	    blue = (int)(reduce(bluecount[(ydots - y - 1)*xdots+x] + COLOUR_OFFSET) * blue_multiplier);

	    if (red > 255) red = 255; if (red < 0) red = 0;
	    if (green > 255) green = 255; if (green < 0) green = 0;
	    if (blue > 255) blue = 255; if (blue < 0) blue = 0;

	    RGBPoint((WORD)x, (WORD)y, (BYTE)red, (BYTE)green, (BYTE)blue);
	    }
	}
    return 0;
    }


