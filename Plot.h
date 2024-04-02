// Plot.h: interface for the CPlot class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "Dib.h"
#include "manpwin.h"
#include "manp.h"
#include "colour.h"
#include "fractype.h"

// Plot types
#define	USEPALETTE  1
#define	USEWPIXELS  2

#pragma once

class CPlot
    {
    public:
	void	GetRGB(DWORD colour, BYTE *rgb);
	void    PlotPoint(WORD x, WORD y, DWORD colour);
	void	OutRGBpoint(WORD x, WORD y, RGBTRIPLE colour);
	DWORD   GetColour(WORD x, WORD y);
	void	OutputLine(WORD x0, WORD line, WORD length, DWORD *buffer);
	void	RefreshScreen(void);
	void	DisplayCircle(int centrex, int centrey, int radius, DWORD colour);
	void	DisplayFilledCircle(int centrex, int centrey, int radius, DWORD colour);
	int	Display3DCircle(CDib *Dib3D, int centrex, int centrey, int radius, BYTE *colour);
	void	InitPlot(long thresholdIn, CTrueCol *TrueColIn, double *wpixelsIn, int xdotsIn, int ydotsIn, int widthIn, int heightIn, int bits_per_pixelIn, CDib *DibIn, int flagsIn);
	void	DoPlot(int x, int y, DWORD colour);
	void	FilterPoint(WORD x, WORD y, DWORD colour, RGBTRIPLE *FilterRGB);
	void	genline(int x0, int y0, int x1, int y1, DWORD col);

	int	flags = 0;

    private:
	long	threshold;			// maximun iterations
	CTrueCol    *TrueCol;			// palette info
	double 	*wpixels;			// floating point iterations for each pixels
	int	xdots, ydots, height, width, bits_per_pixel;

	CDib	*Dib;				// Device Independent Bitmap

    };

