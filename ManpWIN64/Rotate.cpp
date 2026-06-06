/*
    ROTATE.CPP - Routines that manipulate the Video DAC on VGA Adapters
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <vector>
#include "manp.h"
#include <time.h>
//#include <libs.h>
#include <string.h>
#include "Dib.h"
#include "colour.h"

// global variables
	int	StartColourCycling;	// we can start the colour cycling from any point..good for cycling animations

// routines in this module

static	void	set_palette1(BYTE *, BYTE *),set_palette2(BYTE *, BYTE *),
		set_palette3(BYTE *, BYTE *, BYTE *);

extern 	void	set_palette(void), bell(int);
extern	int	user_data(HWND);
//extern	void	init_windows_colours(HWND, char *);
extern	void	InitTrueColourPalette(BYTE);

static	int	paused;				// rotate-is-paused flag 

//extern	CTrueCol    TrueCol;			// palette info

static unsigned char Red[3]    = {255, 0, 0};	// for shifted-Fkeys
static unsigned char Green[3]  = { 0,255, 0};
static unsigned char Blue[3]   = { 0, 0,255};
static unsigned char Black[3]  = { 0, 0, 0};
static unsigned char White[3]  = {255,255,255};
static unsigned char Yellow[3] = {255,255, 0};
static unsigned char Brown[3]  = {127,127, 0};

char mapmask[13] = {"*.map"};

//extern	std::vector<float> wpixels;		// floating point iterations for each pixels
//extern	HWND	GlobalHwnd;			// to allow passing of hwnd 
//extern	CDib	Dib;				// Device Independent Bitmap

/***************************************************************************
	Rotate VGA palette
***************************************************************************/

void	rotate_vga_palette(int dir, int delay)
    {
    long	i, k;
    int		j, m;
    size_t 	local_width;
    DWORD	colour, n, pointer;

    if (dir == 1)
	{
	if (++StartColourCycling > gManp->threshold)
	    StartColourCycling -= gManp->threshold;
	}
    else
	{
	if (--StartColourCycling < 0)
	    StartColourCycling += gManp->threshold;
	}
//    InitTrueColourPalette(FALSE);

    local_width = ComputeWidthBytes((DWORD)gManp->Dib.DibWidth, (DWORD)gManp->Dib.BitsPerPixel);
    for (j = 0; j < gManp->Dib.DibHeight - 1; ++j)
	{
	i = ((long) (gManp->Dib.DibHeight - 1 - j) * (long) (local_width + 3 - ((local_width - 1) % 4)));
	k = ((long)j * (long)gManp->Dib.DibWidth);
	for (m = 0L; m < gManp->Dib.DibWidth; ++m)
	    {
	    if (gManp->time_to_reinit + gManp->time_to_restart + gManp->time_to_quit + gManp->time_to_zoom)
		{
		gManp->cycleflag = FALSE;
		return;
		}
	    colour = (DWORD) (gManp->wpixels[k + m]);
	    colour += StartColourCycling;
	    colour %= gManp->threshold;
	    n = colour + colour + colour;
	    pointer = m + m + m + i;
	    gManp->Dib.DibPixels[pointer + 0] = gManp->TrueCol.PalettePtr[n].rgbtRed;
	    gManp->Dib.DibPixels[pointer + 1] = gManp->TrueCol.PalettePtr[n].rgbtGreen;
	    gManp->Dib.DibPixels[pointer + 2] = gManp->TrueCol.PalettePtr[n].rgbtBlue;
	    }
//	for (n = 0; n < 3; ++n)
//	    *(pixels + i + m * 3 + n) = *(VGA_PALETTE + (((BYTE) (*(wpixels + k + m))) * 3L + 2 - n));
	}
    InvalidateRect(gManp->GlobalHwnd, &gManp->r, FALSE);
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
	if (user_data(gManp->GlobalHwnd) == -1)	// rotate until a key gets hit  
	    return -1;    
	if (!gManp->cycleflag)			// rotate until a cycling turned off  
	    break;    

	rotate_vga_palette(direction, vga_speed);  
	}
    return 0;
    }

