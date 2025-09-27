/****************************************************
    PERTORBIT.CPP a module to explore Perturbation Orbit functions
    Thanks to Claude Heiland-Allen https://fractalforums.org/programming/11/perturbation-code-for-cubic-and-higher-order-polynomials/2783,
    SuperHeal https://fractalforums.org/index.php?topic=5524.new#new
    and Shirom Makkad https://github.com/ShiromMakkad/MandelbrotPerturbation
    for their inspiration and methods of calculating perturbation fractals
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include "PertEngine.h"
#include "Potential.h"

//////////////////////////////////////////////////////////////////////
// Individual point calculation
//////////////////////////////////////////////////////////////////////

int	CPerturbation::iterateFractalWithPerturbation(Complex *XSubN, Complex *DeltaSub0, Complex *DeltaSubN, int MaxIteration, double bailout, int x, int y, int user_data(HWND hwnd),
	CTZfilter *TZfilter, ExpComplex	*ExpDC, Complex *dc, bool *glitched)
    {
    Complex	temp, temp1;
    ExpComplex	ExpTemp, ExpTemp1;

    int		iteration = 0;
    do
	{
	if (user_data(NULL) < 0)
	    return -1;
	if (ArithType == DOUBLESHIFT)
	    temp1 = *(XSubN + iteration) + *DeltaSubN * Scaling;
	else
	    temp1 = *(XSubN + iteration) + *DeltaSubN;

#ifdef	ALLOW_SIN
	PertFunctions((XSubN + iteration), (sinZp + iteration), (cosZp + iteration), &DeltaSubN, &DeltaSub0);
#else
	PertFunctions((XSubN + iteration), DeltaSubN, DeltaSub0);
#endif
	iteration++;
	temp = *(XSubN + iteration) + *DeltaSubN * Scaling;
	ZCoordinateMagnitudeSquared = temp.CSumSqr();

	if (SlopeType == DERIVSLOPE)
	    {
	    if (ArithType == DOUBLESHIFT)
		{
		ExpTemp1 = temp1;
		BigCalculateDerivativeSlope(ExpDC, ExpTemp1);
		}
	    else
		CalculateDerivativeSlope(dc, temp1);
	    }

	if (OutsideMethod >= TIERAZONFILTERS)
	    {
	    Complex z = *(XSubN + iteration) + *DeltaSubN;
	    TZfilter->DoTierazonFilter(z, (long *)&iteration);
	    }
	else if (InsideMethod == BOF60 || InsideMethod == BOF61)
	    {
	    Complex z = *(XSubN + iteration) + *DeltaSubN;
	    BOFmagnitude = z.CSumSqr();
	    if (BOFmagnitude < min_orbit)
		{
		min_orbit = BOFmagnitude;
		min_index = iteration + 1L;
		}
	    }

	// This is Pauldelbrot's glitch detection method. You can see it here: http://www.fractalforums.com/announcements-and-news/pertubation-theory-glitches-improvement/.
	// As for why it looks so weird, it's because I've squared both sides of his equation and moved the |ZsubN| to the other side to be precalculated.
	// For more information, look at where the reference point is calculated.
	// I also only want to store this point once.

	if (calculateGlitches == true && *glitched == false && ZCoordinateMagnitudeSquared < *(PerturbationToleranceCheck + iteration))
	    {
	    Point pt(x, y, iteration);
	    glitchPoints[GlitchPointCount] = pt;
	    GlitchPointCount++;
	    *glitched = true;
	    break;
	    }

	//use bailout radius of 256 for smooth coloring.
	} while (ZCoordinateMagnitudeSquared < bailout && iteration < MaxIteration);
    return iteration;
    }

//////////////////////////////////////////////////////////////////////
// Individual point calculation
//////////////////////////////////////////////////////////////////////

int	CPerturbation::iterateFractalWithPerturbationExp(ExpComplex *ExpXSubN, ExpComplex *ExpDeltaSub0, ExpComplex *ExpDeltaSubN, int MaxIteration, double bailout, int x, int y, int user_data(HWND hwnd),
	CTZfilter *TZfilter, ExpComplex	*ExpDC, bool *glitched)
    {
    Complex	temp, temp1;
    ExpComplex	ExpTemp, ExpTemp1;
    ExpComplex	ExpX_N;
    floatexp	ExpBailout = bailout;
    int		iteration = 0;

    do
	{
	if (user_data(NULL) < 0)
	    return -1;
	ExpTemp1 = *(ExpXSubN + iteration) + *ExpDeltaSubN;

	if (SlopeType == DERIVSLOPE)
	    BigCalculateDerivativeSlope(ExpDC, ExpTemp1);
	ExpX_N = *(ExpXSubN + iteration);

	BigPertFunctions(&ExpX_N, ExpDeltaSubN, ExpDeltaSub0);

	iteration++;
	ExpTemp = *ExpDeltaSubN + *(ExpXSubN + iteration);
	ExpZCoordinateMagnitudeSquared = ExpTemp.CSumSqrExp();

	if (OutsideMethod >= TIERAZONFILTERS)
	    {
	    Complex	tempComplex;
	    ExpTemp = *ExpDeltaSubN + *(ExpXSubN + iteration);
	    tempComplex.x = ExpTemp.x.todouble();
	    tempComplex.y = ExpTemp.y.todouble();
	    TZfilter->DoTierazonFilter(tempComplex, (long *)&iteration);
	    }

	// This is Pauldelbrot's glitch detection method. You can see it here: http://www.fractalforums.com/announcements-and-news/pertubation-theory-glitches-improvement/.
	// As for why it looks so weird, it's because I've squared both sides of his equation and moved the |ZsubN| to the other side to be precalculated.
	// For more information, look at where the reference point is calculated.
	// I also only want to store this point once.

	if (calculateGlitches == true && *glitched == false && ExpZCoordinateMagnitudeSquared < *(ExpPerturbationToleranceCheck + iteration))
	    {
	    Point pt(x, y, iteration);
	    glitchPoints[GlitchPointCount] = pt;
	    GlitchPointCount++;
	    *glitched = true;
	    break;
	    }
	//use bailout radius of 256 for smooth coloring.
	} while (ExpZCoordinateMagnitudeSquared < ExpBailout && iteration < MaxIteration);
    return iteration;
    }

//////////////////////////////////////////////////////////////////////
// Individual point calculation - with BLA
//////////////////////////////////////////////////////////////////////

int	CPerturbation::iterateFractalWithPerturbationBLA(Complex *XSubN, int MaxIteration, double bailout, Complex& DeltaSub0, BLAS *Bla, CTZfilter *TZfilter, Complex *z, int user_data(HWND hwnd))
    {
    int iterations = 0;
    int RefIteration = iterations;

    Complex DeltaSubN = 0.0; //0
    Complex w, dz, d0, temp1;
    *z = 0.0;

    if (user_data(NULL) < 0)
	return -1;
    if (InsideMethod == BOF60 || InsideMethod == BOF61)
	{
	BOFmagnitude = 0.0;
	min_orbit = 100000.0;
	}

    if (OutsideMethod >= TIERAZONFILTERS)
	TZfilter->LoadFilterQ(DeltaSub0);		// initialise the constants used by Tierazon filters

    dz = DeltaSubN;
    d0 = DeltaSub0;
    ZCoordinateMagnitudeSquared = 0;

    double DeltaNormSquared;

    while (iterations < MaxIteration) 			// perturbation iteration
	{
	ZCoordinateMagnitudeSquared = z->CSumSqr();
	if (ZCoordinateMagnitudeSquared >= bailout) 
	    {
	    return iterations;
	    }

	PertFunctions((XSubN + RefIteration), &dz, &d0);
	RefIteration++;
	if (MaxIteration > 1 && RefIteration < MaxIteration) 
	    {
	    *z = (*(XSubN + RefIteration)) + dz;
	    }

	if (OutsideMethod >= TIERAZONFILTERS)
	    {
	    Complex Z = *z;
	    TZfilter->DoTierazonFilter(Z, (long *)&iterations);
	    }
	else if (InsideMethod == BOF60 || InsideMethod == BOF61)
	    {
	    Complex Z = *z;
	    BOFmagnitude = Z.CSumSqr();
	    if (BOFmagnitude < min_orbit)
		{
		min_orbit = BOFmagnitude;
		min_index = iterations + 1L;
		}
	    }
	iterations++;
	if (ArithType == BLA_DOUBLE)
	    {
	    if (EnableApproximation)
		{
		DeltaNormSquared = dz.CSumSqr();
		BLA *bPtr = nullptr;
		while (Bla->isValid && iterations < MaxIteration) 
		    {
		    bPtr = Bla->lookup(RefIteration, DeltaNormSquared, iterations, MaxIteration);
		    if (bPtr == nullptr) 
			{
			break; // No more BLA entries found
			}

		    int l = bPtr->l;
		    iterations += l;
		    if (iterations > MaxIteration) 
			{
			iterations = MaxIteration;
			break;
			}
		    RefIteration += l;
		    if (RefIteration >= MaxIteration)
			{
			RefIteration = MaxIteration - 1; // Avoid out of bounds
			break;
			}

		    DeltaSubN = Complex(bPtr->Ax * dz.x - bPtr->Ay * dz.y + bPtr->Bx * d0.x - bPtr->By * d0.y, bPtr->Ax * dz.y + bPtr->Ay * dz.x + bPtr->Bx * d0.y + bPtr->By * d0.x);
		    dz = DeltaSubN;
		    *z = (*(XSubN + RefIteration)) + dz;
		    DeltaNormSquared = dz.CSumSqr();
		    }
		}
	    }
	if (ArithType == BLA_DBL_UNSUPPORTED)		// otherwise it is done in the BLA process
	    DeltaNormSquared = dz.CSumSqr();
//rebase
	ZCoordinateMagnitudeSquared = z->CSumSqr();
	if (ZCoordinateMagnitudeSquared < DeltaNormSquared || (RefIteration >= MaxRefIteration)) 
	    {
	    dz = *z;
	    RefIteration = 0;
	    }
	}
    return MaxIteration;
    }

//////////////////////////////////////////////////////////////////////
// Individual point calculation - with BLA - Exp
//////////////////////////////////////////////////////////////////////

int	CPerturbation::iterateFractalWithPerturbationBLAExp(ExpComplex *XSubN, int MaxIteration, double bailout, ExpComplex& DeltaSub0, BLAS *Bla, CTZfilter *TZfilter, ExpComplex *z, int user_data(HWND hwnd))
    {
    int iterations = 0;
    int RefIteration = iterations;

    ExpComplex	DeltaSubN = 0.0;
    ExpComplex	w, dz, d0, temp1;
    floatexp	ExpBailout = bailout;
    *z = 0.0;

    if (user_data(NULL) < 0)
	return -1;
    if (InsideMethod == BOF60 || InsideMethod == BOF61)
	{
	BOFmagnitude = 0.0;
	min_orbit = 100000.0;
	}

    if (OutsideMethod >= TIERAZONFILTERS)
	{
	Complex	tempComplex;
	tempComplex.x = z->x.todouble();
	tempComplex.y = z->y.todouble();
	TZfilter->LoadFilterQ(tempComplex);		// initialise the constants used by Tierazon filters
	}

    dz = DeltaSubN;
    d0 = DeltaSub0;
    ExpZCoordinateMagnitudeSquared = 0.0;

    floatexp ExpDeltaNormSquared;

    while (iterations < MaxIteration) 			// perturbation iteration
	{
	ExpZCoordinateMagnitudeSquared = z->CSumSqrExp();
	if (ExpZCoordinateMagnitudeSquared >= bailout)
	    {
	    if (iterations > MaxIteration)
		iterations = MaxIteration; // Avoid out of bounds
	    return iterations;
	    }

	BigPertFunctions((XSubN + RefIteration), &dz, &d0);
	RefIteration++;
	if (MaxIteration > 1 && RefIteration < MaxIteration)
	    {
	    *z = (*(XSubN + RefIteration)) + dz;
	    }

	if (OutsideMethod >= TIERAZONFILTERS)
	    {
	    Complex	tempComplex;
	    tempComplex.x = z->x.todouble();
	    tempComplex.y = z->y.todouble();
	    TZfilter->DoTierazonFilter(tempComplex, (long *)&iterations);
	    }
	else if (InsideMethod == BOF60 || InsideMethod == BOF61)
	    {
	    ExpComplex Z = *z;
	    BOFmagnitude = Z.CSumSqr();
	    if (BOFmagnitude < min_orbit)
		{
		min_orbit = BOFmagnitude;
		min_index = iterations + 1L;
		}
	    }

	iterations++;
	if (ArithType == BLA_FLOATEXP)
	    {
	    if (EnableApproximation)
		{
		ExpDeltaNormSquared = dz.CSumSqrExp();
		BLAExp *bPtr = nullptr;
		while (Bla->isValid && iterations < MaxIteration)
		    {
		    bPtr = Bla->lookupExp(RefIteration, ExpDeltaNormSquared, iterations, MaxIteration);
		    if (bPtr == nullptr) 
			{
			break; // No more BLA entries found
			}

		    int l = bPtr->l;
		    iterations += l;
		    if (iterations > MaxIteration) 
			{
			iterations = MaxIteration;
			break;
			}
		    RefIteration += l;
		    if (RefIteration >= MaxIteration)
			{
			RefIteration = MaxIteration - 1; // Avoid out of bounds
			break;
			}

		    DeltaSubN = ExpComplex(bPtr->Ax * dz.x - bPtr->Ay * dz.y + bPtr->Bx * d0.x - bPtr->By * d0.y, bPtr->Ax * dz.y + bPtr->Ay * dz.x + bPtr->Bx * d0.y + bPtr->By * d0.x);
		    dz = DeltaSubN;
		    *z = (*(XSubN + RefIteration)) + dz;
		    ExpDeltaNormSquared = dz.CSumSqrExp();
		    }
		}
	    }
	if (ArithType == BLA_EXP_UNSUPPORTED)		// otherwise it is done in the BLA process
	    ExpDeltaNormSquared = dz.CSumSqrExp();
//rebase
	ExpZCoordinateMagnitudeSquared = z->CSumSqrExp();
	if (ExpZCoordinateMagnitudeSquared < ExpDeltaNormSquared || (RefIteration >= MaxRefIteration))
	    {
	    dz = *z;
	    RefIteration = 0;
	    }
	}
    return MaxIteration;
    }

