/*
    DITHER.CPP
    Dither Library based on works of Alchemy Mindworks Inc 1995
    The entire source is kept, but the bits not needed in ManpWIN are copied out PHD 2019-06-03

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manpwin.h"
#include "manp.h"
#include "quanterr.h"
#include "Dib.h"
//#include "anim.h"
#include "colour.h"

#define	WRGB_RED		2
#define	WRGB_GREEN		1
#define	WRGB_BLUE		0
#define	DITHERCONTRAST		20
#define	DITHERBRIGHTNESS	20
#define	GREYVALUE(r,g,b)	((((r*30)/100) + ((g*59)/100) + ((b*11)/100)))
#define	ORTHOMATCH(r,g,b)	(((r) & 0x00e0) | (((g) >> 3) & 0x001c)  | (((b) >> 6) & 0x0003))
#define	LPBcolourmap(lpbi)	(LPRGBQUAD)((LPSTR)lpbi+lpbi->biSize)
#define DIBPAL(pDib)		LPBcolourmap(((LPBITMAPINFOHEADER)pDib))

extern	int	dl3quant(BYTE *, BYTE *, int, int, int, int, BYTE *);
extern	void	UpdateTitleBar(HWND);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//extern	HWND	GlobalHwnd;				// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////
	HCURSOR	hStdCursor;
static		CDib			DitherDib;		// Device Independent Bitmap class instance
BYTE		DitherBitsPerPixel = 24;		// for reducing bits per pixel

#define GetChunkyPixel(pxx,nxx) (!((nxx) & 1)) ? (((pxx)[(nxx)>>1] >> 4) & 0x0f) : ((pxx)[(nxx)>>1] & 0x0f)
#define	PutChunkyPixel(pxx,nxx,cxx) (!(nxx & 1)) ? \
(pxx[nxx>>1] &= 0x0f, pxx[nxx>>1] |= (char)((cxx & 0x0f) << 4)) : \
(pxx[nxx>>1] &= 0xf0, pxx[nxx>>1] |= (char)(cxx & 0x0f))

#ifndef max
#define max(a,b)            (((a)>(b))?(a):(b))
#endif

#ifndef min
#define min(a,b)            (((a)<(b))?(a):(b))
#endif

#define	addb(n1,n2)	    max(min((BYTE)(n1)+(short)(n2),255),0)

extern	void	CloseDitherPointers(BYTE);

static	int	ReturnCode = NO_ERR;		// For error tracking
static	HANDLE	hdest = NULL;
static	HANDLE	hsource = NULL;
static	LPBITMAPINFOHEADER lpbd = NULL;
static	LPBITMAPINFOHEADER lpbs = NULL;
static	char	*pstr;				// file name without path
static	BYTE	*PackedLegacyDib = NULL;

//char		DitherFlag = '2';		// 1 = Bayer or Diffuse Floyd, 2 = Floyd_Steinberg
// 3 = Floyd_Steinberg + 'Optimal Colour Mapper'

//	BYTE		masktable[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static	BYTE		bittable[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

LPBITMAPINFOHEADER BuildPackedLegacyDib(CDib& Dib);

static	char bayerPattern[8][8] =
    {
    {	 0,32, 8,40, 2,34,10,42, },
    {	48,16,56,24,50,18,58,26, },
    {	12,44, 4,36,14,46, 6,38, },
    {	60,28,52,20,62,30,54,22, },
    {	 3,35,11,43, 1,33, 9,41, },
    {	51,19,59,27,49,17,57,25, },
    {	15,47, 7,39,13,45, 5,37, },
    {	63,31,55,23,61,29,53,21  },
    };

int GetDibPalette(LPBITMAPINFO lpbi,BYTE *palette)
    {
    unsigned int i,j;

    j=min(1<<lpbi->bmiHeader.biBitCount,256);

    for(i=0;i<j;i++)
	{
	palette[i*RGB_SIZE+RGB_RED]=lpbi->bmiColors[i].rgbRed;
	palette[i*RGB_SIZE+RGB_GREEN]=lpbi->bmiColors[i].rgbGreen;
	palette[i*RGB_SIZE+RGB_BLUE]=lpbi->bmiColors[i].rgbBlue;
	}
    return(j);
    }

/**************************************************************************
Linear 256 Colour Palette
**************************************************************************/

void	GetOrthoPalette(BYTE *buffer)
    {
    static int expandbits32[]={ 32,64,96,128,160,192,224,255 };
    static int expandbits16[]={ 64,128,192,255 };
    static char palette[VGA_PAL_SIZE];
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

    if(buffer != NULL) memcpy(buffer,palette,768);
    }

/**************************************************************************
Dither to 256 Colours
**************************************************************************/

LPBITMAPINFOHEADER  Floyd_Steinberg(WORD NewBitsPerPixel)
    {
    BYTE	*source = NULL, *dest = NULL;
    BYTE	*LegacySource = NULL;    
    BYTE	palette[VGA_PAL_SIZE];
    WORD	i;
    WORD	width, height;

    // make a copy of our display DIB so we don't splatter it during dither
    CDib	TempDib;
    CDib	InterDib;

    width = gManp->Dib.DibWidth;
    height = gManp->Dib.DibHeight;

    if (!TempDib.InitDib(width, height, 24))
	{
	ReturnCode = ERR_CREATE_DIB_FAIL;
	return NULL;
	}

    memcpy(TempDib.DibPixels.data(), gManp->Dib.DibPixels.data(), min(TempDib.DibPixels.size(), gManp->Dib.DibPixels.size()));

    InterDib.InitDib(width, height, 8);

    source = TempDib.DibPixels.data();
    dest = InterDib.DibPixels.data();

    // Bridge modern vector-backed CDib pixels into the legacy packed buffer
    // expected by dl3quant().
    LegacySource = new BYTE[(size_t)width * (size_t)height * 3];

    if (LegacySource == NULL)
	{
	TempDib.CloseDibPtrs();
	InterDib.CloseDibPtrs();
	DitherDib.CloseDibPtrs();
	ReturnCode = ERR_IMAGE_MEMORY_FAIL;
	return NULL;
	}

    for (i = 0; i < height; ++i)
	{
	memcpy(LegacySource + (size_t)i * (size_t)width * 3, source + (size_t)i * TempDib.WidthBytes, (size_t)width * 3);
	}

    source = LegacySource;

    ReturnCode = dl3quant(source, dest, width, height, 1 << NewBitsPerPixel, 1, palette);

    if (LegacySource != NULL)
	{
	delete[] LegacySource;
	LegacySource = NULL;
	}

    if(ReturnCode < 0)
	{
	TempDib.CloseDibPtrs();
	InterDib.CloseDibPtrs();
	DitherDib.CloseDibPtrs();

	return NULL;
	}

    // IB 2009-06-02 The following looks sus to me - what if the 2 colours are not B&W
    if(NewBitsPerPixel == 1)
	{
	if(palette[0] + palette[1] + palette[2] < palette[3] + palette[4] + palette[5])	// there should only be two entries
	    memcpy(palette, "\000\000\000\377\377\377", 6);
	else
	    memcpy(palette, "\377\377\377\000\000\000", 6);
	}

    DitherDib.InitDib(width, height, NewBitsPerPixel);

    RGBQUAD *pRgb = DIBPAL(DitherDib.pDibInf);	// IB 2009-06-01
    // Floyd_Steinberg creates a RGB palette
    for(i = 0; i < 1 << NewBitsPerPixel; ++i)
	{
	pRgb[i].rgbRed  	= (BYTE)palette[i * 3 + WRGB_RED];
	pRgb[i].rgbGreen	= (BYTE)palette[i * 3 + WRGB_GREEN];
	pRgb[i].rgbBlue 	= (BYTE)palette[i * 3 + WRGB_BLUE];
	}

    BYTE    *pd, *ps;
    ps = InterDib.DibPixels.data();
    pd = DitherDib.DibPixels.data();

    // Copy indexed 8-bit pixels into final DIB. 
    // dl3quant() already produced tightly packed 8-bit indices.
    for (i = 0; i < height; ++i)
	{
	memcpy(DitherDib.DibPixels.data() + i * DitherDib.WidthBytes, InterDib.DibPixels.data() + i * width, width);
	}

    TempDib.CloseDibPtrs();
    InterDib.CloseDibPtrs();

    if (PackedLegacyDib != NULL)
	{
	delete[] PackedLegacyDib;
	PackedLegacyDib = NULL;
	}

    PackedLegacyDib = (BYTE *)BuildPackedLegacyDib(DitherDib);

    return (LPBITMAPINFOHEADER)PackedLegacyDib;
    }

/**************************************************************************
	Dither the Image to 2, 16 or 256 Colours
**************************************************************************/

LPBITMAPINFOHEADER  Dither(WORD NewBitsPerPixel)
    {
    char	s[300];
    LPBITMAPINFOHEADER BitMapPtr;

    if (gManp->Dib.BitsPerPixel != 24)
	gManp->Dib.DibTo24();
    ReturnCode = NO_ERR;
    BitMapPtr = Floyd_Steinberg(NewBitsPerPixel);

    if(ReturnCode < 0)
	{
	switch(ReturnCode)
	    {
	    case ERR_INIT_TABLE:
		_snprintf_s(s, 300, _TRUNCATE, "Error Initialising Quantisation Table");
	    	break;
	    case ERR_USER_ABORT:
		_snprintf_s(s, 300, _TRUNCATE, "User Abort");
	    	break;
	    case ERR_QUANTISE_IMAGE:
		_snprintf_s(s, 300, _TRUNCATE, "Error Quantising Image");
	    	break;
	    case ERR_DCQ_START:
		_snprintf_s(s, 300, _TRUNCATE, "Error Starting DCQ");
	    	break;
	    case ERR_BUILD_TABLE:
		_snprintf_s(s, 300, _TRUNCATE, "Error Building Quantisation Table");
	    	break;
	    case ERR_CREATE_DIB_FAIL:
		_snprintf_s(s, 300, _TRUNCATE, "Cannot Create DIB for Quantisation");
	    	break;
	    case ERR_IMAGE_MEMORY_FAIL:
		_snprintf_s(s, 300, _TRUNCATE, "Insufficient Memory for Quantisation");
	    	break;
	    case ERR_ILLEGAL_BITS_PIXEL:
		_snprintf_s(s, 300, _TRUNCATE, "Illegal Quantisation bits per pixel <%d>", NewBitsPerPixel);
	    	break;
	    case ERR_ILLEGAL_DITHER_TYPE:
		_snprintf_s(s, 300, _TRUNCATE, "Illegal Dithering Requested");
	    	break;
	    }
	MessageBox (gManp->GlobalHwnd, s, "Quantisation", MB_ICONEXCLAMATION | MB_OK);
	SetWindowText(gManp->GlobalHwnd, s);
	return NULL;
	}

    ReturnCode = NO_ERR;
    return BitMapPtr;
    }

/**************************************************************************
Build classic contiguous DIB memory block for legacy GIF encoder.

Modern CDib stores pixels separately in vector<BYTE>, but older GIF
code expects:

    BITMAPINFOHEADER + palette + image bits

in one contiguous memory block.
**************************************************************************/
LPBITMAPINFOHEADER BuildPackedLegacyDib(CDib& Dib)
    {
    DWORD PaletteSize;
    DWORD ImageSize;
    DWORD TotalSize;
    DWORD PackedWidthBytes;

    BYTE *Block;
    LPBITMAPINFOHEADER lpbi;
    BYTE *DestBits;

    // GIF expects 256 RGBQUAD palette entries
    PaletteSize = 256 * sizeof(RGBQUAD);

    // Compute packed 8-bit scanline size
    PackedWidthBytes = (DWORD)ComputeWidthBytes((size_t)Dib.DibWidth, (size_t)Dib.BitsPerPixel);

    ImageSize = PackedWidthBytes * Dib.DibHeight;

    TotalSize = sizeof(BITMAPINFOHEADER) + PaletteSize + ImageSize;

    Block = new BYTE[TotalSize];

    if (Block == NULL)
	return NULL;

    memset(Block, 0, TotalSize);

    lpbi = (LPBITMAPINFOHEADER)Block;

    // Copy BITMAPINFOHEADER
    memcpy(lpbi, Dib.pDibInf, sizeof(BITMAPINFOHEADER));

    // Force palette metadata for GIF
    lpbi->biClrUsed = 256;
    lpbi->biClrImportant = 256;
    lpbi->biSizeImage = ImageSize;

    // Copy palette
    memcpy(
	(BYTE *)lpbi + sizeof(BITMAPINFOHEADER),
	DIBPAL(Dib.pDibInf),
	PaletteSize);

    // Pointer to image bits
    DestBits =
	(BYTE *)lpbi
	+ sizeof(BITMAPINFOHEADER)
	+ PaletteSize;

    // Copy indexed pixels
    memcpy(
	DestBits,
	Dib.DibPixels.data(),
	ImageSize);

    return lpbi;
    }
       
void	CloseDitherPointers(BYTE success)
    {
    DitherDib.CloseDibPtrs();
    }

