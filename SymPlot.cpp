/*
    SYMPLOT.CPP a module of symmetry functions.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Pixel.h"

/**************************************************************************
	Select appropriate symmetry plot 
**************************************************************************/

void	CPixel::plot(WORD x, WORD y, DWORD color)
    {
//    if (ghMutex != NULL)
//	WaitForSingleObject(ghMutex, INFINITE);  // no time-out interval

    switch (*PlotType)
	{
	case NOSYM:
	    PlotPixel(x, y, color);
	    break;
	case XAXIS:
	    symplot2(x, y, color);
	    break;
	case YAXIS:
	    symplot2Y(x, y, color);
	    break;
	case XYAXIS:
	    symplot4(x, y, color);
	    break;
	case ORIGIN:
	    symplot2J(x, y, color);
	    break;
	case XAXISBASIN:
	    symplot2basin(x, y, color);
	    break;
	case XYAXISBASIN:
	    symplot4basin(x, y, color);
	    break;
	case FILTERPLOT:
	    Plot.FilterPoint(x, y, color, &(TZfilter->FilterRGB));
	    break;
	default:
	    PlotPixel(x, y, color);
	    break;
	}
//    if (ghMutex != NULL)
//	ReleaseMutex(ghMutex);
    }


/**************************************************************************
	Symmetry plot for X Axis Symmetry
**************************************************************************/

void	CPixel::symplot2(WORD x, WORD y, DWORD color)

    {
    int	i;

    PlotPixel(x, y, color) ;
    i = yystop - (y - yystart);
    if (i > iystop && i < (int)ydots)
	PlotPixel(x, (WORD)i, color) ;
    }

/**************************************************************************
	Symmetry plot for Y Axis Symmetry
**************************************************************************/

void	CPixel::symplot2Y(WORD x, WORD y, DWORD color)

    {
    int	i;

    PlotPixel(x, y, color) ;
    i = xxstop - (x - xxstart);
    if (i > ixstop && i < (int)xdots)
	PlotPixel((WORD)i, y, (int)color) ;
    }

/**************************************************************************
	Symmetry plot for Origin Axis Symmetry
**************************************************************************/

void	CPixel::symplot2J(WORD x, WORD y, DWORD color)

    {
    int	i, j;

    PlotPixel(x, y, color) ;
    i = yystop - (y - yystart);
    j = xxstop - (x - xxstart);
    if (i > iystop && i < (int)ydots && j < (int)xdots)
	PlotPixel((WORD)j, (WORD)i, color) ;
    }

/**************************************************************************
	Symmetry plot for Both Axis Symmetry
**************************************************************************/

void	CPixel::symplot4(WORD x, WORD y, DWORD color)

    {
    int	i, j;

    j = xxstop-(x-xxstart);
    PlotPixel(x, y, color) ;
    if (j > ixstop && j < (int)xdots)
	PlotPixel((WORD)j , y, color) ;
    i = yystop - (y - yystart);
    if (i > iystop && i < (int)ydots)
	{
	PlotPixel(x , (WORD)i, color) ;
	if (j < (int)xdots)
	    PlotPixel((WORD)j , (WORD)i, color) ;
	}
    }

/**************************************************************************
	Symmetry plot for X Axis Symmetry - Striped Newtbasin version
**************************************************************************/

void	CPixel::symplot2basin(WORD x, WORD y, DWORD color)

    {
    int	i, stripe;

    PlotPixel(x, y, color);
    if (subtype == 'S' && color > 8)
	stripe = 8;
    else
	stripe = 0;
    if ((i = yystop - (y - yystart)) > iystop && i < (int)ydots)
	{
	color -= stripe;			/* reconstruct unstriped color */
	color = (*degree + 1 - color) % *degree + 1;	/* symmetrical color */
	color += stripe;					/* add stripe */
	PlotPixel(x, (WORD)i,color);
	}
    }

/**************************************************************************
	Symmetry plot for Both Axis Symmetry  - Newtbasin version
**************************************************************************/

void	CPixel::symplot4basin(WORD x, WORD y, DWORD color)

    {
    int i,j,color1,stripe;

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
    color1 = *degree / 2 + *degree + 2 - (int)color;
    if ((int)color < *degree / 2 + 2)
	color1 = *degree / 2 + 2 - (int)color;
    else
	color1 = *degree / 2 + *degree + 2 - (int)color;
    j = xxstop - (x - xxstart);
    PlotPixel(x, y, color + stripe) ;
    if (j < (int)xdots)
	PlotPixel((WORD)j, y, color1 + stripe) ;
    if ((i = yystop - (y - yystart)) > iystop && i < (int)ydots)
	{
	PlotPixel(x, (WORD)i, stripe + (*degree + 1 - color) % *degree + 1) ;
	if (j < (int)xdots)
	    PlotPixel((WORD)j, (WORD)i, stripe + (*degree + 1 - color1) % *degree + 1) ;
	}
    }

/**************************************************************************
	Do nothing plot!!!
**************************************************************************/

void	CPixel::noplot(WORD x, WORD y, DWORD color)
    {
    }

/**************************************************************************
	Plot correct pixel according to count, i, j
**************************************************************************/

void	CPixel::PlotPixel(WORD x, WORD y, DWORD colour)

    {
    if (*time_to_quit)
	return;
    Plot.PlotPoint(x, y, colour);
    }


