/*
    SLOPE.CPP a module for the per pixel calculations of fractals. 
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include	<atomic>
#include	"slope.h"

extern	std::vector<float> wpixels;
extern	std::atomic<bool> gStopRequested;	// force early exit
extern	std::atomic<long> gPixelsDone;

CSlope::CSlope()
    : wpixels(::wpixels)
    {}

CSlope::CSlope(std::vector<float>& wp)
    : wpixels(wp)
    {}

/***********************************************************************
	The dot product of two vectors a = [a1, a2, ..., an] and b = [b1, b2, ..., bn] is defined as:[1]
	d = a1b1 + a2b2
***********************************************************************/

double CSlope::cdot(Complex a, Complex b)
    {
    return a.x * b.x + a.y * b.y;
    }

/***********************************************************************
	Convert bignum to double double
***********************************************************************/

int CSlope::BigDouble2DD(dd_real* out, BigDouble* in)
    {
    if (!out || !in)
	return -1;

    if (*in == 0.0)
	{
	*out = 0.0;
	return 0;
	}

    BigDouble a = *in;
    int sign = 1;

    if (a < 0.0)
	{
	sign = -1;
	a = -a;
	}

    // --- Normalize using double approximation ---
    double approx = a.BigDoubleToDouble();
    if (!std::isfinite(approx) || approx == 0.0)
	{
	*out = 0.0;
	return -1;
	}

    int exp2;
    std::frexp(approx, &exp2);

    // Scale BigDouble to ~[0.5,1)
    BigDouble m = a;
    m = m * pow(2.0, -exp2);   // replace with exact scale if available

    // --- Extract limbs ---
    double y1 = m.BigDoubleToDouble();
    BigDouble r1 = m - y1;

    double y2 = r1.BigDoubleToDouble();

    if (!std::isfinite(y1) || !std::isfinite(y2))
	{
	*out = 0.0;
	return -1;
	}

    if (sign < 0)
	{
	y1 = -y1;
	y2 = -y2;
	}

    // --- Rescale ---
    y1 = std::ldexp(y1, exp2);
    y2 = std::ldexp(y2, exp2);

    // --- Construct DD ---
    *out = dd_real(y1, y2);

    if (isnan(out->x[0]) || isnan(out->x[1]))
	{
	*out = 0.0;
	return -1;
	}

    return 0;
    }

/***********************************************************************
	Convert bignum variables to double double
***********************************************************************/

int	CSlope::ConvertBignumsDD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth,
	dd_real *DDxgap, dd_real *DDygap, dd_real *DDhor, dd_real *DDvert, dd_real *DDWidth)
    {
    if (BigDouble2DD(DDxgap, &Big_xgap) < 0) return -1;
    if (BigDouble2DD(DDygap, &Big_ygap) < 0) return -1;
    if (BigDouble2DD(DDhor, &BigHor) < 0) return -1;
    if (BigDouble2DD(DDvert, &BigVert) < 0) return -1;
    if (BigDouble2DD(DDWidth, &BigWidth) < 0) return -1;
    return 0;
    }

/***********************************************************************
	Convert bignum to quad double
***********************************************************************/
/*
int CSlope::BigDouble2QD(qd_real* out, BigDouble* in)
    {
    if (!out || !in)
	return -1;

    if (*in == 0.0)
	{
	*out = 0.0;
	return 0;
	}

    BigDouble a = *in;
    int sign = 1;

    if (a < 0.0)
	{
	sign = -1;
	a = -a;
	}

    // --- Normalize ---
    double approx = a.BigDoubleToDouble();
    if (!std::isfinite(approx) || approx == 0.0)
	{
	*out = 0.0;
	return -1;
	}

    int exp2;
    std::frexp(approx, &exp2);

    BigDouble m = a;
    m = m * pow(2.0, -exp2);   // TODO: replace with exact scaling later

    // --- Extract limbs ---
    double y1 = m.BigDoubleToDouble();
    BigDouble r1 = m - y1;

    double y2 = r1.BigDoubleToDouble();
    BigDouble r2 = r1 - y2;

    double y3 = r2.BigDoubleToDouble();
    BigDouble r3 = r2 - y3;

    double y4 = r3.BigDoubleToDouble();

    if (!std::isfinite(y1) || !std::isfinite(y2) ||
	!std::isfinite(y3) || !std::isfinite(y4))
	{
	*out = 0.0;
	return -1;
	}

    if (sign < 0)
	{
	y1 = -y1;
	y2 = -y2;
	y3 = -y3;
	y4 = -y4;
	}

    // --- Rescale ---
    y1 = std::ldexp(y1, exp2);
    y2 = std::ldexp(y2, exp2);
    y3 = std::ldexp(y3, exp2);
    y4 = std::ldexp(y4, exp2);

    // --- Construct QD (important: use constructor!) ---
    *out = qd_real(y1, y2, y3, y4);

    if (isnan(out->x[0]) || isnan(out->x[1]) ||
	isnan(out->x[2]) || isnan(out->x[3]))
	{
	*out = 0.0;
	return -1;
	}

    return 0;
    }

int	CSlope::BigDouble2QD(qd_real *out, BigDouble *in)
    {
    qd_real	x1, x2, x3, x4;
    BigDouble	t1, t2, t3, t4, t5, t6;
    double	y1, y2, y3, y4;

    y1 = in->BigDoubleToDouble();	// truncate to a double
    t1 = y1;
    t2 = *in - t1;			// subtract truncated bit to get the remainder
    y2 = t2.BigDoubleToDouble();	// remainder as a float
    t3 = y2;
    t4 = t2 - t3;
    y3 = t4.BigDoubleToDouble();
    t5 = y3;
    t6 = t4 - t5;
    y4 = t6.BigDoubleToDouble();
    x1 = y1;
    x2 = y2;
    x3 = y3;
    x4 = y4;
    *out = x1 + x2 + x3 + x4;
    if (isnan(out->x[0]) || isnan(out->x[1]))
	{
	*out = 0.0;
	return -1;
	}
    return 0;
    }
*/


/***********************************************************************
	Convert bignum variables to quad double
***********************************************************************/

int	CSlope::ConvertBignumsQD(BigDouble Big_xgap, BigDouble Big_ygap, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth,
    qd_real *QDxgap, qd_real *QDygap, qd_real *QDhor, qd_real *QDvert, qd_real *QDWidth)
    {
    if (Big_xgap.BigDouble2QD(QDxgap) < 0) return -1;
    if (Big_ygap.BigDouble2QD(QDygap) < 0) return -1;
    if (BigHor.BigDouble2QD(QDhor) < 0) return -1;
    if (BigVert.BigDouble2QD(QDvert) < 0) return -1;
    if (BigWidth.BigDouble2QD(QDWidth) < 0) return -1;


/*

    if (BigDouble2QD(QDxgap, &Big_xgap) < 0) return -1;
    if (BigDouble2QD(QDygap, &Big_ygap) < 0) return -1;
    if (BigDouble2QD(QDhor, &BigHor) < 0) return -1;
    if (BigDouble2QD(QDvert, &BigVert) < 0) return -1;
    if (BigDouble2QD(QDWidth, &BigWidth) < 0) return -1;
*/
    return 0;
    }

/***********************************************************************
	Reflection: fractal calculations quad double version
***********************************************************************/

double CSlope::GiveQDReflection(Complex j, BYTE juliaflag, QDComplex cQD, int &iterations, double &smoothIter, double rqlim, long threshold, Complex v)
    {
    int		i = 0; // iteration 
    int		k;
    qd_real	realimagQD;
    QDComplex	sqrQD, vQD, qQD, temp, temp1;

    QDComplex	dCQD = 0.0;		// derivative with respect to c 
    qd_real	QDReflection = 0.0;
    double	reflection = FP_ZERO;	// inside 
    QDComplex	zQD = 0.0;
    qd_real	h2 = param[1];		// height factor of the incoming light
    QDComplex	u;
    int		degree1, degree2;
    QDComplex	unity = { 1.0, 0.0 };

    vQD.x = v.x;
    vQD.y = v.y;

    *degree = (int)param[3];
    if (juliaflag)
	{
	qQD.x = j.x;
	qQD.y = j.y;
	zQD.x = cQD.x;
	zQD.y = cQD.y;
	}
    else
	{
	if (subtype == 3)				// Sine
	    zQD = param[4];
	qQD = cQD;
	}
    for (i = 0; i < threshold; i++)
	{
	switch (subtype)
	    {
	    case 0:					// Mandelbrot
		dCQD = dCQD * (zQD + zQD) + 1.0;
		sqrQD.x = zQD.x * zQD.x;
		sqrQD.y = zQD.y * zQD.y;
		realimagQD = zQD.x * zQD.y;
		zQD.x = qQD.x + sqrQD.x - sqrQD.y;
		zQD.y = qQD.y + realimagQD + realimagQD;
		break;
	    case 1:					// Cubic
		dCQD = dCQD * zQD.CSqr() * 3.0 + 1.0;
		zQD = zQD.CCube() + qQD;
		break;
	    case 2:					// Power
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= zQD;
		dCQD = temp * dCQD * (double)*degree + 1.0;
		zQD = temp * zQD + qQD;
		break;
	    case 3:					// Sin
		dCQD = dCQD * zQD.CCos() + 1.0;
		if (param[3] == 0)
		    zQD = zQD.CSin() * qQD;
		else
		    zQD = zQD.CSin() + qQD;
		break;
	    case 4:					// Sin + 1/c
		dCQD = dCQD * zQD.CCos() + 1.0;
		zQD = zQD.CSin() + unity / qQD;
		break;
	    case 5:					// exp
		dCQD = dCQD * zQD.CExp() + 1.0;
		zQD = zQD.CExp() + qQD;
		break;
	    case 6:					// Power + 1/c
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= zQD;
		dCQD = temp * dCQD * (double)*degree + 1.0;
		zQD = temp * zQD + unity / qQD;
		break;
	    case 7:
		degree1 = (int)param[4];
		degree2 = (int)param[5];
		dCQD = dCQD * zQD.CPolynomial(*degree - 1) * param[3] + zQD.CPolynomial(degree1 - 1) * param[4] + zQD.CPolynomial(degree2 - 1) * param[5] + 1.0;
		zQD = zQD.CPolynomial(*degree) + zQD.CPolynomial(degree1) + zQD.CPolynomial(degree2) + qQD;
		break;
	    case 8:
		dCQD = dCQD * zQD.CSqr() * 3.0 + 1.0;
		zQD = zQD.CCube() + (qQD - 1.0) * zQD + qQD;
		break;
	    case 9:
		dCQD = dCQD * zQD.CSqr() * 0.75 + 2.0;
		zQD = zQD.CCube() / 4.0 + zQD + qQD;
		break;
	    case 10:					// Sin(z^n)
		temp = zQD.CPolynomial(*degree);
		dCQD = dCQD * temp.CCos() * zQD.CPolynomial(*degree - 1) * param[3] + 1.0;
		zQD = temp.CSin() + qQD;
		break;
	    case 11:					// Sinh
		dCQD = dCQD * zQD.CSinh() + 1.0;
		zQD = zQD.CSinh() + qQD;
		break;
	    case 12:					// Sinh(z^n)
		temp = zQD.CPolynomial(*degree);
		dCQD = dCQD * temp.CSinh() * zQD.CPolynomial(*degree - 1) * param[3] + 1.0;
		zQD = temp.CSinh() + qQD;
		break;
	    }

	double	SumSqr = zQD.CSumSqr();
	if (SumSqr > rqlim)
	    {
	    if (smoothing)
		{
		// --- tiny incremental change: compute smooth iteration ------------------
		double mag = sqrt(SumSqr);
		double smoothIter_local = i + 1 - log(log(mag)) / log(SlopeDegree);
		smoothIter = smoothIter_local;
		// ------------------------------------------------------------------------
		}

	    u = zQD / dCQD;
	    u = u / u.CFabs();
	    QDReflection = u.x * vQD.x + u.y * vQD.y + h2;
	    QDReflection = QDReflection / (h2 + 1.0); // rescale so that t does not get QDger than 1
	    reflection = to_double(QDReflection);
	    if (reflection < 0.0) reflection = 0.0;
	    iterations = i;
	    break;
	    }
	}
    if (i == threshold)
	{
	iterations = threshold;
	smoothIter = (double)threshold;
	return 0.0;  // reflection brightness = 0
	}
    return reflection;
    }

/***********************************************************************
	Reflection: fractal calculations double double version
***********************************************************************/

double CSlope::GiveDDReflection(Complex j, BYTE juliaflag, DDComplex cDD, int &iterations, double &smoothIter, double rqlim, long threshold, Complex v)
    {
    int		i = 0; // iteration 
    int		k;
    dd_real	realimagDD;
    DDComplex	sqrDD, vDD, qDD, temp, temp1;

    DDComplex	dCDD = 0.0;		// derivative with respect to c 
    dd_real	DDReflection = 0.0;
    double	reflection = FP_ZERO;	// inside 
    DDComplex	zDD = 0.0;
    dd_real	h2 = param[1];		// height factor of the incoming light
    DDComplex	u;
    int		degree1, degree2;
    DDComplex	unity = { 1.0, 0.0 };

    vDD.x = v.x;
    vDD.y = v.y;

    *degree = (int)param[3];
    if (juliaflag)
	{
	qDD.x = j.x;
	qDD.y = j.y;
	zDD.x = cDD.x;
	zDD.y = cDD.y;
	}
    else
	{
	if (subtype == 3)				// Sine
	    zDD = param[4];
	qDD = cDD;
	}
    for (i = 0; i < threshold; i++)
	{
	switch (subtype)
	    {
	    case 0:					// Mandelbrot
		dCDD = dCDD * (zDD + zDD) + 1.0;
		sqrDD.x = zDD.x * zDD.x;
		sqrDD.y = zDD.y * zDD.y;
		realimagDD = zDD.x * zDD.y;
		zDD.x = qDD.x + sqrDD.x - sqrDD.y;
		zDD.y = qDD.y + realimagDD + realimagDD;
		break;
	    case 1:					// Cubic
		dCDD = dCDD * zDD.CSqr() * 3.0 + 1.0;
		zDD = zDD.CCube() + qDD;
		break;
	    case 2:					// Power
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= zDD;
		dCDD = temp * dCDD * (double)*degree + 1.0;
		zDD = temp * zDD + qDD;
		break;
	    case 3:					// Sin
		dCDD = dCDD * zDD.CCos() + 1.0;
		if (param[3] == 0)
		    zDD = zDD.CSin() * qDD;
		else
		    zDD = zDD.CSin() + qDD;
		break;
	    case 4:					// Sin + 1/c
		dCDD = dCDD * zDD.CCos() + 1.0;
		zDD = zDD.CSin() + unity / qDD;
		break;
	    case 5:					// exp
		dCDD = dCDD * zDD.CExp() + 1.0;
		zDD = zDD.CExp() + qDD;
		break;
	    case 6:					// Power + 1/c
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= zDD;
		dCDD = temp * dCDD * (double)*degree + 1.0;
		zDD = temp * zDD + unity / qDD;
		break;
	    case 7:
		degree1 = (int)param[4];
		degree2 = (int)param[5];
		dCDD = dCDD * zDD.CPolynomial(*degree - 1) * param[3] + zDD.CPolynomial(degree1 - 1) * param[4] + zDD.CPolynomial(degree2 - 1) * param[5] + 1.0;
		zDD = zDD.CPolynomial(*degree) + zDD.CPolynomial(degree1) + zDD.CPolynomial(degree2) + qDD;
		break;
	    case 8:
		dCDD = dCDD * zDD.CSqr() * 3.0 + 1.0;
		zDD = zDD.CCube() + (qDD - 1.0) * zDD + qDD;
		break;
	    case 9:
		dCDD = dCDD * zDD.CSqr() * 0.75 + 2.0;
		zDD = zDD.CCube() / 4.0 + zDD + qDD;
		break;
	    case 10:					// Sin(z^n)
		temp = zDD.CPolynomial(*degree);
		dCDD = dCDD * temp.CCos() * zDD.CPolynomial(*degree - 1) * param[3] + 1.0;
		zDD = temp.CSin() + qDD;
		break;
	    case 11:					// Sinh
		dCDD = dCDD * zDD.CSinh() + 1.0;
		zDD = zDD.CSinh() + qDD;
		break;
	    case 12:					// Sinh(z^n)
		temp = zDD.CPolynomial(*degree);
		dCDD = dCDD * temp.CSinh() * zDD.CPolynomial(*degree - 1) * param[3] + 1.0;
		zDD = temp.CSinh() + qDD;
		break;
	    }

	double	SumSqr = zDD.CSumSqr();
	if (SumSqr > rqlim)
	    {
	    if (smoothing)
		{
		// --- tiny incremental change: compute smooth iteration ------------------
		double mag = sqrt(SumSqr);
		double smoothIter_local = i + 1 - log(log(mag)) / log(SlopeDegree);
		smoothIter = smoothIter_local;
		// ------------------------------------------------------------------------
		}

	    u = zDD / dCDD;
	    u = u / u.CFabs();
	    DDReflection = u.x * vDD.x + u.y * vDD.y + h2;
	    DDReflection = DDReflection / (h2 + 1.0); // rescale so that t does not get DDger than 1
	    reflection = to_double(DDReflection);
	    if (reflection < 0.0) reflection = 0.0;
	    iterations = i;
	    break;
	    }
	}
    if (i == threshold)
	{
	iterations = threshold;
	smoothIter = (double)threshold;
	return 0.0;  // reflection brightness = 0
	}
    return reflection;
    }

/***********************************************************************
	Reflection: fractal calculations Bignum version
***********************************************************************/

double CSlope::GiveBigReflection(Complex j, BYTE juliaflag, BigComplex cBig, int &iterations, double &smoothIter, double rqlim, long threshold, Complex v)
    {
    int		i = 0; // iteration 
    int		k;
    BigDouble	realimagBig;
    BigComplex	sqrBig, vBig, qBig, tempBig, tempBig1;

    BigComplex	dCBig = 0.0;		// derivative with respect to c 
    BigDouble	BigReflection = 0.0;
    double	reflection = FP_ZERO;	// inside 
    BigComplex	zBig = 0.0;
    BigDouble	h2 = param[1];		// height factor of the incoming light
    BigComplex	u;
    int		degree1, degree2;
    BigComplex	unity = { 1.0, 0.0 };

    vBig = v;

    *degree = (int)param[3];
    if (juliaflag)
	{
	qBig = j;
	zBig = cBig;
	}
    else
	{
	if (subtype == 3)				// Sine
	    zBig = param[4];
	qBig = cBig;
	}
    for (i = 0; i < threshold; i++)
	{
	switch (subtype)
	    {
	    case 0:					// Mandelbrot
		dCBig = dCBig * (zBig + zBig) + 1.0;
		sqrBig.x = zBig.x * zBig.x;
		sqrBig.y = zBig.y * zBig.y;
		realimagBig = zBig.x * zBig.y;
		zBig.x = qBig.x + sqrBig.x - sqrBig.y;
		zBig.y = qBig.y + realimagBig + realimagBig;
		break;
	    case 1:					// Cubic
		dCBig = dCBig * zBig.CSqr() * 3.0 + 1.0;
		zBig = zBig.CCube() + qBig;
		break;
	    case 2:					// Power
		tempBig = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    tempBig *= zBig;
		dCBig = tempBig * dCBig * (double)*degree + 1.0;
		zBig = tempBig * zBig + qBig;
		break;
	    case 3:					// Sin
		if (NumThreads == 0)
		    {
		    dCBig = dCBig * zBig.CCos() + 1.0;
		    if (param[3] == 0)
			zBig = zBig.CSin() * qBig;
		    else
			zBig = zBig.CSin() + qBig;
		    }
		else
		    {
		    BigTrig.CCos(&tempBig, zBig);
		    dCBig = dCBig * tempBig + 1.0;
		    BigTrig.CSin(&tempBig, zBig);
		    if (param[3] == 0)
			zBig = tempBig * qBig;
		    else
			zBig = tempBig + qBig;
		    }
		break;
	    case 4:					// Sin + 1/c
		if (NumThreads == 0)
		    {
		    dCBig = dCBig * zBig.CCos() + 1.0;
		    zBig = zBig.CSin() + unity / qBig;
		    }
		else
		    {
		    BigTrig.CCos(&tempBig, zBig);
		    dCBig = dCBig * tempBig + 1.0;
		    BigTrig.CSin(&tempBig, zBig);
		    zBig = tempBig + unity / qBig;
		    }
		break;
	    case 5:					// exp
		if (NumThreads == 0)
		    {
		    dCBig = dCBig * zBig.CExp() + 1.0;
		    zBig = zBig.CExp() + qBig;
		    }
		else
		    {
		    BigTrig.CExp(&tempBig, zBig);
		    dCBig = dCBig * tempBig + 1.0;
		    zBig = tempBig + qBig;
		    }
		break;
	    case 6:					// Power + 1/c
		tempBig = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    tempBig *= zBig;
		dCBig = tempBig * dCBig * (double)*degree + 1.0;
		zBig = tempBig * zBig + unity / qBig;
		break;
	    case 7:
		degree1 = (int)param[4];
		degree2 = (int)param[5];
		dCBig = dCBig * zBig.CPolynomial(*degree - 1) * param[3] + zBig.CPolynomial(degree1 - 1) * param[4] + zBig.CPolynomial(degree2 - 1) * param[5] + 1.0;
		zBig = zBig.CPolynomial(*degree) + zBig.CPolynomial(degree1) + zBig.CPolynomial(degree2) + qBig;
		break;
	    case 8:
		dCBig = dCBig * zBig.CSqr() * 3.0 + 1.0;
		zBig = zBig.CCube() + (qBig - 1.0) * zBig + qBig;
		break;
	    case 9:
		dCBig = dCBig * zBig.CSqr() * 0.75 + 2.0;
		zBig = zBig.CCube() / 4.0 + zBig + qBig;
		break;
	    case 10:					// Sin(z^n)
		if (NumThreads == 0)
		    {
		    tempBig = zBig.CPolynomial(*degree);
		    dCBig = dCBig * tempBig.CCos() * zBig.CPolynomial(*degree - 1) * param[3] + 1.0;
		    zBig = tempBig.CSin() + qBig;
		    }
		else
		    {
		    tempBig = zBig.CPolynomial(*degree);
		    BigTrig.CCos(&tempBig1, tempBig);
		    dCBig = dCBig * tempBig1 * zBig.CPolynomial(*degree - 1) * param[3] + 1.0;
		    BigTrig.CSin(&tempBig1, tempBig);
		    zBig = tempBig1 + qBig;
		    }
		break;
	    case 11:					// Sinh
		if (NumThreads == 0)
		    {
		    dCBig = dCBig * zBig.CSinh() + 1.0;
		    zBig = zBig.CSinh() + qBig;
		    }
		else
		    {
		    BigTrig.CSinh(&tempBig, zBig);
		    dCBig = dCBig * tempBig + 1.0;
		    zBig = tempBig + qBig;
		    }
		break;
	    case 12:					// Sinh(z^n)
		if (NumThreads == 0)
		    {
		    tempBig = zBig.CPolynomial(*degree);
		    dCBig = dCBig * tempBig.CSinh() * zBig.CPolynomial(*degree - 1) * param[3] + 1.0;
		    zBig = tempBig.CSinh() + qBig;
		    }
		else
		    {
		    tempBig = zBig.CPolynomial(*degree);
		    BigTrig.CSinh(&tempBig1, tempBig);
		    dCBig = dCBig * tempBig1 * zBig.CPolynomial(*degree - 1) * param[3] + 1.0;
		    zBig = tempBig1 + qBig;
		    }
		break;
	    }

	double	SumSqr = zBig.CSumSqr();
	if (SumSqr > rqlim)
	    {
	    if (smoothing)
		{
		// --- tiny incremental change: compute smooth iteration ------------------
		double mag = sqrt(SumSqr);
		double smoothIter_local = i + 1 - log(log(mag)) / log(SlopeDegree);
		smoothIter = smoothIter_local;
		// ------------------------------------------------------------------------
		}
	    u = zBig / dCBig;
	    u = u / u.CFabs();
	    BigReflection = u.x * vBig.x + u.y * vBig.y + h2;
	    BigReflection = BigReflection / (h2 + 1.0); // rescale so that t does not get bigger than 1
	    reflection = (double)mpfr_get_d(BigReflection.x, MPFR_RNDN);
	    if (reflection < 0.0) reflection = 0.0;
	    iterations = i;
	    break;
	    }
	}
    if (i == threshold)
	{
	iterations = threshold;
	smoothIter = (double)threshold;
	return 0.0;  // reflection brightness = 0
	}
    return reflection;
    }

/***********************************************************************
	Reflection: fractal calculations double float version
***********************************************************************/

double CSlope::GiveReflection(Complex j, BYTE juliaflag, Complex C, int &iterations, double &smoothIter, double rqlim, long threshold, Complex v)
    {
    int		i = 0;					// iteration 
    int		k;
    double	real_imag;
    Complex	sqr, temp, Q;

    int		degree1, degree2;

    Complex	Z = 0.0;				// initial value for iteration Z0
//    Complex	z = 1.0;				// initial value for iteration Z0 used in Newton fractals
    Complex	dC = 0.0;				// derivative with respect to c 
    double	reflection = FP_ZERO; // inside 
    double	h2 = param[1];				// height factor of the incoming light
    Complex	u;
    Complex	t = { 1.0, 0.0 };

    *degree = (int)param[3];
    if (juliaflag)
	{
	Q = j;
	Z = C;
	}
    else
	{
	if (subtype == 3)				// Sine
	    Z = param[4];
	Q = C;
	}
    for (i = 0; i < threshold; i++)
	{
	switch (subtype)
	    {
	    case 0:					// Mandelbrot
		dC = dC * (Z + Z) + 1.0;
		sqr.x = Z.x * Z.x;
		sqr.y = Z.y * Z.y;
		real_imag = Z.x * Z.y;
		Z.x = Q.x + sqr.x - sqr.y;
		Z.y = Q.y + real_imag + real_imag;
		break;
	    case 1:					// Cubic
		dC = 3.0 * dC * Z.CSqr() + 1.0;
		Z = Z.CCube() + Q;
		break;
	    case 2:					// Power
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= Z;
		dC = temp * dC * (double)*degree + 1.0;
		Z = temp * Z + Q;
//		dC = dC * Z.CPolynomial((degree - 1)) * (double)degree + 1.0;
//		Z = Z.CPolynomial(degree) + Q;
		break;
	    case 3:					// Sine
		dC = dC * Z.CCos() + 1.0;
		if (param[3] == 0)
		    Z = Z.CSin() * Q;
		else
		    Z = Z.CSin() + Q;
		break;
	    case 4:					// Sine + 1/c
		dC = dC * Z.CCos() + 1.0;
		Z = Z.CSin() + t / Q;
		break;
	    case 5:					// exp
		dC = dC * Z.CExp() + 1.0;
		Z = Z.CExp() + Q;
		break;
	    case 6:					// Power + 1/c
		temp = 1.0;
		for (k = 0; k < *degree - 1; k++)
		    temp *= Z;
		dC = temp * dC * (double)*degree + 1.0;
		Z = temp * Z + t/Q;
		break;
	    case 7:
		degree1 = (int)param[4];
		degree2 = (int)param[5];
		dC = dC * Z.CPolynomial(*degree - 1) * param[3] + Z.CPolynomial(degree1 - 1) * param[4] + Z.CPolynomial(degree2 - 1) * param[5] + 1.0;
		Z = Z.CPolynomial(*degree) + Z.CPolynomial(degree1) + Z.CPolynomial(degree2) + Q;
		break;
	    case 8:
		dC = 3.0 * dC * Z.CSqr() + 1.0;
		Z = Z.CCube() + (Q - 1.0) * Z + Q;
		break;
	    case 9:
		dC = dC * Z.CSqr() * 0.75 + 2.0;
		Z = Z.CCube() / 4.0 + Z + Q;
		break;
	    case 10:					// Sin(z^n)
		temp = Z.CPolynomial(*degree);
		dC = dC * temp.CCos() * Z.CPolynomial(*degree - 1) * param[3] + 1.0;
		Z = temp.CSin() + Q;
		break;
	    case 11:					// Sinh
		dC = dC * Z.CSinh() + 1.0;
		Z = Z.CSinh() + Q;
		break;
	    case 12:					// Sinh(z^n)
		temp = Z.CPolynomial(*degree);
		dC = dC * temp.CSinh() * Z.CPolynomial(*degree - 1) * param[3] + 1.0;
		Z = temp.CSinh() + Q;
		break;
//	    case 13:					//  z1 = conj(z)); z = z1*z1+c; [Tricorn or Mandelbar]


/*
405       <reference t="R">
406         Xrn = Xr2 - Xi2 + Cr;
407         Xin = Ci - Xr * Xi * 2;
408       </reference>
409       <perturbation t="R">
410         xrn = 2 * Xr * xr + xr2 - xi2 - 2 * xi * Xi + cr;
411         xin = ci - 2 * (Xr * xi + xr * Xi + xr * xi);
412       </perturbation>
413	    < derivative t = 'M' >
414         dxan = ((-2 * (Xxi * dya)) + ((2 * (Xxr * dxa)) + daa));
415         dxbn = ((-2 * (Xxi * dyb)) + ((2 * (Xxr * dxb)) + dab));
416         dyan = ((-2 * (Xxi * dxa)) + ((-2 * (Xxr * dya)) + dba));
417         dybn = ((-2 * (Xxi * dxb)) + ((-2 * (Xxr * dyb)) + dbb));
418	    < / derivative >


		Complex temp1;
		temp = Z;
		temp.y = -temp.y;			// get conjugate
//		dC = dC * (Z + Z) + 1.0;
		dC = dC * (temp + temp)  * param[9] + 1.0;
		sqr.x = temp.x * temp.x;
		sqr.y = temp.y * temp.y;
		real_imag = temp.x * temp.y;
		Z.x = Q.x + sqr.x - sqr.y;
		Z.y = Q.y + real_imag + real_imag;
//		temp1 = Z + Z;
//		temp1.y = -temp1.y;
//		dC = dC * temp1 * param[9] + 1.0;
//		dC = dC * (temp + temp) + 1.0;
//		dC = dC * (Z + Z) + 1.0;
//		dC.y = -dC.y;
//		temp = Z;
//		temp.y = -temp.y;			// get conjugate
*/
/*
		sqr.x = Z.x * Z.x;
		sqr.y = Z.y * Z.y;
		real_imag = Z.x * Z.y;
		dC.x = ((Z.x + Z.x) * dC.x) - ((Z.y + Z.y) * dC.y) + 1.0;
		dC.y = ((Z.x + Z.x) * dC.y) - ((Z.y + Z.y) * dC.y);
		Z.x = sqr.x - sqr.y + Q.x;
		Z.y = Q.y - real_imag - real_imag;
		break;
*/
/*
	    case 13:					//  z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]
		// Quotient Rule: f/g =	(f'g - g'f) / g*g
		// remember 
		Complex	z1, z2, f, g, df, dg;
		
		z1 = z;
		z2 = z.CSqr();
		f = z*z2 - 1;
		g = z2 * 3;
		df = 3 * z2;
		dg = 6 * z;
		// Applying quotient rule:
		temp = (df * g - dg * f) / g * g;
//		dC = dC * (1.0 - temp) + 2.0;
		dC = dC * Z.CSinh() + 1.0;
		z = z - ((z*z2 - 1) / (z2 * 3)) + Q;
		Z = z - z1;
		break;
*/
	    }

	double	SumSqr = Z.CSumSqr();
	if (SumSqr > rqlim)
	    {
	    if (smoothing)
		{
		// --- tiny incremental change: compute smooth iteration ------------------
		double mag = sqrt(SumSqr);
		double smoothIter_local = i + 1 - log(log(mag)) / log(SlopeDegree);
		smoothIter = smoothIter_local;
		// ------------------------------------------------------------------------
		}

	    u = Z / dC;
	    u = u / u.CFabs();
	    reflection = cdot(u, v) + h2;
	    reflection = reflection / (1.0 + h2); // rescale so that t does not get bigger than 1
	    if (reflection < 0.0) reflection = 0.0;
	    iterations = i;
	    break;
	    }
	}
    if (i == threshold)
	{
	iterations = threshold;
	smoothIter = (double)threshold;
	return 0.0;  // reflection brightness = 0
	}
    return reflection;
    }

/**************************************************************************
	Caluclate the colour of the pixel
**************************************************************************/

RGBTRIPLE CSlope::compute_colour(CTrueCol *TrueCol, Complex j, BYTE juliaflag, Complex c, BigComplex cBig, QDComplex cQD, DDComplex cDD, double rqlim, long threshold, BYTE ArithType, Complex v, 
	    bool *Time2Exit, int &iterations, double &reflectionOut, double &smoothIterations)
    {

    double	reflection;
    BYTE	b;
    RGBTRIPLE	colour;
    int		PaletteStart = (int)(fabs(param[2]));

    *Time2Exit = false;
    switch (ArithType)
	{
	case DOUBLEDOUBLE:
	    reflection = GiveDDReflection(j, juliaflag, cDD, iterations, smoothIterations, rqlim, threshold, v);
	    break;
	case QUADDOUBLE:
	    reflection = GiveQDReflection(j, juliaflag, cQD, iterations, smoothIterations, rqlim, threshold, v);
	    break;
	case ARBITRARYPREC:
	    reflection = GiveBigReflection(j, juliaflag, cBig, iterations, smoothIterations, rqlim, threshold, v);
	    break;
	case DOUBLEFLOAT:
	    reflection = GiveReflection(j, juliaflag, c, iterations, smoothIterations, rqlim, threshold, v);
	    break;
	}

    reflectionOut = reflection;

    if (reflection < 0)
	*Time2Exit = true;

    if (reflection == FP_ZERO) 
	{
	colour.rgbtRed = (BYTE)TrueCol->InsideRed;		// M_waves
	colour.rgbtGreen = (BYTE)TrueCol->InsideGreen;
	colour.rgbtBlue = (BYTE)TrueCol->InsideBlue;
	}
    else // exterior of Mandelbrot set = normal 
	{
	b = (BYTE)(255.0 * reflection);

	if (iterations >= PaletteStart)
	    {
	    // colours swap = don't me??? But it works
	    colour.rgbtRed = (BYTE)((reflection)   * (double)(TrueCol->PalettePtr[(BYTE)((iterations) % 256)].rgbtBlue));	// Red
	    colour.rgbtGreen = (BYTE)((reflection) * (double)(TrueCol->PalettePtr[(BYTE)((iterations) % 256)].rgbtGreen));	// Green
	    colour.rgbtBlue = (BYTE)((reflection)  * (double)(TrueCol->PalettePtr[(BYTE)((iterations) % 256)].rgbtRed));	// Blue
	    }
	else
	    {
	    colour.rgbtRed = b;
	    colour.rgbtGreen = b;
	    colour.rgbtBlue = b;
	    }
	}
    return colour;
    }

/**************************************************************************
    End Threads
**************************************************************************/

void	CSlope::SlopeIsExiting(void)
    {
    EndSlope = true;		// advise thread to ternminate
    }

/**************************************************************************
	Create 2D vector
	Complex v = cexp(2.0 * angle * PI * I); // = exp(1j*angle*2*pi/360)  // unit 2D vector in this direction
	incoming light 3D vector = (v.re,v.im,h2)
**************************************************************************/

void	    CSlope::Create2DVector(Complex *v, double LightAngle)
    {
    Complex	w;	    // unit 2D vector
    w = { 0.0, 2.0 * LightAngle * PI / 360.0};
    *v = w.CExp();
    }

/**************************************************************************
	Slope Fractal
**************************************************************************/

int CSlope::RunSlopeDerivative(HWND hwndIn, int user_data(HWND hwnd), char* StatusBarInfo, bool *ThreadComplete, int subtypeIn, int NumThreadsIn, int threadIn, Complex j, double mandel_width, double hor, double vert, 
					    BYTE BigNumFlag, BigDouble BigHor, BigDouble BigVert, BigDouble BigWidth, double rqlim, long threshold, double paramIn[], CTrueCol *TrueCol, CDib *Dib, 
					    BYTE juliaflag, int xdots, int ydots, int width, int height, WORD *degreeIn, int precisionIn, double ColourSpeedIn, HANDLE ghMutex)
    {
    Complex	c;
    BigComplex	cBig;
    DDComplex	cDD;
    QDComplex	cQD;
    double	xgap, ygap;
    dd_real	DDhor, DDvert, DDxgap, DDygap, DDWidth;
    qd_real	QDhor, QDvert, QDxgap, QDygap, QDWidth;
    RGBTRIPLE	colour;
    int		lastChecked = -1;
    Complex	v;// unit 2D vector in this direction
    int		i;
    double	LightAngle = param[0];
    bool	Time2Exit = false;
    CPlot	Plot;
    int		iterations = 0;
    double	SmoothIterations = 0.0;
    double	ReflectionLocal = 1.0;

    for (i = 0; i < NUMSLOPEDERIVPARAM; i++)
	param[i] = paramIn[i];

    precision = precisionIn;
    thread = threadIn;
    NumThreads = NumThreadsIn;
    subtype = subtypeIn;
    hwnd = hwndIn;
    bits_per_pixel = Dib->BitsPerPixel;
    degree = degreeIn;
    ColourSpeed = ColourSpeedIn;
    smoothing = (ColourSpeed != 0.0);

    gStopRequested = false; // NEW: reset flag before starting threads

    Create2DVector(&v, LightAngle);

    effectiveThreads = (NumThreads > 0) ? NumThreads : 1;
    StripMult = 8;  // or 4
    TotalStrips = effectiveThreads * StripMult;
    StripWidth = (xdots + TotalStrips - 1) / TotalStrips;
    long TotalPixels = (long)xdots * ydots;
    *ThreadComplete = false;
/*
    if (NumThreads == 0)
	StripWidth = xdots;
    else
	StripWidth = xdots / NumThreads + 1;
    StripStart = StripWidth * thread;
*/

    double	temp_x, temp_y;
    double	ScreenRatio = (double) xdots / (double) ydots;

    temp_x = ScreenRatio / (double)(xdots - 1);
    temp_y = 1.0 / (double)(ydots - 1);

    if (BigNumFlag)
	{
	Big_xgap = BigWidth * temp_x;
	Big_ygap = BigWidth * temp_y;
	if (precision <= DDPRECISION)
	    {
	    ConvertBignumsDD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, &DDxgap, &DDygap, &DDhor, &DDvert, &DDWidth);
	    ArithType = DOUBLEDOUBLE;
	    }
	else if (precision <= QDPRECISION)
	    {
	    ConvertBignumsQD(Big_xgap, Big_ygap, BigHor, BigVert, BigWidth, &QDxgap, &QDygap, &QDhor, &QDvert, &QDWidth);
	    ArithType = QUADDOUBLE;
	    }
	else
	    ArithType = ARBITRARYPREC;
	}
    else
	{
	ArithType = DOUBLEFLOAT;
	xgap = mandel_width * temp_x;
	ygap = mandel_width * temp_y;
	}
    Plot.InitPlot(threshold, TrueCol, wpixels, xdots, height, xdots, height, Dib->BitsPerPixel, Dib, USEPALETTE);
    CPlotmode PlotMethod;
    auto yOrder = PlotMethod.generateYOrder(ydots, currentYMode);
    for (int y = ydots - 1; y >= 0; --y)
	{
	int iY = yOrder[y];
	double progress = (double)gPixelsDone / (double)TotalPixels;
//	double progress = (double)iY / ydots;
	switch (ArithType)
	    {
	    case DOUBLEDOUBLE:
		cDD.y = DDvert + DDWidth - DDygap * iY;
		break;
	    case QUADDOUBLE:
		cQD.y = QDvert + QDWidth - QDygap * iY;
		break;
	    case ARBITRARYPREC:
		cBig.y = BigVert + BigWidth - Big_ygap * iY;
		break;
	    case DOUBLEFLOAT:
		c.y = vert + mandel_width - iY * ygap;
		break;
	    }
	if (int(progress * 100) != lastChecked)
	    {
	    lastChecked = int(progress * 100);
	    _snprintf_s(StatusBarInfo, MAXLINE, _TRUNCATE, "Progess (%d%%), %d Threads", int(progress * 100), NumThreads);
	    }
	for (int strip = thread; strip < TotalStrips; strip += effectiveThreads)
	    {
	    int StripStart = strip * StripWidth;
	    int StripEnd = min(StripStart + StripWidth, xdots);

	    for (iX = StripStart; iX < StripEnd; iX++)
//	    for (iX = StripStart; iX < StripStart + StripWidth && iX < xdots; iX++)
		{
		if (user_data(hwnd) < 0)
		    return -1;
		if (AbortRequested())
		    return -1;

		gPixelsDone.fetch_add(1, std::memory_order_relaxed);
		switch (ArithType)
		    {
		    case DOUBLEDOUBLE:
			cDD.x = DDhor + DDxgap * iX;
			break;
		    case QUADDOUBLE:
			cQD.x = QDhor + QDxgap * iX;
			break;
		    case ARBITRARYPREC:
			cBig.x = BigHor + Big_xgap * iX;
			break;
		    case DOUBLEFLOAT:
			c.x = hor + iX * xgap;
			break;
		    }

		colour = compute_colour(TrueCol, j, juliaflag, c, cBig, cQD, cDD, rqlim, threshold, ArithType, v, &Time2Exit, iterations, ReflectionLocal, SmoothIterations);
		if (Time2Exit)
		    return 0;
		//	    plot(iX, iY, iterations);
		if (EndSlope)		// program is trying to terminate. Better get out of the thread
		    {
		    *ThreadComplete = true;
		    return -1;
		    }
		if (ghMutex != NULL)
		    WaitForSingleObject(ghMutex, INFINITE);  // no time-out interval
		if (AbortRequested())
		    return -1;
		if (smoothing)
		    {
		    // INSIDE set -> show user-defined inside colour
		    if (iterations == threshold)
			{
			RGBTRIPLE inside;
			inside.rgbtRed = (BYTE)TrueCol->InsideRed;
			inside.rgbtGreen = (BYTE)TrueCol->InsideGreen;
			inside.rgbtBlue = (BYTE)TrueCol->InsideBlue;

			Plot.OutRGBpoint(iX, iY, inside);
			}
		    else
			{
			// Normal exterior smoothing
			RGBTRIPLE base = GetSmoothedColour(SmoothIterations, ColourSpeed, *TrueCol, &Plot);

			double br = ReflectionLocal;

			RGBTRIPLE final;
			final.rgbtRed	= (BYTE)(base.rgbtRed   * br);
			final.rgbtGreen = (BYTE)(base.rgbtGreen * br);
			final.rgbtBlue	= (BYTE)(base.rgbtBlue  * br);
			// plot using thread-local plot
			if (AbortRequested())
			    return -1;

			// hard bounds check using the dimensions this worker is responsible for
			if ((unsigned)iX >= (unsigned)Dib->DibWidth || (unsigned)iY >= (unsigned)Dib->DibHeight)
			    return -1;

			Plot.OutRGBpoint(iX, iY, final);
			}
		    }
		else
		    Plot.OutRGBpoint(iX, iY, colour);
//		Plot.OutRGBpoint(iX, iY, colour);
		if (ghMutex != NULL)
		    ReleaseMutex(ghMutex);
		}
	    }
	}
    *ThreadComplete = true;
    return 0;
    }

