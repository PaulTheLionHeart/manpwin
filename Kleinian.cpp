/*
 * Copyright (C) 2019 hrkalona
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include	<math.h>
#include	"manp.h"
#include	"Complex.h"
#include	"Pixel.h"

 //////////////////////////////////////////////////////////////////////
 // Initialise Kleinian fractal
 //////////////////////////////////////////////////////////////////////

int	CPixel::InitKleinian(Complex *z, Complex *q)
    {
    u = param[0];
    v1 = param[1];
    K = param[2];
    M = param[3];
    t = {u,v1};
    if (!juliaflag)
	{
	z->x = q->x + param[4];
	z->y = q->y + param[5];
	}

    return 0;
    }

//////////////////////////////////////////////////////////////////////
// Kleinian function where all the work is done
//////////////////////////////////////////////////////////////////////

Complex CPixel::KleinianFunction(Complex *z)
    {
    int	N = 100000000;
    double	re = z->x;
    double	im = z->y;
    Complex	i = {0.0,1.0};

    re = fmod((re + 2 * N - 1 + v1 * im / u), 2.0) - 1 - v1 * im / u;

    double	f = (re >= -v1 * 0.5) ? 1 : -1;
    double	line3_im = u * 0.5 + f * K * u * (1 - exp(-M * fabs(re + v1 * 0.5)));

    z->x = re;
    z->y = im;

    if (im < line3_im) 
	*z = (t * *z - i) / (*z * -i);		// Moebius transformation
    else 
        *z = i / (*z * i + t);			// inverse transformation
    return *z;
    }

//////////////////////////////////////////////////////////////////////
// Calculate Kleinian fractal
//////////////////////////////////////////////////////////////////////

int	CPixel::CalculateKleinian(Complex *z)
    {
    if (z->y < 0.0 || z->y > u)
	return true;

		//If the iterated points enters a 2-cycle , bail out.
    if (*iteration != 0 && z->CSumSqr() < rqlim)
	return true;
    *z = KleinianFunction(z);
    return false;
    }

