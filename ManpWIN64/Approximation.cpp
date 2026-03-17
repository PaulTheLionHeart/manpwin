#include <atomic>
#include "approximation.h"

extern	std::atomic<bool> gStopRequested;	// force early exit

void BLAS::mergeTwoBlas(BLA *x, BLA *y, double blaSize, BLA* TwoBlas) 
    {
    int     l;

    l = x->l + y->l;
    // A = y.A * x.A
    Complex A = Complex(y->Ax * x->Ax - y->Ay * x->Ay, y->Ax * x->Ay + y->Ay * x->Ax);
    // B = y.A * x.B + y.B
    Complex B = Complex(y->Ax * x->Bx - y->Ay * x->By + y->Bx, y->Ax * x->By + y->Ay * x->Bx + y->By);
    double xA = hypot(x->Ax, x->Ay);
    double xB = hypot(x->Bx, x->By);
    double r = MIN(sqrt(x->r2), MAX(0, (sqrt(y->r2) - xB * blaSize) / xA));
    double r2 = r * r;
    TwoBlas->BLALStep(r2, A, B, l);
    }

void BLAS::createLStep(int level, int m, std::vector<Complex> &Ref, double blaSize, double epsilon, BLA* LStep) 
    {
    if (level == 0) 
	{
	createOneStep(Ref, m, epsilon, LStep);
	return;
	}

    int m2 = m << 1;
    int mx = m2 - 1;
    int my = m2;
    int levelm1 = level - 1;
    if (my <= elementsPerLevel[levelm1]) 
	{
	BLA x, y;
	createLStep(levelm1, mx, Ref, blaSize, epsilon, &x);
	createLStep(levelm1, my, Ref, blaSize, epsilon, &y);
	mergeTwoBlas(&x, &y, blaSize, LStep);
	return;
	}
    else 
	{
	createLStep(levelm1, mx, Ref, blaSize, epsilon, LStep);
	return; 
	}
    }

void BLAS::createOneStep(std::vector<Complex> &Ref, int m, double epsilon, BLA* OneStep) 
    {
//	Complex Z = fractal.getArrayValue(referenceDecompressor, Ref, m);
//	Complex A = fractal.getBlaA(Z);
    Complex A, B;
    double mA, r, r2 = 0.0;

    if (AbortRequested())
	return;
    if (m >= MaxIter)
	m = MaxIter - 1;	// Avoid out of bounds access
    Complex Z = Ref[m];
    if (subtype == 0)		// must be Mandelbrot
	{
	A = Z + Z;
	B = Complex(1.0, 0.0);
	mA = hypot(A.x, A.y);
	r = mA * epsilon;
	r2 = r * r;
	}
    else if (subtype == 1)	// power
	{
	A = Z.CPolynomial(power - 1) * (double)power;
	B = Complex(1.0, 0.0);
	r = epsilon * 2 * Z.CFabs() / (power - 1);
	r2 = r * r;
	}
    else if (subtype == 57)					// subtype == 57  ->  Polynomial:  f(Z) = sum_k a_k * Z^k
								// A = f'(Z) = sum_k a_k * k * Z^(k-1)
								// B = 1
								// r  (safe radius): take the conservative min of the power rule used for Z^k,
								//     i.e. r_k = epsilon * 2 * |Z| / (k-1)  for every active k >= 2
	{
	// Build A = f'(Z)
	Complex A = Complex(0.0, 0.0);
	for (int i = 0; i < MAXPOLY; ++i)
	    {
	    const double ak = params[i];				// your polynomial coeffs
	    const int    k = MAXPOLY - i;			// degree for this slot
	    if (ak == 0.0) continue;
	    if (k == 0) continue;				// constant term -> no derivative
	    if (k == 1)
		{						// linear term: derivative = a1
		A += Complex(ak, 0.0);
		}
	    else
		{
		// Z^(k-1)
		Complex Zp(1.0, 0.0);
		for (int t = 0; t < k - 1; ++t)
		    Zp *= Z;
		A += Zp * (ak * (double)k);
		}
	    }

	// B is always 1 for z_{n+1} = f(z_n) + c
	const Complex B(1.0, 0.0);

	// Conservative radius: min over active degrees k>=2 of  epsilon * 2*|Z|/(k-1)
	double r = std::numeric_limits<double>::infinity();
	const double absZ = Z.CFabs();
	for (int i = 0; i < MAXPOLY; ++i)
	    {
	    const double ak = params[i];
	    const int    k = MAXPOLY - i;
	    if (ak == 0.0 || k < 2) continue;
	    /*
			// put this back in if image fails again
			if (fabs(epsilon) < 1e-300)
			    {
			    char buf[128];
			     _snprintf_s(buf, 128, _TRUNCATE, "epsilon underflow: %.3e", epsilon);
			    OutputDebugStringA(buf);
			    }
	    */
	    const double rk = (k > 1) ? (epsilon * 2.0 * absZ / (double)(k - 1)) : std::numeric_limits<double>::infinity();
	    if (rk < r) r = rk;
	    }

	if (!std::isfinite(r))
	    {
	    // fallback (e.g., pure linear/constant): behave like Mandelbrot's heuristic
	    r = epsilon * 2.0 * absZ;
	    }

	// --- Add this tiny floor to prevent double underflow ---
	const double r_min = 1e-300;
	if (fabs(r) < r_min && fabs(r) > 0.0)
	    r = r_min;

	r = max(r, 1e-300);
	const double r2 = r * r;
	if (!std::isfinite(A.x) || !std::isfinite(A.y) ||
	    !std::isfinite(B.x) || !std::isfinite(B.y) ||
	    !std::isfinite(r2)) {
	    debugPrint("BLA FAIL: non-finite at refIter=" + std::to_string(m));
	    debugPrint("A=" + std::to_string(A.x) + "," + std::to_string(A.y));
	    debugPrint("B=" + std::to_string(B.x) + "," + std::to_string(B.y));
	    debugPrint("r2=" + std::to_string(r2));
	    return;   // or skip this pixel safely
	    }
	}

    OneStep->BLALStep(r2, A, B, 1);
    }

void BLAS::mergeOneStep(int m, int elementsSrc, int src, int dest, double blaSize) 
    {
    int mx = m << 1;
    int my = mx + 1;
    if (my < elementsSrc) 
	{
	BLA x = b[src][mx];
	BLA y = b[src][my];
	mergeTwoBlas(&x, &y, blaSize, &(b[dest][m]));
	}
    else 
	{
	b[dest][m] = b[src][mx];
	}
    }

// Add the clear method to free memory  
void BLAS::clear() 
    {
    data.clear();
    b.clear();
    bExp.clear();
    isValid = false;
    }

void BLAS::merge(double blaSize, long divisor) 
    {
//        boolean useThreadsForBla = TaskRender.USE_THREADS_FOR_BLA;
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
		mergeOneStep(m, elementsSrcFinal, srcFinal, destFinal, blaSize);
		}

            elementsSrc = elementsDst;
	    }
        }
    }

void BLAS::init(int M, std::vector<Complex> &Ref, double blaSize, int powerIn, int subtypeIn, long MaxIterIn, double param[])
    {
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
    double precision = 1 / ((double)(1L << BLA_BITS));
    firstLevel = BLA_STARTING_LEVEL - 1;
    isValid = false;
//    size_t  BLASize, BLASSize;    // a few little tests
//    BLASize = sizeof(BLA);
//    BLASSize = sizeof(BLAS);
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

//    long divisor = finalTotal > Constants.MAX_PROGRESS_VALUE ? finalTotal / Constants.PROGRESS_SCALE : 1;
    long divisor = finalTotal > 0x7fffffff ? finalTotal / 10000 : 1;

    L = count;
    b.resize(count);
    LM1 = L - 1;

    if(firstLevel >= elementsPerLevel.size())
	{
	return;
	}

    for(int l = firstLevel; l < b.size(); l++)	
	{
	b[l].resize(elementsPerLevel[l]);
	}

    init(Ref, blaSize, precision, divisor);

    merge(blaSize/*, progress*/, divisor);

    size_t  SizeofB = sizeof(b) * b.size() * b[0].size();

/*
        for (int i = 0; i < L; ++i) {
            std::ostringstream oss;
            size_t ItemCount = b[i].size();
            oss << "Level: " << i << " with " << ItemCount << " items ***************\n";
//            oss << "l = " << b[i][0].getL(GetBLAL) << "\tr = " << sqrt(b[i][0].r2) << " \n";
            debugPrint(oss.str());
            if (ItemCount <= 10) {
                for (int j = 0; j < ItemCount; j++) {
                    std::ostringstream oss;
                    oss << "b[" << i << "][" << j << "]: Ax=" << b[i][j].Ax << ", Ay=" << b[i][j].Ay << ", Bx=" << b[i][j].Bx << ", By=" << b[i][j].By << ", r2=" << b[i][j].r2 << ", l=" << b[i][j].l << "\n";
                    debugPrint(oss.str());
                    }
                }
            else {
                for (int j = 0; j < 5; j++) {
                    std::ostringstream oss;
                    oss << "b[" << i << "][" << j << "]: Ax=" << b[i][j].Ax << ", Ay=" << b[i][j].Ay << ", Bx=" << b[i][j].Bx << ", By=" << b[i][j].By << ", r2=" << b[i][j].r2 << ", l=" << b[i][j].l << "\n";
                    debugPrint(oss.str());
                    }
                for (int j = 0; j < 5; j++) {
                    size_t ItemPtr = j + ItemCount - 5;
                    std::ostringstream oss;
                    oss << "b[" << i << "][" << ItemPtr << "]: Ax=" << b[i][ItemPtr].Ax << ", Ay=" << b[i][ItemPtr].Ay << ", Bx=" << b[i][ItemPtr].Bx << ", By=" << b[i][ItemPtr].By << ", r2=" << b[i][ItemPtr].r2 << ", l=" << b[i][ItemPtr].l << "\n";
                    debugPrint(oss.str());
                    }

                }
                
//            oss << "l = " << b[i][0].getL(GetBLAL) << "\tr = " << sqrt(b[i][0].r2) << " \n";
            }
*/
    isValid = true;
    }

void BLAS::init(std::vector<Complex> &Ref, double blaSize, double epsilon, long divisor) 
    {
    int elements = elementsPerLevel[firstLevel];
    // lots of thread stuff deleted for now

    done = 0;
    for (int m = 0; m < elements; m++) 
	{
	if (AbortRequested())
	    return;
	if (b[firstLevel].size() <= m)
	    break;
	createLStep(firstLevel, m + 1, Ref, blaSize, epsilon, &(b[firstLevel][m]));
	}
    }

const BLA * BLAS::lookup(int m, double z2, int iterations, int max_iterations) const 
    {
    if (m <= 0 || m >= M) 
	{
        return nullptr;
        }

    const BLA *B = nullptr;
    int ix = (m - 1) >> firstLevel;
    for (int level = firstLevel; level < L; ++level) 
	{
	if (AbortRequested())
	    return nullptr;

        int ixm = (ix << level) + 1;
	double z2m = b[level][ix].r2;
        if (m == ixm && z2 < z2m) 
	    {
	    int l = b[level][ix].l;
            if (iterations + l <= max_iterations) 
		{
                B = &(b[level][ix]);
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

BLA	BLA::BLALStep(double r2, Complex A, Complex B, int l) 
    {
    this->Ax = A.x;
    this->Ay = A.y;
    this->Bx = B.x;
    this->By = B.y;
    this->r2 = r2;
    this->l = l;
    return *this;
    }

//////////////////////////////////////////////////////////////////////
// write to output in development environment
//////////////////////////////////////////////////////////////////////

// Logging
//extern HWND hLogBox;
void	BLAS::debugPrint(const std::string& msg)
    {
#ifdef _DEBUG
    OutputDebugStringA(msg.c_str());
#endif
    }
