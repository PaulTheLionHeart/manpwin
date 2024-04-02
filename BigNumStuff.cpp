/*
BigNumStuff.cpp a module for conversion of number to and from BigNums.

Written in Microsoft Visual 'C++' by Paul de Leeuw.

This program is written in "standard" C.Hardware dependant code
(console drivers & serial I / O) is in separate machine libraries.
*/

#include	<Windows.h>
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"complex.h"
#include	"big.h"
#include	"fractalp.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))

extern	double	hor;			// horizontal address
extern	double	vert;			// vertical address
extern	double	xgap;			// gap between pixels
extern	double	ygap;			// gap between pixels
extern	double	yymax;			// max value of vert
extern	double	mandel_width;		// width of display
extern	double	closenuff;		// periodicity bailout
extern	double	rqlim;			// bailout level

extern	Complex	c;
extern	Complex	z, q;

extern	HWND	PixelHwnd;		// pointer to window handle
extern	WORD	type;			// M=mand, J=Julia 1,2,4->

BigDouble	BigCloseEnough, BigBailout, Big_xxmax, Big_yymax, Big_xxmin, Big_yymin, Big_xgap, Big_ygap, BigHor, BigVert, BigWidth;
BigComplex	zBig, cBig, qBig;

BYTE	BigNumFlag = FALSE;		// True if bignum used
int	decimals = 10, precision;

static	int	OldPrecision = 0;

extern	int	ReinitBigVars(int);

/**************** a handy little debugging tool *********************/
#ifdef	_DEBUG
extern	HWND		PixelHwnd;	// pointer to handle for pixel updating
//extern	int		decimals;
extern	void	ConvertBignum2String(char *s, mpfr_t num);
void	ShowBignum(BigDouble x, char *Location)
    {
    char    *s1;
    s1 = new char [SIZEOF_BF_VARS + 1];
    char    ss[24];
    ConvertBignum2String(s1, x.x);
    sprintf(ss, "bflength=%d", (int)strlen(s1));
    MessageBox(PixelHwnd, s1, Location, MB_ICONEXCLAMATION | MB_OK);
    //    MessageBox (PixelHwnd, s1, ss, MB_ICONEXCLAMATION | MB_OK);
    if (s1) { delete[] s1; s1 = NULL; }
    }
#endif
/**************** a handy little debugging tool *********************/

/************************************************************************
    Convert number types
*************************************************************************/

void BigCornerstoFloat(void)
    {
    c.x = (double)mpfr_get_d(cBig.x.x, MPFR_RNDN);
    c.y = (double)mpfr_get_d(cBig.y.x, MPFR_RNDN);
    yymax = (double)mpfr_get_d(Big_yymax.x, MPFR_RNDN);
    xgap = (double)mpfr_get_d(Big_xgap.x, MPFR_RNDN);
    ygap = (double)mpfr_get_d(Big_ygap.x, MPFR_RNDN);
    z.x = (double)mpfr_get_d(zBig.x.x, MPFR_RNDN);
    z.y = (double)mpfr_get_d(zBig.y.x, MPFR_RNDN);
    hor = (double)mpfr_get_d(BigHor.x, MPFR_RNDN);
    vert = (double)mpfr_get_d(BigVert.x, MPFR_RNDN);
    mandel_width = (double)mpfr_get_d(BigWidth.x, MPFR_RNDN);
    q.x = (double)mpfr_get_d(qBig.x.x, MPFR_RNDN);
    q.y = (double)mpfr_get_d(qBig.y.x, MPFR_RNDN);
    closenuff = (double)mpfr_get_d(BigCloseEnough.x, MPFR_RNDN);
    BigNumFlag = FALSE;
    }

/************************************************************************
    Convert number types
*************************************************************************/

void FloatCornerstoBig(void)
    {
    cBig.x = c.x;
    cBig.y = c.y;
    Big_yymax = yymax;
    Big_xgap = xgap;
    Big_ygap = ygap;
    zBig.x = z.x;
    zBig.y = z.y;
    BigHor = hor;
    BigVert = vert;
    BigWidth = mandel_width;
    qBig.x = q.x;
    qBig.y = q.y;
    BigCloseEnough = closenuff;
    BigBailout = rqlim;
    BigNumFlag = TRUE;
    }
/*
void FloatCornerstoBig(void)
    {
    cBig.x = c.x;
    cBig.y = c.y;
    Big_yymax = yymax;
    Big_xgap = xgap;
    Big_ygap = ygap;
    zBig.x = z.x;
    zBig.y = z.y;
    BigHor = hor;
    BigVert = vert;
    BigWidth = mandel_width;
    qBig.x = q.x;
    qBig.y = q.y;
    BigCloseEnough = closenuff;
    BigBailout = 4.0;
    BigNumFlag = TRUE;
    }
*/

/***********************************************************************
    This function calculates the precision needed to distiguish adjacent
    pixels at Fractint's maximum resolution of MAXPIXELS by MAXPIXELS
    (if rez==MAXREZ) or at current resolution (if rez==CURRENTREZ)    
************************************************************************/

#ifdef	DEBUG
extern	void	ShowBignum(BigDouble, char *);
#endif


/*
Orig
*/
int getprecbf_mag(void)

    {
    LDBL	rez;
    BigDouble	temp;
    int	LocalDecimals;

    LocalDecimals = 1;

    if (BigNumFlag)
	{
	mandel_width = (double)mpfr_get_d(BigWidth.x, MPFR_RNDN);
	if (mpfr_sgn(BigWidth.x) == 0 || mpfr_sgn(BigWidth.x) < 0)
	    //	    if (MPFR_FLAGS_ERANGE)
	    return -1;				// no infinite loops
	}
						// which will prevent lack of precision
    if (mandel_width < DBL_MIN)			// we can do a BigNum calculation here to allow deeper zooming
	{
	temp = BigWidth;
	rez = (double)mpfr_get_d(temp.x, MPFR_RNDN);	// maybe we can get a few more decimals out of this
	while (rez < DBL_MIN)
	    {
	    temp = temp * 10.0;
//	    ShowBignum(temp, "temp");
	    rez = (double)mpfr_get_d(temp.x, MPFR_RNDN);	// maybe we can get a few more decimals out of this
	    LocalDecimals++;
	    }
	}
    else
	rez = mandel_width;
    if (LocalDecimals > SIZEOF_BF_VARS - PRECISION_FACTOR)	// okay, I give up. Not enough memory for variables
	return -1;
    while (rez < 1.0 && LocalDecimals < SIZEOF_BF_VARS)
	{
	LocalDecimals++;
	rez *= 10.0;
	}
    LocalDecimals = max(LocalDecimals, 3);
    return(LocalDecimals);
    }

/*
int getprecbf_mag(void)

    {
    BigDouble	rez;
    BigDouble	temp, temp1 = DBL_MIN;
    int	LocalDecimals;

    LocalDecimals = 1;

    if (BigNumFlag)
	{
	mandel_width = (double)mpfr_get_d(BigWidth.x, MPFR_RNDN);
	if (mpfr_sgn(BigWidth.x) == 0 || mpfr_sgn(BigWidth.x) < 0)
//	    if (MPFR_FLAGS_ERANGE)
		return -1;			// no infinite loops
	}
						// which will prevent lack of precision
    if (mandel_width < DBL_MIN)			// we can do a BigNum calculation here to allow deeper zooming
	{
	temp = BigWidth;
	rez = temp;				// maybe we can get a few more decimals out of this
	while (rez < temp1)
	    {
	    temp = temp * 10.0;
//	    ShowBignum(temp, "temp");
	    rez = temp;	
	    LocalDecimals++;
	    }
	}
    else
	rez = mandel_width;
    if (LocalDecimals > SIZEOF_BF_VARS - PRECISION_FACTOR)	// okay, I give up. Not enough memory for variables
	return -1;
    temp1 = 1.0;
    while(rez < temp1 && LocalDecimals < SIZEOF_BF_VARS)
	{
	LocalDecimals++;
	rez *= 10.0;
	}
    LocalDecimals = max(LocalDecimals,3);
    return(LocalDecimals);
    }
*/

/*************************************************************************
    Determine if Bignum is required and if so, what precision 
*************************************************************************/

BOOL	IsBignumFractal(int type)
    {
    int	    i;
    BOOL    IsBig = FALSE;

    for (i = 0; BigFractalSpecific[i].big_calctype; i++)
	if (type == BigFractalSpecific[i].type)	// check the list of "allowed" fractals.    
	    {
	    IsBig = TRUE;
	    break;
	    }
    return IsBig;
    }

int	calcfracinit(void)

    {
    precision = getprecbf_mag();
    decimals = precision + PRECISION_FACTOR;
//    decimals = precision + PRECISION_FACTOR * 3;	// add a bit of margin. Trade off speed against deeper precision.
//    decimals = SIZEOF_BF_VARS;			// full precision at a rediculous slow speed

    if (BigNumFlag)
	{
	if (precision <= DBL_DIG - 3)	// switch back to double when zooming out if using BigNum
//	if (precision <= DBL_DIG - 13)	// switch back to double when zooming out if using BigNum
	    {
	    BigCornerstoFloat();
	    OldPrecision = 0;
	    }
//	if (precision > OldPrecision + 3)
	if (precision > OldPrecision + 10)
	    {
//  This is a cludge that resets all the internal parameters of Bignum and allows ongoing increases in the number of decimals
	    if (ReinitBigVars(decimals) < 0)
		{
		MessageBox (PixelHwnd, "Ran out of resolution - retry", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		return FALSE; 
		}

	    OldPrecision = precision;
	    }
	}
//    else if ((precision > DBL_DIG - 13) && IsBignumFractal(type))
    else if ((precision > DBL_DIG - 3) && IsBignumFractal(type))
	{					// switch to BigNum when zooming in if using double
	OldPrecision = 0;
//	if (init_big_dec(decimals) < 0)
//	    return -1;
	FloatCornerstoBig();
	}
    return 0;
    }


