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
//#include	"anim.h"

//extern	HWND	gManp->GlobalHwnd;			// This is the main windows handle

extern	int	user_data(HWND);
extern	void	InitOscillator(double c1[], int dimensions);
extern	int	DisplayOscillator(double c1[], double cn[], double dt, DWORD colour, double i, int dimensions, int FindCentre);
extern	void	PlotExtras(void);

extern	BOOL	DisplayAxes;
extern	double	dt;				// delta time

static	double	xscale, yscale;

static	double	xMax, yMax, zMax, xMin, yMin, zMin;

/**************************************************************************
	Harmonic Oscillators with Nonlinear Damping (0532 Thermostated Ergodic Oscillator)
	J. C. Sprott and W. G. Hoover September 10, 2017
	https://www.researchgate.net/requests/r106697652.....DOI: 10.1142/S0218127417300373
***************************************************************************/

int	DoHarmonicOscillatorsNonlinearDamping0532ErgodicOscillator(void)
    {
    double	i, c1[3], cn[3], a, b;

    c1[0] = gManp->param[10];	// x
    c1[1] = gManp->param[11];	// y
    c1[2] = gManp->param[12];	// z

    a = gManp->param[0];
    b = gManp->param[1];
    gManp->totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < gManp->iterations; i++)
	{
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	gManp->curpass = (int)(i * gManp->totpasses / gManp->iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] - c1[2] * (a + b * c1[1] * c1[1]) * c1[1];
	cn[2] = a * (c1[1] * c1[1] - 1.0) + b * (c1[1] * c1[1] - 3) * c1[1] * c1[1];
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % gManp->threshold), i, 3, 0) < 0)
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

    c1[0] = gManp->param[10];	// x
    c1[1] = gManp->param[11];	// y
    c1[2] = gManp->param[12];	// z

    a = gManp->param[0];
    gManp->totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < gManp->iterations; i++)
	{
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	gManp->curpass = (int)(i * gManp->totpasses / gManp->iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] - a * c1[1] * c1[2] * c1[2] * c1[2];
	cn[2] = c1[1] * c1[1] - a;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % gManp->threshold), i, 3, 0) < 0)
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

    c1[0] = gManp->param[10];	// x
    c1[1] = gManp->param[11];	// y
    c1[2] = gManp->param[12];	// z
    c1[3] = gManp->param[13];	// w

    a = gManp->param[0];
    gManp->totpasses = 10;

    InitOscillator(c1, 4);						// pass in number of dimensios
    for (i = 0; i < gManp->iterations; i++)
	{
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	gManp->curpass = (int)(i * gManp->totpasses / gManp->iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] - c1[2] * c1[1];
	cn[2] = c1[1] * c1[1] / a - 1.0 - c1[2] * c1[3];
	cn[3] = c1[2] * c1[2] - 1.0;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % gManp->threshold), i, 4, 0) < 0)
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

    c1[0] = gManp->param[10];	// x
    c1[1] = gManp->param[11];	// y
    c1[2] = gManp->param[12];	// z

    a = gManp->param[0];
    gManp->totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < gManp->iterations; i++)
	{
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	gManp->curpass = (int)(i * gManp->totpasses / gManp->iterations);
	cn[0] = c1[1];
	cn[1] = -c1[0] - c1[1] * c1[2];
	cn[2] = fabs(c1[1]) - a;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % gManp->threshold), i, 3, 0) < 0)
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

    c1[0] = gManp->param[10];	// x
    c1[1] = gManp->param[11];	// y
    c1[2] = gManp->param[12];	// z

    a = gManp->param[0];
    gManp->totpasses = 10;

    InitOscillator(c1, 3);						// pass in number of dimensios
    for (i = 0; i < gManp->iterations; i++)
	{
	if (user_data(gManp->GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	gManp->curpass = (int)(i * gManp->totpasses / gManp->iterations);
	tx = a + 0.2 * c1[0] * c1[0];
	cn[0] = c1[1];
	cn[1] = -c1[0] - c1[1] * c1[2];
	cn[2] = c1[1] * c1[1] - tx;
	if (DisplayOscillator(c1, cn, dt, ((DWORD)(i / 10.0) % gManp->threshold), i, 3, 0) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }


