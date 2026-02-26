/*
    FN.CPP a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	"manp.h"
#include	"slope.h"
#include	"Complex.h"

extern	double	param[];
extern	BYTE	juliaflag;		/* Julia implementation of fractal */
extern	double	rqlim;			// bailout level
extern	Complex	z, q, j;
extern	BYTE	degree;			/* power */

static	Complex	sqr;
static	double	real_imag;		/* z_real * z_imag */


//#include	".\parser\cmplx.h"
#ifdef SPLAT
static	double	absolute;
static	double	distance;


extern	long	iteration;		/* globals for speed */

extern	long	threshold;
extern	int	biomorph;		/* biomorph colour */
extern	int	decomp;			/* number of decomposition colours */
extern	WORD	special;		/* special colour, phase */
extern	int	period_level;		/* 0 for no periodicity checking */
extern	WORD	type;			/* M=mand, J=Julia 1,2,4-> */
extern	int	subtype;
extern	BYTE	phaseflag;		/* 3 phases for type SPECIALNEWT fractals */

#endif
/**************************************************************************
	Init CUBIC fractals 
	Courtesy:  ART MATRIX  PO 880  Ithaca, NY  14851-0880  USA
**************************************************************************/

/*
int	init_cubic_df(void)

    {
    Complex	temp;

    switch ((int) param[0])
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
	t3 = q*3;				// T3 = 3*T
	t2 = q.CSqr();				// T2 = T*T
	a = (t2+1)/t3;				// A  = (T2 + 1)/T3
						// B  = 2*A*A*A + (T2 - 2)/T3    
	temp = a.CCube()*2;			// 2*A*A*A
	b = (t2-2)/t3+temp;			// B  = 2*A*A*A + (T2 - 2)/T3
	}
    else if (subtype == 'C' || subtype == 'F')	// CCIN or CFIN
	{
	a = q;					// A = T
						// find B = T + 2*T*T*T
	temp = q.CCube();			// B = T*T*T
	if (subtype == 'C')
	    b = temp+temp+q;			// B = B * 2 + T
	else
	    {
    	    b = (temp-q)*2;			// B = B * 2 - 2 * T
	    a2 = a+a;
	    }
	}
    else if (subtype == 'K')			// CKIN 
	{
	a = 0;
	v = 0;
	b = q;					// B = T
	}
    aa3 = a.CSqr()*3;				// AA3 = A*A*3
    if (!juliaflag)
	z = -a;					// Z = -A
    return(TRUE);
    }
*/
/**************************************************************************
	Run CUBIC type fractals
**************************************************************************/
/*
int	do_cubic_df(void)

    {
    Complex	temp;

    if (subtype == 'K')				// CKIN
	{
	z = z.CCube()+b;				// Z = Z*Z*Z + B
	z.x += param[2];
	z.y += param[3];
	}
    else
	{
	temp = z.CCube()+b;			// Z = Z*Z*Z + B
	z = temp - aa3*z;			// Z = Z*Z*Z - AA3*Z + B
	z.x += param[2];
	z.y += param[3];
	}
    if (z.CSumSqr() > 100.0)
	return (TRUE);
    else
	{
	if (subtype == 'F')
	    {
	    if (q.CSumSqr() < 0.111111)
		{
		iteration = special;
		SpecialFlag = TRUE;			// for decomp and biomorph
		return (TRUE);
		}
	    v = z+a2;
	    }
	else if (subtype == 'K')
	    v = z - v;
	else
	    v = z - a;
	if (v.CSumSqr() <= 0.000001)
	    {
	    iteration = special;
	    SpecialFlag = TRUE;			// for decomp and biomorph
	    return (TRUE);
	    }
	}
    return(FALSE);
    }
*/
/**************************************************************************
	Init type 5 fractal
	Courtesy:  ART MATRIX  PO 880  Ithaca, NY  14851-0880  USA
**************************************************************************/
/*
int	init_SpecialNewton_df(void)

    {
    l2 = q.CSqr();				// L2 = L*L
    a = -l2+0.25;				// A = ( .25,0) - L2
    b = -l2-0.75;				// B = (-.75,0) - L2 
    lm5 = q-0.5;				// LM5 = L - (.5,0)
    lp5 = q+0.5;				// LP5 = L + (.5,0)
    return(TRUE);
    }
*/
/**************************************************************************
	Run type 5 fractals
**************************************************************************/
/*
int	do_SpecialNewton_df(void)

    {
    Complex	top, bottom, z2;

    if ((int)param[0] < 0)
	special = 2;
    else
	special = (int)param[0];
    z2 = z.CSqr();				// z2 = z*z
						// Z  =  (2*Z*Z2 + A)/(3*Z2 + B)
    top = z2*z*2+a;
    bottom = z2*3+b;
    z = top/bottom;
    z.x += param[1];
    z.y += param[2];
    v = z - 1;
//    v.x += param[1];
//    v.y += param[2];
    if (v.CSumSqr() <= 0.000001)
	{
	phaseflag = 0;				// first phase
	return(TRUE);
	}
    v = z - lm5;
    if (v.CSumSqr() <= 0.000001)
	{
	phaseflag = 1;				// second phase
	return(TRUE);
	}
    v = z + lp5;
    if (v.CSumSqr() <= 0.000001)
	{
	phaseflag = 2;				// third phase
	return(TRUE);
	}
    return(FALSE);
    }
*/
/**************************************************************************
	Init MATE fractals
	Courtesy:  ART MATRIX  PO 880  Ithaca, NY  14851-0880  USA
**************************************************************************/
/*
int	init_matein_df(void)

    {
    int	i;
    Complex	temp;

    if ((absolute = q.CSumSqr()) > 1.0)
	return(-1);				// not inside set
    if (!juliaflag)
	z = 1;
			
    for (i = 0; i < 100; ++i)			// DO 300 I = 1,100 
	{
	temp = z.CInvert();			// 300  Z = L*(Z + 1/Z)
	z = q*(z+temp);
	}
			
    distance = 1.0;				// D = 1
    oz = z.CInvert();				// OZ = 1/Z
    return(TRUE);
    }
*/
/**************************************************************************
	Run MATE fractals
**************************************************************************/
/*
int	do_matein_df(void)

    {
    double	epsilon = 0.01;
    double	escape = 10.0E20;
    Complex	t;

    z = q*(z+oz);				// Z = L*(Z + OZ)
    z.x += param[0];
    z.y += param[1];
    oz = z.CInvert();				// OZ = 1/Z
    t = -oz/z+1;				// T = 1 - OZ/Z
			    // D = D*ABSL*(REAL(T)*REAL(T) + IMAG(T)*IMAG(T))
    distance = distance * absolute * t.CSumSqr();

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
    }
*/
/**************************************************************************
	Initialise Mandelbrot
***************************************************************************/

int	init_mandel_df(void)

    {
    if (!juliaflag)
	{
	z.x = q.x + param[0];
	z.y = q.y + param[1];
	}
    sqr = 0;
    real_imag = 0.0;
    return 0;
    }

/**************************************************************************
	Determine count before 'Z' becomes unstable float version
***************************************************************************/

int	do_mandel_df(void)

    {
    sqr.x = z.x * z.x;
    sqr.y = z.y * z.y; 
    real_imag = z.x * z.y;
    z.x = q.x + sqr.x - sqr.y;
    z.y = q.y + real_imag + real_imag;
    return ((sqr.x + sqr.y) >= rqlim);
    }

/**************************************************************************
    The Burning Ship fractal, first described and created by Michael Michelitsch and Otto E. Rössler in 1992, is generated by iterating the function
    in the complex plane which will either escape or remain bounded. The difference between this calculation and that for the Mandelbrot set is 
    that the real and imaginary components are set to their respective absolute values before squaring at each iteration. The mapping is non-analytic 
    because its real and imaginary parts do not obey the Cauchy–Riemann equations.
***************************************************************************/
/*
int	DoBurningShip(void)

    {
    sqr.x = z.x * z.x;
    sqr.y = z.y * z.y; 
    real_imag = fabs(z.x * z.y);
    z.x = sqr.x - sqr.y + q.x;
    z.y = real_imag + real_imag - q.y;
    return ((sqr.x + sqr.y) >= rqlim);
    }
*/
/**************************************************************************
    The Burning Ship fractal for Higher Powers
    The function that is iterated to generate this fractal can be generalized 
    to any arbitrary power:
	z = [|Re(z)| + i |Im(z)|]N + c
    As with the Mandelbrot set, this creates shapes that resemble the original 
    shape but have increasingly many attached arms or buds. With N=2 we get 
    the Burning Ship. The N=3 version has been called the "Bird of Prey."

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
/*
int	DoBurningShipPower(void)

    {
    z.x = fabs(z.x);
    z.y = -fabs(z.y);
    z = z.CPolynomial(degree);	
    z = z+q;
    return (z.CSumSqr() >= rqlim);
    }
*/

/**************************************************************************
	Determine count before 'Z' becomes unstable float version
***************************************************************************/
/*
int	do_mandel4_df(void)

    {
    z = z.CSqr();	// faster than z*z
    z = z.CSqr();	
    z = z+q;
    return (z.CSumSqr() >= rqlim);
    }
*/
/**************************************************************************
	Initialise Mandelbrot Power
***************************************************************************/
/*
int	init_power_df(void)

    {
    degree = (int)param[0];
    if (degree < 1)
	degree = 1; 
    if (!juliaflag)
	{
	z.x = q.x + param[1];
	z.y = q.y + param[2];
	}
    return 0;
    }
*/

/**************************************************************************
	Determine count before 'Z' becomes unstable float version
***************************************************************************/
/*
int	do_power_df(void)

    {
    z = z.CPolynomial(degree);	
    z = z+q;
    return (z.CSumSqr() >= rqlim);
    }
*/
/**************************************************************************
	Initialise SIN Fractal
***************************************************************************/
/*
int	init_sin_df(void)

    {
    if (!juliaflag)
	z = param[3];
    return 0;
    }
*/
/**************************************************************************
	Determine count before 'Z' becomes unstable
	Z = L*SIN(Z)
	sin(x+iy)  = sin(x)cosh(y) + icos(x)sinh(y)
***************************************************************************/
/*
int	do_sin_df(void)

    {
    if (param[2] == 0.0)
	z = q * z.CSin();
    else
	z = q + z.CSin();
    z.x += param[0];
    z.y += param[1];
    return (z.CSumSqr() >= rqlim);
    }
*/
/**************************************************************************
	Run exponential type fractals
**************************************************************************/
/*
int	do_exp_df(void)

    {
    int		compare, i, degree;
    
    degree = (int)(fabs(param[3]));

    for (i = 0; i < degree; i++)
	z = z.CExp();

    if (param[4] == 0.0)
	z = q * z;					// Z = L*EXP(Z)
    else
	z = q + z;					// Z = L*EXP(Z)
						// Complex Exponent: e^(x+iy) = (e^x) * cos(y) + i * (e^x) * sin(y) 
    switch ((int)param[0])
	{
	case 0:
	    subtype = 'R';
	    compare = (z.x >= rqlim);
	    break;
	case 1:
	    subtype = 'I';
	    compare = (z.y >= rqlim);
	    break;
	case 2:
	    subtype = 'M';
	    compare = (z.x >= rqlim || z.x <= -rqlim || z.y >= rqlim || z.y <= -rqlim);
	    break;
	default:
	    subtype = 'R';
	    compare = (z.x >= rqlim);
	    break;
	}
    z.x += param[1];
    z.y += param[2];
    if (compare)
	return(TRUE);
    return(FALSE);
    }
*/
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
/*
int	DoThorn(void)

    {
    double  a, b;
    a = z.x;
    b = z.y;
    z.x = a / cos(b) + q.x;
    z.y = b / sin(a) + q.y;
    return (z.CSumSqr() >= rqlim);
    }
*/
/**************************************************************************
	Initialise RedShiftRider
***************************************************************************/
/*
int	init_RedShiftRider(void)

    {
    a.x = param[0];
    a.y = param[1];
    degree = (int)param[2];
    if (!juliaflag)
	{
	z.x = q.x + param[3];
	z.y = q.y + param[4];
	}
    return 0;
    }
*/
/**************************************************************************
	Run RedShiftRider type fractals
	a*z^2 +/- z^n + c
**************************************************************************/
/*
int	DoRedShiftRider(void)

    {
    z = a * z * z + ((param[5] == 1.0) ? 1.0 : -1.0) * z.CPolynomial(degree);
    z = z+q;
    return (z.CSumSqr() >= rqlim);
    }
*/
/**************************************************************************
	Talis Power Fractal
***************************************************************************/
/*
int	InitTalisPower(void)  // Talis

    {
    degree = (int)param[0];
    if (degree < 1)
	degree = 1;
    if (!juliaflag)
	{
	z.x = q.x + param[2];
	z.y = q.y + param[3];
	}
    return 0;
    }

int	doTalisPower(void)  // Talis    Z = Z^N/(M + Z^(N-1)) + C

    {
    double	m;
    Complex	z1;

    m = param[1];
    z1 = z.CPolynomial(degree - 1);
    z = (z1 * z) / (m + z1) + q;
    return (z.CSumSqr() >= rqlim);
    }
*/
/**************************************************************************
	Polynomial Fractal
***************************************************************************/
/*
int	InitPolynomial(void)
    {
    init_mandel_df();
    for (int i = 0; i < MAXPOLYDEG; i++)			// find highest order of polynomial
	{
	if (param[2 + i] != 0.0)
	    {
	    fractalspecific[type].SlopeDegree = MAXPOLYDEG - i;
	    break;
	    }
	}
    return 0;
    }

int	doPolynomial(void)
    {
    Complex	InitialZ = z;
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
	z = FinalZ + q;
	}
	return (z.CSumSqr() >= rqlim);
    }
*/
/**************************************************************************
	Marcus Escher
**************************************************************************/

/*
static	Complex	New, old;

static int near floatbailout()
{
   z.x = New.x;			// to allow filters to work
   z.y = New.y;
   if ( (( sqr(New.x) ) + ( sqr(New.y) ) ) >= rqlim ) 
	return(1);
   old = New;
   return(0);
}

extern	double	mandel_width;
extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address 
extern	double	ScreenRatio;			// ratio of width / height for the screen
static	double	xscale, yscale;


int	DoMarcusEscher(void)

    {
    int	    X, Y, R, I;
    int	    k, s, t;

    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    for (s = -1200; s < 1200; s++)
	for (t = 0; t < 1200; t++)
	    {
	    X = s;
	    Y = t;
	    for (k = 0; k < 16; k++)
		{
		R = X * X - Y * Y;
		I = 2 * X * Y;
		X = R / 2;
		Y = I / 2;
		if ((X - Y) > 4)
		    {
		    plot ((WORD)(s + 240), (WORD)(t + 240), (long)k * 20);
		    break;
		    }
//		i = (int)((x - hor) * xscale);
//		k = (int)((vert + mandel_width - y) * yscale);
		}
	    }
	
    return 0;
//    iteration = ((3 * iteration) % 255) + 1;
//    return ((X - Y) < rqlim);
    }
*/

