#include <Windows.h>
#include "colour.h"
#include "Dib.h"
#include "complex.h"
#include "BigDouble.h"
#include "BigComplex.h"
#include "OscProcess.h"
#include "fractalp.h"
#include "filter.h"
#include "fract.h"
#include "Tetration.h"
#include "Fractype.h"
#include "slope.h"
#include "plot.h"
#include ".\parser\TrigFn.h"

#ifndef sqr
#define sqr(x) ((x)*(x))
#endif

#define MAXROOTS		64		// Newton fractals
#define distance(z1,z2)		(sqr((z1).x-(z2).x)+sqr((z1).y-(z2).y))
//#define	MINSIZE			1.0e-9

#define	PLUS	1				// MandelPhoenix fractals
#define	ZERO	0
#define	MINUS	-1

#define PASSWIDTH	10

struct tess
    {				    // one of these per box to be done gets stacked 
    int	    x1, x2, y1, y2;	    // left/right top/bottom x/y coords 
    long    top, bot, lft, rgt;	    // edge colours, -1 mixed, -2 unknown
    };

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

#define calcadot(h,c,x,y) { if ((c = calc_frac(h, y, x, user_data)) == -1) return(-1); }

#pragma once
class CPixel
    {
    public:
	int	RunThread(HWND hwnd, int i, HANDLE ghMutex, int StripWidth, CSlope Slope, CFract *Fractal, BigDouble *Big_xxmin, BigDouble *Big_xxmax, BigDouble *Big_yymin, double *xxmin, double *xxmax, double *yymin, int user_data(HWND hwnd));
	long	dofract(HWND hwnd, int row, int col);
	long	DoBigFract(HWND hwnd, int row, int col);
	long	BigCalcFrac(HWND hwnd, int row, int col, int user_data(HWND hwnd));
	int	run_fractal(Complex *z, Complex *q);
	int	run_big_fractal();
	int	init_fractal(Complex *z, Complex *q);
	int	init_big_fractal();
	int	float_decomposition(double z_real, double z_imag);
//	int	potential(double mag, int iterations);
	long	calc_frac(HWND hwnd, int row, int col, int user_data(HWND hwnd));
	Complex	invertz2(Complex  & Cmplx1);
	void	init_stereo_pairs(int pairflag, int *AutoStereo_value);
	void	do_stereo_pairs(int col, int row, long color);
	void	draw_right_image(short y);
	void	InitDistEst(double *xxmin, double *xxmax, double *yymin, double *yymax, double *xx3rd, double *yy3rd, int *distestwidth, int distest);

	int	StandardCalculationMode(HWND hwnd, CSlope Slope, int user_data(HWND hwnd));
	int	FindSymmetry(BYTE _3dflag, int decomp, BYTE pairflag, int method, BOOL invert, int CoordSystem, double param[], WORD degree, WORD type,
		int subtype, BYTE calcmode, int orientation, CFract *Fractal, double hor, double vert, double mandel_width, BYTE BigNumFlag, BigDouble *Big_xxmin,
		BigDouble *Big_xxmax, BigDouble *Big_yymin, BigDouble *Big_yymax, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double ScreenRatio, double *xxmin,
		double *xxmax, double *yymin, double *yymax, WORD special, BYTE juliaflag);

	void	InitPixel0(WORD typeIn, WORD specialIn, int subtypeIn, WORD *degreeIn, double rqlimIn, BOOL ExpandStarTrailColoursIn, BYTE SpecialFlagIn, int precisionIn, int biomorphIn, int methodIn, int orientationIn, int xdotsIn, int ydotsIn, int nFDOptionIn);
	void	InitPixel1(CTZfilter *TZfilterIn, CTrueCol *TrueColIn, COscProcess *OscProcessIn, int period_levelIn, int distestIn, BOOL invertIn, BYTE phaseflagIn, double *wpixelsIn, BYTE juliaflagIn, double closenuffIn, BigDouble BigCloseEnoughIn, BYTE calcmodeIn);
	void	InitPixel2(int CoordSystemIn, BOOL UseCurrentPaletteIn, int reset_periodIn, int colorsIn, double horIn, double vertIn, double mandel_widthIn, BigDouble BigHorIn, BigDouble BigVertIn, BigDouble BigWidthIn, double *yymax, BigDouble *Big_yymaxIn);
	void	InitPixel3(double dStrandsIn, Complex jIn, BYTE pairflagIn, long *andcolor, BYTE _3dflagIn, double xgap, double ygap, BigDouble Big_xgap, BigDouble Big_ygap, Complex *cIn, double ScreenRatio, WORD colours, CFract *Fract);
	void	InitPixel4(BigComplex *cBigIn, Complex *qIn, Complex *zIn, BigComplex *qBigIn, BigComplex *zBigIn, long thresholdIn, BYTE BigNumFlagIn, long *colorIn, int logvalIn, long *iterationIn, double f_radiusIn, double f_xcenterIn, char *LyapSequenceIn);
	void	InitPixel5(double f_ycenterIn, int *symmetryIn, double paramIn[], double potparamIn[], int decompIn, BYTE *logtableIn, int *AutoStereo_valueIn, int widthIn, HWND hwndIn/*, struct workliststuff *worklistIn*/);
	void	InitPixel6(CDib *DibIn, int *PlotTypeIn, int *oldrowIn, int *oldcolIn, int *time_to_zoom, int *time_to_restart, int *time_to_reinit, int *time_to_quit, long fillcolorIn, long *andcolorIn, int *blockindexIn, int *totpassesIn, int *curpassIn);
	void	ManageBignumPrecision(int precision);				// allow internal bignum variables to track current precision requirements

	int	BifurcLambda(double Rate, double *Population);			// it's here becasue it is used in miscfrac.cpp Lyapunov fractal
	int	bifurcation(int user_data(HWND hwnd));						// ditto for bifurcation()
	void	plot_orbits(RGBTRIPLE colour, int count);

	void	plot(WORD x, WORD y, DWORD color);
	void	PlotPixel(WORD, WORD, DWORD);

//	void	DisplayLyapSequence(int *Rxy, int SeqLength);

	// line3d stuff
	void	projection(int x, int y, long col);
//	void	xrot(double, MATRIXPTR);
//	void	yrot(double, MATRIXPTR);
//	void	zrot(double, MATRIXPTR);
//	void	trans(double, double, double, MATRIXPTR);
	void	init3d(int xdots, int ydots, double x_rot, double y_rot, double z_rot, double sclx, double scly, double sclz, long threshold, double hor, double vert);
//	void	identity(MATRIXPTR);
//	int	vmult(VECTORPTR, MATRIXPTR, VECTORPTR);

	WORD	special;		// special colour for CBIN fractals, phase etc
	int	logval;			// log colour map starting value 
	BYTE	*logtable;		// log value table for col comp
	BOOL	ExpandStarTrailColours;	// use the first 16 colours if false, else expand across the whole iteration range

	double	f_radius = 1.0;		// inversion radius 
	double	f_xcenter = 0.0;	// inversion center 
	double	f_ycenter = 0.0;

	double	potparam[3];		// potential parameters
	int	nFDOption;		// used in some Tierazon filters

	WORD	type;			// M=mand, J=Julia 1,2,4->
	int	subtype;		// B=basin, S=stripe, N=normal
	WORD	*degree;		// polynomial degree
	double	rqlim;
	BYTE	SpecialFlag;
	double	dem_delta, dem_width;	// distance estimator variables
	int	biomorph;
	int	method;			// inside and outside filters
	int	orientation;		// 0, 90, 180 or 270 degrees
	int	period_level;		// 0 for no periodicity checking
	int	distest;
	BOOL	invert;			// invert fractal
	BYTE	phaseflag;		// 3 phases for type SPECIALNEWT fractals
	double	closenuff;		// periodicity bailout
	BYTE	juliaflag;		// Julia implementation of fractal
	BYTE	calcmode;		// trace type B, G, 1, 2
	int	CoordSystem;
	BOOL	UseCurrentPalette;	// do we use the ManpWIN palette? If false, generate internal filter palette
	double	dStrands;
	BYTE	pairflag;		// stereo pair
	long	*color;
	int	decomp;			// number of decomposition colours
	long	threshold;
	int	colors; 		// maximum colours available from file
	WORD	colours;		// colours in the file
	long	*iteration;
	double	FloatIteration;
	long	oldcolour;		// for periodicity checking
	double	SlopeError;		// used as error value for Newton type Fwd Diff bailout calcs
	int	*oldrow;
	int	*oldcol;
	long	*andcolor;
	BYTE	_3dflag;		// generate image in 3D
	int	reset_period;		// periodicity checking
	int	*symmetry;		// symmetry flag
	int	xdots, ydots;		// strip dimensions in pixels

	double	hor;			// horizontal address
	double	vert;			// vertical address
	double	mandel_width;		// width of display
	double	xgap;			// gap between pixels
	double	ygap;			// gap between pixels
	int	NonStandardFractal;	// does fractal use standard plotting mode?
	double	*yymax;			// max value of vert

	char	*LyapSequence;		// hold the AB sequence for Lyapunov fractals

	Complex	*z, *q, j, *c, deriv, TempPt;

	double	*wpixels;		// an array of doubles holding slope modified iteration counts

	CTrueCol    *TrueCol;		// palette info
	CTZfilter   *TZfilter;		// Tierazon filters
	COscProcess *OscProcess;
//	RGBTRIPLE   *FilterRGB;		// for Tierazon filters
	CPlot	    Plot;

//	CLine3d	    *Line3d;		// routines for projection and 3D transforms
	int	dem_color;
	Complex	dem_new;
	double	param[NUMPERTPARAM];
//	int	*time_to_quit;		// enable clean exit

	/**************** Big Number Globals *********************/

	int	BigFractPtr = 0;	// point to the Bignum fractal specific stuff
	int	decimals = 10, precision;
	BYTE	BigNumFlag;		// True if bignum used
	double	ScreenRatio;		// ratio of width / height for the screen

	BigDouble	BigCloseEnough, BigBailout, *Big_yymax, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth;
//	BigDouble	Big_oldhor, Big_oldvert, Big_oldwidth;
	BigComplex	*zBig, *cBig, *qBig;
	BigComplex	BigOldZ, BigOlderZ;

	/**************** Big Number Globals *********************/
	// symmetry stuff
	int	ixstart, ixstop, iystart, iystop;	// start, stop here 
	int	xxstart, xxstop;			// these are same as worklist, 
	int	yystart, yystop, yybegin;		// declared as separate items  
	int	*PlotType;
	int	worksym;
	int	workpass, *totpasses, *curpass;		// for 1/2 pass type tracing 
	short	num_worklist;				// resume worklist for standard engine 
	int	xsym_split(int xaxis_row, int xaxis_between/*, int *symmetry*/);
	int	ysym_split(int xaxis_row, int xaxis_between/*, int *symmetry*/);
	void	setsymmetry(int sym, /*int thread, */int uselist, BYTE _3dflag, int decomp, BYTE pairflag, int method, BOOL invert, int CoordSystem, double param[],
		WORD degree, WORD type, int subtype, BYTE calcmode, int orientation, CFract *Fractal, double hor, double vert, double mandel_width, double *xxmax,
		double *xxmin, double *yymin, double *yymax, WORD special, BYTE juliaflag, int *PlotType, BYTE BigNumFlag, BigDouble *Big_xxmin, BigDouble *Big_xxmax,
		BigDouble *Big_yymin, BigDouble *Big_yymax, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double ScreenRatio);
//	DWORD	getcolor(WORD x, WORD y);
	//	int	*symmetry;				// symmetry flag

    private:
	int	DoFilter(int method, int hooper);
	void	CalcFloatIteration(double error, double *wpixels, int row, int col, Complex z, Complex OldZ, Complex OlderZ);
	void	CalcBigFloatIteration(double error, double *wpixels, int row, int col, BigComplex z, BigComplex OldZ, BigComplex OlderZ);
	int	DoBigFilter(int method, int hooper);
	BigComplex	BigInvertz2(BigComplex  & Cmplx1);

	// some fractal routines
	int	InitFunctions(WORD type, Complex *z, Complex *q);
	int	RunFunctions(WORD type, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration);
	int	BigInitFunctions(WORD type, BigComplex *zBig, BigComplex *qBig);
	int	BigRunFunctions(WORD type, BigComplex *zBig, BigComplex *qBig, BYTE *SpecialFlag, long *iteration);
	int	InitFractintFunctions(WORD type, Complex *z, Complex *q);
	int	RunFractintFunctions(WORD type, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration);
	int	InitFractintTrigFunctions(WORD type, Complex *z, Complex *q);
	int	RunFractintTrigFunctions(WORD type, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration);
	int	InitManDerFunctions(int subtype, Complex *z, Complex *q);
	int	RunManDerFunctions(int subtype, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration);
	int	BigInitManDerFunctions(int subtype, BigComplex *zBig, BigComplex *qBig);
	int	BigRunManDerFunctions(int subtype, BigComplex *zBig, BigComplex *qBig, BYTE *SpecialFlag, long *iteration);
	int	InitTierazonFunctions(int subtype, Complex *z, Complex *q);
	int	RunTierazonFunctions(int subtype, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration);
	int	BigInitTierazonFunctions(int subtype, BigComplex *zBig, BigComplex *qBig);
	int	BigRunTierazonFunctions(int subtype, BigComplex *zBig, BigComplex *qBig, BYTE *SpecialFlag, long *iteration);
	int	BigDecomposition(BigDouble z_real, BigDouble z_imag);
	PBCOMPLEX ComplexTetration(PBCOMPLEX a, int n);			// start Tetration procedures
	PBCOMPLEX PBComplexPower(PBCOMPLEX w, PBCOMPLEX z);
	void	Complex2Polar(PBCOMPLEX *a);
	void	Complex2Cart(PBCOMPLEX *a);
	double	RealTetration(double a, int n);
	BOOL	TetrationBailout(PBCOMPLEX z);
	int	InitTetration(Complex *z, Complex *q);
	int	DoTetration(long *iteration);				// end Tetration stuff
	int	InitKleinian(Complex *z, Complex *q);			// start Kleinian procedures
	Complex KleinianFunction(Complex *z);
	int	CalculateKleinian(Complex *z);
	int	PhoenixFractal(Complex *z, Complex *q);
	int	PhoenixPlusFractal(Complex *z, Complex *q);
	int	PhoenixMinusFractal(Complex *z, Complex *q);
	int	PhoenixFractalcplx(Complex *z, Complex *q);
	int	PhoenixCplxPlusFractal(Complex *z, Complex *q);
	int	PhoenixCplxMinusFractal(Complex *z, Complex *q);
	int	ZXTrigPlusZfpFractal(Complex *z, Complex *q, CTrigFn *TrigFn);
	int	ScottZXTrigPlusZfpFractal(Complex *z, Complex *q, CTrigFn *TrigFn);
	int	SkinnerZXTrigSubZfpFractal(Complex *z, Complex *q, CTrigFn *TrigFn);

	// non-raster functions
//	int	DoCurlicues(void);

	// pixel plotting routines
	void	symplot2(WORD, WORD, DWORD);
	void	symplot2Y(WORD, WORD, DWORD);
	void	symplot2J(WORD, WORD, DWORD);
	void	symplot4(WORD, WORD, DWORD);
	void	noplot(WORD, WORD, DWORD);
	void	symplot4basin(WORD, WORD, DWORD);
	void	symplot2basin(WORD, WORD, DWORD);
	CDib	*Dib;

	// trig stuff;
	int	Fn1Index;
	int	Fn2Index;

	// CalcMode stuff
	int	StandardCalc(HWND, int, int user_data(HWND hwnd));
	int	Blinds(HWND, int user_data(HWND hwnd));
	int	DoBlinds(HWND, int user_data(HWND hwnd));
	int	Spiral(HWND, int user_data(HWND hwnd));
	int	OneOrTwoPass(HWND, int user_data(HWND hwnd));
	int	row, col;
	int	add_worklist(int, int, int, int, int, int, int);
	void	tidy_worklist();
	int	combine_worklist();
	struct workliststuff	worklist[MAXCALCWORK];

	// tesseral stuff
	int	tesseral(HWND hwnd, int user_data(HWND hwnd));
	long	tesschkcol(int, int, int);
	long	tesschkrow(int, int, int);
	long	tesscol(HWND, int, int, int, int user_data(HWND hwnd));
	long	tessrow(HWND, int, int, int, int user_data(HWND hwnd));
	void	verline(WORD x0, WORD y0, WORD y1, DWORD colour);
	void	ClearTessMemory(DWORD *dstack);
	long    fillcolor;
	int	currow, curcol;
	int	guessplot;			// paint 1st pass row at a time? 

	// boundary trace stuff
	int	bound_trace_main(HWND, int user_data(HWND hwnd));
	int	boundary_trace(HWND, int, int, int user_data(HWND hwnd));
	long	calc_xy(HWND hwnd, int, int, int user_data(HWND hwnd));
	int	fillseg(int, int, int, long);
	void	reverse_string(DWORD *, DWORD *, int);
	int	LeftX[MAXHORIZONTAL];
	int	RightX[MAXHORIZONTAL];
	WORD	repeats;
	BYTE	save_flag;			// save screen after image
//	long	*andcolor;
//	int	*oldrow;
//	int	*oldcol;

	// Guess stuff
	int	solidguess(HWND hwnd, int user_data(HWND hwnd));
	int	maxblock, halfblock;
	int	right_guess, bottom_guess;
	int	guessrow(HWND, int, int, int, int user_data(HWND hwnd));
	void	plotblock(int, int, int, long, DWORD *dstack);
	int	ssg_blocksize(void);
	void	CPixel::ClearGuessMemory(DWORD *dstack);
	unsigned int prefix[2][maxyblk][maxxblk]; // common temp 
	int	*blockindex;			// 2 ^ n block size
	int	*time_to_zoom;
	int	*time_to_restart;
	int	*time_to_reinit;
	int	*time_to_quit;
	int	width;				// width of whole screen (used for dstack dimensioning in guess and Tess

	// Bifurcation routines
	int	BifQuadMandel(double Rate, double *Population);
	int	BifurcMay(double Rate, double *Population, long beta);
	int	BifurcVerhulstTrig(double Rate, double *Population, CTrigFn *TrigFn);
	int	BifurcAddTrigPi(double Rate, double *Population, CTrigFn *TrigFn);
	int	BifurcSetTrigPi(double Rate, double *Population, CTrigFn *TrigFn);
	int	BifurcStewartTrig(double Rate, double *Population, CTrigFn *TrigFn);
	int	BifurcLambdaTrig(double Rate, double *Population, CTrigFn *TrigFn);

	// line3d stuff
	void	do_line(short, short, short, short, short, short);
	void	interpcolour(short, short, short, short, int, int, int, int);
	//void	do_3d(void);
//	void	mat_mul(MATRIXPTR, MATRIXPTR, MATRIXPTR);
//	void	scale(double, double, double, MATRIXPTR);

	double	x_rot;			// angle display plane to x axis
	double	y_rot;			// angle display plane to y axis
	double	z_rot;			// angle display plane to z axis
	double	sclx, scly, sclz;	// scale 
	double	xval, yval, zval;	// rotate 

	MATRIX	m;			// transformation matrice

	struct PointInfo
	    {
	    int x;
	    int y;
	    int colour;
	    };

	struct PointInfo oldline[MAXHORIZONTAL];	// old pixels 
	struct PointInfo oldpoint;			// old pixels 

	Complex	OldZ, OlderZ;						// local variables for fwd diff calcs

	Complex	a, a2, aa3, b, l2, lm5, lp5, oz, sqr, t2, t3, v;	// local variables for functions
	Complex temp, temp1, temp2, temp3, temp4;
	double	real_imag, absolute, distance;
	HWND	hwnd;

	// local variables for bignum functions
	BigComplex	aBig, a2Big, aa3Big, bBig, l2Big, lm5Big, lp5Big, ozBig, /*sqrBig, */t2Big, t3Big, vBig;
	BigComplex	tempBig, temp1Big, temp2Big, temp3Big;

	// local variables for bignum Mandelbrot Derivatives
	BigComplex	sqrBig, sqrsqrBig;
	BigDouble	realimagBig, RealImagSqrBig, tBig;

	// local variables for rational maps
	int	OldThreshold = 0;					// check to see if threshold changes
	int	penpref[4] = { 1, 2, 3, 4 };				// can we do more in mappying these colours?
	int	pennref[4] = { 9, 10, 11, 12 };
	int	penp[4];
	int	penn[4];
	Complex	alpha, cmcc;
	double	escape, epsilon, der;

									// local variables for Mandelbrot Derivatives
	Complex	sqrsqr;
	double	RealImagSqr;

	// local variables for Tierazon fractals
	Complex	c1, c2, cb, caa3, z1, z2, z3, z4, zd, zt, t;
	double	d;

	BigComplex	c1Big, c2Big, cbBig, caa3Big, z1Big, z2Big, z3Big, z4Big, zdBig, ztBig;

	// local variables for stereo pairs
	float	xshift;
	float	EyeSeparation, ViewingDist;
	short	Left_ofLeftPic, Left_ofRightPic;
	short	PicWidth, PicHeight, sep;
	long	depth, maxc, ystart;
	long	colour_array[MAXHORIZONTAL / 2];	// for right hand image
	long	colour_position[MAXHORIZONTAL / 2];	// for right hand image
	int	ground;					// set limits on separation
	int	stereo_sign;
	int	*AutoStereo_value;			// AutoStereo depth value

							// Tetration stuff
	int	BailoutMethod;				// bailout method
	PBCOMPLEX	aPBC;				// PB Complex

	double	u, v1, K, M;				// Kleinian stuff

							// stuff for Fractint functions
	Complex	Coefficient/*, *floatparm*/;
	double	foldxinitx, foldyinity, foldxinity, foldyinitx;
	Complex	roots[MAXROOTS];			// roots
	Complex	croot, cdegree;
	double	thresh;
	int	root;
	double qc, qci, qcj, qck;			// These are for quaternions and hypercomplex
	int	PhoenixType = ZERO;
	int	PhoenixDegree;

	// Lyapunov stuff
	int	lyapunov_cycles(int filter_cycles, double a, double b, double c);
	unsigned int filter_cycles;
	double	Population, Rate;
	int	lyaLength, lyaSeedOK;
	int	lyaRxy[34];
	int	overflow;

	// Frothy Basin stuff
	void	CPixel::set_Froth_palette(HWND hwnd);
	char	froth3_256c[20] = "froth3.map";
	char	froth6_256c[20] = "froth6.map";
	char	froth3_16c[20] = "froth316.map";
	char	froth6_16c[20] = "froth616.map";
	int	frothsix = 0;
	int	froth_altcolor;
	int	froth_shades;
	long	oldcolor;

    };

