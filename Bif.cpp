/*
    BIF.CPP a module for Bifurcation Images
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	"bif.h"
#include	"manp.h"
#include	"complex.h"
#include	"fractalp.h"
#include	"fractype.h"
#include	"pixel.h"
#include	".\parser\TrigFn.h"	

/***********************************************************************************
	The following are Bifurcation "orbitcalc" routines without trig functions...
	Those with trig functions are in fractals.cpp
***********************************************************************************/

int	CPixel::BifurcLambda(double Rate, double *Population) // Used by lyanupov
    {
    *Population = Rate * *Population * (1 - *Population);
    return (fabs(*Population) > BIG);
    }

int	CPixel::BifQuadMandel(double Rate, double *Population)
    {
    *Population = Rate + *Population * *Population;
    return (fabs(*Population) > BIG1);
    }

int	CPixel::BifurcMay(double Rate, double *Population, long beta)
    {	// X = (lambda * X) / (1 + X)^beta, from R.May as described in Pickover,
	//        Computers, Pattern, Chaos, and Beauty, page 153
    temp.x = 1.0 + *Population;
    temp.x = pow(temp.x, -beta); // pow in math.h included with mpmath.h
    *Population = (Rate * *Population) * temp.x;
    return (fabs(*Population) > BIG);
    }

/***********************************************************************************
	Bifurcation trig stuff...
***********************************************************************************/

int	CPixel::BifurcVerhulstTrig(double Rate, double *Population, CTrigFn *TrigFn)
    {
    //  Population = Pop + Rate * fn(Pop) * (1 - fn(Pop))
    temp.x = *Population;
    temp.y = 0;
    TrigFn->CMPLXtrig(&temp, &temp, Fn1Index);
    *Population += Rate * temp.x * (1 - temp.x);
    return (fabs(*Population) > BIG);
    }
int	CPixel::BifurcLambdaTrig(double Rate, double *Population, CTrigFn *TrigFn)
    {
    //  Population = Rate * fn(Population) * (1 - fn(Population))
    temp.x = *Population;
    temp.y = 0;
    TrigFn->CMPLXtrig(&temp, &temp, Fn1Index);
    *Population = Rate * temp.x * (1 - temp.x);
    return (fabs(*Population) > BIG);
    }
int	CPixel::BifurcAddTrigPi(double Rate, double *Population, CTrigFn *TrigFn)
    {
    temp.x = *Population * PI;
    temp.y = 0;
    TrigFn->CMPLXtrig(&temp, &temp, Fn1Index);
    *Population += Rate * temp.x;
    return (fabs(*Population) > BIG);
    }
int	CPixel::BifurcSetTrigPi(double Rate, double *Population, CTrigFn *TrigFn)
    {
    temp.x = *Population * PI;
    temp.y = 0;
    TrigFn->CMPLXtrig(&temp, &temp, Fn1Index);
    *Population = Rate * temp.x;
    return (fabs(*Population) > BIG);
    }
int	CPixel::BifurcStewartTrig(double Rate, double *Population, CTrigFn *TrigFn)
    {
    //  Population = (Rate * fn(Population) * fn(Population)) - 1.0
    temp.x = *Population;
    temp.y = 0;
    TrigFn->CMPLXtrig(&temp, &temp, Fn1Index);
    *Population = (Rate * temp.x * temp.x) - 1.0;
    return (fabs(*Population) > BIG);
    }

/***********************************************************************************
	Bifurcation calling routine...
***********************************************************************************/

int	CPixel::bifurcation(int user_data(HWND hwnd))

    {
    double	iz;
    int		j, result;
    int		cycles;				// Bif types, num cycles before plot
    static	Complex	tmp;
    double	cpuz, Population, Rate;
    long	beta;
    int		col;
    Complex	c;
    CTrigFn	TrigFn;

    *PlotType = NOSYM;
    result = FALSE;
    cpuz = 1.0 / *ygap;
//    calcmode = '1';				// mainly for display

    cycles = (int)param[0];
    beta = (long)param[2];
    for (col = 0; col < (int)xdots; ++col)
	{
	c.x = col * *xgap + hor;
	Population = param[1];
	Rate = c.x;
	for (j = 0; j < cycles + threshold; ++j)
	    {
	    result = FALSE;
	    if (user_data(hwnd) == -1)		/* user pressed a key? */
		return -1;
	    switch (type)
		{
		case LBIFURCATION:
		case BIFURCATION:
		    result = BifurcVerhulstTrig(Rate, &Population, &TrigFn);
		    break;
		case LBIFLAMBDA:
		case BIFLAMBDA:
		    result = BifurcLambdaTrig(Rate, &Population, &TrigFn);
		    break;
		case LBIFADSINPI:
		case BIFADSINPI:
		    result = BifurcAddTrigPi(Rate, &Population, &TrigFn);
		    break;
		case LBIFEQSINPI:
		case BIFEQSINPI:
		    result = BifurcSetTrigPi(Rate, &Population, &TrigFn);
		    break;
		case LBIFSTEWART:
		case BIFSTEWART:
		    result = BifurcStewartTrig(Rate, &Population, &TrigFn);
		    break;
		case QUADMAND:
		    result = BifQuadMandel(Rate, &Population);
		    break;
		case LBIFMAY:
		case BIFMAY:
		    result = BifurcMay(Rate, &Population, beta);
		    break;
		default:
		    type = BIFURCATION;
		    result = BifurcVerhulstTrig(Rate, &Population, &TrigFn);
		    break;
		}
	    if (result)
		break;
	    if (j >= cycles)
		{
		iz = (Population - vert) * cpuz;
		if (iz > 0 && iz <= ydots)
//		    PlotPixel((WORD)col, (WORD)(ydots - (int)iz), (DWORD)(j - cycles));
		    PlotPixel((WORD)col, (WORD)(ydots - (int)iz), (DWORD)(col % colours));
		}
	    }
	}
    return 0;
    }

