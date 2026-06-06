/*
    Clipboard.cpp - Clipboard routines.

    Written in MICROSOFT 'C++' by Paul de Leeuw.
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manpwin.h"
#include "manp.h"
#include "Dib.h"

#define	DestroyAllObjects()	{	\
	if(hdest != NULL && lpbd != NULL) GlobalUnlock(hdest);\
	if(hsource != NULL && lpbs != NULL) GlobalUnlock(hsource);\
	if(hdest != NULL) GlobalFree(hdest);\
}

#define	RGB_RED			0
#define	RGB_GREEN		1
#define	RGB_BLUE		2
#define	RGB_SIZE		3

extern	DWORD			biCompression;			// BMP type compression
extern	int			file_type;
extern	HPALETTE 		hpal;
extern	HANDLE  	 	BitMap; 			// Bitmap Handle
extern	BOOL	Picture_active_flag;

	BYTE	*CreateDib(WORD, WORD, BYTE *, WORD);
//extern	int	DibBlt(HDC,int,int,int, int,HANDLE,int,int);
BYTE		*GetOrthoPalette(BYTE *);
//extern	int	Force24Bit(HWND, char *);

/**************************************************************************
	Copy image to clipboard
**************************************************************************/

int CopyPictureToClipboard(HWND hwnd)
    {
    LPBITMAPINFOHEADER lpbs=NULL,lpbd=NULL;
    BYTE	*hdest=NULL;
    HANDLE	hsource=NULL;
    WORD	colours_used;
    DWORD	PictureSize, Offset;
    BYTE	palette[768];

    if(gManp->Dib.BitsPerPixel==24)
	GetOrthoPalette(palette);

    if((hdest=CreateDib((WORD)gManp->width, (WORD)gManp->height, (BYTE *)palette, (WORD)gManp->Dib.BitsPerPixel))==NULL)
	{
	DestroyAllObjects();
	return(-1);
	}

    if((lpbd=(LPBITMAPINFOHEADER)GlobalLock(hdest))==NULL) 
	{
	DestroyAllObjects();
	return(-1);
	}

    switch (gManp->Dib.BitsPerPixel)
	{
	case 1:
	    colours_used = 2;
	    break;
	case 4:
	    colours_used = 16;
	    break;
	case 8:
	    colours_used = 256;
	    break;
	default:
	// A 24 bitcount DIB has no color table so use 256
	    colours_used = 0;
	}

    PictureSize = lpbd->biSizeImage;
    Offset = (DWORD)sizeof(BITMAPINFOHEADER)+
			    (DWORD)colours_used*(DWORD)sizeof(RGBQUAD);

    memcpy((char *)lpbd + Offset,(char *)gManp->Dib.DibPixels.data(), PictureSize);	// picture

    lpbd=NULL;
    OpenClipboard(hwnd);
    EmptyClipboard();

    SetClipboardData(CF_DIB,hdest);
    SetClipboardData(CF_PALETTE,hpal);
    CloseClipboard();

    hdest=NULL;

    DestroyAllObjects();

    return(0);
    }

/**************************************************************************
	Linear 256 Colour Palette
**************************************************************************/

#define	ORTHOMATCH(r,g,b)  (((r) & 0x00e0) | (((g) >> 3) & 0x001c)  | (((b) >> 6) & 0x0003))

BYTE *GetOrthoPalette(BYTE *buffer)
    {
    static int expandbits32[]={ 32,64,96,128,160,192,224,255 };
    static int expandbits16[]={ 64,128,192,255 };
    static char palette[768];
    int r,g,b,i,rr,gg,bb;

    for(r=0;r<8;++r) 
	{
	for(g=0;g<8;++g) 
	    {
	    for(b=0;b<4;++b) 
		{
		rr=expandbits32[r];
		gg=expandbits32[g];
		bb=expandbits16[b];

		i=ORTHOMATCH(r<<5,g<<5,b<<6);
		palette[i*RGB_SIZE+RGB_RED]=(char)rr;
		palette[i*RGB_SIZE+RGB_GREEN]=(char)gg;
		palette[i*RGB_SIZE+RGB_BLUE]=(char)bb;
		}
	    }
	}

    if(buffer != NULL) 
	memcpy(buffer,palette,768);
    return((BYTE *)palette);
    }

/**************************************************************************
	Round bit depths up to legal values for a bitmap
**************************************************************************/

WORD AdjustDIBits(WORD n)

    {
    if (n == 1) 
	return(1);
    else if (n > 1 && n <= 4) 
	return(4);
    else if (n > 4 && n <= 8) 
	return(8);
    else 
	return(24); 	
    }

/**************************************************************************
	Create an empty bitmap
**************************************************************************/

BYTE *CreateDib(WORD dx, WORD dy, BYTE *palette, WORD bits)

    {
    BYTE *hdibN;
    BITMAPINFOHEADER bi;
    LPBITMAPINFOHEADER lpbi;
    RGBQUAD FAR *pRgb;
    WORD i;

    bits=AdjustDIBits(bits);

    bi.biSize           = (DWORD)sizeof(BITMAPINFOHEADER);
    bi.biPlanes         = 1;
    bi.biBitCount       = bits;
    bi.biWidth          = (DWORD)dx;
    bi.biHeight         = (DWORD)dy;
    bi.biCompression    = BI_RGB;
    bi.biXPelsPerMeter  = 0;
    bi.biYPelsPerMeter  = 0;
    bi.biClrUsed        = bits > 8 ? 0L : (DWORD)((1<<bits) & 0xffff);
    bi.biClrImportant   = bi.biClrUsed;
    bi.biSizeImage	= (DWORD)(ComputeWidthBytes(bi.biWidth, bi.biBitCount) * dy);
    if ((hdibN = (BYTE *)GlobalAlloc(GMEM_MOVEABLE | GMEM_DISCARDABLE |
		GMEM_ZEROINIT,sizeof(BITMAPINFOHEADER) +
		    (long)bi.biClrUsed*sizeof(RGBQUAD)+bi.biSizeImage))==NULL)
       return(NULL);

    if((lpbi=(LPBITMAPINFOHEADER)GlobalLock(hdibN)) == NULL) 
	{
	GlobalFree(hdibN);
	return(NULL);
	}

	memcpy((LPSTR)lpbi,(LPSTR)&bi,sizeof(BITMAPINFOHEADER));

    if (palette != NULL) 
	{
	pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + (unsigned int)lpbi->biSize);
	for (i = 0; i < bi.biClrUsed; ++i) 
	    {
	    pRgb[i].rgbRed=(char)palette[i*RGB_SIZE+RGB_RED];
	    pRgb[i].rgbGreen=(char)palette[i*RGB_SIZE+RGB_GREEN];
	    pRgb[i].rgbBlue=(char)palette[i*RGB_SIZE+RGB_BLUE];
	    pRgb[i].rgbReserved=0;
	    }
	}						

    GlobalUnlock(hdibN);
    return(hdibN);
    }

/**************************************************************************
	Copy text to clipboard
**************************************************************************/

bool CopyTextToClipboard(HWND hwnd, const std::string& text)
    {
    if (!OpenClipboard(hwnd))
	return false;

    EmptyClipboard();
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);

    if (!hMem)
	{
	CloseClipboard();
	return false;
	}

    char* ptr = static_cast<char*>(GlobalLock(hMem));

    memcpy(ptr, text.c_str(), text.size() + 1);
    GlobalUnlock(hMem);
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();

    return true;
    }