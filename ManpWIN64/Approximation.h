#include <windows.h>
#include <vector>
#include <cmath>
#include <math.h>
#include <sstream>
#include <array>
#include "complex.h"
#include "FloatExp.h"
#include "ExpComplex.h"
#include "Manpwin.h"

#pragma once

extern  void debugPrint(const std::string& msg);

using Real = long double;

#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#define MIN(a,b)	(((a) < (b)) ? (a) : (b))
#define	MAXPOLY		10		// only because we are limited by param[]

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
	void init(int M, std::vector<Complex> &Ref, double blaSize, int power, int subtype, long MaxIterIn, double param[]);
	void initExp(int M, std::vector<ExpComplex> &Ref, floatexp blaSize, int power, int subtype, long MaxIterIn, double param[]);
	const BLA *lookup(int m, double z2, int iterations, int max_iterations) const;
	const BLAExp *lookupExp(int m, floatexp z2, int iterations, int max_iterations) const;
	void clear();	// Add the clear method to free BLA memory
	void debugPrint(const std::string& msg);
	std::vector<double> params;

private:
//	Fractal fractal;

	std::vector<int> elementsPerLevel;

	int firstLevel = 0;
	bool returnL1;

	long finalTotal;
	long MaxIter;
	void mergeTwoBlas(BLA *x, BLA *y, double blaSize, BLA *TwoBlas);
	void mergeTwoBlasExp(BLAExp *x, BLAExp *y, floatexp blaSize, BLAExp *TwoBlas);
	void createLStep(int level, int m, std::vector<Complex> &Ref, double blaSize, double epsilon, BLA *LStep);
	void createLStepExp(int level, int m, std::vector<ExpComplex> &Ref, floatexp blaSize, floatexp epsilon, BLAExp *LStep);
	void createOneStep(std::vector<Complex> &Ref, int m, double epsilon, BLA *OneStep);
	void createOneStepExp(std::vector<ExpComplex> &Ref, int m, floatexp epsilon, BLAExp *OneStep);
	void init(std::vector<Complex> &Ref, double blaSize, double epsilon, long divisor);
	void initExp(std::vector<ExpComplex> &Ref, floatexp blaSize, floatexp epsilon, long divisor);
	void mergeOneStep(int m, int elementsSrc, int src, int dest, double blaSize);
	void mergeOneStepExp(int m, int elementsSrc, int src, int dest, floatexp blaSize);
	void merge(double blaSize, long divisor);
	void mergeExp(floatexp blaSize, long divisor);
//	floatexp sqrtExp(floatexp a);
	int power;
	int subtype;
	long done;
    };


