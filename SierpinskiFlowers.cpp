/*
   SIERPINSHIFLOWERS.CPP a module for Sierpinski Flower Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "OtherFunctions.h"

#define MIN(a,b)    (a <= b ? a : b)
#define MAX(a,b)    (a >= b ? a : b)
#define MAXPOINTS   16
#define	MAXARRAY    64

/**************************************************************************
	Sierpinski Flower Fractal Type Images
***************************************************************************/

int	COtherFunctions::DoSierpinskiFlower()

    {
    int	    cor, k, l, m, n, n1;
    double  d, g, k3; 
    double  x, y;
    int	    x1, y1, k1, k2;
    int	    exp1;
    double  exp;
    DWORD   colour;
    BOOL    flag = FALSE;				// break early

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    Passes = (int)param[0];
    Sides = (int)param[1];
//    Animate = (param[2] != 0.0);
    ExpandPalette = (param[2] != 0.0);

//    if (Animate)
//	return DoAnimatedSierpinskiFlower();
    *totpasses = Passes;
    d = 0.3105 * Sides + 1.0815;			// equation for defining the distance between every polygon. It will be the same in C++
    k3 = (3.5 - 2.7) / (20 - 5) * (Sides - 5) + 2.7;	// this equation is for defining the adequate size of the figure in my BASIC
    k2 = 360360;					// this constant is justifyied to give integers in the division below. = 2*2*2*3*3*5*7*11*13
    k1 = k2 / Sides * 2;				// this constant is a pair with the the above one to give the exact polygon
    g = k1 * PI / k2;
    if (Sides < 8)
	cor = Passes;
    else
	cor = Passes - (int)(log((double)Sides));		// allow early escape if already generated adequate resolution
    for (m = 0; m < Passes; m++)
	{
	if (flag)
	    break;
	*curpass = m;
	colour = (ExpandPalette) ? threshold * (DWORD)m / Passes : (DWORD)m;
	exp1 = IntPower(Sides, m);
	for (n = 0; n < exp1; n++)
	    {
	    if (UserData(hwnd) == -1)		// user pressed a key?
		 return -1;
	    n1 = n;
	    x = 0;
	    y = 0;
	    for (k = 0; k < m; k++)
		{
		l = n1 % Sides;
		n1 = n1 / Sides;					// this is the minimum necessary associate with the number of sides of the polygon
		exp = FloatPower(d, k);
		x = x + k3 * (sin(g * l)) / exp;
		y = y + k3 * (cos(g * l)) / exp;
		}
	    x1 = (int)((x - hor) * xscale);
	    y1 = (int)((vert + mandel_width - y) * yscale);
	    Plot.PlotPoint(x1, y1, colour);
	    if (m >= cor)
		flag = TRUE;
	    }
	}
    return 0;
    }

