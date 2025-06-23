#ifndef PROTOTYP_H
#define PROTOTYP_H

// includes needed to define the prototypes 

#include "mpmath.h"
#include "..\big.h"
#include "fractint.h"
#include "externs.h"
#include <windows.h>
#include <math.h>
#include <stdio.h>

extern void cdecl FPUcplxmul(Complex *, Complex *, Complex *);
extern void cdecl FPUcplxdiv(Complex *, Complex *, Complex *);
extern void cdecl FPUsincos(double *, double *, double *);
extern void cdecl FPUsinhcosh(double *, double *, double *);
extern void cdecl FPUcplxlog(Complex *, Complex *);

extern Complex ComplexPower(Complex, Complex);
extern void cvtcorners(double, double, LDBL, double, double, double);
extern Complex ComplexPower(Complex, Complex);
extern void cvtcentermag(double *, double *, LDBL *, double *, double *, double *);
extern int Formula(void);
extern int FormPerPixel(void);
extern int find_file_item(HWND, char *, char *, FILE **);
extern void free_workarea(void);

#endif