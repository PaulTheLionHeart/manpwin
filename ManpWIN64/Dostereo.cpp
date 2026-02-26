/*
    DOSTEREO.CPP a module to view 3D images.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <conio.h>
#include "manp.h"
#include "pixel.h"

/*
static	float	xshift;
static	float	EyeSeparation, ViewingDist;
static	short	Left_ofLeftPic, Left_ofRightPic;
static	short	PicWidth, PicHeight, sep;
static	long	depth, maxc, ystart;
static	long	colour_array[MAXHORIZONTAL / 2];	// for right hand image
static	long	colour_position[MAXHORIZONTAL / 2];	// for right hand image

extern	int	stereo_sign;
extern	BYTE	pairflag;		// stereo pair
extern	int	ground;			// set limits on separation
extern	int	AutoStereo_value;	// AutoStereo depth value

extern	long	threshold;		// maximum iterations
//extern	int	user_data(HWND);
*/

/**************************************************************************
	Do stereo pairs
	
	Pairflag is used as a global variable in this module and BT.C to 
	inhibit unwanted rows and columns (row, col) to remove excess 
	processing. Pairglag is set up in USER.C and initialised to 2 which 
	means half screen width pictures.
	-On is used to indicate stereo pairs are required and n is the 
	inverse size of the window.
	Autostereo depth parameters are entered using -Rn flag.
	
	Variable stereo_sign is used to convert valleys into hills. (Negative
	value for -R flag).
	
	Arrays colour_array[] and colour_position[] are used to reverse the
	direction of plotting to prevent low values overwriting on one side 
	and high values on the other.
	
	xdots and ydots are screen size for particular video mode.
**************************************************************************/

void	CPixel::init_stereo_pairs(int pairflag, int *AutoStereo_value)

    {
    PicWidth = xdots / pairflag;			// WIDTH  OF EACH PICTURE
    PicHeight = ydots / pairflag;			// HEIGHT OF EACH PICTURE
    ystart = (ydots - PicHeight) / 2;			// starting pos for windows

    ground = PicWidth / 2;				// a dot separation for distant objects 
    if (*AutoStereo_value < 0)
	{
	*AutoStereo_value = -*AutoStereo_value;
	stereo_sign = -1;
	}
    else
	stereo_sign = 1;
    if (*AutoStereo_value < 5)
	*AutoStereo_value = 5;				// low values blow up calcs
        
    depth = xdots / *AutoStereo_value;
    maxc = (threshold > colours) ? colours : threshold;

    EyeSeparation = (float)(xdots / 3);			// 180 PIXELS = 6.5 CMS ON 640X480 SCREEN

							// Ideal viewing distance 10 * eye separation
    ViewingDist = EyeSeparation * 10;
							// left hand rectangle
    Left_ofLeftPic = xdots / 2 - PicWidth;
							// right hand rectangle
    Left_ofRightPic = xdots / 2;
    }

/**************************************************************************
	Do stereo pairs
	In BT.C every second row and column are inhibited to remove excess
	processing.
**************************************************************************/

void	CPixel::do_stereo_pairs(int col, int row, long color)

    {
    int	x1, x2, y;

    if (stereo_sign == 1)
	sep = ground - (short) (depth * (maxc - color) / maxc);
    else
	sep = ground - (short) (depth * color / maxc);
    xshift = EyeSeparation * sep / ViewingDist;

/* 
left hand picture: xshift +ve for receding depth with eyes crossed; 
if xshift -ve valleys become hills.
*/

    y = (short)((long)(row / (short) pairflag) + ystart);

    x1 = col / pairflag + (short) xshift + Left_ofLeftPic;
    if (x1 >= 0 && x1 < ((short)xdots >> 1) - 2)
	PlotPixel((WORD)x1, (WORD)y, color);

// right hand picture: 

//	Use this if direct plotting of right picture
    x2 = col / pairflag - (short) xshift + Left_ofRightPic;
    if (x2 > (xdots >> 1) + 2 && x2 < xdots)
	PlotPixel((WORD)x2, (WORD)y, color);

// load up array for image
    colour_array[col / pairflag] = color;
    colour_position[col / pairflag] = col / pairflag - (short) xshift + Left_ofRightPic;
    }

/**************************************************************************
	Draw right hand image after end of final pixel for active line
**************************************************************************/

void	CPixel::draw_right_image(short y)

    {
    short	i, row;

    y = (short)((long)(y / (short) pairflag) + ystart);
    row = (short)((long)y / (long)(pairflag + ystart));
    for (i = (xdots / pairflag) - 1; i; --i)
	if (colour_position[i] > ((short)xdots >> 1) + 2 && colour_position[i] < (short)xdots)
	    PlotPixel((WORD)(colour_position[i]), (WORD)row, colour_array[i]);
    }
