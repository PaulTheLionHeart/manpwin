/*
    FractintFnTemplate.H - Template for handling Fractint Functions for all arithmetic types.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <functional>
#include <cmath>

#pragma once

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

/**************************************************************************
    Initialise Newton fractals.

    Newton solves:
	F(z) = z^degree - 1 = 0

    The subtype controls ordinary Newton rendering or root/basin colouring.
    For basin modes, calculate the roots of unity used to identify which
    root each orbit converges to.

    Mandelbrot-style operation starts the orbit at pixel / 3.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Newton(CPixel* self, TComplex* z, TComplex* q, double* param, WORD* degree, int& subtype, TComplex* roots, TReal& thresh, TReal pi, int& period_level, long& color, bool juliaflag)
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

/**************************************************************************
    Initialise Barnsley fractals.

    Mandelbrot:
	z = pixel + initial perturbation
	temp = pixel

    Julia:
	z = pixel
	temp = Julia constant
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Barnsley(CPixel* self, TComplex* z, TComplex* q, TComplex& c, double* param, bool juliaflag, TComplex& temp)
    {
    temp = *q;

    if (juliaflag)
	*z = c;
    else
	{
	z->x = q->x + TReal(param[0]);
	z->y = q->y + TReal(param[1]);
	}
    }

/**************************************************************************
    Initialise Phoenix and MandelPhoenix fractals.

    Handles both real and complex parameter versions.

    temp   = first Phoenix parameter
    temp2  = second Phoenix parameter
    temp3  = previous z, initially zero

    PhoenixDegree and PhoenixType are derived from the Fractint degree
    parameter.  Symmetry is also set here for the complex parameter form.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Phoenix(CPixel* self, TComplex* z, TComplex* q, TComplex& c, double* param, bool juliaflag, WORD type, TComplex& t, TComplex& temp, TComplex& temp2, TComplex& temp3, int& PhoenixDegree, int& PhoenixType, int* symmetry, bool ComplexPhoenix)
    {
    t = TransformPointT<TComplex, TReal>(c, self->TransformType, double(self->f_radius), double(self->f_xcenter), double(self->f_ycenter));

    temp.x = TReal(param[0]);
    temp.y = TReal(param[1]);
    temp2.x = TReal(param[2]);
    temp2.y = TReal(param[3]);

    if (juliaflag)
	{
	temp = *q;
	}
    else if (ComplexPhoenix)
	{
	z->x = t.x + TReal(param[0]);
	z->y = t.y + TReal(param[1]);
	}
    else
	{
	if (type == MANDPHOENIXFP)
	    *z = temp = t;
	else
	    *z = t;
	}

    temp3.x = TReal(0.0);
    temp3.y = TReal(0.0);

    PhoenixDegree = ComplexPhoenix ? (int)param[4] : (int)param[2];

    if (PhoenixDegree < 2 && PhoenixDegree > -3)
	PhoenixDegree = 0;

    if (ComplexPhoenix)
	param[4] = (double)PhoenixDegree;
    else
	param[2] = (double)PhoenixDegree;

    if (PhoenixDegree == 0)
	{
	if (ComplexPhoenix)
	    {
	    if (temp2.x != TReal(0.0) || temp2.y != TReal(0.0))
		*symmetry = NOSYM;
	    else
		*symmetry = ORIGIN;

	    if (temp.y == TReal(0.0) && temp2.y == TReal(0.0))
		*symmetry = XAXIS;
	    }

	PhoenixType = ZERO;
	}
    else if (PhoenixDegree >= 2)
	{
	PhoenixDegree--;

	if (ComplexPhoenix)
	    {
	    if (temp.y == TReal(0.0) && temp2.y == TReal(0.0))
		*symmetry = XAXIS;
	    else
		*symmetry = NOSYM;
	    }

	PhoenixType = PLUS;
	}
    else
	{
	PhoenixDegree = abs(PhoenixDegree);
	PhoenixDegree -= 2;

	if (ComplexPhoenix)
	    {
	    if (temp.y == TReal(0.0) && temp2.y == TReal(0.0))
		*symmetry = XAXIS;
	    else
		*symmetry = NOSYM;
	    }

	PhoenixType = MINUS;
	}
    }

/**************************************************************************
    Initialise complex Phoenix and MandelPhoenix fractals.

    param[0], param[1] = first complex parameter / initial perturbation
    param[2], param[3] = second complex parameter p2
    param[4]           = degree selector

    temp   = first complex parameter
    temp2  = second complex parameter
    temp3  = previous z, initially zero

    Julia uses the Julia constant as the first complex parameter.
    MandelPhoenix starts at the transformed pixel plus the initial
    perturbation.

    Degree:
	0, 1, -1, -2 -> ZERO
	>= 2         -> PLUS
	<= -3        -> MINUS
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_PhoenixCplx(CPixel* self, TComplex* z, TComplex* q, TComplex& c, double* param, bool juliaflag, TComplex& t, TComplex& temp, TComplex& temp2, 
	TComplex& temp3, int& PhoenixDegree, int& PhoenixType, int* symmetry)
    {
    t = TransformPointT<TComplex, TReal>(c, self->TransformType, double(self->f_radius), double(self->f_xcenter), double(self->f_ycenter));

    temp.x = TReal(param[0]);
    temp.y = TReal(param[1]);

    temp2.x = TReal(param[2]);
    temp2.y = TReal(param[3]);

    if (juliaflag)
	{
	temp = *q;
	}
    else
	{
	z->x = t.x + TReal(param[0]);
	z->y = t.y + TReal(param[1]);
	}

    temp3.x = TReal(0.0);
    temp3.y = TReal(0.0);

    PhoenixDegree = (int)param[4];

    if (PhoenixDegree < 2 && PhoenixDegree > -3)
	PhoenixDegree = 0;

    param[4] = (double)PhoenixDegree;

    if (PhoenixDegree == 0)
	{
	if (temp2.x != TReal(0.0) || temp2.y != TReal(0.0))
	    *symmetry = NOSYM;
	else
	    *symmetry = ORIGIN;

	if (temp.y == TReal(0.0) && temp2.y == TReal(0.0))
	    *symmetry = XAXIS;

	PhoenixType = ZERO;
	}
    else if (PhoenixDegree >= 2)
	{
	PhoenixDegree--;

	if (temp.y == TReal(0.0) && temp2.y == TReal(0.0))
	    *symmetry = XAXIS;
	else
	    *symmetry = NOSYM;

	PhoenixType = PLUS;
	}
    else
	{
	PhoenixDegree = abs(PhoenixDegree);
	PhoenixDegree -= 2;

	if (temp.y == TReal(0.0) && temp2.y == TReal(0.0))
	    *symmetry = XAXIS;
	else
	    *symmetry = NOSYM;

	PhoenixType = MINUS;
	}
    }

/**************************************************************************
    Initialise Fractint Magnet 1 and Magnet 2.

    Mandelbrot: z = 0, c = pixel coordinate.
    Julia:      preserve the Julia starting point.

    The Magnet recurrence uses c directly; no additional
    initialisation coefficients are required.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_MagnetFractal(CPixel* self, TComplex* z, TComplex* q, double* param, bool juliaflag)
    {
    if (!juliaflag)
	{
	z->x = TReal(param[0]);
	z->y = TReal(param[1]);
	}
    }

/**************************************************************************
    Initialise Complex Marks Mandelbrot.

    Mandelbrot:
	z(0) = pixel + initial perturbation

    The coefficient is:
	coefficient = c^(p - 1)

    where p is the complex exponent supplied by param[2] and param[3].
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_ComplexMarksMand(CPixel* self, TComplex* z, TComplex* q, double* param, bool juliaflag, TComplex* MarkCoeff)
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

/**************************************************************************
    Initialise Spider fractal.

    Mandelbrot:
	z(0) = pixel + initial perturbation

    temp stores the evolving c value used by the Spider recurrence:

	z(n+1) = z(n)^2 + c(n)
	c(n+1) = c(n) / 2 + z(n+1)
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Spider(CPixel* self, TComplex* z, TComplex* q, double* param, bool juliaflag, TComplex& temp)
    {
    if (!juliaflag)
	{
	z->x = q->x + TReal(param[0]);
	z->y = q->y + TReal(param[1]);
	}

    temp = *q;
    }

/**************************************************************************
    Initialise Man O' War fractal.

    temp stores the constant term established for the orbit.
    temp1 stores the previous orbit value required by the recurrence.

    Julia uses the Julia constant directly.
    Mandelbrot applies the transformed pixel and initial perturbation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Manowar(CPixel* self, TComplex* z, TComplex* q, TComplex& c, double* param, bool juliaflag, TComplex& t, TComplex& temp, TComplex& temp1)
    {
    // No periodicity
    // (you already handle this globally, so nothing needed here)

    t = TransformPointT<TComplex, TReal>(c, self->TransformType, double(self->f_radius), double(self->f_xcenter), double(self->f_ycenter));
    
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

/**************************************************************************
    Initialise Complex Newton fractal.

    Solves:
	z^degree = root

    Both degree and root may be complex.

    param[0], param[1] = complex degree
    param[2], param[3] = complex root
    param[4]           = Newton or basin colouring
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_ComplexNewton(CPixel* self, TComplex* z, TComplex* q, double* param, TComplex& croot, TComplex& cdegree, int& subtype)
    {
    croot.x = TReal(param[2]);
    croot.y = TReal(param[3]);

    cdegree.x = TReal(param[0]);
    cdegree.y = TReal(param[1]);

    *z = *q;

    subtype = ((int)param[4] == 0) ? 'N' : 'B';
    }

/**************************************************************************
    Initialise Lambda fractal.

    Iteration:
	z(n+1) = c * z(n) * (1 - z(n))

    Julia:
	c = Julia constant

    Mandelbrot:
	c = pixel + initial perturbation
	z(0) = 0.5
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Lambda(CPixel* self, TComplex* z, TComplex* q, double* param, bool juliaflag, TComplex& temp)
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

/**************************************************************************
    Initialise the Fractint z-power family.

    Used by Mandel/Julia z-power, z^z + z^p and tetration variants.

    temp  stores the primary complex parameter.
    temp2 stores the power parameter.
    temp1 stores the initial orbit value.

    The starting pixel is transformed before Mandelbrot initialisation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_ZPowerGroup(CPixel* self, TComplex* z, TComplex* q, TComplex& c, double* param, bool juliaflag, TComplex& t, TComplex& temp, TComplex& temp1, TComplex& temp2)
    {
    t = TransformPointT<TComplex, TReal>(c, self->TransformType, double(self->f_radius), double(self->f_xcenter), double(self->f_ycenter));

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

/**************************************************************************
    Initialise Mark Peterson's Marks Mandelbrot/Julia fractals.

    The recurrence uses the coefficient:

	coefficient = c^(power - 1)

    so that the principal iteration has the form:

	z(n+1) = coefficient * z(n)^2 + c

    Julia uses the supplied Julia constant; Mandelbrot uses the
    transformed pixel and optional initial perturbation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Marks(CPixel* self, TComplex* z, TComplex* q, TComplex& c, double* param, bool juliaflag, TComplex& t, TComplex& MarkCoeff)
    {
    t = TransformPointT<TComplex, TReal>(c, self->TransformType, double(self->f_radius), double(self->f_xcenter), double(self->f_ycenter));

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

/**************************************************************************
    Initialise quaternion Mandelbrot/Julia fractals.

    The four quaternion components are stored as:

	z.x, z.y, temp.x, temp.y

    Mandelbrot starts at zero and uses the transformed pixel for the
    first two components of c.

    Julia starts at the transformed pixel and uses the Julia constant.
    param[2] and param[3] supply the remaining two quaternion components.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Quat(CPixel* self, TComplex* z, TComplex* q, TComplex& c, double* param, bool juliaflag, TComplex& t, TReal& qc, TReal& qci, TReal& qcj, TReal& qck)
    {
    t = TransformPointT<TComplex, TReal>(c, self->TransformType, double(self->f_radius), double(self->f_xcenter), double(self->f_ycenter));

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

/**************************************************************************
    Initialise Fractint Halley fractal.

    Each pixel supplies the starting value of z.
    Degree is constrained to 2 or greater.
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_Halley(CPixel* self, TComplex* z, TComplex* q, double* param, WORD* degree)
    {
    *degree = (WORD)param[0];
    if (*degree < 2)
	*degree = 2;

    *z = *q;
    }

/**************************************************************************
    DivideBrot5 initialisation.

    Mandelbrot:
	z(0) = 0
	c = pixel

    Julia:
	z(0) = pixel
	c = Julia constant
**************************************************************************/

template <typename TComplex, typename TReal>
inline void Init_DivideBrot5(CPixel* self, TComplex* z, TComplex* q, double* param, bool juliaflag)
    {
    if (!juliaflag)
	{
	z->x = TReal(0.0);
	z->y = TReal(0.0);
	}
    }

// ------------------------------------------------------------
// Template version of RunFunctions (first step: Mandel only)
// ------------------------------------------------------------

/**************************************************************************
    Barnsley 1 iteration.

    If Re(z) >= 0:
	z = z * c - c

    otherwise:
	z = z * c + c
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Barnsley1(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex next = (*z) * temp;

    if (z->x >= TReal(0.0))
	next = next - temp;
    else
	next = next + temp;

    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Barnsley 2 iteration.

    Calculate z * c.

    If Im(z * c) >= 0:
	z = z * c - c

    otherwise:
	z = z * c + c
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Barnsley2(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex next = (*z) * temp;

    if (next.y >= TReal(0.0))
	next = next - temp;
    else
	next = next + temp;

    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Barnsley 3 iteration.

    Start with:
	z = z^2 - 1

    If Re(z_old) <= 0:
	z += c * Re(z_old)
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Barnsley3(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TReal oldx = z->x;

    TComplex next = z->CSqr();
    next.x -= TReal(1.0);

    if (oldx <= TReal(0.0))
	{
	next.x += temp.x * oldx;
	next.y += temp.y * oldx;
	}

    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Phoenix iteration, degree type ZERO.

    z(n+1) = z(n)^2 + p + q*y(n)
    y(n+1) = z(n)

    temp.x = p
    temp.y = q
    temp3  = previous z
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Phoenix(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, TComplex& temp3, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex next = z->CSqr();

    next.x += temp.x + temp.y * temp3.x;
    next.y += temp.y * temp3.y;

    temp3 = *z;
    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Phoenix PLUS iteration.

    z(n+1) = z(n)^(degree-1) * (z(n) + p) + q*y(n)
    y(n+1) = z(n)

    PhoenixDegree has already been adjusted during initialisation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_PhoenixPlus(CPixel* self, TComplex* z, TComplex& temp, TComplex& temp3, int PhoenixDegree, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex power = z->CPolynomial(PhoenixDegree);

    TComplex zp = *z;
    zp.x += temp.x;

    TComplex next = power * zp;

    next.x += temp.y * temp3.x;
    next.y += temp.y * temp3.y;

    temp3 = *z;
    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Phoenix MINUS iteration.

    z(n+1) = z(n)^(degree-2) * (z(n)^2 + p) + q*y(n)
    y(n+1) = z(n)

    PhoenixDegree has already been adjusted during initialisation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_PhoenixMinus(CPixel* self, TComplex* z, TComplex& temp, TComplex& temp3, int PhoenixDegree, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex power = z->CPolynomial(PhoenixDegree);

    TComplex z2p = z->CSqr();
    z2p.x += temp.x;

    TComplex next = power * z2p;

    next.x += temp.y * temp3.x;
    next.y += temp.y * temp3.y;

    temp3 = *z;
    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Newton iteration for:

	F(z) = z^degree - 1

    Newton's method gives:

	z(n+1) = z(n) -
		 (z(n)^degree - 1) /
		 (degree * z(n)^(degree-1))

    Convergence is detected from the change in z.

    The S and B subtypes additionally identify the root reached and
    use that root to determine the output colour.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Newton(CPixel* self, TComplex* z, TComplex* q, TComplex& z1, TComplex& z2, TComplex& zd, long* iteration, WORD* degree, int subtype, TComplex* roots, TReal thresh, long& color, long threshold)
    {
    int tmpcolor;
    int i;

    color = *iteration;
    z2 = *z;
    z1 = z->CPolynomial(*degree - 1);
    *z = *z - (z1 * *z - TReal(1)) / (z1 * TReal(*degree));
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

/**************************************************************************
    Complex Marks Mandelbrot iteration.

    z(n+1) = coefficient * (z(n)^2 + c)

    The coefficient was calculated during initialisation from the
    user-supplied complex exponent.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_ComplexMarksMandelbrot(CPixel* self, TComplex* z, TComplex* q, TComplex& sqr, TReal& realimag, TComplex& coefficient, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    realimag = z->x * z->y;

    z->x = sqr.x - sqr.y;
    z->y = realimag + realimag;

    *z = coefficient * (*z) + (*q) *coefficient;

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim, oldZ);
    }

/**************************************************************************
    Spider iteration.

    z(n+1) = z(n)^2 + c(n)
    c(n+1) = c(n) / 2 + z(n+1)

    temp stores the evolving value of c.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Spider(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, int bailout_type, TReal rqlim, TComplex* oldZ)
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

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim, oldZ);
    }

/**************************************************************************
    Man O' War iteration.

    The recurrence includes the previous orbit value in addition to
    the usual quadratic term.

    temp1 stores the previous z value.
    temp stores the constant term established during initialisation.

    Mandelbrot also retains its initial parameter perturbation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_ManOWar(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, TComplex& temp1, TComplex& temp3, double* param, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex sqr;

    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    // Julia: temp already contains the Julia constant.
    // Mandelbrot: retain the additional parameter perturbation.
    temp3.x = sqr.x - sqr.y + temp.x + temp1.x + (self->juliaflag ? 0.0 : param[0]);
    temp3.y = 2.0 * z->x * z->y + temp.y + temp1.y + (self->juliaflag ? 0.0 : param[1]);
    temp1 = *z;

    *z = temp3;

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim, oldZ);
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

/**************************************************************************
    Complex Newton iteration.

    Solves:
	z^degree = root

    where both degree and root may be complex.

    Newton's method can be rearranged as:

	z(n+1) =
	    ((degree - 1) * z(n)^degree + root) /
	    (degree * z(n)^(degree - 1))

    Basin mode derives a colour from the argument of the converged root.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_ComplexNewton(CPixel* self, TComplex* z, TComplex* q, TComplex& croot, TComplex& cdegree, long* iteration, int subtype, long& color)
    {
    TComplex tmp, cd1, New, temp;

    TReal mod;
    long coloriter;
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

/**************************************************************************
    Lambda iteration.

    z(n+1) = c * z(n) * (1 - z(n))

    temp contains c, established during initialisation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Lambda(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, int bailout_type, TReal rqlim, TComplex* oldZ)
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

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim, oldZ);
    }

/**************************************************************************
    Complex z-power iteration.

    z(n+1) = z(n)^p + c

    temp2 contains the complex power p.
    q contains the pixel coordinate or Julia constant c.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_ZPower(CPixel* self, TComplex* z, TComplex* q, TComplex& temp2, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    *z = (*z) ^ temp2;
    *z = (*z) + (*q);

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Fractint z^z + z^p iteration.

    z(n+1) = z(n)^z(n) + z(n)^p + c

    param[2] supplies the integer power p.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_ZtoZPlusZPower(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, double* param, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    temp = z->CPolynomial((int)param[2]);

    *z = (*z) ^ (*z);
    *z = temp + (*z) + (*q);

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Mark Peterson's Marks Mandelbrot iteration.

    z(n+1) = coefficient * z(n)^2 + c

    where:

	coefficient = c^(power - 1)

    The coefficient is calculated once during initialisation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_MarksMandel(CPixel* self, TComplex* z, TComplex* q, TComplex& temp, TComplex& temp1, TComplex& coefficient, TComplex& t, int bailout_type, TReal rqlim, TComplex* oldZ)
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

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim, oldZ);
    }

/**************************************************************************
    Quaternion iteration.

    q(n+1) = q(n)^2 + c

    The quaternion is represented by four real components:

	a0 = z.x
	a1 = z.y
	a2 = temp.x
	a3 = temp.y

    The orbit escapes when the four-dimensional squared magnitude
    exceeds the bailout limit.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Quat(CPixel* self, TComplex* z, TComplex& temp, TReal qc, TReal qci, TReal qcj, TReal qck, TReal rqlim)
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

/**************************************************************************
    Tetration iteration.

    z(n+1) = c^z(n)

    q contains the base c.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Tetrate(CPixel* self, TComplex* z, TComplex* q, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    *z = (*q) ^ (*z);
    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Fractint Magnet 1 iteration.

    z = ((z^2 + c - 1) / (2z + c - 2))^2

    c is the pixel coordinate for Mandelbrot, or the Julia constant.
    Preserve Fractint's denominator check before division.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Magnet1(CPixel* self, TComplex* z, TComplex* q, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex top = z->CSqr() + *q - TReal(1.0);
    TComplex bot = (*z) * TReal(2.0) + *q - TReal(2.0);

    TReal div = bot.CSumSqr();

    // Fractint exits if the denominator is too small.
    if (div < TReal(FLT_MIN))
	return 1;

    TComplex temp = top / bot;
    *z = temp.CSqr();

    TComplex sqr;
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    return BailoutCore<TComplex>(
	bailout_type, z, &sqr, rqlim, oldZ);
    }

/**************************************************************************
    Fractint Magnet 2 iteration.

    z = ((z^3 + 3(c-1)z + (c-1)(c-2)) /
	 (3z^2 + 3(c-2)z + (c-1)(c-2)+1))^2

    c is the pixel coordinate for Mandelbrot, or the Julia constant.
    Preserve Fractint's denominator check before division.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Magnet2(CPixel* self, TComplex* z, TComplex* q, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex cm1 = *q - TReal(1.0);
    TComplex cm2 = *q - TReal(2.0);
    TComplex cm1cm2 = cm1 * cm2;

    TComplex z2 = z->CSqr();

    TComplex top = (*z) * z2 + cm1 * (*z) * 3.0 + cm1cm2;
    TComplex bot = z2 * 3.0 + cm2 * (*z) * 3.0 + cm1cm2 + TReal(1.0);

    TReal div = bot.CSumSqr();

    // Fractint exits if the denominator is too small.
    if (div < TReal(FLT_MIN))
	return 1;

    TComplex temp = top / bot;
    *z = temp.CSqr();

    TComplex sqr;
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    return BailoutCore<TComplex>(
	bailout_type, z, &sqr, rqlim, oldZ);
    }

/**************************************************************************
    Fractint Halley iteration.

    F(z)  = z^(a+1) - z
    F'(z) = (a+1)z^a - 1
    F''(z)= a(a+1)z^(a-1)

    param[1] = relaxation coefficient
    param[2] = convergence epsilon
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_Halley(CPixel* self, TComplex* z, double* param, WORD degree)
    {
    TComplex x_to_a_less_one;
    TComplex x_to_a;
    TComplex x_to_a_plus_one;
    TComplex fx;
    TComplex f1_prime;
    TComplex f2_prime;
    TComplex numer1;
    TComplex numer2;
    TComplex denom;

    TComplex oldz = *z;

    x_to_a_less_one = *z;

    for (int deg = 2; deg < degree; deg++)
	x_to_a_less_one = (*z) * x_to_a_less_one;

    x_to_a = (*z) * x_to_a_less_one;
    x_to_a_plus_one = (*z) * x_to_a;

    fx = x_to_a_plus_one - (*z);

    f2_prime = x_to_a_less_one * TReal((degree + 1) * degree);
    f1_prime = x_to_a * TReal(degree + 1);
    f1_prime.x -= TReal(1.0);

    numer1 = f2_prime * fx;

    denom = f1_prime * TReal(2.0);
    numer1 = numer1 / denom;

    numer2 = f1_prime - numer1;
    numer2 = fx / numer2;

    numer2 = numer2 * TReal(param[1]);

    *z = (*z) - numer2;

    TReal oldmod = sqrt(oldz.CSumSqr());
    TReal newmod = sqrt(z->CSumSqr());

    if (fabs(newmod - oldmod) < TReal(param[2]))
	return TRUE;

    return FALSE;
    }

/**************************************************************************
    DivideBrot5 iteration by Jim Muth.

    Original:
	z = z^2 / (z^(2-a) + b) + c

    For a > 2, rearranged to avoid negative powers:
	z = z^a / (1 + b*z^(a-2)) + c

    c is the pixel coordinate for Mandelbrot, or the Julia constant.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_DivideBrot5(CPixel* self, TComplex* z, TComplex* q, int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    int a = (int)self->param[0];
    TReal b = TReal(self->param[1]);

    TComplex top;
    TComplex bot;

    if (a <= 2)
	{
	top = z->CSqr();
	bot = z->CPolynomial(2 - a);
	bot.x += b;
	}
    else
	{
	top = z->CPolynomial(a);
	bot = z->CPolynomial(a - 2) * b;
	bot.x += TReal(1.0);
	}

    TReal div = bot.CSumSqr();

    if (div < TReal(FLT_MIN))
	return 1;

    *z = top / bot + *q;

    TComplex sqr;
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;

    return BailoutCore<TComplex>(bailout_type, z, &sqr, rqlim, oldZ);
    }

/**************************************************************************
    Complex Phoenix iteration, degree type ZERO.

    z(n+1) = z(n)^2 + p1 + p2*y(n)
    y(n+1) = z(n)

    q     = p1
    temp2 = p2
    temp3 = previous z
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_PhoenixCplx(CPixel* self, TComplex* z, TComplex* q,
    TComplex& temp2, TComplex& temp3,
    int bailout_type, TReal rqlim, TComplex* oldZ)
    {
    TComplex next = z->CSqr();

    next = next + *q + temp2 * temp3;

    temp3 = *z;
    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Complex Phoenix PLUS iteration.

    z(n+1) = z(n)^(degree-1) * (z(n) + p1) + p2*y(n)
    y(n+1) = z(n)

    PhoenixDegree has already been adjusted during initialisation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_PhoenixCplxPlus(CPixel* self, TComplex* z, TComplex* q,
    TComplex& temp2, TComplex& temp3,
    int PhoenixDegree, int bailout_type,
    TReal rqlim, TComplex* oldZ)
    {
    TComplex power = z->CPolynomial(PhoenixDegree);

    TComplex next = power * ((*z) + (*q));
    next = next + temp2 * temp3;

    temp3 = *z;
    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }

/**************************************************************************
    Complex Phoenix MINUS iteration.

    z(n+1) = z(n)^(degree-2) * (z(n)^2 + p1) + p2*y(n)
    y(n+1) = z(n)

    PhoenixDegree has already been adjusted during initialisation.
**************************************************************************/

template <typename TComplex, typename TReal>
inline int Iter_PhoenixCplxMinus(CPixel* self, TComplex* z, TComplex* q,
    TComplex& temp2, TComplex& temp3,
    int PhoenixDegree, int bailout_type,
    TReal rqlim, TComplex* oldZ)
    {
    TComplex power = z->CPolynomial(PhoenixDegree);

    TComplex next = power * (z->CSqr() + *q);
    next = next + temp2 * temp3;

    temp3 = *z;
    *z = next;

    return DoBailout(bailout_type, z, rqlim, oldZ);
    }


