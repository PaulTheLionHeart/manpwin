#include <cmath>
#include "ParserTemplate.h"
#include "ParserCtx.h"

#pragma once

/**************************************************************************
    Let's template the arguments
**************************************************************************/

template<
    void(*D)(Complex*),
    void(*DD)(DDComplex*),
    void(*QD)(QDComplex*)
>
inline void DispatchUnary()
    {
    switch (gManp->MathType)
	{
	case DOUBLEFLOAT:
	    D(&Ctx().Arg1->d);
	    break;

	case DOUBLEDOUBLE:
	    DD(&Ctx().Arg1->dd);
	    break;

	case QUADDOUBLE:
	    QD(&Ctx().Arg1->qd);
	    break;
	}
    }

template<
    void(*D)(Complex*, const Complex*),
    void(*DD)(DDComplex*, const DDComplex*),
    void(*QD)(QDComplex*, const QDComplex*)
>
inline void DispatchBinaryPop()
    {
    switch (gManp->MathType)
	{
	case DOUBLEFLOAT:
	    D(&Ctx().Arg2->d, &Ctx().Arg1->d);
	    break;

	case DOUBLEDOUBLE:
	    DD(&Ctx().Arg2->dd, &Ctx().Arg1->dd);
	    break;

	case QUADDOUBLE:
	    QD(&Ctx().Arg2->qd, &Ctx().Arg1->qd);
	    break;
	}

    Ctx().Arg1--;
    Ctx().Arg2--;
    }

