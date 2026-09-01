/*
Arithmetic.cpp - a module for conversion of number to and from BigNums.

Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include	<Windows.h>
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"complex.h"
#include	"big.h"
#include	"fractalp.h"
#include	"manp.h"
#include	"Arithmetic.h"
#include	"DDMatrix.h"
#include	"QDMatrix.h"
#include	"BigMatrix.h"
#include	"DDComplex.h"
#include	"QDComplex.h"
#include	"SafeStrings.h"

#define max(a,b)    (((a) > (b)) ? (a) : (b))

//int	decimals = 10;

/**************** a handy little debugging tool *********************/
#ifdef	_DEBUG
void ShowBignum(BigDouble x, char *Location)
    {
    char s1[SIZEOF_BF_VARS + 1]{};
    x.ToString(s1, SIZEOF_BF_VARS + 1, false);
    MessageBox(gManp->GlobalHwnd, s1, Location, MB_ICONEXCLAMATION | MB_OK);
    }
#endif
/**************** a handy little debugging tool *********************/

/************************************************************************
    Convert number types
*************************************************************************/

void	CManp::BigCornerstoFloat(void)
    {
    hor = (double)mpfr_get_d(BigHor.x, MPFR_RNDN);
    vert = (double)mpfr_get_d(BigVert.x, MPFR_RNDN);
    mandel_width = (double)mpfr_get_d(BigWidth.x, MPFR_RNDN);
    BigNumFlag = FALSE;
    }

/************************************************************************
    Convert number types
*************************************************************************/

void	CManp::FloatCornerstoBig(void)
    {
    BigHor = hor;
    BigVert = vert;
    BigWidth = mandel_width;
    BigNumFlag = TRUE;
    }

/***********************************************************************
    This function calculates the precision needed to distiguish adjacent
    pixels at Fractint's maximum resolution of MAXPIXELS by MAXPIXELS
    (if rez==MAXREZ) or at current resolution (if rez==CURRENTREZ)    
************************************************************************/

#ifdef	DEBUG
extern	void	ShowBignum(BigDouble, char *);
#endif

int	CManp::getprecbf_mag(void)
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

/*************************************************************************
    Determine if Bignum is required and if so, what precision 
*************************************************************************/

BOOL	CManp::IsBignumFractal(int type)
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

int	CManp::calcfracinit(void)
    {
    precision = getprecbf_mag();
    if (precision < 0)
	return -1;

    decimals = precision + PRECISION_FACTOR;

    if (BigNumFlag)
	{
	if (precision <= DBL_DIG - 3)	// switch back to double when zooming out if using BigNum
	    {
	    BigCornerstoFloat();
	    OldPrecision = 0;
	    }
	// Update in 10-digit steps to avoid changing MPFR precision on every
	// small increase in the required calculation precision.
	if (precision > OldPrecision + 10)
	    {
	    // Periodically increase the precision of persistent BigNum variables
	    // as the zoom deepens.  Existing values are preserved.
	    if (ChangeBigPrecision(decimals) < 0)
		{
		MessageBox (GlobalHwnd, "Ran out of resolution - retry", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		return -1;
		}

	    OldPrecision = precision;
	    }
	}
    else if ((precision > DBL_DIG - 3) && IsBignumFractal(type))
	{					// switch to BigNum when zooming in if using double
	OldPrecision = 0;
	FloatCornerstoBig();
	}
    return 0;
    }


