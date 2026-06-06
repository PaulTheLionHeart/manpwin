/*
    DerivSlopeTemplate.H - Template for handling derivative slope for all arithmetic types.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <functional>
#include "Complex.h"

#pragma once

/**************************************************************************
    Local high-precision abs() for derivative slope only
***************************************************************************/

inline double CFabsSlope(const Complex& z)
    {
    return sqrt(z.x * z.x + z.y * z.y);
    }

inline dd_real CFabsSlope(const DDComplex& z)
    {
    return sqrt((z.x * z.x) + (z.y * z.y));
    }

inline qd_real CFabsSlope(const QDComplex& z)
    {
    return sqrt((z.x * z.x) + (z.y * z.y));
    }

inline BigDouble CFabsSlope(const BigComplex& z)
    {
    BigDouble temp;

    mpfr_mul(temp.x, z.x.x, z.x.x, MPFR_RNDN);

    BigDouble imag2;
    mpfr_mul(imag2.x, z.y.x, z.y.x, MPFR_RNDN);

    mpfr_add(temp.x, temp.x, imag2.x, MPFR_RNDN);

    BigDouble out;
    mpfr_sqrt(out.x, temp.x, MPFR_RNDN);

    return out;
    }


/***********************************************************************
	Let's find the regions of special colour
***********************************************************************/

template <typename TComplex>
bool IsArtMatrixSpecialPixelT(TComplex C, Complex j, BYTE juliaflag, double* param, long threshold)
    {
    TComplex z, q;
    TComplex a, b, a2 = 0, aa3, t2, t3, temp, v = 0;
    char variety;

    // --- variety ---
    switch ((int)param[4])
	{
        case 0: variety = 'B'; break;
        case 1: variety = 'C'; break;
        case 2: variety = 'F'; break;
        case 3: variety = 'K'; break;
        default: variety = 'B'; break;
	}

    // --- init ---
    if (juliaflag)
	{
        q = j;
        z = C;
	}
    else
	{
        q = C;
        z = 0.0;
	}

    // --- init cubic ---
    if (variety == 'B')
	{
        t3 = q * 3.0;
        t2 = q.CSqr();
        a = (t2 + 1.0) / t3;
        temp = a.CCube() * 2.0;
        b = (t2 - 2.0) / t3 + temp;
	}
    else if (variety == 'C' || variety == 'F')
	{
        a = q;
        temp = q.CCube();

        if (variety == 'C')
            b = temp + temp + q;
        else
	    {
            b = (temp - q) * 2.0;
            a2 = a + a;
	    }
	}
    else if (variety == 'K')
	{
        a = 0.0;
        v = 0.0;
        b = q;
	}

    aa3 = a.CSqr() * 3.0;
    z = -a;

    // --- iterate ---
    for (int i = 0; i < threshold; i++)
	{
        if (variety == 'K')
	    {
            z = z.CCube() + b;
	    }
        else
	    {
            temp = z.CCube() + b;
            z = temp - aa3 * z;
	    }

        if (z.CSumSqr() > 100.0)
            return false;

        if (variety == 'F')
	    {
            if (q.CSumSqr() < 0.111111)
                return true;

            v = z + a2;
	    }
        else if (variety == 'K')
	    {
            v = z - v;
	    }
        else
	    {
            v = z - a;
	    }

        if (v.CSumSqr() <= 0.000001)
            return true;
	}

    return false;
    }

/***********************************************************************
	Some conversion helpers
***********************************************************************/

inline double ToDouble(double x)
    {
    return x;
    }

inline double ToDouble(const dd_real& x)
    {
    return to_double(x);
    }

inline double ToDouble(const qd_real& x)
    {
    return to_double(x);
    }

inline double ToDouble(const BigDouble& x)
    {
    return x.BigDoubleToDouble();   // or whatever your BigDouble uses
    }

/***********************************************************************
	Reflection: fractal calculations double float version
***********************************************************************/
template <typename TComplex>
double	GiveReflectionT(Complex j, BYTE juliaflag, TComplex C, int &iterations, double &smoothIter, double rqlim, long threshold, Complex v,
	double* param, int subtype, char variety, WORD *degree, bool smoothing)
    {
    int		i = 0;					// iteration 
    int		k;
    TComplex	sqr, temp, Q;
    TComplex	a = 0.0, b = 0.0, aa3 = 0.0, a2 = 0.0;
    int		SlopeDegree = 2;  // default

    int		degree1, degree2;

    TComplex	Z = 0.0;				// initial value for iteration Z0
//    Complex	z = 1.0;				// initial value for iteration Z0 used in Newton fractals
    TComplex	dC = 0.0;				// derivative with respect to c 
    double	reflection = FP_ZERO; // inside 
    double	h2 = param[1];				// height factor of the incoming light
    TComplex	u;
    TComplex	t = { 1.0, 0.0 };

    *degree = (int)param[3];
    if (juliaflag)
	{
	Q = j;
	Z = C;
	}
    else
	{
	if (subtype == 3)				// Sine
	    {
	    Z = param[4];
	    Q = C;
	    }
	else if (subtype == 13)          // Art Matrix Cubic
	    {
	    TComplex temp;

	    Q = C;

	    if (variety == 'B')          // CBIN
		{
		TComplex t3 = Q * 3.0;
		TComplex t2 = Q.CSqr();
		a = (t2 + 1.0) / t3;
		temp = a.CCube() * 2.0;
		b = (t2 - 2.0) / t3 + temp;
		}
	    else if (variety == 'C' || variety == 'F')   // CCIN / CFIN
		{
		a = Q;
		temp = Q.CCube();
		if (variety == 'C')
		    b = temp + temp + Q;
		else
		    {
		    b = (temp - Q) * 2.0;
		    a2 = a + a;
		    }
		}
	    else if (variety == 'K')     // CKIN
		{
		a = 0.0;
		b = Q;
		}

	    aa3 = a.CSqr() * 3.0;
	    Z = -a;
	    }
	else
	    Q = C;
	}
    for (i = 0; i < threshold; i++)
	{
	auto real_imag = Z.x * Z.y;
	switch (subtype)
	    {
	    case 0:					// Mandelbrot
		SlopeDegree = 2;
		dC = dC * (Z + Z) + 1.0;
		sqr.x = Z.x * Z.x;
		sqr.y = Z.y * Z.y;
		real_imag = Z.x * Z.y; 
		Z.x = Q.x + sqr.x - sqr.y;
		Z.y = Q.y + real_imag + real_imag;
		break;
	    case 1:					// Cubic
		SlopeDegree = 3;
		dC = dC * Z.CSqr() * 3.0 + 1.0;
		Z = Z.CCube() + Q;
		break;
	    case 2:					// Power
		SlopeDegree = *degree;
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= Z;
		dC = temp * dC * (double)*degree + 1.0;
		Z = temp * Z + Q;
//		dC = dC * Z.CPolynomial((degree - 1)) * (double)degree + 1.0;
//		Z = Z.CPolynomial(degree) + Q;
		break;
	    case 3:					// Sine
		SlopeDegree = 2;
		dC = dC * Z.CCos() + 1.0;
		if (param[3] == 0)
		    Z = Z.CSin() * Q;
		else
		    Z = Z.CSin() + Q;
		break;
	    case 4:					// Sine + 1/c
		SlopeDegree = 2;
		dC = dC * Z.CCos() + 1.0;
		Z = Z.CSin() + t / Q;
		break;
	    case 5:					// exp
		SlopeDegree = 2;
		dC = dC * Z.CExp() + 1.0;
		Z = Z.CExp() + Q;
		break;
	    case 6:					// Power + 1/c
		SlopeDegree = *degree;
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= Z;
		dC = temp * dC * (double)*degree + 1.0;
		Z = temp * Z + t/Q;
		break;
	    case 7:
		SlopeDegree = *degree;
		degree1 = (int)param[4];
		degree2 = (int)param[5];
		dC = dC * Z.CPolynomial(*degree - 1) * param[3] + Z.CPolynomial(degree1 - 1) * param[4] + Z.CPolynomial(degree2 - 1) * param[5] + 1.0;
		Z = Z.CPolynomial(*degree) + Z.CPolynomial(degree1) + Z.CPolynomial(degree2) + Q;
		break;
	    case 8:
		SlopeDegree = 3;
		dC = dC * Z.CSqr() * 3.0 + 1.0;
		Z = Z.CCube() + (Q - 1.0) * Z + Q;
		break;
	    case 9:
		SlopeDegree = 3;
		dC = dC * Z.CSqr() * 0.75 + 2.0;
		Z = Z.CCube() / 4.0 + Z + Q;
		break;
	    case 10:					// Sin(z^n)
		SlopeDegree = *degree;
		temp = Z.CPolynomial(*degree);
		dC = dC * temp.CCos() * Z.CPolynomial(*degree - 1) * param[3] + 1.0;
		Z = temp.CSin() + Q;
		break;
	    case 11:					// Sinh
		SlopeDegree = 2;
		dC = dC * Z.CSinh() + 1.0;
		Z = Z.CSinh() + Q;
		break;
	    case 12:					// Sinh(z^n)
		SlopeDegree = *degree;
		temp = Z.CPolynomial(*degree);
		dC = dC * temp.CSinh() * Z.CPolynomial(*degree - 1) * param[3] + 1.0;
		Z = temp.CSinh() + Q;
		break;
	    case 13:    // Art Matrix Cubic
		{
		SlopeDegree = 3;
		TComplex oldZ = Z;
		TComplex dFdZ;

		if (variety == 'K')
		    {
		    dFdZ = oldZ.CSqr() * 3.0;
		    Z = oldZ.CCube() + b;
		    }
		else
		    {
		    dFdZ = oldZ.CSqr() * 3.0 - aa3;
		    Z = oldZ.CCube() - aa3 * oldZ + b;
		    }

		dC = dFdZ * dC + 1.0;
		break;
		}
	    }

	auto SumSqr = Z.CSumSqr();

	if (SumSqr > rqlim)
	    {
	    if (smoothing)
		{
		double SumSqrD = ToDouble(SumSqr);
		double mag = sqrt(SumSqrD);
		double smoothIter_local = i + 1 - log(log(mag)) / log(SlopeDegree);
		smoothIter = smoothIter_local;
		}

	    //------------------------------------------------
	    // High-precision derivative slope direction
	    //
	    // u = Z / dC
	    // u = u / |u|
	    //------------------------------------------------

	    u = Z / dC;

	    u = u / u.CFabs();

//	    auto uMag = CFabsSlope(u);
//	    u = u / uMag;

	    //------------------------------------------------
	    // Dot product remains high precision until final
	    // conversion to double.
	    //------------------------------------------------

	    auto dot_hp = u.x * v.x + u.y * v.y;

	    double dot = ToDouble(dot_hp);

	    reflection = dot + h2;
	    reflection = reflection / (1.0 + h2);
	    if (reflection < 0.0)
		reflection = 0.0;

	    iterations = i;
	    break;
	    }
	}
    if (i >= threshold)
	{
	iterations = threshold;
	smoothIter = (double)threshold;
	return 0.0;  // reflection brightness = 0
	}

    return reflection;
    }

