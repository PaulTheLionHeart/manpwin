//////////////////////////////////////////////////////////////////////
//
// TrigFn.cpp: Function list.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Windows.h>
#include <time.h>
#include "complex.h"
#include "TrigFn.h"
#include "ParserTemplate.h"

void CTrigFn::HComplexTrig(_HCMPLX *h, _HCMPLX *out, int index)		// extends the unary function f to *h1 
    {
    // This is the whole beauty of Hypercomplex numbers - *ANY* unary
    // complex valued function of a complex variable can easily
    // be generalized to hypercomplex numbers
    THComplexTrig<_HCMPLX, Complex>(h, out, index);
    }

// call appropriate trig function
void CTrigFn::CMPLXtrig(Complex *zIn, Complex *zOut, int index)
    {
    TCMPLXtrig(zIn, zOut, index);
    }

int CTrigFn::FindFunct(const char *Str, int NumFns)
    {
    int		n;
    size_t	size;

    size = strlen(Str);
    for (n = 0; n < NumFns; n++)
	{
	if (!_strnicmp(FunctList[n], Str, size))
	    return n;
	}
    return -1;			// not found
    }




