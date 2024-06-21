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
#include "Fract.h"
#include ".\parser\TrigFn.h"	

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
extern	double	rqlim;			// bailout level
extern	Complex	z;
extern	CFract	Fractal;		// current fractal stuff

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

/*
This is an attempt to run it from within Cpixel class but the images aren't so nice

int DoPopcorn(void)
    {
    Complex c, z;
    Complex x, y, px, a = Complex(3, 0);
    double h = 0.05;

    threshold = (long)param[0];
    h = param[2];
    a.x = param[3];
    a.y = param[4];
    Complex	s, t;
    Complex r;
    x = z.x;
    y = z.y;
    px = x;
    s = g1(y + g2(a*y));
    t = g1(x + g2(a*x));
    x = x - h * s.x - h * t.y;
    s = g1(px + g2(a*px));
    t = g1(y + g2(a*y));
    y = y - h * s.x - h * t.y;
    z = x + y.CFlip();
    x = x / Complex(z.CSumSqr(), 0.0), y = y / Complex(z.CSumSqr(), 0.0);
    z = x + y.CFlip();
    return (sqr(z.x) + sqr(z.y) <= max_size);
    }
*/

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

void FPUsincos(const double *Angle, double *Sin, double *Cos)
    {
    if (isnan(*Angle) || isinf(*Angle))
	{
	*Sin = 0.0;
	*Cos = 1.0;
	}
    else
	{
	*Sin = sin(*Angle);
	*Cos = cos(*Angle);
	}
    }

/*
include additional trig... not so successful

int DoPopcornFractal()

    {
    Complex tmpx, tmpy, tmp;
    double  h = param[0];
    Complex c;
    CTrigFn TrigFn;
    double  rqlim2 = sqrt(rqlim);

    c.x = param[1];
    c.y = param[2];
    // tmpx contains the generalized value of the old real "x" equation
    tmp = c * z.y;
    //	    CMPLXtimesreal(parm2, old.y, tmp);			// tmp = (C * old.y)        
    TrigFn.CMPLXtrig(&tmp, &tmpx, Fractal.Fn2Index);		// tmpx = trig1(tmp)   
    tmpx.x += z.y;					// tmpx = old.y + trig1(tmp)
    TrigFn.CMPLXtrig(&tmpx, &tmp, Fractal.Fn1Index);		// tmp = trig0(tmpx) 
    tmpx = tmp * h;
    //	    CMPLXmult(tmp, parm, tmpx);				// tmpx = tmp * h

		// tmpy contains the generalized value of the old real "y" equation 
    tmp = c * z.x;
    //	    CMPLXtimesreal(parm2, old.x, tmp);			// tmp = (C * old.x) 
    //	    CMPLXtrig3(tmp, tmpy);				// tmpy = trig3(tmp) 
    TrigFn.CMPLXtrig(&tmp, &tmpy, Fractal.Fn2Index);		// tmpx = trig1(tmp)  // maybe we can add other values later  

    tmpy.x += z.x;					// tmpy = old.x + trig1(tmp)
    //	    CMPLXtrig2(tmpy, tmp);				// tmp = trig2(tmpy) 
    TrigFn.CMPLXtrig(&tmpx, &tmp, Fractal.Fn1Index);		// tmp = trig0(tmpx)  // maybe we can add other values later

    //	    CMPLXmult(tmp, parm, tmpy);				// tmpy = tmp * h 
    tmpy = tmp * h;

    z.x = z.x - tmpx.x - tmpy.y;
    z.y = z.y - tmpy.x - tmpx.y;
    if (plot == noplot)
	{
	plot_orbit(new.x, new.y, 1 + row % colors);
	old = new;
	}
    tempsqrx = sqr(z->x); i  tmpx
    tempsqry = sqr(z->y);

    if ((sqr(z.x) + sqr(z.y)) >= rqlim
	|| fabs(z.x) > rqlim2 || fabs(z.y) > rqlim2)
	return(1);
    //	    old = new;
    return(0);
    //	    d = z->CSumSqr();
    //	    return (d >= rqlim);
    }
*/


int DoPopcornFractal()
    {
    Complex	g_tmp_z = { 0.0 };
    double	g_sin_x;
    double	g_cos_x;
    double	g_magnitude = 0.0;
    double	StepSize = (param[0] > 0.0) ? param[0] : 0.05;
    Complex	C;

    C.x = param[1];
    C.y = param[2];

    if (C.x == 0.0 && C.y == 0)
	C = 3.0;
    g_tmp_z = z * C;
    FPUsincos(&g_tmp_z.x, &g_sin_x, &g_cos_x);
    double siny;
    double cosy;
    FPUsincos(&g_tmp_z.y, &siny, &cosy);
    g_tmp_z.x = g_sin_x / g_cos_x + z.x;
    g_tmp_z.y = siny / cosy + z.y;
    FPUsincos(&g_tmp_z.x, &g_sin_x, &g_cos_x);
    FPUsincos(&g_tmp_z.y, &siny, &cosy);
    z.x = z.x - siny * StepSize;
    z.y = z.y - g_sin_x * StepSize;
/*
    if (g_plot == noplot)
	{
	plot_orbit(g_new_z.x, g_new_z.y, 1 + g_row % g_colors);
	g_old_z = g_new_z;
	}
    else
*/
//    g_magnitude = sqr(z.x) + sqr(z.y);
    return (sqr(z.x) + sqr(z.y) >= rqlim);

//    if (g_magnitude >= rqlim)
//	{
//	return 1;
//	}
//    g_old_z = g_new_z;
//    return 0;
    }

/*
int DoPopcornFractal()
    {
    g_tmp_z = g_old_z;
    g_tmp_z.x *= 3.0;
    g_tmp_z.y *= 3.0;
    FPUsincos(&g_tmp_z.x, &g_sin_x, &g_cos_x);
    double siny;
    double cosy;
    FPUsincos(&g_tmp_z.y, &siny, &cosy);
    g_tmp_z.x = g_sin_x / g_cos_x + g_old_z.x;
    g_tmp_z.y = siny / cosy + g_old_z.y;
    FPUsincos(&g_tmp_z.x, &g_sin_x, &g_cos_x);
    FPUsincos(&g_tmp_z.y, &siny, &cosy);
    g_new_z.x = g_old_z.x - param[0] * siny;
    g_new_z.y = g_old_z.y - param[0] * g_sin_x;
    
	if (g_plot == noplot)
	    {
	    plot_orbit(g_new_z.x, g_new_z.y, 1 + g_row % g_colors);
	    g_old_z = g_new_z;
	    }
	else
    
    {
    g_temp_sqr_x = sqr(g_new_z.x);
    }
    g_temp_sqr_y = sqr(g_new_z.y);
    g_magnitude = g_temp_sqr_x + g_temp_sqr_y;
    if (g_magnitude >= rqlim)
	{
	return 1;
	}
    g_old_z = g_new_z;
    return 0;
    }
*/
