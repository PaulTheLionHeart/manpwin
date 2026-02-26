/******************************************************************/
/* Continuous potential calculation for Mandelbrot and Julia      */
/* Reference: Science of Fractal Images p. 190.                   */
/* Special thanks to Mark Peterson for his "MtMand" program that  */
/* beautifully approximates plate 25 (same reference) and spurred */
/* on the inclusion of similar capabilities in FRACTINT.          */
/*                                                                */
/* The purpose of this function is to calculate a color value     */
/* for a fractal that varies continuously with the screen pixels  */
/* locations for better rendering in 3D.                          */
/*                                                                */
/* Here "magnitude" is the modulus of the orbit value at          */
/* "iterations". The potparms[] are user-entered paramters        */
/* controlling the level and slope of the continuous potential    */
/* surface. Returns color.  - Tim Wegner 6/25/89                  */
/*                                                                */
/*                     -- Change history --                       */
/*                                                                */
/* 09/12/89   - added floatflag support and fixed float underflow */
/*                                                                */
/******************************************************************/

#include	"potential.h"

int	CPotential::potential(double mag, int iterations, long threshold, CTrueCol *TrueCol, int colors, double potparam[])
    {
    float /*f_mag,f_tmp,*/pot;
    double d_tmp;
    int i_pot;
    long l_pot;

    if(iterations < threshold)
	{
	pot = (float)(l_pot = iterations+2);
	if(l_pot <= 0 || mag <= 1.0)
	    pot = (float)0.0;
	else /* pot = log(mag) / pow(2.0, (double)pot); */
	    {
//         if(l_pot < 120 && !floatflag) /* empirically determined limit of fShift */
//         {
//            f_mag = (float)mag;
//            fLog14(f_mag,f_tmp); /* this SHOULD be non-negative */
//            fShift(f_tmp,(char)-l_pot,pot);
//         }
//         else
		{
		d_tmp = log(mag)/(double)pow(2.0,(double)pot);
		if(d_tmp > FLT_MIN) /* prevent float type underflow */
		    pot = (float)d_tmp;
		else
		    pot = (float)0.0;
		}
	    }
      /* following transformation strictly for aesthetic reasons */
      /* meaning of parameters:
            potparam[0] -- zero potential level - highest color -
            potparam[1] -- slope multiplier -- higher is steeper
            potparam[2] -- rqlim value if changeable (bailout for modulus) */

	if(pot > 0.0)
	    {
//         if(floatflag)
            pot = (float)sqrt((double)pot);
//         else
//         {
//            fSqrt14(pot,f_tmp);
//            pot = f_tmp;
//         }
	    pot = (float)(potparam[0] - pot*potparam[1] - 1.0);
	    }
	else
	    pot = (float)(potparam[0] - 1.0);
	if(pot < 1.0)
	    pot = (float)1.0; /* avoid color 0 */
	}
    else if(TrueCol->inside_colour >= 0)
	pot = (float)TrueCol->inside_colour;
    else /* inside < 0 implies inside=maxit, so use 1st pot param instead */
	pot = (float)potparam[0];

    i_pot = (int)((l_pot = (long)(pot * 256)) >> 8);
    if(i_pot >= colors)
	{
	i_pot = colors - 1;
	l_pot = 255;
	}

    return(i_pot);
    }

