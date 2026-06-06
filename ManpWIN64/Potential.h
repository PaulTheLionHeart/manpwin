/*
    Potential.h - interface for the CPotential class.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<windows.h>
#include	<math.h>
#include	<float.h>
#include	"colour.h"

#pragma once
class CPotential
    {
    public:
    int	CPotential::potential(double mag, int iterations, long threshold, CTrueCol *TrueCol, int colors, double potparam[]);
    private:
    };

