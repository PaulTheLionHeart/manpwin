/*
    TransformTemplate.h - Template for handling specific transformations.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <functional>
#include <cmath>
#include <type_traits>

#pragma once

/**************************************************************************
    Affine coordinate transformation

    x' = a*x + b*y + e
    y' = c*x + d*y + f
**************************************************************************/

template<typename TComplex, typename TReal, typename TParams>
TComplex AffineTransformT(const TComplex& Cmplx1, const TParams& Params)
    {
    TComplex temp;

    temp.x = Params.a * Cmplx1.x + Params.b * Cmplx1.y + Params.e;
    temp.y = Params.c * Cmplx1.x + Params.d * Cmplx1.y + Params.f;

    return temp;
    }

/**************************************************************************
    Inverse affine transformation

    Converts a transformed point back to the original coordinate system.
**************************************************************************/

template<typename TComplex, typename TReal, typename TParams>
TComplex InverseAffineTransformT(const TComplex& Cmplx1, const TParams& Params)
    {
    TComplex temp;

    TReal det = Params.a * Params.d - Params.b * Params.c;

    if (det == 0.0)
	return Cmplx1;

    TReal x = Cmplx1.x - Params.e;
    TReal y = Cmplx1.y - Params.f;

    temp.x = (Params.d * x - Params.b * y) / det;
    temp.y = (-Params.c * x + Params.a * y) / det;

    return temp;
    }

/**************************************************************************
    Mobius coordinate transformation

    w = (a*z + b) / (c*z + d)
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex MobiusTransformT(const TComplex& z, const MobiusTransformParams& Params)
    {
    TComplex a;
    TComplex b;
    TComplex c;
    TComplex d;

    a.x = Params.aReal;
    a.y = Params.aImag;

    b.x = Params.bReal;
    b.y = Params.bImag;

    c.x = Params.cReal;
    c.y = Params.cImag;

    d.x = Params.dReal;
    d.y = Params.dImag;

    TComplex numerator = a * z + b;
    TComplex denominator = c * z + d;

    TReal DenomNorm = denominator.x * denominator.x + denominator.y * denominator.y;

    if (DenomNorm <= (TReal)FLT_MIN)
	return z;

    return numerator / denominator;
    }

/**************************************************************************
    Joukowski coordinate transformation

    w = z + k / z
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex JoukowskiTransformT(const TComplex& z, const JoukowskiTransformParams& Params)
    {
    TComplex temp;

    TReal Norm = z.x * z.x + z.y * z.y;

    if (Norm <= (TReal)FLT_MIN)
	return z;

    // k / z
    TReal RealPart = (Params.kReal * z.x + Params.kImag * z.y) / Norm;
    TReal ImagPart = (Params.kImag * z.x - Params.kReal * z.y) / Norm;

    temp.x = z.x + RealPart;
    temp.y = z.y + ImagPart;

    return temp;
    }

/**************************************************************************
Twist coordinate transformation

Rotates the point around the selected centre.
The twist angle increases with distance from the centre.
**************************************************************************/

template<typename TComplex, typename TReal>
TComplex TwistTransformT(const TComplex& z, const TwistTransformParams& Params)
    {
    TComplex temp;

    TReal x = z.x - Params.CentreX;
    TReal y = z.y - Params.CentreY;

    TReal RadiusSquared = (TReal)Params.Radius * (TReal)Params.Radius;

    if (RadiusSquared <= (TReal)FLT_MIN)
	return z;

    TReal DistanceSquared = x * x + y * y;

    TReal Angle = (TReal)Params.Strength * DistanceSquared / RadiusSquared;

    TReal CosAngle = cos(Angle);
    TReal SinAngle = sin(Angle);

    temp.x = Params.CentreX + x * CosAngle - y * SinAngle;
    temp.y = Params.CentreY + x * SinAngle + y * CosAngle;

    return temp;
    }


