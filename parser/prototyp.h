/*
    Prototyp.h - used for a few functions in formula parser.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#ifndef PROTOTYP_H
#define PROTOTYP_H

// includes needed to define the prototypes 

#include "mpmath.h"
#include "..\ManpWIN64\big.h"
#include "..\ManpWIN64\ManpWin.h"
#include "fractint.h"
#include "externs.h"
#include <windows.h>
#include <math.h>
#include <stdio.h>

extern void cdecl FPUsincos(double *, double *, double *);

extern void cvtcorners(double, double, LDBL, double, double, double);
extern Complex ComplexPower(Complex, Complex);
extern void cvtcentermag(double *, double *, LDBL *, double *, double *, double *);
extern int find_file_item(HWND, char *, char *, FILE **);
extern void free_workarea(void);

#endif