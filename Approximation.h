#include <windows.h>
#include <vector>
#include <cmath>
#include <math.h>
#include <sstream>
//#include <complex>
#include <array>
#include "complex.h"
#include "FloatExp.h"
#include "ExpComplex.h"

extern  void debugPrint(const std::string& msg);

using Real = long double;
//using Complex = std::complex<Real>;

#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define MIN(a,b)	(((a) < (b)) ? (a) : (b))

class BLA {
public:
    double	r2;
    double	Ax;
    double	Ay;
    double	Bx;
    double	By;
    int		l;
    BLA	BLALStep(double r2, Complex A, Complex B, int l);

    private:

    };

class BLAExp {
public:
    floatexp	r2;
    floatexp	Ax;
    floatexp	Ay;
    floatexp	Bx;
    floatexp	By;
    int		l;
    BLAExp	BLALStepExp(floatexp r2, ExpComplex A, ExpComplex B, int l);

private:

    };

class BLAS {
    public:
	int	M;
	int	L;
	int	LM1;	    // Level -1
	bool	first;	    // used to indicate what form of getL() to use
	std::vector<double> data;
	std::vector<std::vector<BLA>> b;
	std::vector<std::vector<BLAExp>> bExp;
	bool isValid;
//	BLAS(Fractal fractal);
	void init(int M, Complex *Ref, double blaSize, int power, int subtype);
	void initExp(int M, ExpComplex *Ref, floatexp blaSize, int power, int subtype);
	BLA *lookup(int m, double z2, int iterations, int max_iterations);
	BLAExp *lookupExp(int m, floatexp z2, int iterations, int max_iterations);
	void clear();	// Add the clear method to free BLA memory
//	floatexp hypotExp(floatexp, floatexp);
	void debugPrint(const std::string& msg);

private:
//	Fractal fractal;

	std::vector<int> elementsPerLevel;

	int firstLevel = 0;
	bool returnL1;

	long finalTotal;

	void mergeTwoBlas(BLA *x, BLA *y, double blaSize, BLA *TwoBlas);
	void mergeTwoBlasExp(BLAExp *x, BLAExp *y, floatexp blaSize, BLAExp *TwoBlas);
	void createLStep(int level, int m, Complex *Ref, double blaSize, double epsilon, /*ReferenceDecompressor referenceDecompressor*/BLA *LStep);
	void createLStepExp(int level, int m, ExpComplex *Ref, floatexp blaSize, floatexp epsilon, /*ReferenceDecompressor referenceDecompressor*/BLAExp *LStep);
	void createOneStep(Complex *Ref, int m, double epsilon, /*ReferenceDecompressor referenceDecompressor*/BLA *OneStep);
	void createOneStepExp(ExpComplex *Ref, int m, floatexp epsilon, /*, ReferenceDecompressor referenceDecompressor*/BLAExp *OneStep);
	void init(Complex *Ref, double blaSize, double epsilon/*, JProgressBar progress*/, long divisor);
	void initExp(ExpComplex *Ref, floatexp blaSize, floatexp epsilon/*, JProgressBar progress*/, long divisor);
	void mergeOneStep(int m, int elementsSrc, int src, int dest, double blaSize);
	void mergeOneStepExp(int m, int elementsSrc, int src, int dest, floatexp blaSize);
	void merge(double blaSize/*, JProgressBar progress*/, long divisor);
	void mergeExp(floatexp blaSize, /*JProgressBar progress, */long divisor);
//	floatexp sqrtExp(floatexp a);
	int power;
	int subtype;
	long done;
//	long old_chunk;
//	void setProgress(JProgressBar progress, long val, long divisor);
//	void init(int M, DoubleReference Ref, double blaSize, JProgressBar progress);
//	void init(int M, DeepReference Ref, MantExp blaSize, JProgressBar progress);
//	long getTotalElements();
//	BLA lookupBackwards(int m, double z2, int iterations, int max_iterations);
//	BLADeep lookup(int m, MantExp z2, int iterations, int max_iterations);
//	public BLADeep lookupBackwards(int m, MantExp z2, int iterations, int max_iterations);
    };


