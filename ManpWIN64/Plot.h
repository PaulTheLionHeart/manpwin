/*
    Plot.h - interface for the CPlot class.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <vector>
#include <atomic>
#include "Dib.h"
#include "manpwin.h"
//#include "manp.h"
#include "colour.h"
#include "fractype.h"

// Plot types
#define	USEPALETTE  1
#define	USEWPIXELS  2

// Control flags
#define PIXEL_NORMAL   0x00
#define PIXEL_INSIDE   0x01
#define PIXEL_SPECIAL  0x02

#define TESS_SPECIAL_COLOUR   -4L

// the following are used to control Forward Differencing to set a pixel to special colour or  to make sure that we don't splatter inside 
// colours when filters are used. These are written into wpixels as control flags
#define	    INSIDEPIXEL				-1.0
#define	    SPECIALPIXEL			-2.0

#pragma once

class CPlot
    {
    public:
	CPlot();                              // default constructor
	CPlot(std::vector<float>& wp);        // explicit constructor
	void	GetRGB(DWORD colour, BYTE *rgb);
	void	GetRGB(DWORD colour, RGBTRIPLE *rgb);
	void	GetRGB(DWORD colour, RGBTRIPLE *rgb, BYTE *Palette);
	void    PlotPoint(WORD x, WORD y, DWORD colour);
	void	OutRGBpoint(WORD x, WORD y, RGBTRIPLE colour);
	DWORD   GetColour(WORD x, WORD y);
	DWORD	GetColour(WORD x, WORD y, BYTE &flags);
	void	OutputLine(WORD x0, WORD line, WORD length, DWORD *buffer);
	void	RefreshScreen(void);
	void	DisplayCircle(int centrex, int centrey, int radius, DWORD colour);
	void	DisplayFilledCircle(int centrex, int centrey, int radius, DWORD colour);
	int	Display3DCircle(CDib *Dib3D, int centrex, int centrey, int radius, BYTE *colour);
	void	InitPlot(long thresholdIn, CTrueCol *TrueColIn, std::vector<float> *wpixelsIn, int xdotsIn, int ydotsIn, int widthIn, int heightIn, int bits_per_pixelIn, CDib *DibIn, int flagsIn);
	void	DoPlot(int x, int y, DWORD colour);
	void	FilterPoint(WORD x, WORD y, DWORD colour, RGBTRIPLE *FilterRGB);
	void	genline(int x0, int y0, int x1, int y1, DWORD col);

	int	flags = 0;

    private:
	bool	DebugValidate(const char* where) const;
	long	threshold;			// maximun iterations
	CTrueCol    *TrueCol;			// palette info
	std::vector<float>* wpixels = nullptr;
	int	xdots, ydots, height, width, bits_per_pixel;

	CDib	*Dib;				// Device Independent Bitmap

    };

