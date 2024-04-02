#include "pixel.h"
#include "slope.h"
#include "fractype.h"
#include "fract.h"

#pragma once

class CSym
    {
    public:
	void	CSym::InitSym(int *ixstartIn, int *ixstopIn, int *iystartIn, int *iystopIn, int *xxstartIn, int *xxstopIn, int *yystartIn, int *yystopIn, int *yybeginIn,
	    short *num_worklistIn, CCalcMode CalcModeIn, int worksymIn, int workpassIn, int subtypeIn, double vertIn, double mandel_widthIn, double ScreenRatioIn,
	    double horIn, WORD typeIn, BYTE degreeIn, double xxminIn, double xxmaxIn, double yyminIn, double yymaxIn, int symmetryIn, BYTE BigNumFlagIn,
	    BigDouble Big_xxmaxIn, BigDouble Big_xxminIn, BigDouble Big_yymaxIn, BigDouble Big_yyminIn, BigDouble BigHorIn, BigDouble BigVertIn, BigDouble BigWidthIn);
	int	FindSymmetry(BYTE _3dflag, int decomp, BYTE pairflag, int method, BOOL invert, int CoordSystem, double param[], BYTE juliaflag, WORD special, BYTE calcmode,
	    int orientation, CFract Fractal);

	int	*ixstart, *ixstop, *iystart, *iystop;	// start, stop here 
	int	*xxstart, *xxstop;			// these are same as worklist, 
	int	*yystart, *yystop, *yybegin;		// declared as separate items  
	short	*num_worklist;				// resume worklist for standard engine 
	int	PlotType;

    private:
	CTrueCol    *TrueCol;				// palette info
	CDib	*Dib;
	CCalcMode   CalcMode;
	double	*wpixels;				// an array of doubles holding slope modified iteration counts
	BYTE	calcmode;
	long	threshold;				// maximum iterations 
	BYTE	juliaflag;				// Julia implementation of fractal 
	int	symmetry;				// symmetry flag 

	int	worksym;				// for the sake of calcmand   
	int	workpass;				// for the sake of calcmand  
	int	subtype;				// B=basin, S=stripe, N=normal
	double	vert;					// vertical address
	double	mandel_width;				// width of display
	double	ScreenRatio;				// ratio of width / height for the screen
	double	hor;					// horizontal address
	WORD	type;					// M=mand, N=Newton etc
	BYTE	degree;					// degree 
	double	xxmin, xxmax, yymin, yymax;		// corners 

	BYTE	BigNumFlag;				// True if bignum used
	BigDouble    Big_xxmax, Big_xxmin, Big_yymax, Big_yymin, BigHor, BigVert, BigWidth;

	int	xsym_split(int xaxis_row, int xaxis_between);
	int	ysym_split(int xaxis_row, int xaxis_between);
	int	setsymmetry(int sym, int uselist);

    };

