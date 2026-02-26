#include "manp.h"
#include "colour.h"
#include "Dib.h"
//#include "pixel.h"
#include "PlotRoutines.h"

//extern CPixel PlotPixel;
//extern	double	*SlopeReflections;		// store values used for slope for each pixel
//extern	double	OldReflection;

void CPlotRoutines::InitPlot(BYTE calcmodeIn, HWND hwndIn, long thresholdIn, CDib *DibIn, DWORD  *wpixelsIn, CTrueCol *TrueColIn, int blockindexIn)

    {
    hwnd = hwndIn;
    calcmode = calcmodeIn;
    threshold = thresholdIn;
    Dib = DibIn;
    wpixels = wpixelsIn;
    TrueCol = TrueColIn;
    blockindex = blockindexIn;
    }

/**************************************************************************
	Run standard plotting mode fractals
**************************************************************************/

int	CPlotRoutines::StandardCalculationMode(long calc_frac(HWND, int, int), void(*plot)(WORD, WORD, DWORD), int user_data(HWND hwnd), void symplot2(WORD, WORD, DWORD), void symplot2Y(WORD, WORD, DWORD), void symplot2J(WORD, WORD, DWORD), void symplot4(WORD, WORD, DWORD),
			    CWorklist *WORKLIST, int *reset_period, long *colour, long fillcolor, MYVARIABLES *pVar)

    {
    switch (calcmode)
	{
	case 'B':
	    if (bound_trace_main(calc_frac, user_data, symplot2, symplot2Y, symplot2J, symplot4, reset_period, colour, pVar) == -1)
		return -1;
	    break;
	case 'G':
	    if (solidguess(calc_frac,  symplot2, symplot2J, WORKLIST, reset_period, pVar) == -1)
		return -1;
	    break;
	case 'T':
	    if (tesseral(calc_frac, user_data, symplot2, reset_period, WORKLIST, fillcolor, pVar) == -1)
		return -1;
	    break;
	case 'S':
	    if (Spiral(calc_frac, pVar) < 0)
		return -1;
	    break;
	case '1':
	case '2':
	    if (OneOrTwoPass(calc_frac, plot, WORKLIST, reset_period, colour, pVar) == -1)
		return -1;
	    break;
	case 'V':
	case 'H':
	    if (Blinds(calc_frac, plot, WORKLIST, reset_period, colour, pVar) < 0)
		return -1;
	    break;
	default:
	    calcmode = 'G';
	    if (solidguess(calc_frac, symplot2, symplot2J, WORKLIST, reset_period, pVar) == -1)
		return -1;
	}
//    PlotPixel.DoSlope2();
    return 0;
    }

/**************************************************************************
	Routine for 1 or 2 passes
**************************************************************************/

int	CPlotRoutines::OneOrTwoPass(long calc_frac(HWND, int, int), void(*plot)(WORD, WORD, DWORD), CWorklist *WORKLIST, int *reset_period, long *colour, MYVARIABLES *pVar)

    {
    int	i, row = 0;

    WORKLIST->totpasses = 1;
    if (calcmode == '2')
	WORKLIST->totpasses = 2;
    if (calcmode == '2' && WORKLIST->workpass == 0)		/* do 1st pass of two */
	{
	if (StandardCalc(calc_frac, plot, 1, &row, WORKLIST, reset_period, colour, pVar) == -1)
	    {
	    WORKLIST->AddWorklist(xxstart, xxstop, yystart, yystop, row, 0, WORKLIST->worksym);
	    return(-1);
	    }
	if (WORKLIST->num_worklist > 0) /* worklist not empty, defer 2nd pass */
	    {
	    WORKLIST->AddWorklist(xxstart, xxstop, yystart, yystop, yystart, 1, WORKLIST->worksym);
	    return(0);
	    }
	WORKLIST->workpass = 1;
	yybegin = yystart;
	}

    if (StandardCalc(calc_frac, plot, 2, &row, WORKLIST, reset_period, colour, pVar) == -1)			// second or only pass
	{
	i = yystop;
	if (iystop != yystop)										// must be due to symmetry
	    i -= row - iystart;
	WORKLIST->AddWorklist(xxstart, xxstop, row, i, row, WORKLIST->workpass, WORKLIST->worksym);
	return(-1);
	}
    return(0);
    }

/**************************************************************************
	Standard calculation
**************************************************************************/

int	CPlotRoutines::StandardCalc(long calc_frac(HWND, int, int), void(*plot)(WORD, WORD, DWORD), int passnum, int *row, CWorklist *WORKLIST, int *reset_period, long *colour, MYVARIABLES *pVar)

    {
    int col;

    WORKLIST->curpass = passnum;
    *row = yybegin;
    while (*row <= iystop)
	{
	*reset_period = 1;
	col = ixstart;
	while (col <= ixstop)
	    {				// on 2nd pass of two, skip even pts
	    if (passnum == 1 || calcmode == '1' || ((*row) & 1) != 0 || (col & 1) != 0)
		{
		if (calc_frac(hwnd, *row, col) == -1)
		    return(-1);								// interrupted
		if (passnum == 1)							// first pass, copy pixel and col++
		    {
		    if (((*row) & 1) == 0 && *row < iystop)
			{
			(*plot)((WORD)col, (WORD)((*row) + 1), *colour);
			if ((col & 1) == 0 && col < ixstop)
			    (*plot)((WORD)(col + 1), (WORD)((*row) + 1), *colour);
			}
		    if ((col & 1) == 0 && col < ixstop)
			(*plot)((WORD)(++col), (WORD)(*row), *colour);
		    }
		}
	    ++col;
	    }
	if (passnum == 1 && ((*row) & 1) == 0)
	    ++(*row);
	++(*row);
	}
    return(0);
    }

/**************************************************************************
	Routine for Spiralling In
**************************************************************************/

int	CPlotRoutines::Spiral(long calc_frac(HWND, int, int), MYVARIABLES *pVar)

    {
    int	xmin, xmax, ymin, ymax, row, col, i;

    xmin = ixstart;
    ymin = yybegin;
    xmax = ixstop;
    ymax = yystop;

    //    num_worklist = 0;								// only do as one segment

    for (i = 0; i < PASSWIDTH; i++)
	{
	xmin = ixstart + i;
	ymin = yybegin + i;
	xmax = ixstop - i;
	ymax = yystop - i;
	while (ymin <= ymax && xmin <= xmax)
	    {
	    row = ymin;
	    for (col = xmin; col < xmax; col++)
		{
		if (calc_frac(hwnd, row, col) == -1)
		    return(-1);		// interrupted
		}
	    col = xmax;
	    for (row = ymin; row < ymax; row++)
		{
		if (calc_frac(hwnd, row, col) == -1)
		    return(-1);		// interrupted
		}
	    row = ymax;
	    for (col = xmax; col >= xmin; col--)
		{
		if (calc_frac(hwnd, row, col) == -1)
		    return(-1);		// interrupted
		}
	    col = xmin;
	    for (row = ymax; row >= ymin; row--)
		{
		if (calc_frac(hwnd, row, col) == -1)
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

int	CPlotRoutines::Blinds(long calc_frac(HWND, int, int), void(*plot)(WORD, WORD, DWORD), CWorklist *WORKLIST, int *reset_period, long *colour, MYVARIABLES *pVar)

    {
    int	    row, col, i;

    WORKLIST->totpasses = PASSWIDTH;
    if (calcmode == 'H')			// Horizontal Blinds
	{
	for (i = 0; i < PASSWIDTH; i++)
	    {
	    WORKLIST->curpass = i;
	    row = yybegin + i;
	    while (row <= iystop)
		{
		*reset_period = 1;
		col = ixstart;
		while (col <= ixstop)
		    {
		    if (calc_frac(hwnd, row, col) == -1)
			return(-1);			// interrupted
		    if (row < iystop && col < ixstop)
			(*plot)((WORD)col, (WORD)row, *colour);
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
	    WORKLIST->curpass = i;
	    col = ixstart + i;
	    while (col <= ixstop)
		{
		*reset_period = 1;
		row = yybegin;
		while (row <= iystop)
		    {
		    if (calc_frac(hwnd, row, col) == -1)
			return(-1);			// interrupted
		    if (row < iystop && col < ixstop)
			(*plot)((WORD)col, (WORD)row, *colour);
		    ++row;
		    }
		col += PASSWIDTH;
		}
	    }
	}
    return(0);
    }

#ifdef SPLAT
/**************************************************************************
	Add goodies for other segments of image
**************************************************************************/

int	CPlotRoutines::add_worklist(int xfrom, int xto, int yfrom, int yto, int ybegin, int pass, int sym, short *num_worklist)
    {
    if (*num_worklist >= MAXCALCWORK)
	return(-1);
    worklist[*num_worklist].xxstart = xfrom;
    worklist[*num_worklist].xxstop = xto;
    worklist[*num_worklist].yystart = yfrom;
    worklist[*num_worklist].yystop = yto;
    worklist[*num_worklist].yybegin = ybegin;
    worklist[*num_worklist].pass = pass;
    worklist[*num_worklist].sym = sym;
    ++(*num_worklist);
    tidy_worklist(num_worklist);
    return(0);
    }

/**************************************************************************
	Look for 2 entries which can freely merge
**************************************************************************/

int	CPlotRoutines::combine_worklist(short *num_worklist)

    {
    int	i, j;

    for (i = 0; i < *num_worklist; ++i)
	if (worklist[i].yystart == worklist[i].yybegin)
	    for (j = i + 1; j < *num_worklist; ++j)
		if (worklist[j].sym == worklist[i].sym
		    && worklist[j].yystart == worklist[j].yybegin
		    && worklist[i].pass == worklist[j].pass)
		    {
		    if (worklist[i].xxstart == worklist[j].xxstart
			&& worklist[i].xxstop == worklist[j].xxstop)
			{
			if (worklist[i].yystop + 1 == worklist[j].yystart)
			    {
			    worklist[i].yystop = worklist[j].yystop;
			    return(j);
			    }
			if (worklist[j].yystop + 1 == worklist[i].yystart)
			    {
			    worklist[i].yystart = worklist[j].yystart;
			    worklist[i].yybegin = worklist[j].yybegin;
			    return(j);
			    }
			}
		    if (worklist[i].yystart == worklist[j].yystart
			&& worklist[i].yystop == worklist[j].yystop)
			{
			if (worklist[i].xxstop + 1 == worklist[j].xxstart)
			    {
			    worklist[i].xxstop = worklist[j].xxstop;
			    return(j);
			    }
			if (worklist[j].xxstop + 1 == worklist[i].xxstart)
			    {
			    worklist[i].xxstart = worklist[j].xxstart;
			    return(j);
			    }
			}
		    }
    return(0);				// nothing combined
    }

/**************************************************************************
	Combine mergeable entries, resort
**************************************************************************/

void	CPlotRoutines::tidy_worklist(short *num_worklist)

    {
    int	i, j;
    struct	workliststuff tempwork;

    while (i = combine_worklist(num_worklist))
	{						// merged two, delete the gone one
	while (++i < *num_worklist)
	    worklist[i - 1] = worklist[i];
	--*num_worklist;
	}
    for (i = 0; i < *num_worklist; ++i)
	for (j = i + 1; j < *num_worklist; ++j)
	    if (worklist[j].pass < worklist[i].pass
		|| (worklist[j].pass == worklist[i].pass
		    && (worklist[j].yystart < worklist[i].yystart
			|| (worklist[j].yystart == worklist[i].yystart
			    && worklist[j].xxstart < worklist[i].xxstart))))
		{					// dumb sort, swap 2 entries to correct order
		tempwork = worklist[i];
		worklist[i] = worklist[j];
		worklist[j] = tempwork;
		}
    }

#endif

/**************************************************************************
	Get values of r, g, b
**************************************************************************/

BYTE *	CPlotRoutines::GetRGB(DWORD colour)

    {
    DWORD	j, c;
    static	BYTE	rgb[3];

    c = (colour < MAXTHRESHOLD) ? colour : (colour % MAXTHRESHOLD);			// threshold can be bigger than palette
    j = c + c + c;
    *(rgb + 0) = *(TrueCol->PalettePtr + j + 2);
    *(rgb + 1) = *(TrueCol->PalettePtr + j + 1);
    *(rgb + 2) = *(TrueCol->PalettePtr + j + 0);
    return (rgb);
    }

/**************************************************************************
	Do correct put_line
**************************************************************************/

void	CPlotRoutines::output_line(WORD x0, WORD line, WORD length, DWORD *buffer)

    {
    DWORD	i, k;
    DWORD	local_width;
    WORD	j;
    if (x0 < 0 || x0 >= width || line < 0 || line >= height - 1 || (x0 + length) < 0 || (x0 + length) > width)
	return;

    local_width = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel);
    i = ((long)(height - 1 - line) * (long)(local_width + 3 - ((local_width - 1) % 4)) + (long)(x0 * 3));
    k = ((long)line * (long)width) + (long)x0;
    for (j = 0; j <= length; ++j)
	{
	*(wpixels + k++) = (DWORD)(*(buffer + j));
	if ((*(buffer + j)) >= (DWORD)threshold)		// handle inside colour
	    {
	    *(Dib->DibPixels + i + 0) = (BYTE)TrueCol->InsideBlue;
	    *(Dib->DibPixels + i + 1) = (BYTE)TrueCol->InsideGreen;
	    *(Dib->DibPixels + i + 2) = (BYTE)TrueCol->InsideRed;
	    }
	else
	    memcpy(Dib->DibPixels + i, GetRGB((DWORD)(*(buffer + j))), 3);
	i += 3L;
	}
    }



#ifdef SPLAT
/**************************************************************************
	Symmetry plot for X Axis Symmetry
**************************************************************************/

void	CPlotRoutines::symplot2(WORD x, WORD y, DWORD color)

    {
    int	i;

    point(x, y, color);
    i = yystop - (y - yystart);
    if (i > iystop && i < (int)ydots)
	point(x, (WORD)i, color);
    }

/**************************************************************************
	Symmetry plot for Y Axis Symmetry
**************************************************************************/

void	CPlotRoutines::symplot2Y(WORD x, WORD y, DWORD color)

    {
    int	i;

    point(x, y, color);
    i = xxstop - (x - xxstart);
    if (i < (int)xdots)
	point((WORD)i, y, (int)color);
    }

/**************************************************************************
	Symmetry plot for Origin Axis Symmetry
**************************************************************************/

void	CPlotRoutines::symplot2J(WORD x, WORD y, DWORD color)

    {
    int	i, j;

    point(x, y, color);
    i = yystop - (y - yystart);
    j = xxstop - (x - xxstart);
    if (i > iystop && i < (int)ydots && j < (int)xdots)
	point((WORD)j, (WORD)i, color);
    }

/**************************************************************************
	Symmetry plot for Both Axis Symmetry
**************************************************************************/

void	CPlotRoutines::symplot4(WORD x, WORD y, DWORD color)

    {
    int	i, j;

    j = xxstop - (x - xxstart);
    point(x, y, color);
    if (j < (int)xdots)
	point((WORD)j, y, color);
    i = yystop - (y - yystart);
    if (i > iystop && i < (int)ydots)
	{
	point(x, (WORD)i, color);
	if (j < (int)xdots)
	    point((WORD)j, (WORD)i, color);
	}
    }

/**************************************************************************
	Symmetry plot for X Axis Symmetry - Striped Newtbasin version
**************************************************************************/

void	CPlotRoutines::symplot2basin(WORD x, WORD y, DWORD color)

    {
    int	i, stripe;

    point(x, y, color);
    if (subtype == 'S' && color > 8)
	stripe = 8;
    else
	stripe = 0;
    if ((i = yystop - (y - yystart)) > iystop && i < (int)ydots)
	{
	color -= stripe;			/* reconstruct unstriped color */
	color = (degree + 1 - color) % degree + 1;	/* symmetrical color */
	color += stripe;					/* add stripe */
	point(x, (WORD)i, color);
	}
    }

/**************************************************************************
	Symmetry plot for Both Axis Symmetry  - Newtbasin version
**************************************************************************/

void	CPlotRoutines::symplot4basin(WORD x, WORD y, DWORD color)

    {
    int i, j, color1, stripe;

    if (color == 0)				/* assumed to be "inside" color */
	{
	symplot4(x, y, color);
	return;
	}
    if (subtype == 'S' && color > 8)
	stripe = 8;
    else
	stripe = 0;
    color -= stripe;			/* reconstruct unstriped color */
    color1 = degree / 2 + degree + 2 - (int)color;
    if ((int)color < degree / 2 + 2)
	color1 = degree / 2 + 2 - (int)color;
    else
	color1 = degree / 2 + degree + 2 - (int)color;
    j = xxstop - (x - xxstart);
    point(x, y, color + stripe);
    if (j < (int)xdots)
	point((WORD)j, y, color1 + stripe);
    if ((i = yystop - (y - yystart)) > iystop && i < (int)ydots)
	{
	point(x, (WORD)i, stripe + (degree + 1 - color) % degree + 1);
	if (j < (int)xdots)
	    point((WORD)j, (WORD)i, stripe + (degree + 1 - color1) % degree + 1);
	}
    }

/**************************************************************************
	Do nothing plot!!!
**************************************************************************/

void	CPlotRoutines::noplot(WORD x, WORD y, DWORD color)
    {
    }
#endif
