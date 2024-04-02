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
extern void Arcsinz(Complex z, Complex *rz);
extern void Arccosz(Complex z, Complex *rz);
extern void Arcsinhz(Complex z, Complex *rz);
extern void Arccoshz(Complex z, Complex *rz);
extern void Arctanhz(Complex z, Complex *rz);
extern void Arctanz(Complex z, Complex *rz);
extern Complex ComplexPower(Complex, Complex);
extern void cvtcorners(double, double, LDBL, double, double, double);
extern Complex ComplexPower(Complex, Complex);
extern Complex ComplexSqrtFloat(double, double);
extern void cvtcentermag(double *, double *, LDBL *, double *, double *, double *);
extern int Formula(void);
extern int form_per_pixel(void);
extern int find_file_item(HWND, char *, char *, FILE **);
extern void free_workarea(void);

#endif