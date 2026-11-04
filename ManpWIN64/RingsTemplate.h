/*
    RingsTemplate.h - templated Paul Bourke Rings implementation.

    Based on the Tetration work by Paul Bourke:
    http://paulbourke.net/fractals/ringsn/

    ManpWIN templated and extended-precision implementation
    by Paul de Leeuw.
*/


/**************************************************************************
    RingsTemplate.h

    Paul Bourke's Rings fractal, inspired by Roger Bagula (2019).

    z(n+1) = R * z^2 * (z^2 + sigma) / (sigma*z^2 + 1)

    where:
	R     = exp(2*pi*i*phi)
	phi   = (1 + sqrt(5))/2
	sigma = param[0]

    z0 is the pixel coordinate.

    The template supports escape-time and complex orbit-sum
    colouring without changing the underlying iteration formula.
**************************************************************************/

#pragma once

/**************************************************************************
    UseFourCornerRings()

    Determine whether the selected Rings subtype requests
    four-corner orbit-separation rendering.

    Parameter indices are local to each Rings subtype.
**************************************************************************/

inline bool UseFourCornerRings(int subtype, const double* param)
    {
    switch (subtype)
	{
	case 0:                         // Bourke / Bagula Rings
	    return (param[1] == 1.0);

	case 1:                         // Herman Rings
	    return (param[2] == 1.0);

	case 2:                         // Generalised Herman Rings
	    return (param[3] == 1.0);
/*
	case 3:                         // Roger's Rational Rings
	    return (param[1] == 1.0);

	case 4:                         // McMullen Rings
	    return (param[4] == 1.0);
*/
	case 5:                         // Singular Perturbation
	    return (param[6] == 1.0);

	case 6:                         // Shifted-Pole McMullen Rings
	    return (param[8] == 1.0);

	case 8:				// Blaschke Products
	    return (param[3] == 1.0);

	case 9:				// Newton Rational Rings
	    return (param[1] == 1.0);

	case 10:			// Basilica–Rabbit Rational Map
	    return (param[2] == 1.0);

	case 11:			// Bicritical Rational Rings
	    return (param[5] == 1.0);

	case 12:			// Chebyshev–Halley Rings
	    return (param[2] == 1.0);

	case 13:			// Joukowski Rational Rings
	    return param[4] == 1.0;

	case 14:			// Sierpinski Rational Rings
	    return param[5] == 1.0;

	case 15:			// Perturbed Basilica Rings
	    return param[2] == 1.0;

	default:
	    return false;
	}
    }

/**************************************************************************
    UseOrbitSumColouringRings()

    Return the orbit-sum colouring mode for the selected Rings subtype.

    0 = Off
    1 = Orbit-sum palette colouring
    2 = Orbit-sum contour colouring
**************************************************************************/

inline int UseOrbitSumColouringRings(int subtype, const double* param)
    {
    switch (subtype)
	{
	case 0:                         // Bourke / Bagula Rings
	    return (int)param[2];

	case 1:                         // Herman Rings
	    return (int)param[3];

	case 2:                         // Generalised Herman Rings
	    return (int)param[4];

	case 3:                         // Roger's Rational Rings
	    return (int)param[1];

	case 4:                         // McMullen Rings
	    return (int)param[4];

	case 5:				// Singular Perturbation
	    return (int)param[7];

	case 6:                         // Shifted-Pole McMullen Rings
	    return (int)param[9];

	case 8:                         // Blaschke Products
	    return (int)param[4];

	case 9:				// Newton Rational Rings
	    return (int)param[2];

	case 10:			// Basilica–Rabbit Rational Map
	    return (int)param[3];

	case 11:			// Bicritical Rational Rings
	    return (int)param[6];

	case 12:			// Chebyshev–Halley Rings
	    return (int)param[3];

	case 13:			// Joukowski Rational Rings
	    return (int)param[5];

	case 14:			// Sierpinski Rational Rings
	    return (int)param[6];

	case 15:			// Perturbed Basilica Rings
	    return (int)param[3];

	default:
	    return 0;
	}
    }
    
/**************************************************************************
    Init_PBRings()

    Initialise the selected Rings formula for one pixel.

    The subtype is the index into RingSpecific[].

    Set the initial pixel value, clear orbit accumulation and
    calculate the rotation required by the selected formula.

    Rotation is computed once per pixel, not once per iteration.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_PBRings(TComplex* z, TComplex* q, TComplex& orbitSum, TComplex& rotation, int subtype, const double* param)
    {
    *z = *q;

    orbitSum.x = TReal(0.0);
    orbitSum.y = TReal(0.0);

    double angle = 0.0;

    switch (subtype)
	{
	case 0:             // Bourke / Bagula Rings
	    angle = TWO_PI * ((1.0 + sqrt(5.0)) / 2.0);
	    break;

	case 1:             // Herman Rings
	    angle = TWO_PI * param[0];
	    break;

	case 2:             // Generalised Herman Rings
	    angle = TWO_PI * (param[1] - 1.0);
	    break;

	case 8:             // Blaschke Products
	    angle = TWO_PI * param[2];
	    break;

	default:            // Roger, McMullen, Singular Perturbation
	    rotation.x = TReal(1.0);
	    rotation.y = TReal(0.0);
	    return;
	}

    rotation.x = TReal(cos(angle));
    rotation.y = TReal(sin(angle));
    }

/**************************************************************************
    Evaluate one iteration of the Rings formula.

    The rotation must already have been calculated during setup.

    Returns false if the denominator is zero.
**************************************************************************/

template <typename TComplex, typename TReal>
inline bool PBRingsStep(TComplex& z, const TReal& sigma, const TComplex& rotation)
    {
    // w = z*z
    TComplex w = z.CSqr();

    // numerator = w * (w + sigma)
    TComplex factor = w;
    factor.x = factor.x + sigma;

    TComplex numerator = w * factor;

    // denominator = sigma*w + 1
    TComplex denominator;

    denominator.x = sigma * w.x + TReal(1.0);
    denominator.y = sigma * w.y;

    // Pole in the rational map
    if (denominator.x == TReal(0.0) && denominator.y == TReal(0.0))
	return false;

    // Rational part
    TComplex result = numerator / denominator;

    // Apply the golden-ratio rotation
    TComplex next;

    next.x = result.x * rotation.x - result.y * rotation.y;
    next.y = result.x * rotation.y + result.y * rotation.x;

    z = next;

    return true;
    }

/**************************************************************************
    Evaluate one iteration of the Herman Rings formula.

	z(n+1) = rotation * z*z*(z-4) / (1-4*z)

    rotation = exp(2*pi*i*tau)

    The rotation must already have been calculated during setup.

    The classical example uses tau approximately 0.6151732,
    giving a Herman ring with golden-mean rotation number.

    Returns false if the denominator is zero.

    Reference:
    John Milnor, Dynamics in One Complex Variable,
    Herman ring example.
**************************************************************************/

template <typename TComplex, typename TReal>
inline bool PBHermanStep(TComplex& z, const TReal& c, const TComplex& rotation)
    {
    TComplex z2 = z.CSqr();

    TComplex numerFactor = z;
    numerFactor.x = numerFactor.x - c;

    TComplex numerator = z2 * numerFactor;

    TComplex denominator;
    denominator.x = TReal(1.0) - c * z.x;
    denominator.y = -c * z.y;

    if (denominator.x == TReal(0.0) && denominator.y == TReal(0.0))
	return false;

    TComplex result = numerator / denominator;

    TComplex next;
    next.x = result.x * rotation.x - result.y * rotation.y;
    next.y = result.x * rotation.y + result.y * rotation.x;

    z = next;
    return true;
    }

/**************************************************************************
    PBGeneralHermanStep()

    Generalised Herman rational map, degree 3 to 20.

	z(next) = rotation * z^2 *
		  (z-a)/(1-a*z) *
		  [(z-b)/(1-b*z)]^(degree-3)

    Degree 3: one rational factor (a).
    Degree 4: two rational factors (a, b), matching Bourke's
	      second Herman Rings equation.
    Higher degrees: experimental extensions.

    Returns false if a denominator is zero.
**************************************************************************/

template <typename TComplex, typename TReal>
inline bool PBGeneralHermanStep(TComplex& z, int degree, const TReal& a, const TReal& b, const TComplex& rotation)
    {
    if (degree < 3)
	degree = 3;

    if (degree > 20)
	degree = 20;

    TComplex result = z.CSqr();

    // Multiply by the first rational factor, using a,
    // followed by degree-3 factors using b.

    for (int j = 0; j < degree - 2; j++)
	{
	const TReal factor = (j == 0) ? a : b;

	TComplex numerator = z;
	numerator.x = numerator.x - factor;

	TComplex denominator;
	denominator.x = TReal(1.0) - factor * z.x;
	denominator.y = -factor * z.y;

	if (denominator.x == TReal(0.0) && denominator.y == TReal(0.0))
	    return false;

	TComplex quotient = numerator / denominator;
	result = result * quotient;
	}

    // Apply the precomputed rotation.

    TComplex next;
    next.x = result.x * rotation.x - result.y * rotation.y;
    next.y = result.x * rotation.y + result.y * rotation.x;

    z = next;

    return true;
    }

/**************************************************************************
    PBRogerRingsStep()

    Roger Bagula's rational rings, presented by Paul Bourke,
    October 2019.

    Original:
	z(next) = z*z + (|z| + i) / (1 + |z|)

    Generalised:
	z(next) = z*z *
		  (1+g)(1+g^3)(1+g^5)(1+g^7)
		  + (|z| + i) / (1 + |z|)

	g = 1 / (G*z)

    The generalised map has a pole at z=0 or G=0.
    Returns false when the rational calculation is undefined.
**************************************************************************/

template <typename TComplex, typename TReal>
inline bool PBRogerRingsStep(TComplex& z, bool generalised, const TReal& G)
    {
    // Save the magnitude of the original z.

    TReal magnitude = sqrt(z.CSumSqr());
    TReal divisor = TReal(1.0) + magnitude;

    // Common additive term: (|z| + i)/(1 + |z|)

    TComplex additive;
    additive.x = magnitude / divisor;
    additive.y = TReal(1.0) / divisor;

    TComplex next = z.CSqr();

    if (generalised)
	{
	if (G == TReal(0.0) || (z.x == TReal(0.0) && z.y == TReal(0.0)))
	    return false;

	// g = 1/(G*z)

	TComplex denominator;
	denominator.x = G * z.x;
	denominator.y = G * z.y;

	TComplex one;
	one.x = TReal(1.0);
	one.y = TReal(0.0);

	TComplex g = one / denominator;

	// Odd powers: g, g^3, g^5, g^7

	TComplex g2 = g.CSqr();
	TComplex g3 = g2 * g;
	TComplex g5 = g3 * g2;
	TComplex g7 = g5 * g2;

	next = next * (one + g);
	next = next * (one + g3);
	next = next * (one + g5);
	next = next * (one + g7);
	}

    z = next + additive;

    return true;
    }

/**************************************************************************
    PBMcMullenStep()

    McMullen rational map:

	z(next) = z^n + lambda / z^m

    n and m are positive integer powers.
    lambda is a complex parameter.

    For suitable small nonzero lambda, some McMullen maps have
    Julia sets consisting of a Cantor set of circles.

    The map has a pole at z = 0 when lambda != 0.

    Returns false if the denominator is zero.
**************************************************************************/

template <typename TComplex>
inline bool PBMcMullenStep(TComplex& z, int n, int m, const TComplex& lambda)
    {
    if (n < 2 || n > 20 || m < 1 || m > 20)
	return false;

    if (z.x == 0.0 && z.y == 0.0)
	return false;

    // Calculate z^n and z^m using integer multiplication.

    TComplex zn = z;
    TComplex zm = z;

    for (int j = 1; j < n; j++)
	zn = zn * z;

    for (int j = 1; j < m; j++)
	zm = zm * z;

    // Rational term lambda / z^m.

    TComplex numerator = lambda;
    TComplex rational = numerator / zm;

    z = zn + rational;

    return true;
    }

/**************************************************************************
    PBSingularPerturbationStep()

    Singularly perturbed polynomial:

	z(next) = z^n + c + lambda / z^m

    Generalises the McMullen map by adding a complex constant c.

    n, m: positive integer powers (n = 2..20, m = 1..20)
    c: complex additive parameter
    lambda: complex perturbation parameter

    Returns false at the pole z = 0 or for invalid powers.
**************************************************************************/

template <typename TComplex>
inline bool PBSingularPerturbationStep(TComplex& z, int n, int m, const TComplex& c, const TComplex& lambda)
    {
    if (n < 2 || n > 20 || m < 1 || m > 20)
	return false;

    if (z.x == 0.0 && z.y == 0.0)
	return false;

    TComplex zn = z;
    TComplex zm = z;

    for (int j = 1; j < n; j++)
	zn = zn * z;

    for (int j = 1; j < m; j++)
	zm = zm * z;

    // Mutable copies for legacy complex arithmetic operators.
    TComplex numerator = lambda;
    TComplex additive = c;

    TComplex rational = numerator / zm;

    TComplex next = zn + additive + rational;

    // Detect NaN or an excessively large result before storing it.
    auto magnitudeSquared = next.CSumSqr();

    if (!(magnitudeSquared >= 0.0) || !(magnitudeSquared < 1.0e100))
	return false;

    z = next;

    return true;
    }

/**************************************************************************
    PBShiftedMcMullenStep()

    Shifted-pole McMullen rational map:

	z(next) = z^n + c + lambda / (z-a)^m

    n, m: positive integer powers (n = 2..20, m = 1..20)
    c: complex additive parameter
    lambda: complex perturbation parameter
    a: complex position of the pole

    Returns false at the pole z = a or for invalid powers.
**************************************************************************/

template <typename TComplex>
inline bool PBShiftedMcMullenStep(TComplex& z, int n, int m, const TComplex& c, const TComplex& lambda, const TComplex& a)
    {
    if (n < 2 || n > 20 || m < 1 || m > 20)
	return false;

    // The rational term has a pole at z = a.

    TComplex shifted = z;
    shifted.x = shifted.x - a.x;
    shifted.y = shifted.y - a.y;

    if (shifted.x == 0.0 && shifted.y == 0.0)
	return false;

    // Calculate z^n.

    TComplex zn = z;

    for (int j = 1; j < n; j++)
	zn = zn * z;

    // Calculate (z-a)^m.

    TComplex zm = shifted;

    for (int j = 1; j < m; j++)
	zm = zm * shifted;

    // Mutable copies for legacy complex arithmetic operators.

    TComplex numerator = lambda;
    TComplex additive = c;

    TComplex rational = numerator / zm;

    TComplex next = zn + additive + rational;

    z = next;

    return true;
    }

/**************************************************************************
    PBMultiPoleStep()

    Multi-pole rational map:

	z(next) = z^n + c
		+ lambda / (z-a)^m
		+ lambda / (z-b)^m

    n: polynomial degree (2..20)
    m: pole exponent (1..20)
    c: complex additive constant
    lambda: shared complex perturbation parameter
    a, b: complex pole positions

    Uses the same lambda and exponent for both poles.

    Returns false for invalid powers or if an iterate reaches
    either pole.

    When a == b, the two rational terms combine into
    2*lambda/(z-a)^m.
**************************************************************************/

template <typename TComplex>
inline bool PBMultiPoleStep(TComplex& z, int n, int m, const TComplex& c, const TComplex& lambda, const TComplex& a, const TComplex& b)
    {
    if (n < 2 || n > 20 || m < 1 || m > 20)
	return false;

    // Calculate the displacement from each pole.

    TComplex za = z;
    za.x = za.x - a.x;
    za.y = za.y - a.y;

    TComplex zb = z;
    zb.x = zb.x - b.x;
    zb.y = zb.y - b.y;

    // Avoid division by zero at either pole.

    if ((za.x == 0.0 && za.y == 0.0) || (zb.x == 0.0 && zb.y == 0.0))
	return false;

    // Calculate z^n.

    TComplex zn = z;

    for (int j = 1; j < n; j++)
	zn = zn * z;

    // Calculate (z-a)^m and (z-b)^m.

    TComplex zma = za;
    TComplex zmb = zb;

    for (int j = 1; j < m; j++)
	{
	zma = zma * za;
	zmb = zmb * zb;
	}

    // Mutable numerators for legacy complex arithmetic operators.

    TComplex numeratorA = lambda;
    TComplex numeratorB = lambda;
    TComplex additive = c;

    TComplex rationalA = numeratorA / zma;
    TComplex rationalB = numeratorB / zmb;

    // Combine polynomial and both rational contributions.

    TComplex next = zn + additive;
    next = next + rationalA;
    next = next + rationalB;

    z = next;

    return true;
    }

/**************************************************************************
    PBBlaschkeStep()

    Finite Blaschke product of degree 3.

	z(next) = rotation * z *
		  (z-a)/(1-a*z) *
		  (z-b)/(1-b*z)

    a, b: real coefficients, normally between -1 and 1.
    rotation: precomputed complex rotation exp(2*pi*i*tau).

    The unit circle is invariant for valid Blaschke parameters.

    Returns false if either denominator is zero.
**************************************************************************/

template <typename TComplex, typename TReal>
inline bool PBBlaschkeStep(TComplex& z, const TReal& a, const TReal& b, const TComplex& rotation)
    {
    // First rational factor: (z-a)/(1-a*z)

    TComplex numeratorA = z;
    numeratorA.x = numeratorA.x - a;

    TComplex denominatorA;
    denominatorA.x = TReal(1.0) - a * z.x;
    denominatorA.y = -a * z.y;

    if (denominatorA.x == TReal(0.0) &&
	denominatorA.y == TReal(0.0))
	return false;

    TComplex factorA = numeratorA / denominatorA;

    // Second rational factor: (z-b)/(1-b*z)

    TComplex numeratorB = z;
    numeratorB.x = numeratorB.x - b;

    TComplex denominatorB;
    denominatorB.x = TReal(1.0) - b * z.x;
    denominatorB.y = -b * z.y;

    if (denominatorB.x == TReal(0.0) && denominatorB.y == TReal(0.0))
	return false;

    TComplex factorB = numeratorB / denominatorB;

    // Combine the factors.

    TComplex result = z * factorA;
    result = result * factorB;

    // Apply the precomputed complex rotation.

    TComplex next;
    next.x = result.x * rotation.x - result.y * rotation.y;
    next.y = result.x * rotation.y + result.y * rotation.x;

    z = next;

    return true;
    }

/**************************************************************************
    RunRingsStep()

    Execute one iteration of the selected Rings formula.

    The subtype is the index into RingSpecific[].
    Parameters are local to each subtype, starting at param[0].

    Supports DOUBLE, DD, QD and MPFR arithmetic through TComplex/TReal.

    Returns false if a mathematical step fails.

    Orbit accumulation, bailout and iteration counting remain the
    responsibility of the calling iteration engine.
**************************************************************************/

template <typename TComplex, typename TReal>
inline bool Iter_PBRings(TComplex& z, int subtype, const double* param, const TComplex& rotation)
    {
    switch (subtype)
	{
	case 0:                         // Bourke / Bagula Rings
	    {
	    const TReal sigma = TReal(param[0]);
	    return PBRingsStep<TComplex, TReal>(z, sigma, rotation);
	    }

	case 1:                         // Herman Rings
	    {
	    const TReal c = TReal(param[1]);
	    return PBHermanStep<TComplex, TReal>(z, c, rotation);
	    }

	case 2:                         // Generalised Herman Rings
	    {
	    const int degree = (int)param[0];
	    const TReal a = TReal(param[1]);
	    const TReal b = TReal(param[2]);
	    return PBGeneralHermanStep<TComplex, TReal>(z, degree, a, b, rotation);
	    }

	case 3:                         // Generalised Roger's Rational Rings
	    {
	    const TReal G = TReal(param[0]);
	    return PBRogerRingsStep<TComplex, TReal>(z, true, G);
	    }

	case 4:                         // McMullen Rings
	    {
	    const int n = (int)param[0];
	    const int m = (int)param[1];

	    TComplex lambda;
	    lambda.x = TReal(param[2]);
	    lambda.y = TReal(param[3]);

	    return PBMcMullenStep<TComplex>(z, n, m, lambda);
	    }

	case 5:                         // Singular Perturbation
	    {
	    const int n = (int)param[0];
	    const int m = (int)param[1];

	    TComplex c;
	    c.x = TReal(param[2]);
	    c.y = TReal(param[3]);

	    TComplex lambda;
	    lambda.x = TReal(param[4]);
	    lambda.y = TReal(param[5]);

	    return PBSingularPerturbationStep<TComplex>(z, n, m, c, lambda);
	    }

	case 6:                         // Shifted-Pole McMullen
	    {
	    // Local parameters:
	    // param[0] = n
	    // param[1] = m
	    // param[2] = c real
	    // param[3] = c imag
	    // param[4] = lambda real
	    // param[5] = lambda imag
	    // param[6] = a real
	    // param[7] = a imag

	    const int n = (int)param[0];
	    const int m = (int)param[1];

	    TComplex c;
	    c.x = TReal(param[2]);
	    c.y = TReal(param[3]);

	    TComplex lambda;
	    lambda.x = TReal(param[4]);
	    lambda.y = TReal(param[5]);

	    TComplex a;
	    a.x = TReal(param[6]);
	    a.y = TReal(param[7]);

	    return PBShiftedMcMullenStep<TComplex>(z, n, m, c, lambda, a);
	    }

	case 7:                         // Multi-Pole Rational Map
	    {
	    // param[0] = n
	    // param[1] = m
	    // param[2] = c real
	    // param[3] = c imag
	    // param[4] = lambda real
	    // param[5] = lambda imag
	    // param[6] = a real
	    // param[7] = a imag
	    // param[8] = b real
	    // param[9] = b imag

	    const int n = (int)param[0];
	    const int m = (int)param[1];

	    TComplex c;
	    c.x = TReal(param[2]);
	    c.y = TReal(param[3]);

	    TComplex lambda;
	    lambda.x = TReal(param[4]);
	    lambda.y = TReal(param[5]);

	    TComplex a;
	    a.x = TReal(param[6]);
	    a.y = TReal(param[7]);

	    TComplex b;
	    b.x = TReal(param[8]);
	    b.y = TReal(param[9]);

	    return PBMultiPoleStep<TComplex>(z, n, m, c, lambda, a, b);
	    }

	case 8:                         // Blaschke Products
	    {
	    const TReal a = TReal(param[0]);
	    const TReal b = TReal(param[1]);

	    return PBBlaschkeStep<TComplex, TReal>(z, a, b, rotation);
	    }

	case 9:     // Newton Rational Rings
	    {
	    int n = (int)param[0];

	    if (n < 2)
		n = 2;

	    // Calculate z^(n-1).
	    TComplex power;
	    power.x = TReal(1.0);
	    power.y = TReal(0.0);

	    for (int i = 0; i < n - 1; i++)
		power = power * z;

	    // Newton denominator: n * z^(n-1).
	    TComplex denominator = power;

	    denominator.x = denominator.x * TReal(n);
	    denominator.y = denominator.y * TReal(n);

	    // Avoid division by zero at the pole.
	    if (denominator.x == TReal(0.0) && denominator.y == TReal(0.0))
		return false;

	    // Newton numerator: z^n - 1.
	    TComplex numerator = power * z;
	    numerator.x = numerator.x - TReal(1.0);

	    // Newton iteration.
	    z = z - numerator / denominator;

	    return true;
	    }

	case 10:    // Basilica-Rabbit Rational Map
	    {
	    // f(z) = (z^2 - a) / (z^2 - 1)
	    // a = param[0] + i * param[1]

	    TComplex a;
	    a.x = TReal(param[0]);
	    a.y = TReal(param[1]);

	    TComplex z2 = z * z;

	    TComplex numerator = z2;
	    numerator.x = numerator.x - a.x;
	    numerator.y = numerator.y - a.y;

	    TComplex denominator = z2;
	    denominator.x = denominator.x - TReal(1.0);

	    // Poles at z = +1 and z = -1.
	    if (denominator.x == TReal(0.0) && denominator.y == TReal(0.0))
		return false;

	    z = numerator / denominator;

	    return true;
	    }

	case 11:    // Bicritical Rational Rings
	    {
	    // f(z) = (z^n + a) / (b*z^n + 1)
	    //
	    // param[0] = degree n
	    // param[1] = a real
	    // param[2] = a imaginary
	    // param[3] = b real
	    // param[4] = b imaginary

	    int n = (int)param[0];

	    if (n < 2)
		n = 2;

	    TComplex a;
	    a.x = TReal(param[1]);
	    a.y = TReal(param[2]);

	    TComplex b;
	    b.x = TReal(param[3]);
	    b.y = TReal(param[4]);

	    // Calculate z^n.
	    TComplex power;
	    power.x = TReal(1.0);
	    power.y = TReal(0.0);

	    for (int i = 0; i < n; i++)
		power = power * z;

	    // Numerator = z^n + a.
	    TComplex numerator = power + a;

	    // Denominator = b*z^n + 1.
	    TComplex denominator = b * power;
	    denominator.x = denominator.x + TReal(1.0);

	    // Avoid division by zero at a pole.
	    if (denominator.x == TReal(0.0) && denominator.y == TReal(0.0))
		return false;

	    z = numerator / denominator;

	    return true;
	    }

	case 12:    // Chebyshev-Halley Rational Rings
	    {
	    // f(z) = z^n - 1
	    //
	    // param[0] = degree n
	    // param[1] = Chebyshev-Halley parameter alpha
	    //
	    // alpha = 0.0 : Chebyshev
	    // alpha = 0.5 : Halley
	    // alpha = 1.0 : Super-Halley

	    int n = (int)param[0];

	    if (n < 2)
		n = 2;

	    TReal alpha = TReal(param[1]);

	    TComplex one;
	    one.x = TReal(1.0);
	    one.y = TReal(0.0);

	    // Calculate z^(n-1).
	    TComplex power = one;

	    for (int i = 0; i < n - 1; i++)
		power = power * z;

	    // Calculate z^n and f(z).
	    TComplex zn = power * z;
	    TComplex f = zn - one;

	    // f'(z) = n * z^(n-1).
	    TComplex fp = power;
	    fp.x = fp.x * TReal(n);
	    fp.y = fp.y * TReal(n);

	    if (fp.x == TReal(0.0) && fp.y == TReal(0.0))
		return false;

	    // Newton correction: f(z) / f'(z).
	    TComplex correction = f / fp;

	    // For f(z) = z^n - 1:
	    //
	    // L(z) = (n-1) * (z^n - 1) / (n * z^n)

	    TComplex Lnumerator = f;
	    Lnumerator.x = Lnumerator.x * TReal(n - 1);
	    Lnumerator.y = Lnumerator.y * TReal(n - 1);

	    TComplex Ldenominator = zn;
	    Ldenominator.x = Ldenominator.x * TReal(n);
	    Ldenominator.y = Ldenominator.y * TReal(n);

	    if (Ldenominator.x == TReal(0.0) && Ldenominator.y == TReal(0.0))
		return false;

	    TComplex L = Lnumerator / Ldenominator;

	    // Denominator = 1 - alpha * L.
	    TComplex denominator = L;
	    denominator.x = denominator.x * alpha;
	    denominator.y = denominator.y * alpha;

	    denominator = one - denominator;

	    if (denominator.x == TReal(0.0) &&
		denominator.y == TReal(0.0))
		return false;

	    // Factor = 1 + L / (2 * (1 - alpha * L)).
	    denominator.x = denominator.x * TReal(2.0);
	    denominator.y = denominator.y * TReal(2.0);

	    TComplex factor = one + L / denominator;

	    // Chebyshev-Halley iteration.
	    z = z - factor * correction;

	    return true;
	    }

	case 13:    // Joukowski Rational Rings
	    {
	    // f(z) = a * (z + 1/z) + b
	    //
	    // param[0] = a real
	    // param[1] = a imaginary
	    // param[2] = b real
	    // param[3] = b imaginary
	    // param[4] = Four Corner
	    // param[5] = Orbit Sum Colouring

	    TComplex a;
	    a.x = TReal(param[0]);
	    a.y = TReal(param[1]);

	    TComplex b;
	    b.x = TReal(param[2]);
	    b.y = TReal(param[3]);

	    // The map has a pole at z = 0.
	    if (z.x == TReal(0.0) && z.y == TReal(0.0))
		return false;

	    // Calculate 1/z.
	    TComplex one;
	    one.x = TReal(1.0);
	    one.y = TReal(0.0);

	    TComplex reciprocal = one / z;

	    // Apply the Joukowski rational map.
	    z = a * (z + reciprocal) + b;

	    return true;
	    }

	case 14:    // Sierpinski Rational Rings
	    {
	    // f(z) = a * (z + 1/z^2) + b
	    //
	    // param[0] = a real
	    // param[1] = a imaginary
	    // param[2] = b real
	    // param[3] = b imaginary
	    // param[4] = Bailout (reserved)
	    // param[5] = Four Corner
	    // param[6] = Orbit Sum Colouring

	    TComplex a;
	    a.x = TReal(param[0]);
	    a.y = TReal(param[1]);

	    TComplex b;
	    b.x = TReal(param[2]);
	    b.y = TReal(param[3]);

	    // Pole at z = 0.
	    if (z.x == TReal(0.0) && z.y == TReal(0.0))
		return false;

	    TComplex one;
	    one.x = TReal(1.0);
	    one.y = TReal(0.0);

	    // Calculate 1/z^2.
	    TComplex z2 = z * z;

	    if (z2.x == TReal(0.0) && z2.y == TReal(0.0))
		return false;

	    TComplex reciprocal = one / z2;

	    // Apply the rational map.
	    z = a * (z + reciprocal) + b;

	    return true;
	    }

	case 15:    // Perturbed Basilica Rings
	    {
	    // f(z) = z^2 - 1 + lambda/z^2
	    //
	    // param[0] = lambda real
	    // param[1] = lambda imaginary
	    // param[2] = Four Corner
	    // param[3] = Orbit Sum Colouring

	    TComplex lambda;
	    lambda.x = TReal(param[0]);
	    lambda.y = TReal(param[1]);

	    TComplex one;
	    one.x = TReal(1.0);
	    one.y = TReal(0.0);

	    // Calculate z^2.
	    TComplex z2 = z * z;

	    // Pole at z = 0.
	    if (z2.x == TReal(0.0) && z2.y == TReal(0.0))
		return false;

	    // Calculate lambda/z^2.
	    TComplex perturbation = lambda / z2;

	    // Apply the Perturbed Basilica rational map.
	    z = z2 - one + perturbation;

	    return true;
	    }

	default:
	    return false;
	}
    }

/**************************************************************************
    IterateRingsWithColour()

    Execute one Rings iteration and update the selected orbit measurements.

    Colouring modes:
        0 - Normal
        1 - Orbit Sum
        2 - Orbit Sum Contours
        3 - Orbit Trap
        4 - Convergence

    Supports DOUBLE, DD and QD.

    Returns true when iteration should terminate.
**************************************************************************/

template <typename TComplex, typename TReal>
inline bool IterateRingsWithColour(TComplex& z, int subtype, const double* param, const TComplex& rotation, TComplex& orbitSum, int colouringMode, TReal& orbitTrapMin, TReal& minDeltaSqr, double rqlim)
    {
    // Save the orbit position before iteration.
    TComplex prev = z;

    // Perform the selected Rings formula.
    if (!Iter_PBRings<TComplex, TReal>(z, subtype, param, rotation))
        return true;

    // Existing orbit-sum accumulation.
    orbitSum = orbitSum + z;

    // Mode 3: Minimum squared distance to the origin.
    if (colouringMode == 3)
        {
        TReal distanceSquared =
            z.x * z.x + z.y * z.y;

        if (distanceSquared >= TReal(0.0) && distanceSquared < orbitTrapMin)
            orbitTrapMin = distanceSquared;
        }

    // Mode 4: Minimum squared distance between successive orbit points.
    if (colouringMode == 4)
        {
        TReal dx = z.x - prev.x;
        TReal dy = z.y - prev.y;

        TReal deltaSquared = dx * dx + dy * dy;

        if (deltaSquared >= TReal(0.0) && deltaSquared < minDeltaSqr)
            minDeltaSqr = deltaSquared;
        }

    // Preserve the existing bailout behaviour.
    return (z.CSumSqr() > rqlim);
    }

/**************************************************************************
    MeasureFourCornerRings()

    Experimental four-corner orbit-separation mapping.

    Iterates four nearby complex starting points using the selected
    Rings formula.

    Records the maximum separation encountered during iteration,
    rather than only the separation between the final iterates.

    Optionally accumulates the sum of all four evolving orbits.

    orbitSum == nullptr:
        Calculate separation only.

    orbitSum != nullptr:
        Calculate separation and accumulate orbit sums.

    Returns false if a mathematical step fails or a corner escapes.

    No palette indexing or pixel plotting takes place here.
**************************************************************************/

template <typename TComplex, typename TReal>
inline bool MeasureFourCornerRings(const TComplex corners[4], int subtype, const double* param, const TComplex& rotation, int maxIterations, double escapeLimitSquared, double& separation, TComplex* orbitSum = nullptr)
    {
    TComplex orbit[4];

    for (int k = 0; k < 4; k++)
        orbit[k] = corners[k];

    separation = 0.0;

    double maxDistanceSquared = 0.0;

    // Initialise optional orbit-sum accumulator.

    if (orbitSum)
        {
        orbitSum->x = TReal(0.0);
        orbitSum->y = TReal(0.0);
        }

    for (int n = 0; n < maxIterations; n++)
        {
        // Advance all four orbits by one iteration.

        for (int k = 0; k < 4; k++)
            {
            if (!Iter_PBRings<TComplex, TReal>(orbit[k], subtype, param, rotation))
                return false;

            double magnitudeSquared = orbit[k].CSumSqr();

            if (!(magnitudeSquared <= escapeLimitSquared))
                return false;

            // Accumulate the new iterate, not the starting corner.

            if (orbitSum)
                {
                *orbitSum = *orbitSum + orbit[k];
                }
            }

        // Measure maximum pairwise separation after this iteration.

        for (int i = 0; i < 4; i++)
            {
            for (int j = i + 1; j < 4; j++)
                {
                TComplex difference = orbit[i];

                difference.x = difference.x - orbit[j].x;
                difference.y = difference.y - orbit[j].y;

                double distanceSquared = difference.CSumSqr();

                if (!(distanceSquared >= 0.0))
                    return false;

                if (distanceSquared > maxDistanceSquared)
                    maxDistanceSquared = distanceSquared;
                }
            }
        }

    separation = sqrt(maxDistanceSquared);

    return true;
    }

/**************************************************************************
    RingsColourToDouble()

    Convert a real value to double only when required for
    palette indexing or standard floating-point classification.
**************************************************************************/

inline double RingsColourToDouble(double value)
    {
    return value;
    }

inline double RingsColourToDouble(const dd_real& value)
    {
    return to_double(value);
    }

inline double RingsColourToDouble(const qd_real& value)
    {
    return to_double(value);
    }

/**************************************************************************
    ColourRingsOrbitSum()

    Convert orbit-sum magnitude to a palette index.

    Supports DOUBLE, DD and QD arithmetic.

    Mode 1: Original orbit-sum palette colouring.
    Mode 2: Contour-line colouring.

    Mathematical calculations retain TReal precision.
    Conversion to double occurs only for classification and
    final palette indexing.
**************************************************************************/

template <typename TReal>
inline long ColourRingsOrbitSum(const TReal& magnitude, int mode)
    {
    if (!std::isfinite(RingsColourToDouble(magnitude)) || magnitude < TReal(0.0))
	return 0;

    TReal value = log(TReal(1.0) + magnitude) * TReal(4.0);

    if (mode == 2)
	{
	TReal fraction = value - floor(value);

	if (fraction < TReal(0.08) || fraction > TReal(0.92))
	    return 1;
	}

    return (long)RingsColourToDouble(value);
    }

/**************************************************************************
    ColourRingsDistance()

    Colour a Rings orbit measurement using its minimum squared distance.

    Used by:
	Mode 3 - Orbit Trap
	Mode 4 - Convergence

    Supports double, dd_real and qd_real.

    Arithmetic retains TReal precision until the final conversion
    to a palette index.
**************************************************************************/

template <typename TReal>
inline long ColourRingsDistance(const TReal& minDistanceSquared)
    {
    double distance = RingsColourToDouble(minDistanceSquared);

    if (!std::isfinite(distance) || distance < 0.0 || distance == DBL_MAX)
	return 0;

    TReal value = -log(minDistanceSquared + TReal(1.0e-12)) * TReal(8.0);
    double colour = RingsColourToDouble(value);

    if (!std::isfinite(colour))
	return 0;

    if (colour < 0.0)
	colour = 0.0;

    return (long)colour;
    }

/**************************************************************************
    UpdateRingsDistance()

    Update the minimum squared orbit distance.

    Mode 3 - Distance from the origin.
    Mode 4 - Distance between consecutive orbit positions.

    Supports DOUBLE, DD and QD complex arithmetic.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void UpdateRingsDistance(const TComplex& previous, const TComplex& current, int mode, TReal& minimumDistanceSquared)
    {
    if (mode != 3 && mode != 4)
	return;

    TComplex delta = current;

    if (mode == 4)
	{
	delta.x = current.x - previous.x;
	delta.y = current.y - previous.y;
	}

    TReal distanceSquared = delta.x * delta.x + delta.y * delta.y;

    if (distanceSquared >= TReal(0.0) && distanceSquared < minimumDistanceSquared)
	minimumDistanceSquared = distanceSquared;
    }



