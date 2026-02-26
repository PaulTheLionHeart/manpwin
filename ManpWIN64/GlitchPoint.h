#pragma once
#include "BigComplex.h"

struct GlitchPointStruct
    {
    int		x, y;	// pixel address
    double	Zmin;	// the value to be tested
    long	count;	// how many Glitch pixels so far
    BigComplex	C;	// reference point
    };
