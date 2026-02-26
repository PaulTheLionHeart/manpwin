/*
Miscellaneous fractal-specific code (formerly in CALCFRAC.C)
*/

#include <windows.h>
#include "Complex.h"
#include "Pixel.h"
#include "resource.h"

extern	BYTE	juliaflag;		//* Julia implementation of fractal 
//extern	Complex	z, q, j;
extern	double	rqlim;			// bailout level
extern	WORD	type;			// M=mand, J=Julia 1,2,4-> etc
extern	int	subtype;		// A - E

extern	int	time_to_zoom;			// time to zoom in or out?
extern	int	time_to_restart;		// time to restart?
extern	int	time_to_reinit;			// time to reinitialize?
extern	int	time_to_quit;			// time to quit?
extern	int	time_to_load;			// time to load file?
extern	BOOL	bTrack;				// TRUE if user is selecting a region

extern	std::vector<std::unique_ptr<CPixel>> Pixel;	// routines for escape fractals
//extern	CPixel	Pixel[];

static	Complex	Sqr;
static	double	real_imag;		// z_real * z_imag 

int	row, col;
long	color;
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
/*
    if (!juliaflag)
	{
	z.x = q.x + param[0];
	z.y = q.y + param[1];
	}
    Sqr = 0;
    real_imag = 0.0;
*/
    return 0;
    }

/**************************************************************************
	Determine count before 'Z' becomes unstable float version
***************************************************************************/

int	do_mandel_df(void)

    {
/*
    Sqr.x = z.x * z.x;
    Sqr.y = z.y * z.y;
    real_imag = z.x * z.y;
    z.x = q.x + Sqr.x - Sqr.y;
    z.y = q.y + real_imag + real_imag;
*/
    return ((Sqr.x + Sqr.y) >= rqlim);
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
    return	Pixel[0]->bifurcation(user_data);
    }

/**************************************************************************
	Get a key or mouse click and process
**************************************************************************/

int user_data(HWND hwnd)

    {
    MSG msg;

    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}

    if (!bTrack && time_to_reinit + time_to_restart + time_to_quit + time_to_zoom + time_to_load)
	return(-1);					  // don't do this if mouse-button is down
    return 0;
    }

