/*
    FUNCTIONS.CPP a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	"manp.h"
#include	"fractype.h"
#include	"Complex.h"
#include	"pixel.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::InitFunctions(WORD type, Complex *z, Complex *q)
    {
    switch (type)
	{
	case MANDELFP:					// Mandelbrot
	case MANDEL:					// to handle fractint par files
	case JULIA:					// to handle fractint par files
	case JULIAFP:					// like he said
/**************************************************************************
    Run thorn type fractals
    Written by Paul Bourke
    Fractal attributed to Andrew Wayne Graff, alternatively named the "Secant Sea"
    Contribution by Dane Vandeputte
    Original: May 2004, Updated: May 2012
    Sample C source code -- Contribution by Adam Majewski
    This fractal is created by iterating the functions (xn,yn) and shading each pixel (x0,y0)
    depending on whether or how fast the series escapes to infinity.
    The parameter (cx,cy) is gives (slightly) different fractal images.
**************************************************************************/
	case THORN:					// Thorn Fractal
/**************************************************************************
    The Burning Ship fractal, first described and created by Michael Michelitsch and Otto E. R�ssler in 1992, is generated by iterating the function
    in the complex plane which will either escape or remain bounded. The difference between this calculation and that for the Mandelbrot set is
    that the real and imaginary components are set to their respective absolute values before squaring at each iteration. The mapping is non-analytic
    because its real and imaginary parts do not obey the Cauchy�Riemann equations.
***************************************************************************/
	case BURNINGSHIP:				// Burning Ship
	    if (!juliaflag)
		{
		z->x = q->x + param[0];
		z->y = q->y + param[1];
		}
	    sqr = 0;
	    real_imag = 0.0;
	    break;
/**************************************************************************
    The Burning Ship fractal for Higher Powers
    The function that is iterated to generate this fractal can be generalized
    to any arbitrary power:
	z = [|Re(z)| + i |Im(z)|]N + c
    As with the Mandelbrot set, this creates shapes that resemble the original
    shape but have increasingly many attached arms or buds. Below is a table
    showing the first few of these.
    With N=2 we get the Burning Ship. The N=3 version has been called the "Bird of Prey."

    The odd powers are symmetric around an axes that are oriented at +/- 45 degrees.
    This is probably due to the symmetry of the absolute value functions. This
    property has been mentioned at this site (external link) back in 1999.

    These images are all centered at (0,0) in the complex number plane, and all
    have a size of 5 horizontally and 4 vertically. This size is a bit too big
    and results in a lot of blank space in the images, but the N=2 fractal is
    offset from the origin, so the larger size is needed to have all the images
    drawn with a consistent location and size so they can be compared accurately.

    Here are a few images of the Cubic Burning Ship. The "bug" images are named
    because they look like actual bugs, not because of a software error! These
    are the first deep zoom images ever published of the cubic Burning Ship fractal.
    The left image size is 1.0e-19, and the right image is 1.0e-23.
    http://www.hpdz.net/StillImages/BurningShip.htm
	***************************************************************************/
	case BURNINGSHIPPOWER:				// Burning Ship to higher power
	case POWER:					// Power
	case JULIA4FP:
	case JULIA4:
	case MANDEL4FP:
	case MANDEL4:
	    *degree = (int)param[0];
	    if (*degree < 1)
		*degree = 1;
		if (type == JULIA4FP || type == JULIA4 || type == MANDEL4FP || type == MANDEL4)	// handle legacy Fractint types
		    *degree = 4;
	    if (!juliaflag)
		{
		z->x = q->x + param[1];
		z->y = q->y + param[2];
		}
	    break;

	case CUBIC:					// Art Matrix Cubic
	    switch ((int)param[0])
		{
		case 0:
		    subtype = 'B';
		    break;
		case 1:
		    subtype = 'C';
		    break;
		case 2:
		    subtype = 'F';
		    break;
		case 3:
		    subtype = 'K';
		    break;
		default:
		    subtype = 'B';
		    break;
		}
	    if ((int)param[1] < 0)
		special = 0;
	    else
		special = (int)param[1];

	    period_level = FALSE;			// no periodicity checking
	    if (subtype == 'B')				// CBIN 
		{
		t3 = *q * 3;				// T3 = 3*T
		t2 = q->CSqr();				// T2 = T*T
		a = (t2 + 1) / t3;			// A  = (T2 + 1)/T3
							// B  = 2*A*A*A + (T2 - 2)/T3    
		temp = a.CCube() * 2;			// 2*A*A*A
		b = (t2 - 2) / t3 + temp;		// B  = 2*A*A*A + (T2 - 2)/T3
		}
	    else if (subtype == 'C' || subtype == 'F')	// CCIN or CFIN
		{
		a = *q;					// A = T
							// find B = T + 2*T*T*T
		temp = q->CCube();			// B = T*T*T
		if (subtype == 'C')
		    b = temp + temp + *q;		// B = B * 2 + T
		else
		    {
		    b = (temp - *q) * 2;		// B = B * 2 - 2 * T
		    a2 = a + a;
		    }
		}
	    else if (subtype == 'K')			// CKIN 
		{
		a = 0;
		v = 0;
		b = *q;					// B = T
		}
	    aa3 = a.CSqr() * 3;				// AA3 = A*A*3
	    if (!juliaflag)
		*z = -a;				// Z = -A
	    break;

	case SPECIALNEWT:				// Art Matrix Newton
	    l2 = q->CSqr();				// L2 = L*L
	    a = -l2 + 0.25;				// A = ( .25,0) - L2
	    b = -l2 - 0.75;				// B = (-.75,0) - L2 
	    lm5 = *q - 0.5;				// LM5 = L - (.5,0)
	    lp5 = *q + 0.5;				// LP5 = L + (.5,0)
	    break;

	case MATEIN:					// Art Matriuc Matein fractal
	    if ((absolute = q->CSumSqr()) > 1.0)
		return(-1);				// not inside set
	    if (!juliaflag)
		*z = 1;

	    for (int i = 0; i < 100; ++i)		// DO 300 I = 1,100 
		{
		temp = z->CInvert();			// 300  Z = L*(Z + 1/Z)
		*z = *q * (*z + temp);
		}

	    distance = 1.0;				// D = 1
	    oz = z->CInvert();				// OZ = 1/Z
	    break;

	case SINFRACTAL:				// Sine
	    if (!juliaflag)
		*z = param[3];
	    break;

	case REDSHIFTRIDER:				// RedShiftRider    a*z^2 +/- z^n + c
	    a.x = param[0];
	    a.y = param[1];
	    *degree = (int)param[2];
	    if (!juliaflag)
		{
		z->x = q->x + param[3];
		z->y = q->y + param[4];
		}
	    break;

	case TALIS:					// Talis Power    Z = Z^N/(M + Z^(N-1)) + C
	    *degree = (int)param[0];
	    if (*degree < 1)
		*degree = 1;
	    if (!juliaflag)
		{
		z->x = q->x + param[2];
		z->y = q->y + param[3];
		}
	    break;

	case POLYNOMIAL:				// Polynomial
	    if (!juliaflag)
		{
		z->x = q->x + param[0];
		z->y = q->y + param[1];
		}
	    for (int i = 0; i < MAXPOLYDEG; i++)	// find highest order of polynomial to help with forward differencing calculations
		{
		if (param[2 + i] != 0.0)
		    {
		    fractalspecific[type].SlopeDegree = MAXPOLYDEG - i;
		    break;
		    }
		}
	    break;

	case RATIONALMAP:				// Art Matrix Rational Map 
	    switch ((int)param[0])
		{
		case 0:
		    subtype = 'A';
		    break;
		case 1:
		    subtype = 'B';
		    break;
		default:
		    subtype = 'A';
		    break;
		}
	    if ((int)param[1] < 0)
		special = 0;
	    else
		special = (int)param[1];
	    if (threshold != OldThreshold)
		{
		OldThreshold = threshold;
		int gap = threshold / 16;			// split the colour map into 16 equal parts
		for (int i = 0; i < 4; i++)
		    {
		    penp[i] = penpref[i] * gap;
		    penn[i] = pennref[i] * gap;
		    }
		}

	    if (subtype == 'A')
		{
		cmcc = *q - q->CSqr();			// CMCC = C - C*C
		temp = -*q + 2;
		a = temp / cmcc;			// A = (2 - C)/CMCC
		temp = cmcc + 1;
		b = -temp / cmcc;			// B = -(CMCC + 1)/CMCC

							// ALPHA = 1/(C*C * (B + B + B*B/A) * (2*A*C + B))
		temp = a * *q * 2 + b;			// 2*A*C + B
		temp1 = b.CSqr() / a + b + b;		// (B + B + B*B/A)
		temp3 = q->CSqr()*temp1*temp;
		alpha = temp3.CInvert();
		}
	    else if (subtype == 'B')
		{
		a = *q;
		b = a + 1;
		temp = a.CSqr() - 1;
		alpha = a / temp;			// ALPHA = A/(A*A - 1)
		}
	    else
		return(ERROR);				// unknown subtype

							// ESCAPE  =   4/ABS(ALPHA)
							// ESCAPE  =   ESCAPE*ESCAPE
	    if (alpha.x != 0.0 || alpha.y != 0.0)
		escape = 16.0 / alpha.CSumSqr();
	    else
		return(FALSE);				// no naughty division
	    epsilon = 0.000001 / escape;		// EPSILN = 0.000001/ESCAPE 

	    der = 1.0;					// DER = 1.0 
	    if (!juliaflag)
		{					// Z = -B/(A + A)
		temp = -a * 2;
		*z = b / temp;
		}
	    // iterating Z  = 1/(A*Z*Z + B*Z + 1) has various proterties:
	    // 	   Z  = 1/(A*Z*Z + B*Z + 1)
	    // Julia   Z  = 1/(A*Z*Z + B*Z + 1)
	    // Julia   Z  = 1/(A*Z*Z + B*Z + 1)
	    // ?????   Z  = 1/(A*Z*Z + B*Z + 1)

	    int	zcount;
	    switch (subtype)
		{
		case 'A':
		    if (juliaflag)
			zcount = 4;
		    else
			zcount = 2;
		    break;
		case 'B':
		    zcount = 3;
		    break;
		}

	    for (int i = 0; i < zcount; ++i)
		{
							// 1/(A*Z*Z + B*Z + 1)
		temp = b * *z + 1;			// B*Z + 1
		temp1 = z->CSqr()*a + temp;		// (A*Z*Z + B*Z + 1)
		*z = temp1.CInvert();			// Z = 1/(A*Z*Z + B*Z + 1)
		}
	    break;

	case MANDELDERIVATIVES:				// a group of Mandelbrot Derivatives
	    InitManDerFunctions(subtype, z, q);
	    break;

	case TIERAZON:					// a group of Tierazon fractals
	    InitTierazonFunctions(subtype, z, q);
	    break;

	case NEWTONAPPLE:				// a specific Tierazon fractal
	    InitTierazonFunctions(55, z, q);
	    break;

	case NEWTONFLOWER:				// a specific Tierazon fractal
	    InitTierazonFunctions(35, z, q);
	    break;

	case NEWTONMSET:				// a specific Tierazon fractal
	    InitTierazonFunctions(52, z, q);
	    break;

	case NEWTONPOLYGON:				// a specific Tierazon fractal
	    InitTierazonFunctions(31, z, q);
	    break;

	case NEWTONCROSS:				// a specific Tierazon fractal
	    InitTierazonFunctions(59, z, q);
	    break;

	case NEWTONJULIANOVA:				// a specific Tierazon fractal
	    InitTierazonFunctions(2, z, q);
	    break;

	case NEWTONVARIATION:				// a specific Tierazon fractal
	    InitTierazonFunctions(87, z, q);
	    break;

	case QUARTET1:				// a specific Tierazon fractal
	    InitTierazonFunctions(85, z, q);
	    break;

	case QUARTET2:				// a specific Tierazon fractal
	    InitTierazonFunctions(85, z, q);
	    break;

	case QUARTET3:				// a specific Tierazon fractal
	    InitTierazonFunctions(96, z, q);
	    break;

	case NOVA:
	    if (*degree < 2)
		*degree = 2;
	    z->x = 1.0 + param[1];
	    z->y = 0.0 + param[2];
	    break;

	case QUAD:				// a specific Tierazon fractal
	    InitTierazonFunctions(90, z, q);
	    break;

	case RAMONSIN:				// a specific Tierazon fractal
	    InitTierazonFunctions(116, z, q);
	    break;

	case RAMONCOS:				// a specific Tierazon fractal
	    InitTierazonFunctions(117, z, q);
	    break;

	case FORMULA05:				// a specific Tierazon fractal
	    InitTierazonFunctions(5, z, q);
	    break;

	case TEDDY:				// a specific Tierazon fractal
	    InitTierazonFunctions(104, z, q);
	    break;

/*
#define NEWTONFLOWER		186
#define NEWTONMSET		190
#define NEWTONPOLYGON		174
#define NEWTONCROSS		193
#define NEWTONJULIANOVA		191
#define NEWTONVARIATION		207	
#define QUARTET1		194
#define QUARTET2		195
#define QUARTET3		196
#define NOVA			208	
#define QUAD			201
#define RAMONSIN		197
#define RAMONCOS		198
#define FORMULA05		199
#define TALIS			192
#define TEDDY			189

*/
	case TETRATION:				// a specific Tierazon fractal
	    InitTetration(z, q);
	    break;

	case KLEINIAN:				// a specific Tierazon fractal
	    InitKleinian(z, q);
	    break;
	}

    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::RunFunctions(WORD type, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration)
    {
    switch (type)
	{
	case MANDELFP:					// Mandelbrot
	case MANDEL:					// to handle fractint par files
	case JULIA:					// to handle fractint par files
	case JULIAFP:					// like he said

	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = z->x * z->y;
	    z->x = q->x + sqr.x - sqr.y;
	    z->y = q->y + real_imag + real_imag;
	    return ((sqr.x + sqr.y) >= rqlim);
/*
	    {
	    Complex z2 = {z->x * z->x, z->y * z->y};
	    Complex one = {1.0,0.0};
	    *z = (*z * *z * *z) / (one + z2) + *q;
	    return (z->CSumSqr() >= rqlim);
	    }

	    {
	    z->y += q->y * sin(z->x);
	    z->x += q->x * z->y;
	    return (z->CSumSqr() >= rqlim);
	    }
*/
	case POWER:					// Power
	case JULIA4FP:
	case JULIA4:
	case MANDEL4FP:
	case MANDEL4:
	    *z = z->CPolynomial(*degree);
	    *z = *z + *q;
	    return (z->CSumSqr() >= rqlim);

	case BURNINGSHIP:				// Burning Ship
	    sqr.x = z->x * z->x;
	    sqr.y = z->y * z->y;
	    real_imag = fabs(z->x * z->y);
	    z->x = sqr.x - sqr.y + q->x;
	    z->y = real_imag + real_imag - q->y;
	    return ((sqr.x + sqr.y) >= rqlim);

	case BURNINGSHIPPOWER:				// Burning Ship to higher power
	    z->x = fabs(z->x);
	    z->y = -fabs(z->y);
	    *z = z->CPolynomial(*degree);
	    *z = *z + *q;
	    return (z->CSumSqr() >= rqlim);

	case CUBIC:					// Art Matrix Cubic
	    if (subtype == 'K')				// CKIN
		{
		*z = z->CCube() + b;			// Z = Z*Z*Z + B
		z->x += param[2];
		z->y += param[3];
		}
	    else
		{
		temp = z->CCube() + b;			// Z = Z*Z*Z + B
		*z = temp - aa3 * *z;			// Z = Z*Z*Z - AA3*Z + B
		z->x += param[2];
		z->y += param[3];
		}
	    if (z->CSumSqr() > 100.0)
		return (TRUE);
	    else
		{
		if (subtype == 'F')
		    {
		    if (q->CSumSqr() < 0.111111)
			{
			*iteration = special;
			*SpecialFlag = TRUE;			// for decomp and biomorph
			return (TRUE);
			}
		    v = *z + a2;
		    }
		else if (subtype == 'K')
		    v = *z - v;
		else
		    v = *z - a;
		if (v.CSumSqr() <= 0.000001)
		    {
		    *iteration = special;
		    *SpecialFlag = TRUE;			/* for decomp and biomorph */
		    return (TRUE);
		    }
		return (FALSE);
		}

	case SPECIALNEWT:				// Art Matrix Newton
	    {
	    if ((int)param[0] < 0)
		special = 2;
	    else
		special = (int)param[0];
	    Complex z2 = z->CSqr();			// z2 = z*z
							// Z  =  (2*Z*Z2 + A)/(3*Z2 + B)
	    Complex top = z2 * *z * 2 + a;
	    Complex bottom = z2 * 3 + b;
	    *z = top / bottom;
	    z->x += param[1];
	    z->y += param[2];
	    v = *z - 1;
	    //    v.x += param[1];
	    //    v.y += param[2];
	    if (v.CSumSqr() <= 0.000001)
		{
		phaseflag = 0;				// first phase
		return(TRUE);
		}
	    v = *z - lm5;
	    if (v.CSumSqr() <= 0.000001)
		{
		phaseflag = 1;				// second phase
		return(TRUE);
		}
	    v = *z + lp5;
	    if (v.CSumSqr() <= 0.000001)
		{
		phaseflag = 2;				// third phase
		return(TRUE);
		}
	    return(FALSE);
	    }

	case MATEIN:					// Art Matriuc Matein fractal
	    epsilon = 0.01;
	    escape = 10.0E20;
	    *z = *q * (*z + oz);			// Z = L*(Z + OZ)
	    z->x += param[0];
	    z->y += param[1];
	    oz = z->CInvert();				// OZ = 1/Z
	    temp = -oz / *z + 1;			// T = 1 - OZ/Z
							// D = D*ABSL*(REAL(T)*REAL(T) + IMAG(T)*IMAG(T))
	    distance = distance * absolute * temp.CSumSqr();

	    if (distance <= epsilon)
		{
		phaseflag = 0;			// first phase
		return(TRUE);
		}
	    if (distance > escape)
		{
		phaseflag = 1;			// second phase
		return(TRUE);
		}
	    return(FALSE);

/**************************************************************************
    Determine count before 'Z' becomes unstable
    Z = L*SIN(Z)
    sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/
	case SINFRACTAL:				// Sine
	    if (param[2] == 0.0)
		*z = *q * z->CSin();
	    else
		*z = *q + z->CSin();
	    z->x += param[0];
	    z->y += param[1];
	    return (z->CSumSqr() >= rqlim);

	case EXPFRACTAL:				// Exponential
	    {
	    int compare;

	    *degree = (int)(fabs(param[3]));
	    for (int i = 0; i < *degree; i++)
		*z = z->CExp();

	    if (param[4] == 0.0)
		*z = *q * *z;				// Z = L*EXP(Z)
	    else
		*z = *q + *z;				// Z = L*EXP(Z)
							// Complex Exponent: e^(x+iy) = (e^x) * cos(y) + i * (e^x) * sin(y) 
	    switch ((int)param[0])
		{
		case 0:
		    subtype = 'R';
		    compare = (z->x >= rqlim);
		    break;
		case 1:
		    subtype = 'I';
		    compare = (z->y >= rqlim);
		    break;
		case 2:
		    subtype = 'M';
		    compare = (z->x >= rqlim || z->x <= -rqlim || z->y >= rqlim || z->y <= -rqlim);
		    break;
		default:
		    subtype = 'R';
		    compare = (z->x >= rqlim);
		    break;
		}
	    z->x += param[1];
	    z->y += param[2];
	    if (compare)
		return(TRUE);
	    return(FALSE);
	    }

	case THORN:					// Thorn Fractal
	    {
	    double a1 = z->x;
	    double b1 = z->y;
	    z->x = a1 / cos(b1) + q->x;
	    z->y = b1 / sin(a1) + q->y;
	    return (z->CSumSqr() >= rqlim);
	    }

	case REDSHIFTRIDER:				// RedShiftRider    a*z^2 +/- z^n + c
	    *z = a * *z * *z + ((param[5] == 1.0) ? 1.0 : -1.0) * z->CPolynomial(*degree);
	    *z = *z + *q;
	    return (z->CSumSqr() >= rqlim);

	case TALIS:					// Talis Power    Z = Z^N/(M + Z^(N-1)) + C
	    {
	    double m = param[1];
	    a = z->CPolynomial(*degree - 1);
	    *z = (a * *z) / (m + a) + *q;
	    return (z->CSumSqr() >= rqlim);
	    }

	case POLYNOMIAL:				// Polynomial
	    {
	    Complex	InitialZ = *z;
	    Complex	FinalZ = { 0.0, 0.0 };

		{
		for (int m = 0; m < MAXPOLYDEG; m++)
		    {
		    Complex BigComplexTemp = InitialZ;
		    if (param[2 + m] != 0.0)
			{
			for (int k = 0; k < MAXPOLYDEG - m - 1; k++)
			    BigComplexTemp *= InitialZ;
			FinalZ += (BigComplexTemp * param[2 + m]);
			}
		    }
		*z = FinalZ + *q;
		}
	    return (z->CSumSqr() >= rqlim);
	    }

	case RATIONALMAP:				// Art Matrix Rational Map 
	    {
	    Complex az = a * *z;			// AZ = A*Z
	    temp = az * *z + 1;
	    temp1 = b * *z + temp;
	    *z = temp1.CInvert();			// Z = 1/(AZ*Z + B*Z + 1)
	    z->x += param[2];
	    z->y += param[3];
	    if (z->CSumSqr() > escape)
		{
		if ((alpha.x * z->y + alpha.y * z->x) <= 0.0)
		    *color = penp[*iteration % 4];
		else
		    *color = penn[*iteration % 4];
		*iteration = *color;
		return (TRUE);
		}
	    temp = az * 2 + b;
	    Complex d = z->CSqr()*temp;			// D = (2*AZ + B)*Z*Z
	    double dist = d.CSumSqr();
	    der *= dist;				// DER  =   DER*(REAL(D)*REAL(D) + IMAG(D)*IMAG(D))
	    if (der < epsilon)
		{
		*iteration = threshold;
		return(TRUE);
		}
	    if (*iteration >= threshold)
		{
		*iteration = special;
		return(TRUE);
		}
	    return(FALSE);				// continue iterations
	    }

	case MANDELDERIVATIVES:				// a group of Mandelbrot Derivatives
	    return (RunManDerFunctions(subtype, z, q, SpecialFlag, iteration));

	case TIERAZON:					// a group of Tierazon fractalsa
	    return (RunTierazonFunctions(subtype, z, q, SpecialFlag, iteration));

	case NEWTONAPPLE:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(55, z, q, SpecialFlag, iteration));

	case NEWTONFLOWER:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(35, z, q, SpecialFlag, iteration));

	case NEWTONMSET:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(52, z, q, SpecialFlag, iteration));

	case NEWTONPOLYGON:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(31, z, q, SpecialFlag, iteration));

	case NEWTONCROSS:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(59, z, q, SpecialFlag, iteration));

	case NEWTONJULIANOVA:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(2, z, q, SpecialFlag, iteration));

	case NEWTONVARIATION:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(87, z, q, SpecialFlag, iteration));

	case QUARTET1:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(85, z, q, SpecialFlag, iteration));

	case QUARTET2:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(85, z, q, SpecialFlag, iteration));

	case QUARTET3:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(96, z, q, SpecialFlag, iteration));

	case NOVA:
	    {
	    double	d;
	    Complex	z1, zd, fn, f1n;

	    z1 = *z;
	    f1n = z->CPolynomial(*degree - 1);			// z^(deg - 1) - first derivative power
	    fn = f1n * *z;						// z^(deg)
	    *z = *z - (fn - 1) / (*degree*f1n) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);
	    }
	case QUAD:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(90, z, q, SpecialFlag, iteration));

	case RAMONSIN:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(116, z, q, SpecialFlag, iteration));

	case RAMONCOS:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(117, z, q, SpecialFlag, iteration));

	case FORMULA05:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(5, z, q, SpecialFlag, iteration));

	case TEDDY:				// a specific Tierazon fractal
	    return (RunTierazonFunctions(104, z, q, SpecialFlag, iteration));

	case TETRATION:				// Tetration fractal
	    return(DoTetration(iteration));
	    break;

	case KLEINIAN:				// Kleinian fractal
	    return (CalculateKleinian(z));
	    break;

	}
    return 0;
    }
