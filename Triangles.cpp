/*
   TRIANGLES.CPP a module for Triangle Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "OtherFunctions.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 16

/**************************************************************************
	Get the power of an interger
***************************************************************************/

int	COtherFunctions::IntPower(int base, int n)
    {
    int	i, p;

    p = 1;
    for (i = 1; i <= n; i++)
	p *= base;
    return p;
    }

/**************************************************************************
	Get the power of a double
***************************************************************************/

double	COtherFunctions::FloatPower(double base, int n)
    {
    int	i;
    double p;

    p = 1.0;
    for (i = 1; i <= n; i++)
	p *= base;
    return p;
    }

/**************************************************************************
	Draw Box
***************************************************************************/

void	COtherFunctions::DrawBox(int x0, int y0, int x1, int y1, DWORD colour, BOOL fill, CPlot Plot)
    {
    int	i, j, k;

    if (fill)
	{
	j = MIN(y0, y1);
	k = MAX(y0, y1);
	for (i = j; i <= k; i++)
	    Plot.genline(x0, i, x1, i, colour);
	}
    else
	{
	Plot.genline(x0, y0, x0, y1, colour);
	Plot.genline(x0, y1, x1, y1, colour);
	Plot.genline(x1, y1, x1, y0, colour);
	Plot.genline(x1, y0, x0, y0, colour);
	}
    }

/**************************************************************************
	Triangle Fractal Type Images
***************************************************************************/

int	COtherFunctions::DoTriangle()

    {
    double  u1, u2, v1, v2, size, ForthRoot;
    int	    i, k, l, m, n, n1;
    double  a, FloatExp;
    double  x, y, z;
    int	    x1, x2, x3, y1, y2, y3;
    int	    exp, exp1;
    DWORD   colour;
    double  u[MAXPOINTS], v[MAXPOINTS];
    int	    c[MAXPOINTS], d[MAXPOINTS];
    int	    offset;
    BOOL    flag = FALSE;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    if (Sides > MAXPOINTS)
	Sides = MAXPOINTS;
    if (Sides < 3)
	Sides = 3;
    if (Divisor == 0)		// no norty division
	Divisor = 1;
    if (Remainder == 0)		// no norty division
	Remainder = 1;
    *totpasses = Passes;
//    z = PI * (Sides - 2) / Sides;
    z = 2.0 * PI / Sides;
    a = (subtype == 'O') ? ROOT2 : ROOT3;
    for (m = 0; m < Passes; m++)
	{
	*curpass = m;
	colour = (ExpandPalette) ? threshold * (DWORD)m / Passes : (DWORD)m;
	switch (subtype)
	    {
	    case 'A':					
	    case 'B':					
	    case 'C':					
	    case 'E':					
	    case 'L':					
		exp1 = IntPower(3, m);
		break;
	    case 'M':					
	    case 'N':					
		exp1 = IntPower(Passes, m);
		break;
	    case 'D':					
		exp1 = IntPower(5, m);
		break;
	    case 'F':					
	    case 'G':					
	    case 'H':					
	    case 'J':					
	    case 'K':					
		exp1 = IntPower(5, m) + 2;
		break;
	    case 'I':					
		exp1 = IntPower(3, m) - 1;
		break;
	    case 'O':					
		exp1 = IntPower(10, m);
		break;
	    case 'P':					
	    case 'Q':					
	    case 'R':					
	    case 'S':					
	    case 'T':					
	    case 'U':					
		exp1 = IntPower(8, m);
		break;
	    default:					
		exp1 = IntPower(8, m);
		break;
	    }
	for (n = 0; n < exp1; n++)
	    {
	    if (UserData(hwnd) == -1)		// user pressed a key?
		 return -1;
	    n1 = n;
	    x = 0;
	    y = 0;
	    switch (subtype)
		{
		case 'A':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(2, k);
			x = x + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			}
		    break;
		case 'B':					
		case 'D':					
		case 'I':					
		case 'M':					
		case 'N':					
		case 'O':					
		case 'U':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(2, k);
			x = x + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'C':					
		    for (k = 0; k <= (int)sqrt((double)(m+n)) - m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(2, k);
			x = x + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'E':					
		    for (k = 0; k <= (int)sqrt(fabs((double)(m+n+Passes))); k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(2, k);
			x = x + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'F':					
		case 'G':					
		case 'H':					
		case 'J':					
		case 'K':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(3, k);
			x = x + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			exp = IntPower(2, k);
			x = x + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			}
		    break;
		case 'L':					
		    for (k = 0; k <= (int)sqrt(fabs((double)(m+n+Passes)) + sqrt((double)(m+Passes))); k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(2, k);
			x = x + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'P':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(3, k);
			x = x + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'Q':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
//			exp = IntPower(2, k);
			x = x + sin((jAngle * l + 1) * PI / wAngle);
			y = y + cos((jAngle * l + 1) * PI / wAngle);
			}
		    break;
		case 'R':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(2, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			exp = IntPower(5, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			}
		    break;
		case 'S':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(10, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			exp = IntPower(6, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			}
		    break;
		case 'T':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			FloatExp = FloatPower(Exponent, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / FloatExp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / FloatExp;
			exp = IntPower(5, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			}
		    break;
		default:					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = IntPower(2, k);
			x = x + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		}

	    exp = IntPower(2, m + 1);
	    u1 = x + a / (double)exp;
	    u2 = x - a / (double)exp;
	    v1 = y - 1 / (double)exp;
	    v2 = y + 1 / (double)(subtype == 'F' || subtype == 'H' || subtype == 'J' 
					|| subtype == 'K' || subtype == 'M' || subtype == 'N' || subtype == 'U' ? exp : (exp >> 1));

	    x1 = (int)((u1 - hor) * xscale);
	    x2 = (int)((u2 - hor) * xscale);
	    x3 = (int)((x - hor) * xscale);
	    y1 = (int)((vert + mandel_width - v1) * yscale);
	    y2 = (int)((vert + mandel_width - v2) * yscale);
	    y3 = (int)((vert + mandel_width - y) * yscale);

	    switch (subtype)
		{
		case 'F': 
    		    Plot.genline(x1, y2, x3, y2, colour);
		    Plot.genline(x3, y2, x2, y2, colour);
		    Plot.genline(x2, y2, x1, y1, colour);
		    Plot.genline(x1, y1, x2, y1, colour);
		    Plot.genline(x2, y1, x1, y2, colour);
		    break;
		case 'J': 
		    Plot.genline(x1, y2, x3, y1, colour);
		    Plot.genline(x3, y1, x2, y2, colour);
		    Plot.genline(x2, y2, x1, y1, colour);
		    Plot.genline(x1, y1, x2, y1, colour);
		    Plot.genline(x2, y1, x1, y2, colour);
		    break;
		case 'H':
		    Plot.genline(x1, y2, x3, y1, colour);
		    Plot.genline(x3, y1, x2, y1, colour);
		    Plot.genline(x2, y1, x1, y2, colour);
		    break;
		case 'K':
		    Plot.genline(x1, y2, x3, y1, colour);
		    Plot.genline(x3, y1, x2, y2, colour);
		    Plot.genline(x2, y2, x1, y2, colour);
		    break;
		case 'M':
		case 'N':
		    DrawBox(x1, y1, x2, y2, colour, subtype == 'M', Plot);
		    break;
		case 'O':
		    ForthRoot = sqrt(sqrt((double)Sides));
		    size = sqrt((u1 - u2) * (u1 - u2) + (v1 - v2) * (v1 - v2)) / (4 * ForthRoot);	// defines the size of the polygon, that is a function of the number of sides
		    if (PlotStars)
			offset = (Sides % 2) ? Sides / 2 : Sides / 2 - 1;
		    else
			offset = 1;
		    for (i = 0; i < Sides; i++)
			{
			u[i] = x + size * cos(i * z);
			v[i] = y + size * sin(i * z);
			c[i] = (int)((u[i] - hor) * xscale);
			d[i] = (int)((vert + mandel_width - v[i]) * yscale);
			}
		    if (FillPolygon)
			{
			for (i = 0; i < Sides; i++)
			    polygon.read(c[i], d[i], i);				// load verticies
			polygon.init(i, colour);					// initialise vertices and colour
			polygon.calcs();						// find MAX,MIN
			polygon.display(Plot);						// fill polygon
			}
		    else
			{
			for (i = 0; i < Sides; i++)
			    {
			    k = (i + offset < Sides) ? i + offset : i + offset - Sides;
			    Plot.genline(c[i], d[i], c[k], d[k], colour);
			    }
			}
		    break;
		case 'P':
		case 'Q':
		    Plot.genline(x1, y1, x3, y2, colour);
		    Plot.genline(x3, y2, x2, y1, colour);
		    Plot.genline(x2, y1, x1, y1, colour);
		    Plot.genline(x1, y1, x1, y2, colour);
		    Plot.genline(x1, y2, x2, y2, colour);
		    Plot.genline(x2, y2, x2, y1, colour);
		    // PAINT ((u1 + u2 + x) / 3, (v1 + v2 + v1) / 3), m
		    break;
		case 'U':
		    if (Plot.GetColour(x1, y1) > 0)	// we realy need to test ALL the vertices. So we must analyse all the combinations of x(i) X y(i)
			break;  
		    if (Plot.GetColour(x1, y2) > 0)
			break; 
		    if (Plot.GetColour(x3, y1) > 0)
			break;
		    if (Plot.GetColour(x3, y2) > 0)
			break;
		    if (Plot.GetColour(x2, y1) > 0)
			break;
		    if (Plot.GetColour(x2, y2) > 0)
			break;
		    DrawBox(x1, y1, x2, y2, colour, TRUE, Plot);
		    break;
		case 'V':
		    ForthRoot = sqrt(sqrt((double)Sides));
//		    ForthRoot = (1/(double)Sides) * 2;
		    size = sqrt((u1 - u2) * (u1 - u2) + (v1 - v2) * (v1 - v2)) / (ForthRoot);	// defines the size of the polygon, that is a function of the number of sides
//		    if (PlotStars)
//			offset = (Sides % 2) ? Sides / 2 : Sides / 2 - 1;
//		    else
			offset = 1;
		    for (i = 0; i < Sides; i++)
			{
			u[i] = x + size * cos(i * z);
			v[i] = y + size * sin(i * z);
			c[i] = (int)((u[i] - hor) * xscale);
			d[i] = (int)((vert + mandel_width - v[i]) * yscale);
			}
		    flag = FALSE;
		    for (i = 0; i < Sides; i++)
			{
			if (Plot.GetColour(c[i], d[i]) > 0)	// we really need to test ALL the vertices. So we must analyse all the combinations of x(i) X y(i)
			    {
			    flag = TRUE;
			    break;  
			    }
			if (Plot.GetColour((c[i] + c[(i+1)%Sides])/2, (d[i] + d[(i+1)%Sides])/2) > 0)	// we really need to test intermediate points between ALL the vertices. 
			    {
			    flag = TRUE;
			    break;  
			    }
			if (Sides == 3)								// triangles still blow up even with midpoints. try additional points
			    {
			    if (Plot.GetColour((c[i] + c[(i+1)%Sides] * 3)/4, (d[i] + d[(i+1)%Sides] * 3)/4) > 0)	 // one quater along
				{
				flag = TRUE;
				break;  
				}
			    if (Plot.GetColour((c[i] * 3 + c[(i+1)%Sides])/4, (d[i] * 3 + d[(i+1)%Sides])/4) > 0)	 // three quarters along
				{
				flag = TRUE;
				break;  
				}
			    }
			}
		    if (flag)
			break;
		    for (i = 0; i < Sides; i++)
			polygon.read(c[i], d[i], i);				// load verticies
		    polygon.init(i, colour);					// initialise vertices and colour
		    polygon.calcs();						// find MAX,MIN
		    polygon.display(Plot);					// fill polygon
		    break;
		default:
		    Plot.genline(x1, y1, x3, y2, colour);
		    Plot.genline(x3, y2, x2, y1, colour);
		    Plot.genline(x2, y1, x1, y1, colour);
		    break;
		}
	    }
	}
    return 0;
    }

/**************************************************************************
    Get parameters for Triangles
**************************************************************************/

void	COtherFunctions::GetExpandPalette(BOOL in)	// spread colours across palette
    {
    ExpandPalette = in;
    }
void	COtherFunctions::GetPlotStars(BOOL in)		// stars or polygons?
    {
    PlotStars = in;
    }
void	COtherFunctions::GetFillPolygon(BOOL in)	// fill polygons?
    {
    FillPolygon = in;
    }
void	COtherFunctions::GetjAngle(int in)
    {
    jAngle = in;
    }
void	COtherFunctions::GetwAngle(int in)		// sets the overall shape of the fractal
    {
    wAngle = in;
    }
void	COtherFunctions::GetDivisor(int in)
    {
    Divisor = in;
    }
void	COtherFunctions::GetRemainder(int in)		// who knows, I don't
    {
    Remainder = in;
    }
void	COtherFunctions::GetExponent(double in)
    {
    Exponent = in;
    }
void	COtherFunctions::GetSides(int in)		// sides of polygon
    {
    Sides = in;
    }
void	COtherFunctions::GetPasses(int in)		// number of passes
    {
    Passes = in;
    }

