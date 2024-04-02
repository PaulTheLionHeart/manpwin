/*
    CALCMODE.CPP a module for plotting modes.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Pixel.h"

/**************************************************************************
	Routine for 1 or 2 passes
**************************************************************************/

int	CPixel::OneOrTwoPass(HWND hwnd, int user_data(HWND hwnd))

    {
    int	i;

    *totpasses = 1;
    if (calcmode == '2')
	*totpasses = 2;
    if (calcmode == '2' && workpass == 0)		/* do 1st pass of two */
	{
	if (StandardCalc(hwnd, 1, user_data) == -1)
	    {
	    add_worklist(xxstart,xxstop,yystart,yystop,row,0,worksym);
	    return(-1);
	    }
	if (num_worklist > 0) /* worklist not empty, defer 2nd pass */
	    {
	    add_worklist(xxstart,xxstop,yystart,yystop,yystart,1,worksym);
	    return(0);
	    }
	workpass = 1;
	yybegin = yystart;
	}

    if (StandardCalc(hwnd, 2, user_data) == -1)			/* second or only pass */
	{
	i = yystop;
	if (iystop != yystop)			/* must be due to symmetry */
	    i -= row - iystart;
	add_worklist(xxstart,xxstop,row,i,row,workpass,worksym);
	return(-1);
	}
    return(0);
    }

/**************************************************************************
	Standard calculation
**************************************************************************/

int	CPixel::StandardCalc(HWND hwnd, int passnum, int user_data(HWND hwnd))

    {
    *curpass = passnum;
    row = yybegin;
    while (row <= iystop)
	{
	reset_period = 1;
	col = ixstart;
	while (col <= ixstop)
	    {				// on 2nd pass of two, skip even pts
	    if (passnum == 1 || calcmode == '1' || calcmode == 'F' || (row & 1) != 0 || (col & 1) != 0)
		{
		if (calc_frac(hwnd, row, col, user_data) == -1)
		    return(-1);		// interrupted
		if (passnum == 1)		// first pass, copy pixel and col++
		    {
		    if ((row & 1) == 0 && row < iystop)
			{
			plot((WORD)col, (WORD)(row + 1), *color);
			if ((col & 1) == 0 && col < ixstop)
			    plot((WORD)(col + 1), (WORD)(row + 1), *color);
			}
		    if ((col & 1) == 0 && col < ixstop)
			plot((WORD)(++col), (WORD)row, *color);
		    }
		}
	    ++col;
	    }
	if (passnum == 1 && (row & 1) == 0)
	    ++row;
	++row;
	}
    return(0);
    }

/**************************************************************************
	Routine for Spiralling In
**************************************************************************/

int	CPixel::Spiral(HWND hwnd, int user_data(HWND hwnd))

    {
    int	xmin, xmax, ymin, ymax, row, col, i;

    xmin = ixstart;
    ymin = yybegin;
    xmax = ixstop;
    ymax = yystop;
    *totpasses = PASSWIDTH;

//    num_worklist = 0;								// only do as one segment

    for (i = 0; i < PASSWIDTH; i++)
	{
	*curpass = i;
	xmin = ixstart + i;
	ymin = yybegin + i;
	xmax = ixstop - i;
	ymax = yystop - i;
	while (ymin <= ymax && xmin <= xmax)
	    {
	    row = ymin;
	    for (col = xmin; col < xmax; col++)
		{
		if (calc_frac(hwnd, row, col, user_data) == -1)
		    return(-1);		// interrupted
		}
	    col = xmax;
	    for (row = ymin; row < ymax; row++)
		{
		if (calc_frac(hwnd, row, col, user_data) == -1)
		    return(-1);		// interrupted
		}
	    row = ymax;
	    for (col = xmax; col >= xmin; col--)
		{
		if (calc_frac(hwnd, row, col, user_data) == -1)
		    return(-1);		// interrupted
		}
	    col = xmin;
	    for (row = ymax; row >= ymin; row--)
		{
		if (calc_frac(hwnd, row, col, user_data) == -1)
		    return(-1);		// interrupted
		}
        										
	    xmin += PASSWIDTH;
	    ymin += PASSWIDTH;
	    xmax -= PASSWIDTH;
	    ymax -= PASSWIDTH;
	    }
	}
    return(0);
    }

/**************************************************************************
	Routine for Vertical or Horizontal Blinds
**************************************************************************/

int	CPixel::Blinds(HWND hwnd, int user_data(HWND hwnd))

    {
    int	i;
    *totpasses = PASSWIDTH;
    if (calcmode == 'H')			// Horizontal Blinds
	{
	for (i = 0; i < PASSWIDTH; i++)
	    {
	    *curpass = i;
	    row = yybegin + i;
	    while (row <= yystop)
		{
		reset_period = 1;
		col = ixstart;
		while (col <= ixstop)
		    {
		    if (calc_frac(hwnd, row, col, user_data) == -1)
			return(-1);			// interrupted
		    ++col;
		    }
		row += PASSWIDTH;
		}
	    }
	}
    else						// Vertical Blinds
	{
	for (i = 0; i < PASSWIDTH; i++)
	    {
	    *curpass = i;
	    col = ixstart + i;
	    while (col <= ixstop)
		{
		reset_period = 1;
		row = yybegin;
		while (row <= yystop)
		    {
		    if (calc_frac(hwnd, row, col, user_data) == -1)
			return(-1);			// interrupted
		    ++row;
		    }
		col += PASSWIDTH;
		}
	    }
	}
    return(0);
    }

/**************************************************************************
	Routine for Vertical or Horizontal Blinds
**************************************************************************/

int	CPixel::DoBlinds(HWND hwnd, int user_data(HWND hwnd))

    {
    int	i;

    if (Blinds(hwnd, user_data) == -1)				// second or only pass
	{
	i = yystop;
	if (iystop != yystop)			// must be due to symmetry
	    i -= row - iystart;
	add_worklist(xxstart,xxstop,row,i,row,workpass,worksym);
	return(-1);
	}
    return(0);
    }

/**************************************************************************
	Add goodies for other segments of image
**************************************************************************/

int	CPixel::add_worklist(int xfrom, int xto, int yfrom, int yto, int ybegin, int pass, int sym)
    {
    if (num_worklist >= MAXCALCWORK)
	return(-1);
    worklist[num_worklist].xxstart = xfrom;
    worklist[num_worklist].xxstop  = xto;
    worklist[num_worklist].yystart = yfrom;
    worklist[num_worklist].yystop  = yto;
    worklist[num_worklist].yybegin = ybegin;
    worklist[num_worklist].pass    = pass;
    worklist[num_worklist].sym     = sym;
    ++num_worklist;
    tidy_worklist();
    return(0);
}

/**************************************************************************
	Look for 2 entries which can freely merge
**************************************************************************/

int	CPixel::combine_worklist()

    {
    int	i, j;

    for (i = 0; i < num_worklist; ++i)
	if (worklist[i].yystart == worklist[i].yybegin)
	    for (j = i + 1; j < num_worklist; ++j)
		if (worklist[j].sym == worklist[i].sym
		    && worklist[j].yystart == worklist[j].yybegin
		    && worklist[i].pass == worklist[j].pass)
		    {
		    if ( worklist[i].xxstart == worklist[j].xxstart
			&& worklist[i].xxstop  == worklist[j].xxstop)
			{
			if (worklist[i].yystop+1 == worklist[j].yystart)
			    {
			    worklist[i].yystop = worklist[j].yystop;
			    return(j);
			}
		    if (worklist[j].yystop+1 == worklist[i].yystart)
			{
			worklist[i].yystart = worklist[j].yystart;
			worklist[i].yybegin = worklist[j].yybegin;
			return(j);
			}
		    }
		if ( worklist[i].yystart == worklist[j].yystart
		       && worklist[i].yystop  == worklist[j].yystop)
		    {
		    if (worklist[i].xxstop+1 == worklist[j].xxstart)
			{
			worklist[i].xxstop = worklist[j].xxstop;
			return(j);
			}
		    if (worklist[j].xxstop+1 == worklist[i].xxstart)
			{
			worklist[i].xxstart = worklist[j].xxstart;
			return(j);
			}
		    }
		}
    return(0);				/* nothing combined */
    }

/**************************************************************************
	Combine mergeable entries, resort
**************************************************************************/

void	CPixel::tidy_worklist()

    {
    int	i, j;
    struct	workliststuff tempwork;

    while (i = combine_worklist())
	{					/* merged two, delete the gone one */
	while (++i < num_worklist)
	    worklist[i-1] = worklist[i];
	--num_worklist;
	}
    for (i = 0; i < num_worklist; ++i)
	for (j = i + 1; j<num_worklist; ++j)
	    if (worklist[j].pass < worklist[i].pass
		|| (worklist[j].pass == worklist[i].pass
		&& (worklist[j].yystart < worklist[i].yystart
		|| (worklist[j].yystart == worklist[i].yystart
		&& worklist[j].xxstart <  worklist[i].xxstart))))
		{			/* dumb sort, swap 2 entries to correct order */
		tempwork = worklist[i];
		worklist[i] = worklist[j];
		worklist[j] = tempwork;
		}
    }

/**************************************************************************
	Run standard plotting mode fractals
**************************************************************************/

int	CPixel::StandardCalculationMode(HWND hwnd, CSlope Slope, int user_data(HWND hwnd))

    {
//    *ThreadComplete = false;
    switch (calcmode)
	{
	case 'B':
	    if (bound_trace_main(hwnd, user_data) == -1)
		return -1;
	    break;
	case 'G':
	    if (solidguess(hwnd, user_data) == -1)
		return -1;
	    break;
	case 'T':
	    if (tesseral(hwnd, user_data) == -1)
		return -1;
	    break;
	case 'S':
	    if (Spiral(hwnd, user_data) == -1)
		return -1;
	    break;
	case '1':
	case '2':
	    if (OneOrTwoPass(hwnd, user_data) == -1)
		return -1;
	    break;
	case 'V':
	case 'H':
	    if (DoBlinds(hwnd, user_data) == -1)
		return -1;
	    break;
	case 'F':
	    if (OneOrTwoPass(hwnd, user_data) == -1)
//	    if (solidguess(hwnd, Pix) == -1)
//	    if (bound_trace_main(hwnd, Pix, num_worklist) == -1)
//	    if (tesseral(hwnd, Pix, user_data) == -1)
		return -1;
	    Slope.RenderSlope(xdots, ydots, 0/*PertColourMethod*/, 0/*PalOffset*/, 1.0/*IterDiv*/);	// fix later
	    break;
	default:
	    calcmode = 'G';
	    if (solidguess(hwnd, user_data) == -1)
		return -1;
	}

//    *ThreadComplete = true;
    return 0;
    }

/**************************************************************************
	Init standard plotting mode fractals
**************************************************************************/

//void	CCalcMode::InitCalcMode0(struct workliststuff *worklistIn, int *symmetryIn)
//    {
//    symmetry = symmetryIn;
//    for (int i = 0; i < MAXCALCWORK; i++)
//	worklist[i] = &worklistIn[i];
//    }
//void	CCalcMode::InitCalcMode1(int *totpassesIn, int *curpassIn, long *colorIn, int reset_periodIn, BYTE calcmodeIn, long thresholdIn, BYTE juliaflagIn,
//    long fillcolorIn, int *blockindexIn, BYTE save_flagIn, int *time_to_zoomIn, int *time_to_restartIn, int *time_to_reinitIn, int *time_to_quitIn,
//    CTrueCol *TrueColIn, CDib *DibIn, double *wpixelsIn, long *andcolorIn, int *oldrowIn, int *oldcolIn, int *PlotTypeIn, int threadIn)

//    {
//    totpasses = totpassesIn; curpass = curpassIn; color = colorIn; reset_period = reset_periodIn; calcmode = calcmodeIn; threshold = thresholdIn; juliaflag = juliaflagIn;
//    fillcolor = fillcolorIn; blockindex = blockindexIn; save_flag = save_flagIn; time_to_zoom = time_to_zoomIn; time_to_restart = time_to_restartIn;
//    time_to_reinit = time_to_reinitIn; time_to_quit = time_to_quitIn; TrueCol = TrueColIn; Dib = DibIn; wpixels = wpixelsIn; /*andcolor = andcolorIn; oldrow = oldrowIn;
//    oldcol = oldcolIn; PlotType = PlotTypeIn; thread = threadIn;
//    }
