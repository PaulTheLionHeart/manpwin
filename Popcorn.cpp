////////////////////////////////////////////////////////
//	POPCORN.CPP a module for the per pixel calculations of the Popcorn fractal. 
//	Popcorn fractals are instances of Integrated Fractal Systems involving trigonometric
//	functions. In this program, we study the effect of spherical, swirl and 
//	pseudo-horseshoe functions on popcorn fractals to produce talismanic and tantric designs.
//	Written in Microsoft Visual 'C++' by Paul de Leeuw.
////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Dib.h"
#include "ManpWIN.h"
#include "Manp.h"
#include "Complex.h"
#include "Plot.h"

extern	long	threshold;
extern	int	curpass, totpasses;
extern	HWND	GlobalHwnd;		// This is the main windows handle
extern	double	mandel_width;		// width of display 
extern	double	hor;			// horizontal address 
extern	double	vert;			// vertical address 
extern	double	ScreenRatio;		// ratio of width / height for the screen
extern	double	xgap;			// gap between pixels 
extern	double	ygap;			// gap between pixels 
extern	double	param[];
extern	int	xdots, ydots;

extern	int	user_data(HWND);
extern	CPlot	Plot;			// image plotting routines 

static	double	xmax=1.7,xmin=-1.7,ymax=1.7,ymin=-1.7;
static	int	max_size = 1124;
static	int	variation = 0;

/**************************************************************************
	Transformation variations
**************************************************************************/

Complex g1(Complex x)
    {
    Complex	t;

    switch (variation)
	{
	case 0:
	    return (x.CSin());
	case 1:
	    return (x.CSin()+x.CCos());
	case 2:
	    t = x + x.CSin();
	    return (t.CSinh());
	case 3:
	    return (x.CSinh());
	case 4:
	    return (x.CSin());
	case 5:
	    return (x.CSin()+x.CCos());
	default:
	    return (x.CSin());
	}
    }

Complex g2(Complex x)
    {
    Complex	t;

    switch (variation)
	{
	case 0:
	    return (x.CTan());
	case 1:
	    return (x+x.CTan());
	case 2:
	    t = x+x.CTan();
	    return (x.CCos());
	case 3:
	    t = (x)+x.CTan();
	    return (t.CSin());
	case 4:
	    t = (x)+x.CTanh();
	    return t.CSinh();
	case 5:
	    t = (x)+x.CTan();
	    return (t.CSin());
	default:
	    return (x.CTan());
	}
    }

/**************************************************************************
	Popcorn fractal engine
**************************************************************************/

int DoPopcorn(void)
    {
    Complex c,z; 
    Complex x,y,px,a=Complex(3,0);
    int color;
    int col=0,row;
    double h = 0.05;

    threshold = (long)param[0];
    variation = (int)(fabs(param[1]));
    h = param[2];
    a.x = param[3];
    a.y = param[4];

    xmax = hor + mandel_width * ScreenRatio;
    xmin = hor;
    ymax = vert + mandel_width;
    ymin = vert;
    totpasses = 10;
    while(col<xdots)
	{ 
	col++;
	curpass = (int)col * totpasses / xdots;
	for(row=0;row<ydots;row++)
	    {
	    if (user_data(GlobalHwnd) == -1)				// user pressed a key?
		return -1;
	    z=c=Complex(xmin+col*xgap,ymax-row*ygap);
	    color=0;
	    while((color < threshold)&&(z.CSumSqr()<max_size))
		{
		Complex	s, t;
		Complex r;
		x=z.x;
		y=z.y; 
		px=x;
		s = g1(y+g2(a*y));
		t = g1(x+g2(a*x));
		x=x-h*s.x-h*t.y;
		s = g1(px+g2(a*px));
		t = g1(y+g2(a*y));
		y=y-h*s.x-h*t.y;
//		x=x-h*real(g1(y+g2(a*y)))-h*imag(g1(x+g2(a*x)));
//		y=y-h*real(g1(px+g2(a*px)))-h*imag(g1(y+g2(a*y)));
		z = x + y.CFlip();
		x = x / Complex(z.CSumSqr(), 0.0), y = y / Complex(z.CSumSqr(), 0.0);
		z = x + y.CFlip();
//		z = (x)+CFlip((y));
//		x = x / Complex(CSumSqr((z)), 0.0), y = y / Complex(CSumSqr((z)), 0.0);
//		z = (x)+CFlip((y));
		color++;
		double angle=45.0*PI/180.0;
		int x0=(int)((z.x*cos(angle)-z.y*sin(angle)-xmin)/xgap);
		int y0=(int)((ymax-z.x*sin(angle)-z.y*cos(angle))/ygap);
		if(Plot.GetColour(x0+50,y0)<224)
		    Plot.PlotPoint(x0+50,y0, Plot.GetColour(x0+50,y0)+1);
		}
	    }
	}
    return 0;
    }