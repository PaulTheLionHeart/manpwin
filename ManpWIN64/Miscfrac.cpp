/*
   MISCFRAC.CPP - Miscellaneous fractal-specific code (formerly in CALCFRAC.C in Fractint)

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include "Complex.h"
#include "Pixel.h"
#include "resource.h"
#include "Manp.h"

int	overflow;
int	save_release;

extern	int user_data(HWND hwnd);

/***************** Some small useful ones ********************/

void FPUsincos(double *Angle, double *Sin, double *Cos)
    {
    *Sin = sin(*Angle);
    *Cos = cos(*Angle);
    }

Complex ComplexPower(Complex x, Complex y)
    {
    Complex z;
    z = x ^ y;
    return z;
    }

int	NullSetup(void)				// sometimes we just don't want to do anything 
    {
    return 1;
    }

/**************************************************************************
    Initialise Mandelbrot
***************************************************************************/

int	init_mandel_df(void)
    {
    return 0;
    }

/**************************************************************************
	Determine count before 'Z' becomes unstable float version
***************************************************************************/

int	do_mandel_df(void)
    {
    return (true);
    }

/************ standalone engine for "bifurcation" types ***************/

/***************************************************************/
/* The following code now forms a generalised Fractal Engine   */
/* for Bifurcation fractal typeS.  By rights it now belongs in */
/* CALCFRACT.C, but it's easier for me to leave it here !      */

/* Original code by Phil Wilson, hacked around by Kev Allen.   */

/* Besides generalisation, enhancements include Periodicity    */
/* Checking during the plotting phase (AND halfway through the */
/* filter cycle, if possible, to halve calc times), quicker    */
/* floating-point calculations for the standard Verhulst type, */
/* and new bifurcation types (integer bifurcation, f.p & int   */
/* biflambda - the real equivalent of complex Lambda sets -    */
/* and f.p renditions of bifurcations of r*sin(Pi*p), which    */
/* spurred Mitchel Feigenbaum on to discover his Number).      */

/* To add further types, extend the fractalspecific[] array in */
/* usual way, with Bifurcation as the engine, and the name of  */
/* the routine that calculates the next bifurcation generation */
/* as the "orbitcalc" routine in the fractalspecific[] entry.  */

/* Bifurcation "orbitcalc" routines get called once per screen */
/* pixel column.  They should calculate the next generation    */
/* from the doubles Rate & Population (or the longs lRate &    */
/* lPopulation if they use integer math), placing the result   */
/* back in Population (or lPopulation).  They should return 0  */
/* if all is ok, or any non-zero value if calculation bailout  */
/* is desirable (eg in case of errors, or the series tending   */
/* to infinity).		Have fun !		       */
/***************************************************************/

int	bifurcation(void)
    {
    return	gManp->Pixel[0]->bifurcation(user_data);
    }

/**************************************************************************
	Get a key or mouse click and process
**************************************************************************/

int	user_data(HWND hwnd)
    {
    MSG msg;

    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}

    if (!gManp->bTrack && gManp->time_to_reinit + gManp->time_to_restart + gManp->time_to_quit + gManp->time_to_zoom + gManp->time_to_load)
	return(-1);					  // don't do this if mouse-button is down
    return 0;
    }

