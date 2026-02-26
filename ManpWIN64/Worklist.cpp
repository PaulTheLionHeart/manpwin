// Worklist.cpp: implementation of the CWorklist class.
//
//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "Worklist.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorklist::CWorklist()				// work list entry for std escape time engines
    {
    WorklistArray[0].yystart = WorklistArray[0].xxstart = WorklistArray[0].yybegin = 0;
    WorklistArray[0].yystop = 0;
    WorklistArray[0].xxstop = 0;
    num_worklist = 1;
    workpass = 0;
    WorklistArray[0].xxstart = 0;
    WorklistArray[0].yystart = WorklistArray[0].yybegin = 0;
    WorklistArray[0].pass = WorklistArray[0].sym = 0;
    }

CWorklist::~CWorklist()
    {
    }

/**************************************************************************
	Add goodies for other segments of image
**************************************************************************/

int CWorklist::AddWorklist(int xfrom, int xto, int yfrom, int yto, int ybegin, int pass, int sym)

    {
    if (num_worklist >= MAXCALCWORK)
	return(-1);
    WorklistArray[num_worklist].xxstart = xfrom;
    WorklistArray[num_worklist].xxstop = xto;
    WorklistArray[num_worklist].yystart = yfrom;
    WorklistArray[num_worklist].yystop = yto;
    WorklistArray[num_worklist].yybegin = ybegin;
    WorklistArray[num_worklist].sym = sym;
    WorklistArray[num_worklist].pass = pass;
    ++num_worklist;
    TidyWorklist();
    return(0);
    }

void	CWorklist::CopyWorklist(int to, int from)
    {
    WorklistArray[to].xxstart = WorklistArray[from].xxstart;	// screen window for this entry
    WorklistArray[to].xxstop = WorklistArray[from].xxstop;
    WorklistArray[to].yystart = WorklistArray[from].yystart;
    WorklistArray[to].yystop = WorklistArray[from].yystop;
    WorklistArray[to].yybegin = WorklistArray[from].yybegin;	// start row within window, for 2pass/ssg resume 
    WorklistArray[to].sym = WorklistArray[from].sym;		// if symmetry in window, prevents bad combines 
    WorklistArray[to].pass = WorklistArray[from].pass;		// for 2pass and solid guessing 
    }


/**************************************************************************
	Work list entry for std escape time engines
**************************************************************************/

void	CWorklist::TidyWorklist()

    {
    int	i, j, tempwork;

    while (i = CombineWorklist())
	{					// merged two, delete the gone one
	while (++i < num_worklist)
	    {
	    WorklistArray[i - 1].xxstart = WorklistArray[i].xxstart;	// screen window for this entry
	    WorklistArray[i - 1].xxstop = WorklistArray[i].xxstop;
	    WorklistArray[i - 1].yystart = WorklistArray[i].yystart;
	    WorklistArray[i - 1].yystop = WorklistArray[i].yystop;
	    WorklistArray[i - 1].yybegin = WorklistArray[i].yybegin;	// start row within window, for 2pass/ssg resume 
	    WorklistArray[i - 1].sym = WorklistArray[i].sym;		// if symmetry in window, prevents bad combines 
	    WorklistArray[i - 1].pass = WorklistArray[i].pass;		// for 2pass and solid guessing 
	    }
	--num_worklist;
	}
    for (i = 0; i < num_worklist; ++i)
	{
	for (j = i + 1; j < num_worklist; ++j)
	    {
	    if (WorklistArray[j].pass < WorklistArray[i].pass || (WorklistArray[j].pass == WorklistArray[i].pass && (WorklistArray[j].yystart < WorklistArray[i].yystart || (WorklistArray[j].yystart == WorklistArray[i].yystart && WorklistArray[j].xxstart < WorklistArray[i].xxstart))))
		{			// dumb sort, swap 2 entries to correct order 
		WorklistArray[i - 1].xxstart = WorklistArray[i].xxstart;	// screen window for this entry
		WorklistArray[i - 1].xxstop = WorklistArray[i].xxstop;
		WorklistArray[i - 1].yystart = WorklistArray[i].yystart;
		WorklistArray[i - 1].yystop = WorklistArray[i].yystop;
		WorklistArray[i - 1].yybegin = WorklistArray[i].yybegin;	// start row within window, for 2pass/ssg resume 
		WorklistArray[i - 1].sym = WorklistArray[i].sym;		// if symmetry in window, prevents bad combines 
		WorklistArray[i - 1].pass = WorklistArray[i].pass;		// for 2pass and solid guessing 

		tempwork = WorklistArray[i].xxstart;	WorklistArray[i].xxstart = WorklistArray[j].xxstart;	WorklistArray[j].xxstart    = tempwork;
		tempwork = WorklistArray[i].xxstop;	WorklistArray[i].xxstop  = WorklistArray[j].xxstop;	WorklistArray[j].xxstop	    = tempwork;
		tempwork = WorklistArray[i].yystart;	WorklistArray[i].yystart = WorklistArray[j].yystart;	WorklistArray[j].yystart    = tempwork;
		tempwork = WorklistArray[i].yystop;	WorklistArray[i].yystop  = WorklistArray[j].yystop;	WorklistArray[j].yystop	    = tempwork;
		tempwork = WorklistArray[i].yybegin;	WorklistArray[i].yybegin = WorklistArray[j].yybegin;	WorklistArray[j].yybegin    = tempwork;
		tempwork = WorklistArray[i].sym;	WorklistArray[i].sym	 = WorklistArray[j].sym;	WorklistArray[j].sym	    = tempwork;
		tempwork = WorklistArray[i].pass;	WorklistArray[i].pass	 = WorklistArray[j].pass;	WorklistArray[j].pass	    = tempwork;
		}
	    }
	}
    }

/**************************************************************************
	Look for 2 entries which can freely merge
**************************************************************************/

int	CWorklist::CombineWorklist()

    {
    int	i, j;

    for (i = 0; i < num_worklist; ++i)
	{
	if (WorklistArray[i].yystart == WorklistArray[i].yybegin)
	    {
	    for (j = i + 1; j < num_worklist; ++j)
		{
		if (WorklistArray[j].sym == WorklistArray[i].sym && WorklistArray[j].yystart == WorklistArray[j].yybegin && WorklistArray[i].pass == WorklistArray[j].pass)
		    {
		    if (WorklistArray[i].xxstart == WorklistArray[j].xxstart && WorklistArray[i].xxstop == WorklistArray[j].xxstop)
			{
			if (WorklistArray[i].yystop + 1 == WorklistArray[j].yystart)
			    {
			    WorklistArray[i].yystop = WorklistArray[j].yystop;
			    return(j);
			    }
			if (WorklistArray[j].yystop + 1 == WorklistArray[i].yystart)
			    {
			    WorklistArray[i].yystart = WorklistArray[j].yystart;
			    WorklistArray[i].yybegin = WorklistArray[j].yybegin;
			    return(j);
			    }
			}
		    if (WorklistArray[i].yystart == WorklistArray[j].yystart && WorklistArray[i].yystop == WorklistArray[j].yystop)
			{
			if (WorklistArray[i].xxstop + 1 == WorklistArray[j].xxstart)
			    {
			    WorklistArray[i].xxstop = WorklistArray[j].xxstop;
			    return(j);
			    }
			if (WorklistArray[j].xxstop + 1 == WorklistArray[i].xxstart)
			    {
			    WorklistArray[i].xxstart = WorklistArray[j].xxstart;
			    return(j);
			    }
			}
		    }
		}
	    }
	}
    return(0);				// nothing combined 
    }


