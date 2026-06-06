/*
    QDTrig.h - interface for the CQDTrig class.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <string.h>
#include "QDComplex.h"

#pragma once
class CQDTrig
    {
    public:
	void	sincos_qd(qd_real *s, qd_real *c, qd_real n);
	void	sinhcosh_qd(qd_real *s, qd_real *c, const qd_real &n);
    };

