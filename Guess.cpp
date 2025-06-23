/*
    GUESS.CPP a module to do super solid guessing.
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include "Pixel.h"

/**************************************************************************
	Super solid guessing
**************************************************************************/

/*
   I, Timothy Wegner, invented this solidguessing idea and implemented it in
   more or less the overall framework you see here.  I am adding this note
   now in a possibly vain attempt to secure my place in history, because
   Pieter Branderhorst has totally rewritten this routine, incorporating
   a *MUCH* more sophisticated algorithm.  His revised code is not only
   faster, but is also more accurate. Harrumph!
*/

int	CPixel::solidguess(HWND hwnd, int user_data(HWND hwnd))

    {
    int i,x,y,xlim,ylim,blocksize;
    unsigned int *pfxp0,*pfxp1;
    unsigned int u;

    guessplot=(*PlotType != NOSYM && *PlotType != XAXIS && *PlotType != ORIGIN);
			/* check if guessing at bottom & right edges is ok */
    bottom_guess = (*PlotType == XAXIS || (*PlotType == NOSYM && iystop+1 == (int)ydots));
    right_guess = (*PlotType == ORIGIN || ((*PlotType == NOSYM || *PlotType == XAXIS) && ixstop+1 == (int)xdots));

		// there seems to be a bug in solid guessing at bottom and side
    bottom_guess = right_guess = 0;		// TIW march 1995

    i = maxblock = blocksize = ssg_blocksize();
    *totpasses = 1;
    while ((i >>= 1) > 1) 
	++(*totpasses);

    //	ensure window top and left are on required boundary, treat window
    //	as larger than it really is if necessary (this is the reason symplot
    //	routines must check for > xdots/ydots before plotting sym points) */


#ifdef DEBUG
wsprintf(s, "xxstart=%d,xxstop=%d,yystart=%d,yystop=%d,yystart=%d", xxstart,xxstop,yystart,yystop,yystart);
MessageBox (hwnd, s, "guess()", MB_ICONEXCLAMATION | MB_OK);
#endif
#ifdef DEBUG
wsprintf(s, "maxblock=%d,halfblock=%d,guessplot=%d,right_guess=%d,bottom_guess=%d", maxblock,halfblock,guessplot,right_guess,bottom_guess);
MessageBox (hwnd, s, "guess()", MB_ICONEXCLAMATION | MB_OK);
#endif


    ixstart &= -1 - (maxblock-1);
    iystart = yybegin;
    iystart &= -1 - (maxblock-1);
    if (workpass == 0)			// otherwise first pass already done
	{				// first pass, calc every blocksize**2 pixel, quarter result & paint it
	*curpass = 1;
	if (iystart <= yystart)		// first time for this window, init it
	    {
	    currow = 0;
	    memset(&tprefix[1][0][0],0,maxxblk*maxyblk*2);	// noskip flags off
	    reset_period = 1;
	    row=iystart;
	    for (col = ixstart; col <= ixstop; col += maxblock)
		{						// calc top row 
		if (calc_frac(hwnd, row, col, user_data) == -1)
		    {
		    add_worklist(xxstart,xxstop,yystart,yystop,yybegin,0,worksym);
		    goto exit_solidguess; 
		    }
		reset_period = 0;
		}
	    }
	else
	    memset(&tprefix[1][0][0],-1,maxxblk*maxyblk*2);	// noskip flags on

#ifdef DEBUG
MessageBox (hwnd, "1.", "guess()", MB_ICONEXCLAMATION | MB_OK);
#endif


	for(y=iystart; y<=iystop; y+=blocksize)
	    {
	    currow = y;
	    i = 0;
	    if (y + blocksize <= iystop)
		{						// calc the row below
		row=y+blocksize;
		reset_period = 1;
		for (col = ixstart; col <= ixstop; col += maxblock)
		    {
		    if (calc_frac(hwnd, row, col, user_data) == -1)
			return-1;
		    reset_period = 0;
		    }
		}

	    reset_period = 0;				    // FRACTINT v19.2
//	    calc_frac(hwnd, y, 0, user_data);		    // added to remove missing pixels evident since early versions of ManpWIN PHD 220716
	    if (i == -1 || guessrow(hwnd, 1,y,blocksize, user_data) != 0)	// interrupted?
		{
		if (y < yystart)
		    y = yystart;
		add_worklist(xxstart,xxstop,yystart,yystop,y,0,worksym);
		goto exit_solidguess;
		}
	    }

	if (num_worklist)				    // work list not empty, just do 1st pass
	    {
	    add_worklist(xxstart,xxstop,yystart,yystop,yystart,1,worksym);
	    goto exit_solidguess;
	    }
	++workpass;
	iystart = yystart & (-1 - (maxblock-1));

      // calculate skip flags for skippable blocks
	xlim=(ixstop+maxblock)/maxblock+1;
	ylim=((iystop+maxblock)/maxblock+15)/16+1;
	if (right_guess == 0)		// no right edge guessing, zap border
	    for (y = 0; y <= ylim; ++y)
		tprefix[1][y][xlim] = -1;
	if (bottom_guess == 0)		// no bottom edge guessing, zap border
	    {
	    i = (iystop + maxblock) / maxblock + 1;
	    y = i / 16 + 1;
	    i = 1 << (i & 15);
	    for (x = 0; x <= xlim; ++x)
		tprefix[1][y][x] |= i;
	    }
      // set each bit in tprefix[0] to OR of it & surrounding 8 in tprefix[1]
    for (y = 0; ++y < ylim;)
	    {
    	    pfxp0= (unsigned int *)&tprefix[0][y][0];
	    pfxp1= (unsigned int *)&tprefix[1][y][0];
	    for(x = 0; ++x<xlim;)
		{
		++pfxp1;
		u = *(pfxp1 - 1) | *pfxp1 | *(pfxp1 + 1);
		*(++pfxp0) = u | (u >> 1) | (u << 1)
		   |((*(pfxp1-(maxxblk+1))|*(pfxp1-maxxblk)|*(pfxp1-(maxxblk-1)))>>15)
		      |((*(pfxp1+(maxxblk-1))|*(pfxp1+maxxblk)|*(pfxp1+(maxxblk+1)))<<15);
		}
	    }
	}
    else							// first pass already done 
	memset(&tprefix[0][0][0], -1, maxxblk*maxyblk*2);	// noskip flags on 

#ifdef DEBUG
MessageBox (hwnd, "2.", "guess()", MB_ICONEXCLAMATION | MB_OK);
#endif


	// remaining pass(es), halve blocksize & quarter each blocksize**2 
    i = workpass;
    while (--i > 0)			// allow for already done passes 
	blocksize = blocksize>>1;
    reset_period = 0;			// FRACTINT v19.2
    while ((blocksize=blocksize>>1)>=2)
	{
	*curpass = workpass + 1;


#ifdef DEBUG
wsprintf(s, "Pass = %d", workpass);
MessageBox (hwnd, s, "guess()", MB_ICONEXCLAMATION | MB_OK);
#endif

	for (y = iystart; y <= iystop; y += blocksize)
	    {
	    currow = y;
	    if (guessrow(hwnd, 0, y, blocksize, user_data) !=0)
		{
		if (y < yystart)
		    y = yystart;
		add_worklist(xxstart,xxstop,yystart,yystop,y,workpass,worksym);
		goto exit_solidguess;
		}
	    }
	++workpass;
	if (num_worklist		// work list not empty, do one pass at a time
		    && blocksize > 2)	// if 2, we just did last pass
	    {
	    add_worklist(xxstart,xxstop,yystart,yystop,yystart,workpass,worksym);
	    goto exit_solidguess;
	    }
	iystart = yystart & (-1 - (maxblock-1));
	}

    exit_solidguess:
    if (*time_to_reinit + *time_to_restart + *time_to_quit + *time_to_zoom)
	return(-1);
    else
	return(0);
    }

/**************************************************************************
	Give back memory
**************************************************************************/

void	CPixel::ClearGuessMemory(DWORD *dstack)
    {
    if (dstack)
	{
	delete dstack;
	dstack = NULL;
	}
    }

/**************************************************************************
	Guess row
**************************************************************************/

int	CPixel::guessrow(HWND hwnd, int firstpass, int y, int blocksize, int user_data(HWND hwnd))

    {
    long	x, i, j	,color;
    long	xplushalf, xplusblock;
    long	ylessblock, ylesshalf, yplushalf, yplusblock;
    long	    c21,c31,c41;	// cxy is the color of pixel at (x,y)
    long	c12,c22,c32,c42;	// where c22 is the top left corner of
    long	c13,c23,c33;		// the block being handled in current 
    long	    c24,c44;		// iteration			      

    long	guessed23, guessed32, guessed33, guessed12, guessed13;
    long	prev11, fix21, fix31;
    unsigned long	*pfxptr, pfxmask;

    DWORD *dstack = NULL;

    #ifdef DEBUG
    char	s[200];

    if (NonStandardImage)
	{
	wsprintf(s, "firstpass=%d,y=%d,blocksize=%d", firstpass,y,blocksize);
	MessageBox (hwnd, s, "guess()", MB_ICONEXCLAMATION | MB_OK);
	}
    #endif

    if ((dstack = new DWORD[width * 3]) == NULL)
	return(-1);

    c44 = c41 = c42 = 0L;		// just for warning (from FRACTINT v18.26)

    halfblock = blocksize >> 1;
    i = y / maxblock;
    pfxptr = (unsigned long *)&tprefix[firstpass][(i >> 4) + 1][ixstart / maxblock];
    pfxmask = 1 << (i & 15);
    ylesshalf = y - halfblock;
    ylessblock = y - blocksize;			// constants, for speed 
    yplushalf = y + halfblock;
    yplusblock = y + blocksize;
    prev11 = -1;
    c24 = c12 = c13 = c22 = (long)Plot->GetColour((WORD)ixstart, (WORD)y);
    c31 = c21 = (long)Plot->GetColour((WORD)ixstart, (WORD)((y > 0) ? ylesshalf : 0));
    if (yplusblock <= iystop)
	c24 = (long)Plot->GetColour((WORD)ixstart, (WORD)yplusblock);
    else if(bottom_guess == 0)
	c24 = -1;
    guessed12 = guessed13 = 0;

    for (x = ixstart; x <= ixstop;)		// increment at end, or when doing continue
	{
	if ((x & (maxblock - 1)) == 0)		// time for skip flag stuff
	    {
	    ++pfxptr;
	    if (firstpass == 0 && (*pfxptr&pfxmask) == 0)  // check for fast skip 
		{
			    // next useful in testing to make skips visible
    /*
		if(halfblock==1)
		    {
		    PlotPixel(x + 1, y, 4);
		    PlotPixel(x, y + 1, 4);
		    PlotPixel(x + 1, y + 1, 4);
		    }
    */
		x += maxblock;
		prev11 = c31 = c21 = c24 = c12 = c13 = c22;
		guessed12 = guessed13 = 0;
		continue;
		}
	    }
	if (firstpass)					// 1st pass, paint topleft corner
	    plotblock(0, x, y, c22, dstack);
							// setup variables 
	xplusblock = (xplushalf = x + halfblock) + halfblock;
	if (xplushalf > ixstop)
	    {
	    if(right_guess == 0)
		c31 = -1;
	    }
	else if (y > 0)
	    c31 = (long)Plot->GetColour((WORD)xplushalf, (WORD)ylesshalf);
	if (xplusblock <= ixstop)
	    {
	    if(yplusblock <= iystop)
		c44 = (long)Plot->GetColour((WORD)xplusblock, (WORD)yplusblock);
	    c41 = (long)Plot->GetColour((WORD)xplusblock, (WORD)((y > 0) ? ylesshalf : 0));
	    c42 = (long)Plot->GetColour((WORD)xplusblock, (WORD)y);
	    }
	else if (right_guess == 0)
	    c41 = c42 = c44 = -1;
	if (yplusblock > iystop)
	    c44 = (bottom_guess) ? c42 : -1;

		    // guess or calc the remaining 3 quarters of current block
	guessed23 = guessed32 = guessed33 = 1;
	c23 = c32 = c33 = c22;
	if (yplushalf > iystop)
	    {
	    if (bottom_guess == 0)
		c23 = c33 = -1;
	    guessed23 = guessed33 = -1;
	    }
	if (xplushalf > ixstop)
	    {
	    if (right_guess == 0)
		c32 = c33 = -1;
	    guessed32 = guessed33 = -1;
	    }

	for (;;)		// go around till none of 23,32,33 change anymore
	    {
	    if (guessed33 > 0
		 && (c33!=c44 || c33!=c42 || c33!=c24 || c33!=c32 || c33!=c23))
		{
		calcadot(hwnd, c33, xplushalf, yplushalf);
		guessed33 = 0;
		}
	    if(guessed32 > 0
		&& (c32!=c33 || c32!=c42 || c32!=c31 || c32!=c21
		|| c32!=c41 || c32!=c23))
		    {
		    calcadot(hwnd, c32, xplushalf, y);
		    guessed32 = 0;
		    continue;
		    }
		if(guessed23 > 0
		    && (c23!=c33 || c23!=c24 || c23!=c13 || c23!=c12 || c23!=c32))
		    {
		    calcadot(hwnd, c23, x, yplushalf);
		    guessed23 = 0;
		    continue;
		    }
		break;
	    }

	if (firstpass) // note whether any of block's contents were calculated 
	    if (guessed23 == 0 || guessed32 == 0 || guessed33 == 0)
		*pfxptr |= pfxmask;

	if (halfblock > 1)	// not last pass, check if something to display
	    {
	    if (firstpass)	// display guessed corners, fill in block
		{
		if (guessplot)
		    {
		    if (guessed23 > 0)
			plot((WORD)x, (WORD)yplushalf, c23);
		    if (guessed32 > 0)
			plot((WORD)xplushalf, (WORD)y, c32);
		    if (guessed33 > 0)
			plot((WORD)xplushalf, (WORD)yplushalf, c33);
		    }
		}
	    plotblock(1, x, yplushalf, c23, dstack);
	    plotblock(0, xplushalf, y, c32, dstack);
	    plotblock(1, xplushalf, yplushalf, c33, dstack);
	    }
	else				// repaint changed blocks
	    {
	    if (c23 != c22)
		plotblock(-1, x, yplushalf, c23, dstack);
	    if (c32 != c22)
		plotblock(-1, xplushalf, y, c32, dstack);
	    if (c33 != c22)
		plotblock(-1, xplushalf, yplushalf, c33, dstack);
	    }

	  // check if some calcs in this block mean earlier guesses need fixing
	fix21 = ((c22 != c12 || c22 != c32)
		&& c21 == c22 && c21 == c31 && c21 == prev11
		&& y > 0
		&& (x == ixstart || c21 == (long)Plot->GetColour((WORD)(x - halfblock), (WORD)ylessblock))
		&& (xplushalf > ixstop || c21 == (long)Plot->GetColour((WORD)xplushalf, (WORD)ylessblock))
		&& c21 == (long)Plot->GetColour((WORD)x, (WORD)ylessblock));
	fix31 = (c22 != c32
		&& c31 == c22 && c31 == c42 && c31 == c21 && c31 == c41
		&& y > 0 && xplushalf <= ixstop
		&& c31 == (long)Plot->GetColour((WORD)xplushalf, (WORD)ylessblock)
		&& (xplusblock > ixstop || c31 == (long)Plot->GetColour((WORD)xplusblock, (WORD)ylessblock))
		&& c31 == (long)Plot->GetColour((WORD)x, (WORD)ylessblock));
	prev11 = c31;				// for next time around
	if (fix21)
	    {
	    calcadot(hwnd, c21, x, ylesshalf);
	    if (halfblock > 1 && c21 != c22)
		plotblock(-1, x, ylesshalf, c21, dstack);
	    }
	if (fix31)
	    {
	    calcadot(hwnd, c31, xplushalf, ylesshalf);
	    if(halfblock > 1 && c31 != c22)
		plotblock(-1, xplushalf, ylesshalf, c31, dstack);
	    }
	if(c23 != c22)
	    {
	    if (guessed12)
		{
		calcadot(hwnd, c12, x - halfblock, y);
		if (halfblock > 1 && c12 != c22)
		    plotblock(-1, x - halfblock, y, c12, dstack);
		}
	    if (guessed13)
		{
		calcadot(hwnd, c13, x - halfblock, yplushalf);
		if (halfblock > 1 && c13 != c22)
		    plotblock(-1, x - halfblock, yplushalf, c13, dstack);
		}
	    }

	c22 = c42;
	c24 = c44;
	c13 = c33;
	c31 = c21 = c41;
	c12 = c32;
	guessed12 = guessed32;
	guessed13 = guessed33;
	x += blocksize;
	}						// end x loop

    if (firstpass == 0 || guessplot)
	{
	ClearGuessMemory(dstack);
	return 0;
	}

       // paint rows the fast way
    for (i = 0;i<halfblock;++i)
	{
	if ((j = y + i) <= iystop) 
	    Plot->OutputLine((WORD)xxstart, (WORD)j, (WORD)(ixstop - xxstart + 1), &dstack[xxstart]);
	if ((j = y + i + halfblock) <= iystop)
	    Plot->OutputLine((WORD)xxstart, (WORD)j, (WORD)(ixstop - xxstart + 1), &dstack[xxstart + (xdots * 2)]);
	if (save_flag)
	    {
	    ClearGuessMemory(dstack);
	    return -1;
	    }
	}

    if (*PlotType != NOSYM)			// symmetry, just vertical & origin the fast way
	{
	if (*PlotType == ORIGIN)			// origin sym, reverse lines
	    for (i = (ixstop + xxstart + 1) / 2; --i >= xxstart;)
		{
		color = dstack[i];
		dstack[i] = dstack[j = ixstop - (i - xxstart)];
		dstack[j] = (DWORD)color;
		j += (xdots * 2);
		color = dstack[i + (xdots * 2)];
		dstack[i + (xdots * 2)] = dstack[j];
		dstack[j] = (DWORD)color;
		}
	for (i = 0; i < halfblock; ++i)
	    {
	    if ((j = yystop - (y + i - yystart)) > iystop && j < (int)ydots)
		Plot->OutputLine((WORD)xxstart, (WORD)j, (WORD)(ixstop - xxstart + 1), &dstack[xxstart]);
	    if ((j = yystop - (y + i + halfblock - yystart)) > iystop && j < (int)ydots)
		Plot->OutputLine((WORD)xxstart, (WORD)j, (WORD)(ixstop - xxstart + 1), &dstack[xxstart + (xdots * 2)]);
	    if (save_flag)
		return -1;
	    }
	}

    ClearGuessMemory(dstack);
    return 0;
    }

/**************************************************************************
	Plot Block of Pixels
**************************************************************************/

void	CPixel::plotblock(int buildrow, int x, int y, long colour, DWORD *dstack)

    {
    int i,xlim,ylim;

    if ((xlim = x + halfblock) > ixstop)
	xlim = ixstop + 1;
    if (buildrow >= 0 && guessplot == 0)		// save it for later put_line
	{
	if (buildrow == 0)
	    for (i = x; i < xlim; ++i)
		dstack[i] = (DWORD)colour;
	else
	    for (i = x; i < xlim; ++i)
		dstack[i + (xdots * 2)] = (DWORD)colour;
	}
					// paint it
    if ((ylim = y + halfblock) > iystop)
	{
	if (y > iystop)
	    return;
	ylim = iystop + 1;
	}
    for (i = x; ++i < xlim;)
	plot((WORD)i, (WORD)y, colour);			// skip 1st dot on 1st row
    while (++y<ylim)
	for (i = x; i < xlim; ++i)
	    plot((WORD)i, (WORD)y, colour);
    }

/**************************************************************************
	Calculate blocksize
**************************************************************************/

int	CPixel::ssg_blocksize(void)

    {
    int	blocksize, i;
   // blocksize 4 if <300 rows, 8 if 300-599, 16 if 600-1199, 32 if >=1200

    if (*blockindex < 1 || *blockindex > 8 || *blockindex == 2)
	blocksize = 4;				/* default */
    else
	blocksize = 1 << *blockindex;

    i = 300;
    while (i <= (int)ydots)
	{
	blocksize += blocksize;
	i += i;
	}
					// increase blocksize if tprefix array not big enough 
    while (blocksize * (maxxblk - 2) < (int)xdots || blocksize * (maxyblk - 2) * 16 < (int)ydots)
	blocksize += blocksize;
    return (blocksize);
    }

