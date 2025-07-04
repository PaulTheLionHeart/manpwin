/* PARSERFP.CPP  -- Part of FRACTINT fractal drawer.  */

/*   By Chuck Ebbert  CompuServe [76306,1226]  */
/*                     internet: 76306.1226@compuserve.com  */

/* Fast floating-point parser code.  The functions beginning with  */
/*    "fStk" are in PARSERA.ASM.  PARSER.C calls this code after  */
/*    it has parsed the formula.  */

/*   Converts the function pointers/load pointers/store pointers  */
/*       built by parsestr() into an optimized array of function  */
/*       pointer/operand pointer pairs.  */

/*   As of 31 Dec 93, also generates executable code in memory.  */
/*       Define the varible COMPILER to generate executable code.  */
/*       COMPILER must also be defined in PARSERA.ASM. */


/* Revision history:  */

/* 15 Mar 1997 TIW  */
/*    Fixed if/else bug, replaced stopmsg with pstopmsg */

/* 09 Mar 1997 TIW/GGM  */
/*    Added support for if/else */

/* 30 Jun 1996 TIW  */
/*    Removed function names if TESTFP not defined to save memory      */
/*    Function fStkFloor added to support new 'floor' function         */
/*    Function fStkCeil  added to support new 'ceil'  function         */
/*    Function fStkTrunc added to support new 'trunc' function         */
/*    Function fStkRound added to support new 'round' function         */

/* 15 Feb 1995 CAE  */
/*    added safety tests to pointer conversion code  */
/*    added the capability for functions to require 4 free regs  */

/* 8 Feb 1995 CAE  */
/*    Comments changed.  */

/* 8 Jan 1995 JCO  */
/*    Function fStkASin added to support new 'asin' function in v19    */
/*    Function fStkASinh added to support new 'asinh' function in v19  */
/*    Function fStkACos added to support new 'acos' function in v19    */
/*    Function fStkACosh added to support new 'acosh' function in v19  */
/*    Function fStkATan added to support new 'atan' function in v19    */
/*    Function fStkATanh added to support new 'atanh' function in v19  */
/*    Function fStkSqrt added to support new 'sqrt' function in v19    */
/*    Function fStkCAbs added to support new 'cabs' function in v19    */
/*    Added support for a third parameter p3    */

/* 31 Dec 1993 CAE  */
/*    Fixed optimizer bug discovered while testing compiler.  */

/* 29 Dec 1993 CAE  */
/*    Added compiler.  */

/* 04 Dec 1993 CAE  */
/*    Added optimizations for LodImagAdd/Sub/Mul.  */

/* 06 Nov 1993 CAE  */
/*    Added optimizer support for LodRealPwr and ORClr2 functions.  */
/*    If stack top is a real, a simpler sqr() or mod() fn will be  */
/*          called (fStkSqr3() was added.)  */
/*    The identities x^0=1, x^1=x, and x^-1=recip(x) are now used by the  */
/*          optimizer.  (fStkOne() was added for this.)  */

/* 31 Oct 1993 CAE  */
/*    Optimizer converts '2*x' and 'x*2' to 'x+x'. */
/*        "     recognizes LastSqr as a real if not stored to.  */

/* 9 Oct 1993 CAE  */
/*    Functions are now converted via table search.                    */
/*    Added "real" stack count variable and debug msgs for stack size. */
/*    Added optimizer extension for commutative multiply.              */
/*    P1, P2 will be treated as consts if they are never stored to.    */
/*    Function fStkStoClr2 now emitted for sto,clr with 2 on stack.    */
/*       "     fStkZero added to support new 'zero' function in v18    */
/*    Added optimization for x^2 -> sqr(x).                            */
/*    Changed "stopmsg" to "DBUGMSG" and made all macros upper case.   */
/*       (debugflag=324 now needed for debug msgs to print.)           */

/* 12 July 1993 (for v18.1) by CAE to fix optimizer bug  */

/* 22 MAR 1993 (for Fractint v18.0)  */

/* ******************************************************************* */
/*                                                                     */
/*  (c) Copyright 1992-1995 Chuck Ebbert.  All rights reserved.        */
/*                                                                     */
/*    This code may be freely distributed and used in non-commercial   */
/*    programs provided the author is credited either during program   */
/*    execution or in the documentation, and this copyright notice     */
/*    is left intact.  Sale of this code, or its use in any commercial */
/*    product requires permission from the author.  Nominal            */
/*    distribution and handling fees may be charged by shareware and   */
/*    freeware distributors.                                           */
/*                                                                     */
/* ******************************************************************* */

/* Uncomment the next line to enable debug messages.  */
/* #define TESTFP 1 */

/* Use startup parameter "debugflag=324" to show debug messages after  */
/*    compiling with above #define uncommented.  */

#include <string.h>
#include <ctype.h>
#include <time.h>

  /* see Fractint.c for a description of the "include"  hierarchy */
//#include "port.h"
#include "prototyp.h"

#define MAXLSYS		255		// Max LSystem in file

/* global data  */
struct fls far *pfls = (struct fls far *)0;





int	MaxFormNameChoices = MAXLSYS;
//char	FormNameChoices[80][25];
char	FormName[30];

extern	char	lsys_type[];
extern	char	lsys_Label[];			// for display in type selection
extern	char	lptr[][100];
extern	int	lsys_num;

extern	char	infile[];	// defined in manpwin.h



extern	char	*str_find_ci(char *, char *);





#if !defined (XFRACT) && !defined (WIN32) /* --  */

/* not moved to PROTOTYPE.H because these only communicate within
   PARSER.C and other parser modules */

extern union Arg *Arg1, *Arg2;
extern double _1_, _2_;
extern union Arg s[20], far * far *Store, far * far *Load;
extern int StoPtr, LodPtr, OpPtr;
extern unsigned int vsp, LastOp;
extern struct ConstArg far *v;
extern int InitLodPtr, InitStoPtr, InitOpPtr, LastInitOp;
extern void (far * far *f)(void);
extern JUMP_CONTROL_ST far *jump_control;
extern int uses_jump, jump_index;

typedef void OLD_FN(void);  /* old C functions  */

OLD_FN  StkLod, StkClr, StkSto, EndInit, StkJumpLabel;
OLD_FN  dStkAdd, dStkSub, dStkMul, dStkDiv;
OLD_FN  dStkSqr, dStkMod;
OLD_FN  dStkSin, dStkCos, dStkSinh, dStkCosh, dStkCosXX;
OLD_FN  dStkTan, dStkTanh, dStkCoTan, dStkCoTanh;
OLD_FN  dStkLog, dStkExp, dStkPwr;
OLD_FN  dStkLT, dStkLTE;
OLD_FN  dStkFlip, dStkReal, dStkImag;
OLD_FN  dStkConj, dStkNeg, dStkAbs;
OLD_FN  dStkRecip, StkIdent;
OLD_FN  dStkGT, dStkGTE, dStkNE, dStkEQ;
OLD_FN  dStkAND, dStkOR;
OLD_FN  dStkZero;
OLD_FN  dStkSqrt;
OLD_FN  dStkASin, dStkACos, dStkASinh, dStkACosh;
OLD_FN  dStkATanh, dStkATan;
OLD_FN  dStkCAbs;
OLD_FN  dStkFloor;
OLD_FN  dStkCeil;
OLD_FN  dStkTrunc;
OLD_FN  dStkRound;
OLD_FN  StkJump, dStkJumpOnTrue, dStkJumpOnFalse;
OLD_FN  dStkOne;

typedef void (near NEW_FN)(void);  /* new 387-only ASM functions  */

NEW_FN  fStkPull2;  /* pull up fpu stack from 2 to 4  */
NEW_FN  fStkPush2;  /* push down fpu stack from 8 to 6  */
NEW_FN  fStkPush2a;  /* push down fpu stack from 6 to 4  */
NEW_FN  fStkPush4;  /* push down fpu stack from 8 to 4  */
NEW_FN  fStkLodDup;  /* lod, dup  */
NEW_FN  fStkLodSqr;  /* lod, sqr, dont save magnitude(i.e. lastsqr)  */
NEW_FN  fStkLodSqr2;  /* lod, sqr, save lastsqr  */
NEW_FN  fStkStoDup;  /* store, duplicate  */
NEW_FN  fStkStoSqr;  /* store, sqr, save lastsqr  */
NEW_FN  fStkStoSqr0;  /* store, sqr, dont save lastsqr  */
NEW_FN  fStkLodDbl;  /* load, double  */
NEW_FN  fStkStoDbl;  /* store, double  */
NEW_FN  fStkReal2;  /* fast ver. of real  */
NEW_FN  fStkSqr;  /* sqr, save magnitude in lastsqr  */
NEW_FN  fStkSqr0;  /* sqr, no save magnitude  */
NEW_FN  fStkClr1;  /* clear fpu  */
NEW_FN  fStkClr2;  /* test stack top, clear fpu  */
NEW_FN  fStkStoClr1;  /* store, clr1  */
NEW_FN  fStkAdd, fStkSub;
NEW_FN  fStkSto, fStkSto2;  /* fast ver. of sto  */
NEW_FN  fStkLod, fStkEndInit;
NEW_FN  fStkMod, fStkMod2;  /* faster mod  */
NEW_FN  fStkLodMod2, fStkStoMod2;
NEW_FN  fStkLTE, fStkLodLTEMul, fStkLTE2, fStkLodLTE;
NEW_FN  fStkLodLTE2, fStkLodLTEAnd2;
NEW_FN  fStkLT, fStkLodLTMul, fStkLT2, fStkLodLT;
NEW_FN  fStkLodLT2;
NEW_FN  fStkGTE, fStkLodGTE, fStkLodGTE2;
NEW_FN  fStkGT, fStkGT2, fStkLodGT, fStkLodGT2;
NEW_FN  fStkEQ, fStkLodEQ, fStkNE, fStkLodNE;
NEW_FN  fStkAND, fStkANDClr2, fStkOR, fStkORClr2;
NEW_FN  fStkSin, fStkSinh, fStkCos, fStkCosh, fStkCosXX;
NEW_FN  fStkTan, fStkTanh, fStkCoTan, fStkCoTanh;
NEW_FN  fStkLog, fStkExp, fStkPwr;
NEW_FN  fStkMul, fStkDiv;
NEW_FN  fStkFlip, fStkReal, fStkImag, fStkRealFlip, fStkImagFlip;
NEW_FN  fStkConj, fStkNeg, fStkAbs, fStkRecip;
NEW_FN  fStkLodReal, fStkLodRealC, fStkLodImag;
NEW_FN  fStkLodRealFlip, fStkLodRealAbs;
NEW_FN  fStkLodRealMul, fStkLodRealAdd, fStkLodRealSub, fStkLodRealPwr;
NEW_FN  fStkLodImagMul, fStkLodImagAdd, fStkLodImagSub;  /* CAE 4Dec93  */
NEW_FN  fStkLodImagFlip, fStkLodImagAbs;
NEW_FN  fStkLodConj;
NEW_FN  fStkLodAdd, fStkLodSub, fStkLodSubMod, fStkLodMul;
NEW_FN  fStkPLodAdd, fStkPLodSub;  /* push-lod-add/sub  */
NEW_FN  fStkIdent;
NEW_FN  fStkStoClr2;  /* store, clear stack by popping  */
NEW_FN  fStkZero;  /* to support new parser fn.  */
NEW_FN  fStkDbl;  /* double the stack top  CAE 31OCT93  */
NEW_FN  fStkOne, fStkSqr3;  /* sqr3 is sqr/mag of a real  CAE 09NOV93  */
NEW_FN  fStkSqrt;
NEW_FN  fStkASin, fStkACos, fStkASinh, fStkACosh;
NEW_FN  fStkATanh, fStkATan;
NEW_FN  fStkCAbs;
NEW_FN  fStkFloor, fStkCeil, fStkTrunc, fStkRound; /* rounding functions */
NEW_FN  fStkJump, fStkJumpOnTrue, fStkJumpOnFalse, fStkJumpLabel; /* flow */
NEW_FN  fStkOne;   /* to support new parser fn.  */

/* check to see if a const is being loaded  */
/* the really awful hack below gets the first char of the name  */
/*    of the variable being accessed  */
/* if first char not alpha, or const p1, p2, or p3 are being accessed  */
/*    then this is a const.  */
#define IS_CONST(x) \
      (!isalpha(**(((char * far *)x ) - 2 ) ) \
      || (x==&PARM1 && p1const ) \
      || (x==&PARM2 && p2const ) \
      || (x==&PARM3 && p3const ) \
      || (x==&PARM4 && p4const ) \
      || (x==&PARM5 && p5const ) )

/* is stack top a real?  */
#define STACK_TOP_IS_REAL \
      ( prevfptr == fStkReal || prevfptr == fStkReal2 \
      || prevfptr == fStkLodReal || prevfptr == fStkLodRealC \
      || prevfptr == fStkLodRealAbs \
      || prevfptr == fStkImag || prevfptr == fStkLodImag )

/* remove push operator from stack top  */
#define REMOVE_PUSH --cvtptrx, stkcnt+=2

#define CLEAR_STK 127
#define FNPTR(x) pfls[(x)].function  /* function pointer */
#define OPPTR(x) pfls[(x)].operand   /* operand pointer */
#define NO_OPERAND (union Arg near *)0
#define NO_FUNCTION (void (near *)(void))0
#define LASTSQR v[4].a
#define PARM1 v[1].a
#define PARM2 v[2].a
#define PARM3 v[8].a
#define PARM4 v[17].a
#define PARM5 v[18].a
#define MAX_STACK 8   /* max # of stack register avail  */

#ifdef TESTFP
int pstopmsg(int x,char far *msg)
{
   static FILE *fp = NULL;
   if(fp == NULL)
      fp = fopen("fpdebug.txt","w");
   if(fp)
   {
#ifndef XFRACT
      fprintf(fp,"%Fs\n",msg);
#else
      fprintf(fp,"%s\n",msg);
#endif
      fflush(fp);
   }
   return(x); /* just to quiet warnings */
}

#define stopmsg pstopmsg

#define DBUGMSG(x,y) if (debugflag==324 || debugflag==322 ) stopmsg((x), (y))
#define DBUGMSG1(x,y,p) \
      if (debugflag==324 || debugflag==322 ){ \
         sprintf(cDbgMsg, (y), (p) ); \
         stopmsg((x), cDbgMsg ); \
      }
#define DBUGMSG2(x,y,p,q) \
      if (debugflag==324 || debugflag==322 ){ \
         sprintf(cDbgMsg, (y), (p), (q) ); \
         stopmsg((x), cDbgMsg ); \
      }
#define DBUGMSG3(x,y,p,q,r) \
      if (debugflag==324 || debugflag==322 ){ \
         sprintf(cDbgMsg, (y), (p), (q), (r) ); \
         stopmsg((x), cDbgMsg ); \
      }
#define DBUGMSG4(x,y,p,q,r,s) \
      if (debugflag==324 || debugflag==322 ){ \
         sprintf(cDbgMsg, (y), (p), (q), (r), (s) ); \
         stopmsg((x), cDbgMsg ); \
      }
#define FNAME(a,b,c,d,e,f) a,b,c,d,e,f    /* use the function name string */
#else

#define DBUGMSG(x,y)
#define DBUGMSG1(x,y,p)
#define DBUGMSG2(x,y,p,q)
#define DBUGMSG3(x,y,p,q,r)
#define DBUGMSG4(x,y,p,q,r,s)
#define FNAME(a,b,c,d,e,f) b,c,d,e,f    /* don't use the function name string */
#endif /* TESTFP */

#define FN_LOD            0
#define FN_CLR            1
#define FN_ADD            2
#define FN_SUB            3
#define FN_MUL            4
#define FN_DIV            5
#define FN_STO            6
#define FN_SQR            7
#define FN_ENDINIT        8
#define FN_MOD            9
#define FN_LTE           10
#define FN_SIN           11
#define FN_COS           12
#define FN_SINH          13
#define FN_COSH          14
#define FN_COSXX         15
#define FN_TAN           16
#define FN_TANH          17
#define FN_COTAN         18
#define FN_COTANH        19
#define FN_LOG           20
#define FN_EXP           21
#define FN_PWR           22
#define FN_LT            23
#define FN_FLIP          24
#define FN_REAL          25
#define FN_IMAG          26
#define FN_CONJ          27
#define FN_NEG           28
#define FN_ABS           29
#define FN_RECIP         30
#define FN_IDENT         31
#define FN_GT            32
#define FN_GTE           33
#define FN_NE            34
#define FN_EQ            35
#define FN_AND           36
#define FN_OR            37
#define FN_ZERO          38
#define FN_SQRT          39
#define FN_ASIN          40
#define FN_ACOS          41
#define FN_ASINH         42
#define FN_ACOSH         43
#define FN_ATANH         44
#define FN_ATAN          45
#define FN_CABS          46
#define FN_FLOOR         47
#define FN_CEIL          48
#define FN_TRUNC         49
#define FN_ROUND         50
#define FN_JUMP          51
#define FN_JUMP_ON_TRUE  52
#define FN_JUMP_ON_FALSE 53
#define FN_JUMP_LABEL    54
#define FN_ONE           55


/* number of "old" functions in the table.  */
/* these are the ones that the parser outputs  */

#define LAST_OLD_FN   FN_ONE
#define NUM_OLD_FNS   LAST_OLD_FN + 1

/* total number of functions in the table.  */

#define LAST_FN          FN_ONE
#define NUM_FNS          LAST_FN + 1

static unsigned char
   realstkcnt,   /* how many scalars are really on stack  */
   stkcnt,       /* # scalars on FPU stack  */
   lastsqrused,  /* was lastsqr loaded in the formula?  */
   lastsqrreal,  /* was lastsqr stored explicitly in the formula?  */
   p1const,      /* is p1 a constant?  */
   p2const,      /* ...and p2?  */
   p3const,      /* ...and p3?  */
   p4const,      /* ...and p4?  */
   p5const;      /* ...and p5?  */

static unsigned int
   cvtptrx;      /* subscript of next free entry in pfls  */

static void (near *prevfptr )(void);  /* previous function pointer  */

/* the entries in this table must be in the same order as  */
/*    the #defines above  */
/* this table is searched sequentially  */
struct fn_entry {

#ifdef TESTFP
   char far *fname;  /* function name  */
#endif
   void (far *infn)(void);  /* 'old' function pointer  */
         /* (infn points to an operator fn in parser.c)  */

   void (near *outfn)(void);  /* pointer to equiv. fast fn.  */

   char min_regs;  /* min regs needed on stack by this fn.  */
         /* (legal values are 0, 2, 4)  */

   char free_regs;  /* free regs required by this fn  */
         /* (legal values are 0, 2, 4)  */

   char delta;  /* net change to # of values on the fp stack  */
         /* (legal values are -2, 0, +2)  */

} static far afe[NUM_OLD_FNS] = {  /* array of function entries  */

   {FNAME("Lod",     StkLod,      fStkLod,    0, 2, +2) },          /*  0  */
   {FNAME("Clr",     StkClr,      fStkClr1,   0, 0,  CLEAR_STK) },  /*  1  */
   {FNAME("+",       dStkAdd,     fStkAdd,    4, 0, -2) },          /*  2  */
   {FNAME("-",       dStkSub,     fStkSub,    4, 0, -2) },          /*  3  */
   {FNAME("*",       dStkMul,     fStkMul,    4, 2, -2) },          /*  4  */
   {FNAME("/",       dStkDiv,     fStkDiv,    4, 2, -2) },          /*  5  */
   {FNAME("Sto",     StkSto,      fStkSto,    2, 0,  0) },          /*  6  */
   {FNAME("Sqr",     dStkSqr,     fStkSqr,    2, 2,  0) },          /*  7  */
   {FNAME(":",       EndInit,     fStkEndInit,0, 0,  CLEAR_STK) },  /*  8  */
   {FNAME("Mod",     dStkMod,     fStkMod,    2, 0,  0) },          /*  9  */
   {FNAME("<=",      dStkLTE,     fStkLTE,    4, 0, -2) },          /* 10  */
   {FNAME("Sin",     dStkSin,     fStkSin,    2, 2,  0) },          /* 11  */
   {FNAME("Cos",     dStkCos,     fStkCos,    2, 2,  0) },          /* 12  */
   {FNAME("Sinh",    dStkSinh,    fStkSinh,   2, 2,  0) },          /* 13  */
   {FNAME("Cosh",    dStkCosh,    fStkCosh,   2, 2,  0) },          /* 14  */
   {FNAME("Cosxx",   dStkCosXX,   fStkCosXX,  2, 2,  0) },          /* 15  */
   {FNAME("Tan",     dStkTan,     fStkTan,    2, 2,  0) },          /* 16  */
   {FNAME("Tanh",    dStkTanh,    fStkTanh,   2, 2,  0) },          /* 17  */
   {FNAME("CoTan",   dStkCoTan,   fStkCoTan,  2, 2,  0) },          /* 18  */
   {FNAME("CoTanh",  dStkCoTanh,  fStkCoTanh, 2, 2,  0) },          /* 19  */
   {FNAME("Log",     dStkLog,     fStkLog,    2, 2,  0) },          /* 20  */
   {FNAME("Exp",     dStkExp,     fStkExp,    2, 2,  0) },          /* 21  */
   {FNAME("^",       dStkPwr,     fStkPwr,    4, 2, -2) },          /* 22  */
   {FNAME("<",       dStkLT,      fStkLT,     4, 0, -2) },          /* 23  */
   {FNAME("Flip",    dStkFlip,    fStkFlip,   2, 0,  0) },          /* 24  */
   {FNAME("Real",    dStkReal,    fStkReal,   2, 0,  0) },          /* 25  */
   {FNAME("Imag",    dStkImag,    fStkImag,   2, 0,  0) },          /* 26  */
   {FNAME("Conj",    dStkConj,    fStkConj,   2, 0,  0) },          /* 27  */
   {FNAME("Neg",     dStkNeg,     fStkNeg,    2, 0,  0) },          /* 28  */
   {FNAME("Abs",     dStkAbs,     fStkAbs,    2, 0,  0) },          /* 29  */
   {FNAME("Recip",   dStkRecip,   fStkRecip,  2, 2,  0) },          /* 30  */
   {FNAME("Ident",   StkIdent,    fStkIdent,  2, 0,  0) },          /* 31  */
   {FNAME(">",       dStkGT,      fStkGT,     4, 0, -2) },          /* 32  */
   {FNAME(">=",      dStkGTE,     fStkGTE,    4, 0, -2) },          /* 33  */
   {FNAME("!=",      dStkNE,      fStkNE,     4, 0, -2) },          /* 34  */
   {FNAME("==",      dStkEQ,      fStkEQ,     4, 0, -2) },          /* 35  */
   {FNAME("&&",      dStkAND,     fStkAND,    4, 0, -2) },          /* 36  */
   {FNAME("||",      dStkOR,      fStkOR,     4, 0, -2) },          /* 37  */
   {FNAME("Zero",    dStkZero,    fStkZero,   2, 0,  0) },          /* 38  */
   {FNAME("Sqrt",    dStkSqrt,    fStkSqrt,   2, 2,  0) },          /* 39  */
   {FNAME("ASin",    dStkASin,    fStkASin,   2, 4,  0) },          /* 40  */
   {FNAME("ACos",    dStkACos,    fStkACos,   2, 4,  0) },          /* 41  */
   {FNAME("ASinh",   dStkASinh,   fStkASinh,  2, 4,  0) },          /* 42  */
   {FNAME("ACosh",   dStkACosh,   fStkACosh,  2, 4,  0) },          /* 43  */
   {FNAME("ATanh",   dStkATanh,   fStkATanh,  2, 4,  0) },          /* 44  */
   {FNAME("ATan",    dStkATan,    fStkATan,   2, 4,  0) },          /* 45  */
   {FNAME("CAbs",    dStkCAbs,    fStkCAbs,   2, 0,  0) },          /* 46  */
   {FNAME("Floor",   dStkFloor,   fStkFloor,  2, 0,  0) },          /* 47  */
   {FNAME("Ceil",    dStkCeil,    fStkCeil,   2, 0,  0) },          /* 48  */
   {FNAME("Trunc",   dStkTrunc,   fStkTrunc,  2, 0,  0) },          /* 49  */
   {FNAME("Round",   dStkRound,   fStkRound,  2, 0,  0) },          /* 50  */
   {FNAME("Jump",        StkJump,         fStkJump,       0, 0, 0)},/* 51  */
   {FNAME("JumpOnTrue",  dStkJumpOnTrue,  fStkJumpOnTrue, 2, 0, 0)},/* 52  */
   {FNAME("JumpOnFalse", dStkJumpOnFalse, fStkJumpOnFalse,2, 0, 0)},/* 53  */
   {FNAME("JumpLabel",   StkJumpLabel,    fStkJumpLabel,  0, 0, 0)},/* 54  */
   {FNAME("One",     dStkOne,     fStkOne,    2, 0,  0) }           /* 55  */
};

#ifdef TESTFP
static char cDbgMsg[255];
#endif  /* TESTFP  */

static int CvtFptr(void (near * ffptr)(void), int MinStk, int FreeStk,
      int Delta )
{
   union Arg near *otemp;    /* temp operand ptr  */
   union Arg far *testload;
#ifdef TESTFP
   int prevstkcnt;
#endif
   double dTemp;

   int Max_On_Stack = MAX_STACK - FreeStk;  /* max regs allowed on stack  */
   int Num_To_Push; /* number of regs to push  */

   /* first do some sanity checks  */ /* CAE 15Feb95  */
   if ( (Delta != -2 && Delta != 0 && Delta != 2 && Delta != CLEAR_STK)
         || (FreeStk != 0 && FreeStk != 2 && FreeStk != 4)
         || (MinStk != 0 && MinStk != 2 && MinStk != 4) ){
awful_error:
      stopmsg (0,"FATAL INTERNAL PARSER ERROR!");
      return 0;  /* put out dire message and revert to old parser  */
   }

   /* this if statement inserts a stack push or pull into the token array  */
   /*   it would be much better to do this *after* optimization  */
   if ((int)stkcnt < MinStk ) { /* not enough operands on fpu stack  */
      DBUGMSG2(0, "Inserted pull.  Stack: %2d --> %2d", stkcnt, stkcnt+2 );
      OPPTR(cvtptrx) = NO_OPERAND;
      FNPTR(cvtptrx++) = fStkPull2;  /* so adjust the stack, pull operand  */
      stkcnt += 2;
   }
   else if ((int)stkcnt > Max_On_Stack ) { /* too many operands  */

      Num_To_Push = stkcnt - Max_On_Stack;
      if (Num_To_Push == 2 ){
         if (stkcnt == MAX_STACK ){
            /* push stack down from max to max-2  */
            FNPTR(cvtptrx) = fStkPush2;
         }
         else if (stkcnt == MAX_STACK - 2 ){
            /* push stack down from max-2 to max-4  */
            FNPTR(cvtptrx) = fStkPush2a;
         }
         else {
            goto awful_error;
         }
         DBUGMSG2(0,
               "Inserted push.  Stack: %2d --> %2d", stkcnt, stkcnt-2 );
         OPPTR(cvtptrx++) = NO_OPERAND;
         stkcnt -= 2;
      }
      else if (Num_To_Push == 4 ){
         /* push down from max to max-4  */
         FNPTR(cvtptrx) = fStkPush4;
         DBUGMSG2(0,
               "Inserted push.  Stack: %2d --> %2d", stkcnt, stkcnt-4 );
         OPPTR(cvtptrx++) = NO_OPERAND;
         stkcnt -= 4;
      }
      else {
         goto awful_error;
      }
   }

   /* set the operand pointer here for store function  */
   if (ffptr == fStkSto ){
      OPPTR(cvtptrx) = (void near *)FP_OFF((Store[StoPtr++]));
   }
   else if (ffptr == fStkLod && debugflag == 322 ){
      /* when disabling optimizer, set load pointer here  */
      OPPTR(cvtptrx) = (void near *)FP_OFF((Load[LodPtr++]));
   }
   else { /* the optimizer will set the pointer for load fn.  */
      OPPTR(cvtptrx) = NO_OPERAND;
   }

   if (debugflag == 322 ){
      goto SkipOptimizer;
   } /* --------------------------  begin optimizer  --------------------- */

   /* This optimizer grew from a simple if statement into a monster.  */

   /* Most of the bugs in the optimizer have been in the code that  */
   /*   juggles the overflow stack.  */

   /* For the following:  */
   /*   * == cvtptrx points to this  */
   /*  () == this is about to be added to the array  */

   /* ******************************************************************** */
   if (ffptr == fStkLod) { /* about to add Lod to the array  */

      if (prevfptr == fStkLod && Load[LodPtr-1] == Load[LodPtr] ) {
         /* previous non-adjust operator was Lod of same operand  */
         /* ? lodx ? (*lodx)  */
         if (FNPTR(--cvtptrx) == fStkPush2 ){ /* prev fn was push  */
            /* ? lod *push (lod)  */
            --cvtptrx;  /* found  *lod push (lod)  */
            if (FNPTR(cvtptrx-1) == fStkPush2){ /* always more ops here  */
               DBUGMSG(0, "push *lod push (lod) -> push4 (*loddup)" );
               FNPTR(cvtptrx-1) = fStkPush4;
            }
            else { /* prev op not push  */
               DBUGMSG(0, "op *lod push (lod) -> op pusha(p=0) (*loddup)" );
               OPPTR(cvtptrx) = NO_OPERAND;  /* use 'alternate' push fn.  */
               FNPTR(cvtptrx++) = fStkPush2a;  /* push w/2 free on stack  */
               /* operand ptr will be set below  */
            }
         }
         else {  /* never  push *lod (lod)  so must be  */
            DBUGMSG(0, "op *lod (lod) -> op (*loddup)" );
         }
         ffptr = fStkLodDup;
      }
      else if (prevfptr == fStkSto2
            && Store[StoPtr-1] == Load[LodPtr] ){
         /* store, load of same value  */
         /* only one operand on stack here when prev oper is Sto2  */
         DBUGMSG(0, "*sto2 (lod) -> (*stodup)" );
         --cvtptrx;
         ffptr = fStkStoDup;
      }
      /* This may cause roundoff problems when later operators  */
      /*  use the rounded value that was stored here, while the next  */
      /*  operator uses the more accurate internal value.  */
      else if (prevfptr == fStkStoClr2
               && Store[StoPtr-1] == Load[LodPtr] ){
         /* store, clear, load same value found  */
         /* only one operand was on stack so this is safe  */
         DBUGMSG (0, "*StoClr2 (Lod) -> (*Sto2)" );
         --cvtptrx;
         ffptr = fStkSto2;  /* use different Sto fn  */
      }
      else {
         testload = Load[LodPtr];
         if (testload == &LASTSQR && lastsqrreal ){
            /* -- LastSqr is a real.  CAE 31OCT93  */
            DBUGMSG(0, "(*lod[lastsqr]) -> (*lodreal)" );
            ffptr = fStkLodReal;
         }
         else if (IS_CONST(testload) && testload->d.y == 0.0 ){
            DBUGMSG(0, "(*lod) -> (*lodrealc)" );
            ffptr = fStkLodRealC;  /* a real const is being loaded  */
         }
      }
      /* set the operand ptr here  */
      OPPTR(cvtptrx) = (void near *)FP_OFF((Load[LodPtr++]));
   }
   /* ******************************************************************** */
   else if (ffptr == fStkAdd ){

      if (prevfptr == fStkLodDup ){ /* there is never a push before add  */
         --cvtptrx;  /* found  ? *loddup (add)  */
         if (cvtptrx!=0 && FNPTR(cvtptrx-1) == fStkPush2a ){
            /* because  push lod lod  is impossible so is  push loddup  */
            DBUGMSG(0, "pusha *loddup (add) -> (*loddbl),stk+=2" );
            REMOVE_PUSH;
            OPPTR(cvtptrx) = OPPTR(cvtptrx+1);  /* fix opptr  */
         }
         else if (cvtptrx!=0 && FNPTR(cvtptrx-1) == fStkPush4 ){
            DBUGMSG(0, "push4 *loddup (add) -> push2 (*loddbl),stk+=2" );
            FNPTR(cvtptrx-1) = fStkPush2;
            stkcnt += 2;  /*  CAE added 12 July 1993 to fix bug  */
         }
         else {
            DBUGMSG(0, "op *loddup (add) -> op (*loddbl)" );
         }
         ffptr = fStkLodDbl;
      }
      else if (prevfptr == fStkStoDup ){
         DBUGMSG(0, "stodup (*add) -> (*stodbl)" );
         /* there are always exactly 4 on stack here  */
         --cvtptrx;
         ffptr = fStkStoDbl;
      }
      else if (prevfptr == fStkLod ){ /* have found  lod (*add)  */
         --cvtptrx;     /*  ? *lod (add)  */
         if (FNPTR(cvtptrx-1) == fStkPush2 ){
            DBUGMSG(0, "*push load (add) -> (*plodadd),stk+=2" );
            REMOVE_PUSH;
            OPPTR(cvtptrx) = OPPTR(cvtptrx+1);  /* fix opptrs  */
            ffptr = fStkPLodAdd;
         }
         else {
            DBUGMSG(0, "op *lod (add) -> op (*lodadd)" );
            ffptr = fStkLodAdd;
         }
      }
      else if (prevfptr == fStkLodReal || prevfptr == fStkLodRealC ){
         --cvtptrx;  /* found  ? *lodreal (add)  */
         if (FNPTR(cvtptrx-1) == fStkPush2 ){
            DBUGMSG(0, "*push lodreal (add) -> (*lodrealadd),stk+=2" );
            REMOVE_PUSH;
            OPPTR(cvtptrx) = OPPTR(cvtptrx+1);  /* fix opptrs  */
         }
         else {
            DBUGMSG(0, "*lodreal (add) -> (*lodrealadd)" );
         }
         ffptr = fStkLodRealAdd;
      }
      else if (prevfptr == fStkLodImag ){  /* CAE 4DEC93  */
         --cvtptrx;  /* found  ? *lodimag (add)  */
         if (FNPTR(cvtptrx-1) == fStkPush2 ){
            DBUGMSG(0, "*push lodimag (add) -> (*lodimagadd),stk+=2" );
            REMOVE_PUSH;
            OPPTR(cvtptrx) = OPPTR(cvtptrx+1);  /* fix opptrs  */
         }
         else {
            DBUGMSG(0, "*lodimag (add) -> (*lodimagadd)" );
         }
         ffptr = fStkLodImagAdd;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkSub ){

      if (prevfptr == fStkLod ){
         /* found  lod (*sub)  */
         --cvtptrx;  /*  *lod (sub)  */
         /* there is never a sequence (lod push sub )  */
         if (FNPTR(cvtptrx-1) == fStkPush2 ){
            DBUGMSG(0, "*push lod (sub) -> (*plodsub),stk+=2" );
            REMOVE_PUSH;
            OPPTR(cvtptrx) = OPPTR(cvtptrx+1);  /* fix opptrs  */
            ffptr = fStkPLodSub;
         }
         else {
            DBUGMSG(0, "*lod (sub) -> (*lodsub)" );
            ffptr = fStkLodSub;
         }
      }
      else if (prevfptr == fStkLodReal || prevfptr == fStkLodRealC ){
         --cvtptrx;  /*  ? *lodreal (sub)  */
         if (FNPTR(cvtptrx-1) == fStkPush2 ){
            DBUGMSG(0, "*push lodreal (sub) -> (*lodrealsub),stk+=2" );
            REMOVE_PUSH;
            OPPTR(cvtptrx) = OPPTR(cvtptrx+1);  /* fix opptrs  */
         }
         else {
            DBUGMSG(0, "*lodreal (sub) -> (*lodrealsub)" );
         }
         ffptr = fStkLodRealSub;
      }
      else if (prevfptr == fStkLodImag ){  /* CAE 4DEC93  */
         --cvtptrx;  /*  ? *lodimag (sub)  */
         if (FNPTR(cvtptrx-1) == fStkPush2 ){
            DBUGMSG(0, "*push lodimag (sub) -> (*lodimagsub),stk+=2" );
            REMOVE_PUSH;
            OPPTR(cvtptrx) = OPPTR(cvtptrx+1);  /* fix opptrs  */
         }
         else {
            DBUGMSG(0, "*lodimag (sub) -> (*lodimagsub)" );
         }
         ffptr = fStkLodImagSub;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkMul ){

      if (prevfptr == fStkLodDup ){
         /* found  loddup ? (*mul)  */
         if (FNPTR(--cvtptrx) == fStkPush2 ){
            DBUGMSG(0, "loddup *push (mul) -> (*lodsqr),stk+=2" );
            REMOVE_PUSH;
         }
         else {
            DBUGMSG(0, "*loddup (mul) -> (*lodsqr)" );
         }
         ffptr = fStkLodSqr;
      }
      else if (prevfptr == fStkStoDup ){ /* no pushes here, 4 on stk.  */
         DBUGMSG(0, "stodup (mul) -> (*stosqr0)" );
         --cvtptrx;
         ffptr = fStkStoSqr0;  /* dont save lastsqr here ever  */
      }
      else if (prevfptr == fStkLod ){
         --cvtptrx;  /*  lod *? (mul)  */
         if (FNPTR(cvtptrx) == fStkPush2 ){ /*  lod *push (mul)  */
            --cvtptrx;  /* ? *lod push (mul)  */
            if(FNPTR(cvtptrx-1) == fStkPush2 ){
               DBUGMSG(0, "push *lod push (mul) -> push4 (*lodmul)" );
               FNPTR(cvtptrx-1) = fStkPush4;
            }
            else {
               DBUGMSG(0, "op *lod push (mul) -> op pusha (*lodmul)" );
               OPPTR(cvtptrx+1) = OPPTR(cvtptrx);  /* fix operand ptr  */
               FNPTR(cvtptrx) = fStkPush2a;
               OPPTR(cvtptrx) = NO_OPERAND;
               cvtptrx++;
            }
         }
         else {
            DBUGMSG(0, "*lod (mul) -> (*lodmul)" );
         }
         ffptr = fStkLodMul;

         /**********************  begin extension  ***  CAE 9 Oct 93  ****/
         /*  change loadreal a, lodmul b --> lod b, lodrealmul a  */

         FNPTR(cvtptrx) = NO_FUNCTION;  /* mark the pending fn as null  */
         if (FNPTR(cvtptrx-1) == fStkPush4
               || FNPTR(cvtptrx-1) == fStkPush2a ){
            --cvtptrx;  /* look back past this push  */
         }

         if (FNPTR(cvtptrx-1) == fStkLodRealC
               && Load[LodPtr-2]->d.x == _2_ ){
            /* -- Convert '2*a' into 'a+a'.                CAE 31OCT93  */
            if (FNPTR(cvtptrx) == NO_FUNCTION ){
               DBUGMSG(0, "lodreal[2] (*lodmul[b])"
                     " -> (*loddbl[b])" );
               OPPTR(cvtptrx-1) = OPPTR(cvtptrx);
            }
            else if (FNPTR(cvtptrx) == fStkPush2a ){
               DBUGMSG(0, "lodreal[2] *pusha (lodmul[b])"
                     " -> loddbl[b],stk+=2" );
               OPPTR(cvtptrx-1) = OPPTR(cvtptrx+1);
               stkcnt += 2;
            }
            else if (FNPTR(cvtptrx) == fStkPush4 ){
               DBUGMSG(0, "lodreal[2] *push4 (lodmul[b])"
                     " -> loddbl[b],stk+=4" );
               OPPTR(cvtptrx-1) = OPPTR(cvtptrx+1);
               stkcnt += 4;
            }
            FNPTR(--cvtptrx) = NO_FUNCTION;  /* so no increment later  */
            ffptr = fStkLodDbl;
         }
         else if (FNPTR(cvtptrx-1) == fStkLodReal
               || FNPTR(cvtptrx-1) == fStkLodRealC ){
            /* lodreal *?push?? (*?lodmul)  */
            otemp = OPPTR(cvtptrx-1);  /* save previous fn's operand  */
            FNPTR(cvtptrx-1) = fStkLod;  /* prev fn = lod  */
            /* Moved setting of prev lodptr to below         CAE 31DEC93  */
            /* This was a bug causing a bad loadptr to be set here  */
            /* 3 lines marked 'prev lodptr=this' below replace this line  */
            if (FNPTR(cvtptrx) == NO_FUNCTION ){
               DBUGMSG(0, "lodreal[a] (*lodmul[b])"
                    " -> lod[b] (*lodrealmul[a])" );
               OPPTR(cvtptrx-1) = OPPTR(cvtptrx);  /* prev lodptr=this  */
            }
            else if (FNPTR(cvtptrx) == fStkPush2a ){
               DBUGMSG(0, "lodreal[a] *pusha (lodmul[b])"
                     " -> lod[b] (*lodrealmul[a]),stk+=2" );
               /* set this fn ptr to null so cvtptrx won't be incr later  */
               FNPTR(cvtptrx) = NO_FUNCTION;
               OPPTR(cvtptrx-1) = OPPTR(cvtptrx+1);  /* prev lodptr=this  */
               stkcnt += 2;
            }
            else if (FNPTR(cvtptrx) == fStkPush4 ){
               DBUGMSG(0, "lodreal[a] *push4 (lodmul[b])"
                     " -> lod[b] push2 (*lodrealmul[a]),stk+=2" );
               FNPTR(cvtptrx++) = fStkPush2;
               OPPTR(cvtptrx-2) = OPPTR(cvtptrx);  /* prev lodptr=this  */
               /* we know cvtptrx points to a null function now  */
               stkcnt += 2;
            }
            OPPTR(cvtptrx) = otemp;  /* switch the operands  */
            ffptr = fStkLodRealMul;  /* next fn is now lodrealmul  */
         }

         if (FNPTR(cvtptrx) != NO_FUNCTION ){
            cvtptrx++;  /* adjust cvtptrx back to normal if needed  */
         }
         /* **********************  end extension  *********************** */
      }
      else if (prevfptr == fStkLodReal || prevfptr == fStkLodRealC ){

         --cvtptrx;  /* found  lodreal *? (mul)  */
         if (FNPTR(cvtptrx) == fStkPush2 ){
            DBUGMSG(0, "lodreal *push2 (mul) -> (*lodrealmul),stk+=2" );
            REMOVE_PUSH;
         }
         else {
            DBUGMSG(0, "*lodreal (mul) -> (*lodrealmul)" );
         }
         ffptr = fStkLodRealMul;

         /**********************  begin extension  ***  CAE 31OCT93  ****/
         if (prevfptr == fStkLodRealC  /* use prevfptr here  */
               && Load[LodPtr-1]->d.x == _2_ ){
            if (FNPTR(cvtptrx) == fStkPush2 ){
               DBUGMSG(0, "push (*lodrealmul[2]) -> (*dbl),stk+=2" );
               REMOVE_PUSH;
            }
            else {
               DBUGMSG(0, "*lodrealmul[2] -> (*dbl)" );
            }
            OPPTR(cvtptrx) = NO_OPERAND;
            ffptr = fStkDbl;

            if (FNPTR(cvtptrx-1) == fStkLod ){
               DBUGMSG(0, "lod (*dbl) -> (*loddbl)" );
               --cvtptrx;
               ffptr = fStkLodDbl;
            }
            else if (FNPTR(cvtptrx-1) == fStkSto2 ){
               DBUGMSG(0, "sto2 (*dbl) -> (*stodbl)" );
               --cvtptrx;
               ffptr = fStkStoDbl;
            }
         }
         /************************  end extension  ***  CAE 31OCT93  ****/
      }
      else if (prevfptr == fStkLodImag ){  /* CAE 4DEC93  */

         --cvtptrx;  /* found  lodimag *? (mul)  */
         if (FNPTR(cvtptrx) == fStkPush2 ){
            DBUGMSG(0, "lodimag *push2 (mul) -> (*lodimagmul),stk+=2" );
            REMOVE_PUSH;
         }
         else {
            DBUGMSG(0, "*lodimag (mul) -> (*lodimagmul)" );
         }
         ffptr = fStkLodImagMul;
      }
      else if (prevfptr == fStkLodLT && FNPTR(cvtptrx-1) != fStkPull2 ){
         /* this shortcut fails if  Lod LT Pull Mul  found  */
         DBUGMSG(0, "LodLT (*Mul) -> (*LodLTMul)" );
         --cvtptrx;  /* never  lod LT Push Mul  here  */
         ffptr = fStkLodLTMul;
      }
      else if (prevfptr == fStkLodLTE && FNPTR(cvtptrx-1) != fStkPull2 ){
         DBUGMSG(0, "LodLTE (*mul) -> (*LodLTEmul)" );
         --cvtptrx;
         ffptr = fStkLodLTEMul;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkClr1 && prevfptr == fStkSto ){

      --cvtptrx;
      if (stkcnt == 2 ){
         DBUGMSG(0, "sto (*clr1) -> (*stoclr2)" );
         ffptr = fStkStoClr2;
      }
      else {
         DBUGMSG(0, "sto (*clr1) -> (*stoclr1)" );
         ffptr = fStkStoClr1;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkDiv ){

      if (prevfptr == fStkLodRealC && vsp < Max_Args - 1 ){
         /* have found a divide by a real constant  */
         /*  and there is space to create a new one  */
         /* lodrealc ? (*div)  */
         if (FNPTR(--cvtptrx) == fStkPush2 ){
            DBUGMSG(0, "lodrealc *push (div) -> (*lodrealmul),stk+=2" );
            REMOVE_PUSH;
         }
         else {
            DBUGMSG(0, "*lodrealc (div) -> (*lodrealmul)" );
         }
         v[vsp].s = (void near *)0;  /* this constant has no name  */
         v[vsp].len = 0;
         v[vsp].a.d.x = _1_ / Load[LodPtr-1]->d.x;
         v[vsp].a.d.y = 0.0;
         {
            void far *p = &v[vsp++].a;
            OPPTR(cvtptrx) = (void near *)FP_OFF(p);  /* isn't C fun!  */
         }
         ffptr = fStkLodRealMul;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkReal ){

      if (prevfptr == fStkLod ){
         DBUGMSG(0, "lod (*real) -> (*lodreal)" );
         --cvtptrx;
         ffptr = fStkLodReal;
      }
      else if (stkcnt < MAX_STACK ){
         DBUGMSG(0, "(*real) -> (*real2)" );
         ffptr = fStkReal2;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkImag && prevfptr == fStkLod ){

      DBUGMSG(0, "lod (*imag) -> lodimag" );
      --cvtptrx;
      ffptr = fStkLodImag;
   }
   /* ******************************************************************** */
   else if (ffptr == fStkConj && prevfptr == fStkLod ){

      DBUGMSG(0, "lod (*conj) -> (*lodconj)" );
      --cvtptrx;
      ffptr = fStkLodConj;
   }
   /* ******************************************************************** */
   else if (ffptr == fStkMod && stkcnt < MAX_STACK ){

      DBUGMSG(0, "(*mod) -> (*mod2)" );
      ffptr = fStkMod2;  /* use faster version if room on stack  */
      if (prevfptr == fStkLod ){
         DBUGMSG(0, "lod (*mod2) -> (*lodmod2)" );
         --cvtptrx;
         ffptr = fStkLodMod2;
      }
      else if (prevfptr == fStkSto || prevfptr == fStkSto2 ){
         DBUGMSG(0, "sto (*mod2) -> (*stomod2)" );
         --cvtptrx;
         ffptr = fStkStoMod2;
      }
      else if (prevfptr == fStkLodSub ){
         DBUGMSG(0, "lodsub (*mod2) -> (*lodsubmod)" );
         --cvtptrx;
         ffptr = fStkLodSubMod;
      }
      else if (STACK_TOP_IS_REAL){  /* CAE 06NOV93  */
         DBUGMSG(0, "(*mod2[st real]) -> (*sqr3)" );
         ffptr = fStkSqr3;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkFlip ){

      if (prevfptr == fStkReal || prevfptr == fStkReal2 ){
         DBUGMSG(0, "real (*flip) -> (*realflip)" );
         --cvtptrx;
         ffptr = fStkRealFlip;
      }
      else if (prevfptr == fStkImag ){
         DBUGMSG(0, "imag (*flip) -> (*imagflip)" );
         --cvtptrx;
         ffptr = fStkImagFlip;
      }
      else if (prevfptr == fStkLodReal ){
         DBUGMSG(0, "lodreal (*flip) -> (*lodrealflip)" );
         --cvtptrx;
         ffptr = fStkLodRealFlip;
      }
      else if (prevfptr == fStkLodImag ){
         DBUGMSG(0, "lodimag (*flip) -> (*lodimagflip)" );
         --cvtptrx;
         ffptr = fStkLodImagFlip;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkAbs ){

      if (prevfptr == fStkLodReal ){
         DBUGMSG(0, "lodreal (*abs) -> (*lodrealabs)" );
         --cvtptrx;
         ffptr = fStkLodRealAbs;
      }
      else if (prevfptr == fStkLodImag ){
         DBUGMSG(0, "lodimag (*abs) -> (*lodimagabs)" );
         --cvtptrx;
         ffptr = fStkLodImagAbs;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkSqr ){

      if (prevfptr == fStkLod && FNPTR(cvtptrx-1) != fStkPush2 ){
         DBUGMSG(0, "lod (*sqr) -> (*lodsqr)" );
         --cvtptrx;
         ffptr = fStkLodSqr;  /* assume no need to save lastsqr  */
         if (lastsqrused){
            DBUGMSG(0, "(*lodsqr) -> (*lodsqr2)" );
            ffptr = fStkLodSqr2;  /* lastsqr is being used  */
         }
      }
      else if (prevfptr == fStkSto2 ){
         DBUGMSG(0, "sto2 (*sqr) -> (*stosqr0)" );
         --cvtptrx;
         ffptr = fStkStoSqr0;  /* assume no need to save lastsqr  */
         if (lastsqrused) {
            DBUGMSG(0, "(*stosqr0) -> (*stosqr)" );
            ffptr = fStkStoSqr;  /* save lastsqr  */
         }
      }
      else {
         if (!lastsqrused) {
            DBUGMSG(0, "(*sqr) -> (*sqr0)" );
            ffptr = fStkSqr0;  /* don't save lastsqr  */
            if (STACK_TOP_IS_REAL){  /* CAE 06NOV93  */
               DBUGMSG(0, "(*sqr0[st real]) -> (*sqr3)" );
               ffptr = fStkSqr3;
            }
         }
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkPwr ){

      if (prevfptr == fStkLodRealC ){
         dTemp = Load[LodPtr-1]->d.x;
         if (dTemp == _2_ || dTemp == _1_ || dTemp == -1.0 || dTemp == 0.0 ){
            /* change ^[-1,0,1,or 2] to recip,one,ident,sqr  CAE 06NOV93  */
            if (FNPTR(cvtptrx-1) == fStkPush2 ){
               DBUGMSG(0, "LodRealC[-1,0,1,2] Push (*Pwr)"
                     " -> (*[recip,1,ident,Sqr0]), stk+=2" );
               REMOVE_PUSH;  /* lod[?] (push) *pwr */
            }
            else {
               DBUGMSG(0, "LodRealC[-1,0,1,2] (*Pwr)"
                     " -> (*[recip,1,ident,sqr0])" );
            }
            --cvtptrx;
            OPPTR(cvtptrx) = NO_OPERAND;
            if (dTemp == _2_ ){
               DBUGMSG(0, "[]=Sqr0" );
               ffptr = fStkSqr0;  /* no need to compute lastsqr here  */
               if (FNPTR(cvtptrx-1) == fStkLod ){
                  DBUGMSG(0, "Lod (*Sqr0) -> (*LodSqr)" );
                  --cvtptrx;
                  ffptr = fStkLodSqr;  /* dont save lastsqr  */
               }
               else if (FNPTR(cvtptrx-1) == fStkSto2 ){
                  DBUGMSG(0, "Sto2 (*Sqr0) -> (*StoSqr0)" );
                  --cvtptrx;
                  ffptr = fStkStoSqr0;  /* dont save lastsqr  */
               }
            }
            else if (dTemp == _1_ ){
               DBUGMSG(0, "[]=Ident" );
               ffptr = fStkIdent;
            }
            else if (dTemp == 0.0 ){
               DBUGMSG(0, "[]=One" );
               ffptr = fStkOne;
            }
            else if (dTemp == -1.0 ){
               DBUGMSG(0, "[]=Recip" );
               ffptr = fStkRecip;
            }
         }
         else if (FNPTR(cvtptrx-1) == prevfptr ){
            --cvtptrx;
            ffptr = fStkLodRealPwr;  /* see comments below  */
         }
      }
      else if (prevfptr == fStkLodReal && FNPTR(cvtptrx-1) == prevfptr ){
         /* CAE 6NOV93  */
         /* don't handle pushes here, lodrealpwr needs 4 free  */
         DBUGMSG(0, "LodReal (*Pwr) -> (*LodRealPwr)" );
         --cvtptrx;
         ffptr = fStkLodRealPwr;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkLTE ){

      if (prevfptr == fStkLod
            || prevfptr == fStkLodReal || prevfptr == fStkLodRealC ){
         DBUGMSG(0, "Lod (*LTE) -> (*LodLTE)" );
         --cvtptrx;
         ffptr = fStkLodLTE;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkLT ){

      if (prevfptr == fStkLod || prevfptr == fStkLodReal
            || prevfptr == fStkLodRealC ){
         DBUGMSG(0, "Lod (*LT) -> (*LodLT)" );
         --cvtptrx;
         ffptr = fStkLodLT;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkGT ){

      if (prevfptr == fStkLod
            || prevfptr == fStkLodReal || prevfptr == fStkLodRealC ){
         DBUGMSG(0, "Lod (*GT) -> (*LodGT)" );
         --cvtptrx;
         ffptr = fStkLodGT;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkGTE ){

      if (prevfptr == fStkLod
            || prevfptr == fStkLodReal || prevfptr == fStkLodRealC ){
         DBUGMSG(0, "Lod (*GTE) -> (*LodGTE)" );
         --cvtptrx;
         ffptr = fStkLodGTE;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkNE ){

      if (prevfptr == fStkLod
            || prevfptr == fStkLodReal || prevfptr == fStkLodRealC ){
         DBUGMSG(0, "Lod (*NE) -> (*LodNE)" );
         --cvtptrx;
         ffptr = fStkLodNE;
      }
   }
   /* ******************************************************************** */
   else if (ffptr == fStkEQ ){

      if (prevfptr == fStkLod
            || prevfptr == fStkLodReal || prevfptr == fStkLodRealC ){
         DBUGMSG(0, "Lod (*EQ) -> (*LodEQ)" );
         --cvtptrx;
         ffptr = fStkLodEQ;
      }
   }
   /* ******************************************************************** */
SkipOptimizer:  /* -------------  end of optimizer ----------------------- */

   FNPTR(cvtptrx++) = prevfptr = ffptr;
#ifdef TESTFP
   prevstkcnt = stkcnt;
#endif
   if (Delta == CLEAR_STK ){
      realstkcnt = stkcnt = 0;
   }
   else {
      stkcnt = (unsigned char)(stkcnt + Delta);
      realstkcnt = (unsigned char)(realstkcnt + Delta);
   }

   DBUGMSG3(0, "Stack:  %2d --> %2d,  Real stack:  %2d",
         prevstkcnt, stkcnt, realstkcnt );

   return 1;  /* return 1 for success  */
}

int fpfill_jump_struct(void)
{ /* Completes all entries in jump structure. Returns 1 on error) */
  /* On entry, jump_index is the number of jump functions in the formula*/
   int i = 0;
   int checkforelse = 0;
   NEW_FN near * JumpFunc = NULL;
   int find_new_func = 1;
   JUMP_PTRS_ST jump_data[MAX_JUMPS];

   for (OpPtr = 0; OpPtr < (int) LastOp; OpPtr++) {
      if(find_new_func) {
         switch (jump_control[i].type) {
            case 1:
               JumpFunc = fStkJumpOnFalse;
               break;
            case 2:
               checkforelse = !checkforelse;
               if(checkforelse)
                  JumpFunc = fStkJump;
               else
                  JumpFunc = fStkJumpOnFalse;
               break;
            case 3:
               JumpFunc = fStkJump;
               break;
            case 4:
               JumpFunc = fStkJumpLabel;
               break;
            default:
               break;
         }
         find_new_func = 0;
      }
      if(pfls[OpPtr].function == JumpFunc) {
         jump_data[i].JumpOpPtr = OpPtr*4;
         i++;
         find_new_func = 1;
      }
   }

      /* Following for safety only; all should always be false */
   if(i != jump_index || jump_control[i - 1].type != 4
           || jump_control[0].type != 1) {
      return 1;
   }

   while(i > 0) {
      i--;
      i = fill_if_group(i, jump_data);
   }
   return i < 0 ? 1 : 0;
}

extern int fform_per_pixel(void);       /* these fns are in parsera.asm  */
extern int BadFormula(void);
extern void (far Img_Setup )(void);

int CvtStk() {  /* convert the array of ptrs  */
   extern char FormName[];
   void (far *ftst)(void);
   void (near *ntst)(void);
   union Arg far *testoperand;
   struct fn_entry far *pfe;
   int fnfound;

   lastsqrreal = 1;  /* assume lastsqr is real (not stored explicitly)  */
   p1const = p2const = p3const = (unsigned char)1;  /* . . . p1, p2, p3 const  */
   p4const = p5const = (unsigned char)1;  /* . . . p4, p5 const  */
   lastsqrused = 0;  /* ... and LastSqr is not used  */

   /* now see if the above assumptions are true */
   for (OpPtr = LodPtr = StoPtr = 0; OpPtr < (int)LastOp; OpPtr++ ){
      ftst = f[OpPtr];
      if (ftst == StkLod ){
         if (Load[LodPtr++] == &LASTSQR ){
            lastsqrused = 1;
         }
      }
      else if (ftst == StkSto ){
         testoperand = Store[StoPtr++];
         if (testoperand == &PARM1 ){
            p1const = 0;
         }
         else if (testoperand == &PARM2 ){
            p2const = 0;
         }
         else if (testoperand == &PARM3 ){
            p3const = 0;
         }
         else if (testoperand == &PARM4 ){
            p4const = 0;
         }
         else if (testoperand == &PARM5 ){
            p5const = 0;
         }
         else if (testoperand == &LASTSQR ){
            lastsqrreal = 0;
         }
      }
   }

   if (!p1const) {
      DBUGMSG(0, "p1 not constant" );
   }
   if (!p2const) {
      DBUGMSG(0, "p2 not constant" );
   }
   if (!p3const) {
      DBUGMSG(0, "p3 not constant" );
   }
   if (!p4const) {
      DBUGMSG(0, "p4 not constant" );
   }
   if (!p5const) {
      DBUGMSG(0, "p5 not constant" );
   }
   if (lastsqrused) {
      DBUGMSG(0, "LastSqr loaded" );
      if (!lastsqrreal) {
         DBUGMSG(0, "LastSqr stored" );
      }
   }

   if (f[LastOp-1] != StkClr ){
      DBUGMSG(0, "Missing clr added at end" );
      /* should be safe to modify this  */
      f[LastOp++] = StkClr;
   }

   prevfptr = (void (near *)(void))0;
   cvtptrx = realstkcnt = stkcnt = 0;

   for (OpPtr = LodPtr = StoPtr = 0; OpPtr < (int)LastOp; OpPtr++) {
      ftst = f[OpPtr];
      fnfound = 0;
      for (pfe = &afe[0]; pfe <= &afe[LAST_OLD_FN]; pfe++ ){
         if (ftst == pfe->infn ){
            fnfound = 1;
            ntst = pfe->outfn;
            if (ntst == fStkClr1 && OpPtr == (int)(LastOp-1) ){
               ntst = fStkClr2;  /* convert the last clear to a clr2  */
               DBUGMSG(0, "Last fn (CLR) --> (is really CLR2)" );
            }
            if (ntst == fStkIdent && debugflag != 322 ){
               /* ident will be skipped here  */
               /* this is really part of the optimizer  */
               DBUGMSG(0, "IDENT was skipped" );
            }
            else {
#ifndef XFRACT
               DBUGMSG4(0, "fn=%Fs, minstk=%1i, freestk=%1i, delta=%3i",
                     pfe->fname,
                     (int)(pfe->min_regs),
                     (int)(pfe->free_regs),
                     (int)(pfe->delta) );
#else
               DBUGMSG4(0, "fn=%s, minstk=%1i, freestk=%1i, delta=%3i",
                     pfe->fname,
                     (int)(pfe->min_regs),
                     (int)(pfe->free_regs),
                     (int)(pfe->delta) );
#endif
               if (!CvtFptr(ntst,
                      pfe->min_regs,
                      pfe->free_regs,
                      pfe->delta) ){
                   return 1;
               }
            }
         }
      }
      if (!fnfound ){
         /* return success so old code will be used  */
         /* stopmsg(0, "Fast 387 parser failed, reverting to slower code." );*/
         return 1;  /* this should only happen if random numbers are used  */
      }
   } /* end for  */

   if (debugflag == 322 ){
      goto skipfinalopt;
   } /* ------------------------------ final optimizations ---------- */

   /* cvtptrx -> one past last operator (always clr2)  */
   --cvtptrx;  /* now it points to the last operator  */
   ntst = FNPTR(cvtptrx-1);
   /* ntst is the next-to-last operator  */

   if (ntst == fStkLT ){
      DBUGMSG(0, "LT Clr2 -> LT2" );
      FNPTR(cvtptrx-1) = fStkLT2;
   }
   else if (ntst == fStkLodLT ){
      DBUGMSG(0, "LodLT Clr2 -> LodLT2" );
      FNPTR(cvtptrx-1) = fStkLodLT2;
   }
   else if (ntst == fStkLTE ){
      DBUGMSG(0, "LTE Clr2 -> LTE2" );
      FNPTR(cvtptrx-1) = fStkLTE2;
   }
   else if (ntst == fStkLodLTE ){
      DBUGMSG(0, "LodLTE Clr2 -> LodLTE2" );
      FNPTR(cvtptrx-1) = fStkLodLTE2;
   }
   else if (ntst == fStkGT ){
      DBUGMSG(0, "GT Clr2 -> GT2" );
      FNPTR(cvtptrx-1) = fStkGT2;
   }
   else if (ntst == fStkLodGT ){
      DBUGMSG(0, "LodGT Clr2 -> LodGT2" );
      FNPTR(cvtptrx-1) = fStkLodGT2;
   }
   else if (ntst == fStkLodGTE ){
      DBUGMSG(0, "LodGTE Clr2 -> LodGTE2" );
      FNPTR(cvtptrx-1) = fStkLodGTE2;
   }
   else if (ntst == fStkAND ){
      DBUGMSG(0, "AND Clr2 -> ANDClr2" );
      FNPTR(cvtptrx-1) = fStkANDClr2;
      ntst = FNPTR(cvtptrx-2);
      if (ntst == fStkLodLTE ){
         DBUGMSG(0, "LodLTE ANDClr2 -> LodLTEAnd2" );
         --cvtptrx;
         FNPTR(cvtptrx-1) = fStkLodLTEAnd2;
      }
   }
   else if (ntst == fStkOR ){  /* CAE 06NOV93  */
      DBUGMSG(0, "OR Clr2 -> ORClr2" );
      FNPTR(cvtptrx-1) = fStkORClr2;
   }
   else {
      ++cvtptrx;  /* adjust this back since no optimization was found  */
   }

skipfinalopt:  /* -------------- end of final optimizations ------------ */

   LastOp = cvtptrx;  /* save the new operator count  */
   LASTSQR.d.y = 0.0;  /* do this once per image  */

   /* now change the pointers  */
   if (FormName[0] != 0 &&
        (uses_jump == 0 || fpfill_jump_struct() == 0)){ /* but only if parse succeeded  */
      curfractalspecific->per_pixel = fform_per_pixel;
      curfractalspecific->orbitcalc = fFormula;
   }
   else {
      curfractalspecific->per_pixel = BadFormula;
      curfractalspecific->orbitcalc = BadFormula;
   }

   Img_Setup();  /* call assembler setup code  */
   return 1;
}

#endif /* XFRACT  */



/*
  Read a formula file, picking off the formula names.
  Formulas use the format "  name = { ... }  name = { ... } "
*/

int get_formula_names(HWND hwnd, char *filename)	// get the fractal formula names

{
FILE	*File;
char	s[480];
//char	/*msg[81],*/ tempstring[480];
int	i, j;

strcpy(infile, filename);				  
strcpy(FormName, "Not Loaded Yet");
if ((File = fopen(filename, "r")) == NULL)
    {
    wsprintf(s, "Can't Open Formula File: <%s>", filename);
    MessageBox (hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
    return -1;
    }

lsys_num = 0;
/*
while(fscanf(File, " %20[^ \n\t({]", lptr[lsys_num]) != EOF) {
    int c;
    
    while(c = getc(File)) {
	if(c == EOF || c == '{' || c == '\n')
	    break;
	}
    if(c == EOF)
	break;
    else if(c != '\n'){
	lsys_num++;
	if (lsys_num >= MaxFormNameChoices) break;
skipcomments:
	if(fscanf(File, "%200[^}]", tempstring) == EOF) break;
	if (getc(File) != '}') goto skipcomments;
	if (stricmp(lptr[lsys_num-1],"") == 0 ||
	    stricmp(lptr[lsys_num-1],"comment") == 0)
	    lsys_num--;
	}
    }
fclose(File);
qsort(lptr,(size_t)lsys_num,sizeof(lptr[0]),(int(*)(const void*, const void *))strcmp);
*/
while (fgets(s, 160, File) != NULL)
    {
    if (str_find_ci(s, "{") != 0 && str_find_ci(s, "comment") == 0)
	{
	for (i = 0; s[i] == ' '; ++i);
	for (j = 0; s[j]; ++j)
	    if (s[j] == '{' || s[j] == '\n' || s[j] == ' ' || s[j] == '(')
		s[j] = '\0';
	s[20] = '\0';
	if (s[0] && (s[0] != ';' && s[0] != ' ' && s[0] != '\n'))
	    {
	    strcpy(lptr[lsys_num], s + i);
	    if (lsys_num < MaxFormNameChoices)
		++lsys_num;
	    else
		break;
	    }
	}
    }

qsort(lptr,(size_t)lsys_num,sizeof(lptr[0]),(int(*)(const void*, const void *))strcmp);

//qsort((void *)lptr, (size_t)lsys_num, sizeof(lptr[0]), (int (*)(void *, void *)) compare_lsys);
//qsort((void *)lptr, (size_t)numformulas, sizeof(lptr[0]), 
//				(int (__cdecl *)(const void *, const void *)) compare_lsys);
fclose(File);
/*
lsys_ptr = 0;					// default to first lsys
if (PreviousLsys_ptr >= lsys_num)
    PreviousLsys_ptr = 0;

if (lsys_ptr >= numformulas)
    lsys_ptr = 0;
*/
strcpy(lsys_Label, "Formula");
/*
return 0;



   int numformulas, i;
   FILE *File;
   char msg[81], tempstring[201];

   FormName[0] = 0;		// start by declaring failure
   for (i = 0; i < MaxFormNameChoices; i++) {
      FormNameChoices[i][0] = 0;
//      win_choices[i] = FormNameChoices[i];
      }

   if((File = fopen(FormFileName, "rt")) == NULL) {
      sprintf("I Can't find %s", FormFileName);
      stopmsg(1,msg);
      return(-1);
   }

   numformulas = 0;
   while(fscanf(File, " %20[^ \n\t({]", FormNameChoices[numformulas]) != EOF) {
      int c;

      while(c = getc(File)) {
	 if(c == EOF || c == '{' || c == '\n')
	    break;
      }
      if(c == EOF)
	 break;
      else if(c != '\n'){
	 numformulas++;
	 if (numformulas >= MaxFormNameChoices) break;
skipcomments:
	 if(fscanf(File, "%200[^}]", tempstring) == EOF) break;
	 if (getc(File) != '}') goto skipcomments;
	 if (stricmp(FormNameChoices[numformulas-1],"") == 0 ||
	     stricmp(FormNameChoices[numformulas-1],"comment") == 0)
		 numformulas--;
      }
   }
   fclose(File);
//   win_numchoices = numformulas;
   qsort(FormNameChoices,win_numchoices,25,
         (int(*)(const void*, const void *))strcmp);
*/
   return(0);
}
/*
int parse_formula_names()	 // parse a fractal formula name
{

   lstrcpy(FormName, win_choices[win_choicemade]);

   if (RunForm(FormName)) {
       FormName[0] = 0; 	// declare failure
       stopmsg(0,"Can't Parse that Formula");
       return(0);
       }

return(1);
}
*/
