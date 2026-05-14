//////////////////////////////////////////////////////////////////////
//
// QDTrigFn.cpp: Function list. Quad-double version
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <time.h>
#include "complex.h"
#include "TrigFn.h"
#include "ParserTemplate.h"

void CTrigFn::QDHComplexTrig(_QDHCMPLX *h, _QDHCMPLX *out, int index)		// extends the unary function f to *h1
    {
    // This is the whole beauty of Hypercomplex numbers - *ANY* unary
    // complex valued function of a complex variable can easily
    // be generalized to hypercomplex numbers
    THComplexTrig<_QDHCMPLX, QDComplex>(h, out, index);
    }

// call appropriate trig function
void CTrigFn::QDCMPLXtrig(QDComplex *zIn, QDComplex *zOut, int index)
    {
    TCMPLXtrig(zIn, zOut, index);
    }





