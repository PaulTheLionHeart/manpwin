/*
    FRACTINTFUNCTIONS.CPP a module for the per pixel calculations of fractals originally from Fractint 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	"manp.h"
#include	"fractype.h"
#include	"Complex.h"
#include	"pixel.h"
#include	"bif.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::InitFractintFunctions(WORD type, Complex *z, Complex *q)
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

	    *degree = (int)param[0];			// so we have to get the degrees and subtype from the parameters
	    if (*degree < 2)
		*degree = 3;				// defaults to 3, but 2 is possible 
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
	    thresh = 0.3 * PI / (double)*degree;	// less than half distance between roots

	    if (subtype == 'S' || subtype == 'B')
		{
		if (*degree > MAXROOTS)
		    *degree = MAXROOTS;

		// list of roots to discover where we converged for newtbasin

		for (int i = 0; i < *degree; i++)
		    {
		    roots[i].x = cos(i*PI*2.0 / (double)*degree);
		    roots[i].y = sin(i*PI*2.0 / (double)*degree);
		    }
		}
	    period_level = FALSE;			// no periodicity checking
	    *color = 0;
	    if (!juliaflag)
		*z = *q / 3;
	    break;

	case COMPLEXMARKSMAND:					// Complex Mark's Mandelbrot
	    {
	    Complex pwr;

	    if (!juliaflag)
		{
		z->x = q->x + param[0];
		z->y = q->y + param[1];
		}
	    sqr = 0;
	    real_imag = 0.0;
	    pwr.x = param[2] - 1.0;
	    pwr.y = param[3];
	    Coefficient = *q ^ pwr;
	    break;
	    }

	case SPIDERFP:					// Spider(XAXIS) { c=z=pixel: z=z*z+c; c=c/2+z, |z|<=4 }
	    if (!juliaflag)
		{
		z->x = q->x + param[0];
		z->y = q->y + param[1];
		}
	    sqr = 0.0;
	    real_imag = 0.0;
	    temp = *q;
	    break;

	case MANOWARFP:					// From Art Matrix via Lee Skinner
	case MANOWARJFP:				// to handle fractint par files
	case MANOWARJ:					// to handle fractint par files
	case MANOWAR:					// to handle fractint par files
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    t = (invert) ? invertz2(*c) : *c;
	    if (juliaflag)
		{
		*z = t;
		temp.x = q->x;
		temp.y = q->y;
		}
	    else
		{
		z->x = temp.x = t.x + param[0];
		z->y = temp.y = t.y + param[1];
		}
	    sqr.x = sqr(z->x);
	    sqr.y = sqr(z->y);
	    temp1 = *z;
	    break;

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
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
//	    t = (invert) ? invertz2(*c) : *c;
	    temp.x = q->x;
	    temp.y = q->y;
	    temp2.x = 0.0;
	    temp2.y = 0.0;
	    if (juliaflag)
		*z = *c;
	    break;

	case COMPLEXNEWTON:
	case COMPLEXBASIN:
	    croot = 1;
	    cdegree = 3;
	    croot.x = param[2];
	    croot.y = param[3];
	    cdegree.x = param[0];
	    cdegree.y = param[1];
	    thresh = 0.3 * PI / cdegree.x;		// less than half distance between roots
	    *z = *q;
	    subtype = ((int)param[4] == 0.0) ? 'N' : 'B';
	    break;

	case ESCHER:					// Science of Fractal Images pp. 185, 187
	    InitFunctions(MANDELFP, z, q);
	    break;

	case MANDELLAMBDAFP:				// variation of classical Mandelbrot/Julia
	case MANDELLAMBDA:
	case LAMBDAFP:
	case LAMBDA:
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		temp.x = q->x;
		temp.y = q->y;
		}
	    else
		{
		z->x = q->x + param[0];
		z->y = q->y + param[1];
		temp.x = z->x;
		temp.y = z->y;
		*z = 0.5;
		}
	    sqr.x = sqr(z->x);  // precalculated value for regular Mandelbrot
	    sqr.y = sqr(z->y);
	    break;

	case PHOENIXFP:
	case PHOENIX:
	case MANDPHOENIXFP:
	case MANDPHOENIX:
	    t = (invert) ? invertz2(*c) : *c;
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		temp.x = q->x;
		temp.y = q->y;
		}
	    else
		{
		if (type == MANDPHOENIXFP)
		    *z = temp = t;
		else
		    *z = t;
		}
	    temp3 = 0;					// set temp3 to Y value
	    PhoenixDegree = (int)temp2.x;
	    if (PhoenixDegree < 2 && PhoenixDegree > -3)
		PhoenixDegree = 0;
	    param[2] = (double)PhoenixDegree;
	    if (PhoenixDegree == 0)
		PhoenixType = ZERO;
	    if (PhoenixDegree >= 2)
		{
		PhoenixDegree = PhoenixDegree - 1;
		PhoenixType = PLUS;
		}
	    if (PhoenixDegree <= -3)
		{
		PhoenixDegree = abs(PhoenixDegree);
		PhoenixDegree -= 2;
		PhoenixType = MINUS;
		}
	    break;

	case MANDPHOENIXFPCPLX:
	case MANDPHOENIXCPLX:
	case PHOENIXCPLX:
	case PHOENIXFPCPLX:
	    t = (invert) ? invertz2(*c) : *c;
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		temp.x = q->x;
		temp.y = q->y;
		}
	    else
		{
		z->x = t.x + param[0];
		z->y = t.y + param[1];
		}
	    temp3 = 0;					// set temp3 to Y value
	    PhoenixDegree = (int)param[4];
	    if (PhoenixDegree < 2 && PhoenixDegree > -3)
		PhoenixDegree = 0;
	    param[4] = (double)PhoenixDegree;
	    if (PhoenixDegree == 0)
		{
		if (temp2.x != 0 || temp2.y != 0)
		    *symmetry = NOSYM;
		else
		    *symmetry = ORIGIN;
		if (temp.y == 0 && temp2.y == 0)
		    *symmetry = XAXIS;
		PhoenixType = ZERO;
		}
	    if (PhoenixDegree >= 2)
		{
		PhoenixDegree = PhoenixDegree - 1;
		if (temp.y == 0 && temp2.y == 0)
		    *symmetry = XAXIS;
		else
		    *symmetry = NOSYM;
		PhoenixType = PLUS;
		}
	    if (PhoenixDegree <= -3)
		{
		PhoenixDegree = abs(PhoenixDegree);
		PhoenixDegree -= 2;
		if (temp.y == 0 && temp2.y == 0)
		    *symmetry = XAXIS;
		else
		    *symmetry = NOSYM;
		PhoenixType = MINUS;
		}
	    break;

	case FPMANDELZPOWER:
	case LMANDELZPOWER:
	case LJULIAZPOWER:
	case FPMANZTOZPLUSZPWR:
	case FPJULZTOZPLUSZPWR:
	case TETRATEFP:					// Tetrate(XAXIS) { c=z=pixel: z=c^z, |z|<=(P1+3)
	    t = (invert) ? invertz2(*c) : *c;
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		temp.x = q->x;
		temp.y = q->y;
		}
	    else
		{
		z->x = t.x + param[0];
		z->y = t.y + param[1];
		}
	    sqr.x = sqr(z->x);
	    sqr.y = sqr(z->y);
	    temp1 = *z;					// set temp3 to Y value
	    break;

	case MARKSMANDELFP:
	case MARKSMANDEL:
	case MARKSJULIAFP:
	case MARKSJULIA:
	    {
	    Complex pwr;

	    t = (invert) ? invertz2(*c) : *c;
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		t.x = q->x;
		t.y = q->y;
		}
	    else
		{
		z->x = t.x + param[0];
		z->y = t.y + param[1];
		}
	    sqr = 0;
	    real_imag = 0.0;
	    pwr.x = param[2] - 1.0;
	    pwr.y = 0;
	    Coefficient = *q ^ pwr;
	    break;
	    }

	case QUATFP:
	case QUATJULFP:
	    t = (invert) ? invertz2(*c) : *c;
	    temp = 0;
	    if (juliaflag)
		{
		*z = t;
		qc = q->x;
		qci = q->y;
		}
	    else
		{
		*z = 0;
		qc = t.x;
		qci = t.y;
		}
	    qcj = param[2];
	    qck = param[3];
	    break;

	case UNITYFP:					// Unity Fractal - brought to you by Mark Peterson - you won't find this in any fractal books unless they saw it here first - Mark invented it!
	case SIERPINSKI:				// following code translated from basic - see "Fractals Everywhere" by Michael Barnsley, p. 251, Program 7.1.1
	case SIERPINSKIFP:
	    if (!juliaflag)
		*z = *q;
	    period_level = FALSE;			// no periodicity checking
	    break;

	case VL:					// Beauty of Fractals pp. 125 - 127
	    t = (invert) ? invertz2(*c) : *c;
	    temp.x = param[0];
	    temp.y = param[1];
	    temp2.x = param[2];
	    temp2.y = param[3];
	    period_level = FALSE;			// no periodicity checking (get rid of bug 31/7/00 PHD)
	    if (juliaflag)
		{
		temp.x = q->x;
		temp.y = q->y;
		}
	    else
		{
		z->x = t.x + param[0];
		z->y = t.y + param[1];
		}
	    sqr.x = sqr(z->x);
	    sqr.y = sqr(z->y);
	    temp1 = *z;					// set temp3 to Y value
	    if (param[0] < 0.0) param[0] = 0.0;
	    if (param[1] < 0.0) param[1] = 0.0;
	    if (param[0] > 1.0) param[0] = 1.0;
	    if (param[1] > 1.0) param[1] = 1.0;
	    break;

	case MAGNET1M:
	case MAGNET1J:
	    InitTierazonFunctions(102, z, q);
	    break;

	case MAGNET2M:
	case MAGNET2J:
	    InitTierazonFunctions(103, z, q);
	    break;

	/******************* standalone engine for "lyapunov" ********************
	Roy Murphy [76376,721]
	revision history:
	initial version: Winter '91
	Fall '92 integration of Nicholas Wilt's ASM speedups
	Jan 93' integration with calcfrac() yielding boundary tracing,
	tesseral, and solid guessing, and inversion, inside=nnn	
	save_release behavior:	
	1730 & prior: ignores inside=, calcmode='1', (a,b)->(x,y)	
	1731: other calcmodes and inside=nnn
	1732: the infamous axis swap: (b,a)->(x,y),
	the order parameter becomes a long int	
	**************************************************************************/
	/*
	This routine sets up the sequence for forcing the Rate parameter
	to vary between the two values.  It fills the array lyaRxy[] and
	sets lyaLength to the length of the sequence.

	The sequence is coded in the bit pattern in an integer.
	Briefly, the sequence starts with an A the leading zero bits
	are ignored and the remaining bit sequence is decoded.  The
	sequence ends with a B.  Not all possible sequences can be
	represented in this manner, but every possible sequence is
	either represented as itself, as a rotation of one of the
	representable sequences, or as the inverse of a representable
	sequence (swapping 0s and 1s in the array.)  Sequences that
	are the rotation and/or inverses of another sequence will generate
	the same lyapunov exponents.

	A few examples follow:
	    number    sequence
		0	ab
		1	aab
		2	aabb
		3	aaab
		4	aabbb
		5	aabab
		6	aaabb (this is a duplicate of 4, a rotated inverse)
		7	aaaab
		8	aabbbb	etc.
	*/

	case LYAPUNOV:
	    {
	    long    i, k;
	    int	    save_release = 0;
	    char    TempSeq[120];
	    char    *p;

	    if ((filter_cycles = (WORD)(param[2])) == 0)
		filter_cycles = threshold / 2;
	    lyaSeedOK = param[0] > 0 && param[0] <= 1;
	    lyaLength = (int)strlen(LyapSequence);
	    strcpy(TempSeq, LyapSequence);
/*
	    i = (long)(param[0]);
	    lyaRxy[0] = 1;
	    for (t = 32; t >= 0; t--)
		if (i & (1 << t)) break;
	    for (; t >= 0; t--)
		lyaRxy[lyaLength++] = (i & (1 << t)) != 0;
	    lyaRxy[lyaLength++] = 0;
	    if (save_release < 1732)		// swap axes prior to 1732
		for (t = lyaLength; t >= 0; t--)
		    lyaRxy[t] = !lyaRxy[t];
*/
	    p = TempSeq;			// better make sure we have no silly characters

	    for (i = 0, k = 0; i < lyaLength; i++)
		{
		char	c = toupper(*(TempSeq + i));
		if (c == 'A' || c == 'B' || c == 'C')
		    {
		    switch (c)
			{
			case 'A':
			    lyaRxy[k] = 0;
			    break;
			case 'B':
			    lyaRxy[k] = 1;
			    break;
			case 'C':
			    lyaRxy[k] = 2;
			    break;
			}
		    LyapSequence[k] = c;
		    k++;
		    }
		}
	    LyapSequence[k] = '\0';
	    lyaLength = (int)strlen(LyapSequence);
/*
	    for (i = 0; i < 1; i++)
		{
		lyaRxy[i] = 1;
		lyaRxy[i + 2] = 0;
		}
	    lyaLength = 2;
*/
//	    DisplayLyapSequence(lyaRxy, lyaLength);
	    }
	break;

	case FROTH:
	    {
	    if (param[0] != 3 && param[0] != 6)		// if no match then
		param[0] = 3;				// make it 3
	    frothsix = param[0] == 6;
	    froth_altcolor = param[1] != 0;
	    froth_shades = (colors - 1) / (frothsix ? 6 : 3);
	
	    if (rqlim < 6.0)
		rqlim = 6.0;				// rqlim needs to be at least 6 or so 
	    set_Froth_palette(hwnd);			// make the best of the .map situation 
//	orbit_color = !frothsix && colors >= 16 ? (froth_shades<<1)+1 : colors-1;
	    }
	break;


	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::RunFractintFunctions(WORD type, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration)
    {
    switch (type)
	{
	case NEWTON:
	case NEWTBASIN:
	case MPNEWTBASIN:
	case MPNEWTON:
	{
	    int		tmpcolor;
	    int		i;

	    color = iteration;
	    z2 = *z;
	    z1 = z->CPolynomial(*degree - 1);
	    *z = *z - (z1 * *z - *q - 1) / (z1 * *degree);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    if (d < MINSIZE)
		{

		if (subtype == 'S' || subtype == 'B')
		    {
		    tmpcolor = -1;
		    // this code determines which degree-th root of root the Newton formula converges to. 
		    // The roots of a 1 are distributed on a circle of radius 1 about the origin.
		    for (i = 0; i < *degree; i++)
			{
			// color in alternating shades with iteration according to which root of 1 it converged to
			if (distance(roots[i], z2) < thresh)
			    {
			    if (subtype == 'S')
				tmpcolor = 1 + (i & 7) + ((*color & 1) << 3);
			    else
				tmpcolor = 1 + i;
			    break;
			    }
			}
		    if (tmpcolor == -1)
			*color = threshold;
		    else
			*color = tmpcolor;
		    }

		return(TRUE);
		}
	    else
		return(FALSE);
	    }

	case COMPLEXMARKSMAND:					// Complex Mark's Mandelbrot
	    sqr.x = sqr(z->x);
	    sqr.y = sqr(z->y);
	    real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y;
	    z->y = real_imag + real_imag;
	    *z = Coefficient * *z + *q;
	    return BailoutTest(z, sqr);

	case SPIDERFP:						// Spider(XAXIS) { c=z=pixel: z=z*z+c; c=c/2+z, |z|<=4 }
	    sqr.x = sqr(z->x);
	    sqr.y = sqr(z->y);
	    real_imag = z->x * z->y;
	    z->x = sqr.x - sqr.y + temp.x;
	    z->y = 2 * real_imag + temp.y;
	    temp.x = temp.x / 2 + z->x;
	    temp.y = temp.y / 2 + z->y;
	    return BailoutTest(z, sqr);

	case MANOWARFP:						// From Art Matrix via Lee Skinner
	case MANOWARJFP:					// to handle fractint par files
	case MANOWARJ:						// to handle fractint par files
	case MANOWAR:						// to handle fractint par files
	    sqr.x = sqr(z->x);
	    sqr.y = sqr(z->y);
	    temp3.x = sqr.x - sqr.y + temp.x + temp1.x + param[0];
	    temp3.y = 2.0 * z->x * z->y + temp.y + temp1.y + param[1];
	    temp1 = *z;
	    d = z->CSumSqr();
	    *z = temp3;
	    return BailoutTest(z, sqr);

	case BARNSLEYM1:					// Barnsley's Mandelbrot type M1 from "Fractals Everywhere" by Michael Barnsley, p. 322
	case BARNSLEYJ1:
	case BARNSLEYM1FP:
	case BARNSLEYJ1FP:
	    // calculate intermediate products
	    foldxinitx = z->x * temp.x;
	    foldyinity = z->y * temp.y;
	    foldxinity = z->x * temp.y;
	    foldyinitx = z->y * temp.x;
	    // orbit calculation 
	    if (z->x >= 0)
		{
//		z->x = (foldxinitx - q->x - foldyinity + param[0]);
//		z->y = (foldyinitx - q->y + foldxinity + param[1]);
		temp1.x = foldxinitx - temp.x - foldyinity/* + param[0]*/;	// param removed as it splatters julia
		temp1.y = foldyinitx - temp.y + foldxinity/* + param[1]*/;
		}
	    else
		{
//		z->x = (foldxinitx + q->x - foldyinity + param[0]);
//		z->y = (foldyinitx + q->y + foldxinity + param[1]);
		temp1.x = foldxinitx + temp.x - foldyinity/* + param[0]*/;
		temp1.y = foldyinitx + temp.y + foldxinity/* + param[1]*/;
		}
	    *z = temp1;

	    return FractintBailoutTest(z);

	case BARNSLEYM2:					// An unnamed Mandelbrot/Julia function from "Fractals Everywhere" by Michael Barnsley, p. 331, example 4.2
	case BARNSLEYJ2:
	case BARNSLEYM2FP:
	case BARNSLEYJ2FP:
	    // calculate intermediate products
	    foldxinitx = z->x * temp.x;
	    foldyinity = z->y * temp.y;
	    foldxinity = z->x * temp.y;
	    foldyinitx = z->y * temp.x;

		// orbit calculation
	    if (foldxinity + foldyinitx >= 0)
		{
		temp1.x = foldxinitx - temp.x - foldyinity/* + param[0]*/;	// param removed as it splatters julia
		temp1.y = foldyinitx - temp.y + foldxinity/* + param[1]*/;
		}
	    else
		{
		temp1.x = foldxinitx + temp.x - foldyinity/* + param[0]*/;
		temp1.y = foldyinitx + temp.y + foldxinity/* + param[1]*/;
		}
	    *z = temp1;
	    return FractintBailoutTest(z);


	case BARNSLEYM3:					// An unnamed Mandelbrot/Julia function from "Fractals Everywhere" by Michael Barnsley, p. 292, example 4.1 
	case BARNSLEYJ3:
	case BARNSLEYM3FP:
	case BARNSLEYJ3FP:
	    // calculate intermediate products
	    foldxinitx = z->x * z->x;
	    foldyinity = z->y * z->y;
	    foldxinity = z->x * z->y;

	    // orbit calculation
	    if (z->x > 0)
		{
		temp1.x = foldxinitx - foldyinity - 1.0/* + param[0]*/;	// param removed as it splatters julia
		temp1.y = foldxinity * 2/* + param[1]*/;
		}
	    else
		{
		temp1.x = foldxinitx - foldyinity - 1.0 + temp.x * z->x/* + param[0]*/;
		temp1.y = foldxinity * 2/* + param[1]*/;

		// This term added by Tim Wegner to make dependent on the imaginary part of the parameter. (Otherwise Mandelbrot is uninteresting).
		temp1.y += temp.y * z->x;
		}
	    *z = temp1;
	    return FractintBailoutTest(z);

	case COMPLEXNEWTON:
	case COMPLEXBASIN:
	    {
	    double	mod;
	    int		coloriter;
	    Complex	tmp, temp, cd1, New;
	    double	MPthreshold = 0.001;

	    // New = ((cdegree-1) * old**cdegree) + croot
	    //         ----------------------------------
	    //              cdegree * old**(cdegree-1)         

	    *color = *iteration;
	    cd1.x = cdegree.x - 1.0;
	    cd1.y = cdegree.y;
	    //    temp = CComplexPower(z, cd1);
	    temp = *z ^ cd1;
	    New = temp * *z;
	    tmp = New - croot;
	    if (tmp.CSumSqr() < MPthreshold)
		{
		if (subtype == 'N')
		    return(1);
		if (fabs(z->y) < .01)
		    z->y = 0.0;

		temp = z->CLog();
		tmp = temp * cdegree;
		mod = tmp.y / TWO_PI;
		coloriter = (long)mod;
		if (fabs(mod - coloriter) > 0.5)
		    {
		    if (mod < 0.0)
			coloriter--;
		    else
			coloriter++;
		    }
		coloriter += 2;
		if (coloriter < 0)
		    coloriter += 128;
		*iteration = coloriter;					// PHD 2009-10-13
		return(1);
		}
	    tmp = New * cd1;
	    tmp += croot;
	    //    tmp.x += croot.x;
	    //    tmp.y += croot.y;
	    cd1 = temp * cdegree;
	    *z = tmp / cd1;
	    return(0);
	    }

	case ESCHER:							// Science of Fractal Images pp. 185, 187
	    {
	    Complex oldtest, newtest, testsqr;
	    double  testsize = 0.0;
	    int	    testiter = 0;

	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    temp.x = sqr.x - sqr.y;					// standard Julia with C == (0.0, 0.0i)
	    temp.y = 2.0 * z->x * z->y;
	    oldtest.x = temp.x * 15.0;					// scale it
	    oldtest.y = temp.y * 15.0;
	    testsqr.x = sqr(oldtest.x);					// set up to test with user-specified ...
	    testsqr.y = sqr(oldtest.y);					//    ... Julia as the target set
	//    while (testsize <= rqlim && testiter < threshold) // nested Julia loop
	    while (TRUE) // nested Julia loop
		{
		if (testsize > rqlim)
		    break;
		if (testiter >= threshold)
		    break;
		newtest.x = testsqr.x - testsqr.y + param[0];
		newtest.y = 2.0 * oldtest.x * oldtest.y + param[1];
		testsize = (testsqr.x = sqr(newtest.x)) + (testsqr.y = sqr(newtest.y));
		oldtest = newtest;
		testiter++;
		}
	    if (testsize > rqlim)
		{
		*z = temp;
		return FractintBailoutTest(z);
		}
	    else							// make distinct level sets if point stayed in target set
		{
		//	iteration = ((3L * iteration) % 255L) + 1L;
		*iteration = ((3 * *iteration) % 255) + 1;
		return 1;
		}
	    }

	case MANDELLAMBDAFP:				// variation of classical Mandelbrot/Julia
	case MANDELLAMBDA:
	case LAMBDAFP:
	case LAMBDA:
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;

	    sqr.x = z->x - sqr.x + sqr.y;
	    sqr.y = -(z->y * z->x);
	    sqr.y += sqr.y + z->y;

	    z->x = temp.x * sqr.x - temp.y * sqr.y;
	    z->y = temp.x * sqr.y + temp.y * sqr.x;
	    return BailoutTest(z, sqr);

	case PHOENIXFP:
	case PHOENIX:
	case MANDPHOENIXFP:
	case MANDPHOENIX:
	    if (PhoenixType == ZERO)
		return(PhoenixFractal(z, q));
	    else if (PhoenixType == PLUS)
		return(PhoenixPlusFractal(z, q));
	    else
		return(PhoenixMinusFractal(z, q));
	    return 1;					// just to shut up compiler warnings

	case MANDPHOENIXFPCPLX:
	case MANDPHOENIXCPLX:
	case PHOENIXCPLX:
	case PHOENIXFPCPLX:
	    if (PhoenixType == ZERO)
		return(PhoenixFractalcplx(z, q));
	    else if (PhoenixType == PLUS)
		return(PhoenixCplxPlusFractal(z, q));
	    else
		return(PhoenixCplxMinusFractal(z, q));
	    return 1;					// just to shut up compiler warnings


	case LJULIAZPOWER:
	case LMANDELZPOWER:
	case FPMANDELZPOWER:
	    *z = *z ^ temp2;
	    *z += *q;
	    return FractintBailoutTest(z);

	case FPMANZTOZPLUSZPWR:
	case FPJULZTOZPLUSZPWR:
	    temp = z->CPolynomial((int)param[2]);
	    *z = *z ^ *z;
	    *z = temp + *z + *q;
	    return FractintBailoutTest(z);

	case MARKSMANDELFP:				// Mark Peterson's variation of "lambda" function
	case MARKSMANDEL:
	case MARKSJULIAFP:
	case MARKSJULIA:
	    // Z1 = (C^(exp-1) * Z**2) + C
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    temp.x = sqr.x - sqr.y;
	    temp.y = z->x * z->y * 2;

	    temp1.x = Coefficient.x * temp.x - Coefficient.y * temp.y + t.x;
	    temp1.y = Coefficient.x * temp.y + Coefficient.y * temp.x + t.y;
	    *z = temp1;
	    return BailoutTest(z, sqr);

	case QUATFP:
	case QUATJULFP:
	    {
	    double a0, a1, a2, a3, n0, n1, n2, n3, magnitude;
	    a0 = z->x;
	    a1 = z->y;
	    a2 = temp.x;
	    a3 = temp.y;

	    n0 = a0 * a0 - a1 * a1 - a2 * a2 - a3 * a3 + qc;
	    n1 = 2 * a0*a1 + qci;
	    n2 = 2 * a0*a2 + qcj;
	    n3 = 2 * a0*a3 + qck;
	    // Check bailout
	    magnitude = a0 * a0 + a1 * a1 + a2 * a2 + a3 * a3;
	    if (magnitude > rqlim) {
		return 1;
		}
	    z->x = n0;
	    z->y = n1;
	    temp.x = n2;
	    temp.y = n3;
	    return(0);
	    }

	case SIERPINSKI:				// following code translated from basic - see "Fractals Everywhere" by Michael Barnsley, p. 251, Program 7.1.1
	case SIERPINSKIFP:
	    temp.x = z->x + z->x;
	    temp.y = z->y + z->y;
	    if (z->y > 0.5)
		temp.y = temp.y - 1;
	    else if (z->x > 0.5)
		temp.x = temp.x - 1;
	    // end barnsley code
	    *z = temp;
	    return FractintBailoutTest(z);

	case TETRATEFP:					// Tetrate(XAXIS) { c=z=pixel: z=c^z, |z|<=(P1+3)
	    *z = *q ^ *z;
	    return FractintBailoutTest(z);

	case UNITYFP:					// Unity Fractal - brought to you by Mark Peterson - you won't find this in any fractal books unless they saw it here first - Mark invented it!
	    temp.x = sqr(z->x) + sqr(z->y);
	    //   if((XXOne > 2.0) || (fabs(XXOne - 1.0) < ddelmin))
	    if ((temp.x > 2.0) || (fabs(temp.x - 1.0) < 0.01))
		return(1);
	    z->y = (2.0 - temp.x)* z->x;
	    z->x = (2.0 - temp.x)* z->y;
	    temp = *z;					// PHD added this line
	    return(0);

	case VL:					// Beauty of Fractals pp. 125 - 127
	    {
	    double a, b, ab, half, u, w, xy;

	    half = param[0] / 2.0;
	    xy = z->x * z->y;
	    u = z->x - xy;
	    w = -z->y + xy;
	    a = z->x + param[1] * u;
	    b = z->y + param[1] * w;
	    ab = a * b;
	    temp.x = z->x + half * (u + (a - ab));
	    temp.y = z->y + half * (w + (-b + ab));
	    *z = temp;
	    return FractintBailoutTest(z);
	}


	case MAGNET1M:
	case MAGNET1J:
	    return (RunTierazonFunctions(102, z, q, SpecialFlag, iteration));

	case MAGNET2M:
	case MAGNET2J:
	    return (RunTierazonFunctions(103, z, q, SpecialFlag, iteration));

	case LYAPUNOV:
	    {
	    double ra, rb, rc;			// r in the direction of x, y, and z or A, B, C

	    rc = param[2];			// add z component C - good for animation
	    overflow = FALSE;
	    if (param[0] == 1) 
		Population = (1.0 + rand()) / (2.0 + RAND_MAX);
	    else if (param[0] == 0) 
		{
		if (fabs(Population) > BIG || Population == 0 || Population == 1)
		    Population = (1.0 + rand()) / (2.0 + RAND_MAX);
		}
	    else Population = param[0];
		{
		ra = c->x;
		rb = c->y;
		}
	    *color = lyapunov_cycles(filter_cycles, ra, rb, rc);
	    z->x = Rate;						// sort of to allow filters and FwdDiff to give something!!!
	    z->y = Population;
	    if (TrueCol->inside_colour > 0 && *color == 0)
		*color = TrueCol->inside_colour;
	    else if (*color >= colors)
		*color = colors - 1;
	//    (*plot)((WORD)col, (WORD)row, color);
	    *iteration = *color;
	//    return color;
	    return TRUE;
	    }


	case FROTH:		// Froth Fractal type - per pixel 1/2/g, called with row & col set
	    {
	    // These points were determined imperically and verified experimentally 
	    // using the program WL-Plot, a plotting program which has a mode for 
	    // orbits of recursive relations. 
#define CLOSE    1e-6		// seems like a good value 
#define SQRT3    1.732050807568877193
#define A        1.02871376822
#define B1       (A/2)
#define M2       SQRT3
#define B2       (-A)
#define M3       (-SQRT3)
#define B3       (-A)
#define X1MIN   -1.04368901270
#define X1MAX    1.33928675524
#define XMIDT   -0.339286755220
#define X2MAX1   0.96729063460
#define XMIDR    0.61508950585
#define X3MIN1  -0.22419724936
#define X2MIN2  -1.11508950586
#define XMIDL   -0.27580275066
#define X3MAX2   0.07639837810
#define FROTH_BITSHIFT  28

	    int		found_attractor = 0;
	    double	magnitude;
	    double	x, y, nx, ny, x2, y2;
	    double	close = CLOSE;
	    double	a = A;
	    double	b1 = B1;
	    double	xmidt = XMIDT;
	    double	m2 = M2;
	    double	b2 = B2;
	    double	m3 = M3;
	    double	b3 = B3;
	    double	x1min = X1MIN;
	    double	x1max = X1MAX;
	    double	x2max1 = X2MAX1;
	    double	xmidr = XMIDR;
	    double	x3min1 = X3MIN1;
	    double	x2min2 = X2MIN2;
	    double	xmidl = XMIDL;
	    double	x3max2 = X3MAX2;

	    long	color = 0;

	    x = c->x;
	    y = c->y;
	    magnitude = (x2 = sqr(x)) + (y2 = sqr(y));
	    while (!found_attractor && (magnitude < rqlim) && (color < threshold))
		{
		// simple formula: z = z^2 + conj(z*(-1+ai))
		// but it's the attractor that makes this so interesting 
		nx = x2 - y2 - x - a * y;
		ny = (x + x)*y - a * x + y;
		x = nx;
		y = ny;
		if (frothsix)					// repeat mapping 
		    {
		    nx = sqr(x) - sqr(y) - x - a * y;
		    ny = (x + x)*y - a * x + y;
		    x = nx;
		    y = ny;
		    }
		magnitude = (x2 = sqr(x)) + (y2 = sqr(y));
		color++;

		//	 if (show_orbit)
		//	    plot_orbit(x, y, -1);

		if (x > x1min && x < x1max && fabs(b1 - y) < close)
		    {
		    if (!frothsix || x < xmidt)
			found_attractor = 1;
		    else
			found_attractor = 2;

		    }
		else if (fabs(m2*x + b2 - y) < close)
		    {
		    if (x > xmidr && x < x2max1)
			found_attractor = !frothsix ? 2 : 4;
		    else if (x > x3min1 && x < xmidr)
			found_attractor = !frothsix ? 3 : 6;
		    }
		else if (fabs(m3*x + b3 - y) < close)
		    {
		    if (x > x2min2 && x < xmidl)
			found_attractor = !frothsix ? 2 : 3;
		    else if (x > xmidl && x < x3max2)
			found_attractor = !frothsix ? 3 : 5;
		    }
		}

	    // inside - Here's where non-palette based images would be nice.  Instead, we'll use blocks of (colors-1)/3 or (colors-1)/6 and use special froth  
	    // color maps in attempt to replicate the images of James Alexander.    
	    if (found_attractor)
		{
		if (colors >= 256)
		    {
		    if (!froth_altcolor)
			{
			if (color > froth_shades)
			    color = froth_shades;
			}
		    else
			color = froth_shades * color / threshold;
		    if (color == 0)
			color = 1;
		    color += froth_shades * (found_attractor - 1);
		    }
		else if (colors >= 16)
		    {						// only alternate coloring scheme available for 16 colors 
		    long lshade;

		    // Trying to make a better 16 color distribution.
		    // Since their are only a few possiblities, just handle each case. 
		    // This is a mostly guess work here. 
		    lshade = ((long)color << 16) / threshold;
		    if (!frothsix)
			{
			if (lshade < 2622)       // 0.04
			    color = 1;
			else if (lshade < 10486) // 0.16 
			    color = 2;
			else if (lshade < 23593) // 0.36 
			    color = 3;
			else if (lshade < 41943) // 0.64 
			    color = 4;
			else
			    color = 5;
			color += 5 * (found_attractor - 1);
			}
		    else
			{
			if (lshade < 10486)      // 0.16 
			    color = 1;
			else
			    color = 2;
			color += 2 * (found_attractor - 1);
			}
		    }
		else					// use a color corresponding to the attractor
		    color = found_attractor;
		oldcolor = color;
		}
	    else if (color >= threshold)
		color = oldcolor;			// inside, but didn't get sucked in by attractor. 
	    else					// outside 
		color = 0;				// all outside points are color 0 

	  //   (*plot)((WORD)col, (WORD)row, color);
	    *iteration = color;
	    return TRUE;

#undef CLOSE
#undef SQRT3
#undef A
#undef B1
#undef XMIDT
#undef M2
#undef B2
#undef M3
#undef B3
#undef X1MIN
#undef X1MAX
#undef X2MAX1
#undef XMIDR
#undef X3MIN1
#undef X2MIN2
#undef XMIDL
#undef X3MAX2
//#undef FROTH_BITSHIFT
#undef D_TO_L
	    }




	}
    return 0;
    }

/**************************************************************************
    These functions are called by Phoenic and MandelPheonix fractals
**************************************************************************/

int	CPixel::PhoenixFractal(Complex *z, Complex *q)
    {
	// z(n+1) = z(n)^2 + p + qy(n),  y(n+1) = z(n)
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;
    temp1.x = z->x * z->y;
    temp4.x = sqr.x - sqr.y + temp.x + (temp.y * temp3.x);
    temp4.y = (temp1.x + temp1.x) + (temp.y * temp3.y);
    temp3 = *z;							// set temp3 to Y value
    *z = temp4;
    return BailoutTest(z, sqr);
    }

int	CPixel::PhoenixPlusFractal(Complex *z, Complex *q)
    {
	// z(n+1) = z(n)^(degree-1) * (z(n) + p) + qy(n),  y(n+1) = z(n)
    int i;
    Complex oldplus, newminus;
    oldplus = *z;
    temp1 = *z;
    for (i = 1; i < PhoenixDegree; i++)
        {							// degree >= 2, degree=degree-1 in setup
	temp1 = temp1 * *z;					// = old^(degree-1)
	}
    oldplus.x += temp.x;
    newminus = oldplus * temp1;
    temp4.x = newminus.x + (temp.y * temp3.x);
    temp4.y = newminus.y + (temp.y * temp3.y);
    temp3 = *z;							// set temp3 to Y value
    *z = temp4;
    return FractintBailoutTest(z);
    }

int	CPixel::PhoenixMinusFractal(Complex *z, Complex *q)
    {
	// z(n+1) = z(n)^(degree-2) * (z(n)^2 + p) + qy(n),  y(n+1) = z(n)
    int i;
    Complex oldsqr, newminus;
    oldsqr = *z * *z;
    temp1 = *z;
    for (i = 1; i < PhoenixDegree; i++) {			// degree >= 3, degree=degree-2 in setup
	temp1 = temp1 * *z;					// = old^(degree-1)
	}
    oldsqr.x += temp.x;
    newminus = temp1 * oldsqr;
    temp4.x = newminus.x + (temp.y * temp3.x);
    temp4.y = newminus.y + (temp.y * temp3.y);
    temp3 = *z;							// set temp3 to Y value
    *z = temp4;
    return FractintBailoutTest(z);
    }

int	CPixel::PhoenixFractalcplx(Complex *z, Complex *q)
    {
    // z(n+1) = z(n)^2 + p1 + p2*y(n),  y(n+1) = z(n)
    sqr.x = z->x * z->x;
    sqr.y = z->y * z->y;
    temp1.x = z->x * z->y;
    temp4.x = sqr.x - sqr.y + q->x + (temp2.x * temp3.x) - (temp2.y * temp3.y);
    temp4.y = (temp1.x + temp1.x) + q->y + (temp2.x * temp3.y) + (temp2.y * temp3.x);
    temp3 = *z;							// set tmp3 to Y value
    *z = temp4;
    return BailoutTest(z, sqr);
    }

int	CPixel::PhoenixCplxPlusFractal(Complex *z, Complex *q)
    {
    // z(n+1) = z(n)^(degree-1) * (z(n) + p) + qy(n),  y(n+1) = z(n)
    int i;
    Complex oldplus, newminus;
    oldplus = *z;
    temp1 = *z;
    for (i = 1; i < PhoenixDegree; i++) {			// degree >= 2, degree=degree-1 in setup
	temp1 = *z * temp1;					// = old^(degree-1) 
	}
    oldplus.x += q->x;
    oldplus.y += q->y;
    newminus = temp1 * oldplus;
    temp1 = temp2 * temp3;
    temp4.x = newminus.x + temp1.x;
    temp4.y = newminus.y + temp1.y;
    temp3 = *z;							// set temp3 to Y value
    *z = temp4;
    return FractintBailoutTest(z);
    }

int	CPixel::PhoenixCplxMinusFractal(Complex *z, Complex *q)
    {
    // z(n+1) = z(n)^(degree-2) * (z(n)^2 + p) + qy(n),  y(n+1) = z(n)
    int i;
    Complex oldsqr, newminus;

    oldsqr = *z * *z;
    temp1 = *z;
    for (i = 1; i < PhoenixDegree; i++)
	{							// degree >= 3, degree=degree-2 in setup
	temp1 = *z * temp1;
	}
    oldsqr.x += q->x;
    oldsqr.y += q->y;
    newminus = temp1 * oldsqr;
    temp1 = temp2 * temp3;
    temp4.x = newminus.x + temp1.x;
    temp4.y = newminus.y + temp1.y;
    temp3 = *z;							// set tmp2 to Y value
    *z = temp4;
    return FractintBailoutTest(z);
    }

/**************************************************************************
    These functions are called by Lyapunov fractals
**************************************************************************/

int	CPixel::lyapunov_cycles(int filter_cycles, double a, double b, double c)
    {
    int color, count, i, lnadjust;
    double lyap, total, temp;
    // e10=22026.4657948  e-10=0.0000453999297625

    for (i = 0; i < filter_cycles; i++) 
	{
	for (count = 0; count < lyaLength; count++) 
	    {
//	    Rate = lyaRxy[count] ? a : b;
	    Rate = ((lyaRxy[count] == 2) ? c : (lyaRxy[count] == 1) ? a : b);
	    //	    if (curfractalspecific->orbitcalc()) {
	    if (BifurcLambda(Rate, &Population)) 
		{
		overflow = TRUE;
		goto jumpout;
		}
	    }
	}
    total = 1.0;
    lnadjust = 0;
    for (i = 0; i < threshold / 2; i++) 
	{
	for (count = 0; count < lyaLength; count++) 
	    {
	    Rate = ((lyaRxy[count] == 2) ? c : (lyaRxy[count] == 1) ? a : b);
//	    Rate = lyaRxy[count] ? a : b;
	    //	    if (curfractalspecific->orbitcalc()) {
	    if (BifurcLambda(Rate, &Population)) 
		{
		overflow = TRUE;
		goto jumpout;
		}
	    temp = fabs(Rate - 2.0*Rate*Population);
	    if ((total *= (temp)) == 0) 
		{
		overflow = TRUE;
		goto jumpout;
		}
	    }
	while (total > 22026.4657948) 
	    {
	    total *= 0.0000453999297625;
	    lnadjust += 10;
	    }
	while (total < 0.0000453999297625) 
	    {
	    total *= 22026.4657948;
	    lnadjust -= 10;
	    }
	}

jumpout:
    if (overflow || total <= 0 || (temp = log(total) + lnadjust) > 0)
	color = 0;
    else 
	{
	if (logval)
	    lyap = -temp / ((double)lyaLength*i);
	else
	    lyap = 1 - exp(temp / ((double)lyaLength*i));
	color = 1 + (int)(lyap * (colors - 1));
	}
    return color;
    }

/**************************************************************************
    Colour maps which attempt to replicate the Frothy Basin images of James Alexander. 
**************************************************************************/

void	CPixel::set_Froth_palette(HWND hwnd)
    {
    char *mapname;

    if (colors >= 16)
	{
	if (colors >= 256)
	    {
	    if (frothsix)
		mapname = froth6_256c;
	    else
		mapname = froth3_256c;
	    }
	else				// colors >= 16 
	    {
	    if (frothsix)
		mapname = froth6_16c;
	    else
		mapname = froth3_16c;
	    }
//	rotate(1);
	}
    }

/**************************************************************************
    Bailout Test
**************************************************************************/

bool	CPixel::FractintBailoutTest(Complex *z)
    {
    Complex TempSqr;
    double  magnitude;
    double  manhmag;
    double  manrmag;

    switch (BailoutTestType)
	{
	case BAIL_MOD:
	    TempSqr.x = sqr(z->x);
	    TempSqr.y = sqr(z->y);
	    magnitude = TempSqr.x + TempSqr.y;
	    return (magnitude >= rqlim);

	case BAIL_REAL:
	    TempSqr.x = sqr(z->x);
	    return (TempSqr.x >= rqlim);
	
	case BAIL_IMAG:
	    TempSqr.y = sqr(z->y);
	    return (TempSqr.y >= rqlim);

	case BAIL_OR:
	    TempSqr.x = sqr(z->x);
	    TempSqr.y = sqr(z->y);
	    return (TempSqr.x >= rqlim || TempSqr.y >= rqlim);
	
	case BAIL_AND:
	    TempSqr.x = sqr(z->x);
	    TempSqr.y = sqr(z->y);
	    return (TempSqr.x >= rqlim && TempSqr.y >= rqlim);

	case MANH:
	    manhmag = fabs(z->x) + fabs(z->y);
	    return ((manhmag * manhmag) >= rqlim);

	case MANR:
	    manrmag = z->x + z->y;	    // don't need abs() since we square it next
	    return ((manrmag * manrmag) >= rqlim);

	default:
	    TempSqr.x = sqr(z->x);
	    TempSqr.y = sqr(z->y);
	    magnitude = TempSqr.x + TempSqr.y;
	    return (magnitude >= rqlim);
	}
    }


