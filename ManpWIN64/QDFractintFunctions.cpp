/*
    QDFRACTINTFUNCTIONS.CPP a module for the per pixel calculations of fractals originally from Fractint 
    This version uses double-double arithmetic.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.

    Note that not all Fractint functions are included because tehre is no need for high resolution

    #ifdef ALLOWALLFRACTINTFUNCTIONS

*/

#include <math.h>
#include "manp.h"
#include "fractype.h"
#include "Complex.h"
#include "QDComplex.h"
#include "pixel.h"
#include "FractintFnTemplate.h"
#include "BailoutTemplate.h"
#include "FractalMathsTemplate.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::QDInitFractintFunctions(WORD type, QDComplex *z, QDComplex *q)
    {
    switch (type)
	{
	case NEWTON:
	case NEWTBASIN:
	case MPNEWTBASIN:
	case MPNEWTON:
	    // The roots of a polynomial function of the form x^n - 1 = 0.

	    // Newton's method to determine the root of f(x) = 0, given a value of x:

	    // DO WHILE(abs(x2 - x1) >= delta1 or abs(f(x2) > delta2 or f'(x1) != 0)
	    //   set x2 = x1 - f(x1) / f'(x1)
	    //   set x1 = x2
	    // ENDDO

	    Init_Newton<QDComplex, qd_real>(this, z, q, param, degree, subtype, rootsQD, threshQD, qd_real::_pi, period_level, color, juliaflag);
	    break;

	case COMPLEXMARKSMAND:					// Complex Mark's Mandelbrot
	    Init_ComplexMarksMand<QDComplex, qd_real>(this, z, q, param, juliaflag, &CoefficientQD);
	    break;

	case SPIDERFP:					// Spider(XAXIS) { c=z=pixel: z=z*z+c; c=c/2+z, |z|<=4 }
	    Init_Spider<QDComplex, qd_real>(this, z, q, param, juliaflag, tempQD);
	    break;

	case MANOWARFP:					// From Art Matrix via Lee Skinner
	case MANOWARJFP:				// to handle fractint par files
	case MANOWARJ:					// to handle fractint par files
	case MANOWAR:					// to handle fractint par files
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    Init_Manowar<QDComplex, qd_real>(this, z, q, &cQD, param, juliaflag, invert, tQD, tempQD, temp1QD);
	    break;

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case BARNSLEYM1:
	case BARNSLEYM2:
	case BARNSLEYM3:
	case BARNSLEYJ1:
	case BARNSLEYJ2:
	case BARNSLEYJ3:
	case BARNSLEYM1FP:
	case BARNSLEYM2FP:
	case BARNSLEYM3FP:
	case BARNSLEYJ1FP:
	case BARNSLEYJ2FP:
	case BARNSLEYJ3FP:
	    break;
#endif
	case COMPLEXNEWTON:
	case COMPLEXBASIN:
	    Init_ComplexNewton<QDComplex, qd_real>(this, z, q, param, crootQD, cdegreeQD, subtype);
	    break;

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case ESCHER:					// Science of Fractal Images pp. 185, 187
	    InitFunctions(MANDELFP, z, q);
	    break;
#endif
	case MANDELLAMBDAFP:				// variation of classical Mandelbrot/Julia
	case MANDELLAMBDA:
	case LAMBDAFP:
	case LAMBDA:
	    Init_Lambda<QDComplex, qd_real>(this, z, q, param, juliaflag, tempQD);
	    break;

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case PHOENIXFP:
	case PHOENIX:
	case MANDPHOENIXFP:
	case MANDPHOENIX:
	    break;

	case MANDPHOENIXFPCPLX:
	case MANDPHOENIXCPLX:
	case PHOENIXCPLX:
	case PHOENIXFPCPLX:
	    break;
#endif

	case FPMANDELZPOWER:
	case LMANDELZPOWER:
	case LJULIAZPOWER:
	case FPMANZTOZPLUSZPWR:
	case FPJULZTOZPLUSZPWR:
	case TETRATEFP:					// Tetrate(XAXIS) { c=z=pixel: z=c^z, |z|<=(P1+3)
	    Init_ZPowerGroup<QDComplex, qd_real>(this, z, q, cQD, param, juliaflag, invert, tQD, tempQD, temp1QD, temp2QD);
	    break;

	case MARKSMANDELFP:
	case MARKSMANDEL:
	case MARKSJULIAFP:
	case MARKSJULIA:
	    Init_Marks<QDComplex, qd_real>(this, z, q, cQD, param, juliaflag, invert, tQD, CoefficientQD);
	    break;

	case QUATFP:
	case QUATJULFP:
	    Init_Quat<QDComplex, qd_real>(this, z, q, cQD, param, juliaflag, invert, tQD, qcQD, qciQD, qcjQD, qckQD);
	    break;

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case UNITYFP:					// Unity Fractal - brought to you by Mark Peterson - you won't find this in any fractal books unless they saw it here first - Mark invented it!
	case SIERPINSKI:				// following code translated from basic - see "Fractals Everywhere" by Michael Barnsley, p. 251, Program 7.1.1
	case SIERPINSKIFP:
	    break;

	case VL:					// Beauty of Fractals pp. 125 - 127
	    break;
#endif

	case MAGNET1M:
	case MAGNET1J:
	    QDInitTierazonFunctions(102, z, q);
	    break;

	case MAGNET2M:
	case MAGNET2J:
	    QDInitTierazonFunctions(103, z, q);
	    break;

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case LYAPUNOV:
	break;

	case FROTH:
	break;
#endif

	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::QDRunFractintFunctions(WORD type, QDComplex *z, QDComplex *q, BYTE *SpecialFlag, long *iteration)
    {
    QDComplex   a, b, t, z1, z2, zd, zt;

    switch (type)
	{
	case NEWTON:
	case NEWTBASIN:
	case MPNEWTBASIN:
	case MPNEWTON:
	    return Iter_Newton<QDComplex, qd_real>(this, z, q, z1, z2, zd, iteration, degree, subtype, rootsQD, threshQD, color, threshold);

	case COMPLEXMARKSMAND:					// Complex Mark's Mandelbrot
	    return Iter_ComplexMarksMandelbrot<QDComplex, qd_real>(this, z, q, sqrQD, realimagQD, CoefficientQD, BailoutTestType, rqlim);

	case SPIDERFP:						// Spider(XAXIS) { c=z=pixel: z=z*z+c; c=c/2+z, |z|<=4 }
	    return Iter_Spider<QDComplex, qd_real>(this, z, q, tempQD, BailoutTestType, rqlim);

	case MANOWARFP:						// From Art Matrix via Lee Skinner
	case MANOWARJFP:					// to handle fractint par files
	case MANOWARJ:						// to handle fractint par files
	case MANOWAR:						// to handle fractint par files
	    return Iter_ManOWar<QDComplex, qd_real>(this, z, q, tempQD, temp1QD, temp3QD, param, BailoutTestType, rqlim);

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case BARNSLEYM1:					// Barnsley's Mandelbrot type M1 from "Fractals Everywhere" by Michael Barnsley, p. 322
	case BARNSLEYJ1:
	case BARNSLEYM1FP:
	case BARNSLEYJ1FP:
	    return DoBailout(bailout_type, z, rqlim);

	case BARNSLEYM2:					// An unnamed Mandelbrot/Julia function from "Fractals Everywhere" by Michael Barnsley, p. 331, example 4.2
	case BARNSLEYJ2:
	case BARNSLEYM2FP:
	case BARNSLEYJ2FP:
	    return DoBailout(bailout_type, z, rqlim);

	case BARNSLEYM3:					// An unnamed Mandelbrot/Julia function from "Fractals Everywhere" by Michael Barnsley, p. 292, example 4.1 
	case BARNSLEYJ3:
	case BARNSLEYM3FP:
	case BARNSLEYJ3FP:
	    return DoBailout(bailout_type, z, rqlim);
#endif

	case COMPLEXNEWTON:
	case COMPLEXBASIN:
	    return Iter_ComplexNewton<QDComplex, qd_real>(this, z, q, crootQD, cdegreeQD, iteration, subtype, color);

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case ESCHER:							// Science of Fractal Images pp. 185, 187
	    return 1;					// just to shut up compiler warnings
#endif
	case MANDELLAMBDAFP:				// variation of classical Mandelbrot/Julia
	case MANDELLAMBDA:
	case LAMBDAFP:
	case LAMBDA:
	    return Iter_Lambda<QDComplex, qd_real>(this, z, q, tempQD, BailoutTestType, rqlim);

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case PHOENIXFP:
	case PHOENIX:
	case MANDPHOENIXFP:
	case MANDPHOENIX:
	    return 1;					// just to shut up compiler warnings

	case MANDPHOENIXFPCPLX:
	case MANDPHOENIXCPLX:
	case PHOENIXCPLX:
	case PHOENIXFPCPLX:
	    return 1;					// just to shut up compiler warnings
#endif

	case LJULIAZPOWER:
	case LMANDELZPOWER:
	case FPMANDELZPOWER:
	    return Iter_ZPower<QDComplex, qd_real>(this, z, q, temp2QD, BailoutTestType, rqlim);

	case FPMANZTOZPLUSZPWR:
	case FPJULZTOZPLUSZPWR:
	    return Iter_ZtoZPlusZPower<QDComplex, qd_real>(this, z, q, tempQD, param, BailoutTestType, rqlim);

	case MARKSMANDELFP:				// Mark Peterson's variation of "lambda" function
	case MARKSMANDEL:
	case MARKSJULIAFP:
	case MARKSJULIA:
	    // Z1 = (C^(exp-1) * Z**2) + C
	    return Iter_MarksMandel<QDComplex, qd_real>(this, z, q, tempQD, temp1QD, CoefficientQD, tQD, BailoutTestType, rqlim);

	case QUATFP:
	case QUATJULFP:
	    return Iter_Quat<QDComplex, qd_real>(this, z, tempQD, qcQD, qciQD, qcjQD, qckQD, rqlim);

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case SIERPINSKI:				// following code translated from basic - see "Fractals Everywhere" by Michael Barnsley, p. 251, Program 7.1.1
	case SIERPINSKIFP:
	    return DDFractintBailoutTest(z, rqlim, BailoutTestType);
#endif

	case TETRATEFP:					// Tetrate(XAXIS) { c=z=pixel: z=c^z, |z|<=(P1+3)
	    return Iter_Tetrate<QDComplex, qd_real>(this, z, q, BailoutTestType, rqlim);

#ifdef ALLOWALLFRACTINTFUNCTIONS

	case UNITYFP:					// Unity Fractal - brought to you by Mark Peterson - you won't find this in any fractal books unless they saw it here first - Mark invented it!
	    return(0);

	case VL:					// Beauty of Fractals pp. 125 - 127
	    return DoBailout(bailout_type, z, rqlim);
#endif

	case MAGNET1M:
	case MAGNET1J:
	    return (QDRunTierazonFunctions(102, z, q, &z2, SpecialFlag, iteration));

	case MAGNET2M:
	case MAGNET2J:
	    return (QDRunTierazonFunctions(103, z, q, &z2, SpecialFlag, iteration));

#ifdef ALLOWALLFRACTINTFUNCTIONS
	case LYAPUNOV:
	    return TRUE;

	case FROTH:		// Froth Fractal type - per pixel 1/2/g, called with row & col set
	    return TRUE;

#endif



	}
    return 0;
    }

