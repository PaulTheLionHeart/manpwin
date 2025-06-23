  /* see Fractint.c for a description of the "include"  hierarchy */
//#include "port.h"
#include "prototyp.h"


/* start of string literals cleanup */

char s_real[] =		"real";
char s_imag[] =		"imag";
char s_atan[] =		"atan";
char s_sin [] =		"sin";
char s_sinh[] =		"sinh";
char s_cos [] =		"cos";
char s_cosh[] =		"cosh";
char s_sqr [] =		"sqr";
char s_log [] =		"log";
char s_exp [] =		"exp";
char s_abs [] =		"abs";
char s_conj [] =	"conj";
char s_fn1 [] =		"fn1";
char s_fn2 [] =		"fn2";
char s_fn3 [] =		"fn3";
char s_fn4 [] =		"fn4";
char s_flip [] =	"flip";
char s_floor [] =	"floor";
char s_ceil [] =	"ceil";
char s_trunc [] =	"trunc";
char s_round [] =	"round";
char s_tan [] =		"tan";
char s_tanh [] =	"tanh";
char s_cotan [] =	"cotan";
char s_cotanh [] =	"cotanh";
char s_cosxx [] =	"cosxx";
char s_srand [] =	"srand";
char s_recip [] =	"recip";
char s_ident [] =	"ident";
char s_asin [] =	"asin";
char s_asinh [] =	"asinh";
char s_acos [] =	"acos";
char s_acosh [] =	"acosh";
char s_atanh [] =	"atanh";
char s_cabs [] =	"cabs";
char s_sqrt [] =	"sqrt";
char s_ismand [] =	"ismand";

#define rad_to_deg(x) ((x)*(180.0/PI)) /* most people "think" in degrees */
#define deg_to_rad(x) ((x)*(PI/180.0))
/*
convert corners to center/mag
Rotation angles indicate how much the IMAGE has been rotated, not the
zoom box.  Same goes for the Skew angles
*/

#ifdef _MSC_VER
#pragma optimize( "", off )
#endif

void cvtcentermag(double *Xctr, double *Yctr, LDBL *Magnification, double *Xmagfactor, double *Rotation, double *Skew)
{
   double Width, Height;
   double a, b; /* bottom, left, diagonal */
   double a2, b2, c2; /* squares of above */
   double tmpx1, tmpx2, tmpy1, tmpy2, tmpa; /* temporary x, y, angle */
  
   /* simple normal case first */
   if (xx3rd == xxmin && yy3rd == yymin)
   { /* no rotation or skewing, but stretching is allowed */
      Width  = xxmax - xxmin;
      Height = yymax - yymin;
      *Xctr = (xxmin + xxmax)/2.0;
      *Yctr = (yymin + yymax)/2.0;
      *Magnification  = 2.0/Height;
      *Xmagfactor =  Height / (DEFAULTASPECT * Width);
      *Rotation = 0.0;
      *Skew = 0.0;
      }
   else
   {
      /* set up triangle ABC, having sides abc */
      /* side a = bottom, b = left, c = diagonal not containing (x3rd,y3rd) */
      tmpx1 = xxmax - xxmin;
      tmpy1 = yymax - yymin;
      c2 = tmpx1*tmpx1 + tmpy1*tmpy1;
   
      tmpx1 = xxmax - xx3rd;
      tmpy1 = yymin - yy3rd;
      a2 = tmpx1*tmpx1 + tmpy1*tmpy1;
      a = sqrt(a2);
      *Rotation = -rad_to_deg(atan2( tmpy1, tmpx1 )); /* negative for image rotation */
   
      tmpx2 = xxmin - xx3rd;
      tmpy2 = yymax - yy3rd;
      b2 = tmpx2*tmpx2 + tmpy2*tmpy2;
      b = sqrt(b2);
   
      tmpa = acos((a2+b2-c2)/(2*a*b)); /* save tmpa for later use */
      *Skew = 90.0 - rad_to_deg(tmpa);
   
      *Xctr = (xxmin + xxmax)*0.5;
      *Yctr = (yymin + yymax)*0.5;
   
      Height = b * sin(tmpa);
   
      *Magnification  = 2.0/Height; /* 1/(h/2) */
      *Xmagfactor = Height / (DEFAULTASPECT * a);
   
      /* if vector_a cross vector_b is negative */
      /* then adjust for left-hand coordinate system */
      if ( tmpx1*tmpy2 - tmpx2*tmpy1 < 0 && debugflag != 4010)
      {
         *Skew = -*Skew;
         *Xmagfactor = -*Xmagfactor;
         *Magnification = -*Magnification;
      }
   }
   /* just to make par file look nicer */
   if (*Magnification < 0)
   {
      *Magnification = -*Magnification;
      *Rotation += 180;
   }
#ifdef DEBUG
   {
      double txmin, txmax, tx3rd, tymin, tymax, ty3rd;
      double error;
      txmin = xxmin;
      txmax = xxmax;
      tx3rd = xx3rd;
      tymin = yymin;
      tymax = yymax;
      ty3rd = yy3rd;
      cvtcorners(*Xctr, *Yctr, *Magnification, *Xmagfactor, *Rotation, *Skew);
      error = sqr(txmin - xxmin) +
              sqr(txmax - xxmax) +
              sqr(tx3rd - xx3rd) +
              sqr(tymin - yymin) +
              sqr(tymax - yymax) +
              sqr(ty3rd - yy3rd);
//      if(error > .001)
//         showcornersdbl("cvtcentermag problem");
      xxmin = txmin;
      xxmax = txmax;
      xx3rd = tx3rd;
      yymin = tymin;
      yymax = tymax;
      yy3rd = ty3rd;         
   } 
#endif
   return;
}

/* convert center/mag to corners */
void cvtcorners(double Xctr, double Yctr, LDBL Magnification, double Xmagfactor, double Rotation, double Skew)
{
   double x, y;
   double h, w; /* half height, width */
   double tanskew, sinrot, cosrot;

   if (Xmagfactor == 0.0)
      Xmagfactor = 1.0;

   h = (double)(1/Magnification);
   w = h / (DEFAULTASPECT * Xmagfactor);

   if (Rotation == 0.0 && Skew == 0.0)
      { /* simple, faster case */
      xx3rd = xxmin = Xctr - w;
      xxmax = Xctr + w;
      yy3rd = yymin = Yctr - h;
      yymax = Yctr + h;
      return;
      }

   /* in unrotated, untranslated coordinate system */
   tanskew = tan(deg_to_rad(Skew));
   xxmin = -w + h*tanskew;
   xxmax =  w - h*tanskew;
   xx3rd = -w - h*tanskew;
   yymax = h;
   yy3rd = yymin = -h;

   /* rotate coord system and then translate it */
   Rotation = deg_to_rad(Rotation);
   sinrot = sin(Rotation);
   cosrot = cos(Rotation);

   /* top left */
   x = xxmin * cosrot + yymax *  sinrot;
   y = -xxmin * sinrot + yymax *  cosrot;
   xxmin = x + Xctr;
   yymax = y + Yctr;

   /* bottom right */
   x = xxmax * cosrot + yymin *  sinrot;
   y = -xxmax * sinrot + yymin *  cosrot;
   xxmax = x + Xctr;
   yymin = y + Yctr;

   /* bottom left */
   x = xx3rd * cosrot + yy3rd *  sinrot;
   y = -xx3rd * sinrot + yy3rd *  cosrot;
   xx3rd = x + Xctr;
   yy3rd = y + Yctr;

   return;
}

