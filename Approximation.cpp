#include "approximation.h"

extern  long threshold;

void BLAS::mergeTwoBlas(BLA *x, BLA *y, double blaSize, BLA* TwoBlas) {

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

    void BLAS::createLStep(int level, int m, Complex *Ref, double blaSize, double epsilon, /*ReferenceDecompressor referenceDecompressor*/BLA* LStep) {

	if (level == 0) {
            createOneStep(Ref, m, epsilon, /*referenceDecompressor*/LStep);
            return;
	    }

	int m2 = m << 1;
	int mx = m2 - 1;
	int my = m2;
	int levelm1 = level - 1;
	if (my <= elementsPerLevel[levelm1]) {
            BLA x, y;
	    createLStep(levelm1, mx, Ref, blaSize, epsilon, /*referenceDecompressor*/&x);
            createLStep(levelm1, my, Ref, blaSize, epsilon, /*referenceDecompressor*/&y);
	    mergeTwoBlas(&x, &y, blaSize, LStep);
            return;
	    }
	else {
            createLStep(levelm1, mx, Ref, blaSize, epsilon, /*referenceDecompressor*/LStep);
	    return; 
	    }
	}

void BLAS::createOneStep(Complex *Ref, int m, double epsilon, /*ReferenceDecompressor referenceDecompressor*/BLA* OneStep) {
//	Complex Z = fractal.getArrayValue(referenceDecompressor, Ref, m);
//	Complex A = fractal.getBlaA(Z);
    Complex A, B;
    double mA, r, r2;

    if (m >= threshold)
	m = threshold - 1;	// Avoid out of bounds access
    Complex Z = Ref[m];
    if (subtype == 0)		// must be Mandelbrot
	{
	A = Z + Z;
	B = Complex(1.0, 0.0);
	mA = hypot(A.x, A.y);
	r = mA * epsilon;
	r2 = r * r;
	}
    else
	{
	A = Z.CPolynomial(power - 1) * (double)power;
	B = Complex(1.0, 0.0);
	r = epsilon * 2 * Z.CFabs() / (power - 1);
	r2 = r * r;
	}
    OneStep->BLALStep(r2, A, B, 1);
    }

void BLAS::mergeOneStep(int m, int elementsSrc, int src, int dest, double blaSize) {
        int mx = m << 1;
        int my = mx + 1;
        if (my < elementsSrc) {
            BLA x = b[src][mx];
            BLA y = b[src][my];

            mergeTwoBlas(&x, &y, blaSize, &(b[dest][m]));
            }
        else {
            b[dest][m] = b[src][mx];
            }
        }

// Add the clear method to free memory  
void BLAS::clear() {
    data.clear();
    b.clear();
    bExp.clear();
    isValid = false;
    }

void BLAS::merge(double blaSize/*, JProgressBar progress*/, long divisor) {

//        boolean useThreadsForBla = TaskRender.USE_THREADS_FOR_BLA;
        bool useThreadsForBla = false;

        int elementsDst = 0;
        int src = firstLevel;
        size_t maxLevel = elementsPerLevel.size() - 1;
        for (int elementsSrc = elementsPerLevel[src]; src < maxLevel && elementsSrc > 1; src++) {

            int srcp1 = src + 1;
            elementsDst = elementsPerLevel[srcp1];
            int dst = srcp1;

            int elementsSrcFinal = elementsSrc;
            int srcFinal = src;
            int destFinal = dst;
            {
               for(int m = 0; m < elementsDst; m++) {
                   mergeOneStep(m, elementsSrcFinal, srcFinal, destFinal, blaSize);
/*
                   if (progress != null) {
                       done++;
                       long val = done;

                       if(val % 1000 == 0) {
                           setProgress(progress, val, divisor);
                       }
                   }
                }
*/
            }

            elementsSrc = elementsDst;
        }
/*
        if (progress != null) {
            long val = done;
            setProgress(progress, val, divisor);
*/
        }
    }

void BLAS::init(int M, Complex *Ref, double blaSize, int powerIn, int subtypeIn) {
    power = powerIn;
    subtype = subtypeIn;
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

    if(m <= 0){
	return;
    }

    std::vector<int> elemPerLevel;

    long total = 1;
    int count = 1;

    for (; m > 1; m = (m + 1) >> 1) {
	elemPerLevel.push_back(m);
	total += m;
	count++;
    }
    elemPerLevel.push_back(m);

    elementsPerLevel.assign(elemPerLevel.begin(), elemPerLevel.end());

    finalTotal = total;
    long removedTotal = 0;

    if(firstLevel > 0) {
	for(int i = 0; i < firstLevel && i < elementsPerLevel.size(); i++) {
	    removedTotal += elementsPerLevel[i];
	}
	finalTotal -= removedTotal;
    }

    returnL1 = firstLevel == 0;

//    long divisor = finalTotal > Constants.MAX_PROGRESS_VALUE ? finalTotal / Constants.PROGRESS_SCALE : 1;
    long divisor = finalTotal > 0x7fffffff ? finalTotal / 10000 : 1;
/*
    if(progress != null) {
        progress.setMaximum((int)(finalTotal > Constants.MAX_PROGRESS_VALUE ? Constants.PROGRESS_SCALE : finalTotal));
    }
*/


    L = count;
    b.resize(count);
    LM1 = L - 1;

    if(firstLevel >= elementsPerLevel.size()){
	return;
    }

    for(int l = firstLevel; l < b.size(); l++) {
	b[l].resize(elementsPerLevel[l]);
    }

    init(Ref, blaSize, precision/*, progress*/, divisor);

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

void BLAS::init(Complex *Ref, double blaSize, double epsilon/*, JProgressBar progress*/, long divisor) {

    	int elements = elementsPerLevel[firstLevel];
	// lots of thread stuff deleted for now

        done = 0;
	for (int m = 0; m < elements; m++) {
            if (b[firstLevel].size() <= m)
                break;
	    createLStep(firstLevel, m + 1, Ref, blaSize, epsilon/*, referenceDecompressor*/, &(b[firstLevel][m]));

	    /*
            if (progress != null) {
		done++;
		long val = done;

		if (val % 1000 == 0) {
		    setProgress(progress, val, divisor);
		}
            */

//	if (progress != null) {
//	    long val = done;
//	    setProgress(progress, val, divisor);
//	    }
	}
    }
/*
    private void setProgress(JProgressBar progress, long val, long divisor) {
	int progres_val = (int)(val / divisor);
	progress.setValue(progres_val);
	progress.setString(BLA_CALCULATION_STR + " " + String.format("%3d", (int)((double)(progres_val) / progress.getMaximum() * 100)) + "%");
	}
*/
BLA * BLAS::lookup(int m, double z2, int iterations, int max_iterations) {
    if (m <= 0 || m >= M) {
        return nullptr;
        }

    BLA *B = nullptr;
    int ix = (m - 1) >> firstLevel;
    for (int level = firstLevel; level < L; ++level) {
        int ixm = (ix << level) + 1;
	double z2m = b[level][ix].r2;
        if (m == ixm && z2 < z2m) {
	    int l = b[level][ix].l;
            if (iterations + l <= max_iterations) {
                B = &(b[level][ix]);
                }
            }
        else {
            break;
            }
        ix = ix >> 1;
        }
    return B;
    }

BLA	BLA::BLALStep(double r2, Complex A, Complex B, int l) {
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
    OutputDebugStringA(msg.c_str());
    //    if (hLogBox) 
    //	{
    //	int length = GetWindowTextLengthA(hLogBox);
    //	SendMessageA(hLogBox, EM_SETSEL, (WPARAM)length, (LPARAM)length);
    //	SendMessageA(hLogBox, EM_REPLACESEL, FALSE, (LPARAM)msg.c_str());
    //	}
    }
