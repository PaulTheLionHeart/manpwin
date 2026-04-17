// FractalMathsTemplate.h

#include <windows.h>
#include <functional>
#include <cmath>

#pragma once

template <typename TComplex, typename TReal>
inline TComplex InvertZ2(
    TComplex& c,
    TReal xcenter,
    TReal ycenter,
    TReal radius)
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

