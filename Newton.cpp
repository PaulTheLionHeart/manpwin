/*
    NEWTON.CPP a module for the per pixel calculations of the Newton fractal. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.

    The roots of a polynomial function of the form x^n - 1 = 0.
    
    Newton's method to determine the root of f(x) = 0, given a value of x:

    DO WHILE (abs(x2 - x1)>= delta1 or abs(f(x2) > delta2 or f'(x1) != 0)
	set x2 = x1 - f(x1) / f'(x1)
	set x1 = x2
    ENDDO

    Note that complex arithmetic is required.
*/

#include	"manp.h"
#include	<math.h>
#include	"fractalp.h"
#include	"complex.h"
#include	".\parser\cmplx.h"

extern	long	iteration;
extern	BYTE	juliaflag;			// Julia implementation of fractal

extern	long	threshold;
extern	long	color;
extern	int	period_level;			// 0 for no periodicity checking
extern	BYTE	degree;
extern	WORD	type;				// M=mand, N=Newton etc
extern	int	subtype;			// B=basin, S=stripe, N=normal
						// Distance of complex z from unit circle
#define DIST1(z)		(((z).x-1.0)*((z).x-1.0)+((z).y)*((z).y))
#define distance(z1,z2)		(sqr((z1).x-(z2).x)+sqr((z1).y-(z2).y))
#define sqr(x)			((x)*(x))
//#define FLT_MIN			10e-50
//#define FLT_MAX			10e+50
#define MAXROOTS		64
#define	MINSIZE			1.0e-9

extern	int	root;
extern	double	param[];

//static	Complex	New, old;
static	Complex	roots[MAXROOTS];			// roots
static	Complex	croot, cdegree;

static	double	thresh;
static	double	TwoPi, MPthreshold;

extern	Complex	z, q;

/**************************************************************************
	Newton/NewtBasin Routines: Setup
**************************************************************************/
/*
void	NewtonSetup(void)

    {
    int	    i;

    degree = (int)param[0];			// so we have to get the degrees and subtype from the parameters
    if (degree < 2)
	degree = 3;				// defaults to 3, but 2 is possible 
    switch ((int)param[1])
	{
	case 0:
	    subtype = 'N';
	    break;
	case 1:
	    subtype = 'S';
	    break;
	case 2:
	    subtype = 'B';
	    break;
	default:
	    subtype = 'N';
	    break;
	}

    root = 1;					// set up table of roots of 1 along unit circle

		    				// precalculated values 
    thresh = 0.3 * PI / (double) degree;	// less than half distance between roots

    if (subtype == 'S' || subtype == 'B')
	{
	if(degree > MAXROOTS)
	    degree = MAXROOTS;

			// list of roots to discover where we converged for newtbasin

	for (i = 0; i < degree; i++)
	    {
	    roots[i].x = cos(i*PI*2.0/(double)degree);
	    roots[i].y = sin(i*PI*2.0/(double)degree);
	    }
	}
    }
*/
/**************************************************************************
	Initialise Newton fractal
**************************************************************************/
/*
int	init_newton_df(void)

    {
    period_level = FALSE;			// no periodicity checking
    color = 0;
    if (!juliaflag)
	z = q/3;
    return(TRUE);
    }
*/
/**************************************************************************
	Run Newton fractal
**************************************************************************/
/*
int	do_newton_df(void)

    {
    Complex	z1, z2, zd;
    double	d;
    int		tmpcolor;
    int		i;

    color = iteration;
    z2 = z;
    z1 = z.CPolynomial(degree - 1);
    z = z - (z1*z - q - 1) / (z1 * degree);
    zd = z - z2;
    d = zd.CSumSqr();
    if (d < MINSIZE)
	{

	if (subtype == 'S' || subtype == 'B')
	    {
	    tmpcolor = -1;
	    // this code determines which degree-th root of root the Newton formula converges to. 
	    // The roots of a 1 are distributed on a circle of radius 1 about the origin.
	    for (i = 0; i < degree; i++)
		{
		// color in alternating shades with iteration according to which root of 1 it converged to
		if (distance(roots[i], z2) < thresh)
		    {
		    if (subtype == 'S')
			tmpcolor = 1 + (i & 7) + ((color & 1) << 3);
		    else
			tmpcolor = 1 + i;
		    break;
		    }
		}
	    if (tmpcolor == -1)
		color = threshold;
	    else
		color = tmpcolor;
	    }

	return(TRUE);
	}
    else
	return(FALSE);
    }
*/
/**************************************************************************
	Complex Newton/NewtBasin Routines: Setup
**************************************************************************/

/*
int ComplexNewtonSetup(void) 
    {
//    if (!fractalspecific[type].isNewton)	// the function was called from a Fractint Par file, (default of flag)
//	subtype = fractalspecific[type].flags;
//    fractalspecific[type].isNewton = FALSE;	// restore to default. Only TRUE if Newton called from a dialogue box
//    degree = (int)param[0];			// show the real part of the degree in status
    subtype = ((int)param[4] == 0.0) ? 'N' : 'B';
    return(1);
    }
*/
/*
int InitComplexNewton(void) 
    {
    MPthreshold = 0.001;
//    if(param[0] != 0.0 || param[1] != 0.0 || param[2] != 0.0 || param[3] != 0.0) 
	{
	croot = 1;
	cdegree = 3;
	croot.x = param[2];
	croot.y = param[3];
	cdegree.x = param[0];
	cdegree.y = param[1];
	TwoPi = asin(1.0) * 4;
	thresh = 0.3 * PI / cdegree.x;	// less than half distance between roots
	z = q;
	}
    return(1);
    }    

*/

/**************************************************************************
	Run Complex Newton fractal
**************************************************************************/
/*
int ComplexNewton(void) 
    {
    double mod;
    int coloriter;
    Complex	tmp, temp, cd1, New;

   // New = ((cdegree-1) * old**cdegree) + croot
   //         ----------------------------------
   //              cdegree * old**(cdegree-1)         

    color = iteration;
    cd1.x = cdegree.x - 1.0;
    cd1.y = cdegree.y;
//    temp = CComplexPower(z, cd1);
    temp = z^cd1;
    New = temp*z;
    tmp = New - croot;
    if(tmp.CSumSqr() < MPthreshold) 
	{
	if (subtype == 'N')
	    return(1);
	if(fabs(z.y) < .01)
	    z.y = 0.0;

	temp = z.CLog();
	tmp = temp*cdegree;
	mod = tmp.y/TwoPi;
	coloriter = (long)mod;
	if(fabs(mod - coloriter) > 0.5) 
	    {
	    if(mod < 0.0)
		coloriter--;
	    else
		coloriter++;
	    }
	coloriter += 2;
	if(coloriter < 0)
	    coloriter += 128;
	iteration = coloriter;		    // PHD 2009-10-13
	return(1);
	}
    tmp = New*cd1;
    tmp += croot;
//    tmp.x += croot.x;
//    tmp.y += croot.y;
    cd1 = temp*cdegree;
    z = tmp/cd1;
    return(0);
    }
*/
