  /* see Fractint.c for a description of the "include"  hierarchy */
//#include "port.h"
#include "prototyp.h"


/* start of string literals cleanup */
char s_atan[]    = "atan";
char s_iter[]    = "iter";
char s_real[]    = "real";
char s_mult[]     = "mult";
char s_sum[]     = "summ";
char s_imag[]    = "imag";
char s_zmag[]    = "zmag";
char s_bof60[]   = "bof60";
char s_bof61[]   = "bof61";
char s_maxiter[] =  "maxiter";
char s_epscross[] =  "epsiloncross";
char s_startrail[] =  "startrail";
char s_normal[] =  "normal";
char s_period[] = "period";
char s_fmod[] = "fmod";
char s_tdis[] = "tdis";
char s_or[]     = "or";
char s_and[]    = "and";
char s_mod[]    = "mod";
char s_16bit[] =            "16bit";
char s_387[] =              "387";
char s_3d[] =               "3d";
char s_3dmode[] =           "3dmode";
char s_adapter[] =          "adapter";
char s_afi[] =              "afi";
char s_ambient[] =          "ambient";
char s_askvideo[] =         "askvideo";
char s_aspectdrift[] =      "aspectdrift";
char s_attack[] =           "attack";
char s_atten[] =            "attenuate";
char s_autokey[] =          "autokey";
char s_autokeyname[] =      "autokeyname";
char s_background[] =       "background";
char s_bailout[] =          "bailout";
char s_bailoutest[] =       "bailoutest";
char s_batch[] =            "batch";
char s_beep[] =             "beep";
char s_biomorph[] =         "biomorph";
char s_biospalette[] =      "biospalette";
char s_brief[] =            "brief";
char s_bright[] =           "bright";
char s_centermag[] =        "center-mag";
char s_cga[] =              "cga";
char s_coarse[] =           "coarse";
char s_colorps[] =          "colorps";
char s_colors[] =           "colors";
char s_comment[] =          "comment";
char s_comport[] =          "comport";
char s_converge[] =         "converge";
char s_corners[] =          "corners";
char s_cr[] =               "cr";
char s_crlf[] =             "crlf";
char s_crop[] =             "crop";
char s_cyclelimit[] =       "cyclelimit";
char s_cyclerange[] =       "cyclerange";
char s_curdir[] =           "curdir";
char s_debug[] =            "debug";
char s_debugflag[] =        "debugflag";
char s_decay[] =            "decay";
char s_decomp[] =           "decomp";
char s_distest[] =          "distest";
char s_dither[] =           "dither";
char s_ega[] =              "ega";
char s_egamono[] =          "egamono";
char s_epsf[] =             "epsf";
char s_exitmode[] =         "exitmode";
char s_exitnoask[] =        "exitnoask";
char s_fastrestore[] =      "fastrestore";
char s_filename[] =         "filename";
char s_fillcolor[] =        "fillcolor";
char s_filltype[] =         "filltype";
char s_finattract[] =       "finattract";
char s_float[] =            "float";
char s_formulafile[] =      "formulafile";
char s_formulaname[] =      "formulaname";
char s_fpu[] =              "fpu";
char s_fract001prn[] =      "fract001.prn";
char s_fullcolor[] =        "fullcolor";
char s_function[] =         "function";
char s_gif87a[] =           "gif87a";
char s_halftone[] =         "halftone";
char s_haze[] =             "haze";
char s_hertz[] =            "hertz";
char s_hgc[] =              "hgc";
char s_high[] =             "high";
char s_ifs[] =              "ifs";
char s_ifs3d[] =            "ifs3d";
char s_ifsfile[] =          "ifsfile";
char s_initorbit[] =        "initorbit";
char s_inside[] =           "inside";
char s_interocular[] =      "interocular";
char s_invert[] =           "invert";
char s_iterincr[] =         "iterincr";
char s_julibrot3d[] =       "julibrot3d";
char s_julibroteyes[] =     "julibroteyes";
char s_julibrotfromto[] =   "julibrotfromto";
char s_latitude[] =         "latitude";
char s_lf[] =               "lf";
char s_lfile[] =            "lfile";
char s_lightname[] =        "lightname";
char s_lightsource[] =      "lightsource";
char s_linefeed[] =         "linefeed";
char s_lname[] =            "lname";
char s_logmap[] =           "logmap";
char s_logmode[] =          "logmode";
char s_longitude[] =        "longitude";
char s_low[] =              "low";
char s_makedoc[] =          "makedoc";
char s_makemig[] =          "makemig";
char s_makepar[] =          "makepar";
char s_manh[]    = "manh";
char s_manr[]    = "manr";
char s_map[] =              "map";
char s_maxcolorres[] =      "maxcolorres";
char s_mcga[] =             "mcga";
char s_mid[] =              "mid";
char s_miim[] =             "miim";
char s_nobof[] =            "nobof";
char s_mono[] =             "mono";
char s_none[] =             "none";
char s_noninterlaced[] =    "noninterlaced";
char s_off[] =              "off";
char s_olddemmcolors[] =    "olddemmcolors";
char s_orbitdelay[] =       "orbitdelay";
char s_orbitname[] =        "orbitname";
char s_orbitsave[] =        "orbitsave";
char s_orgfrmdir[] =        "orgfrmdir";
char s_origin[] =           "origin";
char s_outside[] =          "outside";
char s_overlay[] =          "overlay";
char s_overwrite[] =        "overwrite";
char s_params[] =           "params";
char s_parmfile[] =         "parmfile";
char s_passes[] =           "passes";
char s_periodicity[] =      "periodicity";
char s_perspective[] =      "perspective";
char s_pi[] =               "pi";
char s_pixel[] =            "pixel";
char s_pixelzoom[] =        "pixelzoom";
char s_play[] =             "play";
char s_plotstyle[] =        "plotstyle";
char s_polyphony[] =        "polyphony";
char s_potential[] =        "potential";
char s_preview[] =          "preview";
char s_printer[] =          "printer";
char s_printfile[] =        "printfile";
char s_prox[] =             "proximity";
char s_radius[] =           "radius";
char s_ramvideo[] =         "ramvideo";
char s_randomize[] =        "randomize";
char s_ranges[] =           "ranges";
char s_ray[] =              "ray";
char s_record[] =           "record";
char s_release[] =          "release";
char s_srelease[] =         "srelease";
char s_reset[] =            "reset";
char s_rleps[] =            "rleps";
char s_rotation[] =         "rotation";
char s_roughness[] =        "roughness";
char s_rseed[] =            "rseed";
char s_savename[] =         "savename";
char s_savetime[] =         "savetime";
char s_scalemap[] =         "scalemap";
char s_scalexyz[] =         "scalexyz";
char s_showbox[] =          "showbox";
char s_showdot[] =          "showdot";
char s_showorbit[] =        "showorbit";
char s_smoothing[] =        "smoothing";
char s_sound[] =            "sound";
char s_sphere[] =           "sphere";
char s_stereo[] =           "stereo";
char s_sustain[] =          "sustain";
char s_symmetry[] =         "symmetry";
char s_truecolor[] =        "truecolor";
char s_truemode[] =         "truemode";
char s_tempdir[] =          "tempdir";
char s_workdir[] =          "workdir";
char s_usegrayscale[] =     "usegrayscale";
char s_monitorwidth[] =     "monitorwidth";
char s_targa_overlay[] =    "targa_overlay";
char s_textcolors[] =       "textcolors";
char s_textsafe[] =         "textsafe";
char s_title[] =            "title";
char s_tplus[] =            "tplus";
char s_translate[] =        "translate";
char s_transparent[] =      "transparent";
char s_type[] =             "type";
char s_vesadetect[] =       "vesadetect";
char s_vga[] =              "vga";
char s_video[] =            "video";
char s_viewwindows[] =      "viewwindows";
char s_volume[] =           "volume";
char s_warn[] =             "warn";
char s_waterline[] =        "waterline";
char s_wavetype[]=          "wavetype";
char s_xaxis[] =            "xaxis";
char s_xyadjust[] =         "xyadjust";
char s_xyaxis[] =           "xyaxis";
char s_xyshift[] =          "xyshift";
char s_yaxis [] =           "yaxis";
char s_sin [] =             "sin";
char s_sinh [] =            "sinh";
char s_cos [] =             "cos";
char s_cosh [] =            "cosh";
char s_sqr [] =             "sqr";
char s_log [] =             "log";
char s_exp [] =             "exp";
char s_abs [] =             "abs";
char s_conj [] =            "conj";
char s_fn1 [] =             "fn1";
char s_fn2 [] =             "fn2";
char s_fn3 [] =             "fn3";
char s_fn4 [] =             "fn4";
char s_flip [] =            "flip";
char s_floor [] =           "floor";
char s_ceil [] =            "ceil";
char s_trunc [] =           "trunc";
char s_round [] =           "round";
char s_tan [] =             "tan";
char s_tanh [] =            "tanh";
char s_cotan [] =           "cotan";
char s_cotanh [] =          "cotanh";
char s_cosxx [] =           "cosxx";
char s_srand [] =           "srand";
char s_recip [] =           "recip";
char s_ident [] =           "ident";
char s_zero [] =            "zero";
char s_one  [] =            "one";
char s_asin [] =            "asin";
char s_asinh [] =           "asinh";
char s_acos [] =            "acos";
char s_acosh [] =           "acosh";
char s_atanh [] =           "atanh";
char s_cabs [] =            "cabs";
char s_sqrt [] =            "sqrt";
char s_ismand [] =          "ismand";






/*

  The following Complex function routines added by Tim Wegner November 1994.

*/

#define Sqrtz(z,rz) (*(rz) = ComplexSqrtFloat((z).x, (z).y))

/* rz=Arcsin(z)=-i*Log{i*z+sqrt(1-z*z)} */
void Arcsinz(Complex z, Complex *rz)
{
    Complex tempz1,tempz2;

  FPUcplxmul( &z, &z, &tempz1);
  tempz1.x = 1 - tempz1.x; tempz1.y = -tempz1.y;  /* tempz1 = 1 - tempz1 */
  Sqrtz( tempz1, &tempz1);

  tempz2.x = -z.y; tempz2.y = z.x;                /* tempz2 = i*z  */
  tempz1.x += tempz2.x;  tempz1.y += tempz2.y;    /* tempz1 += tempz2 */
  FPUcplxlog( &tempz1, &tempz1);
  rz->x = tempz1.y;  rz->y = -tempz1.x;           /* rz = (-i)*tempz1 */
}   /* end. Arcsinz */


/* rz=Arccos(z)=-i*Log{z+sqrt(z*z-1)} */
void Arccosz(Complex z, Complex *rz)
{
    Complex temp;

  FPUcplxmul( &z, &z, &temp);
  temp.x -= 1;                                 /* temp = temp - 1 */
  Sqrtz( temp, &temp);

  temp.x += z.x; temp.y += z.y;                /* temp = z + temp */

  FPUcplxlog( &temp, &temp);
  rz->x = temp.y;  rz->y = -temp.x;              /* rz = (-i)*tempz1 */
}   /* end. Arccosz */

void Arcsinhz(Complex z, Complex *rz)
{
    Complex temp;

  FPUcplxmul( &z, &z, &temp);
  temp.x += 1;                                 /* temp = temp + 1 */
  Sqrtz( temp, &temp);
  temp.x += z.x; temp.y += z.y;                /* temp = z + temp */
  FPUcplxlog( &temp, rz);
}  /* end. Arcsinhz */

/* rz=Arccosh(z)=Log(z+sqrt(z*z-1)} */
void Arccoshz(Complex z, Complex *rz)
{
    Complex tempz;
  FPUcplxmul( &z, &z, &tempz);
  tempz.x -= 1;                              /* tempz = tempz - 1 */
  Sqrtz( tempz, &tempz);
  tempz.x = z.x + tempz.x; tempz.y = z.y + tempz.y;  /* tempz = z + tempz */
  FPUcplxlog( &tempz, rz);
}   /* end. Arccoshz */

/* rz=Arctanh(z)=1/2*Log{(1+z)/(1-z)} */
void Arctanhz(Complex z, Complex *rz)
{
    Complex temp0,temp1,temp2;

  if( z.x == 0.0){
    rz->x = 0;
    rz->y = atan( z.y);
    return;
  }
  else{
    if( fabs(z.x) == 1.0 && z.y == 0.0){
      return;
    }
    else if( fabs( z.x) < 1.0 && z.y == 0.0){
      rz->x = log((1+z.x)/(1-z.x))/2;
      rz->y = 0;
      return;
    }
    else{
      temp0.x = 1 + z.x; temp0.y = z.y;             /* temp0 = 1 + z */
      temp1.x = 1 - z.x; temp1.y = -z.y;            /* temp1 = 1 - z */
      FPUcplxdiv(&temp0, &temp1, &temp2);
      FPUcplxlog(&temp2, &temp2);
      rz->x = .5*temp2.x; rz->y = .5*temp2.y;       /* rz = .5*temp2 */
      return;
    }
  }
}   /* end. Arctanhz */

/* rz=Arctan(z)=i/2*Log{(1-i*z)/(1+i*z)} */
void Arctanz(Complex z, Complex *rz)
{
    Complex temp0,temp1,temp2,temp3;
  if( z.x == 0.0 && z.y == 0.0)
    rz->x = rz->y = 0;
  else if( z.x != 0.0 && z.y == 0.0){
    rz->x = atan( z.x);
    rz->y = 0;
  }
  else if( z.x == 0.0 && z.y != 0.0){
    temp0.x = z.y;  temp0.y = 0.0;
    Arctanhz( temp0, &temp0);
    rz->x = -temp0.y; rz->y = temp0.x;              /* i*temp0 */
  }
  else if( z.x != 0.0 && z.y != 0.0){

    temp0.x = -z.y; temp0.y = z.x;                  /* i*z */
    temp1.x = 1 - temp0.x; temp1.y = -temp0.y;      /* temp1 = 1 - temp0 */
    temp2.x = 1 + temp0.x; temp2.y = temp0.y;       /* temp2 = 1 + temp0 */

    FPUcplxdiv(&temp1, &temp2, &temp3);
    FPUcplxlog(&temp3, &temp3);
    rz->x = -temp3.y*.5; rz->y = .5*temp3.x;           /* .5*i*temp0 */
  }
}   /* end. Arctanz */

Complex ComplexSqrtFloat(double x, double y)
{
   double mag;
   double theta;
   Complex  result;

   if(x == 0.0 && y == 0.0)
      result.x = result.y = 0.0;
   else
   {
      mag   = sqrt(sqrt(x*x + y*y));
      theta = atan2(y, x) / 2;
      FPUsincos(&theta, &result.y, &result.x);
      result.x *= mag;
      result.y *= mag;
   }
   return result;
}

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

