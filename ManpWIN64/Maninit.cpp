/*
   MANINIT.CPP a module with initialising type routines for MANP.C.
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.
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
#include "..\parser\cmplx.h"
#include "colour.h"
#include "Arithmetic.h"

int	period_level;			// 0 for no periodicity checking
static	int	first_init = TRUE;	// first time once only init done

extern 	void	InitTrueColourPalette(BYTE);
extern	int	mainview(HWND, BOOL);

/**************************************************************************
	Setup log table
**************************************************************************/

void	CManp::init_log(HWND hwnd)
    {
    long	lf, prev, limit, LocalThreshold;
    double	m;
    int		i;

    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
    if (gManp->logflag == NULL)
	{
	if ((gManp->logtable = new BYTE [MAXTHRESHOLD + 2]) == NULL)
	    {
	    MessageBox (hwnd, "Can't allocate log palette memory. Using windows default", "Error", MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    gManp->logflag = ERROR;				// don't use if no memory
	    return;					// no point in setting up table
	    }
	gManp->logflag = TRUE;
	}
    lf = logval;
    m = log((double) (LocalThreshold - lf)) / ((double) (colours - ((lf) ? 2 : 1)));
    for (prev = 1; prev <= lf; ++prev)
	*(gManp->logtable + prev) = 1;
    for (i = ((lf) ? 2 : 1); i < colours; ++i)
	{ 
	limit = lf + (int) exp((double) i * m);
	if (limit > LocalThreshold || i == colours - 1)
	    limit = LocalThreshold;
	while (prev <= limit)
	    *(gManp->logtable + prev++) = i;
	}
    *gManp->logtable = 0;
    for (i = 1; i < LocalThreshold; ++i)		// spread top include unused colours
	if (*(gManp->logtable + i) > *(gManp->logtable + i - 1))
	    *(gManp->logtable + i) = *(gManp->logtable + i - 1) + 1;
//    for (i = threshold; i < MAXTHRESHOLD - 1; ++i)		// init the rest
//	*(logtable + i) = colours - 1;
    }

/**************************************************************************
	Initialise screen and files
**************************************************************************/

void	CManp::init(HWND hwnd)
    {
//    static	struct Arg argfirst,argsecond;
    int	i;

    if (first_init)
	first_init = FALSE;
    else
	return;

    colours = 256;
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

int	CManp::analyse_corner(char *s)
    {
    char	*t;

    t = s;
    while(*t)
	{
	if (!isdigit(*t) && *t != '.' && *t != '+' && *t != '-' && *t != 'e' && *t != 'E')
	    *t = ' ';
	t++;
	}
    // keep full size as we don't know how big they are before we analyse them
    char s1[SIZEOF_BF_VARS]{};
    char s2[SIZEOF_BF_VARS]{};
    char s3[SIZEOF_BF_VARS]{};    
    char s4[SIZEOF_BF_VARS]{};
    char s5[SIZEOF_BF_VARS]{};

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
	return -1;
	}
    if (precision > DBL_DIG - 3)
	{
	decimals = precision + PRECISION_FACTOR;
//	if (init_big_dec(dec) < 0)
//	    return -1;
	if (ChangeBigPrecision(decimals) < 0)				// increase precision of Big numbers	
	    {
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
    return 0;
    }

/**************************************************************************
	Get 3D parameters
**************************************************************************/

char	*strtok1(char * s1, const char * s2)	// required to use different 
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

void	CManp::AnalysePalette(char *s)
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

void	CManp::AnalyseDistEst(char *s)
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

void	CManp::analyse_3d(char *s)
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

void	CManp::AnalyseRotation(char *s)
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

void	CManp::AnalyseMethod(char *s)
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

void	CManp::analyse_inside(char *s)
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

void	CManp::get_julia_loc(char *s)
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

/**************************************************************************
	Process Invert paramemters
**************************************************************************/

int	CManp::ProcessInvert(char *s)
    {
    char	*t;
    int		count;

    t = s;
    while (*s && *s != ' ')
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e')
	    *s = ' ';
	s++;
	}

    count = sscanf(t, "%lf %lf %lf", &f_radius, &f_xcenter, &f_ycenter);    // inversion radius, center 
    if (f_radius)
	invert = TRUE;
    return 0;
    }



