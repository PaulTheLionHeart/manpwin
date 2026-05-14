/*
   UNDO.CPP a module to store and reload parameters for undo.
    
    Written in MICROSOFT VISUAL 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <Windows.h>
#include "Undo.h"
#include "BigDouble.h"
#include "Manp.h"

/**************** Big Number Globals *********************/

//extern	int	decimals;
//extern	BYTE	BigNumFlag;		// True if bignum used
/**************** Big Number Globals *********************/

//extern	WORD	type;			// fractal type

//extern	void	ConvertString2Bignum(mpfr_t num, char *s);
struct	UNDO	undo, TempUNDO;

/**************************************************************************
	Write to Undo Buffer
**************************************************************************/

void	CManp::InitUndo(void)
    {
    undo.BigNumFlag = FALSE;
    undo.hor = hor;
    undo.vert = vert;
    undo.mandel_width = mandel_width;
    }

/**************************************************************************
	Undo
**************************************************************************/

void	CManp::Redo(struct UNDO *u1)
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

void	CManp::Undo(struct UNDO *u1)
    {
    u1->BigNumFlag = BigNumFlag;
    u1->type = type;
    if (BigNumFlag)
	{
	BigHor.ToString(u1->UndoBig_hor, SIZEOF_BF_VARS, false);
	BigVert.ToString(u1->UndoBig_vert, SIZEOF_BF_VARS, false);
	BigWidth.SafeSprintf(u1->UndoBig_width, SIZEOF_BF_VARS, "%.20Re");
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

void	CManp::SaveUndo(BOOL current)
    {
    Undo((current) ? &undo : &TempUNDO);
    }

/**************************************************************************
	Read from Undo Buffer
**************************************************************************/

void	CManp::LoadUndo(BOOL UndoFlag)
    {
    Redo((UndoFlag) ? &TempUNDO : &undo);
    }

