#include <Windows.h>
#include "colour.h"
#include "Dib.h"
#include "Manp.h"
#include "fractype.h"

#pragma once
class CSymPlot
    {
    public:
	void	InitSymPlot(int xxstartIn, int xxstopIn, int yystartIn, int yystopIn, int yybeginIn, int ixstartIn, int ixstopIn, int iystartIn, int iystopIn, 
		CDib *DibIn, int PlotTypeIn, CTrueCol *TrueColIn, double *wpixelsIn, int subtypeIn, BYTE degreeIn, long thresholdIn, WORD typeIn, BYTE calcmodeIn, 
		RGBTRIPLE *FilterRGBIn);
	void	plot(WORD x, WORD y, DWORD color);
	void	PlotPixel(WORD, WORD, DWORD);
	CTrueCol    *TrueCol;		// palette info
	CDib	*Dib;
	double	*wpixels;		// an array of doubles holding slope modified iteration counts


    private:
	// plotting modes

	void	symplot2(WORD, WORD, DWORD);
	void	symplot2Y(WORD, WORD, DWORD);
	void	symplot2J(WORD, WORD, DWORD);
	void	symplot4(WORD, WORD, DWORD);
	void	noplot(WORD, WORD, DWORD);
	void	symplot4basin(WORD, WORD, DWORD);
	void	symplot2basin(WORD, WORD, DWORD);
	void	FilterPoint(WORD x, WORD y, DWORD colour);	// ignore the colour - naughty use of the global FilterRGB
	void	GetRGB(DWORD colour, BYTE *rgb);
	int	xxstart, xxstop;				// these are same as worklist, 
	int	yystart, yystop, yybegin;			// declared as separate items  
	int	ixstart, ixstop, iystart, iystop;		// start, stop here 
	int	PlotType;
	int	subtype;		// B=basin, S=stripe, N=normal
	BYTE	degree;			// polynomial degree
	long	threshold;
	WORD	type;			// M=mand, J=Julia 1,2,4->
	BYTE	calcmode;		// trace type B, G, 1, 2
	RGBTRIPLE   *FilterRGB;		// for Tierazon filters
    };

