/*
   MANINIT.CPP a module with initialising type routines for MANP.C.
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <conio.h>
#include <stdio.h>
#include "Fract.h"
#include "manp.h"
#include "fractype.h"
#include "fractalp.h"
#include "complex.h"
#include "big.h"
#include "BigDouble.h"
#include "DDComplex.h"
#include "QDComplex.h"
#include ".\parser\cmplx.h"
#include "colour.h"
#include "Arithmetic.h"

BYTE	orbit_flag;			// display orbits?
double	closenuff;			// periodicity bailout
long	closenuff_int;			// periodicity bailout
int	period_level;			// 0 for no periodicity checking
static	int	first_init = TRUE;	// first time once only init done

extern	BYTE	orig_palette[];		// loaded palette

extern  BYTE	*logtable;		// log value table for col comp

extern	WORD	type;			// M=mand, J=Julia
extern	char	floatflag;		// floating point maths
extern	BYTE	juliaflag;		// Julia implementation of fractal
extern	BYTE	screenflag;		// replay saved screen
extern	int	logflag;		// log colour map required
extern	int	logval;			// log colour map start
extern	long	threshold;		// maximum iterations
extern	int	window_depth;		// data window size
extern	int	window_width;		// modify for Julia set
extern	BYTE	pairflag;		// stereo pair
extern	BYTE	save_flag;		// save screen after image
extern	WORD	colours;

extern	double	x_rot;			// angle display plane to x axis
extern	double	y_rot;			// angle display plane to y axis
extern	double	z_rot;			// angle display plane to z axis
extern	double	sclx, scly, sclz;	// scale

extern	int	RotationAngle;		// rotate image in degrees
extern	Complex	RotationCentre;		// centre of rotation

extern	double	hor;			// horizontal address
extern	double	vert;			// vertical address
extern	double	mandel_width;		// width of display
extern	double	ScreenRatio;		// ratio of width / height for the screen
extern	double	xgap;			// gap between pixels
extern	double	ygap;			// gap between pixels

extern	int	xdots, ydots, bits_per_pixel;

extern	int	InsideMethod;		// inside filters
extern	int	OutsideMethod;		// outside filters
extern	double	dStrands;		// for Tierazon filters
extern	int	nFDOption;		// Fractal Dimension option for Tierazon filters
extern	BOOL	UseCurrentPalette;	// do we use the ManpWIN palette for Tierazon filters? If false, generate internal filter palette
extern	double	potparam[];		// potential parameters
extern	int	distest, distestwidth;	// distance estimation

// Note that julia parameters are also used as starting values for Mandelbrot

extern	double	param[];
extern	double	rqlim;

// number declarations **********************************************************
extern	BYTE	BigNumFlag;		// True if bignum used
extern	MATH_TYPE MathType;
extern	int	decimals, precision;
extern	BigDouble	BigBailout, BigCloseEnough, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth;
extern	dd_real	DDBailout, DDCloseEnough, DDxgap, DDygap, DDHor, DDVert, DDWidth;
extern	qd_real	QDBailout, QDCloseEnough, QDxgap, QDygap, QDHor, QDVert, QDWidth;

extern	int getprecbf_mag(void);
// number declarations **********************************************************

extern	struct	svga	*video_type;	/* card specific */

void	get_julia_loc(char *);

extern 	void	bell(int), NewtonSetup(void), setup_defaults(void), 
		InitTrueColourPalette(BYTE)/*, DisplayPalette(HWND, BOOL)*/;

extern	int	mainview(HWND, BOOL);
extern	int	init_big_2(HWND);
extern	void	ConvertString2Bignum(mpfr_t num, char *s);
extern	int	ChangeBigPrecision(int decimals);

struct MP {
   double val;
};

struct MPC {
        struct MP x, y;
};
struct Arg {
    Complex     d;
   struct MPC m;
//   _LCMPLX    l;
};

extern	struct	Arg *Arg1, *Arg2;

extern	CTrueCol    TrueCol;		// palette info
extern	CFract	Fractal;
extern	Complex	j;

/**************************************************************************
	Setup log table
**************************************************************************/

void	init_log(HWND hwnd)

    {
    long	lf, prev, limit, LocalThreshold;
    double	m;
    int		i;

    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
    if (logflag == NULL)
	{
	if ((logtable = new BYTE [MAXTHRESHOLD + 2]) == NULL)
	    {
	    MessageBox (hwnd, "Can't allocate log palette memory. Using windows default", "Error", MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    logflag = ERROR;				// don't use if no memory
	    return;					// no point in setting up table
	    }
	logflag = TRUE;
	}
    lf = logval;
    m = log((double) (LocalThreshold - lf)) / ((double) (colours - ((lf) ? 2 : 1)));
    for (prev = 1; prev <= lf; ++prev)
	*(logtable + prev) = 1;
    for (i = ((lf) ? 2 : 1); i < colours; ++i)
	{ 
	limit = lf + (int) exp((double) i * m);
	if (limit > LocalThreshold || i == colours - 1)
	    limit = LocalThreshold;
	while (prev <= limit)
	    *(logtable + prev++) = i;
	}
    *logtable = 0;
    for (i = 1; i < LocalThreshold; ++i)		// spread top include unused colours
	if (*(logtable + i) > *(logtable + i - 1))
	    *(logtable + i) = *(logtable + i - 1) + 1;
//    for (i = threshold; i < MAXTHRESHOLD - 1; ++i)		// init the rest
//	*(logtable + i) = colours - 1;
    }

/**************************************************************************
	Initialise screen and files
**************************************************************************/

void	init(HWND hwnd)

    {
    static	struct Arg argfirst,argsecond;
    int	i;

    if (first_init)
	first_init = FALSE;
    else
	return;

    colours = 256;
    bits_per_pixel = 24;

    Arg1 = &argfirst; Arg2 = &argsecond;		// needed by all the ?Stk* functions
    save_flag = FALSE;
    InitTrueColourPalette(FALSE);

    /////////					// initialise Fractal object
    Fractal.LoadFnArray();
    for (i = 0; i < NUMPARAM; i++)
	Fractal.ParamValue[i] = &param[i];
    rqlim = 4.0;
    Fractal.rqlim = &rqlim;
    /////////					// initialise Fractal object

    mainview(hwnd, FALSE);				// all screen specific stuff
    }

/**************************************************************************
	Get corner and width
**************************************************************************/

int	analyse_corner(char *s)

    {
    char	*t;
    char	*s1;
    char	*s2;
    char	*s3;
    char	*s4;
    char	*s5;

//    int		len1, len2, len3, len4, len5;
    t = s;
    while(*t)
	{
	if (!isdigit(*t) && *t != '.' && *t != '+' && *t != '-' && *t != 'e' && *t != 'E')
	    *t = ' ';
	t++;
	}
    // keep full size as we don't know how big they are before we analyse them
    s1 = new char[SIZEOF_BF_VARS];
    s2 = new char[SIZEOF_BF_VARS];
    s3 = new char[SIZEOF_BF_VARS];
    s4 = new char[SIZEOF_BF_VARS];
    s5 = new char[SIZEOF_BF_VARS];
    sscanf(s, "%s %s %s %s %s", s1, s2, s3, s4, s5);
    sscanf(s, "%lf %lf %lf %lf %lf", &hor, &vert, &mandel_width, &param[0], &param[1]);

    if (mandel_width < DBL_MIN)						// we can do a BigNum calculation here to allow deeper zooming
	{
	ConvertString2Bignum(BigWidth.x, s3);
	BigNumFlag = TRUE;
	}

    precision = getprecbf_mag();
    if (precision < 0)							// exceeded allowable precision
	{
	if (s1) delete[] s1;
	if (s2) delete[] s2;
	if (s3) delete[] s3;
	if (s4) delete[] s4;
	if (s5) delete[] s5;
	return -1;
	}
    if (precision > DBL_DIG - 3)
	{
	decimals = precision + PRECISION_FACTOR;
//	if (init_big_dec(dec) < 0)
//	    return -1;
	if (ChangeBigPrecision(decimals) < 0)				// increase precision of Big numbers	
	    {
	    if (s1) delete[] s1;
	    if (s2) delete[] s2;
	    if (s3) delete[] s3;
	    if (s4) delete[] s4;
	    if (s5) delete[] s5;
	    return -1;							// too many decimals for library
	    }

	BigNumFlag = TRUE;
	BigBailout = rqlim;
	ConvertString2Bignum(BigHor.x, s1);
	ConvertString2Bignum(BigVert.x, s2);
	ConvertString2Bignum(BigWidth.x, s3);
	if (mpfr_sgn(BigWidth.x) == 0)					// no naughty division
	    mpfr_set_d(BigWidth.x, 1.0, MPFR_RNDN);
	}
    else
	{
	BigNumFlag = FALSE;
	if (mandel_width < DBL_MIN)
	    mandel_width = 1.0;
	}
	if (s1) delete[] s1;
	if (s2) delete[] s2;
	if (s3) delete[] s3;
	if (s4) delete[] s4;
	if (s5) delete[] s5;
	return 0;
    }

/**************************************************************************
	Get 3D parameters
**************************************************************************/

char	*strtok1(register char * s1, register const char * s2)	// required to use different 
								// static variable to main 
								// strtok() in user.c
    {
    static	char	*sp;

    if(!s1)
	s1 = sp;
    if(!s1)
	return NULL;
    s1 += strspn(s1, s2);	/* skip leading delimiters */
    if(!*s1)
	return sp = NULL;
    sp = s1 + strcspn(s1, s2);
    if(*sp)
	*sp++ = 0;
    else
	sp = 0;
    return s1;
    }

void	AnalysePalette(char *s/*, HWND hwnd*/)

    {
    char	*t;      
    char	seps[]   = " \t\n";
    char	*token;

    t = s;
    while(*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-')
	    *s = ' ';
	s++;
	}

    token = strtok1(t, seps);
    if (!token)
	return;
    TrueCol.RedStartInt = atoi(token);
    token = strtok1(NULL, seps);
    if (!token)
	return;
    TrueCol.GreenStartInt = atoi(token);
    token = strtok1(NULL, seps);
    if (!token)
	return;
    TrueCol.BlueStartInt = atoi(token);
    token = strtok1(NULL, seps);
    if (!token)
	return;
    TrueCol.RedIncInt = atoi(token);
    token = strtok1(NULL, seps);
    if (!token)
	return;
    TrueCol.GreenIncInt = atoi(token);
    token = strtok1(NULL, seps);
    if (!token)
	return;
    TrueCol.BlueIncInt = atoi(token);
    return;
    }

/**************************************************************************
	Get Distance Estimation Params
**************************************************************************/

void	AnalyseDistEst(char *s)

    {
    char	*t;
    int	r;

    t = s;
    while (*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-')
	    *s = ' ';
	s++;
	}
    r = sscanf(t, "%d, %d", &distest, &distestwidth);
    }

/**************************************************************************
	Get 3D parameters
**************************************************************************/

void	analyse_3d(char *s)

    {
    char	*t;
    int	r;

    t = s;
    while (*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-')
	    *s = ' ';
	s++;
	}
    r = sscanf(t, "%lf %lf %lf %lf %lf %lf", &sclx, &scly, &sclz, &x_rot, &y_rot, &z_rot);
    }

/**************************************************************************
	Get 3D parameters
**************************************************************************/

void	AnalyseRotation(char *s)

    {
    char	*t;
    int		r;

    t = s;
    while (*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-')
	    *s = ' ';
	s++;
	}
    r = sscanf(t, "%d %lf %lf", &RotationAngle, &RotationCentre.x, &RotationCentre.y);
    }


/**************************************************************************
	Get filter parameters
**************************************************************************/

void	AnalyseMethod(char *s)

    {
    char	*t;      
    int		r, method;
    bool	IsInside;
    IsInside = (*s == 'I');

    t = s + 1;
    while(*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-')
	    *s = ' ';
	s++;
	}

    method = atoi(t);
    if (IsInside)
	InsideMethod = method;
    else
	OutsideMethod = method;
    if (method == POTENTIAL)
	r = sscanf(t+3, "%lf %lf %lf", &potparam[0], &potparam[1], &potparam[2]);
    else
	r = sscanf(t+3, "%lf %d %d", &dStrands, &nFDOption, &UseCurrentPalette);
    }

/**************************************************************************
	Get Inside Colour
**************************************************************************/

void	analyse_inside(char *s)

    {
    char	*t;      
    char	seps[]   = " \t\n";
    char	*token;

    t = s;
    while(*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-')
	    *s = ' ';
	s++;
	}
    token = strtok1(t, seps);
    if (!token)
	return;
    TrueCol.inside_colour = atoi(token);
    token = strtok1(NULL, seps);
    if (!token)
	return;
    TrueCol.InsideRed = atoi(token);
    token = strtok1(NULL, seps);
    if (!token)
	return;
    TrueCol.InsideGreen = atoi(token);
    token = strtok1(NULL, seps);
    if (!token)
	return;
    TrueCol.InsideBlue = atoi(token);
    }

/**************************************************************************
	Get Julia location on Mandelbrot set
**************************************************************************/

void	get_julia_loc(char *s)

    {
    char	*t;

    t = s;
    while(*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e' && *s != 'E')
	    *s = ' ';
	s++;
	}
    sscanf(t, "%lf %lf", &(j.x), &(j.y));
    }



