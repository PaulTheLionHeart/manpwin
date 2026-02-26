// Filter.h: interface for the Tierazon filters.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Complex.h"
#include "colour.h"

#define	MAXFDOPTIONS	20
#define	MAXTEMP		50000
#define	MAXSIZE		1.0e+6
#define	MINSIZE		1.0e-6
#define	OFFSET_COLOR	64
#define	DUAL_BFACTOR	3
#define PIXELS_BETWEEN_UPDATES	100		// how many pixels should we wait before updating image?

struct TierazonFilterStuff			// database of Tierazon filters
    {
    char    *name;				// name of the fractal 
    char    calctype;				// only allow '1', '2', blinds or spiral 
    bool    rgb;				// normal point routine or filter RGB routine 
    };

extern	struct	TierazonFilterStuff	TierazonFilter[];	// default values for each 
extern	struct	TierazonFilterStuff	TierazonColour[];	// default values for each 
extern	char	*TierazonFDOptions[];		// list of option 

class CTZfilter
    {
    public:
	void	LoadFilterQ(Complex q);
	void	InitFilter(int method, long threshold, double dStrands, int nFDOption, BOOL UseCurrentPalette);
	int	DoTierazonFilter(Complex z, long *iteration);
	int	EndTierazonFilter(Complex z, long *iteration, CTrueCol *TrueCol);
	void	ColourMethods(Complex z, Complex q, CTrueCol *TrueCol, int ColourMethod);
	void	FDimension(int FilterType, long *iteration);

	RGBTRIPLE FilterRGB;
	int	method;
	int	nFDOption = 4;

    private:
	double	x_y_to_degrees(double x_, double y_);

	double	pXTemp[MAXTEMP], pYTemp[MAXTEMP];		// Data array pointers
	double	temp;
	double	dStrands_LO, dStrands_HI, zsav, xtot, ytot, ztot, xavg, xdif, yavg, ydif, zavg, zdif, limit;
	double	d_real, d_imag;
	long	iteration_x, iteration_y, iteration_z;
	double	d_color_x = 0, d_color_y = 0, d_color_z = 0;
	int	n_color_x = 0, n_color_y = 0, n_color_z = 0;
	double	dsx, dsy, dStrands_LO_cx, dStrands_HI_cx, dStrands_cxd, dStrands_cyd;

	int	ColourOffset, size_x, size_y, j, d, px, py, nUsingBuffers;
	double	ssq1;
	double	dStrands_df, limit_cxd, limit_cyd, b;
	double	dStrands_LO_cy, dStrands_HI_cy, dif_test, m_lower, m_upper, dt;
	double	dFactor = 0.80, dzx, dzy, rj, gj, bj, xsav, ysav, dFactor_2;
	double	dStrands = 0.08;		// for Tierazon filters

	Complex q;
	long	threshold;
	long	*iteration;
	BOOL	UseCurrentPalette;
	int	FilterType;		// Note that FilterType = method - TIERAZONFILTERS

	int	nGrn = 2;
	int	nBlu = 2;
	int	nRed = 2;
	int	nGrnStart = 58;
	int	nBluStart = 27;
	int	nRedStart = 143;
    };

