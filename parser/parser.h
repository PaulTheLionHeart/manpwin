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

extern	void dRandom(void);
extern	void ddRandom(void);
extern	void qdRandom(void);
extern	void RandomSeed(void);
extern	void dStkSRand(void);
extern	void ddStkSRand(void);
extern	void (*StkSRand)(void);
extern	void dStkLodDup();
extern	void dStkLodSqr();
extern	void dStkLodSqr2();
extern	void dStkLodDbl();
extern	void dStkSqr0();
extern	void dStkSqr3();
extern	void dStkAbs(void);
extern	void ddStkAbs(void);
extern	void (*StkAbs)(void);
extern	void dStkSqr(void);
extern	void ddStkSqr(void);
extern	void (*StkSqr)(void);
extern	void dStkAdd(void);
extern	void ddStkAdd(void);
extern	void (*StkAdd)(void);
extern	void dStkSub(void);
extern	void ddStkSub(void);
extern	void (*StkSub)(void);
extern	void dStkConj(void);
extern	void ddStkConj(void);
extern	void (*StkConj)(void);
extern	void dStkFloor(void);
extern	void ddStkFloor(void);
extern	void (*StkFloor)(void);
extern	void dStkCeil(void);
extern	void ddStkCeil(void);
extern	void (*StkCeil)(void);
extern	void dStkTrunc(void);
extern	void ddStkTrunc(void);
extern	void (*StkTrunc)(void);
extern	void dStkRound(void);
extern	void ddStkRound(void);
extern	void (*StkRound)(void);
extern	void dStkZero(void);
extern	void ddStkZero(void);
extern	void (*StkZero)(void);
extern	void dStkOne(void);
extern	void ddStkOne(void);
extern	void (*StkOne)(void);
extern	void dStkReal(void);
extern	void ddStkReal(void);
extern	void (*StkReal)(void);
extern	void dStkImag(void);
extern	void ddStkImag(void);
extern	void (*StkImag)(void);
extern	void dStkNeg(void);
extern	void ddStkNeg(void);
extern	void (*StkNeg)(void);
extern	void dStkMul(void);
extern	void ddStkMul(void);
extern	void (*StkMul)(void);
extern	void dStkDiv(void);
extern	void ddStkDiv(void);
extern	void (*StkDiv)(void);
extern	void dStkMod(void);
extern	void ddStkMod(void);
extern	void (*StkMod)(void);
extern	void StkSto(void);
extern	void (*PtrStkSto)(void);
extern	void StkLod(void);
extern	void (*PtrStkLod)(void);
extern	void StkClr(void);
extern	void (*PtrStkClr)(void);
extern	void dStkFlip(void);
extern	void ddStkFlip(void);
extern	void (*StkFlip)(void);
extern	void dStkSin(void);
extern	void ddStkSin(void);
extern	void (*StkSin)(void);
extern	void dStkTan(void);
extern	void ddStkTan(void);
extern	void (*StkTan)(void);
extern	void dStkTanh(void);
extern	void ddStkTanh(void);
extern	void (*StkTanh)(void);
extern	void dStkCoTan(void);
extern	void ddStkCoTan(void);
extern	void (*StkCoTan)(void);
extern	void dStkCoTanh(void);
extern	void ddStkCoTanh(void);
extern	void (*StkCoTanh)(void);
extern	void dStkRecip(void);
extern	void (*StkRecip)(void);
extern	void StkIdent(void);
extern	void (*dStkIdent)(void);
extern	void dStkSinh(void);
extern	void ddStkSinh(void);
extern	void (*StkSinh)(void);
extern	void dStkCos(void);
extern	void ddStkCos(void);
extern	void (*StkCos)(void);
extern	void dStkCosXX(void);
extern	void ddStkCosXX(void);
extern	void (*StkCosXX)(void);
extern	void dStkCosh(void);
extern	void ddStkCosh(void);
extern	void (*StkCosh)(void);
extern	void dStkASin(void);
extern	void ddStkASin(void);
extern	void (*StkASin)(void);
extern	void dStkASinh(void);
extern	void ddStkASinh(void);
extern	void (*StkASinh)(void);
extern	void dStkACos(void);
extern	void ddStkACos(void);
extern	void (*StkACos)(void);
extern	void dStkACosh(void);
extern	void ddStkACosh(void);
extern	void (*StkACosh)(void);
extern	void dStkATan(void);
extern	void ddStkATan(void);
extern	void (*StkATan)(void);
extern	void dStkATanh(void);
extern	void ddStkATanh(void);
extern	void (*StkATanh)(void);
extern	void dStkSqrt(void);
extern	void ddStkSqrt(void);
extern	void (*StkSqrt)(void);
extern	void dStkCAbs(void);
extern	void ddStkCAbs(void);
extern	void (*StkCAbs)(void);
extern	void dStkLT(void);
extern	void ddStkLT(void);
extern	void (*StkLT)(void);
extern	void dStkGT(void);
extern	void ddStkGT(void);
extern	void (*StkGT)(void);
extern	void dStkLTE(void);
extern	void ddStkLTE(void);
extern	void (*StkLTE)(void);
extern	void dStkGTE(void);
extern	void ddStkGTE(void);
extern	void (*StkGTE)(void);
extern	void dStkEQ(void);
extern	void ddStkEQ(void);
extern	void (*StkEQ)(void);
extern	void dStkNE(void);
extern	void ddStkNE(void);
extern	void (*StkNE)(void);
extern	void dStkOR(void);
extern	void ddStkOR(void);
extern	void (*StkOR)(void);
extern	void dStkAND(void);
extern	void ddStkAND(void);
extern	void (*StkAND)(void);
extern	void dStkLog(void);
extern	void ddStkLog(void);
extern	void (*StkLog)(void);
extern	void dStkExp(void);
extern	void ddStkExp(void);
extern	void (*StkExp)(void);
extern	void dStkPwr(void);
extern	void ddStkPwr(void);
extern	void (*StkPwr)(void);

// QD Parser functions added PHD 12/06/2025

extern	void qdStkAdd(void);
extern	void qdStkSub(void);
extern	void qdStkNeg(void);
extern	void qdStkMul(void);
extern	void qdStkSin(void);
extern	void qdStkSinh(void);
extern	void qdStkLT(void);
extern	void qdStkLTE(void);
extern	void qdStkMod(void);
extern	void qdStkSqr(void);
extern	void qdStkCos(void);
extern	void qdStkCosh(void);
extern	void qdStkLog(void);
extern	void qdStkExp(void);
extern	void qdStkPwr(void);
extern	void qdStkDiv(void);
extern	void qdStkAbs(void);
extern	void qdStkReal(void);
extern	void qdStkImag(void);
extern	void qdStkConj(void);
extern	void qdStkFlip(void);
extern	void qdStkTan(void);
extern	void qdStkTanh(void);
extern	void qdStkCoTan(void);
extern	void qdStkCoTanh(void);
extern	void qdStkCosXX(void);
extern	void qdStkGT(void);
extern	void qdStkGTE(void);
extern	void qdStkEQ(void);
extern	void qdStkNE(void);
extern	void qdStkAND(void);
extern	void qdStkOR(void);
extern	void qdStkSRand(void);
extern	void qdStkASin(void);
extern	void qdStkASinh(void);
extern	void qdStkACos(void);
extern	void qdStkACosh(void);
extern	void qdStkATan(void);
extern	void qdStkATanh(void);
extern	void qdStkCAbs(void);
extern	void qdStkSqrt(void);
extern	void qdStkZero(void);
extern	void qdStkFloor(void);
extern	void qdStkCeil(void);
extern	void qdStkTrunc(void);
extern	void qdStkRound(void);
extern	void qdStkOne(void);

