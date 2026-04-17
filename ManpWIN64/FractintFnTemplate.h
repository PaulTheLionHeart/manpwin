// FractintFnTemplate.h

#include <windows.h>
#include <functional>
#include <cmath>

#pragma once

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Newton(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    double* param,
    WORD* degree,
    int& subtype,
    TComplex* roots,
    TReal& thresh,
    TReal pi,
    int& period_level,
    long& color,
    bool juliaflag)
    {
    // degree
    *degree = (WORD)param[0];
    if (*degree < 2)
	*degree = 3;

    // subtype (leave as control logic)
    switch ((int)param[1])
	{
	case 0: subtype = 'N'; break;
	case 1: subtype = 'S'; break;
	case 2: subtype = 'B'; break;
	default: subtype = 'N'; break;
	}

    // threshold
    thresh = TReal(0.3) * pi / TReal(*degree);

    if (subtype == 'S' || subtype == 'B')
	{
	if (*degree > MAXROOTS)
	    *degree = MAXROOTS;

	for (int i = 0; i < *degree; i++)
	    {
	    // TEMP: double-based trig (safe fallback)

	    TReal angle = TReal(i) * TReal(2.0) * pi / TReal(*degree);

	    roots[i].x = cos(angle);
	    roots[i].y = sin(angle);

	    if (subtype == 'S')
		{
		TComplex diff = roots[i] - (*z);
		TReal dist2 = diff.CSumSqr();
		}
	    }
	}

    period_level = false;
    color = 0;

    if (!juliaflag)
	*z = (*q) / TReal(3);
    }

template <typename TComplex, typename TReal>
inline void Init_ComplexMarksMand(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    TComplex* MarkCoeff)
    {
    if (!juliaflag)
	{
	z->x = q->x + TReal(param[0]);
	z->y = q->y + TReal(param[1]);
	}

    TComplex pwr;
    pwr.x = TReal(param[2]) - TReal(1.0);
    pwr.y = TReal(param[3]);

    *MarkCoeff = (*q) ^ pwr;
    }

template <typename TComplex, typename TReal>
inline void Init_Spider(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    TComplex& temp)
    {
    if (!juliaflag)
	{
	z->x = q->x + TReal(param[0]);
	z->y = q->y + TReal(param[1]);
	}

    temp = *q;
    }

template <typename TComplex, typename TReal>
inline void Init_Manowar(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex* c,
    double* param,
    bool juliaflag,
    bool invert,
    TComplex& t,
    TComplex& temp,
    TComplex& temp1)
    {
    // No periodicity
    // (you already handle this globally, so nothing needed here)

    t = (invert) ? InvertZ2<TComplex, TReal>(*c, TReal(self->f_xcenter), TReal(self->f_ycenter), TReal(self->f_radius)) : *c;

    if (juliaflag)
	{
	*z = t;
	temp = *q;
	}
    else
	{
	z->x = t.x + TReal(param[0]);
	z->y = t.y + TReal(param[1]);
	temp = *z;
	}

    temp1 = *z;

    // sqr NOT needed here
    }

template <typename TComplex, typename TReal>
inline void Init_ComplexNewton(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    double* param,
    TComplex& croot,
    TComplex& cdegree,
    int& subtype)
    {
    croot.x = TReal(param[2]);
    croot.y = TReal(param[3]);

    cdegree.x = TReal(param[0]);
    cdegree.y = TReal(param[1]);

    *z = *q;

    subtype = ((int)param[4] == 0) ? 'N' : 'B';
    }

template <typename TComplex, typename TReal>
inline void Init_Lambda(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    double* param,
    bool juliaflag,
    TComplex& temp)
    {
    // no periodicity

    if (juliaflag)
	{
	temp = *q;
	}
    else
	{
	z->x = q->x + TReal(param[0]);
	z->y = q->y + TReal(param[1]);
	temp = *z;
	z->x = TReal(0.5);
	z->y = TReal(0.0);
	}

    // sqr NOT needed
    }

template <typename TComplex, typename TReal>
inline void Init_ZPowerGroup(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& c,
    double* param,
    bool juliaflag,
    bool invert,
    TComplex& t,
    TComplex& temp,
    TComplex& temp1,
    TComplex& temp2)
    {
    t = (invert)
	? InvertZ2<TComplex, TReal>(
	    c,
	    TReal(self->f_xcenter),
	    TReal(self->f_ycenter),
	    TReal(self->f_radius))
	: c;

    temp.x = TReal(param[0]);
    temp.y = TReal(param[1]);

    temp2.x = TReal(param[2]);
    temp2.y = TReal(param[3]);

    // no periodicity

    if (juliaflag)
	{
	temp = *q;
	}
    else
	{
	z->x = t.x + TReal(param[0]);
	z->y = t.y + TReal(param[1]);
	}

    temp1 = *z;
    }

template <typename TComplex, typename TReal>
inline void Init_Marks(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& c,
    double* param,
    bool juliaflag,
    bool invert,
    TComplex& t,
    TComplex& MarkCoeff)
    {
    t = (invert)
	? InvertZ2<TComplex, TReal>(
	    c,
	    TReal(self->f_xcenter),
	    TReal(self->f_ycenter),
	    TReal(self->f_radius))
	: c;

    if (juliaflag)
	{
	t = *q;
	}
    else
	{
	z->x = t.x + TReal(param[0]);
	z->y = t.y + TReal(param[1]);
	}

    if (param[2] == 4.0)
	{
	q->y = q->y + 1.0e-24;
	}

    // MPFR MarksMandel appears unstable for certain isolated powers at very deep zooms(around 1e-55 and beyond).
    // Other arithmetic modes and nearby powers behave normally.Deferred for later investigation.

    TReal pwr;
    pwr = TReal(param[2]) - TReal(1.0);
    MarkCoeff = (*q) ^ pwr;
    }

template <typename TComplex, typename TReal>
inline void Init_Quat(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& c,
    double* param,
    bool juliaflag,
    bool invert,
    TComplex& t,
    TReal& qc,
    TReal& qci,
    TReal& qcj,
    TReal& qck)
    {
    t = (invert)
	? InvertZ2<TComplex, TReal>(
	    c,
	    TReal(self->f_xcenter),
	    TReal(self->f_ycenter),
	    TReal(self->f_radius))
	: c;

    if (juliaflag)
	{
	*z = t;
	qc = q->x;
	qci = q->y;
	}
    else
	{
	z->x = TReal(0);
	z->y = TReal(0);
	qc = t.x;
	qci = t.y;
	}

    qcj = TReal(param[2]);
    qck = TReal(param[3]);
    }









// ------------------------------------------------------------
// Template version of RunFunctions (first step: Mandel only)
// ------------------------------------------------------------

template <typename TComplex, typename TReal>
inline int Iter_Newton(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& z1,
    TComplex& z2,
    TComplex& zd,
    long* iteration,
    WORD* degree,
    int subtype,
    TComplex* roots,
    TReal thresh,
    long& color,
    long threshold)
    {
    int tmpcolor;
    int i;

    color = *iteration;

    z2 = *z;

    z1 = z->CPolynomial(*degree - 1);

    *z = *z - (z1 * *z - *q - TReal(1)) / (z1 * TReal(*degree));

    zd = *z - z2;

    TReal d = zd.CSumSqr();

    if (d < TReal(MINSIZE))
	{
	if (subtype == 'S' || subtype == 'B')
	    {
	    tmpcolor = -1;

	    for (i = 0; i < *degree; i++)
		{
		if (distance(roots[i], z2) < thresh)
		    {
		    if (subtype == 'S')
			tmpcolor = 1 + (i & 7) + ((color & 1) << 3);
		    else
			tmpcolor = 1 + i;

		    break;
		    }
		}

	    if (tmpcolor == -1)
		color = threshold;
	    else
		color = tmpcolor;
	    }

	return TRUE;
	}

    return FALSE;
    }

template <typename TComplex, typename TReal>
inline int Iter_ComplexMarksMandelbrot(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& sqr,
    TReal& realimag,
    TComplex& coefficient,
    int bailout_type,
    TReal rqlim)
    {
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    realimag = z->x * z->y;

    z->x = sqr.x - sqr.y;
    z->y = realimag + realimag;

    *z = coefficient * (*z) + (*q) *coefficient;

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim);
    }

template <typename TComplex, typename TReal>
inline int Iter_Spider(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& temp,
    int bailout_type,
    TReal rqlim)
    {
    TComplex sqr;
    TReal realimag;

    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    realimag = z->x * z->y;

    z->x = sqr.x - sqr.y + temp.x;
    z->y = realimag + realimag + temp.y;

    temp.x = temp.x / TReal(2) + z->x;
    temp.y = temp.y / TReal(2) + z->y;

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim);
    }

template <typename TComplex, typename TReal>
inline int Iter_ManOWar(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& temp,
    TComplex& temp1,
    TComplex& temp3,
    double* param,
    int bailout_type,
    TReal rqlim)
    {
    TComplex sqr;

    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    temp3.x = sqr.x - sqr.y + temp.x + temp1.x + param[0];
    temp3.y = (z->x * z->y + z->x * z->y) + temp.y + temp1.y + param[1];

    temp1 = *z;

    *z = temp3;

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim);
    }

inline double RealToDouble(double x)
    {
    return x;
    }

inline double RealToDouble(const dd_real& x)
    {
    return to_double(x);
    }

inline double RealToDouble(const qd_real& x)
    {
    return to_double(x);
    }

inline double RealToDouble(const BigDouble& x)
    {
    return x.BigDoubleToDouble();
    }

template <typename TComplex, typename TReal>
inline int Iter_ComplexNewton(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& croot,
    TComplex& cdegree,
    long* iteration,
    int subtype,
    long& color)
    {
    TComplex tmp, cd1, New, temp;

    TReal mod;
    long coloriter;
//    TReal MPthreshold = TReal(0.001);
    double MPthreshold = 0.001;

    color = *iteration;

    cd1.x = cdegree.x - TReal(1);
    cd1.y = cdegree.y;

    temp = (*z) ^ cd1;

    New = temp * (*z);

    tmp = New - croot;

    if (tmp.CSumSqr() < MPthreshold)
	{
	if (subtype == 'N')
	    return 1;

	if (fabs(z->y) < TReal(0.01))
	    z->y = TReal(0);

	temp = z->CLog();

	tmp = temp * cdegree;

	mod = tmp.y / TReal(TWO_PI);

//	coloriter = (long)to_double(mod);
	coloriter = (long)RealToDouble(mod);

	if (fabs(mod - TReal(coloriter)) > TReal(0.5))
	    {
	    if (mod < TReal(0))
		coloriter--;
	    else
		coloriter++;
	    }

	coloriter += 2;

	if (coloriter < 0)
	    coloriter += 128;

	*iteration = coloriter;

	return 1;
	}

    tmp = New * cd1;
    tmp = tmp + croot;

    cd1 = temp * cdegree;

    *z = tmp / cd1;

    return 0;
    }

template <typename TComplex, typename TReal>
inline int Iter_Lambda(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& temp,
    int bailout_type,
    TReal rqlim)
    {
    TComplex sqr;

    // precalc old squares
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    // reuse sqr exactly like original code
    sqr.x = z->x - sqr.x + sqr.y;
    sqr.y = -(z->y * z->x);
    sqr.y += sqr.y + z->y;

    TReal newx = temp.x * sqr.x - temp.y * sqr.y;
    TReal newy = temp.x * sqr.y + temp.y * sqr.x;

    z->x = newx;
    z->y = newy;

    // recompute for bailout
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim);
    }

template <typename TComplex, typename TReal>
inline int Iter_ZPower(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& temp2,
    int bailout_type,
    TReal rqlim)
    {
    *z = (*z) ^ temp2;
    *z = (*z) + (*q);

    return DoBailout(bailout_type, z, rqlim);
    }

template <typename TComplex, typename TReal>
inline int Iter_ZtoZPlusZPower(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& temp,
    double* param,
    int bailout_type,
    TReal rqlim)
    {
    temp = z->CPolynomial((int)param[2]);

    *z = (*z) ^ (*z);
    *z = temp + (*z) + (*q);

    return DoBailout(bailout_type, z, rqlim);
    }

template <typename TComplex, typename TReal>
inline int Iter_MarksMandel(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    TComplex& temp,
    TComplex& temp1,
    TComplex& coefficient,
    TComplex& t,
    int bailout_type,
    TReal rqlim)
    {
    TComplex sqr;

    temp = 0.0;
    temp1 = 0.0;
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    temp.x = sqr.x - sqr.y;
    temp.y = (z->x * z->y) + (z->x * z->y);

    temp1.x = coefficient.x * temp.x - coefficient.y * temp.y + t.x;
    temp1.y = coefficient.x * temp.y + coefficient.y * temp.x + t.y;

    *z = temp1;

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim);
    }

template <typename TComplex, typename TReal>
inline int Iter_Quat(
    CPixel* self,
    TComplex* z,
    TComplex& temp,
    TReal qc,
    TReal qci,
    TReal qcj,
    TReal qck,
    TReal rqlim)
    {
    TReal a0 = z->x;
    TReal a1 = z->y;
    TReal a2 = temp.x;
    TReal a3 = temp.y;

    TReal n0 = a0 * a0 - a1 * a1 - a2 * a2 - a3 * a3 + qc;
    TReal n1 = (a0*a1 + a0 * a1) + qci;
    TReal n2 = (a0*a2 + a0 * a2) + qcj;
    TReal n3 = (a0*a3 + a0 * a3) + qck;

    TReal magnitude = a0 * a0 + a1 * a1 + a2 * a2 + a3 * a3;

    if (magnitude > rqlim)
	return 1;

    z->x = n0;
    z->y = n1;
    temp.x = n2;
    temp.y = n3;

    return 0;
    }

template <typename TComplex, typename TReal>
inline int Iter_Tetrate(
    CPixel* self,
    TComplex* z,
    TComplex* q,
    int bailout_type,
    TReal rqlim)
    {
    *z = (*q) ^ (*z);

    return DoBailout(bailout_type, z, rqlim);
    }


