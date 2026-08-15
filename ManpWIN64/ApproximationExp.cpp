/*
    ApproximationExp.cpp - code for evaluating floatexp version of BLA (Bivariant Linear Approximation) in perturbation.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <atomic>
#include "approximation.h"

extern	std::atomic<bool> gStopRequested;	// force early exit

void BLAS::mergeTwoBlasExp(BLAExp *x, BLAExp *y, floatexp blaSize, BLAExp* TwoBlas) 
    {
    int     l;
    floatexp    zero = 0.0;
    l = x->l + y->l;
    // A = y.A * x.A
    ExpComplex A = ExpComplex(y->Ax * x->Ax - y->Ay * x->Ay, y->Ax * x->Ay + y->Ay * x->Ax);
    // B = y.A * x.B + y.B
    ExpComplex B = ExpComplex(y->Ax * x->Bx - y->Ay * x->By + y->Bx, y->Ax * x->By + y->Ay * x->Bx + y->By);
    floatexp xA = xA.hypotExp(x->Ax, x->Ay);
    floatexp xB = xB.hypotExp(x->Bx, x->By);
    floatexp r = MIN(r.sqrtExp(x->r2), MAX(zero, (r.sqrtExp(y->r2) - xB * blaSize) / xA));
    floatexp r2 = r * r;
    TwoBlas->BLALStepExp(r2, A, B, l);
    }

void BLAS::createLStepExp(int level, int m, std::vector<ExpComplex> &Ref, floatexp blaSize, floatexp epsilon, /*ReferenceDecompressor referenceDecompressor*/BLAExp* LStep) 
    {
    if (level == 0) 
	{
	createOneStepExp(Ref, m, epsilon, /*referenceDecompressor*/LStep);
	return;
	}

    int m2 = m << 1;
    int mx = m2 - 1;
    int my = m2;
    int levelm1 = level - 1;
    if (my <= elementsPerLevel[levelm1]) 
	{
	BLAExp x, y;
	createLStepExp(levelm1, mx, Ref, blaSize, epsilon, &x);
	createLStepExp(levelm1, my, Ref, blaSize, epsilon, &y);
	mergeTwoBlasExp(&x, &y, blaSize, LStep);
	return;
	}
    else 
	{
	createLStepExp(levelm1, mx, Ref, blaSize, epsilon, LStep);
	return; 
	}
    }

void BLAS::createOneStepExp(std::vector<ExpComplex> &Ref, int m, floatexp epsilon, BLAExp *OneStep) 
    {
//    Complex Z = fractal.getArrayValue(referenceDecompressor, Ref, m);
//    Complex A = fractal.getBlaA(Z);
    ExpComplex A, B;
    floatexp mA, r, r2 = 0.0;

    if (m >= MaxIter)
	m = MaxIter - 1; // Avoid out of bounds access
    ExpComplex Z = Ref[m];
    if (subtype == 0)		// must be Mandelbrot
	{
	A = Z + Z;
	B = Complex(1.0, 0.0);
	mA = mA.hypotExp(A.x, A.y);
	r = mA * epsilon;
	r2 = r * r;
	}
    else if (subtype == 1)
	{
	A = Z.CPolynomial(power - 1) * (double)power;
	B = Complex(1.0, 0.0);
	r = epsilon * 2 * Z.CFabs() / (power - 1);
	r2 = r * r;
	}
    else if (subtype == 57)
	{
	// derivative A in ExpComplex
	for (int i = 0; i < MAXPOLY; ++i)
	    {
	    const double ak = params[i];
	    const int    k = MAXPOLY - i;
	    if (ak == 0.0) continue;
	    if (k == 0) continue;
	    if (k == 1)
		{
		A += ExpComplex(ak, 0.0);
		}
	    else
		{
		ExpComplex Zp(1.0);
		for (int t = 0; t < k - 1; ++t)
		    Zp *= Z;
		A += Zp * floatexp(ak * (double)k);
		}
	    }

	B = ExpComplex(1.0, 0.0);

	floatexp absZ = Z.CFabs();	    // magnitude in full precision
	floatexp best_r;		    // will hold the minimum
	bool have_r = false;

	floatexp eps = floatexp(epsilon);

	for (int i = 0; i < MAXPOLY; ++i)
	    {
	    floatexp ak = floatexp(params[i]);     // coefficient
	    int k = MAXPOLY - i;                   // power index high -> low

	    if (ak.iszero() || k < 2)
		continue;

	    floatexp denom = floatexp(k - 1);
	    floatexp rk = eps * floatexp(2) * absZ / denom;

	    if (!have_r || rk < best_r)
		{
		best_r = rk;
		have_r = true;
		}
	    }

	if (!have_r)
	    {
	    // fallback if polynomial was visually linear or degenerate
	    best_r = eps * floatexp(2) * absZ;
	    }

	floatexp r2 = best_r * best_r;
	}

    OneStep->BLALStepExp(r2, A, B, 1);
    }

void BLAS::mergeOneStepExp(int m, int elementsSrc, int src, int dest, floatexp blaSize) 
    {
    int mx = m << 1;
    int my = mx + 1;
    if (my < elementsSrc) 
	{
	BLAExp x = bExp[src][mx];
	BLAExp y = bExp[src][my];

	mergeTwoBlasExp(&x, &y, blaSize, &(bExp[dest][m]));
	}
    else 
	{
	bExp[dest][m] = bExp[src][mx];
	}
    }

void BLAS::mergeExp(floatexp blaSize/*, JProgressBar progress*/, long divisor) 
    {
    bool useThreadsForBla = false;

    int elementsDst = 0;
    int src = firstLevel;
    size_t maxLevel = elementsPerLevel.size() - 1;
    for (int elementsSrc = elementsPerLevel[src]; src < maxLevel && elementsSrc > 1; src++) 
	{
	int srcp1 = src + 1;
	elementsDst = elementsPerLevel[srcp1];
	int dst = srcp1;

	int elementsSrcFinal = elementsSrc;
	int srcFinal = src;
	int destFinal = dst;
	    {
	    for(int m = 0; m < elementsDst; m++) 
		{
		mergeOneStepExp(m, elementsSrcFinal, srcFinal, destFinal, blaSize);
		}
	    elementsSrc = elementsDst;
	    }
        }
    }

void BLAS::initExp(int M, std::vector<ExpComplex> &Ref, floatexp blaSize, int powerIn, int subtypeIn, long MaxIterIn, double param[])
    {
    isValid = false;

    if (AbortRequested())
	return;

    power = powerIn;
    subtype = subtypeIn;
    MaxIter = MaxIterIn;
    params.clear();
    for (int i = 0; i < 10; i++)
	params.push_back(param[i]);

    int BLA_BITS = 23;
    int BLA_STARTING_LEVEL = 4;
    floatexp precision = 1.0 / ((double)(1L << BLA_BITS));
    firstLevel = BLA_STARTING_LEVEL - 1;
//        size_t  BLASize, BLASSize;    // a few little tests
//        BLASize = sizeof(BLA);
//        BLASSize = sizeof(BLAS);
    first = true;
    this->M = M;
    int m = M - 1;
    if(m <= 0)
	{
	return;
	}

    std::vector<int> elemPerLevel;

    long total = 1;
    int count = 1;

    for (; m > 1; m = (m + 1) >> 1) 
	{
	elemPerLevel.push_back(m);
	total += m;
	count++;
	}
    elemPerLevel.push_back(m);

    elementsPerLevel.assign(elemPerLevel.begin(), elemPerLevel.end());

    finalTotal = total;
    long removedTotal = 0;

    if(firstLevel > 0) 
	{
	for(int i = 0; i < firstLevel && i < elementsPerLevel.size(); i++) 
	    {
	    if (AbortRequested())
		return;
	    removedTotal += elementsPerLevel[i];
	    }
	finalTotal -= removedTotal;
	}

    returnL1 = firstLevel == 0;
    long divisor = finalTotal > 0x7fffffff ? finalTotal / 10000 : 1;
    L = count;
    bExp.resize(count);
    LM1 = L - 1;

    if(firstLevel >= elementsPerLevel.size())
	{
	return;
	}

    for(int l = firstLevel; l < bExp.size(); l++) 
	{
	bExp[l].resize(elementsPerLevel[l]);
	}

    if (initExp(Ref, blaSize, precision/*, progress*/, divisor) < 0)
	return;
    mergeExp(blaSize/*, progress*/, divisor);
    size_t  SizeofbExp = sizeof(bExp) * bExp.size() * bExp[0].size();
    isValid = true;
    }

int	BLAS::initExp(std::vector<ExpComplex> &Ref, floatexp blaSize, floatexp epsilon/*, JProgressBar progress*/, long divisor)
    {
    int elements = elementsPerLevel[firstLevel];
    done = 0;
    for (int m = 0; m < elements; m++) 
	{
	if (AbortRequested())
	    return -1;

	if (bExp[firstLevel].size() <= m)
	    break;
	createLStepExp(firstLevel, m + 1, Ref, blaSize, epsilon/*, referenceDecompressor*/, &(bExp[firstLevel][m]));
	}
    return 0;
    }

const BLAExp * BLAS::lookupExp(int m, floatexp z2, int iterations, int max_iterations) const
    {
    if (m <= 0 || m >= M) 
	{
        return nullptr;
        }

    const BLAExp *B = nullptr;
    int ix = (m - 1) >> firstLevel;
    for (int level = firstLevel; level < L; ++level) 
	{
	if (AbortRequested())
	    return nullptr;
	if (level < 0 || level >= (int)b.size())
	    return nullptr;
	if (ix < 0 || ix >= (int)b[level].size())
	    return nullptr;

	int ixm = (ix << level) + 1;
	floatexp z2m = bExp[level][ix].r2;
        if (m == ixm && z2 < z2m) 
	    {
	    int l = bExp[level][ix].l;
            if (iterations + l <= max_iterations) 
		{
                B = &(bExp[level][ix]);
                }
            }
        else 
	    {
            break;
            }
        ix = ix >> 1;
        }
    return B;
    }

BLAExp	BLAExp::BLALStepExp(floatexp r2, ExpComplex A, ExpComplex B, int l) 
    {
    this->Ax = A.x;
    this->Ay = A.y;
    this->Bx = B.x;
    this->By = B.y;
    this->r2 = r2;
    this->l = l;
    return *this;
    }

