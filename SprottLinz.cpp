/*
   SPROTTLINZ.CPP a module to explore Sprott-Linz Chaotic Attractor

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

/**************************************************************************
	Sprott-302 Chaotic Attractor
***************************************************************************/

int	DoSprott302(void)

    {
    double	i, c1[3], cn[3], b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    b = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = sin(c1[1]) - b * c1[0];
	cn[1] = sin(c1[2]) - b * c1[1];
	cn[2] = sin(c1[0]) - b * c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-330 Chaotic Attractor
***************************************************************************/

int	DoSprott330(void)

    {
    double	i, c1[3], cn[3], alpha, beta, a, m0, m1, fxz, k;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    alpha = param[0];
    beta = param[1];
    a = param[2];
    m0 = param[3];
    m1 = param[4];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	fxz = m1 * c1[0] + 0.5 * (m0 - m1) * (fabs(c1[0] + 1) - fabs(c1[0] - 1));
	k = (fabs(c1[2]) <= a) ? -fxz : fxz;
	cn[0] = alpha * (c1[1] - k);
	cn[1] = c1[0] - c1[1] + c1[2];
	cn[2] = -beta * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-374 'Easter Island' Chaotic Attractor
***************************************************************************/

int	DoSprott374(void)

    {
    double	i, c1[3], cn[3], gp, gr, np, nr;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    gp = param[0];
    gr = param[1];
    np = param[2];
    nr = param[3];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = gp * c1[0] * (1.0 - c1[0] / np / c1[2]);
	cn[1] = gr * c1[1] * (1.0 - c1[1] / c1[2]);
	cn[2] = c1[2] / (1.0 + nr * c1[1]) * (1.0 - c1[2]) - c1[0];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 60.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-390 Chaotic Attractor
***************************************************************************/

int	DoSprott390(void)

    {
    double	i, c1[4], cn[4], a, b, c, r;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    c1[3] = param[13];	// w

    a = param[0];
    b = param[1];
    c = param[2];
    r = param[3];
    totpasses = 10;

    InitOscillator(c1, 4);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = a * (c1[1] - c1[0]) + c1[3];
	cn[1] = c * c1[0] - c1[1] - c1[0] * c1[2];
	cn[2] = -b * c1[2] + c1[0] * c1[1];
	cn[3] = -c1[0] * c1[2] - r * c1[3];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 4, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-391 Chaotic Attractor
***************************************************************************/

int	DoSprott391(void)

    {
    double	i, c1[3], cn[3], c8;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    c8 = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[0] - c1[1] + 0.5 * c1[2] - 3.0 * c1[1] * c1[2];
	cn[1] = -0.1 * c1[0] - 6.0 * c1[1] + c1[0] * c1[2] - c1[1] * c1[2];
	cn[2] = 0.06 * c1[0] - 10.0 * c1[1] - 5.0 * c1[2] + 2.0 + c1[0] + c1[1] + c8 * c1[2] * c1[0];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-392 Chaotic Attractor
	Elementary quadratic chaotic flows  or Sprott 392
	Elementary quadratic chaotic flows with no equilibria
	Yasser Shekofteh, Sajad Jafari, Julien Clinton Sprott
	J.C. Sprott, S. Mohammad Reza Hashemi Golpayegani 16 January 2013
***************************************************************************/

int	DoSprott392(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];

    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = -0.8 * c1[0] - 0.5 * c1[1] * c1[1] + c1[0] * c1[2] + a;
	cn[1] = -0.8 * c1[1] - 0.5 * c1[2] * c1[2] + c1[1] * c1[0] + a;
	cn[2] = -0.8 * c1[2] - 0.5 * c1[0] * c1[0] + c1[1] * c1[2] + a;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-395 Chaotic Attractor
***************************************************************************/

int	DoSprott395(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] * c1[2] + a;
	cn[1] = c1[0] * c1[0] - c1[1];
	cn[2] = 1.0 - 4.0 * c1[0];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-397 Chaotic Attractor
	Chaotic biological systems  or Sprott 397
	A Gaussian mixture model based cost function for parameter
	estimation of chaotic biological systems
	Yasser Shekofteh, Sajad Jafari, Julien Clinton Sprott
	S. Mohammad Reza Hashemi Golpayegani, Farshad Almasganj 24 May 2014
***************************************************************************/

int	DoSprott397(void)

    {
    double	i, ro, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    ro = param[0];
    a = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = 3.0 * c1[0] * c1[0] - c1[0] * c1[0] * c1[0] + c1[1] - c1[2] + a;
	cn[1] = 1.0 - 5.0 * c1[0] * c1[0] - c1[1];
	cn[2] = ro * (4.0 * (c1[0] + 1.6) - c1[2]);
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-406 Chaotic Attractor
***************************************************************************/

int	DoSprott406(void)

    {
    double	i, c1[4], cn[4], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    c1[3] = param[13];	// w

    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 4);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] - c1[0];
	cn[1] = -a * c1[0] * c1[2] + c1[3];
	cn[2] = c1[0] * c1[1] - 1.0;
	cn[3] = -b * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 4, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-407-I Chaotic Attractor
	Simplest Chaotic Flows with Involutional Symmetries
	August 9, 2013 J. C. SPROTT June 5, 2013
***************************************************************************/

int	DoSprott407a(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[0] * (1.0 - c1[1]);
	cn[1] = c1[2];
	cn[2] = -c1[1] - a * c1[2] + c1[0] * c1[0];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-407-II Chaotic Attractor
	Simplest Chaotic Flows with Involutional Symmetries
	August 9, 2013 J. C. SPROTT June 5, 2013
***************************************************************************/

int	DoSprott407b(void)

    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] - b * c1[0];
	cn[1] = c1[1] * c1[2] - c1[0];
	cn[2] = a - c1[1] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-407-III Chaotic Attractor
	Simplest Chaotic Flows with Involutional Symmetries
	August 9, 2013 J. C. SPROTT June 5, 2013
***************************************************************************/

int	DoSprott407c(void)

    {
    double	i, c1[3], cn[3], r;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    r = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] - c1[0];
	cn[1] = -c1[0] * c1[2];
	cn[2] = c1[0] * c1[1] - r;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-407-IV Chaotic Attractor
	Simplest Chaotic Flows with Involutional Symmetries
	August 9, 2013 J. C. SPROTT June 5, 2013
***************************************************************************/

int	DoSprott407d(void)

    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] - b * c1[0];
	cn[1] = -c1[0] + c1[1] * c1[2];
	cn[2] = a - c1[1] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-408 Chaotic Attractor
	MODIFIED COLPITTS OSCILLATOR-I MULTISTABILITY IN A BUTTERFLY FLOW
	CHUNBIAO LI and J. C. SPROTT June 5, 2013
***************************************************************************/

int	DoSprott408(void)

    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] * (1 + c1[2]);
	cn[1] = c1[2] * (c1[1] - c1[0]);
	cn[2] = b - a * c1[2] - c1[0] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-409 Chaotic Attractor
***************************************************************************/

int	DoSprott409(void)

    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] + c1[1] * c1[2];
	cn[2] = -c1[0] - a * c1[0] * c1[1] - b * c1[0] * c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-409II Chaotic Attractor
***************************************************************************/

int	DoSprott409II(void)

    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] + c1[1] * c1[2];
	cn[2] = -a * c1[0] * c1[1] - b * c1[0] * c1[1] - c1[1] * c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-411 Chaotic Attractor
***************************************************************************/

int	DoSprott411(void)

    {
    double	i, c1[3], cn[3], a, alpha, beta, m0, m1, k, f;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    alpha = param[0];
    beta = param[1];
    m0 = param[2];
    m1 = param[3];
    a = param[4];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	f = m1 * c1[0] + 0.5 * (m0 - m1) * (fabs(c1[0] + 1.0) - fabs(c1[0] - 1.0));
	k = (fabs(c1[2]) <= a) ? -f : f;
	cn[0] = alpha * (c1[1] - k);
	cn[1] = c1[0] - c1[1] + c1[2];
	cn[2] = -beta * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-412 Chaotic Attractor
	Multistability of coupled systems (or Sprott 412)
	Comment on “How to obtain extreme multistability in coupled dynamical systems”
	J. C. Sprott, Chunbiao Li 23 June 2014
***************************************************************************/

int	DoSprott412(void)

    {
    double	i, c1[4], cn[4], u;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    c1[3] = param[13];	// w
    u = param[0];

    totpasses = 10;

    InitOscillator(c1, 4);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] - c1[0];
	cn[1] = -c1[0] * c1[2];
	cn[2] = c1[0] * c1[1]/* - c1[3]*/ - u + c1[0];
	cn[3] = c1[1] - c1[0];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 4, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-416 Chaotic Attractor
***************************************************************************/

int	DoSprott416(void)

    {
    double	i, c1[4], cn[4], a, b, c;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    c1[3] = param[13];	// w

    a = param[0];
    b = param[1];
    c = param[2];
    totpasses = 10;

    InitOscillator(c1, 4);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] - c1[0] * c1[2] - c1[1] * c1[2] + c1[3];
	cn[1] = a * c1[0] * c1[2];
	cn[2] = c1[1] * c1[1] - b * c1[2] * c1[2];
	cn[3] = -c1[1] * c;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 4, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-421 Chaotic Attractor
	Nose-Hoover oscillators II (or Sprott 421)
	Heat conduction, and the lack thereof, in time-reversible dynamical systems:
	Generalized Nose-Hoover oscillators with a temperature gradient
	Julien Clinton Sprott, William Graham Hoover and Carol Griswold Hoover 25 April 2014
***************************************************************************/

int	DoSprott421(void)

    {
    double	i, c1[3], cn[3];

    c1[0] = param[10];	// q
    c1[1] = param[11];	// p
    c1[2] = param[12];	// z

    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] - c1[2] * c1[1];
	cn[2] = c1[1] * c1[1] - 1.0 - 0.42 * tanh(c1[0]);
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-423 Chaotic Attractor
	A dynamical system with a strange attractor and invariant tori
	21 March 2014
***************************************************************************/

int	DoSprott423(void)

    {
    double	i, c1[3], cn[3];

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] + 2.0 * c1[0] * c1[1];
	cn[1] = 1.0 - 2.0 * c1[0] * c1[0] + c1[1] * c1[2];
	cn[2] = c1[0] - c1[0] * c1[0] - c1[1] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzA(void)

    {
    double	i, c1[3], cn[3];

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] + c1[1] * c1[2];
	cn[2] = 1 - c1[1] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzB(void)

    {
    double	i, c1[3], cn[3];

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] * c1[2];
	cn[1] = c1[0] - c1[1];
	cn[2] = 1 - c1[0] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzC(void)

    {
    double	i, c1[3], cn[3];

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] * c1[2];
	cn[1] = c1[0] - c1[1];
	cn[2] = 1.0 - c1[0] * c1[0];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzD(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = -c1[1];
	cn[1] = c1[0] + c1[2];
	cn[2] = c1[0] * c1[2] + a * c1[1] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzE(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] * c1[2];
	cn[1] = c1[0] * c1[0] - c1[1];
	cn[2] = 1 - a * c1[0];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzF(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] + c1[2];
	cn[1] = -c1[0] + a * c1[1];
	cn[2] = c1[0] * c1[0] - c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzG(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = a * c1[0] + c1[2];
	cn[1] = c1[0] * c1[2] - c1[1];
	cn[2] = -c1[0] + c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzH(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = -c1[1] + c1[2] * c1[2];
	cn[1] = c1[0] + a * c1[1];
	cn[2] = c1[0] - c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzI(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = a * c1[1];
	cn[1] = c1[0] + c1[2];
	cn[2] = c1[0] + c1[1] * c1[1] - c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzJ(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = a * c1[2];
	cn[1] = -a * c1[1] + c1[2];
	cn[2] = -c1[0] + c1[1] + c1[1] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzK(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[0] * c1[1] - c1[2];
	cn[1] = c1[0] - c1[1];
	cn[2] = c1[0] + a * c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzL(void)

    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] + a * c1[2];
	cn[1] = b * c1[0] * c1[0] - c1[1];
	cn[2] = 1 - c1[0];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzM(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = -c1[2];
	cn[1] = -c1[0] * c1[0] - c1[1];
	cn[2] = a + a * c1[0] + c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzQ(void)

    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = -c1[2];
	cn[1] = c1[0] - c1[1];
	cn[2] = a * c1[0] + c1[1] * c1[1] + b * c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i * 0.01) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Linz Chaotic Attractor
***************************************************************************/

int	DoSprottLinzS(void)

    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    b = param[1];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = a - c1[1];
	cn[1] = b + c1[2];
	cn[2] = c1[0] * c1[1] - c1[2];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)i % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott-Marcus Chaotic Attractor
***************************************************************************/

int	DoSprottMarcus(void)

    {
    double	i, c1[3], cn[3], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1] * c1[2] - a;
	cn[1] = c1[0] * c1[0] - c1[1] + c1[2] / 2.0;
	cn[2] = 1.0 - 4.0 * c1[0] + c1[2] / 5.0;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 5.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott on Wind Energy Conversion
	Design of a chaos synchronisation-based maximum power tracking controller 
	for a wind-energy conversion system
	Tung-Sheng Zhan, Jian Liung Chen, Shi-Jaw Chen, Cong-Hui Huang, Chia-Hung Lin 30th October 2013
***************************************************************************/

int	DoSprottWind(void)

    {
    double	i, c1[3], cn[3], a, b, s;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    a = param[0];
    b = param[1];

    totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	s = ((c1[0] < 0) ? -1.0 : (c1[0] == 0.0) ? 0.0 : 1.0);
	cn[0] = c1[1];
	cn[1] = c1[2];
	cn[2] = -a * c1[2] - b * c1[1] - 1.2 * c1[0] + s + s;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 35.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

