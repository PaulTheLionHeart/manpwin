/*
    PLOT.CPP a module to interface screen routines with pixels[].
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Plot.h"

void	CPlot::InitPlot(long thresholdIn, CTrueCol *TrueColIn, double *wpixelsIn, int xdotsIn, int ydotsIn, int widthIn, int heightIn, int bits_per_pixelIn, CDib *DibIn, int flagsIn)
    {
    threshold = thresholdIn;			// maximun iterations
    TrueCol = TrueColIn;			// palette info
    wpixels = wpixelsIn;			// floating point iterations for each pixels
    xdots = xdotsIn;
    ydots = ydotsIn; 
    height = heightIn; 
    width = widthIn;
    bits_per_pixel = bits_per_pixelIn;
    flags = flagsIn;

    Dib = DibIn;				// Device Independent Bitmap
    }

/**************************************************************************
	Get values of r, g, b
**************************************************************************/

void	CPlot::GetRGB(DWORD colour, BYTE *rgb)

    {
    DWORD	j, c;

    c = (colour < MAXTHRESHOLD) ? colour : (colour % MAXTHRESHOLD);			// threshold can be bigger than palette
    j = c + c + c;
    *(rgb + 0) = *(TrueCol->PalettePtr + j + 2);
    *(rgb + 1) = *(TrueCol->PalettePtr + j + 1);
    *(rgb + 2) = *(TrueCol->PalettePtr + j + 0);
    }

/**************************************************************************
	Plot correct pixel according to count, i, j
**************************************************************************/

void	CPlot::PlotPoint(WORD x, WORD y, DWORD colour)

    {
    DWORD	i;
    DWORD	local_width;
    DWORD	address;

    if (x < 0 || x >= width || y < 0 || y >= height)
	return;
    if (flags & USEPALETTE)
 	{
	// first do screen
	local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
	i = ((DWORD) (height - 1 - y) * (DWORD) (local_width + 3 - ((local_width - 1) % 4)) + (DWORD)(x * 3));
	if (colour == (DWORD)threshold)			// handle inside colour
	    {
	    *(Dib->DibPixels + i + 0) = (BYTE)TrueCol->InsideBlue;
	    *(Dib->DibPixels + i + 1) = (BYTE)TrueCol->InsideGreen;
	    *(Dib->DibPixels + i + 2) = (BYTE)TrueCol->InsideRed;
	    }
	else
	    {
	    BYTE	rgb[3];
	    GetRGB(colour, rgb);
	    if (Dib->DibPixels)
		memcpy(Dib->DibPixels + i, rgb, 3);
	    }
//	memcpy(Dib.DibPixels + i, GetRGB(colour), 3);
						    // Now do reference
	i = ((DWORD)y * (DWORD)width) + (DWORD)x;

	if (flags & USEWPIXELS)
//	if (calcmode != 'F' && type != PERTURBATION && type != SLOPEFORWARDDIFF)		// these cases get wpixels directly from forward differencing algorithms
	    {
	    if (x >= 0 && x < width && y >= 0 && y < ydots)
		*(wpixels + i) = (double)colour;
	    }
	}
    else
	{
	if (x < width && y < height)
	    {
	    address = (WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel) * y);
	    memcpy(Dib->DibPixels + address + x * 3L, &colour, 3);
	    }
	}
    }

/**************************************************************************
	Plot correct pixel according to count, i, j
**************************************************************************/

void	CPlot::OutRGBpoint(WORD x, WORD y, RGBTRIPLE colour)

    {
    DWORD	i;
    DWORD	local_width;
    // first do screen
    if (x < 0 || x >= Dib->DibWidth || y < 0 || y >= Dib->DibHeight)
	return;
    local_width = WIDTHBYTES((DWORD)xdots * (DWORD)Dib->BitsPerPixel);
    i = ((DWORD)(Dib->DibHeight - 1 - y) * (DWORD)(local_width + 3 - ((local_width - 1) % 4)) + (DWORD)(x * 3));
    if (Dib->DibPixels)
	memcpy(Dib->DibPixels + i, &colour, 3);
    }

/**************************************************************************
	Get the colour of a point
**************************************************************************/

DWORD	CPlot::GetColour(WORD x, WORD y)

    {
    DWORD	i;

    if (x < 0 || x >= width || y < 0 || y >= height)
	return 0L;
    i = ((long)y * (long)xdots) + (long)x;

    if (x >= 0 && x < xdots/* - 1*/ && y >= 0 && y < ydots - 1)
	{
	return (((DWORD)(*(wpixels + i)) & 0x7fffffff));
	}

    return 0L;
    }

/**************************************************************************
	Do correct put_line
**************************************************************************/

void	CPlot::OutputLine(WORD x0, WORD line, WORD length, DWORD *buffer)

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
	*(wpixels + k++) = (double) (*(buffer + j));
	if ((*(buffer + j)) >= (DWORD)threshold)		// handle inside colour
	    {
	    *(Dib->DibPixels + i + 0) = (BYTE)TrueCol->InsideBlue;
	    *(Dib->DibPixels + i + 1) = (BYTE)TrueCol->InsideGreen;
	    *(Dib->DibPixels + i + 2) = (BYTE)TrueCol->InsideRed;
	    }
	else
	    {
	    BYTE	rgb[3];
	    GetRGB((DWORD)(*(buffer + j)), rgb);
	    if (Dib->DibPixels)
		memcpy(Dib->DibPixels + i, rgb, 3);
	    }
//	memcpy(Dib.DibPixels + i, GetRGB((DWORD) (*(buffer + j))), 3);
	i += 3L;
	}
    }

/**************************************************************************
	Refresh Screen
**************************************************************************/

void	CPlot::RefreshScreen(void)

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
	    if (n >= (DWORD)(threshold) && TrueCol->inside_colour > 0)		// inside colour
		{
		*(Dib->DibPixels + i + m * 3 + 0) = (BYTE)TrueCol->InsideBlue;
		*(Dib->DibPixels + i + m * 3 + 1) = (BYTE)TrueCol->InsideGreen;
		*(Dib->DibPixels + i + m * 3 + 2) = (BYTE)TrueCol->InsideRed;
		}
	    else
		{
		n = ((DWORD) (*(wpixels + k + m)) % MAXTHRESHOLD) * 3;
		*(Dib->DibPixels + i + m * 3 + 0) = *(TrueCol->PalettePtr + n + 2);
		*(Dib->DibPixels + i + m * 3 + 1) = *(TrueCol->PalettePtr + n + 1);
		*(Dib->DibPixels + i + m * 3 + 2) = *(TrueCol->PalettePtr + n + 0);
		}
	    }
        }
    }

/*---------------------------------------------------------------------
	Display a Circle (based on Michener's Algorithm)
-------------------------------------------------------------------*/

void	CPlot::DoPlot(int x, int y, DWORD colour)
    {
    if (x >= 0 && y >= 0 && x < xdots && y < ydots)				// allow underflow testing
	PlotPoint(x, y, colour);
    }

void	CPlot::DisplayCircle(int centrex, int centrey, int radius, DWORD colour)

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

/*---------------------------------------------------------------------
	Display a filled Circle
-------------------------------------------------------------------*/

void	CPlot::DisplayFilledCircle(int centrex, int centrey, int radius, DWORD colour)

    {
    int x, y;

    for (x = -radius; x <= radius; x++)
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

/*-----------------------------------------
	Sexy 3D Circle
-----------------------------------------*/

int	CPlot::Display3DCircle(CDib *Dib3D, int centrex, int centrey, int radius, BYTE *colour)

    {
    int		i, x, y;
    DWORD	address;
    BYTE	temp[6];

    for (x = -radius; x <= radius; x++)
	{
	if (x + centrex < 0 || x + centrex >= Dib3D->DibWidth)
	    continue;
	int height = (int)sqrt((double)(radius * radius) - (double)(x * x));

	for (y = -height; y <= height; y++)
	    {
	    if (y + centrey < 0 || y + centrey >= Dib3D->DibHeight)
		continue;
	    address = (WIDTHBYTES((DWORD)Dib3D->DibWidth * (DWORD)bits_per_pixel) * (Dib3D->DibHeight - 1 - y - centrey));
	    memcpy(temp + 3, colour, 3);
	    if (radius != 0)
		{
		for (i = 0; i < 3; i++)
		    temp[i] = (BYTE)((((DWORD)temp[i + 3]) * (DWORD)(x + radius)) / (long)(radius * 2));				// make look 3D
		memcpy(Dib3D->DibPixels + address + (x + centrex) * 3L, temp, 3);
		}
	    }
	}
    return 0;
    }

/**************************************************************************
	Point plotting for filter
***************************************************************************/

void	CPlot::FilterPoint(WORD x, WORD y, DWORD colour, RGBTRIPLE *FilterRGB)	// 

    {
    DWORD	address;
    long	i;

    if (x < xdots && y < ydots)
	{
	address = (WIDTHBYTES((DWORD)xdots * (DWORD)Dib->BitsPerPixel) * (ydots - y - 1));
	memcpy(Dib->DibPixels + address + x * 3L, FilterRGB, 3);
	i = ((long)y * (long)xdots) + (long)x;
	if (x >= 0 && x < xdots && y >= 0 && y < ydots)
	    *(wpixels + i) = (double)colour;
	}
    }

