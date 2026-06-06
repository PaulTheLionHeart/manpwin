/*
    PixelTemplate.h - Template for handling pixel functions for all arithmetic types.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <functional>
#include "Complex.h"
#include "Potential.h"
#include "BigDouble.h"
#include "DecompTemplate.h"

#pragma once

template<typename T>
inline double ToDouble(const T& x)
    {
    return (double)x;
    }

inline double ToDouble(double x) { return x; }
inline double ToDouble(const dd_real& x) { return to_double(x); }
inline double ToDouble(const qd_real& x) { return to_double(x); }
inline double ToDouble(const BigDouble& x) { return x.BigDoubleToDouble(); }

inline double LogT(double x) { return log(x); }
inline double LogT(const dd_real& x) { return to_double(log(x)); }
inline double LogT(const qd_real& x) { return to_double(log(x)); }
inline double LogT(const BigDouble& x)
    {
    BigDouble result;
    CBigTrig trig;

    trig.log_bf(&result, x);
    return result.BigDoubleToDouble();
    }

/**************************************************************************
	Filter
**************************************************************************/

template<typename TComplex, typename TReal>
long DoFilterT(long iteration, TComplex& z, int method, int hooper, long threshold, int colours, int decomp,
	int biomorph, double rqlim, int special, bool logval, BYTE* logtable, int colors, CTrueCol *TrueCol, double* potparam)
    {
    double magnitude = 0.0;
    CPotential Pot;

    if (colours == 256 && decomp > 0)
	{
	iteration = DecompositionT<TComplex, TReal>(z.x, z.y, decomp, colours);
	}
    else if (logval)
	{
	iteration = (BYTE)(*(logtable + (iteration % MAXTHRESHOLD)));
	}
    else if (biomorph >= 0)
	{
	double rqlim2 = sqrt(rqlim);
	if (fabs(z.x) < rqlim2 || fabs(z.y) < rqlim2)
	    iteration = biomorph;
	}
    else
	{
	switch (method)
	    {
	    case EPSCROSS:
		if (hooper == 1)
		    iteration = special;
		else if (hooper == 2)
		    iteration = (special << 1);
		break;

	    case REAL:
		iteration += (long)ToDouble(z.x) + 7;
		break;

	    case IMAG:
		iteration += (long)ToDouble(z.y) + 7;
		break;

	    case MULT:
		if (z.y != 0.0)
		    iteration = (long)((double)iteration * ToDouble(z.x / z.y));
		break;

	    case SUM:
		iteration += (long)ToDouble(z.x + z.y);
		break;

	    case ATAN:
		iteration = (long)(fabs(atan2(ToDouble(z.y), ToDouble(z.x))) * 180.0 / PI);
		break;

	    case POTENTIAL:
		magnitude = z.CSumSqr();
		iteration = Pot.potential(
		    magnitude,
		    iteration,
		    threshold,
		    TrueCol,
		    colors,
		    potparam
		);
		break;

	    case PERT1:
		if (iteration != threshold)
		    iteration = (int)((iteration -
			log2(log2((double)z.CSumSqr()))) * 5) % 256;

	    case PERT2:
		if (iteration != threshold)
		    iteration = (int)(iteration -
		    (log(0.5 * (double)z.CSumSqr()) -
			log(0.5 * log(256))) / log(2)) % 256;
		break;
	    }

	if (iteration < 0)
	    iteration = 0;

	if (iteration > threshold && decomp <= threshold)
	    iteration = threshold;
	}

    return iteration;
    }

/**************************************************************************
	Get Float Iteration per pixel
**************************************************************************/

template<typename TComplex>
double ComputeFloatIterationT(double FloatIteration, double error, TComplex& z, TComplex& OldZ, TComplex& OlderZ, int type, int subtype, WORD* degree, double rqlim, long threshold)
    {
    double log_zn, nu, t;
    int SlopeDegree, BailoutType;

    if (FloatIteration >= threshold)
	return FloatIteration;

    TComplex a, b, root = { 1.0, 0.0 };

    if (type == TIERAZON)
	{
	SlopeDegree = (TierazonSpecific[subtype].SlopeDegree == -1) ? *degree : TierazonSpecific[subtype].SlopeDegree;
	BailoutType = TierazonSpecific[subtype].BailoutType;
	}
    else if (type == MANDELDERIVATIVES)
	{
	SlopeDegree = (MandelDerivSpecific[subtype].SlopeDegree == -1) ? *degree : MandelDerivSpecific[subtype].SlopeDegree;
	BailoutType = MandelDerivSpecific[subtype].BailoutType;
	}
    else
	{
	SlopeDegree = (fractalspecific[type].SlopeDegree == -1) ? *degree : fractalspecific[type].SlopeDegree;
	BailoutType = fractalspecific[type].BailoutType;
	}

    switch (BailoutType)
	{
	case ESCAPING:
	    log_zn = ToDouble(LogT(z.x * z.x + z.y * z.y) / SlopeDegree);
	    nu = LogT(log_zn / LogT(SlopeDegree)) / LogT(SlopeDegree);
	    return FloatIteration + 1 - nu;

	case ESCAPING1:
	    log_zn = ToDouble(LogT(sqr(rqlim)) - LogT(OldZ.x * OldZ.x + OldZ.y * OldZ.y));
	    t = ToDouble(LogT(z.x * z.x + z.y * z.y) - LogT(OldZ.x * OldZ.x + OldZ.y * OldZ.y));
	    nu = log_zn / t;
	    return FloatIteration + nu;

	case ESCAPING2:
	    log_zn = ToDouble(LogT(z.x * z.x + z.y * z.y) / LogT(sqr(rqlim)));
	    t = ToDouble(LogT(z.x * z.x + z.y * z.y) / LogT(OldZ.x * OldZ.x + OldZ.y * OldZ.y));
	    nu = log_zn / t;
	    return FloatIteration + 1 - nu;

	case CONVERGING:
	    a = OldZ - OlderZ;
	    b = z - OldZ;
	    log_zn = ToDouble(LogT(error) - LogT(a.x * a.x + a.y * a.y));
	    t = ToDouble(LogT(b.x * b.x + b.y * b.y) - LogT(a.x * a.x + a.y * a.y));
	    nu = log_zn / t;
	    return FloatIteration + nu;

	case CONVERGING1:
	    a = OldZ - OlderZ;
	    b = z - OldZ;
	    log_zn = ToDouble(LogT(error) / LogT(b.x * b.x + b.y * b.y));
	    t = ToDouble(LogT(b.x * b.x + b.y * b.y) / LogT(a.x * a.x + a.y * a.y));
	    nu = log_zn / t;
	    return FloatIteration + nu;

	case CONVERGINGMAG:
	    a = OldZ - root;
	    b = z - root;
	    log_zn = ToDouble(LogT(error) - LogT(a.x * a.x + a.y * a.y));
	    t = ToDouble(LogT(b.x * b.x + b.y * b.y) - LogT(a.x * a.x + a.y * a.y));
	    nu = log_zn / t;
	    return FloatIteration + nu;

	case CONVERGINGMAG1:
	    a = OldZ - root;
	    b = z - root;
	    log_zn = ToDouble(LogT(error) / LogT(b.x * b.x + b.y * b.y));
	    t = ToDouble(LogT(b.x * b.x + b.y * b.y) - LogT(a.x * a.x + a.y * a.y));
	    nu = log_zn / t;
	    return FloatIteration + nu;

	default:
	    log_zn = ToDouble(LogT(z.x * z.x + z.y * z.y) / SlopeDegree);
	    nu = ToDouble(log(log_zn / LogT(SlopeDegree)) / LogT(SlopeDegree));
	    return FloatIteration + 1 - nu;
	}
    }

/**************************************************************************
     Invert fractal
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex Invertz2T(const TComplex& Cmplx1, TReal f_radius, TReal f_xcenter, TReal f_ycenter)
    {
    TComplex temp;
    TReal tempsqrx;

    temp.x = Cmplx1.x - f_xcenter;
    temp.y = Cmplx1.y - f_ycenter;

    tempsqrx = sqr(temp.x) + sqr(temp.y);

    if (fabs(tempsqrx) > (TReal)FLT_MIN)
	tempsqrx = f_radius / tempsqrx;
    else
	tempsqrx = (TReal)FLT_MAX;

    temp.x *= tempsqrx;
    temp.y *= tempsqrx;

    temp.x += f_xcenter;
    temp.y += f_ycenter;

    return temp;
    }



template<typename T>
struct MathOps;

template<>
struct MathOps<double>
    {
    static void MulInt(double& out, double gap, int val)
	{
	out = gap * val;
	}
    };

template<>
struct MathOps<BigDouble>
    {
    static void MulInt(BigDouble& out, const BigDouble& gap, int val)
	{
	gap.MulInt(out, val);
	}
    };

template<typename T, typename GapType>
void ComputeRotatedCoordsT(int row, int col, int& oldrow, int& oldcol, int RotationAngle, T& cx, T& cy, GapType& xgap, GapType& ygap, T& yymax, T& hor)
    {
    if (row != oldrow)
	{
	T temp;

	switch (RotationAngle)
	    {
	    case NORMAL:
		MathOps<T>::MulInt(temp, ygap, row);
		cy = yymax - temp;
		break;

	    case 90:
		MathOps<T>::MulInt(temp, xgap, row);
		cx = yymax - temp;
		break;

	    case 180:
		MathOps<T>::MulInt(temp, ygap, row);
		cy = -(yymax - temp);
		break;

	    case 270:
		MathOps<T>::MulInt(temp, xgap, row);
		cx = -(yymax - temp);
		break;
	    }

	oldrow = row;
	}

    if (col != oldcol)
	{
	T temp;

	switch (RotationAngle)
	    {
	    case NORMAL:
		MathOps<T>::MulInt(temp, xgap, col);
		cx = temp + hor;
		break;

	    case 90:
		MathOps<T>::MulInt(temp, ygap, col);
		cy = temp + hor;
		break;

	    case 180:
		MathOps<T>::MulInt(temp, xgap, col);
		cx = -(temp + hor);
		break;

	    case 270:
		MathOps<T>::MulInt(temp, ygap, col);
		cy = -(temp + hor);
		break;
	    }

	oldcol = col;
	}
    }


