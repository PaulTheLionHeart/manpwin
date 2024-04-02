// Fract.cpp: implementation of the CFract class.
//
//////////////////////////////////////////////////////////////////////

#include "Fract.h"
#include ".\parser\TrigFn.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFract::CFract()
    {
    }

CFract::~CFract()
    {
    }

//////////////////////////////////////////////////////////////////////
//	Initialise Windows Device Independent Bitmap
//////////////////////////////////////////////////////////////////////

int	CFract::LoadFnArray(void)	// initialise function array
    {
    int	i;
    CTrigFn	TrigFn;

    for (i = 0; i < MAXFUNCTIONS, *(TrigFn.FunctList[i]); i++)
	fnptr[i] = TrigFn.FunctList[i];
    TotalFunctions = i;
    return TotalFunctions;
    }

int	CFract::InitData(void)	// initialise function array
    {
    FractName = "";	// you guessed it
    NumParam = 0;	// Number of parameters required for fractal
    NumFunct = 0;	// Number of functions required for fractal
//    rqlim = 4;		// bailout limit
    Fn1 = "";
    Fn2 = "";		// function name pointers
    return 1;
    }



