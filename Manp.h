#include <ctype.h>
#include <windows.h>
#include <setjmp.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "manpwin.h"

#pragma once

#define MAXLINE		250		/* maximum length of a line */
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

#define NORMAL	0			// orientation is usually 0 degrees

#define	MAXDIM	36			// for oscillators, curves, knots, surfaces etc

struct workliststuff	/* work list entry for std escape time engines */
    {
    int xxstart;	/* screen window for this entry */
    int xxstop;
    int yystart;
    int yystop;
    int yybegin;	/* start row within window, for 2pass/ssg resume */
    int sym;		/* if symmetry in window, prevents bad combines */
    int pass;		/* for 2pass and solid guessing */
    };
#define MAXCALCWORK 12

// defines for plotting including symmetry 
#define  NOSYM		0
#define  XAXIS		1
#define  YAXIS		2   // symplot2Y
#define  XYAXIS 	3   // symplot4
#define  ORIGIN 	4   // symplot2J
#define  PI_SYM 	5
#define  XAXIS_NOIMAG  -6
#define  XAXIS_NOREAL	6
#define  XAXISBASIN	7   // symplot2basin
#define  XYAXISBASIN	8   // symplot4basin
#define  FILTERPLOT	9   // FilterPoint
#define  PREVIEWPLOT	10  // PreviewPoint
//#define  CirclePLOT	11  // CirclePoint

#define  NOPLOT        99
#define  SETUP_SYM    100

#define	INCOMPLETE	0
#define	COMPLETE	1
#define	INITIALISING	2
#define	INFORMATION	3
#define	CALCULATINGREF	4

//extern	WORD	colours;

struct FNCT_LIST 
    {
    char *s;
    void (**ptr)(void);
    };

enum	ProcessType { STANDARD, INITANIM, RUNANIM, EVOLUTION, MORPHING };
enum	AxisDisplayType { OFF, DISPLAYALL, DISPLAYSINGLE };

