#pragma once

#include	<windows.h>
#include	"complex.h"
#include	"big.h"
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"Dib.h"
#include	"OscProcess.h"
#include	"filter.h"
#include	".\parser\cmplx.h"

typedef struct MyVariables 
    {
    Complex	z;						// basic complex variables
    Complex	q;
    Complex	j;
    Complex	c;

    double	hor;						// horizontal address
    double	vert;						// vertical address 
    double	xgap;						// gap between pixels 
    double	ygap;						// gap between pixels 

    int		method;						// inside and outside filters
    WORD	type;						// M=mand, J=Julia 1,2,4-> 

    int		time_to_zoom;					// time to zoom in or out?
    int		time_to_restart;				// time to restart?
    int		time_to_reinit;					// time to reinitialize?
    int		time_to_quit;					// time to quit?
 //   int 	time_to_load;					// time to load file?
 //   int	time_to_break;					// time to break out of animation?

    CTZfilter	*TZfilter;					// Tierazon filters
    RGBTRIPLE	*FilterRGB;					// for Tierazon filters

//    DWORD 	*wpixels;					// the device-independent bitmap  pixels
/*
    Complex	temp0;						// some temp ones!!!
    Complex	temp1;
    Complex	temp2;
    Complex	temp3;
    Complex	temp4;
    Complex	temp5;
    Complex	tempsqr;
    Complex	tempxy;

    double	tmpfl0;						// some temp floats just in case
    double	tmpfl1;
    double	tmpfl2;
    double	tmpfl3;
    double	xgap;						// gap between pixels
    double	ygap;
    int		row, col, currow, curcol;
    int		oldcol;
    int		oldrow;
    long	andcolor;
    int		PhoenixDegree;

    int		time_to_zoom;					// time to zoom in or out?
    int		time_to_restart;				// time to restart?
    int		time_to_reinit;					// time to reinitialize?
    int		time_to_quit;					// time to quit?
    int 	time_to_load;					// time to load file?
    int		time_to_break;					// time to break out of animation?
								// some general storage
    long	color, oldcolor;
    int		iteration;
    int		reset_period;					// periodicity checking
    int		ThreadNum;
    int		TotalThreads;




    long	iteration;					// globals for speed
    int		method;						// inside and outside filters
    BOOL	ExpandStarTrailColours = TRUE;			// use the first 16 colours if false, else expand across the whole iteration range
    BOOL	invert = FALSE;					// invert fractal

    BYTE	SpecialFlag;					// tell graphing routine of special conditions
    WORD	special;					// special colour for CBIN fractals
    int		biomorph;					// biomorph colour

    int		FilterType;					// data for Tierazon filters

    _CMPLX	old;						// to allow Fractint fractals to work with filters
    double	dem_delta, dem_width;				// distance estimator variables 
    double	dem_toobig;
    int		CoordSystem;

    double	dStrands;

    long	threshold;
    int		decomp;						// number of decomposition colours 
    long	color;						// used for Newton root locations 
    BYTE	orbit_flag;					// display orbits? 
    BYTE	juliaflag;					// Julia implementation of fractal 
    BYTE	phaseflag;					// 3 phases for type SPECIALNEWT fractals 
    WORD	type;						// M=mand, J=Julia 1,2,4-> 
    int		subtype;					// B=basin, S=stripe, N=normal 
    long	oldcolour;					// for periodicity checking 
    int		reset_period;					// periodicity checking 
    int		period_level;					// 0 for no periodicity checking 
    double	closenuff;					// periodicity bailout 
    _CMPLX	parm, parm2;					// got to get the stuff into the routines someway
    double	param[];
    //extern	_CMPLX	New, old;
    double	rqlim;
    int		distest;
    BYTE	BigNumFlag;					// True if bignum used
    int		NonStandardFractal;				// does fractal use standard plotting mode?
    BYTE	pairflag;					// stereo pair
    double	yymax;						// max value of vert
    BYTE	calcmode;					// trace type B, G, 1, 2 
    BYTE	_3dflag;					// generate image in 3D

    int		oldrow = -1;
    int		oldcol = -1;
    long	andcolor;

    int		orientation;					// 0, 90, 180 or 270 degrees

    int		logval;						// log colour map 
    BYTE 	*logtable;					// log value table for col comp

    int		float_decomposition(double, double);
    int		potential(double, int);
    long	BigCalcFrac(HWND, int, int);
    void	projection(int, int, long);
    void	do_stereo_pairs(int, int, long);
    void	draw_right_image(short);

    BOOL	UseCurrentPalette;				// do we use the ManpWIN palette? If false, generate internal filter palette
    Complex	invertz2(Complex  &);
    COscProcess	OscProcess;

    CTrueCol    TrueCol;


    Complex	z, q, j, c, deriv;

    */


//    long	*wpixels;					// pointer to an array of colours for each pixel
    // Bignum variables
    BigComplex	zBig, qBig, cBig;
    BigDouble	BigBailout;

//    CDib	ThreadDib;					// Device Independent Bitmap
//    HWND	hwnd;

    } MYVARIABLES;

