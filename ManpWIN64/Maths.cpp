/*
    MATHS.CPP - a module for transcendental function evaluation.
    
    Written in Visual 'C++' by Paul de Leeuw.
*/

#include	<math.h>
#include	"manp.h"
#include	"Complex.h"

/**************************************************************************
	Complex Power Function
**************************************************************************/

static	double	xt, yt, t2;

void	cpower(Complex *result, Complex *base, int exp)
    {
    xt = base->x;   yt = base->y;

    if (exp & 1)
	{
	result->x = xt;
	result->y = yt;
	}
    else
	{
	result->x = 1.0;
	result->y = 0.0;
	}

    exp >>= 1;
    while (exp)
	{
	t2 = (xt + yt) * (xt - yt);
	yt = xt * yt;
	yt = yt + yt;
	xt = t2;

	if (exp & 1)
	    {
	    t2 = xt * result->x - yt * result->y;
	    result->y = result->y * xt + yt * result->x;
	    result->x = t2;
	    }
	exp >>= 1;
	}
    }


