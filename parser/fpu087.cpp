/* Fpu087.cpp
 * This file contains routines to replace fpu087.asm.
 *
 * This file Copyright 1991 Ken Shirriff.  It may be used according to the
 * fractint license conditions, blah blah blah.
 */

#include "..\Complex.h"

int	DivideOverflow;

void FPUcplxmul(Complex *x, Complex *y, Complex *z)
{
    double tx;
    tx = x->x * y->x - x->y * y->y;
    z->y = x->x * y->y + x->y * y->x;
    z->x = tx;
}

void FPUcplxdiv(Complex *x, Complex *y, Complex *z)
{
    double mod,tx,yxmod,yymod;
    mod = y->x * y->x + y->y * y->y;
    if (mod==0) {
	DivideOverflow++;
    }
    yxmod = y->x/mod;
    yymod = - y->y/mod;
    tx = x->x * yxmod - x->y * yymod;
    z->y = x->x * yymod + x->y * yxmod;
    z->x = tx;
}

void FPUsinhcosh(double *Angle, double *Sinh, double *Cosh)
{
    *Sinh = sinh(*Angle);
    *Cosh = cosh(*Angle);
}

void FPUcplxlog(Complex *x, Complex *z)
{
    double mod,zx,zy;
    mod = sqrt(x->x*x->x + x->y*x->y);
    zx = log(mod);
    zy = atan2(x->y,x->x);

    z->x = zx;
    z->y = zy;
}

