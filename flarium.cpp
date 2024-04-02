////////////////////////////////////////////////////////////////
// Flarium.cpp a module for the per pixel calculations of Flarium fractals. 
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

static	int	TierazonPtr = 1, TierazonNum = 0;
extern	Complex	z, q;
static	Complex	a, b, caa3, z2, z4;

///////////////////////////////
//  z=z-(z*z*z-1)/3*z*z; z=z*c;
///////////////////////////////

int init_Flarium_06()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_06()
    {
    double	d;
    Complex	z1, zd;

    z2 = z;
    z1 = z.CSqr();
    z=z-(z*z1-1)/3*z1;
    z=z*q;
    zd = z-z2;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*z+c)^(cn+c);
///////////////////////////////

int init_Flarium_07()
    {
    if (!juliaflag)
	z = q;
    z2.x = param[0];
    z2.y = param[1];
    return 0;
    }

int do_Flarium_07()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = (z*z+q)^(z2+q);
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = z*z*z*z+1/c; Sharon's Star
///////////////////////////////

int init_Flarium_08()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_08()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z = z4+q.CInvert();
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*z/2+c)*(z*z/2+c); Sharon's Space Probe
///////////////////////////////

int init_Flarium_09()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_09()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr() / 2 + q;
    z = z2.CSqr();
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  t = (c/2)^2; z = z*z*z*z+t+c; Sharon08
///////////////////////////////

int init_Flarium_10()
    {
    Complex	t = q / 2;
    z2 = t.CSqr();
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_10()
    {
    double	d;
    Complex	z1, zd, z2a;

    z1 = z;
    z2a = z.CSqr();
    z4 = z2a.CSqr();
    z = z4+z2+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  t=z^(z+2); z = z*z*z*z+t+c;
///////////////////////////////

int init_Flarium_11()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    z2=z^(z+2);
    return 0;
    }

int do_Flarium_11()
    {
    double	d;
    Complex	z1, zd, z2a;

    z1 = z;
    z2a = z.CSqr();
    z4 = z2a.CSqr();
    z = z4+z2+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z+z*z/2)+c;
///////////////////////////////

int init_Flarium_12()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_12() 
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = (z+z.CSqr()/2)+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z2 = 0;z1 = z;z = (z*z*z*z-z2*z2*z2*z2+c)*(z*z*z*z-z2*z2*z2*z2+c);z2 = z1
///////////////////////////////

int init_Flarium_13()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    z2 = 0;
    return 0;
    }

int do_Flarium_13()
    {
    double	d;
    Complex	z1, zd, z2a, quadz2, t;

    z1 = z;
    z2a =z.CSqr();
    z4 = z2a.CSqr();
    z2a = z2.CSqr();
    quadz2 = z2a.CSqr();
//    z = (z*z*z*z-z2*z2*z2*z2+q)*(z*z*z*z-z2*z2*z2*z2+q);
    t = (z4 - quadz2 + q);
    z = t.CSqr();
    z2 = z1;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z2=0; z = z - (z*z*z*z-1)/(4*z*z*z) + c;
///////////////////////////////

int init_Flarium_15()
    {
    z.x = 1.0 + param[0];
    z.y = 0.0 + param[1];
    return 0;
    }

int do_Flarium_15() 
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CCube();
    z = z - (z2*z-1)/(4*z2) + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z = z - (z*z*z*z*z-1)/(5*z*z*z*z) + c;
///////////////////////////////

int init_Flarium_16()
    {
    z.x = 1.0 + param[0];
    z.y = 0.0 + param[1];
    return 0;
    }

int do_Flarium_16()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z = z - (z4*z-1)/(5*z4) + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z = z*z*z*z+c.imag()+c.real()*z2; 4th Order Phoenix
///////////////////////////////

int init_Flarium_20()
    {
    Complex t;

    z.x = q.x + param[0];
    z.y = q.y + param[1];
    t.x = 0;
    t.y = 1;
    z2 = 0;
    if (!juliaflag)
	q *= t;
    else
	{
	q *= t;	
	z *= t;
	}
    return 0;
    }

int do_Flarium_20()
    {
    double	d;
    Complex	z1, zd, z2a;

    z1 = z;
    z2a = z.CSqr();
    z4 = z2a.CSqr();
    z = z4 + q.y + q.x * z2;
    z2 = z1;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=(1/z*z-c)*(z*z*z*z+c); Sharon03
///////////////////////////////

int init_Flarium_21()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_21()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z=(z.CInvert()*z-q)*(z4+q);
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z1=z*z*z;z=z-(z*z1-z)/(4*z1-z);z=z*c;
///////////////////////////////

int init_Flarium_25()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_25()
    {
    double	d;
    Complex	z1, zd;

    z2 = z;
    z1=z.CCube();
    z=z-(z*z1-z)/(4*z1-z);
    z=z*q;
    zd = z-z2;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z*z*(cn+z)/(cn+z+c)+c; 
///////////////////////////////

int init_Flarium_27()
    {
    if (!juliaflag)
	z = q;
    z2.x = param[0];
    z2.y = param[1];
    return 0;
    }

int do_Flarium_27()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z=z.CSqr()*(z2+z)/(z2+z+q)+q; 
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=(1/z*z-c)*(z*z*z*z+c); Sharon03
///////////////////////////////

int init_Flarium_28()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_28()
    {
    double	d;
    Complex	z1, zd;
    int		i;

    z1 = z;
    if (iteration >= threshold)
	{
	// Inside
	q = z;
	z += 1;
	i = 0;
	d = 1;
	while (d > MINSIZE && i++ < threshold)
	    {
	    z1 = z;
	    z = z - (z.CCube()-1)/(3*z.CSqr()) + q;
	    zd = z-z1;
	    d = zd.CSumSqr();
	    }
	iteration = i;
	return TRUE;
	}
    z=z.CSqr() + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  c = (z*z*z-1)/(3*z*z); z -= (z*z*z-1)/(3*z*z)*c;
///////////////////////////////

int init_Flarium_29()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    q = (z*z*z-1)/(3*z*z);
    return 0;
    }

int do_Flarium_29()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z -= (z.CCube()-1)/(3*z.CSqr())*q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z -= (z*z*z-1)/(3*z*z)*c;
///////////////////////////////

int init_Flarium_30()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_30()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z -= (z.CCube()-1)/(3*z.CSqr())*q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  t=(c*c*c-1)/(3*c*c);a=t;b=2*t*t*t-2*t;aa3=a*a*3;z = z*z*z-aa3*z+b;
///////////////////////////////

int init_Flarium_31()
    {
    Complex t;

    t=(q.CCube()-1)/(3*q.CSqr());			
    a=t;
    b=2*t.CCube()-2*t;
    caa3=a.CSqr()*3;
    if (!juliaflag)
	z=-a;
    z2.x = 42;
    z2.y = 42;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_31()
    {
    double	d;
    Complex	z1, zd;

    z2 = z;
    z = z.CCube()-caa3*z+b;
    zd = z-z2;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z2=z;z1 = z*z;z = z - (z*z1-1)/(3*z1) + c;
///////////////////////////////

int init_Flarium_32()
    {
    if (!juliaflag)
	z = 1;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_32()
    {
    double	d;
    Complex	z1, zd;

    z2 = z;
    z1 = z.CSqr();
    z = z - (z*z1-1)/(3*z1) + q;
    zd = z-z2;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14]
///////////////////////////////

int init_Flarium_34()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_34()
    {
    double	d;
    Complex	z1, zd, t;

    z2 = z;
    t = z.CSqr();
    z1 = z*t.CSin()/2;
    z = z1.CSqr() + q;
    zd = z-z2;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=c*(z.csin());
///////////////////////////////

int init_Flarium_35()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_35()
    {
    double	d;
    Complex	z1, zd;

    z2 = z;
    z = q*z.CSin();
    zd = z-z2;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=(c*z).csin();
///////////////////////////////

int init_Flarium_36()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_36()
    {
    double	d;
    Complex	z1, zd, t;

    z2 = z;
    t = z * q;
    z = t.CSin();
    zd = z-z2;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=(z*z*z-1)/(3*z*z); z = c*(z.csin() + z.ccos());
///////////////////////////////

int init_Flarium_37()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    z=(z*z*z-1)/(3*z*z);
    return 0;
    }

int do_Flarium_37()
    {
    double	d;
    Complex	z1, zd;

    z2 = z;
    z = q*(z.CSin() + z.CCos());
    zd = z-z2;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = z*z+c; z.set_real(z.real()*z.real()); [Variation real]
///////////////////////////////

int init_Flarium_38()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_38()
    {
    double	d;
    Complex	z1, zd;

    z2 = z;
    z = z*z+q;
    z.x = z.x*z.x;
    zd = z-z2;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z1 = z*z*z*z; z = c*z1/4*z1 + z;
///////////////////////////////

int init_Flarium_40()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_40()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z = q*z4/4*z4 + z;
    zd = z-z4;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = c*(z*z*z*z).csin();
///////////////////////////////

int init_Flarium_41()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_41()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z = q*z4.CSin();	
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = z*z*z*z + (z*c).csin() + c;
///////////////////////////////

int init_Flarium_43()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_43()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    t = z * q;
    z = z4 + t.CSin() + q;	
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=(z*z*z*z-z)/(4*z*z*z); z=c*z.csin();
///////////////////////////////

int init_Flarium_44()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    z=(z*z*z*z-z)/(4*z*z*z);
    return 0;
    }

int do_Flarium_44()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = q*z.CSin();	
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=(z*z*z*z-1)/(4*z*z*z); z=c*z.csin();
///////////////////////////////

int init_Flarium_45()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    z=(z*z*z*z-1)/(4*z*z*z);
    return 0;
    }

int do_Flarium_45()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = q*z.CSin();	
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z1= z*(z*z).csin()/2; z=z1*z1 + c; [Sharon14 N-Method]
///////////////////////////////

int init_Flarium_46()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_46()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    t = z.CSqr();
    z2 = t.CSin()*z/2;
    z = z2.CSqr() + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=(z*z*z*z-z)/((4*z*z*z)-z); z=c*z.csin();
///////////////////////////////

int init_Flarium_49()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    z=(z*z*z*z-z)/((4*z*z*z)-z);
    return 0;
    }

int do_Flarium_49()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = q*z.CSin();	
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = z*z*z*z+1/c; [Sharon's Star M-Set]
///////////////////////////////

int init_Flarium_50()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_50()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z = z4+q.CInvert();
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*z/2+c)*(z*z/2+c); [Space Probe M-Set]
///////////////////////////////

int init_Flarium_51()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_51()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr()/2 + q;
    z = z2.CSqr();
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = z*z*z*z+t+c; [Sharon08 M-Set]
///////////////////////////////

int init_Flarium_52()
    {
    Complex t;
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    t = q / 2;
    z2 = t.CSqr();
    return 0;
    }

int do_Flarium_52()
    {
    double	d;
    Complex	z1, z2a, zd;

    z1 = z;
    z2a = z.CSqr();
    z4 = z2a.CSqr();
    z = z4+z2+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z+z*z/2)+c; [M-Set]
///////////////////////////////

int init_Flarium_53()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_53()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = (z+z.CSqr()/2) + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = z*z*c+c; [M-Set]
///////////////////////////////

int init_Flarium_54()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_54()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = z.CSqr()*q+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*z).csin()*z*z*c+c; [M-Set]
///////////////////////////////

int init_Flarium_55()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_55()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    t = z.CSqr();
    z = t.CSin()*z.CSqr()*q+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*z+c)/(z*z-c); [M-Set]
///////////////////////////////

int init_Flarium_56()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_56()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z = (z2+q)/(z2-q); 
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=z-(z*z1-z)/((5*z1)-z)+c; [5th Order Nova Variation]
///////////////////////////////

int init_Flarium_57()
    {
    if (!juliaflag)
	z = 1;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_57()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z=z-(z*z4-z)/((5*z4)-z)+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z-(z*z1-z)/((3*z1)-z)+c; [3rd Order Nova Variation]
///////////////////////////////

int init_Flarium_58()
    {
    if (!juliaflag)
	z = 1;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_58()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z=z-(z*z2-z)/((3*z2)-z)+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z-(z*z1-z)/((3*z1)-1)+c; [3rd Order Nova Variation]
///////////////////////////////

int init_Flarium_59()
    {
    if (!juliaflag)
	z = 1;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_59()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z=z-(z*z2-z)/((3*z2)-1)+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z-(z*z1-z)/((5*z1)-1)+c; [5th Order Nova Variation]
///////////////////////////////

int init_Flarium_60()
    {
    if (!juliaflag)
	z = 1;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_60()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z=z-(z*z4-z)/((5*z4)-1)+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z=z*z*z*z+(c+(c/pi));
///////////////////////////////

int init_Flarium_61()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    q=q+(q/PI);
    return 0;
    }

int do_Flarium_61()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CSqr();
    z4 = z2.CSqr();
    z = z4 + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=z*z*(1+z)/(1+z+c)+c;
///////////////////////////////

int init_Flarium_62()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_62()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z=z.CSqr()*(1+z)/(1+z+q)+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  PHOENIX: z1=z; z=z*z*z*z+c.real()*z2/2+c.imag()*z2/2+c; z2=z1;
///////////////////////////////

int init_Flarium_64()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    z2 = z;
    return 0;
    }

int do_Flarium_64()
    {
    double	d;
    Complex	z1, zd, z2a;

    z1 = z;
    z2a = z.CSqr();
    z4 = z2a.CSqr();
    z=z4+q.x*q.y*z2/2+q;
    z2 = z1;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = ((z-(((z^2)-1)/(2*z)))^2)*c;
///////////////////////////////

int init_Flarium_66()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_66()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z=z-(z.CSqr()-1)/(2*z);
    z=z.CSqr()*q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c;
///////////////////////////////

int init_Flarium_67()
    {
    degree = (int)param[0];
    if (degree < 2)
	degree = 2; 
    if (!juliaflag)
	z = 1;
    z.x += param[1];
    z.y += param[2];
    return 0;
    }

int do_Flarium_67()
    {
    double	d;
    Complex	z1, zd, fn, f1n;

    z1 = z;
    f1n = z.CPolynomial(degree-1);			// z^(deg - 1) - first derivative power
    fn = f1n*z;						// z^(deg)
    z = z - (fn-1)/(degree*f1n);
    z = z.CSqr() * q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=((z*z*z*z-1)/(1-(3*z*z*z)))*c;
///////////////////////////////

int init_Flarium_80()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_80()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z2 = z.CCube();   // A correct one, finally
    z=((z*z2-1)/(1-(3*z2)))*q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

///////////////////////////////
//  z = (z*(z*z).csin()/2).csin()+c; [Sharon15]
///////////////////////////////

int init_Flarium_90()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_90()
    {
    double	d;
    Complex	z1, zd, s, t;

    z1 = z;
    t = z.CSqr();
    s = z * t.CSin() / 2;
    z = s.CSin()+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*(z+z).ccos()/2); z = z*z+c; [Sharon16]
///////////////////////////////

int init_Flarium_91()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_91()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    t = z + z;
    z = (z*t.CCos()/2);
    z = z.CSqr()+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*(z+z).csin()/2); z = z*z+c; [Sharon17]
///////////////////////////////

int init_Flarium_92()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_92()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    t = z + z;
    z = (z*t.CSin()/2);
    z = z.CSqr()+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*(z+z*z).csin()/2); z = z*z+c; [Sharon18]
///////////////////////////////

int init_Flarium_93()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_93()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    t = z + z.CSqr();
    z = (z*t.CSin()/2);
    z = z.CSqr()+q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
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

int init_Flarium_111()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_111()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    t = z.CSqr();
//    z2 = z*CSinError(CSqr(z))/2;
    z2 = z*t.CSin()/2;
    z = z2.CSqr() + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=z^n*c+z*c; Dragon curve variation
///////////////////////////////

int init_Flarium_112()
    {
    degree = (int)param[0];
    if (degree < 2)
	degree = 2; 
    if (!juliaflag)
	z = q;
    z.x += param[1];
    z.y += param[2];
    return 0;
    }

int do_Flarium_112()
    {
    double	d;
    Complex	z1, zd, fn;

    z1 = z;
    fn = z.CPolynomial(degree);
    z=fn*q+z*q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = z-(z*z*z-z*c-1)/(3*z*z+c-1);
///////////////////////////////

int init_Flarium_117()
    {
    if (!juliaflag)
	z = 0;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_117()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = z-(z*z*z-z*q-1)/(3*z*z+q-1);
//    z = z-(CCube(z)-z*q-1)/(3*CSqr(z)+q-1);
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
//    return (d > rqlim);
    }

///////////////////////////////
//  z = (z*z*z*z*z).csin() + c; [Sharon19]
///////////////////////////////

int init_Flarium_118()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_118()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    t = z.CCube()*z.CSqr();
    z = t.CSin() + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = (z+(z*z)/.192).csin() + c; [Sharon's Butterfly]
///////////////////////////////

int init_Flarium_119()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_119()
    {
    double	d;
    Complex	z1, zd, t;

    z1 = z;
    t = z + (z*z) / .192;
    z = t.CSin() + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z = z+z*z*z/4 + c; [Sharon21]
///////////////////////////////

int init_Flarium_125()
    {
    if (!juliaflag)
	z = q;
    z.x += param[0];
    z.y += param[1];
    return 0;
    }

int do_Flarium_125()
    {
    double	d;
    Complex	z1, zd;

    z1 = z;
    z = z+z.CCube()/4 + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }

///////////////////////////////
//  z=z^2+c [Jaenisch method]
///////////////////////////////

int init_Flarium_145()
    {
    degree = (int)param[0];
    if (degree < 2)
	degree = 2; 
    if (!juliaflag)
	z = q;
    z4.x = param[1];
    z4.y = param[2];
    z2 = z;
    return 0;
    }

int do_Flarium_145()
    {
    double	d;
    Complex	z1, zd;
    int		i, rzflag, jrw;
    int		nFF = 1;
    int		NMAX = (threshold * 2) / 3;
    double	r, rz;
    long	LocalIteration;
//    BOOL	b_MAX;

    z1 = z;
    if (iteration >= threshold)
	{
	// Inside
	z = z2;
	i = 0;
	jrw=0;
	LocalIteration=NMAX-2;
	while (i++ < LocalIteration)
	    {
	    //z=z*z + c;
//	    z=CCube(z) + q;
//	    z = CPolynomial(z, degree) + q;
	    z=z^z4 + q;

	    if (z.x < 0)
		z.x = -(log10(fabs(z.x)));
	    else if (z.x > 0)
		z.x = log10(fabs(z.x));
	    else z.x = 0;

	    if (z.y < 0)
		z.y = -(log10(fabs(z.y)));
	    else if (z.y > 0)
		z.y =  log10(fabs(z.y));
	    else z.y = 0;

	    r = z.CSumSqr();

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
    z = z.CPolynomial(degree) + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d > rqlim);
    }





///////////////////////////////
//  z = z - (z^n-1)/(n*z^(n-1)) + c;
///////////////////////////////

int init_Nova()
    {
    if (degree < 2)
	degree = 2; 
    z.x = 1.0 + param[1];
    z.y = 0.0 + param[2];
    return 0;
    }

int do_Nova()
    {
    double	d;
    Complex	z1, zd, fn, f1n;

    z1 = z;
    f1n = z.CPolynomial(degree-1);			// z^(deg - 1) - first derivative power
    fn = f1n*z;						// z^(deg)
    z = z - (fn-1)/(degree*f1n) + q;
    zd = z-z1;
    d = zd.CSumSqr();
    return (d < MINSIZE);
    }

