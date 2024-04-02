/****************************************************
    TETRATION.CPP a module to explore Tetration
    http://paulbourke.net/fractals/tetration/
    Written in Microsoft Visual C++ by Paul de Leeuw.
****************************************************/

#include <math.h>
#include "manp.h"
#include "Complex.h"
#include "pixel.h"

/************************************
   Iterative definition for Tetration
************************************/

PBCOMPLEX CPixel::ComplexTetration(PBCOMPLEX aPBC, int n)
    {
    int i;
    PBCOMPLEX b,unity = {1,0,1,0};

    if (n < 0) // Actually this is improper, tetration is only defined for n >= 0
	n = -n;
    if (n == 0)
	return(unity);

    b = aPBC;
    for (i = 0; i < n; i++) 
	b = PBComplexPower(aPBC,b);
    return(b);
    }

/*****************************************
	Complex power of a complex number
*****************************************/

PBCOMPLEX CPixel::PBComplexPower(PBCOMPLEX w, PBCOMPLEX z)
    {
    PBCOMPLEX d;
	
    d.r = pow(w.r,z.x) * exp(-z.y * w.t);
    d.t = z.y * log(w.r) + z.x * w.t;
    Complex2Cart(&d);
    return(d);
    }

void CPixel::Complex2Polar(PBCOMPLEX *aPBC)
    {
    aPBC->r = sqrt(aPBC->x*aPBC->x + aPBC->y*aPBC->y);
    aPBC->t = atan2(aPBC->y,aPBC->x);
    }
void CPixel::Complex2Cart(PBCOMPLEX *aPBC)
    {
    aPBC->x = aPBC->r * cos(aPBC->t);
    aPBC->y = aPBC->r * sin(aPBC->t);
    }

/*****************************************
   Recursive definition for Tetration
*****************************************/

double CPixel::RealTetration(double a,int n)
    {
    double b;

    if (n < 0) // Actually this is improper, tetration is only value for n >= 0
	n = -n;
    if (n == 0) 
	{
	return(1);
	} 
    else
	{
	b = pow(a,RealTetration(a,n-1));
	}
    return(b);
    }

/*****************************************
   Bailout Routines for Tetration
*****************************************/

BOOL	CPixel::TetrationBailout(PBCOMPLEX z)
    {
    switch (BailoutMethod) 
	{
	case 5: // theta
	    return (z.t > rqlim);
	    break;
	case 4: // both
	    return (z.x > rqlim || z.y > rqlim);
	    break;
	case 3: // mag
	    return (z.r > rqlim);
	    break;
	case 2: // both
	    return (fabs(z.x) > rqlim || fabs(z.y) > rqlim);
	    break;
	case 1: // imag
	    return (z.y > rqlim);
	    break;
	case 0:
	default: // real
	    return (z.x > rqlim);
	    break;
	}
    }

/**************************************************************************
	Initialise Tetration
***************************************************************************/

int	CPixel::InitTetration(Complex *z, Complex *q)

    {
    if (!juliaflag)
	{
	z->x = q->x + param[0];
	z->y = q->y + param[1];
	}
    aPBC.x = z->x;
    aPBC.y = z->y;
    BailoutMethod = (int)param[2];
    Complex2Polar(&aPBC);
    return 0;
    }

/**************************************************************************
	Determine count before 'Z' becomes unstable float version
***************************************************************************/

int	CPixel::DoTetration(long *iteration)

    {
    PBCOMPLEX	b;
    int		n;

    for (n = 0; n < threshold; n++) 
	{
	b = ComplexTetration(aPBC, n);
//	if ((b.r) > rqlim)
	if (TetrationBailout(b))
	    break;
	}
    *iteration = n;
    return 1;
    }

