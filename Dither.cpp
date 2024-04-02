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
#include "quanterr.h"
#include "Dib.h"
#include "anim.h"
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


//extern	int	Force24Bit(void);
//extern	void	ChangeView(int, int, int, int, int, int, int, int, char);
//extern	void	SetupView(void);
//extern	int	GeneratePalette(char *, char *, char *);
//extern	int	dl1quant(BYTE *, BYTE *, int, int, int, int, BYTE *);
extern	int	dl3quant(BYTE *, BYTE *, int, int, int, int, BYTE *);
//extern	int	SetupUndo(void);
extern	void	UpdateTitleBar(HWND);
//extern	void	CreateTitleBar(char *, char *, int, int, int);
//extern	void	ShowMessage(char *);

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;				// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////
	HCURSOR	hStdCursor;
//extern	BYTE	*PalettePtr;				// points to true colour palette

// Global Variables:
extern	CDib			Dib;			// Device Independent Bitmap class instance
static	CDib			DitherDib;		// Device Independent Bitmap class instance
extern	CTrueCol		TrueCol;		// palette info

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
//char		DitherFlag = '2';		// 1 = Bayer or Diffuse Floyd, 2 = Floyd_Steinberg
// 3 = Floyd_Steinberg + 'Optimal Colour Mapper'

	BYTE		masktable[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static	BYTE		bittable[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

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
Dithering Engine for 256 Colours
**************************************************************************/

/*
void DiffuseFloyd(BYTE *line1,BYTE *line2,short x,short y,short r,
    short g,short b,short mr,short mg,short mb,short width,short depth)
    {
    short dr,dg,db,xpos,vr,vg,vb, a1, b1, c1;

    dr=(r-mr)>>4;
    dg=(g-mg)>>4;
    db=(b-mb)>>4;

    vr=dr<<1;
    vg=dg<<1;
    vb=db<<1;

    if((x+1) < width && (y+1) < depth)
	{
	xpos=RGB_SIZE*(x+1);
	a1 = *(line2 + xpos+WRGB_RED)  =addb(*(line2 + xpos+WRGB_RED),dr);
	b1 = *(line2 + xpos+WRGB_GREEN)=addb(*(line2 + xpos+WRGB_GREEN),dg);
	c1 = *(line2 + xpos+WRGB_BLUE) =addb(*(line2 + xpos+WRGB_BLUE),db);

	}

    dr+=vr; dg+=vg; db+=vb;

    if((x-1) > 0 && (y+1) < depth)
	{
	xpos=RGB_SIZE*(x-1);
	*(line2 + xpos+WRGB_RED)  =addb(*(line2 + xpos+WRGB_RED),dr);
	*(line2 + xpos+WRGB_GREEN)=addb(*(line2 + xpos+WRGB_GREEN),dg);
	*(line2 + xpos+WRGB_BLUE) =addb(*(line2 + xpos+WRGB_BLUE),db);

	}

    dr+=vr; dg+=vg; db+=vb;

    if((y+1) < depth)
	{
	xpos=RGB_SIZE*(x);
	*(line2 + xpos+WRGB_RED)  =addb(*(line2 + xpos+WRGB_RED),dr);
	*(line2 + xpos+WRGB_GREEN)=addb(*(line2 + xpos+WRGB_GREEN),dg);
	*(line2 + xpos+WRGB_BLUE) =addb(*(line2 + xpos+WRGB_BLUE),db);

	}

    dr+=vr; dg+=vg; db+=vb;

    if((x+1) < width)
	{
	xpos=RGB_SIZE*(x+1);
	*(line1 + xpos+WRGB_RED)  =addb(*(line1 + xpos+WRGB_RED),dr);
	*(line1 + xpos+WRGB_GREEN)=addb(*(line1 + xpos+WRGB_GREEN),dg);
	*(line1 + xpos+WRGB_BLUE) =addb(*(line1 + xpos+WRGB_BLUE),db);

	}
    }
*/

/**************************************************************************
Dither to 256 Colours
**************************************************************************/

LPBITMAPINFOHEADER  Floyd_Steinberg(WORD NewBitsPerPixel)
    {
    BYTE	*source = NULL, *dest = NULL, *pDestImage = NULL;
    BYTE	palette[VGA_PAL_SIZE];
    WORD	i, j;
    WORD	width, height;
//    BYTE	*pd, *pr, *ps, *pl;

    // make a copy of our display DIB so we don't splatter it during dither

    CDib	TempDib(Dib);
    CDib	InterDib;

    width = Dib.DibWidth;
    height = Dib.DibHeight;

    InterDib.InitDib(width, height, 8);

    source = TempDib.DibPixels;
    dest = InterDib.DibPixels;

    // IB 2009-06-02 Floyd_Steinberg requires a packed pixel array.
    // The following packs the pixel array within TempDib
	{
	DWORD	SourceWidthBytes = WIDTHBYTES((DWORD)width * 24L);
	RGBTRIPLE *	Dest = (RGBTRIPLE *)((BYTE *)source);
	RGBTRIPLE *	Source;
	for(i = 0; i < height; ++i)
	    {
	    Source = (RGBTRIPLE *)((BYTE *)source + i * SourceWidthBytes);
	    for(j = 0; j < width; ++j)
		{
		*Dest++ = *Source++;
		}
	    }
	}

//    if(DitherFlag == '2')
//	{
//	ReturnCode = dl1quant(source, dest, width, height, 1 << NewBitsPerPixel, 1, palette);
//	}
//    else if(DitherFlag == '3')
	ReturnCode = dl3quant(source, dest, width, height, 1 << NewBitsPerPixel, 1, palette);
//    else
//	{
//	ReturnCode = ERR_ILLEGAL_DITHER_TYPE;
//	}
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

    BYTE	*pd, *pr, *ps, *pl;
//    ps = InterDib.DibPixels;
//    pd = DitherDib.DibPixels;
    ps = InterDib.DibPixels;
    pd = DitherDib.DibPixels;
    switch(NewBitsPerPixel)
	{
	case 1:
	    for(i = 0; i < height; ++i)
		{
//		int j8;
//    BYTE	SourcePix;
    BYTE	*prj8;
		pr = ps;
		pl = pd;
		ps += width;
		pd += WIDTHBYTES((DWORD)width * 1L);
		for(j = 0, prj8 = pr; j <= width >> 3; ++j, prj8+=8)
		    {
		    *(pl + j) =	  (*(prj8 + 0)) << 7 | (*(prj8 + 1)) << 6
				| (*(prj8 + 2)) << 5 | (*(prj8 + 3)) << 4
				| (*(prj8 + 4)) << 3 | (*(prj8 + 5)) << 2
				| (*(prj8 + 6)) << 1 | (*(prj8 + 7)) << 0;
		    }
		}

	    break;
	case 4:
	    for(i = 0; i < height; ++i)
		{
		pr = ps;
		pl = pd;
		ps += width;
		pd += WIDTHBYTES((DWORD)width * 4L);
	// IB 2009-06-06 The following looks sus to me - what if there an odd number of pixels
		for(j = 0; j <= width >> 1;++j)
		    *(pl + j) = *(pr + j + j + 1) | (*(pr + j + j)) << 4;
		}
	    break;
	case 8:
	    for(i = 0; i < height; ++i)
		{
		// this convoluted copy required because source has word boundaries and destination requires RGB triplets only
		memcpy(pd + WIDTHBYTES((DWORD)width * (DWORD)NewBitsPerPixel) * i, ps + i * width, width);
		}
	    break;
	}

    TempDib.CloseDibPtrs();
    InterDib.CloseDibPtrs();

    return (LPBITMAPINFOHEADER)DitherDib.pDibInf;
    }

/**************************************************************************
Dither to 256 Colours
**************************************************************************/

/*
LPBITMAPINFOHEADER  Dither256(void)
    {
    BYTE	*pr;
    BYTE	palette[VGA_PAL_SIZE];
    WORD	k, r, g, b, mr, mg, mb;
    WORD	i, j;
    CDib	TempDib;
    // make a copy of our display DIB so we don't splatter it during dither
    TempDib = Dib;
    lpbs = (LPBITMAPINFOHEADER)TempDib.pDibInf;

    GetOrthoPalette(palette);

    DitherDib.InitDib(Dib.DibWidth, Dib.DibHeight, palette, 8);
    lpbd = (LPBITMAPINFOHEADER)DitherDib.pDibInf;

    BYTE *	source = TempDib.DibPixels;
    BYTE *	dest = DitherDib.DibPixels;

    for(i = 0; i < DitherDib.DibHeight; ++i)
	{
	for(j = 0; j < DitherDib.DibWidth; ++j)
	    {
	    k = RGB_SIZE * j;
	    r = source[k + WRGB_RED] & 0x00ff;
	    g = source[k + WRGB_GREEN] & 0x00ff;
	    b = source[k + WRGB_BLUE] & 0x00ff;

	    k = ORTHOMATCH(r, g, b);
	    pr = palette + RGB_SIZE * k;

	    *(dest + j) = (BYTE)k;
	    mr = pr[RGB_RED] & 0x00ff;
	    mg = pr[RGB_GREEN] & 0x00ff;
	    mb = pr[RGB_BLUE] & 0x00ff;

	    DiffuseFloyd(source,
		source + Dib.WidthBytes,
		j,
		i,
		r,
		g,
		b,
		mr,
		mg,
		mb,
		DitherDib.DibWidth,
		DitherDib.DibHeight);

	    }

	source += (long)Dib.WidthBytes;
	dest += (long)DitherDib.WidthBytes;
	}

    TempDib.CloseDibPtrs();

    return(lpbd);
    }
*/

/**************************************************************************
Dither to 16 Colours
**************************************************************************/

/*
LPBITMAPINFOHEADER  Dither16(void)
    {
    static BYTE fixedpalette[]=
	{
	0,  0,  0,
	255,  0,  0,
	0,255,  0,
	255,255,  0,
	0,  0,255,
	255,  0,255,
	0,255,255,
	255,255,255,

	0,  0,  0,
	255,  0,  0,
	0,255,  0,
	255,255,  0,
	0,  0,255,
	255,  0,255,
	0,255,255,
	255,255,255

	// start dull colours
	};

    BYTE	*pl;
    BYTE	*pd, *pr, *ps, *pDestImage;
    WORD	i,j,m,n;
    int	r,g,b;

    DitherDib.InitDib(Dib.DibWidth, Dib.DibHeight, fixedpalette, 4);
    lpbd = (LPBITMAPINFOHEADER)DitherDib.pDibInf;

    ps = Dib.DibPixels;
    pd = DitherDib.DibPixels;
    pDestImage = pd;

    for(i = 0; i < DitherDib.DibHeight; ++i)
	{
	pr = ps;
	ps += Dib.WidthBytes;

	if(Dib.BitsPerPixel > 8)
	    {
	    for(j = 0; j < DitherDib.DibWidth;++j)
		{
		r = pr[WRGB_RED] & 0x00ff;
		g = pr[WRGB_GREEN] & 0x00ff;
		b = pr[WRGB_BLUE] & 0x00ff;

		m = (bayerPattern[j & 0x0007][i & 0x0007] << 2);
		n = 0;

		if(r > (int)m)
		    n |= 0x01;
		if(g > (int)m)
		    n |= 0x02;
		if(b > (int)m)
		    n |= 0x04;

		pr += RGB_SIZE;

		PutChunkyPixel(pd, j, n);
		}
	    }
	else
	    {
	    for(j = 0; j < DitherDib.DibWidth;++j)
		{
		pl = TrueCol.PalettePtr + *pr++ * RGB_SIZE;
		r = pl[RGB_RED];
		g = pl[RGB_GREEN];
		b = pl[RGB_BLUE];

		m = (bayerPattern[j & 0x0007][i & 0x0007] << 2);
		n = 0;

		if(r > (int)m)
		    n |= 0x01;
		if(g > (int)m)
		    n |= 0x02;
		if(b > (int)m)
		    n |= 0x04;

		PutChunkyPixel(pd,j,n);
		}
	    }

	pd += DitherDib.WidthBytes;
	}

    return(lpbd);
    }
*/

/**************************************************************************
Dither to 2 Colours
**************************************************************************/

/*
LPBITMAPINFOHEADER  Dither2(void)
    {
    BYTE	*pDestImage;
    WORD	i,j,n;
    BYTE	*source=NULL, *dest=NULL, *pss;
    double	a,fac;
    BYTE	palette[VGA_PAL_SIZE], greylevel[VGA_COLOURS];
    BYTE	remap[VGA_COLOURS];
    static char pal[]="\000\000\000\377\377\377";

    memcpy(palette, pal, 6);

    DitherDib.InitDib(Dib.DibWidth, Dib.DibHeight, palette, 1);
    lpbd = (LPBITMAPINFOHEADER)DitherDib.pDibInf;

    source = Dib.DibPixels;
    dest = DitherDib.DibPixels;
    pDestImage = dest;

    for(i = 0; i < 256; ++i)
	greylevel[i] = GREYVALUE(TrueCol.PalettePtr[i*RGB_SIZE+RGB_RED],
	    TrueCol.PalettePtr[i*RGB_SIZE+RGB_GREEN],
	    TrueCol.PalettePtr[i*RGB_SIZE+RGB_BLUE]);

    fac = 256 / (256 - ((double)DITHERCONTRAST * 2));
    a = (double)DITHERBRIGHTNESS - (double)DITHERCONTRAST;

    for(i = 0;i < 256; ++i)
	{
	if(a > 254)
	    n=254;
	else if(a < 1)
	    n=1;
	else
	    n = (WORD) a;
	remap[i] = (BYTE) n;
	a += fac;
	}

    for(i = 0; i < DitherDib.DibHeight; ++i)
	{
	for(j = 0; j < DitherDib.DibWidth; ++j)
	    {
	    if(Dib.BitsPerPixel == 24)
		{
		pss = source + (long)j * RGB_SIZE;
		n = GREYVALUE(*(pss + 2), *(pss + 1), *(pss + 0));
		}
	    else if(Dib.BitsPerPixel == 8)
		n = (greylevel[source[j]]) & 0x00ff;
	    else
		n = (greylevel[GetChunkyPixel(source,j)]) & 0x00ff;

	    if((n = remap[n] >> 2) > (WORD)bayerPattern[i & 0x0007][j & 0x0007])
		dest[j >> 3] |= masktable[j & 0x0007];
	    else
		dest[j >> 3] &= ~masktable[j & 0x0007];
	    }

	source += Dib.WidthBytes;
	dest += DitherDib.WidthBytes;
	}

    return(lpbd);
    }
*/

/**************************************************************************
	Dither the Image to 2, 16 or 256 Colours
**************************************************************************/

LPBITMAPINFOHEADER  Dither(WORD NewBitsPerPixel)
    {
    char	s[300];
    LPBITMAPINFOHEADER BitMapPtr;

    Dib.DibTo24();			// Dither assumes we have a 24 bit image. PGV crashes if we don't.
    ReturnCode = NO_ERR;
    BitMapPtr = Floyd_Steinberg(NewBitsPerPixel);

    if(ReturnCode < 0)
	{
	switch(ReturnCode)
	    {
	    case ERR_INIT_TABLE:
	    	wsprintf(s, "Error Initialising Quantisation Table");
	    	break;
	    case ERR_USER_ABORT:
	    	wsprintf(s, "User Abort");
	    	break;
	    case ERR_QUANTISE_IMAGE:
	    	wsprintf(s, "Error Quantising Image");
	    	break;
	    case ERR_DCQ_START:
	    	wsprintf(s, "Error Starting DCQ");
	    	break;
	    case ERR_BUILD_TABLE:
	    	wsprintf(s, "Error Building Quantisation Table");
	    	break;
	    case ERR_CREATE_DIB_FAIL:
	    	wsprintf(s, "Cannot Create DIB for Quantisation");
	    	break;
	    case ERR_IMAGE_MEMORY_FAIL:
	    	wsprintf(s, "Insufficient Memory for Quantisation");
	    	break;
	    case ERR_ILLEGAL_BITS_PIXEL:
	    	wsprintf(s, "Illegal Quantisation bits per pixel <%d>", NewBitsPerPixel);
	    	break;
	    case ERR_ILLEGAL_DITHER_TYPE:
	    	wsprintf(s, "Illegal Dithering Requested");
	    	break;
	    }
	MessageBox (GlobalHwnd, s, "Quantisation", MB_ICONEXCLAMATION | MB_OK);
	SetWindowText(GlobalHwnd, s);
	return NULL;
	}

    // Format filename along with the DIB attributes
//    CreateTitleBar("", "", DitherDib.DibWidth, DitherDib.DibHeight, NewBitsPerPixel);
//    UpdateTitleBar(GlobalHwnd);
    ReturnCode = NO_ERR;
    return BitMapPtr;
    }

void	CloseDitherPointers(BYTE success)
    {
    DitherDib.CloseDibPtrs();
    }

