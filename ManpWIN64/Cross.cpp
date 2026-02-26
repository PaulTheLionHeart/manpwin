/*
   CROSS.CPP a module for Cross Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "OtherFunctions.h"

/**************************************************************************
	Cross Roads Fractal Type Images
***************************************************************************/

int	COtherFunctions::DoCrossRoads()

    {
    int		j, k;
    double	x, y, xx, xold, yy, temp, exponent; 
    long	i, num;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    BYTE    subtype = 0;
//    x = 0.365;
//    y = -0.129;
    x = 1;
    y = 1;

    a = param[0];
    b = param[1];
    C = param[2];
    exponent = param[3];
    subtype = (int)param[4];
    num = (int)param[5];
    *totpasses = 10;
    //DisplayFractal(hwnd);

    for(i = 0; i < num; i++)
	{
	if (UserData(hwnd) == -1)		// user pressed a key?
	    return -1;
	*curpass = i * 10 / num;
	switch (subtype)
	    {
	    case 0:				// cross roads
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * x + C);
		else
		    temp = pow(fabs(b * x + C), exponent);
		xx = y - (x / fabs(x)) / temp;
		yy = a - x;
		break;
	    case 1:				// Almost Lyapunov
		xold = x;
		yy = y + x;
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * xold + C);
		else
		    temp = pow(fabs(b * xold + C), exponent);
		xx = yy - (xold / fabs(xold)) / temp;
		yy = a - xold;
		break;
	    case 2:				// Clockwork
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(y)) / temp;
		yy = a + C - x;
		break;
	    case 3:				// Convergence of henons
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(y)) / temp;
		yy = a + C - x - y;
		break;
	    case 4:				// Leaking
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * x + C);
		else
		    temp = pow(fabs(b * x + C), exponent);
		xx = y - (x + y / fabs(x)) / temp;
		yy = a - x;
		break;
	    case 5:				// Lord of the Rings
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(y)) / temp;
		yy = a + C - x - y;
		break;
	    case 6:				// Poltergiest
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + C);
		else
		    temp = pow(fabs(b * y + C), exponent);
		xx = y - (x + y / fabs(x)) / temp;
		yy = a - x;
		break;
	    case 7:				// Resonance
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(y)) / temp;
		yy = a + C - x;
		break;
	    case 8:				// Pit
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(x)) / temp;
		yy = a - x;
		break;
	    case 9:				// Conflict
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y) + x;
		else
		    temp = pow((fabs(b * y) + x), exponent);
		xx = y - (x - y / fabs(y)) / temp;
		yy = a + C - x;
		break;
	    case 10:				// Confusion
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * y + x);
		else
		    temp = pow(fabs(b * y + x), exponent);
		xx = y - (x + y / fabs(x)) / temp;
		yy = a + C - x;
		break;


	    default:				// cross roads
		if (exponent <= 0.0 || exponent == 1.0)
		    temp = fabs(b * x + C);
		else
		    temp = pow(fabs(b * x + C), exponent);
		xx = y - (x / fabs(x)) / temp;
		yy = a - x;
		subtype = 0;
		break;
	    }
	x = xx / Div[subtype];
	y = yy;
	OscProcess.ChangeCoordSystem(&x, &y, &temp, x, y, fabs(atan2(y, x)*180.0/PI), CoordSystem);
	if (subtype == 9)			// Conflict
	    {
	    j = (int)((x - hor) * xscale * 1000) % xdots;
	    k = (int)((vert + mandel_width - y) * yscale * 1000) % ydots;
	    }
	else
	    {
	    j = (int)((x - hor) * xscale);
	    k = (int)((vert + mandel_width - y) * yscale);
	    }
	Plot.PlotPoint(j, k, (long)fabs(atan2(y, x)*180.0/PI));
	}
    return 0;
    }

