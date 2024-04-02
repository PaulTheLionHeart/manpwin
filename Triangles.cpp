/*
   TRIANGLES.CPP a module for Triangle Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"menu.h"
#include	"anim.h"
#include	"polygon.h"
#include	"Plot.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 16

extern	HWND	GlobalHwnd;				// This is the main windows handle

extern	int	user_data(HWND);
extern	int	time_to_reinit;			// time to start fractal again??
extern	int	xdots, ydots;

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	double	param[];
extern	WORD	type;				// fractal type
extern	int	subtype;			// A - V
extern	int	curpass, totpasses;

extern	CPlot	Plot;		// image plotting routines 

static	double	xscale, yscale;
static	int	Passes = 8;
static	BOOL	ExpandPalette = TRUE;		// spread colours across palette
static	BOOL	PlotStars = TRUE;		// stars or polygons?
static	BOOL	FillPolygon = TRUE;		// fill polygons?
static	int	jAngle = 4, wAngle = 6;		// sets the overall shape of the fractal
//static	int	Divisor = 3, Remainder = 3;	// who knows, I don't
static	int	Divisor = 9, Remainder = 8;	// who knows, I don't
static	int	Sides = 3;			// sides of polygon
static	double	Exponent = 1.0;
static	CPoly	polygon;			// polygon class

/**************************************************************************
	Get the power of an interger
***************************************************************************/

int	power(int base, int n)
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

double	FloatPower(double base, int n)
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

void	DrawBox(int x0, int y0, int x1, int y1, DWORD colour, BOOL fill, CPlot Plot)
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

int	DoTriangle(void)

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

    totpasses = Passes;
//    z = PI * (Sides - 2) / Sides;
    z = 2.0 * PI / Sides;
    a = (subtype == 'O') ? ROOT2 : ROOT3;
    for (m = 0; m < Passes; m++)
	{
	curpass = m;
	colour = (ExpandPalette) ? threshold * (DWORD)m / Passes : (DWORD)m;
	switch (subtype)
	    {
	    case 'A':					
	    case 'B':					
	    case 'C':					
	    case 'E':					
	    case 'L':					
		exp1 = power(3, m);
		break;
	    case 'M':					
	    case 'N':					
		exp1 = power(Passes, m);
		break;
	    case 'D':					
		exp1 = power(5, m);
		break;
	    case 'F':					
	    case 'G':					
	    case 'H':					
	    case 'J':					
	    case 'K':					
		exp1 = power(5, m) + 2; 
		break;
	    case 'I':					
		exp1 = power(3, m) - 1;
		break;
	    case 'O':					
		exp1 = power(10, m);
		break;
	    case 'P':					
	    case 'Q':					
	    case 'R':					
	    case 'S':					
	    case 'T':					
	    case 'U':					
		exp1 = power(8, m);
		break;
	    default:					
		exp1 = power(8, m);
		break;
	    }
	for (n = 0; n < exp1; n++)
	    {
	    if (user_data(GlobalHwnd) == -1)		// user pressed a key?
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
			exp = power(2, k);
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
			exp = power(2, k);
			x = x + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'C':					
		    for (k = 0; k <= (int)sqrt((double)(m+n)) - m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = power(2, k);
			x = x + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'E':					
		    for (k = 0; k <= (int)sqrt(fabs((double)(m+n+Passes))); k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = power(2, k);
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
			exp = power(3, k);
			x = x + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			exp = power(2, k);
			x = x + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			}
		    break;
		case 'L':					
		    for (k = 0; k <= (int)sqrt(fabs((double)(m+n+Passes)) + sqrt((double)(m+Passes))); k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = power(2, k);
			x = x + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'P':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = power(3, k);
			x = x + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		case 'Q':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
//			exp = power(2, k);
			x = x + sin((jAngle * l + 1) * PI / wAngle);
			y = y + cos((jAngle * l + 1) * PI / wAngle);
			}
		    break;
		case 'R':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = power(2, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			exp = power(5, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			}
		    break;
		case 'S':					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = power(10, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			exp = power(6, k);
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
			exp = power(5, k);
			x = x + (sin((jAngle * l + 1) * PI / wAngle)) / exp;
			y = y + (cos((jAngle * l + 1) * PI / wAngle)) / exp;
			}
		    break;
		default:					
		    for (k = 0; k < m; k++)
			{
			l = n1 % Remainder;
			n1 = n1 / Divisor;
			exp = power(2, k);
			x = x + ((sin((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			y = y + ((cos((jAngle * l + 1) * PI / wAngle)) / exp) * 2;
			}
		    break;
		}

	    exp = power(2, m + 1);
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

DLGPROC FAR PASCAL TriangleDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static     char	temp;
     static     UINT	tempParam;
     static     WORD	temp_special;
     HWND		hCtrl;
     BOOL		bTrans ;
     char		s[24];

     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = subtype;
	        switch (subtype)
		    {
		    case 'A':
			tempParam = IDC_A;
			break;
		    case 'B':
			tempParam = IDC_B;
			break;
		    case 'C':
			tempParam = IDC_C;
			break;
		    case 'D':
			tempParam = IDC_D;
			break;
		    case 'E':
			tempParam = IDC_E;
			break;
		    case 'F':
			tempParam = IDC_F;
			break;
		    case 'G':
			tempParam = IDC_G;
			break;
		    case 'H':
			tempParam = IDC_H;
			break;
		    case 'I':
			tempParam = IDC_I;
			break;
		    case 'J':
			tempParam = IDC_J;
			break;
		    case 'K':
			tempParam = IDC_K;
			break;
		    case 'L':
			tempParam = IDC_L;
			break;
		    case 'M':
			tempParam = IDC_M;
			break;
		    case 'N':
			tempParam = IDC_N;
			break;
		    case 'O':
			tempParam = IDC_O;
			break;
		    case 'P':
			tempParam = IDC_P;
			break;
		    case 'Q':
			tempParam = IDC_Q;
			break;
		    case 'R':
			tempParam = IDC_R;
			break;
		    case 'S':
			tempParam = IDC_S;
			break;
		    case 'T':
			tempParam = IDC_T;
			break;
		    case 'U':
			tempParam = IDC_U;
			break;
		    case 'V':
			tempParam = IDC_V;
			break;
		    default:				// uninitialised
			tempParam = IDC_A;
			temp = 'A';
			break;
		    }
		CheckRadioButton(hDlg, IDC_A, IDC_V, tempParam);
		SetDlgItemInt(hDlg, IDC_PARAM1, (int)Passes, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM2, (int)Remainder, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM3, (int)Divisor, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM4, (int)jAngle, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM5, (int)wAngle, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM6, (int)Sides, TRUE);

	        sprintf(s, "%3.2f", Exponent);
		SetDlgItemText(hDlg, IDC_PARAM7, s);
		hCtrl = GetDlgItem (hDlg, IDC_EXPANDPALETTE);
		SendMessage(hCtrl, BM_SETCHECK, ExpandPalette, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_STAR);
		SendMessage(hCtrl, BM_SETCHECK, PlotStars, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_FILLPOLYGON);
		SendMessage(hCtrl, BM_SETCHECK, FillPolygon, 0L);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return FALSE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_A:
		    case IDC_B:
		    case IDC_C:
		    case IDC_D:
		    case IDC_E:
		    case IDC_F:
		    case IDC_G:
		    case IDC_H:
		    case IDC_I:
		    case IDC_J:
		    case IDC_K:
		    case IDC_L:
		    case IDC_M:
		    case IDC_N:
		    case IDC_O:
		    case IDC_P:
		    case IDC_Q:
		    case IDC_R:
		    case IDC_S:
		    case IDC_T:
		    case IDC_U:
		    case IDC_V:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_A:
				temp = 'A';
				break;
			    case IDC_B:
				temp = 'B';
				break;
			    case IDC_C:
				temp = 'C';
				break;
			    case IDC_D:
				temp = 'D';
				break;
			    case IDC_E:
				temp = 'E';
				break;
			    case IDC_F:
				temp = 'F';
				break;
			    case IDC_G:
				temp = 'G';
				break;
			    case IDC_H:
				temp = 'H';
				break;
			    case IDC_I:
				temp = 'I';
				break;
			    case IDC_J:
				temp = 'J';
				break;
			    case IDC_K:
				temp = 'K';
				break;
			    case IDC_L:
				temp = 'L';
				break;
			    case IDC_M:
				temp = 'M';
				break;
			    case IDC_N:
				temp = 'N';
				break;
			    case IDC_O:
				temp = 'O';
				break;
			    case IDC_P:
				temp = 'P';
				break;
			    case IDC_Q:
				temp = 'Q';
				break;
			    case IDC_R:
				temp = 'R';
				break;
			    case IDC_S:
				temp = 'S';
				break;
			    case IDC_T:
				temp = 'T';
				break;
			    case IDC_U:
				temp = 'U';
				break;
			    case IDC_V:
				temp = 'V';
				break;
			    }

			CheckRadioButton(hDlg, IDC_A, IDC_V, (int) LOWORD(wParam));
		        return (DLGPROC)TRUE ;

		    case IDOK:
			Passes = GetDlgItemInt(hDlg, IDC_PARAM1, &bTrans, TRUE);
			Remainder = GetDlgItemInt(hDlg, IDC_PARAM2, &bTrans, TRUE);
			Divisor = GetDlgItemInt(hDlg, IDC_PARAM3, &bTrans, TRUE);
			jAngle = GetDlgItemInt(hDlg, IDC_PARAM4, &bTrans, TRUE);
			wAngle = GetDlgItemInt(hDlg, IDC_PARAM5, &bTrans, TRUE);
			Sides = GetDlgItemInt(hDlg, IDC_PARAM6, &bTrans, TRUE);
			if (Sides > MAXPOINTS)
			    Sides = MAXPOINTS;
			if (Sides < 3)
			    Sides = 3;
			GetDlgItemText(hDlg, IDC_PARAM7, s, 20);
			sscanf(s, "%lf", &Exponent);
			subtype = temp;
			hCtrl = GetDlgItem (hDlg, IDC_EXPANDPALETTE);
			ExpandPalette = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_STAR);
			PlotStars = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_FILLPOLYGON);
			FillPolygon = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }
