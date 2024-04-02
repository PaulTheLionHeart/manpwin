/*
    ROTATE.CPP - Routines that manipulate the Video DAC on VGA Adapters
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "manp.h"
#include <time.h>
//#include <libs.h>
#include <string.h>
#include "Dib.h"
#include "colour.h"

// global variables
	int	StartColourCycling;	// we can start the colour cycling from any point..good for cycling animations

/* routines in this module	*/

//static	void	pauserotate(void);
static	void	set_palette1(BYTE *, BYTE *),set_palette2(BYTE *, BYTE *),
		set_palette3(BYTE *, BYTE *, BYTE *);

extern	BYTE	orig_palette[];		/* loaded palette */
//extern	PAINTSTRUCT 	ps;
extern	RECT 		r;
extern	BYTE	cycleflag;		// do colour cycling
extern	long	threshold;		// maximun iterations
//extern	BYTE	TrueColourFlag;		// Use 3D palette generation
//extern	int	time_to_break;		// time to break out of animation?
extern	int	time_to_zoom;		// time to zoom in or out?
extern	int	time_to_restart;	// time to restart?
extern	int	time_to_reinit;		// time to reinitialize?
extern	int	time_to_quit;		// time to quit?

extern 	void	set_palette(void), bell(int);
extern	int	user_data(HWND);
//extern	void	init_windows_colours(HWND, char *);
extern	void	InitTrueColourPalette(BYTE);

static	int	paused;				/* rotate-is-paused flag */
static	BYTE	temp_palette[MAXTHRESHOLD * 3];

extern	CTrueCol    TrueCol;		// palette info
//extern	BYTE	*PalettePtr;			// points to true colour palette

static unsigned char Red[3]    = {255, 0, 0};	/* for shifted-Fkeys */
static unsigned char Green[3]  = { 0,255, 0};
static unsigned char Blue[3]   = { 0, 0,255};
static unsigned char Black[3]  = { 0, 0, 0};
static unsigned char White[3]  = {255,255,255};
static unsigned char Yellow[3] = {255,255, 0};
static unsigned char Brown[3]  = {127,127, 0};
/*
static unsigned char Magenta[3]  = {255,0, 255};
static unsigned char Cyan[3]  = {0,255,255};
*/

char mapmask[13] = {"*.map"};
//char mapfile[80] = {".\\"};

//extern	BYTE	 huge		*pixels;			// the device-independent bitmap  pixels
extern	double 	*wpixels;			// floating point iterations for each pixels
extern	HWND	GlobalHwnd;			// to allow passing of hwnd to find_file_item()

//#include "default.c"			/* default palette data */
extern	CDib	Dib;				// Device Independent Bitmap

/***************************************************************************
	Rotate VGA palette
***************************************************************************/

void	rotate_vga_palette(int dir, int delay)

    {
    long	i, k;
    int		j, m;
    long	local_width;
    DWORD	colour, n, pointer;

    if (dir == 1)
	{
	if (++StartColourCycling > threshold)
	    StartColourCycling -= threshold;
	}
    else
	{
	if (--StartColourCycling < 0)
	    StartColourCycling += threshold;
	}
//    InitTrueColourPalette(FALSE);

    local_width = WIDTHBYTES((DWORD)Dib.DibWidth * (DWORD)Dib.BitsPerPixel);
    for (j = 0; j < Dib.DibHeight - 1; ++j)
	{
	i = ((long) (Dib.DibHeight - 1 - j) * (long) (local_width + 3 - ((local_width - 1) % 4)));
	k = ((long)j * (long) Dib.DibWidth);
	for (m = 0L; m < Dib.DibWidth; ++m)
	    {
	    if (time_to_reinit + time_to_restart + time_to_quit + time_to_zoom)
		{
		cycleflag = FALSE;
		return;
		}
	    colour = (DWORD) (*(wpixels + k + m));
	    colour += StartColourCycling;
	    colour %= threshold;
	    n = colour + colour + colour;
	    pointer = m + m + m + i;
	    *(Dib.DibPixels + pointer + 0) = *(TrueCol.PalettePtr + n + 2);
	    *(Dib.DibPixels + pointer + 1) = *(TrueCol.PalettePtr + n + 1);
	    *(Dib.DibPixels + pointer + 2) = *(TrueCol.PalettePtr + n + 0);
	    }
//	for (n = 0; n < 3; ++n)
//	    *(pixels + i + m * 3 + n) = *(VGA_PALETTE + (((BYTE) (*(wpixels + k + m))) * 3L + 2 - n));
	}
    InvalidateRect(GlobalHwnd, &r, FALSE);
    }

/**************************************************************************
	Rotate the palette routine
**************************************************************************/

int rotate(int direction)

    {
    int	/*kbdchar, */more, last, next, maxreg;
    int	fkey, step, fstep, /*istep, jstep, oldstep, */ega_step, vga_speed;
    //int	incr, fromred, fromblue, fromgreen, tored, toblue, togreen;
    int	/*i, */changecolor, changedirection;

    static	BYTE	default_ega[] = {0,1,2,3,4,5,6,7,56,57,58,59,60,61,62,63};
    static	int	fsteps[] = {2,4,8,12,16,24,32,40,54,100}; /* (for Fkeys) */

    paused = FALSE;				/* not paused			*/
    step = 0;				/* single-step			*/
    ega_step = 0;				/* for rotation of EGA palette 	*/
    vga_speed = 0;				/* for rotation of VGA palette 	*/
    fkey = 0;				/* no random coloring		*/
    fstep = 1;
    changecolor = -1;			/* no color (rgb) to change	*/
    changedirection = 0;			/* no color derection to change */
    //incr = 999;				/* ready to randomize		*/
    srand((unsigned)time(NULL));		/* randomize things		*/

//if (direction == 0)
//    {					/* firing up in paused mode?	*/
//    pauserotate();			/* then force a pause		*/
//    direction = 1;			/* and set a rotate direction	*/
//    }

    maxreg = 256;				/* maximum register to rotate	*/

    last = maxreg;				/* last box that was filled	*/
    next = 1;				/* next box to be filled	*/
    if (direction < 0)
	{
	last = 1;
	next = maxreg;
	}

    more = 1;
    while (more) 
	{
	if (user_data(GlobalHwnd) == -1)	// rotate until a key gets hit  
	    return -1;    
	if (!cycleflag)			// rotate until a cycling turned off  
	    break;    

	rotate_vga_palette(direction, vga_speed);  
	}
    return 0;
    }

