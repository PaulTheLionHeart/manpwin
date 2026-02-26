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
#include <cassert>

#include "trigfn.h"
#include "ParserCtx.h"
#include "..\ManpWIN64\Complex.h"
#include "..\ManpWIN64\DDComplex.h"
#include "..\ManpWIN64\fract.h"

#define ChkFloatDenom(denom)\
    if (fabs(denom) <= DBL_MIN) {\
        if (save_release > 1920) Ctx().overflow = 1;\
        return;\
    }

#define ChkDDDenom(denom)\
    if (fabs(denom) <= DBL_MIN) {\
        if (save_release > 1920) Ctx().overflow = 1;\
        return;\
    }

#define ChkQDDenom(denom)\
    if (fabs(denom) <= DBL_MIN) {\
        if (save_release > 1920) Ctx().overflow = 1;\
        return;\
    }

static CTrigFn	TrigFn;
CParser* g_activeParser = nullptr;   // One pointer per thread later; single-thread for now

//static	struct Arg LastSqr;

extern	int Randomized;

// use the following when only float functions are implemented to get MP math and Integer math

// Random number code, MCP 11-21-91
static	unsigned long RandNum;

unsigned long NewRandNum(void)
    {
    return(RandNum = ((RandNum << 15) + rand15()) ^ RandNum);
    }

void dRandom(void)
    {
    long x, y;

    // Use the same algorithm as for fixed math so that they will generate the same fractals when the srand() function is used.

    bitshift = 20;			// got to make up some silly value PHD 2009-10-17
    x = NewRandNum() >> (32 - bitshift);
    y = NewRandNum() >> (32 - bitshift);
    Ctx().Arg2->d.x = ((double)x / (1L << bitshift));
    Ctx().Arg2->d.y = ((double)y / (1L << bitshift));
    }

void ddRandom(void)
    {
    long x, y;

    // Use the same algorithm as for fixed math so that they will generate the same fractals when the srand() function is used.

    bitshift = 20;			// got to make up some silly value PHD 2009-10-17
    x = NewRandNum() >> (32 - bitshift);
    y = NewRandNum() >> (32 - bitshift);
    Ctx().Arg2->dd.x = ((double)x / (1L << bitshift));
    Ctx().Arg2->dd.y = ((double)y / (1L << bitshift));
    }

void qdRandom(void)
    {
    long x, y;

    // Use the same algorithm as for fixed math so that they will generate the same fractals when the srand() function is used.

    bitshift = 20;			// got to make up some silly value PHD 2009-10-17
    x = NewRandNum() >> (32 - bitshift);
    y = NewRandNum() >> (32 - bitshift);
    Ctx().Arg2->qd.x = ((double)x / (1L << bitshift));
    Ctx().Arg2->qd.y = ((double)y / (1L << bitshift));
    }

void RandomSeed(void)
    {
    time_t ltime;

    // Use the current time to randomize the random number sequence.
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
    Ctx().Arg1->d.x = (double)x / 12000.0;
    Ctx().Arg1->d.y = (double)y / 12000.0;
    }

void ddStkSRand(void)
    {
    long x, y;

    x = rand15();
    y = rand15();
    Ctx().Arg1->dd.x = (double)x / 12000.0;
    Ctx().Arg1->dd.y = (double)y / 12000.0;
    }

void qdStkSRand(void)
    {
    long x, y;

    x = rand15();
    y = rand15();
    Ctx().Arg1->qd.x = (double)x / 12000.0;
    Ctx().Arg1->qd.y = (double)y / 12000.0;
    }

void (*StkSRand)(void) = dStkSRand;

void dStkLodDup()
    {
    auto& ctx = CParser::s_current->m_ctx;

    Ctx().Arg1+=2;
    Ctx().Arg2+=2;
    *Ctx().Arg2 = *Ctx().Arg1 = *ctx.Load[ctx.LodPtr];;
    ctx.LodPtr+=2;
    }

void dStkLodSqr()
    {
    auto& ctx = CParser::s_current->m_ctx;

    Ctx().Arg1++;
    Ctx().Arg2++;
    Ctx().Arg1->d.y = Ctx().Load[Ctx().LodPtr]->d.x * Ctx().Load[Ctx().LodPtr]->d.y * 2.0;
    Ctx().Arg1->d.x = (Ctx().Load[Ctx().LodPtr]->d.x * Ctx().Load[Ctx().LodPtr]->d.x) - (Ctx().Load[Ctx().LodPtr]->d.y * Ctx().Load[Ctx().LodPtr]->d.y);
    Ctx().LodPtr++;
    }

void dStkLodSqr2()
    {
    struct Arg LastSqr;
    Ctx().Arg1++;
    Ctx().Arg2++;
    LastSqr.d.x = Ctx().Load[Ctx().LodPtr]->d.x * Ctx().Load[Ctx().LodPtr]->d.x;
    LastSqr.d.y = Ctx().Load[Ctx().LodPtr]->d.y * Ctx().Load[Ctx().LodPtr]->d.y;
    Ctx().Arg1->d.y = Ctx().Load[Ctx().LodPtr]->d.x * Ctx().Load[Ctx().LodPtr]->d.y * 2.0;
    Ctx().Arg1->d.x = LastSqr.d.x - LastSqr.d.y;
    LastSqr.d.x += LastSqr.d.y;
    LastSqr.d.y = 0;
    Ctx().LodPtr++;
    }

void dStkStoDup(){}
void dStkStoSqr(){}
void dStkStoSqr0(){}

void dStkLodDbl()
    {
    Ctx().Arg1++;
    Ctx().Arg2++;
    Ctx().Arg1->d.x = Ctx().Load[Ctx().LodPtr]->d.x * 2.0;
    Ctx().Arg1->d.y = Ctx().Load[Ctx().LodPtr]->d.y * 2.0;
    Ctx().LodPtr++;
    }

void dStkStoDbl(){}
void dStkReal2(){}

void dStkSqr0()
    {
    struct Arg LastSqr;
    LastSqr.d.y = Ctx().Arg1->d.y * Ctx().Arg1->d.y; /* use LastSqr as temp storage */
    Ctx().Arg1->d.y = Ctx().Arg1->d.x * Ctx().Arg1->d.y * 2.0;
    Ctx().Arg1->d.x = Ctx().Arg1->d.x * Ctx().Arg1->d.x - LastSqr.d.y;
    }

void dStkSqr3()
    {
    Ctx().Arg1->d.x = Ctx().Arg1->d.x * Ctx().Arg1->d.x;
    }

void dStkAbs(void) 
    {
    Ctx().Arg1->d.x = fabs(Ctx().Arg1->d.x);
    Ctx().Arg1->d.y = fabs(Ctx().Arg1->d.y);
    }

void ddStkAbs(void) 
    {
    Ctx().Arg1->dd.x = abs(Ctx().Arg1->dd.x);
    Ctx().Arg1->dd.y = abs(Ctx().Arg1->dd.y);
    }

void qdStkAbs(void) 
    {
    Ctx().Arg1->qd.x = abs(Ctx().Arg1->qd.x);
    Ctx().Arg1->qd.y = abs(Ctx().Arg1->qd.y);
    }

void(*StkAbs)(void) = dStkAbs;
void(*PtrStkAbs)(void) = dStkAbs;

void dStkSqr(void) 
    {
    struct Arg LastSqr;
    LastSqr.d.x = Ctx().Arg1->d.x * Ctx().Arg1->d.x;
    LastSqr.d.y = Ctx().Arg1->d.y * Ctx().Arg1->d.y;
    Ctx().Arg1->d.y = Ctx().Arg1->d.x * Ctx().Arg1->d.y * 2.0;
    Ctx().Arg1->d.x = LastSqr.d.x - LastSqr.d.y;
    LastSqr.d.x += LastSqr.d.y;
    LastSqr.d.y = 0;
/*
    char b[128];
    sprintf_s(b, "SQR input=(%g,%g) output=(%g,%g)\n",
       Ctx().Load[LodPtr]->d.x,
       Ctx().Load[LodPtr]->d.y,
       Ctx().Arg1->d.x,
       Ctx().Arg1->d.y);
    OutputDebugStringA(b);
*/
    }

void ddStkSqr(void) 
    {
    struct Arg LastSqr;
    LastSqr.dd.x = Ctx().Arg1->dd.x * Ctx().Arg1->dd.x;
    LastSqr.dd.y = Ctx().Arg1->dd.y * Ctx().Arg1->dd.y;
    Ctx().Arg1->dd.y = Ctx().Arg1->dd.x * Ctx().Arg1->dd.y * 2.0;
    Ctx().Arg1->dd.x = LastSqr.dd.x - LastSqr.dd.y;
    LastSqr.dd.x += LastSqr.dd.y;
    LastSqr.dd.y = 0.0;
    }

void qdStkSqr(void) 
    {
    struct Arg LastSqr;
    LastSqr.qd.x = Ctx().Arg1->qd.x * Ctx().Arg1->qd.x;
    LastSqr.qd.y = Ctx().Arg1->qd.y * Ctx().Arg1->qd.y;
    Ctx().Arg1->qd.y = Ctx().Arg1->qd.x * Ctx().Arg1->qd.y * 2.0;
    Ctx().Arg1->qd.x = LastSqr.qd.x - LastSqr.qd.y;
    LastSqr.qd.x += LastSqr.qd.y;
    LastSqr.qd.y = 0.0;
    }

void (*StkSqr)(void) = dStkSqr;

void dStkAdd(void) 
    {
    Ctx().Arg2->d.x += Ctx().Arg1->d.x;
    Ctx().Arg2->d.y += Ctx().Arg1->d.y;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkAdd(void) 
    {
    Ctx().Arg2->dd.x += Ctx().Arg1->dd.x;
    Ctx().Arg2->dd.y += Ctx().Arg1->dd.y;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkAdd(void) 
    {
    Ctx().Arg2->qd.x += Ctx().Arg1->qd.x;
    Ctx().Arg2->qd.y += Ctx().Arg1->qd.y;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkAdd)(void) = dStkAdd;
void(*PtrStkAdd)(void) = dStkAdd;

void dStkSub(void) 
    {
    Ctx().Arg2->d.x -= Ctx().Arg1->d.x;
    Ctx().Arg2->d.y -= Ctx().Arg1->d.y;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkSub(void) 
    {
    Ctx().Arg2->dd.x -= Ctx().Arg1->dd.x;
    Ctx().Arg2->dd.y -= Ctx().Arg1->dd.y;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkSub(void) 
    {
    Ctx().Arg2->qd.x -= Ctx().Arg1->qd.x;
    Ctx().Arg2->qd.y -= Ctx().Arg1->qd.y;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkSub)(void) = dStkSub;
void(*PtrStkSub)(void) = dStkSub;

void dStkConj(void) 
    {
    Ctx().Arg1->d.y = -Ctx().Arg1->d.y;
    }

void ddStkConj(void) 
    {
    Ctx().Arg1->dd.y = -Ctx().Arg1->dd.y;
    }

void qdStkConj(void) 
    {
    Ctx().Arg1->qd.y = -Ctx().Arg1->qd.y;
    }

void (*StkConj)(void) = dStkConj;

void dStkFloor(void) 
    {
    Ctx().Arg1->d.x = floor(Ctx().Arg1->d.x);
    Ctx().Arg1->d.y = floor(Ctx().Arg1->d.y);
    }

void ddStkFloor(void) 
    {
    Ctx().Arg1->dd.x = floor(Ctx().Arg1->dd.x);
    Ctx().Arg1->dd.y = floor(Ctx().Arg1->dd.y);
    }

void qdStkFloor(void) 
    {
    Ctx().Arg1->qd.x = floor(Ctx().Arg1->qd.x);
    Ctx().Arg1->qd.y = floor(Ctx().Arg1->qd.y);
    }

void (*StkFloor)(void) = dStkFloor;

void dStkCeil(void) 
    {
    Ctx().Arg1->d.x = ceil(Ctx().Arg1->d.x);
    Ctx().Arg1->d.y = ceil(Ctx().Arg1->d.y);
    }

void ddStkCeil(void) 
    {
    Ctx().Arg1->dd.x = ceil(Ctx().Arg1->dd.x);
    Ctx().Arg1->dd.y = ceil(Ctx().Arg1->dd.y);
    }

void qdStkCeil(void) 
    {
    Ctx().Arg1->qd.x = ceil(Ctx().Arg1->qd.x);
    Ctx().Arg1->qd.y = ceil(Ctx().Arg1->qd.y);
    }

void (*StkCeil)(void) = dStkCeil;

void dStkTrunc(void) 
    {
    Ctx().Arg1->d.x = (int)(Ctx().Arg1->d.x);
    Ctx().Arg1->d.y = (int)(Ctx().Arg1->d.y);
    }

void ddStkTrunc(void) 
    {
    Ctx().Arg1->dd.x = (int)(to_double(Ctx().Arg1->dd.x));
    Ctx().Arg1->dd.y = (int)(to_double(Ctx().Arg1->dd.y));
    }

void qdStkTrunc(void) 
    {
    Ctx().Arg1->qd.x = (int)(to_double(Ctx().Arg1->qd.x));
    Ctx().Arg1->qd.y = (int)(to_double(Ctx().Arg1->qd.y));
    }

void (*StkTrunc)(void) = dStkTrunc;

void dStkRound(void) 
    {
    Ctx().Arg1->d.x = floor(Ctx().Arg1->d.x+.5);
    Ctx().Arg1->d.y = floor(Ctx().Arg1->d.y+.5);
    }

void ddStkRound(void) 
    {
    Ctx().Arg1->dd.x = floor(Ctx().Arg1->dd.x + 0.5);
    Ctx().Arg1->dd.y = floor(Ctx().Arg1->dd.y + 0.5);
    }

void qdStkRound(void) 
    {
    Ctx().Arg1->qd.x = floor(Ctx().Arg1->qd.x + 0.5);
    Ctx().Arg1->qd.y = floor(Ctx().Arg1->qd.y + 0.5);
    }

void (*StkRound)(void) = dStkRound;

void dStkZero(void) 
    {
    Ctx().Arg1->d.y = Ctx().Arg1->d.x = 0.0;
    }

void ddStkZero(void) 
    {
    Ctx().Arg1->dd.y = Ctx().Arg1->dd.x = 0.0;
    }

void qdStkZero(void) 
    {
    Ctx().Arg1->qd.y = Ctx().Arg1->qd.x = 0.0;
    }

void (*StkZero)(void) = dStkZero;

void dStkOne(void) 
    {
    Ctx().Arg1->d.x = 1.0;
    Ctx().Arg1->d.y = 0.0;
    }

void ddStkOne(void) 
    {
    Ctx().Arg1->dd.x = 1.0;
    Ctx().Arg1->dd.y = 0.0;
    }

void qdStkOne(void) 
    {
    Ctx().Arg1->qd.x = 1.0;
    Ctx().Arg1->qd.y = 0.0;
    }

void (*StkOne)(void) = dStkOne;

void dStkReal(void) 
    {
    Ctx().Arg1->d.y = 0.0;
    }

void ddStkReal(void) 
    {
    Ctx().Arg1->dd.y = 0.0;
    }

void qdStkReal(void) 
    {
    Ctx().Arg1->qd.y = 0.0;
    }

void (*StkReal)(void) = dStkReal;

void dStkImag(void) 
    {
    Ctx().Arg1->d.x = Ctx().Arg1->d.y;
    Ctx().Arg1->d.y = 0.0;
    }

void ddStkImag(void) 
    {
    Ctx().Arg1->dd.x = Ctx().Arg1->dd.y;
    Ctx().Arg1->dd.y = 0.0;
    }

void qdStkImag(void) 
    {
    Ctx().Arg1->qd.x = Ctx().Arg1->qd.y;
    Ctx().Arg1->qd.y = 0.0;
    }

void (*StkImag)(void) = dStkImag;

void dStkNeg(void) 
    {
    Ctx().Arg1->d.x = -Ctx().Arg1->d.x;
    Ctx().Arg1->d.y = -Ctx().Arg1->d.y;
    }

void ddStkNeg(void) 
    {
    Ctx().Arg1->dd.x = -Ctx().Arg1->dd.x;
    Ctx().Arg1->dd.y = -Ctx().Arg1->dd.y;
    }

void qdStkNeg(void) 
    {
    Ctx().Arg1->qd.x = -Ctx().Arg1->qd.x;
    Ctx().Arg1->qd.y = -Ctx().Arg1->qd.y;
    }

void(*StkNeg)(void) = dStkNeg;
void(*PtrStkNeg)(void) = dStkNeg;

void dStkMul(void) 
    {
    FPUcplxmul(&Ctx().Arg2->d, &Ctx().Arg1->d, &Ctx().Arg2->d);
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkMul(void) 
    {
    Ctx().Arg2->dd = Ctx().Arg2->dd * Ctx().Arg1->dd;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkMul(void) 
    {
    Ctx().Arg2->qd = Ctx().Arg2->qd * Ctx().Arg1->qd;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkMul)(void) = dStkMul;
void(*PtrStkMul)(void) = dStkMul;

void dStkDiv(void) 
    {
    FPUcplxdiv(&Ctx().Arg2->d, &Ctx().Arg1->d, &Ctx().Arg2->d);
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkDiv(void) 
    {
    Ctx().Arg2->dd = Ctx().Arg2->dd / Ctx().Arg1->dd;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkDiv(void) 
    {
    Ctx().Arg2->qd = Ctx().Arg2->qd / Ctx().Arg1->qd;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkDiv)(void) = dStkDiv;
void(*PtrStkDiv)(void) = dStkDiv;

void dStkMod(void) 
    {
    Ctx().Arg1->d.x = (Ctx().Arg1->d.x * Ctx().Arg1->d.x) + (Ctx().Arg1->d.y * Ctx().Arg1->d.y);
    Ctx().Arg1->d.y = 0.0;
    }

void ddStkMod(void) 
    {
    Ctx().Arg1->dd.x = (Ctx().Arg1->dd.x * Ctx().Arg1->dd.x) + (Ctx().Arg1->dd.y * Ctx().Arg1->dd.y);
    Ctx().Arg1->dd.y = 0.0;
    }

void qdStkMod(void) 
    {
    Ctx().Arg1->qd.x = (Ctx().Arg1->qd.x * Ctx().Arg1->qd.x) + (Ctx().Arg1->qd.y * Ctx().Arg1->qd.y);
    Ctx().Arg1->qd.y = 0.0;
    }

void(*StkMod)(void) = dStkMod;
void(*PtrStkMod)(void) = dStkMod;

inline void SyncStackRegs(ParserExecContext& ctx)
    {
    ctx.Arg2 = ctx.Arg1 - 1;
    }

void StkSto(void) 
    {
    auto& ctx = Ctx();

    VMTRAP(ctx.StoPtr >= (int)ctx.StoreIdx.size(), 2010);
    uint16_t dest = ctx.StoreIdx[ctx.StoPtr++];

    VMTRAP(dest == 0xFFFF || dest >= ctx.v_count, 2001);

    // need at least 1 value on stack
    VMTRAP(ctx.Arg1 < ctx.s.data(), 2002);

    // store top-of-stack into variable
    ctx.v[dest].a = *ctx.Arg1;

    // pop ONLY if this mode wants it
    if (ctx.sto_pops)
	{
	ctx.Arg1--;
	SyncStackRegs(ctx);
	ctx.Arg2 = ctx.Arg1 - 1;
	}
    else
	{
	// keep Arg2 consistent with "top-1"
	ctx.Arg2 = ctx.Arg1 - 1;
	}
    }

void(*PtrStkSto)(void) = StkSto;

void StkLod()
    {
    auto& ctx = Ctx();
    VMTRAP(ctx.LodPtr >= (int)ctx.LoadIdx.size(), 3002);
    uint16_t src = ctx.LoadIdx[ctx.LodPtr++];

    VMTRAP(src == 0xFFFF || src >= ctx.v_count, 3003);

    // PUSH
    VMTRAP(ctx.Arg1 + 1 >= ctx.s.data() + ctx.s.size(), 3001);

    ++ctx.Arg1;                   // PRE-INCREMENT
    *ctx.Arg1 = ctx.v[src].a;
    SyncStackRegs(ctx);
    ctx.Arg2 = ctx.Arg1 - 1;
    }

void (*PtrStkLod)(void) = StkLod;

void StkClr(void) 
    {
    Ctx().s[0] = *Ctx().Arg1;
    Ctx().Arg1 = &Ctx().s[0];
    Ctx().Arg2 = Ctx().Arg1;
    Ctx().Arg2--;
    }

void(*PtrStkClr)(void) = StkClr;

/* MCP 4-9-91, Added Flip() */

void dStkFlip(void) 
    {
    double t;

    t = Ctx().Arg1->d.x;
    Ctx().Arg1->d.x = Ctx().Arg1->d.y;
    Ctx().Arg1->d.y = t;
    }

void ddStkFlip(void) 
    {
    dd_real t;

    t = Ctx().Arg1->dd.x;
    Ctx().Arg1->dd.x = Ctx().Arg1->dd.y;
    Ctx().Arg1->dd.y = t;
    }

void qdStkFlip(void) 
    {
    qd_real t;

    t = Ctx().Arg1->qd.x;
    Ctx().Arg1->qd.x = Ctx().Arg1->qd.y;
    Ctx().Arg1->qd.y = t;
    }

void (*StkFlip)(void) = dStkFlip;

void dStkSin(void) 
    {
    double sinx, cosx, sinhy, coshy;

    FPUsincos(&Ctx().Arg1->d.x, &sinx, &cosx);
    FPUsinhcosh(&Ctx().Arg1->d.y, &sinhy, &coshy);
    Ctx().Arg1->d.x = sinx*coshy;
    Ctx().Arg1->d.y = cosx*sinhy;
    }

void ddStkSin(void) 
    {
    dd_real sinx, cosx, sinhy, coshy;

    sincos(Ctx().Arg1->dd.x, sinx, cosx);
    sincosh(Ctx().Arg1->dd.y, sinhy, coshy);
    Ctx().Arg1->dd.x = sinx * coshy;
    Ctx().Arg1->dd.y = cosx * sinhy;
    }

void qdStkSin(void) 
    {
    qd_real sinx, cosx, sinhy, coshy;

    sincos(Ctx().Arg1->qd.x, sinx, cosx);
    sincosh(Ctx().Arg1->qd.y, sinhy, coshy);
    Ctx().Arg1->qd.x = sinx * coshy;
    Ctx().Arg1->qd.y = cosx * sinhy;
    }

void (*StkSin)(void) = dStkSin;

// The following functions are supported by both the parser and for fn variable replacement. TIW 04-22-91

void dStkTan(void) 
    {
    double sinx, cosx, sinhy, coshy, denom;
    Ctx().Arg1->d.x *= 2;
    Ctx().Arg1->d.y *= 2;
    FPUsincos(&Ctx().Arg1->d.x, &sinx, &cosx);
    FPUsinhcosh(&Ctx().Arg1->d.y, &sinhy, &coshy);
    denom = cosx + coshy;
    ChkFloatDenom(denom);
    Ctx().Arg1->d.x = sinx/denom;
    Ctx().Arg1->d.y = sinhy/denom;
    }

void ddStkTan(void) 
    {
    dd_real sinx, cosx, sinhy, coshy, denom;
    Ctx().Arg1->dd.x *= 2;
    Ctx().Arg1->dd.y *= 2;
    sincos(Ctx().Arg1->dd.x, sinx, cosx);
    sincosh(Ctx().Arg1->dd.y, sinhy, coshy);
    denom = cosx + coshy;
    ChkDDDenom(denom);
    Ctx().Arg1->dd.x = sinx / denom;
    Ctx().Arg1->dd.y = sinhy / denom;
    }

void qdStkTan(void) 
    {
    qd_real sinx, cosx, sinhy, coshy, denom;
    Ctx().Arg1->qd.x *= 2;
    Ctx().Arg1->qd.y *= 2;
    sincos(Ctx().Arg1->qd.x, sinx, cosx);
    sincosh(Ctx().Arg1->qd.y, sinhy, coshy);
    denom = cosx + coshy;
    ChkQDDenom(denom);
    Ctx().Arg1->qd.x = sinx / denom;
    Ctx().Arg1->qd.y = sinhy / denom;
    }

void (*StkTan)(void) = dStkTan;

void dStkTanh(void) 
    {
    double siny, cosy, sinhx, coshx, denom;
    Ctx().Arg1->d.x *= 2;
    Ctx().Arg1->d.y *= 2;
    FPUsincos(&Ctx().Arg1->d.y, &siny, &cosy);
    FPUsinhcosh(&Ctx().Arg1->d.x, &sinhx, &coshx);
    denom = coshx + cosy;
    ChkFloatDenom(denom);
    Ctx().Arg1->d.x = sinhx/denom;
    Ctx().Arg1->d.y = siny/denom;
    }

void ddStkTanh(void) 
    {
    dd_real siny, cosy, sinhx, coshx, denom;
    Ctx().Arg1->dd.x *= 2;
    Ctx().Arg1->dd.y *= 2;
    sincos(Ctx().Arg1->dd.y, siny, cosy);
    sincosh(Ctx().Arg1->dd.x, sinhx, coshx);
    denom = coshx + cosy;
    ChkDDDenom(denom);
    Ctx().Arg1->dd.x = sinhx / denom;
    Ctx().Arg1->dd.y = siny / denom;
    }

void qdStkTanh(void) 
    {
    qd_real siny, cosy, sinhx, coshx, denom;
    Ctx().Arg1->qd.x *= 2;
    Ctx().Arg1->qd.y *= 2;
    sincos(Ctx().Arg1->qd.y, siny, cosy);
    sincosh(Ctx().Arg1->qd.x, sinhx, coshx);
    denom = coshx + cosy;
    ChkQDDenom(denom);
    Ctx().Arg1->qd.x = sinhx / denom;
    Ctx().Arg1->qd.y = siny / denom;
    }

void (*StkTanh)(void) = dStkTanh;

void dStkCoTan(void) 
    {
    double sinx, cosx, sinhy, coshy, denom;
    Ctx().Arg1->d.x *= 2;
    Ctx().Arg1->d.y *= 2;
    FPUsincos(&Ctx().Arg1->d.x, &sinx, &cosx);
    FPUsinhcosh(&Ctx().Arg1->d.y, &sinhy, &coshy);
    denom = coshy - cosx;
    ChkFloatDenom(denom);
    Ctx().Arg1->d.x = sinx/denom;
    Ctx().Arg1->d.y = -sinhy/denom;
    }

void ddStkCoTan(void) 
    {
    dd_real sinx, cosx, sinhy, coshy, denom;
    Ctx().Arg1->dd.x *= 2;
    Ctx().Arg1->dd.y *= 2;
    sincos(Ctx().Arg1->dd.x, sinx, cosx);
    sincosh(Ctx().Arg1->dd.y, sinhy, coshy);
    denom = coshy - cosx;
    ChkDDDenom(denom);
    Ctx().Arg1->dd.x = sinx / denom;
    Ctx().Arg1->dd.y = -sinhy / denom;
    }

void qdStkCoTan(void) 
    {
    qd_real sinx, cosx, sinhy, coshy, denom;
    Ctx().Arg1->qd.x *= 2;
    Ctx().Arg1->qd.y *= 2;
    sincos(Ctx().Arg1->qd.x, sinx, cosx);
    sincosh(Ctx().Arg1->qd.y, sinhy, coshy);
    denom = coshy - cosx;
    ChkQDDenom(denom);
    Ctx().Arg1->qd.x = sinx / denom;
    Ctx().Arg1->qd.y = -sinhy / denom;
    }

void (*StkCoTan)(void) = dStkCoTan;

void dStkCoTanh(void) 
    {
    double siny, cosy, sinhx, coshx, denom;
    Ctx().Arg1->d.x *= 2;
    Ctx().Arg1->d.y *= 2;
    FPUsincos(&Ctx().Arg1->d.y, &siny, &cosy);
    FPUsinhcosh(&Ctx().Arg1->d.x, &sinhx, &coshx);
    denom = coshx - cosy;
    ChkFloatDenom(denom);
    Ctx().Arg1->d.x = sinhx/denom;
    Ctx().Arg1->d.y = -siny/denom;
    }

void ddStkCoTanh(void) 
    {
    dd_real siny, cosy, sinhx, coshx, denom;
    Ctx().Arg1->dd.x *= 2;
    Ctx().Arg1->dd.y *= 2;
    sincos(Ctx().Arg1->dd.y, siny, cosy);
    sincosh(Ctx().Arg1->dd.x, sinhx, coshx);
    denom = coshx - cosy;
    ChkDDDenom(denom);
    Ctx().Arg1->dd.x = sinhx / denom;
    Ctx().Arg1->dd.y = -siny / denom;
    }

void qdStkCoTanh(void) 
    {
    qd_real siny, cosy, sinhx, coshx, denom;
    Ctx().Arg1->qd.x *= 2;
    Ctx().Arg1->qd.y *= 2;
    sincos(Ctx().Arg1->qd.y, siny, cosy);
    sincosh(Ctx().Arg1->qd.x, sinhx, coshx);
    denom = coshx - cosy;
    ChkQDDenom(denom);
    Ctx().Arg1->qd.x = sinhx / denom;
    Ctx().Arg1->qd.y = -siny / denom;
    }

void (*StkCoTanh)(void) = dStkCoTanh;

// The following functions are not directly used by the parser - support
// for the parser was not provided because the existing parser language
// represents these quite easily. They are used for fn variable support
// in miscres.c but are placed here because they follow the pattern of
// the other parser functions. TIW 04-22-91

void dStkRecip(void) 
    {
    double mod;
    mod =Ctx().Arg1->d.x * Ctx().Arg1->d.x + Ctx().Arg1->d.y * Ctx().Arg1->d.y;
    ChkFloatDenom(mod);
    Ctx().Arg1->d.x =  Ctx().Arg1->d.x/mod;
    Ctx().Arg1->d.y = -Ctx().Arg1->d.y/mod;
    }

void (*StkRecip)(void) = dStkRecip;

void StkIdent(void) 
    { // do nothing - the function Z
    }

void (*dStkIdent)(void) = StkIdent;

// End TIW 04-22-91

void dStkSinh(void) 
    {
    double siny, cosy, sinhx, coshx;

    FPUsincos(&Ctx().Arg1->d.y, &siny, &cosy);
    FPUsinhcosh(&Ctx().Arg1->d.x, &sinhx, &coshx);
    Ctx().Arg1->d.x = sinhx*cosy;
    Ctx().Arg1->d.y = coshx*siny;
    }

void ddStkSinh(void) 
    {
    dd_real siny, cosy, sinhx, coshx;

    sincos(Ctx().Arg1->dd.y, siny, cosy);
    sincosh(Ctx().Arg1->dd.x, sinhx, coshx);
    Ctx().Arg1->dd.x = sinhx * cosy;
    Ctx().Arg1->dd.y = coshx * siny;
    }

void qdStkSinh(void) 
    {
    qd_real siny, cosy, sinhx, coshx;

    sincos(Ctx().Arg1->dd.y, siny, cosy);
    sincosh(Ctx().Arg1->dd.x, sinhx, coshx);
    Ctx().Arg1->qd.x = sinhx * cosy;
    Ctx().Arg1->qd.y = coshx * siny;
    }

void (*StkSinh)(void) = dStkSinh;

void dStkCos(void) 
    {
    double sinx, cosx, sinhy, coshy;

    FPUsincos(&Ctx().Arg1->d.x, &sinx, &cosx);
    FPUsinhcosh(&Ctx().Arg1->d.y, &sinhy, &coshy);
    Ctx().Arg1->d.x = cosx*coshy;
    Ctx().Arg1->d.y = -sinx*sinhy; // TIW 04-25-91 sign
    }

void ddStkCos(void) 
    {
    dd_real sinx, cosx, sinhy, coshy;

    sincos(Ctx().Arg1->dd.x, sinx, cosx);
    sincosh(Ctx().Arg1->dd.y, sinhy, coshy);
    Ctx().Arg1->dd.x = cosx * coshy;
    Ctx().Arg1->dd.y = -sinx * sinhy; // TIW 04-25-91 sign
    }

void qdStkCos(void) 
    {
    qd_real sinx, cosx, sinhy, coshy;

    sincos(Ctx().Arg1->qd.x, sinx, cosx);
    sincosh(Ctx().Arg1->qd.y, sinhy, coshy);
    Ctx().Arg1->qd.x = cosx * coshy;
    Ctx().Arg1->qd.y = -sinx * sinhy; /* TIW 04-25-91 sign */
    }

void (*StkCos)(void) = dStkCos;

// Bogus version of cos, to replicate bug which was in regular cos till v16:

void dStkCosXX(void) 
    {
    dStkCos();
    Ctx().Arg1->d.y = -Ctx().Arg1->d.y;
    }

void ddStkCosXX(void) 
    {
    ddStkCos();
    Ctx().Arg1->dd.y = -Ctx().Arg1->dd.y;
    }

void qdStkCosXX(void) 
    {
    qdStkCos();
    Ctx().Arg1->qd.y = -Ctx().Arg1->qd.y;
    }

void (*StkCosXX)(void) = dStkCosXX;

void dStkCosh(void) 
    {
    double siny, cosy, sinhx, coshx;

    FPUsincos(&Ctx().Arg1->d.y, &siny, &cosy);
    FPUsinhcosh(&Ctx().Arg1->d.x, &sinhx, &coshx);
    Ctx().Arg1->d.x = coshx*cosy;
    Ctx().Arg1->d.y = sinhx*siny;
    }

void ddStkCosh(void) 
    {
    dd_real siny, cosy, sinhx, coshx;

    sincos(Ctx().Arg1->dd.y, siny, cosy);
    sincosh(Ctx().Arg1->dd.x, sinhx, coshx);
    Ctx().Arg1->dd.x = coshx * cosy;
    Ctx().Arg1->dd.y = sinhx * siny;
    }

void qdStkCosh(void) 
    {
    qd_real siny, cosy, sinhx, coshx;

    sincos(Ctx().Arg1->qd.y, siny, cosy);
    sincosh(Ctx().Arg1->qd.x, sinhx, coshx);
    Ctx().Arg1->qd.x = coshx * cosy;
    Ctx().Arg1->qd.y = sinhx * siny;
    }

void (*StkCosh)(void) = dStkCosh;

// TIW added arc functions here 11-25-94

void dStkASin(void) 
    {
    TrigFn.Arcsinz(Ctx().Arg1->d, &(Ctx().Arg1->d));
    }

void ddStkASin(void) 
    {
    TrigFn.DDArcsinz(Ctx().Arg1->dd, &(Ctx().Arg1->dd));
    }

void qdStkASin(void) 
    {
    TrigFn.QDArcsinz(Ctx().Arg1->qd, &(Ctx().Arg1->qd));
    }

void (*StkASin)(void) = dStkASin;

void dStkASinh(void)	
    {
    TrigFn.Arcsinhz(Ctx().Arg1->d, &(Ctx().Arg1->d));
    }

void ddStkASinh(void) 
    {
    TrigFn.DDArcsinhz(Ctx().Arg1->dd, &(Ctx().Arg1->dd));
    }

void qdStkASinh(void) 
    {
    TrigFn.QDArcsinhz(Ctx().Arg1->qd, &(Ctx().Arg1->qd));
    }

void (*StkASinh)(void) = dStkASinh;

void dStkACos(void) 
    {
    TrigFn.Arccosz(Ctx().Arg1->d, &(Ctx().Arg1->d));
    }

void ddStkACos(void) 
    {
    TrigFn.DDArccosz(Ctx().Arg1->dd, &(Ctx().Arg1->dd));
    }

void qdStkACos(void) 
    {
    TrigFn.QDArccosz(Ctx().Arg1->qd, &(Ctx().Arg1->qd));
    }

void (*StkACos)(void) = dStkACos;

void dStkACosh(void) 
    {
    TrigFn.Arccoshz(Ctx().Arg1->d, &(Ctx().Arg1->d));
    }

void ddStkACosh(void) 
    {
    TrigFn.DDArccoshz(Ctx().Arg1->dd, &(Ctx().Arg1->dd));
    }

void qdStkACosh(void) 
    {
    TrigFn.QDArccoshz(Ctx().Arg1->qd, &(Ctx().Arg1->qd));
    }

void (*StkACosh)(void) = dStkACosh;

void dStkATan(void) 
    {
    TrigFn.Arctanz(Ctx().Arg1->d, &(Ctx().Arg1->d));
    }

void ddStkATan(void) 
    {
    TrigFn.DDArctanz(Ctx().Arg1->dd, &(Ctx().Arg1->dd));
    }

void qdStkATan(void) 
    {
    TrigFn.QDArctanz(Ctx().Arg1->qd, &(Ctx().Arg1->qd));
    }

void (*StkATan)(void) = dStkATan;

void dStkATanh(void) 
    {
    TrigFn.Arctanhz(Ctx().Arg1->d, &(Ctx().Arg1->d));
    }

void ddStkATanh(void) 
    {
    TrigFn.DDArctanhz(Ctx().Arg1->dd, &(Ctx().Arg1->dd));
    }

void qdStkATanh(void) 
    {
    TrigFn.QDArctanhz(Ctx().Arg1->qd, &(Ctx().Arg1->qd));
    }

void (*StkATanh)(void) = dStkATanh;

void dStkSqrt(void) 
    {
    Ctx().Arg1->d = TrigFn.ComplexSqrtFloat(Ctx().Arg1->d.x, Ctx().Arg1->d.y);
    }

void ddStkSqrt(void) 
    {
    Ctx().Arg1->dd = Ctx().Arg1->dd.CSqrt();
    }

void qdStkSqrt(void) 
    {
    Ctx().Arg1->qd = Ctx().Arg1->qd.CSqrt();
    }

void (*StkSqrt)(void) = dStkSqrt;

void dStkCAbs(void) 
    {
    Ctx().Arg1->d.x = sqrt(sqr(Ctx().Arg1->d.x)+sqr(Ctx().Arg1->d.y));
    Ctx().Arg1->d.y = 0.0;
    }

void ddStkCAbs(void) 
    {
    Ctx().Arg1->dd.x = sqrt(sqr(Ctx().Arg1->dd.x) + sqr(Ctx().Arg1->dd.y));
    Ctx().Arg1->dd.y = 0.0;
    }

void qdStkCAbs(void) 
    {
    Ctx().Arg1->qd.x = sqrt(sqr(Ctx().Arg1->qd.x) + sqr(Ctx().Arg1->qd.y));
    Ctx().Arg1->qd.y = 0.0;
    }

void (*StkCAbs)(void) = dStkCAbs;

// TIW end arc functions 11-25-94

void dStkLT(void) 
    {
    Ctx().Arg2->d.x = (double)(Ctx().Arg2->d.x < Ctx().Arg1->d.x);
    Ctx().Arg2->d.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkLT(void) 
    {
    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x < Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkLT(void) 
    {
    Ctx().Arg2->qd.x = (qd_real)(Ctx().Arg2->qd.x < Ctx().Arg1->qd.x);
    Ctx().Arg2->qd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkLT)(void) = dStkLT;
void(*PtrStkLT)(void) = dStkLT;

void dStkGT(void) 
    {
    Ctx().Arg2->d.x = (double)(Ctx().Arg2->d.x > Ctx().Arg1->d.x);
    Ctx().Arg2->d.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkGT(void) 
    {
    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x > Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkGT(void) 
    {
    Ctx().Arg2->qd.x = (qd_real)(Ctx().Arg2->qd.x > Ctx().Arg1->qd.x);
    Ctx().Arg2->qd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkGT)(void) = dStkGT;
void(*PtrStkGT)(void) = dStkGT;

void dStkLTE(void) 
    {
    Ctx().Arg2->d.x = (double)(Ctx().Arg2->d.x <= Ctx().Arg1->d.x);
    Ctx().Arg2->d.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkLTE(void) 
    {
    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x <= Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkLTE(void) 
    {
    Ctx().Arg2->qd.x = (qd_real)(Ctx().Arg2->qd.x <= Ctx().Arg1->qd.x);
    Ctx().Arg2->qd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkLTE)(void) = dStkLTE;
void(*PtrStkLTE)(void) = dStkLTE;

void dStkGTE(void) 
    {
    Ctx().Arg2->d.x = (double)(Ctx().Arg2->d.x >= Ctx().Arg1->d.x);
    Ctx().Arg2->d.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkGTE(void) 
    {
    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x >= Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkGTE(void) 
    {
    Ctx().Arg2->qd.x = (qd_real)(Ctx().Arg2->qd.x >= Ctx().Arg1->qd.x);
    Ctx().Arg2->qd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkGTE)(void) = dStkGTE;
void(*PtrStkGTE)(void) = dStkGTE;

void dStkEQ(void) 
    {
    Ctx().Arg2->d.x = (double)(Ctx().Arg2->d.x == Ctx().Arg1->d.x);
    Ctx().Arg2->d.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkEQ(void) 
    {
    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x == Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkEQ(void) 
    {
    Ctx().Arg2->qd.x = (qd_real)(Ctx().Arg2->qd.x == Ctx().Arg1->qd.x);
    Ctx().Arg2->qd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkEQ)(void) = dStkEQ;
void(*PtrStkEQ)(void) = dStkEQ;

void dStkNE(void) 
    {
    Ctx().Arg2->d.x = (double)(Ctx().Arg2->d.x != Ctx().Arg1->d.x);
    Ctx().Arg2->d.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkNE(void) 
    {
    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x != Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkNE(void) 
    {
    Ctx().Arg2->qd.x = (qd_real)(Ctx().Arg2->qd.x != Ctx().Arg1->qd.x);
    Ctx().Arg2->qd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void (*StkNE)(void) = dStkNE;
void(*PtrStkNE)(void) = dStkNE;

void dStkOR(void) 
    {
    Ctx().Arg2->d.x = (double)(Ctx().Arg2->d.x || Ctx().Arg1->d.x);
    Ctx().Arg2->d.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkOR(void) 
    {
    double  a, b;
    a = to_double(Ctx().Arg2->dd.x);
    b = to_double(Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.x = (dd_real)(a || b);
//    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x || Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkOR(void) 
    {
    double  a, b;
    a = to_double(Ctx().Arg2->qd.x);
    b = to_double(Ctx().Arg1->qd.x);
    Ctx().Arg2->qd.x = (qd_real)(a || b);
    Ctx().Arg2->qd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkOR)(void) = dStkOR;
void(*PtrStkOR)(void) = dStkOR;

void dStkAND(void) 
    {
    Ctx().Arg2->d.x = (double)(Ctx().Arg2->d.x && Ctx().Arg1->d.x);
    Ctx().Arg2->d.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkAND(void) 
    {
    double  a, b;
    a = to_double(Ctx().Arg2->dd.x);
    b = to_double(Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.x = (dd_real)(a && b);
//    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x && Ctx().Arg1->dd.x);
    Ctx().Arg2->dd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkAND(void) 
    {
    double  a, b;
    a = to_double(Ctx().Arg2->qd.x);
    b = to_double(Ctx().Arg1->qd.x);
    Ctx().Arg2->qd.x = (qd_real)(a && b);
    //    Ctx().Arg2->dd.x = (dd_real)(Ctx().Arg2->dd.x && Ctx().Arg1->dd.x);
    Ctx().Arg2->qd.y = 0.0;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkAND)(void) = dStkAND;
void(*PtrStkAND)(void) = dStkAND;

void dStkLog(void) 
    {
    FPUcplxlog(&Ctx().Arg1->d, &Ctx().Arg1->d);
    }

void ddStkLog(void) 
    {
    Ctx().Arg1->dd = Ctx().Arg1->dd.CLog();
    }

void qdStkLog(void) 
    {
    Ctx().Arg1->qd = Ctx().Arg1->qd.CLog();
    }

void (*StkLog)(void) = dStkLog;

void FPUcplxexp(Complex *x, Complex *z) 
    {
    double e2x, siny, cosy;

    e2x = exp(x->x);
    FPUsincos(&x->y, &siny, &cosy);
    z->x = e2x * cosy;
    z->y = e2x * siny;
    }

void dStkExp(void) 
    {
    FPUcplxexp(&Ctx().Arg1->d, &Ctx().Arg1->d);
    }

void ddStkExp(void) 
    {
    Ctx().Arg1->dd = Ctx().Arg1->dd.CExp();
    }

void qdStkExp(void) 
    {
    Ctx().Arg1->qd = Ctx().Arg1->qd.CExp();
    }

void (*StkExp)(void) = dStkExp;

void dStkPwr(void) 
    {
    Ctx().Arg2->d = ComplexPower(Ctx().Arg2->d, Ctx().Arg1->d);
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void ddStkPwr(void) 
    {
    Ctx().Arg2->dd = Ctx().Arg2->dd ^ Ctx().Arg1->dd;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void qdStkPwr(void) 
    {
    Ctx().Arg2->qd = Ctx().Arg2->qd ^ Ctx().Arg1->qd;
    Ctx().Arg1--;
    Ctx().Arg2--;
    }

void(*StkPwr)(void) = dStkPwr;
void(*PtrStkPwr)(void) = dStkPwr;

void EndInit(void)
    {
    Ctx().LastInitOp = Ctx().OpPtr;
    Ctx().InitJumpIndex = Ctx().jump_cursor;
    }

void(*PtrEndInit)(void) = EndInit;

void StkJump(void)
    {
    auto& ctx = Ctx();
    const int cur = ctx.jump_cursor;

    VMTRAP(ctx.jump_cursor >= ctx.jump_index, 4001);
    ctx.OpPtr = ctx.jump_control[cur].ptrs.JumpOpPtr;
    ctx.LodPtr = ctx.jump_control[cur].ptrs.JumpLodPtr;
    ctx.StoPtr = ctx.jump_control[cur].ptrs.JumpStoPtr;
    ctx.jump_cursor = ctx.jump_control[cur].DestJumpIndex;
    }

void(*PtrStkJump)(void) = StkJump;

void dStkJumpOnFalse(void)
    {
    if (Ctx().Arg1->d.x == 0)
	StkJump();
    else
	Ctx().jump_cursor++;
    }

void ddStkJumpOnFalse(void)
    {
    if (Ctx().Arg1->dd.x == 0)
	StkJump();
    else
	Ctx().jump_cursor++;
    }

void qdStkJumpOnFalse(void)
    {
    if (Ctx().Arg1->qd.x == 0)
	StkJump();
    else
	Ctx().jump_cursor++;
    }

void(*StkJumpOnFalse)(void) = dStkJumpOnFalse;
void(*PtrStkJumpOnFalse)(void) = dStkJumpOnFalse;

void dStkJumpOnTrue(void)
    {
    if (Ctx().Arg1->d.x)
	StkJump();
    else
	Ctx().jump_cursor++;
    }

void ddStkJumpOnTrue(void)
    {
    if (Ctx().Arg1->dd.x != 0.0)
	StkJump();
    else
	Ctx().jump_cursor++;
    }

void qdStkJumpOnTrue(void)
    {
    if (Ctx().Arg1->qd.x != 0.0)
	StkJump();
    else
	Ctx().jump_cursor++;
    }

void(*StkJumpOnTrue)(void) = dStkJumpOnTrue;
void(*PtrStkJumpOnTrue)(void) = dStkJumpOnTrue;

void StkJumpLabel(void)
    {
    Ctx().jump_cursor++;
    }

void(*PtrStkJumpLabel)(void) = StkJumpLabel;


