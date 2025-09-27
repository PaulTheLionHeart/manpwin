#include "approximation.h"

extern  long threshold;
/*
floatexp BLAS::hypotExp(floatexp x, floatexp y) {
    floatexp t = x*x + y*y;
    return sqrtExp(t);
    }

floatexp BLAS::sqrtExp(floatexp a) {
    //double dexp = exp * 0.5;
    //return new MantExp(Math.sqrt(mantissa) * Math.pow(2, dexp - (int)dexp), (int)dexp);
    floatexp ret;
    boolean isOdd = (a.exp & 1) != 0;
    double  temp;
    ret.exp = (isOdd) ? (a.exp - 1) / 2 : a.exp / 2;
    temp = (isOdd) ? 2.0 * a.val : a.val;
    ret.val = sqrt(temp);
    return ret;
    }
*/

void BLAS::mergeTwoBlasExp(BLAExp *x, BLAExp *y, floatexp blaSize, BLAExp* TwoBlas) {

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

void BLAS::createLStepExp(int level, int m, ExpComplex *Ref, floatexp blaSize, floatexp epsilon, /*ReferenceDecompressor referenceDecompressor*/BLAExp* LStep) {

    if (level == 0) {
	createOneStepExp(Ref, m, epsilon, /*referenceDecompressor*/LStep);
	return;
	}

    int m2 = m << 1;
    int mx = m2 - 1;
    int my = m2;
    int levelm1 = level - 1;
    if (my <= elementsPerLevel[levelm1]) {
	BLAExp x, y;
	createLStepExp(levelm1, mx, Ref, blaSize, epsilon, /*referenceDecompressor*/&x);
	createLStepExp(levelm1, my, Ref, blaSize, epsilon, /*referenceDecompressor*/&y);
	mergeTwoBlasExp(&x, &y, blaSize, LStep);
	return;
	}
    else {
	createLStepExp(levelm1, mx, Ref, blaSize, epsilon, /*referenceDecompressor*/LStep);
	return; 
	}
    }

void BLAS::createOneStepExp(ExpComplex *Ref, int m, floatexp epsilon, /*ReferenceDecompressor referenceDecompressor*/BLAExp *OneStep) {
//    Complex Z = fractal.getArrayValue(referenceDecompressor, Ref, m);
//    Complex A = fractal.getBlaA(Z);
    ExpComplex A, B;
    floatexp mA, r, r2;

    if (m >= threshold)
	m = threshold - 1; // Avoid out of bounds access
    ExpComplex Z = Ref[m];
    if (subtype == 0)		// must be Mandelbrot
	{
	A = Z + Z;
	B = Complex(1.0, 0.0);
	mA = mA.hypotExp(A.x, A.y);
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

    OneStep->BLALStepExp(r2, A, B, 1);
    }

void BLAS::mergeOneStepExp(int m, int elementsSrc, int src, int dest, floatexp blaSize) {
        int mx = m << 1;
        int my = mx + 1;
        if (my < elementsSrc) {
            BLAExp x = bExp[src][mx];
            BLAExp y = bExp[src][my];

            mergeTwoBlasExp(&x, &y, blaSize, &(bExp[dest][m]));
            }
        else {
            bExp[dest][m] = bExp[src][mx];
            }
        }

void BLAS::mergeExp(floatexp blaSize/*, JProgressBar progress*/, long divisor) {

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
                   mergeOneStepExp(m, elementsSrcFinal, srcFinal, destFinal, blaSize);
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

void BLAS::initExp(int M, ExpComplex *Ref, floatexp blaSize, int powerIn, int subtypeIn) {
	power = powerIn;
	subtype = subtypeIn;
	int BLA_BITS = 23;
	int BLA_STARTING_LEVEL = 4;
	floatexp precision = 1.0 / ((double)(1L << BLA_BITS));
        firstLevel = BLA_STARTING_LEVEL - 1;
        isValid = false;
//        size_t  BLASize, BLASSize;    // a few little tests
//        BLASize = sizeof(BLA);
//        BLASSize = sizeof(BLAS);
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


//        long divisor = finalTotal > Constants.MAX_PROGRESS_VALUE ? finalTotal / Constants.PROGRESS_SCALE : 1;
        long divisor = finalTotal > 0x7fffffff ? finalTotal / 10000 : 1;
/*
        if(progress != null) {
            progress.setMaximum((int)(finalTotal > Constants.MAX_PROGRESS_VALUE ? Constants.PROGRESS_SCALE : finalTotal));
        }
*/


        L = count;
        bExp.resize(count);
        LM1 = L - 1;

        if(firstLevel >= elementsPerLevel.size()){
            return;
        }

        for(int l = firstLevel; l < bExp.size(); l++) {
	    bExp[l].resize(elementsPerLevel[l]);
        }

        initExp(Ref, blaSize, precision/*, progress*/, divisor);

        mergeExp(blaSize/*, progress*/, divisor);

        size_t  SizeofbExp = sizeof(bExp) * bExp.size() * bExp[0].size();
/*
        for (int i = firstLevel; i < L; ++i) {

            std::ostringstream oss;
            size_t ItemCount = bExp[i].size();
            oss << "Level: " << i << " with " << ItemCount << " items ***************\n";
//            oss << "l = " << bExp[i][0].getL(GetBLAL) << "\tr = " << sqrt(bExp[i][0].r2) << " \n";
            debugPrint(oss.str());
            if (ItemCount <= 10) {
                for (int j = 0; j < ItemCount; j++) {
                    std::ostringstream oss;
                    oss << "bExp[" << i << "][" << j << "]: AxV=" << bExp[i][j].Ax.val << " AxE=" << bExp[i][j].Ax.exp << ", AyV=" << bExp[i][j].Ay.val << ", AyE=" << bExp[i][j].Ay.exp 
		    << ", BxV=" << bExp[i][j].Bx.val << ", BxE=" << bExp[i][j].Bx.exp << ", ByV=" << bExp[i][j].By.val << ", ByE=" << bExp[i][j].By.exp << ", r2V=" << bExp[i][j].r2.val 
		    << ", r2E=" << bExp[i][j].r2.exp << ", l=" << bExp[i][j].l << "\n";
                    debugPrint(oss.str());
                    }
                }
            else {
                for (int j = 0; j < 5; j++) {
                    std::ostringstream oss;
                    oss << "bExp[" << i << "][" << j << "]: AxV=" << bExp[i][j].Ax.val << " AxE=" << bExp[i][j].Ax.exp << ", AyV=" << bExp[i][j].Ay.val << ", AyE=" << bExp[i][j].Ay.exp 
		    << ", BxV=" << bExp[i][j].Bx.val << ", BxE=" << bExp[i][j].Bx.exp << ", ByV=" << bExp[i][j].By.val << ", ByE=" << bExp[i][j].By.exp << ", r2V=" << bExp[i][j].r2.val 
		    << ", r2E=" << bExp[i][j].r2.exp << ", l=" << bExp[i][j].l << "\n";
//                    oss << "bExp[" << i << "][" << j << "]: Ax=" << bExp[i][j].Ax.todouble() << ", Ay=" << bExp[i][j].Ay.todouble() << ", Bx=" << bExp[i][j].Bx.todouble() << ", By=" << bExp[i][j].By.todouble() 
//		    << ", r2=" << bExp[i][j].r2.todouble() << ", l=" << bExp[i][j].l << "\n";
                    debugPrint(oss.str());
                    }
                for (int j = 0; j < 5; j++) {
                    size_t ItemPtr = j + ItemCount - 5;
                    std::ostringstream oss;
                    oss << "bExp[" << i << "][" << ItemPtr << "]: AxV=" << bExp[i][ItemPtr].Ax.val << " AxE=" << bExp[i][ItemPtr].Ax.exp << ", AyV=" << bExp[i][ItemPtr].Ay.val << ", AyE=" << bExp[i][ItemPtr].Ay.exp
		    << ", BxV=" << bExp[i][ItemPtr].Bx.val << ", BxE=" << bExp[i][ItemPtr].Bx.exp << ", ByV=" << bExp[i][ItemPtr].By.val << ", ByE=" << bExp[i][ItemPtr].By.exp << ", r2V=" << bExp[i][ItemPtr].r2.val
		    << ", r2E=" << bExp[i][ItemPtr].r2.exp << ", l=" << bExp[i][ItemPtr].l << "\n";
//                    oss << "bExp[" << i << "][" << ItemPtr << "]: Ax=" << bExp[i][ItemPtr].Ax.todouble() << ", Ay=" << bExp[i][ItemPtr].Ay.todouble() << ", Bx=" << bExp[i][ItemPtr].Bx.todouble() 
//		    << ", By=" << bExp[i][ItemPtr].By.todouble() << ", r2=" << bExp[i][ItemPtr].r2.todouble() << ", l=" << bExp[i][ItemPtr].l << "\n";
                    debugPrint(oss.str());
                    }

                }
	    }
*/
/*
		{
		std::ostringstream oss;
		oss << "l = " << bExp[i][0].l << "\tr = " << sqrt(bExp[i][0].r2.todouble()) << " \n";
		debugPrint(oss.str());
		}
            }
*/
        isValid = true;
    }

void BLAS::initExp(ExpComplex *Ref, floatexp blaSize, floatexp epsilon/*, JProgressBar progress*/, long divisor) {

    	int elements = elementsPerLevel[firstLevel];
	// lots of thread stuff deleted for now

        done = 0;
	for (int m = 0; m < elements; m++) {
            if (bExp[firstLevel].size() <= m)
                break;
	    createLStepExp(firstLevel, m + 1, Ref, blaSize, epsilon/*, referenceDecompressor*/, &(bExp[firstLevel][m]));

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
BLAExp * BLAS::lookupExp(int m, floatexp z2, int iterations, int max_iterations) {
    if (m <= 0 || m >= M) {
        return nullptr;
        }

    BLAExp *B = nullptr;
    int ix = (m - 1) >> firstLevel;
    for (int level = firstLevel; level < L; ++level) {
        int ixm = (ix << level) + 1;
	floatexp z2m = bExp[level][ix].r2;
        if (m == ixm && z2 < z2m) {
	    int l = bExp[level][ix].l;
            if (iterations + l <= max_iterations) {
                B = &(bExp[level][ix]);
                }
            }
        else {
            break;
            }
        ix = ix >> 1;
        }
    return B;
    }

BLAExp	BLAExp::BLALStepExp(floatexp r2, ExpComplex A, ExpComplex B, int l) {
    this->Ax = A.x;
    this->Ay = A.y;
    this->Bx = B.x;
    this->By = B.y;
    this->r2 = r2;
    this->l = l;
    return *this;
    }

