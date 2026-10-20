/*
    MANPWIN.h - Definitions for WinMain and message processing loop.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

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

// Per-pixel classifications used by Pixel, Perturbation and Slope.
// These are stored in PixelFlags; wpixels remains available for numeric iteration, filter and Forward Difference values.
#define PIXEL_NORMAL	0x00
#define PIXEL_INSIDE	0x01
#define PIXEL_SPECIAL	0x02

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
#define MERGINGCLOUDDENSITY 6

// affine structure used by Lorenz (as opposed to affine transformation - see below)
struct affine	// legacy Lorenz/Fractint screen conversion
    {
    // weird order so a,b,e and c,d,f are vectors
    double a;
    double b;
    double e;
    double c;
    double d;
    double f;
    };


struct FNCT_LIST
    {
    char *s;
    void(**ptr)(void);
    };

enum	ProcessType	{ STANDARD, INITANIM, RUNANIM, EVOLUTION, MORPHING };
enum	AxisDisplayType { OFF, DISPLAYALL, DISPLAYSINGLE };

// lots of transformation parameters
enum TRANSFORMTYPE
    {
    TRANSFORM_NONE = 0,

    // Standard transformations
    TRANSFORM_INVERSION,
    TRANSFORM_COMPLEX_LOG,
    TRANSFORM_FOLD_X,
    TRANSFORM_FOLD_Y,
    TRANSFORM_FOLD_XY,
    TRANSFORM_FOLD_DIAGONAL,
    TRANSFORM_FOLD_DIAGONALS,
    TRANSFORM_RADIAL_BULGE,
    TRANSFORM_RADIAL_PINCH,
    TRANSFORM_CIRCLE_FOLD,
    TRANSFORM_RADIAL_CLAMP,
    TRANSFORM_RADIAL_COMPRESS,
    TRANSFORM_COMPLEX_EXP,          // 13
    TRANSFORM_COMPLEX_SIN,          // 14

    // Special transformations
    TRANSFORM_AFFINE,               // 15
    TRANSFORM_MOBIUS,               // 16
    TRANSFORM_JOUKOWSKI,            // 17
    TRANSFORM_TWIST,                // 18

    // 19 reserved for future named transformation

    TRANSFORM_POWER_BASE = 20
    };

const int FIRST_TRANSFORM_POWER = 2;

/**************************************************************************
    Transformation PAR file errors
**************************************************************************/

enum TRANSFORMPARERROR
    {
    TRANSFORMPAR_OK = 0,
    TRANSFORMPAR_NO_TYPE = -1,
    TRANSFORMPAR_NORMAL = -2,
    TRANSFORMPAR_AFFINE = -3,
    TRANSFORMPAR_MOBIUS = -4,
    TRANSFORMPAR_JOUKOWSKI = -5,
    TRANSFORMPAR_TWIST = -6
    };

enum TRANSFORMDLGMODE
    {
    TRANSFORMDLG_NORMAL = 0,
    TRANSFORMDLG_ANIMATION,
    };

/**************************************************************************
    Affine transformation parameters
    Initialised with identity coefficients
**************************************************************************/

struct AffineTransformParams	// generic six-coefficient double affine transform
    {
    double a = 1.0;
    double b = 0.0;
    double e = 0.0;
    double c = 0.0;
    double d = 1.0;
    double f = 0.0;
    };

/**************************************************************************
    View affine transformation parameters

    Used for view geometry such as arbitrary rotation, skew and
    magnification.  These parameters are not part of the user-selected
    fractal transformation and are not edited by the affine transform
    dialogue.
**************************************************************************/

template<typename TReal>
struct ViewAffineParamsT    // ManpWIN view geometry, templated for double/DD/QD/Big
    {
    TReal a = 1.0;
    TReal b = 0.0;
    TReal e = 0.0;
    TReal c = 0.0;
    TReal d = 1.0;
    TReal f = 0.0;
    };

/**************************************************************************
    Mobius transformation parameters
    Initialised with identity coefficients
**************************************************************************/

struct MobiusTransformParams
    {
    double aReal = 1.0;
    double aImag = 0.0;

    double bReal = 0.0;
    double bImag = 0.0;

    double cReal = 0.0;
    double cImag = 0.0;

    double dReal = 1.0;
    double dImag = 0.0;
    };

/**************************************************************************
    Joukowski transformation parameters
**************************************************************************/

struct JoukowskiTransformParams
    {
    double kReal = 1.0;
    double kImag = 0.0;
    };

/**************************************************************************
    Twist transformation parameters
**************************************************************************/

struct TwistTransformParams
    {
    double Strength = 1.0;
    double Radius = 1.0;
    double CentreX = 0.0;
    double CentreY = 0.0;
    };

/**************************************************************************
    Return the integer power represented by a power transformation
**************************************************************************/

inline int GetTransformPower(TRANSFORMTYPE TransformType)
    {
    return (int)TransformType - (int)TRANSFORM_POWER_BASE + FIRST_TRANSFORM_POWER;
    }

/**************************************************************************
    Return the transformation type represented by an integer power
**************************************************************************/

inline TRANSFORMTYPE MakePowerTransformType(int Power)
    {
    return (TRANSFORMTYPE) ((int)TRANSFORM_POWER_BASE + Power - FIRST_TRANSFORM_POWER);
    }

/**************************************************************************
    What kind of Julia animation do we want?
**************************************************************************/

enum JULIA_ANIM_FRACTAL
    {
    JULIA_ANIM_NORMAL,
    JULIA_ANIM_INVERSE
    };

enum REALTIMEJULIAMODE
    {
    RTJ_OFF = 0,
    RTJ_JULIA,
    RTJ_INVERSE_JULIA
    };

