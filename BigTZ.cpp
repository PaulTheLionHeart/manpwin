////////////////////////////////////////////////////////////////
// BigTZ.cpp a module for the per pixel calculations of Tierazon fractals using Bignum functions. 
//
// By: stephen c. ferguson
// http://home1.gte.net/itriazon/
// email: itriazon@gte.net
//
/////////////////////////////////////////////////////
// Updated in Microsoft Visual 'C++' by Paul de Leeuw.
// These are listed in numerical order of the functions.
// Note that CSqr(z) is used in place of z*z and CCube(z) in place of z*z*z for speed
/////////////////////////////////////////////////////

#include	<math.h>
#include	"manp.h"
#include	"Complex.h"
#include	"big.h"
#include	"fractype.h"
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"resource.h"
#include	"pixel.h"



extern	void	ShowBignum(BigDouble x, char *Location);


/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::BigInitTierazonFunctions(int subtype, BigComplex *zBig, BigComplex *qBig)
    {
    switch (subtype)
	{
	case 0:						// Mandelbrot
	case 1:						// Mandelbrot (Tierazon version)
	case 4:						// Talis, z=((z*z)/(1+z))+c
	case 5:						// Newton variation, z=((z*z*z-z-1)/(3*z*z-1)-z)*c
	case 101:					// M-Set Method
	case 117:					// Ramon, z = (c/cos(z))^2
	case 118:					// N - Set Method
	    BigInitFunctions(MANDELFP, zBig, qBig);
	    break;

	case 10:					// z=z*z*z+c
	case 11:					// z=z*z*z*z+c
	case 12:					// z=z*z*z*z*z+c
	case 13:					// z=z*z*z*z*z*z+c
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    BigInitFunctions(POWER, zBig, qBig);
	    break;

	case 2:						// Nova, init: z=1; iterate: z=z-((z*z*z-1)/(3*z*z))+c
	    if (!juliaflag)
		{
		//	Note that julia parameters are also used as starting values for Mandelbrot
		zBig->x = param[0] + 1.0;
		zBig->y = param[1];
		}
	    //    else
	    //	z = q;
	    break;

	case 3:						// Ushiki's Phoenix, z=z*z-.5*z+c; z=z*z-.5*z2+c; z2=z; z=z1
	{
	c1Big.x = 0;
	c1Big.y = 1;
	if (!juliaflag)
	    *zBig = *qBig;
	// Rotate it so its pointing upward
	*zBig = *zBig * c1Big;
	*qBig = *qBig * c1Big;
	z2Big = 0;
	break;
	}

	case 6:						// z=z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)
	case 7:						// Newton variation, z = z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)
	case 61:					// Polynomials, z=z*z+c; real=imag; imag=real
	case 102:					// Ramon, z = ((z*z+c-1)/(2*z+c-2))^2; [magneto 1]
	    if (!juliaflag)
		{
		zBig->x = param[0];
		zBig->y = param[1];
		}
	    break;

	case 8:						// Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-1)+c
	case 9:						// Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-z)+c
	    if (!juliaflag)
		{
		//	Note that julia parameters are also used as starting values for Mandelbrot
		zBig->x = param[0] + 1.0;
		zBig->y = param[1];
		}
	    //    else
	    //	z = q;
	    break;

	case 14:					// z1=z*z+c; z=z*z+c*z2; z2=z
	    if (!juliaflag)
		{
		*zBig = *qBig;
		zBig->x += param[0];
		zBig->y += param[1];
		}
	    z2Big = 1.0;
	    break;

	case 15:					// Phoenix II, z1=z; z=z*z + real(c) + imag(c)*z2; z2=z1
	case 16:					// Phoenix III, z1=z; z=z*z*z +   real(c) + imag(c)*z2; z2=z1
	case 17:					// Phoenix IV,  z1=z; z=z*z*z +.5*real(c) + imag(c)*z2; z2=z1
	    c1Big.x = 0;
	    c1Big.y = -1;
	    if (!juliaflag)
		*zBig = *qBig;
	    // Rotate it so its pointing upward
	    if (subtype != 17)
		{
		*zBig = *zBig * c1Big;
		z2Big = 0;
		}
	    else
		z2Big = *zBig;
	    *qBig = *qBig * c1Big;

	    //c.x = 0.563;   // Paul Carlson's point
	    //c.x = 0.56667; // Ushiki's / Pickover's point

	    break;

	    /**************************************************************************
		    Newton M-Set, Explanation by Paul Carlson
	    ***************************************************************************/

	    /*
	    From - Sun Nov 09 16:15:28 1997
	    Path: news.gte.net!newsfeed.gte.net!europa.clark.net!199.0.154.56!ais.net!ix.netcom.com!news
	    From: pjcarlsn@ix.netcom.com (Paul and/or Joyce Carlson)
	    Newsgroups: alt.binaries.pictures.fractals
	    Subject: Fractal: Newton Msets Explained
	    Date: Sun, 09 Nov 1997 21:54:12 GMT
	    Organization: Netcom
	    Lines: 72
	    Expires: 30 days
	    Message-ID: <6457u5$p8p@sjx-ixn2.ix.netcom.com>
	    NNTP-Posting-Host: ftc-co2-12.ix.netcom.com
	    X-NETCOM-Date: Sun Nov 09 12:54:29 PM PST 1997
	    X-Newsreader: Forte Free Agent 1.0.82
	    Xref: news.gte.net alt.binaries.pictures.fractals:9670

	    Quite often in the past when I've posted a zoom into a
	    Newton Mset I've received a message or two asking me exactly
	    what a Newton Mset is, and how to go about programming
	    or writing a formula for one.  Because I plan on posting
	    several of them in the near future, I thought I'd save
	    some time by posting an explanation in advance.

	    The Newton method is a numerical method of solving for
	    the roots of real or complex valued polynomial equations.
	    An initial guess is made at a root and then the method is
	    iterated, producing new values that (hopefully) converge
	    to a root of the equation.

	    The traditional Newton fractal is similar to a Julia set
	    in that the pixel values are used as the initial values
	    (or guesses).  A typical equation that is used to produce
	    a traditional Newton fractal is:

		Z^3 - 1 = 0

	    On the other hand, what I call the Newton Mset method
	    solves for the roots of an equation in which the pixel
	    value appears in the equation.  For example, in this
	    equation the pixel value is denoted as C:

		Z^3 + C*Z^2 - Z + C = 0

	    Because C has a different value for every pixel, the
	    method actually solves a different equation for every
	    pixel.  Now the question is what to use for the initial
	    guess for each solution.  The answer is to use one of
	    the "critical points."  These are the values of Z for
	    which the second derivative vanishes and can be found
	    by setting the second derivative of the equation to
	    zero and solving for Z.  In the example equation
	    above:

		The function:          f(Z)   = Z^3 + C*Z^2 - Z + C
		The first derivative:  f'(Z)  = 3*Z^2 + 2*C*Z - 1
		The second derivative: f''(Z) = 6*Z + 2*C
		Therefore,  setting 6*Z + 2*C = 0 we have Z = -C/3

	    The variable Z is initialized to -C/3 prior to the
	    iteration loop.  From there, everything proceeds as
	    usual using the general Newton Method formula:

		Z[n+1] = Z[n] - f(Z[n]) / f'(Z[n])

	    A root is assumed to be found when Z[n] and Z[n+1] are
	    very close together.

	    In the Newton Mset fractals that I will be posting the
	    colors of the pixels have nothing to do with which
	    root a pixel converged to, unlike the more traditional
	    coloring method.

	    I hope this helps someone.

	    Regards,

	    Paul Carlson
	    ------------------------------------------------------------------
			    email   pjcarlsn@ix.netcom.com

	    WWW Fractal Galleries   http://sprott.physics.wisc.edu/carlson.htm
				    http://fractal.mta.ca/fractals/carlson/
				    http://www.cnam.fr/fractals/carlson.html

		    anonymous FTP   ftp.cnam.fr   /pub/Fractals/carlson
	    ------------------------------------------------------------------
	    */
	case 18:					// Newton/Mandel, z=-z/3; iterate: z=z-(z*z*z+z*z*c-z+c)/(3*z*z+2*c*z-1)
	case 19:					// Newton/Mandel, z=-z/3; iterate: z=z-(z*z*z+z*z*c+z+c)/(3*z*z+2*c*z+1)
	case 24:					// Newton/Mandel, z=-z/3;  iterate: z=z-((z^3)+z*z*c +c)/(3*z*z+2*z*c)
	case 105:					// More Fractals, 3rd order Newton/ Mset [figure 8]
	    if (!juliaflag)
		*zBig = -*qBig / 3;
	    break;
	case 20:					// Newton/Mandel, z= z/2; iterate: z=z-(z^4-(z^3)*c-z-c)/(4*(z^3)-3*(z^2)*c-1)
	    if (!juliaflag)
		*zBig = *qBig / 2;
	    break;

	case 21:					// Newton/Mandel, z=-1/(3*z); iterate: z=z-((z^3)*c+z^2+z+c)/(3*(z^2)*c+2*z+1)
	    if (!juliaflag)
		*zBig = -(*qBig * 3);
	    *zBig = zBig->CInvert();
	    break;

	case 22:					// Newton/Mandel, z=-.5; iterate: z=z-((z^4)c+(z^3)*c+z+c)/(4*(z^3)*c+3*z*z*c+1)
	    if (!juliaflag)
		*zBig = -0.5;
	    break;

	case 23:					// Newton/Mandel, z=-.5/z; iterate: z=z-((z^4)*c+(z^3)+z+c)/(4*(z^3)*c+3*z*z+1)
	    if (!juliaflag)
		*zBig = *qBig / -0.5;
	    *zBig = zBig->CInvert();
	    break;

	case 25:					// Newton/Mandel, z=-z/2;  iterate: z=z-((z^4)+(z^3)*c+c)/(4*(z^3)+3*z*z*c)
	    if (!juliaflag)
		*zBig = -*qBig / 2;
	    break;

	case 26:					// Newton/Mandel, 5th order Newton Mset
	case 27:					// Newton/Mandel, 7th order Newton Mset
	case 28:					// Newton/Mandel, 9th order Newton Mset
	case 29:					// Newton/Mandel, 13th order Newton Mset
	case 30:					// Newton/Mandel, 8th order Newton Mset
	case 31:					// Newton/Mandel, Newton Diamond
	case 32:					// Newton/Mandel, Newton Pentagon
	case 33:					// Newton/Mandel, Newton Hexagon
	case 34:					// Newton/Mandel, Newton Octagon
	case 35:					// Newton/Mandel, 9th order Newton Flower
	case 52:					// Newton/Mandel, 8th order Newton flower
	case 53:					// Newton/Mandel, 6th order Newton Mset
	case 54:					// More Newton Msets, 15th order Newton Mset flower
	case 56:					// More Newton Msets, 25th order Newton Mset flower
	case 57:					// More Newton Msets, 38th order Newton Mset flower
	case 58:					// More Newton Msets, 50th order Newton Mset flower
	case 106:					// More Fractals, 5th order N/Mset
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		{
		zBig->x = -qBig->x * (double)(*degree - 2) / (double)*degree + param[1];
		zBig->y = -qBig->y * (double)(*degree - 2) / (double)*degree + param[2];
		}
	    break;

	case 36:					// Newton Variations, z=z-(z*z*z*z*c-z+c)/(4*z*z*z*c)
	case 37:					// Newton Variations, z=z-(z*z*z-z+c)/(3*z*z)
	case 38:					// Newton Variations, z=z-(z*z*z*c-z*c-1)/(3*z*z*c)
	case 39:					// Newton Variations, z=z-(z*z*z*c-z*z*c-1)/(3*z*z*c)
	case 40:					// Newton Variations, z=z-(z*z*z*c-1)/(3*z*z*c)
	case 41:					// Newton Variations, z=z-(z*z*z*c-z-1)/(3*z*z*c-z)
	case 42:					// Newton Variations, z=z-(z*z*z*c-z*c-1)/(3*z*z*c-z)
	case 43:					// Newton Variations, z=z-(z*z*z*c-z*z-1)/(3*z*z*c-3*z*z-3*z)
	case 44:					// Newton Variations, z=z-(z*z*z*c-z*z*c-1)/(3*z*z*c-z*c-z)
	case 45:					// Newton Variations, z=((z-(z*z*z-z)/(3*z*z-1))^2)*c
	case 46:					// Trig functions, z = ccos(z*c)*c
	case 47:					// Trig functions, z = ((((z*z).csin())*z/2)^2)+c, Sharon 14
	case 48:					// Trig functions, z = ((z*z).csin()).clog()+c
	case 49:					// Trig functions, z = z*z*sin(z.real()) + c*z*cos(z.imag()) + c
	case 50:					// Trig functions, z = csin(z)*ccos(c)
	case 51:					// Trig functions, z = csin(z*z*z*z)*c
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    if (subtype == 50)
		*qBig = qBig->CCos();
	    break;

	case 55:					// More Newton Msets, 4th order Newton's apple
//	case 59:					// More Newton Msets, 5th order Newton Mset
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*zBig = *qBig * ((double)*degree);
	    *zBig = zBig->CInvert();
	    zBig->x = zBig->x + param[1];
	    zBig->y = zBig->y + param[2];
	    break;


	case 59:					// More Newton Msets, 5th order Newton Mset
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*zBig = *qBig * 2.0;
	    *zBig = zBig->CInvert();
	    zBig->x =  - zBig->x + param[1];
	    zBig->y =  - zBig->y + param[2];
	    break;

	case 60:					// More Newton Msets, 18th order Newton Mset flower
	    if (!juliaflag)
		*zBig = -*qBig * (5814.0 / 4896.0);
	    *zBig = zBig->CInvert();
	    break;

	case 67:					// Newton Variations II, z=z-(((z^3)-z)/((3*z*z-1)))+c
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*zBig = 1;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    return 0;

	case 62:					// Newton Variations II, z=(((z^3)-z-1)/((z^3)-1)-z)*c
	case 63:					// Newton Variations II, z=(((z^4)-z*z-1)/(4*z*z-1)-z)*c
	case 64:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-fabs(z)-1))*c
	case 65:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-z-1))*c
	case 66:					// Newton Variations II, z=(((z^4)-z-1)/(4*(z^3)-z-1)-z)*c
	case 68:					// Newton Variations II, z=(z-((z^3)-1)/(4*z*z-z-1))*c
	case 69:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-z))*c
	case 70:					// Newton Variations II, z=(z-((z^4)-1)/(4*(z^3)-z))*c
	case 71:					// Newton Variations II, z=(z-((z^4)-1)/(3*(z^3)-z))*c
	case 72:					// Newton Variations II, z=(z-((z^4)-z-1)/(3*(z^3)-z))*c
	case 73:					// Newton Variations II, z=c*(z-((z^3)-z)/(3*z*z-1))
	case 74:					// Newton Variations II, z=((1-z-(z^4))/(z-(4*(z^3)))-z)*c
	case 75:					// Newton Variations II, z=((z-(z^3)*z)/(z-(3*(z^3)))-z)*c
	case 76:					// Newton Variations II, z=((z-(z^3))/(z-(3*(z^3)))-z)*c
	case 77:					// Newton Variations II, z=(((z^3)-1)/(2*z*z-1)-z)*c
	case 78:					// Newton Variations II, z=(((z^3)-z-1)/(3*(z^3)-1)-z)*c
	case 87:					// Newton Variations, z=z-((z^3)-1)/(3*z*z); z=(z^3)*c
	case 88:					// Newton Variations, z=z-((z^4)-1)/(4*(z^3)); z=(z^4)*c
	case 89:					// Newton Variations, z=z-((z^5)-1)/(5*(z^4)); z=(z^5)*c
	case 90:					// Polynomials, z=z+c [quad]
	case 94:					// Polynomials, z=c*z*(2 - z*z)
	case 97:					// Polynomials, z = z*z+c*(1+sqrt(z))
	case 98:					// Quartets, z1=z; z=z*z*z*z/(1+z)+c;; c=z1
	case 100:					// Polynomials, real(z) += sin(real(z)); z=z*z+c
	case 107:					// More Fractals, z1 = z; z = sin(z) - c; c = 1/(z*50); [quartet]
	case 108:					// More Fractals, z=(z*c)/(1+z)+c; [Talis II]
	case 109:					// More Fractals, z=(c+z*z*c)/(1-z*z*c)
	case 110:					// More Fractals, z = (c+(z^6))/(1+(z^2))
	case 111:					// Ramon, z = (z^2)/(1+c*(z^2))
	case 112:					// Ramon, z = c*cos(z)
	case 113:					// Ramon, z = c*sin(z)
	case 114:					// Ramon, z = c*exp(z)
	case 115:					// Ramon, z=z^2+c; c=c/2+z [spider]
	case 116:					// Ramon, z = (c/sin(z))^2
	case 119:					// Flarium 06, Newton Variations: z=z-(z*z*z-1)/3*z*z; z=z*c
	case 121:					// Flarium 08, Sharon Webb: z = z*z*z*z+1/c; [Sharon Star]
	case 122:					// Flarium 09, Sharon Webb: z = (z*z/2+c)*(z*z/2+c); [Sharon's Space Probe]
	case 125:					// Flarium 12, Sharon Webb: z = (z+z*z/2)+c
	case 130:					// Flarium 21, Sharon Webb: z=(1/z*z-c)*(z*z*z*z+c); [Sharon03]
	case 131:					// Flarium 25, Newton Variations: z=z-(z*z*z*z-z)/(4*z*z*z-z); z=z*c
	case 133:					// Flarium 28, Derbyshire / Newton: z=z - (z*z*z-1)/(3*z*z)+c; [Nova-Mandelbrot-MultiFract]
	case 135:					// Flarium 30, Derbyshire / Newton: z -= (z*z*z-1)/(3*z*z)*c
	case 138:					// Flarium 34, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14]
	case 139:					// Flarium 35, Polynomial: z=c*(z.csin())
	case 140:					// Flarium 36, Polynomial: z=(c*z).csin()
	case 142:					// Flarium 38, Polynomial: z = z*z+c; z.set_real(z.real()*z.real()); [Variation real]
	case 143:					// Flarium 40, Polynomial: z1 = z*z*z*z; z = c*z1/4*z1 + z
	case 144:					// Flarium 41, Polynomial: z = c*(z*z*z*z).csin()
	case 145:					// Flarium 43, Polynomial: z = z*z*z*z + (z*c).csin() + c
	case 148:					// Flarium 46, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14 N-Method]
	case 150:					// Flarium 50, Sharon Webb: z = z*z*z*z+1/c; [Sharon's Star M-Set]
	case 151:					// Flarium 51, Sharon Webb: z = (z*z/2+c)*(z*z/2+c); [Space Probe M-Set]
	case 153:					// Flarium 53, Sharon Webb: z = (z+z*z/2)+c; [M-Set]
	case 154:					// Flarium 54, Polynomial: z = z*z*c+c; [M-Set]
	case 155:					// Flarium 55, Polynomial: z = (z*z).csin()*z*z*c+c; [M-Set]
	case 156:					// Flarium 56, Polynomial: z = (z*z+c)/(z*z-c); [M-Set]
	case 162:					// Flarium 62, Polynomial: z=z*z*(1+z)/(1+z+c)+c
	case 164:					// Flarium 66, Newton Variations: z = ((z-(((z^2)-1)/(2*z)))^2)*c
	case 166:					// Flarium 80, Newton Variations: z=((z*z*z*z-1)/(1-(3*z*z*z)))*c
	case 167:					// Flarium 90, Sharon Webb: z = (z*(z*z).csin()/2).csin()+c; [Sharon15]
	case 168:					// Flarium 91, Sharon Webb: z = (z*(z+z).ccos()/2); z = z*z+c; [Sharon16]
	case 169:					// Flarium 92, Sharon Webb: z = (z*(z+z).csin()/2); z = z*z+c; [Sharon17]
	case 170:					// Flarium 93, Sharon Webb: z = (z*(z+z*z).csin()/2); z = z*z+c; [Sharon18]
	case 171:					// Flarium 111, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Alden's Ray Method]
	case 174:					// Flarium 118, Sharon Webb: z = (z*z*z*z*z).csin() + c; [Sharon19]
	case 175:					// Flarium 119, Sharon Webb: z = (z+(z*z)/.192).csin() + c; [Sharon's Butterfly]
	case 176:					// Flarium 125, Sharon Webb: z = z+z*z*z/4 + c; [Sharon21]
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    break;

	case 79:					// Polynomials, CBAP  F(z) = Z^3 - 3*(A^2)*Z + B(MOD 2)
	    {
	    BigComplex ca, tt, t2, t3;

	    tt = *qBig;
	    t3 = tt * 3;
	    t2 = tt * tt;
	    ca = (t2 + 1) / t3;
	    //cb=2*ca*ca*ca+(t2-cmplx(2,0))/t3;
	    cbBig = ca.CCube() * 2 + (t2 - 2) / t3;
	    caa3Big = ca.CSqr() * 3;
	    if (!juliaflag)
		*zBig = -ca;
	    }
	    break;

	case 80:					// Quartets, z=.5; z=z*z-z2*z2+c; z2=z
	    z2Big = 0.5;
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    break;

	case 81:					// Quartets, z2=2; z=z*z*z*z+z2+c; z2=z1
	case 82:					// Quartets, z2=z; z=z*z*z*z+5*z2*c; z2=z1
	    if (!juliaflag)
		*zBig = *qBig;
	    z2Big = *zBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    break;

	case 83:					// Quartets, t=0; z1=z; z=z*z*z-t*t*t+c; z=z1
	    z2Big = 0;
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    break;

	case 84:					// Quartets, t=0; z1=z; z=z*z*z*z-t*t*t*t+c; t=z1
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    z2Big.x = param[1];	    // t = 0;
	    z2Big.y = param[2];
	    if (!juliaflag)
		*zBig = 0;
	    break;

	case 85:					// Quartets, z2=z; z=(z^4)+c; c=z2
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		{
		zBig->x = qBig->x + param[1];
		zBig->y = qBig->y + param[2];
		}
	    break;

	case 86:					// Newton Variations, z=z-((z^4)-z)/(4*(z^3)-1); z=z*z*c
	    if (!juliaflag)
		*zBig = qBig->CInvert();
	    zBig->x += param[0];
	    zBig->y += param[1];
	    break;

	case 91:					// Polynomials, z=z*z*z-caa3*z+cb; [CCAP]
	    {
	    BigComplex ca, t, t2, t3;

	    t = *qBig;
	    ca = t;
	    cbBig = t.CCube() * 2 + t;
	    caa3Big = ca * ca * 3;
	    if (!juliaflag)
		*zBig = -ca;
	    }
	    break;

	case 92:					// Polynomials, z=z*z*z-caa3*z+cb; [CFAP]
	    {
	    BigComplex ca, t, t2, t3;

	    t = *qBig;
	    ca = t;
	    cbBig = (t.CCube() - t) * 2;
	    caa3Big = ca.CSqr() * 3;
	    //    ca2 = ca+ca;
	    if (!juliaflag)
		*zBig = -ca;
	    }
	    break;

	case 93:					// Quartets, z1=z; z=z*z*z*z-z2+c; z2 = z1
	    if (!juliaflag)
		*zBig = *qBig;
	    z2Big = *zBig;
	    zBig->x += param[1];
	    zBig->y += param[2];
	    break;

	case 95:					// Polynomials, z=z*z*z*z/t1+c
	    {
	    BigComplex t;

	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    t = *qBig + 1;
	    z2Big = t.CSin();
	    break;
	    }

	case 96:					// Quartets, z1=z; z=z*z*z*z+z2/2+c;; z2=z1
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    z2Big = *qBig;
	    if (!juliaflag)
		{
		zBig->x = param[1];
		zBig->y = param[2];
		}
	    break;

	case 99:					// Quartets, z1=z; z2=c+z2/z-z; z=z*c+z2/c
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    z2Big = zBig->CSin();	//z2 = c-c.csin();
	    z2Big = *zBig - z2Big;
	    break;

	case 103:					// Ramon, z = ((z*z*z+3*c2*z+c1)/(3*z*z+3*c2*z+c1+1))^2; [magneto 2]
	    if (!juliaflag)
		{
		zBig->x = param[0];
		zBig->y = param[1];
		}
	    c2Big = *qBig - 1;
	    c1Big = c2Big * (*qBig - 2);
	    break;

	case 104:					// Ramon, z = ((z*z+c)/(2*z))^2 (Teddy)
	    if (!juliaflag)
		*zBig = 1;
	    break;

	case 120:					// Flarium 07, Polynomial: z = (z*z+c)^(cn+c)
	case 132:					// Flarium 27, Polynomial: z=z*z*(cn+z)/(cn+z+c)+c
	    if (!juliaflag)
		*zBig = *qBig;
	    z2Big.x = param[0];
	    z2Big.y = param[1];
	    break;

	case 123:					// Flarium 10, Sharon Webb: z = z*z*z*z+((c/2)^2)+c; [Sharon08]
	    {
	    BigComplex	t = *qBig / 2;
	    z2Big = t.CSqr();
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    }
	    break;

	case 124:					// Flarium 11, Sharon Webb: z = z*z*z*z+(z^(z+cn))+c
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    z2Big = *zBig ^ (*zBig + 2.0);
	    break;

	case 126:					// Flarium 13, Sharon Webb: z=(z*z*z*z-z2*z2*z2*z2+c)^2
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    z2Big = 0;
	    break;

	case 127:					// Flarium 15, Newton Variations: z = z - (z*z*z*z-1)/(4*z*z*z) + c; [4th Order Nova]
	case 128:					// Flarium 16, Newton Variations: z = z - (z*z*z*z*z-1)/(5*z*z*z*z) + c; [5th Order Nova]
	    zBig->x = 1.0 + param[0];
	    zBig->y = 0.0 + param[1];
	    break;

	case 129:					// Flarium 20, Phoenix: z = z*z*z*z+c.imag()+c.real()*z2; [4th Order Phoenix]
	    {
	    BigComplex t;

	    zBig->x = qBig->x + param[0];
	    zBig->y = qBig->y + param[1];
	    t.x = 0;
	    t.y = 1;
	    z2Big = 0;
	    if (!juliaflag)
		*qBig *= t;
	    else
		{
		*qBig *= t;
		*zBig *= t;
		}
	    }
	    break;

	case 134:					// Flarium 29, Derbyshire / Newton: c = (z*z*z-1)/(3*z*z); z -= (z*z*z-1)/(3*z*z)*c
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    *qBig = (*zBig * *zBig * *zBig - 1) / (*zBig * *zBig * 3);
	    break;

	case 136:					// Flarium 31, Polynomial: z = z*z*z-aa3*z+b
	    {
	    BigComplex t;

	    t = (qBig->CCube() - 1) / (qBig->CSqr() * 3);
	    aBig = t;
	    bBig = t.CCube() * 2 - t * 2;
	    caa3Big = aBig.CSqr() * 3;
	    if (!juliaflag)
		*zBig = -aBig;
	    z2Big.x = 42;
	    z2Big.y = 42;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    return 0;
	    }
	    break;

	case 137:					// Flarium 32, Derbyshire / Newton: 3rd Order Nova in a M-Set (Try single pass)
	case 157:					// Flarium 57, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-z)+c; [5th Order Nova Variation]
	case 158:					// Flarium 58, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]
	case 159:					// Flarium 59, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-1)+c; [3rd Order Nova Variation]
	case 160:					// Flarium 60, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-1)+c; [5th Order Nova Variation]
	    if (!juliaflag)
		*zBig = 1;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    break;

	case 141:					// Flarium 37, Polynomial: z=(z*z*z-1)/(3*z*z); z = c*(z.csin() + z.ccos())
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    *zBig = (*zBig * *zBig * *zBig - 1) / (*zBig * *zBig * 3);
	    break;

	case 146:					// Flarium 44, Polynomial: z=(z*z*z*z-z)/(4*z*z*z); z=c*z.csin()
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    *zBig = (*zBig * *zBig * *zBig * *zBig - *zBig) / (*zBig * *zBig * *zBig * 4);
	    break;

	case 147:					// Flarium 45, Polynomial: z=(z*z*z*z-1)/(4*z*z*z); z=c*z.csin()
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    *zBig = (*zBig * *zBig * *zBig * *zBig - 1) / (*zBig * *zBig * *zBig * 4);
	    break;

	case 149:					// Flarium 49, Polynomial: z=(z*z*z*z-z)/((4*z*z*z)-z); z=c*z.csin()
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    *zBig = (*zBig * *zBig * *zBig * *zBig - *zBig) / ((*zBig * *zBig * *zBig * 4) - *zBig);
	    break;

	case 152:					// Flarium 52, Sharon Webb: z = z*z*z*z+t+c; [Sharon08 M-Set]
	    {
	    BigComplex t;

	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    t = *qBig / 2;
	    z2Big = t.CSqr();
	    }
	    break;

	case 161:					// Flarium 61, Polynomial: z=z*z*z*z+(c+(c/pi))
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    *qBig = *qBig + (*qBig / PI);
	    break;

	case 163:					// Flarium 64, PHOENIX: z1=z; z=z*z*z*z+c.real()*z2/2+c.imag()*z2/2+c; z2=z1
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    z2Big = *zBig;
	    break;

	case 165:					// Flarium 67-69, Newton Variations: z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*zBig = 1;
	    zBig->x += param[1];
	    zBig->y += param[2];
	    break;

	case 172:					// Flarium 112-116, Polynomials: z=z^n*c+z*c; Dragon curve variations
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*zBig = *qBig;
	    zBig->x += param[1];
	    zBig->y += param[2];
	    break;

	case 173:					// Flarium 117, Newton Variations: z = z-(z*z*z-z*c-1)/(3*z*z+c-1)
	    if (!juliaflag)
		*zBig = 0;
	    zBig->x += param[0];
	    zBig->y += param[1];
	    break;

	case 177:					// Flarium 145, Polynomial: z=z^2+c [Jaenisch method]
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*zBig = *qBig;
	    z4Big.x = param[1];
	    z4Big.y = param[2];
	    z2Big = *zBig;
	    break;
	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::BigRunTierazonFunctions(int subtype, BigComplex *zBig, BigComplex *qBig, BYTE *SpecialFlag, long *iteration)
    {
    switch (subtype)
	{
	case 0:						// Mandelbrot
	case 1:						// Mandelbrot (Tierazon version)
	case 101:					// M-Set Method
	case 118:					// N - Set Method
	    return (BigRunFunctions(MANDELFP, zBig, qBig, SpecialFlag, iteration));

	case 2:						// Nova, init: z=1; iterate: z=z-((z*z*z-1)/(3*z*z))+c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - ((*zBig*z2Big - 1) / (z2Big * 3)) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 3:						// Ushiki's Phoenix, z=z*z-.5*z+c; z=z*z-.5*z2+c; z2=z; z=z1
	    *zBig = zBig->CSqr() - *zBig * 0.5 + *qBig;
	    z1Big = zBig->CSqr() - z2Big * 0.5 + *qBig;
	    z2Big = *zBig;
	    *zBig = z1Big;
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 4:						// Talis, z=((z*z)/(1+z))+c
	    *zBig = zBig->CSqr() / (*zBig + 1) + *qBig;
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 5:						// Newton variation, z=((z*z*z-z-1)/(3*z*z-1)-z)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = ((*zBig*z2Big - *zBig - 1) / (z2Big * 3 - 1) - *zBig) * *qBig;
	    //  z=((z*z2-z-1)/(3*z2-1)-z)*c;  
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 6:						// z=z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    *zBig = *zBig - ((*zBig*z1Big) + (*qBig - 1) * *zBig - *qBig) / ((z1Big) * 3 + *qBig - 1);
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 7:						// Newton variation, z = z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)
	    z2Big = *zBig;
	    *zBig = (*zBig - ((zBig->CCube() + *qBig * *zBig - *zBig - *qBig) / (zBig->CSqr() * 3 + *qBig - 1))) + *qBig;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 8:						// Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-1)+c
	    z2Big = *zBig;
	    z1Big = zBig->CCube();
	    *zBig = *zBig - (*zBig*z1Big - *zBig) / (z1Big * 4 - 1) + *qBig;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 9:						// Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-z)+c
	    z2Big = *zBig;
	    z1Big = zBig->CCube();
	    *zBig = *zBig - (*zBig*z1Big - *zBig) / (z1Big * 4 - *zBig) + *qBig;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 10:					// z=z*z*z+c
	case 11:					// z=z*z*z*z+c
	case 12:					// z=z*z*z*z*z+c
	case 13:					// z=z*z*z*z*z*z+c
	    *zBig = zBig->CPolynomial(*degree) + *qBig;	// z^deg - function power
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 14:					// z1=z*z+c; z=z*z+c*z2; z2=z
	    z1Big = zBig->CSqr() + *qBig;
	    *zBig = zBig->CSqr() + *qBig * z2Big;
	    z2Big = z1Big;
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 15:					// Phoenix II, z1=z; z=z*z + real(c) + imag(c)*z2; z2=z1
	    z1Big = *zBig;
	    *zBig = zBig->CSqr() + qBig->x + z2Big * qBig->y;
	    z2Big = z1Big;
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 16:					// Phoenix III, z1=z; z=z*z*z +   real(c) + imag(c)*z2; z2=z1
	    z1Big = *zBig;
	    *zBig = zBig->CCube() + qBig->x + z2Big * qBig->y;
	    //z = z*z + __real__ c + __imag__ c * z2;
	    z2Big = z1Big;
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 17:					// Phoenix IV,  z1=z; z=z*z*z +.5*real(c) + imag(c)*z2; z2=z1
	    z1Big = *zBig;
	    *zBig = zBig->CCube() + qBig->x / 2 + z2Big * qBig->y;
	    z2Big = z1Big;
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 18:					// Newton/Mandel, z=-z/3; iterate: z=z-(z*z*z+z*z*c-z+c)/(3*z*z+2*c*z-1)
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z1Big + z1Big * *qBig - *zBig + *qBig) / (z1Big * 3 + *qBig * *zBig * 2 - 1);
	    //z=z1*z1+c;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 19:					// Newton/Mandel, z=-z/3; iterate: z=z-(z*z*z+z*z*c+z+c)/(3*z*z+2*c*z+1)
	    z2Big = *zBig;
	    *zBig = *zBig - (zBig->CCube() + *qBig * zBig->CSqr() + *zBig + *qBig) / (zBig->CSqr() * 3 + *qBig * *zBig * 2 + 1);
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 20:					// Newton/Mandel, z= z/2; iterate: z=z-(z^4-(z^3)*c-z-c)/(4*(z^3)-3*(z^2)*c-1)
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    //    z = z-c;
	    *zBig = *zBig - (z1Big.CSqr() - z1Big * *zBig * *qBig - *zBig - *qBig) / (*zBig*z1Big * 4 - z1Big * *qBig * 3 - 1);
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 21:					// Newton/Mandel, z=-1/(3*z); iterate: z=z-((z^3)*c+z^2+z+c)/(3*(z^2)*c+2*z+1)
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z1Big * *qBig + z1Big + *zBig + *qBig) / (z1Big * *qBig * 3 + *zBig * 2 + 1);
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 22:					// Newton/Mandel, z=-.5; iterate: z=z-((z^4)c+(z^3)*c+z+c)/(4*(z^3)*c+3*z*z*c+1)
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    *zBig = *zBig - (z1Big.CSqr() * *qBig + z1Big * *zBig * *qBig + *zBig + *qBig) / (z1Big * *zBig * *qBig * 4 + z1Big * *qBig * 3 + 1);
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 23:					// Newton/Mandel, z=-.5/z; iterate: z=z-((z^4)*c+(z^3)+z+c)/(4*(z^3)*c+3*z*z+1)
	    z2Big = *zBig;
	    z1Big = *zBig * *zBig;
	    *zBig = *zBig - (z1Big.CSqr() * *qBig + z1Big * *zBig + *zBig + *qBig) / (z1Big  * *zBig  * *qBig * 4 + z1Big * 3 + 1);
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 24:					// Newton/Mandel, z=-z/3;  iterate: z=z-((z^3)+z*z*c +c)/(3*z*z+2*z*c)
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    *zBig = *zBig - (z1Big * *zBig + z1Big * *qBig + *qBig) / (z1Big * 3 + *zBig * *qBig * 2);
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 25:					// Newton/Mandel, z=-z/2;  iterate: z=z-((z^4)+(z^3)*c+c)/(4*(z^3)+3*z*z*c)
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    *zBig = *zBig - (z1Big.CSqr() + z1Big * *zBig * *qBig + *qBig) / (z1Big * *zBig * 4 + *zBig * *zBig * *qBig * 3);
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 26:					// Newton/Mandel, 5th order Newton Mset
	case 27:					// Newton/Mandel, 7th order Newton Mset
	case 28:					// Newton/Mandel, 9th order Newton Mset
	case 29:					// Newton/Mandel, 13th order Newton Mset
	case 30:					// Newton/Mandel, 8th order Newton Mset
	case 53:					// Newton/Mandel, 6th order Newton Mset
	    {
	    BigComplex	fn, f1n, f2n, a, b;

	    z1Big = *zBig;
	    f2n = zBig->CPolynomial(*degree - 2);	// z^(deg - 2) - second derivative power
	    f1n = f2n * *zBig;				// z^(deg - 1) - first derivative power
	    fn = f1n * *zBig;				// z^deg - function power
	    a = f1n * *qBig + fn + *qBig;
	    b = f1n * ((double)*degree) + *qBig * (f2n*((double)(*degree - 1)));
	    *zBig = *zBig - a / b;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);
	    }

	case 31:					// Newton/Mandel, Newton Diamond
	case 32:					// Newton/Mandel, Newton Pentagon
	case 33:					// Newton/Mandel, Newton Hexagon
	case 34:					// Newton/Mandel, Newton Octagon
	{
	BigComplex	fn, f1n, f2n, a, b;

	z1Big = *zBig;
	f2n = zBig->CPolynomial(*degree - 2);		// z^(deg - 2) - second derivative power
	f1n = f2n * *zBig;				// z^(deg - 1) - first derivative power
	fn = f1n * *zBig;				// z^deg - function power
	a = f1n * *qBig + fn + *zBig + *qBig;
	b = f1n * ((double)*degree) + 1.0 + *qBig * (f2n*((double)(*degree - 1)));
	*zBig = *zBig - a / b;
	zdBig = *zBig - z1Big;
	d = zdBig.CSumSqr();
	return (d < MINSIZE);
	}

	case 35:					// Newton/Mandel, 9th order Newton Flower
	case 52:					// Newton/Mandel, 8th order Newton flower
	case 54:					// More Newton Msets, 15th order Newton Mset flower
	case 56:					// More Newton Msets, 25th order Newton Mset flower
	case 57:					// More Newton Msets, 38th order Newton Mset flower
	case 58:					// More Newton Msets, 50th order Newton Mset flower
	case 106:					// More Fractals, 5th order N/Mset
	    {
	    BigComplex	 fn, f1n, f2n, a, b;

	    z1Big = *zBig;
	    f2n = zBig->CPolynomial(*degree - 2);	// z^(deg - 2) - second derivative power
	    f1n = f2n * *zBig;				// z^(deg - 1) - first derivative power
	    fn = f1n * *zBig;				// z^deg - function power
	    a = f1n * *qBig + fn - *zBig;
	    b = f1n * ((double)*degree) + *qBig * (f2n*((double)(*degree - 1))) - 1.0;
	    *zBig = *zBig - a / b;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();

	    return (d < MINSIZE);
	    }

	case 36:					// Newton Variations, z=z-(z*z*z*z*c-z+c)/(4*z*z*z*c)
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = *zBig - (z3Big* *zBig * *qBig - *zBig + *qBig) / (z3Big * *qBig * 4);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 37:					// Newton Variations, z=z-(z*z*z-z+c)/(3*z*z)
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big - *zBig + *qBig) / (z2Big * 3);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 38:					// Newton Variations, z=z-(z*z*z*c-z*c-1)/(3*z*z*c)
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big * *qBig - *zBig * *qBig - 1) / (z2Big * *qBig * 3);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 39:					// Newton Variations, z=z-(z*z*z*c-z*z*c-1)/(3*z*z*c)
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big * *qBig - *zBig * *zBig * *qBig - 1) / (z2Big * *qBig * 3);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 40:					// Newton Variations, z=z-(z*z*z*c-1)/(3*z*z*c)
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big * *qBig - 1) / (z2Big * *qBig * 3);
	    *zBig = zBig->CSqr() * *qBig + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 41:					// Newton Variations, z=z-(z*z*z*c-z-1)/(3*z*z*c-z)
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big * *qBig - *zBig - 1) / (z2Big * *qBig * 3 - *zBig);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 42:					// Newton Variations, z=z-(z*z*z*c-z*c-1)/(3*z*z*c-z)
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big * *qBig - *zBig * *qBig - 1) / (z2Big * *qBig * 3 - *zBig);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 43:					// Newton Variations, z=z-(z*z*z*c-z*z-1)/(3*z*z*c-3*z*z-3*z)
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big * *qBig - z2Big - 1) / (z2Big * *qBig * 3 - z2Big * 3 - *zBig * 3);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 44:					// Newton Variations, z=z-(z*z*z*c-z*z*c-1)/(3*z*z*c-z*c-z)
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big * *qBig - z2Big * *qBig - 1) / (z2Big * *qBig * 3 - *zBig * *qBig - *zBig);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 45:					// Newton Variations, z=((z-(z*z*z-z)/(3*z*z-1))^2)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = (*zBig - (*zBig*z2Big - *zBig) / (z2Big * 3 - 1));
	    *zBig = *zBig * *zBig * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 46:					// Trig functions, z = ccos(z*c)*c
	    z1Big = *zBig;
	    *zBig = *zBig * *qBig;
	    *zBig = zBig->CCos();
	    *zBig = *zBig * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 47:					// Trig functions, z = ((((z*z).csin())*z/2)^2)+c, Sharon 14
	    z1Big = *zBig;
	    *zBig = zBig->CSqr();
	    *zBig = zBig->CSin();
	    *zBig = zBig->CSqr() / 2.0;
	    *zBig = zBig->CSqr() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 48:					// Trig functions, z = ((z*z).csin()).clog()+c
	    z1Big = *zBig;
	    *zBig = zBig->CSqr();
	    *zBig = zBig->CSin();
	    *zBig = zBig->CLog();
	    *zBig = *zBig + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 49:					// Trig functions, z = z*z*sin(z.real()) + c*z*cos(z.imag()) + c
		//  z = z*z*sin(z.real()) + c*z*cos(z.imag()) + c;
	    z1Big = *zBig;
	    *zBig = zBig->CSqr()*zBig->x.BigSin() + *qBig * *zBig*zBig->y.BigCos() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 50:					// Trig functions, z = csin(z)*ccos(c)
	    z1Big = *zBig;
	    *zBig = zBig->CSin();
	    *zBig = *zBig * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 51:					// Trig functions, z = csin(z*z*z*z)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    aBig = z2Big.CSqr();
	    *zBig = *qBig * aBig.CSin();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 55:					// More Newton Msets, 4th order Newton's apple
	{
	BigDouble	s, t, u;
	BigComplex	fn, f1n, f2n, f3n, c, e, f;

	z1Big = *zBig;
	f3n = zBig->CPolynomial(*degree - 4);		// z^(deg - 4) - third derivative power
	f2n = f3n * *zBig;				// z^(deg - 2) - second derivative power
	f1n = f2n * *zBig;				// z^(deg - 1) - first derivative power
	fn = f1n * *zBig;				// z^deg - function power
	c = *qBig * fn*((double)*degree);
	e = f1n * ((double)(*degree - 1));
	aBig = c - e - f2n * ((double)(*degree - 2));	// top row
	s = (double)*degree * (double)(*degree - 1);
	t = (double)(*degree - 1) * (double)(*degree - 2);
	u = (double)(*degree - 2) * (double)(*degree - 3);
	c = *qBig * f1n*s;
	e = f2n * t;
	bBig = c - e - f3n * u;			// bottom row
	*zBig = *zBig - aBig / bBig;
	zdBig = *zBig - z1Big;
	d = zdBig.CSumSqr();
	return (d < MINSIZE);
	}

	/*
		case 56:					// More Newton Msets, 25th order Newton Mset flower
		case 57:					// More Newton Msets, 38th order Newton Mset flower
		case 58:					// More Newton Msets, 50th order Newton Mset flower
		    {
		    double	s, t, u;
		    Complex	fn, f1n, f2n, f3n, c, e, f;

		    f3n = z->CPolynomial(degree - 4);		// z^(deg - 4) - third derivative power
		    f2n = f3n * *zBig;				// z^(deg - 2) - second derivative power
		    f1n = f2n * *zBig;				// z^(deg - 1) - first derivative power
		    fn = f1n * *zBig;				// z^deg - function power
		    c = *q * fn*((double)degree);
		    e = f1n * ((double)(degree - 1));
		    a = c - e - f2n * ((double)(degree - 2));	// top row
		    s = (double)degree * (double)(degree - 1);
		    t = (double)(degree - 1) * (double)(degree - 2);
		    u = (double)(degree - 2) * (double)(degree - 3);
		    c = *q * f1n*s;
		    e = f2n * t;
		    b = c - e - f3n * u;			// bottom row
		    *z = *z - a / b;
		    zdBig = *zBig - z1Big;
		    d = zdBig.CSumSqr();
		    return (d < MINSIZE);
		    }
	*/

	case 59:					// More Newton Msets, 5th order Newton Mset
	{
	BigDouble	s, t;
	BigComplex	fn, f1n, f2n, c;

	z1Big = *zBig;
	f2n = zBig->CPolynomial(*degree - 3);		// z^(deg - 3) - second derivative power
	f1n = f2n * *zBig;				// z^(deg - 2) - first derivative power
	fn = f1n * *zBig;				// z^(deg - 1) - function power
	c = *qBig * fn*((double)*degree);
	aBig = f1n * ((double)(*degree - 1)) + c - 1.0;
	t = (double)*degree * (double)(*degree - 1);
	s = (double)(*degree - 1) * (double)(*degree - 2);
	c = *qBig * f1n*t;
	bBig = f2n * s + c;
	*zBig = *zBig - aBig / bBig;
	zdBig = *zBig - z1Big;
	d = zdBig.CSumSqr();
	return (d < MINSIZE);
	}

	case 60:					// More Newton Msets, 18th order Newton Mset flower
	{
	BigComplex	z8, z16;

	z1Big = *zBig;
	z2Big = zBig->CSqr();
	z4Big = z2Big.CSqr();
	z8 = z4Big.CSqr();
	z16 = z8.CSqr();
	*zBig = *zBig - (z16*z2Big * *qBig * 19 + z16 * *zBig * 18 - *zBig) / (z16 * *zBig * *qBig * 342 + z16 * 306 - 1);
	zdBig = *zBig - z1Big;
	d = zdBig.CSumSqr();
	return (d < MINSIZE);
	}

	case 61:					// Polynomials, z=z*z+c; real=imag; imag=real
	{
	BigDouble t;

	z1Big = *zBig;
	*zBig = zBig->CSqr() + *qBig;
	t = zBig->x;
	zBig->x = zBig->y;
	zBig->y = t;
	zdBig = *zBig - z1Big;
	d = zdBig.CSumSqr();
	return (d < MINSIZE || d > MAXSIZE);
	}

	case 62:					// Newton Variations II, z=(((z^3)-z-1)/((z^3)-1)-z)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = ((*zBig*z2Big - *zBig - 1) / (z2Big * *zBig - 1) - *zBig) * *qBig;   // 3rd order M-Set
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 63:					// Newton Variations II, z=(((z^4)-z*z-1)/(4*z*z-1)-z)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = ((z2Big.CSqr() - z2Big - 1) / (z2Big * 4 - 1) - *zBig) * *qBig;  // 
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 64:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-fabs(z)-1))*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = (*zBig - (*zBig*z2Big - 1) / (z2Big * 3 - zBig->CFabs() - 1)) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 65:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-z-1))*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = (*zBig - (*zBig*z2Big - 1) / (z2Big * 3 - *zBig - 1)) * *qBig;  // Almost makes a connection
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 66:					// Newton Variations II, z=(((z^4)-z-1)/(4*(z^3)-z-1)-z)*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = ((*zBig*z3Big - *zBig - 1) / (z3Big * 4 - *zBig - 1) - *zBig) * *qBig;  // Some strange curls
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 67:					// Newton Variations II, z=z-(((z^3)-z)/((3*z*z-1)))+c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - ((*zBig*z2Big - *zBig) / ((z2Big * 3 - 1))) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 68:					// Newton Variations II, z=(z-((z^3)-1)/(4*z*z-z-1))*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = (*zBig - (*zBig*z2Big - 1) / (z2Big * 4 - *zBig - 1)) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 69:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-z))*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = (*zBig - (*zBig*z2Big - 1) / (z2Big * 3 - *zBig)) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 70:					// Newton Variations II, z=(z-((z^4)-1)/(4*(z^3)-z))*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = (*zBig - (*zBig*z3Big - 1) / (z3Big * 4 - *zBig)) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 71:					// Newton Variations II, z=(z-((z^4)-1)/(3*(z^3)-z))*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = (*zBig - (*zBig*z3Big - 1) / (z3Big * 3 - *zBig)) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 72:					// Newton Variations II, z=(z-((z^4)-z-1)/(3*(z^3)-z))*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = (*zBig - (*zBig*z3Big - *zBig - 1) / (z3Big * 3 - *zBig)) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 73:					// Newton Variations II, z=c*(z-((z^3)-z)/(3*z*z-1))
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = (*zBig - (*zBig*z2Big - *zBig) / (z2Big * 3 - 1));  // 105
	    *zBig = *zBig * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 74:					// Newton Variations II, z=((1-z-(z^4))/(z-(4*(z^3)))-z)*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    aBig = 1;
	    *zBig = ((aBig - *zBig - z3Big * *zBig) / (*zBig - (z3Big * 4)) - *zBig) * *qBig;  // sort of a good one
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 75:					// Newton Variations II, z=((z-(z^3)*z)/(z-(3*(z^3)))-z)*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = ((*zBig - z3Big * *zBig) / (*zBig - (z3Big * 3)) - *zBig) * *qBig;  // 107
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 76:					// Newton Variations II, z=((z-(z^3))/(z-(3*(z^3)))-z)*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = ((*zBig - z3Big) / (*zBig - (z3Big * 3)) - *zBig) * *qBig;  // 108
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 77:					// Newton Variations II, z=(((z^3)-1)/(2*z*z-1)-z)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = ((z2Big * *zBig - 1) / (z2Big * 2 - 1) - *zBig) * *qBig;  // 109
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 78:					// Newton Variations II, z=(((z^3)-z-1)/(3*(z^3)-1)-z)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = ((*zBig*z2Big - *zBig - 1) / (*zBig*z2Big * 3 - 1) - *zBig) * *qBig; // 2nd Order M-Set 110
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 79:					// Polynomials, CBAP  F(z) = Z^3 - 3*(A^2)*Z + B(MOD 2)
	    z1Big = *zBig;
	    *zBig = zBig->CCube() - caa3Big * *zBig + cbBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 80:					// Quartets, z=.5; z=z*z-z2*z2+c; z2=z
	    z1Big = *zBig;
	    *zBig = zBig->CSqr() - z2Big.CSqr() + *qBig;
	    z2Big = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 81:					// Quartets, z2=2; z=z*z*z*z+z2+c; z2=z1
	    z1Big = *zBig;
	    ztBig = zBig->CSqr();
	    *zBig = ztBig.CSqr() + z2Big + *qBig;
	    z2Big = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 82:					// Quartets, z2=z; z=z*z*z*z+5*z2*c; z2=z1
	    z1Big = *zBig;
	    ztBig = zBig->CSqr();
	    *zBig = ztBig.CSqr() + z2Big * *qBig * 5;
	    z2Big = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 83:					// Quartets, t=0; z1=z; z=z*z*z-t*t*t+c; z=z1
	    z1Big = *zBig;
	    *zBig = zBig->CCube() - z2Big.CCube() + *qBig;
	    z2Big = z1Big;
	    //    z=z*z*z-t*t*t+c;
	    //    t=z1;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 84:					// Quartets, t=0; z1=z; z=z*z*z*z-t*t*t*t+c; t=z1
	    z1Big = *zBig;
	    aBig = zBig->CPolynomial(*degree) + *qBig;
	    bBig = z2Big.CPolynomial(*degree);	// global z2 used for t
	    *zBig = aBig - bBig;
	    z2Big = z1Big;				// t = z1
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 85:					// Quartets, z2=z; z=(z^4)+c; c=z2
	    z1Big = *zBig;
	    z2Big = *zBig;
	    //  z = z*z*z*z+q;
	    *zBig = zBig->CPolynomial(*degree) + *qBig;
	    *qBig = z2Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 86:					// Newton Variations, z=z-((z^4)-z)/(4*(z^3)-1); z=z*z*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = *zBig - (*zBig*z3Big - *zBig) / (z3Big * 4 - 1);
	    *zBig = zBig->CSqr() * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 87:					// Newton Variations, z=z-((z^3)-1)/(3*z*z); z=(z^3)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big - 1) / (z2Big * 3);
	    *zBig = zBig->CCube() * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 88:					// Newton Variations, z=z-((z^4)-1)/(4*(z^3)); z=(z^4)*c
	    z1Big = *zBig;
	    z3Big = zBig->CCube();
	    *zBig = *zBig - (*zBig*z3Big - 1) / (z3Big * 4);
	    *zBig = zBig->CCube() * *zBig * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 89:					// Newton Variations, z=z-((z^5)-1)/(5*(z^4)); z=(z^5)*c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = *zBig - (*zBig*z4Big - 1) / (z4Big * 5);
	    *zBig = zBig->CCube()*zBig->CSqr() * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 90:					// Polynomials, z=z+c [quad]
	    z1Big = *zBig;
	    *zBig = *zBig + *qBig;
	    zBig->x += (zBig->x > (BigDouble)0.0) ? -2.0 : 2.0;
	    zBig->y += (zBig->y > (BigDouble)0.0) ? -2.0 : 2.0;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > rqlim);

	case 91:					// Polynomials, z=z*z*z-caa3*z+cb; [CCAP]
	    z1Big = *zBig;
	    aBig = caa3Big * *zBig - cbBig;
	    *zBig = zBig->CCube() - aBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 92:					// Polynomials, z=z*z*z-caa3*z+cb; [CFAP]
	    z1Big = *zBig;
	    aBig = caa3Big * *zBig;
	    *zBig = zBig->CCube() - aBig + cbBig;
	    //    z = z*z*z-caa3*z+cb;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 93:					// Quartets, z1=z; z=z*z*z*z-z2+c; z2 = z1
	    z1Big = *zBig;
	    ztBig = zBig->CSqr();
	    *zBig = ztBig.CSqr() - z2Big + *qBig;
	    z2Big = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 94:					// Polynomials, z=c*z*(2 - z*z)
	    z1Big = *zBig;
	    aBig = -*zBig * *zBig + 2;
	    bBig = aBig * *zBig;
	    *zBig = bBig * *qBig;
	    //    z = c*z*(2 - z*z);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 95:					// Polynomials, z=z*z*z*z/t1+c
	    z1Big = *zBig;
	    ztBig = zBig->CSqr();
	    aBig = ztBig.CSqr();
	    *zBig = aBig / z2Big + *qBig;
	    //    z = z*z*z*z/t1+c;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 96:					// Quartets, z1=z; z=z*z*z*z+z2/2+c;; z2=z1
	    z1Big = *zBig;
	    //    z=z*z*z*z+z2/2+q;
	    *zBig = zBig->CPolynomial(*degree) + z2Big / 2.0 + *qBig;
	    z2Big = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 97:					// Polynomials, z = z*z+c*(1+sqrt(z))
	    z1Big = *zBig;
	    z2Big = zBig->CSumSqr();
	    *zBig = *zBig * *zBig + *qBig * (z2Big + 1);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 98:					// Quartets, z1=z; z=z*z*z*z/(1+z)+c;; c=z1
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = z2Big.CSqr() / (*zBig + 1) + *qBig;
	    *qBig = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 99:					// Quartets, z1=z; z2=c+z2/z-z; z=z*c+z2/c
	    z1Big = *zBig;
	    z2Big = z2Big / *zBig + *qBig - *zBig;
	    aBig = z2Big / *qBig;
	    *zBig = *zBig * *qBig + aBig;
	    //    z2=c+z2/z-z;
	    //    z=z*c+z2/c;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 100:					// Polynomials, real(z) += sin(real(z)); z=z*z+c
	    z1Big = *zBig;
	    //__imag__ z += sin(__imag__ z);
	    zBig->x += zBig->x.BigSin();
	    *zBig = *zBig * *zBig + *qBig;
	    //temp = __real__ z;
	    //__imag__ z = __real__ z;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 102:					// Ramon, z = ((z*z+c-1)/(2*z+c-2))^2; [magneto 1]
	    z1Big = *zBig;
	    //z = ((z*z+c-1)/(z+z+c-2))*((z*z+c-1)/(z+z+c-2));
	    aBig = zBig->CSqr() + *qBig - 1;
	    bBig = *zBig * 2 + *qBig - 2;
	    *zBig = aBig / bBig;
	    //    z = (z*z+q-1)/(z*2+q-2);
	    *zBig = zBig->CSqr();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    //    return (d < MINSIZE);
	    return (d < MINSIZE || d > MAXSIZE);

	case 103:					// Ramon, z = ((z*z*z+3*c2*z+c1)/(3*z*z+3*c2*z+c1+1))^2; [magneto 2]
	{
	BigComplex c2z3;

	z1Big = *zBig;
	// z = ((z*z*z+(3*(c-1)*z)+((c-1)*(c-2)))/(3*z*z+(3*(c-2)*z)+((c-1)*(c-2))+1))*((z*z*z+(3*(c-1)*z)+((c-1)*(c-2)))/(3*z*z+(3*(c-2)*z)+((c-1)*(c-2))+1));
	z2Big = zBig->CSqr();
	//    z = (z*z2+3*c2*z+c1)/(3*z2+3*c2*z+c1+1);	
	c2z3 = *zBig * c2Big*3.0;
	aBig = *zBig * z2Big + c2z3 + c1Big;
	bBig = z2Big * 3 + c2z3 + c1Big + 1;
	*zBig = aBig / bBig;
	*zBig = *zBig * *zBig;

	zdBig = *zBig - z1Big;
	d = zdBig.CSumSqr();
	//    return (d < MINSIZE);
	return (d < MINSIZE || d > MAXSIZE);
	}

	case 104:					// Ramon, z = ((z*z+c)/(2*z))^2
	    z1Big = *zBig;
	    *zBig = (*zBig * *zBig + *qBig) / (*zBig + *zBig);
	    *zBig = zBig->CSqr();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 105:					// More Fractals, 3rd order Newton/ Mset [figure 8]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - ((*zBig*z2Big + z2Big * *qBig - *zBig) / (z2Big * 3 + *zBig * *qBig * 2 - 1));
	    zdBig = z1Big - *zBig;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 107:					// More Fractals, z1Big = z; z = sin(z) - c; c = 1/(z*50); [quartet]
	    z1Big = *zBig;
	    *zBig = zBig->CSin();
	    *zBig = *zBig - *qBig;
	    aBig = *zBig * 50;
	    *qBig = aBig.CInvert();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 108:					// More Fractals, z=(z*c)/(1+z)+c; [Talis II]
	    *zBig = (*zBig * *qBig) / (*zBig + 1) + *qBig;
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 109:					// More Fractals, z=(c+z*z*c)/(1-z*z*c)
	    z2Big = zBig->CSqr() * *qBig;
	    //    z2 = z*z*qBig;
	    *zBig = (*qBig + z2Big) / (-z2Big + 1);
	    d = zBig->CSumSqr();
	    return (d > rqlim);

	case 110:					// More Fractals, z = (c+(z^6))/(1+(z^2))
	    z1Big = *zBig;
	    *zBig = zBig->CSqr();
	    *zBig = (*qBig + zBig->CCube()) / (*zBig + 1);
	    zdBig = z1Big - *zBig;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 111:					// Ramon, z = (z^2)/(1+c*(z^2))
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = z2Big / (*qBig*z2Big + 1);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 112:					// Ramon, z = c*cos(z)
	    z1Big = *zBig;
	    *zBig = *qBig * zBig->CCos();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 113:					// Ramon, z = c*sin(z)
	    z1Big = *zBig;
	    *zBig = *qBig * zBig->CSin();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 114:					// Ramon, z = c*exp(z)
	    z1Big = *zBig;
	    *zBig = *qBig * zBig->CExp();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 115:					// Ramon, z=z^2+c; c=c/2+z [spider]
	    z1Big = *zBig;
	    *zBig = zBig->CSqr() + *qBig;
	    *qBig = *qBig / 2 + *zBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE || d > rqlim);

	case 116:					// Ramon, z = (c/sin(z))^2
	    z1Big = *zBig;
	    *zBig = *qBig / zBig->CSin();
	    *zBig = zBig->CSqr();
	    zdBig = *zBig - z1Big;
	    return (zdBig.CSumSqr() < MINSIZE || zBig->CSumSqr() > rqlim);

	case 117:					// Ramon, z = (c/cos(z))^2
	    z1Big = *zBig;
	    *zBig = *qBig / zBig->CCos();
	    *zBig = zBig->CSqr();
	    zdBig = *zBig - z1Big;
	    return (zdBig.CSumSqr() < MINSIZE || zBig->CSumSqr() > rqlim);

	case 119:					// Flarium 06, Newton Variations: z=z-(z*z*z-1)/3*z*zBig; z=z*c
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z1Big - 1) / 3 * z1Big;
	    *zBig = *zBig * *qBig;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 120:					// Flarium 07, Polynomial: z = (z*z+c)^(cn+c)
	    z1Big = *zBig;
	    *zBig = (*zBig * *zBig + *qBig) ^ (z2Big + *qBig);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 121:					// Flarium 08, Sharon Webb: z = z*z*z*z+1/c; [Sharon Star]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = z4Big + qBig->CInvert();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 122:					// Flarium 09, Sharon Webb: z = (z*z/2+c)*(z*z/2+c); [Sharon's Space Probe]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr() / 2 + *qBig;
	    *zBig = z2Big.CSqr();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 123:					// Flarium 10, Sharon Webb: z = z*z*z*z+((c/2)^2)+c; [Sharon08]
	    {
	    BigComplex z2a;

	    z1Big = *zBig;
	    z2a = zBig->CSqr();
	    z4Big = z2a.CSqr();
	    *zBig = z4Big + z2Big + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 124:					// Flarium 11, Sharon Webb: z = z*z*z*z+(z^(z+cn))+c
	    {
	    BigComplex z2a;

	    z1Big = *zBig;
	    z2a = zBig->CSqr();
	    z4Big = z2a.CSqr();
	    *zBig = z4Big + z2Big + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 125:					// Flarium 12, Sharon Webb: z = (z+z*z/2)+c
	    z1Big = *zBig;
	    *zBig = (*zBig + zBig->CSqr() / 2) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 126:					// Flarium 13, Sharon Webb: z=(z*z*z*z-z2*z2*z2*z2+c)^2
	    {
	    BigComplex	z2a, quadz2;

	    z1Big = *zBig;
	    z2a = zBig->CSqr();
	    z4Big = z2a.CSqr();
	    z2a = z2Big.CSqr();
	    quadz2 = z2a.CSqr();
	    //    z = (z*z*z*z-z2*z2*z2*z2+q)*(z*z*z*z-z2*z2*z2*z2+q);
	    aBig = (z4Big - quadz2 + *qBig);
	    *zBig = aBig.CSqr();
	    z2Big = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 127:					// Flarium 15, Newton Variations: z = z - (z*z*z*z-1)/(4*z*z*z) + c; [4th Order Nova]
	    z1Big = *zBig;
	    z2Big = zBig->CCube();
	    *zBig = *zBig - (z2Big * *zBig - 1) / (z2Big * 4) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 128:					// Flarium 16, Newton Variations: z = z - (z*z*z*z*z-1)/(5*z*z*z*z) + c; [5th Order Nova]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = *zBig - (z4Big * *zBig - 1) / (z4Big * 5) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 129:					// Flarium 20, Phoenix: z = z*z*z*z+c.imag()+c.real()*z2; [4th Order Phoenix]
	    {
	    BigComplex z2a;

	    z1Big = *zBig;
	    z2a = zBig->CSqr();
	    z4Big = z2a.CSqr();
	    *zBig = z4Big + qBig->y + z2Big * qBig->x;
	    z2Big = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	}

	case 130:					// Flarium 21, Sharon Webb: z=(1/z*z-c)*(z*z*z*z+c); [Sharon03]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = (zBig->CInvert() * *zBig - *qBig)*(z4Big + *qBig);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 131:					// Flarium 25, Newton Variations: z=z-(z*z*z*z-z)/(4*z*z*z-z); z=z*c
	    z2Big = *zBig;
	    z1Big = zBig->CCube();
	    *zBig = *zBig - (*zBig*z1Big - *zBig) / (z1Big * 4 - *zBig);
	    *zBig = *zBig * *qBig;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 132:					// Flarium 27, Polynomial: z=z*z*(cn+z)/(cn+z+c)+c
	    z1Big = *zBig;
	    *zBig = zBig->CSqr()*(z2Big + *zBig) / (z2Big + *zBig + *qBig) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 133:					// Flarium 28, Derbyshire / Newton: z=z - (z*z*z-1)/(3*z*z)+c; [Nova-Mandelbrot-MultiFract]
	    {
	    int		i;

	    z1Big = *zBig;
	    if (*iteration >= threshold)
		{
		// Inside
		*qBig = *zBig;
		*zBig = *zBig + 1;
		i = 0;
		d = 1;
		while (d > MINSIZE && i++ < threshold)
		    {
		    z1Big = *zBig;
		    *zBig = *zBig - (zBig->CCube() - 1) / (zBig->CSqr() * 3) + *qBig;
		    zdBig = *zBig - z1Big;
		    d = zdBig.CSumSqr();
		    }
		*iteration = i;
		return TRUE;
		}
	    *zBig = zBig->CSqr() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 134:					// Flarium 29, Derbyshire / Newton: c = (z*z*z-1)/(3*z*z); z -= (z*z*z-1)/(3*z*z)*c
	    z1Big = *zBig;
	    *zBig -= (zBig->CCube() - 1) / (zBig->CSqr() * 3) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 135:					// Flarium 30, Derbyshire / Newton: z -= (z*z*z-1)/(3*z*z)*c
	    z1Big = *zBig;
	    *zBig -= (zBig->CCube() - 1) / (zBig->CSqr() * 3) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 136:					// Flarium 31, Polynomial: z = z*z*z-aa3*z+b
	    z2Big = *zBig;
	    *zBig = zBig->CCube() - caa3Big * *zBig + bBig;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 137:					// Flarium 32, Derbyshire / Newton: 3rd Order Nova in a M-Set (Try single pass)
	    z2Big = *zBig;
	    z1Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z1Big - 1) / (z1Big * 3) + *qBig;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 138:					// Flarium 34, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14]
	    z2Big = *zBig;
	    aBig = zBig->CSqr();
	    z1Big = *zBig * aBig.CSin() / 2;
	    *zBig = z1Big.CSqr() + *qBig;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 139:					// Flarium 35, Polynomial: z=c*(z.csin())
	    z2Big = *zBig;
	    *zBig = *qBig * zBig->CSin();
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 140:					// Flarium 36, Polynomial: z=(c*z).csin()
	    z2Big = *zBig;
	    aBig = *zBig * *qBig;
	    *zBig = aBig.CSin();
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 141:					// Flarium 37, Polynomial: z=(z*z*z-1)/(3*z*z); z = c*(z.csin() + z.ccos())
	    z2Big = *zBig;
	    *zBig = *qBig * (zBig->CSin() + zBig->CCos());
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 142:					// Flarium 38, Polynomial: z = z*z+c; z.set_real(z.real()*z.real()); [Variation real]
	    z2Big = *zBig;
	    *zBig = *zBig * *zBig + *qBig;
	    zBig->x = zBig->x*zBig->x;
	    zdBig = *zBig - z2Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 143:					// Flarium 40, Polynomial: z1Big = z*z*z*zBig; z = c*z1/4*z1 + z
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = *qBig * z4Big / 4 * z4Big + *zBig;
	    zdBig = *zBig - z4Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 144:					// Flarium 41, Polynomial: z = c*(z*z*z*z).csin()
	    z1Big = *zBig;
//	    z2Big = zBig->CSqr();
//	    z4Big = z2Big.CSqr();
//	    *zBig = *qBig * z4Big.CSin();		// CSin() stuffs up, but sin(x)  = cos(x+pi/2)
	    z4Big = zBig->CPolynomial(4);
	    z4Big = z4Big + (PI / 2);
	    *zBig = *qBig * z4Big.CCos();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 145:					// Flarium 43, Polynomial: z = z*z*z*z + (z*c).csin() + c
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    aBig = *zBig * *qBig;
	    *zBig = z4Big + aBig.CSin() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 146:					// Flarium 44, Polynomial: z=(z*z*z*z-z)/(4*z*z*z); z=c*z.csin()
	    z1Big = *zBig;
	    *zBig = *qBig * zBig->CSin();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 147:					// Flarium 45, Polynomial: z=(z*z*z*z-1)/(4*z*z*z); z=c*z.csin()
	    z1Big = *zBig;
	    *zBig = *qBig * zBig->CSin();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 148:					// Flarium 46, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14 N-Method]
	    z1Big = *zBig;
	    aBig = zBig->CSqr();
	    z2Big = aBig.CSin() * *zBig / 2;
	    *zBig = z2Big.CSqr() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 149:					// Flarium 49, Polynomial: z=(z*z*z*z-z)/((4*z*z*z)-z); z=c*z.csin()
	    z1Big = *zBig;
	    *zBig = *qBig * zBig->CSin();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 150:					// Flarium 50, Sharon Webb: z = z*z*z*z+1/c; [Sharon's Star M-Set]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = z4Big + qBig->CInvert();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 151:					// Flarium 51, Sharon Webb: z = (z*z/2+c)*(z*z/2+c); [Space Probe M-Set]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr() / 2 + *qBig;
	    *zBig = z2Big.CSqr();
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 152:					// Flarium 52, Sharon Webb: z = z*z*z*z+t+c; [Sharon08 M-Set]
	    {
	    BigComplex	z2a;

	    z1Big = *zBig;
	    z2a = zBig->CSqr();
	    z4Big = z2a.CSqr();
	    *zBig = z4Big + z2Big + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 153:					// Flarium 53, Sharon Webb: z = (z+z*z/2)+c; [M-Set]
	    z1Big = *zBig;
	    *zBig = (*zBig + zBig->CSqr() / 2) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 154:					// Flarium 54, Polynomial: z = z*z*c+c; [M-Set]
	    z1Big = *zBig;
	    *zBig = zBig->CSqr() * *qBig + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 155:					// Flarium 55, Polynomial: z = (z*z).csin()*z*z*c+c; [M-Set]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = z2Big.CSin() * zBig->CSqr() * *qBig + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 156:					// Flarium 56, Polynomial: z = (z*z+c)/(z*z-c); [M-Set]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = (z2Big + *qBig) / (z2Big - *qBig);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 157:					// Flarium 57, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-z)+c; [5th Order Nova Variation]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = *zBig - (*zBig*z4Big - *zBig) / ((z4Big * 5) - *zBig) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 158:					// Flarium 58, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big - *zBig) / ((z2Big * 3) - *zBig) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 159:					// Flarium 59, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-1)+c; [3rd Order Nova Variation]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    *zBig = *zBig - (*zBig*z2Big - *zBig) / ((z2Big * 3) - 1) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 160:					// Flarium 60, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-1)+c; [5th Order Nova Variation]
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = *zBig - (*zBig*z4Big - *zBig) / ((z4Big * 5) - 1) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 161:					// Flarium 61, Polynomial: z=z*z*z*z+(c+(c/pi))
	    z1Big = *zBig;
	    z2Big = zBig->CSqr();
	    z4Big = z2Big.CSqr();
	    *zBig = z4Big + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 162:					// Flarium 62, Polynomial: z=z*z*(1+z)/(1+z+c)+c
	    z1Big = *zBig;
	    *zBig = zBig->CSqr()*(*zBig + 1) / (*zBig + *qBig + 1) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 163:					// Flarium 64, PHOENIX: z1=z; z=z*z*z*z+c.real()*z2/2+c.imag()*z2/2+c; z2=z1
	    {
	    BigComplex	z2a;

	    z1Big = *zBig;
	    z2a = zBig->CSqr();
	    z4Big = z2a.CSqr();
	    *zBig = z4Big + z2Big * qBig->x * qBig->y / 2 + *qBig;
	    z2Big = z1Big;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 164:					// Flarium 66, Newton Variations: z = ((z-(((z^2)-1)/(2*z)))^2)*c
	    z1Big = *zBig;
	    *zBig = *zBig - (zBig->CSqr() - 1) / (*zBig * 2);
	    *zBig = zBig->CSqr() * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 165:					// Flarium 67-69, Newton Variations: z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c
	    {
	    BigComplex	fn, f1n;

	    z1Big = *zBig;
	    f1n = zBig->CPolynomial(*degree - 1);	// z^(deg - 1) - first derivative power
	    fn = f1n * *zBig;				// z^(deg)
	    *zBig = *zBig - (fn - 1) / (f1n * *degree);
	    *zBig = zBig->CSqr() * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 166:					// Flarium 80, Newton Variations: z=((z*z*z*z-1)/(1-(3*z*z*z)))*c
	    z1Big = *zBig;
	    z2Big = zBig->CCube();   // A correct one, finally
	    aBig = 1;
	    *zBig = ((*zBig*z2Big - 1) / (aBig - (z2Big * 3))) * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 167:					// Flarium 90, Sharon Webb: z = (z*(z*z).csin()/2).csin()+c; [Sharon15]
	    {
	    BigComplex	s;

	    z1Big = *zBig;
	    aBig = zBig->CSqr();
	    s = *zBig * aBig.CSin() / 2;
	    *zBig = s.CSin() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 168:					// Flarium 91, Sharon Webb: z = (z*(z+z).ccos()/2); z = z*z+c; [Sharon16]
	    z1Big = *zBig;
	    aBig = *zBig + *zBig;
	    *zBig = (*zBig*aBig.CCos() / 2);
	    *zBig = zBig->CSqr() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 169:					// Flarium 92, Sharon Webb: z = (z*(z+z).csin()/2); z = z*z+c; [Sharon17]
	    z1Big = *zBig;
	    aBig = *zBig + *zBig;
	    *zBig = (*zBig*aBig.CSin() / 2);
	    *zBig = zBig->CSqr() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 170:					// Flarium 93, Sharon Webb: z = (z*(z+z*z).csin()/2); z = z*z+c; [Sharon18]
	    z1Big = *zBig;
	    aBig = *zBig + zBig->CSqr();
	    *zBig = (*zBig*aBig.CSin() / 2);
	    *zBig = zBig->CSqr() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 171:					// Flarium 111, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Alden's Ray Method]
	    z1Big = *zBig;
	    aBig = zBig->CSqr();
	    //    z2 = z*CSinError(CSqr(z))/2;
	    z2Big = *zBig * aBig.CSin() / 2;
	    *zBig = z2Big.CSqr() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 172:					// Flarium 112-116, Polynomials: z=z^n*c+z*c; Dragon curve variations
	    {
	    BigComplex	fn;

	    z1Big = *zBig;
	    fn = zBig->CPolynomial(*degree);
	    *zBig = fn * *qBig + *zBig * *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }

	case 173:					// Flarium 117, Newton Variations: z = z-(z*z*z-z*c-1)/(3*z*z+c-1)
	    z1Big = *zBig;
	    *zBig = *zBig - (*zBig * *zBig * *zBig - *zBig * *qBig - 1) / (*zBig * *zBig * 3 + *qBig - 1);
	    //    z = z-(CCube(z)-z*q-1)/(3*CSqr(z)+q-1);
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d < MINSIZE);

	case 174:					// Flarium 118, Sharon Webb: z = (z*z*z*z*z).csin() + c; [Sharon19]
	    z1Big = *zBig;
	    aBig = zBig->CCube()*zBig->CSqr();
	    *zBig = aBig.CSin() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 175:					// Flarium 119, Sharon Webb: z = (z+(z*z)/.192).csin() + c; [Sharon's Butterfly]
	    z1Big = *zBig;
	    aBig = *zBig + (*zBig * *zBig) / 0.192;
	    *zBig = aBig.CSin() + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 176:					// Flarium 125, Sharon Webb: z = z+z*z*z/4 + c; [Sharon21]
	    z1Big = *zBig;
	    *zBig = *zBig + zBig->CCube() / 4 + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);

	case 177:					// Flarium 145, Polynomial: z=z^2+c [Jaenisch method]
	    {
	    int		i, rzflag, jrw;
	    int		nFF = 1;
	    long	NMAX = (threshold * 2) / 3;
	    double	r, rz;
	    long	LocalIteration;
	    BigDouble	s;
	    //    BOOL	b_MAX;

	    z1Big = *zBig;
	    if (*iteration >= threshold)
		{
		// Inside
		*zBig = z2Big;
		i = 0;
		jrw = 0;
		LocalIteration = NMAX - 2;
		while (i++ < LocalIteration)
		    {
		    //z=z*z + c;
	//	    z=CCube(z) + q;
	//	    z = CPolynomial(z, degree) + q;
		    *zBig = *zBig ^ z4Big + *qBig;
		    tBig = 0;
		    if (zBig->x < tBig)
			{
			s = zBig->x.BigAbs();
			zBig->x = -(s.BigLog10());
			}
		    else if (zBig->x > tBig)
			{
			s = zBig->x.BigAbs();
			zBig->x = s.BigLog();
			}
		    else zBig->x = 0;

		    if (zBig->y < (BigDouble)0)
			{
			s = zBig->y.BigAbs();
			zBig->y = -(s.BigLog10());
			}
		    else if (zBig->y > tBig)
			{
			s = zBig->y.BigAbs();
			zBig->y = s.BigLog10();
			}
		    else zBig->y = 0;

		    r = zBig->CSumSqr();

		    if (i == 0)
			{
			//		k1 = 1; 
			rz = r;
			}
		    else if (i == 1)
			{
			if (r > rz)
			    rzflag = 1;
			else
			    rzflag = -1;
			rz = r;
			}
		    else
			{
			if (r > rz && rzflag != 1)
			    {
			    jrw += nFF;
			    rzflag = 1;
			    }
			else
			    {
			    if (r < rz && rzflag != -1)
				{
				jrw += nFF;
				rzflag = -1;
				}
			    }
			rz = r;
			}
		    }
		*iteration = jrw;
		return TRUE;
		}
	    *zBig = zBig->CPolynomial(*degree) + *qBig;
	    zdBig = *zBig - z1Big;
	    d = zdBig.CSumSqr();
	    return (d > rqlim);
	    }
	}
    return 0;
    }


