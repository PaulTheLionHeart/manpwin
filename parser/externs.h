#ifndef EXTERNS_H
#define EXTERNS_H

#ifndef DEBUG
#define DEBUG 1
#endif

#include    "..\Complex.h"

extern int                   active_system;
extern int                   adapter;
extern int                   bitshift;
extern int                   col;
extern int                   debugflag;
extern long		     threshold;
extern int                   overflow;
extern double                param[];
extern int                   row;
extern int                   save_release;
extern int                   symmetry;
extern char                  s_abs[];
extern char                  s_acos[];
extern char                  s_acosh[];
extern char                  s_asin[];
extern char                  s_asinh[];
extern char                  s_atan[];
extern char                  s_atanh[];
extern char                  s_cabs[];
extern char                  s_ceil[];
extern char                  s_conj[];
extern char                  s_cosh[];
extern char                  s_cosxx[];
extern char                  s_cos[];
extern char                  s_cotanh[];
extern char                  s_cotan[];
extern char                  s_exp[];
extern char                  s_flip[];
extern char                  s_floor[];
extern char                  s_fn1[];
extern char                  s_fn2[];
extern char                  s_fn3[];
extern char                  s_fn4[];
extern char                  s_ident[];
extern char                  s_imag[];
extern char                  s_ismand[];
extern char                  s_log[];
extern char                  s_real[];
extern char                  s_recip[];
extern char                  s_round[];
extern char                  s_sinh[];
extern char                  s_sin[];
extern char                  s_sqr[];
extern char                  s_sqrt[];
extern char                  s_srand[];
extern char                  s_tanh[];
extern char                  s_tan[];
extern char                  s_trunc[];
extern char                  s_xaxis[];
extern char                  s_xyadjust[];
extern char                  s_xyaxis[];
extern char                  s_xyshift[];
extern char                  s_x[];
extern char                  s_yaxis[];
extern char                  s_yes[];
extern char                  s_y[];
extern char                  s_zero[];
extern char                  s_zmag[];
extern char                  s_z[];
extern long                  total_formula_mem;
extern BYTE                  used_extra;
extern int                   xdots;
extern double                xx3rd;
extern double                xxmax;
extern double                xxmin;
extern int                   ydots;
extern double                yy3rd;
extern double                yymax;
extern double                yymin;
#endif
