/*
    MPMATH.H - argument structures for formula parser

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/
 
#ifndef MPMATH_H
#define MPMATH_H

#ifndef _CMPLX_DEFINED
#include "cmplx.h"
#endif

#include "..\qdlib\dd_real.h"
#include "..\qdlib\qd_real.h"
#include "..\ManpWIN64\Complex.h"
#include "..\ManpWIN64\DDComplex.h"
#include "..\ManpWIN64\QDComplex.h"

 /*** Formula Declarations ***/
struct Arg 
    {
    Complex	d;
    DDComplex	dd;
    QDComplex	qd;
    };

struct ConstArg 
    {
    char *s;
    int len;
    struct Arg a;
    };

extern void(*dtrig0)(void);
extern void(*dtrig1)(void);

/* -------------------------------------------------------------------- */
/*   The following #defines allow the complex transcendental functions  */
/*   in parser.cpp to be used here thus avoiding duplicated code.         */
/* -------------------------------------------------------------------- */

#define  CMPLXtrig0(arg,out) Arg1->d = (arg); dtrig0(); (out)=Arg1->d
#define  CMPLXtrig1(arg,out) Arg1->d = (arg); dtrig1(); (out)=Arg1->d

#endif
