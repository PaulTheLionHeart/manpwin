////////////////////////////////////////////////////////////////
// Tierazon.cpp a module for the per pixel calculations of Tierazon fractals. 
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
#include	"Complex.h"
#include	"fractype.h"
#include	"pixel.h"

/**************************************************************************
	Initialise functions for each pixel
**************************************************************************/

int	CPixel::InitTierazonFunctions(int subtype, Complex *z, Complex *q)
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
	    InitFunctions(MANDELFP, z, q);
	    break;

	case 10:					// z=z*z*z+c
	case 11:					// z=z*z*z*z+c
	case 12:					// z=z*z*z*z*z+c
	case 13:					// z=z*z*z*z*z*z+c
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    InitFunctions(POWER, z, q);
	    break;

	case 2:						// Nova, init: z=1; iterate: z=z-((z*z*z-1)/(3*z*z))+c
	    if (!juliaflag)
		{
		//	Note that julia parameters are also used as starting values for Mandelbrot
		z->x = param[0] + 1.0;
		z->y = param[1];
		}
	    //    else
	    //	z = q;
	    break;

	case 3:						// Ushiki's Phoenix, z=z*z-.5*z+c; z=z*z-.5*z2+c; z2=z; z=z1
	    {
	    c1.x = 0;
	    c1.y = 1;
	    if (!juliaflag)
		*z = *q;
	    // Rotate it so its pointing upward
	    *z = *z * c1;
	    *q = *q * c1;
	    z2 = 0;
	    break;
	    }

	case 6:						// z=z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)
	case 7:						// Newton variation, z = z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)
	case 61:					// Polynomials, z=z*z+c; real=imag; imag=real
	case 102:					// Ramon, z = ((z*z+c-1)/(2*z+c-2))^2; [magneto 1]
	    if (!juliaflag)
		{
		z->x = param[0];
		z->y = param[1];
		}
	    break;

	case 8:						// Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-1)+c
	case 9:						// Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-z)+c
	    if (!juliaflag)
		{
		//	Note that julia parameters are also used as starting values for Mandelbrot
		z->x = param[0] + 1.0;
		z->y = param[1];
		}
	    //    else
	    //	z = q;
	    break;

	case 14:					// z1=z*z+c; z=z*z+c*z2; z2=z
	    if (!juliaflag)
		{
		*z = *q;
		z->x += param[0];
		z->y += param[1];
		}
	    z2 = 1.0;
	    break;

	case 15:					// Phoenix II, z1=z; z=z*z + real(c) + imag(c)*z2; z2=z1
	case 16:					// Phoenix III, z1=z; z=z*z*z +   real(c) + imag(c)*z2; z2=z1
	case 17:					// Phoenix IV,  z1=z; z=z*z*z +.5*real(c) + imag(c)*z2; z2=z1
	    c1.x = 0;
	    c1.y = -1;
	    if (!juliaflag)
		*z = *q;
	    // Rotate it so its pointing upward
	    if (subtype != 17)
		{
		*z = *z * c1;
		z2 = 0;
		}
	    else
		z2 = *z;
	    *q = *q * c1;

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
		*z = -*q / 3;
	    break;
	case 20:					// Newton/Mandel, z= z/2; iterate: z=z-(z^4-(z^3)*c-z-c)/(4*(z^3)-3*(z^2)*c-1)
	    if (!juliaflag)
		*z = *q / 2;
	    break;

	case 21:					// Newton/Mandel, z=-1/(3*z); iterate: z=z-((z^3)*c+z^2+z+c)/(3*(z^2)*c+2*z+1)
	    if (!juliaflag)
		*z = -(*q * 3);
	    *z = z->CInvert();
	    break;

	case 22:					// Newton/Mandel, z=-.5; iterate: z=z-((z^4)c+(z^3)*c+z+c)/(4*(z^3)*c+3*z*z*c+1)
	    if (!juliaflag)
		*z = -0.5;
	    break;

	case 23:					// Newton/Mandel, z=-.5/z; iterate: z=z-((z^4)*c+(z^3)+z+c)/(4*(z^3)*c+3*z*z+1)
  	    if (!juliaflag)
		*z = *q / -0.5;
	    *z = z->CInvert();
	    break;

	case 25:					// Newton/Mandel, z=-z/2;  iterate: z=z-((z^4)+(z^3)*c+c)/(4*(z^3)+3*z*z*c)
	    if (!juliaflag)
		*z = -*q / 2;
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
		z->x = -q->x * (double)(*degree - 2) / (double)*degree + param[1];
		z->y = -q->y * (double)(*degree - 2) / (double)*degree + param[2];
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
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    if (subtype == 50)
		*q = q->CCos();
	    break;

	case 55:					// More Newton Msets, 4th order Newton's apple
//	case 59:					// More Newton Msets, 5th order Newton Mset
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*z = *q * ((double)*degree);
	    *z = z->CInvert();
	    z->x = z->x + param[1];
	    z->y = z->y + param[2];
	    break;


	case 59:					// More Newton Msets, 5th order Newton Mset
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*z = *q * 2.0;
	    *z = z->CInvert();
	    z->x = param[1] - z->x;
	    z->y = param[2] - z->y;
	    break;

	case 60:					// More Newton Msets, 18th order Newton Mset flower
	    if (!juliaflag)
		*z = -*q * (5814.0 / 4896.0);
	    *z = z->CInvert();
	    break;

	case 67:					// Newton Variations II, z=z-(((z^3)-z)/((3*z*z-1)))+c
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*z = 1;
	    z->x += param[0];
	    z->y += param[1];
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
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    break;

	case 79:					// Polynomials, CBAP  F(z) = Z^3 - 3*(A^2)*Z + B(MOD 2)
	    {
	    Complex ca, t2, t3;

	    t = *q;
	    t3 = t * 3;
	    t2 = t * t;
	    ca = (t2 + 1) / t3;
	    //cb=2*ca*ca*ca+(t2-cmplx(2,0))/t3;
	    cb = ca.CCube() * 2 + (t2 - 2) / t3;
	    caa3 = ca.CSqr() * 3;
	    if (!juliaflag)
		*z = -ca;
	    }    
	    break;

	case 80:					// Quartets, z=.5; z=z*z-z2*z2+c; z2=z
	    z2 = 0.5;
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    break;

	case 81:					// Quartets, z2=2; z=z*z*z*z+z2+c; z2=z1
	case 82:					// Quartets, z2=z; z=z*z*z*z+5*z2*c; z2=z1
	    if (!juliaflag)
		*z = *q;
	    z2 = *z;
	    z->x += param[0];
	    z->y += param[1];
	    break;

	case 83:					// Quartets, t=0; z1=z; z=z*z*z-t*t*t+c; z=z1
	    z2 = 0;
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    break;

	case 84:					// Quartets, t=0; z1=z; z=z*z*z*z-t*t*t*t+c; t=z1
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    z2.x = param[1];	    // t = 0;
	    z2.y = param[2];
	    if (!juliaflag)
		*z = 0;
	    break;

	case 85:					// Quartets, z2=z; z=(z^4)+c; c=z2
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		{
		z->x = q->x + param[1];
		z->y = q->y + param[2];
		}
	    break;

	case 86:					// Newton Variations, z=z-((z^4)-z)/(4*(z^3)-1); z=z*z*c
	    if (!juliaflag)
		*z = q->CInvert();
	    z->x += param[0];
	    z->y += param[1];
	    break;

	case 91:					// Polynomials, z=z*z*z-caa3*z+cb; [CCAP]
	    {
	    Complex ca, t2, t3;

	    t = *q;
	    ca = t;
	    cb = t.CCube() * 2 + t;
	    caa3 = ca * ca * 3;
	    if (!juliaflag)
		*z = -ca;
	    }
	break;

	case 92:					// Polynomials, z=z*z*z-caa3*z+cb; [CFAP]
	    {
	    Complex ca, t2, t3;

	    t = *q;
	    ca = t;
	    cb = (t.CCube() - t) * 2;
	    caa3 = ca.CSqr() * 3;
	    //    ca2 = ca+ca;
	    if (!juliaflag)
		*z = -ca;
	    }
	break;

	case 93:					// Quartets, z1=z; z=z*z*z*z-z2+c; z2 = z1
	    if (!juliaflag)
		*z = *q;
	    z2 = *z;
	    z->x += param[1];
	    z->y += param[2];
	    break;

	case 95:					// Polynomials, z=z*z*z*z/t1+c
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    t = *q + 1;
	    z2 = t.CSin();
	    break;

	case 96:					// Quartets, z1=z; z=z*z*z*z+z2/2+c;; z2=z1
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    z2 = *q;
	    if (!juliaflag)
		{
		z->x = param[1];
		z->y = param[2];
		}
	    break;

	case 99:					// Quartets, z1=z; z2=c+z2/z-z; z=z*c+z2/c
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    z2 = z->CSin();	//z2 = c-c.csin();
	    z2 = *z - z2;
	    break;

	case 103:					// Ramon, z = ((z*z*z+3*c2*z+c1)/(3*z*z+3*c2*z+c1+1))^2; [magneto 2]
	    if (!juliaflag)
		{
		z->x = param[0];
		z->y = param[1];
		}
	    c2 = *q - 1;
	    c1 = c2 * (*q - 2);
	    break;

	case 104:					// Ramon, z = ((z*z+c)/(2*z))^2 (Teddy)
	    if (!juliaflag)
		*z = 1;
	    break;

	case 120:					// Flarium 07, Polynomial: z = (z*z+c)^(cn+c)
	case 132:					// Flarium 27, Polynomial: z=z*z*(cn+z)/(cn+z+c)+c
	    if (!juliaflag)
		*z = *q;
	    z2.x = param[0];
	    z2.y = param[1];
	    break;

	case 123:					// Flarium 10, Sharon Webb: z = z*z*z*z+((c/2)^2)+c; [Sharon08]
	    t = *q / 2;
	    z2 = t.CSqr();
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    break;

	case 124:					// Flarium 11, Sharon Webb: z = z*z*z*z+(z^(z+cn))+c
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    z2 = *z ^ (*z + 2);
	    break;

	case 126:					// Flarium 13, Sharon Webb: z=(z*z*z*z-z2*z2*z2*z2+c)^2
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    z2 = 0;
	    break;

	case 127:					// Flarium 15, Newton Variations: z = z - (z*z*z*z-1)/(4*z*z*z) + c; [4th Order Nova]
	case 128:					// Flarium 16, Newton Variations: z = z - (z*z*z*z*z-1)/(5*z*z*z*z) + c; [5th Order Nova]
	    z->x = 1.0 + param[0];
	    z->y = 0.0 + param[1];
	    break;

	case 129:					// Flarium 20, Phoenix: z = z*z*z*z+c.imag()+c.real()*z2; [4th Order Phoenix]
	    z->x = q->x + param[0];
	    z->y = q->y + param[1];
	    t.x = 0;
	    t.y = 1;
	    z2 = 0;
	    if (!juliaflag)
		*q *= t;
	    else
		{
		*q *= t;
		*z *= t;
		}
	    break;

	case 134:					// Flarium 29, Derbyshire / Newton: c = (z*z*z-1)/(3*z*z); z -= (z*z*z-1)/(3*z*z)*c
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    *q = (*z * *z * *z - 1) / (3 * *z * *z);
	    break;

	case 136:					// Flarium 31, Polynomial: z = z*z*z-aa3*z+b
	    t = (q->CCube() - 1) / (3 * q->CSqr());
	    a = t;
	    b = 2 * t.CCube() - 2 * t;
	    caa3 = a.CSqr() * 3;
	    if (!juliaflag)
		*z = -a;
	    z2.x = 42;
	    z2.y = 42;
	    z->x += param[0];
	    z->y += param[1];
	    return 0;
	    break;

	case 137:					// Flarium 32, Derbyshire / Newton: 3rd Order Nova in a M-Set (Try single pass)
	case 157:					// Flarium 57, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-z)+c; [5th Order Nova Variation]
	case 158:					// Flarium 58, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]
	case 159:					// Flarium 59, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-1)+c; [3rd Order Nova Variation]
	case 160:					// Flarium 60, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-1)+c; [5th Order Nova Variation]
	    if (!juliaflag)
		*z = 1;
	    z->x += param[0];
	    z->y += param[1];
	    break;

	case 141:					// Flarium 37, Polynomial: z=(z*z*z-1)/(3*z*z); z = c*(z.csin() + z.ccos())
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    *z = (*z * *z * *z - 1) / (3 * *z * *z);
	    break;

	case 146:					// Flarium 44, Polynomial: z=(z*z*z*z-z)/(4*z*z*z); z=c*z.csin()
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    *z = (*z * *z * *z * *z - *z) / (4 * *z * *z * *z);
	    break;

	case 147:					// Flarium 45, Polynomial: z=(z*z*z*z-1)/(4*z*z*z); z=c*z.csin()
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    *z = (*z * *z * *z * *z - 1) / (4 * *z * *z * *z);
	    break;

	case 149:					// Flarium 49, Polynomial: z=(z*z*z*z-z)/((4*z*z*z)-z); z=c*z.csin()
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    *z = (*z * *z * *z * *z - *z) / ((4 * *z * *z * *z) - *z);
	    break;

	case 152:					// Flarium 52, Sharon Webb: z = z*z*z*z+t+c; [Sharon08 M-Set]
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    t = *q / 2;
	    z2 = t.CSqr();
	    break;

	case 161:					// Flarium 61, Polynomial: z=z*z*z*z+(c+(c/pi))
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    *q = *q + (*q / PI);
	    break;

	case 163:					// Flarium 64, PHOENIX: z1=z; z=z*z*z*z+c.real()*z2/2+c.imag()*z2/2+c; z2=z1
	    if (!juliaflag)
		*z = *q;
	    z->x += param[0];
	    z->y += param[1];
	    z2 = *z;
	    break;

	case 165:					// Flarium 67-69, Newton Variations: z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*z = 1;
	    z->x += param[1];
	    z->y += param[2];
	    break;

	case 172:					// Flarium 112-116, Polynomials: z=z^n*c+z*c; Dragon curve variations
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*z = *q;
	    z->x += param[1];
	    z->y += param[2];
	    break;

	case 173:					// Flarium 117, Newton Variations: z = z-(z*z*z-z*c-1)/(3*z*z+c-1)
	    if (!juliaflag)
		*z = 0;
	    z->x += param[0];
	    z->y += param[1];
	    break;

	case 177:					// Flarium 145, Polynomial: z=z^2+c [Jaenisch method]
	    *degree = (int)param[0];
	    if (*degree < 2)
		*degree = 2;
	    if (!juliaflag)
		*z = *q;
	    z4.x = param[1];
	    z4.y = param[2];
	    z2 = *z;
	    break;
	}
    return 0;
    }

/**************************************************************************
	Run functions for each iteration
**************************************************************************/

int	CPixel::RunTierazonFunctions(int subtype, Complex *z, Complex *q, BYTE *SpecialFlag, long *iteration)
    {
    switch (subtype)
	{
	case 0:						// Mandelbrot
	case 1:						// Mandelbrot (Tierazon version)
	case 101:					// M-Set Method
	case 118:					// N - Set Method
	    return (RunFunctions(MANDELFP, z, q, SpecialFlag, iteration));

	case 2:						// Nova, init: z=1; iterate: z=z-((z*z*z-1)/(3*z*z))+c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - ((*z*z2 - 1) / (z2 * 3)) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 3:						// Ushiki's Phoenix, z=z*z-.5*z+c; z=z*z-.5*z2+c; z2=z; z=z1
	    *z = z->CSqr() - *z * 0.5 + *q;
	    z1 = z->CSqr() - z2 * 0.5 + *q;
	    z2 = *z;
	    *z = z1;
	    d = z->CSumSqr();
	    return (d > rqlim);

	case 4:						// Talis, z=((z*z)/(1+z))+c
	    *z = z->CSqr() / (*z + 1) + *q;
	    d = z->CSumSqr();
	    return (d >= rqlim);

	case 5:						// Newton variation, z=((z*z*z-z-1)/(3*z*z-1)-z)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = ((*z*z2 - *z - 1) / (z2 * 3 - 1) - *z) * *q;  
	    //  z=((z*z2-z-1)/(3*z2-1)-z)*c;  
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 6:						// z=z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)
	    z2 = *z;
	    z1 = z->CSqr();
	    *z = *z - ((*z*z1) + (*q - 1) * *z - *q) / ((z1) * 3 + *q - 1);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 7:						// Newton variation, z = z-((z*z*z)+(c-1)*z-c)/(3*(z*z)+c-1)
	    z2 = *z;
	    *z = (*z - ((z->CCube() + *q * *z - *z - *q) / (z->CSqr() * 3 + *q - 1))) + *q;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 8:						// Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-1)+c
	    z2 = *z;
	    z1 = z->CCube();
	    *z = *z - (*z*z1 - *z) / (z1 * 4 - 1) + *q;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 9:						// Nova variant, init: z=1; iterate: z=z-((z*z*z*z-z)/(4*z*z*z-z)+c
	    z2 = *z;
	    z1 = z->CCube();
	    *z = *z - (*z*z1 - *z) / (z1 * 4 - *z) + *q;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 10:					// z=z*z*z+c
	case 11:					// z=z*z*z*z+c
	case 12:					// z=z*z*z*z*z+c
	case 13:					// z=z*z*z*z*z*z+c
	    *z = z->CPolynomial(*degree) + *q;		// z^deg - function power
	    d = z->CSumSqr();
	    return (d > rqlim);

	case 14:					// z1=z*z+c; z=z*z+c*z2; z2=z
	    z1 = z->CSqr() + *q;
	    *z = z->CSqr() + *q * z2;
	    z2 = z1;
	    d = z->CSumSqr();
	    return (d > rqlim);

	case 15:					// Phoenix II, z1=z; z=z*z + real(c) + imag(c)*z2; z2=z1
	    z1 = *z;
	    *z = z->CSqr() + q->x + z2 * q->y;
	    z2 = z1;
	    d = z->CSumSqr();
	    return (d > rqlim);

	case 16:					// Phoenix III, z1=z; z=z*z*z +   real(c) + imag(c)*z2; z2=z1
	    z1 = *z;
	    *z = z->CCube() + q->x + z2 * q->y;
	    //z = z*z + __real__ c + __imag__ c * z2;
	    z2 = z1;
	    d = z->CSumSqr();
	    return (d > rqlim);

	case 17:					// Phoenix IV,  z1=z; z=z*z*z +.5*real(c) + imag(c)*z2; z2=z1
	    z1 = *z;
	    *z = z->CCube() + .5 * q->x + z2 * q->y;
	    z2 = z1;
	    d = z->CSumSqr();
	    return (d > rqlim);

	case 18:					// Newton/Mandel, z=-z/3; iterate: z=z-(z*z*z+z*z*c-z+c)/(3*z*z+2*c*z-1)
	    z2 = *z;
	    z1 = z->CSqr();
	    *z = *z - (*z*z1 + z1 * *q - *z + *q) / (z1 * 3 + *q * *z * 2 - 1);
	    //z=z1*z1+c;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 19:					// Newton/Mandel, z=-z/3; iterate: z=z-(z*z*z+z*z*c+z+c)/(3*z*z+2*c*z+1)
	    z2 = *z;
	    *z = *z - (z->CCube() + *q * z->CSqr() + *z + *q) / (z->CSqr() * 3 + *q * *z * 2 + 1);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 20:					// Newton/Mandel, z= z/2; iterate: z=z-(z^4-(z^3)*c-z-c)/(4*(z^3)-3*(z^2)*c-1)
	    z2 = *z;
	    z1 = z->CSqr();
	    //    z = z-c;
	    *z = *z - (z1.CSqr() - z1 * *z * *q - *z - *q) / (*z*z1 * 4 - z1 * *q * 3 - 1);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 21:					// Newton/Mandel, z=-1/(3*z); iterate: z=z-((z^3)*c+z^2+z+c)/(3*(z^2)*c+2*z+1)
	    z2 = *z;
	    z1 = z->CSqr();
	    *z = *z - (*z*z1 * *q + z1 + *z + *q) / (z1 * *q * 3 + *z * 2 + 1);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 22:					// Newton/Mandel, z=-.5; iterate: z=z-((z^4)c+(z^3)*c+z+c)/(4*(z^3)*c+3*z*z*c+1)
	    z2 = *z;
	    z1 = z->CSqr();
	    *z = *z - (z1.CSqr() * *q + z1 * *z * *q + *z + *q) / (z1 * *z * *q * 4 + z1 * *q * 3 + 1);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 23:					// Newton/Mandel, z=-.5/z; iterate: z=z-((z^4)*c+(z^3)+z+c)/(4*(z^3)*c+3*z*z+1)
	    z2 = *z;
	    z1 = *z * *z;
	    *z = *z - (z1.CSqr() * *q + z1 * *z + *z + *q) / (z1 * *z * *q * 4 + z1 * 3 + 1);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 24:					// Newton/Mandel, z=-z/3;  iterate: z=z-((z^3)+z*z*c +c)/(3*z*z+2*z*c)
	    z2 = *z;
	    z1 = z->CSqr();
	    *z = *z - (z1 * *z + z1 * *q + *q) / (z1 * 3 + *z * *q * 2);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 25:					// Newton/Mandel, z=-z/2;  iterate: z=z-((z^4)+(z^3)*c+c)/(4*(z^3)+3*z*z*c)
	    z2 = *z;
	    z1 = z->CSqr();
	    *z = *z - (z1.CSqr() + z1 * *z * *q + *q) / (z1 * *z * 4 + *z * *z * *q * 3);
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 26:					// Newton/Mandel, 5th order Newton Mset
	case 27:					// Newton/Mandel, 7th order Newton Mset
	case 28:					// Newton/Mandel, 9th order Newton Mset
	case 29:					// Newton/Mandel, 13th order Newton Mset
	case 30:					// Newton/Mandel, 8th order Newton Mset
	case 53:					// Newton/Mandel, 6th order Newton Mset
	{
	    Complex	fn, f1n, f2n, a, b;

	    z1 = *z;
	    f2n = z->CPolynomial(*degree - 2);		// z^(deg - 2) - second derivative power
	    f1n = f2n * *z;				// z^(deg - 1) - first derivative power
	    fn = f1n * *z;				// z^deg - function power
	    a = f1n * *q + fn + *q;
	    b = f1n * ((double)*degree) + *q * (f2n*((double)(*degree - 1)));
	    *z = *z - a / b;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);
	    }

	case 31:					// Newton/Mandel, Newton Diamond
	case 32:					// Newton/Mandel, Newton Pentagon
	case 33:					// Newton/Mandel, Newton Hexagon
	case 34:					// Newton/Mandel, Newton Octagon
	    {
	    Complex	fn, f1n, f2n, a, b;

	    z1 = *z;
	    f2n = z->CPolynomial(*degree - 2);		// z^(deg - 2) - second derivative power
	    f1n = f2n * *z;				// z^(deg - 1) - first derivative power
	    fn = f1n * *z;				// z^deg - function power
	    a = f1n * *q + fn + *z + *q;
	    b = f1n * ((double)*degree) + 1.0 + *q * (f2n*((double)(*degree - 1)));
	    *z = *z - a / b;
	    zd = *z - z1;
	    d = zd.CSumSqr();
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
	    Complex	 fn, f1n, f2n, a, b;

	    z1 = *z;
	    f2n = z->CPolynomial(*degree - 2);		// z^(deg - 2) - second derivative power
	    f1n = f2n * *z;				// z^(deg - 1) - first derivative power
	    fn = f1n * *z;				// z^deg - function power
	    a = f1n * *q + fn - *z;
	    b = f1n * ((double)*degree) + *q * (f2n*((double)(*degree - 1))) - 1.0;
	    *z = *z - a / b;
	    zd = *z - z1;
	    d = zd.CSumSqr();

	    return (d < MINSIZE);
	    }

	case 36:					// Newton Variations, z=z-(z*z*z*z*c-z+c)/(4*z*z*z*c)
	    z1 = *z;
	    z3 = z->CCube();
	    *z = *z - (z3* *z * *q - *z + *q) / (z3 * *q * 4);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 37:					// Newton Variations, z=z-(z*z*z-z+c)/(3*z*z)
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 - *z + *q) / (z2 * 3);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 38:					// Newton Variations, z=z-(z*z*z*c-z*c-1)/(3*z*z*c)
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 * *q - *z * *q - 1) / (z2 * *q * 3);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 39:					// Newton Variations, z=z-(z*z*z*c-z*z*c-1)/(3*z*z*c)
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 * *q - *z * *z * *q - 1) / (z2 * *q * 3);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 40:					// Newton Variations, z=z-(z*z*z*c-1)/(3*z*z*c)
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 * *q - 1) / (z2 * *q * 3);
	    *z = z->CSqr() * *q + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 41:					// Newton Variations, z=z-(z*z*z*c-z-1)/(3*z*z*c-z)
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 * *q - *z - 1) / (z2 * *q * 3 - *z);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 42:					// Newton Variations, z=z-(z*z*z*c-z*c-1)/(3*z*z*c-z)
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 * *q - *z * *q - 1) / (z2 * *q * 3 - *z);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 43:					// Newton Variations, z=z-(z*z*z*c-z*z-1)/(3*z*z*c-3*z*z-3*z)
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 * *q - z2 - 1) / (z2 * *q * 3 - z2 * 3 - *z * 3);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 44:					// Newton Variations, z=z-(z*z*z*c-z*z*c-1)/(3*z*z*c-z*c-z)
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 * *q - z2 * *q - 1) / (z2 * *q * 3 - *z * *q - *z);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 45:					// Newton Variations, z=((z-(z*z*z-z)/(3*z*z-1))^2)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = (*z - (*z*z2 - *z) / (z2 * 3 - 1));
	    *z = *z * *z * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 46:					// Trig functions, z = ccos(z*c)*c
	    z1 = *z;
	    *z = *z * *q;
	    *z = z->CCos();
	    *z = *z * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 47:					// Trig functions, z = ((((z*z).csin())*z/2)^2)+c, Sharon 14
	    z1 = *z;
	    *z = z->CSqr();
	    *z = z->CSin();
	    *z = z->CSqr() / 2.0;
	    *z = z->CSqr() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 48:					// Trig functions, z = ((z*z).csin()).clog()+c
	    z1 = *z;
	    *z = z->CSqr();
	    *z = z->CSin();
	    *z = z->CLog();
	    *z = *z + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 49:					// Trig functions, z = z*z*sin(z.real()) + c*z*cos(z.imag()) + c
		//  z = z*z*sin(z.real()) + c*z*cos(z.imag()) + c;
	    z1 = *z;
	    *z = z->CSqr()*sin(z->x) + *q * *z*cos(z->y) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 50:					// Trig functions, z = csin(z)*ccos(c)
	    z1 = *z;
	    *z = z->CSin();
	    *z = *z * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 51:					// Trig functions, z = csin(z*z*z*z)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    a = z2.CSqr();
	    *z = *q * a.CSin();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 55:					// More Newton Msets, 4th order Newton's apple
	    {
	    double	s, t, u;
	    Complex	fn, f1n, f2n, f3n, c, e, f;

	    z1 = *z;
	    f3n = z->CPolynomial(*degree - 4);		// z^(deg - 4) - third derivative power
	    f2n = f3n * *z;				// z^(deg - 2) - second derivative power
	    f1n = f2n * *z;				// z^(deg - 1) - first derivative power
	    fn = f1n * *z;				// z^deg - function power
	    c = *q * fn*((double)*degree);
	    e = f1n * ((double)(*degree - 1));
	    a = c - e - f2n * ((double)(*degree - 2));	// top row
	    s = (double)*degree * (double)(*degree - 1);
	    t = (double)(*degree - 1) * (double)(*degree - 2);
	    u = (double)(*degree - 2) * (double)(*degree - 3);
	    c = *q * f1n*s;
	    e = f2n * t;
	    b = c - e - f3n * u;			// bottom row
	    *z = *z - a / b;
	    zd = *z - z1;
	    d = zd.CSumSqr();
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
	    f2n = f3n * *z;				// z^(deg - 2) - second derivative power
	    f1n = f2n * *z;				// z^(deg - 1) - first derivative power
	    fn = f1n * *z;				// z^deg - function power
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
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);
	    }
*/

	case 59:					// More Newton Msets, 5th order Newton Mset
	    {
	    double	s, t;
	    Complex	fn, f1n, f2n, c;

	    z1 = *z;
	    f2n = z->CPolynomial(*degree - 3);		// z^(deg - 3) - second derivative power
	    f1n = f2n * *z;				// z^(deg - 2) - first derivative power
	    fn = f1n * *z;				// z^(deg - 1) - function power
	    c = *q * fn*((double)*degree);
	    a = f1n * ((double)(*degree - 1)) + c - 1.0;
	    t = (double)*degree * (double)(*degree - 1);
	    s = (double)(*degree - 1) * (double)(*degree - 2);
	    c = *q * f1n*t;
	    b = f2n * s + c;
	    *z = *z - a / b;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);
	    }

	case 60:					// More Newton Msets, 18th order Newton Mset flower
	    {
	    Complex	z8, z16;

	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    z8 = z4.CSqr();
	    z16 = z8.CSqr();
	    *z = *z - (z16*z2 * *q * 19 + z16 * *z * 18 - *z) / (z16 * *z * *q * 342 + z16 * 306 - 1);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);
	    }

	case 61:					// Polynomials, z=z*z+c; real=imag; imag=real
	    {
	    double t;

	    z1 = *z;
	    *z = z->CSqr() + *q;
	    t = z->x;
	    z->x = z->y;
	    z->y = t;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);
	    }

	case 62:					// Newton Variations II, z=(((z^3)-z-1)/((z^3)-1)-z)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = ((*z*z2 - *z - 1) / (z2 * *z - 1) - *z) * *q;   // 3rd order M-Set
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 63:					// Newton Variations II, z=(((z^4)-z*z-1)/(4*z*z-1)-z)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = ((z2.CSqr() - z2 - 1) / (z2 * 4 - 1) - *z) * *q;  // 
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 64:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-fabs(z)-1))*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = (*z - (*z*z2 - 1) / (z2 * 3 - z->CFabs() - 1)) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 65:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-z-1))*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = (*z - (*z*z2 - 1) / (z2 * 3 - *z - 1)) * *q;  // Almost makes a connection
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 66:					// Newton Variations II, z=(((z^4)-z-1)/(4*(z^3)-z-1)-z)*c
	    z1 = *z;
	    z3 = z->CCube();
	    *z = ((*z*z3 - *z - 1) / (z3 * 4 - *z - 1) - *z) * *q;  // Some strange curls
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 67:					// Newton Variations II, z=z-(((z^3)-z)/((3*z*z-1)))+c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - ((*z*z2 - *z) / ((z2 * 3 - 1))) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 68:					// Newton Variations II, z=(z-((z^3)-1)/(4*z*z-z-1))*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = (*z - (*z*z2 - 1) / (z2 * 4 - *z - 1)) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 69:					// Newton Variations II, z=(z-((z^3)-1)/(3*z*z-z))*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = (*z - (*z*z2 - 1) / (z2 * 3 - *z)) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 70:					// Newton Variations II, z=(z-((z^4)-1)/(4*(z^3)-z))*c
	    z1 = *z;
	    z3 = z->CCube();
	    *z = (*z - (*z*z3 - 1) / (z3 * 4 - *z)) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 71:					// Newton Variations II, z=(z-((z^4)-1)/(3*(z^3)-z))*c
	    z1 = *z;
	    z3 = z->CCube();
	    *z = (*z - (*z*z3 - 1) / (z3 * 3 - *z)) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 72:					// Newton Variations II, z=(z-((z^4)-z-1)/(3*(z^3)-z))*c
	    z1 = *z;
	    z3 = z->CCube();
	    *z = (*z - (*z*z3 - *z - 1) / (z3 * 3 - *z)) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 73:					// Newton Variations II, z=c*(z-((z^3)-z)/(3*z*z-1))
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = (*z - (*z*z2 - *z) / (z2 * 3 - 1));  // 105
	    *z = *z * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 74:					// Newton Variations II, z=((1-z-(z^4))/(z-(4*(z^3)))-z)*c
	    z1 = *z;
	    z3 = z->CCube();
	    a = 1;
	    *z = ((a - *z - z3 * *z) / (*z - (z3 * 4)) - *z) * *q;  // sort of a good one
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 75:					// Newton Variations II, z=((z-(z^3)*z)/(z-(3*(z^3)))-z)*c
	    z1 = *z;
	    z3 = z->CCube();
	    *z = ((*z - z3 * *z) / (*z - (z3 * 3)) - *z) * *q;  // 107
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 76:					// Newton Variations II, z=((z-(z^3))/(z-(3*(z^3)))-z)*c
	    z1 = *z;
	    z3 = z->CCube();
	    *z = ((*z - z3) / (*z - (z3 * 3)) - *z) * *q;  // 108
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 77:					// Newton Variations II, z=(((z^3)-1)/(2*z*z-1)-z)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = ((z2 * *z - 1) / (z2 * 2 - 1) - *z) * *q;  // 109
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 78:					// Newton Variations II, z=(((z^3)-z-1)/(3*(z^3)-1)-z)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = ((*z*z2 - *z - 1) / (*z*z2 * 3 - 1) - *z) * *q; // 2nd Order M-Set 110
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 79:					// Polynomials, CBAP  F(z) = Z^3 - 3*(A^2)*Z + B(MOD 2)
	    z1 = *z;
	    *z = z->CCube() - caa3 * *z + cb;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 80:					// Quartets, z=.5; z=z*z-z2*z2+c; z2=z
	    z1 = *z;
	    *z = z->CSqr() - z2.CSqr() + *q;
	    z2 = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 81:					// Quartets, z2=2; z=z*z*z*z+z2+c; z2=z1
	    z1 = *z;
	    zt = z->CSqr();
	    *z = zt.CSqr() + z2 + *q;
	    z2 = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 82:					// Quartets, z2=z; z=z*z*z*z+5*z2*c; z2=z1
	    z1 = *z;
	    zt = z->CSqr();
	    *z = zt.CSqr() + z2 * *q * 5;
	    z2 = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 83:					// Quartets, t=0; z1=z; z=z*z*z-t*t*t+c; z=z1
	    z1 = *z;
	    *z = z->CCube() - z2.CCube() + *q;
	    z2 = z1;
	    //    z=z*z*z-t*t*t+c;
	    //    t=z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 84:					// Quartets, t=0; z1=z; z=z*z*z*z-t*t*t*t+c; t=z1
	    z1 = *z;
	    a = z->CPolynomial(*degree) + *q;
	    b = z2.CPolynomial(*degree);	// global z2 used for t
	    *z = a - b;
	    z2 = z1;				// t = z1
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 85:					// Quartets, z2=z; z=(z^4)+c; c=z2
	    z1 = *z;
	    z2 = *z;
	    //  z = z*z*z*z+q;
	    *z = z->CPolynomial(*degree) + *q;
	    *q = z2;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 86:					// Newton Variations, z=z-((z^4)-z)/(4*(z^3)-1); z=z*z*c
	    z1 = *z;
	    z3 = z->CCube();
	    *z = *z - (*z*z3 - *z) / (z3 * 4 - 1);
	    *z = z->CSqr() * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 87:					// Newton Variations, z=z-((z^3)-1)/(3*z*z); z=(z^3)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 - 1) / (z2 * 3);
	    *z = z->CCube() * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 88:					// Newton Variations, z=z-((z^4)-1)/(4*(z^3)); z=(z^4)*c
	    z1 = *z;
	    z3 = z->CCube();
	    *z = *z - (*z*z3 - 1) / (z3 * 4);
	    *z = z->CCube() * *z * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 89:					// Newton Variations, z=z-((z^5)-1)/(5*(z^4)); z=(z^5)*c
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = *z - (*z*z4 - 1) / (z4 * 5);
	    *z = z->CCube()*z->CSqr() * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 90:					// Polynomials, z=z+c [quad]
	    z1 = *z;
	    *z = *z + *q;
	    z->x += (z->x >= 0.0) ? -2.0 : 2.0;
	    z->y += (z->y >= 0.0) ? -2.0 : 2.0;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > rqlim);

	case 91:					// Polynomials, z=z*z*z-caa3*z+cb; [CCAP]
	    z1 = *z;
	    a = caa3 * *z - cb;
	    *z = z->CCube() - a;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 92:					// Polynomials, z=z*z*z-caa3*z+cb; [CFAP]
	    z1 = *z;
	    a = caa3 * *z;
	    *z = z->CCube() - a + cb;
	    //    z = z*z*z-caa3*z+cb;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 93:					// Quartets, z1=z; z=z*z*z*z-z2+c; z2 = z1
	    z1 = *z;
	    zt = z->CSqr();
	    *z = zt.CSqr() - z2 + *q;
	    z2 = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 94:					// Polynomials, z=c*z*(2 - z*z)
	    z1 = *z;
	    a = -*z * *z + 2;
	    b = a * *z;
	    *z = b * *q;
	    //    z = c*z*(2 - z*z);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 95:					// Polynomials, z=z*z*z*z/t1+c
	    z1 = *z;
	    zt = z->CSqr();
	    a = zt.CSqr();
	    *z = a / z2 + *q;
	    //    z = z*z*z*z/t1+c;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 96:					// Quartets, z1=z; z=z*z*z*z+z2/2+c;; z2=z1
	    z1 = *z;
	    //    z=z*z*z*z+z2/2+q;
	    *z = z->CPolynomial(*degree) + z2 / 2.0 + *q;
	    z2 = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 97:					// Polynomials, z = z*z+c*(1+sqrt(z))
	    z1 = *z;
	    z2 = z->CSumSqr();
	    *z = *z * *z + *q * (z2 + 1);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 98:					// Quartets, z1=z; z=z*z*z*z/(1+z)+c;; c=z1
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = z2.CSqr() / (*z + 1) + *q;
	    *q = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 99:					// Quartets, z1=z; z2=c+z2/z-z; z=z*c+z2/c
	    z1 = *z;
	    z2 = z2 / *z + *q - *z;
	    a = z2 / *q;
	    *z = *z * *q + a;
	    //    z2=c+z2/z-z;
	    //    z=z*c+z2/c;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 100:					// Polynomials, real(z) += sin(real(z)); z=z*z+c
	    z1 = *z;
	    //__imag__ z += sin(__imag__ z);
	    z->x += sin(z->x);
	    *z = *z * *z + *q;
	    //temp = __real__ z;
	    //__imag__ z = __real__ z;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 102:					// Ramon, z = ((z*z+c-1)/(2*z+c-2))^2; [magneto 1]
	    z1 = *z;
	    //z = ((z*z+c-1)/(z+z+c-2))*((z*z+c-1)/(z+z+c-2));
	    a = z->CSqr() + *q - 1;
	    b = *z * 2 + *q - 2;
	    *z = a / b;
	    //    z = (z*z+q-1)/(z*2+q-2);
	    *z = z->CSqr();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    //    return (d < MINSIZE);
	    return (d < MINSIZE || d > MAXSIZE);

	case 103:					// Ramon, z = ((z*z*z+3*c2*z+c1)/(3*z*z+3*c2*z+c1+1))^2; [magneto 2]
	    {
	    Complex c2z3;

	    z1 = *z;
	    // z = ((z*z*z+(3*(c-1)*z)+((c-1)*(c-2)))/(3*z*z+(3*(c-2)*z)+((c-1)*(c-2))+1))*((z*z*z+(3*(c-1)*z)+((c-1)*(c-2)))/(3*z*z+(3*(c-2)*z)+((c-1)*(c-2))+1));
	    z2 = z->CSqr();
	    //    z = (z*z2+3*c2*z+c1)/(3*z2+3*c2*z+c1+1);	
	    c2z3 = *z * c2*3.0;
	    a = *z * z2 + c2z3 + c1;
	    b = z2 * 3 + c2z3 + c1 + 1;
	    *z = a / b;
	    *z = *z * *z;

	    zd = *z - z1;
	    d = zd.CSumSqr();
	    //    return (d < MINSIZE);
	    return (d < MINSIZE || d > MAXSIZE);
	    }

	case 104:					// Ramon, z = ((z*z+c)/(2*z))^2
	    z1 = *z;
	    *z = (*z * *z + *q) / (*z + *z);
	    *z = z->CSqr();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 105:					// More Fractals, 3rd order Newton/ Mset [figure 8]
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - ((*z*z2 + z2 * *q - *z) / (z2 * 3 + *z * *q * 2 - 1));
	    zd = z1 - *z;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 107:					// More Fractals, z1 = z; z = sin(z) - c; c = 1/(z*50); [quartet]
	    z1 = *z;
	    *z = z->CSin();
	    *z = *z - *q;
	    a = *z * 50;
	    *q = a.CInvert();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 108:					// More Fractals, z=(z*c)/(1+z)+c; [Talis II]
	    *z = (*z * *q) / (*z + 1) + *q;
	    d = z->CSumSqr();
	    return (d > rqlim);

	case 109:					// More Fractals, z=(c+z*z*c)/(1-z*z*c)
	    z2 = z->CSqr() * *q;
	    //    z2 = z*z*q;
	    *z = (*q + z2) / (-z2 + 1);
	    d = z->CSumSqr();
	    return (d > rqlim);

	case 110:					// More Fractals, z = (c+(z^6))/(1+(z^2))
	    z1 = *z;
	    *z = z->CSqr();
	    *z = (*q + z->CCube()) / (*z + 1);
	    zd = z1 - *z;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 111:					// Ramon, z = (z^2)/(1+c*(z^2))
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = z2 / (*q*z2 + 1);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 112:					// Ramon, z = c*cos(z)
	    z1 = *z;
	    *z = *q * z->CCos();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 113:					// Ramon, z = c*sin(z)
	    z1 = *z;
	    *z = *q * z->CSin();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 114:					// Ramon, z = c*exp(z)
	    z1 = *z;
	    *z = *q * z->CExp();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > MAXSIZE);

	case 115:					// Ramon, z=z^2+c; c=c/2+z [spider]
	    z1 = *z;
	    *z = z->CSqr() + *q;
	    *q = *q / 2 + *z;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE || d > rqlim);

	case 116:					// Ramon, z = (c/sin(z))^2
	    z1 = *z;
	    *z = *q / z->CSin();
	    *z = z->CSqr();
	    zd = *z - z1;
	    return (zd.CSumSqr() < MINSIZE || z->CSumSqr() > rqlim);

	case 117:					// Ramon, z = (c/cos(z))^2
	    z1 = *z;
	    *z = *q / z->CCos();
	    *z = z->CSqr();
	    zd = *z - z1;
	    return (zd.CSumSqr() < MINSIZE || z->CSumSqr() > rqlim);

	case 119:					// Flarium 06, Newton Variations: z=z-(z*z*z-1)/3*z*z; z=z*c
	    z2 = *z;
	    z1 = z->CSqr();
	    *z = *z - (*z*z1 - 1) / 3 * z1;
	    *z = *z * *q;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 120:					// Flarium 07, Polynomial: z = (z*z+c)^(cn+c)
	    z1 = *z;
	    *z = (*z * *z + *q) ^ (z2 + *q);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 121:					// Flarium 08, Sharon Webb: z = z*z*z*z+1/c; [Sharon Star]
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = z4 + q->CInvert();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 122:					// Flarium 09, Sharon Webb: z = (z*z/2+c)*(z*z/2+c); [Sharon's Space Probe]
	    z1 = *z;
	    z2 = z->CSqr() / 2 + *q;
	    *z = z2.CSqr();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 123:					// Flarium 10, Sharon Webb: z = z*z*z*z+((c/2)^2)+c; [Sharon08]
	    {
	    Complex z2a;

	    z1 = *z;
	    z2a = z->CSqr();
	    z4 = z2a.CSqr();
	    *z = z4 + z2 + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 124:					// Flarium 11, Sharon Webb: z = z*z*z*z+(z^(z+cn))+c
	    {
	    Complex z2a;

	    z1 = *z;
	    z2a = z->CSqr();
	    z4 = z2a.CSqr();
	    *z = z4 + z2 + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 125:					// Flarium 12, Sharon Webb: z = (z+z*z/2)+c
	    z1 = *z;
	    *z = (*z + z->CSqr() / 2) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 126:					// Flarium 13, Sharon Webb: z=(z*z*z*z-z2*z2*z2*z2+c)^2
	    {
	    Complex	z2a, quadz2;

	    z1 = *z;
	    z2a = z->CSqr();
	    z4 = z2a.CSqr();
	    z2a = z2.CSqr();
	    quadz2 = z2a.CSqr();
	    //    z = (z*z*z*z-z2*z2*z2*z2+q)*(z*z*z*z-z2*z2*z2*z2+q);
	    t = (z4 - quadz2 + *q);
	    *z = t.CSqr();
	    z2 = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 127:					// Flarium 15, Newton Variations: z = z - (z*z*z*z-1)/(4*z*z*z) + c; [4th Order Nova]
	    z1 = *z;
	    z2 = z->CCube();
	    *z = *z - (z2 * *z - 1) / (4 * z2) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 128:					// Flarium 16, Newton Variations: z = z - (z*z*z*z*z-1)/(5*z*z*z*z) + c; [5th Order Nova]
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = *z - (z4* * z - 1) / (5 * z4) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 129:					// Flarium 20, Phoenix: z = z*z*z*z+c.imag()+c.real()*z2; [4th Order Phoenix]
	    {
	    Complex z2a;
	    
	    z1 = *z;
	    z2a = z->CSqr();
	    z4 = z2a.CSqr();
	    *z = z4 + q->y + q->x * z2;
	    z2 = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 130:					// Flarium 21, Sharon Webb: z=(1/z*z-c)*(z*z*z*z+c); [Sharon03]
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = (z->CInvert() * *z - *q)*(z4 + *q);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 131:					// Flarium 25, Newton Variations: z=z-(z*z*z*z-z)/(4*z*z*z-z); z=z*c
	    z2 = *z;
	    z1 = z->CCube();
	    *z = *z - (*z*z1 - *z) / (4 * z1 - *z);
	    *z = *z * *q;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 132:					// Flarium 27, Polynomial: z=z*z*(cn+z)/(cn+z+c)+c
	    z1 = *z;
	    *z = z->CSqr()*(z2 + *z) / (z2 + *z + *q) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 133:					// Flarium 28, Derbyshire / Newton: z=z - (z*z*z-1)/(3*z*z)+c; [Nova-Mandelbrot-MultiFract]
	    {
	    int		i;
	
	    z1 = *z;
	    if (*iteration >= threshold)
		{
		// Inside
		*q = *z;
		*z += 1;
		i = 0;
		d = 1;
		while (d > MINSIZE && i++ < threshold)
		    {
		    z1 = *z;
		    *z = *z - (z->CCube() - 1) / (3 * z->CSqr()) + *q;
		    zd = *z - z1;
		    d = zd.CSumSqr();
		    }
		*iteration = i;
		return TRUE;
		}
	    *z = z->CSqr() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 134:					// Flarium 29, Derbyshire / Newton: c = (z*z*z-1)/(3*z*z); z -= (z*z*z-1)/(3*z*z)*c
	    z1 = *z;
	    *z -= (z->CCube() - 1) / (3 * z->CSqr()) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 135:					// Flarium 30, Derbyshire / Newton: z -= (z*z*z-1)/(3*z*z)*c
	    z1 = *z;
	    *z -= (z->CCube() - 1) / (3 * z->CSqr()) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 136:					// Flarium 31, Polynomial: z = z*z*z-aa3*z+b
	    z2 = *z;
	    *z = z->CCube() - caa3 * *z + b;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 137:					// Flarium 32, Derbyshire / Newton: 3rd Order Nova in a M-Set (Try single pass)
	    z2 = *z;
	    z1 = z->CSqr();
	    *z = *z - (*z*z1 - 1) / (3 * z1) + *q;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 138:					// Flarium 34, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14]
	    z2 = *z;
	    t = z->CSqr();
	    z1 = *z * t.CSin() / 2;
	    *z = z1.CSqr() + *q;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 139:					// Flarium 35, Polynomial: z=c*(z.csin())
	    z2 = *z;
	    *z = *q * z->CSin();
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 140:					// Flarium 36, Polynomial: z=(c*z).csin()
	    z2 = *z;
	    t = *z * *q;
	    *z = t.CSin();
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 141:					// Flarium 37, Polynomial: z=(z*z*z-1)/(3*z*z); z = c*(z.csin() + z.ccos())
	    z2 = *z;
	    *z = *q * (z->CSin() + z->CCos());
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 142:					// Flarium 38, Polynomial: z = z*z+c; z.set_real(z.real()*z.real()); [Variation real]
	    z2 = *z;
	    *z = *z * *z + *q;
	    z->x = z->x*z->x;
	    zd = *z - z2;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 143:					// Flarium 40, Polynomial: z1 = z*z*z*z; z = c*z1/4*z1 + z
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = *q * z4 / 4 * z4 + *z;
	    zd = *z - z4;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 144:					// Flarium 41, Polynomial: z = c*(z*z*z*z).csin()
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = *q * z4.CSin();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 145:					// Flarium 43, Polynomial: z = z*z*z*z + (z*c).csin() + c
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    t = *z * *q;
	    *z = z4 + t.CSin() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 146:					// Flarium 44, Polynomial: z=(z*z*z*z-z)/(4*z*z*z); z=c*z.csin()
	    z1 = *z;
	    *z = *q * z->CSin();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 147:					// Flarium 45, Polynomial: z=(z*z*z*z-1)/(4*z*z*z); z=c*z.csin()
	    z1 = *z;
	    *z = *q * z->CSin();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 148:					// Flarium 46, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14 N-Method]
	    z1 = *z;
	    t = z->CSqr();
	    z2 = t.CSin() * *z / 2;
	    *z = z2.CSqr() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 149:					// Flarium 49, Polynomial: z=(z*z*z*z-z)/((4*z*z*z)-z); z=c*z.csin()
	    z1 = *z;
	    *z = *q * z->CSin();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 150:					// Flarium 50, Sharon Webb: z = z*z*z*z+1/c; [Sharon's Star M-Set]
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = z4 + q->CInvert();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 151:					// Flarium 51, Sharon Webb: z = (z*z/2+c)*(z*z/2+c); [Space Probe M-Set]
	    z1 = *z;
	    z2 = z->CSqr() / 2 + *q;
	    *z = z2.CSqr();
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 152:					// Flarium 52, Sharon Webb: z = z*z*z*z+t+c; [Sharon08 M-Set]
	    {
	    Complex	z2a;

	    z1 = *z;
	    z2a = z->CSqr();
	    z4 = z2a.CSqr();
	    *z = z4 + z2 + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 153:					// Flarium 53, Sharon Webb: z = (z+z*z/2)+c; [M-Set]
	    z1 = *z;
	    *z = (*z + z->CSqr() / 2) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 154:					// Flarium 54, Polynomial: z = z*z*c+c; [M-Set]
	    z1 = *z;
	    *z = z->CSqr() * *q + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 155:					// Flarium 55, Polynomial: z = (z*z).csin()*z*z*c+c; [M-Set]
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = z2.CSin() * z->CSqr() * *q + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 156:					// Flarium 56, Polynomial: z = (z*z+c)/(z*z-c); [M-Set]
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = (z2 + *q) / (z2 - *q);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 157:					// Flarium 57, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-z)+c; [5th Order Nova Variation]
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = *z - (*z*z4 - *z) / ((5 * z4) - *z) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 158:					// Flarium 58, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 - *z) / ((3 * z2) - *z) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 159:					// Flarium 59, Derbyshire / Newton: z=z-(z*z1-z)/((3*z1)-1)+c; [3rd Order Nova Variation]
	    z1 = *z;
	    z2 = z->CSqr();
	    *z = *z - (*z*z2 - *z) / ((3 * z2) - 1) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 160:					// Flarium 60, Derbyshire / Newton: z=z-(z*z1-z)/((5*z1)-1)+c; [5th Order Nova Variation]
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = *z - (*z*z4 - *z) / ((5 * z4) - 1) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 161:					// Flarium 61, Polynomial: z=z*z*z*z+(c+(c/pi))
	    z1 = *z;
	    z2 = z->CSqr();
	    z4 = z2.CSqr();
	    *z = z4 + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 162:					// Flarium 62, Polynomial: z=z*z*(1+z)/(1+z+c)+c
	    z1 = *z;
	    *z = z->CSqr()*(1 + *z) / (1 + *z + *q) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 163:					// Flarium 64, PHOENIX: z1=z; z=z*z*z*z+c.real()*z2/2+c.imag()*z2/2+c; z2=z1
	    {
	    Complex	z2a;

	    z1 = *z;
	    z2a = z->CSqr();
	    z4 = z2a.CSqr();
	    *z = z4 + q->x * q->y*z2 / 2 + *q;
	    z2 = z1;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 164:					// Flarium 66, Newton Variations: z = ((z-(((z^2)-1)/(2*z)))^2)*c
	    z1 = *z;
	    *z = *z - (z->CSqr() - 1) / (2 * *z);
	    *z = z->CSqr() * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 165:					// Flarium 67-69, Newton Variations: z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c
	    {
	    Complex	fn, f1n;

	    z1 = *z;
	    f1n = z->CPolynomial(*degree - 1);			// z^(deg - 1) - first derivative power
	    fn = f1n * *z;						// z^(deg)
	    *z = *z - (fn - 1) / (*degree*f1n);
	    *z = z->CSqr() * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 166:					// Flarium 80, Newton Variations: z=((z*z*z*z-1)/(1-(3*z*z*z)))*c
	    z1 = *z;
	    z2 = z->CCube();   // A correct one, finally
	    *z = ((*z*z2 - 1) / (1 - (3 * z2))) * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 167:					// Flarium 90, Sharon Webb: z = (z*(z*z).csin()/2).csin()+c; [Sharon15]
	    {
	    Complex	s;

	    z1 = *z;
	    t = z->CSqr();
	    s = *z * t.CSin() / 2;
	    *z = s.CSin() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 168:					// Flarium 91, Sharon Webb: z = (z*(z+z).ccos()/2); z = z*z+c; [Sharon16]
	    z1 = *z;
	    t = *z + *z;
	    *z = (*z*t.CCos() / 2);
	    *z = z->CSqr() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 169:					// Flarium 92, Sharon Webb: z = (z*(z+z).csin()/2); z = z*z+c; [Sharon17]
	    z1 = *z;
	    t = *z + *z;
	    *z = (*z*t.CSin() / 2);
	    *z = z->CSqr() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 170:					// Flarium 93, Sharon Webb: z = (z*(z+z*z).csin()/2); z = z*z+c; [Sharon18]
	    z1 = *z;
	    t = *z + z->CSqr();
	    *z = (*z*t.CSin() / 2);
	    *z = z->CSqr() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 171:					// Flarium 111, Sharon Webb: z1= z*(z*z).csin()/2; z=z1*z1 + c; [Alden's Ray Method]
	    z1 = *z;
	    t = z->CSqr();
	    //    z2 = z*CSinError(CSqr(z))/2;
	    z2 = *z * t.CSin() / 2;
	    *z = z2.CSqr() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 172:					// Flarium 112-116, Polynomials: z=z^n*c+z*c; Dragon curve variations
	    {
	    Complex	fn;

	    z1 = *z;
	    fn = z->CPolynomial(*degree);
	    *z = fn * *q + *z * *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }

	case 173:					// Flarium 117, Newton Variations: z = z-(z*z*z-z*c-1)/(3*z*z+c-1)
	    z1 = *z;
	    *z = *z - (*z * *z * *z - *z * *q - 1) / (3 * *z * *z + *q - 1);
	    //    z = z-(CCube(z)-z*q-1)/(3*CSqr(z)+q-1);
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d < MINSIZE);

	case 174:					// Flarium 118, Sharon Webb: z = (z*z*z*z*z).csin() + c; [Sharon19]
	    t = z->CCube()*z->CSqr();
	    *z = t.CSin() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 175:					// Flarium 119, Sharon Webb: z = (z+(z*z)/.192).csin() + c; [Sharon's Butterfly]
	    z1 = *z;
	    t = *z + (*z * *z) / .192;
	    *z = t.CSin() + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 176:					// Flarium 125, Sharon Webb: z = z+z*z*z/4 + c; [Sharon21]
	    z1 = *z;
	    *z = *z + z->CCube() / 4 + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);

	case 177:					// Flarium 145, Polynomial: z=z^2+c [Jaenisch method]
	    {
	    int		i, rzflag, jrw;
	    int		nFF = 1;
	    long	NMAX = (threshold * 2) / 3;
	    double	r, rz;
	    long	LocalIteration;
	    //    BOOL	b_MAX;

	    z1 = *z;
	    if (*iteration >= threshold)
		{
		// Inside
		*z = z2;
		i = 0;
		jrw = 0;
		LocalIteration = NMAX - 2;
		while (i++ < LocalIteration)
		    {
		    //z=z*z + c;
	//	    z=CCube(z) + q;
	//	    z = CPolynomial(z, degree) + q;
		    *z = *z ^ z4 + *q;

		    if (z->x < 0)
			z->x = -(log10(fabs(z->x)));
		    else if (z->x > 0)
			z->x = log10(fabs(z->x));
		    else z->x = 0;

		    if (z->y < 0)
			z->y = -(log10(fabs(z->y)));
		    else if (z->y > 0)
			z->y = log10(fabs(z->y));
		    else z->y = 0;

		    r = z->CSumSqr();

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
	    *z = z->CPolynomial(*degree) + *q;
	    zd = *z - z1;
	    d = zd.CSumSqr();
	    return (d > rqlim);
	    }
	}
    return 0;
    }


