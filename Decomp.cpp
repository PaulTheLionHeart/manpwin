/*
    DECOMP.CPP a module for decomposition of the per pixel calculations. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	"manp.h"
#include	"complex.h"
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	<math.h>
#include	"pixel.h"

#define		BITSHIFT	25

/**************************************************************************
	Floating Point Decomposition of Colours
**************************************************************************/

int	CPixel::float_decomposition(double z_real, double z_imag)

    {
    double	cos45 = 0.70710678118654750; // cos 45	degrees
    double	sin45 = 0.70710678118654750; // sin 45	degrees
    double	cos22_5 = 0.92387953251128670; // cos 22.5	degrees
    double	sin22_5 = 0.38268343236508980; // sin 22.5	degrees
    double	cos11_25 = 0.98078528040323040; // cos 11.25	degrees
    double	sin11_25 = 0.19509032201612820; // sin 11.25	degrees
    double	cos5_625 = 0.99518472667219690; // cos 5.625	degrees
    double	sin5_625 = 0.09801714032956060; // sin 5.625	degrees
    double	tan22_5 = 0.41421356237309500; // tan 22.5	degrees
    double	tan11_25 = 0.19891236737965800; // tan 11.25	degrees
    double	tan5_625 = 0.09849140335716425; // tan 5.625	degrees
    double	tan2_8125 = 0.04912684976946725; // tan 2.8125	degrees
    double	tan1_4063 = 0.02454862210892544; // tan 1.4063	degrees

    short	temp = 0;
    int	i;
    Complex	alt, New;
    long	colour;

    if (BigNumFlag)
	{
	New.x = (zBig->x.x, MPFR_RNDN);
	New.y = (zBig->y.x, MPFR_RNDN);
	}
    else
	{
	New.x = z_real;
	New.y = z_imag;
	}

    colour = 0;
    if (New.y < 0)
	{
	temp = 2;
	New.y = -New.y;
	}
    if (New.x < 0)
	{
	++temp;
	New.x = -New.x;
	}
    if (decomp >= 8)
	{
	temp <<= 1;
	if (New.x < New.y)
	    {
	    ++temp;
	    alt.x = New.x;				/* just swap them */
	    New.x = New.y;
	    New.y = alt.x;
	    }
	if (decomp >= 16)
	    {
	    temp <<= 1;
	    if (New.x*tan22_5 < New.y)
		{
		++temp;
		alt = New;
		New.x = alt.x * cos45 + alt.y * sin45;
		New.y = alt.x * sin45 - alt.y * cos45;
		}

	    if (decomp >= 32)
		{
		temp <<= 1;
		if (New.x*tan11_25 < New.y)
		    {
		    ++temp;
		    alt = New;
		    New.x = alt.x*cos22_5 + alt.y*sin22_5;
		    New.y = alt.x*sin22_5 - alt.y*cos22_5;
		    }

		if (decomp >= 64)
		    {
		    temp <<= 1;
		    if (New.x*tan5_625 < New.y)
			{
			++temp;
			alt = New;
			New.x = alt.x*cos11_25 + alt.y*sin11_25;
			New.y = alt.x*sin11_25 - alt.y*cos11_25;
			}

		    if (decomp >= 128)
			{
			temp <<= 1;
			if (New.x*tan2_8125 < New.y)
			    {
			    ++temp;
			    alt = New;
			    New.x = alt.x*cos5_625 + alt.y*sin5_625;
			    New.y = alt.x*sin5_625 - alt.y*cos5_625;
			    }

			if (decomp == 256)
			    {
			    temp <<= 1;
			    if ((New.x*tan1_4063 < New.y))
				++temp;
			    }
			}
		    }
		}
	    }
	}
    for (i = 1; temp > 0; ++i)
	{
	if (temp & 1)
	    colour = (1 << i) - 1 - colour;
	temp >>= 1;
	}
    if (decomp == 2)
	colour &= 1;
    if (colours > decomp)
	colour++;
    return (colour);
    }

/**************************************************************************
	Floating Point Decomposition of Colours
**************************************************************************/

int	CPixel::BigDecomposition(BigDouble z_real, BigDouble z_imag)
    {
    double	cos45 = 0.70710678118654750; // cos 45	degrees
    double	sin45 = 0.70710678118654750; // sin 45	degrees
    double	cos22_5 = 0.92387953251128670; // cos 22.5	degrees
    double	sin22_5 = 0.38268343236508980; // sin 22.5	degrees
    double	cos11_25 = 0.98078528040323040; // cos 11.25	degrees
    double	sin11_25 = 0.19509032201612820; // sin 11.25	degrees
    double	cos5_625 = 0.99518472667219690; // cos 5.625	degrees
    double	sin5_625 = 0.09801714032956060; // sin 5.625	degrees
    double	tan22_5 = 0.41421356237309500; // tan 22.5	degrees
    double	tan11_25 = 0.19891236737965800; // tan 11.25	degrees
    double	tan5_625 = 0.09849140335716425; // tan 5.625	degrees
    double	tan2_8125 = 0.04912684976946725; // tan 2.8125	degrees
    double	tan1_4063 = 0.02454862210892544; // tan 1.4063	degrees

    short	temp = 0;
    int	i;
    BigComplex	alt, New;
    long	colour;

    New.x = z_real;
    New.y = z_imag;

    colour = 0;
    if (New.y < (BigDouble)0)
	{
	temp = 2;
	New.y = -New.y;
	}
    if (New.x < (BigDouble)0)
	{
	++temp;
	New.x = -New.x;
	}
    if (decomp >= 8)
	{
	temp <<= 1;
	if (New.x < New.y)
	    {
	    ++temp;
	    alt.x = New.x;				/* just swap them */
	    New.x = New.y;
	    New.y = alt.x;
	    }
	if (decomp >= 16)
	    {
	    temp <<= 1;
	    if (New.x*tan22_5 < New.y)
		{
		++temp;
		alt = New;
		New.x = alt.x * cos45 + alt.y * sin45;
		New.y = alt.x * sin45 - alt.y * cos45;
		}

	    if (decomp >= 32)
		{
		temp <<= 1;
		if (New.x*tan11_25 < New.y)
		    {
		    ++temp;
		    alt = New;
		    New.x = alt.x*cos22_5 + alt.y*sin22_5;
		    New.y = alt.x*sin22_5 - alt.y*cos22_5;
		    }

		if (decomp >= 64)
		    {
		    temp <<= 1;
		    if (New.x*tan5_625 < New.y)
			{
			++temp;
			alt = New;
			New.x = alt.x*cos11_25 + alt.y*sin11_25;
			New.y = alt.x*sin11_25 - alt.y*cos11_25;
			}

		    if (decomp >= 128)
			{
			temp <<= 1;
			if (New.x*tan2_8125 < New.y)
			    {
			    ++temp;
			    alt = New;
			    New.x = alt.x*cos5_625 + alt.y*sin5_625;
			    New.y = alt.x*sin5_625 - alt.y*cos5_625;
			    }

			if (decomp == 256)
			    {
			    temp <<= 1;
			    if ((New.x*tan1_4063 < New.y))
				++temp;
			    }
			}
		    }
		}
	    }
	}
    for (i = 1; temp > 0; ++i)
	{
	if (temp & 1)
	    colour = (1 << i) - 1 - colour;
	temp >>= 1;
	}
    if (decomp == 2)
	colour &= 1;
    if (colours > decomp)
	colour++;
    return (colour);
    }
