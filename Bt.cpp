/*                                                                
    BT.CPP a module for boundary tracing routines for MANPWIN.C.
    
    Written in MICROSOFT 'C' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Pixel.h"

/************************************************************************
	Return the color for a pixel
************************************************************************/

long	CPixel::calc_xy(HWND hwnd, int mx, int my, int user_data(HWND hwnd))

    {
    *color = (long)Plot.GetColour((WORD)mx, (WORD)my);
					// see if pixel is black
    if (*color != 0L)			// pixel is NOT black so we must have already 
	{				// calculated its color, so lets skip it 
	repeats++;			// count successive easy ones 
	return(*color);
	}
    repeats = 0;			// we'll have to work for this one to reset counter 

    col = mx;
    row = my;
    *color = calc_frac(hwnd, row, col, user_data);
    return(*color);
    }

/************************************************************************
	This is where it all happens
************************************************************************/

int	CPixel::boundary_trace(HWND hwnd, int C, int R, int user_data(HWND hwnd))

    {
    enum
	{
	North, East, South, West
	}
	Dir;
    int	C_first, low_row, iters;
    long	bcolor;

    low_row = R;
    Dir = East;
    bcolor = *color;
    C_first = C;
    iters = 0;
    repeats = 0;

   // main loop of BTM inside this loop the boundary is traced on screen! 
    do
	{
	if (user_data(hwnd) == -1)		// user pressed a key? 
	    return(-1);
	iters++;				// count times thru loop 
	if (C < LeftX[R])
	    LeftX[R] = C;			// to aid in filling polygon later 
	if (C > RightX[R])
	    RightX[R] = C;			// maintain left and right limits 
	else
	    if (R == low_row)
		if (C <= C_first)		// works 99.9% of time! 
		    break;
	switch (Dir)
	    {
	    case North :
		if (R > low_row)
		    if(calc_xy(hwnd, C,R - 1, user_data) == bcolor)
			{
			R--;
			if (C > ixstart)
			    if(calc_xy(hwnd, C - 1, R, user_data) == bcolor)
				{
				C--;
				Dir = West;
				}
			    break;
			}
		Dir = East;
		break;
	    case East :
		if (C < ixstop)
		    if(calc_xy(hwnd, C + 1, R, user_data) == bcolor)
			{
			C++;
			if (R > low_row)
			    if(calc_xy(hwnd, C, R - 1, user_data) == bcolor)
				{
				R--;
				Dir = North;
				}
			break;
			}
		Dir = South;
		break;
	    case South :
		if (R < iystop)
		    if(calc_xy(hwnd, C, R + 1, user_data) == bcolor)
			{
			R++;
			if (C < ixstop)
			    if(calc_xy(hwnd, C + 1, R, user_data) == bcolor)
				{
				C++;
				Dir = East;
				}
			break;
			}
		Dir = West;
		break;
	    case West:
		if (C > ixstart)
		    if(calc_xy(hwnd, C - 1, R, user_data) == bcolor)
			{
			C--;
			if (R < iystop)
			    if(calc_xy(hwnd, C, R + 1, user_data) == bcolor)
				{
				R++;
				Dir = South;
				}
			break;
			}
		    Dir = North;
		    break;
	    }			// case 
	}
    while (repeats < 30000);	// emergency backstop, should never be needed
				// PB, made above very high to allow for resumes;  did some checking of code first, and testing, to confirm that it seems unnecessary 
    if (iters < 4)
	{
	LeftX[low_row] = xdots;
	RightX[low_row] = -xdots;
	if (low_row+1 <= iystop)
	    {
	    LeftX[low_row + 1] = xdots;
	    RightX[low_row + 1] = -xdots;
	    }
	return(0);  // no need to fill a polygon of 3 points!
	}

				// Avoid tracing around whole fractal object
    if (iystop + 1 == (int)ydots)
	if (LeftX[0] == 0)
	    if (RightX[0] == ixstop)
		if (LeftX[iystop] == 0)
		    if (RightX[iystop] == ixstop)
			{
		    // clean up in this RARE case or next fills will fail! 
			for (low_row = 0; low_row <= (int)ydots - 1; low_row++)
			    {
			    LeftX[low_row] = xdots;
			    RightX[low_row] = -xdots;
			    }
			return(0);
			}
		// fill in the traced polygon, simple but it works darn well
    C = 0;
    for (R = low_row; R < iystop; R++)
	if (RightX[R] != -xdots)
	    {
	    if (user_data(hwnd) == -1)			// user pressed a key?
		return(-1);
	    if ((C = fillseg(LeftX[R], RightX[R], R, bcolor)) == -1)
		return(-1);
	    LeftX[R]  = xdots;
	    RightX[R] = -xdots;				// reset array element
	    }
	else if (C != 0)				// this is why C = 0 above! 
	    return(0);
    return(0);
    }							// BTM function

/************************************************************************
	Fill a segment within a marked boundary
************************************************************************/

int	CPixel::fillseg(int LeftX, int RightX, int R, long bcolor)

    {
    DWORD		*forwards = NULL;
    DWORD		*backwards = NULL;
    register int	modeON, C;
    int	i;
    long	gcolor;
    //BYTE	s[1200];

    if ((forwards = new DWORD[width + 200]) == NULL)
	return(-1);
    if ((backwards = new DWORD[width * 3]) == NULL)
	return(-1);

    modeON = 0;

    for(i = 0; i <= RightX - LeftX + 1; i++)
	forwards[i] = (DWORD)Plot.GetColour((WORD)(LeftX + i), (WORD)R);
    for (C = LeftX; C <= RightX; C++)
	{
	gcolor = forwards[C - LeftX];
	if (modeON != 0 && gcolor == 0)
	    forwards[C - LeftX] = bcolor;
	else
	    {
	    if (gcolor == bcolor)			// TW saved a GetColour here
		modeON = 1;
	    else
		modeON = 0;
	    }
	}

    if (*PlotType == NOSYM) 				// no symmetry! easy!
	Plot.OutputLine((WORD)LeftX, (WORD)R, (WORD)(RightX - LeftX + 1), forwards);
    else if (*PlotType == XAXIS) 			// X-axis symmetry
	{
	Plot.OutputLine((WORD)LeftX, (WORD)R, (WORD)(RightX - LeftX + 1), forwards);
	if ((i = yystop - (R - yystart)) > iystop)
	    Plot.OutputLine((WORD)LeftX, (WORD)i, (WORD)(RightX - LeftX + 1), forwards);
	}
    else if (*PlotType == ORIGIN) 			// Origin symmetry
	{
	reverse_string(backwards, forwards, RightX - LeftX + 1);
	Plot.OutputLine((WORD)LeftX, (WORD)R, (WORD)(RightX - LeftX + 1), forwards);
	if ((i = yystop - (R - yystart)) > iystop)
	    Plot.OutputLine((WORD)(xxstop - (RightX - ixstart)), (WORD)i, (WORD)(RightX - LeftX + 1), backwards);
	}
    else if (*PlotType == YAXIS) 			// Y-axis symmetry
	{
	reverse_string(backwards, forwards, RightX - LeftX + 1);
	Plot.OutputLine((WORD)LeftX, (WORD)R, (WORD)(RightX - LeftX + 1), forwards);
	Plot.OutputLine((WORD)(xxstop - (RightX - ixstart)), (WORD)R, (WORD)(RightX - LeftX + 1), backwards);
	}
    else if (*PlotType == XYAXIS) 			// X-axis and Y-axis symmetry
	{
	reverse_string(backwards, forwards, RightX - LeftX + 1);
	Plot.OutputLine((WORD)LeftX, (WORD)R, (WORD)(RightX - LeftX + 1), forwards);
	Plot.OutputLine((WORD)(xxstop - (RightX - ixstart)), (WORD)R, (WORD)(RightX - LeftX + 1), backwards);
	if ((i = yystop - (R - yystart)) > iystop)
	    {
	    Plot.OutputLine((WORD)LeftX, (WORD)i, (WORD)(RightX - LeftX + 1), forwards);
	    Plot.OutputLine((WORD)(xxstop - (RightX - ixstart)), (WORD)i, (WORD)(RightX - LeftX + 1), backwards);
	    }
	}
    else  						// the other symmetry types are on their own!
	{
	for (i = LeftX; i <= RightX; i++)
	    plot((WORD)i, (WORD)R, forwards[i - LeftX]);
	}

    if (forwards)
	{
	delete forwards;
	forwards = NULL;
	}
    if (backwards)
	{
	delete backwards;
	backwards = NULL;
	}

    return(C);
    }

/************************************************************************
	Copy a string backwards for symmetry functions
************************************************************************/

void	CPixel::reverse_string(DWORD *t, DWORD *s, int len)

    {
    register int	i;

    len--;
    for(i = 0; i <= len; i++)
	t[i] = s[len - i];
    }

/************************************************************************
	Boundary trace begins here
************************************************************************/

int	CPixel::bound_trace_main(HWND hwnd, int user_data(HWND hwnd))

    {
    int	    i;

    for (i = 0; i < (int)ydots; i++)
	{
	LeftX[i] = 3000;
	RightX[i] = -3000;
	}

    *oldrow = *oldcol = 3000;
    *andcolor = threshold;						// used if color = 0
    for (currow = iystart; currow <= iystop; currow++)
	{
	for (curcol = ixstart; curcol <= ixstop; curcol++)
	    {
	    if (save_flag)
		{
		if (iystop != yystop)
		    iystop = yystop - (currow - yystart); // allow for sym
		add_worklist(xxstart,xxstop,currow,iystop,currow,0,worksym);
		return -1;
		}
	    *color = (long)Plot.GetColour((WORD)curcol, (WORD)currow);

	/* if pixel is BLACK (0) then we haven't done it yet!
	so first calculate its color and call the routine
	that will try and trace a polygon if one exists */

	    if (*color == 0L)
		{
		reset_period = 1;
		row = currow;
		col = curcol;
		if ((*color = calc_frac(hwnd, row, col, user_data)) == -1)
		    return -1;
		if (boundary_trace(hwnd, curcol, currow, user_data) == -1)
		    return -1;
		}
	    }
	}
    return 0;
    }

