///////////////////////////////////////////////////////////////
// FDimension.cpp taken from rsx_fdim.c
////////////////////////////////////////////////////////////////
//
// By: stephen c. ferguson
// http://home1.gte.net/itriazon/
// email: itriazon@gte.net
//
/////////////////////////////////////////////////////
// Updated in Microsoft Visual 'C++' by Paul de Leeuw.
///////////////////////////////////////////////////////////////

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include "filter.h"
#include "complex.h"
#include "resource.h"

///////////////////////////////////////////	
void CTZfilter::FDimension(int FilterType, long *iteration)
    {	
    long	i;
    Complex	t, t1, temp1, temp2;
    long	maxit_save;
// Average variables
    double	x_mean;
    double	y_mean;
    Complex	z_mean;
///////////////////////////////////////////	
// Fractal dimension variables
    int		i2, jrw;
    int		bDimensionVariant = 0;			// let's initialise it until we know what it means
    double	x_std, y_std, dm, da;
    double	denominator, x_rmin, x_rmax, y_rmin, y_rmax;
    Complex	z_std;
    Complex	z_rmin;
    Complex	z_rmax;

    Complex	cx_std;
    Complex	cy_std;
    Complex	cz_std;

    Complex	cFDx;
    Complex	cFDy;
    Complex	cFDz;

    Complex	cRng_x;
    Complex	cRng_y;
    Complex	cRng_z;

    Complex	cNMAX;

    double	dFDx;		// Fractal Dimension x
    double	dFDy;		// Fractal Dimension y
//    double	dFDz;		// Fractal Dimension z

    double	dFDx_0;		// Initial F Dimension x
    double	dFDy_0;		// Initial F Dimension y

    int		nDIter_x;
    int		nDIter_y;

    maxit_save = threshold;
    i = threshold = *iteration;	

	// Set up to Calculate the Fractal Dimension

	// Initialize the mean with zero
    x_mean = 0;
    y_mean = 0;

	// Initialize the range	with the first value
    x_rmin = pXTemp[0];	// x range min
    x_rmax = pXTemp[0];	// x range max
    y_rmin = pYTemp[0];	// y range min
    y_rmax = pYTemp[0];	// y range max

    switch(FilterType)
	{
	case 27:			// Fractal Dimension
	case 33:
	case 35:
	case 37:
	case 38:
	case 48:
	case 49:
	    if (i < 3)
		{
		rj = gj = bj = i = 0;
		threshold = maxit_save;
		return;
		}

	    for (i2 = 0 ; i2<threshold % MAXTEMP; i2++)
		{
		// Get sum x and y
		x_mean += pXTemp[i2];											
		y_mean += pYTemp[i2];

		// Get min x
		if (pXTemp[i2] < x_rmin)
		    x_rmin = pXTemp[i2];

		// Get max x
		if (pXTemp[i2] > x_rmax)
		    x_rmax = pXTemp[i2];

		// Get min y
		if (pYTemp[i2] < y_rmin)
		    y_rmin = pYTemp[i2];

		// Get max y
		if (pYTemp[i2] > y_rmax)
		    y_rmax = pYTemp[i2];
		}

	    x_mean = x_mean / threshold;
	    y_mean = y_mean / threshold;

	    x_std = 0;
	    y_std = 0;

	    for (i2 = 0 ; i2<threshold ; i2++)
		{
		x_std = x_std + (pXTemp[i2] - x_mean)*(pXTemp[i2] - x_mean); 
		y_std = y_std + (pYTemp[i2] - y_mean)*(pYTemp[i2] - y_mean); 
		}

	    x_std = sqrt(x_std / (threshold-1));		// Standard Deviation x
	    y_std = sqrt(y_std / (threshold-1));		// Standard Deviation y

	    // Initialize complex variables
	    //cx_std = cmplx(x_std, 0);	// Complex Standard deviation	x
	    //cy_std = cmplx(y_std, 0);	// Complex Standard deviation	y

	    cx_std.x = x_std;
	    cx_std.y = 0;

	    cy_std.x = y_std;
	    cy_std.y = 0;

	    //cFDx = cmplx (1, 0);				// Complex Fractal Dimension x
	    //cFDy = cmplx (1, 0);				// Complex Fractal Dimension y

	    cFDx.x = 1;
	    cFDx.y = 0;

	    cFDy.x = 1;
	    cFDy.y = 0;


	    //cRng_x = cmplx (x_rmax - x_rmin, 0);	// Complex Range x
	    //cRng_y = cmplx (y_rmax - y_rmin, 0);	// Complex Range y

	    cRng_x.x = x_rmax - x_rmin;
	    cRng_x.y = 0;

	    cRng_y.x = y_rmax - y_rmin;
	    cRng_y.y = 0;

	    //cNMAX = cmplx (threshold, 0); 

	    cNMAX.x = threshold; 
	    cNMAX.y = 0; 

	    //////////////////////////////////////////////////////////////
	    // Fractal Dimension Equation ...
	    // Ju = Upper Jaenisch coefficient 
	    // Initialize Ju=1;
	    // Iterate Ju = [log(R/S*N*J]/[log(1/N)] until abs(Ju+1)-Ju < .0001.
	    // if ( 0  <= J <= .5,) D(j) = 1/(1-Ju)
	    // if (.5  <=	J <= 1.0) D(j) = 1/Ju.
	    ///////////////////////////////////////////////////////////////

	    //////////////////////////////////////////////////////////////
	    // Real
	    ////////////////////////////////////////////////////////////				
	    dm = 99;
	    da = 0;
	    nDIter_x = 1;
	    denominator = log((double)(1.0/(double)threshold));

	    // Calculate Fractal Dimension (real)
	    da = dm;

	    //cFDx = (cRng_x/(cx_std*cNMAX*cFDx)).clog()/denominator;

//	    t = cRng_x/(cx_std*cNMAX*cFDx);
	    temp1 = cNMAX*cFDx;				// just in case my complex routines are shaky
	    temp2 = temp1*cx_std;
	    t = cRng_x/temp2;
	    t1 = t.CLog();
	    cFDx = t1/denominator;				

	    dm = cFDx.x;

	    dFDx_0 = 2.0 - dm;

	    dFDx = cFDx.x;

	    if (dFDx <= .5)
		dFDx = 1 / (1 - dFDx);
	    else
		dFDx = 1 / dFDx;

	    ////////////////////////////////////////////////////////////				
	    // Imaginary
	    ////////////////////////////////////////////////////////////				
	    dm = 99;
	    da = 0;
	    nDIter_y = 1;

	    // Calculate Fractal Dimension (real)
	    da = dm;

	    //cFDy = (cRng_y/(cy_std*cNMAX*cFDy)).clog()/denominator;

//	    t = cRng_y/(cy_std*cNMAX*cFDy);
	    temp1 = cNMAX*cFDy;				// just in case my complex routines are shaky
	    temp2 = temp1*cy_std;
	    t = cRng_y/temp2;
	    t1 = t.CLog();
	    cFDy = t1/denominator;												

	    dm = cFDy.x;

	    dFDy_0 = 2.0 - dm;

	    dFDy = cFDy.x;

	    if (dFDy <= .5)
		dFDy = 1 / (1 - dFDy);
	    else
		dFDy = 1 / dFDy;
	    ////////////////////////////////////////////////////////////				

	    if (bDimensionVariant)
		{
		xsav = dFDx;
		ysav = dFDy;
		}

	    ////////////////////////////////////////////////////////////
	    // Apply Fractal Dimension to a color

	    switch (nFDOption)
		{
		//case 0:
		//	// Addition
		//	jrw = (int) ((dFDx+dFDy) * 100 * dFactor);
		//	break;

		case 1:
		    // Fractal Dimension Real Initial (FD_0)
		    rj = gj = bj = (dFDx_0 * 200.0 * dFactor);

		    break;

		case 2:
		    // Fractal Dimension Imginary	Initial (FD_0)
		    rj = gj = bj = (dFDy_0 * 200.0 * dFactor);
		    break;

		case 3:
		    // (FD_0) Addition

		    rj = ((dFDx_0+dFDy_0) * 200 * dFactor);
		    gj = ((dFDx_0) * 200 * dFactor);
		    bj = ((dFDy_0) * 200 * dFactor);						

		    break;

		case 4:	

		    // (FD_0) Sum of magnitude
		    rj = ((dFDx_0*dFDx_0+dFDy_0*dFDy_0) * 200 * dFactor);
		    gj = ((dFDx_0*dFDx_0) * 200 * dFactor);
		    bj = ((dFDy_0*dFDy_0) * 200 * dFactor);

		    break;

		case 5:
		    // (FD_0) Multiplication
		    rj = (dFDx_0 * dFDy_0 * 200 * dFactor);
		    gj = (dFDx_0 * 200 * dFactor);
		    bj = (dFDx_0 * 200 * dFactor);

		    break;

		case 6:
		    // Fractal Dimension Real
		    rj = gj = bj = (int) (dFDx * 200.0 * dFactor);
		    break;

		case 7:
		    // Fractal Dimension Imaginary
		    rj  = gj = bj = (dFDy * 200.0 * dFactor);
		    break;

		case 8:
		    // FD Addition

		    //wsprintf (cstr, "step 1");
		    //MessageBox(NULL, cstr, "Debug", MB_OK);					

		    rj = ((dFDx+dFDy) * 200 * dFactor);
		    gj = ((dFDx) * 200 * dFactor);
		    bj = ((dFDy) * 200 * dFactor);
		    break;

		case 9:	
		    // FD Sum of magnitude
		    rj = ((dFDx*dFDx+dFDy*dFDy) * 200 * dFactor);
		    gj = ((dFDx*dFDx) * 200 * dFactor);
		    bj = ((dFDy*dFDy) * 200 * dFactor);
		    break;

		case 10:
		    // FD Multiplication
		    rj = (dFDx * dFDy * 200 * dFactor);
		    gj = (dFDx * 200 * dFactor);
		    bj = (dFDx * 200 * dFactor);
		    break;

		case 11:
		    rj = dFDx * 200.0 * dFactor;
		    gj = dFDy * 200.0 * dFactor;
		    bj = sqrt(rj * gj + rj * gj);
		    break;

		case 12:
		    rj = dFDx * 500.0 * dFactor;
		    gj = dFDy * 500.0 * dFactor;
		    bj = fabs(rj - gj);
		    break;

		case 13:
		    rj = dFDx * 500.0 * dFactor;
		    gj = dFDy * 500.0 * dFactor;
		    bj = (rj + gj)/2;
		    break;

		case 14:						
		    rj = fabs(t.x) * 2000.0 * dFactor;
		    gj = fabs(t.y) * 2000.0 * dFactor;
		    bj = (rj + gj)/2;
		    break;

		case 15:					
		    rj = fabs(t.x) * 2000.0 * dFactor;
		    gj = fabs(t.y) * 2000.0 * dFactor;
		    bj = fabs(rj - gj)*2;
		    break;

		case 16:
		    rj = fabs(cFDx.x) * 1000.0 * dFactor;
		    gj = fabs(cFDy.x) * 1000.0 * dFactor;
		    bj = sqrt(rj * gj + rj * gj);
		    break;

		case 17:
		    bj = dFDx * 1000.0 * dFactor;
		    gj = dFDy * 1000.0 * dFactor;
		    rj = gj;
		    break;

		case 18:
		    rj = fabs(t.x) * 2000.0 * dFactor;
		    gj = fabs(t.y) * 2000.0 * dFactor;
		    bj = rj;
		    break;

		case 19:
		    rj = fabs(cFDx.x) * 1000.0 * dFactor;
		    gj = fabs(cFDy.x) * 1000.0 * dFactor;
		    bj = rj;
		    break;

		case 20:
		    rj = fabs(cFDx.x)*fabs(cFDy.x) * 1000 * dFactor;
		    gj = fabs(cFDx.x) * 1000 * dFactor;
		    bj = fabs(cFDy.x) * 1000 * dFactor;
		    break;

		default:
//		    AfxMessageBox("FD Error");
		    break;
		}				

	    threshold = maxit_save;
	    return;

	    break;

	case 28:			// Standard deviation
	case 34:			// Strands Std
	case 36:
	    if (i < 2)
		{
		rj = gj = bj = i = 0;
		threshold = maxit_save;
		return;
		}

	    for (i2 = 0 ; i2<i ; i2++)
		{
			// Get sum x and y
		x_mean += pXTemp[i2];											
		y_mean += pYTemp[i2];

			// Get min x
		if (pXTemp[i2] < x_rmin)
		    x_rmin = pXTemp[i2];

			// Get max x
		if (pXTemp[i2] > x_rmax)
		    x_rmax = pXTemp[i2];

			// Get min y
		if (pYTemp[i2] < y_rmin)
		    y_rmin = pYTemp[i2];

			// Get max y
		if (pYTemp[i2] > y_rmax)
		    y_rmax = pYTemp[i2];
		}

	    if (i != 0)
		{
		x_mean = x_mean / (double)i;
		y_mean = y_mean / (double)i;
		}

	    x_std = 0;
	    y_std = 0;

	    for (i2 = 0 ; i2<i ; i2++)
		{
		x_std = x_std + (pXTemp[i2] - x_mean)*(pXTemp[i2] - x_mean); 
		y_std = y_std + (pYTemp[i2] - y_mean)*(pYTemp[i2] - y_mean); 
		}

	    if (i>1)
		{
		x_std = (x_std / i);
		y_std = (y_std / i);
		}
	    else
		{
		x_std = 0;
		y_std = 0;
		}				

	    if (x_std == 0 && y_std == 0)
		{
		jrw = 0;
		rj = gj = bj = 0;
		}
	    else
		{
		rj = (log((x_std)) * 30 * dFactor); // 10
		gj = (log((y_std)) * 30 * dFactor); // 10
		bj = (log((x_std + y_std)/2) * 30 * dFactor); // 10
		}

	    threshold = maxit_save;
	    return;
	}
    }

