/*
FRACTALS.CPP, FRACTALP.C and CALCFRAC.C actually calculate the fractal
images (well, SOMEBODY had to do it!).  The modules are set up so that
all logic that is independent of any fractal-specific code is in
CALCFRAC.C, the code that IS fractal-specific is in FRACTALS.C, and the
struscture that ties (we hope!) everything together is in FRACTALP.C.
Original author Tim Wegner, but just about ALL the authors have
contributed SOME code to this routine at one time or another, or
contributed to one of the many massive restructurings.

The Fractal-specific routines are divided into three categories:

1. Routines that are called once-per-orbit to calculate the orbit
   value. These have names like "XxxxFractal", and their function
   pointers are stored in fractalspecific[type].orbitcalc. EVERY
   New fractal type needs one of these. Return 0 to continue iterations,
   1 if we're done. Results for integer fractals are left in 'lnew.x' and
   'lnew.y', for floating point fractals in 'New.x' and 'New.y'.

2. Routines that are called once per pixel to set various variables
   prior to the orbit calculation. These have names like xxx_per_pixel
   and are fairly generic - chances are one is right for your New type.
   They are stored in fractalspecific[type].per_pixel.

3. Routines that are called once per screen to set various variables.
   These have names like XxxxSetup, and are stored in
   fractalspecific[type].per_image.

4. The main fractal routine. Usually this will be StandardFractal(),
   but if you have written a stand-alone fractal routine independent
   of the StandardFractal mechanisms, your routine name goes here,
   stored in fractalspecific[type].calctype.per_image.

Adding a New fractal type should be simply a matter of adding an item
to the 'fractalspecific' structure, writing (or re-using one of the existing)
an appropriate setup, per_image, per_pixel, and orbit routines.

--------------------------------------------------------------------   */

/*
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <windows.h>
#include "Complex.h"
#include ".\parser\mpmath.h"
#include "manp.h"
#include "bif.h"

extern	double	xgap;			// gap between pixels
extern	double	ygap;			// gap between pixels
extern	double	hor;			// horizontal address
extern	double	vert;			// vertical address
extern	double	mandel_width;		// width of display

extern	Complex	z, c, q;

extern int rflag, rseed;		// random number seed

double tempsqrx, tempsqry;
Complex tmp, New;
Complex parm;


void(*dtrig0)(void) = dStkSin;
void(*dtrig1)(void) = dStkSqr;
void(*dtrig2)(void) = dStkSinh;
void(*dtrig3)(void) = dStkCosh;


int	debugflag;			// for debugging purposes

extern void	noplot(WORD, WORD, DWORD);
extern	void	genline(int, int, int, int, const DWORD);


void FPUsincos(double *Angle, double *Sin, double *Cos)
    {
    *Sin = sin(*Angle);
    *Cos = cos(*Angle);
    }

Complex ComplexPower(Complex x, Complex y)
    {
    Complex z;
    z = x^y;
    return z;
    }

int	NullSetup(void)				// sometimes we just don't want to do anything 
    {
    return 1;
    }
*/

#ifdef SPLAT

//extern	CFract	Fractal;		// current fractal stuff
extern	CTrueCol    TrueCol;		// palette info

#define NEWTONDEGREELIMIT  100
#undef	ALLOW_LONG			// Float only at this stage

//extern struct complex  initorbit;
//extern struct lcomplex linitorbit;
extern char useinitorbit;
extern double fgLimit;
//extern int distest;

extern void (*ltrig0)();
extern void (*ltrig1)();
extern void (*ltrig2)();
extern void (*ltrig3)();

BYTE trigndx[] = {SIN,SQR,SINH,COSH};

void (*dtrig0)(void) = dStkSin;
void (*dtrig1)(void) = dStkSqr;
void (*dtrig2)(void) = dStkSinh;
void (*dtrig3)(void) = dStkCosh;

int    MandelFnSetup(void);

//extern void (*dtrig0)();
//extern void (*dtrig1)();
//extern void (*dtrig2)();
//extern void (*dtrig3)();

/* -------------------------------------------------------------------- */
/*   The following #defines allow the complex transcendental functions	*/
/*   in parser.c to be used here thus avoiding duplicated code. 	*/
/* -------------------------------------------------------------------- */

//typedef  struct complex _CMPLX;

#define CMPLXmod(z)	  (sqr((z).x)+sqr((z).y))
#define CMPLXconj(z)	((z).y =  -((z).y))

#define  CMPLXtrig0(arg,out) Arg1->d = (arg); dtrig0(); (out)=Arg1->d
#define  CMPLXtrig1(arg,out) Arg1->d = (arg); dtrig1(); (out)=Arg1->d
#define  CMPLXtrig2(arg,out) Arg1->d = (arg); dtrig2(); (out)=Arg1->d
#define  CMPLXtrig3(arg,out) Arg1->d = (arg); dtrig3(); (out)=Arg1->d

#define CMPLXsin(arg,out)    Arg1->d = (arg); dStkSin();  (out) = Arg1->d
#define CMPLXcos(arg,out)    Arg1->d = (arg); dStkCos();  (out) = Arg1->d
#define CMPLXsinh(arg,out)   Arg1->d = (arg); dStkSinh(); (out) = Arg1->d
#define CMPLXcosh(arg,out)   Arg1->d = (arg); dStkCosh(); (out) = Arg1->d
#define CMPLXlog(arg,out)    Arg1->d = (arg); dStkLog();  (out) = Arg1->d
#define CMPLXexp(arg,out)    FPUcplxexp(&(arg), &(out))
/*
#define CMPLXsqr(arg,out)    Arg1->d = (arg); dStkSqr();  (out) = Arg1->d
*/
#define CMPLXsqr(arg,out)    \
   (out).x = sqr((arg).x) - sqr((arg).y);\
   (out).y = ((arg).x+(arg).x) * (arg).y
#define CMPLXsqr_old(out)	\
   (out).y = (z.x+z.x) * z.y;\
   (out).x = tempsqrx - tempsqry

#define CMPLXpwr(arg1,arg2,out)   (out)= ComplexPower((arg1), (arg2))
#define CMPLXmult1(arg1,arg2,out)    Arg2->d = (arg1); Arg1->d = (arg2);\
	 dStkMul(); Arg1++; Arg2++; (out) = Arg2->d
#define CMPLXmult(arg1,arg2,out)  \
	{\
	   Complex TmP;\
	   TmP.x = (arg1).x*(arg2).x - (arg1).y*(arg2).y;\
	   TmP.y = (arg1).x*(arg2).y + (arg1).y*(arg2).x;\
	   (out) = TmP;\
	 }
#define CMPLXadd(arg1,arg2,out)    \
    (out).x = (arg1).x + (arg2).x; (out).y = (arg1).y + (arg2).y
#define CMPLXsub(arg1,arg2,out)    \
    (out).x = (arg1).x - (arg2).x; (out).y = (arg1).y - (arg2).y
#define CMPLXtimesreal(arg,real,out)   \
    (out).x = (arg).x*(real);\
    (out).y = (arg).y*(real)

#define CMPLXrecip(arg,out)    \
   { double denom; denom = sqr((arg).x) + sqr((arg).y);\
     if(denom==0.0) {(out).x = 1.0e10;(out).y = 1.0e10;}else\
    { (out).x =  (arg).x/denom;\
     (out).y = -(arg).y/denom;}}

extern int xshift, yshift;
long Exp086(long);
double fmod(double,double);
extern int biomorph;
extern int forcesymmetry;
extern int symmetry;
//LCMPLX lcoefficient,lold,lnew,lparm, linit,ltmp,ltmp2,lparm2;
long	ltempsqrx,ltempsqry;
extern	int decomp[];
extern	double param[];
extern	int potflag;				/* potential enabled? */
extern	double xxmin,xxmax,yymin,yymax;		/* corners */
extern	int overflow;
extern	int integerfractal;		/* TRUE if fractal uses integer math */
extern	int	period_level;		/* 0 for no periodicity checking */
//extern	BYTE	degree;			/* power */
extern	BYTE	juliaflag;		/* Julia implementation of fractal */
extern	int	xdots, ydots;
extern	long	iteration;

int maxcolor;
int root, basin;
double floatmin,floatmax;
double roverd, d1overd/*, threshold*/;
Complex tmp2;
Complex initial,tmp,/*old,*/New,saved/*,ComplexPower()*/;
Complex	staticroots[16]; /* roots array for degree 16 or less */
Complex	*roots = staticroots;
struct MPC	*MPCroots;
extern int row, col;
extern long color;
extern int invert;
//double dx0[MAXHORIZONTAL], dy0[MAXHORIZONTAL];
//double dx1[MAXHORIZONTAL], dy1[MAXHORIZONTAL];
long FgHalf;

Complex one;
Complex pwr;
Complex Coefficient;

//extern int	inside_colour; 			/* "inside" color to use    */
//extern int	outside;			/* "outside" color to use   */
extern	int	finattract;
extern	WORD	type;			/* fractal type */



//extern	long	far *lx0, far *ly0;	/* X, Y points */
//extern	long	far *lx1, far *ly1;	/* X, Y points */
//extern	long	delx,dely;		/* X, Y increments */
//extern	long	delmin; 		/* min(max(dx),max(dy) */
//double	ddelmin;			/* min(max(dx),max(dy) */
//extern	long	fudge;			/* fudge factor (2**n) */
//extern	int	bitshift;		/* bit shift for fudge */
int	bitshiftless1;			/* bit shift less 1 */

#ifndef sqr
#define sqr(x) ((x)*(x))
#endif

//#ifndef lsqr
//#define lsqr(x) (multiply((x),(x),bitshift))
//#endif

#define modulus(z)	 (sqr((z).x)+sqr((z).y))
#define conjugate(pz)	((pz)->y = 0.0 - (pz)->y)
#define distance(z1,z2)  (sqr((z1).x-(z2).x)+sqr((z1).y-(z2).y))
#define pMPsqr(z) (*pMPmul((z),(z)))
#define MPdistance(z1,z2)  (*pMPadd(pMPsqr(*pMPsub((z1).x,(z2).x)),pMPsqr(*pMPsub((z1).y,(z2).y))))

double twopi = PI*2.0;
static int c_exp;


/* These are local but I don't want to pass them as parameters */
Complex lambda;
double magnitude, rqlim = 64.0, rqlim2 = 64.0;
Complex parm,parm2;
Complex *floatparm;
//LCMPLX *longparm; /* used here and in jb.c */
extern int (*calctype)();
extern unsigned long lm;		/* magnitude limit (CALCMAND) */
extern void	cpower(Complex *, Complex *, int);
extern void	complex_mult(Complex *, Complex, Complex);
extern	void(*plot)(WORD, WORD, DWORD);

/* -------------------------------------------------------------------- */
/*		These variables are external for speed's sake only      */
/* -------------------------------------------------------------------- */

double sinx,cosx,sinhx,coshx;
double siny,cosy,sinhy,coshy;
double tmpexp;

double foldxinitx,foldyinity,foldxinity,foldyinitx;
#ifdef	ALLOW_LONG			// Float only at this stage
long oldxinitx,oldyinity,oldxinity,oldyinitx;
long longtmp;
extern long lmagnitud, llimit, llimit2, l16triglim;
#endif
int		periodicitycheck;
extern char floatflag;

//extern int StandardFractal();
//extern int NewtonFractal2(); /* Lee Crocker's Newton code */
//
///* these are in mpmath_c.c */
////extern int ComplexNewtonSetup(void);
//extern int ComplexNewton(void), ComplexBasin(void), MarksCplxMand(void);
//extern int MarksCplxMandperp(void);
//
///* these are in (I think) JB.C */
//extern int Std4dFractal(), JulibrotSetup(), jb_per_pixel();
//
//extern int Lsystem();
//extern int lya_setup(void);
//extern int lyapunov(void);
//
///* temporary variables for trig use */
//long lcosx, lcoshx, lsinx, lsinhx;
//long lcosy, lcoshy, lsiny, lsinhy;

/*
**  details of finite attractors (required for Magnet Fractals)
**  (can also be used in "coloring in" the lakes of Julia types)
*/
extern	      int      attractors; /* number of finite attractors   */
extern Complex  attr[];	   /* finite attractor values (f.p) */

/*
**  pre-calculated values for fractal types Magnet2M & Magnet2J
*/
Complex	T_Cm1;	      /* 3 * (floatparm - 1)		    */
Complex	T_Cm2;	      /* 3 * (floatparm - 2)		    */
Complex	T_Cm1Cm2;     /* (floatparm - 1) * (floatparm - 2) */

//extern	Complex	z, q, c;


/**************************************************************************
	Invert fractal
**************************************************************************/

Complex	invertz2(Complex  & Cmplx1)

    {
    static	Complex	temp;
    double	tempsqrx;

    temp.x = Cmplx1.x;
    temp.y = Cmplx1.y;
    temp.x -= f_xcenter; temp.y -= f_ycenter;	// Normalize values to center of circle

    tempsqrx = sqr(temp.x) + sqr(temp.y);	// Get old radius
    if (fabs(tempsqrx) > FLT_MIN)
	tempsqrx = f_radius / tempsqrx;
    else
	tempsqrx = FLT_MAX;			// a big number, but not TOO big
    temp.x *= tempsqrx;
    temp.y *= tempsqrx;				// Perform inversion
    temp.x += f_xcenter;
    temp.y += f_ycenter;			// Renormalize
    return  temp;
    }

/* -------------------------------------------------------------------- */
/*		Start of fractal routines											*/
/* -------------------------------------------------------------------- */

void FloatPreCalcMagnet2() /* precalculation for Magnet2 (M & J) for speed */
    {
    T_Cm1.x = floatparm->x - 1.0;   T_Cm1.y = floatparm->y;
    T_Cm2.x = floatparm->x - 2.0;   T_Cm2.y = floatparm->y;
    T_Cm1Cm2.x = (T_Cm1.x * T_Cm2.x) - (T_Cm1.y * T_Cm2.y);
    T_Cm1Cm2.y = (T_Cm1.x * T_Cm2.y) + (T_Cm1.y * T_Cm2.x);
    T_Cm1.x += T_Cm1.x + T_Cm1.x;   T_Cm1.y += T_Cm1.y + T_Cm1.y;
    T_Cm2.x += T_Cm2.x + T_Cm2.x;   T_Cm2.y += T_Cm2.y + T_Cm2.y;
    }


/* -------------------------------------------------------------------- */
/*		Stand-alone routines											*/
/* -------------------------------------------------------------------- */

//extern int orbit2dfloat();
//extern int orbit2dlong();
//extern int kamtorusfloatorbit();
//extern int kamtoruslongorbit();
//
///* functions defined elswhere needed for fractalspecific */
//extern int hopalong2dfloatorbit();
//extern int martin2dfloatorbit();
//extern int orbit3dfloat();
//extern int orbit3dlong();
//extern int lorenz3dlongorbit();
//extern int orbit3dlongsetup();
//extern int lorenz3dfloatorbit();
//extern int lorenz3d1floatorbit();
//extern int lorenz3d3floatorbit();
//extern int lorenz3d4floatorbit();
//extern int orbit3dfloatsetup();
//extern int rosslerfloatorbit();
//extern int rosslerlongorbit();
//extern int henonfloatorbit();
//extern int henonlongorbit();
//extern int pickoverfloatorbit();
//extern int gingerbreadfloatorbit();
//extern int diffusion();
//extern int plasma();
//extern int test();
//extern int ifs(HWND);
//extern int Bifurcation(void);
//extern int BifurcVerhulstTrig(void);
//extern int LongBifurcVerhulst(void);
//extern int BifurcLambdaTrig(void);
//extern int LongBifurcLambda(void);
//extern int BifurcAddTrigPi(void);
//extern int LongBifurcAddSinPi(void);
//extern int BifurcSetTrigPi(void);
//extern int LongBifurcSetSinPi(void);
//extern int BifurcStewartTrig(void);
//extern int LongBifurcStewart(void);
//extern int popcorn(void);
extern void cdecl FPUcplxexp387(Complex *, Complex *);
extern void cdecl FPUcplxlog(Complex *, Complex *);
//extern void cdecl FPUcplxmul(Complex *, Complex *, Complex *);
extern	Complex	invertz2(Complex  &);


Complex ComplexPower(Complex x, Complex y)
    {
    Complex z, cLog, t = {0,0};
    double e2x, siny, cosy;

    FPUcplxlog(&x, &cLog);
//    FPUcplxmul(&cLog, &y, &t);
    t =  y* cLog;

//   if(fpu == 387)
//      FPUcplxexp387(&t, &z);
//   else 
	{
	e2x = exp(t.x);
	FPUsincos(&t.y, &siny, &cosy);
	z.x = e2x * cosy;
	z.y = e2x * siny;
	}
    return(z);
    }

/* -------------------------------------------------------------------- */
/*		Bailout Routines Macros 				*/
/* -------------------------------------------------------------------- */

static int near floatbailout(void)
{
    z = New;			// to allow filters to work
    if ((magnitude = (tempsqrx = sqr(New.x)) + (tempsqry = sqr(New.y))) >= rqlim)
       {
//       z = old;
       return(1);
       }
    z = New;
   return(0);
}

#define FLOATTRIGBAILOUT()  \
   if (fabs(z.y) >= rqlim2) return(1);

#define LONGTRIGBAILOUT()  \
   if(labs(lold.y) >= llimit2 || overflow) { overflow=0;return(1);}

#define LONGXYTRIGBAILOUT()  \
   if(labs(lold.x) >= llimit2 || labs(lold.y) >= llimit2 || overflow)\
	{ overflow=0;return(1);}

#define FLOATXYTRIGBAILOUT()  \
   if (fabs(z.x) >= rqlim2 || fabs(z.y) >= rqlim2) return(1);

#define FLOATHTRIGBAILOUT()  \
   if (fabs(z.x) >= rqlim2) return(1);

#define LONGHTRIGBAILOUT()  \
   if(labs(lold.x) >= llimit2 || overflow) { overflow=0;return(1);}

#define TRIG16CHECK(X)	\
      if(labs((X)) > l16triglim || overflow) { overflow=0;return(1);}

#define FLOATEXPBAILOUT()  \
   if (fabs(z.y) >= 1.0e8) return(1);\
   if (fabs(z.x) >= 6.4e2) return(1);

//#define LONGEXPBAILOUT()  \
//   if (labs(lold.y) >= (1000L<<bitshift)) return(1);\
//   if (labs(lold.x) >=	  (8L<<bitshift)) return(1);

//#if 0
/* this define uses usual trig instead of fast trig */
#define FPUsincos(px,psinx,pcosx) \
   *(psinx) = sin(*(px));\
   *(pcosx) = cos(*(px));

#define FPUsinhcosh(px,psinhx,pcoshx) \
   *(psinhx) = sinh(*(px));\
   *(pcoshx) = cosh(*(px));
//#endif

/* -------------------------------------------------------------------- */
/*		Fractal (once per iteration) routines			*/
/* -------------------------------------------------------------------- */
static double xt, yt, t2;

int		z_to_the_z(Complex *z, Complex *out)
{
    static Complex tmp1,tmp2;
    /* raises complex z to the z power */
    int errno_xxx;
    errno_xxx = 0;

    if(fabs(z->x) < DBL_EPSILON) return(-1);

    /* log(x + iy) = 1/2(log(x*x + y*y) + i(arc_tan(y/x)) */
    tmp1.x = .5*log(sqr(z->x)+sqr(z->y));

    /* the fabs in next line added to prevent discontinuity in image */
    tmp1.y = atan(fabs(z->y/z->x));

    /* log(z)*z */
    tmp2.x = tmp1.x * z->x - tmp1.y * z->y;
    tmp2.y = tmp1.x * z->y + tmp1.y * z->x;

    /* z*z = e**(log(z)*z) */
    /* e**(x + iy) =  e**x * (cos(y) + isin(y)) */

    tmpexp = exp(tmp2.x);

    FPUsincos(&tmp2.y,&siny,&cosy);
    out->x = tmpexp*cosy;
    out->y = tmpexp*siny;
    return(errno_xxx);
}

/* Distance of complex z from unit circle */
#define DIST1(z) (((z).x-1.0)*((z).x-1.0)+((z).y)*((z).y))
#define LDIST1(z) (lsqr((((z).x)-fudge)) + lsqr(((z).y)))

#ifdef NEWTON
//complex_mult(_CMPLX *pz, _CMPLX arg1,_CMPLX arg2);
int		complex_div(Complex *pz, Complex arg1, Complex arg2);

extern long threshold;			/* maximum iterations (steps) */
#endif


/*
int	Barnsley1FPFractal(void)
{
   // Barnsley's Mandelbrot type M1 from "Fractals Everywhere" by Michael Barnsley, p. 322
   // note that fast >= 287 equiv in fracsuba.asm must be kept in step 

    // calculate intermediate products
   floatparm = &parm;
   foldxinitx = z.x * floatparm->x;
   foldyinity = z.y * floatparm->y;
   foldxinity = z.x * floatparm->y;
   foldyinitx = z.y * floatparm->x;
   // orbit calculation 
   if(z.x >= 0)
	{
       New.x = (foldxinitx - floatparm->x - foldyinity + param[0]);
       New.y = (foldyinitx - floatparm->y + foldxinity + param[1]);
       }
   else
       {
       New.x = (foldxinitx + floatparm->x - foldyinity + param[0]);
       New.y = (foldyinitx + floatparm->y + foldxinity + param[1]);
       }
   return(floatbailout());
}

int	Barnsley2FPFractal(void)
{
   // An unnamed Mandelbrot/Julia function from "Fractals Everywhere" by Michael Barnsley, p. 331, example 4.2

   // calculate intermediate products
   foldxinitx = z.x * floatparm->x;
   foldyinity = z.y * floatparm->y;
   foldxinity = z.x * floatparm->y;
   foldyinitx = z.y * floatparm->x;

   // orbit calculation
   if(foldxinity + foldyinitx >= 0)
   {
       New.x = foldxinitx - floatparm->x - foldyinity + param[0];
       New.y = foldyinitx - floatparm->y + foldxinity + param[1];
   }
   else
   {
       New.x = foldxinitx + floatparm->x - foldyinity + param[0];
       New.y = foldyinitx + floatparm->y + foldxinity + param[1];
   }
   return(floatbailout());
}
*/
/*
int	JuliafpFractal(void)
{
   // floating point version of classical Mandelbrot/Julia
   /* note that fast >= 287 equiv in fracsuba.asm must be kept in step
   New.x = tempsqrx - tempsqry + floatparm->x;
   New.y = 2.0 * z.x * z.y + floatparm->y;
   return(floatbailout());
}

static double f(double x, double y)
{
   return(x - x*y);
}

static double g(double x, double y)
{
   return(-y + x*y);
}


int	LambdaFPFractal(void)
{
   // variation of classical Mandelbrot/Julia
   // note that fast >= 287 equiv in fracsuba.asm must be kept in step




//   floatparm->x = 0.85;
//   floatparm->y = 0.6;



   tempsqrx = z.x - tempsqrx + tempsqry;
   tempsqry = -(z.y * z.x);
   tempsqry += tempsqry + z.y;

   New.x = floatparm->x * tempsqrx - floatparm->y * tempsqry;
   New.y = floatparm->x * tempsqry + floatparm->y * tempsqrx;
   return(floatbailout());
}


int	SierpinskiFPFractal(void)
{
   // following code translated from basic - see "Fractals Everywhere" by Michael Barnsley, p. 251, Program 7.1.1

   New.x = z.x + z.x;
   New.y = z.y + z.y;
   if(z.y > 0.5)
      New.y = New.y - 1;
   else if (z.x > 0.5)
      New.x = New.x - 1;

   // end barnsley code
   return(floatbailout());
}


int	LambdaexponentFractal(void)
{
    // found this in  "Science of Fractal Images"
   FLOATEXPBAILOUT();
   FPUsincos  (&z.y,&siny,&cosy);

//   if (old.x >= rqlim && cosy >= 0.0) return(1);
   if (z.x >= 4.0 && cosy >= 0.0) return(1);
   tmpexp = exp(z.x);
   tmp.x = tmpexp*cosy;
   tmp.y = tmpexp*siny;

   //multiply by lamda 
   New.x = floatparm->x*tmp.x - floatparm->y*tmp.y;
   New.y = floatparm->y*tmp.x + floatparm->x*tmp.y;
   z = New;
//   z.x = old.x;			// to allow filters to work
//   z.y = old.y;
   return(0);
}


int	FloatTrigPlusExponentFractal(void)
{
   // another Scientific American biomorph type
   // z(n+1) = e**z(n) + trig(z(n)) + C

   if (fabs(z.x) >= 6.4e2) return(1); // DOMAIN errors
   tmpexp = exp(z.x);
   FPUsincos  (&z.y,&siny,&cosy);
   CMPLXtrig0(z,New);

   //New =   trig(old) + e**old + C 
   New.x += tmpexp*cosy + floatparm->x;
   New.y += tmpexp*siny + floatparm->y;
   return(floatbailout());
}


#define XXOne New.x

int	UnityfpFractal(void)
{
   // brought to you by Mark Peterson - you won't find this in any fractal books unless they saw it here first - Mark invented it!

   XXOne = sqr(z.x) + sqr(z.y);
//   if((XXOne > 2.0) || (fabs(XXOne - 1.0) < ddelmin))
   if((XXOne > 2.0) || (fabs(XXOne - 1.0) < 0.01))
      return(1);
   z.y = (2.0 - XXOne)* z.x;
   z.x = (2.0 - XXOne)* z.y;
   New = z;  // PHD added this line
   return(0);
}

#undef XXOne
*/
/*
int	floatZtozPluszpwrFractal(void)
    {
    cpower(&New, &z, (int)param[2]);
    z = ComplexPower(z, z);
//    floatparm = &parm;
//    New.x = New.x + old.x + floatparm->x;
//    New.y = New.y + old.y + floatparm->y;
    New.x = New.x + z.x + q.x + param[0];
    New.y = New.y + z.y + q.y + param[1];
    return(floatbailout());
    }

int	floatZpowerFractal(void)
{
   cpower(&New, &z, c_exp);
   New.x += floatparm->x;
   New.y += floatparm->y;
   return(floatbailout());
}

int	floatCmplxZpowerFractal(void)
{
   New = ComplexPower(z, parm2);
   New.x += floatparm->x + param[0];
   New.y += floatparm->y + param[1];
   return(floatbailout());
}


int		Barnsley3FPFractal(void)
    {
    // An unnamed Mandelbrot/Julia function from "Fractals Everywhere" by Michael Barnsley, p. 292, example 4.1 


    // calculate intermediate products
    foldxinitx = z.x * z.x;
    foldyinity = z.y * z.y;
    foldxinity = z.x * z.y;

    // orbit calculation
    if (z.x > 0)
	{
	New.x = foldxinitx - foldyinity - 1.0 + param[0];
	New.y = foldxinity * 2 + param[1];
	}
    else
	{
	New.x = foldxinitx - foldyinity - 1.0 + floatparm->x * z.x + param[0];
	New.y = foldxinity * 2 + param[1];

	// This term added by Tim Wegner to make dependent on the imaginary part of the parameter. (Otherwise Mandelbrot is uninteresting.
	New.y += floatparm->y * z.x;
	}
    return(floatbailout());
    }


int	TrigPlusZsquaredfpFractal(void)
{
   /* From Scientific American, July 1989 
   /* A Biomorph			  
   /* z(n+1) = trig(z(n))+z(n)**2+C	  

   CMPLXtrig0(z,New);
   New.x += tempsqrx - tempsqry + floatparm->x;
   New.y += 2.0 * z.x * z.y + floatparm->y;
   return(floatbailout());
}


int	Richard8fpFractal(void)
{
   //  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
   CMPLXtrig0(z,New);
//   CMPLXtrig1(*floatparm,tmp);
   New.x += tmp.x;
   New.y += tmp.y;
   return(floatbailout());
}
*/

int	PopcornFractal(void)
{
   extern int row;
   tmp = z;
   tmp.x *= 3.0;
   tmp.y *= 3.0;
   FPUsincos(&tmp.x,&sinx,&cosx);
   FPUsincos(&tmp.y,&siny,&cosy);
   tmp.x = sinx/cosx + z.x;
   tmp.y = siny/cosy + z.y;
   FPUsincos(&tmp.x,&sinx,&cosx);
   FPUsincos(&tmp.y,&siny,&cosy);
   New.x = z.x - parm.x*siny;
   New.y = z.y - parm.x*sinx;
   /*
   New.x = old.x - parm.x*sin(old.y+tan(3*old.y));
   New.y = old.y - parm.x*sin(old.x+tan(3*old.x));
   */
   z.x = New.x;			// to allow filters to work
   z.y = New.y;
   if(plot == noplot)
   {
//      plot_orbit(New.x,New.y,1+row%colors);
       z = New;
   }
   else
   /* FLOATBAILOUT(); */
   /* PB The above line was weird, not what it seems to be!  But, bracketing
	 it or always doing it (either of which seem more likely to be what
	 was intended) changes the image for the worse, so I'm not touching it.
	 Same applies to int form in next routine. */
   /* PB later: recoded inline, still leaving it weird */
      tempsqrx = sqr(New.x);
   tempsqry = sqr(New.y);
   if((magnitude = tempsqrx + tempsqry) >= rqlim) return(1);
   z = New;
   return(0);
}

/*
int MarksCplxMand(void)
{
   tmp.x = tempsqrx - tempsqry;
   tmp.y = 2* z.x*z.y;
//   FPUcplxmul(&tmp, &Coefficient, &New);
   New = Coefficient * tmp;
   New.x += floatparm->x;
   New.y += floatparm->y;
   return(floatbailout());
}
*/
/*
int SpiderfpFractal(void)
{
   // Spider(XAXIS) { c=z=pixel: z=z*z+c; c=c/2+z, |z|<=4 }
   New.x = tempsqrx - tempsqry + tmp.x;
   New.y = 2 * z.x * z.y + tmp.y;
   tmp.x = tmp.x/2 + New.x;
   tmp.y = tmp.y/2 + New.y;
   return(floatbailout());
}


int	TetratefpFractal(void)
{
   // Tetrate(XAXIS) { c=z=pixel: z=c^z, |z|<=(P1+3) }
//   floatparm = &parm;
   New = ComplexPower(*floatparm, z);
   return(floatbailout());
}

int	ZXTrigPlusZfpFractal(void)
{
   /* z = (p1*z*trig(z))+p2*z 
   CMPLXtrig0(z,tmp); 	 /* tmp  = trig(old)		 
   CMPLXmult(parm,tmp,tmp);	 /* tmp  = p1*trig(old) 	 
   CMPLXmult(z,tmp,tmp2);	 /* tmp2 = p1*old*trig(old)	 
   CMPLXmult(parm2, z,tmp);	 /* tmp  = p2*old		 
   CMPLXadd(tmp2,tmp,New);	 /* New  = p1*trig(old) + p2*old 
   return(floatbailout());
}

int	ScottZXTrigPlusZfpFractal(void)
{
   // z = (z*trig(z))+z 
   CMPLXtrig0(z,tmp); 	/* tmp	= trig(old)	  
   CMPLXmult(z,tmp,New);	 /* New  = old*trig(old)   
   CMPLXadd(New, z,New);	 /* New  = trig(old) + old 
   return(floatbailout());
}

int	SkinnerZXTrigSubZfpFractal(void)
{
   /* z = (z*trig(z))-z 
   CMPLXtrig0(z,tmp); 	/* tmp	= trig(old)	  
   CMPLXmult(z,tmp,New);	 /* New  = old*trig(old)   
   CMPLXsub(New, z,New);	 /* New  = trig(old) - old 
   return(floatbailout());
}
*/
/*
int	DoZXTrigPlusZfpFractal(void)
    {
    if(parm.x == 1.0 && parm.y == 0.0 && parm2.y == 0.0)
	{
	if(parm2.x == 1.0)	// Scott variant
	    return(ScottZXTrigPlusZfpFractal());
	else if(parm2.x == -1.0)	// Skinner variant
	    return(SkinnerZXTrigSubZfpFractal());
	}
    return(ZXTrigPlusZfpFractal());
    }

int	Sqr1overTrigfpFractal(void)
{
   // z = sqr(1/trig(z))
   CMPLXtrig0(z, z);
   CMPLXrecip(z, z);
   CMPLXsqr(z,New);
   return(floatbailout());
}


int	TrigPlusTrigfpFractal(void)
{
   // z = trig0(z)*p1+trig1(z)*p2
   CMPLXtrig0(z,tmp);
   CMPLXmult(parm,tmp,tmp);
   CMPLXtrig1(z, z);
   CMPLXmult(parm2, z, z);
   CMPLXadd(tmp, z,New);
   return(floatbailout());
}


int	TrigXTrigfpFractal(void)
{
   // z = trig0(z)*trig1(z)
   CMPLXtrig0(z,tmp);
   CMPLXtrig1(z, z);
   CMPLXmult(tmp, z,New);
   return(floatbailout());
}
*/

/********************************************************************/
/*  Next six orbit functions are one type - extra functions are     */
/*    special cases written for speed.				    */
/********************************************************************/

/*
int	TrigZsqrdfpFractal(void)
{
   // { z=pixel: z=trig(z*z), |z|<TEST }
   tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
   tempsqry = sqr(z.y);
   CMPLXsqr_old(tmp);
   CMPLXtrig0(tmp,New);
   return(floatbailout());
}

int	SqrTrigfpFractal(void)
{
   // SZSB(XYAXIS) { z=pixel, TEST=(p1+3): z=sin(z)*sin(z), |z|<TEST}
   CMPLXtrig0(z,tmp);
   CMPLXsqr(tmp,New);
   return(floatbailout());
}

int	LambdaTrigfpFractal(void)
{
   FLOATXYTRIGBAILOUT();
   CMPLXtrig0(z,tmp); 	     // tmp = trig(old) 
//   floatparm = &parm;
   CMPLXmult(*floatparm,tmp,New);   // New = longparm*trig(old) 
   z = New;
   return(0);
}

int	LambdaTrigfpFractal1(void)
{
   FLOATTRIGBAILOUT(); // sin,cos
   CMPLXtrig0(z,tmp); 	     // tmp = trig(old) 
//   floatparm = &parm;
   CMPLXmult(*floatparm,tmp,New);   // New = longparm*trig(old)
   z = New;
   return(0);
}

int	LambdaTrigfpFractal2(void)
{
   FLOATHTRIGBAILOUT(); // sinh,cosh
   CMPLXtrig0(z,tmp); 	     // tmp = trig(old) 
//   floatparm = &parm;
   CMPLXmult(*floatparm,tmp,New);   // New = longparm*trig(old) 
   z = New;
   return(0);
}

int	LambdaTrigfp(void)
{
if (strcmp(Fractal.Fn1, "sin") == 0 || strcmp(Fractal.Fn1, "cos") == 0)
					// exp added so that "bows" fractal from FRACTINT.par runs properly
    return (LambdaTrigfpFractal1());
else if (strcmp(Fractal.Fn1, "exp") == 0)
    return (LambdaexponentFractal());
else if (strcmp(Fractal.Fn1, "sinh") == 0 || strcmp(Fractal.Fn1, "cosh") == 0)
    return (LambdaTrigfpFractal2());
else
    return (LambdaTrigfpFractal());
}

int	LambdaTrigOrTrigfpFractal(void)
{
   // z = trig0(z)*p1 if mod(old) < p2.x and trig1(z)*p1 if mod(old) >= p2.x
   if (CMPLXmod(z) < parm2.x)
	{
	CMPLXtrig0(z, z);
//	FPUcplxmul(floatparm, &z, &New);
	 New = z * *floatparm;
       }
   else
	{
	CMPLXtrig1(z, z);
//	FPUcplxmul(floatparm,&z,&New);
	New = z * *floatparm;
       }
   return(floatbailout());
}

int	JuliaTrigOrTrigfpFractal(void)
    {
   // z = trig0(z)+p1 if mod(old) < p2.x and trig1(z)+p1 if mod(old) >= p2.x
    if (CMPLXmod(z) < parm2.x)
       {
	CMPLXtrig0(z, z);
	CMPLXadd(*floatparm, z,New);
       }
    else
       {
	CMPLXtrig1(z, z);
	CMPLXadd(*floatparm, z,New);
       }
    return(floatbailout());
    }


int	ManOWarfpFractal(void)
{
   // From Art Matrix via Lee Skinner
   // note that fast >= 287 equiv in fracsuba.asm must be kept in step

//   floatparm = &parm;
   New.x = tempsqrx - tempsqry + tmp.x + floatparm->x + param[0];
   New.y = 2.0 * z.x * z.y + tmp.y + floatparm->y + param[1];
   tmp = z;
   return(floatbailout());
}

int	InitMostFracfp(void)
{
   // Catch all for when we don't know how to init a function
    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    z.x = parm.x = c.x + param[0];
    z.y = parm.y = c.y + param[1];
    floatparm = &parm;
    tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
    tempsqry = sqr(z.y);
    tmp = z;
    return(1);
}


int MarksMandelPwrfpFractal(void)
{
   CMPLXtrig0(z,New);
   CMPLXmult(tmp,New,New);
   New.x += floatparm->x;
   New.y += floatparm->y;
   return(floatbailout());
}


int	CirclefpFractal(void)
{
//   extern int colors;
   extern long color;
   int i;
   i = (int)(param[0]*(tempsqrx+tempsqry));
   color = i&(255);
   return(1);
}

/* -------------------------------------------------------------------- */
/*		Phoenix Fractals					*/
/* -------------------------------------------------------------------- */

#define	PLUS	1
#define	ZERO	0
#define	MINUS	-1

int	PhoenixType = ZERO;
int	PhoenixDegree;
/*
int		PhoenixFractal(void)
{
// z(n+1) = z(n)^2 + p + qy(n),  y(n+1) = z(n)
   tmp.x = z.x * z.y;
   New.x = tempsqrx - tempsqry + floatparm->x + (floatparm->y * tmp2.x);
   New.y = (tmp.x + tmp.x) + (floatparm->y * tmp2.y);
   tmp2 = z; // set tmp2 to Y value
   return(floatbailout());
}

int		PhoenixPlusFractal(void)
{
// z(n+1) = z(n)^(degree-1) * (z(n) + p) + qy(n),  y(n+1) = z(n)
int i;
Complex oldplus, newminus;
   oldplus = z;
   tmp = z;
   for(i=1; i<PhoenixDegree; i++) 
       { // degree >= 2, degree=degree-1 in setup
//       FPUcplxmul(&z, &tmp, &tmp); // = old^(degree-1)
       tmp = tmp * z; // = old^(degree-1)
       }
   oldplus.x += floatparm->x;
//   FPUcplxmul(&tmp, &oldplus, &newminus);
   newminus = oldplus * tmp;
   New.x = newminus.x + (floatparm->y * tmp2.x);
   New.y = newminus.y + (floatparm->y * tmp2.y);
   tmp2 = z; // set tmp2 to Y value
   return(floatbailout());
}

int		PhoenixMinusFractal(void)
    {
    // z(n+1) = z(n)^(degree-2) * (z(n)^2 + p) + qy(n),  y(n+1) = z(n)
    int i;
    Complex oldsqr, newminus;
//    FPUcplxmul(&z, &z, &oldsqr);
    oldsqr = z * z;
    tmp = z;
   for(i=1; i<PhoenixDegree; i++) { // degree >= 3, degree=degree-2 in setup
//     FPUcplxmul(&z, &tmp, &tmp); // = old^(degree-2) 
       tmp = tmp * z; // = old^(degree-1)
       }
   oldsqr.x += floatparm->x;
//   FPUcplxmul(&tmp, &oldsqr, &newminus);
   newminus = tmp * oldsqr;
   New.x = newminus.x + (floatparm->y * tmp2.x);
   New.y = newminus.y + (floatparm->y * tmp2.y);
   tmp2 = z; // set tmp2 to Y value
   return(floatbailout());
}

int	DoPhoexnixFractal(void)
    {
    if(PhoenixType == ZERO)
	return(PhoenixFractal());
    else if(PhoenixType == PLUS)
	return(PhoenixPlusFractal());
    else
	return(PhoenixMinusFractal());
    return 1;					// just to shut up compiler warnings
    }

int		PhoenixFractalcplx(void)
    {
// z(n+1) = z(n)^2 + p1 + p2*y(n),  y(n+1) = z(n)
    tmp.x = z.x * z.y;
    New.x = tempsqrx - tempsqry + floatparm->x + (parm2.x * tmp2.x) - (parm2.y * tmp2.y);
    New.y = (tmp.x + tmp.x) + floatparm->y + (parm2.x * tmp2.y) + (parm2.y * tmp2.x);
    tmp2 = z; // set tmp2 to Y value
    return(floatbailout());
    }


int	PhoenixSetup(void)
    {
    Complex t = (invert) ? invertz2(c) : c;
//   longparm = &lparm; // added to consolidate code 10/1/92 JCO
    parm.x = param[0];
    parm.y = param[1];
    parm2.x = param[2];
    parm2.y = param[3];
    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    if (juliaflag)
	{
	parm.x = q.x;
	parm.y = q.y;
//	old.x = z.x;
//	old.y = z.y;
	}
    else
	{
	z.x = t.x;
	z.y = t.y;
	}
    tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
    tempsqry = sqr(z.y);
    tmp2.x = 0; // set tmp2 to Y value
    tmp2.y = 0; // set tmp2 to Y value
    floatparm = &parm;
    PhoenixDegree = (int)parm2.x;
    if(PhoenixDegree < 2 && PhoenixDegree > -3) 
	PhoenixDegree = 0;
    param[2] = (double)PhoenixDegree;
    if(PhoenixDegree == 0)
	PhoenixType = ZERO;
    if(PhoenixDegree >= 2)
	{
	PhoenixDegree = PhoenixDegree - 1;
	PhoenixType = PLUS;
	}
    if(PhoenixDegree <= -3)
	{
	PhoenixDegree = abs(PhoenixDegree) - 2;
	PhoenixType = MINUS;
	}

   return(1);
}

int		PhoenixCplxPlusFractal(void)
{
// z(n+1) = z(n)^(degree-1) * (z(n) + p) + qy(n),  y(n+1) = z(n)
int i;
Complex oldplus, newminus;
   oldplus = z;
   tmp = z;
//    tmp2.x = 0; // set tmp2 to Y value
//    tmp2.y = 0; // set tmp2 to Y value
   for(i=1; i<PhoenixDegree; i++) { // degree >= 2, degree=degree-1 in setup
//     FPUcplxmul(&z, &tmp, &tmp); // = old^(degree-1) 
       tmp = z * tmp; // = old^(degree-1) 
       }
   oldplus.x += floatparm->x;
   oldplus.y += floatparm->y;
//   FPUcplxmul(&tmp, &oldplus, &newminus);
//   FPUcplxmul(&parm2, &tmp2, &tmp);
   newminus = tmp * oldplus;
   tmp = parm2 * tmp2;
   New.x = newminus.x + tmp.x;
   New.y = newminus.y + tmp.y;
   tmp2 = z; // set tmp2 to Y value
   return(floatbailout());
}

int	PhoenixCplxMinusFractal(void)
    {
    // z(n+1) = z(n)^(degree-2) * (z(n)^2 + p) + qy(n),  y(n+1) = z(n)
    int i;
    Complex oldsqr, newminus;

//   FPUcplxmul(&z, &z, &oldsqr);
    oldsqr = z * z;
    tmp = z;
    for(i=1; i<PhoenixDegree; i++) 
	{ // degree >= 3, degree=degree-2 in setup
//	FPUcplxmul(&z, &tmp, &tmp); // = old^(degree-2)
	tmp = z * tmp;
	}
   oldsqr.x += floatparm->x;
   oldsqr.y += floatparm->y;
//   FPUcplxmul(&tmp, &oldsqr, &newminus);
//   FPUcplxmul(&parm2, &tmp2, &tmp);
   newminus = tmp * oldsqr;
   tmp = parm2 * tmp2;
   New.x = newminus.x + tmp.x;
   New.y = newminus.y + tmp.y;
   tmp2 = z; // set tmp2 to Y value
   return(floatbailout());
}

int	DoPhoexnixComplex(void)
    {
    if(PhoenixType == ZERO)
	return(PhoenixFractalcplx());
    else if(PhoenixType == PLUS)
	return(PhoenixCplxPlusFractal());
    else
	return(PhoenixCplxMinusFractal());
    return 1;					// just to shut up compiler warnings
    }


int	PhoenixCplxSetup(void)
    {
    Complex t = (invert) ? invertz2(c) : c;
//   longparm = &lparm;
    initial.x = t.x;
    initial.y = t.y;
    floatparm = &initial;
    parm.x = param[0];
    parm.y = param[1];
    parm2.x = param[2];
    parm2.y = param[3];
    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    if (juliaflag)
	{
//    floatparm->x = 0.85;
//    floatparm->y = 0.6;
	floatparm->x = q.x;
	floatparm->y = q.y;
//	old.x = z.x;
//	old.y = z.y;
	}
    else
	{
//	dz_real = dq_real + param[0];
//	dz_imag = dq_imag + param[1];
//	floatparm->x = c_real;
//	floatparm->y = c_imag;
//	old.x = 0.0;
//	old.y = 0.0;
	z.x = initial.x + param[0];
	z.y = initial.y + param[1];
	}
//    if (!juliaflag)
//	{
//	old.x = initial.x;
//	old.y = initial.y;
//	}
//    else
//	{
//	old.x = parm.x = initial.x;
//	old.y = parm.y = initial.y;
//	}
//    old.x = c_real;
//    old.y = c_imag;
    tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
    tempsqry = sqr(z.y);
    tmp2.x = 0; // set tmp2 to Y value
    tmp2.y = 0; // set tmp2 to Y value
//    floatparm = &parm;
    PhoenixDegree = (int)param[4];
    PhoenixDegree = 0;					// yeah, yeah... one day I'll allow an additional parameter
    if(PhoenixDegree < 2 && PhoenixDegree > -3) 
       PhoenixDegree = 0;
    param[4] = (double)PhoenixDegree;
    tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
    tempsqry = sqr(z.y);
    if(PhoenixDegree == 0)
	{
	if(parm2.x != 0 || parm2.y != 0)
	    symmetry = NOSYM;
	else
	    symmetry = ORIGIN;
	if(parm.y == 0 && parm2.y == 0)
	    symmetry = XAXIS;
	PhoenixType = ZERO;
	}
    if(PhoenixDegree >= 2)
	{
	PhoenixDegree = PhoenixDegree - 1;
	if(parm.y == 0 && parm2.y == 0)
	    symmetry = XAXIS;
	else
	    symmetry = NOSYM;
	PhoenixType = PLUS;
	}
    if(PhoenixDegree <= -3)
	{
	PhoenixDegree = abs(PhoenixDegree) - 2;
	if(parm.y == 0 && parm2.y == 0)
	    symmetry = XAXIS;
	else
	    symmetry = NOSYM;
	PhoenixType = MINUS;
	}

    return(1);
    }



int	MandPhoenixSetup(void)
    {
//   longparm = &linit; // added to consolidate code 10/1/92 JCO
    Complex t = (invert) ? invertz2(c) : c;

    parm.x = param[0];
    parm.y = param[1];
    parm2.x = param[2];
    parm2.y = param[3];
    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    if (juliaflag)
	{
	parm.x = q.x;
	parm.y = q.y;
//	old.x = z.x;
//	old.y = z.y;
	}
    else
	{
	z = parm = t;
	}
//    old.x = c_real;
//    old.y = c_imag;
    tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
    tempsqry = sqr(z.y);
    tmp2.x = 0; // set tmp2 to Y value
    tmp2.y = 0; // set tmp2 to Y value
    floatparm = &parm;
//    floatparm = &init;
    PhoenixDegree = (int)parm2.x;
    if(PhoenixDegree < 2 && PhoenixDegree > -3) 
	PhoenixDegree = 0;
    param[2] = (double)PhoenixDegree;
    if(PhoenixDegree == 0)
	PhoenixType = ZERO;
    if(PhoenixDegree >= 2)
	{
	PhoenixDegree = PhoenixDegree - 1;
	PhoenixType = PLUS;
	}
    if(PhoenixDegree <= -3)
	{
	PhoenixDegree = abs(PhoenixDegree) - 2;
	PhoenixType = MINUS;
	}

   return(1);
}
*/

/* -------------------------------------------------------------------- */
/*		Initialization (once per pixel) routines		*/
/* -------------------------------------------------------------------- */

//int mandelfp_per_pixel()
//{
//floatparm = &initial;
//   /* floating point mandelbrot */
//   /* mandelfp */
//
////   if(invert)
////      invertz2(&initial);
////   else
////      initial.x = dx0[col]+dx1[row];
//    switch (type)
//      {
//	case MAGNET2M:
//	    FloatPreCalcMagnet2();
//	case MAGNET1M:		 /* Crit Val Zero both, but neither   */
//	    z.x = z.y = 0.0; /* is of the form f(Z,C) = Z*g(Z)+C  */
//	    break;
//	case MANDELLAMBDAFP:		/* Critical Value 0.5 + 0.0i  */
////	    floatparm->x = 0.0;
////	    floatparm->y = 0.0;
//	    z.x = 0.5;
//	    z.y = 0.0;
//	    break;
//	default:
////	    old.x = c_real;
////	    old.y = c_imag;
//	    z.x = 0.5;
//	    z.y = 0.0;
//	    floatparm->x = 0.0;
//	    floatparm->y = 0.0;
////	    old = initial;
//	    break;
//      }
//
//   /* alter init value */
////   if(useinitorbit == 1)
////      old = initorbit;
////   else if(useinitorbit == 2)
////      old = initial;
//
//   if(TrueCol.inside_colour == -60 || TrueCol.inside_colour == -61)
//   {
//      /* kludge to match "Beauty of Fractals" picture since we start
//	 Mandelbrot iteration with init rather than 0 */
//      z.x = parm.x; /* initial pertubation of parameters set */
//      z.y = parm.y;
//      color = -1;
//   }
//   else
//   {
////     old.x += parm.x;
////     old.y += parm.y;
//   }
//   tmp = initial; /* for spider */
//   tempsqrx = sqr(z.x);  /* precalculated value for regular Mandelbrot */
//   tempsqry = sqr(z.y);
//   return(1); /* 1st iteration has been done */
//}

/*
int marks_mandelpwrfp_per_pixel(void)
    {
    MandelFnSetup();
//    init_mandel_df();
    tmp = z;
    tmp.x -= 1;
    CMPLXpwr(z,tmp,tmp);
    return(1);
    }
*/
//int juliafp_per_pixel()
//{
//   /* floating point julia */
//   /* juliafp */
////   if(invert)
////      invertz2(&old);
////   else
//   {
////     old.x = dx0[col]+dx1[row];
////     old.y = dy0[row]+dy1[col];
//    z = c;
//   }
//   tempsqrx = sqr(z.x);  /* precalculated value for regular Julia */
//   tempsqry = sqr(z.y);
//   tmp = z;
//   return(0);
//}
//
extern	int	init_mandel_df(void);
extern	int	TrigSetup(void);

/*
int	otherrichard8fp_per_pixel(void)
    {
   //  Richard8 {c = z = pixel: z=sin(z)+sin(pixel),|z|<=50}
//    init_mandel_df();
    period_level = FALSE;			// no periodicity checking (get rid of bug PHD 2009-10-16)
    Complex	t = (invert) ? invertz2(c) : c;

    initial.x = t.x;
    initial.y = t.y;
    if (!juliaflag)
	{
	z = t;
	}
//    else
//	{
//	old.x = z.x;
//	old.y = z.y;
//	}
    parm.x = param[0];
    parm.y = param[1];
    parm2.x = param[2];
    parm2.y = param[3];
    z.x += parm.x;	 // initial pertubation of parameters set
    z.y += parm.y;

//    tempsqrx = sqr(old.x);  // precalculated value for regular Mandelbrot
//    tempsqry = sqr(old.y);
//    tmp.x = c_real;
//    tmp.y = c_imag;
//    othermandelfp_per_pixel();
    floatparm = &z;
    CMPLXtrig1(*floatparm,tmp);
    CMPLXmult(tmp,parm2,tmp);
    return(1);
}


int othermandelfp_per_pixel(void)
    {
    Complex	t = (invert) ? invertz2(c) : c;

    initial.x = t.x;
    initial.y = t.y;
//    initial.x = c.x;
//    initial.y = c.y;
    floatparm = &initial;
    parm.x = param[0];
    parm.y = param[1];
    parm2.x = param[2];
    parm2.y = param[3];
    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    if (juliaflag)
	{
	floatparm->x = q.x;
	floatparm->y = q.y;
//	old.x = z.x;
//	old.y = z.y;
	}
    else
	{
	z.x = initial.x + param[0];
	z.y = initial.y + param[1];
	}
    tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
    tempsqry = sqr(z.y);
    tmp = z;
    return 1;
    }

//Complex coefficient;


int marksmandelfp_per_pixel(void)
    {
   // marksmandel 
    Complex	t = (invert) ? invertz2(c) : c;

    initial.x = t.x;
    initial.y = t.y;
//    initial.x = c.x;
//    initial.y = c.y;
    floatparm = &initial;
    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    if (juliaflag)
	{
	initial.x = q.x;
	initial.y = q.y;
//	old.x = z.x;
//	old.y = z.y;
	}
    else
	{
	z.x = initial.x + param[0];
	z.y = initial.y + param[1];
	}
//    if(param[2] < 1)
//	param[2] = 1;
    tempsqrx = sqr(z.x);
    tempsqry = sqr(z.y);

    pwr.x = param[2] - 1.0;
    pwr.y = 0;
    coefficient = ComplexPower(initial, pwr);
    return(1); // 1st iteration has been done 
    }


int	MarksLambdafpFractal(void)
{
   // Mark Peterson's variation of "lambda" function

   // Z1 = (C^(exp-1) * Z**2) + C
   tmp.x = tempsqrx - tempsqry;
   tmp.y = z.x * z.y *2;

   New.x = coefficient.x * tmp.x - coefficient.y * tmp.y + floatparm->x;
   New.y = coefficient.x * tmp.y + coefficient.y * tmp.x + floatparm->y;

   return(floatbailout());
}


int MarksCplxMandperp(void)
    {
    Complex t = (invert) ? invertz2(c) : c;
//   if(invert)
//      invertz2(&initial);
//   else
    initial.x = t.x;
    initial.y = t.y;
    floatparm = &initial;
    if (juliaflag)
	{
	initial.x = q.x;
	initial.y = q.y;
//	old.x = z.x;
//	old.y = z.y;
	}
    else
	{
	z.x = initial.x + param[0];
	z.y = initial.y + param[1];
	}
//      initial.x = dx0[col]+dx1[row];
//   old.x = initial.x + parm.x; // initial pertubation of parameters set
//   old.y = initial.y + parm.y;
   tempsqrx = sqr(z.x);  // precalculated value
   tempsqry = sqr(z.y);
   pwr.x = param[2] - 1.0;
   pwr.y = param[3];
   Coefficient = ComplexPower(initial, pwr);
   return(1);
}
*/

/* These are for quaternions */
/*
double qc,qci,qcj,qck;

int	QuaternionFPFractal(void)
{
   double a0,a1,a2,a3,n0,n1,n2,n3;
   floatparm = &parm;
   a0 = z.x;
   a1 = z.y;
   a2 = floatparm->x;
   a3 = floatparm->y;

   n0 = a0*a0-a1*a1-a2*a2-a3*a3 + qc;
   n1 = 2*a0*a1 + qci;
   n2 = 2*a0*a2 + qcj;
   n3 = 2*a0*a3 + qck;
   // Check bailout
   magnitude = a0*a0+a1*a1+a2*a2+a3*a3;
   if (magnitude>rqlim) {
       return 1;
   }
   z.x = n0;
   z.y = n1;
   floatparm->x = n2;
   floatparm->y = n3;
//   z.x = old.x;			// to allow filters to work
//   z.y = old.y;
   return(0);
}


int quaternionfp_per_pixel(void)
    {
    Complex	t = (invert) ? invertz2(c) : c;

    floatparm = &parm;
    if (juliaflag)
	{
	z = t;
*/
/*
	old.x = c.x;
	old.y = c.y;
	floatparm->x = param[4];
	floatparm->y = param[5];
	param[0] = q.x;
	param[1] = q.y;
	qc  = param[0];
	qci = param[1];
*/
/*
	floatparm->x = 0;
	floatparm->y = 0;
	qc  = q.x;
	qci = q.y;
	}
    else
	{
	z.x = 0;
	z.y = 0;
	floatparm->x = 0;
	floatparm->y = 0;
	qc  = t.x;
	qci = t.y;
	}
    qcj = param[2];
    qck = param[3];
    return(0);
    }
*/
extern	void HComplexTrig0(_HCMPLX *, _HCMPLX *);

/*
int InitHyperComplexFP(void)
    {
    Complex t = (invert) ? invertz2(c) : c;

    MandelFnSetup();
    if (juliaflag)
	{
	z = q;
	floatparm->x = t.x;
	floatparm->y = t.y;
	}
    else
	{
	z.x = 0;
	z.y = 0;
	floatparm->x = 0;
	floatparm->y = 0;
	}
//    qc  = dxpixel();
//    qci = dypixel();
    qc  = t.x;
    qci = t.y;
    qcj = param[2];
    qck = param[3];
    return(0);
    }

int	HyperComplexFPFractal(void)
{
   _HCMPLX hold, hnew;
   hold.x = z.x;
   hold.y = z.y;
   hold.z = floatparm->x;
   hold.t = floatparm->y;

//   HComplexSqr(&hold,&hnew); 
   HComplexTrig0(&hold,&hnew);

   hnew.x += qc;
   hnew.y += qci;
   hnew.z += qcj;
   hnew.t += qck;

   z.x = hnew.x;
   z.y = hnew.y;
//   z.x = New.x;			// to allow filters to work
//   z.y = New.y;
   floatparm->x = hnew.z;
   floatparm->y = hnew.t;

   // Check bailout
   magnitude = sqr(z.x)+sqr(z.y)+sqr(floatparm->x)+sqr(floatparm->y);
   if (magnitude>rqlim) {
       return 1;
   }
   return(0);
}

int	VLSetup(void)
    {
    MandelFnSetup();
    if (param[0] < 0.0) param[0] = 0.0;
    if (param[1] < 0.0) param[1] = 0.0;
    if (param[0] > 1.0) param[0] = 1.0;
    if (param[1] > 1.0) param[1] = 1.0;
    floatparm = &parm;
    return 1;
    }


int	VLfpFractal(void) // Beauty of Fractals pp. 125 - 127
{
   double a, b, ab, half, u, w, xy;

   half = param[0] / 2.0;
   xy = z.x * z.y;
   u = z.x - xy;
   w = -z.y + xy;
   a = z.x + param[1] * u;
   b = z.y + param[1] * w;
   ab = a * b;
   New.x = z.x + half * (u + (a - ab));
   New.y = z.y + half * (w + (-b + ab));
   return(floatbailout());
}

//extern	long	iteration;		// globals for speed


int	EscherfpFractal(void) // Science of Fractal Images pp. 185, 187
    {
    Complex oldtest, newtest, testsqr;
    double testsize = 0.0;
    int testiter = 0;
    
    New.x = tempsqrx - tempsqry; // standard Julia with C == (0.0, 0.0i)
    New.y = 2.0 * z.x * z.y;
    oldtest.x = New.x * 15.0;    // scale it
    oldtest.y = New.y * 15.0;
    testsqr.x = sqr(oldtest.x);  // set up to test with user-specified ...
    testsqr.y = sqr(oldtest.y);  //    ... Julia as the target set
//    while (testsize <= rqlim && testiter < threshold) // nested Julia loop
    while (TRUE) // nested Julia loop
	{
	if (testsize > rqlim)
	    break;
	if (testiter >= threshold)
	    break;
	newtest.x = testsqr.x - testsqr.y + param[0];
	newtest.y = 2.0 * oldtest.x * oldtest.y + param[1]; 
	testsize = (testsqr.x = sqr(newtest.x)) + (testsqr.y = sqr(newtest.y));
	oldtest = newtest;
	testiter++;
	}
    if (testsize > rqlim) 
	return(floatbailout()); // point not in target set
    else // make distinct level sets if point stayed in target set
	{
//	iteration = ((3L * iteration) % 255L) + 1L;
	iteration = ((3 * iteration) % 255) + 1;
	return 1;
	}
}
*/

/* -------------------------------------------------------------------- */
/*		Setup (once per fractal image) routines 		*/
/* -------------------------------------------------------------------- */
/*
int    TrigSetup(void)
    {
    Complex t;

    period_level = FALSE;			// no periodicity checking (get rid of bug PHD 2009-10-16)
    if (!juliaflag)
	{
	t = (invert) ? invertz2 (c) : c;
//	floatparm = &parm;
//	floatparm->x = q.x;
//	floatparm->y = q.y;
	z = t;
	}
    else
	{
//	floatparm = &parm;
//	floatparm->x = q.x;
//	floatparm->y = q.y;
//	old = z;
	}
    parm.x = param[0];
    parm.y = param[1];
    parm2.x = param[2];
    parm2.y = param[3];
//    floatparm = &parm;
//    tempsqrx = sqr(old.x);  // precalculated value for regular Mandelbrot
//    tempsqry = sqr(old.y);
    return 1;
    }

int    MandelFnSetup(void)
    {
    Complex t = (invert) ? invertz2(c) : c;

    initial.x = t.x;
    initial.y = t.y;
    floatparm = &initial;
    parm.x = param[0];
    parm.y = param[1];
    parm2.x = param[2];
    parm2.y = param[3];
    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    if (juliaflag)
	{
	floatparm->x = q.x;
	floatparm->y = q.y;
//	old.x = z.x;
//	old.y = z.y;
	}
    else
	{
	z.x = initial.x + param[0];
	z.y = initial.y + param[1];
	}
    tempsqrx = sqr(z.x);  / precalculated value for regular Mandelbrot 
    tempsqry = sqr(z.y);
    tmp = z;
    return 1;
    }


int	SierpinskiFPSetup(void)
{
   // sierpinski 
    if (juliaflag)
	{
//	old.x = z.x;
//	old.y = z.y;
	}
    else
	{
	z = q;
	}
     period_level = FALSE;			// no periodicity checking
   periodicitycheck = 0;		// disable periodicity checks 
//   tmp.x = 1;
//   tmp.y = 0.5;
//   rqlim = 127.0;			// Sierpinski
//   rqlim = 4.0;				// BarnsleyM1-3

//   old.x = z.x;
//   old.y = z.y;

   return(1);
}


int	LambdaFPSetup(void)
{
   // Lambda Fractal 
//period_level = 4;
period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
floatparm = &initial;
rqlim = 4.0;

if (juliaflag)
    {
//    floatparm->x = 0.85;
//    floatparm->y = 0.6;
    floatparm->x = q.x;
    floatparm->y = q.y;
//    old.x = z.x;
//    old.y = z.y;
    }
else
    {
    z.x = q.x + param[0];
    z.y = q.y + param[1];
    floatparm->x = z.x;
    floatparm->y = z.y;
    z.x = 0.5;
    z.y = 0.0;
    }
//floatparm->x = dz_real;
//floatparm->y = dz_imag;
tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
tempsqry = sqr(z.y);
return(1);
}
*/

/*
int	UnityFPSetup(void)
{
   // Unity Fractal 
   period_level = FALSE;			// no periodicity checking
   rqlim = 4.0;					// BarnsleyM1-3
   periodicitycheck=0;


#ifdef DEBUG
//MessageBox (GlobalHwnd, "Got here", "UnitySetup", MB_ICONEXCLAMATION | MB_OK);
DoCaption (GlobalHwnd, "UnitySetup");
#endif

    z = q;
//    old.x = z.x;
//    old.y = z.y;
    return(1);
}
*/
/*
int	SpiderFPSetup(void)
    {
   // Spider Fractal
//period_level = 4;
    Complex t = (invert) ? invertz2(c) : c;

    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    rqlim = 4.0;
    if (juliaflag)
//	return(JuliafpSetup());
//	juliafp_per_pixel();
	{
	z = q;
	}
    else
	{
	z.x = q.x + param[0];
	z.y = q.y + param[1];
	}
    tmp.x = t.x;
    tmp.y = t.y;
//   old.x = dz_real;
// old.y = dz_imag;
    tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
    tempsqry = sqr(z.y);

    return(1);
    }


int	TestSetup(void)
    {
//period_level = 4;
    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
    rqlim = 4.0;
    initial.x = c.x;  // value to bail out in one iteration
    initial.y = c.y;
    parm.x = c.x;
    parm.y = c.y;
    parm2.x = 0.0;
    parm2.y = 0.0;
 
    floatparm = &parm;

//      initial.x = dx0[col]+dx1[row];

//   if(useinitorbit == 1)
//      old = initorbit;
//   else

    z = initial;

    z.x += parm.x;	 // initial pertubation of parameters set
    z.y += parm.y;

//   floatparm->x = c_real;
//   floatparm->y = c_imag;

    

//   tmp = initial; // for spider 
//   tmp.x = c_real;
//   tmp.y = c_imag;
//   tmp.x = c_real;
//   tmp.y = c_imag;
//   old.x = dz_real;
//   old.y = dz_imag;
//   old.x = 0.0;
//   old.y = 0.0;
    tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
    tempsqry = sqr(z.y);

    return(1);
    }


int	SetupTetrate(void)			// so we can use the same fractalp.h and fractalp.cpp as other versions
    {
    return MandelFnSetup();
    }


int	NullSetup(void)				// sometimes we just don't want to do anything 
    {
    return 1;
    }


int	BarnsleySetup(void)
{
//   period_level = 4;
   period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
   rqlim = 4.0;
//   initial.x = 0.0;  // value to bail out in one iteration
//   initial.y = 0.0;

   parm.x = q.x;
   parm.y = q.y;
   parm2.x = 0.0;
   parm2.y = 0.0;
 
   floatparm = &parm;
//   floatparm->x = c_real;
//   floatparm->y = c_imag;

    

//   tmp = initial; // for spider
//   tmp.x = c_real;
//   tmp.y = c_imag;
//   tmp.x = c_real;
//   tmp.y = c_imag;
   if (juliaflag)
//	return(JuliafpSetup());
//	juliafp_per_pixel();
	{
       z = c;
	}
//   else
//	{
//	old.x = z.x;
//	old.y = z.y;
//	}
    
//   old.x = 0.0;
//   old.y = 0.0;
   tempsqrx = sqr(z.x);  // precalculated value for regular Mandelbrot
   tempsqry = sqr(z.y);

   return(1);
}
*/

/***********************************************************
	Bresenham's algorithm for drawing line 
***********************************************************/

/*
void draw_line (int X1, int Y1, int X2, int Y2, int color)

{               // uses Bresenham algorithm to draw a line
    int dX, dY;                     // vector components
    int row, col,
        final,                      // final row or column number
        G,                  // used to test for New row or column
        inc1,           // G increment when row or column doesn't change
        inc2;               // G increment when row or column changes
    char pos_slope;
//    extern int xdots,ydots;

    dX = X2 - X1;                   // find vector components
    dY = Y2 - Y1;
    pos_slope = (dX > 0);                   // is slope positive?
    if (dY < 0)
    pos_slope = !pos_slope;
    if (abs (dX) > abs (dY))                // shallow line case
    {
        if (dX > 0)         // determine start point and last column
        {
            col = X1;
            row = Y1;
            final = X2;
        }
        else
        {
            col = X2;
            row = Y2;
            final = X1;
        }
        inc1 = 2 * abs (dY);            // determine increments and initial G
        G = inc1 - abs (dX);
        inc2 = 2 * (abs (dY) - abs (dX));
        if (pos_slope)
            while (col <= final)    // step through columns checking for New row
            {
                (*plot) ((WORD)col, (WORD)row, color);
                col++;
                if (G >= 0)             // it's time to change rows
                {
                    row++;      // positive slope so increment through the rows
                    G += inc2;
                }
                else                        // stay at the same row
                    G += inc1;
            }
        else
            while (col <= final)    // step through columns checking for New row
            {
                (*plot) ((WORD)col, (WORD)row, color);
                col++;
                if (G > 0)              // it's time to change rows
                {
                    row--;      // negative slope so decrement through the rows
                    G += inc2;
                }
                else                        // stay at the same row 
                    G += inc1;
            }
    }   // if |dX| > |dY|
    else                            // steep line case
    {
        if (dY > 0)             // determine start point and last row
        {
            col = X1;
            row = Y1;
            final = Y2;
        }
        else
        {
            col = X2;
            row = Y2;
            final = Y1;
        }
        inc1 = 2 * abs (dX);            // determine increments and initial G
        G = inc1 - abs (dY);
        inc2 = 2 * (abs (dX) - abs (dY));
        if (pos_slope)
            while (row <= final)    // step through rows checking for New column
            {
                (*plot) ((WORD)col, (WORD)row, color);
                row++;
                if (G >= 0)                 // it's time to change columns
                {
                    col++;  // positive slope so increment through the columns
                    G += inc2;
                }
                else                    // stay at the same column
                    G += inc1;
            }
        else
            while (row <= final)    // step through rows checking for New column
            {
                (*plot) ((WORD)col, (WORD)row, color);
                row++;
                if (G > 0)                  // it's time to change columns 
                {
                    col--;  // negative slope so decrement through the columns
                    G += inc2;
                }
                else                    // stay at the same column
                    G += inc1;
            }
    }
}   // draw_line
*/


/*

/////////////////////////////
// Bifurcation trig stuff...
/////////////////////////////

int BifurcVerhulstTrig()
    {
//  Population = Pop + Rate * fn(Pop) * (1 - fn(Pop))
    tmp.x = Population;
    tmp.y = 0;
    CMPLXtrig0(tmp, tmp);
    Population += Rate * tmp.x * (1 - tmp.x);
    return (fabs(Population) > BIG);
    }
int BifurcLambdaTrig()
    {
//  Population = Rate * fn(Population) * (1 - fn(Population))
    tmp.x = Population;
    tmp.y = 0;
    CMPLXtrig0(tmp, tmp);
    Population = Rate * tmp.x * (1 - tmp.x);
    return (fabs(Population) > BIG);
    }
int BifurcAddTrigPi()
    {
    tmp.x = Population * PI;
    tmp.y = 0;
    CMPLXtrig0(tmp, tmp);
    Population += Rate * tmp.x;
    return (fabs(Population) > BIG);
    }
int BifurcSetTrigPi()
    {
    tmp.x = Population * PI;
    tmp.y = 0;
    CMPLXtrig0(tmp, tmp);
    Population = Rate * tmp.x;
    return (fabs(Population) > BIG);
    }
int BifurcStewartTrig()
    {
//  Population = (Rate * fn(Population) * fn(Population)) - 1.0
    tmp.x = Population;
    tmp.y = 0;
    CMPLXtrig0(tmp, tmp);
    Population = (Rate * tmp.x * tmp.x) - 1.0;
    return (fabs(Population) > BIG);
    }
*/

#endif
