/*
    FractalMathsTemplate.H - Template for handling additional functions for all arithmetic types.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <functional>
#include <cmath>
#include <type_traits>
#include "TransformTemplate.h"

#pragma once

/**************************************************************************
    Invert fractal
**************************************************************************/

template <typename TComplex, typename TReal>
inline TComplex InvertZ2(const TComplex& c, double radius, double xcenter, double ycenter)
    {
    TComplex temp;

    temp.x = c.x - xcenter;
    temp.y = c.y - ycenter;

    TReal r2 = temp.x * temp.x + temp.y * temp.y;

    if (fabs(r2) > TReal(1e-30))
        r2 = radius / r2;
    else
        r2 = TReal(1e30);

    temp.x *= r2;
    temp.y *= r2;

    temp.x += xcenter;
    temp.y += ycenter;

    return temp;
    }

/**************************************************************************
    Fold X coordinate transformation
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex FoldXT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    x = fabs(x);

    temp.x = f_xcenter + x * f_radius;
    temp.y = f_ycenter + y * f_radius;

    return temp;
    }

/**************************************************************************
Fold Y coordinate transformation
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex FoldYT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    y = fabs(y);

    temp.x = f_xcenter + x * f_radius;
    temp.y = f_ycenter + y * f_radius;

    return temp;
    }

/**************************************************************************
    Absolute fold coordinate transformation

    Reflects the point into the positive X/Y quadrant relative to the
    selected transformation centre.
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex FoldXYT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    x = fabs(x);
    y = fabs(y);

    temp.x = f_xcenter + x * f_radius;
    temp.y = f_ycenter + y * f_radius;

    return temp;
    }

/**************************************************************************
    Fold diagonal coordinate transformation
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex FoldDiagonalT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    TReal u = x - y;
    TReal v = x + y;

    u = fabs(u);

    TReal NewX = (v + u) * (TReal)0.5;
    TReal NewY = (v - u) * (TReal)0.5;

    temp.x = f_xcenter + NewX * f_radius;
    temp.y = f_ycenter + NewY * f_radius;

    return temp;
    }
    
/**************************************************************************
    Double diagonal fold coordinate transformation
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex FoldDiagonalsT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    TReal u = fabs(x - y);
    TReal v = fabs(x + y);

    TReal NewX = (v + u) * (TReal)0.5;
    TReal NewY = (v - u) * (TReal)0.5;

    temp.x = f_xcenter + NewX * f_radius;
    temp.y = f_ycenter + NewY * f_radius;

    return temp;
    }

/**************************************************************************
    Radial bulge coordinate transformation

    Expands distance from the centre cubically while preserving angle.
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex RadialBulgeT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    if (fabs(f_radius) > FLT_MIN)
	{
	TReal distanceSquared = x * x + y * y;
	double radiusSquared = f_radius * f_radius;

	TReal scale = distanceSquared / radiusSquared;

	temp.x = f_xcenter + x * scale;
	temp.y = f_ycenter + y * scale;
	}
    else
	{
	temp = Cmplx1;
	}

    return temp;
    }

/**************************************************************************
    Radial pinch coordinate transformation
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex RadialPinchT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    double radiusSquared = f_radius * f_radius;
    TReal distanceSquared = x * x + y * y;

    if (radiusSquared > FLT_MIN)
	{
	TReal scale = radiusSquared / (radiusSquared + distanceSquared);

	temp.x = f_xcenter + x * scale;
	temp.y = f_ycenter + y * scale;
	}
    else
	{
	temp = Cmplx1;
	}

    return temp;
    }

/**************************************************************************
    Circle fold coordinate transformation
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex CircleFoldT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    TReal r = sqrt(x * x + y * y);

    if (fabs(r) > (TReal)FLT_MIN)
	{
	TReal NewRadius = fabs(r - f_radius);
	TReal scale = NewRadius / r;

	temp.x = f_xcenter + x * scale;
	temp.y = f_ycenter + y * scale;
	}
    else
	{
	temp = Cmplx1;
	}

    return temp;
    }

/**************************************************************************
    Radial clamp coordinate transformation

    Points inside the selected radius are unchanged.
    Points outside the radius are projected onto the circle.
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex RadialClampT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    TReal distance = sqrt(x * x + y * y);

    if (distance > f_radius && fabs(distance) > (TReal)FLT_MIN)
	{
	TReal scale = f_radius / distance;

	temp.x = f_xcenter + x * scale;
	temp.y = f_ycenter + y * scale;
	}
    else
	{
	temp = Cmplx1;
	}

    return temp;
    }

/**************************************************************************
    Soft radial compression coordinate transformation

    Compresses distance from the centre smoothly toward the selected
    radius while preserving direction.
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex RadialCompressT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    TReal distance = sqrt(x * x + y * y);

    if (fabs(f_radius + distance) > (TReal)FLT_MIN)
	{
	TReal scale = f_radius / (f_radius + distance);

	temp.x = f_xcenter + x * scale;
	temp.y = f_ycenter + y * scale;
	}
    else
	{
	temp = Cmplx1;
	}

    return temp;
    }

/**************************************************************************
    Integer power coordinate transformation

    w = C + (z - C)^Power / R^(Power - 1)

    where:
	C = centre
	R = scale
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex PowerTransformT(const TComplex& Cmplx1, int Power, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    if (Power < 1 || fabs(f_radius) <= FLT_MIN)
	return Cmplx1;

    TComplex base;
    base.x = Cmplx1.x - f_xcenter;
    base.y = Cmplx1.y - f_ycenter;

    TComplex result;
    result.x = 1.0;
    result.y = 0.0;

    for (int i = 0; i < Power; i++)
	{
	TComplex next;

	next.x = result.x * base.x - result.y * base.y;
	next.y = result.x * base.y + result.y * base.x;

	result = next;
	}

    double scale = 1.0;

    for (int i = 1; i < Power; i++)
	scale *= f_radius;

    temp.x = f_xcenter + result.x / scale;
    temp.y = f_ycenter + result.y / scale;

    return temp;
    }

/**************************************************************************
    Complex logarithm transformation

    w = C + R * log((z - C) / R)

    where:
	C = centre
	R = scale
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex ComplexLogT(const TComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    TReal x = Cmplx1.x - f_xcenter;
    TReal y = Cmplx1.y - f_ycenter;

    TReal modulusSquared = sqr(x) + sqr(y);

    if (fabs(modulusSquared) > (TReal)FLT_MIN && fabs(f_radius) > FLT_MIN)
	{
	double radiusSquared = f_radius * f_radius;
	temp.x = f_xcenter + f_radius * (TReal)0.5 * log(modulusSquared / radiusSquared);
	temp.y = f_ycenter + f_radius * atan2(y, x);
	}
    else
	{
	temp.x = (TReal)-FLT_MAX;
	temp.y = f_ycenter;
	}

    return temp;
    }

/**************************************************************************
    Complex logarithm transformation for arbitrary precision
**************************************************************************/

template<>
inline BigComplex ComplexLogT<BigComplex, BigDouble>(const BigComplex& Cmplx1, double f_radius, double f_xcenter, double f_ycenter)
    {
    BigComplex temp;
    CBigTrig trig;

    BigDouble x = Cmplx1.x - f_xcenter;
    BigDouble y = Cmplx1.y - f_ycenter;

    BigDouble modulusSquared = sqr(x) + sqr(y);

    if (fabs(modulusSquared) > BigDouble(FLT_MIN) && fabs(f_radius) > FLT_MIN)
	{
	double radiusSquared = f_radius * f_radius;

	BigDouble LogArgument = modulusSquared / radiusSquared;
	BigDouble LogValue;
	BigDouble Angle;

	trig.log_bf(&LogValue, LogArgument);
	trig.atan2_bf(&Angle, y, x);

	temp.x = f_xcenter + f_radius * 0.5 * LogValue;
	temp.y = f_ycenter + f_radius * Angle;
	}
    else
	{
	temp.x = -FLT_MAX;
	temp.y = f_ycenter;
	}

    return temp;
    }

/**************************************************************************
    Complex exponential coordinate transformation

    w = C + R * exp((z - C) / R)
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex ComplexExpT(const TComplex& z, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    if (fabs(f_radius) <= FLT_MIN)
	return z;

    TReal x = (z.x - f_xcenter) / f_radius;
    TReal y = (z.y - f_ycenter) / f_radius;

    TReal ExpX = exp(x);

    temp.x = f_xcenter + f_radius * ExpX * cos(y);
    temp.y = f_ycenter + f_radius * ExpX * sin(y);

    return temp;
    }

/**************************************************************************
    Complex exponential transformation for arbitrary precision

    w = C + R * exp((z - C) / R)
**************************************************************************/

template<>
inline BigComplex ComplexExpT<BigComplex, BigDouble>(const BigComplex& z, double f_radius, double f_xcenter, double f_ycenter)
    {
    if (fabs(f_radius) <= FLT_MIN)
	return z;

    BigDouble Radius = f_radius;
    BigDouble CentreX = f_xcenter;
    BigDouble CentreY = f_ycenter;

    BigComplex Normalised;
    Normalised.x = (z.x - CentreX) / Radius;
    Normalised.y = (z.y - CentreY) / Radius;

    BigComplex Transformed = Normalised.CExp();

    BigComplex Result;
    Result.x = CentreX + Transformed.x * Radius;
    Result.y = CentreY + Transformed.y * Radius;

    return Result;
    }

/**************************************************************************
    Complex sine coordinate transformation

    w = C + R * sin((z - C) / R)
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex ComplexSinT(const TComplex& z, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex temp;

    if (fabs(f_radius) <= FLT_MIN)
	return z;

    TReal x = (z.x - f_xcenter) / f_radius;
    TReal y = (z.y - f_ycenter) / f_radius;

    temp.x = f_xcenter + f_radius * sin(x) * cosh(y);
    temp.y = f_ycenter + f_radius * cos(x) * sinh(y);

    return temp;
    }

/**************************************************************************
    Complex sine transformation for arbitrary precision

    w = C + R * sin((z - C) / R)
**************************************************************************/

template<>
inline BigComplex ComplexSinT<BigComplex, BigDouble>(
    const BigComplex& z,
    double f_radius,
    double f_xcenter,
    double f_ycenter)
    {
    if (fabs(f_radius) <= FLT_MIN)
	return z;

    BigDouble Radius = f_radius;
    BigDouble CentreX = f_xcenter;
    BigDouble CentreY = f_ycenter;

    BigComplex Normalised;
    Normalised.x = (z.x - CentreX) / Radius;
    Normalised.y = (z.y - CentreY) / Radius;

    BigComplex Transformed = Normalised.CSin();

    BigComplex Result;
    Result.x = CentreX + Transformed.x * Radius;
    Result.y = CentreY + Transformed.y * Radius;

    return Result;
    }

/**************************************************************************
    Apply the selected coordinate transformation to a fractal point
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex TransformPointT(const TComplex& c, TRANSFORMTYPE TransformType, double f_radius, double f_xcenter, double f_ycenter)
    {
    if (TransformType >= TRANSFORM_POWER_BASE)
	{
	int Power = GetTransformPower(TransformType);
	return PowerTransformT<TComplex, TReal>(c, Power, f_radius, f_xcenter, f_ycenter);
	}

    switch (TransformType)
	{
	case TRANSFORM_INVERSION:
	    return InvertZ2<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_COMPLEX_LOG:
	    return ComplexLogT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_FOLD_X:
	    return FoldXT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_FOLD_Y:
	    return FoldYT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_FOLD_XY:
	    return FoldXYT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_FOLD_DIAGONAL:
	    return FoldDiagonalT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_FOLD_DIAGONALS:
	    return FoldDiagonalsT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_RADIAL_BULGE:
	    return RadialBulgeT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_RADIAL_PINCH:
	    return RadialPinchT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_CIRCLE_FOLD:
	    return CircleFoldT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_RADIAL_CLAMP:
	    return RadialClampT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_RADIAL_COMPRESS:
	    return RadialCompressT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_COMPLEX_EXP:
	    return ComplexExpT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_COMPLEX_SIN:
	    return ComplexSinT<TComplex, TReal>(c, f_radius, f_xcenter, f_ycenter);

	case TRANSFORM_AFFINE:
	    return AffineTransformT<TComplex, TReal>(c, gManp->AffineParams);

	case TRANSFORM_MOBIUS:
	    return MobiusTransformT<TComplex, TReal>(c, gManp->MobiusParams);

	case TRANSFORM_JOUKOWSKI:
	    return JoukowskiTransformT<TComplex, TReal>(c, gManp->JoukowskiParams);

	case TRANSFORM_TWIST:
	    return TwistTransformT<TComplex, TReal>(c, gManp->TwistParams);

	case TRANSFORM_NONE:
	default:
	    return c;
	}
    }

/**************************************************************************
    Initialise Julia or Mandelbrot starting values using the selected
    coordinate transformation
**************************************************************************/

template<typename TComplex, typename TReal>
void SetupJuliaT(TComplex& c, TComplex& z, TComplex& q, const Complex& j, bool juliaflag, TRANSFORMTYPE TransformType, double f_radius, double f_xcenter, double f_ycenter)
    {
    TComplex TransformedC = TransformPointT<TComplex, TReal>(c, TransformType, f_radius, f_xcenter, f_ycenter);

    if (juliaflag)
	{
	q.x = j.x;
	q.y = j.y;
	z = TransformedC;
	}
    else
	{
	q = TransformedC;
	z = 0.0;
	}
    }

