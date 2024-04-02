/* The Ant Automaton is based on an article in Scientific American, July 1994.
 * The original Fractint implementation was by Tim Wegner in Fractint 19.0.
 * This routine is a major rewrite by Luciano Genero & Fulvio Cappelli using
 * tables for speed, and adds a second ant type, multiple ants, and random
 * rules.
 *
 * Revision history:
 * 20 Mar 95 LG/FC First release of table driven version
 * 31 Mar 95 LG/FC Fixed a bug that writes one pixel off the screen
 * 31 Mar 95 LG/FC Changed ant type 1 to produce the same pattern as the
 *                   original implementation (they were mirrored on the
 *                   x axis)
 * 04 Apr 95 TW    Added wrap option and further modified the code to match
 *                   the original algorithm. It now matches exactly.
 * 10 Apr 95 TW    Suffix array does not contain enough memory. Crashes at
 *                   over 1024x768. Changed to extraseg.
 * 12 Apr 95 TW    Added maxants range check.
 * June 2023 PHD, upgrade to C++ for ManpWIN
 */

#include "OtherFunctions.h"
#include "plot.h"
#include "resource.h"

// possible value of idir e relative movement in the 4 directions
// for x 0, 1, 0, -1
// for y 1, 0, -1, 0

extern	int	user_data(HWND);
extern	void	DisplayStatusBarInfo(int, char *);
extern	char	*ShowTime(double time);

/**************************************************************************
	turkmite from scientific american july 1994 pag 91 Tweaked by Luciano Genero & Fulvio Cappelli
**************************************************************************/

void	COtherFunctions::TurkMite1(int maxtur, size_t rule_len, char *ru, long maxpts, long wait)
    {
    int	    color, ix, iy, idir, pixel, i;
    int	    /*kbdchar, */step, antwrap;
    int	    x[MAX_ANTS + 1], y[MAX_ANTS + 1];
    int	    next_col[MAX_ANTS + 1], rule[MAX_ANTS + 1], dir[MAX_ANTS + 1];
    long    count;
    int	    lastChecked = -1;
    double  ElapsedTime;
    __time64_t     AntTime = FrameStart.time;

    antwrap = ((param[3] == 0) ? 0 : 1);
    step = (int) wait;
    if (step == 1)
	wait = 0;
    else
	step = 0;
    if (rule_len == 0)
	{						// random rule 
	for (color = 0; color < MAX_ANTS; color++)
	    {						// init the rules and colors for the turkmites: 1 turn left, -1 turn right 
	    rule[color] = 1 - (RANDOMANT(2) * 2);
	    next_col[color] = color + 1;
	    }
							// close the cycle 
	next_col[color] = 0;
	}
    else
	{						// user defined rule
	for (color = 0; color < rule_len; color++)
	    {						// init the rules and colors for the turkmites: 1 turn left, -1 turn right
	    rule[color] = (ru[color] * 2) - 1;
	    next_col[color] = color + 1;
	    }
							// repeats to last color 
	for (color = (int)rule_len; color < MAX_ANTS; color++)
	    {						// init the rules and colors for the turkmites: 1 turn left, -1 turn right
	    rule[color] = rule[color % rule_len];
	    next_col[color] = color + 1;
	    }
							// close the cycle
	next_col[color] = 0;
	}
    for (color = maxtur; color; color--)
	{						// init the various turmites N.B. non use x[0], y[0], dir[0]
	if (rule_len)
	    {
	    dir[color] = 1;
	    x[color] = XO;
	    y[color] = YO;
	    }
	else
	    {
	    dir[color] = RANDOMANT(DIRS);
	    x[color] = RANDOMANT(xdots);
	    y[color] = RANDOMANT(ydots);
	    }
	}
    maxpts = maxpts / (long) INNER_LOOP;
    for (count = 0; count < maxpts; count++)
	{
	double progress = (double)count / (double)maxpts;
	if (int(progress * 100) != lastChecked)
	    {
	    lastChecked = int(progress * 100);
	    _ftime64(&FrameEnd);					// initialise time counter
	    ElapsedTime = (double)(FrameEnd.time) + (double)(FrameEnd.millitm) / 1000.0 - (double)(FrameStart.time) - (double)(FrameStart.millitm) / 1000.0;
	    sprintf(AntStatus, "Progress = (%d%%), Time %s", int(progress * 100), ShowTime(ElapsedTime));
	    if (AntTime < FrameEnd.time)
		DisplayStatusBarInfo(INCOMPLETE, "");
	    }

	InvalidateRect(hwnd, NULL, FALSE);
	if (user_data(hwnd) == -1)		// user pressed a key? 
	    return;

	for (i = INNER_LOOP; i; i--)
	    {
	    if (wait > 0 && step == 0)
		{
		for (color = maxtur; color; color--)
		    {						// move the various turmites
		    ix = x[color];					// temp vars 
		    iy = y[color];
		    idir = dir[color];

		    pixel = Plot.GetColour(ix, iy);
		    Plot.PlotPoint(ix, iy, 15);
		    Sleep(wait);
		    Plot.PlotPoint(ix, iy, next_col[pixel]);
//		    Plot.PlotPoint(ix, iy, 25);
		    idir += rule[pixel];
		    idir &= 3;
		    if (antwrap == 0)
			if ((idir == 0 && iy == ydots - 1) ||
			    (idir == 1 && ix == xdots - 1) ||
			    (idir == 2 && iy == 0) ||
			    (idir == 3 && ix == 0))
				return;
		    x[color] = incx[idir][ix];
		    y[color] = incy[idir][iy];
		    dir[color] = idir;
		    }
		}
	    else
		{
		for (color = maxtur; color; color--)
		    {						// move the various turmites without delay 
		    ix = x[color];					// temp vars 
		    iy = y[color];
		    idir = dir[color];
		    pixel = Plot.GetColour(ix, iy);
		    Plot.PlotPoint(ix, iy, next_col[pixel]);
//		    Plot.PlotPoint(ix, iy, 39);
		    idir += rule[pixel];
		    idir &= 3;
		    if (antwrap == 0)
			if ((idir == 0 && iy == ydots - 1) ||
			    (idir == 1 && ix == xdots - 1) ||
			    (idir == 2 && iy == 0) ||
			    (idir == 3 && ix == 0))
				return;
		    x[color] = incx[idir][ix];
		    y[color] = incy[idir][iy];
		    dir[color] = idir;
		    }
		}
	    }
	}
    return;
    }

/**************************************************************************
	Main ant routine
**************************************************************************/

int	COtherFunctions::ant(void)
    {
    int	    maxants, /*AntType, */i;
    size_t  rule_len;
//   int oldhelpmode ;
    long maxpts, wait;
    char rule[MAX_ANTS];
    char *extra;

    extra = new char [(xdots + 2) * sizeof(int) * DIRS + (ydots + 2) * sizeof(int) * MAX_ANTS];

    for (i = 0; i < DIRS; i++)
	{
	incx[i] = (int *) (extra + (xdots + 2) * sizeof(int) * i);
	incy[i] = (int *) (extra + (xdots + 2) * sizeof(int) * DIRS + (ydots + 2) *sizeof(int) * i);
	}

// In this vectors put all the possible point that the ants can visit. Wrap them from a side to the other instead of simply end calculation
    for (i = 0; i < xdots; i++)
	{
	incx[0][i] = i;
	incx[2][i] = i;
	}

    for(i = 0; i < xdots; i++)
	incx[3][i] = i + 1;
    incx[3][xdots-1] = 0; // wrap from right of the screen to left 

    for(i = 1; i < xdots; i++)
	incx[1][i] = i - 1;
    incx[1][0] = xdots-1; // wrap from left of the screen to right 

    for (i = 0; i < ydots; i++)
	{
	incy[1][i] = i;
	incy[3][i] = i;
	}
    for (i = 0; i < ydots; i++)
	incy[0][i] = i + 1;
    incy[0][ydots - 1] = 0;      // wrap from the top of the screen to the bottom 
    for (i = 1; i < ydots; i++)
	incy[2][i] = i - 1;
    incy[2][0] = ydots - 1;      // wrap from the bottom of the screen to the top 
   //oldhelpmode = helpmode;
   //helpmode = ANTCOMMANDS;
    maxpts = (long) param[1];
    maxpts = labs(maxpts);
   //wait = abs(orbit_delay);
    sprintf(rule, "%.17g", param[0]);
    rule_len = strlen(rule);
    if (rule_len > 1)
	{                            // if rule_len == 0 random rule
	for (i = 0; i < rule_len; i++)
	    {
	    if (rule[i] != '1')
		rule[i] = (char) 0;
	    else
		rule[i] = (char) 1;
	    }
	}
    else
	rule_len = 0;

   // set random seed for reproducibility
    if ((!rflag) && param[4] == 1)
	--AntRseed;
    if (param[5] != 0 && param[4] != 1)
	AntRseed = (int)param[4];

    srand(AntRseed);
    if (!rflag) ++AntRseed;

    maxants = (int) param[2];
    if (maxants < 1)             // if maxants == 0 maxants random
	maxants = 2 + RANDOMANT(MAX_ANTS - 2);
    else if (maxants > MAX_ANTS)
	param[2] = maxants = MAX_ANTS;

    wait = 0;
    TurkMite1(maxants, rule_len, rule, maxpts, wait);
    DisplayStatusBarInfo(COMPLETE, "");
    if (extra) { delete[] extra; extra = NULL; }
    return 0;
    }

/**************************************************************************
	Main Tower routine
**************************************************************************/

int	COtherFunctions::tower(void)
    {
    double  *z;		// z is a 2D matrix that is the number of hits in the coordinates 'c' and 'd' that I use.
    double  *m;		// m is the height of the number of hits. 'c' and 'd' are respectively the abscissae and co-ordinates.
    double  *l;		// co-ordinates
    double  *p;		// co-ordinates
    int	    a, b, c, d;	// initial points (a,b) and (c,d) in this sequence. they were chosen avoiding an early touching in the limits of the screen
    int	    e, f;
    DWORD   cor, colour;
    double  mmax = 0.0;
    int	    lastChecked = -1;
    double  ElapsedTime;
    __time64_t     AntTime = FrameStart.time;
    int	    AntSize = (int)param[3];
    if (AntSize < 1) AntSize = 1;
    double  HeightScale = fabs(param[4]);
    a = xdots / 2; b = ydots / 2;
//    srand((unsigned)time(NULL));
//    int RandomDigit = rand() % 10 - 5;
    c = a + 1;
    d = b - 2;
    z = new double[MAXIMUM_TOWER * MAXIMUM_TOWER];
    m = new double[MAXIMUM_TOWER];
    l = new double[MAXIMUM_TOWER];
    p = new double[MAXIMUM_TOWER];

    DWORD	BackGround;
    int		BGColPtr = (int)param[2];
    if (BGColPtr < 0) BGColPtr = 0; if (BGColPtr > 255) BGColPtr = 255;
    BackGround = *(TrueCol->DefaultPalettePtr + 3 * BGColPtr + 2) + (*(TrueCol->DefaultPalettePtr + 3 * BGColPtr + 1) << 8) + (*(TrueCol->DefaultPalettePtr + 3 * BGColPtr + 0) << 16);
    Dib->ClearDib(BackGround);

    for (int j = 2; j < MAXIMUM_POINTS; j++)	// number of loops for the evolution of the ant till it reaches the limit of 'my' screen that is, pixel 640.
	{
	if (user_data(hwnd) == -1)	// user pressed a key?
	    return -1;
	double progress = (double)j / (double)MAXIMUM_POINTS;
	if (int(progress * 100) != lastChecked)
	    {
	    lastChecked = int(progress * 100);
	    _ftime64(&FrameEnd);					// initialise time counter
	    ElapsedTime = (double)(FrameEnd.time) + (double)(FrameEnd.millitm) / 1000.0 - (double)(FrameStart.time) - (double)(FrameStart.millitm) / 1000.0;
	    sprintf(AntStatus, "Progress = (%d%%), Time %s", int(progress * 100), ShowTime(ElapsedTime));
	    if (AntTime < FrameEnd.time)
		DisplayStatusBarInfo(INCOMPLETE, "");
	    }
	if (a != c)			// option: if segment (a,b)-(c,d) is horizontal, goto 1 else if segment (a,b)-(c,d) is vertical, goto 5
	    {
	    if (a < c)			// if segment (a,b)-(c,d) is E -> W goto 2 else if segment (a,b)-(c,d) is W -> E goto 3, , using geographical names
		{
		if ((colour = Plot.GetColour(c, d)) == 15)	//segment (a,b)-(c,d) is E -> W:
		    {
		    f = d + AntSize * 2 + 1;
		    e = c;
		    }
		else
		    {
		    f = d - (AntSize * 2 + 1);
		    e = c;
		    }
		}
	    else
		{
		if ((colour = Plot.GetColour(c, d)) == 15)	//segment (a,b)-(c,d) is W -> E:
		    {
		    f = d - (AntSize * 2 + 1);
		    e = c;
		    }
		else
		    {
		    f = d + AntSize * 2 + 1;
		    e = c;
		    }
		}
	    }
	else if (b != d)
	    {
	    if (d > b)
		{
		if ((colour = Plot.GetColour(c, d)) == 15)	// segment (a,b)-(c,d) has the sense N -> S
		    {
		    e = c - (AntSize * 2 + 1);
		    f = d;
		    }
		else
		    {
		    e = c + AntSize * 2 + 1;
		    f = d;
		    }
		}
	    else
		{
		if ((colour = Plot.GetColour(c, d)) == 15)	// segment (a,b)-(c,d) has the sense S -> N
		    {
		    e = c + AntSize * 2 + 1;
		    f = d;
		    }
		else
		    {
		    e = c - (AntSize * 2 + 1);
		    f = d;
		    }
		}
	    }
	a = c; b = d; c = e; d = f;			// points move forwards, (a,b) are now the previous (c,d) and new (c,d) has the coordinates (e,f)
	if ((colour = Plot.GetColour(c, d)) == 15)	// here we change the color for the construction of the new (c,d) 
	    cor = 0L;
	else
	    cor = 15;
	if (d < 0)
	    d = 50;
	else if (d > ydots)
	    d = ydots - 50;		// making the ant rebound at the limit of the wall in the horizontal direction
	if (c < 0)
	    c = 50;
	else if (c > xdots)
	    c = xdots - 50;		// making the ant rebound at the limit of the wall in the vertical direction

	Plot.DisplayFilledCircle(c, d, AntSize, cor);	// here we materialize the drawing of the new (c,d) with its inverse color
	// z(c, d) = z(c, d) + 1: here we count, cell by cell, the number of the visits of the single 'ant' - this idea was Johnathan's
	DWORD	index = d * MAXIMUM_TOWER + c;
	*(z + index) = *(z + index) + 1;
	*(l + j) = c;
	*(p + j) = d;
	*(m + j) = *(z + index);			// here we mount the 3 linear matrix, the co-ordinate, the abscissa and the vertical variables.
	if (*(m + j) > mmax)				// this will divide the colors of the mountain : till 1.5 / 7, green; from 1.5 / 7 to 4.5 / 7 gray and from 5 / 7 to 7 / 7 white
	    mmax = *(m + j);				// that proportion I saw in a voyage to the Andes mountain in Chile and Argentina - the trees did not climbe above the 600 meters altitude.
	if (j % (int)param[0] == 0)
	    Sleep(1);					// slowing the movement of the ant. In C++ there is a command to slow the processing here
	}

    if (MessageBox(hwnd, "Save current image to Clipboard?", "Continue", MB_ICONEXCLAMATION | MB_DEFBUTTON2 | MB_YESNO) == IDYES)   // move to next part of the program
	SendMessage(hwnd, WM_COMMAND, IDM_COPY, 0L);			// copy current image to clipboard  
	// here is the end of the first draw of the ants in the plane.
    Dib->ClearDib(0L);
    int x, y;						// store starinbg point for each line
    x = xdots / 2;
    y = ydots - 50;
    for (int i = 2; i < MAXIMUM_TOWER - 5; i++)
	{
	if (user_data(hwnd) == -1)			// user pressed a key?
	    return -1;
	double progress = (double)i / (double)(MAXIMUM_TOWER - 1);
	if (int(progress * 100) != lastChecked)
	    {
	    lastChecked = int(progress * 100);
	    _ftime64(&FrameEnd);					// initialise time counter
	    ElapsedTime = (double)(FrameEnd.time) + (double)(FrameEnd.millitm) / 1000.0 - (double)(FrameStart.time) - (double)(FrameStart.millitm) / 1000.0;
	    sprintf(AntStatus, "Progress = (%d%%), Time %s", int(progress * 100), ShowTime(ElapsedTime));
	    if (AntTime < FrameEnd.time)
		DisplayStatusBarInfo(INCOMPLETE, "");
	    }
	if (*(m + i) < 1.5 / 7 * mmax)
	    cor = 2;					// here I define the colors : the forest(DOS color 3), the nude rock, now gray(DOS color 8) and the snow color = 15
	else if ((1.5 / 7 * mmax) < *(m + i) && *(m + i) < (4.5 / 7 * mmax))
	    cor = 8;
	else
	    cor = 15;
	if (i % (int)param[1] == 0)
	    Sleep(1);					// Here we slow the construction of the tower otherwise it would be instaneous. ANT do that construction by vagarose steps !!!!
	
	int xIn, xOut, yIn, yOut;			// use these to check boundary conditions of the screen
	xIn = x; yIn = y;
	xOut = (int)(*(l + i) + 10); yOut = (int)(ydots - 50 - *(m + i) * 10 * HeightScale);
	if (xIn < 0) xIn = 0;  if (xIn >= xdots) xIn = xdots - 1;
	if (yIn < 0) yIn = 0;  if (yIn >= xdots) yIn = ydots - 1;
	if (xOut < 0) xOut = 0;  if (xOut >= xdots) xOut = xdots - 1;
	if (yOut < 0) yOut = 0;  if (yOut >= xdots) yOut = ydots - 1;

	if (xIn > 10 && xOut > 10 && yIn > 10 && yOut > 10)	// no silly lines at startup
	    Plot.genline(xIn, yIn, xOut, yOut, cor);
	x = (int)(*(l + i) + 10);
	y = (int)(ydots - 50 - *(m + i) * 10 * HeightScale);
	}

    if (z) { delete[] z; z = NULL; }
    if (m) { delete[] m; m = NULL; }
    if (l) { delete[] l; l = NULL; }
    if (p) { delete[] p; p = NULL; }
    return 0;
    }

