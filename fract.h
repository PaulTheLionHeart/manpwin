// Fract1.h: interface for the CFract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRACT1_H__6FDA296F_2F52_425E_9566_83DADB05AAFA__INCLUDED_)
#define AFX_FRACT1_H__6FDA296F_2F52_425E_9566_83DADB05AAFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	MAXFUNCTIONS	60

class CFract  
{
public:
	CFract();
	virtual ~CFract();

    int		LoadFnArray(void);			// initialise function array
    int		InitData(void);				// initialise all public data
    void	LoadFunction(char *, int);		// get pointer to trig functions (Fn1, Fn2)

    char	*FractName;				// you guessed it
    double	*ParamValue[10];			// pointers to the parameters to fractal
    char	*ParamName[10];				// pointers to the parameters names
    int		NumParam;				// Number of parameters required for fractal
    int		NumFunct;				// Number of functions required for fractal
    int		Fn1Index;				// pointer to first function
    int		Fn2Index;				// pointer to second function
    double	*rqlim;					// pointer to bailout limit
    char	*fnptr[MAXFUNCTIONS];			// names of the functions
    char	*Fn1, *Fn2;				// function name pointers
    int		TotalFunctions;				// how many functions exist in the list?
    int		TierazonPtr = 1, TierazonNum = 0;
    int		MandelDerivPtr = 1, MandelDerivNum = 0;
    int		SlopeDerivPtr = 0, SlopeDerivNum = 0;
    int		SlopeFwdDiffPtr = 0, SlopeFwdDiffNum = 0;

};

#endif // !defined(AFX_FRACT1_H__6FDA296F_2F52_425E_9566_83DADB05AAFA__INCLUDED_)
