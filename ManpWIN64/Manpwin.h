/*------------------------
   MANPWIN.H header file
  ------------------------*/
/*
#pragma once
  // comment out the following line for registered version
//#define SHAREWARE	1

// comment out the following line for 16 bit version
#define WIN95		1				// 32 bit code

#ifdef	WIN95						// 32 bit code
*/
#undef  huge
#define huge      
#undef  _export
#define _export

#include "Big.h"

//#endif

//#define DOUBLE		double

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

#define TRUE		1
#define FALSE		0
#define EVER		(;;)

#define	MAXHORIZONTAL	40000
#define	MAXVERTICAL	40000
#define	VGA_PAL_SIZE	768
#define	EGA_PAL_SIZE	16
#define VGA_COLOURS	256

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

#define	MAXDATALINE	SIZEOF_BF_VARS * 3 + PRECISION_FACTOR	// maximum frame data length

/*
struct	HenonStr		// hold variables for Henon Mapping
{
    double  a;
    double  xStart;
    double  yStart;
    DWORD   points;
};
*/
#define	FixedGlobalAlloc(n) 	(char *)GlobalAlloc(GMEM_FIXED, n)
#define	FixedGlobalFree(p)	GlobalFree(p)
#define	FixedGlobalRealloc(p,n) GlobalRealloc(p,n)

#define	RGB_RED			0
#define	RGB_GREEN		1
#define	RGB_BLUE		2
#define	RGB_SIZE		3

/* Macro to restrict a given value to an upper or lower boundary value */
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* Macro to align given value to the closest DWORD (unsigned long ) */
#define ALIGNULONG(i)   ((i+3)/4*4)

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)   ((i+31)/32*4)

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

