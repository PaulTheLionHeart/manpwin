//////////////////////////////////////////////////////////////////////
//
// DDTrigFn.cpp: Function list. Double-double version
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <time.h>
#include "complex.h"
#include "TrigFn.h"
#include "ParserTemplate.h"

void CTrigFn::DDHComplexTrig(_DDHCMPLX *h, _DDHCMPLX *out, int index)		// extends the unary function f to *h1
    {
    // This is the whole beauty of Hypercomplex numbers - *ANY* unary
    // complex valued function of a complex variable can easily
    // be generalized to hypercomplex numbers
    THComplexTrig<_DDHCMPLX, DDComplex>(h, out, index);
    }

// call appropriate trig function
void CTrigFn::DDCMPLXtrig(DDComplex *zIn, DDComplex *zOut, int index)
    {
    TCMPLXtrig(zIn, zOut, index);
    }


