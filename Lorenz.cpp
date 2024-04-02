/*
   This file contains two 3 dimensional orbit-type fractal
   generators - IFS and LORENZ3D, along with code to generate
   red/blue 3D images. Tim Wegner
*/

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <windows.h>
//#include "fractint.h"
#include "fractype.h"
#include "plot.h"
#include "Dib.h"
#include "colour.h"
#include "Matrix.h"
#include "Complex.h"

#define NUMIFS	  32	 /* number of ifs functions in ifs array */
#define IFSPARM    7	 /* number of ifs parameters */
#define IFS3DPARM 13	 /* number of ifs 3D parameters */
#define XROT	  x_rot		/* rotate x-axis 60 degrees */
#define YROT	  y_rot		/* rotate y-axis 90 degrees */
#define ZROT	  z_rot		/* rotate x-axis  0 degrees */

extern	double	x_rot;			/* angle display plane to x axis */
extern	double	y_rot;			/* angle display plane to y axis */
extern	double	z_rot;			/* angle display plane to z axis */

extern	int	xdots, ydots;

extern	HWND	GlobalHwnd;		// to allow passing of hwnd to find_file_item()
extern	char	*str_find_ci(char *, char *);
extern	CPlot	Plot;		// image plotting routines 

extern	CDib	Dib;			// Device Independent Bitmaps
extern	CTrueCol    TrueCol;		// palette info

#define	MaxIFSNameChoices   80
extern	char	lptr[][100];
//char	*win_choices[80];

struct affine
{
   /* weird order so a,b,e and c,d,f are vectors */
   double a;
   double b;
   double e;
   double c;
   double d;
   double f;
};

struct float3dvtinf /* data used by 3d view transform subroutine */
{
   long ct;		/* iteration counter */
   double orbit[3];		   /* interated function orbit value */
   double viewvect[3];	      /* orbit transformed for viewing */
   double viewvect1[3];        /* orbit transformed for viewing */
   double maxvals[3];
   double minvals[3];
   MATRIX doublemat;	/* transformation matrix */
   MATRIX doublemat1;	/* transformation matrix */
   int row,col; 	/* results */
   int row1,col1;
   struct affine cvt;
};

/* Routines in this module	*/

int (*orbitcalc)();		/* function that calculates one orbit */
//int  orbit3dlongsetup();
int  orbit3dfloatsetup();
//int  lorenz3dlongorbit(long *, long *, long *);
int  lorenz3dfloatorbit(double *, double *, double *);
int  lorenz3d1floatorbit(double *, double *, double *);
int  lorenz3d3floatorbit(double *, double *, double *);
int  lorenz3d4floatorbit(double *, double *, double *);
int  henonfloatorbit(double *, double *, double *);
//int  henonlongorbit(long *, long *, long *);
int  rosslerfloatorbit(double *, double *, double *);
int  pickoverfloatorbit(double *, double *, double *);
int  gingerbreadfloatorbit(double *, double *, double *);
//int  rosslerlongorbit(long *, long *, long *);
int  kamtorusfloatorbit(double *, double *, double *);
//int  kamtoruslongorbit(long *, long *, long *);
int  orbit2dfloat();
//int  orbit2dlong(void);
//int  orbit3dlongcalc(void);
int  orbit3dfloatcalc(void);
int  funny_glasses_call(int (*calc)());
int  ifs(HWND);
int  orbit3dfloat(void);
//int  orbit3dlong(void);
int  ifs2d(HWND);
int  ifs3d(HWND);

//static int  ifs3dlong(void);
static int  ifs3dfloat(HWND);
static double determinant(double mat[3][3]);
static int  solve3x3(double mat[3][3],double vec[3],double ans[3]);
	int  setup_convert_to_screen(struct affine *);
//static int  l_setup_convert_to_screen(struct l_affine *);
static void setupmatrix(MATRIX);
//static int  long3dviewtransf(struct long3dvtinf *inf);
static int  float3dviewtransf(struct float3dvtinf *inf);

extern WORD type;
//extern int init3d[15];
extern char floatflag;
/*extern */VECTOR view;
/*extern */int xxadjust = 0, yyadjust = 0;
/*extern */int xxadjust1 = 0, yyadjust1 = 0;
/*extern */int xshift = 0,yshift = 0;
/*extern */int xshift1 = 0,yshift1 = 0;
//extern int	debugflag;			/* for debugging purposes */
//extern int	xdots, ydots;		/* coordinates of dots on the screen  */
extern long	threshold;				/* try this many iterations */
extern double param[];
extern double	xxmin,xxmax,yymin,yymax/*,xx3rd,yy3rd*/; /* selected screen corners  */
double	xx3rd,yy3rd; /* selected screen corners  */
//extern	int	diskvideo;			/* for disk-video klooges */
/*extern */int	bitshift;			/* bit shift for fudge */
/*extern */long	fudge;				/* fudge factor (2**n) */
extern int	colors; 			/* maximum colors available */
//extern int display3d;

/*extern */float *ifs_defn;
/*extern */int ifs_type;
/*extern */int  ifs_changed;
//*extern */char IFSFileName[80];
extern char lsys_type[];
extern char	lsys_Label[];			// for display in type selection

int	IFS_ptr = 0;
int	IFS_num;
extern	int	lsys_num;
extern	double	mandel_width;			// width of display
extern	double	hor, vert, ScreenRatio;

//extern	void   (_fastcall *standardplot)(int,int,int);
extern	void	fastline(const WORD, const WORD, const WORD, const WORD, const DWORD);

static int t;
static long /*l_dx,l_dy,l_dz,l_dt,l_a,l_b,l_c,*/l_d;

static double dx,dy,dz,dt,a,b,c,d;
static double adt,bdt,cdt,xdt,ydt,zdt;
static double initx,inity,initz;
static double initorbit[3];
//extern int inside_colour;

extern int  calc_status, resuming;
//extern int  diskisactive;
extern char savename[];
extern	DWORD	BackgroundColour;	// set background colour for IFS and L-System fractals

/* these are potential user parameters */
int WFconnect = 1;    /* flag to connect points with a line */
int waste = 100;    /* waste this many points before plotting */
int ProjectionPlane = 2; /* projection plane - default is to plot x-y */

double sinx, cosx;
double siny, cosy;

extern	int	user_data(HWND);
//extern	void	identity(MATRIXPTR);
extern	int	find_file_item(HWND, char *,char *,FILE **);

/******************************************************************/
/*		   zoom box conversion functions		  */
/******************************************************************/

static double determinant(double mat[3][3]) /* determinant of 3x3 matrix */
//double mat[3][3];
    {
   /* calculate determinant of 3x3 matrix */
    return(mat[0][0]*mat[1][1]*mat[2][2] +
	  mat[0][2]*mat[1][0]*mat[2][1] +
	  mat[0][1]*mat[1][2]*mat[2][0] -
	  mat[2][0]*mat[1][1]*mat[0][2] -
	  mat[1][0]*mat[0][1]*mat[2][2] -
	  mat[0][0]*mat[1][2]*mat[2][1]);
    }

static int solve3x3(double mat[3][3], double vec[3], double ans[3]) /* solve 3x3 inhomogeneous linear equations */
//double mat[3][3], vec[3], ans[3];
{
   /* solve 3x3 linear equation [mat][ans] = [vec] */
   double denom;
   double tmp[3][3];
   int i;
   denom = determinant(mat);
   if(fabs(denom) < DBL_EPSILON) /* test if can solve */
     return(-1);
   memcpy(tmp,mat,sizeof(double)*9);
   for(i=0;i<3;i++)
   {
      tmp[0][i] = vec[0];
      tmp[1][i] = vec[1];
      tmp[2][i] = vec[2];
      ans[i]  =  determinant(tmp)/denom;
      tmp[0][i] = mat[0][i];
      tmp[1][i] = mat[1][i];
      tmp[2][i] = mat[2][i];
    }
    return(0);
}

int setup_convert_to_screen(struct affine *scrn_cnvt)
{
   /* we do this twice - rather than having six equations with six unknowns,
      everything partitions to two sets of three equations with three
      unknowns. Nice, because who wants to calculate a 6x6 determinant??
    */
   double mat[3][3];
   double vec[3];
   /*
      first these equations - solve for a,b,e
      a*xxmin+b*yymax+e == 0	    (upper left)
      a*xx3rd+b*yy3rd+e == 0	    (lower left)
      a*xxmax+b*yymin+e == xdots-1  (lower right)
   */

   xxmin = hor;								// these ones are needed for lorenz type fractals
   xxmax = hor + mandel_width * ScreenRatio;
   yymin = vert;
   yymax = vert + mandel_width;



   xx3rd = xxmin;
   yy3rd = yymin;




   
   mat[0][0] = xxmin;
   mat[0][1] = yymax;
   mat[0][2] = 1.0;
   mat[1][0] = xx3rd;
   mat[1][1] = yy3rd;
   mat[1][2] = 1.0;
   mat[2][0] = xxmax;
   mat[2][1] = yymin;
   mat[2][2] = 1.0;
   vec[0]    = 0.0;
   vec[1]    = 0.0;
   vec[2]    = (double)(xdots-1);

   if(solve3x3(mat,vec, &(scrn_cnvt->a)))
      return(-1);
   /*
      now solve these:
      c*xxmin+d*yymax+f == 0
      c*xx3rd+d*yy3rd+f == ydots-1
      c*xxmax+d*yymin+f == ydots-1
      (mat[][] has not changed - only vec[])
   */
   vec[0]    = 0.0;
   vec[1]    = (double)(ydots-1);
   vec[2]    = (double)(ydots-1);

   if(solve3x3(mat,vec, &scrn_cnvt->c))
      return(-1);
   return(0);
}

/*
static int l_setup_convert_to_screen(struct l_affine *l_cvt)
{
   struct affine cvt;

   // MCP 7-7-91, This function should return a something!
   if(setup_convert_to_screen(&cvt))
      return(-1);
   l_cvt->a = cvt.a*fudge; l_cvt->b = cvt.b*fudge; l_cvt->c = cvt.c*fudge;
   l_cvt->d = cvt.d*fudge; l_cvt->e = cvt.e*fudge; l_cvt->f = cvt.f*fudge;

   // MCP 7-7-91
   return(0);
}
*/

/******************************************************************/
/*   setup functions - put in fractalspecific[fractype].per_image */
/******************************************************************/

static double orbit;
//static long   l_orbit;

//extern	double	HenonA, HenonXStart, HenonYStart;
extern	double	HenonA, HenonStep;

int orbit3dfloatsetup()
{



//type = FPGINGERBREAD;



   WFconnect = 1;
   waste = 100;
   ProjectionPlane = 2;

   if(type==FPHENON || type==FPPICKOVER || type==FPGINGERBREAD || type == LATOO || type == KAMFP || type == KAM3DFP || type == FPHOPALONG)
      WFconnect=0;
   if (type == FPLORENZ3D || type==FPLORENZ3D1 || type==FPLORENZ3D3 || type==FPLORENZ3D4)
      waste = 750;
   if (type==FPROSSLER)
      waste = 500;
   if (type==FPLORENZ)
      ProjectionPlane = 1; /* plot x and z */

   initorbit[0] = 1;  /* initial conditions */
   initorbit[1] = 1;
   initorbit[2] = 1;
   if (type==FPGINGERBREAD)
   {
      initorbit[0] = param[0];	/* initial conditions */
      initorbit[1] = param[1];
//      initorbit[0] = -0.1;	/* initial conditions */
//      initorbit[1] = 0.0;
   }
   if(type==FPHENON || type==FPPICKOVER)
   {
      a =  param[0];
      b =  param[1];
      c =  param[2];
      d =  param[3];
//      a =  2.24;
//      b =  0.43;
//      c =  -0.65;
//      d =  -2.43;
   }
   else if(type==KAMFP || type==KAM3DFP)
   {
//       a = HenonA;
//       a = 1.3;
//       b = mandel_width / 50.0;
//       b = HenonStep;
//       b = 0.05;
//       c = 5.5;
//       t = l_d = (threshold > 3200) ? 32000 : threshold * 10;
//       t = l_d = threshold;

      a = param[0];	      // angle
      if(param[1] <= 0.0)
	 param[1] = .01;
      b =  param[1];	// stepsize
      c =  param[2];	// stop
      t = l_d =  (long)param[3];     // points per orbit

      sinx = sin(a);
      cosx = cos(a);
      orbit = 0;
      initorbit[0] = initorbit[1] = initorbit[2] = 0;
   } else if(type==FPHOPALONG || type==FPMARTIN)

   {
      initorbit[0] = 0;  /* initial conditions */
      initorbit[1] = 0;
      initorbit[2] = 0;
      WFconnect = 0;

      a =  param[0];
      b =  param[1];
      c =  param[2];
      d =  param[3];

//      a =  (type == FPHOPALONG) ? 0.4 : 3.14;
//      b =  1.0;
//      c =  0.0;
//      d =  0.0;
   } else
   {
      dt = param[0];
      a =  param[1];
      b =  param[2];
      c =  param[3];
   }

   /* precalculations for speed */
   adt = a*dt;
   bdt = b*dt;
   cdt = c*dt;

   return(1);
}

int lorenz3d1floatorbit(double *x, double *y, double *z)
{
      double norm;

      xdt = (*x)*dt;
      ydt = (*y)*dt;
      zdt = (*z)*dt;

      /* 1-lobe Lorenz */
      norm = sqrt((*x)*(*x)+(*y)*(*y));
      dx   = (-adt-dt)*(*x) + (adt-bdt)*(*y) + (dt-adt)*norm + ydt*(*z);
      dy   = (bdt-adt)*(*x) - (adt+dt)*(*y) + (bdt+adt)*norm - xdt*(*z) -
	     norm*zdt;
      dz   = (ydt/2) - cdt*(*z);

      *x += dx;
      *y += dy;
      *z += dz;
      return(0);
}

int lorenz3dfloatorbit(double *x, double *y, double *z)
{
      xdt = (*x)*dt;
      ydt = (*y)*dt;
      zdt = (*z)*dt;

      /* 2-lobe Lorenz (the original) */
      dx  = -adt*(*x) + adt*(*y);
      dy  =  bdt*(*x) - ydt - (*z)*xdt;
      dz  = -cdt*(*z) + (*x)*ydt;

      *x += dx;
      *y += dy;
      *z += dz;
      return(0);
}

int lorenz3d3floatorbit(double *x, double *y, double *z)
{
      double norm;

      xdt = (*x)*dt;
      ydt = (*y)*dt;
      zdt = (*z)*dt;

      /* 3-lobe Lorenz */
      norm = sqrt((*x)*(*x)+(*y)*(*y));
      dx   = (-(adt+dt)*(*x) + (adt-bdt+zdt)*(*y)) / 3 +
	     ((dt-adt)*((*x)*(*x)-(*y)*(*y)) +
	     2*(bdt+adt-zdt)*(*x)*(*y))/(3*norm);
      dy   = ((bdt-adt-zdt)*(*x) - (adt+dt)*(*y)) / 3 +
	     (2*(adt-dt)*(*x)*(*y) +
	     (bdt+adt-zdt)*((*x)*(*x)-(*y)*(*y)))/(3*norm);
      dz   = (3*xdt*(*x)*(*y)-ydt*(*y)*(*y))/2 - cdt*(*z);

      *x += dx;
      *y += dy;
      *z += dz;
      return(0);
}

int lorenz3d4floatorbit(double *x, double *y, double *z)
{
      xdt = (*x)*dt;
      ydt = (*y)*dt;
      zdt = (*z)*dt;

      /* 4-lobe Lorenz */
      dx   = (-adt*(*x)*(*x)*(*x) + (2*adt+bdt-zdt)*(*x)*(*x)*(*y) +
	     (adt-2*dt)*(*x)*(*y)*(*y) + (zdt-bdt)*(*y)*(*y)*(*y)) /
	     (2 * ((*x)*(*x)+(*y)*(*y)));
      dy   = ((bdt-zdt)*(*x)*(*x)*(*x) + (adt-2*dt)*(*x)*(*x)*(*y) +
	     (-2*adt-bdt+zdt)*(*x)*(*y)*(*y) - adt*(*y)*(*y)*(*y)) /
	     (2 * ((*x)*(*x)+(*y)*(*y)));
      dz   = (2*xdt*(*x)*(*x)*(*y) - 2*xdt*(*y)*(*y)*(*y) - cdt*(*z));

      *x += dx;
      *y += dy;
      *z += dz;
      return(0);
}

int henonfloatorbit(double *x, double *y, double *z)
{
      double newx,newy;
      newx  = 1 + *y - a*(*x)*(*x);
      newy  = b*(*x);
      *x = newx;
      *y = newy;
      return(0);
}

int rosslerfloatorbit(double *x, double *y, double *z)
{
      xdt = (*x)*dt;
      ydt = (*y)*dt;

      dx = -ydt - (*z)*dt;
      dy = xdt + (*y)*adt;
      dz = bdt + (*z)*xdt - (*z)*cdt;

      *x += dx;
      *y += dy;
      *z += dz;
      return(0);
}

int pickoverfloatorbit(double *x, double *y, double *z)
{
      double newx,newy,newz;
      newx = sin(a*(*y)) - (*z)*cos(b*(*x));
      newy = (*z)*sin(c*(*x)) - cos(d*(*y));
      newz = sin(*x);
      *x = newx;
      *y = newy;
      *z = newz;
      return(0);
}
/* page 149 "Science of Fractal Images" */
int gingerbreadfloatorbit(double *x, double *y, double *z)
{
      double newx;
      newx = 1 - (*y) + fabs(*x);
      *y = *x;
      *x = newx;
      return(0);
}

int kamtorusfloatorbit(double *r, double *s, double *z)
{
   double srr;
   if(t++ >= l_d)
   {
      orbit += b;
      (*r) = (*s) = orbit/3;
      t = 0;
      *z = orbit;
      if(orbit > c)
	 return(1);
   }
   srr = (*s)-(*r)*(*r);
   (*s)=(*r)*sinx+srr*cosx;
   (*r)=(*r)*cosx-srr*sinx;
   return(0);
}

#define Sign(x) ((x)>=0?1:-1)			// note that this is a different definition to the sign defined in line3d.cpp, hence the upper case

int hopalong2dfloatorbit(double *x, double *y, double *z)
{
   double tmp;
   tmp = *y - Sign(*x)*sqrt(fabs(b*(*x)-c));
   *y = a - *x;
   *x = tmp;
   return(0);
}

int martin2dfloatorbit(double *x, double *y, double *z)
{
   double tmp;
   tmp = *y - sin(*x);
   *y = a - *x;
   *x = tmp;
   return(0);
}

int mandelcloudfloat(double *x, double *y, double *z)
{
    double newx,newy,x2,y2;
    x2 = (*x)*(*x);
    y2 = (*y)*(*y);
    if (x2+y2>2) return 1;
    newx = x2-y2+a;
    newy = 2*(*x)*(*y)+b;
    *x = newx;
    *y = newy;
    return(0);
}

/**************************************************************************
	Latoocarfian Fractal
**************************************************************************/


//  Select trig function

Complex	SelectTrig(Complex in, int TrigType)
    {
    switch (TrigType)
	{
	case 0:
	    return in.CSin();
	case 1:
	    return in.CCos();
	case 2:
	    return in.CExp();
	case 3:
	    return in.CSinh();
	case 4:
	    return in.CCosh();
	case 5:
	    return in.CTan();
	}
    return in.CSin();
    }

#define PAR_A   param[0]
#define PAR_B   param[1]
#define PAR_C   param[2]
#define PAR_D   param[3]

Complex	NewZ, OldZ;

int latoofloatorbit(double *x, double *y, double *z)
    {

    double xold, yold, tmp;

    xold = *z; /* for warning only */

    xold = *x;
    yold = *y;

    /*    *x = sin(yold * PAR_B) + PAR_C * sin(xold * PAR_B); */
    OldZ.x = yold * PAR_B;
    OldZ.y = 0;          /* old = (y * B) + 0i (in the complex)*/
//    CMPLXtrig0(OldZ, NewZ);
    NewZ = SelectTrig(OldZ, (int)param[4]);
    tmp = (double)NewZ.x;
    OldZ.x = xold * PAR_B;
    OldZ.y = 0;          /* old = (x * B) + 0i */
//    CMPLXtrig1(OldZ, NewZ);
    NewZ = SelectTrig(OldZ, (int)param[5]);
    *x = PAR_C * NewZ.x + tmp;

    /*    *y = sin(xold * PAR_A) + PAR_D * sin(yold * PAR_A); */
    OldZ.x = xold * PAR_A;
    OldZ.y = 0;          /* old = (y * A) + 0i (in the complex)*/
//    CMPLXtrig2(OldZ, NewZ);
    NewZ = SelectTrig(OldZ, (int)param[6]);
    tmp = (double)NewZ.x;
    OldZ.x = yold * PAR_A;
    OldZ.y = 0;          /* old = (x * B) + 0i */
//    CMPLXtrig3(OldZ, NewZ);
    NewZ = SelectTrig(OldZ, (int)param[7]);
    *y = PAR_D * NewZ.x + tmp;

    return(0);
    }

#undef PAR_A
#undef PAR_B
#undef PAR_C
#undef PAR_D

/**********************************************************************/
/*   Main fractal engines - put in fractalspecific[fractype].calctype */
/**********************************************************************/

int orbit2dfloat(void)
{
//   FILE *fp;
//   double *soundvar;
   double x,y,z;
   int color,col,row;
   int count;
   int oldrow, oldcol;
   double *p0,*p1,*p2;
   struct affine cvt;
   int ret;
   unsigned long maxct,ct;

   setup_convert_to_screen(&cvt);

   /* set up projection scheme */

   if(ProjectionPlane==0)
   {
      p0 = &z;
      p1 = &x;
      p2 = &y;
   }
   else if(ProjectionPlane==1)
   {
      p0 = &x;
      p1 = &z;
      p2 = &y;
   }
   else if(ProjectionPlane==2)
   {
      p0 = &x;
      p1 = &y;
      p2 = &z;
   }

   count = 0;
   if(TrueCol.inside_colour > 0)
      color = TrueCol.inside_colour;
   if(color >= colors)
      color = 1;
   oldcol = oldrow = -1;
   x = initorbit[0];
   y = initorbit[1];
   z = initorbit[2];

/*
   if (resuming)
   {
      start_resume();
      get_resume(sizeof(count),&count,sizeof(color),&color,
		 sizeof(oldrow),&oldrow,sizeof(oldcol),&oldcol,
		 sizeof(x),&x,sizeof(y),&y,sizeof(z),&z,
		 sizeof(t),&t,sizeof(orbit),&orbit,
		 0);
      end_resume();
   }
*/

   maxct = threshold * 400L;
   ct = 0L;
   ret = 0;
   while(ct++ < maxct) // loop until keypress or threshold
//   while(1)
   {
      if (user_data(GlobalHwnd) < 0)  // keypress bails out
//      if(check_key())
      {
/*
	     nosnd();
	     alloc_resume(100,1);
	     put_resume(sizeof(count),&count,sizeof(color),&color,
		    sizeof(oldrow),&oldrow,sizeof(oldcol),&oldcol,
		    sizeof(x),&x,sizeof(y),&y,sizeof(z),&z,
		    sizeof(t),&t,sizeof(orbit),&orbit,
		    0);
*/
	     ret = -1;
	     break;
      }

      if (++count > 1000)
      {        /* time to switch colors? */
	 count = 0;
	 if (++color >= colors)   /* another color to switch to? */
	      color = 1;	/* (don't use the background color) */
      }

      col = (int)(cvt.a*x + cvt.b*y + cvt.e);
      row = (int)(cvt.c*x + cvt.d*y + cvt.f);
      if ( col >= 0 && col < xdots && row >= 0 && row < ydots )
      {

	 if(oldcol != -1 && WFconnect)
	    fastline((WORD)col,(WORD)row,(WORD)oldcol,(WORD)oldrow,color&(colors-1));
	 else
	    Plot.PlotPoint((WORD)col,(WORD)row,(DWORD)(color&(colors-1)));
	 oldcol = col;
	 oldrow = row;
      }
      else
	 oldrow = oldcol = -1;
      if (type == FPLORENZ)
	{
	if (lorenz3dfloatorbit(p0, p1, p2))
	    break;
	}
      else if (type == FPMARTIN)
	{
	if (martin2dfloatorbit(p0, p1, p2))
	    break;
	}
      else if (type == FPHOPALONG)
	{
	if (hopalong2dfloatorbit(p0, p1, p2))
	    break;
	}
      else if (type == FPGINGERBREAD)
	{
	if (gingerbreadfloatorbit(p0, p1, p2))
	    break;
	}
      else if (type == KAMFP)
	  {
	  if (kamtorusfloatorbit(p0, p1, p2))
	      break;
	  }
      else if (type == LATOO)
	  {
	  if (latoofloatorbit(p0, p1, p2))
	      break;
	  }

//      if(/*curfractalspecific->*/orbitcalc(p0, p1, p2))
//	 break;
//      if(fp)
//	  fprintf(fp,orbitsave_format,*p0,*p1,0.0);
   }
//   if(fp)
//      fclose(fp);
   return(ret);
}

int orbit3dfloatcalc(void)
{
//   FILE *fp;
   unsigned count;
   int oldcol,oldrow;
   int oldcol1,oldrow1;
//   extern int init3d[];
   unsigned long maxct;
   int color;
   int ret;
   struct float3dvtinf inf;

   /* setup affine screen coord conversion */
   setup_convert_to_screen(&inf.cvt);

   oldcol = oldrow = -1;
   oldcol1 = oldrow1 = -1;
   color = 2;
   if(color >= colors)
      color = 1;
   inf.orbit[0] = initorbit[0];
   inf.orbit[1] = initorbit[1];
   inf.orbit[2] = initorbit[2];

/*
   if(diskvideo)		// this would KILL a disk drive!
   {
      notdiskmsg();
      return(-1);
   }

   fp = open_orbitsave();
*/
   maxct = threshold*40L;
   count = inf.ct = 0L;
   ret = 0;
   while(inf.ct++ < (long)maxct) /* loop until keypress or threshold */
   {
      /* calc goes here */
      if (++count > 1000)
      {        /* time to switch colors? */
	 count = 0;
	 if (++color >= colors)   /* another color to switch to? */
	    color = 1;	      /* (don't use the background color) */
      }

      if (user_data(GlobalHwnd) < 0)  // keypress bails out
//      if(check_key())
      {
//	 nosnd();
	 ret = -1;
	 break;
      }

      if (type == FPROSSLER)
	{
	rosslerfloatorbit(&inf.orbit[0],&inf.orbit[1],&inf.orbit[2]);
//	    break;
	}
      else if (type == FPPICKOVER)
	pickoverfloatorbit(&inf.orbit[0],&inf.orbit[1],&inf.orbit[2]);
      else if (type == FPLORENZ3D)
	lorenz3dfloatorbit(&inf.orbit[0],&inf.orbit[1],&inf.orbit[2]);
      else if (type == FPLORENZ3D1)
	lorenz3d1floatorbit(&inf.orbit[0],&inf.orbit[1],&inf.orbit[2]);
      else if (type == FPLORENZ3D3)
	lorenz3d3floatorbit(&inf.orbit[0],&inf.orbit[1],&inf.orbit[2]);
      else if (type == FPLORENZ3D4)
	lorenz3d4floatorbit(&inf.orbit[0],&inf.orbit[1],&inf.orbit[2]);
      else if (type == KAM3DFP)
	kamtorusfloatorbit(&inf.orbit[0],&inf.orbit[1],&inf.orbit[2]);

      if (float3dviewtransf(&inf))
      {
	 /* plot if inside window */
	 if (inf.col >= 0)
	 {
//	    if(realtime)
//	       whichimage=1;
	    if(oldcol != -1 && WFconnect)
	       fastline((WORD)inf.col,(WORD)inf.row,(WORD)oldcol,(WORD)oldrow,color&(colors-1));
	    else
		Plot.PlotPoint((WORD)inf.col,(WORD)inf.row,color&(colors-1));
	 }
	 oldcol = inf.col;
	 oldrow = inf.row;
      }
   }
//   if(fp)
//      fclose(fp);
   return(ret);
}

int euler = 0;	    /* use implicit euler approximation for dynamic system */

int dynam2dfloatsetup()
{
   WFconnect = 0;
   euler = 0;
   d = param[0]; /* number of intervals */
//   d = threshold;
   if (d<0) {
      d = -d;
      WFconnect = 1;
   } 
   else if (d==0) {
      d = 1;
   }

   if (type==TEST) {
//   if (type==DYNAMICFP) {
       a = param[2]; // parameter
       b = param[3]; // parameter
       dt = param[1]; // step size
       if (dt<0) {
	  dt = -dt;
	  euler = 1;
       }
       if (dt==0) dt = 0.01;
   }
   return(1);
}

/*
 * This is the routine called to perform a time-discrete dynamical
 * system image.
 * The starting positions are taken by stepping across the image in steps
 * of parameter1 pixels.  maxit differential equation steps are taken, with
 * a step size of parameter2.
 */

#define BAD_PIXEL 10000L

int dynam2dfloat()
{
//   FILE *fp;
//   double *soundvar;
   double x,y/*,z*/;
   int color,col,row;
   int count;
   int oldrow, oldcol;
   double *p0,*p1;
   struct affine cvt;
   int ret;
   int xstep, ystep; /* The starting position step number */
   double xpixel, ypixel; /* Our pixel position on the screen */
   double delxx, delyy, delxx2, delyy2;

//   fp = open_orbitsave();
   /* setup affine screen coord conversion */
   setup_convert_to_screen(&cvt);

   p0 = &x;
   p1 = &y;

   count = 0;
   if(TrueCol.inside_colour > 0)
      color = TrueCol.inside_colour;
   if(color >= colors)
      color = 1;
   oldcol = oldrow = -1;

    xstep = -1;
    ystep = 0;

    ret = 0;
    while(1)
	{
	if (user_data(GlobalHwnd) < 0)  // keypress bails out
	    {
	    ret = -1;
	    break;
	    }

      xstep ++;
      if (xstep>=d) {
	  xstep = 0;
	  ystep ++;
	  if (ystep>d) {
//	      nosnd();
	      ret = -1;
	      break;
	  }
      }

      delxx = (xxmax - xxmin) / (xdots-1);
      delyy = (yymax - yymin) / (ydots-1);
      delxx2 = delyy2 = 0.0;
//      xpixel = dxsize*(xstep+.5)/d;
//      ypixel = dysize*(ystep+.5)/d;
      xpixel = (xdots - 1)*(xstep+.5)/d;
      ypixel = (ydots - 1)*(ystep+.5)/d;
      x = (xxmin+delxx*xpixel) + (delxx2*ypixel);
      y = (yymax-delyy*ypixel) + (-delyy2*xpixel);
      if (type==MANDELCLOUD) {
	  a = x;
	  b = y;
      }
      oldcol = -1;

      if (++color >= colors)   /* another color to switch to? */
	  color = 1;	/* (don't use the background color) */

      for (count=0;count<threshold;count++) {

	  col = (int)(cvt.a*x + cvt.b*y + cvt.e);
	  row = (int)(cvt.c*x + cvt.d*y + cvt.f);
//	  col = cvt.a*x + cvt.b*y + cvt.e;
//	  row = cvt.c*x + cvt.d*y + cvt.f;
	  if ( col >= 0 && col < xdots && row >= 0 && row < ydots )
	  {
//	     if (count>=orbit_delay) {
		 if(oldcol != -1 && WFconnect)
		    fastline((WORD)col,(WORD)row,(WORD)oldcol,(WORD)oldrow,color&(colors-1));
		 else if(count > 0 || type != MANDELCLOUD)
		     Plot.PlotPoint((WORD)col,(WORD)row,color&(colors-1));
/*
		    draw_line(col,row,oldcol,oldrow,color&(colors-1));
		 else if(count > 0 || type != MANDELCLOUD)
		    (*plot)(col,row,color&(colors-1));
*/
//	     }
	     oldcol = col;
	     oldrow = row;
	  }
	  else if((long)abs(row) + (long)abs(col) > BAD_PIXEL) /* sanity check */
            return(ret);
	  else
	     oldrow = oldcol = -1;

          if (type == MANDELCLOUD)
		{
		if (mandelcloudfloat(p0, p1, NULL))
		    break;
		}
//	  if(curfractalspecific->orbitcalc(p0, p1, NULL))
//	     break;
//	  if(fp)
//	      fprintf(fp,orbitsave_format,*p0,*p1,0.0);
	}
   }
//   if(fp)
///      fclose(fp);
   return(ret);
}

#ifdef DOLATER
/* this function's only purpose is to manage funnyglasses related */
/* stuff so the code is not duplicated for ifs3d() and lorenz3d() */
int funny_glasses_call(int (*calc)())
{
   int status;
   status = 0;
   if(glassestype)
      whichimage = 1;
   else
      whichimage = 0;
   plot_setup();
   plot = standardplot;
   status = calc();
   if(realtime && glassestype != 3)
   {
      realtime = 0;
      return(status);
   }
   if(glassestype && status == 0 && display3d)
   {
      if(glassestype==3) /* photographer's mode */
	 if(active_system == 0) { /* dos version */
	    int i;
static char far firstready[]={"\
First image (left eye) is ready.  Hit any key to see it,\n\
then hit <s> to save, hit any other key to create second image."};
	    stopmsg(16,firstready);
	    while ((i = getakey()) == 's' || i == 'S') {
	       diskisactive = 1;
	       savetodisk(savename);
	       diskisactive = 0;
	       }
	    /* is there a better way to clear the screen in graphics mode? */
	    setvideomode(videoentry.videomodeax,
		videoentry.videomodebx,
		videoentry.videomodecx,
		videoentry.videomodedx);
	 }
	 else { 		  /* Windows version */
static char far firstready2[]={"First (Left Eye) image is complete"};
	    stopmsg(0,firstready2);
	    clear_screen();
	    }
//      whichimage = 2;
      plot_setup();
      plot = standardplot;
      /* is there a better way to clear the graphics screen ? */
      if(status = calc())
	 return(status);
      if(glassestype==3) /* photographer's mode */
	 if(active_system == 0) { /* dos version */
static char far secondready[]={"Second image (right eye) is ready"};
	    stopmsg(16,secondready);
	 }
   }
   return(status);
}
#endif

/* double version - mainly for testing */
static int ifs3dfloat(HWND hwnd)
{
//   FILE *fp;
   unsigned long maxct;
   int color;

   double newx,newy,newz,r,sum;

   int k;
   int ret;

   struct float3dvtinf inf;

   float far *ffptr;

   /* setup affine screen coord conversion */
   setup_convert_to_screen(&inf.cvt);
   srand(1);

   inf.orbit[0] = 0;
   inf.orbit[1] = 0;
   inf.orbit[2] = 0;

//   fp = open_orbitsave();

   maxct = threshold*40L;
   inf.ct = 0L;
   ret = 0;
   while(inf.ct++ < (long)maxct) /* loop until keypress or threshold */
   {
      if (user_data(hwnd) < 0)
//      if( check_key() )  /* keypress bails out */
      {
	 ret = -1;
	 break;
      }
      r = rand();	 /* generate fudged random number between 0 and 1 */
      r /= 32767;

      /* pick which iterated function to execute, weighted by probability */
      if (ifs_defn == NULL)
	  return -1;
      sum = ifs_defn[12]; /* [0][12] */
      k = 0;
      while ( sum < r)
      {
	 k++;
	 sum += ifs_defn[k*IFS3DPARM+12];
	 if (ifs_defn[(k+1)*IFS3DPARM+12] == 0) break; /* for safety */
      }

      /* calculate image of last point under selected iterated function */
      ffptr = ifs_defn + k*IFS3DPARM; /* point to first parm in row */
      newx = *ffptr * inf.orbit[0] +
	     *(ffptr+1) * inf.orbit[1] +
	     *(ffptr+2) * inf.orbit[2] + *(ffptr+9);
      newy = *(ffptr+3) * inf.orbit[0] +
	     *(ffptr+4) * inf.orbit[1] +
	     *(ffptr+5) * inf.orbit[2] + *(ffptr+10);
      newz = *(ffptr+6) * inf.orbit[0] +
	     *(ffptr+7) * inf.orbit[1] +
	     *(ffptr+8) * inf.orbit[2] + *(ffptr+11);

      inf.orbit[0] = newx;
      inf.orbit[1] = newy;
      inf.orbit[2] = newz;
//      if(fp)
//	  fprintf(fp,orbitsave_format,newx,newy,newz);
      if (float3dviewtransf(&inf))
      {
	 /* plot if inside window */
	 if (inf.col >= 0)
	 {
//	    if(realtime)
//	       whichimage=1;
	    color = Plot.GetColour((WORD)inf.col,(WORD)inf.row)+1;
	    if( color < colors ) /* color sticks on last value */
		Plot.PlotPoint((WORD)inf.col,(WORD)inf.row,color);
	 }
      }
   } /* end while */
//   if(fp)
//      fclose(fp);
   return(ret);
}

int ifs(HWND hwnd)			// front-end for ifs2d and ifs3d
{
//   if (ifs_defn == NULL && ifsload() < 0)
//	return(-1);
//   if (diskvideo) {		 // this would KILL a disk drive!
//      notdiskmsg();
//      return(-1);
//      }
   Dib.ClearDib(BackgroundColour);			// set background colour
   return((ifs_type == 0) ? ifs2d(hwnd) : ifs3d(hwnd));
}


int ifs2d(HWND hwnd)	// IFS logic shamelessly converted to integer math
{
//   FILE *fp;
   unsigned long maxct,ct;
   int col;
   int row;
   int color;
   int ret;
   float *ffptr;

//   long localifs[NUMIFS][7];	    // local IFS values
   float x,y,newx,newy,r,sum/*, tempr*/;

   int /*i,j,*/k;
   struct affine cvt;

   // setup affine screen coord conversion
   setup_convert_to_screen(&cvt);
   srand(1);

   /*
   for (i = 0; i < NUMIFS; i++)    // fill in the local IFS array
      for (j = 0; j < IFSPARM; j++)
	 localifs[i][j] = ifs_defn[i*IFSPARM+j] * fudge;

   tempr = fudge / 32767;	 // find the proper rand() fudge

   fp = open_orbitsave();
*/
   // make maxct a function of screen size
   // 1k times threshold at EGA resolution seems about right
//   maxct = (float)threshold*(1024.0*xdots*ydots)/(640.0*350.0);
   maxct = threshold * 40L;
   ct = 0L;
   x = y = (float)0.0;
   ret = 0;
   while(ct++ < maxct) // loop until keypress or threshold
   {
//      if( check_key() )  // keypress bails out
      if (user_data(hwnd) < 0)  // keypress bails out
      {
	 ret = -1;
	 break;
      }
      r = (float)rand();	 // generate fudged random number between 0 and 1
      r /= (float)32767.0;

      // pick which iterated function to execute, weighted by probability
      if (ifs_defn == NULL)
	  return -1;
      sum = ifs_defn[6]; /* [0][6] */
      k = 0;
      while (sum < r)
      {
	 k++;
	 sum += ifs_defn[k*IFSPARM+6];
	 if (ifs_defn[(k+1)*IFSPARM+6] == 0) break; /* for safety */
      }

      /* calculate image of last point under selected iterated function */
      ffptr = ifs_defn + k*IFSPARM; /* point to first parm in row */
      newx = *ffptr * x + *(ffptr+1) * y + *(ffptr+4);
      newy = *(ffptr+2) * x + *(ffptr+3) * y + *(ffptr+5);
      x = newx;
      y = newy;
      // plot if inside window
      col = (int)(cvt.a * x + cvt.b * y + cvt.e);
      row = (int)(cvt.c * x + cvt.d * y + cvt.f);
      if (col >= 0 && col < xdots && row >= 0 && row < ydots)
      {
	 // color is count of hits on this pixel
	 color = Plot.GetColour((WORD)col,(WORD)row) + 1;
	 if (color < colors) // color sticks on last value
	     Plot.PlotPoint((WORD)col,(WORD)row,color);
      }
   }
   return(ret);
}

static void setupmatrix(MATRIX doublemat)
    {
   /* build transformation matrix */
   CMatrix  Mat;

   Mat.identity (doublemat);

   /* apply rotations - uses the same rotation variables as line3d.c */
    Mat.xrot ((double)XROT / 57.29577,doublemat);
    Mat.yrot ((double)YROT / 57.29577,doublemat);
    Mat.zrot ((double)ZROT / 57.29577,doublemat);

    // apply scale 
//    scale((double)XSCALE/100.0,(double)YSCALE/100.0,(double)ROUGH/100.0,doublemat);
    }


int ifs3d(HWND hwnd)
    {
    return(ifs3dfloat(hwnd)); /* double version of ifs3d */
    }

static int float3dviewtransf(struct float3dvtinf *inf)
    {
    int i;
    double tmpx, tmpy, tmpz;
    double tmp;
    CMatrix  Mat;

    if (inf->ct == 1)	/* initialize on first call */
	{
	for(i=0;i<3;i++)
	    {
	    inf->minvals[i] =  100000.0; /* impossible value */
	    inf->maxvals[i] = -100000.0;
	    }
	setupmatrix(inf->doublemat);
	}

   // 3D VIEWING TRANSFORM
    Mat.vmult(inf->orbit,inf->doublemat,inf->viewvect);
    if(inf->ct <= waste) // waste this many points to find minz and maxz
	{
      // find minz and maxz
	for(i=0;i<3;i++)
	    if ((tmp = inf->viewvect[i]) < inf->minvals[i])
		inf->minvals[i] = tmp;
	    else if (tmp > inf->maxvals[i])
		inf->maxvals[i] = tmp;
	if(inf->ct == waste)		// time to work it out
	    {
	    view[0] = view[1] = 0;	// center on origin
	 // z value of user's eye - should be more negative than extreme negative part of image
//	 view[2] = (inf->minvals[2]-inf->maxvals[2])*(double)ZVIEWER/100.0;

	 /* center image on origin */
	    tmpx = (-inf->minvals[0]-inf->maxvals[0])/(2.0); /* center x */
	    tmpy = (-inf->minvals[1]-inf->maxvals[1])/(2.0); /* center y */

	 /* apply perspective shift */
	    tmpx += ((double)xshift*(xxmax-xxmin))/(xdots);
	    tmpy += ((double)yshift*(yymax-yymin))/(ydots);
	    tmpz = -(inf->maxvals[2]);
	    Mat.trans(tmpx,tmpy,tmpz,inf->doublemat);
	    }
	return(0);
	}

   /* inf->ct > waste */
   /* apply perspective if requested */
    inf->row = (int)(inf->cvt.c*inf->viewvect[0] + inf->cvt.d*inf->viewvect[1] + inf->cvt.f + yyadjust);
    inf->col = (int)(inf->cvt.a*inf->viewvect[0] + inf->cvt.b*inf->viewvect[1] + inf->cvt.e + xxadjust);
    if (inf->col < 0 || inf->col >= xdots || inf->row < 0 || inf->row >= ydots)
	inf->col = inf->row = -1;
    return(1);
    }

/* --------------------------------------------------------------------- */
/* read in IFS parameters						 */
/* --------------------------------------------------------------------- */

int ifsload(HWND hwnd, char *filename)
    {
    int		i;
    FILE	*ifsfile;
    char	buf[201];
    char	*bufptr;
    char	*token = NULL;
    char	*next_token = NULL;
    char	seps[]   = " \t\n";
    char	msg[120];
    float	suffix[4096];
    int		ret,rowsize;

    if (ifs_defn) 
	{				// release prior parms 
	delete [] ifs_defn;
	ifs_defn = NULL;
	}

    ifs_changed = ifs_type = 0;
    rowsize = IFSPARM;
    type = IFS;
    if (find_file_item(hwnd, filename,lsys_type,&ifsfile) < 0)
	return(-1);

    fgets(buf,200,ifsfile);
    strlwr(buf);
    bufptr = &buf[0];
    while (*bufptr) 
	{
	if (_strnicmp(bufptr,"(3d)",4) == 0) 
	    {
	    ifs_type = 1;
	    rowsize = IFS3DPARM;
	    }
	if (*bufptr == ';')		// trap comments
	    *bufptr = '\0';
	++bufptr;
	}

    for (i = 0; i < (NUMIFS+1)*IFS3DPARM; ++i)
	suffix[i] = (float)0.0;
    i = ret = 0;
    bufptr = &buf[0];
    while(fgets(buf, 200, ifsfile))
	{
	if (str_find_ci(buf, "}") != 0)
	    break;			// we're done
	while (*bufptr) 
	    {
	    if (*bufptr == ';')		// trap comments
		{
		*bufptr = '\0';
		break;
		}
	    ++bufptr;
	    }
	token = strtok_s(buf, seps, &next_token);
	while (token != NULL)
	    {
	    sscanf(token,"%f",&suffix[i]);
	    if (++i >= NUMIFS*rowsize) 
		{
		sprintf(msg,"IFS definition has too many lines = %d", i);
		MessageBox (hwnd, msg, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
		ret = -1;
		break;
		}
	    token = strtok_s(NULL, seps, &next_token);
	    }
	}

    if ((i % rowsize) != 0) 
	{
	sprintf(msg,"invalid IFS definition rowsize = %d, params = %d", rowsize, i);
	MessageBox (hwnd, msg, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	ret = -2;
	}
    if (i == 0 && ret == 0) 
	{
	static char far msg[]={"Empty IFS definition"};
	MessageBox (hwnd, msg, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	ret = -3;
	}
    fclose(ifsfile);
    if (ret == 0)
//	if ((ifs_defn = (float far *)GlobalAlloc(GMEM_FIXED, (long)((NUMIFS+1)*IFS3DPARM*sizeof(float)))) == NULL) 
	if ((ifs_defn = new float [(NUMIFS+1)*IFS3DPARM]) == NULL) 
	    {
	    static char far msg[]={"Insufficient memory for IFS"};
	    MessageBox (hwnd, msg, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	    ret = -4;
	    }
	else
	    for (i = 0; i < (NUMIFS+1)*IFS3DPARM; ++i)
		ifs_defn[i] = suffix[i];

    return(ret);
    }

int find_file_item(HWND hwnd, char *filename,char *itemname,FILE **infile)

    {
    char	tmpname[41];
    long	notepoint;
    char	buf[201];
    int	c;

    if ((*infile = fopen(filename,"rt")) == NULL) 
	{
	wsprintf(buf, "Can't Open File: <%s>", filename);
	MessageBox (hwnd, buf, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return(-1);
	}

    while (TRUE) 
	{
	while ((c = getc(*infile)) == ' ' || c == '\t' || c == '\n') 
	    { }
	if (c == EOF)
	    break;
	if (c == ';')
	    {
	    while ((c = fgetc(*infile)) != '\n' && c != EOF)
		{ }
	    if (c == EOF)
		break;
	    continue;
	    }
	notepoint = ftell(*infile) - 1;
	ungetc(c,*infile);
	if (fscanf(*infile," %40[^ \n\t({]",tmpname) == EOF)
	    break;
	while ((c = getc(*infile)) != EOF && c != '{' && c != '\n')
	    { }
	if (c == EOF)
	    break;
	if (c == '{')
	    {
	    if (stricmp(tmpname,itemname) == 0)
		{
		fseek(*infile,notepoint,SEEK_SET);
		return(0);
		}
	    while ((c = getc(*infile)) != '}' && c != EOF)
		{ }
	    if (c == EOF)
		break;
	    }
	}
    fclose(*infile);
    sprintf(buf,"'%s' definition not found",itemname);
    MessageBox (hwnd, buf, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
    return(-1);
    }

/*
  Read an IFS file, picking off the IFS names.
*/

    int get_IFS_names(HWND hwnd, char *IFSFileName)	 /* get the IFS fractal names */

    {
    int i;
    FILE *File;
    char msg[81], tempstring[201];

    lsys_type[0] = 0;		/* start by declaring failure */
    for (i = 0; i < MaxIFSNameChoices; i++) 
	{
	lptr[i][0] = 0;
	}

    if ((File = fopen(IFSFileName, "rt")) == NULL) 
	{
	sprintf(msg, "I Can't find %s", IFSFileName);
	MessageBox (hwnd, msg, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return(-1);
	}

    lsys_num = 0;
    while(fscanf(File, " %20[^ \n\t({]", lptr[lsys_num]) != EOF)
	{
	int c;

	while(c = getc(File)) 
	    {
	    if(c == EOF || c == '{' || c == '\n')
		break;
	    }
	if(c == EOF)
	    break;
	else if(c != '\n')
	    {
	    lsys_num++;
	    if (lsys_num >= MaxIFSNameChoices)
		break;
skipcomments:
	    if(fscanf(File, "%200[^}]", tempstring) == EOF) 
		break;
	    if (getc(File) != '}') 
		goto skipcomments;
	    if (stricmp(lptr[lsys_num - 1],"") == 0 || stricmp(lptr[lsys_num - 1],"comment") == 0)
		lsys_num--;
	    }
	}
    fclose(File);
    qsort((void *)lptr, (size_t)lsys_num, sizeof(lptr[0]), (int(*)(const void*, const void *))strcmp);
    strcpy(lsys_Label, "Iterated Function System");
    return(0);
    }
