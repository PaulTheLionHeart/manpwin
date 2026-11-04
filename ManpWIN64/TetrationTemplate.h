/*
    TetrationTemplate.h - templated Paul Bourke Tetration implementation.

    Based on the Tetration work by Paul Bourke:
    http://paulbourke.net/fractals/tetration/

    ManpWIN templated and extended-precision implementation
    by Paul de Leeuw.
*/

/**************************************************************************
    Logarithm helpers for Paul Bourke Tetration.
**************************************************************************/

inline double PBTetrationLog(double x)
    {
    return log(x);
    }

inline dd_real PBTetrationLog(dd_real& x)
    {
    return log(x);
    }

inline qd_real PBTetrationLog(qd_real& x)
    {
    return log(x);
    }

/**************************************************************************
    Tetration natural logarithm - native MPFR
**************************************************************************/

inline BigDouble PBTetrationLog(BigDouble& x)
    {
    BigDouble result;
    result.MatchPrecision(x);

    mpfr_log(result.x, x.x, MPFR_RNDN);

    return result;
    }


/**************************************************************************
    Tetration sine and cosine - native MPFR
**************************************************************************/

inline void PBTetrationSinCos(BigDouble* sine,
    BigDouble* cosine,
    BigDouble angle)
    {
    if (!sine || !cosine)
	return;

    sine->MatchPrecision(angle);
    cosine->MatchPrecision(angle);

    mpfr_sin_cos(sine->x, cosine->x, angle.x, MPFR_RNDN);
    }

/**************************************************************************
    Exponential helpers for Paul Bourke Tetration.
**************************************************************************/

inline double PBTetrationExp(double x)
    {
    return exp(x);
    }

inline dd_real PBTetrationExp(dd_real& x)
    {
    return exp(x);
    }

inline qd_real PBTetrationExp(qd_real& x)
    {
    return exp(x);
    }

/**************************************************************************
    Tetration exponential - native MPFR
**************************************************************************/

inline BigDouble PBTetrationExp(BigDouble& x)
    {
    BigDouble result;
    result.MatchPrecision(x);

    mpfr_exp(result.x, x.x, MPFR_RNDN);

    return result;
    }

/**************************************************************************
    Initialise Paul Bourke Tetration.

    Mandelbrot:
	z0 = pixel + (param[0], param[1])

    Julia:
	preserve the incoming z

    The base is also converted to polar form once, matching the
    original Paul Bourke implementation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_PBTetration(CPixel* self, TComplex* z, TComplex* q, double* param, bool juliaflag, TComplex& base, TReal& baseRadius, TReal& baseTheta, int& bailoutMethod)
    {
    if (!juliaflag)
	{
	z->x = q->x + TReal(param[0]);
	z->y = q->y + TReal(param[1]);
	}

    base = *z;

    baseRadius = sqrt(base.x * base.x + base.y * base.y);
    baseTheta = PBTetrationAtan2(base.y, base.x);

    bailoutMethod = (int)param[2];
    }

/**************************************************************************
    Reduce the Paul Bourke Tetration angle to [-pi, pi].

    Tetration can produce extremely large phase values.  Only the phase
    modulo 2*pi is required by sin() and cos(), so reduce it before
    passing it to the trigonometric routines.
**************************************************************************/

inline void PBTetrationReduceAngle(double& theta)
    {
    theta = fmod(theta, TWO_PI);

    if (theta > PI)
	theta -= TWO_PI;
    else if (theta < -PI)
	theta += TWO_PI;
    }

inline void PBTetrationReduceAngle(dd_real& theta)
    {
    dd_real k = floor(theta / dd_real::_2pi);

    theta -= k * dd_real::_2pi;

    if (theta > dd_real::_pi)
	theta -= dd_real::_2pi;
    else if (theta < -dd_real::_pi)
	theta += dd_real::_2pi;
    }

inline void PBTetrationReduceAngle(qd_real& theta)
    {
    qd_real k = floor(theta / qd_real::_2pi);

    theta -= k * qd_real::_2pi;

    if (theta > qd_real::_pi)
	theta -= qd_real::_2pi;
    else if (theta < -qd_real::_pi)
	theta += qd_real::_2pi;
    }

/**************************************************************************
    Reduce a Tetration angle to [-pi, pi].
    Native MPFR arithmetic.
**************************************************************************/

inline void PBTetrationReduceAngle(BigDouble& theta)
    {
    mpfr_prec_t prec = mpfr_get_prec(theta.x);

    BigDouble twoPi;
    BigDouble minusPi;
    twoPi.MatchPrecision(theta);
    minusPi.MatchPrecision(theta);

    mpfr_const_pi(twoPi.x, MPFR_RNDN);
    mpfr_mul_ui(twoPi.x, twoPi.x, 2, MPFR_RNDN);

    // Reduce modulo 2*pi.
    mpfr_fmod(theta.x, theta.x, twoPi.x, MPFR_RNDN);

    // Bring the result into [-pi, pi].
    mpfr_div_ui(minusPi.x, twoPi.x, 2, MPFR_RNDN);

    if (mpfr_cmp(theta.x, minusPi.x) > 0)
	mpfr_sub(theta.x, theta.x, twoPi.x, MPFR_RNDN);

    mpfr_neg(minusPi.x, minusPi.x, MPFR_RNDN);

    if (mpfr_cmp(theta.x, minusPi.x) < 0)
	mpfr_add(theta.x, theta.x, twoPi.x, MPFR_RNDN);
    }

/**************************************************************************
    Sine / cosine helper for Paul Bourke Tetration.
**************************************************************************/

inline void PBTetrationSinCos(double* s, double* c, double angle)
    {
    *s = sin(angle);
    *c = cos(angle);
    }

inline void PBTetrationSinCos(dd_real* s, dd_real* c, dd_real angle)
    {
    CDDTrig trig;

    trig.sincos_dd(s, c, angle);
    }

inline void PBTetrationSinCos(qd_real* s, qd_real* c, qd_real angle)
    {
    CQDTrig trig;

    trig.sincos_qd(s, c, angle);
    }

/**************************************************************************
    atan2 helper for Paul Bourke Tetration.
**************************************************************************/

template <typename TReal>
inline TReal PBTetrationAtan2(TReal& y, TReal& x)
    {
    return atan2(y, x);
    }

/**************************************************************************
    Tetration atan2 - native MPFR
**************************************************************************/

inline BigDouble PBTetrationAtan2(BigDouble& y, BigDouble& x)
    {
    BigDouble angle;

    mpfr_prec_t prec = mpfr_get_prec(x.x);
    if (mpfr_get_prec(y.x) > prec)
	prec = mpfr_get_prec(y.x);

    mpfr_set_prec(angle.x, prec);

    mpfr_atan2(angle.x, y.x, x.x, MPFR_RNDN);

    return angle;
    }

/**************************************************************************
    Paul Bourke Tetration bailout test.

    This preserves the original Tetration.cpp behaviour:

    0 = real part
    1 = imaginary part
    2 = absolute real or imaginary part
    3 = magnitude
    4 = real or imaginary part
    5 = theta

    Note that mode 4 is deliberately NOT abs(x) / abs(y);
    it preserves the original code exactly.
**************************************************************************/

template <typename TComplex, typename TReal>
inline BOOL TetrationBailoutT(TComplex& z, int bailoutMethod, TReal rqlim)
    {
    switch (bailoutMethod)
	{
	case 5:     // theta
	    return (PBTetrationAtan2(z.y, z.x) > rqlim);

	case 4:     // both (signed)
	    return (z.x > rqlim || z.y > rqlim);

	case 3:     // magnitude
	    {
	    TReal magnitude = z.CSumSqr();
	    TReal limit = rqlim * rqlim;
	    return (magnitude > limit);
	    }

	case 2:     // abs(real) or abs(imag)
	    return (fabs(z.x) > rqlim || fabs(z.y) > rqlim);

	case 1:     // imag
	    return (z.y > rqlim);

	case 0:
	default:    // real
	    return (z.x > rqlim);
	}
    }

/**************************************************************************
    Paul Bourke complex power.

    The base is supplied in polar form:

	r = |base|
	t = arg(base)

    For exponent z = x + iy:

	log(result radius) = x*log(r) - y*t
	result angle       = y*log(r) + x*t

    The angle is reduced before evaluating sine and cosine so that
    extremely large tetration phases do not overwhelm the trig routines.
**************************************************************************/

template <typename TComplex, typename TReal>
inline TComplex PBTetrationPower(TReal& baseRadius, TReal& baseTheta, TComplex& exponent)
    {
    TComplex result;

    TReal logRadius = PBTetrationLog(baseRadius);

    TReal magnitudeExponent = exponent.x * logRadius - exponent.y * baseTheta;
    TReal radius = PBTetrationExp(magnitudeExponent);
    TReal theta = exponent.y * logRadius + exponent.x * baseTheta;

    PBTetrationReduceAngle(theta);

    TReal sinTheta;
    TReal cosTheta;

    PBTetrationSinCos(&sinTheta, &cosTheta, theta);

    result.x = radius * cosTheta;
    result.y = radius * sinTheta;

    return result;
    }

/**************************************************************************
    Run Paul Bourke Tetration for one pixel.

    Successive tetration heights are generated incrementally, while
    complex powers use the original Paul Bourke polar formulation.

	n = 0	1
	n = 1	a^a
	n = 2	a^(a^a)
	n = 3	a^(a^(a^a))
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_PBTetration(CPixel* self, TComplex& base, TReal& baseRadius, TReal& baseTheta, int bailoutMethod, long* iteration, long threshold, TReal rqlim)
    {
    TComplex b;
    TComplex unity;

    unity.x = TReal(1.0);
    unity.y = TReal(0.0);

    long n;

    for (n = 0; n < threshold; n++)
	{
	if (n == 0)
	    {
	    b = unity;
	    }
	else if (n == 1)
	    {
	    b = PBTetrationPower<TComplex, TReal>(baseRadius, baseTheta, base);
	    }
	else
	    {
	    b = PBTetrationPower<TComplex, TReal>(baseRadius, baseTheta, b);
	    }

	if (TetrationBailoutT<TComplex, TReal>(b, bailoutMethod, rqlim))
	    break;
	}

    *iteration = n;

    return 1;
    }
    
