/*
 *  REMOVED FORMAL PARAMETERS FROM FUNCTION DEFINITIONS (1/4/92)
 */
 
#ifndef MPMATH_H
#define MPMATH_H

#ifndef _CMPLX_DEFINED
#include "cmplx.h"
#endif

#include "..\qdlib\dd_real.h"
#include "..\qdlib\qd_real.h"
#include "..\Complex.h"
#include "..\DDComplex.h"
#include "..\QDComplex.h"

 /*** Formula Declarations ***/
//enum MATH_TYPE { DOUBLEFLOAT, DOUBLEDOUBLE, QUADDOUBLE};

 /* the following are declared 4 dimensional as an experiment */
 /* changeing declarations to Complex and _LCMPLX restores the code */
 /* to 2D */
struct Arg {
    Complex	d;
    DDComplex	dd;
    QDComplex	qd;
    };

struct ConstArg {
    char *s;
    int len;
    struct Arg a;
    };

extern struct Arg *Arg1, *Arg2;

extern void(*dtrig0)(void);
extern void(*dtrig1)(void);

/* -------------------------------------------------------------------- */
/*   The following #defines allow the complex transcendental functions  */
/*   in parser.c to be used here thus avoiding duplicated code.         */
/* -------------------------------------------------------------------- */

#define  CMPLXtrig0(arg,out) Arg1->d = (arg); dtrig0(); (out)=Arg1->d
#define  CMPLXtrig1(arg,out) Arg1->d = (arg); dtrig1(); (out)=Arg1->d

#endif
