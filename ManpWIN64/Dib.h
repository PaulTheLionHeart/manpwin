// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include <vector>

//#include <Windows.h>
#if !defined(AFX_DIB_H__955DFDBD_51F6_419C_BE14_06287AE21568__INCLUDED_)
#define AFX_DIB_H__955DFDBD_51F6_419C_BE14_06287AE21568__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Dib Error Codes
#define NODIBMEMORY  1
#define NOPIXELMEMORY  2

class CDib
    {
    public:
    CDib();
    CDib(const CDib &);	// Copy Constructor
    virtual		~CDib();
    CDib & operator =(const CDib &);	// Assignment Operator

    BYTE		*InitDib(int Width, int Height, WORD bits);
    BYTE		*InitDib(int Width, int Height, BYTE* palette, WORD bits);
    BYTE		*InitDib(LPBITMAPINFO pDib);
    BYTE		*InitDib(int Width, int Height);
    void		CloseDibPtrs(void);
    void		DibTo24(const CDib &);
    void		DibTo24(LPBITMAPINFO);
    void		DibTo24(LPBITMAPINFO, BYTE *);
    void		DibTo24(void);
    void		Text2Dib(HDC, RECT *, TCHAR *);
    void		Text2Dib(HDC, RECT *, LOGFONT *, TCHAR *);
    void		Text2Dib(HDC, RECT *, COLORREF, COLORREF, LOGFONT *, int, TCHAR *);
    void		ClearDib(DWORD);
    void		ClearDib(int Red, int Green, int Blue);
//    void		CountColours(void);
//    DWORD ColourCount(void (*Progress)(int, int, char *));
//    DWORD ColourCount(void);
    DWORD		Size()		{ return DibSize; };
    WORD		Width()		{ return DibWidth; };
    WORD		Height()	{ return DibHeight; };
    WORD		Bits()		{ return BitsPerPixel; };
    DWORD		NumColours()	{ return DibNumColours; };

    int			DibErrorCode;
    LPBITMAPINFO	pDibInf;		// pointer to the DIB info
    std::vector<BYTE>	DibPixels;		// the device-independent bitmap  pixels
    DWORD		SizeImage;		// Size, in bytes, of the image.
    WORD		DibWidth;		// the size of the image in pixels
    WORD		DibHeight;
    WORD		BitsPerPixel;
    DWORD		DibNumColours;
    WORD		WidthBytes;		// Width, in bytes, of 1 line.

    private:
    DWORD		DibSize;		// Size, in bytes, of the DIB
    };

#endif // !defined(AFX_DIB_H__955DFDBD_51F6_419C_BE14_06287AE21568__INCLUDED_)
