/****************************************************
    PERTSETUP.CPP a module to explore Perturbation
    Thanks to Shirom Makkad fractaltodesktop@gmail.com
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include <atomic>
#include <vector>
#include "PertEngine.h"
#include "Timer.h"

#define min(a,b)    (((a) < (b)) ? (a) : (b))

std::vector<ExpComplex>	ExpXSubN {0};		// references
std::vector<Complex>	XSubN {0};
int	ArithType = DOUBLE;
int	MaxRefIteration = 0;
BLAS    Bla;					// Bilinear Approximation
int	SlopeDegree = 2;

extern	int	subtype;
extern	double	param[];
extern	WORD	degree;				// power
extern	bool	EnableApproximation;		// use BLA on perturbation
extern	int	xdots, ydots;
extern	char	PertStatus[];
extern	int	precision;

extern	std::atomic<bool> gStopRequested;	// force early exit
extern	std::vector<CPerturbation> PertCalculator;

///////////////////////////////////////////////
//  A small struct just for reference building.
///////////////////////////////////////////////

struct RefScratch
    {
    mpfr_t TempReal, TempImag, SqrReal, SqrImag, RealImag;

    void Init(int prec)
	{
	mpfr_init2(TempReal, prec);
	mpfr_init2(TempImag, prec);
	mpfr_init2(SqrReal, prec);
	mpfr_init2(SqrImag, prec);
	mpfr_init2(RealImag, prec);
	}

    void Clear()
	{
	mpfr_clear(TempReal);
	mpfr_clear(TempImag);
	mpfr_clear(SqrReal);
	mpfr_clear(SqrImag);
	mpfr_clear(RealImag);
	}
    };

//////////////////////////////////////////////////////////////////////
// Determine the type of arithmetic to use
//////////////////////////////////////////////////////////////////////

void	PertSetupArithType(int &ArithType, int subtype, long MaxIteration, int precision, BYTE BigNumFlag)
    {
    // okay, we are going to do some complicated arithmetic calculations for the various ways of calculating fractals
    if (!BigNumFlag || (BigNumFlag && precision <= 300))
	{
	if (subtype == 0 || subtype == 1)
	    ArithType = DOUBLE;
	else
	    ArithType = DBL_UNSUPPORTED;		// use Use BLA perturbation code but don't process via BLA table
	}
    else
	{
	if ((subtype == 0 || subtype == 1))		// only for Mandelbrot and power so far
	    ArithType = FLOATEXP;
	else
	    ArithType = EXP_UNSUPPORTED;		// use Use BLA perturbation code but don't process via BLA table
	}
 
    if (ArithType == EXP_UNSUPPORTED || ArithType == FLOATEXP) 
	{
	// get memory for Z array
	ExpXSubN.resize(MaxIteration + 1);
	}
    else
	{
	// get memory for Z array
	XSubN.resize(MaxIteration + 1);
	}
    if (EnableApproximation)
	{
	if (ArithType == EXP_UNSUPPORTED || ArithType == DBL_UNSUPPORTED)
	    EnableApproximation = false;		// we don't support these fractals with BLA
	}
#ifdef _DEBUG
    char	buf[256];
    SAFE_SPRINTF(buf, "PertSetupArithType: precision = %d, ArithType = %d\n", precision, ArithType);
    OutputDebugStringA(buf);
#endif
    }

//////////////////////////////////////////////////////////////////////
// Reference Zoom Point Functions
// Some code is written in raw MPFR code to optimise speed
//////////////////////////////////////////////////////////////////////

void	RefFunctions(BigComplex *centre, BigComplex *Z, int &SlopeDegree, int subtype, double param[], RefScratch& scratch)
    {
//    mpfr_t	TempReal, TempImag, SqrReal, SqrImag, RealImag;
    BigDouble	zisqr, zrsqr, realimag, temp, RealImagSqr;
    BigComplex	sqrsqr, zabsBig, tempzBig, sqrtzBig, SqrBig;
    BigComplex	aBig;

    Complex	rsrA = { param[1], param[2] };			// TheRedshiftRider value of a
    bool	rsrSign = (param[4] == 1.0);			// TheRedshiftRider sign true if positive
    int		power = (int)param[3];

    switch (subtype)
	{
	case 0:							// optimise for Mandelbrot by taking out as many steps as possible
	    {
	    BigDouble ZTimes2 = Z->x.BigX2();
	    SlopeDegree = 2;
	    //	    Z = Z.CSqr() + centre;
	    mpfr_sqr(scratch.SqrReal, Z->x.x, MPFR_RNDN);
	    mpfr_sqr(scratch.SqrImag, Z->y.x, MPFR_RNDN);
	    mpfr_sub(scratch.TempReal, scratch.SqrReal, scratch.SqrImag, MPFR_RNDN);
	    mpfr_add(Z->x.x, scratch.TempReal, centre->x.x, MPFR_RNDN);

	    mpfr_mul(scratch.RealImag, ZTimes2.x, Z->y.x, MPFR_RNDN);
	    mpfr_add(Z->y.x, scratch.RealImag, centre->y.x, MPFR_RNDN);
	    }
	    break;
	    
	case 1:
	    SlopeDegree = power;
	    if (power == 3)
		*Z = Z->CCube() + *centre;			// optimise for Cubic by taking out as many multiplies as possible
	    else
		{
		BigComplex BigComplexTemp = *Z;
		for (int k = 0; k < power - 1; k++)
		    BigComplexTemp *= *Z;
		*Z = BigComplexTemp + *centre;
		}
	    break;
	case 2:							// Burning Ship
	    {
	    BigDouble ZTimes2 = Z->x.BigX2();
	    SlopeDegree = 2;
	    mpfr_sqr(scratch.SqrReal, Z->x.x, MPFR_RNDN);
	    mpfr_sqr(scratch.SqrImag, Z->y.x, MPFR_RNDN);
	    mpfr_sub(scratch.TempReal, scratch.SqrReal, scratch.SqrImag, MPFR_RNDN);
	    mpfr_add(Z->x.x, scratch.TempReal, centre->x.x, MPFR_RNDN);
	    mpfr_mul(scratch.TempImag, ZTimes2.x, Z->y.x, MPFR_RNDN);
	    mpfr_abs(scratch.RealImag, scratch.TempImag, MPFR_RNDN);
	    mpfr_add(Z->y.x, scratch.RealImag, centre->y.x, MPFR_RNDN);
/*
	    zisqr = Z.y * Z.y;
	    zrsqr = Z.x * Z.x;
	    Z.y = (Z.x * Z.y).BigAbs() * 2.0 + centre.y;
	    Z.x = (zrsqr - zisqr) + centre.x;
*/  
	    }
	    break;
	   
	case 3:							// Cubic Burning Ship
	    SlopeDegree = 3;
	    Z->x = Z->x.BigAbs();
	    Z->y = Z->y.BigAbs();
	    *Z = Z->CPolynomial(power) + *centre;
	    break;
	case 4:							// 4th Power Burning Ship
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    realimag = Z->x * Z->y;
	    Z->y = realimag.BigAbs() * (zrsqr - zisqr) * 4.0 - centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 5:							// 5th Power Burning Ship
	    SlopeDegree = 5;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    sqrsqr.x = zrsqr * zrsqr;
	    sqrsqr.y = zisqr * zisqr;
	    RealImagSqr = zrsqr * zisqr;
	    Z->y = Z->y.BigAbs() * (sqrsqr.x * 5.0 - RealImagSqr * 10.0 + sqrsqr.y) - centre->y;
	    Z->x = Z->x.BigAbs() * (sqrsqr.x - RealImagSqr * 10.0 + sqrsqr.y * 5.0) + centre->x;
	    break;
	case 6:							// Celtic
	    SlopeDegree = 2;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    realimag = Z->x * Z->y;
	    Z->y = realimag + realimag + centre->y;
	    temp = zrsqr - zisqr;
	    Z->x = centre->x + temp.BigAbs();
	    break;
	case 7:							// Cubic Celtic
	    SlopeDegree = 3;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = (zrsqr * 3.0 - zisqr) * Z->y + centre->y;
	    temp = (zrsqr - zisqr * 3.0) * Z->x;
	    Z->x = centre->x + temp.BigAbs();
	    break;
	case 8:							// 4th Celtic Buffalo
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = Z->x * Z->y * (zrsqr - zisqr) * 4.0 + centre->y;
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 9:							// 5th Celtic
	    SlopeDegree = 5;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    sqrsqr.x = zrsqr.BigSqr();
	    sqrsqr.y = zisqr.BigSqr();
	    RealImagSqr = zrsqr * zisqr;
	    Z->y = (sqrsqr.x * 5.0 - RealImagSqr * 10.0 + sqrsqr.y) * Z->y + centre->y;
	    temp = (sqrsqr.x - RealImagSqr * 10.0 + sqrsqr.y * 5.0) * Z->x;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 10:						// Mandelbar (Tricorn)
	    SlopeDegree = 2;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    realimag = Z->x * Z->y;
	    Z->x = centre->x + zrsqr - zisqr;
	    Z->y = centre->y - realimag - realimag;
	    break;
	case 11:						// Mandelbar (power)
	    SlopeDegree = power;
	    *Z = Z->CPolynomial(power);
	    Z->y = -Z->y + centre->y;
	    Z->x = Z->x + centre->x;
	    break;
	case 12:						// Buffalo
	    SlopeDegree = 2;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = Z->x.BigAbs() * Z->y.BigAbs() * -2.0 + centre->y;
	    temp = zrsqr - zisqr;
	    Z->x = centre->x + temp.BigAbs();
	    break;
	case 13:						// Cubic Buffalo
	case 14:						// 4th power Buffalo
	case 15:						// 5th power Buffalo
	    SlopeDegree = power;
	    //	    zrsqr = Z->x.BigSqr();
//	    zisqr = Z->y.BigSqr();
//	    temp = (zrsqr * 3.0 - zisqr) * Z->y;
//	    Z->y = temp.BigAbs() + centre->y;
//	    temp = (zisqr - zisqr * 3.0) * Z->x;
//	    Z->x = centre->x + temp.BigAbs();
	    *Z = Z->CPolynomial(power);
	    Z->y = Z->y.BigAbs() + centre->y;
	    Z->x = Z->x.BigAbs() + centre->x;
	    break;
	case 16:						// Mandelbar Celtic
	    SlopeDegree = 2;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = Z->x * Z->y * -2.0 + centre->y;
	    temp = zrsqr - zisqr;
	    Z->x = centre->x + temp.BigAbs();
	    break;
	case 17:						// Perpendicular Mandelbrot
	    SlopeDegree = 2;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = Z->x;
	    realimag = temp.BigAbs() * Z->y;
	    Z->x = centre->x + zrsqr - zisqr;
	    Z->y = -(realimag + realimag - centre->y);
	    break;
	case 18:						// Perpendicular Burning Ship
	    SlopeDegree = 2;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = -Z->x * Z->y.BigAbs() * 2 + centre->y;
	    Z->x = zrsqr - zisqr + centre->x;
	    break;
	case 19:						// Perpendicular Celtic
	    SlopeDegree = 2;
	    temp = Z->x.BigAbs();
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = Z->y * temp * -2.0 + centre->y;
	    temp = zrsqr - zisqr;
	    Z->x = centre->x + temp.BigAbs();
	    break;
	case 20:						// Perpendicular Buffalo
	    SlopeDegree = 2;
	    temp = Z->y.BigAbs();
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = Z->x * temp * -2.0 + centre->y;
	    temp = zrsqr - zisqr;
	    Z->x = centre->x + temp.BigAbs();
	    break;
	case 21:						// Cubic Quasi Burning Ship
	    SlopeDegree = 3;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr * 3.0 - zisqr) * Z->y;
	    Z->y = -temp.BigAbs() - centre->y;
	    Z->x = centre->x + (zrsqr - zisqr * 3.0) * Z->x.BigAbs();
	    break;
	case 22:						// Cubic Partial BS Real
	    SlopeDegree = 3;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = (zrsqr * 3.0 - zisqr) * Z->y + centre->y;
	    Z->x = centre->x + (zrsqr - zisqr * 3.0) * Z->x.BigAbs();
	    break;
	case 23:						// Cubic Partial BS Imag
	    SlopeDegree = 3;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = (zrsqr * 3.0 - zisqr) * Z->y.BigAbs() - centre->y;
	    Z->x = centre->x + (zrsqr - zisqr * 3.0) * Z->x;
	    break;
	case 24:						// Cubic Flying Squirrel
	    SlopeDegree = 3;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr * 3.0 - zisqr) * Z->y;
	    Z->y = temp.BigAbs() + centre->y;
	    Z->x = centre->x + (zrsqr - zisqr * 3.0) * Z->x;
	    break;
	case 25:						// Cubic Quasi Perpendicular
	    SlopeDegree = 3;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = zrsqr * 3.0 - zisqr;
	    Z->y = -temp.BigAbs() * Z->y + centre->y;
	    Z->x = centre->x + (zrsqr - zisqr * 3.0) * Z->x.BigAbs();
	    break;
	case 26:							// 4th Burning Ship Partial Imag
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = Z->x * Z->y.BigAbs() * 4.0 * (zrsqr - zisqr) - centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 27:							// 4th Burning Ship Partial Real
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = Z->x.BigAbs() * Z->y * 4.0 * (zrsqr - zisqr) + centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 28:							// 4th Burning Ship Partial Real Mbar
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = -Z->x.BigAbs() * Z->y * 4.0 * (zrsqr - zisqr) + centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 29:							// 4th Celtic Burning Ship Partial Imag
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->y = Z->x * Z->y.BigAbs() * 4.0 * (zrsqr - zisqr) - centre->y;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 30:							// 4th Celtic Burning Ship Partial Real
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->y = Z->x.BigAbs() * Z->y * 4.0 * (zrsqr - zisqr) + centre->y;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 31:							// 4th Celtic Burning Ship Partial Real Mbar
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->y = -Z->x.BigAbs() * Z->y * 4.0 * (zrsqr - zisqr) + centre->y;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 32:							// 4th Buffalo Partial Imag
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = Z->x * Z->y * (zrsqr - zisqr);
	    Z->y = temp.BigAbs() * 4.0 - centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 33:							// 4th Celtic Mbar
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    Z->y = -Z->x * Z->y * (zrsqr - zisqr) * 4.0 + centre->y;
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 34:							// 4th False Quasi Perpendicular
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = -Z->x * Z->y * temp.BigAbs() * 4.0 + centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 35:							// 4th False Quasi Heart
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = Z->x * Z->y * temp.BigAbs() * 4.0 + centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 36:							// 4th Celtic False Quasi Perpendicular
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = -Z->x * Z->y * temp.BigAbs() * 4.0 + centre->y;
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 37:							// 4th Celtic False Quasi Heart
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = Z->x * Z->y * temp.BigAbs() * 4.0 + centre->y;
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 38:							// 4th Imag Quasi Perpendicular / Heart
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = Z->x * Z->y.BigAbs() * temp.BigAbs() * 4.0 - centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 39:							// 4th Real Quasi Perpendicular
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = -Z->x.BigAbs() * Z->y * temp.BigAbs() * 4.0 + centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 40:							// 4th Real Quasi Heart
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = Z->x.BigAbs() * Z->y * temp.BigAbs() * 4.0 + centre->y;
	    Z->x = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0 + centre->x;
	    break;
	case 41:							// 4th Celtic Imag Quasi Perpendicular / Heart
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = Z->x * Z->y.BigAbs() * temp.BigAbs() * 4.0 - centre->y;
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 42:							// 4th Celtic Real Quasi Perpendicular
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = -Z->x.BigAbs() * Z->y * temp.BigAbs() * 4.0 + centre->y;
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 43:							// 4th Celtic Real Quasi Heart
	    SlopeDegree = 4;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    temp = (zrsqr - zisqr);
	    Z->y = Z->x.BigAbs() * Z->y * temp.BigAbs() * 4.0 + centre->y;
	    temp = zrsqr * zrsqr + zisqr * zisqr - zrsqr * zisqr * 6.0;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 44:							// 5th Burning Ship Partial
	    SlopeDegree = 5;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    sqrsqr.x = zrsqr.BigSqr();
	    sqrsqr.y = zisqr.BigSqr();
	    RealImagSqr = zrsqr * zisqr;
	    Z->y = (sqrsqr.x * 5.0 - RealImagSqr * 10.0 + sqrsqr.y) * Z->y + centre->y;
	    Z->x = (sqrsqr.x - RealImagSqr * 10.0 + sqrsqr.y * 5.0) * Z->x.BigAbs() + centre->x;
	    break;
	case 45:							// 5th Burning Ship Partial Mbar
	    SlopeDegree = 5;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    sqrsqr.x = zrsqr.BigSqr();
	    sqrsqr.y = zisqr.BigSqr();
	    RealImagSqr = zrsqr * zisqr;
	    Z->y = -(sqrsqr.x * 5.0 - RealImagSqr * 10.0 + sqrsqr.y) * Z->y + centre->y;
	    Z->x = (sqrsqr.x - RealImagSqr * 10.0 + sqrsqr.y * 5.0) * Z->x.BigAbs() + centre->x;
	    break;
	case 46:							// 5th Celtic Mbar
	    SlopeDegree = 5;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    sqrsqr.x = zrsqr.BigSqr();
	    sqrsqr.y = zisqr.BigSqr();
	    RealImagSqr = zrsqr * zisqr;
	    Z->y = -(sqrsqr.x * 5.0 - RealImagSqr * 10.0 + sqrsqr.y) * Z->y + centre->y;
	    temp = (sqrsqr.x - RealImagSqr * 10.0 + sqrsqr.y * 5.0) * Z->x;
	    Z->x = temp.BigAbs() + centre->x;
	    break;
	case 47:							// 5th Quasi Burning Ship (BS/Buffalo Hybrid)
	    SlopeDegree = 5;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    sqrsqr.x = zrsqr.BigSqr();
	    sqrsqr.y = zisqr.BigSqr();
	    RealImagSqr = zrsqr * zisqr;
	    temp = (sqrsqr.x * 5.0 - RealImagSqr * 10.0 + sqrsqr.y) * Z->y;
	    Z->y = -temp.BigAbs() - centre->y;
	    Z->x = (sqrsqr.x - RealImagSqr * 10.0 + sqrsqr.y * 5.0) * Z->x.BigAbs() + centre->x;
	    break;
	case 48:							// 5th Quasi Perpendicular
	    SlopeDegree = 5;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    sqrsqr.x = zrsqr.BigSqr();
	    sqrsqr.y = zisqr.BigSqr();
	    RealImagSqr = zrsqr * zisqr;
	    temp = (sqrsqr.x * 5.0 - RealImagSqr * 10.0 + sqrsqr.y);
	    Z->y = -temp.BigAbs() * Z->y + centre->y;
	    Z->x = (sqrsqr.x - RealImagSqr * 10.0 + sqrsqr.y * 5.0) * Z->x.BigAbs() + centre->x;
	    break;
	case 49:							// 5th Quasi Heart
	    SlopeDegree = 5;
	    zrsqr = Z->x.BigSqr();
	    zisqr = Z->y.BigSqr();
	    sqrsqr.x = zrsqr.BigSqr();
	    sqrsqr.y = zisqr.BigSqr();
	    RealImagSqr = zrsqr * zisqr;
	    temp = (sqrsqr.x * 5.0 - RealImagSqr * 10.0 + sqrsqr.y);
	    Z->y = temp.BigAbs() * Z->y + centre->y;
	    Z->x = (sqrsqr.x - RealImagSqr * 10.0 + sqrsqr.y * 5.0) * Z->x.BigAbs() + centre->x;
	    break;
	case 50:							// SimonBrot
	    SlopeDegree = 2;
	    zabsBig.x = Z->x.BigAbs();
	    zabsBig.y = Z->y.BigAbs();
	    tempzBig.y = Z->y * zabsBig.x + Z->x * zabsBig.y;
	    tempzBig.x = Z->x * zabsBig.x - Z->y * zabsBig.y;
	    *Z = tempzBig.CPolynomial(2) + *centre;
	    break;
	case 51:							// Cubic SimonBrot
	    SlopeDegree = 3;
	    zabsBig.x = Z->x.BigAbs();
	    zabsBig.y = Z->y.BigAbs();
	    tempzBig.y = Z->y * zabsBig.x + Z->x * zabsBig.y;
	    tempzBig.x = Z->x * zabsBig.x - Z->y * zabsBig.y;
	    *Z = tempzBig.CPolynomial(3) + *centre;
	    break;
	case 52:							// SimonBrot2 4th
	    SlopeDegree = 4;
	    tempzBig = *Z * *Z;
	    zabsBig.x = tempzBig.x.BigAbs();
	    zabsBig.y = tempzBig.y.BigAbs();
	    *Z = *Z * *Z * zabsBig;
	    Z->y = Z->y + centre->y;
	    Z->x = Z->x + centre->x;
	    break;
	case 53:							// TheRedshiftRider 1: a*z^2+z^3+c
	    SlopeDegree = power;
	    aBig = rsrA;	// convert complex -> big complex
	    *Z = aBig * *Z * *Z + Z->CPolynomial(power) * ((rsrSign) ? 1.0 : -1.0);
	    *Z = *Z + *centre;
	    break;
	case 54:							// Talis
	    SlopeDegree = 3;
	    *Z = (Z->CSqr() / (*Z + 1.0)) + *centre;
	    break;
	case 55:							// Talis Cubic: ((X ^ 3) / (1 + (X ^ 2))) + C
	    SlopeDegree = 3;
	    *Z = (Z->CCube() / (Z->CSqr() + 1.0)) + *centre;
	    break;
	case 56:							// Talis Quartic: 
	    SlopeDegree = 3;
	    SqrBig = Z->CSqr();
	    *Z = (SqrBig.CSqr() / (Z->CCube() + 1.0)) + *centre;
	    break;
	case 57:							// polymnomial
	    {
	    BigComplex	InitialZ = *Z;
	    BigComplex	FinalZ = { 0.0, 0.0 };
		{
		for (int i = MAXPOLY - 1; i >= 0; i--)			// find highest order of polynomial
		    if (param[i] != 0.0)
			{
			SlopeDegree = MAXPOLY - i;
			break;
			}
		for (int m = 0; m < MAXPOLY; m++)
		    {
		    BigComplex BigComplexTemp = InitialZ;
		    if (param[m] != 0.0)
			{
			for (int k = 0; k < MAXPOLY - m - 1; k++)
			    BigComplexTemp *= InitialZ;
			FinalZ += (BigComplexTemp * param[m]);
			}
		    }
		*Z = FinalZ + *centre;
		}
	    }
	    break;

	case 58:							// HPDZ Buffalo
	    SlopeDegree = 2;
	    tempzBig.x = Z->x.BigAbs();
	    tempzBig.y = Z->y.BigAbs();
	    *Z = tempzBig * tempzBig - tempzBig;
	    Z->y += centre->y;
	    Z->x += centre->x;
	    break;

	default:
	    SlopeDegree = 2;
	    *Z = *Z * *Z + *centre;
	    break;
	}
    }

//////////////////////////////////////////////////////////////////////
// Reference Zoom Point
//////////////////////////////////////////////////////////////////////

int	ReferenceZoomPoint(BigComplex& centre, int maxIteration, int user_data(HWND hwnd), char* StatusBarInfo, int *pPertProgress, double bailout, int ArithType, int power, BigDouble BigWidth, int &SlopeDegree, int subtype)
    {
    // Raising this number makes more calculations, but less variation between each calculation (less chance of mis-identifying a glitched point).
    BigComplex	zBig;
    bool	WeHaveMaxRefIteration = false;
    double	CoordinateMagnitudeSquared = 0.0;
    int		lastChecked = -1;
    constexpr	DWORD	REF_UPDATE_INTERVAL_MS = 250;
    DWORD	lastUpdateTick = GetTickCount();
    int		lastPercent = -1;
    double	ZoomRadius = mpfr_get_d(BigWidth.x, MPFR_RNDN);
//    char	buf[256];

    RefScratch scratch;
    scratch.Init(precision * SAFETYMARGIN);	// setup temp variables for reference creation

    SimpleTimer  tRef;
    tRef.start();
   
    if (ArithType == EXP_UNSUPPORTED || ArithType == FLOATEXP)
	{
	// get memory for Z array
	ExpXSubN.resize(maxIteration + 1);
	}
    else
	{
	// get memory for Z array
	XSubN.resize(maxIteration + 1);
	}

    zBig = 0.0;
    for (int i = 0; i <= maxIteration; i++)
	{
	if (AbortRequested())
	    return -1;
	if (ArithType == FLOATEXP || ArithType == EXP_UNSUPPORTED)
	    {
	    PertCalculator[0].BigComplex2ExpComplex(&ExpXSubN[i], zBig);
	    }
	else
	    {
	    XSubN[i] = zBig.CBig2Double();
	    }

	DWORD now = GetTickCount();
	if (now - lastUpdateTick >= 250)	// just check user input and progress every 250 mS
	    {
	    lastUpdateTick = now;

	    // 1. user input
	    if (user_data(NULL) < 0)
		return -1;

	    // 2. progress
	    double progress = (double)i / maxIteration;
	    int percent = (int)(progress * 100.0);

	    _snprintf_s(PertStatus, MAXLINE, _TRUNCATE, "Ref=(%d%%)", percent);
//	    *pPertProgress = percent;
	    }

	// Calculate the set
	RefFunctions(&centre, &zBig, SlopeDegree, subtype, param, scratch);
	if (EnableApproximation && !WeHaveMaxRefIteration)			// only needed for BLA
	    {
	    CoordinateMagnitudeSquared = zBig.CSumSqr();			// no point in further testing once we have MaxRefIteration
	    if ((CoordinateMagnitudeSquared) > bailout && !WeHaveMaxRefIteration)
		{
		MaxRefIteration = i;						// needed for BLA
		WeHaveMaxRefIteration = true;
		break;								// the rest of the table isn't needed for BLA
		}
	    }
	}

    if (!WeHaveMaxRefIteration)
	MaxRefIteration = maxIteration;	// MaxRefIteration not found in main reference loop

    double refSeconds = tRef.stop_ms();
    auto s = FormatElapsed(refSeconds);

#ifdef _DEBUG
    {
    char    buf[256];
    SAFE_SPRINTF(buf, "Reference build: %s, MaxIter = %d, MaxRefIter = %d\n", s.c_str(), maxIteration, MaxRefIteration);
    OutputDebugStringA(buf);
    }
#endif
    scratch.Clear();			// close temp variables for reference creation

    SimpleTimer  tBla;
    tBla.start();

    if (EnableApproximation)
	{
	Bla.clear();
	int M = MaxRefIteration; // the period
	int image_size = min(xdots, ydots);
	if (ArithType == FLOATEXP)
	    {
	    // set up parameters for BLA table
	    ExpComplex temp_size_image_size = ExpComplex(ZoomRadius / (double)image_size, ZoomRadius / (double)image_size);
	    floatexp blaSize = blaSize.hypotExp(temp_size_image_size.x * xdots * 0.5, temp_size_image_size.y * ydots * 0.5);
	    Bla.initExp(M, ExpXSubN, blaSize, power, subtype, maxIteration, param);
	    }
	else if (ArithType == DOUBLE)
	    {
	    Complex temp_size_image_size = Complex(ZoomRadius / (double)image_size, ZoomRadius / (double)image_size);
	    double blaSize = hypot(temp_size_image_size.x * xdots * 0.5, temp_size_image_size.y * ydots * 0.5);
	    Bla.init(M, XSubN, blaSize, power, subtype, maxIteration, param);
	    }
	}

#ifdef _DEBUG
    {
    char    buf[256];
    double blaSeconds = tBla.stop_ms();
    s = FormatElapsed(blaSeconds);
    SAFE_SPRINTF(buf, "BLA build: %s\n", s.c_str());
    OutputDebugStringA(buf);
    }
#endif
    return 0;
    }

