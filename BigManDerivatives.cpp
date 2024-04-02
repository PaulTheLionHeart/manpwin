/*
    BigManDerivative.cpp a module for the per pixel calculations of Bignum fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#ifdef SPLAT

#include	<math.h>
#include	<stdio.h>
#include	"manp.h"
#include	"complex.h"
#include	"big.h"
#include	"fractype.h"
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"fractalp.h"

static	BigDouble	realimagBig;
static	double		distance, absolute;
extern	BigDouble	BigBailout;
extern	double		rqlim;			// bailout level

extern	BigComplex	zBig, qBig;
static	BigComplex	sqrBig;
static  BigDouble	tBig;			// nice temp variable

//static	BigComplex	aBig, a2Big, aa3Big, bBig, l2Big, lm5Big, lp5Big, ozBig, sqrBig, t2Big, t3Big, vBig;

/**************** a handy little debugging tool *********************/
#ifdef	_DEBUG
extern	HWND		PixelHwnd;	// pointer to handle for pixel updating
//extern	int		dec;
extern void		ShowBignum(BigDouble x, char *Location);
#endif
/**************** a handy little debugging tool *********************/

extern	long	iteration;			// globals for speed
extern	BYTE	/*BioFlag, */SpecialFlag;		// tell graphing routine of special conditions
extern	WORD	special;		// special colour for CBIN fractals
extern	int	biomorph;		// biomorph colour
extern	BYTE	degree;			// power
extern	int	decomp;			// number of decomposition colours
extern	BYTE	orbit_flag;		// display orbits?
extern	BYTE	juliaflag;		// Julia implementation of fractal
extern	BYTE	phaseflag;		// 3 phases for type SPECIALNEWT fractals
extern	WORD	type;			// M=mand, J=Julia 1,2,4->
extern	int	subtype;		// B=basin, S=stripe, N=normal
extern	int	reset_period;		// periodicity checking
//extern	int	period_level;		// 0 for no periodicity checking

extern	BYTE	pairflag;		// stereo pair
extern	double	param[];


/**************************************************************************
    Perpendicular Mandelbrot
    The equation is very similar to burning ship except you absolute just 
    the real factor when calculating the imaginary side.
    zi = abs(zr) * zi * -2.0 + JuliaI
    zr = zrsqr - zisqr + JuliaR

    Whereas burning ship is ...
    zi = abs(zr * zi) * 2.0 - JuliaI
    zr = zrsqr - zisqr - JuliaR
    Real -0.737,424,082,465,620,824,452,347,915,7­36,817,521,648,421,984,117,126,135,371,4
    Imaginary -0.355,631,230,436,806,548,631,431,830,9­06,449,574,310,522,006,013,120,497,532,0
    Zooms 182 magnification 6.13e54
***************************************************************************/

int	Do_big_PerpMandelbrot(void)

    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = zBig.x;
    realimagBig = tBig.BigAbs() * zBig.y;
    zBig.x = qBig.x + sqrBig.x - sqrBig.y;
    zBig.y = -(realimagBig + realimagBig - qBig.y);
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Perpendicular BurningShip
    The equation is very similar to burning ship except you absolute just 
    the imaginary factor instead of the real factor when calculating the imaginary side.
    zi = zr * abs(zi) * -2.0 + JuliaI
    zr = zrsqr - zisqr + JuliaR

    Whereas burning ship is ...
    zi = abs(zr * zi) * 2.0 - JuliaI
    zr = zrsqr - zisqr - JuliaR
    Real -0.737,424,082,465,620,824,452,347,915,7­36,817,521,648,421,984,117,126,135,371,4
    Imaginary -0.355,631,230,436,806,548,631,431,830,9­06,449,574,310,522,006,013,120,497,532,0
    Zooms 182 magnification 6.13e54
***************************************************************************/

int	Do_big_PerpBurningShip(void)

    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    realimagBig = zBig.x * zBig.y.BigAbs();
    zBig.x = qBig.x + sqrBig.x - sqrBig.y;
    zBig.y = realimagBig + realimagBig + qBig.y;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    A derivative of the Burning Ship, the Buffalo fractal was composed as follows: 
    Let,
	z0 = 0
	zn+1 = zn^2 - zn + c
    Take,
	z = x + i*y, 
    Then,
	z^2 - z + c = (x + i*y)2 - (x + i*y) + c
    Lastly, take the absolute values of x and y:
	(|x| + i*|y|)2 - (|x| + i*|y|) + c
***************************************************************************/

/*
int	Do_big_Buffalo(void)

    {
    BigComplex	tBig;

    tBig.x = zBig.x.BigAbs();
    tBig.y = zBig.y.BigAbs();
    sqrBig.x = tBig.x.BigSqr();
    sqrBig.y = tBig.y.BigSqr();
    realimagBig = tBig.x * tBig.y;
    zBig.x = qBig.x + sqrBig.x - sqrBig.y - tBig.x;
    zBig.y = realimagBig + realimagBig - qBig.y - tBig.y;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }
*/

/**************************************************************************
    Celtic
***************************************************************************/

int	Do_big_Celtic(void)

    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    if (degree == 2)
	{
	realimagBig = zBig.x * zBig.y;
	zBig.y = -realimagBig - realimagBig - qBig.y;
	tBig = sqrBig.x - sqrBig.y;
	zBig.x = -qBig.x - tBig.BigAbs();
	}
    else	// degree > 2
	{
	zBig = zBig.CPolynomial(degree);
	zBig.y += qBig.y;
	zBig.x = -qBig.x - zBig.x.BigAbs();
	}
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Buffalo - works according to Kalles Fraktaller
***************************************************************************/

int	Do_big_Buffalo(void)

    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    if (degree == 2)
	{
	zBig.y = zBig.x.BigAbs() * zBig.y.BigAbs() * -2.0 - qBig.y;
	tBig = sqrBig.x - sqrBig.y;
	zBig.x = qBig.x + tBig.BigAbs();
	}
    else if (degree == 3)
	{
	tBig = (sqrBig.x * 3.0 - sqrBig.y) * zBig.y;
	zBig.y = tBig.BigAbs() - qBig.y;
	tBig = (sqrBig.x - sqrBig.y * 3.0) * zBig.x;
	zBig.x = qBig.x + tBig.BigAbs();
	}
    else	// degree > 3
	{
	zBig = zBig.CPolynomial(degree);
	zBig.y = zBig.y.BigAbs() - qBig.y;
	zBig.x = zBig.x.BigAbs() + qBig.x;
	}
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Perpendicular Celtic
***************************************************************************/

int	Do_big_PerpCeltic(void)

    {
    tBig = zBig.x.BigAbs();
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = zBig.y * tBig * -2.0 + qBig.y;
    tBig = sqrBig.x - sqrBig.y;
    zBig.x = qBig.x + tBig.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Mandelbar Celtic
***************************************************************************/

int	Do_big_MandelbarCeltic(void)

    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = zBig.x * zBig.y * -2.0 + qBig.y;
    tBig = sqrBig.x - sqrBig.y;
    zBig.x = qBig.x + tBig.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Perpendicular Buffalo - according to Kalles Fraktaller
****************************************************************/

int	Do_big_PerpBuffalo(void)
    {
    tBig = zBig.y.BigAbs();
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = zBig.x * tBig * -2.0 - qBig.y;
    tBig = sqrBig.x - sqrBig.y;
    zBig.x = qBig.x + tBig.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Cubic Flying Squirrel (Buffalo Imaginary)
***************************************************************************/

int	Do_big_CubicFlyingSquirrel(void)

    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x * 3.0 - sqrBig.y) * zBig.y;
    zBig.y = tBig.BigAbs() - qBig.y;
    zBig.x = qBig.x + (sqrBig.x - sqrBig.y * 3.0) * zBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Heart Mandelbrot
****************************************************************/

int	Do_big_HeartMandelbrot(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = zBig.x.BigAbs() * zBig.y * 2.0 - qBig.y;
    zBig.x = sqrBig.x - sqrBig.y + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Celtic Heart 
****************************************************************/

int	Do_big_CelticHeart(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = zBig.x.BigAbs() * zBig.y * 2.0 - qBig.y;
    tBig = sqrBig.x - sqrBig.y;
    zBig.x = qBig.x + tBig.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Partial Cubic Burning Ship Real 
****************************************************************/

int	Do_big_PartialCubicBurningShipReal(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = (sqrBig.x * 3.0 - sqrBig.y) * zBig.y + qBig.y;
    zBig.x = qBig.x + (sqrBig.x - sqrBig.y * 3.0) * zBig.x.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Partial Cubic Burning Ship Imaginary 
****************************************************************/

int	Do_big_PartialCubicBurningShipImag(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = (sqrBig.x * 3.0 - sqrBig.y) * zBig.y.BigAbs() + qBig.y;
    zBig.x = qBig.x + (sqrBig.x - sqrBig.y * 3.0) * zBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Partial Cubic Buffalo Real (Celtic) 
****************************************************************/

int	Do_big_PartialCubicBuffaloReal(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = (sqrBig.x * 3.0 - sqrBig.y) * zBig.y + qBig.y;
    tBig = (sqrBig.x - sqrBig.y * 3.0) * zBig.x;
    zBig.x = qBig.x + tBig.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Cubic Quasi Burning Ship (Buffalo Burning Ship Hybrid)
****************************************************************/

int	Do_big_CubicQuasiBurningShip(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x * 3.0 - sqrBig.y) * zBig.y;
    zBig.y = -tBig.BigAbs() + qBig.y;
    zBig.x = qBig.x + (sqrBig.x - sqrBig.y * 3.0) * zBig.x.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Cubic Quasi Perpendicular
****************************************************************/

int	Do_big_CubicQuasiPerpendicular(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = sqrBig.x * 3.0 - sqrBig.y;
    zBig.y = -tBig.BigAbs() * zBig.y + qBig.y;
    zBig.x = qBig.x + (sqrBig.x - sqrBig.y * 3.0) * zBig.x.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    Cubic Quasi Heart
****************************************************************/

int	Do_big_CubicQuasiHeart(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = sqrBig.x * 3.0 - sqrBig.y;
    zBig.y = tBig.BigAbs() * zBig.y + qBig.y;
    zBig.x = qBig.x + (sqrBig.x - sqrBig.y * 3.0) * zBig.x.BigAbs();
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    4th Order Fractals:
****************************************************************/

/****************************************************************
    Non ABS Variations (2)
****************************************************************/

/****************************************************************
    Mandelbrot 4th Order
****************************************************************/

int	Do_big_Mandelbrot4thOrder(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    realimagBig = zBig.x * zBig.y;
    zBig.y = realimagBig * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
	Mandelbar 4th
***************************************************************************/

int	Do_big_Mandelbar4thOrder(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    realimagBig = zBig.x * zBig.y;
    zBig.y = -realimagBig * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    ***Straight ABS Variations (16)
***************************************************************************/

/**************************************************************************
	Burning Ship 4th
***************************************************************************/

int	Do_big_BurningShip4thOrder(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    realimagBig = zBig.x * zBig.y;
    zBig.y = realimagBig.BigAbs() * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Burning Ship 4th Partial Imag:
***************************************************************************/

int	Do_big_BurningShip4thPartImag(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = zBig.x * zBig.y.BigAbs() * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Burning Ship 4th Partial Real:
***************************************************************************/

int	Do_big_BurningShip4thPartReal(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = zBig.x.BigAbs() * zBig.y * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Burning Ship 4th Partial Real Mbar:
***************************************************************************/

int	Do_big_BurningShip4thPartRealMbar(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = -zBig.x.BigAbs() * zBig.y * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic Burning Ship 4th:
***************************************************************************/

int	Do_big_CelticBurningShip4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    realimagBig = zBig.x * zBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.y = realimagBig.BigAbs() * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic Burning Ship 4th Partial Imag:
***************************************************************************/

int	Do_big_CelticBurningShip4thPartImag(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.y = zBig.x * zBig.y.BigAbs() * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic Burning Ship 4th Partial Real:
***************************************************************************/

int	Do_big_CelticBurningShip4thPartReal(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.y = zBig.x.BigAbs() * zBig.y * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic Burning Ship 4th Partial Real Mbar:
***************************************************************************/

int	Do_big_CelticBurningShip4thPartRealMbar(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.y = -zBig.x.BigAbs() * zBig.y * 4.0 * (sqrBig.x - sqrBig.y) + qBig.y;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Buffalo 4th:
***************************************************************************/

int	Do_big_Buffalo4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = zBig.x * zBig.y * (sqrBig.x - sqrBig.y);
    zBig.y = tBig.BigAbs() * 4.0 + qBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Buffalo 4th Partial Imag:
***************************************************************************/

int	Do_big_Buffalo4thPartImag(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = zBig.x * zBig.y * (sqrBig.x - sqrBig.y);
    zBig.y = tBig.BigAbs() * 4.0 + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic (Buffalo 4th Partial Real):
***************************************************************************/

int	Do_big_CelticBuffalo4thPartReal(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = zBig.x * zBig.y * (sqrBig.x - sqrBig.y) * 4.0 + qBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic 4th Mbar:
***************************************************************************/

int	Do_big_Celtic4thMbar(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    zBig.y = -zBig.x * zBig.y * (sqrBig.x - sqrBig.y) * 4.0 + qBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    ***Quasi ABS Variations (10)
***************************************************************************/

/**************************************************************************
    False Quasi Perpendicular 4th:
***************************************************************************/

int	Do_big_FalseQuasiPerp4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = -zBig.x * zBig.y * tBig.BigAbs() * 4.0 + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    False Quasi Heart 4th:
***************************************************************************/

int	Do_big_FalseQuasiHeart4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = zBig.x * zBig.y * tBig.BigAbs() * 4.0 + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic False Quasi Perpendicular 4th:
***************************************************************************/

int	Do_big_CelticFalseQuasiPerp4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = -zBig.x * zBig.y * tBig.BigAbs() * 4.0 + qBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic False Quasi Heart 4th:
***************************************************************************/

int	Do_big_CelticFalseQuasiHeart4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = zBig.x * zBig.y * tBig.BigAbs() * 4.0 + qBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Imag Quasi Perpendicular / Heart 4th:
***************************************************************************/

int	Do_big_ImagQuasiPerpHeart4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = zBig.x * zBig.y.BigAbs() * tBig.BigAbs() * 4.0 + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Real Quasi Perpendicular 4th:
***************************************************************************/

int	Do_big_RealQuasiPerp4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = -zBig.x.BigAbs() * zBig.y * tBig.BigAbs() * 4.0 + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Real Quasi Heart 4th:
***************************************************************************/

int	Do_big_RealQuasiHeart4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = zBig.x.BigAbs() * zBig.y * tBig.BigAbs() * 4.0 + qBig.y;
    zBig.x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic Imag Quasi Perpendicular / Heart 4th:
***************************************************************************/

int	Do_big_CelticImagQuasiPerpHeart4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = zBig.x * zBig.y.BigAbs() * tBig.BigAbs() * 4.0 + qBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic Real Quasi Perpendicular 4th:
***************************************************************************/

int	Do_big_CelticRealQuasiPerp4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = -zBig.x.BigAbs() * zBig.y * tBig.BigAbs() * 4.0 + qBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic Real Quasi Heart 4th:
***************************************************************************/

int	Do_big_CelticRealQuasiHeart4th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    tBig = (sqrBig.x - sqrBig.y);
    zBig.y = zBig.x.BigAbs() * zBig.y * tBig.BigAbs() * 4.0 + qBig.y;
    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/****************************************************************
    5th Order Fractals:
****************************************************************/

static	BigComplex	sqrsqrBig;
static	BigDouble	RealImagSqrBig;

/**************************************************************************
    Mandelbrot 5th:
***************************************************************************/

int	Do_big_Mandelbrot5thOrder(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    zBig.y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y + qBig.y;
    zBig.x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Mandelbar 5th (Vertical):
***************************************************************************/

int	Do_big_Mandelbar5thOrderVert(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    zBig.y = -(sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y + qBig.y;
    zBig.x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Mandelbar 5th (horizontal):
***************************************************************************/

int	Do_big_Mandelbar5thOrderHor(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    zBig.y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y + qBig.y;
    zBig.x = -(sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Burning Ship 5th:
***************************************************************************/

int	Do_big_BurningShip5thOrder(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    zBig.y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y.BigAbs() + qBig.y;
    zBig.x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Buffalo 5th:
***************************************************************************/

int	Do_big_Buffalo5thOrder(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    tBig = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y;
    zBig.y = tBig.BigAbs() + qBig.y;
    tBig = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Burning Ship 5th Partial:
***************************************************************************/

int	Do_big_BurningShip5thPartial(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    zBig.y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y + qBig.y;
    zBig.x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Burning Ship 5th Partial Mbar:
***************************************************************************/

int	Do_big_BurningShip5thPartialMbar(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    zBig.y = -(sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y + qBig.y;
    zBig.x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic 5th (Buffalo 5th Partial):
***************************************************************************/

int	Do_big_Celtic5thBuffalo5thPartial(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    zBig.y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y + qBig.y;
    tBig = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Celtic 5th Mbar:
***************************************************************************/

int	Do_big_Celtic5thMbar(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    zBig.y = -(sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y + qBig.y;
    tBig = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x;
    zBig.x = tBig.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Quazi Burning Ship 5th (BS/Buffalo Hybrid):
***************************************************************************/

int	Do_big_QuaziBurningShip5thBSBuffaloHybrid(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    tBig = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig.y;
    zBig.y = -tBig.BigAbs() + qBig.y;
    zBig.x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Quazi Perpendicular 5th:
***************************************************************************/

int	Do_big_QuaziPerpendicular5th(void)
    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    tBig = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y);
    zBig.y = -tBig.BigAbs() * zBig.y + qBig.y;
    zBig.x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
    Quazi Heart 5th:
***************************************************************************/

int	Do_big_QuaziHeart5th(void)

    {
    sqrBig.x = zBig.x.BigSqr();
    sqrBig.y = zBig.y.BigSqr();
    sqrsqrBig.x = sqrBig.x.BigSqr();
    sqrsqrBig.y = sqrBig.y.BigSqr();
    RealImagSqrBig = sqrBig.x * sqrBig.y;
    tBig = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y);
    zBig.y = tBig.BigAbs() * zBig.y + qBig.y;
    zBig.x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig.x.BigAbs() + qBig.x;
    return (sqrBig.x + sqrBig.y > BigBailout);
    }

/**************************************************************************
	Run SimonBrot type fractals
	z^n * |z|^2 + c	    (normal)

	It would also be nice to see Simonbrot 3rd, 5th, and 7th, but the formulas for these fractals are 
	z1.5 * |z|2 + pixel, z2.5 * |z|2 + pixel, and z3.5 * |z|2 + pixel. Due to the fact that the powers of 
	some of the terms are fractions, this could be difficult to implement. In a roundabout way, there is 
	a power zero Simonbrot on Kalles Fraktaler, because the power zero Simonbrot is actually the Burning Ship.
**************************************************************************/

int	Do_big_SimonBrot(void)

    {
    BigComplex	zabsBig, tempzBig, sqrtzBig;
    BigComplex	half = 0.5;

    zabsBig.x = zBig.x.BigAbs();
    zabsBig.y = zBig.y.BigAbs();
    tempzBig.y = zBig.y * zabsBig.x + zBig.x * zabsBig.y;
    tempzBig.x = zBig.x * zabsBig.x - zBig.y * zabsBig.y;
    if (degree % 2 == 1)
	sqrtzBig = zBig^half;				// use square root power if degree is odd
    else
	sqrtzBig = 1.0;
    zBig = tempzBig.CPolynomial(degree / 2) * sqrtzBig + qBig;
    return (zBig.x.BigSqr() + zBig.y.BigSqr() > BigBailout);
    }

/**************************************************************************
	Run SimonBrot2 type fractals
	z^n * |z^2| + c	    (SimonBrot2)
**************************************************************************/

int	Do_big_SimonBrot2(void)

    {
    BigComplex	zabsBig, tempzBig, sqrtzBig;
    BigComplex	half = 0.5;

    tempzBig = zBig * zBig;
    zabsBig.x = tempzBig.x.BigAbs();
    zabsBig.y = -tempzBig.y.BigAbs();
    tempzBig = zabsBig;
    if (degree % 2 == 1)
	sqrtzBig = zBig^half;				// use square root power if degree is odd
    else
	sqrtzBig = 1.0;
    zBig = zBig.CPolynomial(degree / 2) * sqrtzBig * tempzBig + qBig;
    return (zBig.x.BigSqr() + zBig.y.BigSqr() > BigBailout);
    }

/**************************************************************************
	Kung Fu Panda type fractals
	z = abs(z*z)
	z = z * z + p
**************************************************************************/

int	Do_big_KungFuPanda(void)

    {
    BigComplex	t1, t2;

    t1 = zBig * zBig;
    t2.x = t1.x.BigAbs();
    t2.y = t1.y.BigAbs();
    zBig = t2 * t2 - qBig;

    return (zBig.x.BigSqr() + zBig.y.BigSqr() > BigBailout);
    }

/**************************************************************************
	HPDZ Buffalo type fractals
	z := (((x^2 - y^2) - |x|) + i (|2xy| - |y|)) + c
	or
	w := |x| + i |y|
	z := w^2 - w + c
**************************************************************************/

int	Do_big_HPDZ_Buffalo(void)

    {
    BigComplex w;

    w.x = zBig.x.BigAbs();
    w.y = zBig.y.BigAbs();
    zBig = w * w - w;
    zBig.x += qBig.x;
    zBig.y -= qBig.y;
    return (zBig.x.BigSqr() + zBig.y.BigSqr() > BigBailout);
    }

/**************************************************************************
	General Mandelbrot Derivatives Fractal
***************************************************************************/
/*
int	init_big_MandelDeriv(void) 
    {
    BigBailout = rqlim;
    return (MandelDerivSpecific[subtype].big_per_pixel());
    }


int	do_big_MandelDeriv(void) 

    {
    return (MandelDerivSpecific[subtype].big_calctype());
    }

*/

#endif

