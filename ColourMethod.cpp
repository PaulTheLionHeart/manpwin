/////////////////////////////////////////////////
// rsxcolor.c
/////////////////////////////////////////////////
//
// By: stephen c. ferguson
// http://home1.gte.net/itriazon/
// email: itriazon@gte.net
//
/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include "Complex.h"
#include "filter.h"
#include "manpwin.h"
#include "manp.h"
#include "colour.h"

///////////////////////////////////////////////

double CTZfilter::x_y_to_degrees(double x_, double y_)
    {
    double a;

    if (x_ >= 0 && y_ >= 0)
	a = atan(y_/x_);			// Q0
    else
	if (x_ < 0 && y_ >= 0)
	    a = atan(y_/x_)+PI;			// Q1
	else
	    if (x_ < 0 && y_ < 0)		
		a = atan(y_/x_)+PI;		// Q3
	    else
		a = atan(y_/x_)+2*PI;		// Q4

    return a*RAD;
    }

//////////////////////////////////////////////////////////

void CTZfilter::ColourMethods(Complex z, Complex q, CTrueCol *TrueCol,int ColourMethod)
    {
    double	temp, xsav_, ysav_, dzx_save, dzy_save, x_size, y_size, x, y, deg;
    xsav = z.x;
    ysav = z.y;
    x = z.x;
    y = z.y;

    nGrn = TrueCol->GreenIncInt;
    nBlu = TrueCol->BlueIncInt;
    nRed = TrueCol->RedIncInt;
    nGrnStart = TrueCol->GreenStartInt;
    nBluStart = TrueCol->BlueStartInt;
    nRedStart = TrueCol->RedStartInt;

    switch(ColourMethod)
	{
	case 0:
	    break;

	case 1:
	    temp = fabs(sin(x * x * 10))*fabs(sin(y * y * 10)) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 2:
	    temp = fabs(sin(xsav * 50))*fabs(sin(ysav * 50)) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += (rj + gj) / 2;
	    break;

	case 3:
	    temp = atan(1 / fabs(xsav / ysav)) - atan(1 / fabs(ysav / xsav)) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 4:
	    xsav_ = x;
	    ysav_ = y;

	    temp = atan(1 / fabs(xsav_ / ysav_)) - atan(1 / fabs(ysav_ / xsav_)) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 5:
	    dzx = fabs(xsav);
	    dzy = fabs(ysav);

	    temp = atan(1 / fabs(dzx / dzy)) + atan(1 / fabs(dzy / dzx)) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 6:
	    dzx = fabs(xsav);
	    dzy = fabs(ysav);

	    temp = atan(fabs(dzx / dzy)) * 10 * dFactor;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 7:
	    dzx = fabs(xsav);
	    dzy = fabs(ysav);
	    dzx_save = fabs(x);
	    dzy_save = fabs(y);

	    if (xsav * ysav > 0)
		temp = sin(fabs(dzx*dzy * 100 + dzx_save * dzy_save * 10)) * 100 * dFactor_2;
	    else
		temp = cos(fabs(dzx*dzy * 100 + dzx_save * dzy_save * 10)) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 8:
	    dzx = fabs(xsav);
	    dzy = fabs(ysav);
	    temp = 0;

	    if (dzx < dStrands || dzy < dStrands)
		{
		if (dzx < dStrands)
		    temp = (1 - dzx / dStrands) * 100 * dFactor_2;

		if (dzy < dStrands)
		    temp += (1 - dzy / dStrands) * 100 * dFactor_2;

		rj += temp;
		gj += temp;
		bj += temp;
		}
	    break;

	case 9:
	    dzx = fabs(xsav);
	    dzy = fabs(ysav);

	    if (dzy > dzx)
		temp = sin(fabs(dzx*dzx * 60)) * 100 * dFactor_2;
	    else
		temp = sin(fabs(dzy*dzy * 60)) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 10:
	    dzx = fabs(xsav);
	    dzy = fabs(ysav);
	    dzx_save = fabs(x);
	    dzy_save = fabs(y);

	    temp = z.CFabs()*cos(fabs(dzx*dzx / dzy * dzy)) * 1000 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 11:
	    dzx = fabs(xsav);
	    dzy = fabs(ysav);
	    dzx_save = fabs(x);
	    dzy_save = fabs(y);

	    temp = sin(fabs(dzx*dzx / dzx_save * dzx_save - dzy * dzy / dzy_save * dzy_save) * 10) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 12:
	    dzx = fabs(xsav);
	    dzy = fabs(ysav);
	    dzx_save = fabs(x);
	    dzy_save = fabs(y);

	    temp = cos(fabs(dzx*dzy / dzx_save * dzy_save) * 10) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 13:
	    temp = (atan(1 / fabs(x / y)) - atan(1 / fabs(y / x))) * 100 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 14:
	    xsav_ = x;
	    ysav_ = y;
	    temp = 0;

	    temp = atan(fabs(xsav_ / ysav_)) * 200 * dFactor_2;

	    rj += temp;
	    gj += temp;
	    bj += temp;
	    break;

	case 15:
	    temp = atan(fabs(x / y)) * atan(fabs(xsav / ysav));

	    rj += (temp * 200 * dFactor_2);
	    gj += (temp * 200 * dFactor_2);
	    bj += (temp * 200 * dFactor_2);
	    break;

	case 16:
	    // magnitude
	    x_size = 4 / (double)threshold;
	    y_size = 4 / (double)threshold;

	    rj += (((fabs(x / x_size) + fabs(y / y_size)))*dFactor_2);
	    gj += (((fabs(x / x_size) + fabs(y / y_size)))*dFactor_2);
	    bj += (((fabs(x / x_size) + fabs(y / y_size)))*dFactor_2);
	    break;

	case 17:
	    q.x = x;
	    q.y = y;

	    // Circles
	    x_size = 4 / (double)threshold;
	    y_size = 4 / (double)threshold;

	    q.x = (fabs(q.x / x_size));
	    q.y = (fabs(q.y / y_size));
	    //	z = c;

	    xsav_ = (fabs(xsav / x_size));
	    ysav_ = (fabs(ysav / y_size));
	    //z = c;

	    rj += ((sqrt(xsav_*xsav_ + ysav_ * ysav_))*dFactor);
	    rj += ((sqrt(q.x*q.x + q.y*q.y))*dFactor_2);

	    gj += ((sqrt(xsav_*xsav_ + ysav_ * ysav_))*dFactor);
	    gj += ((sqrt(q.x*q.x + q.y*q.y))*dFactor_2);

	    bj += ((sqrt(xsav_*xsav_ + ysav_ * ysav_))*dFactor);
	    bj += ((sqrt(q.x*q.x + q.y*q.y))*dFactor_2);
	    break;

	case 18:
	    q.x = x;
	    q.y = y;

	    // Radial
	    deg = x_y_to_degrees(q.x, q.y);

	    temp = ((double)threshold) / 180;

	    rj += (temp * deg * dFactor_2);
	    gj += (temp * deg * dFactor_2);
	    bj += (temp * deg * dFactor_2);
	    break;

	case 19:
	    q.x = x;
	    q.y = y;

	    // magnitude
	    x_size = 4 / (double)threshold;
	    y_size = 4 / (double)threshold;

	    rj += (((fabs(q.x / x_size) + fabs(q.y / y_size))))*dFactor_2;
	    gj += (((fabs(q.x / x_size) + fabs(q.y / y_size))))*dFactor_2;
	    bj += (((fabs(q.x / x_size) + fabs(q.y / y_size))))*dFactor_2;
	    break;

	case 20:
	    rj += 500 * dFactor_2*(xsav*ysav);
	    gj += 500 * dFactor_2*(xsav*xsav);
	    bj += 500 * dFactor_2*(ysav*ysav);
	    break;

	case 21:
	    z.x = xsav;
	    z.y = ysav;

	    q.x = x;
	    q.y = y;

	    gj = (50 * dFactor_2*fabs(xsav*xsav * 2 - q.CSumSqr())*(double)nGrn + (double)nGrnStart + gj * (double)nGrn + (double)nGrnStart);
	    bj = (50 * dFactor_2*fabs(ysav*ysav * 2 - q.CSumSqr())*(double)nBlu + (double)nBluStart + bj * (double)nBlu + (double)nBluStart);
	    rj = (50 * dFactor_2*fabs(z.CSumSqr() - q.CSumSqr())*(double)nRed + (double)nRedStart + rj * (double)nRed + (double)nRedStart);
	    break;

	case 22:
	    q.x = x;
	    q.y = y;

	    rj += 2000 * dFactor_2*fabs(ysav*q.y);
	    gj += 2000 * dFactor_2*fabs(xsav*q.x);
	    bj += 2000 * dFactor_2*fabs(xsav*ysav);
	    break;

	case 23:
	    q.x = x;
	    q.y = y;

	    rj += 200 * dFactor_2*(xsav*ysav - q.CSumSqr());
	    gj += 200 * dFactor_2*(xsav*xsav - q.CSumSqr());
	    bj += 200 * dFactor_2*(ysav*ysav - q.CSumSqr());
	    break;

	case 24:
	    rj += 2000 * dFactor_2*xsav*ysav;
	    gj += 2000 * dFactor_2*xsav*xsav;
	    bj += 2000 * dFactor_2*ysav*ysav;
	    break;

	case 25:
	    z.x = xsav;
	    z.y = ysav;

	    q.x = x;
	    q.y = y;

	    rj += 200 * dFactor_2*fabs(z.CSumSqr() - q.CSumSqr());
	    gj += 200 * dFactor_2*fabs(xsav*xsav * 2 - q.CSumSqr());
	    bj += 200 * dFactor_2*fabs(ysav*ysav * 2 - q.CSumSqr());
	    break;

	case 26:
	    gj += sin(rj) * 10 * dFactor_2;
	    rj += sin(gj) * 10 * dFactor_2;
	    bj += sin(bj) * 10 * dFactor_2;
	    break;

	case 27:
	    gj += sin(rj) * 10 * dFactor_2;
	    rj += sin(gj) * 10 * dFactor_2;
	    bj += sin(bj) * 10 * dFactor_2;
	    break;

	case 28:
	    bj += 2000 * dFactor_2*fabs(xsav*ysav);
	    gj += 2000 * dFactor_2*fabs(xsav*xsav);
	    rj += 2000 * dFactor_2*fabs(ysav*ysav);
	    break;

	case 29:
	    bj += 2000 * dFactor_2*fabs(xsav*ysav);
	    rj += 2000 * dFactor_2*fabs(xsav*xsav);
	    gj += 2000 * dFactor_2*fabs(ysav*ysav);
	    break;

	case 30:
	    rj += 1000 * dFactor_2*fabs(xsav*ysav);
	    gj += 1000 * dFactor_2*fabs(xsav*xsav);
	    bj += 1000 * dFactor_2*fabs(ysav*ysav);
	    break;

	case 31:
	    rj += sin(rj)*dFactor_2 * 10;
	    gj += cos(gj)*dFactor_2 * 10;
	    bj += cos(bj)*dFactor_2 * 10;
	    break;

	case 32:
	    rj += sin(rj*rj)*dFactor_2 * 10;
	    gj += sin(gj*gj)*dFactor_2 * 10;
	    bj += sin(bj*bj)*dFactor_2 * 10;
	    break;

	case 33:
	    z.x = xsav;
	    z.y = ysav;

	    rj += 2000 * dFactor_2*(cos(fabs(xsav*xsav)));
	    gj += 2000 * dFactor_2*(cos(fabs(xsav*ysav)));
	    bj += 2000 * dFactor_2*(cos(z.CSumSqr()));
	    break;

	case 34:
	    z.x = xsav;
	    z.y = ysav;

	    rj += 2000 * dFactor_2*(cos(z.CSumSqr()));
	    gj += 2000 * dFactor_2*(sin(fabs(xsav*ysav)));
	    bj += 2000 * dFactor_2*(cos(fabs(ysav*xsav)));
	    break;

	case 35:
	    rj += 500 * dFactor_2*(fabs(atan(xsav / ysav)));
	    gj += 500 * dFactor_2*(fabs(atan(xsav / ysav)));
	    bj += 500 * dFactor_2*(fabs(atan(ysav / xsav)));
	    break;

	case 36:
	    rj += 500 * dFactor_2*(fabs(xsav*xsav));
	    gj += 500 * dFactor_2*(fabs(xsav*xsav));
	    bj += 500 * dFactor_2*(fabs(ysav*ysav));
	    break;

	default:
	    break;
	}
    }

