/*
	This module consists only of the fractalspecific structure
	and a *slew* of defines needed to get it to compile
*/

/* includes needed for fractalspecific */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include ".\parser\fractint.h"
//#include ".\parser\mpmath.h"
//#include "helpdefs.h"
#include "fractype.h"
#include "fractalp.h"

#define	HALF_PI	1.570796326794895

extern	HWND	GlobalHwnd;			// This is the main windows handle

/* functions defined elswhere needed for fractalspecific */
/* moved to prototyp.h */

/* parameter descriptions */
/* Note: parameters preceded by + are integer parameters */

/* for Mandelbrots */
static char realz0[] = "Real Perturbation of Z(0)";
static char imagz0[] = "Imaginary Perturbation of Z(0)";

/* for Julias */
static char realparm[] = "Real Part of Parameter";
static char imagparm[] = "Imaginary Part of Parameter";

/* for Newtons */
static char newtdegree[] = "+Polynomial Degree (>= 2)";

/* for MarksMandel/Julia */
static char exponent[]   = "Real part of Exponent";
static char imexponent[] = "Imag part of Exponent";

/* for Complex Newton */
static char realroot[]	 = "Real part of Root";
static char imagroot[]	 = "Imag part of Root";
static char realdegree[] = "Real part of Degree";
static char imagdegree[] = "Imag part of Degree";

/* for Lorenz */
static char timestep[]	   = "Time Step";

/* for formula */
static char p1real[] = "Real portion of p1";
static char p2real[] = "Real portion of p2";
static char p1imag[] = "Imaginary portion of p1";
static char p2imag[] = "Imaginary portion of p2";

/* trig functions */
static char recoeftrg1[] = "Real Coefficient First Function";
static char imcoeftrg1[] = "Imag Coefficient First Function";
static char recoeftrg2[] = "Real Coefficient Second Function";
static char imcoeftrg2[] = "Imag Coefficient Second Function";

/* MCP 7-7-91
static char recoefsqr[] = "Real Coefficient Square Term";
static char imcoefsqr[] = "Imag Coefficient Square Term";
*/

static char recoef2nd[] = "Real Coefficient Second Term";
static char imcoef2nd[] = "Imag Coefficient Second Term";

/* KAM Torus */
static char kamangle[] = "Angle (radians)";
static char kamstep[] =  "Step size";
static char kamstop[] =  "Stop value";
static char pointsperorbit[] = "+Points per orbit";

/* Newtbasin */
static char stripes[] = "Enter non-zero value for stripes";

/* Newton Polygon */
static char newtpoly[] = "Polynomial Degree (>= 2) 5 is a diamond";

/* Gingerbreadman */
static char initx[] = "Initial x";
static char inity[] = "Initial y";

/* popcorn */
static char step[] = "Step size";

/* bifurcations */
static char filt[] = "+Filter Cycles";
static char seed[] = "Seed Population";

/* frothy basins */
static char frothattractor[] = "+3 or 6 attractor system";
static char frothshade[] =  "+Enter non-zero value for alternate color shading";

/* symmetrical icon fractals */
static char lambda[] = "Lambda";
static char alpha[]  = "Alpha";
static char beta[]   = "Beta";
static char gamma2[]  = "Gamma";
static char omega[]  = "Omega";
static char symdegree[] = "+Degree of symmetry";

static char shiftval[] = "Function Shift Value";

/* ifs */
static char color_method[] = "+Coloring method (0,1)";

/* orbit fractals */
static char A[] = "a";
static char B[] = "b";
static char D[] = "d";
static char H[] = "h";
static char P[] = "p";

/* 4D fractals */
static char C[] = "c";
static char C1[] = "c1";
static char CI[] = "ci";
static char CJ[] = "cj";
static char CK[] = "ck";
static char ZJ[] = "zj";
static char ZK[] = "zk";

/* empty string */
static char ES[] = "";
 
/* bailout defines */
#define FTRIGBAILOUT 2500.0
#define LTRIGBAILOUT   64.0
#define FROTHBAILOUT    6.0
#define STDBAILOUT	4.0
#define NOBAILOUT	0.0

//struct moreparams far moreparams[] = 
//{
//    ICON,  omega,symdegree,ES,ES,ES,ES,0,3,0,0,0,0,
//    ICON3D,omega,symdegree,ES,ES,ES,ES,0,3,0,0,0,0,
//    HYPERCMPLXJFP,ZJ,ZK,   ES,ES,ES,ES,0,0,0,0,0,0,
//    QUATJULFP    ,ZJ,ZK,   ES,ES,ES,ES,0,0,0,0,0,0,   
//    -1,      NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,0,0
//};         

char t_barnsleyj1[]= "*barnsleyj1";
char t_barnsleyj2[]= "*barnsleyj2";
char t_barnsleyj3[]= "*barnsleyj3";
char t_barnsleym1[]= "*barnsleym1";
char t_barnsleym2[]= "*barnsleym2";
char t_barnsleym3[]= "*barnsleym3";
char t_bifplussinpi[]= "*bif+sinpi";
char t_bifeqsinpi[]= "*bif=sinpi";
char t_biflambda[]= "*biflambda";
char t_bifmay[]= "*bifmay";
char t_bifstewart[]= "*bifstewart";
char t_bifurcation[]= "*bifurcation";
char t_demojulia[]= "*demojulia";
char t_demomandel[]= "*demomandel";
char t_fn_z_plusfn_pix_[]= "*fn(z)+fn(pix)";
char t_fn_zz_[]= "*fn(z*z)";
char t_fnfn[]= "*fn*fn";
char t_fnzplusz[]= "*fn*z+z";
char t_fnplusfn[]= "*fn+fn";
char t_formula[]= "*formula";
char t_henon[]= "*henon";
char t_ifs3d[]= "*ifs3d";
char t_julfnplusexp[]= "*julfn+exp";
char t_julfnpluszsqrd[]= "*julfn+zsqrd";
char t_julia[]= "*julia";
char t_julia_fnorfn_[]= "*julia(fn||fn)";
char t_julia4[]= "*julia4";
char t_julia_inverse[]= "*julia_inverse";
char t_julibrot[]= "*julibrot";
char t_julzpower[]= "*julzpower";
char t_kamtorus[]= "*kamtorus";
char t_kamtorus3d[]= "*kamtorus3d";
char t_lambda[]= "*lambda";
char t_lambda_fnorfn_[]= "*lambda(fn||fn)";
char t_lambdafn[]= "*lambdafn";
char t_lorenz[]= "*lorenz";
char t_lorenz3d[]= "*lorenz3d";
char t_mandel[]= "*mandel";
char t_mandel_fnorfn_[]= "*mandel(fn||fn)";
char t_mandel4[]= "*mandel4";
char t_mandelfn[]= "*mandelfn";
char t_mandellambda[]= "*mandellambda";
char t_mandphoenix[]= "*mandphoenix";
char t_mandphoenixcplx[]= "*mandphoenixclx";
char t_manfnplusexp[]= "*manfn+exp";
char t_manfnpluszsqrd[]= "*manfn+zsqrd";
char t_manlam_fnorfn_[]= "*manlam(fn||fn)";
char t_manowar[]= "*manowar";
char t_manowarj[]= "*manowarj";
char t_manzpower[]= "*manzpower";
char t_marksjulia[]= "*marksjulia";
char t_marksmandel[]= "*marksmandel";
char t_marksmandelpwr[]= "*marksmandelpwr";
char t_newtbasin[]= "*newtbasin";
char t_newton[]= "*newton";
char t_phoenix[]= "*phoenix";
char t_phoenixcplx[]= "*phoenixcplx";
char t_popcorn[]= "*popcorn";
char t_popcornjul[]= "*popcornjul";
char t_rossler3d[]= "*rossler3d";
char t_sierpinski[]= "*sierpinski";
char t_spider[]= "*spider";
char t_sqr_1divfn_[]= "*sqr(1/fn)";
char t_sqr_fn_[]= "*sqr(fn)";
char t_tims_error[]= "*tim's_error";
char t_unity[]= "*unity";
char t_frothybasin[]= "*frothybasin";
char t_halley[]= "*halley";

struct	fractalspecificstuff	curfractalspecific;	// info about current fractal

struct fractalspecificstuff fractalspecific[]=
{
    /*	Orig FRACTINT structure
      fractal name, parameter text strings, parameter values,
      helptext, helpformula, flags,
      xmin  xmax  ymin  ymax int tojulia   SlopeDegree  tofloat  symmetry
    |------|-----|-----|-----|--|--------|------------|---------|---------|
      orbit fnct     per_pixel fnct  per_image fnct  calctype fcnt    bailout
    |---------------|---------------|---------------|----------------|-------|
    */
    /*
    {	Orig FRACTINT structure
    fractal name,
       {parameter text strings},
       {parameter values},
       helptext, helpformula, flags,
       xmin, xmax, ymin, ymax,
       int, tojulia, SlopeDegree, tofloat, symmetry,
       orbit fnct, per_pixel fnct, per_image fnct, calctype fcnt,
       bailout
    }
    */
    /*
    {	New ManpWIN structure
    fractal name,
       {parameter text strings},
       {parameter values},
       hor, vert, width,
       NewtonFlag, JuliaFlag, SlopeDegree,
       numparams, numfn, fn1, fn2, flags, symmetry, rqlim,
       init fnct, calctype fcnt, DialogueName, DialogueType, bailout
    }
    */

   {
//#define MANDEL                   0
   t_mandel+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0.0,0.0,0.0,0.0,0.0,0.0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, MANDELFP, 2, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS_NOPARM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_julia+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0.0,0.0,0.0,0.0,0.0,0.0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, JULIAFP, 2, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS_NOPARM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_newtbasin, newtdegree, "Subtype: 0=Normal, 1=Stripes, 2=Basin", ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,0,0,0,0,
   -3.5, -2.0, 4.0, CONVERGING, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   t_lambda+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -3.0,  5.0, -3.0,  3.0, 0, LAMBDAFP, NOFRACTAL, MANDELLAMBDA,  XAXIS_NOPARM,
   //   LambdaFPFractal,mandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
      -4.2, -3.0, 6.0, ESCAPING, LAMBDAFP, 3, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS_NOPARM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_mandel+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0.0,0.0,0.0,0.0,0.0,0.0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, MANDELFP, 2, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS_NOPARM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
//   init_mandel_df, do_mandel_df, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT

   },

   {
//   t_newton, newtdegree, ES, ES, ES, 3.0,0.0,0.0,0.0,
   t_newton, newtdegree, "Subtype: 0=Normal, 1=Stripes, 2=Basin", ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,0,0,0,0,
   -3.5, -2.0, 4.0, CONVERGING, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   t_julia, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0.0,0.0,0.0,0.0,0.0,0.0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, JULIAFP, 2, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS_NOPARM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "plasma",      "Graininess Factor (.1 to 50, default is 2)",
                  ES, ES, ES, ES, ES,ES, ES, ES, ES,
                  2,0,0,0,0,0,0,0,0,0,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 1, 0, NULL, NULL, OTHERFNINPIXEL, NOSYM, 4.0,
   NULL, NULL,  "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   t_mandelfn,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -8.0,  8.0, -6.0,  6.0, 0, LAMBDATRIGFP,NOFRACTAL, MANDELTRIG, XYAXIS_NOPARM,
//   LambdaTrigfpFractal,othermandelfp_per_pixel,MandelTrigSetup,StandardFractal,LTRIGBAILOUT,
   -10.0,  -6.0,  12.0, 0, 2, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_manowar, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.5,  1.5, -1.5,  1.5, 0, MANOWARJFP, NOFRACTAL, MANOWAR,  XAXIS_NOPARM,
//   ManOWarfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
   -4.0,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS_NOPARM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define MANOWAR                 10
   t_manowar+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.5,  1.5, -1.5,  1.5, 0, MANOWARJFP, NOFRACTAL, MANOWAR,  XAXIS_NOPARM,
   //   ManOWarfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
      -4.0,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS_NOPARM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "test",
	"(testpt Param #1)","(testpt param #2)","(testpt param #3)","(testpt param #4)", ES, ES, ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, 2,   NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS_NOPARM, 4.0,
   init_mandel_df, do_mandel_df, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

/*

   {
   "test",
       "(testpt Param #1)","(testpt param #2)","(testpt param #3)","(testpt param #4)", 
       6.0, 2.0, 3.0, 0.0,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 4, 0, NULL, NULL, 0, XAXIS, 4.0,
//   init_Talis, do_Talis, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
//   init_NewtonFlower, do_NewtonFlower, "NewtonGeneral", NewtonGeneral, STDBAILOUT
   NullSetup, mountain, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
*/
/*
   {
   "test","(testpt Param #1)","(testpt param #2)","(testpt param #3)","(testpt param #4)", 0.0, 0.0, 0.0, 0.0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	 NOSYM,
//   NULL,	  NULL, 	    StandaloneSetup, test,    STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 3, 0, NULL, NULL, 0, XAXIS, 4.0,
   marksmandelfp_per_pixel, MarksLambdafpFractal, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
*/

   {
   t_sierpinski+1,  ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -0.9,  1.7, -0.9,  1.7, 1, NOFRACTAL, NOFRACTAL, SIERPINSKIFP,   NOSYM,
//   SierpinskiFractal,long_julia_per_pixel, SierpinskiSetup,StandardFractal,127.0,
   -2.7, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, FRACTINTINPIXEL, NOSYM, 127.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, 127.0
   },

   {
   t_barnsleym1+1,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ1,NOFRACTAL, BARNSLEYM1FP,  XYAXIS_NOPARM,
//   Barnsley1Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
   -3.5,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleyj1+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ1,NOFRACTAL, BARNSLEYM1FP,  XYAXIS_NOPARM,
   //   Barnsley1Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleym2+1,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ2,NOFRACTAL, BARNSLEYM2FP,  YAXIS_NOPARM,
//   Barnsley2Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
   -3.5, -2.0, 4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleyj2+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ2,NOFRACTAL, BARNSLEYM2FP,  YAXIS_NOPARM,
   //   Barnsley2Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
      -3.5, -2.0, 4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_sqr_fn_+1, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, SQRTRIG,XAXIS,
   //   SqrTrigfpFractal,   otherjuliafp_per_pixel, SqrTrigSetup,  StandardFractal,LTRIGBAILOUT,
      -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_sqr_fn_, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, SQRTRIG,XAXIS,
//   SqrTrigfpFractal,   otherjuliafp_per_pixel, SqrTrigSetup,  StandardFractal,LTRIGBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_fnplusfn+1, recoeftrg1,imcoeftrg1,recoeftrg2,imcoeftrg2,ES,ES,ES, ES, ES, ES, 1,0,1,0,0,0,0,0,0,0,
   //   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, TRIGPLUSTRIG,XAXIS,
   //   TrigPlusTrigfpFractal, otherjuliafp_per_pixel, TrigPlusTrigfpSetup,	StandardFractal,LTRIGBAILOUT,
      -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 2, "sin", "tan", TRIGINPIXEL, XAXIS, 64.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define MANDELLAMBDA            20
   t_mandellambda+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -3.0,  5.0, -3.0,  3.0, 0, LAMBDAFP, NOFRACTAL, MANDELLAMBDA,  XAXIS_NOPARM,
   //   LambdaFPFractal,mandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
      -4.2, -3.0, 6.0, ESCAPING, LAMBDAFP, 3, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS_NOPARM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_marksmandel+1, realz0, imagz0, exponent,ES,ES,ES,ES, ES, ES, ES, 0,0,1,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 0, MARKSJULIAFP, NOFRACTAL, MARKSMANDEL,  NOSYM,
   //   MarksLambdafpFractal,marksmandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, 2, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_marksjulia+1, realz0, imagz0, exponent,ES,ES,ES,ES, ES, ES, ES, 0,0,1,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 0, MARKSJULIAFP, NOFRACTAL, MARKSMANDEL,  NOSYM,
   //   MarksLambdafpFractal,marksmandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, 2, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_unity+1, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 1, NOFRACTAL, NOFRACTAL, UNITYFP,   XYAXIS,
//   UnityFractal, long_julia_per_pixel,UnitySetup,StandardFractal,NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_mandel4+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 1, JULIA4,	  NOFRACTAL, MANDEL4FP,  XAXIS_NOPARM,
//   Mandel4Fractal,  mandel_per_pixel, MandellongSetup, StandardFractal,  STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_julia4+1, realparm, imagparm,ES,ES,ES,ES,ES, ES, ES, ES, 0.6,0.55,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 1, NOFRACTAL, MANDEL4, JULIA4FP, ORIGIN,
//   Mandel4Fractal,   julia_per_pixel, JulialongSetup,StandardFractal,	 STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "ifs",color_method,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -8.0,  8.0, -1.0, 11.0, 16, NOFRACTAL, NOFRACTAL, NOFRACTAL,  NOSYM,
//   NULL,	  NULL,      StandaloneSetup, ifs,    NOBAILOUT,
   -3.6,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_ifs3d,color_method,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -11.0,  11.0, -11.0, 11.0, 16, NOFRACTAL, NOFRACTAL, NOFRACTAL,   NOSYM,
//   NULL,	  NULL,      StandaloneSetup, ifs,    NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_barnsleym3+1,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ3,NOFRACTAL, BARNSLEYM3FP,  XAXIS_NOPARM,
//   Barnsley3Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
   -3.5,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleyj3+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ3,NOFRACTAL, BARNSLEYM3FP,  XAXIS_NOPARM,
   //   Barnsley3Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define TRIGSQR                 30
   "fn(z*z)", ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, TRIGSQR,XYAXIS,
   //   TrigZsqrdfpFractal,	 juliafp_per_pixel, JuliafpSetup,  StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_fn_zz_, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, TRIGSQR,XYAXIS,
//   TrigZsqrdfpFractal,	 juliafp_per_pixel, JuliafpSetup,  StandardFractal,STDBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_bifurcation,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   1.9,  3.0, 0,  1.34, 0, NOFRACTAL, NOFRACTAL, LBIFURCATION, NOSYM,
//   BifurcVerhulstTrig, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   1.8,  0.0,  1.5, 0, NOFRACTAL, NOFRACTAL, 2, 1, "ident", NULL, 0, NOSYM, 4.0,
//   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 1, "ident", NULL, 0, XAXIS, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_fnplusfn,recoeftrg1,imcoeftrg1,recoeftrg2,imcoeftrg2,ES,ES,ES, ES, ES, ES, 1,0,1,0,0,0,0,0,0,0,
   //   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, TRIGPLUSTRIG,XAXIS,
   //   TrigPlusTrigfpFractal, otherjuliafp_per_pixel, TrigPlusTrigfpSetup,	StandardFractal,LTRIGBAILOUT,
      -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 2, "sin", "tan", TRIGINPIXEL, XAXIS, 64.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "fn*fn", ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, TRIGXTRIG,XAXIS,
   //   TrigXTrigfpFractal, otherjuliafp_per_pixel, FnXFnSetup,  StandardFractal,LTRIGBAILOUT,
      -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 2, "sin", "tan", TRIGINPIXEL, XAXIS, 64.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_fnfn, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, TRIGXTRIG,XAXIS,
//   TrigXTrigfpFractal, otherjuliafp_per_pixel, FnXFnSetup,  StandardFractal,LTRIGBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 2, "sin", "tan", TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_sqr_1divfn_+1,ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 16, NOFRACTAL, NOFRACTAL, SQR1OVERTRIGFP,NOSYM,
//   Sqr1overTrigFractal, long_julia_per_pixel, SqrTrigSetup,  StandardFractal,LTRIGBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_sqr_1divfn_,ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, SQR1OVERTRIG,NOSYM,
//   Sqr1overTrigfpFractal, otherjuliafp_per_pixel, SqrTrigSetup,  StandardFractal,LTRIGBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   "fn*z+z", recoeftrg1, imcoeftrg2, recoef2nd,imcoef2nd,ES,ES,ES, ES, ES, ES, 1,0,1,0,0,0,0,0,0,0,
   //   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, ZXTRIGPLUSZ,XAXIS,
   //   ZXTrigPlusZfpFractal,   juliafp_per_pixel, ZXTrigPlusZSetup,  StandardFractal,LTRIGBAILOUT,
      -3.5,  -2.5,  5.0, 0, NOFRACTAL, NOFRACTAL, 4, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_fnzplusz,recoeftrg1, imcoeftrg2, recoef2nd,imcoef2nd,ES,ES,ES, ES, ES, ES, 1,0,1,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, NOFRACTAL, ZXTRIGPLUSZ,XAXIS,
//   ZXTrigPlusZfpFractal,   juliafp_per_pixel, ZXTrigPlusZSetup,  StandardFractal,LTRIGBAILOUT,
   -3.5,  -2.5,  5.0, 0, NOFRACTAL, NOFRACTAL, 4, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
//#define KAMFP                   40
   t_kamtorus,kamangle,kamstep,kamstop,pointsperorbit,ES,ES,ES, ES, ES, ES, 1.3,.05,1.5,15000,0,0,0,0,0,0,
//   -1.0,  1.0, -.75, .75, 0, NOFRACTAL, NOFRACTAL, KAM,   NOSYM,
//   kamtorusfloatorbit, NULL, orbit3dfloatsetup, orbit2dfloat, NOBAILOUT,
   -4.0,  -2.4,  5.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit2dfloat, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   t_kamtorus+1,kamangle,kamstep,kamstop,pointsperorbit,ES,ES,ES, ES, ES, ES, 1.3,.05,1.5,150,0,0,0,0,0,0,
//   -1.0,  1.0, -.75, .75,16, NOFRACTAL, NOFRACTAL, KAMFP, NOSYM,
//   kamtoruslongorbit,  NULL, orbit3dlongsetup, orbit2dlong,   NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_kamtorus3d,kamangle,kamstep,kamstop,pointsperorbit,ES,ES,ES, ES, ES, ES, 1.3,0.07,7.5,1500,0,0,0,0,0,0,
//   -3.0,  3.0, -1, 3.5, 0, NOFRACTAL, NOFRACTAL, KAM3D,   NOSYM,
//   kamtorusfloatorbit, NULL, orbit3dfloatsetup, orbit3dfloat, NOBAILOUT,
   -2.8,  -0.74,  3.3, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit3dfloatcalc, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   t_kamtorus3d+1,kamangle,kamstep,kamstop,pointsperorbit,ES,ES,ES, ES, ES, ES, 1.3,.05,1.5,150,0,0,0,0,0,0,
//   -3.0,  3.0, -1, 3.5,16, NOFRACTAL, NOFRACTAL, KAM3DFP, NOSYM,
//   kamtoruslongorbit,  NULL, orbit3dlongsetup, orbit3dlong,   NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_lambdafn+1, realparm, imagparm,ES,ES,ES,ES,ES, ES, ES, ES, 1.0,0.4,0,0,0,0,0,0,0,0,
   //   -2.0, 2.0, -1.5, 1.5, 0, NOFRACTAL, MANDELTRIGFP, LAMBDATRIG, PI_SYM,
   //   LambdaTrigfpFractal,otherjuliafp_per_pixel,LambdaTrigSetup,StandardFractal,LTRIGBAILOUT,
      -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
      //   TrigSetup, LambdaTrigfp, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
	 NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_manfnpluszsqrd+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.5, 1.5, -1.5,  1.5, 0, FPJULTRIGPLUSZSQRD,   NOFRACTAL, LMANTRIGPLUSZSQRD, XAXIS_NOPARM,
   //   TrigPlusZsquaredfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal, STDBAILOUT,
      -4.0,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_julfnpluszsqrd+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.5, 1.5, -1.5,  1.5, 0, FPJULTRIGPLUSZSQRD,   NOFRACTAL, LMANTRIGPLUSZSQRD, XAXIS_NOPARM,
   //   TrigPlusZsquaredfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal, STDBAILOUT,
      -4.0,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_manfnpluszsqrd, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.5, 1.5, -1.5,  1.5, 0, FPJULTRIGPLUSZSQRD,   NOFRACTAL, LMANTRIGPLUSZSQRD, XAXIS_NOPARM,
//   TrigPlusZsquaredfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal, STDBAILOUT,
   -4.0,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_julfnpluszsqrd, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.5, 1.5, -1.5,  1.5, 0, FPJULTRIGPLUSZSQRD,   NOFRACTAL, LMANTRIGPLUSZSQRD, XAXIS_NOPARM,
   //   TrigPlusZsquaredfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal, STDBAILOUT,
      -4.0,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_lambdafn,  realparm, imagparm,ES,ES,ES,ES,ES, ES, ES, ES, 1.0,0.4,0,0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 0, NOFRACTAL, MANDELTRIGFP, LAMBDATRIG, PI_SYM,
//   LambdaTrigfpFractal,otherjuliafp_per_pixel,LambdaTrigSetup,StandardFractal,LTRIGBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 1, "sin", NULL, 0, XAXIS, 64.0,
//   TrigSetup, LambdaTrigfp, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
//#define MANDELTRIG              50
   t_mandelfn+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -8.0,  8.0, -6.0,  6.0, 0, LAMBDATRIGFP,NOFRACTAL, MANDELTRIG, XYAXIS_NOPARM,
   //   LambdaTrigfpFractal,othermandelfp_per_pixel,MandelTrigSetup,StandardFractal,LTRIGBAILOUT,
      -10.0,  -6.0,  12.0, 0, 2, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, LTRIGBAILOUT
   },

   {
   t_manzpower+1, realz0, imagz0, exponent,imexponent,ES,ES,ES, ES, ES, ES, 0,0,2,0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, LJULIAZPOWER, NOFRACTAL, FPMANDELZPOWER,	XAXIS_NOIMAG,
//   longZpowerFractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
   -2.7, -2.0, 4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_julzpower+1, realparm, imagparm, exponent,imexponent,ES,ES,ES, ES, ES, ES, 0.3,0.6,2,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 1, NOFRACTAL, LMANDELZPOWER, FPJULIAZPOWER,	 ORIGIN,
//   longZpowerFractal,long_julia_per_pixel,JulialongSetup,StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_manzpower, realz0, imagz0, exponent,imexponent,ES,ES,ES, ES, ES, ES, 0,0,2,0,0,0,0,0,0,0,
//   -2.5,  1.5, -1.5,  1.5, 0, FPJULIAZPOWER,   NOFRACTAL, LMANDELZPOWER,  XAXIS_NOIMAG,
//   floatZpowerFractal,othermandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
   -3.5,  -2.0,  4.0, 0, 2, NOFRACTAL, 4, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_julzpower, realz0, imagz0, exponent,imexponent,ES,ES,ES, ES, ES, ES, 0,0,2,0,0,0,0,0,0,0,
   //   -2.5,  1.5, -1.5,  1.5, 0, FPJULIAZPOWER,   NOFRACTAL, LMANDELZPOWER,  XAXIS_NOIMAG,
   //   floatZpowerFractal,othermandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 4, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "manzzpwr", realz0, imagz0, exponent,ES,ES,ES,ES, ES, ES, ES, 0,0,2,0,0,0,0,0,0,0,
//   -2.5,  1.5, -1.5, 1.5, 0, FPJULZTOZPLUSZPWR,   NOFRACTAL, NOFRACTAL,  XAXIS_NOPARM,
//   floatZtozPluszpwrFractal,othermandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
   -4.0,  -2.0,  4.0, 0, 2, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "julzzpwr", realz0, imagz0, exponent,ES,ES,ES,ES, ES, ES, ES, 0,0,2,0,0,0,0,0,0,0,
   //   -2.5,  1.5, -1.5, 1.5, 0, FPJULZTOZPLUSZPWR,   NOFRACTAL, NOFRACTAL,  XAXIS_NOPARM,
   //   floatZtozPluszpwrFractal,othermandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
      -4.0,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_manfnplusexp+1,realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -8.0,  8.0, -6.0,  6.0, 16, LJULTRIGPLUSEXP,    NOFRACTAL,  FPMANTRIGPLUSEXP, XAXIS_NOPARM,
//   LongTrigPlusExponentFractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_julfnplusexp+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -8.0,  8.0, -6.0,  6.0, 16, LJULTRIGPLUSEXP,    NOFRACTAL,  FPMANTRIGPLUSEXP, XAXIS_NOPARM,
   //   LongTrigPlusExponentFractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
      -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
      init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_manfnplusexp, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -8.0,  8.0, -6.0,  6.0, 0, FPJULTRIGPLUSEXP, NOFRACTAL, LMANTRIGPLUSEXP,   XAXIS_NOPARM,
//   FloatTrigPlusExponentFractal,othermandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
   -5.4,  -3.0,  6.0, 0, 2, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define FPJULTRIGPLUSEXP        60
   t_julfnplusexp, realparm, imagparm,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, FPMANTRIGPLUSEXP, LJULTRIGPLUSEXP,   NOSYM,
//   FloatTrigPlusExponentFractal,otherjuliafp_per_pixel,JuliafpSetup,StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_popcorn, step, ES, ES,ES,ES,ES,ES, ES, ES, ES, 0.05,0,0,0,0,0,0,0,0,0,
//   -3.0,  3.0, -2.2,  2.2, 0, NOFRACTAL, NOFRACTAL, LPOPCORN,  NOPLOT,
//   PopcornFractal, otherjuliafp_per_pixel,  JuliafpSetup,  popcorn,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_popcorn+1, step, ES, ES,ES,ES,ES,ES, ES, ES, ES, 0.05,0,0,0,0,0,0,0,0,0,
//   -3.0,  3.0, -2.2,  2.2, 16, NOFRACTAL, NOFRACTAL, FPPOPCORN,  NOPLOT,
//   LPopcornFractal,   long_julia_per_pixel, JulialongSetup,popcorn,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_lorenz,timestep,A,B, C,ES,ES,ES, ES, ES, ES, 0.02,5,15,1,0,0,0,0,0,0,
//   -15,  15, 0, 30, 0, NOFRACTAL, NOFRACTAL, LLORENZ,   NOSYM,
//   lorenz3dfloatorbit, NULL,	     orbit3dfloatsetup, orbit2dfloat,	 NOBAILOUT,
   -25.0,  0.0,  28.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit2dfloat, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_lorenz+1,timestep,A,B, C,ES,ES,ES, ES, ES, ES, 0.02,5,15,1,0,0,0,0,0,0,
//   -15,  15, 0, 30, 16, NOFRACTAL, NOFRACTAL, FPLORENZ,   NOSYM,
//   lorenz3dlongorbit, NULL,	    orbit3dlongsetup, orbit2dlong,    NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_lorenz3d+1,timestep,A,B, C,ES,ES,ES, ES, ES, ES, 0.02,5,15,1,0,0,0,0,0,0,
//   -30.0,  30.0,  -30.0,   30.0, 16, NOFRACTAL, NOFRACTAL, FPLORENZ3D,   NOSYM,
//   lorenz3dlongorbit, NULL,	    orbit3dlongsetup, orbit3dlong,    NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_newton+1, newtdegree, "Subtype: 0=Normal, 1=Stripes, 2=Basin", ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,0,0,0,0,
   -3.5, -2.0, 4.0, CONVERGING, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   t_newtbasin+1, newtdegree, "Subtype: 0=Normal, 1=Stripes, 2=Basin", ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,0,0,0,0,
   -3.5, -2.0, 4.0, CONVERGING, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   "complexnewton", realdegree,imagdegree,realroot,imagroot,"Subtype: 0=Normal, 1=Basin", ES, ES, ES, ES, ES, 3.0, 0.0, 1.0, 0.0, 0.0, 0.0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	 NOSYM,
//   ComplexNewton, otherjuliafp_per_pixel,  ComplexNewtonSetup, StandardFractal,NOBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, FRACTINTINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   "complexbasin",  realdegree,imagdegree,realroot,imagroot,"Subtype: 0=Normal, 1=Basin", ES, ES, ES, ES, ES, 3.0, 0.0, 1.0, 0.0, 0.0, 0.0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	 NOSYM,
   //   ComplexNewton, otherjuliafp_per_pixel,  ComplexNewtonSetup, StandardFractal,NOBAILOUT,
      -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, FRACTINTINPIXEL, NOSYM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
//#define COMPLEXMARKSMAND        70
   "cmplxmarksmand", realz0, imagz0, realdegree, imagdegree,ES,ES,ES, ES, ES, ES, 0,0,1,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, COMPLEXMARKSJUL, NOFRACTAL, NOFRACTAL,   NOSYM,
//   MarksCplxMand, MarksCplxMandperp, MandelfpSetup, StandardFractal, STDBAILOUT,
   -3.6,  -2.0,  4.0, 0, 2, NOFRACTAL, 4, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "cmplxmarksjul", realparm, imagparm, realdegree, imagdegree,ES,ES,ES, ES, ES, ES, 0.3,0.6,1,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, COMPLEXMARKSMAND, NOFRACTAL,	NOSYM,
//   MarksCplxMand, juliafp_per_pixel, JuliafpSetup, StandardFractal, STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 4, 0, NULL, NULL, 0, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_formula+1, p1real, p1imag, p2real, p2imag,ES,ES, ES, ES, ES, ES, 0,0,.0,0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, XAXIS, 4.0,
   form_per_pixel, Formula, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_formula, p1real, p1imag, p2real, p2imag,ES,ES, ES, ES, ES, ES, 0,0,0.0,0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 0, NOFRACTAL, NOFRACTAL, FORMULA, SETUP_SYM,
//   Formula, form_per_pixel, fpFormulaSetup, StandardFractal, 0,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, SETUP_SYM, 4.0,
   form_per_pixel, Formula, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_sierpinski, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -0.9,  1.7, -0.9,  1.7, 1, NOFRACTAL, NOFRACTAL, SIERPINSKIFP,   NOSYM,
   //   SierpinskiFractal,long_julia_per_pixel, SierpinskiSetup,StandardFractal,127.0,
      -2.7, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, FRACTINTINPIXEL, NOSYM, 127.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, 127.0
   },

   {
   t_lambda, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -3.0,  5.0, -3.0,  3.0, 0, LAMBDAFP, NOFRACTAL, MANDELLAMBDA,  XAXIS_NOPARM,
   //   LambdaFPFractal,mandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
      -4.2, -3.0, 6.0, ESCAPING, LAMBDAFP, 3, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS_NOPARM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleym1,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ1,NOFRACTAL, BARNSLEYM1FP,  XYAXIS_NOPARM,
   //   Barnsley1Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleyj1,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ1,NOFRACTAL, BARNSLEYM1FP,  XYAXIS_NOPARM,
   //   Barnsley1Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleym2,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ2,NOFRACTAL, BARNSLEYM2FP,  YAXIS_NOPARM,
   //   Barnsley2Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
      -3.5, -2.0, 4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleyj2,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, BARNSLEYJ2,NOFRACTAL, BARNSLEYM2FP,  YAXIS_NOPARM,
   //   Barnsley2Fractal,long_mandel_per_pixel,MandellongSetup,StandardFractal,STDBAILOUT,
      -3.5, -2.0, 4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define BARNSLEYM3FP            80
   t_barnsleym3, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, BARNSLEYJ3FP, NOFRACTAL, BARNSLEYM3,  XAXIS_NOPARM,
//   Barnsley3FPFractal,othermandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL,NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_barnsleyj3, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 0, BARNSLEYJ3FP, NOFRACTAL, BARNSLEYM3,  XAXIS_NOPARM,
   //   Barnsley3FPFractal,othermandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
      -2.7,  -2.0,  4.0, 0, 2, JULIAFP, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL,NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_mandellambda,realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -3.0,  5.0, -3.0,  3.0, 0, LAMBDAFP, NOFRACTAL, MANDELLAMBDA,  XAXIS_NOPARM,
//   LambdaFPFractal,mandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
   -4.2, -3.0, 6.0, ESCAPING, LAMBDAFP, 3, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS_NOPARM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_julibrot+1, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, JULIBROTFP, NOSYM,
//   JuliaFractal, jb_per_pixel, JulibrotSetup, Std4dFractal, STDBAILOUT,
   -2.7, -2.0, 4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_lorenz3d,timestep,A,B,C,ES,ES,ES, ES, ES, ES, 0.02,5,15,1,0,0,0,0,0,0,
//   -30.0,  30.0,  -30.0,   30.0, 0, NOFRACTAL, NOFRACTAL, LLORENZ3D,   NOSYM,
//   lorenz3dfloatorbit, NULL,	     orbit3dfloatsetup, orbit3dfloat,	 NOBAILOUT,
   -28.0,  -15.0,  30.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit3dfloatcalc, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_rossler3d+1,timestep,A,B,C,ES,ES,ES, ES, ES, ES, 0.04, 0.2,.2,5.7,0,0,0,0,0,0,
//   -30.0,  30.0,  -20.0,   40.0, 16, NOFRACTAL, NOFRACTAL, FPROSSLER,	NOSYM,
//   rosslerlongorbit, NULL,	   orbit3dlongsetup, orbit3dlong,    NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_rossler3d,timestep,A,B,C,ES,ES,ES, ES, ES, ES, 0.04, 0.2,.2,5.7,0,0,0,0,0,0,
//   -30.0,  30.0,  -20.0,   40.0, 0, NOFRACTAL, NOFRACTAL, LROSSLER,   NOSYM,
//   rosslerfloatorbit, NULL,	    orbit3dfloatsetup, orbit3dfloat,	NOBAILOUT,
   -48.0,  -16.0,  55.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit3dfloatcalc, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   t_henon+1,A,B,ES,ES,ES,ES,ES, ES, ES, ES, 1.4,.3,0,0,0,0,0,0,0,0,
//   -1.4,  1.4,	-.5,   .5, 16, NOFRACTAL, NOFRACTAL, FPHENON,	NOSYM,
//   henonlongorbit, NULL,	 orbit3dlongsetup, orbit2dlong,    NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_henon,A,B,ES,ES,ES,ES,ES, ES, ES, ES, 1.4,.3,0,0,0,0,0,0,0,0,
//   -1.4,  1.4,	-.5,   .5, 0, NOFRACTAL, NOFRACTAL, LHENON,   NOSYM,
//   henonfloatorbit, NULL,	  orbit3dfloatsetup, orbit2dfloat,    NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   "pickover",A,B,C,D,ES,ES,ES, ES, ES, ES, 2.24,.43,-.65,-2.43,0,0,0,0,0,0,
//   -2.8,  2.8,	-2.0, 2.0, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	 NOSYM,
//   pickoverfloatorbit, NULL,	     orbit3dfloatsetup, orbit3dfloat,	 NOBAILOUT,
   -4.0,  -2.5,  5.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, orbit3dfloatcalc, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define FPGINGERBREAD           90
   "gingerbreadman",initx,inity,ES,ES,ES,ES,ES, ES, ES, ES, -.1,0,0,0,0,0,0,0,0,0,
//   -4.5,  8.5,	-4.5, 8.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	 NOSYM,
//   gingerbreadfloatorbit, NULL, orbit3dfloatsetup, orbit2dfloat,    NOBAILOUT,
   -12.0,  -6.0,  16.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit2dfloat, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "diffusion", "+Border size","+Type (0=Central,1=Falling,2=Square Cavity)",ES, ES,ES,ES,ES, ES, ES, ES, 10,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	 NOSYM,
//   NULL,   NULL,     StandaloneSetup, diffusion,    NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, OTHERFNINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_unity, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, UNITY,   XYAXIS,
//   UnityfpFractal, otherjuliafp_per_pixel,StandardSetup,StandardFractal,NOBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, NULL, NULL, NOBAILOUT
   },

   {
   t_spider, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.5,  1.5, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, SPIDER,  XAXIS_NOPARM,
   //   SpiderfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
      -3.8,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_spider+1,  realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.5,  1.5, -1.5,  1.5, 1, NOFRACTAL, NOFRACTAL, SPIDERFP,  XAXIS_NOPARM,
//   SpiderFractal,mandel_per_pixel, MandellongSetup,StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },
   
   {
   "tetrate", realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	XAXIS,
//   TetratefpFractal,othermandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "magnet1m", realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -4.0, 4.0, -3.0, 3.0, 0, MAGNET1J,NOFRACTAL,NOFRACTAL, XAXIS_NOPARM,
//   Magnet1Fractal,mandelfp_per_pixel,MandelfpSetup,StandardFractal,100.0,
   -3.7,  -3.0,  6.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
//   init_mandel_df, Magnet1Fractal, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT	// replaced with tierazon fractal
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "magnet1j", realparm, imagparm,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -8.0,  8.0, -6.0,  6.0, 0, NOFRACTAL,MAGNET1M,NOFRACTAL, XAXIS_NOIMAG,
//   Magnet1Fractal,juliafp_per_pixel,JuliafpSetup,StandardFractal,100.0,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
//   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "magnet2m", realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -1.5,3.7, -1.95,1.95,   0, MAGNET2J,NOFRACTAL,NOFRACTAL, XAXIS_NOPARM,
//   Magnet2Fractal,mandelfp_per_pixel,MandelfpSetup,StandardFractal,100.0,
   -2.7,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
//   init_mandel_df, Magnet2Fractal, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT	// replaced with tierazon fractal
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "magnet2j", realparm, imagparm,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -8.0,  8.0, -6.0,  6.0, 0, NOFRACTAL,MAGNET2M,NOFRACTAL, XAXIS_NOIMAG,
//   Magnet2Fractal,juliafp_per_pixel,JuliafpSetup,StandardFractal,100.0,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
//   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define LBIFURCATION           100
   t_bifurcation+1,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   1.9,  3.0, 0,  1.34, 1, NOFRACTAL, NOFRACTAL, BIFURCATION, NOSYM,
//   LongBifurcVerhulstTrig, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 1, "sin", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_biflambda+1,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   -2.0, 4.0, -1.0, 2.0, 1, NOFRACTAL, NOFRACTAL, BIFLAMBDA,   NOSYM,
//   LongBifurcLambdaTrig, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   -2.0,  -2.0,  3.6, 0, NOFRACTAL, NOFRACTAL, 2, 1, "ident", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_biflambda,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   -2.0, 4.0, -1.0, 2.0, 0, NOFRACTAL, NOFRACTAL, LBIFLAMBDA,  NOSYM,
//   BifurcLambdaTrig, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   -2.0,  -2.0,  3.6, 0, NOFRACTAL, NOFRACTAL, 2, 1, "ident", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_bifplussinpi,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   0.275,1.45, 0.0, 2.0, 0, NOFRACTAL, NOFRACTAL, LBIFADSINPI,	 NOSYM,
//   BifurcAddTrigPi, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   0.275, 0.0, 2.0, 0, NOFRACTAL, NOFRACTAL, 2, 1, "sin", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_bifeqsinpi,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   -2.5, 2.5, -3.5, 3.5, 0, NOFRACTAL, NOFRACTAL, LBIFEQSINPI,	 NOSYM,
//   BifurcSetTrigPi, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 1, "sin", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_popcornjul, step, ES, ES,ES,ES,ES,ES, ES, ES, ES, 0.05,0,0,0,0,0,0,0,0,0,
//   -3.0,  3.0, -2.2,  2.2, 0, NOFRACTAL, NOFRACTAL, LPOPCORNJUL,  ORIGIN,
//   PopcornFractal, otherjuliafp_per_pixel,  JuliafpSetup,StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_popcornjul+1, step, ES, ES,ES,ES,ES,ES, ES, ES, ES, 0.05,0,0,0,0,0,0,0,0,0,
//   -3.0,  3.0, -2.2,  2.2, 16, NOFRACTAL, NOFRACTAL, FPPOPCORNJUL,  ORIGIN,
//   LPopcornFractal,   long_julia_per_pixel, JulialongSetup,  StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   "lsystem", "+Order",ES,ES,ES,ES,ES,ES, ES, ES, ES, 2,0,0,0,0,0,0,0,0,0,
//   -1, 1, -1, 1, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//   NULL, NULL, StandaloneSetup, Lsystem, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_manowarj,   realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.5,  1.5, -1.5,  1.5, 0, MANOWARJFP, NOFRACTAL, MANOWAR,  XAXIS_NOPARM,
   //   ManOWarfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
      -4.0,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS_NOPARM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_manowarj+1,   realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.5,  1.5, -1.5,  1.5, 0, MANOWARJFP, NOFRACTAL, MANOWAR,  XAXIS_NOPARM,
   //   ManOWarfpFractal,mandelfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
      -4.0,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS_NOPARM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define FNPLUSFNPIXFP          110
   t_fn_z_plusfn_pix_, realz0,imagz0,recoeftrg2,imcoeftrg2,ES,ES,ES, ES, ES, ES, 0,0,1,0,0,0,0,0,0,0,
//   -2.5,  1.5, -1.5, 1.5, 0, NOFRACTAL, NOFRACTAL, FNPLUSFNPIXLONG, NOSYM,
//   Richard8fpFractal,otherrichard8fp_per_pixel, MandelfpSetup,StandardFractal,LTRIGBAILOUT,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 2, "sin", "Sqr", TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_fn_z_plusfn_pix_+1, realz0,imagz0,recoeftrg2,imcoeftrg2,ES,ES,ES, ES, ES, ES, 0,0,1,0,0,0,0,0,0,0,
   //   -2.5,  1.5, -1.5, 1.5, 0, NOFRACTAL, NOFRACTAL, FNPLUSFNPIXLONG, NOSYM,
   //   Richard8fpFractal,otherrichard8fp_per_pixel, MandelfpSetup,StandardFractal,LTRIGBAILOUT,
      -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 2, "sin", "Sqr", TRIGINPIXEL, XAXIS, 64.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_marksmandelpwr, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.5,  1.5, -1.5, 1.5, 0, NOFRACTAL, NOFRACTAL, MARKSMANDELPWR, XAXIS_NOPARM,
//   MarksMandelPwrfpFractal,marks_mandelpwrfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
   -3.5,  -2.0,  4.0, 0, -1, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_marksmandelpwr+1, realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.5,  1.5, -1.5, 1.5, 0, NOFRACTAL, NOFRACTAL, MARKSMANDELPWR, XAXIS_NOPARM,
   //   MarksMandelPwrfpFractal,marks_mandelpwrfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, -1, NOFRACTAL, 2, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_tims_error,    realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.5,  3.0, -2.0,  2.0, 0, NOFRACTAL,	 NOFRACTAL, TIMSERROR,	XAXIS_NOPARM,
//   TimsErrorfpFractal,marks_mandelpwrfp_per_pixel, MandelfpSetup,StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_tims_error+1,    realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.5,  3.0, -2.0,  2.0, 1, NOFRACTAL,	 NOFRACTAL, TIMSERRORFP,  XAXIS_NOPARM,
//   TimsErrorFractal,marks_mandelpwr_per_pixel, MandelSetup,StandardFractal,STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_bifeqsinpi+1,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   -2.5, 2.5, -3.5, 3.5, 1, NOFRACTAL, NOFRACTAL, BIFEQSINPI,	 NOSYM,
//   LongBifurcSetTrigPi, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 1, "ident", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_bifplussinpi+1,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   0.275,1.45, 0.0, 2.0, 1, NOFRACTAL, NOFRACTAL, BIFADSINPI,	 NOSYM,
//   LongBifurcAddTrigPi, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 1, "ident", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_bifstewart,filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   0.7,2.0, -1.1,1.1, 0, NOFRACTAL, NOFRACTAL, LBIFSTEWART, NOSYM,
//   BifurcStewartTrig, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   0.7,  -1.1,  2.2, 0, NOFRACTAL, NOFRACTAL, 2, 1, "ident", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_bifstewart+1, filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.66,0,0,0,0,0,0,0,0,
//   0.7,2.0, -1.1,1.1, 1, NOFRACTAL, NOFRACTAL, BIFSTEWART, NOSYM,
//   LongBifurcStewartTrig, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   0.7,  -1.1,  2.2, 0, NOFRACTAL, NOFRACTAL, 2, 1, "ident", NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define FPHOPALONG             120
   "hopalong",A,B,C,ES,ES,ES,ES, ES, ES, ES, 0.4,1,0,0,0,0,0,0,0,0,
//   -2.0, 3.0, -1.625, 2.625, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	NOSYM,
//   hopalong2dfloatorbit, NULL,	     orbit3dfloatsetup, orbit2dfloat,	 NOBAILOUT,
   -3.6,  -1.7,  4.3, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, orbit2dfloat, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   "circle", "magnification",ES,ES,ES,ES,ES,ES, ES, ES, ES, 200000,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,  XYAXIS,
//   CirclefpFractal, juliafp_per_pixel,  JuliafpSetup,StandardFractal,NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 1, 0, NULL, NULL, 0, XYAXIS, 4.0,
   NULL, NULL, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   "martin","Try values near Pi",ES,ES,ES,ES,ES,ES, ES, ES, ES, 3.14,0,0,0,0,0,0,0,0,0,
//   -32,  32, -32, 32, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	NOSYM,
//   martin2dfloatorbit, NULL,	     orbit3dfloatsetup, orbit2dfloat,	 NOBAILOUT,
   -60.0,  -34.0,  70.0, 0, NOFRACTAL, NOFRACTAL, 1, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit2dfloat, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   "lyapunov", "Population Seed", "Filter Cycles", "C (Z-axis)", ES,ES,ES, ES, ES, ES, ES, 0.5, 0.0, 0, 0,0,0,0,0,0,0,
//   -8.0, 8.0, -6.0, 6.0, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//   BifurcLambda, NULL, lya_setup, lyapunov, NOBAILOUT,
   -10.0,  -6.0,  12.0, ESCAPING, NOFRACTAL, NOFRACTAL, 3, 0, "BA", NULL, FRACTINTINPIXEL, NOSYM, 4.0,
   NULL, NULL, "LyapDlg", (DLGPROC)LyapDlg, NOBAILOUT
   },

   {
   "lorenz3d1",timestep,A,B,C,ES,ES,ES, ES, ES, ES, 0.02,5,15,1,0,0,0,0,0,0,
//   -30.0,  30.0,  -30.0,   30.0, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//   lorenz3d1floatorbit, NULL, orbit3dfloatsetup, orbit3dfloat, NOBAILOUT,
   -140.0,  -84.0,  168.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit3dfloatcalc, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "lorenz3d3",timestep,A,B,C,ES,ES,ES, ES, ES, ES, 0.02,10,28,2.66,0,0,0,0,0,0,
//   -30.0,  30.0,  -30.0,   30.0, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//   lorenz3d3floatorbit, NULL, orbit3dfloatsetup, orbit3dfloat, NOBAILOUT,
   -50.0,  -30.0,  60.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit3dfloatcalc, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "lorenz3d4",timestep,A,B,C,ES,ES,ES, ES, ES, ES, 0.02,10,28,2.66,0,0,0,0,0,0,
//   -30.0,  30.0,  -30.0,   30.0, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//   lorenz3d4floatorbit, NULL, orbit3dfloatsetup, orbit3dfloat, NOBAILOUT,
   -50.0,  -30.0,  60.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   orbit3dfloatsetup, orbit3dfloatcalc, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_lambda_fnorfn_+1, realparm, imagparm, shiftval, ES,ES,ES,ES, ES, ES, ES, 1,0.1,1,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 16, NOFRACTAL, LMANLAMFNFN, FPLAMBDAFNFN, ORIGIN,
//   LambdaTrigOrTrigFractal, long_julia_per_pixel, LambdaTrigOrTrigSetup, StandardFractal,LTRIGBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 3, 2, "sin", "sqr", TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_lambda_fnorfn_, realparm, imagparm, shiftval,ES,ES,ES,ES, ES, ES, ES, 1,0.1,1,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, FPMANLAMFNFN, LLAMBDAFNFN,ORIGIN,
//   LambdaTrigOrTrigfpFractal, otherjuliafp_per_pixel, LambdaTrigOrTrigSetup, StandardFractal,LTRIGBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 3, 2, "sin", "sqr", TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_julia_fnorfn_+1, realparm, imagparm, shiftval, ES,ES,ES,ES, ES, ES, ES, 0,0,8,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 16, NOFRACTAL, LMANFNFN, FPJULFNFN,XAXIS,
//   JuliaTrigOrTrigFractal, long_julia_per_pixel, JuliaTrigOrTrigSetup, StandardFractal,LTRIGBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   NULL, NULL, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
//#define FPJULFNFN              130
   t_julia_fnorfn_, realparm, imagparm, shiftval,ES,ES,ES,ES, ES, ES, ES, 0,0,8,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, NOFRACTAL, FPMANFNFN, LJULFNFN,XAXIS,
//   JuliaTrigOrTrigfpFractal, otherjuliafp_per_pixel, JuliaTrigOrTrigSetup, StandardFractal,LTRIGBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, TRIGINPIXEL, XAXIS, 4.0,
   NULL, NULL, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_manlam_fnorfn_+1, realz0, imagz0, shiftval,ES,ES,ES,ES, ES, ES, ES, 0,0,10,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 16, LLAMBDAFNFN, NOFRACTAL, FPMANLAMFNFN,XAXIS_NOPARM,
//   LambdaTrigOrTrigFractal, long_mandel_per_pixel, ManlamTrigOrTrigSetup, StandardFractal,LTRIGBAILOUT,
   -2.7,  -2.0,  4.0, 0, MANDELFP, NOFRACTAL, 3, 2, "sin", "sqr", TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_manlam_fnorfn_, realz0, imagz0, shiftval,ES,ES,ES,ES, ES, ES, ES, 0,0,10,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, FPLAMBDAFNFN, NOFRACTAL, LMANLAMFNFN,XAXIS_NOPARM,
//   LambdaTrigOrTrigfpFractal, othermandelfp_per_pixel, ManlamTrigOrTrigSetup, StandardFractal,LTRIGBAILOUT,
   -3.5,  -2.0,  4.0, 0, MANDELFP, NOFRACTAL, 3, 2, "sin", "sqr", TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_mandel_fnorfn_+1, realz0, imagz0, shiftval,ES,ES,ES,ES, ES, ES, ES, 0,0,0.5,0,0,0,0,0,0,0,
   //   -4.0,  4.0, -3.0,  3.0, 0, FPJULFNFN, NOFRACTAL, LMANFNFN,XAXIS_NOPARM,
   //   JuliaTrigOrTrigfpFractal, othermandelfp_per_pixel, MandelTrigOrTrigSetup, StandardFractal,LTRIGBAILOUT,
      -3.8,  -2.0,  4.0, 0, MANDELFP, 2, 3, 2, "sin", "sqr", TRIGINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_mandel_fnorfn_, realz0, imagz0, shiftval,ES,ES,ES,ES, ES, ES, ES, 0,0,0.5,0,0,0,0,0,0,0,
//   -4.0,  4.0, -3.0,  3.0, 0, FPJULFNFN, NOFRACTAL, LMANFNFN,XAXIS_NOPARM,
//   JuliaTrigOrTrigfpFractal, othermandelfp_per_pixel, MandelTrigOrTrigSetup, StandardFractal,LTRIGBAILOUT,
   -3.8,  -2.0,  4.0, 0, MANDELFP, 2, 3, 2, "sin", "sqr", TRIGINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_bifmay+1,filt,seed,"Beta >= 2",ES,ES,ES,ES, ES, ES, ES, 300.0,0.9,5,0,0,0,0,0,0,0,
//   -3.5, -0.9, -0.5, 3.2, 16, NOFRACTAL, NOFRACTAL, BIFMAY,	 NOSYM,
//   LongBifurcMay, NULL, BifurcMaySetup, Bifurcation, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_bifmay,filt,seed,"Beta >= 2",ES,ES,ES,ES, ES, ES, ES, 300.0,0.9,5,0,0,0,0,0,0,0,
//   -3.5, -0.9, -0.5, 3.2, 0, NOFRACTAL, NOFRACTAL, LBIFMAY,	 NOSYM,
//   BifurcMay, NULL, BifurcMaySetup, Bifurcation, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_halley+1, "+Order (integer > 1)", "Relaxation coefficient", "Epsilon", ES,ES,ES, ES, ES, ES, ES, 6, 1.0, 0.0001, 0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.3, 1.3, 0, NOFRACTAL, NOFRACTAL, HALLEY, XYAXIS,
//   MPCHalleyFractal, MPCHalley_per_pixel, HalleySetup, StandardFractal, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_halley, "+Order (integer > 1)", "Relaxation coefficient", "Epsilon", ES,ES,ES, ES, ES, ES, ES, 6, 1.0, 0.0001, 0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.3, 1.3, 0, NOFRACTAL, NOFRACTAL, MPHALLEY, XYAXIS,
//   HalleyFractal, Halley_per_pixel, HalleySetup, StandardFractal, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   "dynamic","+# of intervals (<0 = connect)","time step (<0 = Euler",A,B,ES,ES, ES, ES, ES, ES, 50,.1,1,3,0,0,0,0,0,0,
//   -20.0,  20.0,  -20.0,   20.0, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//   dynamfloat, NULL, dynam2dfloatsetup, dynam2dfloat, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   dynam2dfloatsetup, dynam2dfloat, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
//#define QUATFP                 140
   "quat", "notused", "notused",CJ,CK,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, QUATJULFP, NOFRACTAL, NOFRACTAL,  XAXIS,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
//   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   "quatjul", "notused", "notused",CJ,CK,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 0, QUATJULFP, NOFRACTAL, NOFRACTAL,  XAXIS,
      -3.5,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 4, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "cellular", "Initial String | 0 = Random | -1 = Reuse Last Random",
   "Rule = # of digits (see below) | 0 = Random",
   "Type (see below)",
   "Starting Row Number",ES,ES,ES, ES, ES, ES,
   11.0, 3311100320.0, 41.0, 0,0,0,0,0,0,0,
//   -1.0,  1.0, -1.0,  1.0, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,  NOSYM,
//   NULL,	   NULL,   CellularSetup,	 cellular,	  NOBAILOUT,
   -4.0,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, OTHERFNINPIXEL, NOSYM, 4.0,
   NULL, NULL, "CellularDlg", (DLGPROC)CellularDlg, NOBAILOUT
   },

   {
   t_julibrot, ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 0, 0, 0, 0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 0, NOFRACTAL, NOFRACTAL, JULIBROT, NOSYM,
//   JuliafpFractal, jbfp_per_pixel, JulibrotSetup, Std4dfpFractal, STDBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

#ifdef RANDOM_RUN
   {
   t_julia_inverse+1, realparm, imagparm,
   "Max Hits per Pixel", "Random Run Interval", -0.11, 0.6557, 4, 1024,
//   -2.0,  2.0, -1.5, 1.5, 24, NOFRACTAL, MANDEL, INVERSEJULIAFP,  NOSYM,
//   Linverse_julia_orbit, NULL, orbit3dlongsetup, inverse_julia_per_image, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_julia_inverse, realparm, imagparm,
   "Max Hits per Pixel", "Random Run Interval", -0.11, 0.6557, 4, 1024,
//   -2.0,  2.0, -1.5, 1.5,  0, NOFRACTAL, MANDEL, INVERSEJULIA,  NOSYM,
//   Minverse_julia_orbit, NULL, orbit3dfloatsetup, inverse_julia_per_image, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

#else

   {
   t_julia_inverse+1, realparm, imagparm,
   "Max Hits per Pixel", ES,ES,ES, ES, ES, ES, ES, -0.11, 0.6557, 4, 1024,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5, 1.5, 24, NOFRACTAL, MANDEL, INVERSEJULIAFP,  NOSYM,
//   Linverse_julia_orbit, NULL, orbit3dlongsetup, inverse_julia_per_image, NOBAILOUT,
   -2.7, -2.0, 4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   t_julia_inverse, realparm, imagparm,
   "Max Hits per Pixel", ES,ES,ES, ES, ES, ES, ES, -0.11, 0.6557, 4, 1024,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5, 1.5,  0, NOFRACTAL, MANDEL, INVERSEJULIA,  NOSYM,
//   Minverse_julia_orbit, NULL, orbit3dfloatsetup, inverse_julia_per_image, NOBAILOUT,
   -2.7, -2.0, 4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },
#endif

   {
   "mandelcloud","+# of intervals (<0 = connect)",ES,ES,ES,ES,ES,ES, ES, ES, ES,
   50,0,0,0,0,0,0,0,0,0,
//   -2.5,  1.5,  -1.5,   1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//   mandelcloudfloat, NULL, dynam2dfloatsetup, dynam2dfloat, NOBAILOUT,
//   -6.63, -3.87, 7.2, 0, MANDELFP, NOFRACTAL, 1, 0, NULL, NULL, 0, NOSYM, 4.0,
   -3.6, -2.0, 4.0, 0, MANDELFP, NOFRACTAL, 1, 0, NULL, NULL, 0, NOSYM, 4.0,			// why has it changed?
   dynam2dfloatsetup, dynam2dfloat, "SelectFracParams", (DLGPROC)SelectFracParams, NOBAILOUT
   },

   {
   t_phoenix+1, p1real,p2real,"Degree of Z = 0 | >= 2 | <= -3",ES,ES,ES,ES, ES, ES, ES, 0.56667,-0.5,0,0,0,0,0,0,0,0,
   //   -2.0, 2.0, -1.5, 1.5, 0, NOFRACTAL, MANDPHOENIXFP, PHOENIX, XAXIS,
   //   PhoenixFractal, phoenix_per_pixel, PhoenixSetup, StandardFractal, STDBAILOUT,
      -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      //   PhoenixSetup, DoPhoexnixFractal, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
	 NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_phoenix,p1real,p2real,"Degree of Z = 0 | >= 2 | <= -3",ES,ES,ES,ES, ES, ES, ES, 0.56667,-0.5,0,0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 0, NOFRACTAL, MANDPHOENIXFP, PHOENIX, XAXIS,
//   PhoenixFractal, phoenix_per_pixel, PhoenixSetup, StandardFractal, STDBAILOUT,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
//   PhoenixSetup, DoPhoexnixFractal, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_mandphoenix+1, realz0,imagz0,"Degree of Z = 0 | >= 2 | <= -3",ES,ES,ES,ES, ES, ES, ES, 0.0,0.0,0,0,0,0,0,0,0,0,
   //   -2.5, 1.5, -1.5, 1.5, 0, PHOENIXFP, NOFRACTAL, MANDPHOENIX, NOSYM,
   //   PhoenixFractal, mandphoenix_per_pixel, MandPhoenixSetup, StandardFractal, STDBAILOUT,
      -3.5, -2.0, 4.0, 0, MANDELFP, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, NOSYM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define MANDPHOENIXFP          150
   t_mandphoenix,realz0,imagz0,"Degree of Z = 0 | >= 2 | <= -3",ES,ES,ES,ES, ES, ES, ES, 0.0,0.0,0,0,0,0,0,0,0,0,
//   -2.5, 1.5, -1.5, 1.5, 0, PHOENIXFP, NOFRACTAL, MANDPHOENIX, NOSYM,
//   PhoenixFractal, mandphoenix_per_pixel, MandPhoenixSetup, StandardFractal, STDBAILOUT,
   -3.5, -2.0, 4.0, 0, MANDELFP, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "hypercomplex", "notused", "notused",CJ,CK,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, HYPERCMPLXJFP, NOFRACTAL, NOFRACTAL,  XAXIS,
//   HyperComplexFPFractal, quaternionfp_per_pixel,MandelfpSetup,StandardFractal,
//   LTRIGBAILOUT,
   -2.5,  -1.5,  3.0, 0, NOFRACTAL, NOFRACTAL, 4, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "hypercomplexj", "notused", "notused",CJ,CK,ES,ES,ES, ES, ES, ES, 0,0,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 0, HYPERCMPLXJFP, NOFRACTAL, NOFRACTAL,  XAXIS,
   //   HyperComplexFPFractal, quaternionfp_per_pixel,MandelfpSetup,StandardFractal,
   //   LTRIGBAILOUT,
      -2.5,  -1.5,  3.0, 0, JULIAFP, NOFRACTAL, 4, 1, "sin", NULL, TRIGINPIXEL, XAXIS, 64.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_frothybasin+1, frothattractor, frothshade, ES, ES,ES,ES, ES, ES, ES, ES, 3, 0, 0, 0,0,0,0,0,0,0,
//   -3, 3, -2.5, 2, 28, NOFRACTAL, NOFRACTAL, FROTHFP, NOSYM,
//   NULL, NULL, froth_setup, calcfroth, FROTHBAILOUT,
   -4.0,  -2.5,  5.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_frothybasin, frothattractor, frothshade, ES, ES,ES,ES, ES, ES, ES, ES, 3, 0, 0, 0,0,0,0,0,0,0,
   //   -3, 3, -2.5, 2, 28, NOFRACTAL, NOFRACTAL, FROTHFP, NOSYM,
   //   NULL, NULL, froth_setup, calcfroth, FROTHBAILOUT,
      -4.0,  -2.5,  5.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, NOSYM, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_mandel4, realparm, imagparm,ES,ES,ES,ES,ES, ES, ES, ES, 0.6,0.55,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, NOFRACTAL, MANDEL4, JULIA4FP, ORIGIN,
   //   Mandel4Fractal,   julia_per_pixel, JulialongSetup,StandardFractal,	 STDBAILOUT,
      -2.7,  -2.0,  4.0, 0, 2, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
	 NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_julia4, realparm, imagparm,ES,ES,ES,ES,ES, ES, ES, ES, 0.6,0.55,0,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 1, NOFRACTAL, MANDEL4, JULIA4FP, ORIGIN,
   //   Mandel4Fractal,   julia_per_pixel, JulialongSetup,StandardFractal,	 STDBAILOUT,
      -2.7,  -2.0,  4.0, 0, JULIAFP, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
	 NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_marksmandel, realz0, imagz0, exponent,ES,ES,ES,ES, ES, ES, ES, 0,0,1,0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, MARKSJULIAFP, NOFRACTAL, MARKSMANDEL,  NOSYM,
//   MarksLambdafpFractal,marksmandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
   -3.5,  -2.0,  4.0, 0, 2, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_marksjulia, realz0, imagz0, exponent,ES,ES,ES,ES, ES, ES, ES, 0,0,1,0,0,0,0,0,0,0,
   //   -2.0,  2.0, -1.5,  1.5, 0, MARKSJULIAFP, NOFRACTAL, MARKSMANDEL,  NOSYM,
   //   MarksLambdafpFractal,marksmandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
      -3.5,  -2.0,  4.0, 0, 2, NOFRACTAL, 3, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   // dmf
   "icons", lambda, alpha,beta,gamma2,ES,ES, ES, ES, ES, ES, -2.34, 2.0, 0.2, 0.1,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL,  NOFRACTAL, NOSYM,
//   iconfloatorbit, NULL, orbit3dfloatsetup,  orbit2dfloat, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
//#define ICON3D                 160
   // dmf 
   "icons3d", lambda, alpha,beta,gamma2,ES,ES, ES, ES, ES, ES, -2.34, 2.0, 0.2, 0.1,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL,  NOFRACTAL, NOSYM,
//   iconfloatorbit, NULL, orbit3dfloatsetup,  orbit3dfloat, NOBAILOUT,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
    t_phoenixcplx + 1,p1real, p1imag, p2real, p2imag,ES,ES,ES, ES, ES, ES, 0.2, 0, 0.3, 0,0,0,0,0,0,0,
    //      (float)-2.0, (float)2.0, (float)-1.5, (float)1.5,
    //      1, NOFRACTAL, MANDPHOENIXCPLX,PHOENIXFPCPLX, ORIGIN,
    //      LongPhoenixFractalcplx, long_phoenix_per_pixel, PhoenixCplxSetup,
    //                                                           StandardFractal,
    //      STDBAILOUT
       -2.7, -2.0, 4.0, 0, JULIAFP, PHOENIXCPLX, 4, 0, NULL, NULL, FRACTINTINPIXEL, ORIGIN, 4.0,
       NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_phoenixcplx,
      p1real, p1imag, p2real, p2imag,ES,ES,ES, ES, ES, ES,
      0.2, 0, 0.3, 0,0,0,0,0,0,0,
      //      (float)-2.0, (float)2.0, (float)-1.5, (float)1.5,
      //      0, NOFRACTAL, MANDPHOENIXFPCPLX, PHOENIXCPLX, ORIGIN,
      //      PhoenixFractalcplx, phoenix_per_pixel, PhoenixCplxSetup,
      //                                                   StandardFractal,
      //      STDBAILOUT
	 -2.7, -2.0, 4.0, 0, JULIAFP, PHOENIXCPLX, 4, 0, NULL, NULL, FRACTINTINPIXEL, ORIGIN, 4.0,
	 NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_mandphoenixcplx+1, realz0, imagz0, p2real, p2imag,"Degree",ES,ES, ES, ES, ES, 0, 0, 0.5, 0,0,0,0,0,0,0,
      -3.8, -2.0, 4.0, 0, MANDELFP, NOFRACTAL, 5, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
      NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   t_mandphoenixcplx,
      realz0, imagz0, p2real, p2imag,"Degree",ES,ES, ES, ES, ES, 0, 0, 0.5, 0,0,0,0,0,0,0,
//      (float)-2.5, (float)1.5, (float)-1.5, (float)1.5,
//      0, PHOENIXFPCPLX, NOFRACTAL, MANDPHOENIXCPLX, XAXIS,
//      PhoenixFractalcplx, mandphoenix_per_pixel, MandPhoenixCplxSetup,
//                                                          StandardFractal,
//      STDBAILOUT
   -3.8, -2.0, 4.0, 0, MANDELFP, NOFRACTAL, 5, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "ant",
	"Rule String (1's and non-1's, 0 rand)", "Max Points", "Number of Ants (max 256)", "Wrap?","Random Seed",ES,ES, ES, ES, ES, 1100, 1.0E6, 2, 1, 1, 0,0,0,0,0,
//      (float)-1.0, (float)1.0, (float)-1.0, (float)1.0,
//      0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//      NULL, NULL, StandaloneSetup, ant,
//      NOBAILOUT
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, OTHERFNINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
   "chip",
      A, B, C, ES,ES,ES,ES, ES, ES, ES,
      -15,-19,1,0,0,0,0,0,0,0,
//      (float)-760.0, (float)760.0, (float)-570.0, (float)570.0,
//      0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//      (VF)chip2dfloatorbit, NULL, orbit3dfloatsetup, orbit2dfloat,
//      NOBAILOUT
   -2.7, -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   "quadruptwo",
      A, B, C, ES,ES,ES,ES, ES, ES, ES,
      34, 1, 5, 0,0,0,0,0,0,0,
//      (float)-82.93367, (float)112.2749, (float)-55.76383, (float)90.64257,
//      0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//      (VF)quadruptwo2dfloatorbit, NULL, orbit3dfloatsetup, orbit2dfloat,
//      NOBAILOUT
   -2.7, -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   "threeply",
      A, B, C, ES,ES,ES,ES, ES, ES, ES,
      -55, -1, -42, 0,0,0,0,0,0,0,
//      (float)-8000.0, (float)8000.0, (float)-6000.0, (float)6000.0,
//      0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//      (VF)threeply2dfloatorbit, NULL, orbit3dfloatsetup, orbit2dfloat,
//      NOBAILOUT
   -2.7, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   orbit3dfloatsetup, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
   },

   {
   "volterra-lotka",
      H, P, ES, ES,ES,ES, ES, ES, ES, ES, 0.739, 0.739, 0, 0, 0, 0,0,0,0,0,
//      (float)0.0, (float)6.0, (float)0.0, (float)4.5, 
//      0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//      VLfpFractal, otherjuliafp_per_pixel, VLSetup, StandardFractal, 
//      256
   -1.2, -1.2, 4.5, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 256.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define ESCHER                 170
   {
   "escher_julia",
      realparm, imagparm, ES, ES, ES, ES, ES, ES, ES, ES, 0.32, 0.043, 0, 0, 0, 0,0,0,0,0,
//      (float)-1.6, (float)1.6, (float)-1.2, (float)1.2,
//      0, NOFRACTAL, NOFRACTAL, NOFRACTAL, ORIGIN,
//      EscherfpFractal, juliafp_per_pixel, StandardSetup, 
//          StandardFractal,
//      STDBAILOUT
   -3.0, -1.6, 3.2, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FRACTINTINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },                     
  
// From Pickovers' "Chaos in Wonderland"      
// included by Humberto R. Baptista           
// code adapted from king.cpp bt James Rankin 

   {
   "latoocarfian",
      A, B, C, D, "1st Trig: 0=sin,1=cos,2=exp,3=sinh,4=cosh, 5=tan", "2nd Trig: 0=sin,1=cos,2=exp,3=sinh,4=cosh,5=tan", 
	    "3rd Trig: 0=sin,1=cos,2=exp,3=sinh,4=cosh,5=tan", "4th Trig: 0=sin,1=cos,2=exp,3=sinh,4=cosh,5=tan", 
	    ES, ES, -0.966918, 2.879879, 0.765145, 0.744728, 0, 0,0,0,0,0,
//      (float)-2.0, (float)2.0, (float)-1.5, (float)1.5,
//      0, NOFRACTAL, NOFRACTAL, NOFRACTAL, NOSYM,
//      (VF)latoofloatorbit, NULL, orbit3dfloatsetup, orbit2dfloat,
//      NOBAILOUT
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 8, 0, NULL, NULL, 0, XAXIS, 4.0,
//   orbit3dfloatsetup, do_mandel_df, "ParamDlg", (DLGPROC)ParamDlg, STDBAILOUT
    orbit3dfloatsetup, orbit2dfloat, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
    },


// new ones added for ManpWIN.....

//#define FOURIER			172
   {
   "Fourier",
      ES, ES, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0,0.0, 0.0,0.0, 0, 0,0,0,0,0,
   -2.7, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   NullSetup, Fourier, "FourierTypeDlg", (DLGPROC)FourierTypeDlg, STDBAILOUT
   },

//#define CUBIC				173
   {
   "Cubic",
       "Subtype: 0=CBIN, 1=CCIN, 2=CFIN, 3=CKIN", "Special Colour", realz0, imagz0, ES, ES,ES, ES, ES, ES,
      0.0, 2.0, 0.0, 0.0, 0, 0,0,0,0,0,
   -5.3333, -3.0, 6.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define NEWTONPOLYGON			174
   {
   "Newton Polygon",
      {newtpoly, realz0, imagz0, ES,ES,ES, ES, ES, ES, ES},
      {5.0, 0.0,0.0,0.0, 0,0,0,0,0,0},
   -3.5, -2.0, 4.0, CONVERGING, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
//   init_NewtonPolygon, do_NewtonPolygon, "NewtonGeneral", NewtonGeneral, STDBAILOUT
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define HENON				175
   {
   "Hénon",
      "Shapes", "Points per orbit", "x Start", "y Start", "Step size", "Number of orbits",ES, ES, ES, ES,
      1.1111, 5000.0, 0.09800, 0.06100, 0.00500, 1000.0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 6, 0, NULL, NULL, 0, XAXIS, 4.0,
   NullSetup, DoHenon, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define DEMOWALK			176
   {
   "Walk",
      "Step Size (Percentage of Screen)", "Number of Steps", ES, ES,ES, ES,ES, ES, ES, ES,
      1.0, 250.0,0.0,0.0, 0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, OTHERFNINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define FIBONACCI			177
   {
   "Fibonacci",
      ES, ES, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   NullSetup, Fibonacci, "FibTypeDlg", (DLGPROC)FibTypeDlg, STDBAILOUT
   },

//#define SPECIALNEWT			178
   {
   "Special Newton",
      "Phase colour separation", realz0, imagz0, ES, ES, ES,ES, ES, ES, ES,
      75.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5555556, -2.0, 4.0, CONVERGING1, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define MATEIN			179
   {
   "Matein",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5555556, -2.0, 4.0, CONVERGING1, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 10.0E20,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define RATIONALMAP			180
   {
   "Rational Map",
      "Subtype: 0 = type A, 1 = type B", "special colour", realz0, imagz0, ES, ES,ES, ES, ES, ES,
      0.0, 24.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5555556, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define POWER				181
   {
   "Power",
      newtdegree, realz0, imagz0, ES,ES,ES,ES, ES, ES, ES,
      4.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, -1, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   //init_power_df, do_power_df, "PowerTypeDlg", PowerTypeDlg, STDBAILOUT
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define CIRCLESQ			182
   {
   "Circle Squared",
      ES, ES, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -1600.0, -600.0, 1200.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, NULL, NULL, STDBAILOUT
   },

//#define SINFRACTAL			183
   {
   "Sine Function",
      realz0, imagz0, "0=multiply, else=plus", "Start value for z", ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, HALF_PI, 0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 80.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define EXPFRACTAL			184
   {
   "Exponential Function",
       "Subtype: 0 = real, 1 = imaginary, 2 = magnitude", realz0, imagz0, "degree (1 - 4)", "0=multiply, else=plus", ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 1.0, 0.0, 0.0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 10.0,
   NullSetup, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
   "Newton Apple",
      newtdegree, realz0, imagz0, ES,ES,ES,ES, ES, ES, ES,
      4.0, 0.0, 0.0, 0.0, 0.0, 0.0,0,0,0,0,
//   "Newton Apple","(testpt Param #1)","(testpt param #2)","(testpt param #3)","(testpt param #4)", 0.0, 0.0, 0.0, 0.0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	 NOSYM,
//   NULL,	  NULL, 	    StandaloneSetup, test,    STDBAILOUT,
   -1.5,  -0.75,  1.5, CONVERGING, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
//   init_NewtonApple, do_NewtonApple, NULL, NULL, STDBAILOUT
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
//#define NEWTONFLOWER         186
   {
   "Newton Flower",
      newtdegree, realz0, imagz0, ES,ES,ES,ES, ES, ES, ES,
      5.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
//   "5th Order Newton","(testpt Param #1)","(testpt param #2)","(testpt param #3)","(testpt param #4)", 0.0, 0.0, 0.0, 0.0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	 NOSYM,
//   NULL,	  NULL, 	    StandaloneSetup, test,    STDBAILOUT,
   -3.5, -2.0, 4.0, CONVERGING, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
//   init_NewtonFlower, do_NewtonFlower, "NewtonGeneral", NewtonGeneral, STDBAILOUT
   },

//#define MAGNET1                187
   {
   "Magnet 1",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, CONVERGINGMAG, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define MAGNET2                188
   {
   "Magnet 2",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, CONVERGINGMAG, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define TEDDY                  189
   {
   "Teddy (try using the filters)",
      ES, ES, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, NULL, NULL, STDBAILOUT
   },

//#define NEWTONMSET             190
   {
   "Newton M Set",
      newtdegree, realz0, imagz0, ES,ES,ES,ES, ES, ES, ES,
      4.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, CONVERGING, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define NEWTONJULIANOVA        191
   {
   "Newton Julia Nova",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
//#define TALIS                  192
   {
   "Talis Power",
      "Degree", "m", realz0, imagz0, ES, ES,ES, ES, ES, ES, 3.0, 1.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 400.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
//#define NEWTONCROSS            193

   {
   "Newton Cross",
      newtdegree, realz0, imagz0, ES,ES,ES,ES, ES, ES, ES,
      6.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define QUARTET1               194
   {
   "Quartet 1",
      newtdegree, realz0, imagz0, ES,ES,ES,ES, ES, ES, ES,
      4.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define QUARTET2               195
   {
   "Quartet 2",
      newtdegree, realz0, imagz0, ES,ES,ES,ES, ES, ES, ES,
      4.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define QUARTET3               196
   {
   "Quartet 3",
      newtdegree, realz0, imagz0, ES,ES,ES,ES, ES, ES, ES,
      4.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define RAMONSIN               197
   {
   "z=(c/sin(z))^2",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XYAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define RAMONCOS               198
   {
   "z=(c/cos(z))^2",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XYAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define FORMULA05              199
   {
   "Formula 05",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -1.0,  -0.25,  0.5, CONVERGING, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define SHARON                 200

   {
   "Sharon",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
//#define QUAD                   201

   {
   "Quad",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -10.0,  -6.0,  12.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XYAXIS, 8.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

//#define MOUNTAIN               202
   {
   "Mountain",
      ES, ES, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -2.7, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, 0, XAXIS, 4.0,
   NullSetup, mountain, "MountainDlg", (DLGPROC)MountainDlg, STDBAILOUT
   },

//#define FFT	               203
   {
   "Fast Fourier Transform",
      ES, ES, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0,0.0, 0.0,0.0,0,0,0,0,0,0,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, 0, XAXIS, 4.0,
   NullSetup, NullSetup, "FastFourierDlg", (DLGPROC)FastFourierDlg, STDBAILOUT
   },

//#define TIERAZON             204	// general Tierazon fractal - use subtype to specify which one.

   {
   "Tierazon Fractal",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -2.7, -2.0, 4.0, CONVERGING, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define QUADMAND           205
   "QuadMand",filt,seed,ES,ES,ES,ES,ES, ES, ES, ES, 1000.0,0.0,0,0,0,0,0,0,0,0,
//   1.9,  3.0, 0,  1.34, 1, NOFRACTAL, NOFRACTAL, BIFURCATION, NOSYM,
//   LongBifurcVerhulstTrig, NULL, StandaloneSetup, Bifurcation, NOBAILOUT,
   -2.7, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, bifurcation, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define SCREENFORMULA	       206	// interpret fractal directly on screen
   "On Screen Formula", p1real, p1imag, p2real, p2imag, "Keep Init for Julia: 0 = no, 1 = yes",ES,ES, ES, ES, ES, 0,0,0.0,0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -2.7, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, TRIGINPIXEL, NOSYM, 4.0,
   form_per_pixel, Formula, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define NEWTONVARIATION	       207	
   "Newton Variation", newtdegree, realz0, imagz0, ES,ES,ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -3.5, -2.0, 4.0, CONVERGING, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define NOVA	       208	
   "Newton Nova", newtdegree, realz0, imagz0, ES,ES,ES, ES, ES, ES, ES, 3.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -3.5, -2.0, 4.0, CONVERGING, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

   {
//#define MALTHUS		       209	
   "Malthus", "Rate", "Outer Loop Counter", "Inner Loop Counter", "Colours",ES,ES, ES, ES, ES, ES, 2.3, 38.0, 48.0, 10.0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -3.5, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoMalthus, "MalthusDlg",(DLGPROC)MalthusDlg, STDBAILOUT
   },
   {
//#define TRIANGLES	       210	
   "Triangles", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -7.0, -4.0, 8.0, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoTriangle, "TriangleDlg", (DLGPROC)TriangleDlg, STDBAILOUT
   },
   {
//#define GEOMETRY	       211	
   "Geometric Shapes", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -0.75, -0.3, 1.5, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoGeometry, "GeometryDlg", (DLGPROC)GeometryDlg, STDBAILOUT
   },
   {
//#define CIRCLES	       212	
   "Circles", ES, ES, ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
//   -0.4,  -0.3,  2.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   -0.25, -0.2, 2.5, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoCircle, "CircleDlg", (DLGPROC)CircleDlg, STDBAILOUT
   },
   {
//#define IKEDA	       213	
   "Ikeda Attractor", "a", "b", "c", "d", ES, ES, ES, ES, ES, ES, 1.0, 0.9, 0.4, 6.0,0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -2.6, -2.7, 5.0, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoIkeda2D, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define CROSSROADS	       214	
//   "Cross Road", "a", "b", "c", "exponent", "0 for type = 'A' and 'B' otherwise", ES, 2.23, 0.93, 0.31, 1.0, 0, 0,
   "Cross Road", "a", "b", "c", "exponent", "Subtypes 0 to 10", "Iterations", ES, ES, ES, ES, 2.23, 0.76, 0.3, 1.0, 0, 1000000.0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -10.0, -5.0, 12.0, 0, NOFRACTAL, NOFRACTAL, 6, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoCrossRoads, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define PASCALTRIANGLE	       215	
   "Pascal Triangle", "Order - (max=100)", "Modulo for the Pascal Triangle", "Low Colour", "High Colour", "Circle Size", ES, ES, ES, ES, ES, 100.0, 11.0, 4.0, 160.0, 8.0, 0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -6.0, -5.0, 12.0, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoPascal, "NumTriangleDlg", (DLGPROC)NumTriangleDlg, STDBAILOUT
   },
   {
//#define APOLLONIUS	       216
   "Apollonian Circles", 
      ES, ES, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0,0.0, 0.0,0.0,0,0,0,0,0,0,
//       "Offset1 (0 to .6)", "Offset2 (0 to 1)", "Fill = non zero", ES, ES, ES, 0.5, 0.4, 1.0, 0.0, 0.0, 0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -1.0, -0.25, 2.5, 0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoApollonius, "ApolloniusDlg", (DLGPROC)ApolloniusDlg, STDBAILOUT
   },
   {
//#define APOLLONIUSIFS	       217
   "Apollonian Circles (IFS)", "Iterations", "Colour", ES, ES, ES, ES, ES, ES, ES, ES, 10000000.0, 50.0, 0.0, 0.0, 0.0, 0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -9.0, -5.0, 10.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoApolloniusIFS, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define SIERPINSKIFLOWERS      218

//	"Sierpinski Flowers Animation", "Sides", "0 = True Colour, 1 = Default Palette", ES, ES, ES, ES, 6.0, 1.0, 0.0, 0.0, 0.0, 0.0,
//	    -2.0,  -1.5,  3.0, 0, MANDEL, NOFRACTAL, 2, 0, NULL, NULL, 0, NOSYM, 4.0,



   "Sierpinski Flowers", "Passes", "Sides", "0 = Current Palette, 1 = default 256 Palette", ES, ES, ES, ES, ES, ES, ES, 8.0, 6.0, 0.0, 1.0, 0,0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -10.0, -6.0, 12.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoSierpinskiFlower, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define ZIGZAG      219
   "Zigzag Spirals", "a", "k1", "exp 1", "exp 2", "subtype 0, 1, 2, 3", "Number of segments", ES, ES, ES, ES, 20.0, 1.0, 2.0, 2.0, 0, 100.0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -0.36, -0.2, 0.4, 0, NOFRACTAL, NOFRACTAL, 6, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoZigzag, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define GARGOYLE	       220
   "Numeric Gargoyles", "Rule (10 digits of 0 or 1). Most combinations are unstable", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1111010000.0, 0.0, 0.0, 0.0, 0.0, 0.0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -0.3, -0.2, 0.4, 0, NOFRACTAL, NOFRACTAL, 1, 0, NULL, NULL, OTHERFNINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define CURLICUES	       221
   "Curlicues", "Passes", "d: try 1 to 1.5", "skew: try -1 to +1", ES, ES, ES, ES, ES, ES, ES, 1800000, 1.3, 0.0, 0.0, 0.0, 0.0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -1200, -1000, 1800, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, OTHERFNINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define CHUA		       222
   "Mec-Spider's Web", "Iterations", "a Note: if 0 (default), then a, b and c are randomsed", "b", "c", ES, ES, ES, ES, ES, ES, 500, 0.0, 0.1, 0.7, 0.0, 0.0,0,0,0,0,
//   -2.0, 2.0, -1.5, 1.5, 1, NOFRACTAL, NOFRACTAL, FFORMULA, SETUP_SYM,
//   Formula, form_per_pixel, intFormulaSetup, StandardFractal, 0,
   -300, -200,  400, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, OTHERFNINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define BURNINGSHIP	       223
   "Burning Ship", realz0, imagz0,ES,ES,ES,ES,ES, ES, ES, ES, 0.0,0.0,0.0,0.0,0.0,0.0,0,0,0,0,
   -3.5, -2.0,  4.0, 0, MANDEL, 2, 2, 0, NULL, NULL, FUNCTIONINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define OSCILLATORS	       224
   "Oscillators", "iterations", "a", "b",ES,ES,ES,ES, ES, ES, ES, 100000.0,0.5,0.0,0.0,0.0,0.0,0,0,0,0,
   -10.0, -8.0,  16.0, 0, MANDEL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoOscillator, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define FRACTALMAPS	       225
   "Fractal Maps", "iterations", "a", "b",ES,ES,ES,ES, ES, ES, ES, 100000.0,0.5,0.0,0.0,0.0,0.0,0,0,0,0,
   -10.0, -8.0,  16.0, 0, MANDEL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoFractalMaps, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define BURNINGSHIPPOWER       226
   "Burning Ship of Higher Degree",
       "degree",realz0, imagz0,ES, ES,ES,ES, ES, ES, ES,
      {3.0, 0.0, 0.0, 0.0, 0,0,0,0,0,0,},
   -3.5, -2.0,  4.0, 0, NOFRACTAL, -1, 3, 0, NULL, NULL, FUNCTIONINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define THORN			227
   "Thorn Fractal", realz0, imagz0,ES, ES, ES, ES,ES, ES, ES, ES,
      {0.0, 0.0, 0.0, 0.0, 0,0,},
   -3.5, -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, NOSYM, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define MANDELDERIVATIVES	228	// general Mandelbrot Derivatives fractal - use subtype to specify which one.
   "Mandelbrot Derivatives",
      realz0, imagz0, ES, ES, ES, ES,ES, ES, ES, ES,
      0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
   -2.7,  -2.0,  4.0, ESCAPING1, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define BUDDHABROT	       229
   "BuddhaBrot", "Brightness","Iterations",ES,ES, ES,ES,ES, ES, ES, ES,
      {2.0, 150.0, 0.0,0.0, 0,0,0,0,0,0,},
   -2.8, -1.5,  3.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, DoBuddhaBrot, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define POPCORN		230
   "Popcorn",  "Threshold", "Variation (0 to 5)", "h", "a real", "a imag", ES, ES, ES, ES, ES, {50.0, 0.0, 0.05, 3.0, 0,0,0,0,0,0,},
   -2.6, -1.8, 3.6, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_mandel_df, DoPopcorn, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define MANDELBAR	       231
   "Mandelbar (Tricorn)", "degree", realz0, imagz0,"Rotated? (Only for degree > 2, 1=yes 0=no)",ES,ES,ES, ES, ES, ES, 2.0,0.0,0.0,0.0,0.0,0.0,0,0,0,0,
   -3.6, -1.8, 3.6, 0, NOFRACTAL, NOFRACTAL, 4, 0, NULL, NULL, 0, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define REDSHIFTRIDER	       232
   "RedShiftRider (a*z^2 +/- z^n + c)", "a real", "a imag", "n", realz0, imagz0,"Positive? (1=yes 0=no)",ES, ES, ES, ES, 1.0,0.0,3.0,0.0,0.0,1.0,0,0,0,0,
   -3.6, -1.8, 3.6, 0, NOFRACTAL, NOFRACTAL, 6, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define SURFACES	       233
   "Surfaces", "iterations", "a", "b",ES,ES,ES,ES, ES, ES, ES, 100000.0,0.5,0.0,0.0,0.0,0.0,0,0,0,0,
   -10.0, -8.0,  16.0, 0, MANDEL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
   SurfaceSetup, DoSurface, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define KNOTS		       234
   "Knots", "iterations", "a", "b",ES,ES,ES,ES, ES, ES, ES, 100000.0,0.5,0.0,0.0,0.0,0.0,0,0,0,0,
   -10.0, -8.0,  16.0, 0, MANDEL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoKnots, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define CURVES	       235
   "Curves", "iterations", "a", "b",ES,ES,ES,ES, ES, ES, ES, 100000.0,0.5,0.0,0.0,0.0,0.0,0,0,0,0,
   -10.0, -8.0,  16.0, 0, MANDEL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
   NullSetup, DoCurves, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
//#define TETRATION		236
    {
   "Tetration", realz0, imagz0, "Bailout Method: 0-5",ES, ES, ES, ES, ES,ES,ES, 0.0, 0.0, 0.0, 0.0,0,0,0,0,0,0,
//   -2.0,  2.0, -1.5,  1.5, 0, NOFRACTAL, NOFRACTAL, NOFRACTAL,	XAXIS,
//   TetratefpFractal,othermandelfp_per_pixel,MandelfpSetup,StandardFractal,STDBAILOUT,
   -4.0, -3.0, 6.0, 0, NOFRACTAL, NOFRACTAL, 3, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 10000.0,
   NullSetup, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
    },
//#define PERTURBATION		237
    {
   "Perturbation", ES,ES,ES,ES,ES,ES,ES, ES, ES, ES, 2.0,0.0,0.0,0.0,0.0,0.0,0,0,0,0,
   -3.5, -2.0, 4.0, 0, JULIAFP, NOFRACTAL, NUMPARAM, 0, NULL, NULL, 0, XAXIS, 4.00000000001,		// bailout level a smidgen over 4.0 so there's no truncation right at -2.0, 0.0
   InitPerturbation, DoPerturbation, "PertDlg", (DLGPROC)PertDlg, STDBAILOUT
    },
//#define KLEINIAN		238
    {
    "Kleinian Maskit Parameterisation",
       "Moebius Transformation Real","Moebius Transformation Imaginary","Exponential Function Scale","Exponent", ES, ES, ES, ES, ES, ES,
       1.93, 0.04, 0.26, 4.9, 0.0, 0.0,0,0,0,0,
    -0.5, -0.5, 3.0, 0, JULIAFP, NOFRACTAL, 4, 0, NULL, NULL, FUNCTIONINPIXEL, XAXIS, 1.0e-8,
    NullSetup, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
    },
    {
//#define SPROTTMAPS		239
	"Sprott Fractal Maps", "iterations", "a", "b",ES,ES,ES,ES, ES, ES, ES, 100000.0,0.5,0.0,0.0,0.0,0.0,0,0,0,0,
	-10.0, -8.0,  16.0, 0, MANDEL, NOFRACTAL, 3, 0, NULL, NULL, 0, NOSYM, 4.0,
	NullSetup, DoSprottMaps, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
    },
    {
//#define SLOPEDERIVATIVE	240
	"Slope Using Derivatives", "Mandelbrot = 0, else Power degree", "Use Palette? = 0","Light Source Angle (degrees)","Light Source Height",ES, ES, ES, ES, ES,ES,0.0,0.0,45.0,1.5,0.0,0.0,0,0,0,0,
	-3.5, -2.0, 4.0, 0, MANDEL, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 1000.0,
	InitSlopeFractal, DoSlopeFractal, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
    },
    {
//#define SLOPEFORWARDDIFF	241
     "Slope Using Forward Differencing",
	"(testpt Param #1)","(testpt param #2)","(testpt param #3)","(testpt param #4)", ES, ES,ES, ES, ES, ES,
	0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0,0,0,0,
	-4.0,  -2.0,  4.0, 0, MANDELFP, NOFRACTAL, 4, 0, NULL, NULL, 0, NOSYM, 1280.0,
    InitSlopeFractal, DoSlopeFractal, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
    },
    {
    //#define POLYNOMIAL	242
	 "Polynomial",realz0, imagz0, "8th Power Coefficient", "7th Power Coefficient", "6th Power Coefficient", "5th Power Coefficient", "Quartic Coefficient", "Cubic Coefficient", "Square Coefficient", "Z Coefficient",
	    0.0, 0.0, 1.0, 3.0, 2.0, 1.0,0,0,0,1.0,
	    -4.0,  -2.0,  4.0, 0, MANDELFP, NOFRACTAL, 10, 0, NULL, NULL, FUNCTIONINPIXEL, NOSYM, 1280.0,
	NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
    },
    {
    //#define TOWER		243
   "Ant Tower", "Speed 2D Window", "Speed 3D Window", "Background Colour 0 - 255","Ant Size 1-20","Height Scale",ES,ES, ES, ES, ES, 60.0, 60.0, 7, 4, 2.0, 0,0,0,0,0,
   -2.7,  -2.0,  4.0, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, OTHERFNINPIXEL, XAXIS, 4.0,
   NULL, NULL, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },

/*
   {
//#define MARCUSMANDEL		233	// general Marcus fractal - use subtype to specify which one.
   "Marcus Mandlebrot Variations", realz0, imagz0, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   -2.7, -2.0, 4.0, 0, NOFRACTAL, NOFRACTAL, 2, 0, NULL, NULL, 0, XAXIS, 4.0,
   init_Marcus, do_Marcus, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define MANDALA		234
   "Marcus Mandala", "Complexity", ES, ES, ES, ES, ES, 36.0, 0.0, 0.0, 0.0, 0.0, 0.0,
   -2.0, -1.2, 2.4, 0, NOFRACTAL, NOFRACTAL, 1, 0, NULL, NULL, 0, XAXIS, 4.0,
   NullSetup, DoMarcusMandala, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
   {
//#define MARCUSIFS		235
   "Marcus IFS", "Number of dots", "a", "b", "c", "d", ES, 3600000.0, 2.0, 3.0, 3.0,1.0, 0.0,
   -2.0, -1.2, 2.4, 0, NOFRACTAL, NOFRACTAL, 5, 0, NULL, NULL, 0, XAXIS, 4.0,
   NullSetup, DoMarcusWaterfall, "SelectFracParams", (DLGPROC)SelectFracParams, STDBAILOUT
   },
*/
   NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,0,0, 0, 0, 0, 0, 0, 0, 0, 0,				// marks the END of the list
   0,  0, 0,  0, NOFRACTAL, NOFRACTAL, 0, 0, NULL, NULL, 0, NOSYM, 0.0,
   NULL, NULL, 0

};

//////////////////////////////////////////////////////////////////
// database of Perturbation fractals
//////////////////////////////////////////////////////////////////

struct PerturbationSpecificStuff PerturbationSpecific[] =
    {
    //	    old
    //    char	    *name;				// name of the fractal 
    //    char	    *paramname[NUMPERTPARAM];		// name of the parameters 
    //    double    paramvalue[NUMPERTPARAM];		// default parameter values 
    //	  int	    numparams;				// Number of parameters 
    //    double    rqlim;				// bailout value 
    //    int	    RedDhiftRider;			// 1 if true

    // new
    //	    char	*name;				// name of the fractal 
    //	    int		SlopeType;			// none, FwdDiff, Derivative
    //	    int		PaletteStart;			// where does the colour start?
    //	    double	lightDirectionDegrees;		// Slope parameters
    //	    double	bumpMappingDepth;
    //	    double	bumpMappingStrength;
    //	    int		ColourOptions;			// future use?
    //	    char	*paramname[NUMPERTPARAM];	// name of the parameters 
    //	    double	paramvalue[NUMPERTPARAM];	// default parameter values 
    //	    int		numparams;			// Number of parameters 
    //	    double	rqlim;				// bailout value 
    //	    int		RedShiftRider;			// true if fractal = RedShiftRider




	    {
	    "Mandelbrot", 2, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor (Fwd Diff)", "Light Source Height (Derivative)", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2, 1000.0, 0
	    },
	    {
	    "Power", 2, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor (Fwd Diff)","Light Source Height (Derivative)", "Degree", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.5, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0, 0
	    },
	    {
	    "Burning Ship", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic Burning Ship", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Power Burning Ship", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Power Burning Ship", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Celtic", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic Celtic", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic Buffalo", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Celtic", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
//	    "Mandelbar (Tricorn)", "Slope? 0 = off, 1 = Fwd Diff, 2 = Derivative", "Start Palette 0-255", "Light Direction (Degrees)", "Mapping Depth (Fwd Diff)", "Mapping Strength (Fwd Diff)", "Slope Transfer Factor (Fwd Diff)", "Light Source Height (Derivative)", ES, ES, ES, 2.0, 0.0, 45.0, 50.0, 50.0, 1.0, 1.5, 0.0, 0.0, 0.0, 7, 1000.0, 0
	    "Mandelbar (Tricorn)", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Mandelbar Power", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor" ,"Polynomial Degree (2 - 20)", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2, 1000.0, 0
	    },
	    {
	    "Buffalo", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic Buffalo", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Power Buffalo", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Power Buffalo", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Mandelbar Celtic", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Perpendicular Mandelbrot", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Perpendicular Burning Ship", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Perpendicular Celtic", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Perpendicular Buffalo", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic Quasi Burning Ship", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic Partial Burning Ship Real", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic Partial Burning Ship Imag", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic Flying Squirrel", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic Quasi Perpendicular", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Burning Ship Partial Imag", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Burning Ship Partial Real", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Burning Ship Partial Real Mbar", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic Burning Ship Partial Imag", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic Burning Ship Partial Real", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic Burning Ship Partial Real Mbar", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Buffalo Partial Imag", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic Mbar", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th False Quasi Perpendicular", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th False Quasi Heart", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic False Quasi Perpendicular", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic False Quasi Heart", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Imag Quasi Perpendicular / Heart", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Real Quasi Perpendicular", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Real Quasi Heart", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic Imag Quasi Perpendicular / Heart", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic Real Quasi Perpendicular", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "4th Celtic Real Quasi Heart", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Burning Ship Partial", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Burning Ship Partial Mbar", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Celtic Mbar", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Quasi Burning Ship (BS/Buffalo Hybrid)", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Quasi Perpendicular", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "5th Quasi Heart", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "SimonBrot", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Cubic SimonBrot", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "SimonBrot2 4th", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
//	    "TheRedshiftRider: (a*z^2 +/- z^n + c)", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor 0 = off", "Start Palette 0-255", "Light Direction (Degrees)", "Mapping Depth", "Mapping Strength", "a real", "a imag", "n", "Positive? (1=yes 0=no)", ES, 1.0, 0.0, 45.0, 50.0, 50.0, 1.0, 0.0, 3.0, 1.0, 0.0, 9, 1000.0, 1
	    "TheRedshiftRider: (a*z^2 +/- z^n + c)", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", "a real", "a imag", "n", "Positive? (1=yes 0=no)", ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0, 1
	    },
	    {
	    "Talis", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Talis Cubic", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    "Talis Quartic", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
	    {
	    //	    "Mandelbrot", "Slope? 0 = off, 1 = Fwd Diff, 2 = Derivative", "Start Palette 0-255", "Light Direction (Degrees)", "Mapping Depth (Fwd Diff)", "Mapping Strength (Fwd Diff)", "Slope Transfer Factor (Fwd Diff)", "Light Source Height (Derivative)", ES, ES, ES, 2.0, 0.0, 45.0, 50.0, 50.0, 1.0, 1.5, 0.0, 0.0, 0.0, 7, 1000.0, 0

	    "Polynomial", 2, 0, 45.0, 50.0, 50.0, 0, "Tenth Order Coefficient", "Nineth Order Coefficient", "Eighth Order Coefficient", "Seventh Order Coefficient", "Sixth Order Coefficient", "Quintic Coefficient", "Quartic Coefficient", "Cubic Coefficient", "Square Coefficient", "Z Coefficient",
								1.0, 0.0, 4.0, 0.0, 7.0, 0.0, 3.0, 2.0, 2.0, 1.0, 10, 1000.0, 0
	    },
	    {
	    "HPDZ Buffalo", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
	    },
//	    {
//	    "Art Matric Cubic", 2, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor (Fwd Diff)","Light Source Height (Derivative)", "Degree", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2, 1000.0, 0
//	    },
//	    {
//	    "Sin", 0, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
//	    },
//	    {
//	    "Exp", 0, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 1000.0, 0
//	    },
/*
	    {
	    "Cubic Newton", 1, 0, 45.0, 50.0, 50.0, 0, "Slope Transfer Factor 0 = off", "Start Palette 0-255", "Light Direction (Degrees)", "Mapping Depth", "Mapping Strength", ES, "SlopeDegree", ES, ES, ES, 1.0, 0.0, 45.0, 50.0, 50.0, 0.0, 3.0, 0.0, 0.0, 0.0, 6, 16.0, 0
	    },
	    {
	    "Exponential", "Slope Transfer Factor 0 = off", "Start Palette 0-255", "Light Direction (Degrees)", "Mapping Depth", "Mapping Strength", ES, "SlopeDegree", ES, ES, ES, 1.0, 0.0, 45.0, 50.0, 50.0, 0.0, 4.0, 0.0, 0.0, 0.0, 7, 1000.0, 0
	    },
	    {
	    "Sin", "Slope Transfer Factor 0 = off", "Start Palette 0-255", "Light Direction (Degrees)", "Mapping Depth", "Mapping Strength", ES, ES, ES, ES, ES, 1.0, 0.0, 45.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0, 0
	    },
	    {
	    "Magnet1", "Slope Transfer Factor 0 = off", "Start Palette 0-255", "Light Direction (Degrees)", "Mapping Depth", "Mapping Strength", ES, ES, ES, ES, ES, 1.0, 0.0, 45.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0, 0
	    },
	    {
	    "HPDZ Buffalo", ES, ES, ES, ES, ES, ES, 0.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0, 100.0, 0
	    },
*/


	NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

//////////////////////////////////////////////////////////////////
// database of Mandelbrot Derivatives fractals
//////////////////////////////////////////////////////////////////

struct AlternativeSpecificStuff MandelDerivSpecific[] =
    {
    //    char	    *name;				// name of the fractal 
    //    int	    (*per_pixel)(void);			// once-per-pixel init 
    //    int	    (*calctype)(void);			// name of main fractal function 
    //    int	    (*big_per_pixel)(void);		// Bignum versions 
    //    int	    (*big_calctype)(void); 
    //    double    paramvalue[4];			// default parameter values 
    //    double    rqlim;				// bailout value 
    //    int	    symmetry;				// see above
    //	  int	    SlopeDegree;			// set to -1 if taken from param
    //    int	    BailoutType;			// bailout schemes for slope using fwd diff: ESCAPING, CONVERGING, MAGNET etc

	{
	"Perpendicular Mandelbrot",			// 0
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"Burning Ship",					// 1
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 2, ESCAPING
	},
	{
	"Burning Ship of Higher Degree",		// 2
	    NULL, NullSetup,
	    NULL, NullSetup, 3.0, 0.0, 0.0, 0.0, 4.0, NOSYM, -1, ESCAPING
	},
	{
	"Perpendicular Burning Ship",			// 3
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 2, ESCAPING
	},
	{
	"Buffalo",					// 4
	    NULL, NullSetup,
	    NULL, NullSetup, 2.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 2, ESCAPING
	},
	{
	"Perpendicular Buffalo",			// 5
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 2, ESCAPING
	},
	{
	"Mandelbar (Tricorn)",				// 6
	    NULL, NullSetup,
	    NULL, NullSetup, 2.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, ESCAPING
	},
	{
	"Celtic",					// 7
	    NULL, NullSetup,
	    NULL, NullSetup, 2.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, ESCAPING
	},
	{
	"Mandelbar Celtic",				// 8
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 3, ESCAPING
	},
	{
	"Perpendicular Celtic",				// 9
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"Cubic Flying Squirrel (Buffalo Imaginary)",	// 10
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, YAXIS, 3, ESCAPING
	},
	{
	"Heart Mandelbrot",				// 11
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"Celtic Heart",					// 12
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"Partial Cubic Burning Ship Real",		// 13
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 3, ESCAPING
	},
	{
	"Partial Cubic Burning Ship Imaginary",		// 14
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, YAXIS, 3, ESCAPING
	},
	{
	"Partial Cubic Buffalo Real (Celtic)",		// 15
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 3, ESCAPING
	},
	{
	"Cubic Quasi Burning Ship (Buffalo Burning Ship Hybrid)",	// 16
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 3, ESCAPING
	},
	{
	"Cubic Quasi Perpendicular",			// 17
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 3, ESCAPING
	},
	{
	"Cubic Quasi Heart",				// 18
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 3, ESCAPING
	},
	{
	"Mandelbrot 4th Order",				// 19
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 4, ESCAPING
	},
	{
	"Mandelbar 4th Order",				// 20
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 4, ESCAPING
	},
	{
	"Burning Ship 4th Order",			// 21
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Burning Ship 4th Partial Imag",		// 22
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Burning Ship 4th Partial Real",		// 23
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Burning Ship 4th Partial Real Mbar",		// 24
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic Burning Ship 4th",			// 25
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic Burning Ship 4th Partial Imag",		// 26
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic Burning Ship 4th Partial Real",		// 27
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, -1, ESCAPING
	},
	{
	"Celtic Burning Ship 4th Partial Real Mbar",	// 28
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Buffalo 4th",					// 29
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Buffalo 4th Partial Imag",			// 30
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic (Buffalo 4th Partial Real)",		// 31
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic 4th Mbar",				// 32
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"False Quasi Perpendicular 4th",		// 33
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"False Quasi Heart 4th",			// 34
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic False Quasi Perpendicular 4th",		// 35
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic False Quasi Heart 4th",			// 36
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Imag Quasi Perpendicular / Heart 4th",		// 37
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Real Quasi Perpendicular 4th",			// 38
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Real Quasi Heart 4th",				// 39
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic Imag Quasi Perpendicular / Heart 4th",	// 40
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic Real Quasi Perpendicular 4th",		// 41
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Celtic Real Quasi Heart 4th",			// 42
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"Mandelbrot 5th",				// 43
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Mandelbar 5th (Vertical)",			// 44
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Mandelbar 5th (horizontal)",			// 45
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Burning Ship 5th",				// 46
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Buffalo 5th",					// 47
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Burning Ship 5th Partial",			// 48
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Burning Ship 5th Partial Mbar",		// 49
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Celtic 5th (Buffalo 5th Partial)",		// 50
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Celtic 5th Mbar",				// 51
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Quazi Burning Ship 5th (BS/Buffalo Hybrid)",	// 52
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Quazi Perpendicular 5th",			// 53
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"Quazi Heart 5th",				// 54
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 5, ESCAPING
	},
	{
	"SimonBrot",					// 55
	    NULL, NullSetup,
	    NULL, NullSetup, 6.0, 0.0, 0.0, 0.0, 4.0, NOSYM, -1, ESCAPING
	},
	{
	"SimonBrot2",					// 56
	    NULL, NullSetup,
	    NULL, NullSetup, 4.0, 0.0, 0.0, 0.0, 4.0, NOSYM, -1, ESCAPING
	},
	{
	"Kung Fu Panda",				// 57
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 2, ESCAPING
	},
	{
	"HPDZ Buffalo",					// 58
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 100.0, NOSYM, 2, ESCAPING
	},
	{
	"SzegediButterfly 1",				// 59
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 128.0, NOSYM, 2, ESCAPING
	},
	{
	"SzegediButterfly 2",				// 60
	    NULL, NullSetup,
	    NULL, NullSetup, 0.0, 0.0, 0.0, 0.0, 128.0, NOSYM, 2, ESCAPING
	},

	NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0
    };

    //////////////////////////////////////////////////////////////////
    // database of Slope fractals
    //////////////////////////////////////////////////////////////////

    struct SlopeSpecificStuff SlopeDerivSpecific[] =
	{
	//    char	*name;				// name of the fractal 
	//    double	paramvalue[NUMSLOPEDERIVPARAM];	// default parameter values 
	//    int	numparams;			// Number of parameters 
	//    double	rqlim;				// bailout value 

		{
		"Mandelbrot",							// 0
		    45.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0
		},
		{
		"Cubic",							// 1
		    45.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0
		},
		{
		"Power",							// 2
		    45.0, 1.5, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4, 1000.0
		},
		{
		"Sin",								// 3
		    45.0, 1.5, 0.0, 1.0, HALF_PI, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Sin + 1/c",							// 4
		    45.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0
		},
		{
		"Exponential",							// 5
		    45.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0
		},
		{
		"Power + 1/c",							// 6
		    45.0, 1.5, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4, 1000.0
		},
		{
		"Polynomial: Z^n + Z^p + Z^q + C",				// 7
		    45.0, 1.5, 0.0, 4.0, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		},
		{
		"Cactus: z = z*z*z+(c-1)*z+c",					// 8
		    45.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0
		},
		{
		"Sharon's Web z = z*z*z / 4 + z + c",				// 9
		    45.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0
		},
		{
		"Sin(z^n) + c",							// 10
		    45.0, 1.5, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4, 1000.0
		},
		{
		"Sinh",								// 11
		    45.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0
		},
		{
		"Sinh(z^n) + c",						// 12
		    45.0, 1.5, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4, 1000.0
		},
	//	{
	//	"Mandelbar (Tricorn)",						// 13
	//	    45.0, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3, 1000.0
	//	},
	//	{
	//	"3rd Order Nova Variation: z=z-(z*z*z-1)/(3*z*z)+c",	// 13
	//	    45.0, 1.5,  0.0,  4.0, 0.0, 0.0, 1000.0
	//	},

	    NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

    struct SlopeSpecificStuff SlopeFwdDiffSpecific[] =
	{
	//    char	*name;				// name of the fractal 
	//    double	paramvalue[NUMSLOPEDERIVPARAM];	// default parameter values 
	//    int	numparams;			// Number of parameters 
	//    double	rqlim;				// bailout value 

		{
		"Mandelbrot",							// 0
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Burning Ship",							// 1
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Cubic",							// 2
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Power",							// 3
		    1.0, 0.0, 45.0, 50.0, 70.0, 4.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		},
		{
		"Sin",								// 4
		    1.0, 0.0, 45.0, 50.0, 70.0, 1.0, HALF_PI, 0.0, 0.0, 0.0, 7, 1000.0
		},
		{
		"Sin + 1/c",							// 5
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Exponential",							// 6
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Power + 1/c",							// 7
//		    1.0, 50, 50.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		    1.0, 0.0, 45.0, 50.0, 70.0, 4.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		},
		{
		"Polynomial: Z^n + Z^p + Z^q + C",				// 8
		    1.0, 0.0, 45.0, 50.0, 70.0, 4.0, 3.0, 2.0, 0.0, 0.0, 8, 1000.0
		},
		{
		"Cactus: z = z*z*z+(c-1)*z+c",					// 9
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Sharon's Web z = z*z*z / 4 + z + c",				// 10
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Sin(z^n) + c",							// 11
//		    1.0, 50, 50.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		    1.0, 0.0, 45.0, 50.0, 70.0, 4.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		},
		{
		"Sinh",								// 12
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Sinh(z^n) + c",						// 13
//		    1.0, 0.0, 45.0, 50.0, 50.0, 4.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		    1.0, 50, 50.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		},
		{
		"Mandelbar (Tricorn)",						// 14
		    1.0, 0.0, 45.0, 50.0, 70.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5, 1000.0
		},
		{
		"Art Matrix Cubic",						// 15
		    1.0, 0.0, 45.0, 50.0, 70.0, 2.0, 0.0, 0.0, 0.0, 0.0, 7, 1000.0
		},
		{
		"Newton Variation: z=z-(z^n - 1)/(3*z-1)+c",			// 16
		    1.0, 0.0, 45.0, 50.0, 70.0, 4.0, 0.0, 0.0, 0.0, 0.0, 6, 1000.0
		},

	    NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0
	};



//////////////////////////////////////////////////////////////////
// database of Tierazon fractals
//////////////////////////////////////////////////////////////////

struct AlternativeSpecificStuff TierazonSpecific[]=
    {
//    char	    *name;				// name of the fractal 
//    int	    (*per_pixel)(void);			// once-per-pixel init 
//    int	    (*calctype)(void);			// name of main fractal function 
//    int	    (*big_per_pixel)(void);		// Bignum versions 
//    int	    (*big_calctype)(void); 
//    double	    paramvalue[4];			// default parameter values 
//    double	    rqlim;				// bailout value 
//    int	    symmetry;				// see above
//    int	    SlopeDegree;			// set to -1 if taken from param
//    int	    BailoutType;			// bailout schemes for slope using fwd diff: ESCAPING, CONVERGING, MAGNET etc

	{
	"00.) Test Pattern",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 2, ESCAPING
	},
	{
	"01.) Mandel, z=z*z+c", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"02.) Nova, init: z=1; iterate: z=z-((z*z*z-1)/(3*z*z))+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"03.) Ushiki's Phoenix, z=z*z-.5*z+c; z=z*z-.5*z2+c; z2=z; z=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, YAXIS, 2, ESCAPING
	},
	{
	"04.) Talis, z=((z*z)/(1+z))+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 400.0, XAXIS, -1, ESCAPING
	},
	{
	"05.) Newton variation, z=((z*z*z-z-1)/(3*z*z-1)-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"06.) z=z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"07.) Newton variation, z = z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"08.) Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-1)+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"09.) Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-z)+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"10.) z=z*z*z+c",
	    NULL, NULL,
	    NULL, NULL, 3.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, 3, ESCAPING
	},
	{
	"11.) z=z*z*z*z+c",
	    NULL, NULL,
	    NULL, NULL, 4.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 4, ESCAPING
	},
	{
	"12.) z=z*z*z*z*z+c",
	    NULL, NULL,
	    NULL, NULL, 5.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, 5, ESCAPING
	},
	{
	"13.) z=z*z*z*z*z*z+c",
	    NULL, NULL,
	    NULL, NULL, 6.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 6, ESCAPING
	},
	{
	"14.) z1=z*z+c; z=z*z+c*z2; z2=z",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"15.) Phoenix II,  z1=z; z=z*z + real(c) + imag(c)*z2; z2=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 2, ESCAPING
	},
	{
	"16.) Phoenix III, z1=z; z=z*z*z + real(c) + imag(c)*z2; z2=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 3, ESCAPING
	},
	{
	"17.) Phoenix IV,  z1=z; z=z*z*z +.5*real(c) + imag(c)*z2; z2=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, NOSYM, 4, ESCAPING
	},
	{
	"18.) Newton/Mandel, z=-z/3; iterate: z=z-(z*z*z+z*z*c-z+c)/(3*z*z+2*c*z-1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"19.) Newton/Mandel, z=-z/3; iterate: z=z-(z*z*z+z*z*c+z+c)/(3*z*z+2*c*z+1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"20.) Newton/Mandel, z= z/2; iterate: z=z-(z^4-(z^3)*c-z-c)/(4*(z^3)-3*(z^2)*c-1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"21.) Newton/Mandel, z=-1/(3*z); iterate: z=z-((z^3)*c+z^2+z+c)/(3*(z^2)*c+2*z+1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"22.) Newton/Mandel, z=-.5; iterate: z=z-((z^4)c+(z^3)*c+z+c)/(4*(z^3)*c+3*z*z*c+1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"23.) Newton/Mandel, z=-.5/z; iterate: z=z-((z^4)*c+(z^3)+z+c)/(4*(z^3)*c+3*z*z+1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"24.) Newton/Mandel, z=-z/3;  iterate: z=z-((z^3)+z*z*c +c)/(3*z*z+2*z*c)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"25.) Newton/Mandel, z=-z/2;  iterate: z=z-((z^4)+(z^3)*c+c)/(4*(z^3)+3*z*z*c)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"26.) Newton/Mandel, 5th order Newton Mset",
	    NULL, NULL,
	    NULL, NULL, 5.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"27.) Newton/Mandel, 7th order Newton Mset",
	    NULL, NULL,
	    NULL, NULL, 7.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"28.) Newton/Mandel, 9th order Newton Mset",
	    NULL, NULL,
	    NULL, NULL, 9.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"29.) Newton/Mandel, 13th order Newton Mset",
	    NULL, NULL,
	    NULL, NULL, 13.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"30.) Newton/Mandel, 8th order Newton Mset",
	    NULL, NULL,
	    NULL, NULL, 8.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"31.) Newton/Mandel, Newton Diamond",
	    NULL, NULL,
	    NULL, NULL, 5.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"32.) Newton/Mandel, Newton Pentagon",
	    NULL, NULL,
	    NULL, NULL, 6.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"33.) Newton/Mandel, Newton Hexagon",
	    NULL, NULL,
	    NULL, NULL, 7.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"34.) Newton/Mandel, Newton Octagon",
	    NULL, NULL,
	    NULL, NULL, 9.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"35.) Newton/Mandel, 9th order Newton Flower",
	    NULL, NULL,
	    NULL, NULL, 13.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"36.) Newton Variations, z=z-(z*z*z*z*c-z+c)/(4*z*z*z*c)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"37.) Newton Variations, z=z-(z*z*z-z+c)/(3*z*z)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"38.) Newton Variations, z=z-(z*z*z*c-z*c-1)/(3*z*z*c)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"39.) Newton Variations, z=z-(z*z*z*c-z*z*c-1)/(3*z*z*c)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"40.) Newton Variations, z=z-(z*z*z*c-1)/(3*z*z*c)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"41.) Newton Variations, z=z-(z*z*z*c-z-1)/(3*z*z*c-z)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"42.) Newton Variations, z=z-(z*z*z*c-z*c-1)/(3*z*z*c-z)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"43.) Newton Variations, z=z-(z*z*z*c-z*z-1)/(3*z*z*c-3*z*z-3*z)", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"44.) Newton Variations, z=z-(z*z*z*c-z*z*c-1)/(3*z*z*c-z*c-z)", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"45.) Newton Variations, z=((z-(z*z*z-z)/(3*z*z-1))^2)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"46.) Trig functions, z = ccos(z*c)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"47.) Trig functions, z = ((((z*z).csin())*z/2)^2)+c, Sharon 14", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"48.) Trig functions, z = ((z*z).csin()).clog()+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"49.) Trig functions, z = z*z*sin(z.real()) + c*z*cos(z.imag()) + c", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"50.) Trig functions, z = csin(z)*ccos(c)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"51.) Trig functions, z = csin(z*z*z*z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"52.) Newton/Mandel, 8th order Newton flower",
	    NULL, NULL,
	    NULL, NULL, 8.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"53.) Newton/Mandel, 6th order Newton Mset",
	    NULL, NULL,
	    NULL, NULL, 7.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"54.) More Newton Msets, 15th order Newton Mset flower",
	    NULL, NULL,
	    NULL, NULL, 15.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"55.) More Newton Msets, 4th order Newton's apple",
	    NULL, NULL,
	    NULL, NULL, 4.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"56.) More Newton Msets, 25th order Newton Mset flower",
	    NULL, NULL,
	    NULL, NULL, 25.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"57.) More Newton Msets, 38th order Newton Mset flower",
	    NULL, NULL,
	    NULL, NULL, 38.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"58.) More Newton Msets, 50th order Newton Mset flower",
	    NULL, NULL,
	    NULL, NULL, 50.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"59.) More Newton Msets, 5th order Newton Mset",
	    NULL, NULL,
	    NULL, NULL, 6.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"60.) More Newton Msets, 18th order Newton Mset flower",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"61.) Polynomials, z=z*z+c; real=imag; imag=real",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"62.) Newton Variations II, z=(((z^3)-z-1)/((z^3)-1)-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"63.) Newton Variations II, z=(((z^4)-z*z-1)/(4*z*z-1)-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"64.) Newton Variations II, z=(z-((z^3)-1)/(3*z*z-fabs(z)-1))*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"65.) Newton Variations II, z=(z-((z^3)-1)/(3*z*z-z-1))*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"66.) Newton Variations II, z=(((z^4)-z-1)/(4*(z^3)-z-1)-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"67.) Newton Variations II, z=z-(((z^3)-z)/((3*z*z-1)))+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"68.) Newton Variations II, z=(z-((z^3)-1)/(4*z*z-z-1))*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"69.) Newton Variations II, z=(z-((z^3)-1)/(3*z*z-z))*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"70.) Newton Variations II, z=(z-((z^4)-1)/(4*(z^3)-z))*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"71.) Newton Variations II, z=(z-((z^4)-1)/(3*(z^3)-z))*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"72.) Newton Variations II, z=(z-((z^4)-z-1)/(3*(z^3)-z))*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"73.) Newton Variations II, z=c*(z-((z^3)-z)/(3*z*z-1))",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"74.) Newton Variations II, z=((1-z-(z^4))/(z-(4*(z^3)))-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"75.) Newton Variations II, z=((z-(z^3)*z)/(z-(3*(z^3)))-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"76.) Newton Variations II, z=((z-(z^3))/(z-(3*(z^3)))-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"77.) Newton Variations II, z=(((z^3)-1)/(2*z*z-1)-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"78.) Newton Variations II, z=(((z^3)-z-1)/(3*(z^3)-1)-z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"79.) Polynomials, CBAP  F(z) = Z^3 - 3*(A^2)*Z + B(MOD 2)", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 3, ESCAPING
	},
	{
	"80.) Quartets, z=.5; z=z*z-z2*z2+c; z2=z",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"81.) Quartets, z2=2; z=z*z*z*z+z2+c; z2=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 41, ESCAPING
	},
	{
	"82.) Quartets, z2=z; z=z*z*z*z+5*z2*c; z2=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 4, ESCAPING
	},
	{
	"83.) Quartets, t=0; z1=z; z=z*z*z-t*t*t+c; z=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, 3, ESCAPING
	},
	{
	"84.) Quartets, t=0; z1=z; z=z*z*z*z-t*t*t*t+c; t=z1", 
	    NULL, NULL,
	    NULL, NULL, 4.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 4, ESCAPING
	},
	{
	"85.) Quartets, z2=z; z=(z^4)+c; c=z2",
	    NULL, NULL,
	    NULL, NULL, 4.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 4, ESCAPING
	},
	{
	"86.) Newton Variations, z=z-((z^4)-z)/(4*(z^3)-1); z=z*z*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"87.) Newton Variations, z=z-((z^3)-1)/(3*z*z); z=(z^3)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"88.) Newton Variations, z=z-((z^4)-1)/(4*(z^3)); z=(z^4)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"89.) Newton Variations, z=z-((z^5)-1)/(5*(z^4)); z=(z^5)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"90.) Polynomials, z=z+c [quad]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, ESCAPING
	},
	{
	"91.) Polynomials, z=z*z*z-caa3*z+cb; [CCAP]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, 3, CONVERGING
	},
	{
	"92.) Polynomials, z=z*z*z-caa3*z+cb; [CFAP]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, 3, CONVERGING
	},
	{
	"93.) Quartets, z1=z; z=z*z*z*z-z2+c; z2 = z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"94.) Polynomials, z=c*z*(2 - z*z)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, ESCAPING
	},
	{
	"95.) Polynomials, z=z*z*z*z/t1+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"96.) Quartets, z1=z; z=z*z*z*z+z2/2+c;; z2=z1",
	    NULL, NULL,
	    NULL, NULL, 4.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, ESCAPING
	},
	{
	"97.) Polynomials, z = z*z+c*(1+sqrt(z))",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"98.) Quartets, z1=z; z=z*z*z*z/(1+z)+c;; c=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, ESCAPING
	},
	{
	"99.) Quartets, z1=z; z2=c+z2/z-z; z=z*c+z2/c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"100.) Polynomials, real(z) += sin(real(z)); z=z*z+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"101.) M-Set Method",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"102.) Ramon, z = ((z*z+c-1)/(2*z+c-2))^2; [magneto 1]", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"103.) Ramon, z = ((z*z*z+3*c2*z+c1)/(3*z*z+3*c2*z+c1+1))^2; [magneto 2]", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"104.) Ramon, z = ((z*z+c)/(2*z))^2  (Teddy)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"105.) More Fractals, 3rd order Newton/ Mset [figure 8]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"106.) More Fractals, 5th order N/Mset",
	    NULL, NULL,
	    NULL, NULL, 5.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"107.) More Fractals, z1 = z; z = sin(z) - c; c = 1/(z*50); [quartet]", 
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"108.) More Fractals, z=(z*c)/(1+z)+c; [Talis II]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"109.) More Fractals, z=(c+z*z*c)/(1-z*z*c)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"110.) More Fractals, z = (c+(z^6))/(1+(z^2))",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"111.) Ramon, z = (z^2)/(1+c*(z^2))",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"112.) Ramon, z = c*cos(z)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, -1, CONVERGING
	},
	{
	"113.) Ramon, z = c*sin(z)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"114.) Ramon, z = c*exp(z)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"115.) Ramon, z=z^2+c; c=c/2+z [spider]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
	{
	"116.) Ramon, z = (c/sin(z))^2",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, 2, ESCAPING
	},
	{
	"117.) Ramon, z = (c/cos(z))^2",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XYAXIS, 2, ESCAPING
	},
	{
	"118.) N-Set Method",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, 2, ESCAPING
	},
// add some Flarium fractals not included in Tierazon
	{
	"119.) Flarium 06, Newton Variations: z=z-(z*z*z-1)/3*z*z; z=z*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 3, ESCAPING
	},
	{
	"120.) Flarium 07, Polynomial: z = (z*z+c)^(cn+c)",
	    NULL, NULL,
	    NULL, NULL, 3.0, 0.0, 0.0, 0.0, 1000.0, NOSYM, -1, ESCAPING
	},
	{
	"121.) Flarium 08, Sharon Webb: z = z*z*z*z+1/c; [Sharon Star]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"122.) Flarium 09, Sharon Webb: z = (z*z/2+c)*(z*z/2+c); [Sharon's Space Probe]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"123.) Flarium 10, Sharon Webb: z = z*z*z*z+((c/2)^2)+c; [Sharon08]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"124.) Flarium 11, Sharon Webb: z = z*z*z*z+(z^(z+cn))+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"125.) Flarium 12, Sharon Webb: z = (z+z*z/2)+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"126.) Flarium 13, Sharon Webb: z=(z*z*z*z-z2*z2*z2*z2+c)^2",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"127.) Flarium 15, Newton Variations: z = z - (z*z*z*z-1)/(4*z*z*z) + c; [4th Order Nova]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"128.) Flarium 16, Newton Variations: z = z - (z*z*z*z*z-1)/(5*z*z*z*z) + c; [5th Order Nova]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 4.0, XAXIS, -1, CONVERGING
	},
	{
	"129.) Flarium 20, Phoenix: z = z*z*z*z+c.imag()+c.real()*z2; [4th Order Phoenix]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, NOSYM, 4, ESCAPING
	},
	{
	"130.) Flarium 21, Sharon Webb: z=(1/z*z-c)*(z*z*z*z+c); [Sharon03]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"131.) Flarium 25, Newton Variations: z=z-(z*z*z*z-z)/(4*z*z*z-z); z=z*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"132.) Flarium 27, Polynomial: z=z*z*(cn+z)/(cn+z+c)+c",
	    NULL, NULL,
	    NULL, NULL, 3.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"133.) Flarium 28, Derbyshire / Newton: z=z - (z*z*z-1)/(3*z*z)+c; [Nova-Mandelbrot-MultiFract]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"134.) Flarium 29, Derbyshire / Newton: c = (z*z*z-1)/(3*z*z); z -= (z*z*z-1)/(3*z*z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"135.) Flarium 30, Derbyshire / Newton: z -= (z*z*z-1)/(3*z*z)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"136.) Flarium 31, Polynomial: z = z*z*z-aa3*z+b",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 3, ESCAPING
	},
	{
	"137.) Flarium 32, Derbyshire / Newton: 3rd Order Nova in a M-Set (Try single pass)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 3, ESCAPING
	},
	{
	"138.) Flarium 34, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 3, ESCAPING
	},
	{
	"139.) Flarium 35, Polynomial: z=c*(z.csin())",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"140.) Flarium 36, Polynomial: z=(c*z).csin()",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"141.) Flarium 37, Polynomial: z=(z*z*z-1)/(3*z*z); z = c*(z.csin() + z.ccos())",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"142.) Flarium 38, Polynomial: z = z*z+c; z.set_real(z.real()*z.real()); [Variation real]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"143.) Flarium 40, Polynomial: z1 = z*z*z*z; z = c*z1/4*z1 + z",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"144.) Flarium 41, Polynomial: z = c*(z*z*z*z).csin()",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"145.) Flarium 43, Polynomial: z = z*z*z*z + (z*c).csin() + c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"146.) Flarium 44, Polynomial: z=(z*z*z*z-z)/(4*z*z*z); z=c*z.csin()",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"147.) Flarium 45, Polynomial: z=(z*z*z*z-1)/(4*z*z*z); z=c*z.csin()",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"148.) Flarium 46, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14 N-Method]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"149.) Flarium 49, Polynomial: z=(z*z*z*z-z)/((4*z*z*z)-z); z=c*z.csin()",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"150.) Flarium 50, Sharon Webb: z = z*z*z*z+1/c; [Sharon's Star M-Set]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"151.) Flarium 51, Sharon Webb: z = (z*z/2+c)*(z*z/2+c); [Space Probe M-Set]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"152.) Flarium 52, Sharon Webb: z = z*z*z*z+t+c; [Sharon08 M-Set]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"153.) Flarium 53, Sharon Webb: z = (z+z*z/2)+c; [M-Set]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING2
	},
	{
	"154.) Flarium 54, Polynomial: z = z*z*c+c; [M-Set]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"155.) Flarium 55, Polynomial: z = (z*z).csin()*z*z*c+c; [M-Set]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"156.) Flarium 56, Polynomial: z = (z*z+c)/(z*z-c); [M-Set]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"157.) Flarium 57, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-z)+c; [5th Order Nova Variation]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"158.) Flarium 58, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"159.) Flarium 59, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-1)+c; [3rd Order Nova Variation]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"160.) Flarium 60, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-1)+c; [5th Order Nova Variation]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"161.) Flarium 61, Polynomial: z=z*z*z*z+(c+(c/pi))",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 4, ESCAPING
	},
	{
	"162.) Flarium 62, Polynomial: z=z*z*(1+z)/(1+z+c)+c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"163.) Flarium 64, PHOENIX: z1=z; z=z*z*z*z+c.real()*z2/2+c.imag()*z2/2+c; z2=z1",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, NOSYM, 4, ESCAPING
	},
	{
	"164.) Flarium 66, Newton Variations: z = ((z-(((z^2)-1)/(2*z)))^2)*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"165.) Flarium 67-69, Newton Variations: z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c; ",
	    NULL, NULL,
	    NULL, NULL, 3.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 2, ESCAPING
	},
	{
	"166.) Flarium 80, Newton Variations: z=((z*z*z*z-1)/(1-(3*z*z*z)))*c",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"167.) Flarium 90, Sharon Webb: z = (z*(z*z).csin()/2).csin()+c; [Sharon15]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"168.) Flarium 91, Sharon Webb: z = (z*(z+z).ccos()/2); z = z*z+c; [Sharon16]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"169.) Flarium 92, Sharon Webb: z = (z*(z+z).csin()/2); z = z*z+c; [Sharon17]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"170.) Flarium 93, Sharon Webb: z = (z*(z+z*z).csin()/2); z = z*z+c; [Sharon18]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"171.) Flarium 111, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Alden's Ray Method]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"172.) Flarium 112-116, Polynomials: z=z^n*c+z*c; Dragon curve variations",
	    NULL, NULL,
	    NULL, NULL, 3.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"173.) Flarium 117, Newton Variations: z = z-(z*z*z-z*c-1)/(3*z*z+c-1)",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, CONVERGING
	},
	{
	"174.) Flarium 118, Sharon Webb: z = (z*z*z*z*z).csin() + c; [Sharon19]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"175.) Flarium 119, Sharon Webb: z = (z+(z*z)/.192).csin() + c; [Sharon's Butterfly]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, -1, ESCAPING
	},
	{
	"176.) Flarium 125, Sharon Webb: z = z+z*z*z/4 + c; [Sharon21]",
	    NULL, NULL,
	    NULL, NULL, 0.0, 0.0, 0.0, 0.0, 1000.0, XAXIS, 3, ESCAPING
	},
	{
	"177.) Flarium 145, Polynomial: z=z^2+c [Jaenisch method]",
	    NULL, NULL,
	    NULL, NULL, 2.0, 0.0, 4.0, 0.0, 1000.0, NOSYM, 2, ESCAPING
	},
	NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0
    };

//////////////////////////////////////////////////////////////////
// database of Bignum fractal functions
//////////////////////////////////////////////////////////////////

struct BigFractalStuff BigFractalSpecific[]=
    {
//    int	    type;				// label of the fractal 
//    int	    (*big_per_pixel)(void);		// Bignum versions 
//    int	    (*big_calctype)(void); 

	{ CUBIC,	    NULL,			NullSetup },
	{ POWER,	    NULL,			NullSetup },
	{ MANDELFP,	    NULL,			NullSetup },
	{ MANDEL,	    NULL,			NullSetup },
	{ NEWTONPOLYGON,    NULL,			NullSetup },
	{ MATEIN,	    NULL,			NullSetup },
	{ SINFRACTAL,	    NULL,			NullSetup },
	{ EXPFRACTAL,	    NULL,			NullSetup },
	{ SPECIALNEWT,	    NULL,			NullSetup },
	{ NEWTONAPPLE,	    NULL,			NullSetup },
	{ NEWTONFLOWER,	    NULL,			NullSetup },
	{ MAGNET1,	    NULL,			NullSetup },
	{ MAGNET2,	    NULL,			NullSetup },
	{ TEDDY,	    NULL,			NullSetup },
	{ NEWTONMSET,	    NULL,			NullSetup },
	{ NEWTONJULIANOVA,  NULL,			NullSetup },
	{ TALIS,	    NULL,			NullSetup },
	{ NEWTONCROSS,	    NULL,			NullSetup },
	{ QUARTET1,	    NULL,			NullSetup },
	{ QUARTET2,	    NULL,			NullSetup },
	{ QUARTET3,	    NULL,			NullSetup },
	{ RAMONSIN,	    NULL,			NullSetup },
	{ RAMONCOS,	    NULL,			NullSetup },
	{ FORMULA05,	    NULL,			NullSetup },
	{ SHARON,	    NULL,			NullSetup },
	{ QUAD,		    NULL,			NullSetup },
	{ TIERAZON,	    NULL,			NullSetup },
	{ NOVA,		    NULL,			NullSetup },
	{ BURNINGSHIP,	    NULL,			NullSetup },
	{ BURNINGSHIPPOWER, NULL,			NullSetup },
	{ MANDELDERIVATIVES,  NULL,			NullSetup },
	{ PERTURBATION,	    InitPerturbation,		DoPerturbation },	// this is included here because all operations are done in arbitrary precision. 
//	{ PERPMANDEL,	    init_big_mandel,		Do_big_PerpMandelbrot },
//	{ BUFFALO,	    init_big_mandel,		Do_big_Buffalo },
//	{ PERPBURNINGSHIP,  init_big_mandel,		Do_big_PerpBurningShip },
//	{ CELTIC,	    init_big_mandel,		Do_big_Celtic },
//	{ PERPCELTIC,	    init_big_mandel,		Do_big_PerpCeltic },
//	{ MANDELBARCELTIC,  init_big_mandel,		Do_big_MandelbarCeltic },
//	{ PERPBUFFALO,	    init_big_mandel,		Do_big_PerpBuffalo },
//	{ MANDELBAR,	    init_big_mandel,		Do_big_Mandelbar },
//	{ REDSHIFTRIDER,    init_big_RedShiftRider,	Do_big_RedShiftRider },
//	{ REDSHIFTRIDER,    NULL,			NullSetup },
	{ SLOPEDERIVATIVE,  InitSlopeFractal,		DoSlopeFractal },	// this function runs both float and bignum
	{ SLOPEFORWARDDIFF, InitSlopeFractal,		DoSlopeFractal },	// same function used but splits depending on type
//	{ POLYNOMIAL,	    NULL,			NullSetup },
	{ 0, NULL, NULL }
    };

//////////////////////////////////////////////////////////////////
// database of DirectFunction initialisation
//////////////////////////////////////////////////////////////////

char	*InitCond[] = 

    {
    "z=pixel","z=c=pixel","z=c=1/pixel","c=pixel,z=1/c","z=pixel,c=log(1+z)","z=pixel,c=z*z","z=pixel,c=z/(z+1)","z=pixel,c=z/(z*z+1)","z=p1",
    "z = c = pixel , zc = 0",
    "z = pixel, z2 = z*z, z3 = z*z2",
    "z=atanh(pixel)",
    "z=pixel, c=p1; for Julia Animation - Use parameter animation - 'P' key and set ranges for P1",
//    "z = c = pixel, x = real(z), y = imag(z), z1 = y * y, z2 = flip(z1) + 1.0 + x * x; use for feather fractal",
    NULL
    };

//////////////////////////////////////////////////////////////////
// database of DirectFunction initialisation
//////////////////////////////////////////////////////////////////

char	*DirectFormula[] = 

    {
    "z = z*z + c + p1 ; Good old Mandelbrot",
    "z = z*z*z + c + p1 ; Basic Cubic", 
    "z = z*z*z*z + z*z*z + z*z + c + p1	; https://www.youtube.com/watch?v=IN16QJaNHdo",
    "z = z-(z*z*z*z-z)/(4*z*z*z-z)+c + p1 ; Cute little Newton",
    "z = (z^2+c)/(c^2+z) + p1 ; http://www.fractalforums.com/general-discussion-b77/t24459/msg95822/#msg95822", 
    "z = (z^3+c)/(c^3+z) + p1 ; http://www.fractalforums.com/general-discussion-b77/t24459/msg95822/#msg95822", 
    "z = (z^4+c)/(c^4+z) + p1 ; http://www.fractalforums.com/general-discussion-b77/t24459/msg95822/#msg95822", 
    "z = (z^-2+c)/(c^-2+z) + p1 ; http://www.fractalforums.com/general-discussion-b77/t24459/msg95822/#msg95822", 
    "z = (z^-3+c)/(c^-3+z) + p1 ; http://www.fractalforums.com/general-discussion-b77/t24459/msg95822/#msg95822", 
    "z = (z^-4+c)/(c^-4+z) + p1 ; http://www.fractalforums.com/general-discussion-b77/t24459/msg95822/#msg95822", 
    "x = abs(real(z)),y=-imag(z), z = x+flip(y), z=z*z+pixel + p1 ; Perp Mandelbrot", 
    "x = abs(real(z)),y=-abs(imag(z)), z = x+flip(y), z=z*z+pixel + p1 ; Burning Ship", 
    "x = (real(z)),y=-abs(imag(z)), z = x+flip(y), z=z*z+pixel + p1 ; Perp Burning Ship", 
    "x = abs(real(z)),y=(imag(z)), z = x+flip(y), z=z*z+pixel + p1 ; Heart Mandelbrot", 
    "x = -(real(z)),y=(imag(z)), z = x+flip(y), z=z*z+pixel + p1 ; Mandelbar", 
    "x = real(z),y=imag(z), z = flip(x*y*2.0)+abs(x*x-y*y), z=z+pixel + p1 ; Celtic Mandelbrot", 
    "x = real(z),y=imag(z), z = flip(-x*y*2.0)+abs(x*x-y*y), z=z+pixel + p1 ; Celtic Mandelbar", 
    "x = abs(real(z)),y=imag(z), z = flip(-x*y*2.0)+abs(x*x-y*y), z=z+pixel + p1 ; Perp Celtic ", 
    "x = real(z),y=imag(z), z = abs(x*x-y*y)+flip(abs(x)*y*2.0), z=z+pixel + p1 ; Celtic Heart ", 
    "x = real(z),y=imag(z), z = flip(-abs(x*y*2.0))+abs(x*x-y*y), z=z+pixel + p1 ; Buffalo ", 
    "x = real(z),y=imag(z), z = flip(-x*abs(y)*2.0)+abs(x*x-y*y), z=z+pixel + p1 ; Perp Buffalo ", 
    "x = real(z),y=imag(z),z=-(x*x-(y*y*3.0))*x+flip(((x*x*3.0)-y*y)*y)+pixel + p1 ; Cubic Mandelbar Orthoginal", 
    "x = real(z),y=imag(z),z=(x*x-(y*y*3.0))*x+flip(-((x*x*3.0)-y*y)*y)+pixel + p1 ; Cubic Mandelbar Diagonal", 
    "x = real(z),y=imag(z),z=abs((x*x-(y*y*3.0))*x)+flip(abs(((x*x*3.0)-y*y)*y))+pixel + p1 ; Cubic Buffalo", 
    "x = real(z),y=imag(z),z=(x*x-(y*y*3.0))*x+flip(abs(((x*x*3.0)-y*y)*y))+pixel + p1 ; Partial Cubic Buffalo Imaginary", 
    "x = real(z),y=imag(z),z=(x*x-(y*y*3.0))*x+flip(((x*x*3.0)-y*y)*abs(y))+pixel + p1 ; Partial Cubic Burning Ship Imaginary", 
    "x = real(z),y=imag(z),z=(x*x-(y*y*3.0))*abs(x)+flip(((x*x*3.0)-y*y)*y)+pixel + p1 ; Partial Cubic Burning Ship Real", 
    "x = real(z),y=imag(z),z=(x*x-(y*y*3.0))*abs(x)+flip(((x*x*3.0)-y*y)*abs(y))+pixel + p1 ; Cubic Burning Ship ", 
    "x = real(z),y=imag(z),z=abs((x*x-(y*y*3.0))*x)+flip(((x*x*3.0)-y*y)*y)+pixel + p1 ; Partial Cubic Buffalo Real (Celtic)", 
    "x = real(z),y=imag(z),z=(x*x-(y*y*3.0))*abs(x)-flip(abs(((x*x*3.0)-y*y)*y))+pixel + p1 ; Quasi Burning Ship 3rd (Hybrid)", 
    "x = real(z),y=imag(z),z=(x*x-(y*y*3.0))*abs(x)-flip(abs(((x*x*3.0)-y*y))*y)+pixel + p1 ; Quasi Perpendicular 3rd ", 
    "x = real(z),y=imag(z),z=(x*x-(y*y*3.0))*abs(x)+flip(abs(((x*x*3.0)-y*y))*y)+pixel + p1 ; Quasi Heart 3rd ", 
    "z = (pixel ^ z) + p1,|z| <= (P2 + 3) ; Julia form 2 of the Tetration formula --Lee Skinner", 
    "z = (z*z+z)/(2*z*z+c)+p1 ; Guitar fractal Written by Paul Bourke October 2005 m=2", 
    "z = (z*z+z)/(2*z*z*z+c)+p1 ; Guitar fractal Written by Paul Bourke October 2005 m=3", 
    "z = (z*z+z)/(2*z*z*z*z+c)+p1 ; Guitar fractal Written by Paul Bourke October 2005 m=4", 
    "z = (z*z*z*z+z*z+1)/(2*z*z*z-c+1)+p1 ; Binocular fractal Written by Paul Bourke October 2005 m=4", 
    "z = z*z*z+(c-1)*z+c+p1 ; Cactus fractal Written by Paul Bourke October 1991",
    "z = sin(z)+c+p1 ; Test trig functions PHD 2020-07-30",
    "z = sin(z)*c+p1 ; Test trig functions PHD 2020-07-30",
    "z = sqr(sin(z))+c+p1 ; Test nested functions PHD 2020-07-30",
    "IF (zc < 0),  z = z - p1, ELSE, z = z - zc - p1 ,ENDIF, zc = z*c; Sylvie Gallet, March 1997",
    "z1 = conj(z), z = z1 * z1 + c + p1; Tricorn - Test conjugation PHD 2021-01-03",
    "if (imag(c) > 0), z1 = z*z+pixel, else, z1 = z*z*z+pixel, endif, z = z1; Test condtional PHD 2020-07-30",
    "if (imag(c) > 0 && real(c) > 0), z1 = z*z+pixel+p1, elseif (imag(c) > 0 && real(c) < 0), z1 = z*z*z+pixel+p1, elseif (imag(c) < 0 && real(c) > 0), z1 = z*z*z*z*z+pixel+p1, else, z1 = z*z*z*z+pixel+p1,endif, z = z1; Test condtional PHD 2020-07-30",
    "z = (z^z) + asin(z),z = sin(z)+p1; Nice Fractal from FractRacer: use \'z=atanh(pixel)\' as the initial condition",
    "z = (z^(z^z)) + asin(z),z = sin(z)+p1; FractRacer 3 level Tetration: use \'z=atanh(pixel)\' as the initial condition",
    "z = (z^(z^(z^z))) + asin(z),z = sin(z)+p1; FractRacer 4 level Tetration:: use \'z=atanh(pixel)\' as the initial condition",
    "z = (z^(z^(z^(z^z)))) + asin(z),z = sin(z)+p1; FractRacer 5 level Tetration:: use \'z=atanh(pixel)\' as the initial condition",
    "z = (z^(z^(z^(z^(z^z))))) + asin(z),z = sin(z)+p1; FractRacer 6 level Tetration:: use \'z=atanh(pixel)\' as the initial condition",
    "z = (z^(z^(z^(z^(z^(z^z)))))) + asin(z),z = sin(z)+p1; FractRacer 7 level Tetration:: use \'z=atanh(pixel)\' as the initial condition",
    "x = real(z), y = imag(z), z1 = y * y, z2 = flip(z1) + 1.0 + x * x, z = (z*z*z)/z2 + c + p1; Feather Fractal: use bailout of 1000",
    "cx = real(c), cy = imag(c), c1 = cy * cy, c2 = flip(c1) + cx * cx, x = real(z), y = imag(z), z = z * (x*x + y*y) - (z * c2) + p1; SFX Fractal: use bailout of 1000",
    "cx = real(c), cy = imag(c), x = real(z), y = imag(z), z2 = cy * x, z = flip(z2) + 1.0 - cx * x * x + y + p1; Henon Fractal: use bailout of 1000",
    "cx = real(c), cy = imag(c), x = real(z), y = imag(z), z2 = -cy*x + cx*y - y*y*y, z = flip(z2) + y + p1; Duffing Fractal: use bailout of 1000",
    "cx = real(c), cy = imag(c), x = real(z), y = imag(z), t = 0.4 - 6.0 / (1.0 + x*x + y*y), st = sin(t), ct = cos(t), z2 = cy*(x*st + y*ct), z = flip(z2) + 1.0 + cx*(x*ct - y*st) + p1; Ikeda Fractal: use bailout of 1000",
    "cx = real(c), cy = imag(c), x = real(z), y = imag(z), z2 = cy*sin(x), z1 = flip(z2) + cx*y, z = z + z1 + p1; Chirikov Fractal: use bailout of 1000",
    NULL
    };

//////////////////////////////////////////////////////////////////
// database of Oscillator fractals
//////////////////////////////////////////////////////////////////

struct	CoordSysInfoStuff	CoordSysInfo = 
    {
    "Cartesian",			// co-ordinate systems
    "Spherical",
    "Cylindrical",
    "Parabolic",
    "Paraboloidal",
    "Elliptical",
    "Bipolar",
    "Toroidal",
    "Prolate Spheroidal",
    "Oblate Spheroidal",
    "Bispherical",
    "Bipolar Cylindrical",
    "",
    ""
    };

//////////////////////////////////////////////////////////////////
// database of fractal maps
//////////////////////////////////////////////////////////////////

struct OscillatorSpecificStuff FractalMapSpecific[]=
    {
	{
	"2D Circular Chaotic Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, Do2DCircular, "SelectOscParams", (DLGPROC)SelectOscParams, 0.814, 1.0, 1.0
	},
	{
	"2D-MAP-Modelo - Experimental", "k", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.77, -1.22, 2.37, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, Do2DMAPModeloExperimental, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D Logistic-Modulated-Sine-Coupling-Logistic Chaotic Map for Image Encryption", "alpha", "beta", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.75, 3.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.45, -0.07, 1.2, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 160000.0,
	    NullSetup, Do2DLogisticModulatedSineCouplingLogisticChaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D Logistic-Sine-Coupling Map for Image Encryption", "r", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.62, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.1, 1.2, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 160000.0,
	    NullSetup, Do2DLogisticSineCouplingMapImageEncryption, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D Memristive Hyperchaotic Maps and Application in Secure Communication - model 2D MLM", "mu", "k", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.2, 2.08, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-4.8, 0.1, 5.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, Do2DMemristiveHyperchaoticMapsMLM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D Memristive Hyperchaotic Maps and Application in Secure Communication - model 2D MTM", "mu", "k", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -0.6, 1.78, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8, -0.8, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-5.4, 0.26, 6.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, Do2DMemristiveHyperchaoticMapsMTM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D Memristive Hyperchaotic Maps and Application in Secure Communication - model 2D SMM", "mu", "k", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.5, 2.33, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-3.2, 1.4, 3.6, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, Do2DMemristiveHyperchaoticMapsSMM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D Memristive Hyperchaotic Maps and Application in Secure Communication - model 2D MSN", "mu", "k", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -0.1, 1.88, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-3.9, 1.0, 4.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, Do2DMemristiveHyperchaoticMapsMSN, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D Rational Chaotic Fractal Map Type 1", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 4.0, 0.9, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, Do2DRational, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D Rational Chaotic Fractal Map Type 2", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.4, 1.3, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, Do2DRational, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"2D SIMM Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 10.0, 10.0, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -21.0, -12.0, 24.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, Do2DSIMMMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 4.0
	},
	{
	"2D Sine Logistic Modulation Map for Image Encryption", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.0, 3.0, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.7, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.1, 1.2, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, Do2DSineLogisticModulationMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A 2D Hyperchaotic Map with Conditional Symmetry and Attractor Growth I", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 3.38, 0.4, 0.0, 0.0, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -1.5 + PI, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, 9.6, 3.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoA2DHyperchaoticMapConditionalSymmetryAttractorGrowth, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A 2D Hyperchaotic Map with Conditional Symmetry and Attractor Growth II", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 3.34, 0.4, 0.1, -0.000025, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -1.3 + PI, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -73.5, -11.1, 100.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoA2DHyperchaoticMapConditionalSymmetryAttractorGrowth, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.0001, 1.0
	},
	{
	"A 2D Hyperchaotic Map with Conditional Symmetry and Attractor Growth III", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 3.8, 0.8, 0.00001, 0.000025, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -29.5, -2.9, 36.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoA2DHyperchaoticMapConditionalSymmetryAttractorGrowth, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A 2D Hyperchaotic Map with Conditional Symmetry and Attractor Growth IV", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 3.359, 0.4, 0.00001, 1.25, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.075, 0.0025, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.5, 37.6, 2.6, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoA2DHyperchaoticMapConditionalSymmetryAttractorGrowth, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A 2D Hyperchaotic Map with Conditional Symmetry and Attractor Growth V", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 3.44, 0.4, 0.1, 0.00015, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, -7.75, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -8.89, 2.6, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoA2DHyperchaoticMapConditionalSymmetryAttractorGrowth, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A Color Image Encryption Algorithm based on Hyperchaotic Map and Rubik's Cube-Modified Formula", "a", "# of Parallel Lines", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.875, 9.5, 1.0, 1.0, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-1.95, -1.13, 2.28, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	NullSetup, DoAColorImageEncryptionAlgorithmMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.1, 1.0
	},
	{
	"A Higher Dimensional Chaotic Map with Discrete Memristor - 3D-model 2", "a", "c", "w", "k", "b", "Colour Factor", ES, ES, ES, ES, 5.0, 0.5, PI / 3, 0.333, -0.5, 1200.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.7, -2.7, 5.4, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 750000.0,
	    NullSetup, DoAHigherDimensionalChaoticMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.4, 1.0
	},
	{
	"A Higher Dimensional Chaotic Map with Discrete Memristor - 3D-model 3", "a", "c", "w", "k", "b", "Colour Factor", ES, ES, ES, ES, 5.0, 0.5, PI / 3, 0.333, -0.5, 1200.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.7, -2.7, 5.4, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 750000.0,
	    NullSetup, DoAHigherDimensionalChaoticMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.4, 1.0
	},
	{
	"A Hyperchaotic Map with Grid Sinusoidal Cavity", "a", "w", "c", "h", "k", "Colour Factor", ES, ES, ES, ES, 2.5, 6.0, 2.0 * PI, 50.0, 2.0 * PI, 50.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, PI, PI / 10.0, PI, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -2.8, 5.5, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAHyperchaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.8, 0.2
	},
	{
	"A Lightweight Image Encryption Algorithm Based on Message Passing and Chaotic Map", "r1", "r2", "c1", "c2", "Colour Factor", ES, ES, ES, ES, ES, 3.2, PI, 0.19, 0.1, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.75, 0.00002, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.1, 0.2, 0.83, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoALightweightImageEncryptionAlgorithmMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Chaotic Attractor from 2D Discrete Mapping Via Border-Collision Period-Doubling Scenario", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.4, 0.3, 600.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.5, -2.6, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANewChaoticAttractorDiscreteMapping, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 2.0, 1.0
	},
	{
	"A New Chaotic Map for Secure Transmission", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.52, 0.05, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.3, 2.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewChaoticMapSecureTransmission, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Two-dimensional Chaotic Maps with Closed Curve Fixed Points", "a", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 2.29, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.27, 0.28, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.13, -0.8, 1.6, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClass2DChaoticMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
       	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I01", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.971, 0.54, 0.23, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, 0.0, 3.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I02", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 800.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.62, 1.28, 1.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.85, 1.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I03", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.84, 1.25, 1.46, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.32, 2.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I04", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.91, 1.63, 2.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.45, -0.34, 2.7, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I05", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -3.09, 0.09, 0.63, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.54, 1.2, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps5, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I06", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.84, 1.25, 1.46, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.8, -2.54, 3.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps6, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I13", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.5, 2.84, -1.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.32, -1.16, 3.2, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps13, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I14", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.08, 3.02, -3.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -2.5, 4.7, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps14, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I16",
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 20.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.57, 0.56, -2.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.54, 12.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewClassThreeDimensionalMaps16, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Hyperchaotic Map and its Application for Image Encryption",
	     "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 3.0, 2.0, 2000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.4, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.44, -0.25, 0.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewHyperchaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.7, 1.0
	},
	{
	"A New Nonlinear Oscillator with Infinite Number of Coexisting Hidden and Self-Excited Attractors-Convoluted",
	     "w", "azao", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.5, 1.9, 2000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -51.0, 0.02, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewNonlinearOscillatorInfiniteNumberMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Simple 2D Piecewise Linear Map-I",
	    "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.1, 0.2, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.32, 1.6, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewSimple2DPiecewiseLinearMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Simple 2D Piecewise Linear Map-II",
	    "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.1, -0.2, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.27, 1.3, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewSimple2DPiecewiseLinearMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A New Simple 2D Piecewise Linear Map-III",
	    "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.1, -1.1, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.75, -0.18, 1.3, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANewSimple2DPiecewiseLinearMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A Novel Chaos-Based Image Encryption Algorithm using DNA Sequence Operations",
	    "m1", "m2", "l1", "l2", "Colour Factor", ES, ES, ES, ES, ES, 3.055, 3.0, 0.18, 0.14, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.128, 0.456, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, 0.22, 0.8, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANovelChaosBasedImageEncryptionAlgorithmMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
/*
	{
	"A Novel Class of Two-Dimensional Chaotic Maps with infinitely Many Coexisting Attractors",
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 10.0*PI, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, 0.22, 0.8, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoANovelClassTwoDimensionalChaoticMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
*/
	{
	"A Novel Control Method to Counteract the Dynamical Degradation of a Digital Chaotic Sequence", 
	    "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.0, 5.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.57, 0.56, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoANovelClassControlMethodCounteractDynamicalDegradation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.6
	},
	{
	"A Novel Megastable MAP with a Strange Coexisting Attractors: Design, Analysis, and FPGA Implementation",
	    "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.8765, 1.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoANovelMegastableOscillatorMAP, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A Secure Image Encryption Scheme based on a Novel 2D Sine.Cosine Cross.Chaotic (SC3) Map",
	    "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.92, 0.845, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.6, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.29, 0.53, 0.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoASecureImageEncryptionSchemeMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"A Two-Dimensional Chaotic Quasi-Conservative Map - Mira - Gumoswski",
	    "alpha", "mu", "beta", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.005, 0.125, 0.82, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.5, -4.8, 10.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoA2DimensionalChaoticQuasiConservativeMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 2.0
	},
	{
	"Abundance of Attracting, Repelling and Elliptic Periodic Orbits in 2-Dimensional Reversible Maps - Var IV",
	    "e", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 2.5, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -5.2, 11.7, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoAbundanceAttractingRepellingEllipticPeriodicOrbits, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.5, 1.0
	},
	{
	"An Algorithm of Image Encryption Using Logistic and Two-Dimensional Chaotic Economic Maps", 
	    "k", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, 0.87, 2.0, 0.5, 0.5, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.11, 0.15, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -0.2, 10.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAnAlgorithmImageEncryptionUsingLogistic2DimensionalChaoticEconomicMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 2.0
	},
	{
	"An Amplitude-Controllable 3D Hyperchaotic Map with Homogenous Multistability",
	    "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.8925, 1.0, -2.0, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.0, -5.0, 10.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoAnAmplitudeControllable3DHyperchaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"An Improved Image Encryption Algorithm based on Chaotic System Map",
	    "m", "n", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 3.678, 3.97, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.335, 0.6, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.025, -0.067, 0.67, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoAnImprovedImageEncryptionAlgorithmMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.5, 1.0
	},
	{
	"An Initial Controlled Double-Scroll Hyperchaotic Map",
	"a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.0, -1.999, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-48.0, -27.0, 54.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoAnInitialControlledDoubleScrollHyperchaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Anti-Martin: a Variation of the fractal 'MARTIN' formula in Fractint 20", "a", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 1.0 / PI - 4.15, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -394.0, -214.0, 436.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 60000.0,
	    NullSetup, DoAntiMartinMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Application of New 4D Chaotic Map for Secure IP-Communications",
	"a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.55, 1.7, 1.6, 0.4, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.0001, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-1.0, -0.6, 1.4, 1, 2, 3, 5, 4, 4, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoApplicationNew4DChaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.7, 1.0
	},
	{
	"Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.3.0",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.008, 0.1, -0.496, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-22.0, -16.0, 27.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 400000.0,
	NullSetup, DoAreMathematicsSourceArtisticCreationPropertiesMap430, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.4",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.001, 0.5, -0.955, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.751, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-26.5, -16.0, 32.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	NullSetup, DoAreMathematicsSourceArtisticCreationPropertiesMap44, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 3.0, 5.0
	},
	{
	"Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.5-1",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.009, 0.2, -0.35, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-26.5, -16.0, 32.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoAreMathematicsSourceArtisticCreationPropertiesMap451, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 5.0
	},
	{
	"Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.5-2",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.005, 0.45, -0.42, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-16.0, -9.0, 20.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoAreMathematicsSourceArtisticCreationPropertiesMap452, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.5-3",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0083, 0.1, -0.38, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-18.05, -11.0, 22.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoAreMathematicsSourceArtisticCreationPropertiesMap453, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Asymmetry Evolvement and Controllability of a Symmetric Hyperchaotic Map",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 2.1, 0.5, 500.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, -2.0, 1.0, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-5.5, -3.0, 6.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoAsymmetryEvolvementControllabilityMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Basin Chaotic Fractal Map", "a", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.4, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -2.0, 3.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBasin, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bifurcations and Chaos in a Three-Dimensional Generalised Hénon Map - I", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.2759 + 0.15, -0.85, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.14, -1.1, 2.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBifurcationsChaos3DHénonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bifurcations and Chaos in a Three-Dimensional Generalised Hénon Map - II", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.3281 + 0.40, 0.5, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.72, -0.86, 2.2, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBifurcationsChaos3DHénonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bifurcations and Chaos in a Three-Dimensional Generalised Hénon Map - III", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.756 + 0.15, -0.5, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.42, -1.25, 2.8, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBifurcationsChaos3DHénonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bifurcations in the Lozi Fractal Map Marcus Variation I", "a", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.95, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.0, -1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -6.5, 16.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoBifurcationsInLoziMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bifurcations in the Lozi Fractal Map Marcus Variation II", "a", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.2, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.3, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -1.48, 2.2, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoBifurcationsInLoziMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Biometric Information Security Based on Double Chaotic Rotating Diffusion", "t", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.05, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.21, -1.67, 3.5, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoBiometricInformationSecurityMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bogdanov Map-Bifurcations, Mode Locking and Chaos in a Dissipative System", "K", "E", "M", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.2, 0.0, -0.002, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.6, 1.2, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 80000.0,
	    NullSetup, DoBogdanovMapBifurcations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bogdanov - Takens Bifurcation", "b1", "b2", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -6.0 / 26.0, -1.031, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.1, 2.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBogdanovTakensBifurcationMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Boosted Coexisting Chaos in a 2-D Sine Map and Its Hardware Implementation", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.5, 3.8, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, 0.3, 2.6, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoBoostedCoexistingChaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.5, 1.0
	},
	{
	"Burgers Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.5, 0.85, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.4, -2.0, 4.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBurgers, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Business Cycle Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.9, -1.2, 2.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoBusinessCycle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cathala Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.7, -0.82, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.0, 2.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCathalaMAP, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos and Hyperchaos in a Symmetric Coupling of Three Quadratic Maps - I", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.7072, 0.4165, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.05, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 210000.0,
	    NullSetup, DoChaosHyperchaosSymmetricCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos and Hyperchaos in a Symmetric Coupling of Three Quadratic Maps - II", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.71, 0.4165, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.05, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 210000.0,
	    NullSetup, DoChaosHyperchaosSymmetricCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos and Hyperchaos in a Symmetric Coupling of Three Quadratic Maps - III", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.72, 0.4165, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.05, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 210000.0,
	    NullSetup, DoChaosHyperchaosSymmetricCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos-Hyperchaos Transition Map", "a", "epsilon", "real", "imag", "Colour Factor", ES, ES, ES, ES, ES, 3.67857351, 0.965, 1.0, 1.0, 10000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.7, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.8, 0.063, 0.065, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 250000.0,
	    NullSetup, DoChaosHyperchaosTransitionMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.2, 1.0
	},
	{
	"Chaos and Hyperchaos in a Symmetric Coupling of Three Quadratic Maps - IV", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.4, -0.1, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.005, 0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-1.5, -0.5, 2.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoChaosHyperchaosSymmetricCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos and Hyperchaos in a Symmetric Coupling of Three Quadratic Maps - V", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.6, 0.4, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.005, 0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-1.0, -0.4, 1.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoChaosHyperchaosSymmetricCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos and Hyperchaos in a Symmetric Coupling of Three Quadratic Maps - VI", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.709, 0.4165, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-1.6, -0.9, 2.2, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoChaosHyperchaosSymmetricCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos and Hyperchaos in a Symmetric Coupling of Three Quadratic Maps - VII", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.6, 0.4165, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.005, 0.02, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-1.5, -0.5, 2.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	NullSetup, DoChaosHyperchaosSymmetricCoupling, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos-Based Applications with a 6D Memristive System and a 2D SF-SIMM Hyperchaotic Map", "m", "w", "n", "real", "imag", "Colour Factor", ES, ES, ES, ES, 3.0, PI*2, 2.0, 1.0, 1.0, 1000.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-6.4, -3.4, 6.9, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoChaosBasedEngineeringApplications, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos, Control, and Synchronisation in Some Fractional-Order Difference Equations", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.4, 0.2, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-2.4, -1.4, 3.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoChaosControlSynchronisation, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos Synchronisation and Hyperchaos Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.297, 0.2, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.03, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-2.0, -1.1, 2.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 1000000.0,
	NullSetup, DoChaosSynchronisationHyperchaos, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaos Synchronisation in Discrete-Time Systems: the F-M Synchronisation - the 3D Hénon Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.07, 0.3, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.12, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-0.7, -0.35, 0.7, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoChaosSynchronisation3DHenonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.2, 0.6
	},
	{
	"Chaos Synchronisation in Discrete-Time Systems: the F-M Synchronisation - the Fold Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -0.1, -1.7, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.5, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-3.6, -2.0, 4.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoChaosSynchronisationFoldMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chaotic Evolution towards Philosophy and Methodology of Chaotic Optimisation Fractal Map", "a", "b", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, -1.4, 1.6, 2.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -0.2, 1.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaoticEvolutionMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0001, 1.0, 1.0
	},
	{
	"Chaotic Map with No Fixed Points: Entropy, Implementation and Control", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 0.01, 0.1, 2.0, 0.1, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.5, 0.51, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.5, -0.8, 1.5, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChaoticMapNoFixedPoints, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chip - taken from Fractint (plus variations)", "a", "b", "c", "0=cos, 1=sin, 2=atan, 3=tanh", "Colour Factor", ES, ES, ES, ES, ES, -15.0, -19.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -530.0, -300.0, 600.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoChipMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chirikov Standard Fractal Map", "k", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.971635, 1.0, 1.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.5 / 2.0 / PI, 4.5 / 2.0 / PI, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -700000.0, -350000.0, 700000.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoChirikovStandardMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 100000.0, 100000.0
	},
	{
	"Chossat-Golubitsky Map I", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -2.0, 0.0, 2.6, -0.5, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.7, 3.3, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChossatGolubitskyMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chossat-Golubitsky Map II", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 2.0, -0.2, -1.75, 1.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.75, 1.5, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChossatGolubitskyMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chossat-Golubitsky Map III", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 0.1, 1.6, -0.8, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.7, 3.3, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChossatGolubitskyMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chossat-Golubitsky Map IV", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 0.1, 1.52, -0.8, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.7, 3.3, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChossatGolubitskyMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chossat-Golubitsky Map V", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.0, 0.0, -1.9, 0.4, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.7, 3.3, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChossatGolubitskyMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chossat-Golubitsky Map VI", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.0, 0.0, -2.25, 0.2, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.7, 3.3, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChossatGolubitskyMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Chossat-Golubitsky Map VII", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 0.05, 2.275, -0.5, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.7, 3.3, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoChossatGolubitskyMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Classifying and Quantifying Basins of Attraction Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.3, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -5.0, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoClassifyingQuantifyingBasinsAttraction, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Classifying and Quantifying Basins of Attraction - Connected Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.4976, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.15, -0.25, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoClassifyingQuantifyingBasinsAttraction, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Classifying and Quantifying Basins of Attraction - Sine - Sine Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.4976, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.5, 3.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoClassifyingQuantifyingBasinsAttractionSineSine, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clifford Chaotic Fractal Map I", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -1.4, 1.6, 1.0, 0.7, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2,  -2.0,  4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoClifford, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clifford Chaotic Fractal Map II", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.6, -0.6, -1.2, 1.6, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -3.0, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoClifford, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clifford Chaotic Fractal Map III", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.7, 1.7, 0.6, 1.2, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.5, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoClifford, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clifford Chaotic Fractal Map IV", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.5, 1.8, 1.6, 0.9, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoClifford, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clifford Chaotic Fractal Map V", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -1.7, 1.3, -0.1, -1.2, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.5, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoClifford, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clifford Chaotic Fractal Map VI", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -1.7, 1.8, -1.9, -0.4, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoClifford, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clifford Chaotic Fractal Map VII", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -1.8, -2.0, -0.5, -0.9, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoClifford, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clifford Chaotic Fractal Map VIII", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -1.7, 1.8, -1.9, 0.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.3, -2.0, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoClifford, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.5, 1.0
	},
	{
	"Coexistence of Thread and Sheet Chaotic Attractors for Three-Dimensional Lozi Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.8, -1.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.3, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -11.0, 22.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoCoexistenceThreadSheetChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Coexisting Infinite Orbits in an Area-Preserving Lozi Map I", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.2, -1.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -4.0, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoexistingInfiniteOrbits, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Coexisting Infinite Orbits in an Area-Preserving Lozi Map II", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -0.8, -1.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.5, -4.6, 7.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoexistingInfiniteOrbits, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Coexisting Infinite Orbits in an Area-Preserving Lozi Map III", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.75, 0.5, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.1, -1.2, 2.2, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoCoexistingInfiniteOrbits, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Complex Dynamics in Generalised Hénon Map I", "a", "b", "c", "d", "e", "Colour Factor", ES, ES, ES, ES, -1.5, 0.3, 0.8, 0.1, 0.1, 100.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.4, 8.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoComplexDynamicsInGeneralisedHénonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Complex Dynamics in Generalised Hénon Map II", "a", "b", "c", "d", "e", "Colour Factor", ES, ES, ES, ES, 0.5, 0.85, -0.1, -0.04, 0.7, 100.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, 0.2, 3.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoComplexDynamicsInGeneralisedHénonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Complex Dynamics in Generalised Hénon Map III", "a", "b", "c", "d", "e", "Colour Factor", ES, ES, ES, ES, 0.2, 1.0, 0.01, -1.5, 0.23, 100.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.6, 1.8, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoComplexDynamicsInGeneralisedHénonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Complex Dynamics of the Simplest Neuron Model: Singular Chaotic Shilnikov Attractor as Neuron Activity", "a", "b", "i", "c", "Colour Factor", ES, ES, ES, ES, ES, 0.9, 0.2, -1.94, 0.5555, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.7, 2.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.45, 7.26, 1.87, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoComplexDynamicsSimplestNeuronModelMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 4.0, 1.0
	},
	{
	"Complexity and Dynamic Characteristics of a New Discrete-Time Hyperchaotic Model - 2D-HGSM", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 13.7, 0.09, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.14, -0.63, 1.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoComplexityDynamicCharacteristicsMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Constructing Chaotic Map with Multi-Cavity – Sinusoidal Objects", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.0, 0.333333, 100.0, 1.5*PI, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.86, 1.7, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoConstructingChaoticMapMultiCavity, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 2.0, 1.0
	},
	{
	"Controlling Chaos in Maps with Multiple Strange Attractors", "mu", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.9, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.7, 3.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoControllingChaosMapsMultipleStrangeAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},	
	{
	"Controlling Hidden Dynamics and Multistability of a Class of 2D Maps via Linear Augmentation - Eq XII", "b", "d", "e", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.19, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.63, 2.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoControllingHiddenDynamicsMultistabilityXII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Controlling Hidden Dynamics and Multistability of a Class of 2D Maps via Linear Augmentation - Eq XIV", "b", "d", "e", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.25, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.7, -3.3, 6.4, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoControllingHiddenDynamicsMultistabilityXIV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Controlling Hidden Dynamics and Multistability of a Class of 2D Maps via Linear Augmentation - Eq XVI", "b", "d", "e", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.23, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.3, 2.4, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoControllingHiddenDynamicsMultistabilityXVI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cubic Hénon Fractal Map", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.5, -1.8, -2.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.77, 0.77, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0, 
	    NullSetup, DoCubicHenon, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Delayed Logistic Fractal Map", "a", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 2.27, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.3, 1.5, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDelayedLogistic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Design New Chaotic Maps Based on Dimension Expansion I", "a", "w", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.0, PI, 3.0, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.25, -1.25, 2.5, 1, 2, 3, 4, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDesignNewChaoticMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Design New Chaotic Maps Based on Dimension Expansion II", "a", "w", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.0, PI * 6.0 / 5.0, 1.09, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.25, -1.25, 2.5, 1, 2, 3, 4, 5, 5, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDesignNewChaoticMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dual Hénon Fractal Map", "a", "b", "c", "d", "e", "Colour Factor", ES, ES, ES, ES, 2.4, 0.3, 0.1, 3.0, 5.0, 1200.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDualHenonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Duffings Fractal Map 2D", "a", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.5, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -8.0, 16.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDuffings, "SelectOscParams", (DLGPROC)SelectOscParams, PI / 100.0, 1.0, 1.0
	},
	{
	"Duffing Chaotic Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.75, 0.1, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDuffingChaotic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dynamics and Control of the Fractional Forms of Some Rational Chaotic Maps - Chang and Alli Map", "p", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.995, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5010, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -27.0, -10.0, 30.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsControlFractionalFormsChangMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dynamics and Control of the Fractional Forms of Some Rational Chaotic Maps - Rulkov Map", "sigma", "theta", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.001, 0.001, 3.875, 100000, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.6, -4.3, 5.4, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 700000.0,
	    NullSetup, DoDynamicsControlFractionalFormsRulkovMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dynamics and Control of the Fractional Forms of Some Rational Chaotic Maps - Zeraoulia-Sprott Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 6.0, 0.875, 2400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.8, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -38.0, -21.0, 42.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsControlFractionalFormsZeraculiaMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dynamics and Optimisation Control of a Robust Chaotic Map", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.0, 1.98, 1.0, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.15, -2.37, 4.74, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsOptimisationControl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dynamics of a Higher Dimensional Fractional-Order Chaotic Map", "a", "c", "w", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.0, 0.5, PI, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 1.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoDynamicsHigherDimensionalFractionalOrderChaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dynamics of a New Multi-Cavity Hyperchaotic Map and Its DSP Implementation", "a", "phi", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.5, 0.0, 20.0, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.4, 0.8, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoDynamicsNewMultiCavityHyperChaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Dynamical Trapping in the Area-Preserving Hénon Map", "a", "b", "real", "imag", "Colour Factor", ES, ES, ES, ES, ES, 0.218, 0.9989, 1.0, 1.0, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.091, 0.15, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1200000.0,
	    NullSetup, DoDynamicalTrappingAreaPreservingHénonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dynamic Fractal Map based on Fractint I", "a", "b", "Funtion 1: sin() = 0 else cos()", "Funtion 2: sin() = 0 else cos()", "Funtion 3: sin() = 0 else cos()", "Funtion 4: sin() = 0 else cos()", "Reverse x and y", "Colour Factor", 
	    ES, ES, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -102.0, -31.0, 170.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 1200000.0,
	    NullSetup, DoDynamicFractalMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dynamic Fractal Map based on Fractint II", "a", "b", "Funtion 1: sin() = 0 else cos()", "Funtion 2: sin() = 0 else cos()", "Funtion 3: sin() = 0 else cos()", "Funtion 4: sin() = 0 else cos()", "Reverse x and y", "Colour Factor", 
	    ES, ES, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -272.0, -144.0, 334.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 1200000.0,
	    NullSetup, DoDynamicFractalMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Electronically-Implemented Coupled Logistic Maps", "r1", "r2", "e", "real", "imag", "Colour Factor", ES, ES, ES, ES, 2.6, 3.79, 0.963, 1.0, 1.0, 3600.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.28, 0.14, 0.56, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoElectronicallyImplementedCoupledLogisticMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Elhadj - Sprott Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 4.0, 0.3, 14000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.25, -0.05, 0.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoElhadjSprottMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Ergodic Theory of Chaos and Strange Attractors Map", "a", "b1", "b2", "Colour Factor", ES, ES, ES, ES, ES, ES, 3.7, 0.1, 0.15, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, 0.27, 3.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoErgodicTheoryMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Existence and Stability of Chimera States in a Minimal System of Phase Oscillators - as a Map", "r", "beta", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.0, -0.5, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, PI/3, -PI/5, -PI/7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.4, -2.3, 6.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoExistenceStabilityChimeraStatesMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Existence and Stability of Chimera States in a Minimal System of Phase Oscillators - as a 3D Map", 
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.4, -3.0, 6.5, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoExistenceStabilityChimeraStates3DMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.4, 1.2
	},
	{
	"Explore the Potential of Deep Learning and Hyperchaotic Map in the Visual Image Encryption Scheme",
	    "r", "h", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.001, 4.0, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.9, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -1.3, 2.6, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoExplorePotentialDeepLearningMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Fat Hénon Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.4, 0.3, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.7, 1.8, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoFatHenonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 2.6, 1.0
	},
	{
	"Feigenbaum Strange Fractal Map", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 3.277, 0.1,  0.618, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.5, 2.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 10000000.0,
	    NullSetup, DoFeigenbaum, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Four-Dimensional Tinkerbelle Chaotic System for Secure Transmission",
	    "a", "b", "c", "d", "e", ES, ES, ES, ES, ES, 0.9, -0.6, 2.0, 0.5, 3.0, 1000.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "z", ES, ES, ES, ES, ES, ES, 0.1, 0.5, 0.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.6, 2.2, 1, 2, 3, 6, 4, 4, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoFourDimensionalTinkerbelle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Fractal Dream Chaotic Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -0.966918, 2.879879, 0.765145, 0.744728, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -2.0, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoFractalDream, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Fractional Form of a New Three-Dimensional Generalised Hénon Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.7281, 0.5, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.8, 2.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoFractionalFormNew3DGeneralisedHénonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gingerbread Man based on Fractint. Variation 1 - Matryoshka Presentation", "Variation", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.5, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.21, 0.883, 0.24, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGingerbreadManVariations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gingerbread Man based on Fractint. Variation 2 - '1 - y + sin(abs(x))'", "Variation", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -4.6, 11.6, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGingerbreadManVariations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gingerbread Man based on Fractint. Variation 3 - 'One Inverted Signal'", "Variation", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.5, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.2, 13.3, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGingerbreadManVariations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gingerbread Woman Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.0, 2.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.8, -0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.2, -3.4, 12.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoGingerbreadWoman, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 4.0
	},
	{
	"Gumowski-Mira Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 0.0000008, -0.53, 1.0, 1.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -25.0, 50.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoGumowskiMira, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gumowski-Mira Fractal Map - 3 Wings", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -0.48, 0.9924, 1.0, 1.0, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -10.0, 25.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoGumowskiMira3Wings, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gumowski-Mira Fractal Map - 5 Wings", "mu", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, -0.8, 1.0, 1.0, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -27.0, -17.0, 34.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoGumowskiMira5Wings, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hénon Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.4, 0.3, 200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.9, 1.6, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHenonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.6, 1.0
	},
	{
	"Hénon Modified Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.205, 0.305, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.3, 0.15, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -2.33, 2.55, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHenonMarcusMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 5.0, 1.0
	},
	{
	"Homoclinic Tangencies with Infinitely many Asymptotically Stable Single-Round Periodic Solutions Map", "c2", "d5", "lambda", "sigma", "d1", "Colour Factor", ES, ES, ES, ES, -0.5, 1.0, -0.8, -1.25, -1.0, 1000.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.25, 2.45, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHomoclinicTangenciesMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Hopa-Jong Chaotic Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 2.24, 0.65, 0.43, 2.43, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.4, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHopaJong, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperchaotic Attractors of Three-Dimensional Maps and Scenarios of their Appearance I", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, -0.0005, 0.175, -1.52756, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.21, 0.31, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.75, -1.68, 2.6, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperchaoticAttractors3DMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperchaotic Attractors of Three-Dimensional Maps and Scenarios of their Appearance II", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, -0.44, 0.7, -0.44, 150.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.75, -1.68, 2.6, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperchaoticAttractors3DMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperchaotic Attractors of Three-Dimensional Maps and Scenarios of their Appearance III", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.305, 0.445, -1.725, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8, 0.4, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.65, -1.03, 1.6, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoHyperchaoticAttractors3DMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperchaotic Towel Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 3.8, 0.1, 4000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.03, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.33, -0.4, 1.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperchaoticTowelMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 10.0, 1.0
	},
	{
	"Hyper Logistics Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.9, 0.285, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -2.0, 4.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoHyperLogistics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Icons 3D", "alpha", "beta", "gamma", "lambda", "degree", "omega", "Colour Factor", ES, ES, ES, 2.0, 0.2, 0.1, -2.34, 3.0, 0.0, 200.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.003, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoIcons3D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Identification of Chaos in Fractional Chaotic Map (Burger's Map)", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.87, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.7, -2.0, 3.9, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoIdentificationChaosFractionalChaoticMapBurgersMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Ikeda Chaotic Attractor 2D", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.0, 0.9, 0.4, 6.0, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -2.7, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 80000.0,
	    NullSetup, DoIkeda2D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Image Encryption Technology with Fractional 2D Triangle Function Combination Discrete Chaotic Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.9, 0.075, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.75, 3.6, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoImageEncryptionTechnologyFractional2DTriangleFunction, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.5, 1.0
	},
	{
	"Image Encryption using the Two-dimensional Logistic Chaotic Map", "r", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 1.19, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8909, 0.3342, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.46, -0.1, 1.2, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoImageEncryptionUsing2DLogisticChaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Influence of Parametric Symmetry on the Dynamics of 3D Sinusoidal Discrete Systems Map", "a", "b", "c", "d", "m", "Colour Factor", ES, ES, ES, ES, -3.76, 0.9, 0.9, 1.0, 0.01, 800.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8909, 0.3342, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.5, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoInfluenceParametricSymmetryDynamics3DSinusoidalDiscreteSystemsMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Initial-Switched Boosting Bifurcations in 2D Hyperchaotic Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.815, 0.00000000000003, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.8, -1.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-2.6, -3.0, 3.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	NullSetup, DoInitialSwitchedBoostingBifurcations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Jewels or Galaxies Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.0, 2.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -1.8, -0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.2, -2.6, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 2000000.0,
	    NullSetup, DoGingerbreadWoman, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 4.0
	},
	{
	"Kakadu Fractal Map", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.7, 1.2, 0.21, 10000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -0.225, 1.5, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoKakadu, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kaneko-I Attractor", "a", "d", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.3, 2.04, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -0.45, 1.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoKanekoI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kaneko-II Attractor", "a", "d", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.1, 1.23, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.64, -0.22, 1.3, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoKanekoII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Knot Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 34.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lauwerier Attractor", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 3.0, 2.0, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.7, 1.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoLauwerierAttractor, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lorenz Discrete-Time Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.25, 0.75, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.7, -0.21, 5.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 400000.0,
	    NullSetup, DoLorenzDiscreteTimeMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lorenz-2D Chaotic Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 240.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoLorenz2D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.55, 1.0, 1.0
	},
	{
	"Lorenz-3D Chaotic Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoLorenz3D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lorenz-84 Chaotic Fractal Map", "a", "b", "f", "g", "Colour Factor", ES, ES, ES, ES, ES, 1.111, 1.479, 4.494, 0.44, 300.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.6, 8.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoLorenz84, "SelectOscParams", (DLGPROC)SelectOscParams, 0.135, 1.0, 1.0
	},
	{
	"Lorenz and Duffing Like Map", "a", "w", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.8, 0.7, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.4, -3.6, 7.35, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoLorenzDuffingAlikeMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lozi Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.7, 0.5, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -2.0, 4.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoLozi, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"MacMillan Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.6, 0.4, 1.0, 1.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, -0.9, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.5, -7.5, 15.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMacMillan, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Maluquice Craziness", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -8.0, 0.08, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0, -80.0, 160.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 220000.0,
	    NullSetup, DoMaluquice, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 0.1, 1.0
	},
	{
	"Marotto-Lorenz Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.5, 2.05, 8400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -0.2, 0.5, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMarottoLorenz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.1
	},
	{
	"Maynard Smith Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 0.74, 0.75, 1.0, 1.0, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -0.9, 2.4, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMaynardSmith, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Meltzler Map", "h", "a", "b", "Set to 1 for abs(x)", "Set to 1 for abs(y)", "Colour Factor", ES, ES, ES, ES, 0.684, 1.0, 1.0, 0.0, 0.0, 400.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.0, 4.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMeltzlerMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Memristive Rulkov Neuron Model with Magnetic Induction Effects Map", "a", "m", "e", "k", "Modified set to 1.0", "Colour Factor", ES, ES, ES, ES, 0.4, 0.4, 1.0, 0.98, 0.0, 1400.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 3.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -1.8, 5.5, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMemristiveRulkovNeuronModelMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Memristive Rulkov Neuron Model with Magnetic Induction Effects Map - Modified", "a", "m", "e", "k", "Modified set to 1.0", "Colour Factor", ES, ES, ES, ES, 0.4, 0.4, 1.0, 0.98, 1.0, 1400.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 3.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.82, 1.4, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMemristiveRulkovNeuronModelMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Mira Map 05",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.000999, 0.49859, -0.95, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.5, -2.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-70.0, -40.0, 80.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoAreMathematicsSourceArtisticCreationPropertiesMap451, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 2.0, 1.0
	},
	{
	"Mira Map 08",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0083, 0.1, -0.38, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-12.0, -7.5, 15.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap08, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Mira Map 09",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0083, 0.05, -0.8, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-32.0, -16.0, 32.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap09, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Mira Map 12",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.008, 0.45, -0.02496, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-7.9, -3.6, 8.3, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap12, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Mira Map 13",
	"a", "s", "mu", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.008, 0.1, -0.496, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-10.0, -6.0, 12.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap13, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Mira Map 15",
	"a", "s", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.008, 0.1, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-10.0, -6.0, 12.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap15, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Mira Map 16",
	"a", "s", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.008, 0.1, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-10.0, -6.0, 12.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap16, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 2.0
	},
	{
	"Mira Map 19",
	"a", "s", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.00058, 0.5, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.2, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-10.0, -4.5, 10.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap19, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Mira Map 21",
	"a", "s", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.0025, 1.5, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.052, -0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-6.0, -3.5, 7, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap21, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Mira Map 23",
	"a", "s", "m", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.008, 0.05, -0.6, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-20.0, -13.5, 27.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap23, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 2.0
	},
	{
	"Mira Map 24",
	"a", "s", "m", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0105, 0.043, -0.9552, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-8.0, -4.5, 10.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap24, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 2.0
	},
	{
	"Mira Map 25",
	"a", "s", "m", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0005, 1.0, -0.42, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-10.8, -6.0, 12.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap25, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 2.0
	},
	{
	"Mira Map 26",
	"a", "s", "m", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.008, 0.051, 0.16, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.5, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-17.0, -10.0, 20.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap26, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 2.0
	},
	{
	"Mira Map 29",
	"a", "s", "m", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0083, 0.1, -0.38, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-12.0, -8.0, 16.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap29, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 2.0
	},
	{
	"Mira Map 30",
	"a", "s", "m", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.00051, 0.2, -0.00045, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 2.0, 0.6, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-12.0, -8.0, 16.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMap30, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.8, 2.0
	},
	{
	"Mira Map X",
	"a", "s", "m", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.008, 0.05, -0.9, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-40.0, -24.0, 48.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoMiraMapX, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.4, 2.0
	},
	{
	"Metz Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 4.15, 10.0, 1.0, 1.0, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -40.0, 250.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoMetz, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.2, 1.0
	},
	{
	"Mira-Gumowski Fractal Map", "a", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, -0.6624, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 14.7176, -14.6602, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -34.0, 73.0, 1, 2, 3, 2, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoMiraGumowski, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Modifizierte Mira Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 0.15, 1.0, 100.0, 100.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -2.0, 3.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -900.0, -500.0, 1000.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoModifizierteMira, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Multicavity Formations and Complexity Modulation in a Hyperchaotic Discrete System", 
	    "w", "k", "a", "b", "n", "real", "imag", "Colour Factor", ES, ES, PI, 10.0, 10.0, 15.0, 3.0, 0.1, 0.1, 400.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -9.0, 20.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMulticavityFormations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.1, 1.0
	},
	{
	"Multikey Image Encryption Algorithm Based on a High-Complexity Hyperchaotic System",
	    "mu1", "mu2", "real", "imag", "Colour Factor", ES, ES, ES, ES, ES, 7.5, 17.0, 1.0, 1.0, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.5, -3.5, 7.0, 1, 2, 3, 5, 3, 2, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultikeyImageEncryptionAlgorithm, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.005, 1.0
	},
	{
	"Multistability and Cyclic Attractors in Duopoly Games",
	    "mu1", "mu2", "real", "imag", "Colour Factor", ES, ES, ES, ES, ES, 3.87, 3.87, 1.0, 1.0, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.51, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.33, 0.06, 1.0, 1, 2, 3, 5, 3, 2, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoMultistabilityCyclicAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"New Algorithm of Encryption and Decryption of Images Using Chaotic Mapping",
	    "m1", "m2", "c1", "c2", "real", "imag", "Colour Factor", ES, ES, ES, 2.93, 3.17, 0.197, 0.139, 3.0, 3.0, 1200.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0215, 0.5734, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.2, 0.15, 0.9, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNewAlgorithmEncryptionDecryptionMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Novel Two Dimensional Fractional-Order Discrete Chaotic Map and its Application to Image Encryption", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.4, 6.0, 100.0, 100.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoNovel2DFractionalOrderDiscreteChaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.1, 1.0
	},
	{
	"On a Family of Maps with Multiple Chaotic Attractors", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 9.0, 0.81, 1.0, 1.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -10.0, 20.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 300000.0,
	    NullSetup, DoOnFamilyMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"On the Dynamics, Control and Synchronisation of Fractional-Order Ikeda Map I",
	    "delta", "real", "imag", "Colour Factor", "type", ES, ES, ES, ES, ES, 0.78, 1.0, 1.0, 800.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -1.2, 2.4, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoOnDynamicsControlSynchronisationFractionalOrderIkedaMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"On the Dynamics, Control and Synchronisation of Fractional-Order Ikeda Map II",
	    "delta", "real", "imag", "Colour Factor", "type", ES, ES, ES, ES, ES, -0.7, 1.0, 1.0, 800.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -1.7, 2.6, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoOnDynamicsControlSynchronisationFractionalOrderIkedaMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"On the Three-Dimensional Fractional-Order Henon Map with Lorenz-Like Attractors",
	"b", "m1", "m2", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.688888, 0.000000005, 0.8778, 1200.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-0.86, -0.36, 1.4, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoOn3DFractionalOrderHenonMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Parameter Identification for Discrete Memristive Chaotic Map using Adaptive Differential Evolution Algorithm",
	"k", "e", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.9575, 0.1, 1200.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	"x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.185, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	-3.03, -1.76, 3.47, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	NullSetup, DoParameterIdentificationMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Peter de Jong Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 2.0, 2.0, 10.0, 2.0, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.8, -2.25, 4.5, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoPeterDeJong, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Pi Observer Design for a Class of Nondifferentially Flat Systems Map", "g", "s", "a", "u", "Colour Factor", ES, ES, ES, ES, ES, 1.63, 50.0, 200.0, 1.0, 400.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -700.0, 0.0, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -74.0, 8.8, 212.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoPiObserverDesign, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.1
	},
	{
	"Pickover Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, -0.75, 2.44, 1.25, 1.5, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.4, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -1.2, 1.5, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPickover, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Polynom Fractal Map Type A", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.586, 1.124, 0.281, 300.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.8, 2.5, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPolynomA, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Polynom Fractal Map Type B", "a", "b", "c", "d", "e", "f", "Colour Factor", ES, ES, ES, -0.237, 0.453, 0.486, 0.616, 1.141, 0.483, 300.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.4, 5.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPolynomB, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Popcorn 2 Fractal Map", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.05, 3.0, 1400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.6, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -3.8, 20.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoPopcorn2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.03, 1.0
	},
	{
	"Quadruple Two Fractal Map", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 34.0, 0.9, 5.0, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -28.0, 100.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 10000.0,
	    NullSetup, DoQuadruple2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Quadruple Two Fractal Map (Higher Iterations)", "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 34.0, 0.9, 5.0, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2700.0, -1500.0, 3000.0, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoQuadruple2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Quadruptwo Map from Fractint (plus variations)", "a", "b", "c", "0=sin, 1=sinh, 2=exp, 3=tan", "Colour Factor", ES, ES, ES, ES, ES, 34.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -93.0, -42.0, 128.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 150000.0,
	    NullSetup, DoQuadruple2Fractint, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Robust Chaos of Cubic Polynomial Discrete Maps with Application to Pseudorandom Number Generators", "ni", "p2", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 5.7, -3.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.35, -1.1, 2.8, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoRobustChaosCubicPolynomialDiscreteMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.3, 0.3
	},
	{
	"Robust Compression-Encryption of Images Based on SPIHT Coding and Discrete-Time Chaotic System", "alpha1", "alpha2", "alpha3", "a", "b", "Colour Factor", ES, ES, ES, ES, 0.97, 0.97, 0.97, 1.6, 0.1, 100.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 3.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoRobustCompressionEncryptionImagesBasedSPIHTCoding, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Route to Chaos Via Strange Non-Chaotic Attractors by Reshaping Periodic Excitations", "a", "b", "m", "w", "Colour Factor", ES, ES, ES, ES, ES, 2.1, 0.5, 0.93, (sqrt(5.0) - 1.0) / 2.0, 100.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 2.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -1.0, 7.5, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoRouteChaosViaStrangeNonChaoticAttractors, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"SF-SIMM High-Dimensional Hyperchaotic Map and its Performance Analysis", 
	    "a", "w", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 2.0, PI, 0.5, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 13.0 / 19.0 * PI, 17.0 / 23.0 * PI, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.3, 4.6, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSFSIMMHighDimensionalHyperchaoticMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Shadowing in Hidden Attractors - Equation IV",
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.55, 1.2, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoShadowingHiddenAttractorsEquationIV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Shadowing in Hidden Attractors - Equation V",
	    "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -0.32, 1.6, 1, 2, 3, 1, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoShadowingHiddenAttractorsEquationV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sierpinski Polygon Fractal Map", "Scale Factor", "Number of Sides", "Real Z angle", "Imaginary Z angle", "Colour Factor", ES, ES, ES, ES, ES, 0.48, 3.0, 1.3, 1.5, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.5, 3.0, 1, 2, 3, 5, 3, 0, NULL, 0, YAXIS, 1000000.0,
	    NullSetup, DoSierpinskiPoly, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Some Open Problems in Chaos Theory and Dynamics - Equation 2", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.7, 0.5, 2400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.1, -4.2, 20.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSomeOpenProblemsChaosTheory2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Some Open Problems in Chaos Theory and Dynamics - Equation 3", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -7.0, -0.75, 2400.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.4, -9.2, 24.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoSomeOpenProblemsChaosTheory3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.5, 1.0
	},
	{
	"Square Hénon Fractal Map", "a", "Points per Orbit", "Step Size", "b", "c", "Colour Factor", ES, ES, ES, ES, 1.524, 5000, 0.005, 1.0, 1.0, 1000.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.098, 0.061, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -1.5, 3.0, 1, 2, 3, 6, 3, 3, NULL, 0, YAXIS, 10000.0,
	    NullSetup, DoSquareHenon, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Strick Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 0.5, 1.1, 1.3, 1.5, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -1.5, 3.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoStrick, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Svensson Attractor Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 1.4, 1.4, 1.4, 1.4, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoSvensson, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Swirl I from The Pattern Book by Clifford A. Pickover Fractal Map", 
	    "a11", "a12", "a21", "a22", "real", "imag", "Colour Factor", ES, ES, ES, -2.7, -0.08, -0.86, -2.2, 1.0, 1.0, 350.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -0.7, 2.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSwirlPickover, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Swirl II from The Pattern Book by Clifford A. Pickover Fractal Map", 
	    "a11", "a12", "a21", "a22", "real", "imag", "Colour Factor", ES, ES, ES, -2.24, 0.43, -0.65, -2.43, 1.0, 1.0, 350.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -0.7, 2.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSwirlPickover, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Swirl III from The Pattern Book by Clifford A. Pickover Fractal Map", 
	    "a11", "a12", "a21", "a22", "real", "imag", "Colour Factor", ES, ES, ES, 2.0, -1.0, -1.0, -2.4, 1.0, 1.0, 350.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.2, 2.6, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSwirlPickover, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Swirl IV from The Pattern Book by Clifford A. Pickover Fractal Map", 
	    "a11", "a12", "a21", "a22", "real", "imag", "Colour Factor", ES, ES, ES, 0.44, -1.22, 2.5, -1.5, 1.0, 1.0, 350.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.4, 1.8, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSwirlPickover, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Symmetric Key Encryption Based on Rotation-Translation Equation Map",
	    "a", "b", "real", "imag", "Colour Factor", ES, ES, ES, ES, ES, 0.5, 0.98, 1.0, 1.0, 350.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 4.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.68, -0.51, 2.25, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 200000.0,
	    NullSetup, DoSymmetricKeyEncryption, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.7, 1.0
	},
	{
	"Synchronisation of Chaotic Systems Fractal Map", 
	    "a", "b", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, 3.9, 1.1, 1.4, 3500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, -0.5, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.4, -0.1, 1.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 120000.0,
	    NullSetup, DoSynchronisationChaoticSystemsMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"The Simplest Case of a Strange Attractor Map",
	    "Density", "Number of Arms", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.5, 7.0, 1600.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 8.0, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -920.0, -520.0, 1100.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 500000.0,
	    NullSetup, DoTheSimplestCaseStrangeAttractorsMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Three-Dimensional Hénon-Like Maps and Wild Lorenz-Like Attractors", "m1", "m2", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, 1.4, 0.2, 0.1, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 4, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoThreeDimensionalHenonLikeMaps, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"ThreePly - taken from Fractint (plus variations)", "a", "b", "c", "Map Type (0 - 4)", "Colour Factor", ES, ES, ES, ES, ES, -55.0, -1.0, -42.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5500.0, -3200.0, 6160.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 100000.0,
	    NullSetup, DoThreePlyMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tinkerbell Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 0.9, -0.6013, 2.0, 0.5, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.72, -0.64, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 3.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoTinkerbell, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Transformations of Spike and Burst Oscillations in the Stochastic Rulkov Model", "s", "a", "b", "c", "start", "Colour Factor", ES, ES, ES, ES, 0.005, 3.0, 1.0, 1.0, 500.0, 5000000.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -3.4, 4.2, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoTransformationsSpikeBurstOscillations, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 2.0, 1.0
	},
	{
	"Two-Variable Boosting Bifurcation in a Hyperchaotic Map and Its Hardware Implementation I", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.03, 2.83, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.1, -2.93, 5.88, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 700000.0,
	    NullSetup, DoTwoVariableBoostingBifurcationMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Two-Variable Boosting Bifurcation in a Hyperchaotic Map and Its Hardware Implementation II", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.0, 2.55, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.45, -3.0, 5.9, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 700000.0,
	    NullSetup, DoTwoVariableBoostingBifurcationMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Two-Variable Boosting Bifurcation in a Hyperchaotic Map and Its Hardware Implementation III", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.0, 2.75, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.65, -2.84, 5.74, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 700000.0,
	    NullSetup, DoTwoVariableBoostingBifurcationMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Two-Variable Boosting Bifurcation in a Hyperchaotic Map and Its Hardware Implementation IV", "a", "b", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.8, 2.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.52, -2.94, 5.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 700000.0,
	    NullSetup, DoTwoVariableBoostingBifurcationMap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Ushiki Fractal Map", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, ES, 3.7, 0.1, 3.7, 0.15, 1500.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, 0.4, 3.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoUshiki, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Variety of Strange Pseudohyperbolic Attractors in Three-Dimensional Generalized Hénon Maps I", "a", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -1.86, 0.03, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.4, 1.9, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoVarietyStrangePseudohyperbolicAttractorsI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Variety of Strange Pseudohyperbolic Attractors in Three-Dimensional Generalized Hénon Maps II", "a", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.82, 2.06, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoVarietyStrangePseudohyperbolicAttractorsII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Variety of Strange Pseudohyperbolic Attractors in Three-Dimensional Generalized Hénon Maps III", "a", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, -1.86, 0.03, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 1.8, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoVarietyStrangePseudohyperbolicAttractorsIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Variety of Strange Pseudohyperbolic Attractors in Three-Dimensional Generalized Hénon Maps IV", "a", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 0.13, 1.72, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.8, 3.8, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoVarietyStrangePseudohyperbolicAttractorsIV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Variety of Strange Pseudohyperbolic Attractors in Three-Dimensional Generalized Hénon Maps V", "a", "c", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 2.13, -1.296, 5000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.5, 1.0, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 50000.0,
	    NullSetup, DoVarietyStrangePseudohyperbolicAttractorsV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Wu-Yang Fractal Map", "r", "a", "b", "c", "d", "Colour Factor", ES, ES, ES, ES, 1.19, 0.9, -0.6013, 2.0, 0.5, 500.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.891, 0.334, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.1, 1.2, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 1000000.0,
	    NullSetup, DoWuYang, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
//////////////////////////////////////////////////////////////////
// database of Sprott Fractal Maps
//////////////////////////////////////////////////////////////////


struct	OscillatorSpecificStuff	SprottMapSpecific[] = 
    {
	// Sprott Map Type E
	{
	"E AEUBNVIAHERQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.9, -1.4, 1.8, 1, 2, 3, 1, 3, 3, "AEUBNVIAHERQ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E AGHNFODVNJCP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.6, -3.0, 3.8, 1, 2, 3, 1, 3, 3, "AGHNFODVNJCP", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E AHSVIGTJKOTB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -0.3, 1.8, 1, 2, 3, 1, 3, 3, "AHSVIGTJKOTB", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E AMTMNQQXUYGA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.5, 2.4, 1, 2, 3, 1, 3, 3, "AMTMNQQXUYGA", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E BCQAFMFVPXKQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.35, 2.1, 1, 2, 3, 1, 3, 3, "BCQAFMFVPXKQ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E BDNOAXZJNRSG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -0.4, 2.8, 1, 2, 3, 1, 3, 3, "BDNOAXZJNRSG", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E CDJXIYLSYQUM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.7, 1.6, 1, 2, 3, 1, 3, 3, "CDJXIYLSYQUM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E CMMMEWHXRMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.3, 2.8, 1, 2, 3, 1, 3, 3, "CMMMEWHXRMMM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E CSRKVVQLGFFS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.75, 1.7, 1, 2, 3, 1, 3, 3, "CSRKVVQLGFFS", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E CVQKGHQTPHTE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.2, -1.1, 3.0, 1, 2, 3, 1, 3, 3, "CVQKGHQTPHTE", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E DFLQJGDGMSJV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -1.1, 3.0, 1, 2, 3, 1, 3, 3, "DFLQJGDGMSJV", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E DSYUECINGQNV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.85, 1.0, 1, 2, 3, 1, 3, 3, "DSYUECINGQNV", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E DSZHYZHEKUNJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.1, 1.8, 1, 2, 3, 1, 3, 3, "DSZHYZHEKUNJ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E ELXAPXMPQOBT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -0.7, 2.3, 1, 2, 3, 1, 3, 3, "ELXAPXMPQOBT", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E ENWUQSLHYSAT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -0.7, 2.3, 1, 2, 3, 1, 3, 3, "ENWUQSLHYSAT", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E EURCEQVLRNSF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -0.2, 2.2, 1, 2, 3, 1, 3, 3, "EURCEQVLRNSF", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E EYYMKTUMXUVC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, 0.5, 0.7, 1, 2, 3, 1, 3, 3, "EYYMKTUMXUVC", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E FFRXAXMGLFNI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -2.3, 3.2, 1, 2, 3, 1, 3, 3, "FFRXAXMGLFNI", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E FHUPPBRKIWHO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.3, 1.2, 1, 2, 3, 1, 3, 3, "FHUPPBRKIWHO", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E FIRCDERRPVLD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.75, 1.5, 1, 2, 3, 1, 3, 3, "FIRCDERRPVLD", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E FMMMYWCFUMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.2, 1, 2, 3, 1, 3, 3, "FMMMYWCFUMMM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E FOKRIROFDMPQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.2, 1.7, 1, 2, 3, 1, 3, 3, "FOKRIROFDMPQ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E GIIETPIQRRUL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -0.9, 1.9, 1, 2, 3, 1, 3, 3, "GIIETPIQRRUL", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E GITIKLJNSKAT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -0.9, 2.5, 1, 2, 3, 1, 3, 3, "GITIKLJNSKAT", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E GLXOESFTTPSV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.1, 2.0, 1, 2, 3, 1, 3, 3, "GLXOESFTTPSV", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E GOSXRBRCBSPM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.65, -0.36, 1.2, 1, 2, 3, 1, 3, 3, "GOSXRBRCBSPM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E GXQSNSKEECTX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.4, 1.7, 1, 2, 3, 1, 3, 3, "GXQSNSKEECTX", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E HGUHDPHNSGOH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -1.0, 0.9, 1, 2, 3, 1, 3, 3, "HGUHDPHNSGOH", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E HNHBMWMSVEPL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.24, -0.15, 1.0, 1, 2, 3, 1, 3, 3, "HNHBMWMSVEPL", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E HXJCQMYLONDK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.4, -1.66, 4.5, 1, 2, 3, 1, 3, 3, "HXJCQMYLONDK", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E IFLJUWMAICIB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.25, -0.57, 1.23, 1, 2, 3, 1, 3, 3, "IFLJUWMAICIB", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E IFVQJFOOLVDV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.25, -0.634, 1.55, 1, 2, 3, 1, 3, 3, "IFVQJFOOLVDV", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E IJYRRWOBTMEN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.36, -0.65, 1.622, 1, 2, 3, 1, 3, 3, "IJYRRWOBTMEN", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E ILIBVPKJWGRR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.25, 1.0, 1, 2, 3, 1, 3, 3, "ILIBVPKJWGRR", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E IXOFHUZSBQHK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -0.72, 2.4, 1, 2, 3, 1, 3, 3, "IXOFHUZSBQHK", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E JETCOHRSIQFN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.5, 3.0, 1, 2, 3, 1, 3, 3, "JETCOHRSIQFN", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E JOYHUWIVDACF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -1.12, 0.93, 1, 2, 3, 1, 3, 3, "JOYHUWIVDACF", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E JTTSMBOGLLQF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -0.8, 2.0, 1, 2, 3, 1, 3, 3, "JTTSMBOGLLQF", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E JXAICXIXFRHI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -1.27, 1.5, 1, 2, 3, 1, 3, 3, "JXAICXIXFRHI", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E KCBIUIPLETRR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.63, -0.62, 1.65, 1, 2, 3, 1, 3, 3, "KCBIUIPLETRR", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E KJXKXKKDYTLK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.27, -0.59, 1.53, 1, 2, 3, 1, 3, 3, "KJXKXKKDYTLK", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E KLVEVAOSGYJX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.43, -2.2, 3.0, 1, 2, 3, 1, 3, 3, "KLVEVAOSGYJX", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E KMMMYSUKEMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.12, -0.66, 1.6, 1, 2, 3, 1, 3, 3, "KMMMYSUKEMMM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E KPNERVOTBYCM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.2, 2.2, 1, 2, 3, 1, 3, 3, "KPNERVOTBYCM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E LIRZLTCPNHOX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -1.25, 1.9, 1, 2, 3, 1, 3, 3, "LIRZLTCPNHOX", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E LLNJNEAMPLDX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -1.4, 3.3, 1, 2, 3, 1, 3, 3, "LLNJNEAMPLDX", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E LRWEFKFHUBHS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.6, 3.2, 1, 2, 3, 1, 3, 3, "LRWEFKFHUBHS", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E LUFBBFISGJYS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -1.7, 1.5, 1, 2, 3, 1, 3, 3, "LUFBBFISGJYS", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MCRBIPOPHTBN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -0.46, 1.2, 1, 2, 3, 1, 3, 3, "MCRBIPOPHTBN", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MDVAIDOYHYEA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.2, 2.0, 1, 2, 3, 1, 3, 3, "MDVAIDOYHYEA", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MFPGVXTIIDKB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.78, -1.47, 2.45, 1, 2, 3, 1, 3, 3, "MFPGVXTIIDKB", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MJDSFTVHGEEV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.16, 0.0, 1.5, 1, 2, 3, 1, 3, 3, "MJDSFTVHGEEV", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MLDRMQYIQWQD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -0.46, 2.0, 1, 2, 3, 1, 3, 3, "MLDRMQYIQWQD", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MQPUKNVAGCBE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -3.24, 4.93, 1, 2, 3, 1, 3, 3, "MQPUKNVAGCBE", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MTGETXEJWCUR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.85, -1.16, 1.6, 1, 2, 3, 1, 3, 3, "MTGETXEJWCUR", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MTQIBOXSCMRC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.14, -0.852, 2.36, 1, 2, 3, 1, 3, 3, "MTQIBOXSCMRC", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E MVWMGCMaMaRM Sprott Fractal Map (Tinkerbel Map)", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.41, -1.66, 2.46, 1, 2, 3, 1, 3, 3, "MVWMGCMaMaRM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E NDVDPLVKBXEF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.54, 1, 2, 3, 1, 3, 3, "NDVDPLVKBXEF", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E NIDATWFTPOSL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.4, -1.85, 4.0, 1, 2, 3, 1, 3, 3, "NIDATWFTPOSL", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E NNMJRCTVVTYG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, 0.44, 1.1, 1, 2, 3, 1, 3, 3, "NNMJRCTVVTYG", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E ODGQCNXODNYA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.17, 1.7, 1, 2, 3, 1, 3, 3, "ODGQCNXODNYA", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E ODSTPMSDFIAO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.48, -2.54, 7.0, 1, 2, 3, 1, 3, 3, "ODSTPMSDFIAO", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E OOHVSVPDBGXW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.68, -0.7, 1.1, 1, 2, 3, 1, 3, 3, "OOHVSVPDBGXW", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E OUGFJKDHSAJU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.5, 2.6, 1, 2, 3, 1, 3, 3, "OUGFJKDHSAJU", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E PKBNVOONOTTC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.54, -0.55, 1.6, 1, 2, 3, 1, 3, 3, "PKBNVOONOTTC", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E PLKQNGALTVDD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.4, 3.0, 1, 2, 3, 1, 3, 3, "PLKQNGALTVDD", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E QEGJUASEDNUJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.84, -0.374, 1.74, 1, 2, 3, 1, 3, 3, "QEGJUASEDNUJ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E QFFVSLMJJGCR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.7, 1.5, 1, 2, 3, 1, 3, 3, "QFFVSLMJJGCR", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E QHVHQHYTEYQA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.89, -0.723, 2.54, 1, 2, 3, 1, 3, 3, "QHVHQHYTEYQA", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E QKOCSIDVTPGY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.2, 3.0, 1, 2, 3, 1, 3, 3, "QKOCSIDVTPGY", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E QLOIARXYGHAJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.9, 2.4, 1, 2, 3, 1, 3, 3, "QLOIARXYGHAJ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E QVEUTIPLADHO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.77, 1.6, 1, 2, 3, 1, 3, 3, "QVEUTIPLADHO", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E QVHVRXREMJED Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -1.7, 3.2, 1, 2, 3, 1, 3, 3, "QVHVRXREMJED", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E RKKCUNHERKAV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -1.5, 3.8, 1, 2, 3, 1, 3, 3, "RKKCUNHERKAV", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E RLKHGBBDLIKJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.1, -1.87, 3.58, 1, 2, 3, 1, 3, 3, "RLKHGBBDLIKJ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E RMMMQEASYMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.35, 1.5, 1, 2, 3, 1, 3, 3, "RMMMQEASYMMM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E SHKBEWJFUOPJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.8, 1.8, 1, 2, 3, 1, 3, 3, "SHKBEWJFUOPJ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E SOKMLEVUMKDW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -0.6, 2.3, 1, 2, 3, 1, 3, 3, "SOKMLEVUMKDW", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E TAPDHJKMTUBD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.73, -0.82, 1.3, 1, 2, 3, 1, 3, 3, "TAPDHJKMTUBD", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E TJUBWEDNRORR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -1.2, 1.3, 1, 2, 3, 1, 3, 3, "TJUBWEDNRORR", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E TSILUNDQSIFA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -1.7, 2.5, 1, 2, 3, 1, 3, 3, "TSILUNDQSIFA", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E UATWVBSHJIWR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.55, -0.376, 1.28, 1, 2, 3, 1, 3, 3, "UATWVBSHJIWR", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E UEBJLCDISIIQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.56, -0.374, 1.74, 1, 2, 3, 1, 3, 3, "UEBJLCDISIIQ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E UFLXKIETROOO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.4, 2.3, 1, 2, 3, 1, 3, 3, "UFLXKIETROOO", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E UWACXDQIGKHF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -1.1, 1.8, 1, 2, 3, 1, 3, 3, "UWACXDQIGKHF", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E VAVMXOETHDMQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.2, 0.98, 1, 2, 3, 1, 3, 3, "VAVMXOETHDMQ", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E VBUQHNYPGJDF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.3, -3.1, 5.2, 1, 2, 3, 1, 3, 3, "VBUQHNYPGJDF", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E VBWNBDELYHUL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -1.3, 1.8, 1, 2, 3, 1, 3, 3, "VBWNBDELYHUL", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E VDUOTLRBKTJD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.9, -1.66, 2.4, 1, 2, 3, 1, 3, 3, "VDUOTLRBKTJD", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E VHEQLLDMMBFP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.2, 7.2, 1, 2, 3, 1, 3, 3, "VHEQLLDMMBFP", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E WLKWPSMOGIGS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.7, 1.3, 1, 2, 3, 1, 3, 3, "WLKWPSMOGIGS", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E WM?MPMMWMMMM Sprott Fractal Map (Hénon map)", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.4, 2.9, 1, 2, 3, 1, 3, 3, "WM?MPMMWMMMM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E WNCSLFLGIHGL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -1.3, 1.8, 1, 2, 3, 1, 3, 3, "WNCSLFLGIHGL", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E WQKCSBRBQDJX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.8, 1.96, 1, 2, 3, 1, 3, 3, "WQKCSBRBQDJX", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E WQLIJJHEXMPP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.79, -0.84, 2.7, 1, 2, 3, 1, 3, 3, "WQLIJJHEXMPP", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E WRAHGMIKMCHF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.256, -0.7, 0.96, 1, 2, 3, 1, 3, 3, "WRAHGMIKMCHF", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E WXHJEGNRHQFP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.63, -1.32, 2.66, 1, 2, 3, 1, 3, 3, "WXHJEGNRHQFP", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E XJNXAIFANNEN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -2.2, 2.8, 1, 2, 3, 1, 3, 3, "XJNXAIFANNEN", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E XXFGRHFTPDWD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.87, 2.0, 1, 2, 3, 1, 3, 3, "XXFGRHFTPDWD", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E ZMCQGIXPJMJB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.9, -1.1, 2.4, 1, 2, 3, 1, 3, 3, "ZMCQGIXPJMJB", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"E ZPMSGCNFRENG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.7, 3.0, 1, 2, 3, 1, 3, 3, "ZPMSGCNFRENG", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapE, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type F
	{
	"F BOJESOSHMVVWMHJMQIIQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.96, -0.55, 1.44, 1, 2, 3, 1, 3, 3, "BOJESOSHMVVWMHJMQIIQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F EROJRKQNOWMUXJOIYLIM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -0.64, 1.63, 1, 2, 3, 1, 3, 3, "EROJRKQNOWMUXJOIYLIM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F FEWXYQONQQJNVELRTBPS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.29, -1.1, 2.37, 1, 2, 3, 1, 3, 3, "FEWXYQONQQJNVELRTBPS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F HGVGMHYSSJSHFWIBTSUA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.85, 0.17, 1.1, 1, 2, 3, 1, 3, 3, "HGVGMHYSSJSHFWIBTSUA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F HHOLCTHYWJITRPCYLEHO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.74, -1.4, 1.69, 1, 2, 3, 1, 3, 3, "HHOLCTHYWJITRPCYLEHO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F HWHHMFMKFBEKIKSXLMYS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -0.7, 0.58, 1, 2, 3, 1, 3, 3, "HWHHMFMKFBEKIKSXLMYS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
 	{
	"F IRPGVTFIDGCSXMFPKIDJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -1.45, 1.6, 1, 2, 3, 1, 3, 3, "IRPGVTFIDGCSXMFPKIDJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F ISMHQCHPDFKFBKEALIFB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.1, 1.8, 1, 2, 3, 1, 3, 3, "ISMHQCHPDFKFBKEALIFD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F IWOJAIIXSPLVYKSUHUEW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -0.44, 1.0, 1, 2, 3, 1, 3, 3, "IWOJAIIXSPLVYKSUHUEW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F JMANGGKMZMNRBNKCJGCK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.69, 0.95, 1, 2, 3, 1, 3, 3, "JMANGGKMZMNRBNKCJGCK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F JYCBMNFNYOEPYUGHHESU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 1.45, 1, 2, 3, 1, 3, 3, "JYCBMNFNYOEPYUGHHESU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F KAWYMKAEUVRNBGXWUFKH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.33, 1.1, 1, 2, 3, 1, 3, 3, "KAWYMKAEUVRNBGXWUFKH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F KDPVHZGDBDFPTCQHFLXL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.71, -0.78, 0.93, 1, 2, 3, 1, 3, 3, "KDPVHZGDBDFPTCQHFLXL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F KMIABSKYNEQUXZZHSMPC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.77, 2.05, 1, 2, 3, 1, 3, 3, "KMIABSKYNEQUXZZHSMPC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F LAJYVHGVIHGBPQWXYSRJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.11, -1.32, 2.12, 1, 2, 3, 1, 3, 3, "LAJYVHGVIHGBPQWXYSRJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F LGROKJFELDGKXSUEEWYE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.55, -0.82, 0.95, 1, 2, 3, 1, 3, 3, "LGROKJFELDGKXSUEEWYE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F LQOBBRSWKDRNYRQIRKDG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.75, 1.7, 1, 2, 3, 1, 3, 3, "LQOBBRSWKDRNYRQIRKDG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F LUCBPVBOXRJKOFMUFDCN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.1, 1.7, 1, 2, 3, 1, 3, 3, "LUCBPVBOXRJKOFMUFDCN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F LUUCTCPDSRYFJWBQOCYJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.29, 0.66, 0.53, 1, 2, 3, 1, 3, 3, "LUUCTCPDSRYFJWBQOCYJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F LWAQNQVNQOOHIQQMBWSC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.58, -0.172, 1.27, 1, 2, 3, 1, 3, 3, "LWAQNQVNQOOHIQQMBWSC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F LWIAFTKMWQJDIUCXKEKU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.36, -0.765, 1.45, 1, 2, 3, 1, 3, 3, "LWIAFTKMWQJDIUCXKEKU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F MDEEAVWKUXTHEYOHOICN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.557, 0.03, 0.57, 1, 2, 3, 1, 3, 3, "MDEEAVWKUXTHEYOHOICN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F MEGVTLMEQRFCSSTQOYRH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.9, -1.9, 3.6, 1, 2, 3, 1, 3, 3, "MEGVTLMEQRFCSSTQOYRH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F MGGNDPHWONKFQUIIHBVP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.56, 1.2, 1, 2, 3, 1, 3, 3, "MGGNDPHWONKFQUIIHBVP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F MKWIQQVZNEBRWNCJTLON Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.677, -1.29, 0.456, 1, 2, 3, 1, 3, 3, "MKWIQQVZNEBRWNCJTLON", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F NFIVJTOSUSTJWCJAYCLM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.431, -0.154, 0.96, 1, 2, 3, 1, 3, 3, "NFIVJTOSUSTJWCJAYCLM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F NHWLFXVXQGPFILBICUJB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.79, -0.62, 1.16, 1, 2, 3, 1, 3, 3, "NHWLFXVXQGPFILBICUJB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F NHZBEETDORVLAOTUPENH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.02, 0.9, 1, 2, 3, 1, 3, 3, "NHZBEETDORVLAOTUPENH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F NMMMMMMMFUFYXMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.13, 1.24, 1, 2, 3, 1, 3, 3, "NMMMMMMMFUFYXMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F NOYESLFRWEWERNDUKOCI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.95, -0.8, 2.0, 1, 2, 3, 1, 3, 3, "NOYESLFRWEWERNDUKOCI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F NUYLCURDUHQUQMRZQWQB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.02, 1.4, 1, 2, 3, 1, 3, 3, "NUYLCURDUHQUQMRZQWQB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F ODKJNNRHWCSUAJTUGHBP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.18, -0.57, 1.4, 1, 2, 3, 1, 3, 3, "ODKJNNRHWCSUAJTUGHBP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F OGUKSYQYMBLEUWIHRKTQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.47, -0.345, 0.712, 1, 2, 3, 1, 3, 3, "OGUKSYQYMBLEUWIHRKTQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F OMMMMMMMKCXIDMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.25, -0.66, 2.0, 1, 2, 3, 1, 3, 3, "OMMMMMMMKCXIDMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F OVFKWKEIBPGNYPVKWCYU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.85, 1.5, 1, 2, 3, 1, 3, 3, "OVFKWKEIBPGNYPVKWCYU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F QFEOIKPUKEIBJOTDQAMY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -0.97, 1.16, 1, 2, 3, 1, 3, 3, "QFEOIKPUKEIBJOTDQAMY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F RDLDHYKHKPWENJTOWOXD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.64, -0.08, 1.91, 1, 2, 3, 1, 3, 3, "RDLDHYKHKPWENJTOWOXD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F RHBYPULLAQSYMQYGREQP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.67, -0.588, 1.87, 1, 2, 3, 1, 3, 3, "RHBYPULLAQSYMQYGREQP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F RMMMMMMMSYPVKMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.56, -0.24, 2.2, 1, 2, 3, 1, 3, 3, "RMMMMMMMSYPVKMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F SCEYJHFADPMQDMCUWKVW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.81, -0.85, 1.17, 1, 2, 3, 1, 3, 3, "SCEYJHFADPMQDMCUWKVW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F VFOTKULQICUAFLQHUKPX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -1.23, 1.85, 1, 2, 3, 1, 3, 3, "VFOTKULQICUAFLQHUKPX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F XHDPILBCVMHPOJFVXIWK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.16, -0.59, 0.95, 1, 2, 3, 1, 3, 3, "XHDPILBCVMHPOJFVXIWK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"F YAFYNGBDDHNVBPSPAIDF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.037, -0.1565, 0.6, 1, 2, 3, 1, 3, 3, "YAFYNGBDDHNVBPSPAIDF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapF, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type G
	{
	"G EMMMMMMMMMMMMAJSDJMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.22, -0.86, 2.25, 1, 2, 3, 1, 3, 3, "EMMMMMMMMMMMMAJSDJMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G FUXRRRUIRDYKDUBPHHOMOBRIRBINCS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.3, 0.9, 1, 2, 3, 1, 3, 3, "FUXRRRUIRDYKDUBPHHOMOBRIRBINCS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G GNXVYVASWMMNFFQOFJTMRBNRFWREJH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.18, 0.63, 1, 2, 3, 1, 3, 3, "GNXVYVASWMMNFFQOFJTMRBNRFWREJH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G GTLDSNGVTBTEINQKQDPQEGNVFMOKES Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.82, -0.7, 0.73, 1, 2, 3, 1, 3, 3, "GTLDSNGVTBTEINQKQDPQEGNVFMOKES", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G IGXPNBBVEMODKOCLXERMUNVVPWECYS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.932, -0.73, 0.93, 1, 2, 3, 1, 3, 3, "IGXPNBBVEMODKOCLXERMUNVVPWECYS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G ISPJWDOAOFTCUBLOMEWXFCXDJQDRDU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.13, -0.462, 1.2, 1, 2, 3, 1, 3, 3, "ISPJWDOAOFTCUBLOMEWXFCXDJQDRDU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G JCQPYDVBNJUMEBGJROUIHUXIDNDYIH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.33, -0.62, 1.3, 1, 2, 3, 1, 3, 3, "JCQPYDVBNJUMEBGJROUIHUXIDNDYIH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G JNQRXOIMQLYVLQUNWKDCMXQAOYNZOT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.41, -0.8, 1.26, 1, 2, 3, 1, 3, 3, "JNQRXOIMQLYVLQUNWKDCMXQAOYNZOT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G KDSEBTOQIOKPQYHKJUGSWYHMKICRYS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.543, 1.16, 1, 2, 3, 1, 3, 3, "KDSEBTOQIOKPQYHKJUGSWYHMKICRYS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G KJWTVIJQTOGJDROEPYKTNWTGABNVBN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.37, -0.9, 1.4, 1, 2, 3, 1, 3, 3, "KJWTVIJQTOGJDROEPYKTNWTGABNVBN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G KPQYRIDMPDGDKTTRRQPHOCFAOWRABQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.15, -0.23, 0.767, 1, 2, 3, 1, 3, 3, "KPQYRIDMPDGDKTTRRQPHOCFAOWRABQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G LQGRLUFUCASAWSVRVEGGFFNPYHOKRM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.24, 0.75, 1, 2, 3, 1, 3, 3, "LQGRLUFUCASAWSVRVEGGFFNPYHOKRM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G LURFSRHWMSKHTQBKXJDXQSMFJBWUFG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.85, 1.2, 1, 2, 3, 1, 3, 3, "LURFSRHWMSKHTQBKXJDXQSMFJBWUFG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G MEHSXHFEMMYNXCJPRTKWADSAPIBBWR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -0.89, 1.94, 1, 2, 3, 1, 3, 3, "MEHSXHFEMMYNXCJPRTKWADSAPIBBWR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G MTBIQYTGHEOJFMLDRXEIFJHLUPBBUY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.54, -1.187, 1.82, 1, 2, 3, 1, 3, 3, "MTBIQYTGHEOJFMLDRXEIFJHLUPBBUY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G MVMMPSBXJMZCOFROXCKAFGVDQOKGIK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.779, -0.39, 0.854, 1, 2, 3, 1, 3, 3, "MVMMPSBXJMZCOFROXCKAFGVDQOKGIK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G NCDXEXXMVEOQBKWKEIMXJMLTDEDVSR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.23, -0.61, 1.34, 1, 2, 3, 1, 3, 3, "NCDXEXXMVEOQBKWKEIMXJMLTDEDVSR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G NVIKAVSGBOYGFMTIXSOQNOTCVDOZCR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.458, -0.457, 0.83, 1, 2, 3, 1, 3, 3, "NVIKAVSGBOYGFMTIXSOQNOTCVDOZCR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G NWCAVJOVLINUWMKNHCHMQOJADIWHIY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.92, 1.4, 1, 2, 3, 1, 3, 3, "NWCAVJOVLINUWMKNHCHMQOJADIWHIY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G OSLDJIOXHGVDTGGNCNOQYNWJBFHYLX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.962, -0.515, 1.36, 1, 2, 3, 1, 3, 3, "OSLDJIOXHGVDTGGNCNOQYNWJBFHYLX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G PFMQPPBPARCUOLSTATEXQDKEXMLOIF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.22, 1.1, 1, 2, 3, 1, 3, 3, "PFMQPPBPARCUOLSTATEXQDKEXMLOIF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G QDIDSBTPNDBSGOKOKGAKMCCONXFHWQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.8, 1.8, 1, 2, 3, 1, 3, 3, "QDIDSBTPNDBSGOKOKGAKMCCONXFHWQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G QHRLGBFRLNFSNXGUKGWEJKFMQTGRDC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.81, -0.53, 1.46, 1, 2, 3, 1, 3, 3, "QHRLGBFRLNFSNXGUKGWEJKFMQTGRDC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G QIWKNIRBQKIYSLXLCCKGEUQVUELLLJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.55, -0.73, 0.81, 1, 2, 3, 1, 3, 3, "QIWKNIRBQKIYSLXLCCKGEUQVUELLLJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G QXEHVCCLDWBHSHHMUUABJRSPJLTTFC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.848, -0.227, 1.26, 1, 2, 3, 1, 3, 3, "QXEHVCCLDWBHSHHMUUABJRSPJLTTFC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G RLKHETMSIVYWMBBQODMFNGRISGHBSI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.681, -0.454, 1.06, 1, 2, 3, 1, 3, 3, "RLKHETMSIVYWMBBQODMFNGRISGHBSI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G RMJQBCSOAFMBRRSSUHCNBWVSRICXAA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.96, -0.23, 1.2, 1, 2, 3, 1, 3, 3, "RMJQBCSOAFMBRRSSUHCNBWVSRICXAA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G SELTBLWMEENVFBSFAXMVDXKKEDGYXL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.78, -1.0, 1.0, 1, 2, 3, 1, 3, 3, "SELTBLWMEENVFBSFAXMVDXKKEDGYXL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G TNSTDPQKSQNFJAHNCYFLDWFKSPDECV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.33, -0.69, 0.76, 1, 2, 3, 1, 3, 3, "TNSTDPQKSQNFJAHNCYFLDWFKSPDECV", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G TPMJKFSCWUMSHBVPCBUTBRRVXHSXIT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.28, -0.7, 0.95, 1, 2, 3, 1, 3, 3, "TPMJKFSCWUMSHBVPCBUTBRRVXHSXIT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G TUFSHTHCLKCTBYWNMCISKERQISFVAV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.97, -0.73, 1.6, 1, 2, 3, 1, 3, 3, "TUFSHTHCLKCTBYWNMCISKERQISFVAV", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G UETJGIINOTHGFYLJOUVEEMXTEGDHLM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.7, 1.3, 1, 2, 3, 1, 3, 3, "UETJGIINOTHGFYLJOUVEEMXTEGDHLM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G UMMMMMMMMMMMMWEODFMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.21, -1.23, 1.87, 1, 2, 3, 1, 3, 3, "UMMMMMMMMMMMMWEODFMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G UOBMPCJRXSHHSCPMQFZVNESALEKOHY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.77, -0.42, 1.6, 1, 2, 3, 1, 3, 3, "UOBMPCJRXSHHSCPMQFZVNESALEKOHY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"G XEMONYFKDJMDTPNSLGHQLHOOTOQBUN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.71, -0.78, 1.58, 1, 2, 3, 1, 3, 3, "XEMONYFKDJMDTPNSLGHQLHOOTOQBUN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapG, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type H
	{
	"H CJBKUPMMMMMMMMMMWMMMMICMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -1.33, 3.2, 1, 2, 3, 1, 3, 3, "CJBKUPMMMMMMMMMMWMMMMICMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H DCEEYSMMMMMMMMMMWMMMMNCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.8, 2.0, 1, 2, 3, 1, 3, 3, "DCEEYSMMMMMMMMMMWMMMMNCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H FJFWKASKFNFGNFMSAUSHJNGBJCDYVGFHPWLWHSZGTQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.35, 0.8, 1, 2, 3, 1, 3, 3, "FJFWKASKFNFGNFMSAUSHJNGBJCDYVGFHPWLWHSZGTQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H FOWFIPMMMMMMMMMMWMMMMRCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -0.93, 2.8, 1, 2, 3, 1, 3, 3, "FOWFIPMMMMMMMMMMWMMMMRCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 200000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H GEQGOYIKQQPEUJBKPXTVUSJHOVJDUAYYPRNTXFLGAM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.05, 0.8, 1, 2, 3, 1, 3, 3, "GEQGOYIKQQPEUJBKPXTVUSJHOVJDUAYYPRNTXFLGAM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H HANQRENHONYATQYPTNXKNMNQEGDWKYPNSMMMODAOBC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.03, -0.56, 1.0, 1, 2, 3, 1, 3, 3, "HANQRENHONYATQYPTNXKNMNQEGDWKYPNSMMMODAOBC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H HJJYPHMMMMMMMMMMWMMMMUCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -0.72, 2.8, 1, 2, 3, 1, 3, 3, "HJJYPHMMMMMMMMMMWMMMMUCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 500000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H HVOIEGIDJCSFUFJCQGRUGMCLHEPWKRCCYFIRQPYAPH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -0.3, 1.4, 1, 2, 3, 1, 3, 3, "HVOIEGIDJCSFUFJCQGRUGMCLHEPWKRCCYFIRQPYAPH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H IFZLMPJUBERQBKLRRDOWMOLICDPVRJOTHOBSFUKGVL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.18, -0.6, 1.38, 1, 2, 3, 1, 3, 3, "IFZLMPJUBERQBKLRRDOWMOLICDPVRJOTHOBSFUKGVL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H IOMYNHMMMMMMMMMMCMMMMFWMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.4, 2.0, 1, 2, 3, 1, 3, 3, "IOMYNHMMMMMMMMMMCMMMMFWMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 500000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H JVVHSHPOLDTXUCNMYRAYOFBMAAJFMLNAHJQOBBNXQX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.14, -0.56, 0.8, 1, 2, 3, 1, 3, 3, "JVVHSHPOLDTXUCNMYRAYOFBMAAJFMLNAHJQOBBNXQX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H KMMMMMMMMMMMMMMMMMMLEXJLMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.8, 3.8, 3.0, 1, 2, 3, 1, 3, 3, "KMMMMMMMMMMMMMMMMMMLEXJLMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 5.0, 1.0
	},
	{
	"H KTSDLOMMMMMMMMMMWMMMMDCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.68, -1.92, 2.77, 1, 2, 3, 1, 3, 3, "KTSDLOMMMMMMMMMMWMMMMDCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H LONENJVYCXAJUMHKRSEUAOANAQGULOQBHKNWQOVOJN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.62, 1.5, 1, 2, 3, 1, 3, 3, "LONENJVYCXAJUMHKRSEUAOANAQGULOQBHKNWQOVOJN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H LTQSSRKWCLUGBNYOSUKEIIHLUDRJRKFJGTCNXKYOMC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.66, 1.3, 1, 2, 3, 1, 3, 3, "LTQSSRKWCLUGBNYOSUKEIIHLUDRJRKFJGTCNXKYOMC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H MOAQRKMMMMMMMMMMWMMMMYCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.22, -1.34, 4.13, 1, 2, 3, 1, 3, 3, "MOAQRKMMMMMMMMMMWMMMMYCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H MSMTNCONSQJOTKOPAOMQYNDPUQWVQJUEGNWAYGDLIT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.75, 1.5, 1, 2, 3, 1, 3, 3, "MSMTNCONSQJOTKOPAOMQYNDPUQWVQJUEGNWAYGDLIT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H NBCTKKSQMGKBDNBXSLIFLJWOONPAPCGTYTUXWRVMQU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.71, -1.32, 1.9, 1, 2, 3, 1, 3, 3, "NBCTKKSQMGKBDNBXSLIFLJWOONPAPCGTYTUXWRVMQU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H NJCYVPMFLGQMPHCQEOMHMMEIHIJJJDDCRVYEGUXBSF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.63, 1.0, 1, 2, 3, 1, 3, 3, "NJCYVPMFLGQMPHCQEOMHMMEIHIJJJDDCRVYEGUXBSF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H OKEFWLHUMOIXLAEEGWTVRMBTSJOSCJIBARBYXSXLDJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.8, 1.8, 1, 2, 3, 1, 3, 3, "OKEFWLHUMOIXLAEEGWTVRMBTSJOSCJIBARBYXSXLDJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H OLTJASMMMMMMMMMMWMMMMWCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.97, -0.476, 1.85, 1, 2, 3, 1, 3, 3, "OLTJASMMMMMMMMMMWMMMMWCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H OMMMMMMMMMMMMMMMMMMYGSPPMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, 0.37, 1.9, 1, 2, 3, 1, 3, 3, "OMMMMMMMMMMMMMMMMMMYGSPPMMMMMMMMMMMMMMMMMM", 0, YAXIS, 1000000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H PYIESIXFDBTDVPNOUFHZZPNFOCNTRINYYVMVLHHZLE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -1.1, 1.6, 1, 2, 3, 1, 3, 3, "PYIESIXFDBTDVPNOUFHZZPNFOCNTRINYYVMVLHHZLE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H QBKSKIXQMKEOVVMAHXLBOQQJXEYMBUMBOEFVDBAPWU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -1.0, 2.0, 1, 2, 3, 1, 3, 3, "QBKSKIXQMKEOVVMAHXLBOQQJXEYMBUMBOEFVDBAPWU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H QDHFCNDPFVXOIXKPUMIQJJFOKCYELPTJPBSPOFGAPL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.6, 0.75, 1, 2, 3, 1, 3, 3, "QDHFCNDPFVXOIXKPUMIQJJFOKCYELPTJPBSPOFGAPL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H QJDYFYUMIRBQRDGPLHOBEHGBHDNKVCRSSYVJIFIVLF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -1.17, 1.2, 1, 2, 3, 1, 3, 3, "QJDYFYUMIRBQRDGPLHOBEHGBHDNKVCRSSYVJIFIVLF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H QMXVDGMMMMMMMMMMWMMMMBCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.48, -1.94, 2.44, 1, 2, 3, 1, 3, 3, "QMXVDGMMMMMMMMMMWMMMMBCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H ROIARTMMMMMMMMMMWMMMMFCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.42, 1.72, 1, 2, 3, 1, 3, 3, "ROIARTMMMMMMMMMMWMMMMFCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H ROXTCGMMMMMMMMMMWMMMMCCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -2.0, 2.5, 1, 2, 3, 1, 3, 3, "ROXTCGMMMMMMMMMMWMMMMCCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 200000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H RPMBJSMMMMMMMMMMWMMMMKCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.1, -1.57, 2.4, 1, 2, 3, 1, 3, 3, "RPMBJSMMMMMMMMMMWMMMMKCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H RUTHBNTTDGJJJDTVZBMVMIFSIDNDFEGWLJINVMMVXM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.67, -0.7, 0.87, 1, 2, 3, 1, 3, 3, "RUTHBNTTDGJJJDTVZBMVMIFSIDNDFEGWLJINVMMVXM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H SARYDPNQIYYBGSXBFOFLRRPSWDEQGOSMSCONFEBVRP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.2, -0.01, 0.54, 1, 2, 3, 1, 3, 3, "SARYDPNQIYYBGSXBFOFLRRPSWDEQGOSMSCONFEBVRP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H SAXIXDPKULROIQBVOECGZQNWTAEYJAKDETXIOCHJQV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.52, -0.21, 0.98, 1, 2, 3, 1, 3, 3, "SAXIXDPKULROIQBVOECGZQNWTAEYJAKDETXIOCHJQV", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H UCXVGLMMMMMMMMMMWMMMMGCMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.1, -2.1, 2.57, 1, 2, 3, 1, 3, 3, "UCXVGLMMMMMMMMMMWMMMMGCMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H VEMBUMMMMMMMMMMMCMMMMNWMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.6, 1.8, 1, 2, 3, 1, 3, 3, "VEMBUMMMMMMMMMMMCMMMMNWMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 200000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H VFMTTCMMMMMMMMMMCMMMMQWMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -0.5, 2.2, 1, 2, 3, 1, 3, 3, "VFMTTCMMMMMMMMMMCMMMMQWMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 200000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H VNSGEQMMMMMMMMMMWMMMMECMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.129, 0.05, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -2.0, 2.4, 1, 2, 3, 1, 3, 3, "VNSGEQMMMMMMMMMMWMMMMECMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 200000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H VHDXLMSMKIBUMCNKOCPSPJMTFNPEDJQLNFOBTTHMPT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.65, -0.64, 1.0, 1, 2, 3, 1, 3, 3, "VHDXLMSMKIBUMCNKOCPSPJMTFNPEDJQLNFOBTTHMPT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H VNTBSGWPIJIQFTJZIGRJTDXWLMDPWSVUNEFVSBMYFE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.95, -1.2, 1.9, 1, 2, 3, 1, 3, 3, "VNTBSGWPIJIQFTJZIGRJTDXWLMDPWSVUNEFVSBMYFE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"H WBVVHLMMMMMMMMMMCMMMMSWMMMMMMMMMMMMMMMMMMM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.09, -0.54, 2.7, 1, 2, 3, 1, 3, 3, "WBVVHLMMMMMMMMMMCMMMMSWMMMMMMMMMMMMMMMMMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapH, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type I
	{
	"I FJLRNTKMSNJOXRDYPVEOOVTPLMGEAC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -0.16, 2.3, 1, 2, 3, 1, 3, 3, "FJLRNTKMSNJOXRDYPVEOOVTPLMGEAC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I GHCMVFESUFJJGIUOHWFROOIGMGFBEC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.85, 0.8, 1, 2, 3, 1, 3, 3, "GHCMVFESUFJJGIUOHWFROOIGMGFBEC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I GMNIKFCCNCPQFCJRQFUALCCLJPYVYD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.7, 0.89, 1, 2, 3, 1, 3, 3, "GMNIKFCCNCPQFCJRQFUALCCLJPYVYD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I GNXQDPRVJPMBASUKJCRDRWVTDRQTTD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.9, 1.5, 1, 2, 3, 1, 3, 3, "GNXQDPRVJPMBASUKJCRDRWVTDRQTTD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I GVQYNBNOMJCSSJIBFIDXWGGCWUOACV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.68, -0.9, 0.64, 1, 2, 3, 1, 3, 3, "GVQYNBNOMJCSSJIBFIDXWGGCWUOACV", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I HESJWNBMQQEOYLGPPBXGNHTJTPNTUG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.82, -0.7, 0.64, 1, 2, 3, 1, 3, 3, "HESJWNBMQQEOYLGPPBXGNHTJTPNTUG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I HHFGLDKRNJIYWJPMWNUOKJMLAAHQQD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.9, 1.1, 1, 2, 3, 1, 3, 3, "HHFGLDKRNJIYWJPMWNUOKJMLAAHQQD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I HJJDTICQIJETXFYNUSJKJSXGADBDKR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.83, 1.3, 1, 2, 3, 1, 3, 3, "HJJDTICQIJETXFYNUSJKJSXGADBDKR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I HJNHTBGIFNDQMDOYWRYYTFSSRXQBEK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -1.4, 0.9, 1, 2, 3, 1, 3, 3, "HJNHTBGIFNDQMDOYWRYYTFSSRXQBEK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I HORHNHPPRVIVNNJUYHHMMJEWMMSUEU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -1.1, 1.3, 1, 2, 3, 1, 3, 3, "HORHNHPPRVIVNNJUYHHMMJEWMMSUEU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I HSTPPHHFYAJJFBRNMYMYNCEYXLFLRR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.7, 1.0, 1, 2, 3, 1, 3, 3, "HSTPPHHFYAJJFBRNMYMYNCEYXLFLRR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I IIQMSNRHWELIGAGIURCSRIWABJSESC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 1.2, 1, 2, 3, 1, 3, 3, "IIQMSNRHWELIGAGIURCSRIWABJSESC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I IPPSGTMPCELDIPWPUPJCNQNFOBCYCK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.86, -0.13, 0.62, 1, 2, 3, 1, 3, 3, "IPPSGTMPCELDIPWPUPJCNQNFOBCYCK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I IUWOTLCIVQNMKGLXCBGELWUUWUQKIT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.18, 0.22, 0.53, 1, 2, 3, 1, 3, 3, "IUWOTLCIVQNMKGLXCBGELWUUWUQKIT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I IXYMLIVIVWOAXXXLEHKDPICIGQUQGF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.52, 0.01, 0.4, 1, 2, 3, 1, 3, 3, "IXYMLIVIVWOAXXXLEHKDPICIGQUQGF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JEKESGYYFWLOGVKLMEWJMBKHSOIVTI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.5, 1.1, 1, 2, 3, 1, 3, 3, "JEKESGYYFWLOGVKLMEWJMBKHSOIVTI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JIFTNNCYIGTVRLCMNJLKUQDGJIQSJC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.22, -0.13, 0.72, 1, 2, 3, 1, 3, 3, "JIFTNNCYIGTVRLCMNJLKUQDGJIQSJC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JKRADSXGDBHIJTQJJDICEJKYSTXFNU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.95, 1.2, 1, 2, 3, 1, 3, 3, "JKRADSXGDBHIJTQJJDICEJKYSTXFNU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JMUYUNOXHMLLOJREMMWFJRWTHSVXOL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.68, 0.83, 1, 2, 3, 1, 3, 3, "JMUYUNOXHMLLOJREMMWFJRWTHSVXOL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JMYRKHSAUVRKPFVIJDMANDCIGJTIOB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -0.17, 0.81, 1, 2, 3, 1, 3, 3, "JMYRKHSAUVRKPFVIJDMANDCIGJTIOB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JNMYBMYRJFNLFLYRRXCEHHHFPYAPST Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.86, -0.03, 0.75, 1, 2, 3, 1, 3, 3, "JNMYBMYRJFNLFLYRRXCEHHHFPYAPST", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JVSBDFVNDDWWNMDMHDOPCFNYRIYWIU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, 0.075, 0.94, 1, 2, 3, 1, 3, 3, "JVSBDFVNDDWWNMDMHDOPCFNYRIYWIU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JVSURQUIINRDBRJAWRAKMLAHHUAOON Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.76, 0.025, 0.55, 1, 2, 3, 1, 3, 3, "JVSURQUIINRDBRJAWRAKMLAHHUAOON", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I JWWHJKPHMMKUVMKFSRHKJCYOISSQNB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 0.9, 1, 2, 3, 1, 3, 3, "JWWHJKPHMMKUVMKFSRHKJCYOISSQNB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I KLEDDBHYJKFRYPBNYCVPRSVJILWEFP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.7, 1.6, 1, 2, 3, 1, 3, 3, "KLEDDBHYJKFRYPBNYCVPRSVJILWEFP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I KNXVQUEXYETWOOSGNSBDMHTMCPFLNG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.81, -0.03, 0.86, 1, 2, 3, 1, 3, 3, "KNXVQUEXYETWOOSGNSBDMHTMCPFLNG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I KRTYCFPFLTLSMOKRPEKMGPQHMUQYGY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.47, -0.46, 0.77, 1, 2, 3, 1, 3, 3, "KRTYCFPFLTLSMOKRPEKMGPQHMUQYGY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I KTOSTVLDYEKWJRCTSIHIQJQVBSSEWG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -1.8, 1.4, 1, 2, 3, 1, 3, 3, "KTOSTVLDYEKWJRCTSIHIQJQVBSSEWG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I KTSIRHICWJQSSEVWGBJQKVLDSYETTO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, 0.4, 1.2, 1, 2, 3, 1, 3, 3, "KTSIRHICWJQSSEVWGBJQKVLDSYETTO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I KUELCPYRWJFNDCNNRBVQKQREITYMIY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -1.2, 1.5, 1, 2, 3, 1, 3, 3, "KUELCPYRWJFNDCNNRBVQKQREITYMIY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I LGEBMRRGWSRRFOQCLRDOOEARWYJBVE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.9, -0.38, 1.0, 1, 2, 3, 1, 3, 3, "LGEBMRRGWSRRFOQCLRDOOEARWYJBVE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I LITWKWUUUQIVQTIUWOLCNGELBMKGXC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.79, -1.14, 0.9, 1, 2, 3, 1, 3, 3, "LITWKWUUUQIVQTIUWOLCNGELBMKGXC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I LLMEVWJKOGMOIVHTISBKJGYYEFWSEK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.2, 0.9, 1, 2, 3, 1, 3, 3, "LLMEVWJKOGMOIVHTISBKJGYYEFWSEK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I LNQAAYRNMTGVRVNPUNNKQFRBBTTYEW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.03, -0.88, 0.67, 1, 2, 3, 1, 3, 3, "LNQAAYRNMTGVRVNPUNNKQFRBBTTYEW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I LOTMOQYJBPLDUWTSWJQDQJVAQLEDQF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.61, -0.04, 0.65, 1, 2, 3, 1, 3, 3, "LOTMOQYJBPLDUWTSWJQDQJVAQLEDQF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I LSQGYJJPISRVKJNPEXGIOSFDRQYGLO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.0, 2.0, 1, 2, 3, 1, 3, 3, "LSQGYJJPISRVKJNPEXGIOSFDRQYGLO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I LURCEGOHOIQFJKBSNYGSNRUKKIKIHW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, 0.76, 1.5, 1, 2, 3, 1, 3, 3, "LURCEGOHOIQFJKBSNYGSNRUKKIKIHW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I LXRNRBKDJISQKAHRGWUTJONSIKVUBC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.58, -0.44, 1.2, 1, 2, 3, 1, 3, 3, "LXRNRBKDJISQKAHRGWUTJONSIKVUBC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I MNGCLHTMPFKYEQXNXVUETBDSSWOOGN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.48, 0.75, 1, 2, 3, 1, 3, 3, "MNGCLHTMPFKYEQXNXVUETBDSSWOOGN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I MROJCCRWSIMRPTLLENELIVYDEFWQHR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.36, -0.41, 0.86, 1, 2, 3, 1, 3, 3, "MROJCCRWSIMRPTLLENELIVYDEFWQHR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I MTISVBKHOIJFWSYEKEGYLWJKEOGVLM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.23, -0.39, 0.53, 1, 2, 3, 1, 3, 3, "MTISVBKHOIJFWSYEKEGYLWJKEOGVLM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I NDVLLVRFLHPLLFCREMPLGLWNAFRPST Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.25, -0.029, 0.5, 1, 2, 3, 1, 3, 3, "NDVLLVRFLHPLLFCREMPLGLWNAFRPST", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I NJWFGVSOPUNATNJMNRWDQMFKIGMRSB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.75, 1.3, 1, 2, 3, 1, 3, 3, "NJWFGVSOPUNATNJMNRWDQMFKIGMRSB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I NKRCPBNOMEMVQQKSKYEIJOCQWEYOFP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.32, 0.7, 1, 2, 3, 1, 3, 3, "NKRCPBNOMEMVQQKSKYEIJOCQWEYOFP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I NLJYYNNEIORHAKLKJKOVJFTFGGSMQY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.25, -0.06, 0.6, 1, 2, 3, 1, 3, 3, "NLJYYNNEIORHAKLKJKOVJFTFGGSMQY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I NNRCKWREIASTBGRGPADGMGSHPKMPHU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, 0.02, 0.8, 1, 2, 3, 1, 3, 3, "NNRCKWREIASTBGRGPADGMGSHPKMPHU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I NRRXLCEYLFHYAPFSTPHHJMYRYJFBNM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.73, 1.0, 1, 2, 3, 1, 3, 3, "NRRXLCEYLFHYAPFSTPHHJMYRYJFBNM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I NSIUROMOJQIEEHYJKLRKQWULFDDGVB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.58, -0.68, 0.9, 1, 2, 3, 1, 3, 3, "NSIUROMOJQIEEHYJKLRKQWULFDDGVB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I NWJRPOXYLDYOHNEBHCQQAVNFQWJTDP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.7, -0.01, 0.65, 1, 2, 3, 1, 3, 3, "NWJRPOXYLDYOHNEBHCQQAVNFQWJTDP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I OCVGJFNYEVPTEQLASRSELPUHOTDBXP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.2, 0.86, 1, 2, 3, 1, 3, 3, "OCVGJFNYEVPTEQLASRSELPUHOTDBXP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I OGBGSHOUTDPTRFKCORFDLNKOSPNPHA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -0.4, 1.2, 1, 2, 3, 1, 3, 3, "OGBGSHOUTDPTRFKCORFDLNKOSPNPHA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I OHGWFIHJPSGWTOJBXWJKPBLKFRUKKQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.87, -0.75, 1.2, 1, 2, 3, 1, 3, 3, "OHGWFIHJPSGWTOJBXWJKPBLKFRUKKQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I OLORGSFDQYLISYPSQGJJRGINXVKJPE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.95, -0.7, 1.3, 1, 2, 3, 1, 3, 3, "OLORGSFDQYLISYPSQGJJRGINXVKJPE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I PGPJYPMITFPTBEEDRWRTUGSXCJFTWE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.8, 2.2, 1, 2, 3, 1, 3, 3, "PGPJYPMITFPTBEEDRWRTUGSXCJFTWE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I PIIOOVNXNHPAUADBROXSSACJSXGMKX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.05, 0.03, 0.4, 1, 2, 3, 1, 3, 3, "PIIOOVNXNHPAUADBROXSSACJSXGMKX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I PNBJINWBKXSIISTQCVRQNUPKSCLTXS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, 0.05, 0.75, 1, 2, 3, 1, 3, 3, "PNBJINWBKXSIISTQCVRQNUPKSCLTXS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I POBLRCRTKMWNCSJKQQABSKGDYFJGSB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -0.46, 1.8, 1, 2, 3, 1, 3, 3, "POBLRCRTKMWNCSJKQQABSKGDYFJGSB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I QAIENYAFHNIKRLSNRKPMICIXEOQYXR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.52, -0.6, 0.43, 1, 2, 3, 1, 3, 3, "QAIENYAFHNIKRLSNRKPMICIXEOQYXR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I QCBIKKBNREKJEWFTMAJYFGIEBHLFEH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -0.52, 1.6, 1, 2, 3, 1, 3, 3, "QCBIKKBNREKJEWFTMAJYFGIEBHLFEH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I QETFNJNAHIINXFKUHXYHMTTBNJSIII Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.4, -0.72, 0.75, 1, 2, 3, 1, 3, 3, "QETFNJNAHIINXFKUHXYHMTTBNJSIII", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I QNBDVISXIPPLGVLRMKNCMORMJOCIHX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.12, 0.9, 1, 2, 3, 1, 3, 3, "QNBDVISXIPPLGVLRMKNCMORMJOCIHX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I QWGBEJQVSSKYETDTOSVLKHICIWJRTS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.15, -0.13, 0.92, 1, 2, 3, 1, 3, 3, "QWGBEJQVSSKYETDTOSVLKHICIWJRTS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I RGOUVHFMIJQBAKEWDJOVQNUSGCNPDU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, -0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.55, 1.2, 1, 2, 3, 1, 3, 3, "RGOUVHFMIJQBAKEWDJOVQNUSGCNPDU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I RIVIMLQBPFVPSLIKHJNDSPMWMCBGMK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.45, 0.7, 0.3, 1, 2, 3, 1, 3, 3, "RIVIMLQBPFVPSLIKHJNDSPMWMCBGMK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I RQEDYQNTRPNDHNWNOFJOOLNTEEBPSA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.49, -1.0, 1.1, 1, 2, 3, 1, 3, 3, "RQEDYQNTRPNDHNWNOFJOOLNTEEBPSA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I SKKGLKSUNOQLCDKUOBPINHIDBKPOKR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.86, -0.4, 1.5, 1, 2, 3, 1, 3, 3, "SKKGLKSUNOQLCDKUOBPINHIDBKPOKR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I SNQBNOJFJWMPQDTTFNMNNOISRYCDUN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.46, 0.17, 1.3, 1, 2, 3, 1, 3, 3, "SNQBNOJFJWMPQDTTFNMNNOISRYCDUN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I UFPFQLVOLTUAVQYFLEVREPQLSNQRCD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.72, -0.62, 1.5, 1, 2, 3, 1, 3, 3, "UFPFQLVOLTUAVQYFLEVREPQLSNQRCD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I UIMPUSPSEJNDPKKENDVSEHCVWDVEGQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.45, -0.6, 1.2, 1, 2, 3, 1, 3, 3, "UIMPUSPSEJNDPKKENDVSEHCVWDVEGQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I UWECTGSXJFPTFYIGPJPMPRTEWTBEDR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.044, 0.25, 0.54, 1, 2, 3, 1, 3, 3, "UWECTGSXJFPTFYIGPJPMPRTEWTBEDR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I WDWOGDGWGORJOBTUHFQBPRNTCBYQHP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.23, 0.23, 0.55, 1, 2, 3, 1, 3, 3, "WDWOGDGWGORJOBTUHFQBPRNTCBYQHP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I WUBBBVGSWOQFPMBKOPLQKUEIKHSVHM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.62, -0.62, 1.6, 1, 2, 3, 1, 3, 3, "WUBBBVGSWOQFPMBKOPLQKUEIKHSVHM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"I YBHCNQQEOHAWJTFDPQVNNOXYRLDPWJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.65, -0.42, 1.1, 1, 2, 3, 1, 3, 3, "YBHCNQQEOHAWJTFDPQVNNOXYRLDPWJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type J
	{
	"J HYWCCFNMLQJVCGPDQTWBPSMOBWMWVVJWNJYALSWTJGJDMRNHKKKRBDNOMFQM Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.27, 0.62, 1, 2, 3, 1, 3, 3, "HYWCCFNMLQJVCGPDQTWBPSMOBWMWVVJWNJYALSWTJGJDMRNHKKKRBDNOMFQM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J JICKAFXIOXFVGOCIDNIVRPSFYPFGABXKKONQWPAMJGKAGXDBBWFHGXBTPNVD Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.51, 0.0, 0.4, 1, 2, 3, 1, 3, 3, "JICKAFXIOXFVGOCIDNIVRPSFYPFGABXKKONQWPAMJGKAGXDBBWFHGXBTPNVD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J JINFJICIFTJRQDVGRJOYMJDOKXWESQAEQSPNMBUSPSKUQXAIGWIYOMSGHRKP Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.61, -0.28, 0.5, 1, 2, 3, 1, 3, 3, "JINFJICIFTJRQDVGRJOYMJDOKXWESQAEQSPNMBUSPSKUQXAIGWIYOMSGHRKP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J JOPUPCMBPOTKEVGVLVKHJGVVBCECNHWHNHKECLWFNQYOAUVSWNLAKDKXPDMT Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.15, 0.36, 1, 2, 3, 1, 3, 3, "JOPUPCMBPOTKEVGVLVKHJGVVBCECNHWHNHKECLWFNQYOAUVSWNLAKDKXPDMT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J JXLHCRXHALLAQVJQYVATLXKTSALYIDBXICJHQWTYPDQJLMJCVYLLJXGSWJEK Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -0.4, 0.61, 1, 2, 3, 1, 3, 3, "JXLHCRXHALLAQVJQYVATLXKTSALYIDBXICJHQWTYPDQJLMJCVYLLJXGSWJEK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J KKELGSDRUNKNJJWFMKCOTYNISCXKYAPNPFABVLKKLRWIRDKDFUVCQVUHFBGG Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.72, 0.23, 0.58, 1, 2, 3, 1, 3, 3, "KKELGSDRUNKNJJWFMKCOTYNISCXKYAPNPFABVLKKLRWIRDKDFUVCQVUHFBGG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J KWNBMTOFGIDTXQJWSJXLRCHXBDXVKTBCJHKFSJGIHMXOIJNOEWXGSMMJIIYR Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.31, 0.94, 1, 2, 3, 1, 3, 3, "KWNBMTOFGIDTXQJWSJXLRCHXBDXVKTBCJHKFSJGIHMXOIJNOEWXGSMMJIIYR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J LDMKNSOMROLNJUIPQVIFKDIYJMYLSUIWGJFWHTIPRVUTBSGQKMHYPGIDKLPH Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -1.07, 1.0, 1, 2, 3, 1, 3, 3, "LDMKNSOMROLNJUIPQVIFKDIYJMYLSUIWGJFWHTIPRVUTBSGQKMHYPGIDKLPH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J LEDOSLNEYICPJBJNOQYCQGGAVBLMEJUITFIDOJCLNBXAVCQWRMWILBUFOSTS Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.46, 1.24, 1, 2, 3, 1, 3, 3, "LEDOSLNEYICPJBJNOQYCQGGAVBLMEJUITFIDOJCLNBXAVCQWRMWILBUFOSTS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J LHREQNCACHAESQUGCMLHKYPBRMHGLSRTKFMVNBNDMPBWEQSRTHSMVMNRFDYJ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.68, 0.81, 1, 2, 3, 1, 3, 3, "LHREQNCACHAESQUGCMLHKYPBRMHGLSRTKFMVNBNDMPBWEQSRTHSMVMNRFDYJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J LRTXANMYLXUVOBSEXSDALEHOCMKWCIENOKYFLHUWFFTPFAYINNQORTHTBMID Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.35, 0.13, 0.4, 1, 2, 3, 1, 3, 3, "LRTXANMYLXUVOBSEXSDALEHOCMKWCIENOKYFLHUWFFTPFAYINNQORTHTBMID", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J MGRCVAUOFUEKPMTIRMMUMWVVKPRYLUMFOURHQNHMPAQMVNGBGEGNFUGMLAMN Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.42, 0.83, 1, 2, 3, 1, 3, 3, "MGRCVAUOFUEKPMTIRMMUMWVVKPRYLUMFOURHQNHMPAQMVNGBGEGNFUGMLAMN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J NRVAPNYFVEDGIVLUUFLVKNVCGQFEHWUISYBJDFJCKLMKRSYVPPKTBFUUAFQQ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.15, -0.26, 0.5, 1, 2, 3, 1, 3, 3, "NRVAPNYFVEDGIVLUUFLVKNVCGQFEHWUISYBJDFJCKLMKRSYVPPKTBFUUAFQQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J ONRJIUFLWTQFQFTUOCWQLPSSEQGBNLGMCKKEXSFGLHWKSDPJYIQNMJQBAIMT Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.51, -0.385, 0.89, 1, 2, 3, 1, 3, 3, "ONRJIUFLWTQFQFTUOCWQLPSSEQGBNLGMCKKEXSFGLHWKSDPJYIQNMJQBAIMT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J PLFQMNURLBLITVTLBLCIJRIYXLWVQUTEHYHLEDMXPDEIMUQRLOMLBNOYBKIP Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.67, 0.91, 1, 2, 3, 1, 3, 3, "PLFQMNURLBLITVTLBLCIJRIYXLWVQUTEHYHLEDMXPDEIMUQRLOMLBNOYBKIP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"J UDGRDVXMOHNMOYGPJIXMNMJPKXEAIIHNLJKHKBWHJIQUCGCVGQRSQGQDENMX Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.22, -0.12, 0.722, 1, 2, 3, 1, 3, 3, "UDGRDVXMOHNMOYGPJIXMNMJPKXEAIIHNLJKHKBWHJIQUCGCVGQRSQGQDENMX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapJ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type M
	{
	"M FFJSKNPJKXDRXXJIJJGLJAAMGNLVJTMPLNHEBDFIEMMHRNVSGIRVTVOIPFMI Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.78, -0.9, 1.29, 1, 2, 3, 1, 4, 4, "FFJSKNPJKXDRXXJIJJGLJAAMGNLVJTMPLNHEBDFIEMMHRNVSGIRVTVOIPFMI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M GDGPSELYVUMRDWSTCBJFHEQVYBYSLONPKBQVMKRUEVNLKIVVQPQVTDXMDSNO Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, 0.19, 0.4, 1, 2, 3, 1, 4, 4, "GDGPSELYVUMRDWSTCBJFHEQVYBYSLONPKBQVMKRUEVNLKIVVQPQVTDXMDSNO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M GRWGVPWQGWJCPIRJWMNTPPPGTQFHNLMLUPPLJVWESGISIHPHRYLXPAWOAPHN Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.21, -0.91, 0.96, 1, 2, 3, 1, 4, 4, "GRWGVPWQGWJCPIRJWMNTPPPGTQFHNLMLUPPLJVWESGISIHPHRYLXPAWOAPHN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M IJATENDDKJCXVMIMTSTVMKAJQUCTXEOUOOVKPPNMQNWGEOTKMYHCBBQTIJHV Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -0.63, 0.94, 1, 2, 3, 1, 4, 4, "IJATENDDKJCXVMIMTSTVMKAJQUCTXEOUOOVKPPNMQNWGEOTKMYHCBBQTIJHV", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M INXSAGQQMMODEJKPNPITOHHGAQNNPJSUMWCQYQHHFPXIJMSFWPWLNWNQVLRH Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.94, -0.285, 0.76, 1, 2, 3, 1, 4, 4, "INXSAGQQMMODEJKPNPITOHHGAQNNPJSUMWCQYQHHFPXIJMSFWPWLNWNQVLRH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M JRMIKPDMRXMYNQCPDAFRVHUQIWKWUHLVJANFCMXDKYHIJNURYLWQGUTAGALU Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.3, 0.88, 1, 2, 3, 1, 4, 4, "JRMIKPDMRXMYNQCPDAFRVHUQIWKWUHLVJANFCMXDKYHIJNURYLWQGUTAGALU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M KAIUOAYHHCLXYSITCWUNWOLMMDCEQLNQCCMPPHAONXEFCHLXVGVPLRKOFNPS Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.53, 0.136, 0.61, 1, 2, 3, 1, 4, 4, "KAIUOAYHHCLXYSITCWUNWOLMMDCEQLNQCCMPPHAONXEFCHLXVGVPLRKOFNPS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M KSOKQDGVGUFHWQYLSFUYFQBDHTBSXEMHOUSNDCJPGIDNKNGPKQLSLDQWPJNQ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.72, -0.37, 0.78, 1, 2, 3, 1, 4, 4, "KSOKQDGVGUFHWQYLSFUYFQBDHTBSXEMHOUSNDCJPGIDNKNGPKQLSLDQWPJNQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M KWWFTQHWFFBKCFHMUSVFFFANCROGSSPPGOYTTCAONIULPGRITEAQVVQICBKK Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.94, -0.46, 0.88, 1, 2, 3, 1, 4, 4, "KWWFTQHWFFBKCFHMUSVFFFANCROGSSPPGOYTTCAONIULPGRITEAQVVQICBKK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M LAEODJGEXUKTQCRFLVBLLLDGCVOBXMLBJPVBMNFXUIDNVQQMVMRLIYQLLPKJ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.76, -0.42, 0.6, 1, 2, 3, 1, 4, 4, "LAEODJGEXUKTQCRFLVBLLLDGCVOBXMLBJPVBMNFXUIDNVQQMVMRLIYQLLPKJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M LDFKCIKGRFSHJEGMSWSUDINRZWTOMENHOJDQVBQBVWBGCOUYCCBWNJLQSNJT Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.3, -0.11, 0.22, 1, 2, 3, 1, 4, 4, "LDFKCIKGRFSHJEGMSWSUDINRZWTOMENHOJDQVBQBVWBGCOUYCCBWNJLQSNJT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M LOGSXIHUQFPVBKSLJYGHTFKCRWTOSWKTAPFYFSXOSPPOYLLSFYTCVRQPQLEO Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.32, -0.3, 0.4, 1, 2, 3, 1, 4, 4, "LOGSXIHUQFPVBKSLJYGHTFKCRWTOSWKTAPFYFSXOSPPOYLLSFYTCVRQPQLEO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M LQALFYIJMJKKCOBFIVNXFELUGSGBNWHAYWDIQYDRJOUWNLMLOPAKJFBFQHHD Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -1.17, 1.24, 1, 2, 3, 1, 4, 4, "LQALFYIJMJKKCOBFIVNXFELUGSGBNWHAYWDIQYDRJOUWNLMLOPAKJFBFQHHD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M LRLDVIRFJRJGILUQYYYSFKHMQQOWRMIIPRGNQJHRKKHYDMXSIYMFPLQRWSSD Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.48, -0.09, 0.5, 1, 2, 3, 1, 4, 4, "LRLDVIRFJRJGILUQYYYSFKHMQQOWRMIIPRGNQJHRKKHYDMXSIYMFPLQRWSSD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M LWLRTTLKAFELEGNNMCFJSUKSPEFJTPIJFEOPNOPOKIGAJMCKVIAIQULRFSGD Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.78, -0.75, 1.2, 1, 2, 3, 1, 4, 4, "LWLRTTLKAFELEGNNMCFJSUKSPEFJTPIJFEOPNOPOKIGAJMCKVIAIQULRFSGD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MACCBHNSGDQNRKVMRDNJWQBFSLRBGBMLPBRKMNMIORGMDMNTHAVXNEHCLPJA Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.56, -0.45, 0.58, 1, 2, 3, 1, 4, 4, "MACCBHNSGDQNRKVMRDNJWQBFSLRBGBMLPBRKMNMIORGMDMNTHAVXNEHCLPJA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MBDLBAKPFYNXWHCMSKATJRKLYEHEDEMQSLMSRMGJAEIGCMNLNWAGBALVDEKP Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.38, -0.17, 0.45, 1, 2, 3, 1, 4, 4, "MBDLBAKPFYNXWHCMSKATJRKLYEHEDEMQSLMSRMGJAEIGCMNLNWAGBALVDEKP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MCRYBHOGYJKSQUSMVKNKXBOHRYFYOVMYMDHKJUEYPSTJQMAIBWIIFDUTRNOO Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.7, -0.43, 0.8, 1, 2, 3, 1, 4, 4, "MCRYBHOGYJKSQUSMVKNKXBOHRYFYOVMYMDHKJUEYPSTJQMAIBWIIFDUTRNOO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MDBTQODXOJLNOSVMTKEICYSWNSLPFNMVLKNSNYNLPFBPKMMYRUYSPDVSJDUH Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.33, -0.32, 0.46, 1, 2, 3, 1, 4, 4, "MDBTQODXOJLNOSVMTKEICYSWNSLPFNMVLKNSNYNLPFBPKMMYRUYSPDVSJDUH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MDDKCBROKYJMXXQKPBMJSSMSURNIKSONBTWUCCOPRAGMFPNNTVGRNEQDQYNL Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.87, -0.97, 0.9, 1, 2, 3, 1, 4, 4, "MDDKCBROKYJMXXQKPBMJSSMSURNIKSONBTWUCCOPRAGMFPNNTVGRNEQDQYNL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MDWDKNSUXWRILDGMGCLITOYXODGGRBMTUSSBQICHWLNFSMQNEPKHGFMLVOOR Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.68, -0.46, 0.71, 1, 2, 3, 1, 4, 4, "MDWDKNSUXWRILDGMGCLITOYXODGGRBMTUSSBQICHWLNFSMQNEPKHGFMLVOOR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MHOQKISLPQJCPKHMRKWOCRJYWDDHCCMOGJRFCIXILWTJQMUXFLCRKNBGQNFX Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.374, -0.278, 0.386, 1, 2, 3, 1, 4, 4, "MHOQKISLPQJCPKHMRKWOCRJYWDDHCCMOGJRFCIXILWTJQMUXFLCRKNBGQNFX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MICFXPCTGARVSOQMSWGKIIFPRMUVYQMSOEOREOYPIJJBHMLRVNUVMOHPVTOI Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.63, -0.26, 0.6, 1, 2, 3, 1, 4, 4, "MICFXPCTGARVSOQMSWGKIIFPRMUVYQMSOEOREOYPIJJBHMLRVNUVMOHPVTOI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MKCVPGEMLCPWBUHMSLLWTVJFCPTLRGMQLRMYFYMRLCWURMVBIUBXPSDPCNRJ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.59, -0.2, 0.67, 1, 2, 3, 1, 4, 4, "MKCVPGEMLCPWBUHMSLLWTVJFCPTLRGMQLRMYFYMRLCWURMVBIUBXPSDPCNRJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MLIYSWAWFBMXHXQMMWBHKOECJXSHCGMFJSGCGVQFFNBQXMNFMLIRCGTNHSPJ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.767, -0.226, 0.832, 1, 2, 3, 1, 4, 4, "MLIYSWAWFBMXHXQMMWBHKOECJXSHCGMFJSGCGVQFFNBQXMNFMLIRCGTNHSPJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MLOLQBGOTTARNTOMGDQKMUPTBDFTBNMEAWCOWUXLXSIACMNBDWPTMARNKEFN Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.225, -0.1, 0.39, 1, 2, 3, 1, 4, 4, "MLOLQBGOTTARNTOMGDQKMUPTBDFTBNMEAWCOWUXLXSIACMNBDWPTMARNKEFN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MMNLPBOWPGCVVDNMPQURKHKNXKSVKNMTVMUQLJYKOJFNWMUVCIVGROFBTKNK Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.42, -0.22, 0.5, 1, 2, 3, 1, 4, 4, "MMNLPBOWPGCVVDNMPQURKHKNXKSVKNMTVMUQLJYKOJFNWMUVCIVGROFBTKNK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MMTHIGVYGFFABIQMTDXTWMBDVACEUXMHHBJLSXROGSPJUMCNYFRYMTEWRXOX Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.475, -0.34, 0.42, 1, 2, 3, 1, 4, 4, "MMTHIGVYGFFABIQMTDXTWMBDVACEUXMHHBJLSXROGSPJUMCNYFRYMTEWRXOX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MOUOUYJTCQPFCPFMXWQCKIWBKFXTMKMCUNEFQEQOGAKEHMODHRGLONHWMCRN Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.45, 1.5, 1, 2, 3, 1, 4, 4, "MOUOUYJTCQPFCPFMXWQCKIWBKFXTMKMCUNEFQEQOGAKEHMODHRGLONHWMCRN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MPIVFOBHHEHRINUMUXFNKWNAGUFUYPMFRMSWNRDGHVLRWMRRQOIMVCLQMIIO Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.63, -0.19, 0.47, 1, 2, 3, 1, 4, 4, "MPIVFOBHHEHRINUMUXFNKWNAGUFUYPMFRMSWNRDGHVLRWMRRQOIMVCLQMIIO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MQSQNAJVTXENLDPMEUWPPGVLAIGBWOMWTSAFWOBIKWMYQMGBTOVXDILQKJOE Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.98, -0.133, 0.77, 1, 2, 3, 1, 4, 4, "MQSQNAJVTXENLDPMEUWPPGVLAIGBWOMWTSAFWOBIKWMYQMGBTOVXDILQKJOE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MRPEQPUXBARUACIMLYRPCEAAHNRCDJMTTBWAUGSIGONHRMSSSKTJENRLNHIS Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -0.45, 0.81, 1, 2, 3, 1, 4, 4, "MRPEQPUXBARUACIMLYRPCEAAHNRCDJMTTBWAUGSIGONHRMSSSKTJENRLNHIS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MSHIOLQTSKBLPTMMXHSHIETUJVNQHUMNJGXPPOEPESFWIMQIBYNMTVHDIMWD Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.95, -0.53, 1.46, 1, 2, 3, 1, 4, 4, "MSHIOLQTSKBLPTMMXHSHIETUJVNQHUMNJGXPPOEPESFWIMQIBYNMTVHDIMWD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MTRGGKKXNVDIJFJMEIEVINXHQKNGREMFDTYFWSBLCHHRCMFTTJBISMXIMFOL Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, 0.077, 0.42, 1, 2, 3, 1, 4, 4, "MTRGGKKXNVDIJFJMEIEVINXHQKNGREMFDTYFWSBLCHHRCMFTTJBISMXIMFOL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MUCGTWNIGRKIMGYMDCPAPBLHJNHTJOMVFOQMWXNDHAAAEMGYQJFJGKAOACKH Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.47, -0.44, 0.74, 1, 2, 3, 1, 4, 4, "MUCGTWNIGRKIMGYMDCPAPBLHJNHTJOMVFOQMWXNDHAAAEMGYQJFJGKAOACKH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MUDTNUFJXPTSWSIMTPMTLPKKOXFHWHMPMEBAHOSHBEXREMQVUVAJTPSLUAWY Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.34, -0.15, 0.37, 1, 2, 3, 1, 4, 4, "MUDTNUFJXPTSWSIMTPMTLPKKOXFHWHMPMEBAHOSHBEXREMQVUVAJTPSLUAWY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MVMLIKDLYQLMDVFMVKPECJDHLCDYTOMJXOIURJCXPMSEGMPHLESLAQORDPEC Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.72, -0.47, 0.85, 1, 2, 3, 1, 4, 4, "MVMLIKDLYQLMDVFMVKPECJDHLCDYTOMJXOIURJCXPMSEGMPHLESLAQORDPEC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MWDKKHNKMEMAXYGMSYYJMJXQSUCJRXMNIIPFEIQVFPAFSMDWTHJNWILWIVNF Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.34, -0.12, 0.29, 1, 2, 3, 1, 4, 4, "MWDKKHNKMEMAXYGMSYYJMJXQSUCJRXMNIIPFEIQVFPAFSMDWTHJNWILWIVNF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MWHELTGMYANTDKMMMCTWXKGNLBLMCKMCUWBTFKYQJNKHLMFSRVUTQUXLSPGX Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.38, -0.27, 0.5, 1, 2, 3, 1, 4, 4, "MWHELTGMYANTDKMMMCTWXKGNLBLMCKMCUWBTFKYQJNKHLMFSRVUTQUXLSPGX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MWIJHYUXDOIMAYURLHGMNJROEJLFSIMMJYFIIHYJJGSDFGVPAYQSSYGWBQPX Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.32, -0.06, 0.52, 1, 2, 3, 1, 4, 4, "MWIJHYUXDOIMAYURLHGMNJROEJLFSIMMJYFIIHYJJGSDFGVPAYQSSYGWBQPX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MWYKARXJGXRDLMKKESHNSJEQHJRVEERSWWFXEELSGGMUAOEHWYXLMHNLMLON Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.59, -0.35, 0.35, 1, 2, 3, 1, 4, 4, "MWYKARXJGXRDLMKKESHNSJEQHJRVEERSWWFXEELSGGMUAOEHWYXLMHNLMLON", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M MXBCTVQDXMLKMLKMIGWODOXLBKHMTVMKEKXASLXELLAQQMDDPRFUFYVHLSDQ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.58, -0.25, 0.78, 1, 2, 3, 1, 4, 4, "MXBCTVQDXMLKMLKMIGWODOXLBKHMTVMKEKXASLXELLAQQMDDPRFUFYVHLSDQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M NGFOFMKCYIFGJSSOVQKUYKUHTITJWEMUHNPXMFABUWXGAMQNQOXMGQTVJQOA Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.562, -0.235, 0.64, 1, 2, 3, 1, 4, 4, "NGFOFMKCYIFGJSSOVQKUYKUHTITJWEMUHNPXMFABUWXGAMQNQOXMGQTVJQOA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M NGPYOCMJXHJYBRPINWEMXWNDCFTVTEKLKBWHQXQDGBTQDSHGXEXJDSPVFSJM Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.48, -0.3, 0.78, 1, 2, 3, 1, 4, 4, "NGPYOCMJXHJYBRPINWEMXWNDCFTVTEKLKBWHQXQDGBTQDSHGXEXJDSPVFSJM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M OFUUMXCSQOGVSMDRLXJIUCTUGHDYJJJSENYNHFVENLYFJIKPOESTOJJFEEWF Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.088, 0.35, 0.17, 1, 2, 3, 1, 4, 4, "OFUUMXCSQOGVSMDRLXJIUCTUGHDYJJJSENYNHFVENLYFJIKPOESTOJJFEEWF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M PGBTHKTSSWCKASIJIUFPXNLESSLJNUNICHOHEETONYWOGJSWMNXHOXEBLFLG Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.57, -0.44, 0.61, 1, 2, 3, 1, 4, 4, "PGBTHKTSSWCKASIJIUFPXNLESSLJNUNICHOHEETONYWOGJSWMNXHOXEBLFLG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M PGPMWGTAHHNLTBLMELMJBDCXALDYXMUPDLEVAAGYHGXLRRUGOGBABMXJCWNX Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.39, -0.18, 0.54, 1, 2, 3, 1, 4, 4, "PGPMWGTAHHNLTBLMELMJBDCXALDYXMUPDLEVAAGYHGXLRRUGOGBABMXJCWNX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M QCQGOBGVFLHSXPTPDOOVIREBODHGUMPIYTYPHFWBENBNKOFFBLOLOAPMEHUI Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.168, -0.08, 0.35, 1, 2, 3, 1, 4, 4, "QCQGOBGVFLHSXPTPDOOVIREBODHGUMPIYTYPHFWBENBNKOFFBLOLOAPMEHUI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M QVEIINRFUOYTBNCLVDSANJJXUTTSAWFRMGWDETGAKRMRDVAVSIKBLBUWLCHQ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.28, -0.36, 0.755, 1, 2, 3, 1, 4, 4, "QVEIINRFUOYTBNCLVDSANJJXUTTSAWFRMGWDETGAKRMRDVAVSIKBLBUWLCHQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M SOJMJRQYAJSCATEFVCPNKHPIKKNKQOIWADJDWGJBJFISJNSBVNPHWYMOGINL Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -0.52, 0.47, 1, 2, 3, 1, 4, 4, "SOJMJRQYAJSCATEFVCPNKHPIKKNKQOIWADJDWGJBJFISJNSBVNPHWYMOGINL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M UACOEKVOIPOPRTPMOCPMTXVWAWWFGHPFJQVHYQCQOYCWKUEJDPDMAKFRIBQM Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.048, -0.62, 0.36, 1, 2, 3, 1, 4, 4, "UACOEKVOIPOPRTPMOCPMTXVWAWWFGHPFJQVHYQCQOYCWKUEJDPDMAKFRIBQM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"M UDPYXTBXRCCSIDEINXLCHKRYJUJVEASHHHIJTVJFJYVDWOAVJGXGEVPXBQQE Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.21, -0.26, 0.29, 1, 2, 3, 1, 4, 4, "UDPYXTBXRCCSIDEINXLCHKRYJUJVEASHHHIJTVJFJYVDWOAVJGXGEVPXBQQE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapM, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type Q
	{
	"Q AVXVXDXHILACNURDZZAATSXBNVMOWF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.11, -0.78, 0.22, 1, 2, 3, 1, 3, 3, "AVXVXDXHILACNURDZZAATSXBNVMOWF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q CGPVUQQJPHUYULGKOJNBJEAZPENSZH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.9, -6.4, 7.3, 1, 2, 3, 1, 3, 3, "CGPVUQQJPHUYULGKOJNBJEAZPENSZH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q DEIXNUKOGYXVCISQLYFHWQAODFMDJP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.3, -1.4, 5.0, 1, 2, 3, 1, 3, 3, "DEIXNUKOGYXVCISQLYFHWQAODFMDJP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q DIPESXNTJSOUVGMBJGTEQZIMLPUWMW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.8, -3.6, 4.5, 1, 2, 3, 1, 3, 3, "DIPESXNTJSOUVGMBJGTEQZIMLPUWMW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q DPHNYGLPMVYYQOCUIYOLVKFVFIDMIN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.8, -4.6, 8.44, 1, 2, 3, 1, 3, 3, "DPHNYGLPMVYYQOCUIYOLVKFVFIDMIN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q EDWIVONAHLFVWRPYMTDDDLLVUHJKSL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.1, -9.6, 13.0, 1, 2, 3, 1, 3, 3, "EDWIVONAHLFVWRPYMTDDDLLVUHJKSL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q GNSFHVSUHJOBLCJXETSGKQQWWKQPFG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, 0.0, 1.0, 1, 2, 3, 1, 3, 3, "GNSFHVSUHJOBLCJXETSGKQQWWKQPFG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q JFPNELMYBGVUOLEEMJJRICCDKLQMUK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.1, 0.21, 7.2, 1, 2, 3, 1, 3, 3, "JFPNELMYBGVUOLEEMJJRICCDKLQMUK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q JRFPORWBILHPHKQMRUDPUDXIJYGBGF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.28, -5.67, 13.9, 1, 2, 3, 1, 3, 3, "JRFPORWBILHPHKQMRUDPUDXIJYGBGF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q LOTRIDMXRBUSABPKTHBRYGBJUXEAKN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -1.8, 2.6, 1, 2, 3, 1, 3, 3, "LOTRIDMXRBUSABPKTHBRYGBJUXEAKN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q MCMMMWMMMMMiMMLLMMMMMMMNMMMMJM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -55.0, -32.0, 64.0, 1, 2, 3, 1, 3, 3, "MCMMMWMMMMMiMMLLMMMMMMMNMMMMJM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q MMMMMIMMIMMQMMMNMMMMNMMMQMMM0M Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -19.0, 32.0, 1, 2, 3, 1, 3, 3, "MMMMMIMMIMMQMMMNMMMMNMMMQMMM0M", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q QDKWCVBYCTNEUDAXLTTOQEAQJFYHIR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.5, 2.6, 3.4, 1, 2, 3, 1, 3, 3, "QDKWCVBYCTNEUDAXLTTOQEAQJFYHIR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q RREQDTWELEQMTMLAAPRGDJJKLPYAFO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    2.7, 2.3, 2.6, 1, 2, 3, 1, 3, 3, "RREQDTWELEQMTMLAAPRGDJJKLPYAFO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q UCTPNYFKUFMCEWDGLKCVLBOYVZLPLN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.7, -2.4, 4.35, 1, 2, 3, 1, 3, 3, "UCTPNYFKUFMCEWDGLKCVLBOYVZLPLN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Q UKURGZHDOESBKQOJUYROHQYUMXTKEH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, -0.2, -0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -1.8, 2.4, 1, 2, 3, 1, 3, 3, "UKURGZHDOESBKQOJUYROHQYUMXTKEH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapQ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type R
	{
	"R EVUKOGCERMHXEKBCQTJRHCJVXRRIBQAQYHVCXEXIOTVIVKAKKECPWFGPTHXF Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.034206, -0.034206, -0.034206, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.6, 2.58, 1, 2, 3, 1, 3, 3, "EVUKOGCERMHXEKBCQTJRHCJVXRRIBQAQYHVCXEXIOTVIVKAKKECPWFGPTHXF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R EZCDWWWLUDEQLWJPPVSKUWVELXIJLVKHFCIUKVNPWLVEBWUHRVYHGBQMSLOI Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.034206, -0.034206, -0.034206, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.9, 0.06, 2.0, 1, 2, 3, 1, 3, 3, "EZCDWWWLUDEQLWJPPVSKUWVELXIJLVKHFCIUKVNPWLVEBWUHRVYHGBQMSLOI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R FEJMURNKYSKVHFJXVFDMBKSBJFFUUIBTEFUDKLRKSDLTGJBHHGPHUHLQYSPG Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.034206, -0.034206, -0.034206, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -2.8, 5.8, 1, 2, 3, 1, 3, 3, "FEJMURNKYSKVHFJXVFDMBKSBJFFUUIBTEFUDKLRKSDLTGJBHHGPHUHLQYSPG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R FOWMYQVLYQDQSPGYUKWKBKAUXYUSMJLJYFKJPVKEOQPJWBBURCUAMXSNFKYB Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.34206, -0.34206, -0.34206, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -1.5, 0.9, 1, 2, 3, 1, 3, 3, "FOWMYQVLYQDQSPGYUKWKBKAUXYUSMJLJYFKJPVKEOQPJWBBURCUAMXSNFKYB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R FTGHODREUPELRRMEVVELSQBHTDDHYYHFQNAFLQRSEAMVAJGPYWYQBUEHFHRF Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.56, -0.347, 3.7, 1, 2, 3, 1, 3, 3, "FTGHODREUPELRRMEVVELSQBHTDDHYYHFQNAFLQRSEAMVAJGPYWYQBUEHFHRF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},		
	{
	"R HNKDRSEPTGBLEXOWYMEDQCECLQJBHQHMOECLTQTKCFEQWATHMLMIORSHBKNI Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.4, -1.9, 4.8, 1, 2, 3, 1, 3, 3, "HNKDRSEPTGBLEXOWYMEDQCECLQJBHQHMOECLTQTKCFEQWATHMLMIORSHBKNI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R IAFAHLTKWQMGNCDYLEJNEDYULQPHJXKUAMSMDQWKZYDFMVBREESSEGMGOJHK Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.47, 0.12, 1.15, 1, 2, 3, 1, 3, 3, "IAFAHLTKWQMGNCDYLEJNEDYULQPHJXKUAMSMDQWKZYDFMVBREESSEGMGOJHK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R JYQGRWLLATYZGCMHPEZNQNBEDGRLXBSVCZBXHRKLJMERQMCFKTKGKKBLOECH Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.114, -1.48, 2.26, 1, 2, 3, 1, 3, 3, "JYQGRWLLATYZGCMHPEZNQNBEDGRLXBSVCZBXHRKLJMERQMCFKTKGKKBLOECH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R KCDFPFVYNCFPMWIBYKTEQAPMIFHFJXCIJASKNTYYCPTUADVMNSRSJMMSEWSL Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.9, -1.7, 2.5, 1, 2, 3, 1, 3, 3, "KCDFPFVYNCFPMWIBYKTEQAPMIFHFJXCIJASKNTYYCPTUADVMNSRSJMMSEWSL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R LBVMDKPEEHKIDLRFCJPLXOYTVEUPSTSLNJZLANDNYPWYRGCPGRWNUXTWYRVJ Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.54, -0.69, 0.86, 1, 2, 3, 1, 3, 3, "LBVMDKPEEHKIDLRFCJPLXOYTVEUPSTSLNJZLANDNYPWYRGCPGRWNUXTWYRVJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R MGOLOGUGIKKAQUNYYWPNQOPVIXLCONAYJGIROPFQBFMOREQJOOUBXDIVOLYK Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.3, -2.35, 4.4, 1, 2, 3, 1, 3, 3, "MGOLOGUGIKKAQUNYYWPNQOPVIXLCONAYJGIROPFQBFMOREQJOOUBXDIVOLYK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R NDZJLUGWZKQUSBPXESCPJZDIJCEDQEBYGCFPNQIKBYDFYHTOPXMKTZLFHNNN Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -1.15, 2.32, 1, 2, 3, 1, 3, 3, "NDZJLUGWZKQUSBPXESCPJZDIJCEDQEBYGCFPNQIKBYDFYHTOPXMKTZLFHNNN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R NMKSFEKTRDEXQNCWWCSWQECBBFMIXAJOYLCKDIIXRXJHEAEEWJBYBKVXTIFY Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.3, 2.6, 1, 2, 3, 1, 3, 3, "NMKSFEKTRDEXQNCWWCSWQECBBFMIXAJOYLCKDIIXRXJHEAEEWJBYBKVXTIFY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R QPXHNHUENPJJOICSJJMEHIEJEDRVVMSTQASROWWMADDPNGXMCMYFCTRXODNA Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.135, -2.0, 2.9, 1, 2, 3, 1, 3, 3, "QPXHNHUENPJJOICSJJMEHIEJEDRVVMSTQASROWWMADDPNGXMCMYFCTRXODNA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R SCVKBOZJYRMFXUTRKLPCNUEACOFHWFSTPFXNDRKGHVWCTXMLRGXVNCBPEUMH Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.735, -0.38, 0.98, 1, 2, 3, 1, 3, 3, "SCVKBOZJYRMFXUTRKLPCNUEACOFHWFSTPFXNDRKGHVWCTXMLRGXVNCBPEUMH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R VKNTJDLHNLHODWEIGXXHCCOGAZQIFOFXWJPUUFUQOVYSFENYXJSUNDQESHDL Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.26, -2.0, 1.6, 1, 2, 3, 1, 3, 3, "VKNTJDLHNLHODWEIGXXHCCOGAZQIFOFXWJPUUFUQOVYSFENYXJSUNDQESHDL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R XEGQFBYMERYGBWLMXYMXNCMRSTJFUYLIKHXYXVNNFBUKBGFFLSXFPXDHLDHV Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.85, -0.79, 0.63, 1, 2, 3, 1, 3, 3, "XEGQFBYMERYGBWLMXYMXNCMRSTJFUYLIKHXYXVNNFBUKBGFFLSXFPXDHLDHV", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"R YQAALUJBCNPFEHQWYTQIUSPFGLLCGHDXGAJWWOKENEFCYOVHGXGHQXURHTUC Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.53, -2.47, 4.53, 1, 2, 3, 1, 3, 3, "YQAALUJBCNPFEHQWYTQIUSPFGLLCGHDXGAJWWOKENEFCYOVHGXGHQXURHTUC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapR, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type U
	{
	"U BLFBKKFNATJVTJUKJFGALBIPQHVRUMAROTNVHBLAQVSHVHRGLFJAAABFRJFW Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.65, -3.9, 6.4, 1, 2, 3, 1, 4, 4, "BLFBKKFNATJVTJUKJFGALBIPQHVRUMAROTNVHBLAQVSHVHRGLFJAAABFRJFW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U ELMCAPHHLBLVRKPASNCVXJVFUAECFEPGVOCUDIUIKREIEBRUUBRDAJUWWWSX Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -3.3, 3.0, 1, 2, 3, 1, 4, 4, "ELMCAPHHLBLVRKPASNCVXJVFUAECFEPGVOCUDIUIKREIEBRUUBRDAJUWWWSX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U FYVWDQLFQWKLOIJEMACITSWOGCITOAPSKIVTYSYQDYNYBEHIJKTKPRPIAJRU Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.1, -9.7, 4.9, 1, 2, 3, 1, 4, 4, "FYVWDQLFQWKLOIJEMACITSWOGCITOAPSKIVTYSYQDYNYBEHIJKTKPRPIAJRU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U GLICIRJDBAEOQBFPHVDSPDIUVHNRQHCVYAEBCNPSFNBQGAQWQNKJDHQTPQPR Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.375, -0.55, 1.32, 1, 2, 3, 1, 4, 4, "GLICIRJDBAEOQBFPHVDSPDIUVHNRQHCVYAEBCNPSFNBQGAQWQNKJDHQTPQPR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U HKOTCHADGLPGKAPVTDUMJMAHFWLTCKYMUYQDWKMMLLHJCTURPYGAUJGINYVY Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -0.69, 2.4, 1, 2, 3, 1, 4, 4, "HKOTCHADGLPGKAPVTDUMJMAHFWLTCKYMUYQDWKMMLLHJCTURPYGAUJGINYVY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U HLDVFVIDPXYKOEPDTVJCEESBNCCQFCQDBKISIPYHYOAJHHMNCSGCBCTGJHWK Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.1, -6.7, 3.0, 1, 2, 3, 1, 4, 4, "HLDVFVIDPXYKOEPDTVJCEESBNCCQFCQDBKISIPYHYOAJHHMNCSGCBCTGJHWK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U HUPENUENEQIBQOCKEMDVPUCYAIOQJCMJAGNRYHOWLQKWORQCPQIUKCCDWWJA Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, 0.64, 2.8, 1, 2, 3, 1, 4, 4, "HUPENUENEQIBQOCKEMDVPUCYAIOQJCMJAGNRYHOWLQKWORQCPQIUKCCDWWJA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U IDVAPPBAWUENHIRQCTPAKCMLGQESFCTLRCPKOSWLGMENYVHFAEMBNUPQHMLG Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.8, -0.73, 2.4, 1, 2, 3, 1, 4, 4, "IDVAPPBAWUENHIRQCTPAKCMLGQESFCTLRCPKOSWLGMENYVHFAEMBNUPQHMLG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U JKCAWNXIHCYOUPKQBEMUVIEBYFYJTRVXDHCIBIFDEPMJHDBMUVCQDFKXRRFT Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, -0.1, 0.2, 0.3, 0.4, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -2.8, 13.0, 1, 2, 3, 1, 4, 4, "JKCAWNXIHCYOUPKQBEMUVIEBYFYJTRVXDHCIBIFDEPMJHDBMUVCQDFKXRRFT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U KDGJLEXISFBNTDVQVMXQFXCGMQKOSUHDOFPYOOECJEEOPMNLSQCPEYJEYIGO Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.78, 0.4, 7.67, 1, 2, 3, 1, 4, 4, "KDGJLEXISFBNTDVQVMXQFXCGMQKOSUHDOFPYOOECJEEOPMNLSQCPEYJEYIGO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U LYMXJCIVBIBCHWVWSEEKQVKQIOJBQHQSHJOFBPPDWSMBTVSERGGYPPPATWSB Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.18, -1.6, 4.72, 1, 2, 3, 1, 4, 4, "LYMXJCIVBIBCHWVWSEEKQVKQIOJBQHQSHJOFBPPDWSMBTVSERGGYPPPATWSB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U MALDKMYNVYSDIAQVVHAWOJGDLFHKPSQEDJWPSOQCBFHGYMIVIXGXYCITMAXH Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.68, -1.48, 5.5, 1, 2, 3, 1, 4, 4, "MALDKMYNVYSDIAQVVHAWOJGDLFHKPSQEDJWPSOQCBFHGYMIVIXGXYCITMAXH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U RDNTMWHDVMWFNXILJHWBBXSHSSXSYSODWVWJSQMWIITXWHBNDHISNVKJOGWH Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -0.68, 2.2, 1, 2, 3, 1, 4, 4, "RDNTMWHDVMWFNXILJHWBBXSHSSXSYSODWVWJSQMWIITXWHBNDHISNVKJOGWH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U UHXPWNCEPURTFNMIOMYQOVRENSHECMCBGEDFHOKXESOIQNUGLDRHTFVNICEE Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -6.0, 4.52, 1, 2, 3, 1, 4, 4, "UHXPWNCEPURTFNMIOMYQOVRENSHECMCBGEDFHOKXESOIQNUGLDRHTFVNICEE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U UJBACFHDEGOKAPREYHRDQAGWPWEWGNTXXWHPPBRCGFOLHOKQLXBIYJPQYYXL Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.18, -1.0, 2.0, 1, 2, 3, 1, 4, 4, "UJBACFHDEGOKAPREYHRDQAGWPWEWGNTXXWHPPBRCGFOLHOKQLXBIYJPQYYXL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"U WBDELATGLQNRRWJMQMAIRVAIWSKPWVVOCMYQWOCEDNBNMVHPSGRGSYDHADVS Sprott Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", "w", ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.3, 5.8, 5.0, 1, 2, 3, 1, 4, 4, "WBDELATGLQNRRWJMQMAIRVAIWSKPWVVOCMYQWOCEDNBNMVHPSGRGSYDHADVS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapU, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type Y
	{
	"Y AMHDWIODTE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -3.43, 6.9, 1, 2, 3, 1, 3, 3, "AMHDWIODTE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y AQCXBTDRHF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.8, -5.52, 9.44, 1, 2, 3, 1, 3, 3, "AQCXBTDRHF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y BJSRTYCQLD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.26, -1.73, 4.16, 1, 2, 3, 1, 3, 3, "BJSRTYCQLD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y BTDNPHIJTI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -136.0, -44.0, 132.0, 1, 2, 3, 1, 3, 3, "BTDNPHIJTI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y CHTPNOBABB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.36, -0.48, 0.88, 1, 2, 3, 1, 3, 3, "CHTPNOBABB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y CMCCMMWMMM Sprott Fractal Map (Gingerbread Man)", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.5, -8.9, 13.2, 1, 2, 3, 1, 3, 3, "CMCCMMWMMM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y DGPCXGXTIW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.73, -3.15, 3.9, 1, 2, 3, 1, 3, 3, "DGPCXGXTIW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y DNBSGGTQDW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.8, -3.7, 5.0, 1, 2, 3, 1, 3, 3, "DNBSGGTQDW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y EBYSXDGLVB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.7, -5.8, 8.4, 1, 2, 3, 1, 3, 3, "EBYSXDGLVB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y ELRVNTLOBU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -1.64, 4.8, 1, 2, 3, 1, 3, 3, "ELRVNTLOBU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y HKTEILDYRK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.46, 1.37, 0.4, 1, 2, 3, 1, 3, 3, "HKTEILDYRK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y KXILQORBNP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -251.0, -143.0, 295.0, 1, 2, 3, 1, 3, 3, "KXILQORBNP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y LNVRNDFOTX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -1.0, 1.45, 1, 2, 3, 1, 3, 3, "LNVRNDFOTX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y OBLSVULUCA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.56, -0.16, 0.98, 1, 2, 3, 1, 3, 3, "OBLSVULUCA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y OJMOEAKWYV Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.5, 1.4, 1, 2, 3, 1, 3, 3, "OJMOEAKWYV", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y PSRTGNDKND Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.64, -1.75, 3.0, 1, 2, 3, 1, 3, 3, "PSRTGNDKND", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y SMLSCXTGBR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, 0.04, 1.1, 1, 2, 3, 1, 3, 3, "SMLSCXTGBR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y VBTGBLJNUB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.26, -1.52, 3.38, 1, 2, 3, 1, 3, 3, "VBTGBLJNUB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y WOABOXTDFD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.2, -3.59, 5.0, 1, 2, 3, 1, 3, 3, "WOABOXTDFD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Y YEQLKXBMXA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.588, -0.277, 1.45, 1, 2, 3, 1, 3, 3, "YEQLKXBMXA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapY, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type Z
	{
	"Z AMWBPMDUAXGGLB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, 0.82, 2.0, 1, 2, 3, 1, 3, 3, "AMWBPMDUAXGGLB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z BBCMQDQKTJPSUR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -22.0, -12.0, 22.0, 1, 2, 3, 1, 3, 3, "BBCMQDQKTJPSUR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z CBINUXAKUFLEAK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.38, 2.26, 1, 2, 3, 1, 3, 3, "CBINUXAKUFLEAK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z CTFTHMKITVNFFG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.48, -2.25, 1.64, 1, 2, 3, 1, 3, 3, "CTFTHMKITVNFFG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z EIVGPLCKGALTXT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.88, -2.34, 3.92, 1, 2, 3, 1, 3, 3, "EIVGPLCKGALTXT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z FPEIBFLSVWGFPA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.1, -2.0, 3.6, 1, 2, 3, 1, 3, 3, "FPEIBFLSVWGFPA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z HXMLJJRQEDNPRB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.64, -0.24, 3.2, 1, 2, 3, 1, 3, 3, "HXMLJJRQEDNPRB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z IDYKMPGDIGTLSO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.48, -1.47, 2.0, 1, 2, 3, 1, 3, 3, "IDYKMPGDIGTLSO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z KRFMRHHAVUOIKM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -3.3, 4.4, 1, 2, 3, 1, 3, 3, "KRFMRHHAVUOIKM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z LFBPYYLFUKMSED Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.64, -1.56, 3.0, 1, 2, 3, 1, 3, 3, "LFBPYYLFUKMSED", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z MEQGDPOPAFRAES Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.3, -1.75, 4.0, 1, 2, 3, 1, 3, 3, "MEQGDPOPAFRAES", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z MEROSWMYGDQTKO Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.81, -0.55, 2.32, 1, 2, 3, 1, 3, 3, "MEROSWMYGDQTKO", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z MIVEQTDVEEOWVN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.6, 2.0, 1, 2, 3, 1, 3, 3, "MIVEQTDVEEOWVN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z NFEMVRCISEHSIN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.87, -2.84, 5.4, 1, 2, 3, 1, 3, 3, "NFEMVRCISEHSIN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z NVIDLJDFUUQUBK Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -36.0, 40.0, 1, 2, 3, 1, 3, 3, "NVIDLJDFUUQUBK", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z OCGMCVDMPDFMHJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.85, -1.0, 2.13, 1, 2, 3, 1, 3, 3, "OCGMCVDMPDFMHJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z OCYAXYFJKEBRPH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.72, 1.2, 1, 2, 3, 1, 3, 3, "OCYAXYFJKEBRPH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z OFFLRTEFSDFPKP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -2.88, 5.45, 1, 2, 3, 1, 3, 3, "OFFLRTEFSDFPKP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z RLDWMOJKXHBLAN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.83, -0.64, 1.42, 1, 2, 3, 1, 3, 3, "RLDWMOJKXHBLAN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z SVGLBHQEXQQMEE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.5, -0.43, 1.56, 1, 2, 3, 1, 3, 3, "SVGLBHQEXQQMEE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z TXNODBPWLDVKNN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.53, 0.05, 0.8, 1, 2, 3, 1, 3, 3, "TXNODBPWLDVKNN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z VPTKKHLBASNMXF Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.66, -2.23, 1.6, 1, 2, 3, 1, 3, 3, "VPTKKHLBASNMXF", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Z XQUMEEGHXINYBM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.01, 0.02, 0.03, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.368, -0.765, 1.7, 1, 2, 3, 1, 3, 3, "XQUMEEGHXINYBM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMapZ, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type [
	{
	"[ ADXWOPTELDGTHI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -58.0, -36.0, 64.0, 1, 2, 3, 1, 3, 3, "ADXWOPTELDGTHI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ BLWBPPFGISPNVL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.95, 0.88, 1.0, 1, 2, 3, 1, 3, 3, "BLWBPPFGISPNVL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ CBJGRMDVWFYXPI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.3, -8.9, 20.0, 1, 2, 3, 1, 3, 3, "CBJGRMDVWFYXPI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ EQBRGLHUWJGJAJ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -39.0, -23.0, 42.0, 1, 2, 3, 1, 3, 3, "EQBRGLHUWJGJAJ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ IAWNTNHKJOCOSY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.2, 2.7, 1, 2, 3, 1, 3, 3, "IAWNTNHKJOCOSY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ JJMLWFVTUKDTTQ Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.43, -0.335, 1.17, 1, 2, 3, 1, 3, 3, "JJMLWFVTUKDTTQ", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ LDTQTTLAKDRIEN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -39.0, -23.0, 42.0, 1, 2, 3, 1, 3, 3, "LDTQTTLAKDRIEN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ TTKFUVIJVWAJXS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -124.0, -90.0, 142.0, 1, 2, 3, 1, 3, 3, "TTKFUVIJVWAJXS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ TTOLLROXHSWIUS Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    1.59, -0.533, 2.7, 1, 2, 3, 1, 3, 3, "TTOLLROXHSWIUS", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"[ WJSFLSKWACFVHI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -13.0, 28.0, 1, 2, 3, 1, 3, 3, "WJSFLSKWACFVHI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type \ 
	{
	"\\ ASEFAHYBJUVPHIMWYT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -11.0, 21.0, 1, 2, 3, 1, 3, 3, "ASEFAHYBJUVPHIMWYT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ BBTXCFCJUGDJXKONUH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -37.0, -21.0, 42.0, 1, 2, 3, 1, 3, 3, "BBTXCFCJUGDJXKONUH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ CDYVWGQYVUBQOBMVBD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -13.0, 28.0, 1, 2, 3, 1, 3, 3, "CDYVWGQYVUBQOBMVBD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ GHBBADGTAYVMLIBSLG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -68.0, -38.0, 78.0, 1, 2, 3, 1, 3, 3, "GHBBADGTAYVMLIBSLG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ HPBAJVWNAHSWRJWBXM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -12.0, 22.0, 1, 2, 3, 1, 3, 3, "HPBAJVWNAHSWRJWBXM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ IUPGKKXXOTAUMPEPDN Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -111.0, -66.0, 131.0, 1, 2, 3, 1, 3, 3, "IUPGKKXXOTAUMPEPDN", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ JEUALUVCWBLEXYCAVH Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.3, 10.0, 1, 2, 3, 1, 3, 3, "JEUALUVCWBLEXYCAVH", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ JISPFOVANDEBNXUWBY Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.3, -8.23, 15.0, 1, 2, 3, 1, 3, 3, "JISPFOVANDEBNXUWBY", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ JJIASHWXGPXCGROCWG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -8.0, 17.0, 1, 2, 3, 1, 3, 3, "JJIASHWXGPXCGROCWG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ KBDABMSIBRWMQNMUKW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -23.0, -13.0, 27.0, 1, 2, 3, 1, 3, 3, "KBDABMSIBRWMQNMUKW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ LLXGLCOXAJDQJOICVC Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -18.0, 36.0, 1, 2, 3, 1, 3, 3, "LLXGLCOXAJDQJOICVC", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ MEOANCSXDVGCVLGHQT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -15.0, 30.0, 1, 2, 3, 1, 3, 3, "MEOANCSXDVGCVLGHQT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ NBIAYCYGUSVJFXEJGA Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.2, -6.8, 15.0, 1, 2, 3, 1, 3, 3, "NBIAYCYGUSVJFXEJGA", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ NGPKOPDSBDCBGVBBPP Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -35.0, -20.0, 40.0, 1, 2, 3, 1, 3, 3, "NGPKOPDSBDCBGVBBPP", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ RQYUANROUUDKJQSTKU Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -30.0, 60.0, 1, 2, 3, 1, 3, 3, "RQYUANROUUDKJQSTKU", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ SCNPBHGXICIDBPKQSI Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -23.0, -14.0, 28.0, 1, 2, 3, 1, 3, 3, "SCNPBHGXICIDBPKQSI", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ TOFXBCFVQIXXQUWDXB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -22.5, 45.0, 1, 2, 3, 1, 3, 3, "TOFXBCFVQIXXQUWDXB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ TTKFATYUYRXVWLWHQB Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -3.33, 18.0, 1, 2, 3, 1, 3, 3, "TTKFATYUYRXVWLWHQB", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ TXBGOYYMWONUYGMBXX Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -152.0, -91.0, 170.0, 1, 2, 3, 1, 3, 3, "TXBGOYYMWONUYGMBXX", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ UMLPFJYBIWSRAAOSKM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -0.164, 2.2, 1, 2, 3, 1, 3, 3, "UMLPFJYBIWSRAAOSKM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"\\ WJPOONBLHLADVDVCCR Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.7, -5.5, 11.0, 1, 2, 3, 1, 3, 3, "WJPOONBLHLADVDVCCR", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
/*
	// Sprott Map Type ]
	{
	"] DBYHVD Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 1.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "DBYHVD", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"] DTDQCM Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "DTDQCM", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"] ICUEBE Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "ICUEBE", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"] KUTEEL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "KUTEEL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"] PFXOTL Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "PFXOTL", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"] PHXUEG Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "PHXUEG", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"] SRBOSW Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "SRBOSW", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"] XEUOII Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "XEUOII", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	// Sprott Map Type ^
	{
	"^ GHXTOKLXT Sprott Fractal Map", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, ES, 1000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.05, 0.05, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -210.0, -125.0, 181.0, 1, 2, 3, 1, 3, 3, "GHXTOKLXT", 0, YAXIS, 100000.0,
	    NullSetup, DoSprottFractaLMap4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},

*/
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

//////////////////////////////////////////////////////////////////
// database of surfaces
//////////////////////////////////////////////////////////////////

struct OscillatorSpecificStuff SurfaceSpecific[]=
    {
	{
	"4D Klein Bottle",
	    "x start", "x end", "y start", "y end", "p", "r", "e", "Colour Factor", ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 0.333333333, 0.222222222, -0.333333333, 6.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -0.5, 1.0, 1, 2, 3, 8, 4, 4, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, Do4DKleinBottle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Abundant Wave Solutions for Generalized Hietarinta Equation with Hirota's Bilinear Operator",  
	    "x start", "x end", "y start", "y end", "a1", "a2", "b1", "b2", "d1", "Colour Factor", -80.0, 80.0, -60.0, 100.0, 3.0, 0.5, 0.2, 1.0, 1.0, 6.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -121.0, -71.0, 140.0, 1, 3, 2, 10, 3, 3, NULL, 1, YAXIS, 1600.0,
	    SurfaceSetup, DoAbundantWaveSolutions, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.3, 1.0
	},
	{
	"Antisymmetric Torus",
	    "x start", "x end", "y start", "y end", "a", "large radius", "small radius", "Colour Factor", ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 1.2, 6.0, 1.0, 6.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -7.0, 16.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoAntisymmetricTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Apple I Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, 0.0, 2.0*PI, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -10.0, 20.0, 2, 3, 1, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoAppleISurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Apple II Surface", "x start", "x end", "y start", "y end", "R1", "R2", "Colour Factor", ES, ES, ES, -PI, PI, -PI, PI, 5.0, 4.8, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 2, 3, 1, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoAppleIISurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Archimedean Spiral", "x start", "x end", "y start", "y end", "height", "Colour Factor", ES, ES, ES, ES, 0.0, 23.5, 0.0, 1.0, 5.0, 7.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0, -6.0, 30.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoArchimedeanSpiral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Asteroid Ellipsoid", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, 2 * PI, 0.0, 2 * PI, 1.0, 1.0, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -1.3, 2.5, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoAsteroidEllipsoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Asteroid Torus", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 2.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoAsteroidTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Astroceras (Snails and Shells)", "x start", "x end", "y start", "y end", "a", "b", "c", "h", "k", "w", -40.0, -1.0, 0.0, 2*PI, 1.25, 1.25, 1.0, 3.5, 0.0, 0.12,
	    "     N/A", "     N/A", "     N/A", "Direction of Rotation", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -432.0, -200.0, 600.0, 1, 2, 3, 10, 3, 5, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSnailsnShells, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Balls Cylindroid", "x start", "x end", "y start", "y end", "k", "Colour Factor", ES, ES, ES, ES, 0.0, 2.0, 0.0, 2 * PI, 1.0, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -3.0, 6.0, 2, 3, 1, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBallsCylindroid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Banchoff Klein Bottle", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, 2.0, 0.0, 4 * PI, 3.0, 2.0, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.6, -5.0, 10.0, 2, 3, 1, 8, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBanchoffKleinBottle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bell", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, -15.0, 15.0, -15.0, 15.0, 0.2, 16.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -26.0, -10.0, 32.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBell, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bell (polar)", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, 0.0, 2.0*PI, 0.0, 12.0, 0.2, 16.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -100.0, -5.0, 32.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBellPolar, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bell Wave", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, -6.0, 8.0, -2.0*PI, 2.0*PI, 0.2, 1.0, 3.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 2, 3, 8, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoBellWave, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bellerophina (Snails and Shells)", "x start", "x end", "y start", "y end", "a", "b", "c", "h", "k", "w", -10.0, -1.0, 0.0, 2*PI, 0.85, 1.2, 1.0, 0.75, 0.0, 0.06,
	    "     N/A", "     N/A", "     N/A", "Direction of Rotation", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 2, 3, 10, 3, 5, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSnailsnShells, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bent Horns", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -5.0, 5.0, -5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -8.0, 10.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 2000.0,
	    SurfaceSetup, DoBentHorns, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bicorn Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2*PI, 0.0, PI, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -0.8, 2.5, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoBicornSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bicorn Torus I", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 8.0, 3.5, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoBicornTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bicorn Torus II", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 8.0, 3.5, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoBicornTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Binormal Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2 * PI, 0.0, 5.0, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -6.0, 12.0, 1, 3, 2, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoBinormalSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Body of Revolution", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 4.0*PI, 0.0, 2.0*PI, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -5.0, 10.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoBodyRevolution, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bohemian Dome", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 1.0, 2.0, 3.0, 8.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.4, -3.6, 7.2, 2, 1, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoBohemianDome, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bonan-Jeener-Klein Surface 3, 1.5", "x start", "x end", "y start", "y end", "M", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 3.0, 1.5, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBonanJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bonan-Jeener-Klein Surface 4, 1.5", "x start", "x end", "y start", "y end", "M", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 4.0, 1.5, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBonanJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bonan-Jeener-Klein Surface 5, 1.5", "x start", "x end", "y start", "y end", "M", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.5, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.6, -5.0, 10.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBonanJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bonan-Jeener-Klein Surface 6, 1.5", "x start", "x end", "y start", "y end", "M", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 6.0, 1.5, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBonanJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bonan-Jeener-Klein Surface 7, 1.5", "x start", "x end", "y start", "y end", "M", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 7.0, 1.5, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBonanJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Borromean Rings", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 7.0, 1.5, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES,	ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoBorromeanRingsSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bour's Minimal Surface", "x start", "x end", "y start", "y end", "n", "Colour Factor", ES, ES, ES, ES, 0.0, 2*PI, 0.0, 1.0, 4.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES,	ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -246.0, -30.0, 0.6, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBourMinimalSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bow Curve", "x start", "x end", "y start", "y end", "T", "Colour Factor", ES, ES, ES, ES, 0.0, 1.0, 0.0, 1.0, 0.5, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.0, -4.0, 8.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoBowCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bow Tie", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -5.0, 5.0, -5.0, 5.0, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -2.5, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoBowTie, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Boy's Surface", "x factor", "y factor", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 1.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.5, -0.8, 1.6, 1, 2, 3, 3, 3, 3, NULL, 1, YAXIS, 1600.0,
	    SurfaceSetup, DoBoysSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Braided Torus I", "x start", "x end", "y start", "y end", "R", "r", "a", "n", "Colour Factor", ES, 0.0, PI*8.0, 0.0, PI*2.0, 6.0, 1.0, 0.5, 1.25, 6.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoBraidedTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Braided Torus II", "x start", "x end", "y start", "y end", "R", "r", "a", "n", "Colour Factor", ES, 0.0, PI*8.0, 0.0, PI*2.0, 6.0, 1.0, 0.5, 1.5, 6.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoBraidedTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Braided Torus III", "x start", "x end", "y start", "y end", "R", "r", "a", "n", "Colour Factor", ES, 0.0, PI*8.0, 0.0, PI*2.0, 6.0, 1.0, 0.5, 1.75, 6.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoBraidedTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Braided Torus IV", "x start", "x end", "y start", "y end", "R", "r", "a", "n", "Colour Factor", ES, 0.0, PI*8.0, 0.0, PI*2.0, 6.0, 1.0, 0.5, 2.0, 4.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoBraidedTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Braided Torus V", "x start", "x end", "y start", "y end", "R", "r", "a", "n", "Colour Factor", ES, 0.0, PI*8.0, 0.0, PI*2.0, 6.0, 1.0, 0.5, 2.25, 6.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoBraidedTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Breather Surface", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, -20.0, 60.0, -40.0, 40.0, 0.4, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -8.0, 16.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoBreatherSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Bullet Nose", "x start", "x end", "y start", "y end", "a", "b", "Cutoff", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, PI/2, 2.0, 3.0, -12.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -33.0, -10.0, 18.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoBulletNose, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cardioid Torus I", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 10.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoCardioidTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cardioid Torus II", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 10.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoCardioidTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cassinian Oval Torus I", "x start", "x end", "y start", "y end", "R", "a", "b", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 10.0, 2.0, 2.01, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoCassinianOvalTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cassinian Oval Torus II", "x start", "x end", "y start", "y end", "R", "a", "b", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 10.0, 2.0, 2.01, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoCassinianOvalTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Catalan´s Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 8*PI, -1.4, 1.4, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -19.4, -16.2, 32.0, 1, 3, 2, 5, 3, 3, NULL, 1, YAXIS, 1600.0,
	    SurfaceSetup, DoCatalansSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Catalan´s Surface II", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, 0.0, 10.0, -1.0, 1.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -15.0, 25.0, 1, 3, 2, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCatalansSurfaceII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Catenoid", "x start", "x end", "y start", "y end", "c", "Colour Factor", ES, ES, ES, ES, -PI, PI, -PI, PI, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -6.0, 12.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCatenoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cayley Surface", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, -1.5, 1.5, -1.5, 1.5, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -4.0, 8.0, 1, 2, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCayleySurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cissoid Cylinder", "x start", "x end", "y start", "y end", "R", "a", "Colour Factor", ES, ES, ES, -PI, PI, -2.0, 5.0, 3.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCissoidCylinder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Clawson Bottle 4D Volume", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2 * PI, 0.0, 2 * PI, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -4.0, 8.0, 1, 3, 4, 5, 4, 4, NULL, 1 + PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoClawsonBottle4D, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},



	{
	"Cone (Body of Revolution)", "x start", "x end", "y start", "y end", "slope", "Colour Factor", ES, ES, ES, ES, 1.0, 6.0, 0.0, 2*PI, 0.5, 5.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -4.0, 8.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCone, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cornucopia", "x start", "x end", "y start", "y end", "a (sensitive)", "b (sensitive)", "Colour Factor", ES, ES, ES, 0.0, 2*PI, -PI, PI, 1.0, 1.0, 5.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -720.0, -700.0, 1400.0, 2, 1, 3, 7, 3, 3, NULL, 1, YAXIS, 1600.0,
	    SurfaceSetup, DoCornucopia, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cosine Surface I", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.5, 3.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCosineSurfaceI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cosine Surface II", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCosineSurfaceII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cosine Waves", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, -15.0, 15.0, -15.0, 15.0, 1.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCosineWaves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Crescent", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, 1.0, 0.0, 1.0, 3.0, 2.0, 3.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCrescent, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cross Cup", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 2*PI, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -0.8, 2.5, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCrossCup, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Crossed Trough Surface", "x start", "x end", "y start", "y end", "c", "Colour Factor", ES, ES, ES, ES, -PI, PI, -PI, PI, 1.0, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 2, 1, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCrossedTrough, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cuspidal Beaks", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.75, 1.75, -1.75, 1.75, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -14.0, 28.0, 3, 2, 1, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCuspidalBeaks, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cuspidal Butterfly", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.75, 1.75, -1.75, 1.75, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -45.0, 90.0, 3, 1, 2, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCuspidalButterfly, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cuspidal Edge", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 1.0, -1.0, 1.0, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.5, 3.0, 3, 1, 2, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCuspidalEdge, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cylinder", "x start", "x end", "y start", "y end", "r", "Colour Factor", ES, ES, ES, ES, -PI, PI, 0.0, 3.0, 1.0, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -1.0, 4.0, 2, 1, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoCylinder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Cymbelloid Diatom", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI * 2, PI * 2, -PI*2, PI*2, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -1.0, 4.0, 2, 1, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 3000.0,
	    SurfaceSetup, DoCymbelloidDiatom, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dini's Surface", "start", "end", "max angle", "a", "b", "Colour Factor", ES, ES, ES, ES, 0.01, 2.0, 4*PI, 1.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -1.65, 5.0, 1, 2, 3, 6, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoDinisSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.2, 1.0
	},
	{
	"Double Ball", "x start", "x end", "y start", "y end", "R1", "R2", "d", "Colour Factor", ES, ES, 0.0, 2.0*PI, 0.0, 2.0*PI, 6.0, 4.0, 7.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -5.0, 24.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoDoubleBall, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Double Cone", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2.0*PI, -1.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.5, 5.0, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoDoubleCone, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 2.0
	},
	{
	"Drop of Water", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, 0.0, PI, 0.0, 2*PI, 0.5, 1.0, 6.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.5, 3.0, 2, 1, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoDropWater, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Dupin Cyclide", "x start", "x end", "y start", "y end", "a", "b", "c", "d", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 4.0, 1.0, 1.0, 6.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.5, -5.5, 10.0, 2, 1, 3, 9, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoDupinCyclide, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Egg (Body of Revolution)", "x start", "x end", "y start", "y end", "a where (a<=b)", "b", "c", "Colour Factor", ES, ES, 0.0, 3.0, 0.0, 2*PI, 3.0, 4.0, 0.5, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.0, 2.0, 4.4, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoEgg, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Eight Surface",  
	    "x start", "x end", "y start", "y end", "c", "Colour Factor", ES, ES, ES, ES, -PI/2, PI/2, 0.0, 2*PI, 8.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.25, 2.5, 3, 1, 2, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoEightSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Eight Torus",  
	    "x start", "x end", "y start", "y end", "c", "Colour Factor", ES, ES, ES, ES, -PI, PI, -PI, PI, 3.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoEightTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Ellipsoid", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, PI, 2.0, 2.0, 4.0, 6.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoEllipsoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Elliptical Torus", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 2.0, 6.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.5, 7.0, 1, 3, 2, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoEllipticalTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Enneper´s Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -2.0, 2.0, -2.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -8.0, 16.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoEnnepersSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Enneper´s Surface II", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 1.0, -PI, PI, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -5.0, 10.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoEnnepersSurfaceII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Enneper´s Surface III", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -12.0, 24.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoEnnepersSurfaceIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Enneper´s Surface (polar)", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -2.0, 2.0, -2.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -4.8, 9.6, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoEnnepersSurfacePolar, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Epicycloid Cylinder", "x start", "x end", "y start", "y end", "R", "r", "H", "h", "Colour Factor", ES, -PI, PI, 0.0, 1.0, 6.0, 1.0, 4.0, 1.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -124.0, -20.0, 24.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoEpicycloidCylinder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Epicycloid Torus I", "x start", "x end", "y start", "y end", "R1", "R", "r", "h", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 0.333, 0.5, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoEpicycloidTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Epicycloid Torus II", "x start", "x end", "y start", "y end", "R1", "R", "r", "h", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 0.333, 0.2, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoEpicycloidTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Euhoplites (Snails and Shells)", "x start", "x end", "y start", "y end", "a", "b", "c", "h", "k", "w", -40.0, -1.0, 0.0, 2 * PI, 0.6, 0.4, 1.0, 0.9, 0.0, 0.1626,
	    "     N/A", "     N/A", "     N/A", "Direction of Rotation", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -740.0, -436.0, 1000.0, 1, 2, 3, 10, 3, 5, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSnailsnShells, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Evolved Torus", "x start", "x end", "y start", "y end", "n", "a", "Colour Factor", ES, ES, ES, 0.0, 2.0, 0.0, 2.0, 5.0, 3.0, 6.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -160.0, -74.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoEvolvedTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Evolvent Conoid", "x start", "x end", "y start", "y end", "a", "l", "Colour Factor", ES, ES, ES, 0.0, 5 * PI, 0.0, 1.0, 0.02, 1.0, 6.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -62.0, 0.8, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoEvolventConoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Facing Snail", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, -PI/2, PI/2, 0.0, 6.0*PI, 0.2, 6.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.5, 3.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoFacingSnail, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Fano Planes", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -1.1, 2.2, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoFanoPlanes, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Fermat Spiral", "x start", "x end", "y start", "y end", "height", "Colour Factor", ES, ES, ES, ES, -10.0, 10.0, -10.0, 1.0, 0.1, 7.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -3.0, 6.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoFermatSpiral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Fish Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2.0*PI, 0.0, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.75, 1.5, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoFishSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Folium", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.1, 2.2, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoFolium, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Four Intersecting Discs", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI * 4, 0.0, PI * 2, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -65.0, -37.0, 74.0, 2, 1, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoFourIntersectingDiscs, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
//	{
//	"Fresnel Elasticity Area", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, -PI/2, PI/2, -PI, PI, 1.0, 1.0, 2.0, 6.5, 0.0, 0.0,
//	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
//	    -5.0, -4.0, 8.0, 1, 2, 3, 8, 3, 3, NULL, 1, YAXIS, 1500.0,
//	    SurfaceSetup, DoFresnelElasticityArea, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
//	},
	{
	"Funnel", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2.0*PI, 0.1, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.8, -2.5, 5.0, 1, 3, 2, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoFunnel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gaussian Cylinder", "x start", "x end", "y start", "y end", "R", "a", "b", "Colour Factor", ES, ES, -PI, PI, -5.0, 2.0, 2.0, 1.0, 2.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -5.0, 10.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoGaussianCylinder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gear Torus", "x start", "x end", "y start", "y end", "R", "a", "b", "n", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 8.0, 1.0, 2.0, 12.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -8.0, 16.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoGearTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gerono Lemniscate Torus I", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 10.0, 3.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoGeronoLemniscateTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gerono Lemniscate Torus II", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 10.0, 3.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoGeronoLemniscateTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Ghost Plane (Surface is infinite, so boundaries added)", "x start", "x end", "y start", "y end", "Plane Limit", "Colour Factor", ES, ES, ES, ES, -PI, PI, -PI, PI, 20.0, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoGhostPlane, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Goblet", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI*2, 0.0, PI, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.6, -1.5, 3.2, 1, 3, 2, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoGoblet, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gomphonemoid Diatom", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI * 2, PI * 2, -PI * 2, PI * 2, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -1.0, 4.0, 2, 1, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 3000.0,
	    SurfaceSetup, DoGomphonemoidDiatom, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gomphocymbelloid Diatom", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI * 2, PI * 2, -PI * 2, PI * 2, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -21.0, -1.0, 5.0, 2, 1, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 3000.0,
	    SurfaceSetup, DoGomphocymbelloidDiatom, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gray's Klein Bottle", "x start", "x end", "y start", "y end", "a", "m", "n", "Colour Factor", ES, ES, 0.0, 4*PI, 0.0, 2*PI, 3.0, 3.0, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -5.0, 10.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoGraysKleinbottle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Guimard Surface", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 2.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.6, 3.4, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoGuimardSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Gyro", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2.0*PI, -1.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoGyro, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Handkerchief Surface (Saddle)", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 1.0, -1.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -1.5, 3.2, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHandkerchiefSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.4, 1.0
	},
/*
	{
	"Heart Surface I", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 3.0 * PI, 0, PI * 0.9, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -1.5, 3.2, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHeartSurfaceI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
*/	{
	"Helicoid", "x start", "x end", "y start", "y end", "c", "Colour Factor", ES, ES, ES, ES, 0.0, 4.0*PI, -2.0, 2.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.0, -4.0, 8.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHelicoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Henneberg´s Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI/2, PI/2, -1.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -9.0, 18.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHennebergsSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hexagonal Drum", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, PI*2, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.8, 3.6, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHexagonalDrum, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hole Discus", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, 0.0, PI*2, 0.0, PI*2, 2.0, 6.0, 6.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 3, 2, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHoleDiscus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hole Disk", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 3.0, 4.5, 0.0, PI*2, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 3, 2, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHoleDisk, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Horn", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, -PI, PI, 0.0, 1.0, 3.0, 2.0, 3.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -4.0, 12.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHorn, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hornlet", "x start", "x end", "y start", "y end", "a", "b", "c", "h", "Colour Factor", ES, 0, 0.5, 0.0, 1.0, 4.0, 4.0, 4.0, 6.0, 7.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -4.0, 12.0, 1, 2, 3, 9, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHornlet, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperbolic Helicoid", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, -4.0, 4.0, -4.0, 4.0, 5.0, 6.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHyperbolicHelicoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperbolic Octahedron", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI/2, PI/2, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHyperbolicOctahedron, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperbolic Paraboloid", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, -PI/2, PI/2, -PI, PI, 5.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -8.0, 16.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoHyperbolicParaboloid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperbolic Spiral", "x start", "x end", "y start", "y end", "height", "Colour Factor", ES, ES, ES, ES, 0.0, 1.0, 1.1, 60.0, 0.4, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1300.76, -0.54, 1.3, 1, 3, 2, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2400.0,
	    SurfaceSetup, DoHyperbolicSpiral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hyperboloid", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, -PI, PI, -PI, PI, 2.0, 2.0, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -60.0, -33.0, 66.0, 2, 3, 1, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoHyperboloid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hypocycloid Cylinder", "x start", "x end", "y start", "y end", "R", "r", "H", "h", "Colour Factor", ES, -PI, PI, 0.0, 1.0, 6.0, 1.0, 2.0, 1.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -8.0, 16.0, 3, 2, 1, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoHypocycloidCylinder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hypocycloid Torus I", "x start", "x end", "y start", "y end", "R1", "R", "r", "h", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 0.333, 0.5, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoHypocycloidTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Hypocycloid Torus II", "x start", "x end", "y start", "y end", "R1", "R", "r", "h", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 0.333, 1.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoHypocycloidTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Immersion of the Klein Bottle", "x start", "x end", "y start", "y end", "r", "Colour Factor", ES, ES, ES, ES, 0.0, 2 * PI, 0.0, 2 * PI, 3.0, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -3.0, 6.0, 1, 3, 2, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoImmersionOfTheKleinBottle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Insulator (Body of Revolution)", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.5, 12.0, 0.0, 2*PI, 3.0, 0.20, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -5.0, 10.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoInsulator, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Jeener-Klein Surface 1, 3", "x start", "x end", "y start", "y end", "S", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 1.0, 3.0, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.4, -2.0, 4.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Jeener-Klein Surface 2, 4", "x start", "x end", "y start", "y end", "S", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 2.0, 4.0, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.6, -2.5, 5.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Jeener-Klein Surface 3, 5", "x start", "x end", "y start", "y end", "S", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 3.0, 5.0, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.8, -3.5, 7.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Jeener-Klein Surface 4, 5", "x start", "x end", "y start", "y end", "S", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 4.0, 5.0, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Jeener-Klein Surface 5, 6", "x start", "x end", "y start", "y end", "S", "T", "a", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 6.0, 1.0, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.5, -5.0, 10.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoJeenerKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Jet Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, 2*PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -4.0, 8.0, 3, 2, 1, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoJetSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kai-Wing Fung 2 Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 1.0, 0.0, PI * 6, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    0.0, -4.0, 8.0, 3, 2, 1, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoKaiWingFung2Surface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kappa Surface", "x start", "x end", "y start", "y end", "a", "Cutoff", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.05, PI/2, 5.0, -24.0, 6.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -333.0, -18.0, 36.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoKappaSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kidney Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2.0*PI, -PI/2.0, PI/2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.5, 7.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoKidneySurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kinky Torus", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI*2.0, PI*2.0, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.7, 3.5, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoKinkyTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Klein Bottle", "start", "end", "max angle", "a", "b", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 2*PI, 0.3, 8.0, 1.0, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -10.0, 20.0, 2, 1, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoKleinBottle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Klein Bottle (Nordstrand)", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2 * PI, -2 * PI, 2 * PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.0, 6.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoKleinBottleNordstrand, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Klein Cycloid I", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, 4*PI, 0.0, 4*PI, 10.0, 3.0, 2.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 2, 1, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoKleinCycloid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Klein Cycloid II", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, 4*PI, 0.0, 4*PI, 10.0, 5.0, 2.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 2, 1, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoKleinCycloid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Klein Cycloid III", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, 4*PI, 0.0, 4*PI, 10.0, 7.0, 2.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 2, 1, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoKleinCycloid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Klein Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 2 * PI, -1.0, 2 * PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -7.0, 14.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoKleinSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Knitterkugel (Crease Ball)", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, 2 * PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.1, -1.4, 2.8, 2, 1, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoKnitterkugel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Knotted Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2 * PI, 0.0, 5.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.1, -2.5, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoKnottedSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kreisel (Spinning Top) I", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2.0*PI, -1.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 8, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoGyro, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kreisel (Spinning Top) II", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, 0.0, PI, -PI * 2.0, PI * 2.0, 1.2, 7.0,  0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -1.2, 2.4, 1, 3, 2, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoKreiselSurfaceII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kuen's Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.03, 3.11, -4.3, 4.3, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.6, -2.0, 5.0, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoKuensSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Kyle Havens Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -2.0, 2.0, -2.0, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -12.0, 24.0, 1, 3, 2, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoKyleHavensSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lawson Bottle", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2 * PI, 0.0, 2 * PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.5, -42.0, 8.0, 2, 1, 3, 5, 4, 4, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoLawsonBottle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lemniscape", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.0, 2.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoLemniscape, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lemniscate Cylinder", "x start", "x end", "y start", "y end", "r", "H", "Colour Factor", ES, ES, ES, -PI, PI, 0.0, 1.0, 6.0, 3.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -8.0, 16.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoLemniscateCylinder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lemniscate Torus I", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoLemniscateTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lemniscate Torus II", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoLemniscateTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Lemon", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 3.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.8, -4.0, 8.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoLemon, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Limpet Torus", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -3.0, 6.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoLimpetTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Logarithmic Spiral", "x start", "x end", "y start", "y end", "a", "height", "Colour Factor", ES, ES, ES, -1.0, 60.0, -1.0, 1.0, 0.05, 2.0, 8.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -15.0, -7.0, 30.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoLogarithmicSpiral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Loop or Ribbon", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.2, -2.5, 5.0, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoLoop, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Maeder´s Owl", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.001, 1.0, 0.0, 4*PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.8, 3.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoMaedersOwl, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Menn´s Surface", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, -1.0, 1.0, -1.0, 1.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.4, -1.8, 3.0, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoMennsSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Milk Carton", "x start", "x end", "y start", "y end", "k", "Colour Factor", ES, ES, ES, ES, -PI, PI, -1.0, 1.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoMilkCarton, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Möbius Band", "y start", "y end", "radius", "width", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2*PI, 2.0, 0.5, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoMöbiusBand, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Monkey Saddle", "y start", "y end", "radius", "width", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 1.0, -1.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoMonkeySaddle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.6, 1.0
	},
	{
	"Mya Arenaria (Snails and Shells)", "x start", "x end", "y start", "y end", "a", "b", "c", "h", "k", "w", -1.0, 0.52, 0.0, 2*PI, 0.85, 1.6, 3.0, 0.9, 0.0, 2.5,
	    "     N/A", "     N/A", "     N/A", "Direction of Rotation", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -160.0, -84.0, 175.0, 2, 1, 3, 10, 3, 5, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSnailsnShells, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Natica Stellata (Snails and Shells)", "x start", "x end", "y start", "y end", "a", "b", "c", "h", "k", "w", -20.0, 1.0, 0.0, 2*PI, 2.6, 2.4, 1.0, 1.25, -2.8, 0.18,
	    "     N/A", "     N/A", "     N/A", "Direction of Rotation", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -300.0, -300.0, 360.0, 1, 2, 3, 10, 3, 5, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSnailsnShells, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Nautilus (Snails and Shells)", "x start", "x end", "y start", "y end", "a", "b", "c", "h", "k", "w", -20.0, 1.0, 0.0, 2*PI, 1.0, 0.6, 1.0, 1.0, 0.0, 0.18,
	    "     N/A", "     N/A", "     N/A", "Direction of Rotation", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -70.0, -38.0, 82.0, 1, 2, 3, 10, 3, 5, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSnailsnShells, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Naviculoid Diatom", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI * 2, PI * 2, -PI * 2, PI * 2, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -1.0, 4.0, 2, 1, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 3000.0,
	    SurfaceSetup, DoNaviculoidDiatom, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Neovius Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -23.0, -3.2, 8.0, 2, 1, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoNeoviusSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Nephroid Torus I", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 0.4, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoNephroidTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Nephroid Torus II", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 0.4, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoNephroidTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Normal Torus",  
	    "x start", "x end", "y start", "y end", "large radius", "small radius", "Colour Factor", ES, ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 3.0, 1.0, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoNormalTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Oloid (two discs which are inserted into each other at 90°)", 
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -1.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoOloid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Paper Bag",  
	    "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, 0.0, PI*2.0, 0.0, 2.0, 4.0, 2.0, 6.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -15.0, 23.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoPaperBag, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Parabolic Humming Top",
	    "x start", "x end", "y start", "y end", "h", "p", "Colour Factor", ES, ES, ES, 0.0, PI*2.0, 0.0, PI, 3.25, 1.4, 6.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 2, 3, 1, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoParabolicHummingTop, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Paraboloid",  
	    "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, -PI, PI, 0.0, PI, 1.0, 1.0, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -2.6, 16.0, 2, 3, 1, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoParaboloid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Pillow Shape",  
	    "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, 0.0, PI, -PI, PI, 0.4, 6.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 1, 3, 2, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoPillowShape, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Piriform Surface", "x start", "x end", "y start", "y end", "a", "b", "r", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 7.0, 5.0, 0.5, 6.5, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoPiriformSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Piriform Torus I", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 7.0, 1.5, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoPiriformTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Piriform Torus II", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 7.0, 1.5, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoPiriformTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Pisot Triaxial",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.5, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoPisotTriaxial, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Plücker´s Conoid", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 1.0, -2.0, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.3, 4.6, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoPlückersConoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Product Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI / 2, PI / 2, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -60.0, 110.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoProductSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Pseudodevelopable Helicoid", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, -1.5, -0.5, 0.0, PI * 4, 0.2, 0.2, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -7.5, 5.0, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoPseudodevelopableHelicoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Pseudo Cross Cap", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -1.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoPseudoCrossCap, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Pseudocatenoid (Surface is infinite, so boundaries added)", "x start", "x end", "y start", "y end", "Plane Limit", "Colour Factor", ES, ES, ES, ES, -PI, PI, -PI, PI, 3.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -7.0, 14.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoPseudocatenoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Pseudoheliceras Subcatenatum (Snails and Shells)", "x start", "x end", "y start", "y end", "a", "b", "c", "h", "k", "w", -50.0, -1.0, 0.0, 2*PI, 1.6, 1.6, 1.0, 1.5, -7.0, 0.075,
	    "     N/A", "     N/A", "     N/A", "Direction of Rotation", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 1.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -482.0, -436.0, 552.0, 1, 2, 3, 10, 3, 5, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoSnailsnShells, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Pseudosphere", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.1, 3.05, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoPseudosphere, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Rastrigin Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -2 * PI, 2 * PI, 0.0, 2*PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 1, 3, 2, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoRastriginSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.2, 1.0
	},
	{
	"Ribbon", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.8, -3.0, 6.0, 1, 2, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoRibbon, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Richmond Surface (Surface is infinite, so boundaries added)", 
	    "x start", "x end", "y start", "y end", "Plane Limit", "Colour Factor", ES, ES, ES, ES, -1.0, 1.0, -1.0, 1.0, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -2.0, 4.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoRichmondSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Richmond Surface III",
	    "x start", "x end", "y start", "y end", "n", "Colour Factor", ES, ES, ES, ES, 0.4, 1.2, 0.0, 2 * PI, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -2.0, 4.0, 1, 2, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoRichmondSurfaceIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Rounded Cube", "N", "Power that controls corner strength", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, 11.0, 0.2, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -57.0, -2.6, 4.4, 1, 2, 3, 3, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoRoundedCube, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Roman Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI*2.0, 0.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.8, -1.2, 2.4, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoRomanSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Rosenbrock Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -21.0, -2.57, 24.0, 1, 3, 2, 6, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoRosenbrockSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 0.001, 1.0
	},
	{
	"Ruled Rotor Cylindroid", "x start", "x end", "y start", "y end", "a", "b", "n", "Colour Factor", ES, ES, 0.0, PI*2.0, 0.5, 2.0, 1.0, 2.0, 3.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.3, -6.2, 8.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 3000.0,
	    SurfaceSetup, DoRuledRotorCylindroid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Saddle Torus", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.8, -3.6, 7.2, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSaddleTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Scherk Surface",  
	    "x start", "x end", "y start", "y end", "c", "Colour Factor", ES, ES, ES, ES, -PI, PI, -PI, PI, 3.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.2, -5.0, 10.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoScherkSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.3
	},
	{
	"Scherk Surface II",
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2 * PI, 0.0, 0.75, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.2, -5.0, 10.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoScherkSurfaceII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Schnecke (Snail)", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, 0.0, PI*2.0, 0.0, 3.0, 2.0, 3.0, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -9.3, 11.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoSchnecke, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Schnecke II (Snail)", "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, 0.0, PI, -PI * 2.0, PI * 2.0, 1.2, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -2.4, 6.0, 1, 2, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSchneckeII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Seashell", "x start", "x end", "y start", "y end", "a (width)", "b (height)", "c (inner radius)"," n (number of turns)", "Colour Factor", ES, 
	    0.0, 2.0, 0.0, 2.0, 1.0, 10.0, 1.0, 4.0, 7.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -2.0, 15.0, 1, 2, 3, 9, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoSeashell, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Shoe Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -2.0, 2.0, -2.0, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.2, -5.0, 8.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoShoeSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sievert Surface", "x start", "x end", "y start", "y end", "C", "Colour Factor", ES, ES, ES, ES, 0.105, 3.037, -PI/2, PI/2, 5.0, 6.5, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -2.6, 3.2, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSievertSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sine Cone", "x start", "x end", "y start", "y end", "n", "k", "Colour Factor", ES, ES, ES, -PI, PI, -10.0, 10.0, 2.0, 1.0, 6.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSineCone, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sine Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSineSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sinewurfel (Sine Cube)", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0, 2 * PI, 0, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSinewurfel, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Singularity Surface A1", "x start", "x end", "y start", "y end", "Type ", "Colour Factor", ES, ES, ES, ES, -3.0, 3.0, -3.0, 3.0, 0.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -5.4, 10.0, 2, 1, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSingularitySurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Singularity Surface A2--", "x start", "x end", "y start", "y end", "Type ", "Colour Factor", ES, ES, ES, ES, -3.0, 3.0, -3.0, 3.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.7, -22.4, 8.0, 2, 1, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSingularitySurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, -1.0, 1.0
	},
	{
	"Singularity Surface A2+-", "x start", "x end", "y start", "y end", "Type ", "Colour Factor", ES, ES, ES, ES, -3.0, 3.0, -3.0, 3.0, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -5.4, 10.0, 2, 1, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSingularitySurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, -1.0, 1.0
	},
	{
	"Singularity Surface A3--", "x start", "x end", "y start", "y end", "Type ", "Colour Factor", ES, ES, ES, ES, -3.0, 3.0, -3.0, 3.0, 3.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.8, -5.4, 10.0, 1, 2, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSingularitySurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Skidan Ruled Surface", "x start", "x end", "y start", "y end", "a", "h", "n", "Colour Factor", ES, ES, 0.0, 2 * PI, 0.0, 4.0, PI / 4, 2.0, 4.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -9.6, -3.2, 10.0, 1, 2, 3, 8, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSkidanRuledSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Slippers Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.7, -0.7, 1.6, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSlippersSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sine Waves", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, -15.0, 15.0, -15.0, 15.0, 2.0, 2.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 1, 3, 2, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSineWaves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Snail Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -11.0, -6.0, 12.0, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSnailSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Soucoupoid", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -1.8, 3.6, 2, 3, 1, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSoucoupoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sphere I", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSphereI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sphere II", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSphereII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sphere III", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -10.0, 10.0, -10.0, 10.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSphereIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Sphere IV", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -100.0, 100.0, -100.0, 100.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 2, 1, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 3600.0,
	    SurfaceSetup, DoSphereIV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spherical Frustum", "x start", "x end", "d", "h", "a", "Colour Factor", ES, ES, ES, ES, 0.0, PI*2.0, 0.2, 0.4, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.3, 2.6, 2, 1, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 3600.0,
	    SurfaceSetup, DoSphericalFrustum, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spherical Harmonics I", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "m0", "m1", "m2", "m3", "m4", "m5", "m6", "m7", ES, ES, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0,
	    -12.0, -4.5, 9.0, 2, 1, 3, 5, 3, 8, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSphericalHarmonics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spherical Harmonics II", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "m0", "m1", "m2", "m3", "m4", "m5", "m6", "m7", ES, ES, 1.0, 3.0, 2.0, 4.0, 5.0, 7.0, 6.0, 8.0, 0.0, 0.0,
	    -12.0, -4.5, 9.0, 2, 1, 3, 5, 3, 8, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSphericalHarmonics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spherical Harmonics III", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "m0", "m1", "m2", "m3", "m4", "m5", "m6", "m7", ES, ES, 4.0, 2.0, 8.0, 6.0, 1.0, 3.0, 5.0, 7.0, 0.0, 0.0,
	    -12.0, -4.5, 9.0, 2, 1, 3, 5, 3, 8, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSphericalHarmonics, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spherical Rose", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 1.0, 0.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	   "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.8, -0.35, 0.7, 1, 3, 2, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSphericalRose, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spindle Surface", "x start", "x end", "y start", "y end", "a", "c", "Colour Factor", ES, ES, ES, -PI, PI, -PI, PI, 1.0, 1.0, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -4.5, 9.0, 2, 1, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSpindelSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spiral Waves", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, 0.0, PI*2.0, 0.0, 20.0, 2.0, 4.0, 2.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -25.0, 50.0, 2, 1, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSpiralWaves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spiral Torus", "x start", "x end", "y start", "y end", "R1", "R2", "r", "n", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 0.333, 5.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSpiralTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Spring", "x start", "x end", "y start", "y end", "R", "r", "L", "n", "Colour Factor", ES, 0.0, PI*2.0, 0.0, PI*2.0, 12.0, 2.0, 2.0, 4.0, 8.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -33.0, -20.0, 40.0, 3, 1, 2, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2000.0,
	    SurfaceSetup, DoSpring, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Steinbach Screw", "start", "end", "max angle", "Colour Factor", ES, ES, ES, ES, ES, ES, -4.0, 4.0, 2*PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 2, 3, 1, 4, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoSteinbachScrew, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Stiletto Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, 2*PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.6, -1.0, 1.2, 2, 3, 1, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2400.0,
	    SurfaceSetup, DoStilettoSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.6
	},
	{
	"Strangled Torus", "x start", "x end", "y start", "y end", "R", "r", "n", "Colour Factor", ES, ES, 0.0, 2*PI, 0.0, 2*PI, 3.0, 1.0, 7.0, 10.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -3.0, 6.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 2400.0,
	    SurfaceSetup, DoStrangledTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Strophoid Cylinder", "x start", "x end", "y start", "y end", "R", "a", "Colour Factor", ES, ES, ES, -PI, PI, -2.0, 2.0, 6.0, -2.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -8.0, 16.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoStrophoidCylinder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Super Shape I", "x start", "x end", "y start", "y end", "m", "n1", "n2", "n3", "Colour Factor", ES, -PI/2, PI/2, -PI, PI, 6.0, 0.2, 1.7, 1.7, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -14.0, -3.0, 6.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSuperShape, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Super Shape II", "x start", "x end", "y start", "y end", "m", "n1", "n2", "n3", "Colour Factor", ES, -PI/2, PI/2, -2*PI, 2*PI, 5.2, 0.04, 1.7, 1.7, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -700.0, -200.0, 400.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSuperShape, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Super Torus", "x start", "x end", "y start", "y end", "R", "r", "n1", "n2", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 6.0, 2.0, 2.0, 2.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -24.0, -8.0, 16.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoSuperTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Swallow Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -0.75, 0.75, -0.75, 0.75, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.2, -1.4, 3.5, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoSwallowSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.6
	},
	{
	"Tanh Spiral", "x start", "x end", "y start", "y end", "a", "height", "Colour Factor", ES, ES, ES, -2.0, 2.0, -2.0, 1.0, 4.0, 0.1, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -0.5, -2.2, 3.5, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoTanhSpiral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tetrahedral Ellipse", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 1.0, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -34.5, -5.0, 10.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoTetrahedralEllipse, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tooth Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI / 2, 0.0, 2*PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.5, -1.8, 3.6, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoToothSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tortuous Evolved Toroid", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, 2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -81.0, -35.0, 70.0, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 2500.0,
	    SurfaceSetup, DoTortuousEvolvedToroid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Torus Nodes", "x start", "x end", "y start", "y end", "R1", "R2", "r", "p", "q", "Colour Factor", 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 0.333, 7.0, 2.0, 8.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 10, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoTorusNodes, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tractrix Based Minimal Surface", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.5, 4.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoTractrixBasedMinimalSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tractroid", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 4.0, 0.0, 2*PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.6, -1.7, 3.5, 2, 3, 1, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTractroid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.6
	},
	{
	"Tranguloid Trefoil", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoTranguloidTrefoil, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Trash Can", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2.0, 1.0, 2.0, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 2, 3, 1, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTrashCan, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Trefoil Knot I", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 12*PI, 0.0, 2*PI, 6.0, 1.0, 8.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -10.0, 20.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1800.0,
	    SurfaceSetup, DoTrefoilKnotI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Trefoil Knot II", "x start", "x end", "y start", "y end", "R", "r", "a", "n", "Colour Factor", ES, 0.0, 2*PI, 0.0, 4*PI, 6.0, 2.0, 1.0, 3.0, 8.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -36.0, -20.0, 40.0, 3, 1, 2, 9, 3, 3, NULL, 1, XAXIS, 800.0,
	    SurfaceSetup, DoTrefoilKnotII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Triaxial Hexatorus", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.2, -3.0, 6.0, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoTriaxialHexatorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Triaxial Teardrop", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI, 0.0, PI*2, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.4, -1.0, 2.0, 1, 3, 2, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTriaxialTeardrop, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Triaxial Tritorus", "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.5, -2.5, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTriaxialTritorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tricuspoid Torus I", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoTricuspoidTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Tricuspoid Torus II", "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoTricuspoidTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Triple Corkscrew I", "x start", "x end", "y start", "y end", "R", "r", "L", "n", "Colour Factor", ES, -1.0, 1.0, -1.0, 1.0, 6.0, 4.0, 20.0, 4.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -25.0, -15.0, 30.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoTripleCorkscrewI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Triple Corkscrew II", "x start", "x end", "y start", "y end", "R", "r", "L", "n", "a", "b", -1.0, 1.0, -1.0, 1.0, 6.0, 4.0, 20.0, 6.0, 1.0, 4.0,
	    "     N/A", "     N/A", "     N/A", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -42.0, -24.0, 48.0, 1, 2, 3, 10, 3, 4, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoTripleCorkscrewII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Triple Corkscrew III", "x start", "x end", "y start", "y end", "R", "r", "L", "n", "Colour Factor", ES, -1.0, 1.0, -1.0, 1.0, 6.0, 4.0, 20.0, 8.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -150.0, -40.0, 32.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoTripleCorkscrewIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Triple Point Twist", "x start", "x end", "y start", "y end", "c", "Colour Factor", ES, ES, ES, ES, -1.9, 1.5, -2.0, 1.7, -2.0, 7.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.5, -3.0, 6.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTriplePointTwist, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.5
	},
	{
	"Twisted Eight Torus",  
	    "x start", "x end", "y start", "y end", "R", "r", "Colour Factor", ES, ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 3.0, 1.0, 7.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTwistedEightTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Twisted Heart",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, 0.0, PI*2.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -7.0, 14.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTwistedHeart, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Twisted Pipe Surface",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.6, -3.2, 6.4, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoTwistedPipeSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Twisted Plane Surface",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.0, 1.0, -1.0, 1.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.38, 2.7, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoTwistedPlaneSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Twisted Ribbon Surface",
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.9, 7.4, 1, 2, 3, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTwistedRibbonSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Twisted Sphere",  
	    "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, -PI, PI, 0.0, PI*2.0, 4.0, 2.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -5.0, 22.0, 1, 2, 3, 7, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoTwistedSphere, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Twisted Triaxial",  "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.3, -0.7, 1.8, 1, 2, 3, 5, 3, 3, NULL, 1, YAXIS, 1500.0,
	    SurfaceSetup, DoTwistedTriaxial, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Two Torus",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, -PI, PI, 6.5, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoTwoTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Umbilical Torus",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -10.0, 22.0, 2, 3, 1, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoUmbilicalTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Umbrella Surface", "x start", "x end", "y start", "y end", "R", "h", "n", "Colour Factor", ES, ES, 0.0, PI * 2, 0.0, 1.0, 1.2, 0.6, 8.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.0, -1.0, 2.0, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoUmbrellaSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Vase and Spear Head", "x start", "x end", "y start", "y end", "a", "b", "Colour Factor", ES, ES, ES, 0.0, PI * 2, -1.015, 1.015, 2.0, 0.03, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -5.0, 10.0, 1, 3, 2, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1200.0,
	    SurfaceSetup, DoVaseAndSpearHead, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Verrill Surface",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 2.0, PI, 0.5, 3.0*PI, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -16.0, -8.0, 16.0, 2, 3, 1, 5, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoVerrillSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Versiera of the Agnesi Cylinder", "x start", "x end", "y start", "y end", "R", "a", "Colour Factor", ES, ES, ES, -PI, PI, -3.0, 3.0, 3.0, 1.0, 6.5, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -20.0, -12.0, 24.0, 1, 2, 3, 7, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoVersieraAgnesiCylinder, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Virich Cyclic Surface I", "x start", "x end", "y start", "y end", "a", "b", "c", "d", "Colour Factor", ES, 0.0, PI * 2, 0.0, PI * 2, 1.5, 3.0, 2.0, 4.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -7.5, 15.0, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoVirichCyclicSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Virich Cyclic Surface II", "x start", "x end", "y start", "y end", "a", "b", "c", "d", "Colour Factor", ES, 0.0, PI * 2, 0.0, PI * 2, 2.0, 3.0, 1.0, 4.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -10.0, -7.5, 15.0, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoVirichCyclicSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Virich Cyclic Surface III", "x start", "x end", "y start", "y end", "a", "b", "c", "d", "Colour Factor", ES, 0.0, PI * 2, 0.0, PI * 2, 0.75, 3.0, 1.0, 4.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -21.0, -14.0, 28.0, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoVirichCyclicSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Virich Cyclic Surface IV", "x start", "x end", "y start", "y end", "a", "b", "c", "d", "Colour Factor", ES, 0.0, PI * 2, 0.0, PI * 2, 1.5, 3.0, 2.0, 3.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.8, -3.5, 7.0, 1, 2, 3, 7, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoVirichCyclicSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Wallis´s Conical Edge", "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, -1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 2.0, 7.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.6, -1.0, 2.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoWallisConicalEdge, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Wave Ball",  
	    "x start", "x end", "y start", "y end (set to 5 to see structure)", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 14.5, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -34.0, -18.0, 36.0, 2, 3, 1, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoWaveBall, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Wave Torus",  
	    "x start", "x end", "y start", "y end", "R", "r", "a", "n", "Colour Factor", ES, 0.0, PI*2.0, 0.0, PI*2.0, 12.0, 2.0, 1.0, 4.0, 7.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -34.0, -18.0, 36.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoWaveTorus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Waves",  
	    "x start", "x end", "y start", "y end", "a", "b", "c", "Colour Factor", ES, ES, -PI*2.0, PI*2.0, -PI*2.0, PI*2.0, 2.0, 1.0, 1.0, 6.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -13.0, -8.0, 16.0, 1, 2, 3, 8, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoWaves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Wavy Enneper´s Surface",
	    "x start", "x end", "y start", "y end", "s", "Colour Factor", ES, ES, ES, ES, -PI / 2.0, PI/2.0, -1.0, 1.0, 2.0, 1.0, 1.0, 6.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.9, 5.8, 1, 2, 3, 6, 3, 3, NULL, 1 + PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoWavyEnnepersSurface, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Wavy Torus I", "x start", "x end", "y start", "y end", "R1", "R2", "r", "n", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 0.333, 5.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoWaveyTorusI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Wavy Torus II", "x start", "x end", "y start", "y end", "R1", "R2", "r", "n", "Colour Factor", ES, 0.0, 2*PI, 0.0, 2*PI, 5.0, 1.0, 0.333, 5.0, 6.5, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -8.0, -5.0, 10.0, 1, 2, 3, 9, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoWaveyTorusII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"What is the Lowest Order of the Fractional-Order Chaotic Systems to Behave Chaotically...?", 
	    "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, 0.0, 1.0, 0.0, 2*PI, 1.0, 8.0, 0.0, 0.0, 0.0, 0.0,
	    "x", "y", "z", ES, ES, ES, ES, ES, ES, ES, 0.1, 0.1, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.5, -1.3, 2.6, 1, 3, 2, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    NullSetup, DoWhatLowestOrderFractionalOrderChaoticSystems, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Whitney Umbrella",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, -1.5, 1.5, -1.5, 1.5, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.0, -2.5, 5.0, 2, 3, 1, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoWhitneyUmbrella, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Wreath",  
	    "x start", "x end", "y start", "y end", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI*2.0, 0.0, PI*2.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1.2, -0.7, 1.4, 1, 2, 3, 5, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1600.0,
	    SurfaceSetup, DoWreath, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 1.0
	},
	{
	"Zindler´s Conoid",  
	    "x start", "x end", "y start", "y end", "a", "Colour Factor", ES, ES, ES, ES, -0.74, 0.74, -0.74, 0.74, 2.4, 6.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.2, -1.5, 3.0, 1, 2, 3, 6, 3, 3, NULL, 1+PERSPECTIVE, YAXIS, 1500.0,
	    SurfaceSetup, DoZindlersConoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.0, 1.0, 0.07
	},
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
//////////////////////////////////////////////////////////////////
// database of Knots
//////////////////////////////////////////////////////////////////

struct OscillatorSpecificStuff KnotSpecific[]=
    {
	{
	"An Introductions to Harmonic Knots - Generic knots - page 359", "start", "end", "p", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, 0.0, PI * 2, 4.0, 10.0, 12.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 2, 3, 5, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoAnIntroductions2HarmonicKnots, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Borromean Rings", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0, PI*2, 10.0, 12.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.0, 4.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoBorromeanRings, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Cinquefoil Knot", "start", "end", "k", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, 0.0, 10*PI, 2.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.5, 7.0, 1, 2, 3, 5, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoCinquefoilKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Decorative Knot Patterns - Compound Pattern 3", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.3, -2.5, 5.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoDecorativeKnotPatternsCompoundPattern3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 3.0
	},
	{
	"Decorative Knot Patterns - Four Main Loops", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoDecorativeKnotPatternsFourMainLoops, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 3.0
	},
	{
	"Decorative Knot Patterns - Inner Loop Pattern 5", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.2, 4.4, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoDecorativeKnotPatternsInnerLoopPattern5, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 3.0
	},
	{
	"Decorative Knot Patterns - Inner Loop Pattern 3", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -3.6, -2.2, 4.4, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoDecorativeKnotPatternsInnerLoopPattern3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 3.0
	},
	{
	"Decorative Knot Patterns - Inner Loop Pattern 6", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -5.0, -3.0, 6.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoDecorativeKnotPatternsInnerLoopPattern6, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Decorative Knot Patterns - Three Main Loops", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoDecorativeKnotPatternsThreeMainLoops, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Decorative Knot Patterns - Torus Knots Alpha", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 4.0, 25.0, 3.0, 1.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -50.0, -30.0, 60.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Decorative Knot Patterns - Torus Knots Alpha - 2", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 6.0, 12.0, 14.0, 5.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -32.0, -20.0, 40.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Eight Knot", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -30.0, -18.0, 36.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoEightKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Fibonacci Fourier Knots", "start", "end", "Fibonacci number", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, -PI, PI, 5.0, 10.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 3, 2, 5, 6, 6, NULL, 1, YAXIS, 3600.0,
	    NullSetup, DoFibonacciFourierKnots, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Fourier Knot", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoFourierKnotI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Fourier Knot III", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -2.0, -1.2, 2.4, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoFourierKnotIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Granny Knot", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -125.5, 250.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoGrannyKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 3-1", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -360.0, -220.0, 420.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot3_1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 4-1", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -520.0, -360.0, 670.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot4_1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 5-1", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1200.0, -640.0, 1280.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot5_1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 5-2", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -500.0, -340.0, 660.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot5_2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 6-1", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -540.0, -375.0, 650.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot6_1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 6-2", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -540.0, -340.0, 660.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot6_2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 6-3", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -660.0, -400.0, 800.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot6_3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 7-1", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -720.0, -500.0, 1000.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot7_1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 7-2", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -640.0, -420.0, 770.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot7_2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 7-3", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -660.0, -400.0, 800.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot7_3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 7-4", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -400.0, -200.0, 420.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot7_4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 7-5", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -360.0, -210.0, 440.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot7_5, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 7-6", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -660.0, -400.0, 800.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot7_6, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot 7-7", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -450.0, -280.0, 550.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnot7_7, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Knot Universes in Bianchi Type I Cosmology - I", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -7.0, -4.0, 8.0, 1, 8, 2, 4, 8, 8, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoKnotUniversesI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Knot Universes in Bianchi Type I Cosmology - Tempestuous", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -2*PI, 2*PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -120.0, -85.0, 140.0, 7, 8, 5, 4, 8, 8, NULL, 1, YAXIS, 6400.0,
	    NullSetup, DoKnotUniversesITempestuous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 1.0, 1.0
	},
	{
	"Knot Universes in Bianchi Type I Cosmology - III", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -2*PI, 2*PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -240.0, -130.0, 200.0, 2, 3, 1, 4, 8, 8, NULL, 1, YAXIS, 6400.0,
	    NullSetup, DoKnotUniversesICosmologyIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.02, 0.1, 0.02
	},
	{
	"Lissajous 1", "start", "end", "a1", "a2", "a3", "k1", "k2", "k3", "Knot Width", "Colour Factor", -PI, PI, 100.0, 100.0, 100.0, 1.0, 1.0, 1.0, 10.0, 4.0,
	    "l1", "l2", "l3", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, PI/2, PI/2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -120.0, 240.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoLissajous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Lissajous 2", "start", "end", "a1", "a2", "a3", "k1", "k2", "k3", "Knot Width", "Colour Factor", -PI, PI, 100.0, 100.0, 100.0, 1.0, 2.0, 1.0, 10.0, 4.0,
	    "l1", "l2", "l3", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 7.0, PI/2, PI/2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -120.0, 240.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoLissajous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Lissajous 3", "start", "end", "a1", "a2", "a3", "k1", "k2", "k3", "Knot Width", "Colour Factor", -PI, PI, 100.0, 100.0, 100.0, 1.0, 5.0, 3.0, 10.0, 4.0,
	    "l1", "l2", "l3", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, PI/2, PI/2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -120.0, 240.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoLissajous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Lissajous 4", "start", "end", "a1", "a2", "a3", "k1", "k2", "k3", "Knot Width", "Colour Factor", -PI, PI, 100.0, 100.0, 100.0, 3.0, 5.0, 2.0, 10.0, 4.0,
	    "l1", "l2", "l3", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, PI/2, PI/2, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -120.0, 240.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoLissajous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Lissajous 5", "start", "end", "a1", "a2", "a3", "k1", "k2", "k3", "Knot Width", "Colour Factor", -PI, PI, 100.0, 100.0, 100.0, 3.0, 5.0, 7.0, 10.0, 4.0,
	    "l1", "l2", "l3", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 7.0, 5.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -120.0, 240.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoLissajous, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"ManpWIN Knot", "start", "end", "R", "r", "p", "q", "Knot width", "Colour Factor", ES, ES, -20 * PI, 20 * PI, 2.0, 5.0, 4.123105626, 8.0, 4.0, 4.0, 0.0, 0.0,
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0,
	    -12.0, -4.0, 12.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 64000.0,
	    NullSetup, DoMarcusTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0	// note dt is not = 0 because extra dimenstions created using dt
	},
	{
	"Marcus 6D Torus Knot 15, 8", "start", "end", "R", "r", "p", "q", "Knot width", "Colour Factor", ES, ES, -PI, PI, 2.0, 5.0, 15.0, 8.0, 10.0, 4.0, 0.0, 0.0, 
	    "x", "y", "z", "u", "v", "w", ES, ES, ES, ES, 0.1, 0.2, 0.3, 0.1, 0.2, 0.3, 0.0, 0.0, 0.0, 0.0,
	    -7.5, -4.5, 9.0, 1, 5, 3, 8, 6, 6, NULL, 1, YAXIS, 6400.0,
	    NullSetup, DoMarcusTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 1.0	// note dt is not = 0 because extra dimenstions created using dt
	},
	{
	"Paul Bourke's Knot 4", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, 0.0, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -4.6, -3.0, 5.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 3600.0,
	    NullSetup, DoPaulBourkesKnot4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Polynomial Knots -  5 Crossings", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -5.5, 5.5, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -800.0, -450.0, 900.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 4800.0,
	    NullSetup, DoPolynomialKnotsCrossings5, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.0, 0.1
	},
	{
	"Polynomial Knots -  6 Crossings", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -4.0, 4.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -45.0, 90.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 3600.0,
	    NullSetup, DoPolynomialKnotsCrossings6, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.01
	},
	{
	"Polynomial Knots -  7 Crossings", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -3.5, 3.5, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -140.0, -35.0, 150.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 3600.0,
	    NullSetup, DoPolynomialKnotsCrossings7, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.01
	},
	{
	"Polynomial Knots -  8 Crossings", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -3.3, 3.3, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -1300.0, -1200.0, 1400.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 3600.0,
	    NullSetup, DoPolynomialKnotsCrossings8, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 30.0, 0.05
	},
	{
	"Polynomial Knots - 10 Crossings", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -3.2, 3.2, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -80.0, -16.0, 86.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 3600.0,
	    NullSetup, DoPolynomialKnotsCrossings10, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.6, 0.0004
	},
	{
	"Shastri´s Trefoil", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -2.5, 2.5, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -18.0, -6.0, 20.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoShastrisTrefoil, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Square Knot", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -220.0, -125.5, 250.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoSquareKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 3, 2", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 3.0, 2.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 4, 3", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 4.0, 3.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 5, 2", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 5.0, 2.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 5, 3", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 5.0, 3.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 5, 4", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 5.0, 4.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 6, 5", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 6.0, 5.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 7, 2", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 7.0, 2.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 7, 3", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 7.0, 3.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 7, 4", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 7.0, 4.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 7, 5", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 7.0, 5.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 7, 6", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 7.0, 6.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 15, 2", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 15.0, 2.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Torus 2, 15", "start", "end", "R", "r", "p", "q", "Knot Width", "Colour Factor", ES, ES, -PI, PI, 100.0, 25.0, 2.0, 15.0, 10.0, 4.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -250.0, -140.5, 280.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 5200.0,
	    NullSetup, DoTorusKnot, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Trefoil Knot A", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -10.0, 10.0, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -6.0, -3.5, 7.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTrefoilKnotA, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	{
	"Trefoil Knot B", "start", "end", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, -PI, PI, 10.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	    -40.0, -25.5, 50.0, 1, 2, 3, 4, 6, 6, NULL, 1, YAXIS, 1600.0,
	    NullSetup, DoTrefoilKnotB, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	},
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    //////////////////////////////////////////////////////////////////
    // database of Curves
    //////////////////////////////////////////////////////////////////

    struct OscillatorSpecificStuff CurveSpecific[] =
	{
	    {
	    "Abdank-Abakanowicz Quadratrix", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI * 4.0, PI*4.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-13.0, -7.5, 15.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoAbdankAbakanowiczQuadratrix, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Ampersand Curve", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, -PI, PI, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.8, -1.5, 3.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoAmpersandCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Anguinea", "start", "end", "a", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, 6.0, 2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-13.0, -7.5, 15.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoAnguinea, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Archimedean Spiral", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, 0.0, PI*8.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-42.0, -26.0, 50.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoArchimedeanSpiralCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Astroid", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, 0.0, PI*8.0, 9.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-22.0, -12.0, 24.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoAstroid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Bean Curve", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, -PI, PI, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.0, -0.45, 1.5, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoBeanCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Besace", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, 1.0, 2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.7, -0.43, 3.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoBesace, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Bicorn Curve", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, 0.0, PI*2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.5, -0.25, 1.6, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoBicornCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Bifoliate Curve", "start", "a", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, 0.0, PI*2.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.2, -1.5, 3.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoBifoliate, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Biquartic-Carlos Sacre", "start", "n", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, 0.0, PI*2.0, 2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.3, -0.2, 1.6, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoBiquarticCarlosSacre, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Booth's Curve", "start", "a", "b", "e", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES,
		-PI * 2.0, PI*2.0, 10.0, 10.0 / sqrt(11.0), -1.0, 3.0, 6.0, 4.0, 3.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-12.0, -7.0, 14.0, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoBoothsCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Brachistochrone Curve", "start", "a", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI * 2.0, PI*2.0, 0.2, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.7, -1.0, 2.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoBrachistochroneCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Butterfly Curve", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, 0.0, PI*8.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.8, -3.0, 8.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoButterflyCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Cardiod", "start", "end", "index", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI, PI, 2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.0, -3.2, 6.4, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoEpicycloids, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Cassinian Oval", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, 0.0, PI*2.0, 2.0, 2.1, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-4.5, -2.4, 4.8, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoCassinianOval, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Catenary", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -5.0, 5.0, 5.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.2, 2.5, 7.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoCatenary, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Cayley's Sextic", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -5.0, 5.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.2, -4.0, 8.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoCayleysSextic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Chrysanthemum Curve", "Knot Width", "Colour Factor", ES, ES, ES, ES, ES, ES, ES, ES, 3.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-20.0, -12.0, 24.0, 1, 2, 3, 2, 6, 6, NULL, 1, YAXIS, 360000.0,
		NullSetup, DoChrysanthemumCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 5.0
	    },
	    {
	    "Cissoid", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -5.0, 5.0, 9.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-54.0, -10.0, 60.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoCissoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 3.0, 1.0
	    },
	    {
	    "Clelie Curve", "start", "end", "a", "m", "Curve Width", "Colour Factor", ES, ES, ES, ES, -5.0, 5.0, 100.0, 20.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-200.0, -125.0, 250.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoClelieCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.001, 1.0, 1.0
	    },
	    {
	    "Cochleoid", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI * 5.0, PI*15.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.2, -0.8, 1.6, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 7200.0,
		NullSetup, DoCochleoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.01
	    },
	    {
	    "Conchoid of de Sluze", "start", "end", "a", "k1", "k2", "k3", "k4", "k5", "k6", "Curve Width", -1.3, 1.3, -3.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 3.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.5, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoConchoidDeSluze, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Conchoid of de Sluze Family", "start", "end", "a", "k1", "k2", "k3", "k4", "k5", "k6", "Curve Width", -20*PI, 20.0*PI, -3.0, 1.0, 1.0, 1.0, 1.0, 2.718, 2.718, 3.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.5, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoConchoidDeSluze, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.6, 0.1
	    },
	    {
	    "Convict's Curve", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI * 2.0, PI*2.0, 1.0, 2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-4.8, -2.1, 5.4, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoConvictsCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Cornoid", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, 1.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.9, -1.6, 3.2, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoCornoidFamily, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Cornoid Family", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -5*PI, 5*PI, 1.0, 2.7, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.4, -0.8, 1.6, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoCornoidFamily, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.2, 0.1
	    },
	    {
	    "Cruciform (Curve is infinite, so boundaries added)", "start", "end", "a", "Plane Limits", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, 1.0, 12.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-8.0, -5.0, 10.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoCruciform, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Cycloid of Ceva", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, 0.0, PI*8.0, 3.0, 6.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.0, 4.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoCycloidOfCeva, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Cycloids", "start", "end", "r", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, 0.0, PI*8.0, 9.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-67.0, -67.0, 200.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoCycloids, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 3.0, 1.0
	    },
	    {
	    "Deltoid and Polioids (order = R/r and let r=h)", "start", "end", "R", "r", "h", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, 0.0, 2.0 * PI, 9.0, 3.0, 3.0, 3.0, 6.0, 4.0, 3.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-18.0, -10.0, 20.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoHypocycloids, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Devil's Curve", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, 1.0, sqrt(2.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-4.5, -2.4, 4.8, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoDevilsCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Double-Heart Curve-Kerner", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, -PI * 2.0, PI*2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.2, -1.3, 2.6, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoDoubleHeartCurveKerner, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Dumbbell Curve", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -1.0, 1.0, 1.0, 3.0, 6.0, 4.0, 7.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.4, -0.8, 1.6, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoDumbbellCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Eight Curve", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI, PI, 1.0, 3.0, 6.0, 4.0, 7.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.4, -0.8, 1.6, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoEightCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Ellipse", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, 0.0, 2.0 * PI, 4.0, 2.0, 3.0, 6.0, 4.0, 5.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.4, -3.2, 6.4, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoEllipse, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Epicycloids", "start", "end", "index", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI, PI, 9.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-30.0, -18.0, 36.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoEpicycloids, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Epitrochoid", "start", "end", "a", "b", "c", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -3 * PI, 3 * PI, 0.5, 0.3, 0.5, 1.0, 3.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.8, -1.5, 3.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoEpitrochoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Epitrochoid Family", "start", "end", "a", "b", "c", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -12 * PI, 12 * PI, 0.5, 0.3, 0.5, 4.359, 3.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.8, -1.5, 3.0, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoEpitrochoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Equangular Spiral", "start", "end", "angle in degrees", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -2 * PI, 10 * PI, 85.0, 3.0, 8.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-30.0, -18.0, 36.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoEquangularSpiral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Evolute of Ellipse", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, 2.0, 2.0, 3.0, 8.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-4.2, -2.5, 5.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoEvoluteEllipse, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Fermat Spiral", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, -10.0, 10.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.6, -4.0, 8.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoFermatSpiralCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Fish Curve", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI, PI, 1.0, 3.0, 6.0, 4.0, 7.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.4, -0.8, 1.6, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoFishCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Folium of Descartes (Curve is infinite, so boundaries added)", "start", "end", "Plane Limit", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -10.0, 10.0, 5.0, 3.0, 8.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.6, -4.0, 8.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 7200.0,
		NullSetup, DoFoliumDescartes, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Freeth's Nephroid", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -2 * PI, 2 * PI, 1.0, 3.0, 6.0, 4.0, 7.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.6, -3.0, 6.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoFreethsNephroid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Freeth's Nephroid Family", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -20 * PI, 20 * PI, 0.5, 1.73205, 3.0, 6.0, 4.0, 7.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.0, 4.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoFreethsNephroidIrrational, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Gauss Curve", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -5.0, 5.0, 1.0, 2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.4, -2.0, 6.4, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoGaussCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Gears", "start", "end", "a", "b", "number of gears", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, 0.0, 2.0 * PI, 2.0, 2.0, 8.0, 3.0, 6.0, 4.0, 3.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.4, -3.2, 6.4, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoGears, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Hippopede", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -5.0, 5.0, 1.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.8, -1.5, 3.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoHippopede, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Hyperbola (Curve is infinite, so boundaries added)", "start", "end", "a", "b", "Plane Limit", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, 0.0, 2.0 * PI, 2.0, 2.0, 4.0, 3.0, 6.0, 4.0, 3.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.4, -3.2, 6.4, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoHyperbola, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Hyperbolic Spiral", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, 0.5, 25.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.0, -0.5, 1.8, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoHyperbolicSpiralCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Hypocycloids", "start", "end", "R", "r", "h", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, 0.0, 12.0 * PI, 8.0, 5.0, 6.0, 3.0, 6.0, 4.0, 3.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-30.0, -18.0, 36.0, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoHypocycloids, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Hypotrochoids", "start", "end", "R", "r", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, 0.0, 12.0 * PI, 8.0, 5.0, 6.0, 3.0, 6.0, 4.0, 3.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-30.0, -18.0, 36.0, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoHypotrochoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Illumination Curve - Octic", "start", "end", "b", "s", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, 0.6, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.1, -1.5, 3.5, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoIlluminationCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.05
	    },
	    {
	    "Involute of a Circle", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, 0.0, 8.0 * PI, 8.0, 3.0, 6.0, 24.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-450.0, -250.0, 500.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoInvoluteCircle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Kampyle of Eudoxus", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -5.0, 5.0, 1.0, 1.0, 3.0, 6.0, 4.0, 5.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-18.0, -10.0, 20.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoKampyleOfEudoxus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "L'Hospital Quintice", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI * 2.0, PI*2.0, 0.2, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.26, -0.02, 0.3, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoLHospitalQuintic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.01
	    },
	    {
	    "Lemniskate Curve", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, 0.0, PI*8.0, 9.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-12.0, -7.0, 14.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoLemniskate, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Lilliput-Brobdingnag - from Cochleoid Formula", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI * 5.0, PI*5.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.42, -0.25, 0.5, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoLilliputBrobdingnag, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.01
	    },
	    {
	    "Limacon of Pascal", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -2 * PI, 2 * PI, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.2, -2.25, 4.5, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoLimaconPascal, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Lissajous Curves", "start", "end", "a", "b", "c", "n", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -PI, PI, 8.0, 6.0, 3.0, 8.0, 3.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-14.0, -8.0, 16.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoLissajousCurves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Lituus", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, 0.5, 100.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.3, -0.75, 2.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoLituus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.01
	    },
	    {
	    "Logarithmic Spiral", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, 0.0, 64.0, 0.03, 3.0, 8.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-13.0, -8.0, 16.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoLogarithmicSpiralCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Maltese Cross Curve (Curve is infinite, so boundaries added)", "start", "end", "a", "Plane Limit", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -4.0, 4.0, 2.0, 25.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-18.0, -10.0, 20.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 12000.0,
		NullSetup, DoMalteseCross, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Marcus Butterfly 1", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -15.0 * PI, 15.0 * PI, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.8, -0.5, 1.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 120000.0,
		NullSetup, DoMarcusButterfly1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.08, 0.1
	    },
	    {
	    "Marcus Butterfly 2", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -15.0 * PI, 15.0 * PI, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.8, -0.8, 1.6, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 120000.0,
		NullSetup, DoMarcusButterfly2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.016, 0.1
	    },
	    {
	    "Marcus Butterfly 3", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -15.0 * PI, 15.0 * PI, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.65, -0.4, 0.8, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 120000.0,
		NullSetup, DoMarcusButterfly3, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.015, 0.1
	    },
	    {
	    "Marcus Butterfly 4 (Generic)", "start", "end", "a", "m", "n", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, -15.0 * PI, 15.0 * PI, 1.0, 2.0, 3.0, 3.0, 6.0, 4.0, 3.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.5, -1.5, 3.0, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 120000.0,
		NullSetup, DoMarcusButterfly4, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.016, 0.1
	    },
	    {
	    "Marcus Curve (Curve is infinite, so boundaries added)", "start", "end", "a", "m", "n", "Plane Limit", "Curve Width", "Colour Factor", "z amplitude", "z frequency",
		-10 * PI, 10 * PI, 2.0, 11.0, 3.0, 10.0, 3.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.8, -1.0, 2.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 72000.0,
		NullSetup, DoMarcusCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.1, 0.1
	    },
	    {
	    "Mordell Curve", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -4.0, 4.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-18.0, -10.0, 20.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoMordellCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Nodal Curve", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -2 * PI, 2 * PI, 0.1, 1.25, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.46, -0.25, 0.5, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoNodalCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Ophiuride", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -1.5, 1.5, 1.0, 0.6, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.3, -2.0, 4.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoOphiuride, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve A", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 80.0, 1.0, 80.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.5, 4.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve B", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 80.0, 1.0, 40.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.5, 4.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve C", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 60.0, 1.0, 120.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.5, 4.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve D", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 80.0, 1.0, 1.0, 80.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.5, 4.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve E", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 80.0, 1.0, 80.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.0, 4.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve F", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 80.0, 1.0, 1.0, 80.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.0, 4.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve G", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 80.0, 1.0, 80.0, 1.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.0, 4.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve H", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 100.0, 1.0, 50.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.0, 4.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve I", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 150.0, 1.0, 1.0, 100.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.2, 3.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve J", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 200.0, 1.0, 150.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.2, 3.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve K", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 80.0, 80.0, 80.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.2, 3.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve L", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 80.0, 80.0, 1.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.2, 3.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve M", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 9.0, 100.0, 200.0, 9.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.5, -2.3, 3.6, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve N", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 9.0, 200.0, 100.0, 9.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.4, -2.3, 3.6, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve O", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 1.0, 200.0, 200.0, 2.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.2, -2.2, 3.6, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve P", "start", "end", "a", "b", "c", "d", "Curve Width", "Colour Factor", "z amplitude", "z frequency", -5*PI, 5*PI, 2.0, 200.0, 200.0, 1.0, 1.0, 6.0, 4.0, 3.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.2, -2.2, 3.6, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Parametric Curve Q", "start", "end", "a", "b", "c", "d", "e", "i", "j", "Curve Width",  -5*PI, 5*PI, 1.0, 1.0, 60.0, 58.0, 60.0, 1.0, 2.0, 1.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.0, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesQT, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Parametric Curve R", "start", "end", "a", "b", "c", "d", "e", "i", "j", "Curve Width",  -5*PI, 5*PI, 1.0, 1.0, 60.0, 1.0, 60.0, 1.0, 2.0, 1.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.4, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesQT, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Parametric Curve S", "start", "end", "a", "b", "c", "d", "e", "i", "j", "Curve Width",  -5*PI, 5*PI, 1.0, 60.0, 1.0, 1.0, 60.0, 1.0, 2.0, 1.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.4, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesQT, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Parametric Curve T", "start", "end", "a", "b", "c", "d", "e", "i", "j", "Curve Width",  -5*PI, 5*PI, 80.0, 1.0, 2.0, 1.0, 80.0, 1.0, 2.0, 1.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.4, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesQT, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Parametric Curve SS", "start", "end", "a", "b", "c", "d", "e", "i", "j", "Curve Width",  -5*PI, 5*PI, 80.0, 1.0, 2.0, 1.0, 80.0, 1.0, 2.0, 1.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.4, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesQT1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Parametric Curve TT", "start", "end", "a", "b", "c", "d", "e", "i", "j", "Curve Width",  -5*PI, 5*PI, 1.0, 60.0, 1.0, 1.0, 60.0, 1.0, 2.0, 1.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.4, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesQT1, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Parametric Curve SSS", "start", "end", "a", "b", "c", "d", "e", "i", "j", "Curve Width",  -5*PI, 5*PI, 1.0, 60.0, 1.0, 1.0, 60.0, 1.0, 2.0, 1.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.4, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesQT2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Parametric Curve TTT", "start", "end", "a", "b", "c", "d", "e", "i", "j", "Curve Width",  -5*PI, 5*PI, 1.0, 60.0, 1.0, 1.0, 60.0, 1.0, 2.0, 1.0,
		"Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.4, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesQT2, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Generalised Parametric Curve (1,2,1,2,x^3,y^3)", "start", "end", "a", "b", "c", "d", "j", "k", "x power", "y power", 
		-5*PI, 5*PI, 1.0, 2.0, 1.0, 2.0, 1.0, 1.0, 3.0, 3.0,
		"Curve Width", "Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", ES, ES, ES, ES, 2.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-4.4, -2.5, 5.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 100000.0,
		NullSetup, DoParametricCurvesGeneral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Generalised Parametric Curve (1,6,1,6,x^3,y^3)", "start", "end", "a", "b", "c", "d", "j", "k", "x power", "y power", 
		-5*PI, 5*PI, 1.0, 6.0, 1.0, 6.0, 1.0, 1.0, 3.0, 3.0,
		"Curve Width", "Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", ES, ES, ES, ES, 2.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-4.4, -2.5, 5.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 100000.0,
		NullSetup, DoParametricCurvesGeneral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Generalised Parametric Curve (7,1,7,1,x^3,y^3)", "start", "end", "a", "b", "c", "d", "j", "k", "x power", "y power", 
		-5*PI, 5*PI, 7.0, 1.0, 7.0, 1.0, 1.0, 1.0, 3.0, 3.0,
		"Curve Width", "Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", ES, ES, ES, ES, 2.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-4.4, -2.5, 5.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 100000.0,
		NullSetup, DoParametricCurvesGeneral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Generalised Parametric Curve TTTT (80,1,1,80 x^3,y^53)", "start", "end", "a", "b", "c", "d", "j", "k", "x power", "y power",  
		-5*PI, 5*PI, 80.0, 1.0, 1.0, 80.0, 1.0, 2.0, 3.0, 53.0,
		"Curve Width", "Colour Factor", "z amplitude", "z frequency", "     N/A", "     N/A", ES, ES, ES, ES, 1.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-6.4, -3.5, 7.0, 1, 2, 3, 10, 6, 6, NULL, 1, YAXIS, 1000000.0,
		NullSetup, DoParametricCurvesGeneral, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.3
	    },
	    {
	    "Pear-Shaped Curve", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, 1.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.3, -0.5, 1.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 7500.0,
		NullSetup, DoPearShapedCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Pedal of the Parabola", "start", "end", "alpha", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI, PI, -0.5, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.0, -2.5, 5.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 7500.0,
		NullSetup, DoPedaloftheParabola, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.06
	    },
	    {
	    "Piriform Curve", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, 0.0, PI*2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.0, -1.8, 3.6, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoPiriform, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Plateau Curves", "start", "end", "a", "m", "n", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, -1.42, 1.42, 2.0, 5.0, 3.0, 3.0, 6.0, 4.0, 3.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-22.0, -12.5, 25.40, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoPlateauCurves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Poinsot's Spirals", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, -PI * 5, PI * 5, 4.0, 3.0, 6.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.5, -1.0, 2.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoPoinsotsSpirals, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.02
	    },
	    {
	    "Polygasteroid", "start", "a", "b", "e", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES,
		-PI * 2.0, PI*2.0, 5.0, 6.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-12.0, -7.0, 14.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoPolygasteroid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Power Curves", "start", "end", "n", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -3.0, 3.0, 2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-8.5, -0.5, 10.0, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoPowerCurves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Quadratrix of Hippias (Curve is infinite, so boundaries added)", "start", "end", "a", "Plane Limit", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -PI * 4, PI * 4, 2.0 / PI, 10.0, 3.0, 8.0, 12.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-19.0, -10.0, 20.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 7200.0,
		NullSetup, DoQuadratrixHippias, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Ranunculoid", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -PI, PI, 2.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-28.0, -16.0, 32.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoRanunculoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Rhodonea Curves", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(3.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.0, -1.2, 2.4, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoRhodoneaCurves, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Rose Curve", "start", "end", "m", "n", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, 0.0, 4.0 * PI, 10.0, 5.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.4, -1.5, 3.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoRoseCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Scarabaeus Curve", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -2 * PI, 2 * PI, 2.0, 3.0, 3.0, 6.0, 4.0, 7.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-7.6, -4.0, 8.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoScarabaeus, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Semi-Cubic Parabola", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -1.0, 1.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.4, -1.5, 3.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoSemiCubicParabola, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Serpentine", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -4.0, 4.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.0, -3.0, 6.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoSerpentine, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Squircle", "start", "end", "r", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency",  ES, ES, -4.0, 4.0, 1.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.1, -1.2, 2.4, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoSquircle, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Strophoid", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -5.0, 5.0, 9.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-54.0, -30.0, 60.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoStrophoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Swastika Curve", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, -PI, PI, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.6, -1.5, 3.0, 1, 2, 3, 6, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoSwastikaCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Talbot's Curve", "start", "end", "a", "b", "f", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, -PI, PI, 3.5, 5.0, 3.0, 3.0, 6.0, 6.0, 6.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.0, -3.0, 6.0, 1, 2, 3, 9, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoTalbotsCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 2.0, 0.1
	    },
	    {
	    "Tanh Spiral", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -3.0, 3.0, 4.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.0, 4.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoTanhSpiralCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Tear Drop Curve", "start", "end", "a", "n", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -2.0*PI, 2.0*PI, 1.0, 3.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-1.5, -0.8, 1.6, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoTearDropCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Torpedo Curve", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -2.0*PI, 2.0*PI, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.3, -0.4, 0.8, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 7200.0,
		NullSetup, DoTorpedoCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.02
	    },
	    {
	    "Tractrix", "start", "end", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, ES, -3.0, 3.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-3.6, -2.5, 5.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoTractrix, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Tricuspoid", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, 0.0, PI*8.0, 9.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-54.0, -30.0, 60.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoTricuspoid, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 1.0
	    },
	    {
	    "Trifolium", "start", "end", "a", "b", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -4.0, 4.0, 0.2, 3.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-4.5, -2.5, 5.0, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoTrifolium, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Trigonometric Curve-II - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(3.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.0, -1.2, 2.4, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoTrigonometricCurveII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Trigonometric Curve-III - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(3.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.8, -1.6, 3.2, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoTrigonometricCurveIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Trigonometric Curve-IV - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(3.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.8, -1.6, 3.2, 1, 2, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoTrigonometricCurveIV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Trigonometric Curve-V - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(3.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-28.0, -15.0, 30.0, 2, 1, 3, 8, 6, 6, NULL, 1, YAXIS + EVOLVE, 36000.0,
		NullSetup, DoTrigonometricCurveV, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 25.0, 0.001
	    },
	    {
	    "Trigonometric Curve-VI - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(11.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.0, -1.2, 2.4, 2, 1, 3, 8, 6, 6, NULL, 1, YAXIS + EVOLVE, 36000.0,
		NullSetup, DoTrigonometricCurveVI, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 0.001
	    },
	    {
	    "Trigonometric Curve-VII - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(11.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-5.0, -3.0, 6.0, 2, 1, 3, 8, 6, 6, NULL, 1, YAXIS + EVOLVE, 36000.0,
		NullSetup, DoTrigonometricCurveVII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 0.001
	    },
	    {
	    "Trigonometric Curve-VIII - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(17.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.8, -1.6, 3.2, 2, 1, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoTrigonometricCurveVIII, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Trigonometric Curve-IX - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(17.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.8, -1.6, 3.2, 2, 1, 3, 8, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoTrigonometricCurveIX, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Trigonometric Curve-X - based on Rhodonea", "start", "end", "a", "k", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, -15 * PI, 15 * PI, 1.0, -sqrt(17.0), 3.0, 6.0, 4.0, 3.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-2.8, -1.6, 3.2, 2, 1, 3, 8, 6, 6, NULL, 1, YAXIS + EVOLVE, 36000.0,
		NullSetup, DoTrigonometricCurveX, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.5, 0.001
	    },
	    {
	    "Trisetrix of Maclaurin", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -4.0, 4.0, 1.0, 3.0, 6.0, 4.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-7.0, -3.5, 7.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoTrisectrix, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Tschirnhausen Cubic", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -3.0, 3.0, 1.0, 3.0, 6.0, 12.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-52.0, -22.5, 45.0, 1, 2, 3, 7, 6, 6, NULL, 1, YAXIS, 3600.0,
		NullSetup, DoTschirnhausenCubic, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 1.0, 0.1
	    },
	    {
	    "Viviani Curve", "start", "end", "a", "Curve Width", "Colour Factor", "z amplitude", "z frequency", ES, ES, ES, -2.0*PI, 100.0*PI, 1.0, 3.0, 6.0, 12.0, 3.0, 0.0, 0.0, 0.0,
		"     N/A", "     N/A", "     N/A", "     N/A", "     N/A", "     N/A", ES, ES, ES, ES, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		-0.82, -0.9, 1.0, 3, 5, 2, 7, 6, 6, NULL, 1, YAXIS, 36000.0,
		NullSetup, DoVivianiCurve, "SelectOscParams", (DLGPROC)SelectOscParams, 0.01, 0.045, 1.0
	    },
	    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};



#ifdef	PRINTOSC				// defines in fractalp.h
extern	int	OscPtrArray[];			// array of pointers to specific oscillators or fractal maps
extern	WORD	type;				// fractal type
void	PrintOsc(int OscillatorNum)		// used for listing oscillator and fractal map names in d:\temp\OscDump.txt and d:\temp\MapDump.txt
    {
    FILE	*fp;
    int		i;
    errno_t	err;
    char	s[150];

    if (type == OSCILLATORS)
	{
	if ((err = fopen_s(&fp, "d:\\temp\\OscDump.txt", "w+")) != 0)
	    {
	    wsprintf(s, "Can't Open Oscillator File: <d:\\temp\\OscDump.txt>");
	    MessageBox (GlobalHwnd, s, "ManpWIN Osc Dump", MB_ICONEXCLAMATION | MB_OK);
	    return;
	    }
	fprintf(fp, "Oscillators:\n");
	for (i = 0; i < OscillatorNum; ++i)
	    fprintf(fp, "%4d: %s\n", i, OscillatorSpecific[OscPtrArray[i]].name);
	}
    else if (type == FRACTALMAPS)
	{
	if ((err = fopen_s(&fp, "d:\\temp\\MapDump.txt", "w+")) != 0)
	    {
	    wsprintf(s, "Can't Open Map File: <d:\\temp\\MapDump.txt>");
	    MessageBox(GlobalHwnd, s, "ManpWIN Map Dump", MB_ICONEXCLAMATION | MB_OK);
	    return;
	    }
	fprintf(fp, "Fractal Maps:\n");
	for (i = 0; i < OscillatorNum; ++i)
	    fprintf(fp, "%4d: %s\n", i, FractalMapSpecific[OscPtrArray[i]].name);
	}
    else if (type == SPROTTMAPS)
	{
	if ((err = fopen_s(&fp, "d:\\temp\\SprottDump.txt", "w+")) != 0)
	    {
	    wsprintf(s, "Can't Open SprottMap File: <d:\\temp\\SprottDump.txt>");
	    MessageBox(GlobalHwnd, s, "ManpWIN Sprott Map Dump", MB_ICONEXCLAMATION | MB_OK);
	    return;
	    }
	fprintf(fp, "Sprott Fractal Maps:\n");
	for (i = 0; i < OscillatorNum; ++i)
	    fprintf(fp, "%4d: %s\n", i, SprottMapSpecific[OscPtrArray[i]].name);
	}
    else if (type == SURFACES)
	{
	if ((err = fopen_s(&fp, "d:\\temp\\SurfaceDump.txt", "w+")) != 0)
	    {
	    wsprintf(s, "Can't Open Survace File: <d:\\temp\\SurfaceDump.txt>");
	    MessageBox (GlobalHwnd, s, "ManpWIN Surface Dump", MB_ICONEXCLAMATION | MB_OK);
	    return;
	    }
	fprintf(fp, "Surfaces:\n");
	for (i = 0; i < OscillatorNum; ++i)
	    fprintf(fp, "%4d: %s\n", i, SurfaceSpecific[OscPtrArray[i]].name);
	}
    else if (type == KNOTS)
	{
	if ((err = fopen_s(&fp, "d:\\temp\\KnotDump.txt", "w+")) != 0)
	    {
	    wsprintf(s, "Can't Open Knot File: <d:\\temp\\KnotDump.txt>");
	    MessageBox (GlobalHwnd, s, "ManpWIN Knot Dump", MB_ICONEXCLAMATION | MB_OK);
	    return;
	    }
	fprintf(fp, "Knots:\n");
	for (i = 0; i < OscillatorNum; ++i)
	    fprintf(fp, "%4d: %s\n", i, KnotSpecific[OscPtrArray[i]].name);
	}
    else if (type == CURVES)
	{
	if ((err = fopen_s(&fp, "d:\\temp\\CurveDump.txt", "w+")) != 0)
	    {
	    wsprintf(s, "Can't Open Curve File: <d:\\temp\\CurveDump.txt>");
	    MessageBox (GlobalHwnd, s, "ManpWIN Curve Dump", MB_ICONEXCLAMATION | MB_OK);
	    return;
	    }
	fprintf(fp, "Curves:\n");
	for (i = 0; i < OscillatorNum; ++i)
	    fprintf(fp, "%4d: %s\n", i, CurveSpecific[OscPtrArray[i]].name);
	}
    if (fp)
	fclose(fp);
    }
#endif








