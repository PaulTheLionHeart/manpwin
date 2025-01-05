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

int	CPixel::FloatDecomposition(double z_real, double z_imag)

    {
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
	Double Double Decomposition of Colours
**************************************************************************/

int	CPixel::DDDecomposition(dd_real z_real, dd_real z_imag)

    {
    short	temp = 0;
    int	i;
    DDComplex	alt, New;
    long	colour;

    New.x = z_real;
    New.y = z_imag;

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
	Quad Double Decomposition of Colours
**************************************************************************/

int	CPixel::QDDecomposition(qd_real z_real, qd_real z_imag)

    {
    short	temp = 0;
    int	i;
    QDComplex	alt, New;
    long	colour;

    New.x = z_real;
    New.y = z_imag;

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
