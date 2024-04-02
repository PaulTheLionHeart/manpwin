#include "pixel.h"
#include "slope.h"
#include "fractype.h"

#pragma once

#define PASSWIDTH	10

// some guess stuff
#define maxyblk 7    // maxxblk*maxyblk*2 <= 4096, the size of "prefix"
#define maxxblk 202  // each maxnblk is oversize by 2 for a "border"
// maxxblk defn must match fracsubr.c 
/* next has a skip bit for each maxblock unit;
1st pass sets bit  [1]... off only if block's contents guessed;
at end of 1st pass [0]... bits are set if any surrounding block not guessed;
bits are numbered [..][y/16+1][x+1]&(1<<(y&15)) */

typedef short(*TPREFIX)[2][maxyblk][maxxblk];
#define tprefix   (*((TPREFIX)prefix))

#define calcadot(h,c,x,y) { if ((c = Pix->calc_frac(h, y, x)) == -1) return(-1); }

struct tess
    {				    // one of these per box to be done gets stacked 
    int	    x1, x2, y1, y2;	    // left/right top/bottom x/y coords 
    long    top, bot, lft, rgt;	    // edge colours, -1 mixed, -2 unknown
    };

class CCalcMode
    {
    public:
	int	add_worklist(int, int, int, int, int, int, int);
	int	StandardCalculationMode(HWND hwnd, CPixel *Pix, CSlope Slope/*, int user_data(HWND hwnd), int xdots, int ydots, int width, int height, bool *ThreadComplete*/);
	void	InitCalcMode0(struct workliststuff *worklistIn, int *symmetryIn);
	void	InitCalcMode1(int *totpassesIn, int *curpassIn, long *colorIn, int reset_periodIn, BYTE calcmodeIn, long thresholdIn, BYTE juliaflagIn, long fillcolor,
		int *blockindexIn, BYTE save_flagIn, int *time_to_zoom, int *time_to_restart, int *time_to_reinit, int *time_to_quit, CTrueCol *TrueColIn, CDib *DibIn,
		double *wpixels, long *andcolor, int *oldrow, int *oldcol, int *PlotTypeIn, int threadIn);
	int	FindSymmetry(BYTE _3dflag, int decomp, BYTE pairflag, int method, BOOL invert, int CoordSystem, double param[], BYTE degree, WORD type,
		int subtype, BYTE calcmode, int orientation, CFract *Fractal, double hor, double vert, double mandel_width, BYTE BigNumFlag, BigDouble *Big_xxmin,
		BigDouble *Big_xxmax, BigDouble *Big_yymin, BigDouble *Big_yymax, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double ScreenRatio, double *xxmin,
		double *xxmax, double *yymin, double *yymax, WORD special, BYTE juliaflag);

	void	output_line(WORD x0, WORD line, WORD length, DWORD *buffer);


	int	ixstart, ixstop, iystart, iystop;	// start, stop here 
	int	xxstart, xxstop;			// these are same as worklist, 
	int	yystart, yystop, yybegin;		// declared as separate items  
	int	workpass, *totpasses, *curpass;		// for 1/2 pass type tracing 
	short	num_worklist;				// resume worklist for standard engine 
	int	worksym;

	long	*color;
	int	row, col, reset_period;
	// Guess stuff
	int	*blockindex;			// 2 ^ n block size
	BYTE	save_flag;			// save screen after image
	int	*PlotType;

    private:
	void	tidy_worklist();
	int	combine_worklist();
	int	StandardCalc(HWND, CPixel *Pix, int);
	int	Blinds(HWND, CPixel *Pix);
	int	DoBlinds(HWND, CPixel *Pix);
	int	Spiral(HWND, CPixel *Pix);
	int	OneOrTwoPass(HWND, CPixel *Pix);
	int	solidguess(HWND hwnd, CPixel *Pix);
	DWORD	getcolor(WORD x, WORD y);
//	void	noplot(WORD x, WORD y, DWORD color);
//	void	symplot4(WORD x, WORD y, DWORD color);
//	void	symplot2(WORD x, WORD y, DWORD color);
	// symmetry stuff
	int	xsym_split(int xaxis_row, int xaxis_between/*, int *symmetry*/);
	int	ysym_split(int xaxis_row, int xaxis_between/*, int *symmetry*/);
	void	setsymmetry(int sym, /*int thread, */int uselist, BYTE _3dflag, int decomp, BYTE pairflag, int method, BOOL invert, int CoordSystem, double param[],
		BYTE degree, WORD type, int subtype, BYTE calcmode, int orientation, CFract *Fractal, double hor, double vert, double mandel_width, double *xxmax,
		double *xxmin, double *yymin, double *yymax, WORD special, BYTE juliaflag, int *PlotType, BYTE BigNumFlag, BigDouble *Big_xxmin, BigDouble *Big_xxmax,
		BigDouble *Big_yymin, BigDouble *Big_yymax, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double ScreenRatio);
	int	*symmetry;				// symmetry flag

	int	thread;			// for later use

	CTrueCol    *TrueCol;		// palette info
	CDib	*Dib;
	double	*wpixels;		// an array of doubles holding slope modified iteration counts
	BYTE	calcmode;
	long	threshold;				// maximum iterations 
	BYTE	juliaflag;				// Julia implementation of fractal 
	struct workliststuff *worklist[MAXCALCWORK];
	int	*time_to_zoom;
	int	*time_to_restart;
	int	*time_to_reinit;
	int	*time_to_quit;

	// boundary trace stuff
	int	bound_trace_main(HWND, CPixel *Pix);
	int	boundary_trace(HWND, CPixel *Pix, int, int);
	long	calc_xy(HWND hwnd, CPixel *Pix, int, int);
	int	fillseg(int, int, int, long, CPixel *Pix);
	void	reverse_string(DWORD *, DWORD *, int);
	void	GetRGB(DWORD colour, BYTE *rgb);
	int	LeftX[MAXHORIZONTAL];
	int	RightX[MAXHORIZONTAL];
	WORD	repeats;
	long	*andcolor;
	int	*oldrow;
	int	*oldcol;

	// tesseral stuff
	int	tesseral(HWND hwnd, CPixel *Pix, int user_data(HWND hwnd));
	long	tesschkcol(int, int, int);
	long	tesschkrow(int, int, int);
	long	tesscol(HWND, CPixel *Pix, int, int, int);
	long	tessrow(HWND, CPixel *Pix, int, int, int);
	void	verline(WORD x0, WORD y0, WORD y1, DWORD colour, CPixel *Pix);
	void	ClearTessMemory(DWORD *dstack);
	long    fillcolor;
	int	currow, curcol;

	// Guess stuff
	int	maxblock, halfblock;
	int	guessplot;			// paint 1st pass row at a time? 
	int	right_guess, bottom_guess;
	int	guessrow(HWND, CPixel *Pix, int, int, int);
	void	plotblock(int, int, int, long, DWORD *dstack, CPixel *Pix);
	int	ssg_blocksize(void);
	unsigned int prefix[2][maxyblk][maxxblk]; // common temp 
    };

//extern	int	bound_trace_main(HWND, CPixel *Pix, short *num_worklist);
//extern	int	solidguess(HWND, CPixel *Pix);
//extern	int	tesseral(HWND, CPixel *Pix);
