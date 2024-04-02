/*
    TESS.CPP a module to do Tesserial plotting.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Pixel.h"

/**************************************************************************
	Tesseral plot 
**************************************************************************/

int	CPixel::tesseral(HWND hwnd, int user_data(HWND hwnd))
    {
    register struct tess *tp;

    guessplot = (*PlotType != NOSYM && *PlotType != XAXIS);

    DWORD *dstack = NULL;

    if ((dstack = new DWORD[width * 3]) == NULL)
	return(-1);

    tp = (struct tess *)&dstack[0];
    tp->x1 = ixstart;					    // set up initial box 
    tp->x2 = ixstop;
    tp->y1 = iystart;
    tp->y2 = iystop;

    if (workpass == 0) { /* not resuming */
	tp->top = tessrow(hwnd, ixstart,ixstop,iystart, user_data);     // Do top row 
	tp->bot = tessrow(hwnd, ixstart,ixstop,iystop, user_data);      // Do bottom row
	tp->lft = tesscol(hwnd, ixstart,iystart+1,iystop-1, user_data); // Do left column 
	tp->rgt = tesscol(hwnd, ixstop,iystart+1,iystop-1, user_data);  // Do right column 

	if (user_data(hwnd) == -1)			// user pressed a key? */
	    {
	    add_worklist(xxstart,xxstop,yystart,yystop,yystart,0,worksym);
	    return(-1);
	    }
	}

    else 
	{						// resuming, rebuild work stack
	int i,mid,curx,cury,xsize,ysize;
	struct tess *tp2;
	tp->top = tp->bot = tp->lft = tp->rgt = -2;
	cury = yybegin & 0xfff;
	ysize = 1; 
	i = (unsigned)yybegin >> 12;
	while (--i >= 0) ysize <<= 1;
	curx = workpass & 0xfff;
	xsize = 1; 
	i = (unsigned)workpass >> 12;
	while (--i >= 0) xsize <<= 1;
	while (1 == 1) 
	    {
	    tp2 = tp;
	    if (tp->x2 - tp->x1 > tp->y2 - tp->y1) 
		{ // next divide down middle
		if (tp->x1 == curx && (tp->x2 - tp->x1 - 2) < xsize)
		    break;
		mid = (tp->x1 + tp->x2) >> 1;		// Find mid point
		if (mid > curx) 
		    {					// stack right part
		    memcpy(++tp,tp2,sizeof(*tp));
		    tp->x2 = mid;
		    }
		tp2->x1 = mid;
		}
	    else	
		{ 					// next divide across
		if (tp->y1 == cury && (tp->y2 - tp->y1 - 2) < ysize) break;
		mid = (tp->y1 + tp->y2) >> 1;		// Find mid point
		if (mid > cury) 
		    { // stack bottom part
		    memcpy(++tp,tp2,sizeof(*tp));
		    tp->y2 = mid;
		    }
		tp2->y1 = mid;
		}
	    }
	}


//   got_status = 4; // for tab_display 

    while (tp >= (struct tess *)&dstack[0]) {	    // do next box 
	curcol = tp->x1;			    // for tab_display 
	currow = tp->y1;

	if (tp->top == -1 || tp->bot == -1 || tp->lft == -1 || tp->rgt == -1)
	    goto tess_split;
      // for any edge whose color is unknown, set it
	if (tp->top == -2)
	    tp->top = tesschkrow(tp->x1,tp->x2,tp->y1);
	if (tp->top == -1)
	    goto tess_split;
	if (tp->bot == -2)
	    tp->bot = tesschkrow(tp->x1,tp->x2,tp->y2);
	if (tp->bot != tp->top)
	    goto tess_split;
	if (tp->lft == -2)
	    tp->lft = tesschkcol(tp->x1,tp->y1,tp->y2);
	if (tp->lft != tp->top)
	    goto tess_split;
	if (tp->rgt == -2)
	    tp->rgt = tesschkcol(tp->x2,tp->y1,tp->y2);
	if (tp->rgt != tp->top)
	    goto tess_split;

	    {					// all 4 edges are the same color, fill in
	    int i,j;
	    i = 0;
	    if(fillcolor != 0)
		{

		if(fillcolor > 0)
		    tp->top = fillcolor & (colours-1);
//		    tp->top = (colours-1);



		if (guessplot || (j = tp->x2 - tp->x1 - 1) < 2) { // paint dots
		    for (col = tp->x1 + 1; col < tp->x2; col++)
			for (row = tp->y1 + 1; row < tp->y2; row++) {
			    plot((WORD)col,(WORD)row,tp->top);
			    if (user_data(hwnd) == -1)	// user pressed a key? 
				goto tess_end;
			    }
		    }
		else {					// use verline for speed 
		    for (row = tp->y1 + 1; row < tp->y2; row++) 
			{
			verline((WORD)row,(WORD)(tp->x1+1),(WORD)(tp->x2-1),(DWORD)(tp->top));
			if (PlotType != NOSYM)		// symmetry 
			    if ((j = yystop-(row-yystart)) > iystop && j < (int)ydots)
				verline((WORD)j,(WORD)(tp->x1+1),(WORD)(tp->x2-1),(DWORD)(tp->top));
			if (user_data(hwnd) == -1)	// user pressed a key? 
			    goto tess_end;
			}
		    }
		}
	    --tp;
	    }
	    continue;

    tess_split:
		{						// box not surrounded by same color, sub-divide
		int mid,midcolor;
		struct tess *tp2;
		if (tp->x2 - tp->x1 > tp->y2 - tp->y1) 
		    {						// divide down the middle 
		    mid = (tp->x1 + tp->x2) >> 1;		// Find mid point 
		    midcolor = tesscol(hwnd, mid, tp->y1+1, tp->y2-1, user_data); // Do mid column 
		    if (midcolor == -3) goto tess_end;
		    if (tp->x2 - mid > 1) {			// right part >= 1 column 
			if (tp->top == -1) tp->top = -2;
			if (tp->bot == -1) tp->bot = -2;
			tp2 = tp;
			if (mid - tp->x1 > 1) {			// left part >= 1 col, stack right 
			    memcpy(++tp,tp2,sizeof(*tp));
			    tp->x2 = mid;
			    tp->rgt = midcolor;
			    }
			tp2->x1 = mid;
			tp2->lft = midcolor;
			}
		    else
			--tp;
		    }
		else 
		    { 						// divide across the middle 
		    mid = (tp->y1 + tp->y2) >> 1;		// Find mid point 
		    midcolor = tessrow(hwnd, tp->x1+1, tp->x2-1, mid, user_data); // Do mid row 
		    if (midcolor == -3) goto tess_end;
		    if (tp->y2 - mid > 1) {			// bottom part >= 1 column 
			if (tp->lft == -1) tp->lft = -2;
			if (tp->rgt == -1) tp->rgt = -2;
			tp2 = tp;
			if (mid - tp->y1 > 1) {			// top also >= 1 col, stack bottom
			    memcpy(++tp,tp2,sizeof(*tp));
			    tp->y2 = mid;
			    tp->bot = midcolor;
			    }
			tp2->y1 = mid;
			tp2->top = midcolor;
			}
		    else
			--tp;
		    }
		}
	}

tess_end:
       if (tp >= (struct tess *)&dstack[0]) 
	   {				    // didn't complete 
	   int i,xsize,ysize;
	   xsize = ysize = 1;
	   i = 2;
	   while (tp->x2 - tp->x1 - 2 >= i) {
	       i <<= 1; 
	       ++xsize; 
	       }
	   i = 2;
	   while (tp->y2 - tp->y1 - 2 >= i) {
	       i <<= 1; 
	       ++ysize; 
	       }
	   add_worklist(xxstart,xxstop,yystart,yystop, (ysize<<12)+tp->y1,(xsize<<12)+tp->x1,worksym);
	   ClearTessMemory(dstack);
	   return(-1);
	   }
       ClearTessMemory(dstack);
       return(0);

    } /* tesseral */

/**************************************************************************
	Give back memory
**************************************************************************/

void	CPixel::ClearTessMemory(DWORD *dstack)
    {
    if (dstack)
	{
	delete dstack;
	dstack = NULL;
	}
    }

long	CPixel::tesschkcol(int x,int y1,int y2)
    {
    long i;
    i = (long)Plot.GetColour((WORD)x,(WORD)(++y1));
    while (--y2 > y1)
	if ((long)Plot.GetColour((WORD)x,(WORD)y2) != i) return -1;
    return i;
    }

long	CPixel::tesschkrow(int x1,int x2,int y)
    {
    long i;
    i = (long)Plot.GetColour((WORD)x1,(WORD)y);
    while (x2 > x1) 
	{
	if ((long)Plot.GetColour((WORD)x2,(WORD)y) != i) return -1;
	--x2; 
	}
    return i;
    }

long	CPixel::tesscol(HWND hwnd, int x,int y1,int y2, int user_data(HWND hwnd))
    {
    long colcolor,i;
    col = x;
    row = y1;
    reset_period = 1;
    colcolor = calc_frac(hwnd, row, col, user_data);
    reset_period = 0;
    while (++row <= y2) 
	{ /* generate the column */
	if ((i = calc_frac(hwnd, row, col, user_data)) < 0) return -3;
	if (i != colcolor) colcolor = -1;
	}
    return colcolor;
    }

long	CPixel::tessrow(HWND hwnd, int x1,int x2,int y, int user_data(HWND hwnd))
    {
    long rowcolor, i;
    row = y;
    col = x1;
    reset_period = 1;
    rowcolor = calc_frac(hwnd, row, col, user_data);
     reset_period = 0;
    while (++col <= x2) 
	{ /* generate the row */
	if ((i = calc_frac(hwnd, row, col, user_data)) < 0) return -3;
	if (i != rowcolor) rowcolor = -1;
	}
    return rowcolor;
    }

/**************************************************************************
Plot a vertical line of same colour on 256 colour graphics card
**************************************************************************/

void	CPixel::verline(WORD x0, WORD y0, WORD y1, DWORD colour)

    {
    WORD 	length, ylo, yhi;
    DWORD	i;

    ylo = (y0 < y1) ? y0 : y1;
    yhi = (y0 > y1) ? y0 : y1;
    length = yhi - ylo;

    for (i = 0; i <= length; ++i)
	plot((WORD)(ylo + i), x0, colour);
    return;
    }

