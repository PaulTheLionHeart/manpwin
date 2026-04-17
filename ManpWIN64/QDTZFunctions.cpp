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
#include "QDComplex.h"
#include "fractype.h"
#include "pixel.h"
#include "TierazonTemplate.h"
#include "BailoutTemplate.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::QDInitTierazonFunctions(int subtype, QDComplex *z, QDComplex *q)
    {
    TierazonInit_Generic<QDComplex>(subtype, z, q, &z2QD, sqrQD, realimagQD, juliaflag, param, degree, &cbQD, &caa3QD, &c1QD, &c2QD, &bQD, &z4QD);
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::QDRunTierazonFunctions(int subtype, QDComplex *z, QDComplex *q, QDComplex *z2, BYTE *SpecialFlag, long *iteration)
    {
    QDComplex   a, b, t, z1, z3, zd, zt;
    static	double	    d;

    return TierazonIter<QDComplex, qd_real>(subtype, z, q, z2, &z4QD, sqrQD, realimagQD, SpecialFlag, iteration, caa3QD, cbQD, c1QD, c2QD, bQD, rqlim, degree, BailoutTestType);
    return 0;
    }


