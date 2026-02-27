/*
    PLOT.CPP a module to interface screen routines with pixels[].
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Plot.h"

extern	std::vector<float> wpixels; // global vector

///////////////////////////////////////////////////////////////////////
// Explicitly initialize reference members or pointers to null/defaults
///////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
static inline size_t CalcStrideBytes(int width, int bits_per_pixel)
    {
    // WIDTHBYTES already DWORD-aligns
    return (size_t)WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
    }
#endif

#ifdef _DEBUG
bool CPlot::DebugValidate(const char* where) const
    {
    if (!Dib) {
	OutputDebugStringA(where); OutputDebugStringA(": Dib == null\n");
	return false;
	}

    if (width <= 0 || height <= 0) {
	OutputDebugStringA(where); OutputDebugStringA(": bad dimensions\n");
	return false;
	}

    if (bits_per_pixel != 24) {
	OutputDebugStringA(where); OutputDebugStringA(": only 24bpp supported\n");
	return false;
	}

    const size_t stride = CalcStrideBytes(width, bits_per_pixel);
    const size_t need = stride * (size_t)height;

    if (Dib->DibPixels.size() < need) {
	OutputDebugStringA(where); OutputDebugStringA(": DibPixels too small\n");
	return false;
	}

    if ((flags & USEPALETTE) && !TrueCol) {
	OutputDebugStringA(where); OutputDebugStringA(": TrueCol == null\n");
	return false;
	}

    if ((flags & USEWPIXELS)) {
	if (wpixels.size() < (size_t)width * (size_t)height) {
	    OutputDebugStringA(where); OutputDebugStringA(": wpixels too small\n");
	    return false;
	    }
	}

    return true;
    }
#endif

///////////////////////////////////////////////////////////////////////
// Explicitly initialize reference members or pointers to null/defaults
///////////////////////////////////////////////////////////////////////

CPlot::CPlot()
    : wpixels(::wpixels)  // bind to global by default
    {
    width = height = xdots = ydots = 0;
    TrueCol = nullptr;
    Dib = nullptr;
    }

CPlot::CPlot(std::vector<float>& wp)
    : wpixels(wp)
    {
    width = height = xdots = ydots = 0;
    TrueCol = nullptr;
    Dib = nullptr;
    }

void	CPlot::InitPlot(long thresholdIn, CTrueCol *TrueColIn, std::vector <float> &wpixelsIn, int xdotsIn, int ydotsIn, int widthIn, int heightIn, int bits_per_pixelIn, CDib *DibIn, int flagsIn)
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
    DWORD	c;

    c = (colour < MAXPALETTE) ? colour : (colour % MAXPALETTE);				// threshold can be bigger than palette
    *(rgb + 0) = TrueCol->PalettePtr[c].rgbtRed;
    *(rgb + 1) = TrueCol->PalettePtr[c].rgbtGreen;
    *(rgb + 2) = TrueCol->PalettePtr[c].rgbtBlue;
    }

/**************************************************************************
	Get values of r, g, b
**************************************************************************/

void	CPlot::GetRGB(DWORD colour, RGBTRIPLE *rgb)

    {
    DWORD	c;

    c = (colour < MAXPALETTE) ? colour : (colour % MAXPALETTE);				// threshold can be bigger than palette
    rgb->rgbtBlue = TrueCol->PalettePtr[c].rgbtRed;
    rgb->rgbtGreen = TrueCol->PalettePtr[c].rgbtGreen;
    rgb->rgbtRed = TrueCol->PalettePtr[c].rgbtBlue;

//    *rgb = TrueCol->PalettePtr[c];
    }

/**************************************************************************
	Plot correct pixel according to count, i, j
**************************************************************************/

void	CPlot::PlotPoint(WORD x, WORD y, DWORD colour)

    {
    DWORD	i;
    DWORD	local_width;
    DWORD	address;

    if (x >= Dib->DibWidth || y >= Dib->DibHeight)
	{
	OutputDebugStringA("PlotPoint: co-ordinates out of bounds\n");
	return;
	}
    if (Dib->DibPixels.empty())
	{
	OutputDebugStringA("PlotPoint: Pixels are null\n");
	return;
	}
    if (flags & USEPALETTE)
 	{
	// first do screen
	local_width = WIDTHBYTES((DWORD)Dib->DibWidth * (DWORD)bits_per_pixel);
	i = ((DWORD) (Dib->DibHeight - 1 - y) * (DWORD) (local_width + 3 - ((local_width - 1) % 4)) + (DWORD)(x * 3));
	if (i + 3 > Dib->DibPixels.size()) 
	    {
	    OutputDebugStringA("PlotPoint: pixel write out of bounds\n");
	    return;
	    }
	if (colour == (DWORD)threshold)			// handle inside colour
	    {
	    if (i > (DWORD)(Dib->DibHeight * Dib->DibWidth * 3))
		return;
	    Dib->DibPixels[i + 0] = (BYTE)TrueCol->InsideBlue;
	    Dib->DibPixels[i + 1] = (BYTE)TrueCol->InsideGreen;
	    Dib->DibPixels[i + 2] = (BYTE)TrueCol->InsideRed;
	    }
	else
	    {
	    BYTE	rgb[3];
	    GetRGB(colour, rgb);
	    if (!Dib->DibPixels.empty())
		memcpy(Dib->DibPixels.data() + i, rgb, 3);
	    }
//	memcpy(Dib.DibPixels + i, GetRGB(colour), 3);
						    // Now do reference
	i = ((DWORD)y * (DWORD)width) + (DWORD)x;

	if (flags & USEWPIXELS)
//	if (calcmode != 'F' && type != PERTURBATION && type != SLOPEFORWARDDIFF)		// these cases get wpixels directly from forward differencing algorithms
	    {
	    if (wpixels.size() != Dib->DibHeight * Dib->DibWidth)	// we probably resized the image
		return;
	    if (x >= 0 && x < width && y >= 0 && y < ydots)
		wpixels[i] = (float)colour;
	    }
	}
    else
	{
	if (x < width && y < height)
	    {
	    address = (WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel) * y);
	    memcpy(Dib->DibPixels.data() + address + x * 3L, &colour, 3);
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
    if (i + 3 > Dib->DibPixels.size()) {
	OutputDebugStringA("PlotPoint: pixel write out of bounds\n");
	return;
	}
    if (!Dib->DibPixels.empty())
	memcpy(Dib->DibPixels.data() + i, &colour, 3);
    }

/**************************************************************************
	Get the colour of a point
**************************************************************************/

DWORD	CPlot::GetColour(WORD x, WORD y)

    {
    DWORD	i, value = 0L;

    if (x < 0 || x >= width || y < 0 || y >= height)
	return 0L;
    i = ((long)y * (long)xdots) + (long)x;

    if (x >= 0 && x < xdots/* - 1*/ && y >= 0 && y < ydots - 1)
	{
	if (i < wpixels.size())
	    {
	    value = (((DWORD)(wpixels[i]) & 0x7fffffff));
	    if (value > (DWORD)threshold)
		{
		OutputDebugStringA("GetColour: pixel colour out of bounds\n");
		value = 0L;
		}
	    }
	else
	    {
	    OutputDebugStringA("GetColour: pixel co-ordinate out of bounds\n");
	    value = 0L;
	    }
	}

    return value;
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
    if (i + 3 > Dib->DibPixels.size()) {
	OutputDebugStringA("PlotPoint: pixel write out of bounds\n");
	return;
	}
    k = ((long) line * (long) width) + (long) x0;
    for (j = 0; j <= length; ++j) 
	{
	if (wpixels.size() != Dib->DibHeight * Dib->DibWidth)	// we probably resized the image
	    return;
	wpixels[k++] = (float) (*(buffer + j));
	if ((*(buffer + j)) >= (DWORD)threshold)		// handle inside colour
	    {
	    Dib->DibPixels[i + 0] = (BYTE)TrueCol->InsideBlue;
	    Dib->DibPixels[i + 1] = (BYTE)TrueCol->InsideGreen;
	    Dib->DibPixels[i + 2] = (BYTE)TrueCol->InsideRed;
	    }
	else
	    {
	    BYTE	rgb[3];
	    GetRGB((DWORD)(*(buffer + j)), rgb);
	    if (!Dib->DibPixels.empty())
		memcpy(Dib->DibPixels.data() + i, rgb, 3);
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
	if (i + 3 > Dib->DibPixels.size()) {
	    OutputDebugStringA("PlotPoint: pixel write out of bounds\n");
	    return;
	    }
	k = ((long) j * (long) width);
	for (m = 0L; m < width; ++m)
	    {
	    n = ((DWORD) (wpixels[k + m]));
	    if (n >= (DWORD)(threshold) && TrueCol->inside_colour > 0)		// inside colour
		{
		Dib->DibPixels[i + m * 3 + 0] = (BYTE)TrueCol->InsideBlue;
		Dib->DibPixels[i + m * 3 + 1] = (BYTE)TrueCol->InsideGreen;
		Dib->DibPixels[i + m * 3 + 2] = (BYTE)TrueCol->InsideRed;
		}
	    else
		{
		n = ((DWORD) (wpixels[k + m]) % MAXPALETTE);
		Dib->DibPixels[i + m * 3 + 0] = TrueCol->PalettePtr[n].rgbtRed;
		Dib->DibPixels[i + m * 3 + 1] = TrueCol->PalettePtr[n].rgbtGreen;
		Dib->DibPixels[i + m * 3 + 2] = TrueCol->PalettePtr[n].rgbtBlue;
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
		memcpy(Dib3D->DibPixels.data() + address + (x + centrex) * 3L, temp, 3);
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
	memcpy(Dib->DibPixels.data() + address + x * 3L, FilterRGB, 3);
	i = ((long)y * (long)xdots) + (long)x;
	if (x >= 0 && x < xdots && y >= 0 && y < ydots)
	    wpixels[i] = (float)colour;
	}
    }

