/*
    DDMandelDerivatives.cpp  -a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include <math.h>
#include "Complex.h"
#include "DDComplex.h"
#include "fractype.h"
#include "pixel.h"
#include "MandelDerivTemplate.h"
#include "BailoutTemplate.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::DDInitManDerFunctions(int subtype, DDComplex *z, DDComplex *q)
    {
    MandelDerivInit<DDComplex, dd_real>(
	subtype, juliaflag,
	z, q,
	sqrDD, realimagDD,
	degree, param
	);

    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::DDRunManDerFunctions(int subtype, DDComplex *z, DDComplex *q, BYTE *SpecialFlag, long *iteration)
    {
    DDComplex	sqr, sqrsqr;
    dd_real	real_imag, RealImagSqr, rqlimDD = rqlim;

     return MandelDerivDispatch(
		this,                   // pass object
		subtype,
		z,
		q,
		degree,
		sqr,
		real_imag,
		BailoutTestType,
		rqlimDD);
    return 0;
    }
