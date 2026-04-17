////////////////////////////////////////////////////////////////
// BigTZ.cpp a module for the per pixel calculations of Tierazon fractals using Bignum functions. 
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
#include  "manp.h"
#include "Complex.h"
#include "big.h"
#include "fractype.h"
#include "BigDouble.h"
#include "BigComplex.h"
#include "resource.h"
#include "pixel.h"
#include "TierazonTemplate.h"
#include "BailoutTemplate.h"

extern	void	ShowBignum(BigDouble x, char *Location);

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::BigInitTierazonFunctions(int subtype, BigComplex *zBig, BigComplex *qBig)
    {
    TierazonInit_Generic<BigComplex>(subtype, zBig, qBig, &z2Big, sqrBig, realimagBig, juliaflag, param, degree, &cbBig, &caa3Big, &c1Big, &c2Big, &bBig, &z4Big);
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::BigRunTierazonFunctions(int subtype, BigComplex *zBig, BigComplex *qBig, BYTE *SpecialFlag, long *iteration)
    {
    return TierazonIter<BigComplex, BigDouble>(subtype, zBig, qBig, &z2Big, &z4Big, sqrBig, realimagBig, SpecialFlag, iteration, caa3Big, cbBig, c1Big, c2Big, bBig, rqlim, degree, BailoutTestType);
    }


