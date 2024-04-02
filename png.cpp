/* PNG.CPP
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <windows.h>
#include <fcntl.h>
#include "manpwin.h"
#include "manp.h"
#include "Dib.h"
#include "colour.h"
#define PNG_READ_oFFs_SUPPORTED
#define PNG_INTERNAL
#define	PNG_SETJMP_SUPPORTED
#include ".\pnglib\png.h"
#include ".\pnglib\pngstruct.h"

// Compression factor -- this should be:
// 1 -- fastest writes, worst compression
// 9 -- slowest writes, best compression
// Z_DEFAULT_COMPRESSION (6) -- somewhere in the middle
#define	PNG_COMPRESSION	Z_DEFAULT_COMPRESSION
#define PNG_READ_tEXt_SUPPORTED
#define PNG_READ_zTXt_SUPPORTED
#define PNG_WRITE_tEXt_SUPPORTED
#define PNG_WRITE_zTXt_SUPPORTED

int	png_decoder(HWND, char *, char *);
int	decode_png_header(HWND, char *, char *);
//int	load_png_palette(HWND, char *, char *);

typedef unsigned long   ULONG;

extern	long	threshold;

extern	CTrueCol    TrueCol;			// palette info

static	char	PNG_error_buffer[240];
static	HWND	temphwnd;

static	char	*PaletteBuffer = NULL;
static	char	*PixelBuffer = NULL;

extern	RECT 	r;
extern	HDC	hdcMem;				// load picture into memory

extern	int	height, xdots, ydots, width, bits_per_pixel;

extern	BYTE	*GetOrthoPalette(BYTE *);
extern	void	SwapColours(WORD);
extern	char	*FractData(void);
extern	int	mainview(HWND, BOOL);
extern	int	GetParamData(HWND, LPSTR, LPSTR, LPSTR, BOOL);
extern	void	ClosePtrs(void);
extern	void	SaveTextPalette(char *);
//extern	void	SaveIterationsDatabase(char *IterationBuffer);
//extern	void	LoadIterationsDatabase(char *IterationBuffer, int length);
extern	void	LoadTextPalette(char *, int);
extern	void	setup_defaults(void);

extern	CDib	Dib;				// Device Independent Bitmap
extern	"C"	png_text	TextData[];

static	png_structp	read_ptr;
static	png_infop	read_info_ptr, end_info_ptr;
static	png_structp	write_ptr = NULL;
static	png_infop	write_info_ptr = NULL;
static	png_infop	write_end_info_ptr = NULL;
static	int		num_pass, pass;
static	int		bit_depth, color_type;
static	jmp_buf		jmpbuf;
static	png_textp	text_ptr;

//static	png_structp	png_ptr;
//static	png_info	*info_ptr;
static	FILE		*fp = NULL;;
int			DataFromPNGFile = FALSE;	// loaded PNG file?

/**************************************************************************
	Error Handling
**************************************************************************/

// This function is called when there is a warning, but the library thinks it can continue anyway.  Replacement functions don't have to do anything
// here if you don't want to.  In the default configuration, png_ptr is not used, but it is passed in case it may be useful.

static	void	png_default_warning(png_structp png_ptr, png_const_charp message)
    {
    PNG_CONST char *name = "UNKNOWN (ERROR!)";
    if (png_ptr != NULL && png_ptr->error_ptr != NULL)
	name = (char *)png_ptr->error_ptr;
//    fprintf(STDERR, "%s: PNGLib warning: %s\n", name, message);
    sprintf(PNG_error_buffer, "PNGLib warning: %s\n", message);
    }

// This is the default error handling function.  Note that replacements for this function MUST NOT RETURN, or the program will likely crash.  This
// function is used by default, or if the program supplies NULL for the error function pointer in png_set_error_fn().

static	void	png_default_error(png_structp png_ptr, png_const_charp message)
    {
    png_default_warning(png_ptr, message);
   // We can return because png_error calls the default handler, which is actually OK in this case.
    }

/**************************************************************************
	File Reading
**************************************************************************/

/* START of code to validate stdio-free compilation */
/* These copies of the default read/write functions come from pngrio.c and */
/* pngwio.c.  They allow "don't include stdio" testing of the library. */
/* This is the function that does the actual reading of data.  If you are
   not reading from a standard C stream, you should create a replacement
   read_data function and use it at run time with png_set_read_fn(), rather
   than changing the library. */

static	void	png_default_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
    {
    png_size_t check;

   // fread() returns 0 on error, so it is OK to store this in a png_size_t instead of an int, which is what fread() actually returns.
    
    check = (png_size_t)fread(data, (png_size_t)1, length, (FILE *)png_ptr->io_ptr);
    if (check != length)
	{
	png_error(png_ptr, "Read Error");
	}
    }

/**************************************************************************
	Main entry decoder
**************************************************************************/

int	decode_png_header(HWND hwnd, char *infile, char *szAppName)

    {
    char	s[480];
    int		interlace_type, compression_type, filter_type;

//    temphwnd = hwnd;			// yeah, yeah should be done properly using pointers!!

    if ((fp = fopen(infile,"rb")) == NULL) 
	{
	wsprintf(s, "Unable to open file: %s", infile);
	MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

					// allocate the necessary structures
    // Allocating read structures
    read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (read_ptr == NULL)
	return -1;

    png_set_error_fn(read_ptr, (png_voidp)infile, png_default_error, png_default_warning);

    // Allocating read_info and end_info structures
    read_info_ptr = png_create_info_struct(read_ptr);
    if (read_info_ptr == NULL)
	{
	png_destroy_read_struct(&read_ptr, (png_infopp)NULL, (png_infopp)NULL);
	return -1;
	}
    end_info_ptr = png_create_info_struct(read_ptr);
    if (end_info_ptr == NULL)
	{
	png_destroy_read_struct(&read_ptr, &read_info_ptr, (png_infopp)NULL);
	return (ERROR);
	}

    // Setting jmpbuf for read struct
    if (setjmp(png_jmpbuf(read_ptr)))
	{
	wsprintf(s, "Error: %s in PNG file: %s", PNG_error_buffer, infile);
	MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	fclose(fp);
	return (-1);
	}

    // Initializing input stream
    png_set_read_fn(read_ptr, (png_voidp)fp, png_default_read_data);
    png_set_read_status_fn(read_ptr, NULL);
    // Reading info struct
    png_read_info(read_ptr, read_info_ptr);
   // flip the rgb pixels to bgr 
    if (read_ptr->color_type == PNG_COLOR_TYPE_RGB || read_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	png_set_bgr(read_ptr);

    // Transferring info struct
    if (png_get_IHDR(read_ptr, read_info_ptr, (png_uint_32 *)&width, (png_uint_32 *)&height, &bit_depth, &color_type, &interlace_type, &compression_type, &filter_type))
	    png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth, color_type, interlace_type, compression_type, filter_type);

    width = (WORD)(read_ptr->width);				// with of file
    height = (WORD)(read_ptr->height);				// height of file
    bits_per_pixel = (WORD)(read_ptr->pixel_depth);		// number of bits per pixel
//    png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette);
//    memcpy(TrueCol.PalettePtr, read_ptr->palette, RGB_SIZE * read_ptr->num_palette);	// read palette
    return 0;
    }

/**************************************************************************
	Decode the PNG Image
 *************************************************************************/

int   png_decoder(HWND hwnd, char *szAppName, char *infile)

    {
    DWORD	linesize;
    char	s[480];
    long	i;
    int		num_text;
    long	bytes;
    long	bytes_per_pixel;
    BYTE	*LinePtr = Dib.DibPixels;
    png_bytep	*row_pointers;

    // Setting jmpbuf for read struct
    if (setjmp(png_jmpbuf(read_ptr)))
	{
	wsprintf(s, "Error: %s in PNG file: %s", PNG_error_buffer, infile);
	MessageBox(hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	fclose(fp);
	return (-1);
	}

    bytes = WIDTHBYTES((DWORD)read_ptr->width * (DWORD)Dib.BitsPerPixel);
    bytes_per_pixel = bytes / (DWORD)read_ptr->width;
    linesize = (Dib.BitsPerPixel > 8) ? (WORD)WIDTHBYTES((DWORD)Dib.DibWidth * (DWORD)Dib.BitsPerPixel) : Dib.DibWidth;

    if ((row_pointers = new png_bytep[Dib.DibHeight]) == NULL)
	{
	// free the structures 
	if (read_ptr != NULL)
	    png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	fclose(fp);
	sprintf(s, "Not enough memory: %d bytes for line buffer in PNG file: ", linesize);
	MessageBox(hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    for (i = Dib.DibHeight - 1; i >= 0; i--)
	{
	row_pointers[i] = (png_bytep)LinePtr;
	LinePtr += linesize;
	}
    png_read_image(read_ptr, row_pointers);

   // read the rest of the file, getting any additional chunks in info_ptr 
    png_read_end(read_ptr, read_info_ptr);
    if (png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text) > 0)
	{
	for (i = 0; i < (WORD)num_text; i++)
	    {
	    if (strncmp(TextData[i].key, "Comment", 7) == 0)
		{
		if (TextData[i].text_length > 0)
		    {
		    setup_defaults();
		    GetParamData(hwnd, infile, TextData[i].text, "", TRUE);
		    }
		}
// do we really need to store this?
//	    if (strncmp(TextData[i].key, "Pixels", 6) == 0)
//		{
//		if (TextData[i].text_length > (size_t)(width * height))			// yep, we have pixels
//		    LoadIterationsDatabase(TextData[i].text, (int)TextData[i].text_length);
//		}
	    if (strncmp(TextData[i].key, "Palette", 7) == 0)
		{
		if (TextData[i].text_length > 16)		// yep, we have a palette override palette in file if this is found
		    LoadTextPalette(TextData[i].text, (int)TextData[i].text_length);
		}
	    }
	}

   // clean up after the read, and free any memory allocated 
    png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);

   // free the structures and line buffer
    // clean up after the read, and free any memory allocated 
    png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);

    // free the structures and line buffer
    if (row_pointers != NULL)
	delete[] row_pointers;
    // close the file 
    fclose(fp);
   // that's it 
    return(0);
    }

/***************************************************************************
	Paint screen from disk data
***************************************************************************/

int	read_png_file(HWND hwnd, char *infile)

    {
    char	s[120];
    static HCURSOR	hCursor;

    hCursor = LoadCursor((HINSTANCE)NULL, IDC_WAIT);
    SetCursor(hCursor);
    if (decode_png_header(hwnd, infile, "ManpWin") < 0)
	{
	wsprintf(s, "Error: Could not read header in file: <%s> for read", infile);
	MessageBox (hwnd, s, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    xdots = width;
    ydots = height;
    ClosePtrs();				// ready for next screen
    mainview(hwnd, TRUE);				// all screen specific stuff

    if (png_decoder(hwnd, infile, "ManpWin") < 0)
	{
	wsprintf(s, "Error: Could not open file: <%s> for read", infile);
	MessageBox (hwnd, s, "ManpWin", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    DataFromPNGFile = TRUE;			// don't splatter screen by loading data over image
//    palette_flag = TRUE;
    return (ydots - 1);
    SetCursor(hCursor);
    }

/**************************************************************************
	Write Bitmap Image to Portable Network Graphics (PNG) File 
**************************************************************************/

int	write_png_file(HWND hwnd, char *outfile, char *szAppName, char *CommentText)

    {
    char		s[480];
    static HCURSOR	hCursor;
    BYTE		**row_pointers = NULL;
    FILE		*fp = NULL;
    WORD		i;
    png_text	TextInfo;
    long	LocalThreshold;

    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;

    hCursor = LoadCursor((HINSTANCE)NULL, IDC_WAIT);
    SetCursor(hCursor);
    if ((fp = fopen(outfile,"wb")) == NULL) 
	{
	hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// Load pointer cursor.
	SetCursor(hCursor);
	fprintf(stderr, "can't open %s\n", outfile);
	wsprintf(s, "PNG can't open output file: %s", outfile);
	MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    write_ptr =  png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (write_ptr == NULL)
	return -1;

    // Allocating write_info and end_info structures
    write_info_ptr = png_create_info_struct(write_ptr);
    if (write_info_ptr == NULL)
	{
	png_destroy_write_struct(&write_ptr, NULL);
	return -1;
	}
    write_end_info_ptr = png_create_info_struct(write_ptr);
    if (write_end_info_ptr == NULL)
	{
	png_destroy_read_struct(&write_ptr, &write_info_ptr, &write_end_info_ptr);
	return -1;
	}

    png_set_error_fn(write_ptr, (png_voidp)outfile, png_default_error, png_default_warning);

    // Setting jmpbuf for read struct
    if (setjmp(png_jmpbuf(write_ptr)))
	{
	wsprintf(s, "Error: %s in PNG file: %s", PNG_error_buffer, outfile);
	MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	png_destroy_write_struct(&write_ptr, &write_info_ptr);
	if(fp != NULL)
	    fclose(fp);
	return (-1);
	}
    png_init_io(write_ptr, fp);
    png_set_compression_level(write_ptr,PNG_COMPRESSION);
    bit_depth = 8;
    color_type = PNG_COLOR_TYPE_RGB;
    png_set_bgr(write_ptr);
    png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
//    write_ptr->channels = 3;
    png_write_info(write_ptr,write_info_ptr);
    if ((row_pointers = (BYTE **) new BYTE[height * sizeof(BYTE *)]) == NULL)
	{
	if (fp != NULL)
	    fclose(fp);
	png_destroy_write_struct(&write_ptr, &write_info_ptr);
	hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);	// Load pointer cursor.
	SetCursor(hCursor);
	wsprintf(s, "Error: can't allocate memory for row pointer in PNG file: %s", PNG_error_buffer, outfile);
	MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    for (i = 0; i < height; ++i)
	row_pointers[height - i - 1] = Dib.DibPixels + (long)i * (long)(WIDTHBYTES(width * bits_per_pixel));
    png_write_image(write_ptr, row_pointers, hwnd);

	     // write uncompressed chunk
    SetWindowText(hwnd, "Writing parameter info");
    TextInfo.compression = PNG_TEXT_COMPRESSION_NONE;		// compression value
    TextInfo.key = "Comment";					// keyword, 1-79 character description of "text"
//    TextInfo.text = FractData();				// comment, may be an empty string (ie "")
    TextInfo.text = CommentText;				// comment, may be an empty string (ie "")
    TextInfo.text_length = strlen(TextInfo.text);		// length of "text" field
    png_set_text(write_ptr,write_info_ptr, &TextInfo, 1);

/*
    if(PixelBuffer = new char[(DWORD)width * (DWORD)height * 5L])	// we use 5 characters of 6 bits to make an iteration count
	{
	     // write compressed chunk
	SetWindowText(hwnd, "Writing pixel info");
	TextInfo.compression = PNG_TEXT_COMPRESSION_zTXt;	// compression value
//	TextInfo.compression = PNG_TEXT_COMPRESSION_NONE;	// compression value
	TextInfo.key = "Pixels";				// keyword, 1-79 character description of "text"
	*PixelBuffer = '\0';					// start at the beginning
	SaveIterationsDatabase(PixelBuffer);
	TextInfo.text = PixelBuffer;				// comment, may be an empty string (ie "")
	TextInfo.text_length = strlen(TextInfo.text);		// length of "text" field
	png_set_text(write_ptr,write_info_ptr, &TextInfo, 1);
	}
*/

    if(PaletteBuffer = new char[LocalThreshold * 5])		// we use 5 characters of 6 bits to make an RGB triplet
	{
	     // write compressed chunk
	SetWindowText(hwnd, "Writing palette info");
	TextInfo.compression = PNG_TEXT_COMPRESSION_zTXt;	// compression value
//	TextInfo.compression = PNG_TEXT_COMPRESSION_NONE;	// compression value
	TextInfo.key = "Palette";				// keyword, 1-79 character description of "text"
	*PaletteBuffer = '\0';					// start at the beginning
	SaveTextPalette(PaletteBuffer);
	TextInfo.text = PaletteBuffer;				// comment, may be an empty string (ie "")
	TextInfo.text_length = strlen(TextInfo.text);		// length of "text" field
	png_set_text(write_ptr,write_info_ptr, &TextInfo, 1);
	}

//    png_set_PLTE(write_ptr, write_info_ptr, (png_const_colorp)TrueCol.PalettePtr, 250);
    png_write_end(write_ptr, write_info_ptr);
//    png_write_destroy(png_ptr);
    png_destroy_write_struct(&write_ptr, &write_info_ptr);

    if(fp != NULL)
	fclose(fp);
    if (row_pointers != NULL)
	delete[] row_pointers; row_pointers = NULL;
    if (PaletteBuffer)
	delete [] PaletteBuffer;
    if (PixelBuffer)
	delete [] PixelBuffer;

    hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);		// Load pointer cursor.
    SetCursor(hCursor);
    return 0;
    }


