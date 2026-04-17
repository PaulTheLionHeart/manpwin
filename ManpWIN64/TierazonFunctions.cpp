////////////////////////////////////////////////////////////////
// Tierazon.cpp a module for the per pixel calculations of Tierazon fractals. 
//
// By: stephen c. ferguson
// http://home1.gte.net/itriazon/
// email: itriazon@gte.net
//
/////////////////////////////////////////////////////
// Updated in Microsoft Visual 'C++' by Paul de Leeuw.
// These are listed in numerical order of the functions.
// Note that CSqr(z) is used in place of z*z and CCube(z) in place of z*z*z for speed
/////////////////////////////////////////////////////

#include <math.h>
#include "Complex.h"
#include "fractype.h"
#include "pixel.h"
#include "TierazonTemplate.h"
#include "BailoutTemplate.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::InitTierazonFunctions(int subtype, Complex *z, Complex *q)
    {
    TierazonInit_Generic<Complex>(subtype, z, q, &z2, sqr, real_imag, juliaflag, param, degree, &cb, &caa3, &c1, &c2, &b, &z4);
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::RunTierazonFunctions(int subtype, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration)
    {
    return TierazonIter<Complex, double>(subtype, z, q, &z2, &z4, sqr, real_imag, SpecialFlag, iteration, caa3, cb, c1, c2, b, rqlim, degree, BailoutTestType);
    }


