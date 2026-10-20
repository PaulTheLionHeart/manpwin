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
    Initialise logarithmic colour mapping table

    The vector owns its memory. No manual allocation or deletion is needed.
**************************************************************************/

void CManp::init_log(HWND hwnd)
    {
    long lf, prev, limit, LocalThreshold;
    double m;
    int i;

    LocalThreshold = (threshold >= MAXTHRESHOLD)
	? MAXTHRESHOLD - 1 : threshold;

    // Allocate and initialise the logarithmic colour table.
    logtable.resize(MAXTHRESHOLD + 2);

    logflag = TRUE;

    lf = logval;
    m = log((double)(LocalThreshold - lf)) /
	((double)(colours - ((lf) ? 2 : 1)));

    for (prev = 1; prev <= lf; ++prev)
	logtable[prev] = 1;

    for (i = ((lf) ? 2 : 1); i < colours; ++i)
	{
	limit = lf + (int)exp((double)i * m);

	if (limit > LocalThreshold || i == colours - 1)
	    limit = LocalThreshold;

	while (prev <= limit)
	    logtable[prev++] = i;
	}

    logtable[0] = 0;

    for (i = 1; i < LocalThreshold; ++i)
	{
	if (logtable[i] > logtable[i - 1])
	    logtable[i] = logtable[i - 1] + 1;
	}
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
    // Fractal.ParamValue must cover the largest parameter family, not just NUMPARAM.
    for (i = 0; i < MAXPARAM; i++)
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
    std::vector<char>	s1(SIZEOF_BF_VARS);
    std::vector<char>	s2(SIZEOF_BF_VARS);
    std::vector<char>	s3(SIZEOF_BF_VARS);
    std::vector<char>	s4(SIZEOF_BF_VARS);
    std::vector<char>	s5(SIZEOF_BF_VARS);

    sscanf(s, "%s %s %s %s %s", s1.data(), s2.data(), s3.data(), s4.data(), s5.data());
    sscanf(s, "%lf %lf %lf %lf %lf", &hor, &vert, &mandel_width, &param[0], &param[1]);

    if (mandel_width < DBL_MIN)						// we can do a BigNum calculation here to allow deeper zooming
	{
	ConvertString2Bignum(BigWidth.x, s3.data());
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
	ConvertString2Bignum(BigHor.x, s1.data());
	ConvertString2Bignum(BigVert.x, s2.data());
	ConvertString2Bignum(BigWidth.x, s3.data());
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
Get Rotation parameters
**************************************************************************/

void CManp::AnalyseRotation(char *s)
    {
    char    *t;
    char    sx[SIZEOF_BF_VARS];
    char    sy[SIZEOF_BF_VARS];
    int     r;

    t = s;

    while (*s)
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e' && *s != 'E')
	    *s = ' ';
	s++;
	}

    r = sscanf(t, "%d %s %s", &RotationAngle, sx, sy);

    if (r != 3)
	return;

    if (BigNumFlag)
	{
	ConvertString2Bignum(BigRotationCentre.x.x, sx);
	ConvertString2Bignum(BigRotationCentre.y.x, sy);
	}
    else
	{
	RotationCentre.x = strtod(sx, nullptr);
	RotationCentre.y = strtod(sy, nullptr);

	BigRotationCentre.x = RotationCentre.x;
	BigRotationCentre.y = RotationCentre.y;
	}
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
    Process fractal transformation parameters
**************************************************************************/

int CManp::ProcessTransformation(char *s)
    {
    char    *t;
    int     count;
    int     TransformValue;

    t = s;
    while (*s && *s != ' ')
	{
	if (!isdigit(*s) && *s != '.' && *s != '+' && *s != '-' && *s != 'e')
	    *s = ' ';
	s++;
	}

    count = sscanf(t, "%d", &TransformValue);

    if (count != 1)
	return TRANSFORMPAR_NO_TYPE;

    TransformType = (TRANSFORMTYPE)TransformValue;

    switch (TransformType)
	{
	case TRANSFORM_AFFINE:
	    count = sscanf(t, "%d %lf %lf %lf %lf %lf %lf",
		&TransformValue,
		&AffineParams.a,
		&AffineParams.b,
		&AffineParams.e,
		&AffineParams.c,
		&AffineParams.d,
		&AffineParams.f);

	    if (count != 7)
		return TRANSFORMPAR_AFFINE;
	    break;

	case TRANSFORM_MOBIUS:
	    count = sscanf(t, "%d %lf %lf %lf %lf %lf %lf %lf %lf",
		&TransformValue,
		&MobiusParams.aReal,
		&MobiusParams.aImag,
		&MobiusParams.bReal,
		&MobiusParams.bImag,
		&MobiusParams.cReal,
		&MobiusParams.cImag,
		&MobiusParams.dReal,
		&MobiusParams.dImag);

	    if (count != 9)
		return TRANSFORMPAR_MOBIUS;
	    break;

	case TRANSFORM_JOUKOWSKI:
	    count = sscanf(t, "%d %lf %lf",
		&TransformValue,
		&JoukowskiParams.kReal,
		&JoukowskiParams.kImag);

	    if (count != 3)
		return TRANSFORMPAR_JOUKOWSKI;
	    break;

	case TRANSFORM_TWIST:
	    count = sscanf(t, "%d %lf %lf %lf %lf",
		&TransformValue,
		&TwistParams.Strength,
		&TwistParams.Radius,
		&TwistParams.CentreX,
		&TwistParams.CentreY);

	    if (count != 5)
		return TRANSFORMPAR_TWIST;
	    break;

	default:
	    count = sscanf(t, "%d %lf %lf %lf",
		&TransformValue,
		&f_radius,
		&f_xcenter,
		&f_ycenter);

	    if (count != 4)
		return TRANSFORMPAR_NORMAL;
	    break;
	}

    return TRANSFORMPAR_OK;
    }


