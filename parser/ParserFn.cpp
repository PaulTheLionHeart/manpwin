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
#include "ParserTemplate.h"
#include "ParserFnTemplate.h"
#include "..\ManpWIN64\Complex.h"
#include "..\ManpWIN64\DDComplex.h"
#include "..\ManpWIN64\fract.h"
#include "..\ManpWIN64\Manp.h"

//CParser* g_activeParser = nullptr;   // One pointer per thread later; single-thread for now

extern	int Randomized;

// use the following when only float functions are implemented to get MP math and Integer math

// Random number code, MCP 11-21-91
static	unsigned long RandNum;

unsigned long NewRandNum(void)
    {
    return(RandNum = ((RandNum << 15) + rand15()) ^ RandNum);
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

/**************************************************************************
SRand
**************************************************************************/

void ParserStkSRand(void)
    {
    DispatchUnary<&TStkSRand<Complex>, &TStkSRand<DDComplex>, &TStkSRand<QDComplex>>();
    }

void(*StkSRand)(void) = ParserStkSRand;

void ParserStkAbs(void)
    {
    DispatchUnary<&TStkAbs<Complex>, &TStkAbs<DDComplex>, &TStkAbs<QDComplex>>();
    }

void(*StkAbs)(void) = ParserStkAbs;

void ParserStkSqr(void)
    {
    DispatchUnary<&TStkSqr<Complex>, &TStkSqr<DDComplex>, &TStkSqr<QDComplex>>();
    }

void(*StkSqr)(void) = ParserStkSqr;

/**************************************************************************
    Temporary VM compatibility wrappers

    These keep old parser references alive while we migrate tables
    from dStkXXX(void) to ParserStkXXX(void).

    Do not put semantic code here.
**************************************************************************/

void ParserStkAdd(void)
    {
    DispatchBinaryPop<&TStkAdd<Complex>, &TStkAdd<DDComplex>, &TStkAdd<QDComplex>>();
    }

void(*StkAdd)(void) = ParserStkAdd;
void(*PtrStkAdd)(void) = ParserStkAdd;

void ParserStkSub(void)
    {
    DispatchBinaryPop<&TStkSub<Complex>, &TStkSub<DDComplex>, &TStkSub<QDComplex>>();
    }

void(*StkSub)(void) = ParserStkSub;
void(*PtrStkSub)(void) = ParserStkSub;

void ParserStkConj(void)
    {
    DispatchUnary<&TStkConj<Complex>, &TStkConj<DDComplex>, &TStkConj<QDComplex>>();
    }

void(*StkConj)(void) = ParserStkConj;

void ParserStkFloor(void)
    {
    DispatchUnary<&TStkFloor<Complex>, &TStkFloor<DDComplex>, &TStkFloor<QDComplex>>();
    }

void(*StkFloor)(void) = ParserStkFloor;

void ParserStkCeil(void)
    {
    DispatchUnary<&TStkCeil<Complex>, &TStkCeil<DDComplex>, &TStkCeil<QDComplex>>();
    }

void(*StkCeil)(void) = ParserStkCeil;

void ParserStkRound(void)
    {
    DispatchUnary<&TStkRound<Complex>, &TStkRound<DDComplex>, &TStkRound<QDComplex>>();
    }

void(*StkRound)(void) = ParserStkRound;

void ParserStkTrunc(void)
    {
    DispatchUnary<&TStkTrunc<Complex>, &TStkTrunc<DDComplex>, &TStkTrunc<QDComplex>>();
    }

void(*StkTrunc)(void) = ParserStkTrunc;

void ParserStkZero(void)
    {
    DispatchUnary<&TStkZero<Complex>, &TStkZero<DDComplex>, &TStkZero<QDComplex>>();
    }

void(*StkZero)(void) = ParserStkZero;

void ParserStkOne(void)
    {
    DispatchUnary<&TStkOne<Complex>, &TStkOne<DDComplex>, &TStkOne<QDComplex>>();
    }

void(*StkOne)(void) = ParserStkOne;

void ParserStkReal(void)
    {
    DispatchUnary<&TStkReal<Complex>, &TStkReal<DDComplex>, &TStkReal<QDComplex>>();
    }

void(*StkReal)(void) = ParserStkReal;

void ParserStkImag(void)
    {
    DispatchUnary<&TStkImag<Complex>, &TStkImag<DDComplex>, &TStkImag<QDComplex>>();
    }

void(*StkImag)(void) = ParserStkImag;

void ParserStkNeg(void)
    {
    DispatchUnary<&TStkNeg<Complex>, &TStkNeg<DDComplex>, &TStkNeg<QDComplex>>();
    }

void(*StkNeg)(void) = ParserStkNeg;

void ParserStkMul(void)
    {
    DispatchBinaryPop<&TStkMul<Complex>, &TStkMul<DDComplex>, &TStkMul<QDComplex>>();
    }

void(*StkMul)(void) = ParserStkMul;
void(*PtrStkMul)(void) = ParserStkMul;

void ParserStkDiv(void)
    {
    DispatchBinaryPop<&TStkDiv<Complex>, &TStkDiv<DDComplex>, &TStkDiv<QDComplex>>();
    }

void(*StkDiv)(void) = ParserStkDiv;
void(*PtrStkDiv)(void) = ParserStkDiv;

void ParserStkMod(void)
    {
    DispatchUnary<&TStkMod<Complex>, &TStkMod<DDComplex>, &TStkMod<QDComplex>>();
    }

void(*StkMod)(void) = ParserStkMod;
void(*PtrStkMod)(void) = ParserStkMod;

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

// MCP 4-9-91, Added Flip()
void ParserStkFlip(void)
    {
    DispatchUnary<&TStkFlip<Complex>, &TStkFlip<DDComplex>, &TStkFlip<QDComplex>>();
    }

void(*StkFlip)(void) = ParserStkFlip;

void ParserStkSin(void)
    {
    DispatchUnary<&TStkSin<Complex>, &TStkSin<DDComplex>, &TStkSin<QDComplex>>();
    }

void(*StkSin)(void) = ParserStkSin;

// The following functions are supported by both the parser and for fn variable replacement. TIW 04-22-91

void ParserStkTan(void)
    {
    DispatchUnary<&TStkTan<Complex>, &TStkTan<DDComplex>, &TStkTan<QDComplex>>();
    }

void(*StkTan)(void) = ParserStkTan;

void ParserStkTanh(void)
    {
    DispatchUnary<&TStkTanh<Complex>, &TStkTanh<DDComplex>, &TStkTanh<QDComplex>>();
    }

void(*StkTanh)(void) = ParserStkTanh;

void ParserStkCoTan(void)
    {
    DispatchUnary<&TStkCoTan<Complex>, &TStkCoTan<DDComplex>, &TStkCoTan<QDComplex>>();
    }

void(*StkCoTan)(void) = ParserStkCoTan;

void ParserStkCoTanh(void)
    {
    DispatchUnary<&TStkCoTanh<Complex>, &TStkCoTanh<DDComplex>, &TStkCoTanh<QDComplex>>();
    }

void(*StkCoTanh)(void) = ParserStkCoTanh;

// The following functions are not directly used by the parser - support
// for the parser was not provided because the existing parser language
// represents these quite easily. They are used for fn variable support
// in miscres.c but are placed here because they follow the pattern of
// the other parser functions. TIW 04-22-91

/**************************************************************************
    Reciprocal
**************************************************************************/

void ParserStkRecip(void)
    {
    DispatchUnary<&TStkRecip<Complex>, &TStkRecip<DDComplex>, &TStkRecip<QDComplex>>();
    }

void(*StkRecip)(void) = ParserStkRecip;

void ParserStkIdent(void)
    {
    DispatchUnary<&TStkIdent<Complex>, &TStkIdent<DDComplex>, &TStkIdent<QDComplex>>();
    }

void(*pStkIdent)(void) = ParserStkIdent;

// End TIW 04-22-91

void ParserStkSinh(void)
    {
    DispatchUnary<&TStkSinh<Complex>, &TStkSinh<DDComplex>, &TStkSinh<QDComplex>>();
    }

void(*StkSinh)(void) = ParserStkSinh;

void ParserStkCos(void)
    {
    DispatchUnary<&TStkCos<Complex>, &TStkCos<DDComplex>, &TStkCos<QDComplex>>();
    }

void(*StkCos)(void) = ParserStkCos;

void ParserStkCosXX(void)
    {
    DispatchUnary< &TStkCosXX<Complex>, &TStkCosXX<DDComplex>, &TStkCosXX<QDComplex>>();
    }

void(*StkCosXX)(void) = ParserStkCosXX;

void ParserStkCosh(void)
    {
    DispatchUnary<&TStkCosh<Complex>, &TStkCosh<DDComplex>, &TStkCosh<QDComplex>>();
    }

void(*StkCosh)(void) = ParserStkCosh;

void ParserStkASinh(void)
    {
    DispatchUnary<&TStkASinh<Complex>, &TStkASinh<DDComplex>, &TStkASinh<QDComplex>>();
    }

void(*StkASinh)(void) = ParserStkASinh;

void ParserStkASin(void)
    {
    DispatchUnary<&TStkASin<Complex>, &TStkASin<DDComplex>, &TStkASin<QDComplex>>();
    }

void(*StkASin)(void) = ParserStkASin;

void ParserStkACos(void)
    {
    DispatchUnary<&TStkACos<Complex>, &TStkACos<DDComplex>, &TStkACos<QDComplex>>();
    }

void(*StkACos)(void) = ParserStkACos;

void ParserStkACosh(void)
    {
    DispatchUnary<&TStkACosh<Complex>, &TStkACosh<DDComplex>, &TStkACosh<QDComplex>>();
    }

void(*StkACosh)(void) = ParserStkACosh;

void ParserStkATan(void)
    {
    DispatchUnary<&TStkATan<Complex>, &TStkATan<DDComplex>, &TStkATan<QDComplex>>();
    }

void(*StkATan)(void) = ParserStkATan;

void ParserStkATanh(void)
    {
    DispatchUnary<&TStkATanh<Complex>, &TStkATanh<DDComplex>, &TStkATanh<QDComplex>>();
    }

void(*StkATanh)(void) = ParserStkATanh;

void ParserStkSqrt(void)
    {
    DispatchUnary<&TStkSqrt<Complex>, &TStkSqrt<DDComplex>, &TStkSqrt<QDComplex>>();
    }

void(*StkSqrt)(void) = ParserStkSqrt;

void ParserStkCAbs(void)
    {
    DispatchUnary<&TStkCAbs<Complex>, &TStkCAbs<DDComplex>, &TStkCAbs<QDComplex>>();
    }

void(*StkCAbs)(void) = ParserStkCAbs;

// TIW end arc functions 11-25-94

void ParserStkLT(void)
    {
    DispatchBinaryPop<&TStkLT<Complex>, &TStkLT<DDComplex>, &TStkLT<QDComplex>>();  
    }

void(*StkLT)(void) = ParserStkLT;
void(*PtrStkLT)(void) = ParserStkLT;

void ParserStkGT(void)
    {
    DispatchBinaryPop<&TStkGT<Complex>, &TStkGT<DDComplex>, &TStkGT<QDComplex>>();
    }

void(*StkGT)(void) = ParserStkGT;
void(*PtrStkGT)(void) = ParserStkGT;

void ParserStkLTE(void)
    {
    DispatchBinaryPop<&TStkLTE<Complex>, &TStkLTE<DDComplex>, &TStkLTE<QDComplex>>();
    }

void(*StkLTE)(void) = ParserStkLTE;
void(*PtrStkLTE)(void) = ParserStkLTE;

void ParserStkGTE(void)
    {
    DispatchBinaryPop<&TStkGTE<Complex>, &TStkGTE<DDComplex>, &TStkGTE<QDComplex>>();
    }

void(*StkGTE)(void) = ParserStkGTE;
void(*PtrStkGTE)(void) = ParserStkGTE;

void ParserStkEQ(void)
    {
    DispatchBinaryPop<&TStkEQ<Complex>, &TStkEQ<DDComplex>, &TStkEQ<QDComplex>>();
    }

void(*StkEQ)(void) = ParserStkEQ;
void(*PtrStkEQ)(void) = ParserStkEQ;

void ParserStkNE(void)
    {
    DispatchBinaryPop<&TStkNE<Complex>, &TStkNE<DDComplex>, &TStkNE<QDComplex>>();
    }

void(*StkNE)(void) = ParserStkNE;
void(*PtrStkNE)(void) = ParserStkNE;

void ParserStkOR(void)
    {
    DispatchBinaryPop<&TStkOR<Complex>, &TStkOR<DDComplex>, &TStkOR<QDComplex>>();
    }

void(*StkOR)(void) = ParserStkOR;
void(*PtrStkOR)(void) = ParserStkOR;

void ParserStkAND(void)
    {
    DispatchBinaryPop<&TStkAND<Complex>, &TStkAND<DDComplex>, &TStkAND<QDComplex>>();
    }

void(*StkAND)(void) = ParserStkAND;
void(*PtrStkAND)(void) = ParserStkAND;

void ParserStkLog(void)
    {
    DispatchUnary<&TStkLog<Complex>, &TStkLog<DDComplex>, &TStkLog<QDComplex>>();
    }

void(*StkLog)(void) = ParserStkLog;

void ParserStkExp(void)
    {
    DispatchUnary<&TStkExp<Complex>, &TStkExp<DDComplex>, &TStkExp<QDComplex>>();
    }

void(*StkExp)(void) = ParserStkExp;

void ParserStkPwr(void)
    {
    DispatchBinaryPop<&TStkPwr<Complex>, &TStkPwr<DDComplex>, &TStkPwr<QDComplex>>();
    }

void(*StkPwr)(void) = ParserStkPwr;
void(*PtrStkPwr)(void) = ParserStkPwr;

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

void ParserStkJumpOnTrue(void)
    {
    DispatchUnary<&TStkJumpOnTrue<Complex>, &TStkJumpOnTrue<DDComplex>, &TStkJumpOnTrue<QDComplex>>();
    }

void(*StkJumpOnTrue)(void) = ParserStkJumpOnTrue;
void(*PtrStkJumpOnTrue)(void) = ParserStkJumpOnTrue;

void ParserStkJumpOnFalse(void)
    {
    DispatchUnary<&TStkJumpOnFalse<Complex>, &TStkJumpOnFalse<DDComplex>, &TStkJumpOnFalse<QDComplex>>();
    }

void(*StkJumpOnFalse)(void) = ParserStkJumpOnFalse;
void(*PtrStkJumpOnFalse)(void) = ParserStkJumpOnFalse;

void StkJumpLabel(void)
    {
    Ctx().jump_cursor++;
    }

void(*PtrStkJumpLabel)(void) = StkJumpLabel;


