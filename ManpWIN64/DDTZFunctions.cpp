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
#include "DDComplex.h"
#include "fractype.h"
#include "pixel.h"
#include "TierazonTemplate.h"
#include "BailoutTemplate.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::DDInitTierazonFunctions(int subtype, DDComplex *z, DDComplex *q)
    {
    TierazonInit_Generic<DDComplex>(subtype, z, q, &z2DD, sqrDD, realimagDD, juliaflag, param, degree, &cbDD, &caa3DD, &c1DD, &c2DD, &bDD, &z4DD);
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::DDRunTierazonFunctions(int subtype, DDComplex *z, DDComplex *q, DDComplex *z2, BYTE *SpecialFlag, long *iteration)
    {
    DDComplex   a, b, t, z1, z3, zd, zt;
    static	double	    d;

    return TierazonIter<DDComplex, dd_real>(subtype, z, q, z2, &z4DD, sqrDD, realimagDD, SpecialFlag, iteration, caa3DD, cbDD, c1DD, c2DD, bDD, rqlim, degree, BailoutTestType);
    return 0;
    }


