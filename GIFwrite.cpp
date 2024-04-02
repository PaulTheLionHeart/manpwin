/*
	GIF Library
	Copyright (c) 1999 - 2011 Paul de Leeuw Computers
*/

//note: if the GIF reader returns BAD_FILE on a good file, you have the
//default char type set incorrectly. It should be unsigned.

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "manpwin.h"
#include "Dib.h"
#include "Anim.h"

//#define	ALLOWGIFANIMSAVE
#define MAXLINE		150		/* length of line */
#define	WRITE_GIF89	TRUE

#define	GIFCOMMENT	"This GIF file was created by ManpWIN\r"\
			"Copyright \251 1997 - 2012 Paul de Leeuw Computers."

#define NO_CODE		-1
#define GIFLARGESTCODE	4095		// largest possible code
#define GIFTABLESIZE	5003		// table dimensions
#define	GIFSIG87	"GIF87a"
#define	GIFSIG89	"GIF89a"

#define	GIFVERSION	59
#define	GIFSUBVERSION	GIFVERSION+2
#define	GIFREVISION	GIFSUBVERSION+1

#define	BAD_OPEN		-1
#define	BAD_FILE		-2
#define	BAD_READ		-3
#define	BAD_ALLOC		-4
#define	UNSUPPORTED_FUNCTION	-5
#define	BAD_CREATE		-6
#define	BAD_WRITE		-7
#define	TOO_MANY_BITS		-8
#define	MAXRESULTS		-9

/////////////////////////////////////////
// A note about animated GIF saving. The frames exist in 24 bit colour in PGV as all formats and colour depths are
// stored in this format. The Animated GIF file is written as a 256 colour, 8 bits per pixel image. Dithering is used
// to generate an 8 bpp image and the palette is created in this process and is written to the Device Independent 
// Bitmap (DIB) for each frame. This is extracted from the dithered DIB and written out as the palette.
/////////////////////////////////////////

static	WORD		ImageHeight, ImageWidth;

extern	CDib		Dib;			// Device Independent Bitmap class instance
//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;			// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

extern	BYTE	DitherBitsPerPixel;		// for reducing bits per pixel
extern	BYTE	masktable[8];
extern	WORD	CurrentFrame;			// used to increment to the next frame when the timer ticks
extern	int	file_type;
extern	BYTE	Stepflag;			// single step animated GIFs?
extern	HCURSOR	hStdCursor;
extern	double	CurrentDelay;			// delay in milliseconds
extern	BOOL	AnimationForward;		// order of file frames

extern	void	CloseDitherPointers(BYTE);
extern	int	GetDibPalette(LPBITMAPINFO,BYTE *);
extern	LPBITMAPINFOHEADER  Dither(WORD);
extern	void	ShowOutfileMessage(char *);
extern	int	AnimateGIF(void);
extern	int	user_data(HWND);

typedef struct {
	char sig[6];
	WORD screenwidth,screendepth;
	BYTE flags,background;	// force even Byte boundary on structure
	} GIFHEADER;

typedef struct {
	WORD left,top,width,depth;
	} GIFIMAGEBLOCK;

#define GetChunkyPixel(pxx,nxx) (!((nxx) & 1)) ? (((pxx)[(nxx)>>1] >> 4) & 0x0f) : ((pxx)[(nxx)>>1] & 0x0f)

void	GIFDoSkipExtension(FILE * fh);
int	GIFDoUnpackImage(FILE * fh,int bits,LPBITMAPINFOHEADER lpbi,int flags);
int	GIFWriteScreenDesc(FILE * fh,LPBITMAPINFOHEADER lpbi,LPSTR sig);
int	GIFWriteImageDesc(FILE * fh,int i, LPBITMAPINFOHEADER lpbi);
void	GIFInitTable(int min_code_size);
int	GIFFlush(FILE * fh,int n);
void	GIFWriteCode(FILE * fh,int code);
int	GIFCompressImage(FILE * fh,LPBITMAPINFOHEADER lpbi);
int	GIFCompressAnimImage(FILE * fh,LPBITMAPINFOHEADER lpbi,int i);
int	GIFWriteComment(FILE * fh,LPSTR comment);
int	GIFGraphicControlExtension (FILE *fh);

static	int	*oldcode;
static	int	*currentcode;
static	LPSTR	newcode;

static	BYTE	code_buffer[259];		// where the codes go

static	int	code_size;
static	int	clear_code;
static	int	eof_code;
static	int	bit_offset;
static	int	byte_offset;
static	int	bits_left;
static	int	max_code;
static	int	free_code;

// Stuff needed to write an animated GIF

extern	int	TotalFrames;		// total number of animation framese
extern	AnimStruct	ANIM[];		// holds all the date for each animation frame
//extern	WORD	BackGroundColour;	// points to location of background colour in the palette

	BOOL		RunGIF = FALSE;	// are we in the middle of generating a GIF file?

///////////////////////////////////////////////////////////////
// Put a byte out on the appointed stream
///////////////////////////////////////////////////////////////

int	putbyte(int byte,FILE *fh)

    {
    BYTE	c;

    c = byte & 0xff;
    if (fwrite((LPSTR)&c,1, 1, fh) != 1)
	return(EOF);
    else
	return(byte);
    }

///////////////////////////////////////////////////////////////
// Get the number of bits per pixel from the control flags
///////////////////////////////////////////////////////////////

WORD	GetBitsPerPixel(BYTE flags)
    {
    int	numcolors;

    numcolors = 1 << ((flags & 0x07) + 1);
    return ((numcolors > 16) ? 8 : ((numcolors > 2) ? 4 : 1));
    }

/**************************************************************************
	Cleanup
**************************************************************************/

void	CleanupGIF(FILE *fp, LPSTR bp, LPSTR extrabuffer)
    {
    fclose (fp);
    CloseDitherPointers(FALSE);
    if(extrabuffer != NULL)
	delete [] extrabuffer;
    if(bp != NULL)
	delete [] bp;
    }

/**************************************************************************
	Write GIF Image to File
**************************************************************************/

int	write_gif_file(char *outfile, char *title)

    {
    char	s[MAXLINE];
    LPBITMAPINFOHEADER lpbi = NULL;
    LPSTR	bp = NULL;
    DWORD	size;
    int	i;
    static	HCURSOR  hOldCursor;
    static	HCURSOR	hCursor;
    FILE	*fp;
    LPSTR	extrabuffer = NULL;			// to decode 1, 4 bits to bytes
    WORD	linesize;

    hOldCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
    DitherBitsPerPixel = 8;				// just force 8 bits per pixels and be done with it
    RunGIF = TRUE;					// let us know if user has hit a key (keyboard.cpp)

    if ((fp = fopen(outfile,"wb")) == NULL) 
	{
	hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// Load pointer cursor.
	SetCursor(hCursor);
	wsprintf(s, "GIF can't open output file: %s", outfile);
	MessageBox (GlobalHwnd, s, title, MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    Dib = (AnimationForward) ? ANIM[0].animDIB : ANIM[TotalFrames - 1].animDIB;
    if (ANIM[0].animFrameData == NULL)
	{
	MessageBox(GlobalHwnd, "Can't write GIF file due to no image in first frame ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	fclose (fp);
	SetCursor(hStdCursor);
	return -1;
	}

    if ((lpbi = Dither(DitherBitsPerPixel)) == NULL)			// drop DIB to 8 bits per pixel 
	{
	MessageBox(GlobalHwnd, "Can't write GIF file due to dither fail ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	fclose (fp);
	SetCursor(hStdCursor);
	return -1;
	}

    ImageWidth = Dib.DibWidth;
    ImageHeight = Dib.DibHeight;

    linesize = WIDTHBYTES(ImageWidth * 24);		// even if 8 bits/pixel, need to alloc enough for 24 bit output!!

    if ((extrabuffer = new char [linesize]) == NULL)
	{
	wsprintf(s, "Not enough memory: %d bytes for line buffer in JPEG file ", "GIF write", linesize);
	MessageBox(GlobalHwnd, s, "GIF write", MB_ICONEXCLAMATION | MB_OK);
	CleanupGIF(fp, bp, extrabuffer);
	SetCursor(hStdCursor);
	return -1;
	}

//size = (DWORD)(GIFTABLESIZE * sizeof(int)) + (DWORD)(GIFTABLESIZE * sizeof(int)) + (DWORD)GIFTABLESIZE;
    size = (DWORD)(GIFTABLESIZE * sizeof(int)) * 3L;    // enough for old code, new code and the GIFTABLE itself

    if ((bp = new char [size]) == NULL)
	{
	MessageBox(GlobalHwnd, "Can't get memory for GIF encode table in file ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	CleanupGIF(fp, bp, extrabuffer);
	SetCursor(hStdCursor);
	return -1;
	}

    oldcode = (int *)bp;
    currentcode = (int *)(bp + GIFTABLESIZE * sizeof(int));
    newcode = (bp + GIFTABLESIZE * sizeof(int) + GIFTABLESIZE * sizeof(int));

#if WRITE_GIF89
    if (!GIFWriteScreenDesc(fp,lpbi,GIFSIG89))
#else
if (!GIFWriteScreenDesc(output_file,lpbi,GIFSIG87))
#endif
	{
	MessageBox(GlobalHwnd, "Can't write screen descriptor in file ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	CleanupGIF(fp, bp, extrabuffer);
	SetCursor(hStdCursor);
	return -1;
	}

    GIFGraphicControlExtension(fp);
    if (!GIFWriteImageDesc(fp, 0, lpbi))	// just do the original way without writing delay etc
	{
	MessageBox(GlobalHwnd, "Can't write screen descriptor in file ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	CleanupGIF(fp, bp, extrabuffer);
	SetCursor(hStdCursor);
	return -1;
	}

    if (!GIFCompressImage(fp,lpbi))
	{
	MessageBox(GlobalHwnd, "Can't write GIFCompressImage in file ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	CleanupGIF(fp, bp, extrabuffer);
	SetCursor(hStdCursor);
	return -1;
	}

    for (i = 1; i < TotalFrames; i++)			// must be an animated GIF or animated cursor
	{
/*
	if (ANIM[i] == NULL)
	    {
	    MessageBox(GlobalHwnd, "Can't write GIF file due to no image in memory ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	    fclose (fp);
	    SetCursor(hStdCursor);
	    return -1;
	    }
*/
	Dib = (AnimationForward) ? ANIM[i].animDIB : ANIM[TotalFrames - i - 1].animDIB;
//	Dib = ANIM[i].animDIB;
	if ((lpbi = Dither(DitherBitsPerPixel)) == NULL)
	    {
	    MessageBox(GlobalHwnd, "Can't write GIF file due to dither fail ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	    CleanupGIF(fp, bp, extrabuffer);
	    return -1;
	    }

	wsprintf (s, "Writing GIF frame <%d>of<%d>: ", i + 1, TotalFrames);
	SetWindowText(GlobalHwnd, s);			// Show formatted text in the caption bar
	if (!GIFWriteImageDesc(fp, i, lpbi))
	    {
	    MessageBox(GlobalHwnd, "Can't write screen descriptor in file ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	    CleanupGIF(fp, bp, extrabuffer);
	    return -1;
	    }

	if (!GIFCompressImage(fp,lpbi))
	    {
	    MessageBox(GlobalHwnd, "Can't write GIFCompressImage in file ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	    CleanupGIF(fp, bp, extrabuffer);
	    return -1;
	    }

	CloseDitherPointers(TRUE);
	user_data(GlobalHwnd);			// allow breaking of loop
	if (!RunGIF)				// user hit the escape key
	    {
	    CleanupGIF(fp, bp, extrabuffer);
	    return -1;
	    }
	}

#if WRITE_GIF89
    if (GIFWriteComment(fp, GIFCOMMENT) < 0)
	{
	MessageBox(GlobalHwnd, "Can't write comment in file ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	CleanupGIF(fp, bp, extrabuffer);
	SetCursor(hStdCursor);
	return -1;
	}
#endif

    if (putbyte(';',fp) == EOF)
	{
	MessageBox(GlobalHwnd, "Can't write image tag ';' in file ", "GIF write", MB_ICONEXCLAMATION | MB_OK);
	CleanupGIF(fp, bp, extrabuffer);
	SetCursor(hStdCursor);
	return -1;
	}

    CleanupGIF(fp, bp, extrabuffer);
    SetCursor(hStdCursor);
    RunGIF = FALSE;
    return(TRUE);
    }

/**************************************************************************
	Start of routines
**************************************************************************/

int	GIFWriteScreenDesc(FILE *fh, LPBITMAPINFOHEADER lpbi, LPSTR sig)

    {
    GIFHEADER gh;
    BYTE	palette[VGA_PAL_SIZE];
    WORD	n;

    memset((LPSTR)&gh, 0, sizeof(GIFHEADER));
    memcpy(gh.sig, sig, 6);

    gh.screenwidth = (WORD)lpbi->biWidth;
    gh.screendepth = (WORD)lpbi->biHeight;
    gh.background = 0;

    gh.flags = (BYTE)(0x80 | ((DitherBitsPerPixel - 1) << 4) | ((DitherBitsPerPixel - 1) & 0x07));

    if (fwrite((LPSTR)&gh, 1, sizeof(GIFHEADER), fh) != sizeof(GIFHEADER))
	return(FALSE);

    putbyte(0, fh);
    GetDibPalette((LPBITMAPINFO)lpbi, palette);

    n = 3 * (1 << DitherBitsPerPixel);
    if (fwrite(palette, 1, n, fh) != n)
	return(FALSE);

    return(TRUE);
    }


int	GIFGraphicControlExtension (FILE *fh)

{
    if (TotalFrames > 0)				// this is only needed if we write animation frames
	{
	putbyte('!',fh);				// Extension introducer
	putbyte(0xff,fh);				// Graphic control extension label
	putbyte(0x0b,fh);				// block size (= 11)
	putbyte('N',fh);				// Application Identifier
	putbyte('E',fh);
	putbyte('T',fh);
	putbyte('S',fh);
	putbyte('C',fh);
	putbyte('A',fh);
	putbyte('P',fh);
	putbyte('E',fh);
	putbyte('2',fh);
	putbyte('.',fh);
	putbyte('0',fh);
	putbyte(0x03,fh);				// Application data ???
	putbyte(0x01,fh);				
	putbyte(0x00,fh);				// loop animation
	putbyte(0x00,fh);				// loop animation
	putbyte(0x00,fh);				// end
	}

    return(TRUE);
    }

int	GIFWriteImageDesc(FILE *fh, int i, LPBITMAPINFOHEADER lpbi)

    {
    GIFIMAGEBLOCK ib;
    BYTE	flags, transparent_colour;
    WORD	delay, n, local_colours, planes;
    BYTE	palette[VGA_PAL_SIZE];

    memset((LPSTR)&ib, 0, sizeof(GIFIMAGEBLOCK));

    putbyte('!',fh);				// Extension introducer
    putbyte(0xf9,fh);				// Graphic control label
    putbyte(0x04,fh);				// block size (= 4)
    flags = 0;
    putbyte(flags,fh);
    delay = (WORD)(CurrentDelay/10.0);		// CurrentDelay in milliseconds	GIF is in 100 ms
    if (fwrite((LPSTR)&delay, 1, sizeof(WORD), fh) != sizeof(WORD))
	return(FALSE);
    transparent_colour = 0;
    putbyte(transparent_colour, fh);		// transparency index
    putbyte(0x00,fh);				// block terminator

    putbyte(',',fh);
    ib.left = 0;
    ib.top = 0;
//ib.width = (WORD)ImageWidth;
//ib.depth = (WORD)ImageHeight;
    ib.width = (WORD)lpbi->biWidth;
    ib.depth = (WORD)lpbi->biHeight;

    flags = (BYTE)(DitherBitsPerPixel - 1);
    if (TotalFrames > 0)
	flags |= 0x80;				// force local palette

    if (fwrite((LPSTR)&ib, 1, sizeof(GIFIMAGEBLOCK), fh) != sizeof(GIFIMAGEBLOCK))
	return(FALSE);

    putbyte(flags,fh);
    if (flags & 0x80)	 			// local map?
	{
	planes = (flags & 0x07) + 1;
	local_colours = 1 << planes;
	GetDibPalette((LPBITMAPINFO)lpbi, palette);
	n = 3 * (1 << DitherBitsPerPixel);
	if (fwrite(palette, 1, n, fh) != n)
	    return(FALSE);
	}
    return(TRUE);
    }

void GIFInitTable(int min_code_size)

    {
    int	i;

    code_size = min_code_size + 1;
    clear_code = (1 << min_code_size);
    eof_code = clear_code + 1;
    free_code = clear_code + 2;
    max_code = (1 << code_size);

    for (i = 0; i < GIFTABLESIZE; i++)
	*(currentcode + i) = 0;
    }

int	GIFFlush(FILE *fh, int n)

    {
    putbyte(n,fh);
    if ((int)fwrite((LPSTR)code_buffer, 1, n, fh) != n)
	return(FALSE);
    else
	return(TRUE);
    }

void GIFWriteCode(FILE *fh, int code)

    {
    long	temp;

    byte_offset = bit_offset >> 3;
    bits_left = bit_offset & 7;

    if (byte_offset >= 251)
	{
	GIFFlush(fh,byte_offset);
	code_buffer[0] = code_buffer[byte_offset];
	bit_offset = bits_left;
	byte_offset = 0;
	}

    if (bits_left > 0)
	{
	temp = ((long)code << bits_left) | code_buffer[byte_offset];
	code_buffer[byte_offset] = (BYTE)temp;
	code_buffer[byte_offset+1] = (BYTE)(temp >> 8);
	code_buffer[byte_offset+2] = (BYTE)(temp >> 16);
	}
    else
	{
	code_buffer[byte_offset] = (BYTE)code;
	code_buffer[byte_offset+1]=(BYTE)(code >> 8);
	}
    bit_offset += code_size;
    }

int	GIFCompressImage(FILE *fh, LPBITMAPINFOHEADER lpbi)

    {
    BYTE	*linebuffer, *extrabuffer;
    int	prefix_code;
    int	suffix_char;
    int	hx,d,min_code_size;
    int	bytes_left = 0, next_byte = 0, this_line = 0;
    int	i;
    WORD	linewidth;
    int	percent, display_count;
    char	s[120];

    ImageWidth = (WORD)lpbi->biWidth;
    ImageHeight = (WORD)lpbi->biHeight;

    percent = 0;
//    SetWindowText (GlobalHwnd, "Writing GIF File");	// Show formatted text in the caption bar

//linewidth = width;
    linewidth = WIDTHBYTES(ImageWidth * Dib.BitsPerPixel);

    if ((linebuffer = new BYTE [linewidth]) == NULL)
	return(BAD_ALLOC);

    if ((extrabuffer = new BYTE [linewidth]) == NULL)
	{
	delete [] linebuffer;
	return(BAD_ALLOC);
	}

    min_code_size = DitherBitsPerPixel;
    if (min_code_size < 2 || min_code_size > 9)
	{
	if (min_code_size == 1)
	    min_code_size = 2;
	else
	    {
	    delete [] extrabuffer;
	    delete [] linebuffer;
	    return(BAD_WRITE);
	    }
	}

    putbyte(min_code_size, fh);

    bit_offset = 0;
    GIFInitTable(min_code_size);
    GIFWriteCode(fh,clear_code);

    memcpy(linebuffer,LPBimage(lpbi) + WIDTHBYTES((DWORD)ImageWidth * (DWORD)DitherBitsPerPixel)
				    * (DWORD)(ImageHeight - 1 - this_line++), (DWORD)LPBlinewidth(lpbi));

    if (DitherBitsPerPixel == 1)
	{
	memcpy(extrabuffer,linebuffer,LPBlinewidth(lpbi));
	for (i = 0; i < ImageWidth; ++i)
	    {
	    if (extrabuffer[i >> 3] & masktable[i & 0x0007])
		linebuffer[i] = 1;
	    else
		linebuffer[i] = 0;
	    }
	}
    else if(DitherBitsPerPixel > 1 && DitherBitsPerPixel <= 4)
	{
	memcpy(extrabuffer,linebuffer,LPBlinewidth(lpbi));
	for (i = 0; i < ImageWidth; ++i)
	    {
	    linebuffer[i]=GetChunkyPixel(extrabuffer,i);
	    }
	}

    bytes_left = ImageWidth - 1;
    next_byte = 0;
    suffix_char = linebuffer[next_byte++];

    prefix_code = suffix_char;

    for (;;)
	{
	display_count = (10 * this_line) / ImageHeight;
	if (display_count > percent)
	    {
	    percent = display_count;
	    sprintf (s, "Writing File: %d%%", percent * 10);
//	    SetWindowText(GlobalHwnd, s);
	    }

	if (bytes_left > 0)					// we still have bytes left to compress in this line
	    {
	    suffix_char = linebuffer[next_byte++];
	    --bytes_left;
	    }
	else
	    {
	    if (this_line >= ImageHeight)			// we have compressed all the lines
		break;

	    memcpy(linebuffer,LPBimage(lpbi) + WIDTHBYTES((DWORD)ImageWidth * (DWORD)DitherBitsPerPixel)
				    * (DWORD)(ImageHeight - 1 - this_line++), (DWORD)LPBlinewidth(lpbi));    // load next line
	    if(DitherBitsPerPixel == 1)
		{
		memcpy(extrabuffer,linebuffer,LPBlinewidth(lpbi));
		for (i = 0; i < LPBwidth(lpbi); ++i)
		    {
		    if (extrabuffer[i >> 3] & masktable[i & 0x0007])
			linebuffer[i] = 1;
		    else
			linebuffer[i] = 0;
		    }
		}
	    else if (DitherBitsPerPixel > 1 && DitherBitsPerPixel <= 4)
		{
		memcpy(extrabuffer,linebuffer,LPBlinewidth(lpbi));
		for (i = 0; i < LPBwidth(lpbi); ++i)
		    {
		    linebuffer[i] = GetChunkyPixel(extrabuffer, i);
		    }
		}

	    bytes_left = ImageWidth - 1;
	    next_byte = 0;
	    suffix_char = linebuffer[next_byte++];
	    }

	hx = (prefix_code ^ (suffix_char << 5)) % GIFTABLESIZE;
	d = 1;

	for (;;)						// start the compression
	    {
	    if (*(currentcode + hx) == 0)
		{
		GIFWriteCode(fh,prefix_code);
		d = free_code;
		if(free_code <= GIFLARGESTCODE)
		    {
		    *(oldcode + hx) = prefix_code;
		    *(newcode + hx) = (BYTE)suffix_char;
		    *(currentcode + hx) = free_code;
		    free_code++;
		    }

		if (d == max_code)
		    {
		    if (code_size < 12)
			{
			code_size++;
			max_code <<= 1;
			}
		    else
			{
			GIFWriteCode(fh,clear_code);
			GIFInitTable(min_code_size);
			}
		    }
		prefix_code = suffix_char;
		break;
		}
	    if(*(oldcode + hx) == prefix_code && *(newcode + hx) == suffix_char)
		{
		prefix_code = *(currentcode + hx);
		break;
		}
	    hx += d;
	    d += 2;
//	if (hx >= GIFTABLESIZE)
//	    hx -= GIFTABLESIZE;
	    if (hx >= GIFTABLESIZE)		// catch any silly values
		{
		hx -= GIFTABLESIZE;
		d = 1;
		}
	    }
	}

    GIFWriteCode(fh,prefix_code);

    GIFWriteCode(fh,eof_code);

    if(bit_offset > 0)
	GIFFlush(fh,(bit_offset + 7) / 8);

    if(!GIFFlush(fh,0))
	{
	delete [] extrabuffer;
	delete [] linebuffer;
	return(FALSE);
	}

    delete [] extrabuffer;
    delete [] linebuffer;

    return(TRUE);
    }

int	GIFWriteComment(FILE *fh,LPSTR comment)

    {
    int n;

    n=lstrlen(comment);

    putbyte('!',fh);
    putbyte(0xfe,fh);

    do  {
	if (n > 255)
	    {
	    putbyte(255,fh);
	    if(fwrite(comment, 1,255, fh) != 255)
		return(FALSE);
	    comment +=255;
	    n-=255;
	    }
	else
	    {
	    putbyte(n,fh);
	    if((int)fwrite(comment,1,n,fh) != n)
		return(FALSE);
	    putbyte(0,fh);
	    n=0;
	    }
	} while(n);
    return(TRUE);
    }

