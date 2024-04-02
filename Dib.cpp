// Dib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "Dib.h"
#define BITMAPCOMPRESSION(lpbi)     (*(LPDWORD)lpbi >= sizeof(BITMAPINFOHEADER) ? lpbi->biCompression : BI_RGB)
#define BITMAPREDMASK(lpbi)         ( (*(LPDWORD)lpbi >= sizeof(BITMAPV4HEADER)) || (BITMAPCOMPRESSION(lpbi) == BI_BITFIELDS) ?         \
                                    ((PBITMAPV4HEADER)lpbi)->bV4RedMask : 0)
#define BITMAPGREENMASK(lpbi)       ( (*(LPDWORD)lpbi >= sizeof(BITMAPV4HEADER)) || (BITMAPCOMPRESSION(lpbi) == BI_BITFIELDS) ?         \
                                    ((PBITMAPV4HEADER)lpbi)->bV4GreenMask : 0)
#define BITMAPBLUEMASK(lpbi)        ( (*(LPDWORD)lpbi >= sizeof(BITMAPV4HEADER)) || (BITMAPCOMPRESSION(lpbi) == BI_BITFIELDS) ?         \
                                    ((PBITMAPV4HEADER)lpbi)->bV4BlueMask : 0)

#define	RGB_RED			0
#define	RGB_GREEN		1
#define	RGB_BLUE		2
#define	RGB_SIZE		3

/* Macro to determine to round off the given value to the closest byte */
#define WIDTHBYTES(i)   ((i+31)/32*4)
#define PIXELS2BYTES(n)	((n+7)/8)
#define	GREYVALUE(r,g,b) ((((r*30)/100) + ((g*59)/100) + ((b*11)/100)))
#define	ORTHOMATCH(r,g,b)  (((r) & 0x00e0) | (((g) >> 3) & 0x001c)  | (((b) >> 6) & 0x0003))

#define	LPBimage(lpbi)	((BYTE *)lpbi+lpbi->biSize+(long)(lpbi->biClrUsed*sizeof(RGBQUAD)))
#define	LPBwidth(lpbi)	(lpbi->biWidth)
#define	LPBdepth(lpbi)	(lpbi->biHeight)
#define	LPBbits(lpbi)   (lpbi->biBitCount)
#define	LPBcolours(lpbi) (lpbi->biClrUsed)
#define	LPBlinewidth(lpbi) (WIDTHBYTES((WORD)lpbi->biWidth*lpbi->biBitCount))

#define	LPBcolourmap(lpbi) (LPRGBQUAD)((LPSTR)lpbi+lpbi->biSize)
// Macro to return pointer to DIB pixels
#define DIBPEL(pDib) LPBimage(((LPBITMAPINFOHEADER)pDib))
// Macro to return pointer to DIB palette
#define DIBPAL(pDib) LPBcolourmap(((LPBITMAPINFOHEADER)pDib))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDib::CDib()			// device independent bitmap
    {
    DibPixels = NULL;
    pDibInf = NULL;
    DibErrorCode = 0;
    DibHeight = DibWidth = BitsPerPixel = 0;
    }

CDib::CDib(const CDib & Dib1)	// Copy Constructor
    {
    SizeImage = Dib1.SizeImage;
    DibHeight = Dib1.DibHeight;
    DibWidth = Dib1.DibWidth;
    DibSize = Dib1.DibSize;
    BitsPerPixel = Dib1.BitsPerPixel;
    DibErrorCode = 0;
    pDibInf = (LPBITMAPINFO)new BYTE[DibSize];
    if(pDibInf == NULL)
	throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
    memcpy(pDibInf, Dib1.pDibInf, DibSize);
    DibPixels = DIBPEL(pDibInf);
    }

CDib::~CDib()
    {
    CloseDibPtrs();
    }

CDib& CDib::operator=(const CDib & Dib1)	// Assignment Operator
    {
    if(this == & Dib1)	    return * this;	// Attempt to assign to self
    CloseDibPtrs();				// release memory for target DIB
    SizeImage = Dib1.SizeImage;
    DibHeight = Dib1.DibHeight;
    DibWidth = Dib1.DibWidth;
    DibSize = Dib1.DibSize;
    BitsPerPixel = Dib1.BitsPerPixel;
    DibErrorCode = 0;
    pDibInf = (LPBITMAPINFO)new BYTE[DibSize];
    if(pDibInf == NULL)
	throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
    memcpy(pDibInf, Dib1.pDibInf, DibSize);
    DibPixels = DIBPEL(pDibInf);

    return * this;
    }

// Convert DIB into 24 bit - for unsuported sizes, just copy pixels into CDib
void	CDib::DibTo24(LPBITMAPINFO pDib, BYTE *	SourcePixels)
    {
    DWORD 	width = pDib->bmiHeader.biWidth;
    DWORD 	height = pDib->bmiHeader.biHeight;
    WORD	bits = pDib->bmiHeader.biBitCount;
//    if(bits <= 8 && pDib->bmiHeader.biClrUsed == 0)
//	pDib->bmiHeader.biClrUsed = 1 << bits;	// biClrUsed is permitted to be 0, but DIBPEL assumes it is set
//    DWORD	colours_used = pDib->bmiHeader.biClrUsed;

    RGBQUAD *	Palette = DIBPAL(pDib);
    BYTE *	Source;
    BYTE	SourcePel, SourcePix;
    RGBTRIPLE *	Dest;
//    BYTE *	SourcePixels = DIBPEL(pDib);

    DWORD	SourceWidthBytes = WIDTHBYTES((DWORD)width * bits);
    DWORD	DestWidthBytes   = WIDTHBYTES((DWORD)width * 24L);
    DWORD 	i, j, k, wbytes, wpart;

    InitDib(width, height, 24);		// Create an empty 24 bit DIB

    switch(bits)
	{
	case 1:
	    wbytes = width / 8;		// whole bytes in source line
	    wpart = width % 8;		// leftover bits in source line
	    for(i = 0; i < height; ++i)
	    	{
	    	Dest = (RGBTRIPLE *)((BYTE *)DibPixels + i * DestWidthBytes);
	    	Source = SourcePixels + i * SourceWidthBytes;
	    	for(j = 0; j < wbytes; ++j)
		    {
		    SourcePix = *Source;
		    for(k = 0; k < 8; ++k)
		    	{
		    	SourcePel = SourcePix & 0x80 ? 1 : 0;
		    	Dest->rgbtBlue  = Palette[SourcePel].rgbBlue;
		    	Dest->rgbtGreen = Palette[SourcePel].rgbGreen;
		    	Dest->rgbtRed   = Palette[SourcePel].rgbRed;
		    	SourcePix <<= 1;
		    	Dest++;
		    	}
		    Source++;
		    }
	    	SourcePix = *Source;
	    	for(k = 0; k < wpart; ++k)
		    {
		    SourcePel = SourcePix & 0x80 ? 1 : 0;
		    Dest->rgbtBlue  = Palette[SourcePel].rgbBlue;
		    Dest->rgbtGreen = Palette[SourcePel].rgbGreen;
		    Dest->rgbtRed   = Palette[SourcePel].rgbRed;
		    SourcePix <<= 1;
		    Dest++;
		    }
	    	}
	    break;
	case 4:
	    wbytes = width / 2;		// whole bytes in source line
	    wpart = width % 2;		// leftover nibbles in source line
	    for(i = 0; i < height; ++i)
	    	{
	    	Dest = (RGBTRIPLE *)((BYTE *)DibPixels + i * DestWidthBytes);
	    	Source = SourcePixels + i * SourceWidthBytes;

	    	for(j = 0; j < wbytes; ++j)
		    {
		    SourcePel = (*Source & 0xF0) >> 4;
		    for(k = 0; k < 2; ++k)
		    	{
			Dest->rgbtBlue  = Palette[SourcePel].rgbBlue;
			Dest->rgbtGreen = Palette[SourcePel].rgbGreen;
			Dest->rgbtRed   = Palette[SourcePel].rgbRed;
			Dest++;
			SourcePel = *Source & 0x0F;
			}
		    Source++;
		    }
		SourcePel = (*Source & 0xF0) >> 4;
		for(k = 0; k < wpart; ++k)
		    {
		    Dest->rgbtBlue  = Palette[SourcePel].rgbBlue;
		    Dest->rgbtGreen = Palette[SourcePel].rgbGreen;
		    Dest->rgbtRed   = Palette[SourcePel].rgbRed;
		    Dest++;
		    SourcePel = *Source & 0x0F;
		    }
	    	}
	    break;
	case 8:
	    for(i = 0; i < height; ++i)
	    	{
	    	Dest = (RGBTRIPLE *)((BYTE *)DibPixels + i * DestWidthBytes);
	    	Source = SourcePixels + i * SourceWidthBytes;
	    	for(j = 0; j < width; ++j)
		    {
		    SourcePel = *Source;
		    Dest->rgbtBlue  = Palette[SourcePel].rgbBlue;
		    Dest->rgbtGreen = Palette[SourcePel].rgbGreen;
		    Dest->rgbtRed   = Palette[SourcePel].rgbRed;
		    Dest++;
		    Source++;
		    }
	    	}
	    break;
	case 16:
	    {
	    WORD	SourcePel16;
	    WORD *	Source16;
	    DWORD	RedMask   =  0x7C00;
	    DWORD	GreenMask =  0x03E0;
	    DWORD	BlueMask  =  0x001F;
	    if (pDib->bmiHeader.biCompression == BI_BITFIELDS)
		{
		// If this is a 16 or 32 bit bitmap and BI_BITFIELDS is used,
		// bmiColors member contains three DWORD color masks.
		RedMask   =  BITMAPREDMASK  (((LPBITMAPINFOHEADER)pDib));
		GreenMask =  BITMAPGREENMASK(((LPBITMAPINFOHEADER)pDib));
		BlueMask  =  BITMAPBLUEMASK (((LPBITMAPINFOHEADER)pDib));
//		if (pDib->bmiHeader.biSize == sizeof(BITMAPINFOHEADER))
//		    {
//		    // For V4 or V5 headers, this info is included in the header
//			SourcePixels += 3 * sizeof(DWORD);
//		    }
		}
	    // Calculate the magnitude of the shift from the respective Mask (Blue is assumed to be LSD)
	    int 	bs =  0, rs =  0, gs = 0;
	    DWORD	Mask;

	    Mask = BlueMask;
	    while((Mask&0x80) == 0)     { Mask <<= 1; bs++; }
	    Mask = RedMask;
	    while((Mask&1) == 0)	{ Mask >>= 1; rs++; }
	    while((Mask&0x80) == 0)     { Mask <<= 1; rs--; }
	    Mask = GreenMask;
	    while((Mask&1) == 0)	{ Mask >>= 1; gs++; }
	    while((Mask&0x80) == 0)     { Mask <<= 1; gs--; }

     	    for(i = 0; i < height; ++i)
	    	{
	    	Dest = (RGBTRIPLE *)((BYTE *)DibPixels + i * DestWidthBytes);
	    	Source16 = (WORD *)(SourcePixels+ i * SourceWidthBytes);
	    	for(j = 0; j < width; ++j)
		    {
		    SourcePel16 = *Source16;
		    Dest->rgbtBlue  = (BYTE)((SourcePel16 & BlueMask)  << bs);
		    Dest->rgbtGreen = (BYTE)((SourcePel16 & GreenMask) >> gs);
		    Dest->rgbtRed   = (BYTE)((SourcePel16 & RedMask)   >> rs);
		    Dest++;
		    Source16++;
		    }
	    	}
	    }
	    break;
	case 24:
	    memcpy(DibPixels, SourcePixels, SizeImage);
	    break;
	case 32:
	    {
	    DWORD	SourcePel32;
	    DWORD *	Source32;
	    DWORD	RedMask   =  0xFF0000;
	    DWORD	GreenMask =  0x00FF00;
	    DWORD	BlueMask  =  0x0000FF;
	    if (pDib->bmiHeader.biCompression == BI_BITFIELDS)
		{
		// If this is a 16 or 32 bit bitmap and BI_BITFIELDS is used,
		// bmiColors member contains three DWORD color masks.
		RedMask   =  BITMAPREDMASK  (((LPBITMAPINFOHEADER)pDib));
		GreenMask =  BITMAPGREENMASK(((LPBITMAPINFOHEADER)pDib));
		BlueMask  =  BITMAPBLUEMASK (((LPBITMAPINFOHEADER)pDib));
//		if (pDib->bmiHeader.biSize == sizeof(BITMAPINFOHEADER))
//		    {
//		    // For V4 or V5 headers, this info is included in the header
//			SourcePixels += 3 * sizeof(DWORD);
//		    }
		}
	    // Calculate the magnitude of the shift from the respective Mask (Blue is assumed to be LSD)
	    int 	bs =  0, rs =  0, gs = 0;
	    DWORD	Mask;

	    Mask = BlueMask;
	    while((Mask&0x80) == 0)     { Mask <<= 1; bs++; }
	    Mask = RedMask;
	    while((Mask&1) == 0)	{ Mask >>= 1; rs++; }
	    while((Mask&0x80) == 0)     { Mask <<= 1; rs--; }
	    Mask = GreenMask;
	    while((Mask&1) == 0)	{ Mask >>= 1; gs++; }
	    while((Mask&0x80) == 0)     { Mask <<= 1; gs--; }

     	    for(i = 0; i < height; ++i)
	    	{
	    	Dest = (RGBTRIPLE *)((BYTE *)DibPixels + i * DestWidthBytes);
	    	Source32 = (DWORD *)(SourcePixels+ i * SourceWidthBytes);
	    	for(j = 0; j < width; ++j)
		    {
		    SourcePel32 = *Source32;
		    Dest->rgbtBlue  = (BYTE)((SourcePel32 & BlueMask)  << bs);
		    Dest->rgbtGreen = (BYTE)((SourcePel32 & GreenMask) >> gs);
		    Dest->rgbtRed   = (BYTE)((SourcePel32 & RedMask)   >> rs);
		    Dest++;
		    Source32++;
		    }
	    	}
	    }
	    break;
	case 64:
	    {
	    struct RGBOCT{
		BYTE    rgbBlue;
		BYTE    rgbBlueHi;
		BYTE    rgbGreen;
		BYTE    rgbGreenHi;
		BYTE    rgbRed;
		BYTE    rgbRedHi;
		WORD    rgbReserved;
		}  *	Source64;

     	    for(i = 0; i < height; ++i)
	    	{
	    	Dest = (RGBTRIPLE *)((BYTE *)DibPixels + i * DestWidthBytes);
	    	Source64 = (RGBOCT *)(SourcePixels+ i * SourceWidthBytes);
	    	for(j = 0; j < width; ++j)
		    {
		    Dest->rgbtBlue  = Source64->rgbBlue;
		    Dest->rgbtGreen = Source64->rgbGreen;
		    Dest->rgbtRed   = Source64->rgbRed;
		    Dest++;
		    Source64++;
		    }
	    	}
	    }
	    break;
	default:
	    InitDib(width, height, bits);		
	    memcpy(DibPixels, SourcePixels, SizeImage);	// just copy pixels
	    break;
	}
    }

void	CDib::DibTo24(LPBITMAPINFO pDib)
    {
    WORD	bits = pDib->bmiHeader.biBitCount;
    if(bits <= 8 && pDib->bmiHeader.biClrUsed == 0)
	pDib->bmiHeader.biClrUsed = 1 << bits;	// biClrUsed is permitted to be 0, but DIBPEL assumes it is set

    BYTE *	SourcePixels = DIBPEL(pDib);
    if (pDib->bmiHeader.biCompression == BI_BITFIELDS)
	{
	// If this is a 16 or 32 bit bitmap and BI_BITFIELDS is used,
	// bmiColors member contains three DWORD color masks.
	if (pDib->bmiHeader.biSize == sizeof(BITMAPINFOHEADER))
	    {
	    SourcePixels += 3 * sizeof(DWORD);    // For V4 or V5 headers, this info is included in the header
	    }
	}
    DibTo24(pDib, SourcePixels);
}

void	CDib::DibTo24(const CDib & Dib1)
    {
    DibTo24(Dib1.pDibInf);
    }

void	CDib::DibTo24()
    {
    CDib	TempDIB(*this);	// Create TempDIB initialised to current Dib
    DibTo24(TempDIB);
    }

//	Initialise DIB Palette (This is not a member of CDib)
void	SetDibPalette(LPBITMAPINFO pDib, BYTE *palette)
    {
    RGBQUAD *pRgb;
    DWORD i, i3;
    if(palette != NULL)
	{
	pRgb = DIBPAL(pDib);
	for(i = i3 = 0; i < pDib->bmiHeader.biClrUsed; ++i, i3+=RGB_SIZE)
	    {
	    pRgb[i].rgbRed   = (char)palette[i3+RGB_RED];
	    pRgb[i].rgbGreen = (char)palette[i3+RGB_GREEN];
	    pRgb[i].rgbBlue  = (char)palette[i3+RGB_BLUE];
	    pRgb[i].rgbReserved = 0;
	    }
	}
    }

//	Round bit depths up to legal values for a bitmap
// IB 2009-05-28 add 16 bit (and tidy up)
static WORD	AdjustDIBits(WORD n)
    {
    if (n == 1)	return(1);
    if (n <= 4)	return(4);
    if (n <= 8)	return(8);
    if (n <= 16)	return(16);
    if (n <= 24)	return(24);
    if (n <= 32)	return(32);
	return(64);			// found 64 bit PNG files recently PHD 2009-05-16
    }

//	Create an empty bitmap (This is not a member of CDib)
//	NOTE it is the responsibility of the caller to release memory allocated by this function
//	NOTE does not currently handle 16 or 32 bit DIB (which require bit field masks for BI_BITFIELD)
LPBITMAPINFO	CreateEmptyDib(int Width, int Height, WORD bits)
    {
    LPBITMAPINFO	pDib;		// pointer to the DIB info
    bits = AdjustDIBits(bits);
    DWORD	colours_used = bits > 8 ? 0L : (DWORD)((1<<bits) & 0xffff);
    DWORD	SizeImage = WIDTHBYTES(Width * bits) * Height;	// Size, in bytes, of the image made of RGB triplets.
    DWORD	SizeCount = Width * Height;			// Size, in WORDS, of the count array.
    DWORD	DibSize = sizeof(BITMAPINFOHEADER) + colours_used*sizeof(RGBQUAD) + SizeImage + SizeCount;

    // allocate a device-independent bitmap header + palette + pixel array
    if(pDib = (LPBITMAPINFO)new BYTE[DibSize] )
	{
	// fill in the header
	pDib->bmiHeader.biSize	= (DWORD)sizeof(BITMAPINFOHEADER);
	pDib->bmiHeader.biWidth  	= (DWORD)Width;
	pDib->bmiHeader.biHeight 	= (DWORD)Height;
	pDib->bmiHeader.biPlanes 	= 1;
	pDib->bmiHeader.biBitCount 	= bits;
	pDib->bmiHeader.biCompression 	= BI_RGB;
	pDib->bmiHeader.biSizeImage	= SizeImage;	// Size, in bytes, of the image. May be set to zero for BI_RGB bitmaps.
	pDib->bmiHeader.biXPelsPerMeter = 0L;
	pDib->bmiHeader.biYPelsPerMeter = 0L;
	pDib->bmiHeader.biClrUsed 	= colours_used;
	pDib->bmiHeader.biClrImportant 	= 0;
	}
    else
	{
//	throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
	return NULL;
	}
    return pDib;
    }

//	Create an empty bitmap, Initialise Palette (This is not a member of CDib)
//	NOTE it is the responsibility of the caller to release memory allocated by this function
LPBITMAPINFO	CreateEmptyDib(int Width, int Height, BYTE *palette, WORD bits)
    {
    // allocate a device-independent bitmap header + palette + pixel array
    LPBITMAPINFO pDib = CreateEmptyDib(Width, Height, bits);
    if(pDib == NULL )
	{
	return NULL;
	}
    SetDibPalette(pDib, palette);
    return pDib;
    }

//////////////////////////////////////////////////////////////////////
//	Initialise Windows Device Independent Bitmap
//////////////////////////////////////////////////////////////////////

unsigned char	*CDib::InitDib(int Width, int Height, WORD bits)
    {
    CloseDibPtrs();			// give back memory for new DIB

    // allocate a device-independent bitmap header + palette + pixel array
    pDibInf = CreateEmptyDib(Width, Height, bits);
    if(pDibInf == NULL )
	{
	DibErrorCode = NODIBMEMORY;
	throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
	return NULL;
	}

    // Initialise CDib members
    DibPixels = DIBPEL(pDibInf);
    SizeImage = pDibInf->bmiHeader.biSizeImage;	// Size, in bytes, of the image made of RGB triplets.
    DibHeight = Height;			// dimensions in pixels, useful in undo() and redo()
    DibWidth = Width;			// may be useful to replace these as globals in the future
    BitsPerPixel = pDibInf->bmiHeader.biBitCount ;
    DibSize = sizeof(BITMAPINFOHEADER) + pDibInf->bmiHeader.biClrUsed*sizeof(RGBQUAD) + SizeImage;
    WidthBytes = WIDTHBYTES(Width * bits);
    DibNumColours = 0L;
    memset(DibPixels, 0x55, SizeImage);	// this is code for debugging
    return DibPixels;
    }

//	Initialise Windows Device Independent Bitmap and Palette
unsigned char	*CDib::InitDib(int Width, int Height, BYTE *palette, WORD bits)
    {
    InitDib(Width, Height, bits);
    SetDibPalette(pDibInf, palette);
    return DibPixels;
    }

// Currently only supports BI_RGB
unsigned char	*CDib::InitDib(LPBITMAPINFO pDib)
    {
    CloseDibPtrs();			// give back memory for new DIB
    if(pDib->bmiHeader.biCompression != BI_RGB)
	{
	throw "DIB Not supported";
	return NULL;
	}

    // allocate a device-independent bitmap header + palette + pixel array
    InitDib(pDib->bmiHeader.biWidth, pDib->bmiHeader.biHeight, pDib->bmiHeader.biBitCount);
    // Copy palette + pixel array
    memcpy(DIBPAL(pDibInf), DIBPAL(pDib), pDib->bmiHeader.biClrUsed*sizeof(RGBQUAD));	// palette
    memcpy(DibPixels, DIBPEL(pDib), pDib->bmiHeader.biSizeImage);	// picture

    return DibPixels;
    }


////////////////////////////////////////////////////////////////////////////////
//	NOTE This is NOT a proper member of CDib, because it depends on a global
//	This was included to facilitate conversion to CDib,
//	and should be replaced by    InitDib(Width, Height, bits_per_pixel)
//	It is also doubtful whether always creating 24 bit DIB makes sense
////////////////////////////////////////////////////////////////////////////////

// uses global bits_per_pixel
// NOTE this differs from the original in:-
//	AdjustDIBits does not allow 24 bits per pixel (M$ code)
//	no extra memory is allocated
//	bmiHeader.biSizeImage is ALWAYS set to Size of the image. (May be set to zero for BI_RGB bitmaps).

unsigned char	*CDib::InitDib(int Width, int Height)
    {
//    InitDib(Width, Height, (bits_per_pixel > 24) ? bits_per_pixel : 24);	// Force DIB to be at least 24 bit
//    pDibInf->bmiHeader.biBitCount 	= bits_per_pixel;	// restore bits_per_pixel to original

// PHD 2009-06-17 We don't have a global bits_per_pixel any more, so if it is needed to be another bit rate, 
// it has to be specified
    InitDib(Width, Height, 24);	// Force DIB to be at least 24 bit
    pDibInf->bmiHeader.biBitCount 	= 24;	// restore bits_per_pixel to original
    return DibPixels;
    }

//////////////////////////////////////////////////////////////////////
//	Close Dib Pointers
//////////////////////////////////////////////////////////////////////

void	CDib::CloseDibPtrs(void)
    {
    if (DibPixels)
	{
	DibPixels = NULL;
	}
    if (pDibInf)
	{
	delete [] pDibInf;
	pDibInf = NULL;
	}
    }
// IB 2009-09-05 The code from here on ISN'T part of CDib and shouldn't be here
// This is not good OOP.
// You just don't seem to understand. An object (Class CDib) should be independent.
// It can call other code, but it shouldn't access external globals

//////////////////////////////////////////////////////////////////////
//	Populate the count of colours in the image
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	hwnd;					// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////


void CDib::ClearDib(int Red, int Green, int Blue)
    {
    DWORD		i, j;

    RGBTRIPLE *	Dest;
    DWORD	DestWidthBytes   = WIDTHBYTES((DWORD)DibWidth * 24L);

    switch(BitsPerPixel)
	{
	case 24:
	    for(i = 0; i < DibHeight; ++i)
		{
		Dest = (RGBTRIPLE *)((BYTE *)DibPixels + i * DestWidthBytes);
		for(j = 0; j < DibWidth; ++j)
		    {
		    Dest->rgbtBlue  = Blue;
		    Dest->rgbtGreen = Green;
		    Dest->rgbtRed   = Red;
		    Dest++;
		    }
		}
		break;
	default:
	    memset((BYTE *)DibPixels, 0, (size_t)SizeImage);	// if not 24 bit set to 0
	    break;
	}
    }

// Insert text into 24 bit CDib at the location rect 

void	CDib::Text2Dib(HDC hDc, RECT *rect, TCHAR *text)
    {
    Text2Dib(hDc, rect, GetTextColor(hDc), GetBkColor(hDc), NULL, GetBkMode(hDc), text);
    }

void	CDib::Text2Dib(HDC hDc, RECT *rect, LOGFONT *lf, TCHAR *text)
    {
    Text2Dib(hDc, rect, GetTextColor(hDc), GetBkColor(hDc), lf, GetBkMode(hDc), text);
    }
void	CDib::Text2Dib(HDC hDc, RECT *rect, COLORREF FontColour, COLORREF BkgColour, LOGFONT *lf, int TranparentText, TCHAR *text)
    {
    HDC	hdc;
    HBITMAP	hBitmap;

    //    HDC	hDc = GetDC(hwnd); 
    if ((hdc = CreateCompatibleDC(hDc)) == NULL)
	return;
    SetTextColor(hdc, FontColour);
    SetBkColor(hdc, BkgColour);
    SetBkMode(hdc, TranparentText);
    if (lf)
	SelectObject(hdc, CreateFontIndirect(lf));
    hBitmap = CreateCompatibleBitmap(hDc, DibWidth, DibHeight);
    SelectObject(hdc, hBitmap);
    if (SetDIBits(hDc, hBitmap, 0, DibHeight, DibPixels, (LPBITMAPINFO)pDibInf, DIB_RGB_COLORS) == 0)
	return;
    DrawText(hdc, text, -1, rect, DT_CENTER);
    if (GetDIBits(hDc, hBitmap, 0, DibHeight, DibPixels, (LPBITMAPINFO)pDibInf, DIB_RGB_COLORS) == 0)
	return;
    DeleteObject(hBitmap);
    //    ReleaseDC(hwnd, hDc);
    }

//void CDib::ClearDib(RGBTRIPLE colour)
//    {
//    ClearDib(colour.rgbtRed, colour.rgbtGreen, colour.rgbtBlue); 
//    }

void CDib::ClearDib(DWORD colour)
    {
    ClearDib((colour>>16)&0xFF, (colour>>8)&0xFF, colour&0xFF); 
    }
