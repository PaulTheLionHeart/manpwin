/*
   ZIGZAG.CPP a module for Spiral Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "OtherFunctions.h"
#include "SafeStrings.h"

int	COtherFunctions::DoZigzag()

    {
    double  x, y, a, k1, xold, yold;
    int	    e1, e2;
    DWORD   colour;
    int	    i, j, k, j0, k0, n;
    double  exp;
    char    c[120];
    BYTE    subtype = 0;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    a = param[0];
    k1 = param[1];
    e1 = (int)param[2];
    e2 = (int)param[3];
    subtype = (int)param[4];
    n = (int)param[5];
    if (k1 == 0.0)
	k1 = 1E-30;
    *totpasses = 10;
    x = 0.1;
    y = 0.1;

    for (i = 0; i < n; i++)
	{
	if (UserData(hwnd) == -1)		// user pressed a key?
	    return -1;
	xold = x;
	yold = y;
	*curpass = i * 10 / 600;

	SAFE_SPRINTF(c, "i = %d", i);
	SetWindowText (hwnd, c);

	switch (subtype)
	    {
	    case 0:
		exp = FloatPower(xold, e1);
		x = xold * cos(a / k1) - (y + exp) * sin(a);
		exp = FloatPower(xold, e2);
		y = xold * sin(a) - (x + exp) * cos(a);

		break;
	    case 1:
		exp = FloatPower(xold, e1);
		x = xold * cos(a / k1) - (y + exp) * sin(a);
		exp = FloatPower(xold, e2);
		y = xold * sin(a) + (x + exp) * cos(a);
		break;

	    case 2:
		exp = FloatPower(xold, e1);
		x = xold * cos(a / k1) - (y - exp) * sin(a);
		exp = FloatPower(xold, e2);
		y = xold * sin(a) + (x + exp) * cos(a);
		break;

	    case 3:
		exp = FloatPower(xold, e1);
		x = xold * cos(a / k1) - (y + exp) * sin(a);
		exp = FloatPower(xold, e2);
		y = xold * sin(a) - (y + exp) * cos(a);
		break;

	    default:
		subtype = 0;
		exp = FloatPower(xold, e1);
		x = xold * cos(a / k1) - (y + exp) * sin(a);
		exp = FloatPower(xold, e2);
		y = xold * sin(a) - (x + exp) * cos(a);
		break;
	    }

	j0 = (int)((xold - hor) * xscale);
	k0 = (int)((vert + mandel_width - yold) * yscale);

	j = (int)((x - hor) * xscale);
	k = (int)((vert + mandel_width - y) * yscale);

	// okay, let's do some boundary tests. 
	if ((j < 0 || j >= xdots) && (k < 0 || k >= ydots))
	    continue;
	colour = (i * 5) % threshold;

	Plot.genline(j0, k0, j, k, colour);

	Plot.DisplayFilledCircle ((j0 + j) / 2, (k0 + k) / 2, 3, colour);
	}
    return 0;
    }

