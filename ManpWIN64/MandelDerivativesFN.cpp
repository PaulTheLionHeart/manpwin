/*
    MandelDerivativesFN.cpp a module for the per pixel calculations of Mandelbrot Derivatives. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include <math.h>
#include "Complex.h"
#include "fractype.h"
#include "resource.h"
#include "pixel.h"
#include "MandelDerivTemplate.h"
#include "BailoutTemplate.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::InitManDerFunctions(int subtype, Complex *z, Complex *q)
    {
    MandelDerivInit<Complex, double>(subtype, juliaflag, z, q, sqr, real_imag, degree, param);
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::RunManDerFunctions(int subtype, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration)
    {
    return MandelDerivDispatch(
		this,                   // pass object
		subtype,
		z,
		q,
		degree,
		sqr,
		real_imag,
		BailoutTestType,
		rqlim);
    return 0;
    }
