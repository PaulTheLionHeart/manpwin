//////////////////////////////////////////////////////////////////////
// PertFN.cpp a module with Perturbation functions and reference functions
// Code adapted from Kalles Fraktaler source http://www.chillheimer.de/kallesfraktaler/
// Written in Microsoft Visual C++ by Paul de Leeuw.
//////////////////////////////////////////////////////////////////////

#include "FrameCalculator.h"
#include "ExpComplex.h"

#define _abs(a) ((_abs_val=(a))>0?_abs_val:-_abs_val)

//////////////////////////////////////////////////////////////////////
// Individual function point calculations
//////////////////////////////////////////////////////////////////////

void	calculateFrame::BigPertFunctions(ExpComplex *XRef, ExpComplex *DeltaSubN, ExpComplex *DeltaSub0)

    {
    floatexp	Dnr, Dni;
    floatexp	r = XRef->x;
    floatexp	i = XRef->y;
    floatexp	a = DeltaSubN->x;
    floatexp	b = DeltaSubN->y;
    floatexp	a2 = a * a;
    floatexp	b2 = b * b;
    floatexp	a0 = DeltaSub0->x;
    floatexp	b0 = DeltaSub0->y;
    floatexp	ar = a * r;
    floatexp	ib = i * b;
    floatexp	ab;
    floatexp	x = r;
    floatexp	y = i;
    floatexp	x2 = x * x;
    floatexp	y2 = y * y;
    floatexp	r2 = r * r;
    floatexp	i2 = i * i;
    floatexp	c, d;
    floatexp	Multiplier;
    ExpComplex	Dn, z;
    floatexp	_2 = 2, _3 = 3, _5 = 5, _4 = 4, _6 = 6, _8 = 8, _9 = 9, _10 = 10, _12 = 12, _20 = 20, _24 = 24, _30 = 30, _40 = 40, _60 = 60;		// simplify multiplication by constants
    floatexp	temp;
    floatexp	_abs_val;
    ExpComplex	_1 = 1;

    switch (subtype)
	{
	case 0:								// Mandelbrot
	    Dnr = (r * 2 + a) * a - (i * 2 + b) * b + a0;
	    Dni = ((r + a) * b + i * a) * 2 + b0;
	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;

	case 1:								// Power
	    {
	    ExpComplex Zp(1.0, 0.0);
	    ExpComplex sum(0.0, 0.0);
	    for (int i = 0; i < power; i++)
		{
		sum += Zp * (double)PascalArray[i];
		sum *= *DeltaSubN;
		Zp *= *XRef;
		}
	    *DeltaSubN = sum;
	    *DeltaSubN += *DeltaSub0;
	    }
	    break;
	case 2:								// Burning Ship 
	    DeltaSubN->x = a * r * 2.0 + a2 - b * i * 2.0 - b2;
	    DeltaSubN->y = BigDiffAbs(r * i, r * b + i * a + a * b) * 2;
	    //	    DeltaSubN.y = 2.0 * (fabs(XSubN[iteration].x * (XSubN[iteration].y + DeltaSubN.y) + DeltaSubN.x * (XSubN[iteration].y + DeltaSubN.y)) - fabs(XSubN[iteration].x * XSubN[iteration].y)); // pixelation at 1.0E-18
	    *DeltaSubN += *DeltaSub0;
	    break;
	case 3:								// Cubic Burning Ship 
	    {
	    Dnr = BigDiffAbs(r, a);
	    ab = r + a;
	    Dnr = (r*r - i*i*3) * Dnr + (a*r*2 + a2 - i*b*6 - b2*3)*ab.abs() + a0;
	    Dni = BigDiffAbs(i, b);
	    ab = i + b;
	    Dni = (r*r*3 - i * i) * Dni + (r*a*6 + a2*3 - i*b*2 - b2) * ab.abs() + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    }
	    break;
	case 4:								// 4th Power Burning Ship 
	    Dnr = _4 * r2*r*a + _6 * r2*a2 + _4 * r*a2*a + a2 * a2 + _4 * i2*i*b + _6 * i2*b2 + _4 * i*b2*b + b2 * b2 - _12 * r*r*i*b - _6 * r2*b2 - _12 * r*a*i2 - _24 * r*a*i*b - _12 * r*a*b2 - _6 * a2*i2 - _12 * a2*i*b - _6 * a2*b2 + a0;
//	    Dnr = 4 * r2*r*a + 6 * r2*a2 + 4 * r*a2*a + a2 * a2 + 4 * i2*i*b + 6 * i2*b2 + 4 * i*b2*b + b2 * b2 - 12 * r2*i*b - 6 * r2*b2 - 12 * r*a*i2 - 24 * r*a*i*b - 12 * r*a*b2 - 6 * a2*i2 - 12 * a2*i*b - 6 * a2*b2 + a0;
	    Dni = BigDiffAbs(r * i, r * b + a * i + a * b);
	    temp = (r*i + r * b + a * i + a * b);
	    Dni = _4 * (r2 - i2)*(Dni)+ _4 * _abs(r*i + r * b + a * i + a * b)*(_2 * a*r + a2 - _2 * b*i - b2) - b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 5:								// 5th Power Burning Ship 
	    Dnr = BigDiffAbs(r, a);
	    Dnr = (Dnr) * (r2*r2 - _10 * r*r*i*i + _5 * i2*i2) + _abs(r + a) * (_4 * r*r*r*a + _6 * r*r*a2 + _4 * r*a2*a + a2 * a2 - _20 * r2*i*b - _10 * r2*b2 - _20 * r*a*i2 - _40 * r*a*i*b - _20 * r*a*b2 - _10 * a2*i2 - _20 * a2*i*b - _10 * a2*b2
		+ _20 * i2*i*b + _30 * i2*b2 + _20 * i*b2*b + _5 * b2*b2) + a0;
	    Dni = BigDiffAbs(i, b);
	    Dni = (Dni) * (_5 * r2*r2 - _10 * r2*i2 + i2 * i2) + _abs(i + b)*(_20 * r2*r*a + _30 * r2*a2 + _20 * r*a2*a + _5 * a2*a2 - _20 * r2*i*b - _10 * r2*b2 - _20 * r*a*i2 - _40 * r*a*i*b - _20 * r*a*b2 - _10 * a2*i2 - _20 * a2*i*b - _10 * a2*b2
		+ _4 * i2*i*b + _6 * i2*b2 + _4 * i*b2*b + b2 * b2) - b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 6:								// Celtic 
	    Dnr = BigDiffAbs(r2 - i2, (r * 2 + a)*a - (i * 2 + b)*b);
	    Dnr += a0;
	    Dni = r*b * 2 + a*(i + b) * 2 + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 7:								// Cubic Celtic
	    c = r*(r2 - _3 * i2);
	    d = a*(_3 * r2 + a2) + _3 * r*(a2 - _2 * i*b - b2) - _3 * a*(i2 + _2 * i*b + b2);
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    Dni = _3 * i*(_2 * r*a + a2 - b2) + _3 * b*(r2 + _2 * r*a + a2) - b*(b2 + _3 * i2) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 8:								// 4th Celtic Buffalo
	    c = r2*r2+i2*i2-_6*r2*i2;
	    d = _4*r2*r*a+_6*r2*a2+_4*r*a2*a+a2*a2 + _4*i2*i*b+_6*i2*b2+_4*i*b2*b+b2*b2 -_12*a*r*i2-_6*a2*i2-_12*b*r2*i-_24*a*b*r*i-_12*a2*b*i-_6*b2*r2-_12*a*b2*r-_6*a2*b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr += a0;
	    Dni = _12*r2*i*a+_12*r*i*a2-_12*r*i2*b-_12*r*i*b2+_4*r2*r*b+_12*r2*b*a+_12*r*b*a2-_4*r*b2*b+_4*a2*a*i-_4*a*i2*i-_12*a*i2*b-_12*a*i*b2+_4*a2*a*b-_4*a*b2*b + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 9:								// 5th Celtic
	    //Dnr = _abs((x+a) * ((x+a)*(x+a)*(x+a)*(x+a) - 10 * (x+a)*(x+a)*(y+b)*(y+b) + 5 * (y+b)*(y+b)*(y+b)*(y+b))) - _abs(x2*x2*x - 10*x2*x*y2 + 5*x*y2*y2) +a0;
	    //Dnr = _abs(5*x*y2*y2+20*x*b*y2*y-10*x2*x*y2-30*x2*a*y2+30*x*b2*y2-30*x*a2*y2-20*x2*x*b*y-60*x2*a*b*y+20*x*b2*b*y-60*x*a2*b*y+x2*x2*x+5*x2*x2*a-10*x2*x*b2+10*x2*x*a2-30*x2*a*b2
	    //	+10*x2*a2*a+5*x*b2*b2-30*x*a2*b2+5*x*a2*a2+5*a*y2*y2+20*a*b*y2*y+30*a*b2*y2-10*a2*a*y2+20*a*b2*b*y-20*a2*a*b*y+5*a*b2*b2-10*a2*a*b2+a2*a2*a) - _abs(x2*x2*x - 10*x2*x*y2 + 5*x*y2*y2) + a0;
	    c = r2*r2*r - _10*r2*r*i2 + _5*r*i2*i2;
	    d = _20*r*b*i2*i-_30*r2*a*i2+_30*r*b2*i2-_30*r*a2*i2-_20*r2*r*b*i-_60*r2*a*b*i+_20*r*b2*b*i-_60*r*a2*b*i+_5*r2*r2*a-_10*r2*r*b2+_10*r2*r*a2-_30*r2*a*b2+_10*r2*a2*a
			    +_5*r*b2*b2-_30*r*a2*b2+_5*r*a2*a2+_5*a*i2*i2+_20*a*b*i2*i+_30*a*b2*i2-_10*a2*a*i2+_20*a*b2*b*i-_20*a2*a*b*i+_5*a*b2*b2-_10*a2*a*b2+a2*a2*a;
	    Dnr = BigDiffAbs(c, d);
	    Dnr += a0;

	    Dni = _20*i*r2*r*a+_30*i*r2*a2+_20*i*r*a2*a+_5*i*a2*a2-_30*i2*r2*b-_30*i*r2*b2-_20*i2*i*r*a-_60*i2*r*a*b-_60*i*r*a*b2-_10*i2*i*a2-_30*i2*a2*b-_30*i*a2*b2+_5*i2*i2*b
			    +_10*i2*i*b2+_10*i2*b2*b+_5*i*b2*b2+_5*b*r2*r2+_20*b*r2*r*a+_30*b*r2*a2+_20*b*r*a2*a+_5*b*a2*a2-_10*b2*b*r2-_20*b2*b*r*a-_10*b2*b*a2+b2*b2*b +b0;
	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 10:							// Mandelbar (Tricorn) 
	    Dnr = r*a * 2 + a2 - b2 - b*i * 2 + a0;
	    Dni = b0 - (r*b + a * i + a * b) * 2;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 11:							// Mandelbar (power) 
	    {
	    ExpComplex Zp(1.0, 0.0);
	    ExpComplex sum(0.0, 0.0);
	    for (int i = 0; i < power; i++)
		{
		sum += Zp * (double)PascalArray[i];
		sum *= *DeltaSubN;
		Zp *= *XRef;
		}
	    DeltaSubN->x = sum.x;
	    DeltaSubN->y = -sum.y;
	    *DeltaSubN += *DeltaSub0;
	    }
	    break;
	case 12:							// Buffalo
	    Dnr = BigDiffAbs(r2 - i2, _2 * r*a + a2 - _2 * i*b - b2);
	    Dnr = Dnr + a0;
	    Dni = BigDiffAbs(r * i, r * b + a * i + a * b);
	    Dni = b0 - _2 * Dni;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 13:							// Cubic Buffalo
	    c = r * (r2 - _3 * i2);
	    d = a * (_3 * r2 + a2) + _3 * r*(a2 - _2 * i*b - b2) - _3 * a*(i2 + _2 * i*b + b2);
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = i * (_3 * r2 - i2);
	    d = _3 * i*(_2 * r*a + a2 - b2) + _3 * b*(r2 + _2 * r*a + a2) - b * (_3 * i2 + b2);
	    Dni = BigDiffAbs(c, d);
	    Dni = Dni + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 14:							// 4th power Buffalo
	    c = r2 * r2 + i2 * i2 - _6 * r2*i2;
	    d = _4 * r2*r*a + _6 * r2*a2 + _4 * r*a2*a + a2 * a2 + _4 * i2*i*b + _6 * i2*b2 + _4 * i*b2*b + b2 * b2 - _12 * r2*i*b - _6 * r2*b2 - _12 * r*a*i2 - _24 * r*a*i*b - _12 * r*a*b2 - _6 * a2*i2 - _12 * a2*i*b - _6 * a2*b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = _4 * r2*r*i - _4 * r*i2*i;
	    d = -_4 * a*i2*i - _12 * b*r*i2 - _12 * a*b*i2 + _12 * a*r2*i - _12 * b2*r*i + _12 * a2*r*i - _12 * a*b2*i + _4 * a2*a*i + _4 * b*r2*r + _12 * a*b*r2 - _4 * b2*b*r + _12 * a2*b*r - _4 * a*b2*b + _4 * a2*a*b;
	    Dni = BigDiffAbs(c, d);
	    Dni = Dni + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 15:							// 5th power Buffalo
	    c = _5 * r*i2*i2 - _10 * r2*r*i2 + r2 * r2*r;
	    d = _20 * r*b*i2*i - _30 * r2*a*i2 + _30 * r*b2*i2 - _30 * r*a2*i2 - _20 * r2*r*b*i - _60 * r2*a*b*i + _20 * r*b2*b*i - _60 * r*a2*b*i + _5 * r2*r2*a - _10 * r2*r*b2 + _10 * r2*r*a2 
				- _30 * r2*a*b2 + _10 * r2*a2*a + _5 * r*b2*b2 - _30 * r*a2*b2 + _5 * r*a2*a2 + _5 * a*i2*i2 + _20 * a*b*i2*i + _30 * a*b2*i2 - _10 * a2*a*i2 + _20 * a*b2*b*i - _20 * a2*a*b*i + _5 * a*b2*b2 - _10 * a2*a*b2 + a2 * a2*a;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = i2 * i2*i - _10 * i2*i*r2 + _5 * i*r2*r2;
	    d = _5 * i2*i2*b - _20 * i2*i*a*r + _10 * i2*i*b2 - _10 * i2*i*a2 - _30 * i2*b*r2 - _60 * i2*a*b*r + _10 * i2*b2*b - _30 * i2*a2*b + _20 * i*a*r2*r - _30 * i*b2*r2 + _30 * i*a2*r2 - _60 * i*a*b2*r + _20 * i*a2*a*r + _5 * i*b2*b2 
							- _30 * i*a2*b2 + _5 * i*a2*a2 + _5 * b*r2*r2 + _20 * b*a*r2*r - _10 * b2*b*r2 + _30 * b*a2*r2 - _20 * b2*b*a*r + _20 * b*a2*a*r + b2 * b2*b - _10 * b2*b*a2 + _5 * b*a2*a2;
	    Dni = BigDiffAbs(c, d);
	    Dni = Dni + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 16:							// Mandelbar Celtic
	    c = r * r - i * i;
	    d = _2 * r*a + a2 - _2 * i*b - b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    Dni = b0 - _2 * (r*b + a * i + a * b);

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 17:							// Perpendicular Mandelbrot
	    Dnr = _2 * r*a + a2 - b2 - _2 * b*i + a0;
	    c = r;
	    d = a;
	    Dni = BigDiffAbs(c, d);
	    Dni = b0 - Dni * i*2 - _abs(r + a)*b*2;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 18:							// Perpendicular Burning Ship
	    Dnr = _2 * r*a + a2 - b2 - _2 * b*i + a0;
	    c = i;
	    d = b;
	    Dni = BigDiffAbs(c, d);
	    Dni = b0 - Dni * r*2 - a * _abs(i + b)*2;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 19:							// Perpendicular Celtic
	    c = r * r - i * i;
	    d = _2 * r*a + a2 - _2 * i*b - b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = r;
	    d = a;
	    Dni = BigDiffAbs(c, d);
	    Dni = b0 - _abs(r + a)*b*2 - Dni * i*2;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 20:							// Perpendicular Buffalo
	    c = r * r - i * i;
	    d = _2 * r*a + a2 - _2 * i*b - b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = i;
	    d = b;
	    Dni = BigDiffAbs(c, d);
	    Dni = b0 - Dni * r*2 - a * _abs(i + b)*2;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 21:							// Cubic Quasi Burning Ship
	    ar = a*r;
	    ib = i*b;
	    Dnr = BigDiffAbs(r, a);
	    ab = r + a;
	    Dnr = (r2 - _3 * i2) * Dnr + (_2 * ar + a2 - _6 * ib - _3 * b2)*_abs(ab) + a0;
	    c = i*(_3 * r2 - i2);
	    d = _3 * i*(_2 * r*a + a2 - b2) + _3 * b*(r2 + _2 * r*a + a2) - b*(_3 * i2 + b2);
	    Dni = BigDiffAbs(c, d);
	    Dni = -b0 - Dni;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 22:							// Cubic Partial BS Real
	    ar = a*r;
	    ib = i*b;
	    Dnr = BigDiffAbs(r, a);
	    ab = r + a;
	    Dnr = (r2 - _3 * i2) * Dnr + (_2 * ar + a2 - _6 * ib - _3 * b2)*_abs(ab) + a0;
	    Dni = _6 * r*(i*a + a*b) + _3 * i*(a2 - b2) + _3 * b*(r2 - i2) + b*(_3 * a2 - b2) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 23:							// Cubic Partial BS Imag
	    ar = a*r;
	    ib = i*b;
	    Dnr = _3 * r2*a + _3 * r*a2 - _6 * r*i*b - _3 * r*b2 + a*a2 - _3 * i2*a - _6 * i*a*b - _3 * a*b2 + a0;
	    Dni = BigDiffAbs(i, b);
	    ab = i + b;
	    Dni = (_3 * r2 - i2) * Dni + (_6 * ar + _3 * a2 - _2 * ib - b2) * _abs(ab) - b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 24:							// Cubic Flying Squirrel
	    Dnr = _3 * r2*a + _3 * r*a2 - _6 * r*i*b - _3 * r*b2 + a*a2 - _3 * i2*a - _6 * i*a*b - _3 * a*b2 + a0;
	    c = i*(_3 * r2 - i2);
	    d = _3 * i*(_2 * r*a + a2 - b2) + _3 * b*(r2 + _2 * r*a + a2) - b*(_3 * i2 + b2);
	    Dni = BigDiffAbs(c, d);
	    Dni = Dni + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 25:							// Cubic Quasi Perpendicular
	    ar = a*r;
	    ib = i*b;
	    Dnr = BigDiffAbs(r, a);
	    ab = r + a;
	    Dnr = (r2 - _3 * i2) * Dnr + (_2 * ar + a2 - _6 * ib - _3 * b2)*_abs(ab) + a0;
	    c = _3 * r2 - i2;
	    d = _6 * r*a + _3 * a2 - _2 * i*b - b2;
	    Dni = BigDiffAbs(c, d);
	    ab = _3 * r2 + _6 * r*a + _3 * a2 - i2 - _2 * i*b - b2;
	    Dni = b0 - Dni*i - _abs(ab)*b;
	
	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 26:							// 4th Burning Ship Partial Imag
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 + a0;
	    c = y;
	    d = b;
	    Dni = BigDiffAbs(c, d);
	    Dni = _abs(y+b)*(_12*x2*a+_12*x*a2+_4*a2*a - _4*a*y2-_8*b*x*y-_8*a*b*y-_4*b2*x-_4*a*b2) + Dni*(_4*x2*x - _4*x*y2) - b0;
						
	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 27:							// 4th Burning Ship Partial Real
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 + a0;
	    c = x;
	    d = a;
	    Dni = BigDiffAbs(c, d);
	    Dni = _abs(x+a)*(_4*x2*b+_8*x*a*y+_8*x*a*b+_4*a2*y+_4*a2*b - _12*b*y2-_12*b2*y-_4*b2*b) + Dni*(_4*x2*y - _4*y2*y) + b0;
						
	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 28:							// 4th Burning Ship Partial Real Mbar
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 + a0;
	    c = x;
	    d = a;
	    Dni = BigDiffAbs(c, d);
	    Dni = _abs(x+a)*(_12*y2*b+_12*y*b2+_4*b2*b - _8*a*x*y-_4*a2*y-_4*b*x2-_8*a*b*x-_4*a2*b) + Dni*(_4*y2*y - _4*x2*y) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 29:							// 4th Celtic Burning Ship Partial Imag
	    c = x2*x2 + y2*y2 - _6*x2*y2;
	    d = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 + _4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2 - _12*a*x*y2-_6*a2*y2-_12*b*x2*y-_24*a*b*x*y-_12*a2*b*y-_6*b2*x2-_12*a*b2*x-_6*a2*b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = y;
	    d = b;
	    Dni = BigDiffAbs(c, d);
	    Dni = _abs(y+b)*(_12*x2*a+_12*x*a2+_4*a2*a - _4*a*y2-_8*b*x*y-_8*a*b*y-_4*b2*x-_4*a*b2) + Dni*(_4*x2*x - _4*x*y2) - b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 30:							// 4th Celtic Burning Ship Partial Real
	    c = x2*x2 + y2*y2 - _6*x2*y2;
	    d = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 + _4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2 - _12*a*x*y2-_6*a2*y2-_12*b*x2*y-_24*a*b*x*y-_12*a2*b*y-_6*b2*x2-_12*a*b2*x-_6*a2*b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = x;
	    d = a;
	    Dni = BigDiffAbs(c, d);
	    Dni = _abs(x+a)*(_4*x2*b+_8*x*a*y+_8*x*a*b+_4*a2*y+_4*a2*b - _12*b*y2-_12*b2*y-_4*b2*b) + Dni*(_4*x2*y - _4*y2*y) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 31:							// 4th Celtic Burning Ship Partial Real Mbar
	    c = x2*x2 + y2*y2 - _6*x2*y2;
	    d = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 + _4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2 - _12*a*x*y2-_6*a2*y2-_12*b*x2*y-_24*a*b*x*y-_12*a2*b*y-_6*b2*x2-_12*a*b2*x-_6*a2*b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = x;
	    d = a;
	    Dni = BigDiffAbs(c, d);
	    Dni = _abs(x+a)*(_12*y2*b+_12*y*b2+_4*b2*b - _8*a*x*y-_4*a2*y-_4*b*x2-_8*a*b*x-_4*a2*b) + Dni*(_4*y2*y - _4*x2*y) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 32:							// 4th Buffalo Partial Imag
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 + a0;
	    c = _4*x2*x*y-_4*x*y2*y;
	    d = -_4*a*y2*y-_12*b*x*y2-_12*a*b*y2+_12*a*x2*y-_12*b2*x*y+_12*a2*x*y-_12*a*b2*y+_4*a2*a*y+_4*b*x2*x+_12*a*b*x2-_4*b2*b*x+_12*a2*b*x-_4*a*b2*b+_4*a2*a*b;
	    Dni = BigDiffAbs(c, d);
	    Dni=Dni-b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 33:							// 4th Celtic Mbar
	    c = x2*x2 + y2*y2 - _6*x2*y2;
	    d = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 + _4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2 - _12*a*x*y2-_6*a2*y2-_12*b*x2*y-_24*a*b*x*y-_12*a2*b*y-_6*b2*x2-_12*a*b2*x-_6*a2*b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    Dni = b0 - (_12*x2*y*a+_12*x*y*a2-_12*x*y2*b-_12*x*y*b2+_4*x2*x*b+_12*x2*b*a+_12*x*b*a2-_4*x*b2*b+_4*a2*a*y-_4*a*y2*y-_12*a*y2*b-_12*a*y*b2+_4*a2*a*b-_4*a*b2*b);
						
	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 34:							// 4th False Quasi Perpendicular
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 + a0;
	    c = x2-y2;
	    d = -_2*b*y+_2*a*x-b2+a2;
	    Dni = BigDiffAbs(c, d);
	    Dni = -(_4*x*y)*Dni - (_4*x*b + _4*a*y + _4*a*b)*_abs(-y2-_2*b*y+x2+_2*a*x-b2+a2) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 35:							// 4th False Quasi Heart
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 + a0;
	    c = x2-y2;
	    d = -_2*b*y+_2*a*x-b2+a2;
	    Dni = BigDiffAbs(c, d);
	    Dni = (_4*x*y)*Dni + (_4*x*b + _4*a*y + _4*a*b)*_abs(-y2-_2*b*y+x2+_2*a*x-b2+a2) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 36:							// 4th Celtic False Quasi Perpendicular
	    c = x2*x2 + y2*y2 - _6*x2*y2;
	    d = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 + _4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2 - _12*a*x*y2-_6*a2*y2-_12*b*x2*y-_24*a*b*x*y-_12*a2*b*y-_6*b2*x2-_12*a*b2*x-_6*a2*b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = x2-y2;
	    d = -_2*b*y+_2*a*x-b2+a2;
	    Dni = BigDiffAbs(c, d);
	    Dni = -(_4*x*y)*Dni - (_4*x*b + _4*a*y + _4*a*b)*_abs(-y2-_2*b*y+x2+_2*a*x-b2+a2) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 37:							// 4th Celtic False Quasi Heart
	    c = x2*x2 + y2*y2 - _6*x2*y2;
	    d = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 + _4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2 - _12*a*x*y2-_6*a2*y2-_12*b*x2*y-_24*a*b*x*y-_12*a2*b*y-_6*b2*x2-_12*a*b2*x-_6*a2*b2;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = Dnr + a0;
	    c = x2-y2;
	    d = -_2*b*y+_2*a*x-b2+a2;
	    Dni = BigDiffAbs(c, d);
	    Dni = (_4*x*y)*Dni + (_4*x*b + _4*a*y + _4*a*b)*_abs(-y2-_2*b*y+x2+_2*a*x-b2+a2) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 38:							// 4th Imag Quasi Perpendicular / Heart
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 +a0;
	    Dni = _4 * (a) * _abs(- y2*y-_3*b*y2+x2*y+_2*a*x*y-_3*b2*y+a2*y+b*x2+_2*a*b*x-b2*b+a2*b) + _4 * x * BigDiffAbs(- y2*y+x2*y,-_3*b*y2+_2*a*x*y-_3*b2*y+a2*y+b*x2+_2*a*b*x-b2*b+a2*b) - b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 39:							// 4th Real Quasi Perpendicular
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 +a0;
	    Dni = -_4*y*BigDiffAbs(x2*x-x*y2,-a*y2-_2*b*x*y-_2*a*b*y+_3*a*x2-b2*x+_3*a2*x-a*b2+a2*a) - _4*b*_abs(- x*y2-a*y2-_2*b*x*y-_2*a*b*y+x2*x+_3*a*x2-b2*x+_3*a2*x-a*b2+a2*a) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 40:							// 4th Real Quasi Heart
	    Dnr = _4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2+_4*y2*y*b+_6*y2*b2+_4*y*b2*b+b2*b2-_12*x2*y*b-_6*x2*b2-_12*x*a*y2-_24*x*a*y*b-_12*x*a*b2-_6*a2*y2-_12*a2*y*b-_6*a2*b2 +a0;
	    Dni = _4*y*BigDiffAbs(x2*x-x*y2,-a*y2-_2*b*x*y-_2*a*b*y+_3*a*x2-b2*x+_3*a2*x-a*b2+a2*a) + _4*b*_abs(- x*y2-a*y2-_2*b*x*y-_2*a*b*y+x2*x+_3*a*x2-b2*x+_3*a2*x-a*b2+a2*a) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 41:							// 4th Celtic Imag Quasi Perpendicular / Heart
	    Dnr = BigDiffAbs(x2*x2 + y2*y2 - _6*x2*y2,_4*y2*y*b-_12*y2*a*x+_6*y2*b2-_6*y2*a2-_12*x2*y*b-_24*x*y*a*b+_4*b2*b*y-_12*b*y*a2+_4*x2*x*a-_6*x2*b2+_6*x2*a2-_12*b2*x*a+_4*a2*a*x+b2*b2-_6*b2*a2+a2*a2) +a0;
	    Dni = _4 * a * _abs(- y2*y-_3*b*y2+x2*y+_2*a*x*y-_3*b2*y+a2*y+b*x2+_2*a*b*x-b2*b+a2*b) + _4 * x * BigDiffAbs(- y2*y+x2*y,-_3*b*y2+_2*a*x*y-_3*b2*y+a2*y+b*x2+_2*a*b*x-b2*b+a2*b) - b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 42:							// 4th Celtic Real Quasi Perpendicular
	    Dnr = BigDiffAbs(x2*x2 + y2*y2 - _6*x2*y2,_4*y2*y*b-_12*y2*a*x+_6*y2*b2-_6*y2*a2-_12*x2*y*b-_24*x*y*a*b+_4*b2*b*y-_12*b*y*a2+_4*x2*x*a-_6*x2*b2+_6*x2*a2-_12*b2*x*a+_4*a2*a*x+b2*b2-_6*b2*a2+a2*a2) +a0;
	    Dni = -_4*y*BigDiffAbs(x2*x-x*y2,-a*y2-_2*b*x*y-_2*a*b*y+_3*a*x2-b2*x+_3*a2*x-a*b2+a2*a) - _4*b*_abs(- x*y2-a*y2-_2*b*x*y-_2*a*b*y+x2*x+_3*a*x2-b2*x+_3*a2*x-a*b2+a2*a) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 43:							// 4th Celtic Real Quasi Heart
	    Dnr = BigDiffAbs(x2*x2 + y2*y2 - _6*x2*y2,_4*y2*y*b-_12*y2*a*x+_6*y2*b2-_6*y2*a2-_12*x2*y*b-_24*x*y*a*b+_4*b2*b*y-_12*b*y*a2+_4*x2*x*a-_6*x2*b2+_6*x2*a2-_12*b2*x*a+_4*a2*a*x+b2*b2-_6*b2*a2+a2*a2) +a0;
	    Dni = _4*y*BigDiffAbs(x2*x-x*y2,-a*y2-_2*b*x*y-_2*a*b*y+_3*a*x2-b2*x+_3*a2*x-a*b2+a2*a) + _4*b*_abs(- x*y2-a*y2-_2*b*x*y-_2*a*b*y+x2*x+_3*a*x2-b2*x+_3*a2*x-a*b2+a2*a) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 44:							// 5th Burning Ship Partial
	    c = x;
	    d = a;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = (Dnr) * (x2*x2- _10*x2*y2 + _5*y2*y2) + _abs(x+a) * (_4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 -_20*x2*y*b-_10*x2*b2-_20*x*a*y2-_40*x*a*y*b-_20*x*a*b2-_10*a2*y2-_20*a2*y*b-_10*a2*b2 + _20*y2*y*b+_30*y2*b2+_20*y*b2*b+_5*b2*b2) + a0;
	    Dni = _20*y*x2*x*a+_30*y*x2*a2+_20*y*x*a2*a+_5*y*a2*a2-_30*y2*x2*b-_30*y*x2*b2-_20*y2*y*x*a-_60*y2*x*a*b-_60*y*x*a*b2-_10*y2*y*a2-_30*y2*a2*b-_30*y*a2*b2+_5*y2*y2*b+_10*y2*y*b2+_10*y2*b2*b+_5*y*b2*b2+_5*b*x2*x2+_20*b*x2*x*a
												+_30*b*x2*a2+_20*b*x*a2*a+_5*b*a2*a2-_10*b2*b*x2-_20*b2*b*x*a-_10*b2*b*a2+b2*b2*b + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 45:							// 5th Burning Ship Partial Mbar
	    c = x;
	    d = a;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = (Dnr) * (x2*x2- _10*x2*y2 + _5*y2*y2) + _abs(x+a) * (_4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 -_20*x2*y*b-_10*x2*b2-_20*x*a*y2-_40*x*a*y*b-_20*x*a*b2-_10*a2*y2-_20*a2*y*b-_10*a2*b2 + _20*y2*y*b+_30*y2*b2+_20*y*b2*b+_5*b2*b2) + a0;
	    Dni = b0 - (_20*y*x2*x*a+_30*y*x2*a2+_20*y*x*a2*a+_5*y*a2*a2-_30*y2*x2*b-_30*y*x2*b2-_20*y2*y*x*a-_60*y2*x*a*b-_60*y*x*a*b2-_10*y2*y*a2-_30*y2*a2*b-_30*y*a2*b2+_5*y2*y2*b+_10*y2*y*b2+_10*y2*b2*b+_5*y*b2*b2+_5*b*x2*x2+_20*b*x2*x*a
												+_30*b*x2*a2+_20*b*x*a2*a+_5*b*a2*a2-_10*b2*b*x2-_20*b2*b*x*a-_10*b2*b*a2+b2*b2*b);

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 46:							// 5th Celtic Mbar
	    c = _5*x*y2*y2-_10*x2*x*y2+x2*x2*x;
	    d = _20*x*b*y2*y-_30*x2*a*y2+_30*x*b2*y2-_30*x*a2*y2-_20*x2*x*b*y-_60*x2*a*b*y+_20*x*b2*b*y-_60*x*a2*b*y+_5*x2*x2*a-_10*x2*x*b2+_10*x2*x*a2-_30*x2*a*b2+_10*x2*a2*a+_5*x*b2*b2-_30*x*a2*b2+_5*x*a2*a2+_5*a*y2*y2+_20*a*b*y2*y
												+_30*a*b2*y2-_10*a2*a*y2+_20*a*b2*b*y-_20*a2*a*b*y+_5*a*b2*b2-_10*a2*a*b2+a2*a2*a;
	    Dnr = BigDiffAbs(c, d);
	    Dnr+=a0;
	    Dni = b0 - (_20*y*x2*x*a+_30*y*x2*a2+_20*y*x*a2*a+_5*y*a2*a2-_30*y2*x2*b-_30*y*x2*b2-_20*y2*y*x*a-_60*y2*x*a*b-_60*y*x*a*b2-_10*y2*y*a2-_30*y2*a2*b-_30*y*a2*b2+_5*y2*y2*b+_10*y2*y*b2+_10*y2*b2*b+_5*y*b2*b2+_5*b*x2*x2
												+_20*b*x2*x*a+_30*b*x2*a2+_20*b*x*a2*a+_5*b*a2*a2-_10*b2*b*x2-_20*b2*b*x*a-_10*b2*b*a2+b2*b2*b);

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;

	case 47:							// 5th Quasi Burning Ship (BS/Buffalo Hybrid)
	    c = x;
	    d = a;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = (Dnr) * (x2*x2- _10*x2*y2 + _5*y2*y2) + _abs(x+a) * (_4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 -_20*x2*y*b-_10*x2*b2-_20*x*a*y2-_40*x*a*y*b-_20*x*a*b2-_10*a2*y2-_20*a2*y*b-_10*a2*b2 + _20*y2*y*b+_30*y2*b2+_20*y*b2*b+_5*b2*b2) + a0;
	    c = y2*y2*y-_10*y2*y*x2+_5*y*x2*x2;
	    d = _5*y2*y2*b-_20*y2*y*a*x+_10*y2*y*b2-_10*y2*y*a2-_30*y2*b*x2-_60*y2*a*b*x+_10*y2*b2*b-_30*y2*a2*b+_20*y*a*x2*x-_30*y*b2*x2+_30*y*a2*x2-_60*y*a*b2*x+_20*y*a2*a*x+_5*y*b2*b2-_30*y*a2*b2+_5*y*a2*a2+_5*b*x2*x2+_20*b*a*x2*x-_10*b2*b*x2
												+_30*b*a2*x2-_20*b2*b*a*x+_20*b*a2*a*x+b2*b2*b-_10*b2*b*a2+_5*b*a2*a2;
	    Dni = BigDiffAbs(c, d);
	    Dni = -b0-Dni;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 48:							// 5th Quasi Perpendicular
	    c = x;
	    d = a;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = (Dnr) * (x2*x2- _10*x2*y2 + _5*y2*y2) + _abs(x+a) * (_4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 -_20*x2*y*b-_10*x2*b2-_20*x*a*y2-_40*x*a*y*b-_20*x*a*b2-_10*a2*y2-_20*a2*y*b-_10*a2*b2 + _20*y2*y*b+_30*y2*b2+_20*y*b2*b+_5*b2*b2) + a0;
	    c = _5*x2*x2 - _10*x2*y2 + y2*y2;
	    d = _4*b*y2*y-_20*a*x*y2+_6*b2*y2-_10*a2*y2-_20*b*x2*y-_40*a*b*x*y+_4*b2*b*y-_20*a2*b*y+_20*a*x2*x-_10*b2*x2+_30*a2*x2-_20*a*b2*x+_20*a2*a*x+b2*b2-_10*a2*b2+_5*a2*a2;
	    Dni = BigDiffAbs(c, d);
	    Dni = -y * Dni - b * _abs(y2*y2+_4*b*y2*y-_10*x2*y2-_20*a*x*y2+_6*b2*y2-_10*a2*y2-_20*b*x2*y-_40*a*b*x*y+_4*b2*b*y-_20*a2*b*y+_5*x2*x2+_20*a*x2*x-_10*b2*x2+_30*a2*x2-_20*a*b2*x+_20*a2*a*x+b2*b2-_10*a2*b2+_5*a2*a2) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 49:							// 5th Quasi Heart
	    c = x;
	    d = a;
	    Dnr = BigDiffAbs(c, d);
	    Dnr = (Dnr) * (x2*x2- _10*x2*y2 + _5*y2*y2) + _abs(x+a) * (_4*x2*x*a+_6*x2*a2+_4*x*a2*a+a2*a2 -_20*x2*y*b-_10*x2*b2-_20*x*a*y2-_40*x*a*y*b-_20*x*a*b2-_10*a2*y2-_20*a2*y*b-_10*a2*b2 + _20*y2*y*b+_30*y2*b2+_20*y*b2*b+_5*b2*b2) + a0;
	    c = _5*x2*x2 - _10*x2*y2 + y2*y2;
	    d = _4*b*y2*y-_20*a*x*y2+_6*b2*y2-_10*a2*y2-_20*b*x2*y-_40*a*b*x*y+_4*b2*b*y-_20*a2*b*y+_20*a*x2*x-_10*b2*x2+_30*a2*x2-_20*a*b2*x+_20*a2*a*x+b2*b2-_10*a2*b2+_5*a2*a2;
	    Dni = BigDiffAbs(c, d);
	    Dni = y * Dni + b * _abs(y2*y2+_4*b*y2*y-_10*x2*y2-_20*a*x*y2+_6*b2*y2-_10*a2*y2-_20*b*x2*y-_40*a*b*x*y+_4*b2*b*y-_20*a2*b*y+_5*x2*x2+_20*a*x2*x-_10*b2*x2+_30*a2*x2-_20*a*b2*x+_20*a2*a*x+b2*b2-_10*a2*b2+_5*a2*a2) + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 50:							// SimonBrot
	    //Dnr = b*b*abs(b*b)-4*b*abs(a*b)*a-a*a*abs(b*b)-b*b*abs(a*a)+a*a*abs(a*a) + a0;
	    //Dni = - 2*a*b*abs(b*b)-2*b*b*abs(a*b)+2*a*a*abs(a*b)+2*a*b*abs(a*a) + b0;
	    Dnr = (y2)*BigDiffAbs(y2,_2*b*y+b2)-_4*y*x*BigDiffAbs(x*y,x*b+a*y+a*b)-x2* BigDiffAbs(y2,_2*b*y+b2)-y2* BigDiffAbs(x2,_2*x*a+a2)+x2* BigDiffAbs(x2,_2*x*a+a2)
							+ (_2*b*y+b2)*_abs(y2+_2*b*y+b2)-_4*(y*a+b*x+b*a)*_abs(x*y+x*b+a*y+a*b)-(_2*x*a+a2)*_abs(y2+_2*b*y+b2)-(_2*b*y+b2)*_abs(x2+_2*x*a+a2)+(_2*x*a+a2)*_abs(x2+_2*x*a+a2) + a0;
	    Dni = _2*x2*BigDiffAbs(x*y,x*b+a*y+a*b)+_2*x*y*BigDiffAbs(x2,_2*x*a+a2)-_2*x*y*BigDiffAbs(y2,_2*b*y+b2)-_2*y2*BigDiffAbs(x*y,x*b+a*y+a*b)
							+ _2*(_2*x*a+a2)*_abs(x*y+x*b+a*y+a*b) +_2*(x*b+a*y+a*b)*_abs(x2+_2*x*a+a2)-_2*(x*b+a*y+a*b)*_abs(y2+_2*b*y+b2)-_2*(_2*b*y+b2)*_abs(x*y+x*b+a*y+a*b)  + b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	case 51:							// Cubic SimonBrot
	    Dnr= _3* BigDiffAbs(x2*y,x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b)*(y2*y) - BigDiffAbs(y2*y,_3*y2*b+_3*y*b2+b2*b)*(y2*y) + _9* BigDiffAbs(x*y2,_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2)*(x*y2) - _3* BigDiffAbs(x2*x,_3*x2*a+_3*x*a2+a2*a)*(x*y2) + _3* BigDiffAbs(y2*y,_3*y2*b+_3*y*b2+b2*b)*(x2*y)
						- _9* BigDiffAbs(x2*y,x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b)*(x2*y) - _3* BigDiffAbs(x*y2,_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2)*(x2*x) + BigDiffAbs(x2*x,_3*x2*a+_3*x*a2+a2*a)*(x2*x)
						+ _3*_abs(x2*y+x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b)*(_3*y2*b+_3*y*b2+b2*b) - _abs(y2*y+_3*y2*b+_3*y*b2+b2*b)*(_3*y2*b+_3*y*b2+b2*b) + _9*_abs(x*y2+_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2)*(_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2) 
						- _3*_abs(x2*x+_3*x2*a+_3*x*a2+a2*a)*(_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2) + _3*_abs(y2*y+_3*y2*b+_3*y*b2+b2*b)*(x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b) 
						- _9*_abs(x2*y+x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b)*(x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b) - _3*_abs(x*y2+_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2)*(_3*x2*a+_3*x*a2+a2*a) + _abs(x2*x+_3*x2*a+_3*x*a2+a2*a)*(_3*x2*a+_3*x*a2+a2*a) + a0;
	    Dni= _3* BigDiffAbs(x*y2,_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2)*(y2*y) - BigDiffAbs(x2*x,_3*x2*a+_3*x*a2+a2*a)*(y2*y) - _9* BigDiffAbs(x2*y,x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b)*(x*y2) - _9* BigDiffAbs(x*y2,_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2)*(x2*y)
						+ _3* BigDiffAbs(x2*x,_3*x2*a+_3*x*a2+a2*a)*(x2*y) + _3* BigDiffAbs(y2*y,_3*y2*b+_3*y*b2+b2*b)*(x*y2) - BigDiffAbs(y2*y,_3*y2*b+_3*y*b2+b2*b)*(x2*x) + _3* BigDiffAbs(x2*y,x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b)*(x2*x)
						+ _3*_abs(x*y2+_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2)*(_3*y2*b+_3*y*b2+b2*b) - _abs(x2*x+_3*x2*a+_3*x*a2+a2*a)*(_3*y2*b+_3*y*b2+b2*b) - _9*_abs(x2*y+x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b)*(_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2) 
						- _9*_abs(x*y2+_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2)*(x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b) + _3*_abs(x2*x+_3*x2*a+_3*x*a2+a2*a)*(x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b) + _3*_abs(y2*y+_3*y2*b+_3*y*b2+b2*b)*(_2*x*y*b+x*b2+a*y2+_2*a*y*b+a*b2) 
						- _abs(y2*y+_3*y2*b+_3*y*b2+b2*b)*(_3*x2*a+_3*x*a2+a2*a) + _3*_abs(x2*y+x2*b+_2*x*a*y+_2*x*a*b+a2*y+a2*b)*(_3*x2*a+_3*x*a2+a2*a)	+ b0;

	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;

	case 52:							// SimonBrot2 4th
	    DeltaSubN->x = (x2 - y2)*BigDiffAbs(x2 - y2, _2 * x*a + a2 - _2 * y*b - b2) + (_2 * x*a + a2 - _2 * y*b - b2)*_abs(x2 + _2 * x*a + a2 - y2 - _2 * y*b - b2)
		- (_2 * x*y)*BigDiffAbs(_2 * x*y, _2 * x*b + _2 * a*y + _2 * a*b) - (_2 * x*b + _2 * a*y + _2 * a*b)*_abs(_2 * x*y + _2 * x*b + _2 * a*y + _2 * a*b) + a0;
	    DeltaSubN->y = (x2 - y2)*BigDiffAbs(_2 * x*y, _2 * x*b + _2 * a*y + _2 * a*b) + (_2 * x*a + a2 - _2 * y*b - b2)*_abs(_2 * x*y + _2 * x*b + _2 * a*y + _2 * a*b)
		+ (_2 * x*y)*BigDiffAbs(x2 - y2, _2 * x*a + a2 - _2 * y*b - b2) + (_2 * x*b + _2 * a*y + _2 * a*b)*_abs(x2 - y2 + _2 * x*a + a2 - _2 * y*b - b2) + b0;
	    break;
	case 53:							// TheRedshiftRider: a*z^2+/-z^n+c
	    {
	    ExpComplex	Zp(1.0, 0.0);
	    ExpComplex	sum(0.0, 0.0);
	    ExpComplex	ExprsrA;

	    ExprsrA.x = rsrA.x;
	    ExprsrA.y = rsrA.y;
	    Dn = *DeltaSubN;
	    z = {x, y};
	    Multiplier = (rsrSign) ? 1.0 : -1.0;
	    for (int i = 0; i < power; i++)
		{
		sum += Zp * (double)PascalArray[i];
		sum *= *DeltaSubN;
		Zp *= *XRef;
		}
	    *DeltaSubN = sum * Multiplier + (ExprsrA * (z*Dn * 2 + Dn * Dn));
	    *DeltaSubN += *DeltaSub0;
	    }
	    break;
//	case 54:							// HPDZ Buffalo
//	    Dnr = a*(x+x+a)-b*(b+y*2) - BigDiffAbs(x,a) + a0;
//	    Dni = BigDiffAbs(x*y,x*b+a*y+a*b) * 2.0 - BigDiffAbs(y,b) + b0;
//
//	    DeltaSubN->y = Dni;
//	    DeltaSubN->x = Dnr;
	case 54:							// Talis: Zn+1 = ((Zn ^ 2) / (1 + Zn)) + C;
	    {
	    ExpComplex a, b;
	    ExpComplex Z = *XRef;
	    ExpComplex Z2 = Z.CSqr();
	    ExpComplex z = *DeltaSubN;
	    ExpComplex c = *DeltaSub0;
	    *DeltaSubN = (((((z*(Z + z)) + (Z*z)) / (((Z + z) + 1))) - (Z2 * z / (((Z + 1)*((Z + z) + 1))))) + c);
	    //	    *DeltaSubN = (((((z*(Z + z)) + (Z*z)) * 1 / ((1 + (Z + z)))) + ((Z*Z)*(-((0 + z)) * 1 / (((1 + Z)*(1 + (Z + z))))))) + c);
	    }
	    break;
	case 55:							// Talis Cubic: Zn+1 = ((Zn ^ 3) / (1 + Zn^2)) + C;
	    {
	    ExpComplex a, b;
	    ExpComplex X = *XRef;
	    ExpComplex X2 = X.CSqr();
	    ExpComplex X3 = X.CCube();
	    ExpComplex x = *DeltaSubN;
	    ExpComplex c = *DeltaSub0;
	    *DeltaSubN = (((((((x*(X + x)) + (X*x))*(X + x)) + (X2*x)) / ((_1 + ((X + x)*(X + x))))) + (X3*(-((((x*(X + x)) + (X*x)))) / (((_1 + X2)*(_1 + ((X + x)*(X + x)))))))) + c);
	    //	*DeltaSubN = (((((((x*(X + x)) + (X*x))*(X + x)) + ((X*X)*x)) * 1 / ((1 + ((X + x)*(X + x))))) + (((X*X)*X)*(-((0 + ((x*(X + x)) + (X*x)))) * 1 / (((1 + (X*X))*(1 + ((X + x)*(X + x)))))))) + c);
	    }
	    break;
	case 56:							// Talis Quartic: Zn+1 = ((Zn ^ 3) / (1 + Zn^2)) + C;
	    {
	    ExpComplex a, b;
	    ExpComplex X = *XRef;
	    ExpComplex X2 = X.CSqr();
	    ExpComplex X3 = X.CCube();
	    ExpComplex X4 = X2.CSqr();
	    ExpComplex x = *DeltaSubN;
	    ExpComplex c = *DeltaSub0;
	    *DeltaSubN = (((((((x*(X + x)) + (X*x))*((X + x)*(X + x))) + ((X*X)*((x*(X + x)) + (X*x)))) * 1 / ((_1 + (((X + x)*(X + x))*(X + x))))) + ((X4)*(-((((((x*(X + x)) + (X*x))*(X + x)) + (X2*x)))) / (((_1 + X3)*(_1 + (((X + x)*(X + x))*(X + x)))))))) + c);
	    //	*DeltaSubN = (((((((x*(X + x)) + (X*x))*((X + x)*(X + x))) + ((X*X)*((x*(X + x)) + (X*x)))) * 1 / ((1 + (((X + x)*(X + x))*(X + x))))) + (((X*X)*(X*X))*(-((0 + ((((x*(X + x)) + (X*x))*(X + x)) + ((X*X)*x)))) * 1 / (((1 + ((X*X)*X))*(1 + (((X + x)*(X + x))*(X + x)))))))) + c);
	    }
	    break;
	case 57:								// Polynomial
	    {
	    ExpComplex	FinalZ = { 0.0, 0.0 };
	    ExpComplex	InitialZ = *DeltaSubN;

	    for (int m = 0; m < MAXPOLY; m++)
		{
		ExpComplex Zp(1.0, 0.0);
		ExpComplex sum(0.0, 0.0);
		LoadPascal(PascalArray, MAXPOLY - m);				// calculate the pascal's triangle coefficients
		if (param[m] != 0.0)
		    {
		    for (int i = 0; i < MAXPOLY - m; i++)
			{
			sum += Zp * (double)PascalArray[i];
			sum *= InitialZ;
			Zp *= *XRef;
			}
		    FinalZ += (sum * param[m]);
		    }
		}
	    *DeltaSubN = FinalZ + *DeltaSub0;
	    }
	break;
/*
	case 57:							// Cubic Newton: Zn+1 = Zn - ((Zn^3 - 1.0)/(3*Zn^2)) + c
	{
	ExpComplex a, b;
	ExpComplex Z = *XRef;
	ExpComplex Z2 = Z.CSqr();
	ExpComplex Z3 = Z.CCube();
	ExpComplex z = *DeltaSubN;
	//	    Complex z2 = z.CSqr();
	//	    Complex z3 = z.CCube();
	//	    Complex z4 = z2.CSqr();
	ExpComplex c = *DeltaSub0;
	*DeltaSubN = ((z - ((((((((z*(Z + z)) + (Z*z))*(Z + z)) + (Z2*z))) / ((((Z + z)*(Z + z) * 3)))) + ((Z3 - 1)*(-(((((z*(Z + z)) + (Z*z)) * 3))) * 1 / (((Z2 * 3)*(((Z + z)*(Z + z) * 3))))))))) + c);
	//	    *DeltaSubN = ((z - ((((((((z*(Z + z)) + (Z*z))*(Z + z)) + ((Z*Z)*z)) - (0)) * 1 / ((3 * ((Z + z)*(Z + z))))) + ((((Z*Z)*Z) - (1))*(-(((0 * ((Z + z)*(Z + z))) + (3 * ((z*(Z + z)) + (Z*z))))) * 1 / (((3 * (Z*Z))*(3 * ((Z + z)*(Z + z)))))))))) + c);
	}
	break;
*/
	default:
	    Dnr = (r * 2 + a) * a - (i * 2 + b) * b + a0;
	    Dni = ((r + a) * b + i * a) * 2 + b0;
	    DeltaSubN->y = Dni;
	    DeltaSubN->x = Dnr;
	    break;
	}
    }

//////////////////////////////////////////////////////////////////////
// Laser Blaster's Code for removing absolutes from Mandelbrot derivatives
//////////////////////////////////////////////////////////////////////


floatexp	calculateFrame::BigDiffAbs(floatexp c, floatexp d)
    {
    floatexp cd = c + d;

    if (c >= 0.0)
	{
	if (cd >= 0.0)
	    return d;
	else
	    return -d - c.X2();
	}
    else
	{
	if (cd > 0.0)
	    return d + c.X2();
	else
	    return -d;
	}
    }

//////////////////////////////////////////////////////////////////////
//    Convert Bignum variable to FloatExp
//////////////////////////////////////////////////////////////////////

int calculateFrame::Bignum2FloatExp(floatexp *a, BigDouble b)
    {
    double	value;
    long	exp;
    BigDouble	t;

    exp = mpfr_get_exp(b.x);
    mpfr_set(t.x, b.x, MPFR_RNDN);
    mpfr_set_exp(t.x, (mpfr_exp_t) 0);
    value = mpfr_get_d(t.x, MPFR_RNDN);
    *a = value;
    a->exp += exp;
    return 0;
    }

//////////////////////////////////////////////////////////////////////
//    Convert FloatExp variable 20 Bignum
//////////////////////////////////////////////////////////////////////

int calculateFrame::FloatExp2Bignum(BigDouble *a, floatexp b)
    {
    double	value;
    __int64	exp;
    mpfr_exp_t	exp1;
    floatexp	t;

    exp = b.exp;
    t = b;
    t.exp = 0;
    value = t.val;
    *a = value;
    exp1 = mpfr_get_exp(a->x);
    exp1 += (mpfr_exp_t)exp;
    mpfr_set_exp(a->x, exp1);
    return 0;
    }



//////////////////////////////////////////////////////////////////////
//    Convert BigComplex variable to ExpComplex
//////////////////////////////////////////////////////////////////////

int calculateFrame::BigComplex2ExpComplex(ExpComplex *a, BigComplex b)
    {
    Bignum2FloatExp(&(a->x), b.x);
    Bignum2FloatExp(&(a->y), b.y);
    return 0;
    }

//////////////////////////////////////////////////////////////////////
//    Convert ExpComplex variable to BigComplex
//////////////////////////////////////////////////////////////////////

int calculateFrame::ExpComplex2BigComplex(BigComplex *a, ExpComplex b)
    {
    FloatExp2Bignum(&(a->x), b.x);
    FloatExp2Bignum(&(a->y), b.y);
    return 0;
    }

/********************************************************************/
/* Log(r)                                                            */
/********************************************************************/

/*
floatexp	calculateFrame::Log(floatexp n)
    {
    int		i, ln_done = 0;
    double	f;
    floatexp	bftmp1, bftmp2, bftmp3, bftmp4, bftmp5, bftmp6, zero = 0.0, OldR, r;

    floatexp   delta = 10e-10;

    // use Newton's recursive method for zeroing in on ln(n): r=r+n*exp(-r)-1
    if (n < zero || n == zero)
	{				// error, return largest neg value
	r = -10e+300;
	return r;
	}

    f = n.todouble();
    f = logl(f);			// approximate ln(x)
    // no need to check overflow
    // appears to be ok, do ln

    // With Newton's Method, there is no need to calculate all the digits every time.  The precision approximately doubles each iteration.

    r = f;		// start with approximate ln and get negative: -r
    r = -r;
    bftmp5 = r;		// -r
    for (i = 0; i < 20; i++)	// safety net, this shouldn't ever be needed
	{
	bftmp6 = Exp(r);				// exp(-r)
	bftmp2 = bftmp6 * n;				// n*exp(-r)
	bftmp2 = bftmp2 - 1.0;				// n*exp(-r) - 1
	r = r - bftmp2;				// -r - n*exp(-r) - 1
	if (!ln_done)
	    {
	    ln_done = ((r - bftmp5).abs() < delta);	// too small to register
	    bftmp5 = r;				// -r
	    }
	if (ln_done)
	    break;
	}

    r = -r;				// -(-r)
    return r;
    }
*/

/********************************************************************/
/* exp_bf(r)                                                   */
/********************************************************************/

/*
floatexp	calculateFrame::Exp(floatexp n)
    {
    U16		fact = 1;
    int		i, exp_done = 0;
    bool	positive = true;
    floatexp	ExpOut,	bftmp1, bftmp2, zero = 0.0, one = 1.0;
    floatexp	delta = 10e-25;

    if (n < zero)
	{
	n = -n;
	positive = false;
	}

    // use Taylor Series (very slow convergence) 
    ExpOut = 1.0;			// start with c=1
    bftmp1 = 1.0;		// the current x^n/n! 
    for (i = 0; i < 40; i++)	// just for safety
	{
	bftmp2 = bftmp1;
	bftmp1 = bftmp2 * n;
	bftmp1 = bftmp1 / fact++;
	if (!exp_done)
	    {
	    exp_done = (bftmp1.abs() < delta);	// too small to register
	    if (!exp_done)
		ExpOut = ExpOut + bftmp1;
	    }
	if (exp_done)
	    break;
	}
    if (!positive)
	ExpOut = one / ExpOut;
    return  ExpOut;
    }
*/

