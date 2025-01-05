//////////////////////////////////////////////////////////////////////
//
// OtherFunctions.h: fractals that aren't raster based.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include "complex.h"
#include "Fractype.h"
#include "ManpWin.h"
#include "Pixel.h"
#include "Manp.h"

// Error codes for Cellular
#define BAD_T         1
#define BAD_MEM       2
#define STRING1       3
#define STRING2       4
#define TABLEK        5
#define TYPEKR        6
#define RULELENGTH    7

#define CELLULAR_DONE 10

#define rand15()    rand()
#define RANDOM(x)   (rand()%(x))

#ifdef _WIN64
#define MAXIMUM_TOWER	    20001
#else
#define MAXIMUM_TOWER	    10001
#endif
#define MAXIMUM_POINTS	    10000

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

//////////////////////////////////////////////////////////////////////
// Class definition
//////////////////////////////////////////////////////////////////////

class COtherFunctions
    {
    public:
	int	InitOtherFunctions(WORD type, Complex *z, Complex *q, HWND hwndIn, double *wpixels, CTrueCol *TrueColIn, CDib *Dib, char *AntStatus, struct __timeb64 FrameEndIn, struct __timeb64 FrameStartIn);
	int	COtherFunctions::RunOtherFunctions(WORD type, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration, int xdots, int ydots, double param[], long threshold, double hor, double vert, double mandel_width, 
		double ScreenRatio, int *curpass, int *totpasses, int user_data(HWND hwnd), HWND hwnd, int rotate(int dir));

    private:
	CPlot	Plot;

    // Stuff for Cellular
	void	abort_cellular(int err, int t);
	void	thinking(HWND hwnd, int count);

	WORD	cell_array[2][MAXHORIZONTAL];
	int	r, k_1, rule_digits;
	int	lstscreenflag;
	int	rflag = FALSE;
	int	rseed;
	int	row, col;
	HWND	hwnd;

	// Stuff for DemoWalk
	int	colors = 256;

	// Stuff for Plasma
	WORD	rand16();
	void	subDivide(int x1, int y1, int x2, int y2, int user_data(HWND hwnd));
	int	new_subD(int x1, int y1, int x2, int y2, int recur, int user_data(HWND hwnd));
	WORD	adjust(int xa, int ya, int x, int y, int xb, int yb);
	void	set_Plasma_palette(int rotate(int direction));

	int	plasma_check;		// to limit kbd checking 
	int	iparmx;			// iparmx = parm.x * 16 
	int	shiftvalue;		// shift based on #colors 
	int	max_colors = 32000;
	int	recur1 = 1;
	int	pcolors;
	int	recur_level = 0;
	WORD	max_plasma;
	CTrueCol    *TrueCol;
	CDib	*Dib;

	// Stuff for Diffusion
	void	FPUsincos(double *Angle, double *Sin, double *Cos);

	// stuff for ant
#define RANDOMANT(n)    ((int)((long)((long)rand() * (long)(n)) >> 15)) // Generate Random Number 0 <= r < n
#define MAX_ANTS        256
#define XO              (xdots/2)
#define YO              (ydots/2)
#define DIRS            4
#define INNER_LOOP      100
	int	*incx[DIRS];         // tab for 4 directions
	int	*incy[DIRS];
	short	AntRseed;
	short	AntRflag;
	char	*AntStatus;
	struct __timeb64 	FrameEnd;
	struct __timeb64 	FrameStart;
	//	void	TurkMite2(int maxtur, size_t rule_len, char *ru, long maxpts, long wait);
	void	TurkMite1(int maxtur, size_t rule_len, char *ru, long maxpts, long wait);
	int	ant(void);
	int	tower(void);

//	void	setwait(long *wait);
//	int	getakey();


    };

