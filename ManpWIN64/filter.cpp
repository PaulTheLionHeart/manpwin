/*
    filter.cpp - a module for implementing Tierazon filters

    By: stephen c. ferguson
    http://home1.gte.net/itriazon/
    email: itriazon@gte.net

    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include "manp.h"
#include "complex.h"
#include "filter.h"
#include "manpwin.h"
#include "colour.h"

/////////////////////////////////////////////////////
// Initialise thread-local Clouds density map
/////////////////////////////////////////////////////

void CTZfilter::InitCloudDensity(int xdots, int ydots)
    {
    const size_t pixelCount = (size_t)xdots * (size_t)ydots;
    ThreadCloudDensity.Init(xdots, ydots, gManp->hor, gManp->vert, gManp->mandel_width, gManp->AspectRatio);
    }

/////////////////////////////////////////////////////
// Release thread-local Clouds density map
/////////////////////////////////////////////////////

void CTZfilter::CloseCloudDensity()
    {
    ThreadCloudDensity.Close();
    }

/////////////////////////////////////////////////////
// Merge thread-local Clouds density map into final density map
/////////////////////////////////////////////////////

void CTZfilter::MergeCloudDensity(std::vector<unsigned int>& Destination) const
    {
    ThreadCloudDensity.AddDensityTo(Destination);
    }

/////////////////////////////////////////////////////
// Initialise filter variables
/////////////////////////////////////////////////////

void	CTZfilter::LoadFilterQ(Complex qIn)
    {
    q = qIn;
    limit = q.CFabs();
    n_color_x = 0;
    n_color_y = 0;
    n_color_z = 0;

    rj = gj = bj = 0;
    rr = 0.0;

    dt = 0;
    xtot = ytot = ztot = xavg = yavg = zavg = 0;
    zsav = xsav = ysav = d_color_x = d_color_y = d_color_z = 0.0;

    bPositiveX = FALSE;
    bPositiveY = FALSE;
    jrw = 0;
    z1 = 0.0;
    nFF = 1;
    i3 = 0;

    x_rmin = x_rmax = 0.0;
    y_rmin = y_rmax = 0.0;

    dStrands_HI = limit + dStrands;
    dStrands_LO = limit - dStrands;

    dStrands_df = (dStrands_HI - dStrands_LO) / 2;

    dStrands_HI_cx = fabs(q.x) + dStrands;
    dStrands_HI_cy = fabs(q.y) + dStrands;
    dStrands_LO_cx = fabs(q.x) - dStrands;
    dStrands_LO_cy = fabs(q.y) - dStrands;

    dStrands_cxd = dStrands_HI_cx - dStrands_LO_cx;
    dStrands_cyd = dStrands_HI_cy - dStrands_LO_cy;

    limit_cxd = dStrands_LO_cx + dStrands_cxd / 2;
    limit_cyd = dStrands_LO_cy + dStrands_cyd / 2;

    iteration_x = 0;
    iteration_y = 0;
    iteration_z = 0;

    for (int i = 0; i < MAXTEMP; i++)
	{
	pXTemp[i] = 0.0;
	pYTemp[i] = 0.0;
	}

    m_lower = 0;
    m_upper = 256;

    dFactor_2 = 1;
    dFactor = 1;

    if (method > TIERAZONFILTERS)
	FilterType = method - TIERAZONFILTERS;
    }

/////////////////////////////////////////////////////
// Initialise filter variables
/////////////////////////////////////////////////////

void	CTZfilter::InitFilter(int methodIn, long ThresholdIn, double dStrandsIn, int nFDOptionIn, BOOL UseCurrentPaletteIn)
    {
    method = methodIn;
    threshold = ThresholdIn;
    dStrands = dStrandsIn;
    nFDOption = nFDOptionIn;
    UseCurrentPalette = UseCurrentPaletteIn;

    if (method > TIERAZONFILTERS)
	FilterType = method - TIERAZONFILTERS;

    for (int i = 0; i < MAXTEMP; i++)
	{
	pXTemp[i] = 0.0;
	pYTemp[i] = 0.0;
	}
    dBailout = gManp->rqlim/* * gManp->rqlim*/;
    }

//////////////////////////////////////////////////////
// Filters
//////////////////////////////////////////////////////

int	CTZfilter::DoTierazonFilter(Complex z, long *iteration)
    {
    long    TempIndex = *iteration % MAXTEMP;	// don't overflow temp data storage

    limit = q.CFabs();
    dStrands_LO = limit - dStrands;
    dStrands_HI = limit + dStrands;
    switch (FilterType)
	{
	case 1:  // Stalks I			
	    d_real = fabs(z.x/z.y);
	    if (d_real < dStrands)
		{
		xtot += 30*pow((1-d_real/dStrands),*iteration);
		xsav = z.x;
		}

	    d_imag = fabs(z.y/z.x);
	    if (d_imag < dStrands)
		{
		ytot += 30*pow((1-d_imag/dStrands),*iteration);
		ysav = z.y;
		}

	    break;

	case 2:  // Bubbles I
	case 8:  // Bubbles II
	    temp = z.CSumSqr();
	    if (FilterType == 2)
		{
		if (temp < dStrands)
		    {
		    ztot += 5*pow((1-temp/dStrands),*iteration);
		    xsav = ysav = temp;
		    }
		}
	    else
		{
		if (temp < dStrands)
		    {
		    ztot += 10*pow((1-temp/dStrands),*iteration);
		    xsav = ysav = temp;
		    }
		}

	    d_real = fabs(z.x)+fabs(z.y);
	    if (d_real < dStrands)
		{
		xtot += 2*(1-d_real/dStrands);
		xsav = d_real;
		}

	    d_imag = fabs(z.y)+fabs(z.x);
	    if (d_imag < dStrands)
		{
		ytot += 2*(1-d_imag/dStrands);
		ysav = d_imag;
		}

	    break;

	case 3:
	    if (z.CSumSqr() >= MAXSIZE)  // 1e6
		return 0;

	    xtot = xtot + fabs(z.x);
	    xavg = xtot/ *iteration;
	    xdif = xavg - xsav;
	    xsav = xavg;
	    if (fabs(xdif) < dStrands)
		d_color_x = fabs(xdif);

	    ytot = ytot + fabs(z.y);
	    yavg = ytot/ *iteration;
	    ydif = yavg - ysav;
	    ysav = yavg;
	    if (fabs(ydif) < dStrands)
		d_color_y = fabs(ydif);
	    break;

	case 4:
	    if (z.CSumSqr() >= MAXSIZE)	// 1e6
		return 0;

	    ztot = ztot + z.CSumSqr();
	    zavg = ztot/ *iteration;
	    zdif = zavg - zsav;
	    zsav = zavg;
	    if (fabs(zdif) < dStrands)
		{
		d_color_z = fabs(zdif);
		xsav = z.x;
		ysav = z.y;
		}
	    break;

	case 5:  // log(log(...))
	    xsav = z.x;
	    ysav = z.y;		 
	    break;

	case 6:  // Bubbles I
	    temp = fabs(z.x*z.y) *2;
	    if (temp < dStrands)
		{
		ztot += 30*pow((1 - temp/dStrands),*iteration);
		xsav = z.x;
		ysav = z.y;
		iteration_z = *iteration;
		}
	    break;

	case 7:  // dual bubbles
	    temp = fabs(z.x*z.y) *2;
	    if (temp < dStrands)
		{
		ztot += 30*pow((1 - temp/dStrands),*iteration);
		xsav = z.x;
		ysav = z.y;
		iteration_z = *iteration;
		}

	    if (temp < dStrands/DUAL_BFACTOR)
		{
		ztot += 30*pow((1 - temp/dStrands),*iteration);
		xsav = z.x;
		ysav = z.y;
		}

	    break;

	case 9:  // Stalks II
	    d_real = fabs(z.x/z.y);
	    if (d_real < dStrands)
		{
		xtot += 100*pow((1-d_real/dStrands),*iteration);
		xsav = z.x;
		}

	    d_imag = fabs(z.y/z.x);
	    if (d_imag < dStrands)
		{
		ytot += 100*pow((1-d_imag/dStrands),*iteration);
		ysav = z.y;
		}
	    break;

	case 10:  // Rings I
	// Tierazon Filter 10 (Rings I) sets iteration to threshold
	// when it finds the ring to retain.  This is an early-exit
	// signal, not merely a colour value.  Stop processing the
	// orbit immediately; otherwise later iterations can overwrite
	// the selected ring and change the ring display order.
	    ssq1 = sqrt(z.CSumSqr());
//	    if (ssq1 < dStrands_HI && ssq1 > dStrands_LO && zsav < ssq1)
	    if (ssq1 < dStrands_HI && ssq1 > dStrands_LO)
		{
		zsav = ssq1;
		iteration_z = *iteration;
		xsav = z.x;
		ysav = z.y;		 
		*iteration = threshold;
		}
	    break;

	case 11:  // Rings II
	case 12:  // Rings III
	    ssq1 = 1/z.CSumSqr();
	    //if (ssq1 < dStrands_HI && ssq1 > dStrands_LO && zsav < ssq1)
	    if (ssq1 < dStrands_HI && ssq1 > dStrands_LO)
		{
		zsav = ssq1;
		iteration_z = *iteration;
		xsav = z.x;
		ysav = z.y;		 
		*iteration = threshold;
		}
	    break;

	case 13: // Hi Lo I
	    dzx = fabs(z.x);
	    dzy = fabs(z.y);

	    if (dzx < dStrands_HI_cx &&
		dzx > dStrands_LO_cx)
		{
		xtot += (dStrands_HI_cx - dzx)/dStrands_cxd;
		xsav = z.x;
		}

	    if (dzy < dStrands_HI_cy &&
		dzy > dStrands_LO_cy)
		{
		ytot += (dStrands_LO_cy - dzy)/dStrands_cyd;
		ysav = z.y;		 
		}

	    break;

	case 14:
	    ssq1 = (sqrt((z.CSumSqr())));
	    if (ssq1 < dStrands_HI && ssq1 > dStrands_LO && zsav == 0)
		{
		zsav = ssq1;
		iteration_z = *iteration;
		xsav = z.x;
		ysav = z.y;		 
		*iteration = threshold;
		}
	    break;

	    // case 15, 16 at the end (default)

	case 17:
	    if ((fabs(z.x)) < dStrands && xtot == 0)
		{
		xtot = 1;
		xsav += atan(fabs(z.x / z.y))*
		    atan(fabs(z.y / z.x));
		iteration_x = *iteration;
		}

	    if ((fabs(z.y)) < dStrands && ytot == 0)
		{
		ytot = 1;
		ysav += atan(fabs(z.x / z.y))*
		    atan(fabs(z.y / z.x));
		iteration_y = *iteration;
		}

	    break;

	case 18:
	    if (z.CSumSqr() < dStrands)
		{
		zsav += atan(fabs(z.x / z.y))*atan(fabs(z.y / z.x))*dFactor_2;
		xsav = z.x;
		ysav = z.y;		 
		iteration_z = *iteration;
		}
	    break;

	case 19:  //
	    ssq1 = z.CSumSqr();
	    if (ssq1 < dStrands)
		{
		ztot += atan(fabs(z.x / z.y))*
		    atan(fabs(z.y / z.x))
		    *(1+(1-(ssq1/dStrands)));
		xsav = z.x;
		ysav = z.y;		 
		iteration_z = *iteration;
		}

	    break;

	case 20:
	    ssq1 = sqrt(z.CSumSqr());
	    if (ssq1 < dStrands && ssq1 > zsav)
		{
		ztot += atan(fabs(z.x / z.y))*atan(fabs(z.y / z.x))*4*dFactor_2;
		zsav = ssq1;
		iteration_z = *iteration;
		xsav = z.x;
		ysav = z.y;		 
		}
	    break;

	case 21:  // Flowers I
	    ssq1 = z.CSumSqr();
	    if (ssq1 < dStrands && zsav == 0)
		//if (ssq1 < dStrands)
		{
		zsav = ssq1;
		ztot += atan(fabs(z.x / z.y))*atan(fabs(z.y / z.x))*10;

		dif_test = (ztot+(dStrands - zsav)*10*dFactor_2);  // 1300
		if (dif_test < m_lower || dif_test > m_upper)
		    {
		    ztot = 0;
		    zsav = 0;
		    iteration_z = 0;
		    }
		else
		    {
		    iteration_z=*iteration;
		    zsav = dif_test;
		    *iteration = threshold;
		    }

		xsav = z.x;
		ysav = z.y;		 

		}
	    break;

	case 22:  // Flowers II
	    ssq1 = z.CSumSqr();
	    if (ssq1 < dStrands && zsav == 0)
		{
		zsav = ssq1;
		iteration_z = *iteration;
		ztot += (atan(fabs(z.x / z.y))-atan(fabs(z.y / z.x)))*10;

		dif_test = ztot + (dStrands - zsav)*10*dFactor_2;
		if (dif_test < m_lower || dif_test > m_upper)
		    {
		    ztot = 0;
		    zsav = 0;
		    iteration_z = 0;
		    }
		else
		    {
		    iteration_z=*iteration;
		    zsav = dif_test;
		    *iteration = threshold;
		    }

		xsav = z.x;
		ysav = z.y;		 

		}
	    break;

	case 23:  // Flowers III
	    ssq1 = z.CSumSqr();
	    if (ssq1 < dStrands && zsav == 0)
		{
		zsav = ssq1;

		ztot += (atan(fabs(z.x / z.y)))*10;

		//dt = z.x * z.y;
		dif_test = ztot + (dStrands - zsav)*10*dFactor_2;
		if (dif_test < m_lower || dif_test > m_upper)
		    {
		    ztot = 0;
		    zsav = 0;
		    iteration_z = 0;
		    }
		else
		    {
		    iteration_z=*iteration;
		    zsav = dif_test;
		    *iteration = threshold;
		    }

		xsav = z.x;
		ysav = z.y;		 

		}
	    break;

	case 24:  // Flowers IV
	    ssq1 = z.CSumSqr();
	    if (ssq1 < dStrands && zsav == 0)
		{
		zsav = ssq1;
		iteration_z = *iteration;

		ztot += (sin(z.x * z.x*10)-
		    sin(z.y * z.y*10)*10);

		dif_test = ztot+(dStrands - zsav)*10*dFactor_2;  // 1300
		if (dif_test < m_lower || dif_test > m_upper)
		    {
		    ztot = 0;
		    zsav = 0;
		    iteration_z = 0;
		    }
		else
		    {
		    iteration_z=*iteration;
		    zsav = dif_test;
		    *iteration = threshold;
		    }

		xsav = z.x;
		ysav = z.y;		 

		}
	    break;

	case 25:  // Flowers V
	    ssq1 = z.CSumSqr();
	    if (ssq1 < dStrands && zsav == 0)
		{
		zsav = ssq1;
		iteration_z = *iteration;
		ztot += (sin(z.x * z.x*10)*sin(z.y * z.y*10))*10;
		dt = z.x * z.y;
		dif_test = ztot + (dStrands - zsav)*1000*dFactor_2;
		if (dif_test < m_lower || dif_test > m_upper)  // 40
		    {
		    ztot = 0;
		    zsav = 0;
		    iteration_z = 0;
		    xsav = z.x;
		    ysav = z.y;		 
		    }
		}
	    break;

	case 26:
	    ssq1 = sqrt(z.CSumSqr());
	    if (ssq1 < dStrands && zsav == 0)
		{
		ztot += atan(fabs(z.x / z.y))*atan(fabs(z.y / z.x))*4*dFactor_2;
		zsav = ssq1;
		iteration_z = *iteration;
		xsav = z.x;
		ysav = z.y;		 
		}

	    break;

	case 27:  // fractal dimensions		 
	case 28:  // standard deviations
	    pXTemp[TempIndex] = z.x*100;
	    pYTemp[TempIndex] = z.y*100;
	    xsav = z.x;
	    ysav = z.y;		 
	    break;

	case 29:  // Square Stalks & Bubbles			
	case 30:
	    temp = (fabs(z.x) + fabs(z.y));
	    if (temp < dStrands)
		{
		xtot = 10*(1-temp/dStrands);
		ytot = xtot;
		xsav = z.x;
		ysav = z.y;
		iteration_z = *iteration;
		}

	    break;

	case 31:
	case 32:
	    if (fabs(z.x) < dStrands)
		{
		b = (1 - fabs(z.x)/dStrands)*dFactor_2;
		b = b*.3;
		xtot = xtot + b - xtot*b;
		iteration_x++;
		xsav = z.x;
		}

	    if (fabs(z.y) < dStrands)
		{
		b = (1 - fabs(z.y)/dStrands)*dFactor_2;
		b = b*.3;
		ytot = ytot + b - ytot*b;
		iteration_y++;
		ysav = z.y;		 
		}

	    break;

	case 33:  // fractal dimensions of orbit traps			
	case 34:  // stalks standard deviation
	    if (fabs(z.x) < dStrands)
		{
		pXTemp[TempIndex] = (1 - fabs(z.x)/dStrands); // 100
		iteration_x++;
		xsav = z.x;
		}
	    else
		pXTemp[TempIndex] = 1; ////////////

	    if (fabs(z.y) < dStrands)
		{
		pYTemp[TempIndex] = (1 - fabs(z.y)/dStrands); // 100
		iteration_y++;
		ysav = z.y;		 
		}
	    else
		pYTemp[TempIndex] = 1; /////////

	    break;		

	case 35:  //  Hi/Lo stalks dimension
	case 36:
	    ssq1 = fabs(z.x);
	    if (ssq1 < dStrands_HI_cx && ssq1 > dStrands_LO_cx)
		{
		iteration_z++;				
		pXTemp[TempIndex] = 1 - (dStrands_HI_cx - ssq1)/(dStrands_HI_cx - dStrands_LO_cx)*100;
		xsav = z.x;
		}
	    else
		{
		pXTemp[TempIndex] = 1;
		}

	    ssq1 = fabs(z.y);
	    if (ssq1 < dStrands_HI_cy && ssq1 > dStrands_LO_cy)
		{
		iteration_z++;				
		pYTemp[TempIndex] = 1 - (dStrands_HI_cy - ssq1)/(dStrands_HI_cy - dStrands_LO_cy)*100;
		ysav = z.y;		 
		}
	    else
		{
		pYTemp[TempIndex] = 1;
		}

	    break;

	case 37:
	    if (fabs(z.x) < dStrands_HI_cx && 
		fabs(z.x) > dStrands_LO_cx)
		{
		iteration_z++;				
		pXTemp[TempIndex] = 1 - (dStrands_HI_cx - fabs(z.x))/(dStrands_cxd);
		xsav = z.x;
		}			
	    else
		{
		pXTemp[TempIndex] = 1;
		}

	    if (fabs(z.y) < dStrands_HI_cy &&
		fabs(z.y) > dStrands_LO_cy)
		{
		iteration_z++;				
		pYTemp[TempIndex] = 1 - (dStrands_HI_cy - fabs(z.y))/(dStrands_cyd);
		ysav = z.y;		 
		}
	    else
		{
		pYTemp[TempIndex] = 1;
		}

	    break;

	case 38:
	    pXTemp[TempIndex] = atan(fabs(z.x/z.y))*100;
	    pYTemp[TempIndex] = atan(fabs(z.y/z.x))*100;
	    xsav = z.x;
	    ysav = z.y;		 
 		
	    break;
		
	case 39:	// xray stalks
	    if (fabs(z.x) < dStrands)
		{
		xsav = fabs(z.x);
		temp = dStrands/xsav;
		xtot += log(temp);	
		}

	    if (fabs(z.y) < dStrands)
		{
		ysav = fabs(z.y);
		temp = dStrands/ysav;
		ytot += log(temp);	
		}

	    //xsav = z.x;
	    //ysav = z.y;
	    break;

	case 40:		// xray bubbles
	    if (z.CSumSqr() < dStrands)
		{
		zsav = z.CSumSqr();
		//iteration_z++;

		temp = log(2+dStrands/zsav);

		if (*iteration%2==0)
		    xtot += temp;
		else
		    ytot += temp;

		xsav = z.x;
		ysav = z.y;
		}     
	    break;

	case 41:  // xray rings
	    ssq1 = sqrt(z.CSumSqr());
	    if (ssq1 < dStrands_HI && ssq1 > dStrands_LO)
		{
		temp = log(2+dStrands_df/(fabs(limit-ssq1)));

		if (*iteration%2==0)
		    xtot += temp;
		else
		    ytot += temp;

		xsav = fabs(limit-ssq1)*2;
		ysav = fabs(limit-ssq1)*2;		 
		}

	    break;

	case 42:
	    dzx = fabs(z.x);
	    dzy = fabs(z.y);

	    if (dzx < dStrands_HI_cx &&
		dzx > dStrands_LO_cx)
		{
		xsav = fabs(limit_cxd-dzx);
		temp = log(10+limit_cxd/xsav);
		xtot += temp;
		xsav *= 2;
		}

	    if (dzy < dStrands_HI_cy &&
		dzy > dStrands_LO_cy)
		{
		ysav = fabs(limit_cyd-dzy);
		temp = log(10+limit_cyd/ysav);
		ytot += temp;
		ysav *= 2;
		}

	    break;

	case 43:
	    if (fabs(z.x) < dStrands)
		{
		xsav = fabs(z.x);
		xtot += atan(fabs(z.y/z.x)) * atan(fabs(z.x/z.y))*2;
		}

	    if (fabs(z.y) < dStrands)
		{
		ysav = fabs(z.y);
		ytot += atan(fabs(z.y/z.x)) * atan(fabs(z.x/z.y))*2;
		}

	    break;

	case 44:
	    if (  fabs(z.x) < dStrands || fabs(z.y) < dStrands)
		{
		if (fabs(z.x) < dStrands)
		    xtot += 100*(1 - fabs(z.x) /dStrands);

		if (fabs(z.y) < dStrands)
		    ytot += 100*(1 - fabs(z.y) /dStrands);

		if (xtot > threshold*dFactor_2 || ytot > threshold*dFactor_2)
		    *iteration = threshold;

		xsav = z.x;
		ysav = z.y;		 
		}

	    break;

	case 45:
	    if (  fabs(z.x) * fabs(z.y) < dStrands)
		{
		ztot += 100*(1 - (fabs(z.x) * fabs(z.y)/dStrands));

		if (ztot > threshold*dFactor_2)
		    *iteration = threshold;

		xsav = z.x;
		ysav = z.y;		 
		}

	    break;

	case 46:
	    if (z.CSumSqr() < dStrands)
		{
		ztot += 30*(30*dFactor_2/(*iteration))*(1 - (z.CSumSqr()/dStrands));

		iteration_z = *iteration;
		if (ztot > threshold*10)
		    *iteration = threshold;

		xsav = z.x;
		ysav = z.y;		 
		}

	    break;

	case 47:
	    temp = z.CSumSqr();
	    if (temp < dStrands && *iteration > 0)
		{
		ztot += 10*dFactor_2*(100/(*iteration))*(1 - (temp/dStrands));

		iteration_z = *iteration;
		if (ztot > threshold*10)
		    *iteration = threshold;

		xsav = z.x;
		ysav = z.y;		 
		}

	    break;

	case 48:
	    ztot = z.CSumSqr();
	    if (ztot < dStrands)
		{
		pYTemp[TempIndex] = pXTemp[TempIndex] = (dStrands/ztot)*100;
		iteration_z = *iteration;
		}
	    else
		{
		pXTemp[TempIndex] = 0;
		pYTemp[TempIndex] = 0;
		}
	    xsav = z.x;
	    ysav = z.y;		 

	    break; 		

	case 49:
	    ztot = z.CSumSqr();
	    if (ztot < dStrands)
		{
		pYTemp[TempIndex] = pXTemp[TempIndex] = log(dStrands*100/ztot)*100000;
		iteration_z = *iteration;
		}
	    else
		{
		pXTemp[TempIndex] = 0;
		pYTemp[TempIndex] = 0;
		}
	    xsav = z.x;
	    ysav = z.y;		 

	    break; 		

	case 50:
	    d_real = fabs(z.x/z.y);
	    d_imag = fabs(z.y/z.x);

	    if (d_real <= dStrands)
		{
		xtot += (1-d_real/dStrands);
		//xsav = z.x;
		xsav = d_real;
		}

	    if (d_imag < dStrands)
		{
		ytot += (1-d_imag/dStrands);
		//ysav = z.y;		 
		ysav = d_imag;
		}

	    break;

	case 51:  // Stalks & Bubbles			
	case 52:
	    if (fabs(z.x) < dStrands && *iteration > m_lower && *iteration < m_upper)
		{
		xsav = z.x;
		iteration_x = *iteration;
		//iteration_x++;
		}

	    if (fabs(z.y) < dStrands && *iteration > m_lower && *iteration < m_upper)
		{
		ysav = z.y;
		iteration_y = *iteration;
		//iteration_y++;
		}

	    if (z.CSumSqr() < dStrands && *iteration > m_lower && *iteration < m_upper)
		{
		zsav = z.CSumSqr();
		//iteration_z = iteration;
		iteration_z++;
		ysav = z.y;
		xsav = z.x;
		}     
	    break;

	case 53:    // Flarium 07, i+=(int)(sin(fabs(dzx_save)*fabs(dzy_save))*10)
	case 54:    // Flarium 08, i+=(int)(sin(fabs(dzx_save)/fabs(dzy_save))*10)
	    xsav += z.x;
	    ysav += z.y;
	    break;

	case 55:    // Flarium 13, i=(int)((fabs(atan(z.real()/z.imag()))+fabs(atan(dzx_save/dzy_save)))*100)",
	    xsav = z.x;
	    ysav = z.y;
	    break;

	case 56: // Flarium 16, Delta Slope, On Change;, ...J+=FF; plus atan method
	// Deliberate fall-through into Flarium 17.
	case 57: // Flarium 17, Delta Slope, On Change;, ...J+=FF
	    if (xsav == 0.0)
		{
		bPositiveX = FALSE;
		bPositiveY = FALSE;
		}

	    if (fabs(z.x) >= xsav)
		{
		// Count positive X slope change.
		if (!bPositiveX)
		    {
		    jrw += nFF;
		    bPositiveX = TRUE;
		    }
		}
	    else
		{
		// Count negative X slope change.
		if (bPositiveX)
		    {
		    jrw += nFF;
		    bPositiveX = FALSE;
		    }
		}

	    if (fabs(z.y) >= ysav)
		{
		// Count positive Y slope change.
		if (!bPositiveY)
		    {
		    jrw += nFF;
		    bPositiveY = TRUE;
		    }
		}
	    else
		{
		// Count negative Y slope change.
		if (bPositiveY)
		    {
		    jrw += nFF;
		    bPositiveY = FALSE;
		    }
		}

	    xsav = fabs(z.x);
	    ysav = fabs(z.y);
	    break;

	case 58: // Flarium 18, Delta Slope, No Change;, ...J+=FF
	    if (xsav == 0.0)
		{
		bPositiveX = FALSE;
		bPositiveY = FALSE;

		z1 = z;

		// Flarium used dzx_save = 99 merely as an
		// initialisation sentinel.
		xsav = 99.0;

		jrw = threshold / 4;
		break;
		}

	    if (fabs(z.x) >= fabs(z1.x) ||
		fabs(z.y) >= fabs(z1.y))
		{
		// Positive slope, no direction change.
		if (!bPositiveX)
		    {
		    bPositiveX = TRUE;

		    if (jrw - nFF > 0)
			jrw -= nFF;
		    }
		else
		    {
		    if (jrw + nFF * 2 < threshold)
			jrw += nFF * 2;
		    }
		}
	    else
		{
		// Negative slope, no direction change.
		if (bPositiveX)
		    {
		    bPositiveX = FALSE;

		    if (jrw - nFF > 0)
			jrw -= nFF;
		    }
		else
		    {
		    if (jrw + nFF * 2 < threshold)
			jrw += nFF * 2;
		    }
		}

	    z1 = z;
	    break;

	case 59:    // Flarium 30, (atan(fabs(x_rmax * x_rmin)/fabs(y_rmax * y_rmin)) * 40)
	    if (*iteration <= 1)
		{
		x_rmin = z.x;
		x_rmax = z.x;
		y_rmin = z.y;
		y_rmax = z.y;
		}
	    else
		{
		if (x_rmin > z.x)
		    x_rmin = z.x;
		if (x_rmax < z.x)
		    x_rmax = z.x;

		if (y_rmin > z.y)
		    y_rmin = z.y;
		if (y_rmax < z.y)
		    y_rmax = z.y;
		}
	    break;

	case 60:    // Flarium 31, if (fabs(dzx) <= dStrands || fabs(dzy) <= dStrands) rr=ri
	    if ((fabs(z.x) <= dStrands_HI && fabs(z.x) > dStrands_LO) ||
		(fabs(z.y) <= dStrands_HI && fabs(z.y) > dStrands_LO))
		{
		rr += log(z.x * z.x + z.y * z.y) * 13.0;   // Don't mess with this :-)
		}

	    if (fabs(z.x) < dStrands || fabs(z.y) < dStrands)
		{
		i3++;
		}
	    break;

	case 61:    // Flarium 32, if (dzx*dzx+dzy*dzy < limit) rr+=3
	    if (z.x * z.x + z.y * z.y < limit)
		i3++;
	    break;

	case 62:    // Flarium 33, Combination Filters 1,2, && 3
	    {
	    double dx = z.x;
	    double dy = z.y;

	    if (dx < dStrands && dx > -dStrands)
		{
		if (dx < 0.0)
		    dx = -dStrands;
		else
		    dx = dStrands;
		}

	    if (dy < dStrands && dy > -dStrands)
		{
		if (dy < 0.0)
		    dy = -dStrands;
		else
		    dy = dStrands;
		}

	    xsav += log(fabs(dx));
	    ysav += log(fabs(dy));

	    if ((fabs(dx) <= dStrands_HI && fabs(dx) > dStrands_LO) ||
		(fabs(dy) <= dStrands_HI && fabs(dy) > dStrands_LO))
		{
		rr += log(dx * dx + dy * dy) * 13.0;       // Don't mess with this either :-)
		}

	    if (dx * dx + dy * dy < limit)
		i3++;
	    }
	    break;

	case 63:    // Flarium 34, if (fabs(dzx) > dStrands || fabs(dzy) > dStrands)",
	    rr += log(z.x * z.x + z.y * z.y) * 10.0;

	    if (fabs(z.x) > dStrands || fabs(z.y) > dStrands)
		rr += nFF;
	    break;

	case 64:    // Flarium 35, if (fabs(dzx) > dStrands &&&& fabs(dzy) > dStrands)
	    if (fabs(z.x) < limit || fabs(z.y) < limit)
		{
		rr += log(z.x * z.x + z.y * z.y) * 10.0;
		}
	    else
		{
		*iteration += nFF;
		}
	    break;

	case 65:    // Flarium 36, if (fabs(dzx) > limit || fabs(dzy) > limit)",
	    rr += log(z.x * z.x + z.y * z.y) * 10.0;

	    if (fabs(z.x) > limit || fabs(z.y) > limit)
		*iteration += nFF;
	    break;

	case 66:    // Flarium 37, if (fabs(dzx) < limit || fabs(dzy) < limit)",
	    if (fabs(z.x) < limit || fabs(z.y) < limit)
		rr += log(z.x * z.x + z.y * z.y) * 10.0;
	    else
		*iteration += nFF;
	    break;

	case 67:    // Flarium 38, if (fabs(1/dzx*dzy) > limit)",
	    rr += log(z.x * z.x + z.y * z.y) * 10.0;

	    if (fabs((1.0 / z.x) * z.y) > limit)
		*iteration += nFF;
	    break;

	case 68:    // Flarium 39, if (fabs(dzx) > limit || fabs(dzy) > limit)
	    rr += log(z.x * z.x + z.y * z.y) * 10.0;

	    if (fabs(z.x) > limit || fabs(z.y) > limit)
		*iteration += nFF;

	    xsav = z.x;
	    ysav = z.y;
	    break;

	case 69:    // Flarium 40, ri = rr + atan(fabs(dzx_save/dzy_save)) * 30
	    rr += log(z.x * z.x + z.y * z.y) * 2.0;

	    if (fabs(z.x) > limit || fabs(z.y) > limit)
		*iteration += nFF;

	    xsav = z.x;
	    ysav = z.y;
	    break;

	case 70:    // Flarium 41, rr+=log(dzx*dzx+dzy*dzy)*nBay100
	    rr += log(z.x * z.x + z.y * z.y) * dFactor;
	    xsav = z.x;
	    ysav = z.y;
	    break;

	case 71:    // Flarium 46, i = (int)rr; rr+=log(dzx*dzx+dzy*dzy)*(1+nBay100)
	    rr += log(z.x * z.x + z.y * z.y) * (1 + dFactor);
	    break;

	case 72:    // Flarium 47, i = (int)(rr + atan(fabs(dzx_save/dzy_save)) * (10+nBay1000))",
	    rr += log(z.x*z.x + z.y*z.y) * (1.0 + dFactor);
	    xsav = z.x;
	    ysav = z.y;
	    break;

	case 73:    // Flarium 48, if (fabs(dzx) < z.squares() || fabs(dzy) < z.squares())  rr+=nFF",
	    {
	    double mag = sqrt(z.x * z.x + z.y * z.y);

	    if (fabs(z.x) < mag || fabs(z.y) < mag)
		rr += dFactor;
	    }
	    break;

	case 74:    // Flarium 49, rr += atan(fabs(dzy/dzx))*atan(fabs(dzx/dzy))*2",
	    if (fabs(z.x) < dStrands || fabs(z.y) < dStrands)
		rr += atan(fabs(z.y / z.x)) * atan(fabs(z.x / z.y)) * 2.0;
	    break;





	case 75:	// a special filter based on Flarium clouds
	    ThreadCloudDensity.AddPoint(z.x, z.y);
	    break;

	default:  // last z values (gradients)
	    // Case 15, 16, 17
	    xsav = z.x;
	    ysav = z.y;		 
	    break;
	}
    return 0;
    }

//////////////////////////////////////////////////////////
// Once per pixel  filter completion
//////////////////////////////////////////////////////////

int	CTZfilter::EndTierazonFilter(Complex z, long *iteration, CTrueCol *TrueCol)
    {
//    static  int nCount = 0;

    rj = gj = bj = j = *iteration;

    switch (FilterType)
	{
	case 1:
	case 9:
	case 13:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	    if (xsav || ysav)
		{      
		xtot*=dFactor;
		ytot*=dFactor;
		d = (int)sqrt(xtot*xtot + ytot*ytot);

		rj = d + xtot;
		gj = d + ytot;			
		bj = d;

		}
	    else
		rj = gj = bj = j = 0;
	    break;

	case 2:
//	case 8:
	    if (ztot)
		{
		rj = gj = bj = ztot*dFactor;
		}
	    else
		{
		j = 0;
		rj = gj = bj = 0;
		}

	    if (xtot || ytot)
		{      
		d = (int)sqrt(xtot*xtot + ytot*ytot);
		gj = d + xtot*dFactor;
		bj = d + ytot*dFactor;
		}	

	    break;

	case 3:
	    if (d_color_x || d_color_y)
		{
		if (d_color_x)
		    rj = xtot = (dStrands - d_color_x)*1000*dFactor;

		if (d_color_y)
		    gj = ytot = (dStrands - d_color_y)*1000*dFactor;

		*iteration = *iteration + (int)(xtot + ytot)/2;
		bj = *iteration;
		}
	    else
		{
		j = *iteration = 0;
		rj = gj = bj = 0;
		}

	    break;

	case 4:  // Moving average II
	    if (d_color_z)
		{
		ztot = (dStrands - d_color_z)*1000*dFactor;

		*iteration = *iteration + (int)ztot;
		}
	    else
		{
		j = *iteration = 0;
		rj = gj = bj = 0;
		}

	    rj = gj = bj = *iteration;
	    break;

	case 5:
	    if (*iteration<threshold)
		{
		if (z.CSumSqr() > 1)
		    *iteration = (int)((*iteration - log (log (z.CSumSqr())) / log (2.0))*50*dFactor);
		}
	    else
		{
		j = *iteration = 0;
		rj = gj = bj = 0;
		}

	    if (*iteration < 0)
		*iteration = 0;
	    rj = gj = bj = *iteration;
	    break;

	case 6:			
	    rj = ztot*dFactor;
	    gj = rj+xsav*10*dFactor+iteration_z;
	    bj = rj+ysav*10*dFactor+iteration_z;

	    break;

	case 7:
	    rj = ztot*dFactor;
	    gj = rj+xsav*10*dFactor+iteration_z;
	    bj = rj+ysav*10*dFactor+iteration_z;


	    break;

	case 8:
	    if (ztot)
		{
		rj = gj = bj = ztot * dFactor * 2.0;
		}
	    else
		{
		j = 0;
		rj = gj = bj = 0;
		}

	    if (xtot || ytot)
		{
		d = (int)sqrt(xtot * xtot + ytot * ytot);
		gj = d + xtot * dFactor;
		bj = d + ytot * dFactor;
		}

	    break;

	case 10:   // Rings I
	    //if (iteration_z > 2)
	    if (iteration_z)
		{
		ztot = fabs(limit - zsav)*3000*dFactor;  // 500
		*iteration = (int)ztot;
		rj = gj = bj = *iteration;
		}
	    else
		{
		if (*iteration < threshold)
		    {
		    j = *iteration = 0;
		    iteration_z = -1;
		    rj = gj = bj = 0;
		    }
		}
	    break;

	case 11:   // Rings II
	    //if (iteration_z > 2)
	    if (iteration_z)
		{
		ztot = fabs(limit - zsav)*3000*dFactor;  // 500
//		if (iteration_z%2==0)
		    rj = bj = gj = *iteration = (int)ztot;
//		else
//		    rj = bj = gj = iteration = (int)ztot + OFFSET_COLOR;
		}
	    else
		{
		if (*iteration < threshold)
		    {
		    j = *iteration = 0;
		    iteration_z = -1;
		    rj = gj = bj = 0;
		    }
		}
	    break;

	case 12:  // Rings III
	    //if (iteration_z >= 1)
	    if (iteration_z)
		{
		ztot = fabs(limit - zsav)*3000*dFactor;  // 500
		if (iteration_z%2==0)
		    {
		    rj = *iteration = (int)ztot;
		    }
		else
		    {			
		    gj = *iteration = (int)ztot + OFFSET_COLOR;
		    }
		}
	    else
		{
		if (*iteration < threshold)
		    {
		    j = *iteration = 0;
		    iteration_z = -1;
		    rj = gj = bj = *iteration;
		    }
		}

	    bj = rj + gj;

	    break;

	case 14:   // Rings I
	    if (iteration_z)
		{
		//ztot = (fabs(dStrands_HI - zsav))*50*dFactor;
		ztot = (((dStrands_HI - zsav)*.5)/(dStrands_HI- dStrands_LO))*500*dFactor;
		rj = ztot * xsav;
		gj = ztot * ysav;
		bj = (rj + gj)/2;
		}
	    else
		{
		if (*iteration < threshold)
		    {
		    j = *iteration = 0;
		    iteration_z = -1;
		    }
		rj = gj = bj = *iteration;
		}
	    break;

	case 15:
	    if (*iteration >= threshold)
		{
		*iteration = (int)(fabs(sin(z.x * z.x*100))*
		    fabs(sin(z.y * z.y*100))*
		    200*dFactor);
		}
	    else
		{
		ssq1 = z.CSumSqr();
		if (ssq1 <= 2)
		    ssq1 = 2;
		*iteration = (int)((*iteration - log (log (ssq1)) / log (2.0))*10*dFactor_2);
		}
	    if (*iteration > threshold)
		*iteration = threshold;
	    if (*iteration < 0)
		*iteration = 0;
	    rj = gj = bj = *iteration;
	    break;

	case 16:
	    if (*iteration >= threshold)
		{
		*iteration = (int)(fabs(sin(z.x * z.x*100))*fabs(sin(z.y * z.y*100))*
		    200*dFactor);
		rj = gj = bj = *iteration;
		}
	    else
		{
		ztot = atan(fabs(z.x / z.y))-
		    atan(fabs(z.y / z.x))*
		    60*dFactor_2;

		if (*iteration%2==0)
		    rj = *iteration = (int)ztot;
		else
		    gj = *iteration = (int)ztot + OFFSET_COLOR;

		bj = rj + gj + *iteration;
		}
	    if (*iteration > threshold)
		*iteration = threshold;
	    if (*iteration < 0)
		*iteration = 0;
	    break;

	case 17:
	    if (iteration_x || iteration_y)
		{
		if (iteration_x)
		    xtot = xsav*200*dFactor;

		if (iteration_y)
		    ytot = ysav*200*dFactor;

		if (*iteration < threshold)
		    rj = *iteration = (int)sqrt(xtot*xtot+ytot*ytot);
		else
		    gj = *iteration = *iteration + (int)sqrt(xtot*xtot+ytot*ytot);

		bj = rj + gj +* iteration;
		}
	    else
		{
		if (*iteration < threshold)
		    {
		    j =* iteration = 0;
		    iteration_z = -1;
		    }
		rj = gj = bj = *iteration;
		}

	    if (*iteration > threshold)
		*iteration = threshold;
	    if (*iteration < 0)
		*iteration = 0;
	    break;

	case 18:
	    iteration_z += (int)(zsav*3*dFactor);
	    rj = gj = bj = iteration_z;
	    break;

	case 19:
	    if (iteration_z)
		{
		if (iteration_z%2==0)
		    {
		    rj = ztot*2*dFactor;
		    gj = ztot*2*dFactor + OFFSET_COLOR;
		    }
		else
		    {
		    rj = ztot*2*dFactor + OFFSET_COLOR;
		    gj = ztot*2*dFactor;
		    }

		bj = (rj + gj)/2;
		}
	    else
		{
		if (*iteration < threshold)
		    {
		    j = *iteration = 0;
		    iteration_z = -1;
		    }
		rj = gj = bj = *iteration;
		}
	    break;

	case 20:
	    if (iteration_z && *iteration > 1)
		{
		ztot += (dStrands - zsav)*1000*dFactor;
		if (iteration_z%2==0)
		    rj = *iteration = (int)(iteration_z*2 + ztot)/2;
		else
		    gj = *iteration = (int)(iteration_z*2 + ztot)/2 + OFFSET_COLOR;
		bj = rj + gj + *iteration;
		}
	    else
		{
		if (*iteration < threshold)
		    {
		    j = *iteration = 0;
		    iteration_z = -1;
		    }
		rj = gj = bj = *iteration;
		}

	    break;

	case 21:   // Flowers I
	case 22:   // Flowers II
	case 23:   // Flowers III
	case 24:   // Flowers IV
	case 25:   // Flowers V
	    if (*iteration < threshold && iteration_z == 0)
		{
		j = *iteration = 0;
		iteration_z = -1;  // sets the background to black
		rj = gj = bj = *iteration;
		}

	    if (iteration_z == 0)
		{
		if (*iteration < threshold)
		    {
		    j = *iteration = 0;
		    iteration_z = -1;
		    }
		rj = gj = bj = *iteration;
		}

	    if (iteration_z > 0)
		{
		//ztot += iteration_z
		//+ (1 - zsav/dStrands)*1000*dFactor;  // 1300

		if (iteration_z%2 == 0)
		    ztot = dif_test*2*dFactor;
		else
		    ztot = dif_test*2*dFactor + OFFSET_COLOR;

		rj = ztot;
		gj = ztot + OFFSET_COLOR;
		bj = ztot + OFFSET_COLOR + OFFSET_COLOR;

		}
	    break;

	case 26:  
	    if (iteration_z && *iteration > 1)
		{
		if (iteration_z%2==0)
		    rj = *iteration = iteration_z + (int)(ztot*10*dFactor);
		else
		    gj = *iteration = iteration_z + (int)(ztot*10*dFactor) + OFFSET_COLOR;
		bj = rj + gj + *iteration;
		}
	    else
		{
		if (*iteration < threshold)
		    {
		    j = *iteration = 0;
		    iteration_z = -1;
		    }
		rj = gj = bj = *iteration;
		}
	    break;

	case 27:  // Fractal Dimension
	    break;

	case 29:  
	    xtot *= dFactor;
	    ytot *= dFactor;

	    rj = (xtot*xtot+ytot*ytot);
	    gj = rj;
	    bj = rj;

	    break;

	case 30:
	    xtot *= dFactor;
	    ytot *= dFactor;

	    rj = xtot*xtot;
	    gj = (xtot*xtot+ytot*ytot);
	    bj = ytot*ytot;

	    break;

	case 31:
	    rj = gj = bj = 0;
	    iteration_x = (int)(xtot*1000*dFactor);
	    iteration_y = (int)(ytot*1000*dFactor);

	    if (iteration_x || iteration_y)
		{
		if (iteration_x > iteration_y)
		    {
		    if (iteration_x < 256)
			{
			rj = iteration_x;
			}
		    else
			if (iteration_x >= 256 && iteration_x < 512)
			    {
			    rj = 255;
			    gj = iteration_x - 256;
			    }
			else
			    if (iteration_x >= 512 && iteration_x < 768)
				{
				rj = gj = 255;
				bj = iteration_x - 512;
				}
			    else
				if (iteration_x >= 768)
				    {
				    rj = gj = bj = 255;
				    }
		    }
		else
		    {
		    if (iteration_y < 256)
			{
			bj = iteration_y;
			}
		    else
			if (iteration_y >= 256 && iteration_y < 512)
			    {
			    bj = 255;
			    rj = iteration_y - 256;
			    }
			else
			    if (iteration_y >= 512 && iteration_y < 768)
				{
				bj = rj = 255;
				gj = iteration_y - 512;
				}
			    else
				if (iteration_y >= 768)
				    {
				    rj = gj = bj = 255;
				    }
		    }
		}
	    else
		j = 0;

	    break;		

	case 32:
	    rj = gj = bj = 0;

	    if (iteration_x || iteration_y)
		{
		if (iteration_x)
		    xtot = iteration_x + xtot*100*dFactor;

		if (iteration_y)
		    ytot = iteration_y + ytot*100*dFactor;

		d = (int)(xtot + ytot)/2;

		rj = d + xtot;
		gj = d + ytot;			
		bj = d;
		}
	    else
		j = 0;

	    break;		

	case 33:  // FD Strands
	case 34:
	    if (iteration_x || iteration_y)
		j = *iteration;
	    else
		{
		j = *iteration = 0;
		rj = gj = bj = 0;
		}

	    break;

	case 35:
	case 36:
	    if (iteration_z)
		j = *iteration;
	    else
		{
		j = *iteration = 0;
		rj = gj = bj = 0;
		}
	    break;

	case 37:
	    if (iteration_z)
		j = *iteration;
	    else
		{
		j = *iteration = 0;
		rj = gj = bj = 0;
		}
	    break;

	case 38:
	    j = *iteration;
	    break;

	case 45:
	    xtot = ztot;
	    ytot = ztot;

	case 44:  // diamonds
	    if (xtot || ytot )
		{      
		rj = xtot*dFactor;
		gj = ytot*dFactor;			
		bj = (rj + gj)/2;
		}
	    else
		rj = gj = bj = 0;

	    break;

	case 46:		
	case 47:
	    xtot = ztot;
	    ytot = ztot;
	    if (xtot || ytot)
		{      
		if (iteration_z%2 == 0)
		    {
		    rj = xtot*dFactor;
		    gj = ytot*dFactor;			
		    bj = (rj + gj)/2;
		    }
		else
		    {
		    gj = xtot*dFactor+OFFSET_COLOR;
		    bj = ytot*dFactor+OFFSET_COLOR;			
		    rj = (bj + gj)/2;
		    }
		}
	    else
		rj = gj = bj = 0;

	    break;

	case 48:
	case 49:
	    if (iteration_z)
		j = *iteration;
	    else
		{
		j = *iteration = 0;
		rj = gj = bj = 0;
		}
	    break;

	case 50:

	    if (xtot || ytot)
		{      
		rj = xtot*dFactor*5;  // *5
		gj = ytot*dFactor*5;	// *5		
		//bj = sqrt(rj*rj + gj*gj);
		bj = (rj + gj)/2;
		}
	    else
		rj = gj = bj = 0;

	    break;

	case 51:  
	    if (iteration_x || iteration_y)
		{      
		if (iteration_x)
		    //xtot = (dStrands - fabs(xsav))*1000*dFactor;
		    xtot = (1 - fabs(xsav)/dStrands)*100*dFactor;

		if (iteration_y)
		    //ytot = (dStrands - fabs(ysav))*1000*dFactor;
		    ytot = (1 - fabs(ysav)/dStrands)*100*dFactor;

		d = (int)((double) *iteration + (xtot + ytot)/2);

		rj = d + (xtot)/2;
		gj = d + (ytot)/2;			
		bj = d;
		}
	    else
		{
		j = *iteration = 0;
		rj = gj = bj = 0;
		}

	    if (iteration_z)
		{
		ztot = (1 - zsav/dStrands)*100*dFactor;
		rj += ztot;
		gj += ztot;
		bj += ztot;
		}

	    std::swap(rj, bj);
	    break;

	case 52:
	    if (iteration_x || iteration_y)
		{      
		if (iteration_x)
		    //xtot = (dStrands - fabs(xsav))*1000*dFactor;
		    xtot = (1 - fabs(xsav)/dStrands)*100*dFactor;

		if (iteration_y)
		    //ytot = (dStrands - fabs(ysav))*1000*dFactor;
		    ytot = (1 - fabs(ysav)/dStrands)*100*dFactor;

		//d = (double) (iteration_y + iteration_x) + (xtot + ytot)/2;

		rj = xtot;
		bj = ytot;			

		if (rj > bj)
		    gj = rj;
		else
		    gj = bj;

		}

	    if (iteration_z)
		{
		//ztot = (dStrands - zsav)*1000*dFactor;
		ztot = (1 - zsav/dStrands)*100*dFactor;
		rj += ztot;
		gj += ztot;
		bj += ztot;
		}

	    if (iteration_x || iteration_y)
		{
		if (iteration_x > iteration_y)
		    {
		    if (iteration_x < 256)
			{
			rj += iteration_x;
			}
		    else
			if (iteration_x >= 256 && iteration_x < 512)
			    {
			    rj = 255;
			    gj += iteration_x - 256;
			    }
			else
			    if (iteration_x >= 512 && iteration_x < 768)
				{
				rj = gj = 255;
				bj += iteration_x - 512;
				}
		    }
		else
		    {
		    if (iteration_y < 256)
			{
			bj += iteration_y;
			}
		    else
			if (iteration_y >= 256 && iteration_y < 512)
			    {
			    bj = 255;
			    rj += iteration_y - 256;
			    }
			else
			    if (iteration_y >= 512 && iteration_y < 768)
				{
				bj = rj = 255;
				gj += iteration_y - 512;
				}
		    }
		}
	    else
		{
		j = 0;
		rj = gj = bj = 0;
		}

	    std::swap(rj, bj);
	    break;

    	case 53:    // Flarium 07, i+=(int)(sin(fabs(dzx_save)*fabs(dzy_save))*10)
	    {
	    j += (int)(sin(fabs(xsav) * fabs(ysav)) * 10.0);
	    rj += sin(fabs(xsav) * fabs(ysav)) * 10.0;

	    gj += sin(fabs(xsav)) * 10.0;
	    bj += sin(fabs(ysav)) * 10.0;
	    }
	    break;
	case 54:    // Flarium 08, i+=(int)(sin(fabs(dzx_save)/fabs(dzy_save))*10)
	    {
	    double ratio = fabs(xsav) / fabs(ysav);

	    j += (int)(sin(ratio) * 10.0);
	    rj += sin(ratio) * 10.0;

	    gj += cos(ratio) * 10.0;
	    bj += acos(ratio) * 10.0;
	    }
	    break;

	case 55:    // Flarium 13, i=(int)((fabs(atan(z.real()/z.imag()))+fabs(atan(dzx_save/dzy_save)))*100)",
	    {
	    double a = fabs(atan(z.x / z.y));
	    double b = fabs(atan(xsav / ysav));

	    j = (int)((a + b) * 100.0);
	    rj = (a + b) * 100.0;

	    gj = fabs(a * a) * 500.0 * dFactor_2;
	    bj = fabs(b * b) * 500.0 * dFactor_2;
	    }
	    break;

	case 56:    // Flarium 16, Delta Slope, On Change;, ...J+=FF; plus atan method
	    {
	    j += jrw;
	    j = abs(j);

	    rj += jrw;
	    rj = fabs(rj);

	    double dzxLocal = fabs(atan(z.x / z.y));
	    double dzyLocal = fabs(atan(xsav / ysav));

	    j += (int)((dzxLocal + dzyLocal) * 100.0);

	    rj += (dzxLocal + dzyLocal) * 100.0;
	    gj += fabs(dzxLocal) * 200.0;
	    bj += fabs(dzyLocal) * 200.0;
	    }
	    break;

	case 57:    // Flarium 17, Delta Slope, On Change;, ...J+=FF
	    j += jrw;
	    j = abs(j);

	    rj += jrw;
	    rj = fabs(rj);

	    gj = bj = rj;
	    break;

	case 58:    // Flarium 18, Delta Slope, No Change;, ...J+=FF
	    j += jrw;
	    j = abs(j);

	    rj += jrw;
	    rj = fabs(rj);

	    gj = bj = rj;
	    break;

	case 59:    // Flarium 30, (atan(fabs(x_rmax * x_rmin)/fabs(y_rmax * y_rmin)) * 40)
	    {
	    double denom = fabs(y_rmax * y_rmin);

	    if (denom != 0.0)
		{
		double value =
		    atan(fabs(x_rmax * x_rmin) / denom) * 40.0;

		j += (int)value;
		rj += value;
		}

	    gj = bj = rj;
	    }
	    break;

	case 60:    // Flarium 31, if (fabs(dzx) <= dStrands || fabs(dzy) <= dStrands) rr=ri
	    if (j >= threshold - 1)
		j += i3;
	    else
		j += (int)rr;

	    if (rj >= threshold - 1)
		rj += i3;
	    else
		rj += rr;

	    gj = bj = rj;
	    break;

	case 61:    // Flarium 32, if (dzx*dzx+dzy*dzy < limit) rr+=3
	    j += i3;
	    rj += i3;
	    gj = bj = rj;
	    break;

	case 62:    // Flarium 33, Combination Filters 1,2, && 3
	    if (rr != 0.0 && i3 != 0)
		{
		j += (int)(fabs(ysav - xsav) + fabs(rr + i3));
		rj += fabs(ysav - xsav) + fabs(rr + i3);
		}
	    else if (rr != 0.0)
		{
		j += (int)(fabs(ysav - xsav) + fabs(rr));
		rj += fabs(ysav - xsav) + fabs(rr);
		}
	    else if (i3 != 0)
		{
		j += (int)fabs(ysav - xsav) + i3;
		rj += fabs(ysav - xsav) + i3;
		}
	    else
		{
		j += (int)fabs(ysav - xsav);
		rj += fabs(ysav - xsav);
		}

	    gj = bj = rj;
	    break;

	case 63:    // Flarium 34, if (fabs(dzx) > dStrands || fabs(dzy) > dStrands)",
	    rr = (double)((int)rr % (threshold / 2));

	    if ((int)(z.x * 2) % 2 == 0 ||
		(int)(z.y * 2) % 2 == 0)
		j += (int)rr;
	    else
		j += (int)rr + (threshold / 2);

	    if ((int)(z.x * 2) % 2 == 0 ||
		(int)(z.y * 2) % 2 == 0)
		rj += rr;
	    else
		rj += rr + (threshold / 2);

	    gj = bj = rj;
	    break;

	case 64:    // Flarium 35, if (fabs(dzx) > dStrands &&&& fabs(dzy) > dStrands)
	    rr = (double)((int)rr % (threshold / 2));

	    if ((int)(z.x * 2.0) % 2 == 0 ||
		(int)(z.y * 2.0) % 2 == 0)
		{
		j += (int)rr;
		rj += rr;
		}
	    else
		{
		j += (int)rr + threshold / 2;
		rj += rr + threshold / 2;
		}

	    gj = bj = rj;
	    break;

	case 65:    // Flarium 36, if (fabs(dzx) > limit || fabs(dzy) > limit)",
	case 66:    // Flarium 37, if (fabs(dzx) < limit || fabs(dzy) < limit)",
	case 67:    // Flarium 38, if (fabs(1/dzx*dzy) > limit)",
	    j += (int)rr;
	    rj += rr;
	    gj = bj = rj;
	    break;

	case 68:    // Flarium 39, if (fabs(dzx) > limit || fabs(dzy) > limit)
	    {
	    double value = atan(xsav / ysav) * 80.0;

	    j += (int)value;
	    rj += value;
	    gj = bj = rj;
	    }
	    break;

	case 69:    // Flarium 40, ri = rr + atan(fabs(dzx_save/dzy_save)) * 30
	    {
	    double value =
		rr + atan(fabs(xsav / ysav)) * 30.0;

	    j += (int)value;
	    rj += value;
	    gj = bj = rj;
	    }
	    break;

	case 70:    // Flarium 41, rr+=log(dzx*dzx+dzy*dzy)*nBay100
	case 71:    // Flarium 46, i = (int)rr; rr+=log(dzx*dzx+dzy*dzy)*(1+nBay100)
	case 73:    // Flarium 48, if (fabs(dzx) < z.squares() || fabs(dzy) < z.squares())  rr+=nFF",
	    j += (int)rr;
	    rj += rr;
	    gj = bj = rj;
	    break;

	case 72:    // Flarium 47, i = (int)(rr + atan(fabs(dzx_save/dzy_save)) * (10+nBay1000))",
	    {
	    double t = rr + atan(fabs(xsav / ysav)) * (10.0 + dFactor_2);

	    j += (int)t;
	    rj += t;
	    gj = bj = rj;
	    }
	    break;

	case 74:    // Flarium 49, rr += atan(fabs(dzy/dzx))*atan(fabs(dzx/dzy))*2",
	    j += (int)(rr * 10.0 * dFactor);
	    rj += rr * 10.0 * dFactor;
	    gj = bj = rj;
	    break;

	default:
	    break;
	}
	// setup fractal dimensions
    switch (FilterType)
	{		
	case 27:  // Check for a fractal Dimension filter
	case 28:  // standard deviation filter
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 48:
	case 49:
	    FDimension(FilterType, iteration);
	    // FDimension colours were historically produced for the
	    // old BGR plotting convention.  FilterPoint now writes
	    // logical RGB correctly, so restore the FD red/blue order here.
	    std::swap(rj, bj);	    
	    break;

	default:						// Note that FilterType = method - TIERAZONFILTERS
	    if (FilterType >= TIERAZONCOLOURS - TIERAZONFILTERS)
		{
		int ColourMethod = FilterType - (TIERAZONCOLOURS - TIERAZONFILTERS);
		ColourMethods(z, q, TrueCol, ColourMethod);
		}
	    break;
	}

    if (UseCurrentPalette)
	{
	FilterRGB.rgbtBlue = TrueCol->PalettePtr[(BYTE)bj].rgbtBlue;
	FilterRGB.rgbtGreen = TrueCol->PalettePtr[(BYTE)gj].rgbtGreen;
	FilterRGB.rgbtRed = TrueCol->PalettePtr[(BYTE)rj].rgbtRed;
	}
    else
	{
	TrueCol->PalettePtr[(BYTE)((*iteration) % 256)].rgbtBlue = FilterRGB.rgbtBlue = (BYTE)bj;		// make this the new palette
	TrueCol->PalettePtr[(BYTE)((*iteration) % 256)].rgbtGreen = FilterRGB.rgbtGreen = (BYTE)gj;
	TrueCol->PalettePtr[(BYTE)((*iteration) % 256)].rgbtRed = FilterRGB.rgbtRed = (BYTE)rj;
	}
    return 0;
    }

