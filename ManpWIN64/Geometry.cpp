/*
   GEOMETRY.CPP a module for Geometric Shapes Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "OtherFunctions.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 16

/**************************************************************************
	Geometric Shapes Fractal Type Images
***************************************************************************/

int	COtherFunctions::DoGeometry(void)

    {
    int	    i, j, k;
    double  phase, size, x, y, t;
    double  u[MAXPOINTS], v[MAXPOINTS];
    int	    a[MAXPOINTS], b[MAXPOINTS];
    int	    h = 8;
    int	    offset;
    DWORD   colour;

    srand((unsigned)time(NULL));			// randomize things
    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;
    for (i = 0; i < count; i++)
	{
	colour = (count < threshold) ? threshold * (DWORD)i / count : (DWORD)(i % threshold);
	size = h * (1 - sqrt((double)rand() / 32767.0)) / 50;
	x = rand() / 32767.0;
	y = rand() / 32767.0;
	phase= rand() * PI / 32767.0;
	switch (subtype)
	    {
	    case 'A':					
		if (rand() > 16383) 
		    {
		    u[0] = x + size;
		    v[0] = y;
		    u[1] = x;
		    v[1] = y;
		    }
		else
		    {
		    u[0] = x;
		    v[0] = y - size;
		    u[1] = x;
		    v[1] = y;
		    }
		a[0] = (int)((u[0] - hor) * xscale);
		a[1] = (int)((u[1] - hor) * xscale);
		b[0] = (int)((vert + mandel_width - v[0]) * yscale);
		b[1] = (int)((vert + mandel_width - v[1]) * yscale);
		Plot.genline(a[0], b[0], a[1], b[1], colour);
		break;
	    case 'B':					
	    case 'C':					
	    case 'D':					
		t = 2 * PI / Sides;
		if (subtype == 'B' || subtype == 'C')
		    offset = 1;
		else
		    offset = (Sides % 2) ? Sides / 2 : Sides / 2 - 1;
		for (j = 0; j < Sides; j++)
		    {
		    u[j] = x + size * cos(j * t + phase);
		    v[j] = y + size * sin(j * t + phase);
		    a[j] = (int)((u[j] - hor) * xscale);
		    b[j] = (int)((vert + mandel_width - v[j]) * yscale);
		    }
		if (subtype == 'C')
		    {
		    for (j = 0; j < Sides; j++)
			polygon.read(a[j], b[j], j);				// load verticies
		    polygon.init(j, colour);						// initialise vertices and colour
		    polygon.calcs();						// find MAX,MIN
		    polygon.display(Plot);						// fill polygon
		    }
		else
		    {
		    for (j = 0; j < Sides; j++)
			{
			k = (j + offset < Sides) ? j + offset : j + offset - Sides;
			Plot.genline(a[j], b[j], a[k], b[k], colour);
			}
		    }
		break;
	    }
	}
    return 0;
    }

/**************************************************************************
    Get parameters for Geometry
**************************************************************************/

void	COtherFunctions::GetCount(long in)
    {
    count = in;
    }
void	COtherFunctions::GetSubtype(int in)
    {
    subtype = in;
    }
