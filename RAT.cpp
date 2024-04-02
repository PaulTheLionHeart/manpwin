/*
    RAT.CPP a module for the per pixel calculations of fractals. 
    
    Written in MICROSOFT 'C' by Paul de Leeuw.
*/

#include	"manp.h"
#include	<math.h>
#include	"complex.h"

static	double	escape, epsilon, der;

extern	long	iteration;		// globals for speed
extern	long	threshold;
extern	long	color;
extern	WORD	special;		// special colour for CBIN fractals
extern	BYTE	juliaflag;		// Julia implementation of fractal
extern	int	subtype;
extern	double	param[];

extern	Complex	z, q;
static	Complex	a, b, alpha, cmcc;
static	int	OldThreshold = 0;	// check to see if threshold changes

static	int	penpref[] = {1, 2, 3, 4};		// can we do more in mappying these colours?
static	int	pennref[] = {9, 10, 11, 12};
static	int	penp[4];
static	int	penn[4];

/**************************************************************************
	Init Rational Map fractals
**************************************************************************/

int	init_ratmap_df(void)

    {
    int	i, gap;
    int	zcount;
    Complex temp, temp1, temp2, temp3;

    switch ((int) param[0])
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
	gap = threshold / 16;			// split the colour map into 16 equal parts
	for (i = 0; i < 4; i++)
	    {
	    penp[i] = penpref[i] * gap;
	    penn[i] = pennref[i] * gap;
	    }
	}

    if (subtype == 'A')
	{
	cmcc = q-q.CSqr();			// CMCC    =   C - C*C
	temp = -q+2;
	a = temp/cmcc;				// A = (2 - C)/CMCC
	temp = cmcc+1;
	b = -temp/cmcc;				// B = -(CMCC + 1)/CMCC

						// ALPHA = 1/(C*C * (B + B + B*B/A) * (2*A*C + B))
	temp = a*q*2+b;				// 2*A*C + B
	temp1 = b.CSqr()/a+b+b;			// (B + B + B*B/A)
	temp3 = q.CSqr()*temp1*temp;
	alpha = temp3.CInvert();
	}
    else if (subtype == 'B')
	{
	a = q;
	b = a+1;
	temp = a.CSqr()-1;
	alpha = a/temp;				// ALPHA = A/(A*A - 1)
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
	temp = -a*2;
	z = b/temp;
	}
						// iterating Z  = 1/(A*Z*Z + B*Z + 1) has various proterties:
						// 	   Z  = 1/(A*Z*Z + B*Z + 1)
						// Julia   Z  = 1/(A*Z*Z + B*Z + 1)
						// Julia   Z  = 1/(A*Z*Z + B*Z + 1)
						// ?????   Z  = 1/(A*Z*Z + B*Z + 1)

    switch(subtype)
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
        
    for (i = 0; i < zcount; ++i)
	{
						// 1/(A*Z*Z + B*Z + 1)
	temp = b*z+1;				// B*Z + 1
	temp1 = z.CSqr()*a+temp;		// (A*Z*Z + B*Z + 1)
	z = temp1.CInvert();			// Z = 1/(A*Z*Z + B*Z + 1)
	}
    return(TRUE);
    }

/**************************************************************************
	Run Rational Map type fractals
**************************************************************************/

int	do_ratmap_df(void)

    {
    Complex az, d, temp, temp1;
    double  dist;

    az = a*z;					// AZ = A*Z
    temp = az*z+1;
    temp1 = b*z+temp;
    z = temp1.CInvert();			// Z = 1/(AZ*Z + B*Z + 1)
    z.x += param[2];
    z.y += param[3];
    if (z.CSumSqr() > escape)
	{
	if ((alpha.x * z.y + alpha.y * z.x) <= 0.0)
	    color = penp[iteration % 4];
	else
	    color = penn[iteration % 4];
	iteration = color;
	return (TRUE);
	}
    temp = az*2+b;
    d = z.CSqr()*temp;				// D = (2*AZ + B)*Z*Z
    dist = d.CSumSqr();
    der *= dist;				// DER  =   DER*(REAL(D)*REAL(D) + IMAG(D)*IMAG(D))
    if (der < epsilon)
	{
	iteration = threshold;
	return(TRUE);
	}
    if (iteration >= threshold)
	{
	iteration = special;
	return(TRUE);
	}
    return(FALSE);					/* continue iterations */
    }

#ifdef splat
/**************************************************************************
	Init Rational Map fractals
**************************************************************************/

int	init_ratmap_df(void)

    {
    if (!juliaflag)
	z = q;

//    z.x += param[0];
//    z.y += param[1];
    return 0;
    }

/**************************************************************************
	Run Rational Map type fractals
**************************************************************************/

extern	double	rqlim;			// bailout level

int	do_ratmap_df(void)

    {
    double	d;

//    z = CSqr(z) + q / z;
//    z = CSqr(z) + q / CSqr(z);
//    z = CCube(z) + q / CSqr(z);
//    z = CCube(z) + q / z;
    z = z*z*z*z*z - .5 + q/z;
    d = CSumSqr(z);
    return (d > rqlim);
    }
#endif

