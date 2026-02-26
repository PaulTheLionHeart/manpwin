/*
    BigMandelDerivatives.CPP a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	"manp.h"
#include	"Complex.h"
#include	"big.h"
#include	"fractype.h"
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"resource.h"
#include	"pixel.h"



extern	void	ShowBignum(BigDouble x, char *Location);


/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::BigInitManDerFunctions(int subtype, BigComplex *zBig, BigComplex *qBig)
    {
    switch (subtype)
	{
	case 0:				// Perpendicular Mandelbrot
	case 1:				// Burning Ship
	case 3:				// Perpendicular Burning Ship
	case 5:				// Perpendicular Buffalo
	case 8:				// Mandelbar Celtic
	case 9:				// Perpendicular Celtic
	case 10:			// Cubic Flying Squirrel (Buffalo Imaginary)
	case 11:			// Heart Mandelbrot
	case 12:			// Celtic Heart
	case 13:			// Partial Cubic Burning Ship Real
	case 14:			// Partial Cubic Burning Ship Imaginary
	case 15:			// Partial Cubic Buffalo Real (Celtic)
	case 16:			// Cubic Quasi Burning Ship (Buffalo Burning Ship Hybrid)
	case 17:			// Cubic Quasi Perpendicular
	case 18:			// Cubic Quasi Heart
	case 19:			// Mandelbrot 4th Order
	case 20:			// Mandelbar 4th Order
	case 21:			// Burning Ship 4th Order
	case 22:			// Burning Ship 4th Partial Imag
	case 23:			// Burning Ship 4th Partial Real
	case 24:			// Burning Ship 4th Partial Real Mbar
	case 25:			// Celtic Burning Ship 4th
	case 26:			// Celtic Burning Ship 4th Partial Imag
	case 27:			// Celtic Burning Ship 4th Partial Real
	case 28:			// Celtic Burning Ship 4th Partial Real Mbar
	case 29:			// Buffalo 4th Order
	case 30:			// Buffalo 4th Partial Imag
	case 31:			// Celtic (Buffalo 4th Partial Real)
	case 32:			// Celtic 4th Mbar
	case 33:			// False Quasi Perpendicular 4th
	case 34:			// False Quasi Heart 4th
	case 35:			// Celtic False Quasi Perpendicular 4th
	case 36:			// Celtic False Quasi Heart 4th
	case 37:			// Imag Quasi Perpendicular / Heart 4th
	case 38:			// Real Quasi Perpendicular 4th
	case 39:			// Real Quasi Heart 4th
	case 40:			// Celtic Imag Quasi Perpendicular / Heart 4th
	case 41:			// Celtic Real Quasi Perpendicular 4th
	case 42:			// Celtic Real Quasi Heart 4th
	case 43:			// Mandelbrot 5th
	case 44:			// Mandelbar 5th (Vertical)
	case 45:			// Mandelbar 5th (horizontal)
	case 46:			// Burning Ship 5th
	case 47:			// Buffalo 5th
	case 48:			// Burning Ship 5th Partial
	case 49:			// Burning Ship 5th Partial Mbar
	case 50:			// Celtic 5th (Buffalo 5th Partial)
	case 51:			// Celtic 5th Mbar
	case 52:			// Quazi Burning Ship 5th (BS/Buffalo Hybrid)
	case 53:			// Quazi Perpendicular 5th
	case 54:			// Quazi Heart 5th
	case 57:			// Kung Fu Panda
	case 58:			// HPDZ Buffalo
	case 59:			// SzegediButterfly 1
	case 60:			// SzegediButterfly 2

	    BigInitFunctions(MANDELFP, zBig, qBig);
	    break;
	case 2:				// Burning Ship of Higher Degree
	case 4:				// Buffalo
	case 6:				// Mandelbar (Tricorn)
	case 7:				// Celtic
	case 55:			// SimonBrot
	case 56:			// SimonBrot2
	    BigInitFunctions(POWER, zBig, qBig);
	    break;
	}


    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::BigRunManDerFunctions(int subtype, BigComplex *zBig, BigComplex *qBig, BYTE *SpecialFlag, long *iteration)
    {
    switch (subtype)
	{
	case 0:						// Perpendicular Mandelbrot

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

	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = zBig->x;
	    realimagBig = tBig.BigAbs() * zBig->y;
	    zBig->x = qBig->x + sqrBig.x - sqrBig.y;
	    zBig->y = -(realimagBig + realimagBig - qBig->y);
	    return BigBailoutTest(zBig, sqrBig);

	case 1:						// Burning Ship
	    return (BigRunFunctions(BURNINGSHIP, zBig, qBig, SpecialFlag, iteration));

	case 2:						// Burning Ship of Higher Degree
	    return (BigRunFunctions(BURNINGSHIPPOWER, zBig, qBig, SpecialFlag, iteration));

	case 3:						// Perpendicular Burning Ship

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
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    realimagBig = zBig->x * zBig->y.BigAbs();
	    zBig->x = qBig->x + sqrBig.x - sqrBig.y;
	    zBig->y = realimagBig + realimagBig + qBig->y;
	    return BigBailoutTest(zBig, sqrBig);

	case 4:						// Buffalo (works according to Kalles Fraktaller)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    if (*degree == 2)
		{
		zBig->y = zBig->x.BigAbs() * zBig->y.BigAbs() * -2.0 - qBig->y;
		tBig = sqrBig.x - sqrBig.y;
		zBig->x = qBig->x + tBig.BigAbs();
		}
	    else if (*degree == 3)
		{
		tBig = (sqrBig.x * 3.0 - sqrBig.y) * zBig->y;
		zBig->y = tBig.BigAbs() - qBig->y;
		tBig = (sqrBig.x - sqrBig.y * 3.0) * zBig->x;
		zBig->x = qBig->x + tBig.BigAbs();
		}
	    else	// degree > 3
		{
		*zBig = zBig->CPolynomial(*degree);
		zBig->y = zBig->y.BigAbs() - qBig->y;
		zBig->x = zBig->x.BigAbs() + qBig->x;
		}
	    return BigBailoutTest(zBig, sqrBig);

	case 5:						// Perpendicular Buffalo - (according to Kalles Fraktaller)
	    tBig = zBig->x.BigAbs();
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = zBig->y * tBig * -2.0 + qBig->y;
	    tBig = sqrBig.x - sqrBig.y;
	    zBig->x = qBig->x + tBig.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

	case 6:						// Mandelbar (Tricorn)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    if (*degree == 2)
		{
		realimagBig = zBig->x * zBig->y;
		zBig->x = qBig->x + sqrBig.x - sqrBig.y;
		zBig->y = qBig->y - realimagBig - realimagBig;
		}
	    else
		{
		*zBig = zBig->CPolynomial(*degree);
		zBig->y = (param[3] == 1.0 ? zBig->y : -zBig->y) + qBig->y;
		zBig->x = (param[3] == 1.0 ? -zBig->x : zBig->x) + qBig->x;
		}
	    return BigBailoutTest(zBig, sqrBig);

	case 7:						// Celtic
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    if (*degree == 2)
		{
		realimagBig = zBig->x * zBig->y;
		zBig->y = -realimagBig - realimagBig - qBig->y;
		tBig = sqrBig.x - sqrBig.y;
		zBig->x = -qBig->x - tBig.BigAbs();
		}
	    else	// degree > 2
		{
		*zBig = zBig->CPolynomial(*degree);
		zBig->y += qBig->y;
		zBig->x = -qBig->x - zBig->x.BigAbs();
		}
	    return BigBailoutTest(zBig, sqrBig);

	case 8:						// Mandelbar Celtic
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = zBig->x * zBig->y * -2.0 + qBig->y;
	    tBig = sqrBig.x - sqrBig.y;
	    zBig->x = qBig->x + tBig.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

	case 9:						// Perpendicular Celtic
	    tBig = zBig->x.BigAbs();
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = zBig->y * tBig * -2.0 + qBig->y;
	    tBig = sqrBig.x - sqrBig.y;
	    zBig->x = qBig->x + tBig.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

	case 10:					// Cubic Flying Squirrel (Buffalo Imaginary)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x * 3.0 - sqrBig.y) * zBig->y;
	    zBig->y = tBig.BigAbs() - qBig->y;
	    zBig->x = qBig->x + (sqrBig.x - sqrBig.y * 3.0) * zBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 11:					// Heart Mandelbrot
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = zBig->x.BigAbs() * zBig->y * 2.0 - qBig->y;
	    zBig->x = sqrBig.x - sqrBig.y + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 12:					// Celtic Heart
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = zBig->x.BigAbs() * zBig->y * 2.0 - qBig->y;
	    tBig = sqrBig.x - sqrBig.y;
	    zBig->x = qBig->x + tBig.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

	case 13:					// Partial Cubic Burning Ship Real
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = (sqrBig.x * 3.0 - sqrBig.y) * zBig->y + qBig->y;
	    zBig->x = qBig->x + (sqrBig.x - sqrBig.y * 3.0) * zBig->x.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

	case 14:					// Partial Cubic Burning Ship Imaginary
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = (sqrBig.x * 3.0 - sqrBig.y) * zBig->y.BigAbs() + qBig->y;
	    zBig->x = qBig->x + (sqrBig.x - sqrBig.y * 3.0) * zBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 15:					// Partial Cubic Buffalo Real (Celtic)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = (sqrBig.x * 3.0 - sqrBig.y) * zBig->y + qBig->y;
	    tBig = (sqrBig.x - sqrBig.y * 3.0) * zBig->x;
	    zBig->x = qBig->x + tBig.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

	case 16:					// Cubic Quasi Burning Ship (Buffalo Burning Ship Hybrid)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x * 3.0 - sqrBig.y) * zBig->y;
	    zBig->y = -tBig.BigAbs() + qBig->y;
	    zBig->x = qBig->x + (sqrBig.x - sqrBig.y * 3.0) * zBig->x.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

	case 17:					// Cubic Quasi Perpendicular
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = sqrBig.x * 3.0 - sqrBig.y;
	    zBig->y = -tBig.BigAbs() * zBig->y + qBig->y;
	    zBig->x = qBig->x + (sqrBig.x - sqrBig.y * 3.0) * zBig->x.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

	case 18:					// Cubic Quasi Heart
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = sqrBig.x * 3.0 - sqrBig.y;
	    zBig->y = tBig.BigAbs() * zBig->y + qBig->y;
	    zBig->x = qBig->x + (sqrBig.x - sqrBig.y * 3.0) * zBig->x.BigAbs();
	    return BigBailoutTest(zBig, sqrBig);

    /****************************************************************
	4th Order Fractals:
    ****************************************************************/

    /****************************************************************
	Non ABS Variations (2)
    ****************************************************************/

	case 19:					// Mandelbrot 4th Order
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    realimagBig = zBig->x * zBig->y;
	    zBig->y = realimagBig * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 20:					// Mandelbar 4th Order
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    realimagBig = zBig->x * zBig->y;
	    zBig->y = -realimagBig * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

    /****************************************************************
	***Straight ABS Variations (16)
    ****************************************************************/

	case 21:					// Burning Ship 4th Order
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    realimagBig = zBig->x * zBig->y;
	    zBig->y = realimagBig.BigAbs() * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 22:					// Burning Ship 4th Partial Imag
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = zBig->x * zBig->y.BigAbs() * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 23:					// Burning Ship 4th Partial Real
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = zBig->x.BigAbs() * zBig->y * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 24:					// Burning Ship 4th Partial Real Mbar
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = -zBig->x.BigAbs() * zBig->y * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 25:					// Celtic Burning Ship 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    realimagBig = zBig->x * zBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->y = realimagBig.BigAbs() * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 26:					// Celtic Burning Ship 4th Partial Imag
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->y = zBig->x * zBig->y.BigAbs() * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 27:					// Celtic Burning Ship 4th Partial Real
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->y = zBig->x.BigAbs() * zBig->y * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 28:					// Celtic Burning Ship 4th Partial Real Mbar
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->y = -zBig->x.BigAbs() * zBig->y * 4.0 * (sqrBig.x - sqrBig.y) + qBig->y;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 29:					// Buffalo 4th Order
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = zBig->x * zBig->y * (sqrBig.x - sqrBig.y);
	    zBig->y = tBig.BigAbs() * 4.0 + qBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 30:					// Buffalo 4th Partial Imag
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = zBig->x * zBig->y * (sqrBig.x - sqrBig.y);
	    zBig->y = tBig.BigAbs() * 4.0 + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 31:					// Celtic (Buffalo 4th Partial Real)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = zBig->x * zBig->y * (sqrBig.x - sqrBig.y) * 4.0 + qBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 32:					// Celtic 4th Mbar
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    zBig->y = -zBig->x * zBig->y * (sqrBig.x - sqrBig.y) * 4.0 + qBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

    /**************************************************************************
	Quasi ABS Variations (10)
    ***************************************************************************/

	case 33:					// False Quasi Perpendicular 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = -zBig->x * zBig->y * tBig.BigAbs() * 4.0 + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 34:					// False Quasi Heart 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = zBig->x * zBig->y * tBig.BigAbs() * 4.0 + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 35:					// Celtic False Quasi Perpendicular 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = -zBig->x * zBig->y * tBig.BigAbs() * 4.0 + qBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 36:					// Celtic False Quasi Heart 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = zBig->x * zBig->y * tBig.BigAbs() * 4.0 + qBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 37:					// Imag Quasi Perpendicular / Heart 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = zBig->x * zBig->y.BigAbs() * tBig.BigAbs() * 4.0 + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 38:					// Real Quasi Perpendicular 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = -zBig->x.BigAbs() * zBig->y * tBig.BigAbs() * 4.0 + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 39:					// Real Quasi Heart 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = zBig->x.BigAbs() * zBig->y * tBig.BigAbs() * 4.0 + qBig->y;
	    zBig->x = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0 + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 40:					// Celtic Imag Quasi Perpendicular / Heart 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = zBig->x * zBig->y.BigAbs() * tBig.BigAbs() * 4.0 + qBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 41:					// Celtic Real Quasi Perpendicular 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = -zBig->x.BigAbs() * zBig->y * tBig.BigAbs() * 4.0 + qBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 42:					// Celtic Real Quasi Heart 4th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    tBig = (sqrBig.x - sqrBig.y);
	    zBig->y = zBig->x.BigAbs() * zBig->y * tBig.BigAbs() * 4.0 + qBig->y;
	    tBig = sqrBig.x * sqrBig.x + sqrBig.y * sqrBig.y - sqrBig.x * sqrBig.y * 6.0;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

    /****************************************************************
	5th Order Fractals:
    ****************************************************************/

	case 43:					// Mandelbrot 5th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    zBig->y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y + qBig->y;
	    zBig->x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 44:					// Mandelbar 5th (Vertical)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    zBig->y = -(sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y + qBig->y;
	    zBig->x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 45:					// Mandelbar 5th (horizontal)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    zBig->y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y + qBig->y;
	    zBig->x = -(sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 46:					// Burning Ship 5th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    zBig->y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y.BigAbs() + qBig->y;
	    zBig->x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 47:					// Buffalo 5th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    tBig = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y;
	    zBig->y = tBig.BigAbs() + qBig->y;
	    tBig = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 48:					// Burning Ship 5th Partial
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    zBig->y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y + qBig->y;
	    zBig->x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 49:					// Burning Ship 5th Partial Mbar
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    zBig->y = -(sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y + qBig->y;
	    zBig->x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 50:					// Celtic 5th (Buffalo 5th Partial)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    zBig->y = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y + qBig->y;
	    tBig = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 51:					// Celtic 5th Mbar
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    zBig->y = -(sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y + qBig->y;
	    tBig = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x;
	    zBig->x = tBig.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 52:					// Quazi Burning Ship 5th (BS/Buffalo Hybrid)
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    tBig = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y) * zBig->y;
	    zBig->y = -tBig.BigAbs() + qBig->y;
	    zBig->x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 53:					// Quazi Perpendicular 5th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    tBig = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y);
	    zBig->y = -tBig.BigAbs() * zBig->y + qBig->y;
	    zBig->x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 54:					// Quazi Heart 5th
	    sqrBig.x = zBig->x.BigSqr();
	    sqrBig.y = zBig->y.BigSqr();
	    sqrsqrBig.x = sqrBig.x.BigSqr();
	    sqrsqrBig.y = sqrBig.y.BigSqr();
	    RealImagSqrBig = sqrBig.x * sqrBig.y;
	    tBig = (sqrsqrBig.x * 5.0 - RealImagSqrBig * 10.0 + sqrsqrBig.y);
	    zBig->y = tBig.BigAbs() * zBig->y + qBig->y;
	    zBig->x = (sqrsqrBig.x - RealImagSqrBig * 10.0 + sqrsqrBig.y * 5.0) * zBig->x.BigAbs() + qBig->x;
	    return BigBailoutTest(zBig, sqrBig);

	case 55:					// SimonBrot
/**************************************************************************
	Run SimonBrot type fractals
	z^n * |z|^2 + c	    (normal)

	It would also be nice to see Simonbrot 3rd, 5th, and 7th, but the formulas for these fractals are
	z1.5 * |z|2 + pixel, z2.5 * |z|2 + pixel, and z3.5 * |z|2 + pixel. Due to the fact that the powers of
	some of the terms are fractions, this could be difficult to implement. In a roundabout way, there is
	a power zero Simonbrot on Kalles Fraktaler, because the power zero Simonbrot is actually the Burning Ship.
**************************************************************************/
	    {
	    BigComplex	zabsBig, tempzBig, sqrtzBig;

	    zabsBig.x = zBig->x.BigAbs();
	    zabsBig.y = zBig->y.BigAbs();
	    tempzBig.y = zBig->y * zabsBig.x + zBig->x * zabsBig.y;
	    tempzBig.x = zBig->x * zabsBig.x - zBig->y * zabsBig.y;
	    if (*degree % 2 == 1)
		sqrtzBig = zBig->CSqrt();				// use square root power if degree is odd
	    else
		sqrtzBig = 1.0;
	    *zBig = tempzBig.CPolynomial(*degree / 2) * sqrtzBig + *qBig;
	    return BigFractintBailoutTest(zBig);
	    }

	case 56:					// SimonBrot2
/**************************************************************************
	Run SimonBrot2 type fractals
	z^n * |z^2| + c	    (SimonBrot2)
**************************************************************************/
	    {
	    BigComplex	zabsBig, tempzBig, sqrtzBig;

	    tempzBig = *zBig * *zBig;
	    zabsBig.x = tempzBig.x.BigAbs();
	    zabsBig.y = -tempzBig.y.BigAbs();
	    tempzBig = zabsBig;
	    if (*degree % 2 == 1)
		sqrtzBig = zBig->CSqrt();				// use square root power if degree is odd
	    else
		sqrtzBig = 1.0;
	    *zBig = zBig->CPolynomial(*degree / 2) * sqrtzBig * tempzBig + *qBig;
	    return BigFractintBailoutTest(zBig);
	    }

	case 57:					// Kung Fu Panda
/**************************************************************************
	Kung Fu Panda type fractals
	z = abs(z*z)
	z = z * z + p
**************************************************************************/
	    {
	    BigComplex	t1, t2;

	    t1 = *zBig * *zBig;
	    t2.x = t1.x.BigAbs();
	    t2.y = t1.y.BigAbs();
	    *zBig = t2 * t2 - *qBig;

	    return BigFractintBailoutTest(zBig);
	    }

	case 58:					// HPDZ Buffalo
/**************************************************************************
	HPDZ Buffalo type fractals
	z := (((x^2 - y^2) - |x|) + i (|2xy| - |y|)) + c
	or
	w := |x| + i |y|
	z := w^2 - w + c
**************************************************************************/
	    {
	    BigComplex w;

	    w.x = zBig->x.BigAbs();
	    w.y = zBig->y.BigAbs();
	    *zBig = w * w - w;
	    zBig->x += qBig->x;
	    zBig->y -= qBig->y;
//	    return (zBig->x.BigSqr() + zBig->y.BigSqr() > BigBailout);
//	    ShowBignum(zBig->x, " CPixel Inside Buffalo Z.x");
//	    ShowBignum(zBig->y, "CPixel Inside Buffalo Z.y");
//	    ShowBignum(BigBailout, "CPixel bailout");
	    return BigFractintBailoutTest(zBig);
	    }

#ifdef SZEGEDBUTTERFLY
	case 59:					// SzegediButterfly 1
/**************************************************************************
	Szegedi Butterfly
	double temp = complex[0].getIm();
	double temp2 = complex[0].getRe();
	complex[0] = new Complex(temp * temp - Math.sqrt(complex[0].getAbsRe()), temp2 * temp2 - Math.sqrt(complex[0].getAbsIm())).plus_mutable(complex[1]);
	complex[0] = new Complex(temp2 * temp2 - Math.sqrt(complex[0].getAbsIm()), temp * temp - Math.sqrt(complex[0].getAbsRe())).plus_mutable(complex[1]);
**************************************************************************/
	    {
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    z->x = sqr.y - sqrt(fabs(z->x));
	    z->y = sqr.x - sqrt(fabs(z->y));
	    *z += *q;

	    return (z->CSumSqr() >= rqlim);
	    }

	case 60:					// SzegediButterfly 2
	    {
	    double  temp;

	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    temp = sqr.x - sqrt(fabs(z->y));
	    z->y = sqr.y - sqrt(fabs(z->x));
	    z->x = temp;
	    *z += *q;

	    return (z->CSumSqr() >= rqlim);
	    }

#endif // SZEGEDBUTTERFLY
	}
    return 0;
    }
