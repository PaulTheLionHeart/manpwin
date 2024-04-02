/*
    STEREO.CPP a module to view 3D images.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <conio.h>
#include "manp.h"
#include "Dib.h"
#include "colour.h"
#include "plot.h"

//#define MAXLINE		150		/* length of line */
#define TRUE		1
#define FALSE		0
#define EVER		(;;)
#define	SPOT_SIZE	8
#define	SPOT_COL	0x0f
#define	ESC_CHAR	0x1b

#define	REPEATS		4

#define RANDOM(x)	(rand()%(x))

int	orig_mode, eyes, ground;
int	eye_dots = FALSE;		/* eye dots for AutoStereo */
BOOL	AutoStereoActive = FALSE;	// is autostereo currently running?

BYTE	grayflag = 0;			/* flag to use grey value rather than color number */

static	long	depth, maxc;
	int	stereo_sign;
static	BOOL	ShowSpots = FALSE;
static	RGBTRIPLE   SpotBackground[SPOT_SIZE+2][SPOT_SIZE+2];		// hold background pixels for restoration

void	DrawSpots(void);

extern	int	logval;			/* log colour map */
extern	long	threshold;		/* maximum iterations */
extern	int	AutoStereo_value;	/* AutoStereo depth value */
extern	CTrueCol    TrueCol;		// palette info
extern	int	height, xdots, ydots, width, bits_per_pixel;
extern	WORD	colours;
extern	CPlot	Plot;		// image plotting routines 
extern	CDib	Dib;				// Device Independent Bitmap

extern	int	user_data(HWND);


/**************************************************************************
   The getdepth() function allows using the grayscale value of the color 
   as depth, rather than the color number. Maybe I got a little too 
   sophisticated trying to avoid a divide, so the comment tells what all
   the multiplies and shifts are trying to do. The result should be from
   0 to 255. 
**************************************************************************/

static int getdepth(short x, short y)

    {
    short	pal, temp;

    pal = (BYTE)(Plot.GetColour(x, y));

    if (grayflag)  
	{		// effectively (30*R + 59*G + 11*B)/100 scaled 0 to 255
	pal *= 3;
	temp = ((short) (*(TrueCol.PalettePtr + pal + 0) >> 2) * 77 
	    +  (short)  (*(TrueCol.PalettePtr + pal + 1) >> 2) * 151 
	    +  (short)  (*(TrueCol.PalettePtr + pal + 2) >> 2) * 28);
	pal = temp >> 6;
	}
    return(pal);
    }

/**************************************************************************
	Draw 3D function
**************************************************************************/

void	draw3D(HWND hwnd)

    {
    short	x, y, same[MAXHORIZONTAL], colour[MAXHORIZONTAL], sep, i, j, s;

    if (AutoStereoActive)
	if (MessageBox (hwnd, "Autostero already running. Continue? Y/N", "Autostereo", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDNO)
	    return;

    ShowSpots = FALSE;
    eyes = ydots / REPEATS;				// eye separation
    ground = eyes / 2;					// a dot separation for distant objects
    if (AutoStereo_value < 0)
	{
	AutoStereo_value = -AutoStereo_value;
	stereo_sign = -1;
	}
    else
	stereo_sign = 1;
    if (AutoStereo_value < 10)
	AutoStereo_value = 10;				// low values blow up calcs
        
    depth = xdots / AutoStereo_value;
    maxc = (threshold > colours) ? colours : threshold;
    for (y = 0; y < (short)ydots; ++y)
	{
	user_data(hwnd);				// user pressed a key?
	for (x = 0; x < (short)xdots; ++x)
	    same[x] = x;
	for (x = 0; x < (short)xdots; ++x)
	    {
	    if (stereo_sign == 1)
    		sep = ground - (short) (depth * (maxc - getdepth(x, y)) / maxc);
	    else
		sep = ground - (short) (depth * (getdepth(x, y)) / maxc);
	    i = x - (sep + (sep & y & 1)) / 2;
	    j = i + sep;
	    if (0 <= i && j < (short)xdots)
		{
		for (s = same[i]; s != i && s != j; s = same[i])
		    {
		    if (s > j)
			{
			same[i] = j;
			i = j;
			j = s;
			}
		    else
			i = s;
		    }
		same[i] = j;
		}
	    }
	for (x = xdots - 1; x >= 0; x--)
	    {
	    if (same[x] == x)
		colour[x] = RANDOM(255);
	    else
		colour[x] = colour[same[x]];
	    Plot.PlotPoint(x, y, colour[x]);
	    }
	}
//    if (ShowSpots)
//	DrawSpots();
    AutoStereoActive = TRUE;
    }

/**************************************************************************
	Draw 3D function
**************************************************************************/

void	GuideSpot(short x, short y)

    {
    int	    i, j, k;
    long    t;

    for (i = x - SPOT_SIZE / 2; i <= x + SPOT_SIZE / 2; ++i)
	for (j = y - SPOT_SIZE / 2; j <= y + SPOT_SIZE / 2; ++j)
	    {
	    t = ((long) (ydots - 1 - j) * Dib.WidthBytes + (long)(i * 3));
	    if (!ShowSpots)
		{
		SpotBackground[i][j].rgbtRed	= *(Dib.DibPixels + t + 0);	// backup colours for restoration if removing spots
		SpotBackground[i][j].rgbtGreen	= *(Dib.DibPixels + t + 1);
		SpotBackground[i][j].rgbtBlue	= *(Dib.DibPixels + t + 2);
		for (k = 0; k < 3; ++k)
		    *(Dib.DibPixels + t + k) = 0xff;				// set to white
		}
	    else
		{
		*(Dib.DibPixels + t + 0) = SpotBackground[i][j].rgbtRed;	// restore colours from buffer
		*(Dib.DibPixels + t + 1) = SpotBackground[i][j].rgbtGreen;
		*(Dib.DibPixels + t + 2) = SpotBackground[i][j].rgbtBlue;
		}
	    }
//	    point((WORD)i, (WORD)j, SPOT_COL);
    }

/**************************************************************************
	Draw dots and program exit
**************************************************************************/

void	DrawSpots(void)

    {
    GuideSpot((short)(xdots / 2 - eyes / 4), ydots / 2);
    GuideSpot((short)(xdots / 2 + eyes / 4), ydots / 2);
    ShowSpots = !ShowSpots;
    }
