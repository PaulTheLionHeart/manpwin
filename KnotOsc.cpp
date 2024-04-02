/*
   KNOTOSC.CPP a module to explore Knots interpreted as oscillators

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	<time.h>
#include	<math.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"menu.h"
#include	"anim.h"

extern	HWND	GlobalHwnd;			// This is the main windows handle

extern	int	user_data(HWND);
extern	void	InitOscillator(double c1[], int dimensions);
extern	int	DisplayOscillator(double c1[], double cn[], double dt, DWORD colour, double i, int dimensions, int FindCentre);
extern	void	PlotExtras(void);

extern	double	x_rot;				/* angle display plane to x axis */
extern	double	y_rot;				/* angle display plane to y axis */
extern	double	z_rot;				/* angle display plane to z axis */

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	double	param[];
extern	WORD	type;				// fractal type
extern	int	subtype;			// A - E
extern	int	curpass, totpasses;
extern	CFract	Fractal;			// current fractal stuff
//extern	double	VertBias;			// allow vertical stretching of the image
extern	BOOL	DisplayAxes;
extern	double	dt;				// delta time

static	double	xscale, yscale;
extern	double	iterations;

static	double	xMax, yMax, zMax, xMin, yMin, zMin;

///////////////////////////////////////////////////////////////////////////
//  Knot Oscillators start here
///////////////////////////////////////////////////////////////////////////

/**************************************************************************
	Eight Knot as an Oscillator
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2:5)
***************************************************************************/

int	DoEightKnotOsc(void)

    {
    double	t, c1[3], cn[3], dt2;

    c1[0] = cn[0] = param[10];	// x
    c1[1] = cn[1] = param[11];	// y
    c1[2] = cn[2] = param[12];	// z

    dt2 = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t += dt2)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = 10*cos(t) + 10*cos(3*t);
	c1[1] = 6*sin(t) + 10*sin(3*t);
	c1[2] = 4*sin(3*t) - 10*sin(6*t);
	if (DisplayOscillator(c1, cn, dt, ((DWORD)t % threshold), t, 3, 0) < 0)
	    break;
	cn[0] = c1[0];
	cn[1] = c1[1];
	cn[2] = c1[2];
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Granny Knot as an Oscillator
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.3)
***************************************************************************/

int	DoGrannyKnotOsc(void)

    {
    double	t, c1[3], cn[3], dt2;

    c1[0] = cn[0] = param[10];	// x
    c1[1] = cn[1] = param[11];	// y
    c1[2] = cn[2] = param[12];	// z

    dt2 = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t += dt2)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = -22*cos(t) - 128*sin(t) - 44*cos(3*t) - 78*sin(3*t);
	c1[1] = -10*cos(2*t) - 27*sin(2*t) + 38*cos(4*t) + 46*sin(4*t);
	c1[2] =  70*cos(3*t) - 40*sin(3*t);
	if (DisplayOscillator(c1, cn, dt, ((DWORD)t % threshold), t, 3, 0) < 0)
	    break;
	cn[0] = c1[0];
	cn[1] = c1[1];
	cn[2] = c1[2];
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	~Knot Universes in Bianchi Type I Cosmology - Tempestuous as an Oscillator
	Ratbay Myrzakulov 15 July 2012
	https://arxiv.org/abs/1204.1093
***************************************************************************/

int	DoKnotUniversesITempestuousOsc(void)

    {
    double	c1[4], cn[4], t, s21, s22, s23, s31, s32, s33, c21, c22, c23, c31, c32, c33, dt2;

    c1[0] = cn[0] = param[10];	// x
    c1[1] = cn[1] = param[11];	// y
    c1[2] = cn[2] = param[12];	// z
    c1[3] = cn[3] = param[13];	// z

    dt2 = param[0];
    totpasses = 10;

    InitOscillator(c1, 4);						// pass in number of dimensions
    for (t = -iterations; t < iterations; t += dt2)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	s21 = sin(2 * t); 
	s22 = s21 * s21; 
	s23 = s21 * s22;
	s31 = sin(3 * t); 
	s32 = s31 * s31; 
	s33 = s31 * s32;
	c21 = cos(2 * t); 
	c22 = c21 * c21; 
	c23 = c21 * c22;
	c31 = cos(3 * t); 
	c32 = c31 * c31; 
	c33 = c31 * c32;
	c1[0] = (-12 * s32 + 36 * c31 + 18 * c32) * c21 - 49 * s21 * (26 / 49 + c31) * s31;
	c1[1] = -18 * s21 * c32 + (-49 * s31 * c21 - 36 * s21) * c31 - 26 * s31 * c21 + 12 * s32 * s21;
	c1[2] = -30 * s31 * (2 + c31) * c22 - 38 * s21 * (c32 - 27 / 38 * s32 + 58 / 19 * c31 + 40 / 19) * c21 + 30 * s31 * s22 * (2 + c31);
	c1[3] = (6 * c22 - 6 * s22) * c33 + (24 * c22 - 22 * s31 * s21 * c21 - 24 * s22) * c32 
					+ ((6 * s32 - 24) * s22) * c31 - (12 * (c21 - 3 / 4 * s31 * s21)) * (s31 * c21 + 4 / 3 * s21) * s31;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(t * 30.0) % threshold), t, 4, 0) < 0)
	    break;
	cn[0] = c1[0];
	cn[1] = c1[1];
	cn[2] = c1[2];
	cn[3] = c1[3];
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Marcus Torus Knot
	Marcus Rezende August 2018
***************************************************************************/

int	DoMarcusTorusKnotOsc(void)

    {
    double	t, c1[3], cn[3], dt2, R, r, p, q;

    c1[0] = cn[0] = param[10];	// x
    c1[1] = cn[1] = param[11];	// y
    c1[2] = cn[2] = param[12];	// z

    dt2 = param[0];
    R = param[1];
    r = param[2];
    p = param[3];
    q = param[4];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensions
    for (t = -iterations; t < iterations; t += dt2)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	cn[0] = (R + r * cos(p * t)) * cos(q * t);
	cn[1] = (R + r * cos(p * t)) * sin(q * t);
	cn[2] = R * cos(p * t);
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(t * 30.0) % threshold), t, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Torus Knot
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 4.1-13)
***************************************************************************/

int	DoTorusKnotOsc(void)

    {
    double	t, c1[3], cn[3], dt2, R, r, p, q;

    c1[0] = cn[0] = param[10];	// x
    c1[1] = cn[1] = param[11];	// y
    c1[2] = cn[2] = param[12];	// z

    dt2 = param[0];
    R = param[1];
    r = param[2];
    p = param[3];
    q = param[4];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t += dt2)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = (R + r * cos(p * t)) * cos(q * t);
	c1[1] = (R + r * cos(p * t)) * sin(q * t);
	c1[2] = r * sin(p * t);
	if (DisplayOscillator(c1, cn, dt, ((DWORD)t % threshold), t, 3, 0) < 0)
	    break;
	cn[0] = c1[0];
	cn[1] = c1[1];
	cn[2] = c1[2];
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Trefoil Knot A as an Oscillator
	Meier 2001
	http://www.3d-meier.de/ (Representation of knots 2.1)
***************************************************************************/

int	DoTrefoilKnotAOsc(void)

    {
    double	t, c1[3], cn[3], dt2;

    c1[0] = cn[0] = param[10];	// x
    c1[1] = cn[1] = param[11];	// y
    c1[2] = cn[2] = param[12];	// z

    dt2 = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t += dt2)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = (2 + cos(1.5*t))*cos(t);
	c1[1] = (2 + cos(1.5*t))*sin(t);
	c1[2] =  sin(1.5*t);
	if (DisplayOscillator(c1, cn, dt, ((DWORD)t % threshold), t, 3, 0) < 0)
	    break;
	cn[0] = c1[0];
	cn[1] = c1[1];
	cn[2] = c1[2];
	}
    PlotExtras();
    return 0;
    }

