/*
 *  REMOVED FORMAL PARAMETERS FROM FUNCTION DEFINITIONS (1/4/92)
 */
 
#ifndef MPMATH_H
#define MPMATH_H

#ifndef _CMPLX_DEFINED
#include "cmplx.h"
#endif
#include "..\Complex.h"

 /*** Formula Declarations ***/
enum MATH_TYPE { D_MATH, M_MATH, L_MATH };

 /* the following are declared 4 dimensional as an experiment */
 /* changeing declarations to Complex and _LCMPLX restores the code */
 /* to 2D */
struct Arg {
    Complex    d;
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
