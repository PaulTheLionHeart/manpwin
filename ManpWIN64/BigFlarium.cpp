////////////////////////////////////////////////////////////////
// Flarium.cpp a module for the per pixel calculations of Flarium fractals using Bignum functions. 
//
/////////////////////////////////////////////////////
// Updated in Microsoft Visual 'C++' by Paul de Leeuw.
// These are listed in numerical order of the functions.
// Note that CSqr(z) is used in place of z*z and CCube(z) in place of z*z*z for speed
/////////////////////////////////////////////////////

#include	"manp.h"
#include	"complex.h"
#include	"resource.h"
#include	"fractalp.h"
#include	<math.h>
#include	"big.h"
#include	"BigDouble.h"
#include	"BigComplex.h"
#include	"Complex.h"

#define	MINSIZE	1.0e-20
#define	MAXSIZE	1.0e+20
#define	PALSIZE	256

extern	long	iteration;		// globals for speed
extern	long	threshold;

extern	double	param[];
extern	double	rqlim;			// bailout level
extern	BYTE	degree;			// power
extern	BYTE	juliaflag;		// Julia implementation of fractal
extern	int	subtype;		// which Tierazon fractal?

extern	BigComplex	zBig, qBig;
extern	BigDouble	BigBailout;
static	BigComplex	a, b, c1, c2, cb, caa3, z2, z4;

///////////////////////////////
//  z=z-(z*z*z-1)/3*z*z; z=z*c;
///////////////////////////////

int init_big_Flarium_06()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_06()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z2 = zBig;
    z1 = zBig.CSqr();
    zBig=zBig-(zBig*z1-1)/3*z1;
    zBig=zBig*qBig;
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z*z+c)^(cn+c);
///////////////////////////////

int init_big_Flarium_07()
    {
    if (!juliaflag)
	zBig = qBig;
    z2.x = param[0];
    z2.y = param[1];
    return 0;
    }

int do_big_Flarium_07()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = (zBig*zBig+qBig)^(z2+qBig);
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z*z*z*z+1/c; Sharon's Star
///////////////////////////////

int init_big_Flarium_08()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_08()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig = z4+qBig.CInvert();
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z*z/2+c)*(z*z/2+c); Sharon's Space Probe
///////////////////////////////

int init_big_Flarium_09()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_09()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr() / 2 + qBig;
    zBig = z2.CSqr();
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  t = (c/2)^2; z = z*z*z*z+t+c; Sharon08
///////////////////////////////

int init_big_Flarium_10()
    {
    z2 = qBig/2;
    z2 = z2.CSqr();
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_10()
    {
    BigDouble	d;
    BigComplex	z1, zd, z2a;

    z1 = zBig;
    z2a = zBig.CSqr();
    z4 = z2a.CSqr();
    zBig = z4+z2+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  t=z^(z+2); z = z*z*z*z+t+c;
///////////////////////////////

int init_big_Flarium_11()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    z2=zBig^(zBig+2);
    return 0;
    }

int do_big_Flarium_11()
    {
    BigDouble	d;
    BigComplex	z1, zd, z2a;

    z1 = zBig;
    z2a = zBig.CSqr();
    z4 = z2a.CSqr();
    zBig = z4+z2+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z+z*z/2)+c;
///////////////////////////////

int init_big_Flarium_12()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_12() 
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = (zBig+zBig.CSqr()/2)+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z2 = 0;z1 = z;z = (z*z*z*z-z2*z2*z2*z2+c)*(z*z*z*z-z2*z2*z2*z2+c);z2 = z1
///////////////////////////////

int init_big_Flarium_13()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    z2 = 0;
    return 0;
    }

int do_big_Flarium_13()
    {
    BigDouble	d;
    BigComplex	z1, zd, z2a, quadz2, t;

    z1 = zBig;
    z2a = zBig.CSqr();
    z4 = z2a.CSqr();
    z2a = z2.CSqr();
    quadz2 = z2a.CSqr();
//    z = (z*z*z*z-z2*z2*z2*z2+q)*(z*z*z*z-z2*z2*z2*z2+q);
    t = z4-quadz2+qBig;
    zBig = t.CSqr();
    z2 = z1;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z2=0; z = z - (z*z*z*z-1)/(4*z*z*z) + c;
///////////////////////////////

int init_big_Flarium_15()
    {
    zBig.x = 1.0 + param[0];
    zBig.y = 0.0 + param[1];
    return 0;
    }

int do_big_Flarium_15() 
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CCube();
    zBig = zBig - (z2*zBig-1)/(z2*4) + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z = z - (z*z*z*z*z-1)/(5*z*z*z*z) + c;
///////////////////////////////

int init_big_Flarium_16()
    {
    zBig.x = 1.0 + param[0];
    zBig.y = 0.0 + param[1];
    return 0;
    }

int do_big_Flarium_16()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig = zBig - (z4*zBig-1)/(z4*5) + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z = z*z*z*z+c.imag()+c.real()*z2; 4th Order Phoenix
///////////////////////////////

int init_big_Flarium_20()
    {
    BigComplex t;

    zBig.x = qBig.x + param[0];
    zBig.y = qBig.y + param[1];
    t.x = 0;
    t.y = 1;
    z2 = 0;
    if (!juliaflag)
	qBig *= t;
    else
	{
	qBig *= t;	
	zBig *= t;
	}
    return 0;
    }

int do_big_Flarium_20()
    {
    BigDouble	d;
    BigComplex	z1, zd, z2a;

    z1 = zBig;
    z2a = zBig.CSqr();
    z4 = z2a.CSqr();
    zBig = z4 + qBig.y + z2 * qBig.x;
    z2 = z1;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=(1/z*z-c)*(z*z*z*z+c); Sharon03
///////////////////////////////

int init_big_Flarium_21()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_21()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig=(zBig.CInvert()*zBig-qBig)*(z4+qBig);
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z1=z*z*z;z=z-(z*z1-z)/(4*z1-z);z=z*c;
///////////////////////////////

int init_big_Flarium_25()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_25()
    {
    double	d;
    BigComplex	z1, zd;

    z2 = zBig;
    z1=zBig.CCube();
    zBig=zBig-(zBig*z1-zBig)/(z1*4-zBig);
    zBig=zBig*qBig;
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z*z*(cn+z)/(cn+z+c)+c; 
///////////////////////////////

int init_big_Flarium_27()
    {
    if (!juliaflag)
	zBig = qBig;
    z2.x = param[0];
    z2.y = param[1];
    return 0;
    }

int do_big_Flarium_27()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig=zBig.CSqr()*(z2+zBig)/(z2+zBig+qBig)+qBig; 
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=(1/z*z-c)*(z*z*z*z+c); Sharon03
///////////////////////////////

int init_big_Flarium_28()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_28()
    {
    double	d;
    BigDouble	d1;
    BigComplex	z1, zd;
    int		i;

    z1 = zBig;
    if (iteration >= threshold)
	{
	// Inside
	qBig = zBig;
	zBig = zBig + 1;
	i = 0;
	d = 1;
	while (d > MINSIZE && i++ < threshold)
	    {
	    z1 = zBig;
	    zBig = zBig - (zBig.CCube()-1)/(zBig.CSqr() * 3) + qBig;
	    zd = zBig-z1;
	    d = zd.CSumSqr();
	    }
	iteration = i;
	return TRUE;
	}
    zBig=zBig.CSqr() + qBig;
    zd = zBig-z1;
    d1 = zd.CSumSqr();
    return (d1 > BigBailout);
    }

///////////////////////////////
//  c = (z*z*z-1)/(3*z*z); z -= (z*z*z-1)/(3*z*z)*c;
///////////////////////////////

int init_big_Flarium_29()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    qBig = (zBig*zBig*zBig-1)/(zBig*zBig * 3);
    return 0;
    }

int do_big_Flarium_29()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig -= (zBig.CCube()-1)/(zBig.CSqr() * 3)*qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z -= (z*z*z-1)/(3*z*z)*c;
///////////////////////////////

int init_big_Flarium_30()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_30()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig -= (zBig.CCube()-1)/(zBig.CSqr() * 3)*qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  t=(c*c*c-1)/(3*c*c);a=t;b=2*t*t*t-2*t;aa3=a*a*3;z = z*z*z-aa3*z+b;
///////////////////////////////

int init_big_Flarium_31()
    {
    BigComplex t;

    t=(qBig.CCube()-1)/(qBig.CSqr()*3);			
    a=t;
    b=t.CCube()*2-t*2;
    caa3=a.CSqr()*3;
    if (!juliaflag)
	zBig=-a;
    z2.x = 42;
    z2.y = 42;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_31()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z2 = zBig;
    zBig = zBig.CCube()-caa3*zBig+b;
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z2=z;z1 = z*z;z = z - (z*z1-1)/(3*z1) + c;
///////////////////////////////

int init_big_Flarium_32()
    {
    if (!juliaflag)
	zBig = 1;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_32()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z2 = zBig;
    z1 = zBig.CSqr();
    zBig = zBig - (zBig*z1-1)/(z1*3) + qBig;
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14]
///////////////////////////////

int init_big_Flarium_34()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_34()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z2 = zBig;
    a = zBig.CSqr();
    z1 = zBig*a.CSin()/2;
    zBig = z1.CSqr() + qBig;
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=c*(z.csin());
///////////////////////////////

int init_big_Flarium_35()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_35()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z2 = zBig;
    zBig = qBig*zBig.CSin();
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=(c*z).csin();
///////////////////////////////

int init_big_Flarium_36()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_36()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z2 = zBig;
    a = zBig*qBig;
    zBig = a.CSin();
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=(z*z*z-1)/(3*z*z); z = c*(z.csin() + z.ccos());
///////////////////////////////

int init_big_Flarium_37()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    zBig=(zBig*zBig*zBig-1)/(zBig*zBig*3);
    return 0;
    }

int do_big_Flarium_37()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z2 = zBig;
    zBig = qBig*(zBig.CSin() + zBig.CCos());
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z*z+c; z.set_real(z.real()*z.real()); [Variation real]
///////////////////////////////

int init_big_Flarium_38()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_38()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z2 = zBig;
    zBig = zBig*zBig+qBig;
    zBig.x = zBig.x*zBig.x;
    zd = zBig-z2;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z1 = z*z*z*z; z = c*z1/4*z1 + z;
///////////////////////////////

int init_big_Flarium_40()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_40()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig = qBig*z4/4*z4 + zBig;
    zd = zBig-z4;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = c*(z*z*z*z).csin();
///////////////////////////////

int init_big_Flarium_41()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_41()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig = qBig*z4.CSin();	
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z*z*z*z + (z*c).csin() + c;
///////////////////////////////

int init_big_Flarium_43()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_43()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    a = zBig*qBig;
    zBig = z4 + a.CSin() + qBig;	
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=(z*z*z*z-z)/(4*z*z*z); z=c*z.csin();
///////////////////////////////

int init_big_Flarium_44()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    zBig=(zBig*zBig*zBig*zBig-zBig)/(zBig*zBig*zBig*4);
    return 0;
    }

int do_big_Flarium_44()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = qBig*zBig.CSin();	
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=(z*z*z*z-1)/(4*z*z*z); z=c*z.csin();
///////////////////////////////

int init_big_Flarium_45()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    zBig=(zBig*zBig*zBig*zBig-1)/(zBig*zBig*zBig*4);
    return 0;
    }

int do_big_Flarium_45()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = qBig*zBig.CSin();	
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14 N-Method]
///////////////////////////////

int init_big_Flarium_46()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_46()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    a = zBig.CSqr();
    z2 = a.CSin()*zBig/2;
    zBig = z2.CSqr() + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=(z*z*z*z-z)/((4*z*z*z)-z); z=c*z.csin();
///////////////////////////////

int init_big_Flarium_49()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    zBig=(zBig*zBig*zBig*zBig-zBig)/((zBig*zBig*zBig*4)-zBig);
    return 0;
    }

int do_big_Flarium_49()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = qBig*zBig.CSin();	
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z*z*z*z+1/c; [Sharon's Star M-Set]
///////////////////////////////

int init_big_Flarium_50()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_50()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig = z4+qBig.CInvert();
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z*z/2+c)*(z*z/2+c); [Space Probe M-Set]
///////////////////////////////

int init_big_Flarium_51()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_51()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr()/2 + qBig;
    zBig = z2.CSqr();
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z*z*z*z+t+c; [Sharon08 M-Set]
///////////////////////////////

int init_big_Flarium_52()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    a = qBig/2;
    z2 = a.CSqr();
    return 0;
    }

int do_big_Flarium_52()
    {
    BigDouble	d;
    BigComplex	z1, z2a, zd;

    z1 = zBig;
    z2a = zBig.CSqr();
    z4 = z2a.CSqr();
    zBig = z4+z2+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z+z*z/2)+c; [M-Set]
///////////////////////////////

int init_big_Flarium_53()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_53()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = (zBig+zBig.CSqr()/2) + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z*z*c+c; [M-Set]
///////////////////////////////

int init_big_Flarium_54()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_54()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = zBig.CSqr()*qBig+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z*z).csin()*z*z*c+c; [M-Set]
///////////////////////////////

int init_big_Flarium_55()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_55()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    a = zBig.CSqr();
    zBig = a.CSin()*zBig.CSqr()*qBig+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z*z+c)/(z*z-c); [M-Set]
///////////////////////////////

int init_big_Flarium_56()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_56()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    zBig = (z2+qBig)/(z2-qBig); 
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=z-(z*z1-z)/((5*z1)-z)+c; [5th Order Nova Variation]
///////////////////////////////

int init_big_Flarium_57()
    {
    if (!juliaflag)
	zBig = 1;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_57()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig=zBig-(zBig*z4-zBig)/((z4*5)-zBig)+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]
///////////////////////////////

int init_big_Flarium_58()
    {
    if (!juliaflag)
	zBig = 1;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_58()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    zBig=zBig-(zBig*z2-zBig)/((z2*3)-zBig)+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z-(z*z1-z)/((3*z1)-1)+c; [3rd Order Nova Variation]
///////////////////////////////

int init_big_Flarium_59()
    {
    if (!juliaflag)
	zBig = 1;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_59()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    zBig=zBig-(zBig*z2-zBig)/((z2*3)-1)+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z-(z*z1-z)/((5*z1)-1)+c; [5th Order Nova Variation]
///////////////////////////////

int init_big_Flarium_60()
    {
    if (!juliaflag)
	zBig = 1;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_60()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig=zBig-(zBig*z4-zBig)/((z4*5)-1)+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z*z*z*z+(c+(c/pi));
///////////////////////////////

int init_big_Flarium_61()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    qBig=qBig+(qBig/PI);
    return 0;
    }

int do_big_Flarium_61()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr();
    z4 = z2.CSqr();
    zBig = z4 + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=z*z*(1+z)/(1+z+c)+c;
///////////////////////////////

int init_big_Flarium_62()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_62()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig=zBig.CSqr()*(zBig+1)/(zBig+qBig+1)+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  PHOENIX: z1=z; z=z*z*z*z+c.real()*z2/2+c.imag()*z2/2+c; z2=z1;
///////////////////////////////

int init_big_Flarium_64()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    z2 = zBig;
    return 0;
    }

int do_big_Flarium_64()
    {
    BigDouble	d;
    BigComplex	z1, zd, z2a;

    z1 = zBig;
    z2a = zBig.CSqr();
    z4 = z2a.CSqr();
    zBig=z4+(z2/2)*qBig.x*qBig.y+qBig;
    z2 = z1;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = ((z-(((z^2)-1)/(2*z)))^2)*c;
///////////////////////////////

int init_big_Flarium_66()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_66()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig=zBig-(zBig.CSqr()-1)/(zBig+zBig);
    zBig=zBig.CSqr()*qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c;
///////////////////////////////

int init_big_Flarium_67()
    {
    degree = (BYTE)param[0];
    if (degree < 2)
	degree = 2; 
    if (!juliaflag)
	zBig = 1;
    zBig.x += param[1];
    zBig.y += param[2];
    return 0;
    }

int do_big_Flarium_67()
    {
    BigDouble	d;
    BigComplex	z1, zd, fn, f1n;

    z1 = zBig;
    f1n = zBig.CPolynomial(degree-1);			// z^(deg - 1) - first derivative power
    fn = f1n*zBig;						// z^(deg)
    zBig = zBig - (fn-1)/(f1n*degree);
    zBig = zBig.CSqr() * qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=((z*z*z*z-1)/(1-(3*z*z*z)))*c;
///////////////////////////////

int init_big_Flarium_80()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_80()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CCube();   // A correct one, finally
    a = 1;
    zBig=((zBig*z2-1)/(a-(z2*3)))*qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z = (z*(z*z).csin()/2).csin()+c; [Sharon15]
///////////////////////////////

int init_big_Flarium_90()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_90()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    a = zBig.CSqr();
    b = zBig*a.CSin()/2;
    zBig = b.CSin()+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z*(z+z).ccos()/2); z = z*z+c; [Sharon16]
///////////////////////////////

int init_big_Flarium_91()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_91()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig*2;
    zBig = (zBig*z2.CCos()/2);
    zBig = zBig.CSqr()+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z*(z+z).csin()/2); z = z*z+c; [Sharon17]
///////////////////////////////

int init_big_Flarium_92()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_92()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig*2;
    zBig = (zBig*z2.CSin()/2);
    zBig = zBig.CSqr()+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z*(z+z*z).csin()/2); z = z*z+c; [Sharon18]
///////////////////////////////

int init_big_Flarium_93()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_93()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    z2 = zBig.CSqr()+zBig;
    zBig = (zBig*z2.CSin()/2);
    zBig = zBig.CSqr()+qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z1= z*(z*z).csin()/2; z=z1*z1 + c; [Alden's Ray Method]
///////////////////////////////

/*
#define	TOPEXP		350
#define	ERRORCODE	-1.0e60
Complex	CSinError(Complex  & Cmplx1)
    {
    static  Complex a;

    if (Cmplx1.y > TOPEXP)
	{
	a.x = ERRORCODE;
	a.y = 0;
	}
    else if (Cmplx1.y < -TOPEXP) 
	a = 0;
    else
	{
//    return cmplx(.5*(exp(y)+1./exp(y))*sin(x),cos(x)*.5*(exp(y)-1./exp(y)));
	a.x = .5*(exp(Cmplx1.y)+1./exp(Cmplx1.y))*sin(Cmplx1.x);
	a.y = cos(Cmplx1.x)*.5*(exp(Cmplx1.y)-1./exp(Cmplx1.y));
	}
    return a;
    }
*/

int init_big_Flarium_111()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_111()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
//    z2 = z*CSinError(CSqr(z))/2;
    a = zBig.CSqr();
    z2 = zBig*a.CSin()/2;
    zBig = z2.CSqr() + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=z^n*c+z*c; Dragon curve variation
///////////////////////////////

int init_big_Flarium_112()
    {
    degree = (BYTE)param[0];
    if (degree < 2)
	degree = 2; 
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[1];
    zBig.y += param[2];
    return 0;
    }

int do_big_Flarium_112()
    {
    BigDouble	d;
    BigComplex	z1, zd, fn;

    z1 = zBig;
    fn = zBig.CPolynomial(degree);
    zBig=fn*qBig+zBig*qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z-(z*z*z-z*c-1)/(3*z*z+c-1);
///////////////////////////////

int init_big_Flarium_117()
    {
    if (!juliaflag)
	zBig = 0;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_117()
    {
    double	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = zBig-(zBig*zBig*zBig-zBig*qBig-1)/(zBig*zBig*3+qBig-1);
//    z = z-(CCube(z)-z*q-1)/(3*CSqr(z)+q-1);
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
//    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*z*z*z*z).csin() + c; [Sharon19]
///////////////////////////////

int init_big_Flarium_118()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_118()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    a = zBig.CCube()*zBig.CSqr();
    zBig = a.CSin() + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = (z+(z*z)/.192).csin() + c; [Sharon's Butterfly]
///////////////////////////////

int init_big_Flarium_119()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_119()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    a = zBig+(zBig*zBig)/.192;
    zBig = a.CSin() + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z+z*z*z/4 + c; [Sharon21]
///////////////////////////////

int init_big_Flarium_125()
    {
    if (!juliaflag)
	zBig = qBig;
    zBig.x += param[0];
    zBig.y += param[1];
    return 0;
    }

int do_big_Flarium_125()
    {
    BigDouble	d;
    BigComplex	z1, zd;

    z1 = zBig;
    zBig = zBig+zBig.CCube()/4 + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z=z^2+c [Jaenisch method]
///////////////////////////////

int init_big_Flarium_145()
    {
    degree = (BYTE)param[0];
    if (degree < 2)
	degree = 2; 
    if (!juliaflag)
	zBig = qBig;
    z4.x = param[1];
    z4.y = param[2];
    z2 = zBig;
    return 0;
    }

int do_big_Flarium_145()
    {
    BigDouble	s, t, d;
    BigComplex	z1, zd;
    int		i, rzflag, jrw;
    int		nFF = 1;
    int		NMAX = (threshold * 2) / 3;
    BigDouble	r, rz;
    long	LocalIteration;

    z1 = zBig;
    rzflag = 1;
    if (iteration >= threshold)
	{
	// Inside
	zBig = z2;
	i = 0;
	jrw=0;
	LocalIteration=NMAX-2;
	while (i++ < LocalIteration)
	    {
	    zBig=zBig^z4 + qBig;

	    t = 0;
	    if (zBig.x < t)
		{
		s = zBig.x.BigAbs();
		zBig.x = -(s.BigLog10());
		}
	    else if (zBig.x > t)
		{
		s = zBig.x.BigAbs();
		zBig.x = s.BigLog();
		}
	    else zBig.x = 0;

	    if (zBig.y < t)
		{
		s = zBig.y.BigAbs();
		zBig.y = -(s.BigLog10());
		}
	    else if (zBig.y > t)
		{
		s = zBig.y.BigAbs();
		zBig.y =  s.BigLog10();
		}
	    else zBig.y = 0;

	    r = zBig.CSumSqr();

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
		    jrw+=nFF;
		    rzflag = 1;
		    }
		else 
		    {
		    if (r < rz && rzflag != -1)
			{
			jrw+=nFF;
			rzflag = -1;
			}
		    }
		rz = r;
		}
	    }
	iteration = jrw;
	return TRUE;
	}
    zBig = zBig.CPolynomial(degree) + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d > BigBailout);
    }

///////////////////////////////
//  z = z - (z^n-1)/(n*z^(n-1)) + c;
///////////////////////////////

int init_big_Nova()
    {
    if (degree < 2)
	degree = 2; 
    zBig.x = 1.0 + param[1];
    zBig.y = 0.0 + param[2];
    return 0;
    }

int do_big_Nova()
    {
    double	d;
    BigComplex	z1, zd, fn, f1n;

    z1 = zBig;
    f1n = zBig.CPolynomial(degree-1);			// z^(deg - 1) - first derivative power
    fn = f1n*zBig;					// z^(deg)
    zBig = zBig - (fn-1)/(f1n*degree) + qBig;
    zd = zBig-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }
