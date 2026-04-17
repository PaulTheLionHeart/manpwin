/*
    BigMandelDerivatives.CPP a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <math.h>
#include "manp.h"
#include "Complex.h"
#include "big.h"
#include "fractype.h"
#include "BigDouble.h"
#include "BigComplex.h"
#include "resource.h"
#include "pixel.h"
#include "MandelDerivTemplate.h"
#include "BailoutTemplate.h"

extern	void	ShowBignum(BigDouble x, char *Location);

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::BigInitManDerFunctions(int subtype, BigComplex *zBig, BigComplex *qBig)
    {
    MandelDerivInit<BigComplex, BigDouble>(
	subtype, juliaflag,
	zBig, qBig,
	sqrBig, realimagBig,
	degree, param
	);

    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::BigRunManDerFunctions(int subtype, BigComplex *zBig, BigComplex *qBig, BYTE *SpecialFlag, long *iteration)
    {
    BigDouble	rqlimBig = rqlim;
    return MandelDerivDispatch(
		this,                   // pass object
		subtype,
		zBig,
		qBig,
		degree,
		sqrBig,
		realimagBig,
		BailoutTestType,
		rqlimBig);

    return 0;
    }
