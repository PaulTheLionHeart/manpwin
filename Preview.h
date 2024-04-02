// Preview1.h: interface for the CPreview class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREVIEW1_H__8145BF6B_53F4_469E_B8B4_66674A75422C__INCLUDED_)
#define AFX_PREVIEW1_H__8145BF6B_53F4_469E_B8B4_66674A75422C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
//#include "DibMacro.h"
#include "Dib.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#ifndef	WHITE
#define WHITE	0x00ffffff
#endif

//#define	PREVIEW_HEIGHT		177
//#define	PREVIEW_WIDTH		220

#define	HORLINE			0		// define the line types for things like rotate variable angle and trapezoidal correction
#define	VERTLINE		1
#define	BOTHLINE		2
#define	GRIDLINES		3

#define	NUMGRIDLINES		4

class CPreview  
{
public:
	CPreview();
	virtual ~CPreview();

	BOOL	OpenDlgFlag;			// is image dialog box open?
	WORD	PreviewHeight;
	WORD	PreviewWidth;
	WORD	HorOffset;
	WORD	VertOffset;
	CDib	PreviewDib;			// Device Independent Bitmap class instance

//	void	PreviewScale(int Width, int Height);
	int	InitPreview(HWND hwnd);
	int	CopyPreview(HWND hwnd, CDib *Dib);
	int	PreviewGrid(HWND hwnd, int type);
	int	CopyBackground(HWND hwnd, short r, short g, short b);
	int	Preview(HWND hwnd);
	int	ClosePreview(void);


};

#endif // !defined(AFX_PREVIEW1_H__8145BF6B_53F4_469E_B8B4_66674A75422C__INCLUDED_)
