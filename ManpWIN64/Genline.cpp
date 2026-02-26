/*
    GENLINE.CPP -- high level interface to video routines.
    Global line routines.
    
    Written in Visual 'C++' by Paul de Leeuw 23/05/92.
    VESA P. de Leeuw 22/12/94

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<stdlib.h>
#include	"manp.h"
#include	"Plot.h"

/**************************************************************************
	Draw a line from raw x0,y0 to x1,y1
**************************************************************************/

void	CPlot::genline(int x0, int y0, int x1, int y1, DWORD col)

    {
    register int x, y;
    long	xstep, ystep, xcum = 0L, ycum = 0L;
    int	dx, dy;

								// don't bother to display lines outside of screen
    if (x0 < 0 && x1 < 0)		return;
    if (x0 >= xdots && x1 >= xdots)	return;
    if (y0 < 0 && y1 < 0)		return;
    if (y0 >= ydots && y1 >= ydots)	return;

    dx = (int) x1 - (int) x0;
    dy = (int) y1 - (int) y0;
    if (dx == 0)						// vertical line
	{
	dy = (dy > 0) ? 1 : -1;
	for (y = y0; y != y1; y += dy)
	    if (x0 >= 0 && y >= 0 && x0 < xdots && y < ydots)
		PlotPoint(x0, (WORD)y, col);
	}
    else if (y0 == y1)						// horizontal line
	{
	dx = (dx > 0) ? 1 : -1;
	for (x = x0; x != x1; x += dx)
	    if (x >= 0 && y0 >= 0 && x < xdots && y0 < ydots)
		PlotPoint((WORD)x, y0, col);
	}
    else							// sloping line
	{ 
	if (abs(dx) >= abs(dy))					// plot along x axis
	    {
	    ystep = (long)((float) (dy << 16) / (float) dx);
	    if (dy < 0)						// y goes to left
		{
		if (ystep > 0L)
		    ystep = - ystep;				// adjust for wrong sign
		}
	    else if (ystep < 0L)				// y goes to right
		ystep = - ystep;
	    dx = (dx > 0) ? 1 : -1;
	    for (x = x0, y = y0; x != x1 && ((dx > 0) ? x < xdots : x >= 0); x += dx)
		{
		if (x >= 0 && y >= 0 && x < xdots && y < ydots)
		    PlotPoint((WORD)x, (WORD)y, col);
		ycum += ystep;					// cumulative motion along y axis
		y = y0 + (int) (ycum >> 16);
		}
	    }
	else							// plot along y axis
	    {
	    xstep = (long)((float)(dx  << 16) / (float) dy);
	    if (dx < 0)
		{
		if (xstep > 0L)
		    xstep = - xstep;
		}
	    else if (xstep < 0)
		xstep = - xstep;
	    dy = (dy > 0) ? 1 : -1;
	    for (y = y0, x = x0; y != y1 && ((dy > 0) ? y < ydots : y >= 0); y += dy)
		{
		if (x >= 0 && y >= 0 && x < xdots && y < ydots)
		    PlotPoint((WORD)x, (WORD)y, col);
		xcum += xstep;
		x = x0 + (xcum >> 16);
		}
	    }
	}
    }

