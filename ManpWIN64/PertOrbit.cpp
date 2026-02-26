/****************************************************
    PERTORBIT.CPP a module to explore Perturbation Orbit functions
    Thanks to Claude Heiland-Allen https://fractalforums.org/programming/11/perturbation-code-for-cubic-and-higher-order-polynomials/2783,
    SuperHeal https://fractalforums.org/index.php?topic=5524.new#new
    and Shirom Makkad https://github.com/ShiromMakkad/MandelbrotPerturbation
    for their inspiration and methods of calculating perturbation fractals
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include <atomic>
#include "PertEngine.h"
#include "Potential.h"

extern	std::atomic<bool> gStopRequested;	// force early exit

//////////////////////////////////////////////////////////////////////
// Individual point calculation
//////////////////////////////////////////////////////////////////////

int	CPerturbation::iterateFractalWithPerturbationBLA(const std::vector<Complex> *XSubN, int MaxIteration, double bailout, Complex& DeltaSub0, const BLAS *Bla, CTZfilter &TZfilter, Complex &z, Complex &dc, int user_data(HWND hwnd))
    {
    int iterations = 0;
    int RefIteration = iterations;

    Complex DeltaSubN = 0.0; //0
    Complex w, dz, d0, temp1;
    z = 0.0;

    if (user_data(NULL) < 0)
	{
	gStopRequested.store(true, std::memory_order_relaxed);
	return -1;
	}
    if (InsideMethod == BOF60 || InsideMethod == BOF61)
	{
	BOFmagnitude = 0.0;
	min_orbit = 100000.0;
	}

    if (OutsideMethod >= TIERAZONFILTERS)
	TZfilter.LoadFilterQ(DeltaSub0);		// initialise the constants used by Tierazon filters

    dz = DeltaSubN;
    d0 = DeltaSub0;
    ZCoordinateMagnitudeSquared = 0;

    double DeltaNormSquared = 0.0;

    while (iterations < MaxIteration) 			// perturbation iteration
	{
	if (gStopRequested)
	    return -1;
	ZCoordinateMagnitudeSquared = z.CSumSqr();
	if (ZCoordinateMagnitudeSquared >= bailout) 
	    {
	    return iterations;
	    }

	if (SlopeType == DERIVSLOPE) 
	    {
	    // Reuse your legacy routine; handles subtype 0 (Mandelbrot) and 1 (Power)
	    CalculateDerivativeSlope(dc, z);
	    }

	temp1 = (*XSubN)[RefIteration];
	PertFunctions(&temp1, &dz, &d0);
	RefIteration++;
	if (MaxIteration > 1 && RefIteration < MaxIteration) 
	    {
	    temp1 = (*XSubN)[RefIteration];
	    z = temp1 + dz;
	    }

	if (OutsideMethod >= TIERAZONFILTERS)
	    {
	    Complex Z = z;
	    TZfilter.DoTierazonFilter(Z, (long *)&iterations);
	    }
	else if (InsideMethod == BOF60 || InsideMethod == BOF61)
	    {
	    Complex Z = z;
	    BOFmagnitude = Z.CSumSqr();
	    if (BOFmagnitude < min_orbit)
		{
		min_orbit = BOFmagnitude;
		min_index = iterations + 1L;
		}
	    }
	iterations++;
	if (ArithType == DOUBLE)
	    {
	    if (EnableApproximation)
		{
		DeltaNormSquared = dz.CSumSqr();
		const BLA *bPtr = nullptr;
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

		    // Apply linear map to dz
		    const double Ax = bPtr->Ax, Ay = bPtr->Ay;
		    const double Bx = bPtr->Bx, By = bPtr->By;

		    DeltaSubN = Complex(
			Ax * dz.x - Ay * dz.y + Bx * d0.x - By * d0.y,
			Ax * dz.y + Ay * dz.x + Bx * d0.y + By * d0.x
		    );
		    dz = DeltaSubN;
//		    DeltaSubN = Complex(bPtr->Ax * dz.x - bPtr->Ay * dz.y + bPtr->Bx * d0.x - bPtr->By * d0.y, bPtr->Ax * dz.y + bPtr->Ay * dz.x + bPtr->Bx * d0.y + bPtr->By * d0.x);
		    dz = DeltaSubN;
		    // Propagate derivative if slope is enabled
		    if (SlopeType == DERIVSLOPE) {
			Complex A(Ax, Ay);
			Complex B(Bx, By);
			dc = A * dc + B;   // Uses your existing Complex ops
			}

		    temp1 = (*XSubN)[RefIteration];
		    z = temp1 + dz;
		    DeltaNormSquared = dz.CSumSqr();
		    }
		}
	    }
	if (ArithType == DBL_UNSUPPORTED)		// otherwise it is done in the BLA process
	    DeltaNormSquared = dz.CSumSqr();
//rebase
	ZCoordinateMagnitudeSquared = z.CSumSqr();
	if (ZCoordinateMagnitudeSquared < DeltaNormSquared || (RefIteration >= MaxRefIteration)) 
	    {
	    dz = z;
	    RefIteration = 0;
	    }
	}
    return MaxIteration;
    }

//////////////////////////////////////////////////////////////////////
// Individual point calculation - with BLA - Exp
//////////////////////////////////////////////////////////////////////
int	CPerturbation::iterateFractalWithPerturbationBLAExp(const std::vector<ExpComplex> *XSubN, int MaxIteration, double bailout, ExpComplex& DeltaSub0, const BLAS *Bla, CTZfilter &TZfilter, ExpComplex &z, ExpComplex &dc, int user_data(HWND hwnd))
    {
    const int frameMaxIter = MaxIteration;			// <<<
    const size_t refSize = XSubN->size();			// <<<
    if (refSize <= 0) return frameMaxIter;			// <<<

    int iterations = 0;
    int RefIteration = iterations;

    ExpComplex DeltaSubN = 0.0;
    ExpComplex w, dz, d0, temp1;

    floatexp ExpBailout = bailout;
    z = 0.0;

    if (user_data(NULL) < 0) 
	return -1;

    if (InsideMethod == BOF60 || InsideMethod == BOF61)
	{
	BOFmagnitude = 0.0;
	min_orbit = 100000.0;
	}

    if (OutsideMethod >= TIERAZONFILTERS) {
	Complex tempComplex;
	tempComplex.x = z.x.todouble();
	tempComplex.y = z.y.todouble();
	TZfilter.LoadFilterQ(tempComplex);
	}

    dz = DeltaSubN;
    d0 = DeltaSub0;

    // --- FIX #1: make this a LOCAL, not a global ---
    floatexp ExpZCoordinateMagnitudeSquared = floatexp(0.0);

    // --- declare this; will be set on every loop path below ---
    floatexp ExpDeltaNormSquared = floatexp(0.0);

    while (iterations < frameMaxIter)
	{
	ExpZCoordinateMagnitudeSquared = z.CSumSqrExp();

	// --- compare against floatexp bailout, not double ---
	if (ExpZCoordinateMagnitudeSquared > ExpBailout)
	    {
	    if (iterations > frameMaxIter) iterations = frameMaxIter;
	    return iterations;
	    }

	if (SlopeType == DERIVSLOPE)
	    {
	    // Reuse your legacy routine; handles subtype 0 (Mandelbrot) and 1 (Power)
	    BigCalculateDerivativeSlope(dc, z);
	    }

	// perturbation iteration
       // Guard for BigPertFunctions usage
	if ((size_t)RefIteration >= refSize)		    // <<<
	    return frameMaxIter;                            // <<<
	temp1 = (*XSubN)[RefIteration];
	BigPertFunctions(&temp1, &dz, &d0);
	RefIteration++;
	if (MaxIteration > 1 && RefIteration < MaxIteration)
	    {
	    if ((size_t)RefIteration >= refSize)           // <<<
		return frameMaxIter;                        // <<<
	    temp1 = (*XSubN)[RefIteration];
	    z = temp1 + dz;
	    }

	if (OutsideMethod >= TIERAZONFILTERS) {
	    Complex tempComplex;
	    tempComplex.x = z.x.todouble();
	    tempComplex.y = z.y.todouble();
	    TZfilter.DoTierazonFilter(tempComplex, (long*)&iterations);
	    }
	else if (InsideMethod == BOF60 || InsideMethod == BOF61) {
	    ExpComplex Z = z;
	    BOFmagnitude = Z.CSumSqr();
	    if (BOFmagnitude < min_orbit) {
		min_orbit = BOFmagnitude;
		min_index = iterations + 1L;
		}
	    }

	iterations++;

	if (ArithType == FLOATEXP) 
	    {
	    if (EnableApproximation) 
		{
		ExpDeltaNormSquared = dz.CSumSqrExp(); // always (re)set before use
		const BLAExp *bPtr = nullptr;
		while (Bla->isValid && iterations < MaxIteration)
		    {
		    bPtr = Bla->lookupExp(RefIteration, ExpDeltaNormSquared, iterations, MaxIteration);
		    if (bPtr == nullptr)
			{
			break; // No more BLA entries found
			}

		    int l = bPtr->l;

		    iterations += bPtr->l;
		    RefIteration += bPtr->l;

		    if (RefIteration >= (int)XSubN->size())
			return MaxIteration;

		    // Apply linear map to dz
		    floatexp Ax = bPtr->Ax, Ay = bPtr->Ay;
		    floatexp Bx = bPtr->Bx, By = bPtr->By;

		    DeltaSubN = ExpComplex(
			Ax * dz.x - Ay * dz.y + Bx * d0.x - By * d0.y,
			Ax * dz.y + Ay * dz.x + Bx * d0.y + By * d0.x);
		    dz = DeltaSubN;

		    // Propagate derivative if slope is enabled
		    if (SlopeType == DERIVSLOPE)
			{
			ExpComplex A(Ax, Ay);
			ExpComplex B(Bx, By);
			// normal holomorphic case
			dc = A * dc + B;   // Uses your existing Complex ops
			}

		    // Recompose z from reference + delta
		    z = (*XSubN)[RefIteration];
		    z += dz;
			
		    if ((size_t)RefIteration >= refSize)                 // <<<
			return frameMaxIter;                              // <<<
		    temp1 = (*XSubN)[RefIteration];
		    z = temp1 + dz;
		    ExpDeltaNormSquared = dz.CSumSqrExp(); // update for next loop
		    }
		}
	    else {
		// --- FIX #2: set when approximation is OFF ---
		ExpDeltaNormSquared = dz.CSumSqrExp();
		}
	    }
	else if (ArithType == EXP_UNSUPPORTED) 
	    {
	    ExpDeltaNormSquared = dz.CSumSqrExp();
	    }

	// rebase
	ExpZCoordinateMagnitudeSquared = z.CSumSqrExp();
	if (ExpZCoordinateMagnitudeSquared < ExpDeltaNormSquared || (RefIteration >= MaxRefIteration))
	    {
	    dz = z;
	    RefIteration = 0;
	    }
	}
    return MaxIteration;
    }

