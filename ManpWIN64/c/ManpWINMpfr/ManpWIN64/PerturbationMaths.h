#pragma once

#include "complex.h"

inline Complex CExpM1Stable(const Complex& z)
{
    Complex out;

    double em1 = expm1(z.x);
    double ea  = em1 + 1.0;

    double siny = sin(z.y);
    double cosy = cos(z.y);
    double s2   = sin(0.5 * z.y);

    out.x = em1 * cosy - 2.0 * s2 * s2;
    out.y = ea * siny;

    return out;
}