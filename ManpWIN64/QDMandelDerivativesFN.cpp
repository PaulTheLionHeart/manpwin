/*
    QDMandelDerivatives.CPP a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <math.h>
#include "Complex.h"
#include "QDComplex.h"
#include "fractype.h"
#include "Pixel.h"
#include "MandelDerivTemplate.h"
#include "BailoutTemplate.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::QDInitManDerFunctions(int subtype, QDComplex *z, QDComplex *q)
    {
    MandelDerivInit<QDComplex, qd_real>(
	subtype, juliaflag,
	z, q,
	sqrQD, realimagQD,
	degree, param
	);

    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::QDRunManDerFunctions(int subtype, QDComplex *z, QDComplex *q)
    {

    QDComplex	sqr, sqrsqr;
    qd_real	real_imag, RealImagSqr, rqlimQD = rqlim;

    return MandelDerivDispatch(
		this,                   // pass object
		subtype,
		z,
		q,
		degree,
		sqr,
		real_imag,
		BailoutTestType,
		rqlimQD);

    return 0;
    }
