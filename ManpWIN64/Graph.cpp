/*
    GRAPH.CPP a module to interface screen routines with pixels[].
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <windows.h>
#include "Dib.h"
#include "manpwin.h"
#include "manp.h"
#include "colour.h"

extern	long	threshold;			// maximun iterations
extern	CTrueCol    TrueCol;			// palette info
extern	DWORD 	*wpixels;			// the device-independent bitmap  pixels

extern	int	time_to_zoom;			// time to zoom in or out?
extern	int	time_to_restart;		// time to restart?
extern	int	time_to_reinit;			// time to reinitialize?
extern	int	time_to_quit;			// time to quit?
extern	int	time_to_load;			// time to load file?
extern	BOOL	bTrack;				// TRUE if user is selecting a region

extern	CDib	Dib;				// Device Independent Bitmap

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;			// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************
	Get values of r, g, b
**************************************************************************/

BYTE	*GetRGB(DWORD colour)

    {
    DWORD	j, c;
    static	BYTE	rgb[3];

    c = (colour < MAXTHRESHOLD) ? colour : (colour % MAXTHRESHOLD);			// threshold can be bigger than palette
    j = c + c + c;
    *(rgb + 0) = *(TrueCol.PalettePtr + j + 2);
    *(rgb + 1) = *(TrueCol.PalettePtr + j + 1);
    *(rgb + 2) = *(TrueCol.PalettePtr + j + 0);
    return (rgb);
    }

/**************************************************************************
	Plot correct pixel according to count, i, j
**************************************************************************/

void	outpoint(WORD x, WORD y, DWORD colour)

    {
    DWORD	i;
//    time_t	this_time;
    DWORD	local_width;
//    int		PixelUpdate;

/*
    if (BigNumFlag)
	PixelUpdate = 1;
    else if (type == FIBONACCI)
	PixelUpdate = 10;
    else if (type == OSCILLATORS)
	PixelUpdate = 10;
    if (colour > (DWORD)threshold)			// don't write past end of palette
	if (decomp <= 0)				// for small thresholds, we can still have higher decomp levels
	    colour = (DWORD)threshold;
							// check the time every PixelUpdate pixels
    if (++pixelsout > PixelUpdate) 
	{
	pixelsout = 0;
	time(&this_time);
	if ((this_time > (last_time + update_time) || type == FIBONACCI || type == FOURIER) && !bTrack)
	    {
	    InvalidateRect(PixelHwnd, &r, FALSE);
	    last_time = this_time;
	    if (!RunAnimation)
		StatusBarInfo(INCOMPLETE, "");		// display status bar
	    } 
	}
*/


//    if (y == 0 && y % 100 == 0)
//	{
//	InvalidateRect(GlobalHwnd, NULL, FALSE);
//	Sleep(1);
//	}


						    // first do screen
    if (x < 0 || x >= width || y < 0 || y >= height)
	return;
    local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
    i = ((DWORD) (height - 1 - y) * (DWORD) (local_width + 3 - ((local_width - 1) % 4)) + (DWORD)(x * 3));
    if (colour == (DWORD)threshold)			// handle inside colour
	{
	*(Dib.DibPixels + i + 0) = (BYTE)TrueCol.InsideBlue;
	*(Dib.DibPixels + i + 1) = (BYTE)TrueCol.InsideGreen;
	*(Dib.DibPixels + i + 2) = (BYTE)TrueCol.InsideRed;
	}
    else
	memcpy(Dib.DibPixels + i, GetRGB(colour), 3);
						    // Now do reference
    i = ((DWORD)y * (DWORD)width) + (DWORD) x;
    if (x >= 0 && x < xdots - 1 && y >= 0 && y < ydots - 1)
	*(wpixels + i) = (DWORD) colour;
    }

/**************************************************************************
	Get the colour of a point
**************************************************************************/

DWORD	ingetcolor(WORD x, WORD y)

    {
    DWORD	i;
                
    if (x < 0 || x >= width || y < 0 || y >= height)
	return 0L;    
    i = ((long) y * (long) xdots) + (long) x;

    if (x >= 0 && x < xdots/* - 1*/ && y >= 0 && y < ydots - 1)
	{
	return ((DWORD)((*(wpixels + i)) & 0x7fffffff));
	}

    return 0L;    
    }

/**************************************************************************
	Do correct put_line
**************************************************************************/

void	output_line(WORD x0, WORD line, WORD length, DWORD *buffer)

    {
    DWORD	i, k;
    DWORD	local_width;
    WORD	j;
    if (x0 < 0 || x0 >= width || line < 0 || line >= height - 1 || (x0 + length) < 0 || (x0 + length) > width)
	return;

    local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
    i = ((long) (height - 1 - line) * (long) (local_width + 3 - ((local_width - 1) % 4)) + (long)(x0 * 3));
    k = ((long) line * (long) width) + (long) x0;
    for (j = 0; j <= length; ++j) 
	{
	*(wpixels + k++) = (DWORD) (*(buffer + j));
	if ((*(buffer + j)) >= (DWORD)threshold)		// handle inside colour
	    {
	    *(Dib.DibPixels + i + 0) = (BYTE)TrueCol.InsideBlue;
	    *(Dib.DibPixels + i + 1) = (BYTE)TrueCol.InsideGreen;
	    *(Dib.DibPixels + i + 2) = (BYTE)TrueCol.InsideRed;
	    }
	else
	    memcpy(Dib.DibPixels + i, GetRGB((DWORD) (*(buffer + j))), 3);
	i += 3L;
	}
    }

/**************************************************************************
	Plot a vertical line of same colour on 256 colour graphics card
**************************************************************************/

void	verline(WORD x0, WORD y0, WORD y1, DWORD colour)

    {
    WORD 	length, ylo, yhi;
    DWORD	i;

    ylo = (y0 < y1) ? y0: y1;
    yhi = (y0 > y1) ? y0: y1;
    length = yhi - ylo;

    for (i = 0; i <= length; ++i)
	(*plot)((WORD)(ylo + i), x0, colour);
    return;
    }

/**************************************************************************
	Refresh Screen
**************************************************************************/

void	RefreshScreen(void)

    {
    DWORD	local_width;
    DWORD	k, i;
    WORD	j, m;
    DWORD	n;

    for (j = 0; j < height - 1; ++j)
	{
	local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
	i = ((long) (height - 1 - j) * (long) (local_width + 3 - ((local_width - 1) % 4)));
	k = ((long) j * (long) width);
	for (m = 0L; m < width; ++m)
	    {
	    n = ((DWORD) (*(wpixels + k + m)));
	    if (n >= (DWORD)(threshold) && TrueCol.inside_colour > 0)		// inside colour
		{
		*(Dib.DibPixels + i + m * 3 + 0) = (BYTE)TrueCol.InsideBlue;
		*(Dib.DibPixels + i + m * 3 + 1) = (BYTE)TrueCol.InsideGreen;
		*(Dib.DibPixels + i + m * 3 + 2) = (BYTE)TrueCol.InsideRed;
		}
	    else
		{
		n = ((DWORD) (*(wpixels + k + m)) % MAXTHRESHOLD) * 3;
		*(Dib.DibPixels + i + m * 3 + 0) = *(TrueCol.PalettePtr + n + 2);
		*(Dib.DibPixels + i + m * 3 + 1) = *(TrueCol.PalettePtr + n + 1);
		*(Dib.DibPixels + i + m * 3 + 2) = *(TrueCol.PalettePtr + n + 0);
		}
	    }
        }
    }

/**************************************************************************
	Get a key or mouse click and process
**************************************************************************/

int user_data(HWND hwnd)

    {
    MSG msg;

    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}

    if (!bTrack && time_to_reinit + time_to_restart + time_to_quit + time_to_zoom + time_to_load)
	return(-1);					  // don't do this if mouse-button is down
    return 0;
    }
