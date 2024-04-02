/* FRACTINT.H - common structures and values for the FRACTINT routines */

#ifndef FRACTINT_H
#define FRACTINT_H

#ifndef WIN32
typedef BYTE BOOLEAN;
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifndef C6
#define _fastcall       /* _fastcall is a Microsoft C6.00 extension */
#endif

#ifndef XFRACT
#define ftimex ftime
typedef int SEGTYPE;
typedef unsigned USEGTYPE;
#ifdef __TURBOC__
#   define _bios_printer(a,b,c)   biosprint((a),(c),(b))
#   define _bios_serialcom(a,b,c) bioscom((a),(c),(b))
#else
#ifndef __WATCOMC__
#ifndef MK_FP
#   define MK_FP(seg,off) (VOIDFARPTR )( (((long)(seg))<<16) | \
                                          ((unsigned)(off)) )
#endif
#endif
#endif
#else
typedef char * SEGTYPE;
typedef char * USEGTYPE;
#   define MK_FP(seg,off) (VOIDFARPTR )(seg+off)
#include <sys/types.h> /* need size_t */
#endif

#ifndef XFRACT
#define clock_ticks() clock()
#endif

#ifdef XFRACT
#define difftime(now,then) ((now)-(then))
#endif

/* for gotos in former FRACTINT.C pieces */
#define RESTART           1
#define IMAGESTART        2
#define RESTORESTART      3
#define CONTINUE          4

/* these are used to declare arrays for file names */
#define FILE_MAX_PATH  256       /* max length of path+filename  */
#define FILE_MAX_DIR   256       /* max length of directory name */
#define FILE_MAX_DRIVE  3       /* max length of drive letter   */

#define FILE_MAX_FNAME  9       /* max length of filename       */
#define FILE_MAX_EXT    5       /* max length of extension      */

#define MAXMAXLINELENGTH  128   /* upper limit for maxlinelength for PARs */
#define MINMAXLINELENGTH  40    /* lower limit for maxlinelength for PARs */

#define MSGLEN 80               /* handy buffer size for messages */
#define MAXCMT 57               /* length of par comments       */
#define MAXPARAMS 10            /* maximum number of parameters */
#define MAXPIXELS   32767       /* Maximum pixel count across/down the screen */
#define OLDMAXPIXELS 2048       /* Limit of some old fixed arrays */
#define MINPIXELS 10            /* Minimum pixel count across/down the screen */
#define DEFAULTASPECT ((float)0.75)/* Assumed overall screen dimensions, y/x  */
#define DEFAULTASPECTDRIFT ((float)0.02) /* drift of < 2% is forced to 0% */


#define ITEMNAMELEN 18   /* max length of names in .frm/.l/.ifs/.fc */

#define NUMGENES 21

struct moreparams
{
   int      type;                       /* index in fractalname of the fractal */
   char     far *param[MAXPARAMS-4];    /* name of the parameters */
   double   paramvalue[MAXPARAMS-4];    /* default parameter values */
};

typedef struct moreparams far       MOREPARAMS;

struct alternatemathstuff
{
   int type;                    /* index in fractalname of the fractal */
   int math;                    /* kind of math used */
#ifdef XFRACT
   int (*orbitcalc)();  /* function that calculates one orbit */
#else
   int (*orbitcalc)(void);      /* function that calculates one orbit */
#endif
   int (*per_pixel)(void);      /* once-per-pixel init */
   int (*per_image)(void);      /* once-per-image setup */
};

#ifdef WIN32		     /* PHD 13/4/02 ALTERNATE is a key word in MS C++ */
typedef struct alternatemathstuff ALTERNATE1;
#else
typedef struct alternatemathstuff ALTERNATE;
#endif

/* defines for symmetry */
#define  NOSYM          0
#define  XAXIS_NOPARM  -1
#define  XAXIS          1
#define  YAXIS_NOPARM  -2
#define  YAXIS          2
#define  XYAXIS_NOPARM -3
#define  XYAXIS         3
#define  ORIGIN_NOPARM -4
#define  ORIGIN         4
#define  PI_SYM_NOPARM -5
#define  PI_SYM         5
#define  XAXIS_NOIMAG  -6
#define  XAXIS_NOREAL   6
#define  NOPLOT        99
#define  SETUP_SYM    100


#ifndef sqr
#define sqr(x) ((x)*(x))
#endif

#ifdef WIN32
#include "cmplx.h"
#endif
//typedef  _LCMPLX LCMPLX;

/* 3D stuff - formerly in 3d.h */
#ifndef dot_product
#define dot_product(v1,v2)  ((v1)[0]*(v2)[0]+(v1)[1]*(v2)[1]+(v1)[2]*(v2)[2])  /* TW 7-09-89 */
#endif

#define    CMAX    4   /* maximum column (4 x 4 matrix) */
#define    RMAX    4   /* maximum row    (4 x 4 matrix) */
#define    DIM     3   /* number of dimensions */

typedef double MATRIX [RMAX] [CMAX];  /* matrix of doubles */
typedef int   IMATRIX [RMAX] [CMAX];  /* matrix of ints    */
typedef long  LMATRIX [RMAX] [CMAX];  /* matrix of longs   */

/* A MATRIX is used to describe a transformation from one coordinate
system to another.  Multiple transformations may be concatenated by
multiplying their transformation matrices. */

typedef double VECTOR [DIM];  /* vector of doubles */
typedef int   IVECTOR [DIM];  /* vector of ints    */
typedef long  LVECTOR [DIM];  /* vector of longs   */

/* A VECTOR is an array of three coordinates [x,y,z] representing magnitude
and direction. A fourth dimension is assumed to always have the value 1, but
is not in the data structure */

#ifdef PI
#undef PI
#endif
#define PI 3.14159265358979323846
#define SPHERE    init3d[0]             /* sphere? 1 = yes, 0 = no  */
#define ILLUMINE  (FILLTYPE>4)  /* illumination model       */

#define MAXCALCWORK 12

#define MAX_JUMPS 200  /* size of JUMP_CONTROL array */

typedef struct frm_jmpptrs_st {
   int      JumpOpPtr;
   int      JumpLodPtr;
   int      JumpStoPtr;
} JUMP_PTRS_ST;

typedef struct frm_jump_st {
   int      type;
   JUMP_PTRS_ST ptrs;
   int      DestJumpIndex;
} JUMP_CONTROL_ST;

struct baseunit { /* smallest part of a fractint 'gene' */
   void *addr               ; /* address of variable to be referenced */
   void (*varyfunc)(struct baseunit*,int,int); /* pointer to func used to vary it */
                              /* takes random number and pointer to var*/
   int mutate ;  /* flag to switch on variation of this variable */
                  /* 0 for no mutation, 1 for x axis, 2 for y axis */
                  /* in steady field maps, either x or y=yes in random modes*/ 
   char name[16]; /* name of variable (for menu ) */
   char level;    /* mutation level at which this should become active */
};

typedef struct baseunit    GENEBASE;
#define sign(x) (((x) < 0) ? -1 : ((x) != 0)  ? 1 : 0)

/* 
 * The following typedefs allow declaring based data
 * types that are stored in the code segment under MSC,
 * and thus may be overlaid. Use only for constant data.
 * Be sure to use the data right away, since arrays thus
 * declared do not exist when the overlay they belong to
 * is swapped out.
 */

#ifdef	WIN32

typedef char FCODE;
typedef FCODE * PFCODE;
typedef BYTE BFCODE;
typedef short SIFCODE;
typedef short USFCODE;
typedef int IFCODE;
typedef unsigned int UIFCODE;
typedef long LFCODE;
typedef unsigned long ULFCODE;
typedef double DFCODE;
#else

#if (_MSC_VER >= 700)
typedef char __based(__segname("_CODE")) FCODE;
#else
typedef char far FCODE;
#endif

/* pointer to FCODE */
#if (_MSC_VER >= 700)
typedef FCODE * __based(__segname("_CODE")) PFCODE;
#else
typedef FCODE * PFCODE;
#endif

#if (_MSC_VER >= 700)
typedef BYTE __based(__segname("_CODE")) BFCODE;
#else
typedef BYTE far BFCODE;
#endif

#if (_MSC_VER >= 700)
typedef short __based(__segname("_CODE")) SIFCODE;
#else
typedef short far SIFCODE;
#endif

#if (_MSC_VER >= 700)
typedef short __based(__segname("_CODE")) USFCODE;
#else
typedef short far USFCODE;
#endif

#if (_MSC_VER >= 700)
typedef int __based(__segname("_CODE")) IFCODE;
#else
typedef int far IFCODE;
#endif

#if (_MSC_VER >= 700)
typedef unsigned int __based(__segname("_CODE")) UIFCODE;
#else
typedef unsigned int far UIFCODE;
#endif

#if (_MSC_VER >= 700)
typedef long __based(__segname("_CODE")) LFCODE;
#else
typedef long far LFCODE;
#endif

#if (_MSC_VER >= 700)
typedef unsigned long __based(__segname("_CODE")) ULFCODE;
#else
typedef unsigned long far ULFCODE;
#endif

#if (_MSC_VER >= 700)
typedef double __based(__segname("_CODE")) DFCODE;
#else
typedef double far DFCODE;
#endif
#endif
#endif


#if _MSC_VER == 800
#ifndef FIXTAN_DEFINED
/* !!!!! stupid MSVC tan(x) bug fix !!!!!!!!            */
/* tan(x) can return -tan(x) if -pi/2 < x < pi/2       */
/* if tan(x) has been called before outside this range. */
double fixtan( double x );
#define tan fixtan
#define FIXTAN_DEFINED
#endif
#endif
