/*
   SPROTTOSC.CPP a module to explore Sprott Chaotic Oscillators

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
	Harmonic Oscillators with Nonlinear Damping (0532 Thermostated Ergodic Oscillator)
	J. C. Sprott and W. G. Hoover September 10, 2017
	https://www.researchgate.net/requests/r106697652.....DOI: 10.1142/S0218127417300373
***************************************************************************/

int	DoHarmonicOscillatorsNonlinearDamping0532ErgodicOscillator(void)

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
	cn[1] = -c1[0] - c1[2] * (a + b * c1[1] * c1[1]) * c1[1];
	cn[2] = a * (c1[1] * c1[1] - 1.0) + b * (c1[1] * c1[1] - 3) * c1[1] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Harmonic Oscillators with Nonlinear Damping (KBB Oscillator)
	J. C. Sprott and W. G. Hoover September 10, 2017
	https://www.researchgate.net/requests/r106697652.....DOI: 10.1142/S0218127417300373
***************************************************************************/

int	DoHarmonicOscillatorsNonlinearDampingKBBOscillator(void)

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
	cn[0] = c1[1];
	cn[1] = -c1[0] - a * c1[1] * c1[2] * c1[2] * c1[2];
	cn[2] = c1[1] * c1[1] - a;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Harmonic Oscillators with Nonlinear Damping (MKT Doubly-Thermostated Ergodic Oscillator)
	J. C. Sprott and W. G. Hoover September 10, 2017
	https://www.researchgate.net/requests/r106697652.....DOI: 10.1142/S0218127417300373
***************************************************************************/

int	DoHarmonicOscillatorsNonlinearDampingMKTErgodicOscillator(void)

    {
    double	i, c1[4], cn[4], a;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    c1[3] = param[13];	// w

    a = param[0];
    totpasses = 10;

    InitOscillator(c1, 4);						// pass in number of dimensios
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] - c1[2] * c1[1];
	cn[2] = c1[1] * c1[1] / a - 1.0 - c1[2] * c1[3];
	cn[3] = c1[2] * c1[2] - 1.0;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 4, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Harmonic Oscillators with Nonlinear Damping (Munmuangsaen Oscillator)
	J. C. Sprott and W. G. Hoover September 10, 2017
	https://www.researchgate.net/requests/r106697652.....DOI: 10.1142/S0218127417300373
***************************************************************************/

int	DoHarmonicOscillatorsNonlinearDampingKMunmuangsaenscillator(void)

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
	cn[0] = c1[1];
	cn[1] = -c1[0] - c1[1] * c1[2];
	cn[2] = fabs(c1[1]) - a;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
    Harmonic Oscillators with Nonlinear Damping (Nose-Hoover Oscillator with a temperature gradient)
    J. C. Sprott and W. G. Hoover September 10, 2017
    https://www.researchgate.net/requests/r106697652.....DOI: 10.1142/S0218127417300373
***************************************************************************/

int	DoHarmonicOscillatorsNonlinearDampingNoseHoover(void)

    {
    double	i, c1[3], cn[3], a, tx;

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
	tx = a + 0.2 * c1[0] * c1[0];
	cn[0] = c1[1];
	cn[1] = -c1[0] - c1[1] * c1[2];
	cn[2] = c1[1] * c1[1] - tx;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }


