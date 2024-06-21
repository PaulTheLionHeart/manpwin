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

/* orig code
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
*/

void FPUcplxdiv(Complex *x, Complex *y, Complex *z)
    {
    const double mod = y->x * y->x + y->y * y->y;
    if (mod == 0.0 || fabs(mod) <= DBL_MIN)
	{
//	z->x = ID_INFINITY;
//	z->y = ID_INFINITY;
	DivideOverflow++;
	return;
	}

    if (y->y == 0.0) // if y is real
	{
	z->x = x->x / y->x;
	z->y = x->y / y->x;
	}
    else
	{
	const double yxmod = y->x / mod;
	const double yymod = -y->y / mod;
	// Need to compute into temporaries to avoid pointer aliasing
	const double tx = x->x * yxmod - x->y * yymod;
	const double ty = x->x * yymod + x->y * yxmod;
	z->x = tx;
	z->y = ty;
	}
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

