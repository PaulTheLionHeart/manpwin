#include <Windows.h>
#include "manp.h"
#include "colour.h"
#include "Dib.h"
//#include "pixel.h"
#include "worklist.h"
#include "variables.h"

#define PASSWIDTH	10
#define MAXCALCWORK	12

#pragma once
class CPlotRoutines
    {
    public:
	void	InitPlot(BYTE calcmodeIn, HWND hwndIn, long thresholdIn, CDib *Dib, DWORD  *wpixels, CTrueCol *TrueCol, int blockindex);
	int	Spiral(long calc_frac(HWND, int, int), MYVARIABLES *pVar);
	int	Blinds(long calc_frac(HWND, int, int), void(*plot)(WORD, WORD, DWORD), CWorklist *WORKLIST, int *reset_period, long *colour, MYVARIABLES *pVar);
	int	OneOrTwoPass(long calc_frac(HWND, int, int), void(*plot)(WORD, WORD, DWORD), CWorklist *WORKLIST, int *reset_period, long *colour, MYVARIABLES *pVar);
//	int	add_worklist(int xfrom, int xto, int yfrom, int yto, int ybegin, int pass, int sym, short *num_worklist);
	int	StandardCalculationMode(long calc_frac(HWND, int, int), void(*plot)(WORD, WORD, DWORD), int user_data(HWND hwnd), void symplot2(WORD, WORD, DWORD), void symplot2Y(WORD, WORD, DWORD), void symplot2J(WORD, WORD, DWORD), void symplot4(WORD, WORD, DWORD),
								CWorklist *WORKLIST, int *reset_period, long *colour, long fillcolor, MYVARIABLES *pVar);
	int	tesseral(long calc_frac(HWND, int, int), int user_data(HWND hwnd), void symplot2(WORD, WORD, DWORD), int *reset_period, CWorklist *WORKLIST, long fillcolor, MYVARIABLES *pVar);
	int	bound_trace_main(long calc_frac(HWND, int, int), int user_data(HWND hwnd), void symplot2(WORD, WORD, DWORD), void symplot2Y(WORD, WORD, DWORD), void symplot2J(WORD, WORD, DWORD), void symplot4(WORD, WORD, DWORD), int *reset_period, long *color, MYVARIABLES *pVar);
	int	solidguess(long calc_frac(HWND, int, int), void symplot2(WORD, WORD, DWORD), void symplot2J(WORD, WORD, DWORD), CWorklist *WORKLIST, int *reset_period, MYVARIABLES *pVar);
/*
	// symmetry
	void	symplot2basin(WORD, WORD, DWORD);
	void	symplot4basin(WORD, WORD, DWORD);
	void	symplot2(WORD, WORD, DWORD);
	void	symplot2Y(WORD, WORD, DWORD);
	void	symplot2J(WORD, WORD, DWORD);
	void	symplot4(WORD, WORD, DWORD);
	void	noplot(WORD, WORD, DWORD);
	void	setsymmetry(int sym, int uselist);

	void(*point)(WORD, WORD, DWORD);		// pointers to functions
	void(*plot)(WORD, WORD, DWORD);

	void(*psymplot2basin)(WORD, WORD, DWORD);
	void(*psymplot4basin)(WORD, WORD, DWORD);
	void(*psymplot2)(WORD, WORD, DWORD);
	void(*psymplot2Y)(WORD, WORD, DWORD);
	void(*psymplot2J)(WORD, WORD, DWORD);
	void(*psymplot4)(WORD, WORD, DWORD);
	void(*pnoplot)(WORD, WORD, DWORD);
*/
//	WorklistData	worklist[MAXCALCWORK];
	int	ixstart, ixstop, iystart, iystop;	// start, stop here 
	int	yystart, yystop, yybegin;		// declared as separate items 
	int	xxstart, xxstop;			// these are same as worklist

    private:
//	long(*calc_frac)(HWND hwnd, int row, int col);	// calculate the fractal
//	int	combine_worklist(short *num_worklist);
//	void	tidy_worklist(short *num_worklist);
	int	StandardCalc(long calc_frac(HWND, int, int), void(*plot)(WORD, WORD, DWORD), int passnum, int *row, CWorklist *WORKLIST, int *reset_period, long *colour, MYVARIABLES *pVar);
	void	output_line(WORD x0, WORD line, WORD length, DWORD *buffer);
	BYTE	*GetRGB(DWORD colour);

	// some tesseral stuff
	long	tesschkcol(int, int, int);
	long	tesschkrow(int, int, int);
	long	tesscol(long calc_frac(HWND, int, int), HWND, int, int, int, int *reset_period, MYVARIABLES *pVar);
	long	tessrow(long calc_frac(HWND, int, int), HWND, int, int, int, int *reset_period, MYVARIABLES *pVar);
	void	verline(WORD x0, WORD y0, WORD y1, DWORD colour);
	long	fillcolor = -1;				// tesseral fillcolor: -1=normal 0 means don't fill     
	int	guessplot;				// paint 1st pass row at a time?

	// boundary tracing stuff
	long	calc_xy(HWND hwnd, int mx, int my, long calc_frac(HWND, int, int), long *color, MYVARIABLES *pVar);
	int	boundary_trace(HWND hwnd, int C, int R, long calc_frac(HWND, int, int), int user_data(HWND hwnd), void symplot2(WORD, WORD, DWORD), void symplot2Y(WORD, WORD, DWORD), void symplot2J(WORD, WORD, DWORD), void symplot4(WORD, WORD, DWORD), long *color, MYVARIABLES *pVar);
	int	fillseg(void symplot2(WORD, WORD, DWORD), void symplot2y(WORD, WORD, DWORD), void symplot2j(WORD, WORD, DWORD), void symplot4(WORD, WORD, DWORD), int LeftX, int RightX, int R, long bcolor);
	void	reverse_string(DWORD *t, DWORD *s, int len);
	int	*LeftX;					// [MAXHORIZONTAL];
	int	*RightX;				// [MAXHORIZONTAL];
	DWORD	*ForwardLine;				// [MAXHORIZONTAL];
	DWORD	*BackwardLine;				// [MAXHORIZONTAL];
	unsigned	repeats;

	// Solid Guessing stuff
#define calcadot(h,c,x,y) { if ((c = calc_frac(h, y, x)) == -1) return(-1); }
	int	guessrow(long calc_frac(HWND, int, int), void symplot2J(WORD, WORD, DWORD), HWND hwnd, int firstpass, int y, int blocksize, MYVARIABLES *pVar);
	void	plotblock(int buildrow, int x, int y, long colour);
	int	ssg_blocksize(void);
	// static vars for solidguess & its subroutines
	int	maxblock, halfblock;
	int	right_guess, bottom_guess;
#define maxyblk 7					// maxxblk*maxyblk*2 <= 4096, the size of "prefix" 
#define maxxblk 202					// each maxnblk is oversize by 2 for a "border"
	// maxxblk defn must match fracsubr.c 
	// next has a skip bit for each maxblock unit;
	//   1st pass sets bit  [1]... off only if block's contents guessed;
	//   at end of 1st pass [0]... bits are set if any surrounding block not guessed;
	//   bits are numbered [..][y/16+1][x+1]&(1<<(y&15))
	typedef short(*TPREFIX)[2][maxyblk][maxxblk];
#define tprefix   (*((TPREFIX)prefix))
	unsigned int prefix[2][maxyblk][maxxblk];	// common temp
	int	blockindex;				// 2 ^ n block size
	DWORD	*dstack;				// [MAXHORIZONTAL * 3];

	// symmetry
//	int	xsym_split(int xaxis_row, int xaxis_between);
//	int	ysym_split(int yaxis_col, int yaxis_between);

	HWND hwnd;

	CDib	*Dib;
	CTrueCol *TrueCol;
	DWORD  *wpixels;

	BYTE	calcmode;
//	int	row, col;
	long	threshold;				// maximum iterations 
//	void(*Symplot2)(WORD, WORD, DWORD);

    };

