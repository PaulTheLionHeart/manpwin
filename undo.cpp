/*
   UNDO.CPP a module to store and reload parameters for undo.
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Undo.h"
#include "BigDouble.h"

/**************** Big Number Globals *********************/

extern	int	decimals;
extern	BYTE	BigNumFlag;		// True if bignum used
extern	BigDouble   BigHor, BigVert, BigWidth;
/**************** Big Number Globals *********************/

extern	double	hor;			// horizontal address 
extern	double	vert;			// vertical address 
extern	double	mandel_width;		// width of display 
extern	WORD	type;			// fractal type

extern	void	ConvertString2Bignum(mpfr_t num, char *s);
extern	void	ConvertBignum2String(char *s, mpfr_t num);

struct	UNDO	undo, TempUNDO;

/**************************************************************************
	Write to Undo Buffer
**************************************************************************/

void	InitUndo(void)

    {
    undo.BigNumFlag = FALSE;
    undo.hor = hor;
    undo.vert = vert;
    undo.mandel_width = mandel_width;
    }

/**************************************************************************
	Undo
**************************************************************************/

void	Redo(struct UNDO *u1)

    {
    BigNumFlag = u1->BigNumFlag;
    type = u1->type;
    if (BigNumFlag)
	{
	ConvertString2Bignum(BigHor.x, u1->UndoBig_hor);
	ConvertString2Bignum(BigVert.x, u1->UndoBig_vert);
	ConvertString2Bignum(BigWidth.x, u1->UndoBig_width);
	decimals = u1->dec;
	}
    else
	{
	hor = u1->hor;
	vert = u1->vert;
	mandel_width = u1->mandel_width;
	}
    }

/**************************************************************************
	Undo
**************************************************************************/

void	Undo(struct UNDO *u1)

    {
    u1->BigNumFlag = BigNumFlag;
    u1->type = type;
    if (BigNumFlag)
	{
	ConvertBignum2String(u1->UndoBig_hor, BigHor.x);
	ConvertBignum2String(u1->UndoBig_vert, BigVert.x);
	mpfr_sprintf(u1->UndoBig_width, "%.20Re", BigWidth.x);
//	ConvertBignum2String(u1->UndoBig_width, BigWidth.x);
	u1->dec = decimals;
	}
    else
	{
	u1->hor = hor;
	u1->vert = vert;
	u1->mandel_width = mandel_width;
	}
    }

/**************************************************************************
	Write to Undo Buffer
**************************************************************************/

void	SaveUndo(BOOL current)

    {
    Undo((current) ? &undo : &TempUNDO);
    }

/**************************************************************************
	Read from Undo Buffer
**************************************************************************/

void	LoadUndo(BOOL UndoFlag)

    {
    Redo((UndoFlag) ? &TempUNDO : &undo);
    }

