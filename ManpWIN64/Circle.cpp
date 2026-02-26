/*
   CIRCLE.CPP a module for CIRCLE Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "OtherFunctions.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 48

/*---------------------------------------------------------------------
	Generate scaled circle
  -------------------------------------------------------------------*/

void	COtherFunctions::ScaledCircle(double x, double y, double radius, DWORD colour)
    {
    int	    j, k, rad;
    BYTE    rgb[3];

    j = (int)((x - hor) * xscale);
    rad = (int)(radius * xscale);
    k = (int)((vert + mandel_width - y) * yscale);
    switch (FilledCircle)
	{
	case 1: 
	    Plot.DisplayFilledCircle(j, k, rad, colour);
	    break;
	case 2: 
	    Plot.DisplayCircle(j, k, rad, colour);
	    break;
	case 3: 
	    Plot.GetRGB(colour%threshold, rgb);
	    Plot.Display3DCircle(Dib, j, k, rad, rgb);
	    break;
	}
//    if (FillCircle)
//	DisplayFilledCircle(j, k, rad, colour);
//    else
//	DisplayCircle(j, k, rad, colour);
    }

/**************************************************************************
	Ford Froth Circle Images
***************************************************************************/

int	COtherFunctions::DoFordFroth()

    {
    int	    k, h;
    double  x, y;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    if (subtype == 'L')
	{
	x = 0;
	y = 0.5;
	ScaledCircle(x, y +	0.125, y, 1);
	ScaledCircle(x, -y + 0.125, y, 1);
	x = 1;
	ScaledCircle(x, y +	0.125, y, 1);
	ScaledCircle(x, -y + 0.125, y, 1);
	}

    for (k = 1; k < Passes * 12; k++)
	{
	for (h = 1; h < Passes * 12; h++)
	    {
	    x = (double)h / (double)k;
	    y = 1 / (2 * (double)k * (double)k);
	    if (x == 1.0)
		break;
	    ScaledCircle(x, y +	0.125, y, k);
	    ScaledCircle(x, -y + 0.125, y, k);
	    }
	}
    return 0;
    }

/**************************************************************************
	Cantor set curved I.F.S.
	Union of circle curve set with Cantor dust set
***************************************************************************/

int	COtherFunctions::DoCurvedIFS()
    {
    double	x, y, x1, y1, r;
    double	i;
    int		u, v, p; 
    double	iterations = 5000000.0 / mandel_width;

    x = 0.3 ;
    y = 0.1;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    srand((unsigned)time(NULL));
    u = 0; v = 0;							// Initial line draw points
    for (i = 0; i < iterations; i++)
	{
	if (UserData(hwnd) == -1)				// user pressed a key?
	    return -1;
	*curpass = (int)(i * *totpasses / iterations);
	p = ((rand() % 2) == 0 ? 1 : -1);
	r = sqrt(x*x + y*y);
	switch (rand() % 4) 
	    {
	    case 0:
	    case 1:
		x1 = x / r  - 1;
		y1 = p * y / r;
		break;
	    case 2:
		x1 =  x / 3.0;
		y1 =  y / 3.0;
		break;
	    case 3:
		x1 = x / 3.0 + 2.0 / 3.0;
		y1 = y / 3.0;
		break;
	    }
	x = x1;
	y = y1;
	if (i < 100)
	    continue;
	u = (int)((x - hor) * xscale);
	v = (int)((vert + mandel_width - y) * yscale);
	Plot.PlotPoint(u, v, (DWORD)i % threshold);
	}

    return 0;
    }

/*---------------------------------------------------------------------
	Circle Fractal Type Images
  -------------------------------------------------------------------*/

int	COtherFunctions::DoCircles()

    {
    int		gen, bot, top, count, i;
    double	dx, dy, l, frac;
    double	radius, offset, m[10240];
    DWORD	colour;

    if (UseDefaultPalette)
	for (int i = 0; i < 256; i++)
	    TrueCol->PalettePtr[i] = TrueCol->DefaultPalettePtr[i];
    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    if (Passes > 12)
	Passes = 12;

    if (subtype == 'K' || subtype == 'L')
	return (DoFordFroth());				// Ford Froth Circle
    else if (subtype == 'M')
	return DoCurvedIFS();
    offset = xscale - yscale;
    offset = 1;
    radius = 1;
    count = 1;
    frac = 1;
    dx = 1;
    dy = 1;
    m[count] = 1;
    if (subtype != 'J')
	ScaledCircle(m[count] + dx, dy, radius, 1);

    for(gen = 0; gen < Passes; gen++)
	{
	colour = threshold * (DWORD)gen / Passes;
	bot = IntPower(2, gen);
	top = IntPower(2, gen+1);
	radius /= 2;
	l = radius;
	for(i = bot; i < top; i++)
	    {
	    switch (subtype)
		{
		case 'A':
		case 'B':
		    m[count+1] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1);
		    if (subtype == 'B')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 1) % 14 + 1);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+2] + dx, dy, radius, (count + 2) % 14 + 1);
		    if (subtype == 'B')
			ScaledCircle(dy + offset, m[count+2] + dx - offset, radius, (count + 2) % 14 + 1);
		    count += 2;
		    break;
		case 'C':
		case 'D':
		    m[count+1] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 12);
		    if (subtype == 'D')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 1) % 14 + 1);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1);
		    if (subtype == 'D')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 2) % 14 + 1);
		    count += 2;
		    break;
		case 'E':
		case 'F':
		    m[count+1] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1);
		    if (subtype == 'F')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 1) % 14 + 1);
		    m[count+2] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1);
		    if (subtype == 'F')
			ScaledCircle(dy + offset, m[count+1] + dx - offset, radius, (count + 2) % 14 + 1);
		    count += 2;
		    break;
		case 'G':
		    m[count+1] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1);
		    count += 2;
		    break;
		case 'H':
		    m[count+1] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1);
		    m[count+1] = m[i] - frac * l;
		    m[count+2] = m[i] - frac * l;
		    count += 2;
		    break;
		case 'I':
		    m[count+1] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1);
		    m[count+1] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1);
		    m[count+2] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1);
		    count += 2;
		    break;
		case 'J':
		    m[count+1] = m[i] - frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 1) % 14 + 1);
		    ScaledCircle(-m[count+1] + dx - (yscale - 10)/2, dy, radius, (count + 1) % 14 + 1);
		    m[count+2] = m[i] + frac * l;
		    ScaledCircle(m[count+1] + dx, dy, radius, (count + 2) % 14 + 1);
		    ScaledCircle(-m[count+1] + dx - (yscale - 10)/2, dy, radius, (count + 2) % 14 + 1);
		    count += 2;
		    break;
		}
	    }
	}
    return 0;
    }

/**************************************************************************
    Get parameters for Circles
**************************************************************************/

void	COtherFunctions::GetUseDefaultPalette(BOOL in)	// standard EGA palette
    {
    UseDefaultPalette = in;
    }
void	COtherFunctions::GetFilledCircle(int in)		// 1 = filled, 2 = unfilled, 3 = 3D
    {
    FilledCircle = in;
    }

