/*------------------------
   MANPWIN.H header file
  ------------------------*/

#undef  huge
#define huge      
#undef  _export
#define _export

#include <atomic>
#include <exception>
#include "Big.h"
#pragma once

#define FILE_BMP      	0
#define FILE_MAP     	1
#define FILE_PNG	2
#define FILE_IFS	3
#define FILE_LSY	4
#define FILE_PAR	5
#define FILE_FPR	6	// Fractint PAR
#define FILE_SCI	7
#define FILE_FRM	8
#define FILE_COL	9
#define FILE_GIF	10
#define FILE_MPG	11
#define FILE_LST	12
#define FILE_KFR	13	// Kalles KFR
#define FILE_SVG	14	// SVG vector graphics

#define TRUE		1
#define FALSE		0
#define EVER		(;;)

#define	MAXHORIZONTAL	40000
#define	MAXVERTICAL	40000
#define	VGA_PAL_SIZE	768
#define	EGA_PAL_SIZE	16
#define VGA_COLOURS	256
#define	MAXFORMULASTRINGLENGTH		3600	


// defines for inside/outside
#define NONE		0
#define ZMAG		1
#define BOF60		2
#define BOF61		3
#define BIOMORPH	9
#define ITER		10
#define REAL		11
#define IMAG		12
#define MULT		13
#define SUM		14
#define ATAN		15
#define PERT1		16
#define PERT2		17
#define POTENTIAL	18
#define EPSCROSS	90
#define STARTRAIL	91
#define PERIOD		92
#define TIERAZONFILTERS	100
#define TIERAZONCOLOURS	200

#define	ANIMTIMER	1
#define	UPDATETIMER	2

#define NOMULTITHREAD	0
#define RENDER_SLOPE	1
#define RENDER_PERT	2
#define RENDER_PIXEL	3

#define	MAXDATALINE	SIZEOF_BF_VARS * 4	// maximum frame data length

#define	FixedGlobalAlloc(n) 	(char *)GlobalAlloc(GMEM_FIXED, n)
#define	FixedGlobalFree(p)	GlobalFree(p)
#define	FixedGlobalRealloc(p,n) GlobalRealloc(p,n)

#define	RGB_RED			0
#define	RGB_GREEN		1
#define	RGB_BLUE		2
#define	RGB_SIZE		3

// Macro to restrict a given value to an upper or lower boundary value
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

// Macro to align given value to the closest DWORD (unsigned long )
#define ALIGNULONG(i)   ((i+3)/4*4)

// Macro to determine to round off the given value to the closest byte
//#define WIDTHBYTES(i)   ((i+31)/32*4)	    Replaced by ComputeWidthBytes() in Dib.h

#define GET_2B(array,offset)  ((unsigned int) (BYTE)(array[offset]) + \
			       (((unsigned int) (BYTE)(array[offset+1])) << 8))
#define GET_4B(array,offset)  ((long) (BYTE)(array[offset]) + \
			       (((long) (BYTE)(array[offset+1])) << 8) + \
			       (((long) (BYTE)(array[offset+2])) << 16) + \
			       (((long) (BYTE)(array[offset+3])) << 24))

#ifndef sqr
#define sqr(x) ((x)*(x))
#endif

#define DEM_BAILOUT 535.5  // (pb: not sure if this is special or arbitrary)

#ifndef	VIEWMAIN
#define VIEWMAIN
//extern	int	height, xdots, ydots, width, bits_per_pixel, planes;
//extern	BYTE		line_buf[MAXHORIZONTAL * 3];    // true colour = 3 bytes per pixel
//extern	WORD		numcolors;			// colours in the file
//extern	int	fdin;					// FILE descripter
//extern	DWORD 	PASCAL	lread (int, VOID far *, DWORD);
//extern	WORD	iNumColors;    			// Number of colors supported by device
#endif

extern std::atomic<bool> gStopRequested;

// a tiny inline helper making it cleaner and easier to read.
inline bool AbortRequested()
    {
    return gStopRequested.load(std::memory_order_relaxed);
    }

#define MAXLINE		250		// maximum length of a line
#define STATUSSIZE	1600		// size of status bar
#define POSITIONSIZE	1024		// size of position string
#define TRUE		1
#define FALSE		0
#define EVER		(;;)
#define	ESC_CHAR	0x1b
#define	MAXOSC		10000		// maxiumum number of oscillators/curves/surfaces/knots/fractal maps

#define max(a,b)	(((a) > (b)) ? (a) : (b))
#define min(a,b)	(((a) < (b)) ? (a) : (b))

#define	MAXPOLYDEG	8		// only because we are limnited by param[] for polynomial()

#define	dot_product(v1,v2)  ((v1)[0]*(v2)[0]+(v1)[1]*(v2)[1]+(v1)[2]*(v2)[2])

#define BLACK	0
#define BLUE	1
#define GREEN	2
#define RED	4
#define WHITE	15
#define PI	3.141592653589793238462643383279
#define	TWO_PI	6.28318530717958
#define	HALF_PI	1.570796326794895
#define	RAD	57.29577951308233
#define	ROOT2	1.414213562
#define	ROOT3	1.732050808

#define	SINSQR(x)	(sin(x)*sin(x))
#define	COSSQR(x)	(cos(x)*cos(x))
#define	SEC(x)		(1.0/cos(x))
#define	COT(x)		(cos(x)/sin(x))
#define	cube(x)		((x)*(x)*(x))

#define	NUM_ORBITS	240

#define NORMAL	0		// orientation is usually 0 degrees

#define	MAXDIM	36		// for oscillators, curves, knots, surfaces etc

struct workliststuff		// work list entry for std escape time engines
    {
    int xxstart;		// screen window for this entry
    int xxstop;
    int yystart;
    int yystop;
    int yybegin;		// start row within window, for 2pass/ssg resume
    int sym;			// if symmetry in window, prevents bad combines
    int pass;			// for 2pass and solid guessing
    };
#define MAXCALCWORK 12

// defines for plotting including symmetry 
#define  NOSYM		0
#define  XAXIS		1
#define  YAXIS		2	// symplot2Y
#define  XYAXIS 	3	// symplot4
#define  ORIGIN 	4	// symplot2J
#define  PI_SYM 	5
#define  XAXIS_NOIMAG  -6
#define  XAXIS_NOREAL	6
#define  XAXISBASIN	7	// symplot2basin
#define  XYAXISBASIN	8	// symplot4basin
#define  FILTERPLOT	9	// FilterPoint
#define  PREVIEWPLOT	10	// PreviewPoint
//#define  CirclePLOT	11	// CirclePoint

#define  NOPLOT        99
#define  SETUP_SYM    100

#define	INCOMPLETE	0
#define	COMPLETE	1
#define	INITIALISING	2
#define	INFORMATION	3
#define	CALCULATINGREF	4
#define	CLOSINGTHREADS	5

//extern	WORD	colours;

struct FNCT_LIST
    {
    char *s;
    void(**ptr)(void);
    };

enum	ProcessType { STANDARD, INITANIM, RUNANIM, EVOLUTION, MORPHING };
enum	AxisDisplayType { OFF, DISPLAYALL, DISPLAYSINGLE };

