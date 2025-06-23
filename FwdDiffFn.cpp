/*
    FWDDIFF.CPP a module for determining the slope using forward differencing calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" API and not in MFC for portability
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/core/ThreadDraw.java#L3640
     https://github.com/hrkalona/Fractal-Zoomer/blob/master/src/fractalzoomer/main/app_settings/BumpMapSettings.java
*/

#include	"slope.h"

/**************************************************************************
    Calculate functions
**************************************************************************/

void	CSlope::DoSlopeFwdDiffFn(Complex *z, Complex *q)
    {
    Complex	sqr, temp, poly;
    Complex	t = { 1.0, 0.0 };
    double	real_imag;
    int		k;
    int		degree1, degree2;

//    PaletteShift = (int)param[4];

    *degree = (int)param[5];
    switch (subtype)
	{
	case 0:					// Mandelbrot
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;
	    break;
	case 1:					// Burning Ship
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = fabs(z->x * z->y);
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    break;

	case 2:					// Cubic
	    SlopeDegree = 3;
	    *z = z->CCube() + *q;
	    break;
	case 3:					// Power
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + *q;
	    //		Z = Z.CPolynomial(degree) + Q;
	    break;
	case 4:					// Sin
	    SlopeDegree = 2;
	    if (param[5] == 0)
		*z = z->CSin() * *q;
	    else
		*z = z->CSin() + *q;
	    break;
	case 5:					// Sin + 1/c
	    SlopeDegree = 2;
	    *z = z->CSin() + t / *q;
	    break;
	case 6:					// exp
	    SlopeDegree = 2;
	    *z = z->CExp() + *q;
	    break;
	case 7:					// Power + 1/c
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + t / *q;
	    break;
	case 8:
	    degree1 = (int)param[6];
	    degree2 = (int)param[7];
	    SlopeDegree = degree2;
	    *z = z->CPolynomial(*degree) + z->CPolynomial(degree1) + z->CPolynomial(degree2) + *q;
	    break;
	case 9:
	    SlopeDegree = 3;
	    *z = z->CCube() + (*q - 1.0) * *z + *q;
	    break;
	case 10:
	    SlopeDegree = 3;
	    *z = z->CCube() / 4.0 + *z + *q;
	    break;
	case 11:					// Sin(z^n)
	    SlopeDegree = 2;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSin() + *q;
	    break;
	case 12:					// Sinh
	    SlopeDegree = 2;
	    *z = z->CSinh() + *q;
	    break;
	case 13:					// Sinh(z^n)
	    SlopeDegree = *degree;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSinh() + *q;
	    break;
	case 14:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = q->y - real_imag - real_imag;
	    break;
	case 15:					// Art Matrix Cubic
	    SlopeDegree = 3;
	    if (variety == 'K')				// CKIN
		*z = z->CCube() + b;			// Z = Z*Z*Z + B
	    else
		{
		temp = z->CCube() + b;			// Z = Z*Z*Z + B
		*z = temp - aa3 * *z;			// Z = Z*Z*Z - AA3*Z + B
		}
	    break;
	case 16:					// Basic Newton
	    temp = *z;
	    poly = z->CPolynomial(*degree - 1);
	    *z = *z - (poly * *z - *q - 1) / (poly * *degree);
	    t2 = *z - temp;
	    break;
	}
    }

/**************************************************************************
	Calculate Big functions
**************************************************************************/

void	CSlope::DoBigSlopeFwdDiffFn(BigComplex *z, BigComplex *q, BigComplex *bBig, BigComplex *aa3Big)
    {
    BigComplex	sqr, temp, temp1, poly;
    BigComplex	unity = { 1.0, 0.0 };
    BigDouble	real_imag, t;
    int		k;
    int		degree1, degree2;

//    PaletteStart = (int)param[4];

    *degree = (int)param[5];
    switch (subtype)
	{
	case 0:					// Mandelbrot
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;
	    break;
	case 1:					// Burning Ship
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    t = z->x * z->y;
	    real_imag = t.BigAbs();
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    break;

	case 2:					// Cubic
	    SlopeDegree = 3;
	    *z = z->CCube() + *q;
	    break;
	case 3:					// Power
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + *q;
	    //		Z = Z.CPolynomial(degree) + Q;
	    break;
	case 4:					// Sin
	    SlopeDegree = 2;
	    if (NumThreads == 0)
		{
		if (param[5] == 0)
		    *z = z->CSin() * *q;
		else
		    *z = z->CSin() + *q;
		}
	    else
		{
		BigTrig.CSin(&temp, *z);
		if (param[5] == 0)
		    *z = temp * *q;
		else
		    *z = temp + *q;
		}
	    break;
	case 5:					// Sin + 1/c
	    SlopeDegree = 2;
	    if (NumThreads == 0)
		*z = z->CSin() + unity / *q;
	    else
		{
		BigTrig.CSin(&temp, *z);
		*z = temp + unity / *q;
		}
	    break;
	case 6:					// exp
	    SlopeDegree = 2;
	    if (NumThreads == 0)
		*z = z->CExp() + *q;
	    else
		{
		BigTrig.CExp(&temp, *z);
		*z = temp + *q;
		}
	    break;
	case 7:					// Power + 1/c
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + unity / *q;
	    break;
	case 8:
	    degree1 = (int)param[6];
	    degree2 = (int)param[7];
	    SlopeDegree = degree2;
	    *z = z->CPolynomial(*degree) + z->CPolynomial(degree1) + z->CPolynomial(degree2) + *q;
	    break;
	case 9:
	    SlopeDegree = 3;
	    *z = z->CCube() + (*q - 1.0) * *z + *q;
	    break;
	case 10:
	    SlopeDegree = 3;
	    *z = z->CCube() / 4.0 + *z + *q;
	    break;
	case 11:					// Sin(z^n)
	    SlopeDegree = 2;
	    temp = z->CPolynomial(*degree);
	    if (NumThreads == 0)
		*z = temp.CSin() + *q;
	    else
		{
		BigTrig.CSin(&temp1, temp);
		*z = temp1 + *q;
		}
	    break;
	case 12:					// Sinh
	    SlopeDegree = 2;
	    if (NumThreads == 0)
		*z = z->CSinh() + *q;
	    else
		{
		BigTrig.CSinh(&temp, *z);
		*z = temp + *q;
		}
	    break;
	case 13:					// Sinh(z^n)
	    SlopeDegree = *degree;
	    temp = z->CPolynomial(*degree);
	    if (NumThreads == 0)
		{
		*z = temp.CSinh() + *q;
		}
	    else
		{
		BigTrig.CSinh(&temp1, temp);
		*z = temp1 + *q;
		}
	    break;
	case 14:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = q->y - real_imag - real_imag;
	    break;
	case 15:					// Art Matrix Cubic
	    SlopeDegree = 3;
	    if (variety == 'K')				// CKIN
		*z = z->CCube() + *bBig;		// Z = Z*Z*Z + B
	    else
		{
		temp = z->CCube() + *bBig;		// Z = Z*Z*Z + B
		*z = temp - *aa3Big * *z;		// Z = Z*Z*Z - AA3*Z + B
		}
	    break;
	case 16:					// Basic Newton
	    temp = *z;
	    poly = z->CPolynomial(*degree - 1);
	    *z = *z - (poly * *z - *q - 1) / (poly * *degree);
	    zNewtonBig = *z - temp;
	    break;
	}
    }

/**************************************************************************
	Calculate double double functions
**************************************************************************/

    void	CSlope::DoDDSlopeFwdDiffFn(DDComplex *z, DDComplex *q, DDComplex *bDD, DDComplex *aa3DD)
    {
    DDComplex	sqr, temp, temp1, poly;
    DDComplex	unity = { 1.0, 0.0 };
    dd_real	real_imag, t;
    int		k;
    int		degree1, degree2;

//    PaletteStart = (int)param[4];

    *degree = (int)param[5];
    switch (subtype)
	{
	case 0:					// Mandelbrot
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;
	    break;
	case 1:					// Burning Ship
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    t = z->x * z->y;
	    real_imag = abs(t);
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    break;

	case 2:					// Cubic
	    SlopeDegree = 3;
	    *z = z->CCube() + *q;
	    break;
	case 3:					// Power
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + *q;
	    //		Z = Z.CPolynomial(degree) + Q;
	    break;
	case 4:					// Sin
	    SlopeDegree = 2;
	    if (param[5] == 0)
		*z = z->CSin() * *q;
	    else
		*z = z->CSin() + *q;
	    break;
	case 5:					// Sin + 1/c
	    SlopeDegree = 2;
	    *z = z->CSin() + unity / *q;
	    break;
	case 6:					// exp
	    SlopeDegree = 2;
	    *z = z->CExp() + *q;
	    break;
	case 7:					// Power + 1/c
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + unity / *q;
	    break;
	case 8:
	    degree1 = (int)param[6];
	    degree2 = (int)param[7];
	    SlopeDegree = degree2;
	    *z = z->CPolynomial(*degree) + z->CPolynomial(degree1) + z->CPolynomial(degree2) + *q;
	    break;
	case 9:
	    SlopeDegree = 3;
	    *z = z->CCube() + (*q - 1.0) * *z + *q;
	    break;
	case 10:
	    SlopeDegree = 3;
	    *z = z->CCube() / 4.0 + *z + *q;
	    break;
	case 11:					// Sin(z^n)
	    SlopeDegree = 2;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSin() + *q;
	    break;
	case 12:					// Sinh
	    SlopeDegree = 2;
	    *z = z->CSinh() + *q;
	    break;
	case 13:					// Sinh(z^n)
	    SlopeDegree = *degree;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSinh() + *q;
	    break;
	case 14:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = q->y - real_imag - real_imag;
	    break;
	case 15:					// Art Matrix Cubic
	    SlopeDegree = 3;
	    if (variety == 'K')				// CKIN
		*z = z->CCube() + *bDD;			// Z = Z*Z*Z + B
	    else
		{
		temp = z->CCube() + *bDD;		// Z = Z*Z*Z + B
		*z = temp - *aa3DD * *z;		// Z = Z*Z*Z - AA3*Z + B
		}
	    break;
	case 16:					// Basic Newton
	    temp = *z;
	    poly = z->CPolynomial(*degree - 1);
	    *z = *z - (poly * *z - *q - 1) / (poly * *degree);
	    zNewtonDD = *z - temp;
	    break;
	}
    }

/**************************************************************************
	Calculate quad double functions
**************************************************************************/

void	CSlope::DoQDSlopeFwdDiffFn(QDComplex *z, QDComplex *q, QDComplex *bDD, QDComplex *aa3DD)
    {
    QDComplex	sqr, temp, temp1, poly;
    QDComplex	unity = { 1.0, 0.0 };
    qd_real	real_imag, t;
    int		k;
    int		degree1, degree2;

//    PaletteStart = (int)param[4];

    *degree = (int)param[5];
    switch (subtype)
	{
	case 0:					// Mandelbrot
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;
	    break;
	case 1:					// Burning Ship
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    t = z->x * z->y;
	    real_imag = abs(t);
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    break;

	case 2:					// Cubic
	    SlopeDegree = 3;
	    *z = z->CCube() + *q;
	    break;
	case 3:					// Power
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + *q;
	    //		Z = Z.CPolynomial(degree) + Q;
	    break;
	case 4:					// Sin
	    SlopeDegree = 2;
	    if (param[5] == 0)
		*z = z->CSin() * *q;
	    else
		*z = z->CSin() + *q;
	    break;
	case 5:					// Sin + 1/c
	    SlopeDegree = 2;
	    *z = z->CSin() + unity / *q;
	    break;
	case 6:					// exp
	    SlopeDegree = 2;
	    *z = z->CExp() + *q;
	    break;
	case 7:					// Power + 1/c
	    SlopeDegree = *degree;
	    temp = 1.0;
	    for (k = 0; k < *degree - 1; k++)
		temp *= *z;
	    *z = temp * *z + unity / *q;
	    break;
	case 8:
	    degree1 = (int)param[6];
	    degree2 = (int)param[7];
	    SlopeDegree = degree2;
	    *z = z->CPolynomial(*degree) + z->CPolynomial(degree1) + z->CPolynomial(degree2) + *q;
	    break;
	case 9:
	    SlopeDegree = 3;
	    *z = z->CCube() + (*q - 1.0) * *z + *q;
	    break;
	case 10:
	    SlopeDegree = 3;
	    *z = z->CCube() / 4.0 + *z + *q;
	    break;
	case 11:					// Sin(z^n)
	    SlopeDegree = 2;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSin() + *q;
	    break;
	case 12:					// Sinh
	    SlopeDegree = 2;
	    *z = z->CSinh() + *q;
	    break;
	case 13:					// Sinh(z^n)
	    SlopeDegree = *degree;
	    temp = z->CPolynomial(*degree);
	    *z = temp.CSinh() + *q;
	    break;
	case 14:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]
	    SlopeDegree = 2;
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = q->y - real_imag - real_imag;
	    break;
	case 15:					// Art Matrix Cubic
	    SlopeDegree = 3;
	    if (variety == 'K')				// CKIN
		*z = z->CCube() + *bDD;			// Z = Z*Z*Z + B
	    else
		{
		temp = z->CCube() + *bDD;		// Z = Z*Z*Z + B
		*z = temp - *aa3DD * *z;		// Z = Z*Z*Z - AA3*Z + B
		}
	    break;
	case 16:					// Basic Newton
	    temp = *z;
	    poly = z->CPolynomial(*degree - 1);
	    *z = *z - (poly * *z - *q - 1) / (poly * *degree);
	    zNewtonQD = *z - temp;
	    break;
	}
    }


