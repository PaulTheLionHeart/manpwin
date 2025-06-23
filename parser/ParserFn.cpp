/* Parser.cpp (C) 1990, Mark C. Peterson, CompuServe [70441,3353]
   All rights reserved.

   Code may be used in any program provided the author is credited
    either during program execution or in the documentation.  Source
    code may be distributed only in combination with public domain or
    shareware source code.  Source code may be modified provided the
    copyright notice and this message is left unchanged and all
    modifications are clearly documented.

    I would appreciate a copy of any work which incorporates this code,
    however this is optional.

    Mark C. Peterson
    405-C Queen St. Suite #181
    Southington, CT 06489
    (203) 276-9721
*/

// Chuck Ebbert (CompuServe [76306,1226] ) changed code marked 'CAE fp' 
// for fast 387 floating-point math.  See PARSERA.ASM and PARSERFP.C
// (13 Dec 1992.) 

// Modified 12 July 1993 by CAE to fix crash when formula not found.

// Parser functions added PHD 09/06/2025


#include <string.h>
#include <ctype.h>
#include <time.h>

//#include "prototyp.h"
#include "trigfn.h"
#include "..\Complex.h"
#include "..\DDComplex.h"
#include "..\fract.h"

//#define rand15() (rand()&0x7FFF)

#define ChkFloatDenom(denom)\
    if (fabs(denom) <= DBL_MIN) {\
        if (save_release > 1920) overflow = 1;\
        return;\
    }

#define ChkDDDenom(denom)\
    if (fabs(denom) <= DBL_MIN) {\
        if (save_release > 1920) overflow = 1;\
        return;\
    }

#define ChkQDDenom(denom)\
    if (fabs(denom) <= DBL_MIN) {\
        if (save_release > 1920) overflow = 1;\
        return;\
    }

static	    CTrigFn	TrigFn;

static	struct Arg LastSqr;

extern	int Randomized;
extern	int StoPtr, LodPtr, OpPtr; 
extern	struct Arg  s[20], **Store, **Load;
extern	int jump_index;

//extern	DDComplex	DDComplexPower(DDComplex x, DDComplex y);
//extern	QDComplex	QDComplexPower(QDComplex x, QDComplex y);

/* use the following when only float functions are implemented to
   get MP math and Integer math */

/* Random number code, MCP 11-21-91 */
static	unsigned long RandNum;

unsigned long NewRandNum(void)
{
   return(RandNum = ((RandNum << 15) + rand15()) ^ RandNum);
}

void dRandom(void)
{
   long x, y;

   /* Use the same algorithm as for fixed math so that they will generate
          the same fractals when the srand() function is used. */

    bitshift = 20;			// got to make up some silly value PHD 2009-10-17
   x = NewRandNum() >> (32 - bitshift);
   y = NewRandNum() >> (32 - bitshift);
   Arg2->d.x = ((double)x / (1L << bitshift));
   Arg2->d.y = ((double)y / (1L << bitshift));
}

void ddRandom(void)
    {
    long x, y;

    /* Use the same algorithm as for fixed math so that they will generate
	   the same fractals when the srand() function is used. */

    bitshift = 20;			// got to make up some silly value PHD 2009-10-17
    x = NewRandNum() >> (32 - bitshift);
    y = NewRandNum() >> (32 - bitshift);
    Arg2->dd.x = ((double)x / (1L << bitshift));
    Arg2->dd.y = ((double)y / (1L << bitshift));
    }

void qdRandom(void)
    {
    long x, y;

    /* Use the same algorithm as for fixed math so that they will generate
	   the same fractals when the srand() function is used. */

    bitshift = 20;			// got to make up some silly value PHD 2009-10-17
    x = NewRandNum() >> (32 - bitshift);
    y = NewRandNum() >> (32 - bitshift);
    Arg2->qd.x = ((double)x / (1L << bitshift));
    Arg2->qd.y = ((double)y / (1L << bitshift));
    }

void RandomSeed(void)
{
   time_t ltime;

   /* Use the current time to randomize the random number sequence. */
   time(&ltime);
   srand((unsigned int)ltime);

   NewRandNum();
   NewRandNum();
   NewRandNum();
   Randomized = 1;
}

void dStkSRand(void)
{
   long x, y;

   x = rand15();
   y = rand15();
   Arg1->d.x = (double)x / 12000.0;
   Arg1->d.y = (double)y / 12000.0;
}

void ddStkSRand(void)
    {
    long x, y;

    x = rand15();
    y = rand15();
    Arg1->dd.x = (double)x / 12000.0;
    Arg1->dd.y = (double)y / 12000.0;
    }

void qdStkSRand(void)
    {
    long x, y;

    x = rand15();
    y = rand15();
    Arg1->qd.x = (double)x / 12000.0;
    Arg1->qd.y = (double)y / 12000.0;
    }

void (*StkSRand)(void) = dStkSRand;

void dStkLodDup()
{
   Arg1+=2;
   Arg2+=2;
   *Arg2 = *Arg1 = *Load[LodPtr];
   LodPtr+=2;
}

void dStkLodSqr()
{
   Arg1++;
   Arg2++;
   Arg1->d.y = Load[LodPtr]->d.x * Load[LodPtr]->d.y * 2.0;
   Arg1->d.x = (Load[LodPtr]->d.x * Load[LodPtr]->d.x) - (Load[LodPtr]->d.y * Load[LodPtr]->d.y);
   LodPtr++;
}

void dStkLodSqr2()
{
   Arg1++;
   Arg2++;
   LastSqr.d.x = Load[LodPtr]->d.x * Load[LodPtr]->d.x;
   LastSqr.d.y = Load[LodPtr]->d.y * Load[LodPtr]->d.y;
   Arg1->d.y = Load[LodPtr]->d.x * Load[LodPtr]->d.y * 2.0;
   Arg1->d.x = LastSqr.d.x - LastSqr.d.y;
   LastSqr.d.x += LastSqr.d.y;
   LastSqr.d.y = 0;
   LodPtr++;
}

void dStkStoDup(){}
void dStkStoSqr(){}
void dStkStoSqr0(){}

void dStkLodDbl()
{
   Arg1++;
   Arg2++;
   Arg1->d.x = Load[LodPtr]->d.x * 2.0;
   Arg1->d.y = Load[LodPtr]->d.y * 2.0;
   LodPtr++;
}

void dStkStoDbl(){}
void dStkReal2(){}

void dStkSqr0()
{
   LastSqr.d.y = Arg1->d.y * Arg1->d.y; /* use LastSqr as temp storage */
   Arg1->d.y = Arg1->d.x * Arg1->d.y * 2.0;
   Arg1->d.x = Arg1->d.x * Arg1->d.x - LastSqr.d.y;
}

void dStkSqr3()
{
   Arg1->d.x = Arg1->d.x * Arg1->d.x;
}

void dStkAbs(void) {
   Arg1->d.x = fabs(Arg1->d.x);
   Arg1->d.y = fabs(Arg1->d.y);
}

void ddStkAbs(void) {
    Arg1->dd.x = abs(Arg1->dd.x);
    Arg1->dd.y = abs(Arg1->dd.y);
    }

void qdStkAbs(void) {
    Arg1->qd.x = abs(Arg1->qd.x);
    Arg1->qd.y = abs(Arg1->qd.y);
    }

void (*StkAbs)(void) = dStkAbs;

void dStkSqr(void) {
   LastSqr.d.x = Arg1->d.x * Arg1->d.x;
   LastSqr.d.y = Arg1->d.y * Arg1->d.y;
   Arg1->d.y = Arg1->d.x * Arg1->d.y * 2.0;
   Arg1->d.x = LastSqr.d.x - LastSqr.d.y;
   LastSqr.d.x += LastSqr.d.y;
   LastSqr.d.y = 0;
}

void ddStkSqr(void) {
    LastSqr.dd.x = Arg1->dd.x * Arg1->dd.x;
    LastSqr.dd.y = Arg1->dd.y * Arg1->dd.y;
    Arg1->dd.y = Arg1->dd.x * Arg1->dd.y * 2.0;
    Arg1->dd.x = LastSqr.dd.x - LastSqr.dd.y;
    LastSqr.dd.x += LastSqr.dd.y;
    LastSqr.dd.y = 0.0;
    }

void qdStkSqr(void) {
    LastSqr.qd.x = Arg1->qd.x * Arg1->qd.x;
    LastSqr.qd.y = Arg1->qd.y * Arg1->qd.y;
    Arg1->qd.y = Arg1->qd.x * Arg1->qd.y * 2.0;
    Arg1->qd.x = LastSqr.qd.x - LastSqr.qd.y;
    LastSqr.qd.x += LastSqr.qd.y;
    LastSqr.qd.y = 0.0;
    }

void (*StkSqr)(void) = dStkSqr;

void dStkAdd(void) {
    Arg2->d.x += Arg1->d.x;
    Arg2->d.y += Arg1->d.y;
    Arg1--;
    Arg2--;
    }

void ddStkAdd(void) {
    Arg2->dd.x += Arg1->dd.x;
    Arg2->dd.y += Arg1->dd.y;
    Arg1--;
    Arg2--;
    }

void qdStkAdd(void) {
    Arg2->qd.x += Arg1->qd.x;
    Arg2->qd.y += Arg1->qd.y;
    Arg1--;
    Arg2--;
    }

void (*StkAdd)(void) = dStkAdd;

void dStkSub(void) {
   Arg2->d.x -= Arg1->d.x;
   Arg2->d.y -= Arg1->d.y;
   Arg1--;
   Arg2--;
}

void ddStkSub(void) {
    Arg2->dd.x -= Arg1->dd.x;
    Arg2->dd.y -= Arg1->dd.y;
    Arg1--;
    Arg2--;
    }

void qdStkSub(void) {
    Arg2->qd.x -= Arg1->qd.x;
    Arg2->qd.y -= Arg1->qd.y;
    Arg1--;
    Arg2--;
    }

void (*StkSub)(void) = dStkSub;

void dStkConj(void) {
   Arg1->d.y = -Arg1->d.y;
}

void ddStkConj(void) {
    Arg1->dd.y = -Arg1->dd.y;
    }

void qdStkConj(void) {
    Arg1->qd.y = -Arg1->qd.y;
    }

void (*StkConj)(void) = dStkConj;

void dStkFloor(void) {
   Arg1->d.x = floor(Arg1->d.x);
   Arg1->d.y = floor(Arg1->d.y);
}

void ddStkFloor(void) {
    Arg1->dd.x = floor(Arg1->dd.x);
    Arg1->dd.y = floor(Arg1->dd.y);
    }

void qdStkFloor(void) {
    Arg1->qd.x = floor(Arg1->qd.x);
    Arg1->qd.y = floor(Arg1->qd.y);
    }

void (*StkFloor)(void) = dStkFloor;

void dStkCeil(void) {
   Arg1->d.x = ceil(Arg1->d.x);
   Arg1->d.y = ceil(Arg1->d.y);
}

void ddStkCeil(void) {
    Arg1->dd.x = ceil(Arg1->dd.x);
    Arg1->dd.y = ceil(Arg1->dd.y);
    }

void qdStkCeil(void) {
    Arg1->qd.x = ceil(Arg1->qd.x);
    Arg1->qd.y = ceil(Arg1->qd.y);
    }

void (*StkCeil)(void) = dStkCeil;

void dStkTrunc(void) {
   Arg1->d.x = (int)(Arg1->d.x);
   Arg1->d.y = (int)(Arg1->d.y);
}

void ddStkTrunc(void) {
    Arg1->dd.x = (int)(to_double(Arg1->dd.x));
    Arg1->dd.y = (int)(to_double(Arg1->dd.y));
    }

void qdStkTrunc(void) {
    Arg1->qd.x = (int)(to_double(Arg1->qd.x));
    Arg1->qd.y = (int)(to_double(Arg1->qd.y));
    }

void (*StkTrunc)(void) = dStkTrunc;

void dStkRound(void) {
   Arg1->d.x = floor(Arg1->d.x+.5);
   Arg1->d.y = floor(Arg1->d.y+.5);
}

void ddStkRound(void) {
    Arg1->dd.x = floor(Arg1->dd.x + 0.5);
    Arg1->dd.y = floor(Arg1->dd.y + 0.5);
    }

void qdStkRound(void) {
    Arg1->qd.x = floor(Arg1->qd.x + 0.5);
    Arg1->qd.y = floor(Arg1->qd.y + 0.5);
    }

void (*StkRound)(void) = dStkRound;

void dStkZero(void) {
   Arg1->d.y = Arg1->d.x = 0.0;
}

void ddStkZero(void) {
    Arg1->dd.y = Arg1->dd.x = 0.0;
    }

void qdStkZero(void) {
    Arg1->qd.y = Arg1->qd.x = 0.0;
    }

void (*StkZero)(void) = dStkZero;

void dStkOne(void) {
   Arg1->d.x = 1.0;
   Arg1->d.y = 0.0;
}

void ddStkOne(void) {
    Arg1->dd.x = 1.0;
    Arg1->dd.y = 0.0;
    }

void qdStkOne(void) {
    Arg1->qd.x = 1.0;
    Arg1->qd.y = 0.0;
    }

void (*StkOne)(void) = dStkOne;


void dStkReal(void) {
   Arg1->d.y = 0.0;
}

void ddStkReal(void) {
    Arg1->dd.y = 0.0;
    }

void qdStkReal(void) {
    Arg1->qd.y = 0.0;
    }

void (*StkReal)(void) = dStkReal;

void dStkImag(void) {
   Arg1->d.x = Arg1->d.y;
   Arg1->d.y = 0.0;
}

void ddStkImag(void) {
    Arg1->dd.x = Arg1->dd.y;
    Arg1->dd.y = 0.0;
    }

void qdStkImag(void) {
    Arg1->qd.x = Arg1->qd.y;
    Arg1->qd.y = 0.0;
    }

void (*StkImag)(void) = dStkImag;

void dStkNeg(void) {
   Arg1->d.x = -Arg1->d.x;
   Arg1->d.y = -Arg1->d.y;
}

void ddStkNeg(void) {
    Arg1->dd.x = -Arg1->dd.x;
    Arg1->dd.y = -Arg1->dd.y;
    }

void qdStkNeg(void) {
    Arg1->qd.x = -Arg1->qd.x;
    Arg1->qd.y = -Arg1->qd.y;
    }

void (*StkNeg)(void) = dStkNeg;

void dStkMul(void) {
   FPUcplxmul(&Arg2->d, &Arg1->d, &Arg2->d);
   Arg1--;
   Arg2--;
}

void ddStkMul(void) {
    Arg2->dd = Arg2->dd * Arg1->dd;
    Arg1--;
    Arg2--;
    }

void qdStkMul(void) {
    Arg2->qd = Arg2->qd * Arg1->qd;
    Arg1--;
    Arg2--;
    }

void (*StkMul)(void) = dStkMul;

void dStkDiv(void) {
   FPUcplxdiv(&Arg2->d, &Arg1->d, &Arg2->d);
   Arg1--;
   Arg2--;
}

void ddStkDiv(void) {
    Arg2->dd = Arg2->dd / Arg1->dd;
    Arg1--;
    Arg2--;
    }

void qdStkDiv(void) {
    Arg2->qd = Arg2->qd / Arg1->qd;
    Arg1--;
    Arg2--;
    }

void (*StkDiv)(void) = dStkDiv;

void dStkMod(void) {
   Arg1->d.x = (Arg1->d.x * Arg1->d.x) + (Arg1->d.y * Arg1->d.y);
   Arg1->d.y = 0.0;
}

void ddStkMod(void) {
    Arg1->dd.x = (Arg1->dd.x * Arg1->dd.x) + (Arg1->dd.y * Arg1->dd.y);
    Arg1->dd.y = 0.0;
    }

void qdStkMod(void) {
    Arg1->qd.x = (Arg1->qd.x * Arg1->qd.x) + (Arg1->qd.y * Arg1->qd.y);
    Arg1->qd.y = 0.0;
    }

void (*StkMod)(void) = dStkMod;

void StkSto(void) {
   *Store[StoPtr++] = *Arg1;
}

void (*PtrStkSto)(void) = StkSto;

void StkLod(void) {
   Arg1++;
   Arg2++;
   *Arg1 = *Load[LodPtr++];
}

void (*PtrStkLod)(void) = StkLod;

void StkClr(void) {
   s[0] = *Arg1;
   Arg1 = &s[0];
   Arg2 = Arg1;
   Arg2--;
}

void (*PtrStkClr)(void) = StkClr;

/* MCP 4-9-91, Added Flip() */

void dStkFlip(void) {
   double t;

   t = Arg1->d.x;
   Arg1->d.x = Arg1->d.y;
   Arg1->d.y = t;
}

void ddStkFlip(void) {
    dd_real t;

    t = Arg1->dd.x;
    Arg1->dd.x = Arg1->dd.y;
    Arg1->dd.y = t;
    }

void qdStkFlip(void) {
    qd_real t;

    t = Arg1->qd.x;
    Arg1->qd.x = Arg1->qd.y;
    Arg1->qd.y = t;
    }

void (*StkFlip)(void) = dStkFlip;

void dStkSin(void) {
   double sinx, cosx, sinhy, coshy;

   FPUsincos(&Arg1->d.x, &sinx, &cosx);
   FPUsinhcosh(&Arg1->d.y, &sinhy, &coshy);
   Arg1->d.x = sinx*coshy;
   Arg1->d.y = cosx*sinhy;
}

void ddStkSin(void) {
    dd_real sinx, cosx, sinhy, coshy;

    sincos(Arg1->dd.x, sinx, cosx);
    sincosh(Arg1->dd.y, sinhy, coshy);
    Arg1->dd.x = sinx * coshy;
    Arg1->dd.y = cosx * sinhy;
    }

void qdStkSin(void) {
    qd_real sinx, cosx, sinhy, coshy;

    sincos(Arg1->qd.x, sinx, cosx);
    sincosh(Arg1->qd.y, sinhy, coshy);
    Arg1->qd.x = sinx * coshy;
    Arg1->qd.y = cosx * sinhy;
    }

void (*StkSin)(void) = dStkSin;

/* The following functions are supported by both the parser and for fn
   variable replacement. TIW 04-22-91 */

void dStkTan(void) {
   double sinx, cosx, sinhy, coshy, denom;
   Arg1->d.x *= 2;
   Arg1->d.y *= 2;
   FPUsincos(&Arg1->d.x, &sinx, &cosx);
   FPUsinhcosh(&Arg1->d.y, &sinhy, &coshy);
   denom = cosx + coshy;
   ChkFloatDenom(denom);
   Arg1->d.x = sinx/denom;
   Arg1->d.y = sinhy/denom;
}

void ddStkTan(void) {
    dd_real sinx, cosx, sinhy, coshy, denom;
    Arg1->dd.x *= 2;
    Arg1->dd.y *= 2;
    sincos(Arg1->dd.x, sinx, cosx);
    sincosh(Arg1->dd.y, sinhy, coshy);
    denom = cosx + coshy;
    ChkDDDenom(denom);
    Arg1->dd.x = sinx / denom;
    Arg1->dd.y = sinhy / denom;
    }

void qdStkTan(void) {
    qd_real sinx, cosx, sinhy, coshy, denom;
    Arg1->qd.x *= 2;
    Arg1->qd.y *= 2;
    sincos(Arg1->qd.x, sinx, cosx);
    sincosh(Arg1->qd.y, sinhy, coshy);
    denom = cosx + coshy;
    ChkQDDenom(denom);
    Arg1->qd.x = sinx / denom;
    Arg1->qd.y = sinhy / denom;
    }

void (*StkTan)(void) = dStkTan;

void dStkTanh(void) {
   double siny, cosy, sinhx, coshx, denom;
   Arg1->d.x *= 2;
   Arg1->d.y *= 2;
   FPUsincos(&Arg1->d.y, &siny, &cosy);
   FPUsinhcosh(&Arg1->d.x, &sinhx, &coshx);
   denom = coshx + cosy;
   ChkFloatDenom(denom);
   Arg1->d.x = sinhx/denom;
   Arg1->d.y = siny/denom;
}

void ddStkTanh(void) {
    dd_real siny, cosy, sinhx, coshx, denom;
    Arg1->dd.x *= 2;
    Arg1->dd.y *= 2;
    sincos(Arg1->dd.y, siny, cosy);
    sincosh(Arg1->dd.x, sinhx, coshx);
    denom = coshx + cosy;
    ChkDDDenom(denom);
    Arg1->dd.x = sinhx / denom;
    Arg1->dd.y = siny / denom;
    }

void qdStkTanh(void) {
    qd_real siny, cosy, sinhx, coshx, denom;
    Arg1->qd.x *= 2;
    Arg1->qd.y *= 2;
    sincos(Arg1->qd.y, siny, cosy);
    sincosh(Arg1->qd.x, sinhx, coshx);
    denom = coshx + cosy;
    ChkQDDenom(denom);
    Arg1->qd.x = sinhx / denom;
    Arg1->qd.y = siny / denom;
    }

void (*StkTanh)(void) = dStkTanh;

void dStkCoTan(void) {
   double sinx, cosx, sinhy, coshy, denom;
   Arg1->d.x *= 2;
   Arg1->d.y *= 2;
   FPUsincos(&Arg1->d.x, &sinx, &cosx);
   FPUsinhcosh(&Arg1->d.y, &sinhy, &coshy);
   denom = coshy - cosx;
   ChkFloatDenom(denom);
   Arg1->d.x = sinx/denom;
   Arg1->d.y = -sinhy/denom;
}

void ddStkCoTan(void) {
    dd_real sinx, cosx, sinhy, coshy, denom;
    Arg1->dd.x *= 2;
    Arg1->dd.y *= 2;
    sincos(Arg1->dd.x, sinx, cosx);
    sincosh(Arg1->dd.y, sinhy, coshy);
    denom = coshy - cosx;
    ChkDDDenom(denom);
    Arg1->dd.x = sinx / denom;
    Arg1->dd.y = -sinhy / denom;
    }

void qdStkCoTan(void) {
    qd_real sinx, cosx, sinhy, coshy, denom;
    Arg1->qd.x *= 2;
    Arg1->qd.y *= 2;
    sincos(Arg1->qd.x, sinx, cosx);
    sincosh(Arg1->qd.y, sinhy, coshy);
    denom = coshy - cosx;
    ChkQDDenom(denom);
    Arg1->qd.x = sinx / denom;
    Arg1->qd.y = -sinhy / denom;
    }

void (*StkCoTan)(void) = dStkCoTan;

void dStkCoTanh(void) {
   double siny, cosy, sinhx, coshx, denom;
   Arg1->d.x *= 2;
   Arg1->d.y *= 2;
   FPUsincos(&Arg1->d.y, &siny, &cosy);
   FPUsinhcosh(&Arg1->d.x, &sinhx, &coshx);
   denom = coshx - cosy;
   ChkFloatDenom(denom);
   Arg1->d.x = sinhx/denom;
   Arg1->d.y = -siny/denom;
}

void ddStkCoTanh(void) {
    dd_real siny, cosy, sinhx, coshx, denom;
    Arg1->dd.x *= 2;
    Arg1->dd.y *= 2;
    sincos(Arg1->dd.y, siny, cosy);
    sincosh(Arg1->dd.x, sinhx, coshx);
    denom = coshx - cosy;
    ChkDDDenom(denom);
    Arg1->dd.x = sinhx / denom;
    Arg1->dd.y = -siny / denom;
    }

void qdStkCoTanh(void) {
    qd_real siny, cosy, sinhx, coshx, denom;
    Arg1->qd.x *= 2;
    Arg1->qd.y *= 2;
    sincos(Arg1->qd.y, siny, cosy);
    sincosh(Arg1->qd.x, sinhx, coshx);
    denom = coshx - cosy;
    ChkQDDenom(denom);
    Arg1->qd.x = sinhx / denom;
    Arg1->qd.y = -siny / denom;
    }

void (*StkCoTanh)(void) = dStkCoTanh;

/* The following functions are not directly used by the parser - support
   for the parser was not provided because the existing parser language
   represents these quite easily. They are used for fn variable support
   in miscres.c but are placed here because they follow the pattern of
   the other parser functions. TIW 04-22-91 */

void dStkRecip(void) {
   double mod;
   mod =Arg1->d.x * Arg1->d.x + Arg1->d.y * Arg1->d.y;
   ChkFloatDenom(mod);
   Arg1->d.x =  Arg1->d.x/mod;
   Arg1->d.y = -Arg1->d.y/mod;
}

void (*StkRecip)(void) = dStkRecip;

void StkIdent(void) { /* do nothing - the function Z */
}

void (*dStkIdent)(void) = StkIdent;

/* End TIW 04-22-91 */

void dStkSinh(void) {
   double siny, cosy, sinhx, coshx;

   FPUsincos(&Arg1->d.y, &siny, &cosy);
   FPUsinhcosh(&Arg1->d.x, &sinhx, &coshx);
   Arg1->d.x = sinhx*cosy;
   Arg1->d.y = coshx*siny;
}

void ddStkSinh(void) {
    dd_real siny, cosy, sinhx, coshx;

    sincos(Arg1->dd.y, siny, cosy);
    sincosh(Arg1->dd.x, sinhx, coshx);
    Arg1->dd.x = sinhx * cosy;
    Arg1->dd.y = coshx * siny;
    }

void qdStkSinh(void) {
    qd_real siny, cosy, sinhx, coshx;

    sincos(Arg1->dd.y, siny, cosy);
    sincosh(Arg1->dd.x, sinhx, coshx);
    Arg1->qd.x = sinhx * cosy;
    Arg1->qd.y = coshx * siny;
    }

void (*StkSinh)(void) = dStkSinh;

void dStkCos(void) {
   double sinx, cosx, sinhy, coshy;

   FPUsincos(&Arg1->d.x, &sinx, &cosx);
   FPUsinhcosh(&Arg1->d.y, &sinhy, &coshy);
   Arg1->d.x = cosx*coshy;
   Arg1->d.y = -sinx*sinhy; /* TIW 04-25-91 sign */
}

void ddStkCos(void) {
    dd_real sinx, cosx, sinhy, coshy;

    sincos(Arg1->dd.x, sinx, cosx);
    sincosh(Arg1->dd.y, sinhy, coshy);
    Arg1->dd.x = cosx * coshy;
    Arg1->dd.y = -sinx * sinhy; /* TIW 04-25-91 sign */
    }

void qdStkCos(void) {
    qd_real sinx, cosx, sinhy, coshy;

    sincos(Arg1->qd.x, sinx, cosx);
    sincosh(Arg1->qd.y, sinhy, coshy);
    Arg1->qd.x = cosx * coshy;
    Arg1->qd.y = -sinx * sinhy; /* TIW 04-25-91 sign */
    }

void (*StkCos)(void) = dStkCos;

/* Bogus version of cos, to replicate bug which was in regular cos till v16: */

void dStkCosXX(void) {
   dStkCos();
   Arg1->d.y = -Arg1->d.y;
}

void ddStkCosXX(void) {
    ddStkCos();
    Arg1->dd.y = -Arg1->dd.y;
    }

void qdStkCosXX(void) {
    qdStkCos();
    Arg1->qd.y = -Arg1->qd.y;
    }

void (*StkCosXX)(void) = dStkCosXX;

void dStkCosh(void) {
   double siny, cosy, sinhx, coshx;

   FPUsincos(&Arg1->d.y, &siny, &cosy);
   FPUsinhcosh(&Arg1->d.x, &sinhx, &coshx);
   Arg1->d.x = coshx*cosy;
   Arg1->d.y = sinhx*siny;
}

void ddStkCosh(void) {
    dd_real siny, cosy, sinhx, coshx;

    sincos(Arg1->dd.y, siny, cosy);
    sincosh(Arg1->dd.x, sinhx, coshx);
    Arg1->dd.x = coshx * cosy;
    Arg1->dd.y = sinhx * siny;
    }

void qdStkCosh(void) {
    qd_real siny, cosy, sinhx, coshx;

    sincos(Arg1->qd.y, siny, cosy);
    sincosh(Arg1->qd.x, sinhx, coshx);
    Arg1->qd.x = coshx * cosy;
    Arg1->qd.y = sinhx * siny;
    }

void (*StkCosh)(void) = dStkCosh;

/* TIW added arc functions here 11-25-94 */

void dStkASin(void) {
    TrigFn.Arcsinz(Arg1->d, &(Arg1->d));
    }

void ddStkASin(void) {
    TrigFn.DDArcsinz(Arg1->dd, &(Arg1->dd));
    }

void qdStkASin(void) {
    TrigFn.QDArcsinz(Arg1->qd, &(Arg1->qd));
    }

void (*StkASin)(void) = dStkASin;

void dStkASinh(void) {
    TrigFn.Arcsinhz(Arg1->d, &(Arg1->d));
}

void ddStkASinh(void) {
    TrigFn.DDArcsinhz(Arg1->dd, &(Arg1->dd));
    }

void qdStkASinh(void) {
    TrigFn.QDArcsinhz(Arg1->qd, &(Arg1->qd));
    }

void (*StkASinh)(void) = dStkASinh;

void dStkACos(void) {
    TrigFn.Arccosz(Arg1->d, &(Arg1->d));
    }

void ddStkACos(void) {
    TrigFn.DDArccosz(Arg1->dd, &(Arg1->dd));
    }

void qdStkACos(void) {
    TrigFn.QDArccosz(Arg1->qd, &(Arg1->qd));
    }

void (*StkACos)(void) = dStkACos;

void dStkACosh(void) {
    TrigFn.Arccoshz(Arg1->d, &(Arg1->d));
    }

void ddStkACosh(void) {
    TrigFn.DDArccoshz(Arg1->dd, &(Arg1->dd));
    }

void qdStkACosh(void) {
    TrigFn.QDArccoshz(Arg1->qd, &(Arg1->qd));
    }

void (*StkACosh)(void) = dStkACosh;

void dStkATan(void) {
    TrigFn.Arctanz(Arg1->d, &(Arg1->d));
    }

void ddStkATan(void) {
    TrigFn.DDArctanz(Arg1->dd, &(Arg1->dd));
    }

void qdStkATan(void) {
    TrigFn.QDArctanz(Arg1->qd, &(Arg1->qd));
    }

void (*StkATan)(void) = dStkATan;

void dStkATanh(void) {
    TrigFn.Arctanhz(Arg1->d, &(Arg1->d));
    }

void ddStkATanh(void) {
    TrigFn.DDArctanhz(Arg1->dd, &(Arg1->dd));
    }

void qdStkATanh(void) {
    TrigFn.QDArctanhz(Arg1->qd, &(Arg1->qd));
    }

void (*StkATanh)(void) = dStkATanh;

void dStkSqrt(void) {
   Arg1->d = TrigFn.ComplexSqrtFloat(Arg1->d.x, Arg1->d.y);
}

void ddStkSqrt(void) {
    Arg1->dd = Arg1->dd.CSqrt();
    }

void qdStkSqrt(void) {
    Arg1->qd = Arg1->qd.CSqrt();
    }

void (*StkSqrt)(void) = dStkSqrt;

void dStkCAbs(void) {
   Arg1->d.x = sqrt(sqr(Arg1->d.x)+sqr(Arg1->d.y));
   Arg1->d.y = 0.0;
}

void ddStkCAbs(void) {
    Arg1->dd.x = sqrt(sqr(Arg1->dd.x) + sqr(Arg1->dd.y));
    Arg1->dd.y = 0.0;
    }

void qdStkCAbs(void) {
    Arg1->qd.x = sqrt(sqr(Arg1->qd.x) + sqr(Arg1->qd.y));
    Arg1->qd.y = 0.0;
    }

void (*StkCAbs)(void) = dStkCAbs;

/* TIW end arc functions 11-25-94 */

void dStkLT(void) {
   Arg2->d.x = (double)(Arg2->d.x < Arg1->d.x);
   Arg2->d.y = 0.0;
   Arg1--;
   Arg2--;
}

void ddStkLT(void) {
    Arg2->dd.x = (dd_real)(Arg2->dd.x < Arg1->dd.x);
    Arg2->dd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void qdStkLT(void) {
    Arg2->qd.x = (qd_real)(Arg2->qd.x < Arg1->qd.x);
    Arg2->qd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void (*StkLT)(void) = dStkLT;

void dStkGT(void) {
   Arg2->d.x = (double)(Arg2->d.x > Arg1->d.x);
   Arg2->d.y = 0.0;
   Arg1--;
   Arg2--;
}

void ddStkGT(void) {
    Arg2->dd.x = (dd_real)(Arg2->dd.x > Arg1->dd.x);
    Arg2->dd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void qdStkGT(void) {
    Arg2->qd.x = (qd_real)(Arg2->qd.x > Arg1->qd.x);
    Arg2->qd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void (*StkGT)(void) = dStkGT;

void dStkLTE(void) {
   Arg2->d.x = (double)(Arg2->d.x <= Arg1->d.x);
   Arg2->d.y = 0.0;
   Arg1--;
   Arg2--;
}

void ddStkLTE(void) {
    Arg2->dd.x = (dd_real)(Arg2->dd.x <= Arg1->dd.x);
    Arg2->dd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void qdStkLTE(void) {
    Arg2->qd.x = (qd_real)(Arg2->qd.x <= Arg1->qd.x);
    Arg2->qd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void (*StkLTE)(void) = dStkLTE;

void dStkGTE(void) {
   Arg2->d.x = (double)(Arg2->d.x >= Arg1->d.x);
   Arg2->d.y = 0.0;
   Arg1--;
   Arg2--;
}

void ddStkGTE(void) {
    Arg2->dd.x = (dd_real)(Arg2->dd.x >= Arg1->dd.x);
    Arg2->dd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void qdStkGTE(void) {
    Arg2->qd.x = (qd_real)(Arg2->qd.x >= Arg1->qd.x);
    Arg2->qd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void (*StkGTE)(void) = dStkGTE;

void dStkEQ(void) {
   Arg2->d.x = (double)(Arg2->d.x == Arg1->d.x);
   Arg2->d.y = 0.0;
   Arg1--;
   Arg2--;
}

void ddStkEQ(void) {
    Arg2->dd.x = (dd_real)(Arg2->dd.x == Arg1->dd.x);
    Arg2->dd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void qdStkEQ(void) {
    Arg2->qd.x = (qd_real)(Arg2->qd.x == Arg1->qd.x);
    Arg2->qd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void (*StkEQ)(void) = dStkEQ;

void dStkNE(void) {
   Arg2->d.x = (double)(Arg2->d.x != Arg1->d.x);
   Arg2->d.y = 0.0;
   Arg1--;
   Arg2--;
}

void ddStkNE(void) {
    Arg2->dd.x = (dd_real)(Arg2->dd.x != Arg1->dd.x);
    Arg2->dd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void qdStkNE(void) {
    Arg2->qd.x = (qd_real)(Arg2->qd.x != Arg1->qd.x);
    Arg2->qd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void (*StkNE)(void) = dStkNE;

void dStkOR(void) {
   Arg2->d.x = (double)(Arg2->d.x || Arg1->d.x);
   Arg2->d.y = 0.0;
   Arg1--;
   Arg2--;
}

void ddStkOR(void) {
    double  a, b;
    a = to_double(Arg2->dd.x);
    b = to_double(Arg1->dd.x);
    Arg2->dd.x = (dd_real)(a || b);
//    Arg2->dd.x = (dd_real)(Arg2->dd.x || Arg1->dd.x);
    Arg2->dd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void qdStkOR(void) {
    double  a, b;
    a = to_double(Arg2->qd.x);
    b = to_double(Arg1->qd.x);
    Arg2->qd.x = (qd_real)(a || b);
    Arg2->qd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void (*StkOR)(void) = dStkOR;

void dStkAND(void) {
   Arg2->d.x = (double)(Arg2->d.x && Arg1->d.x);
   Arg2->d.y = 0.0;
   Arg1--;
   Arg2--;
}

void ddStkAND(void) {
    double  a, b;
    a = to_double(Arg2->dd.x);
    b = to_double(Arg1->dd.x);
    Arg2->dd.x = (dd_real)(a && b);
//    Arg2->dd.x = (dd_real)(Arg2->dd.x && Arg1->dd.x);
    Arg2->dd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void qdStkAND(void) {
    double  a, b;
    a = to_double(Arg2->qd.x);
    b = to_double(Arg1->qd.x);
    Arg2->qd.x = (qd_real)(a && b);
    //    Arg2->dd.x = (dd_real)(Arg2->dd.x && Arg1->dd.x);
    Arg2->qd.y = 0.0;
    Arg1--;
    Arg2--;
    }

void (*StkAND)(void) = dStkAND;

void dStkLog(void) {
   FPUcplxlog(&Arg1->d, &Arg1->d);
}

void ddStkLog(void) {
    Arg1->dd = Arg1->dd.CLog();
    }

void qdStkLog(void) {
    Arg1->qd = Arg1->qd.CLog();
    }

void (*StkLog)(void) = dStkLog;

void FPUcplxexp(Complex *x, Complex *z) {
   double e2x, siny, cosy;

      e2x = exp(x->x);
      FPUsincos(&x->y, &siny, &cosy);
      z->x = e2x * cosy;
      z->y = e2x * siny;
}

void dStkExp(void) {
      FPUcplxexp(&Arg1->d, &Arg1->d);
   }

void ddStkExp(void) {
    Arg1->dd = Arg1->dd.CExp();
    }

void qdStkExp(void) {
    Arg1->qd = Arg1->qd.CExp();
    }

void (*StkExp)(void) = dStkExp;

void dStkPwr(void) {
   Arg2->d = ComplexPower(Arg2->d, Arg1->d);
   Arg1--;
   Arg2--;
}

void ddStkPwr(void) {
    Arg2->dd = Arg2->dd ^ Arg1->dd;
    Arg1--;
    Arg2--;
    }

void qdStkPwr(void) {
    Arg2->qd = Arg2->qd ^ Arg1->qd;
    Arg1--;
    Arg2--;
    }

void (*StkPwr)(void) = dStkPwr;


