/*
    DDTrig.h - interface for the CDDTrig class.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <string.h>
#include "DDComplex.h"

#pragma once
class CDDTrig
    {
    public:
	void	sincos_dd(dd_real *s, dd_real *c, dd_real n);
	void	sinhcosh_dd(dd_real *s, dd_real *c, dd_real n);

//    private:
    };

