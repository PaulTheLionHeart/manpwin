/*
    HENON.CPP - a program to investigate Henon mapping. For more information
    refer to BYTE December 1986.
    
    Written in MICROSNOT VISUAL C++ by Paul de Leeuw.
*/

#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include "manpwin.h"
#include "OscProcess.h"
#include "Plot.h"
#include "Manp.h"

#define TRUE		1
#define FALSE		0

#define	ESC_CHAR	0x1b
#define	MAXREAL		1E+10

extern	int	user_data(HWND);
static	double	xscale, yscale, cosa, sina;
	char	grid;

void	henon(int, double *, double *);

/**************************************************************************
	Do Hénon Mapping
**************************************************************************/

int	DoHenon(void)
    {
    int		i, j, orbitn, HenonPoints;
    double	HenonA, xold, yold, HenonXStart, HenonYStart, HenonStep;//    char	s[200];

//    SetWindowText (GlobalHwnd, "Entering Henon()");
//      "shapes", "points per orbit", "x start", "y start", "step size", ES,

    HenonA = gManp->param[0];
    HenonPoints = (int)gManp->param[1];
    HenonXStart = gManp->param[2];
    HenonYStart = gManp->param[3];
    HenonStep = gManp->param[4];
    orbitn = (int)gManp->param[5];

    cosa = cos(HenonA);
    sina = sin(HenonA);
    xold = HenonXStart;					/* starting pt for 1st orbit */
    yold = HenonYStart;
    xscale = (double) (gManp->xdots - 1) / (gManp->mandel_width * gManp->AspectRatio);
    yscale = (double) (gManp->ydots - 1) / gManp->mandel_width;
    for (j = 0; j < orbitn; ++j)			/* main loop j = orbit no. */
	{
//	_snprintf_s(s, MAXLINE, _TRUNCATE, "Henon Mapping Orbit %d of %d", j, orbitn);
//	SetWindowText (GlobalHwnd, s);
	for (i = 0; i < HenonPoints; ++i)
	    {
	    if (user_data(gManp->GlobalHwnd) == -1)		/* user pressed a key? */
		return (-1);
	    if ((xold < MAXREAL && xold > - MAXREAL)
				    && (yold < MAXREAL && yold > -MAXREAL))
		henon(j, &yold, &xold);
	    }
	xold = HenonXStart + HenonStep * j;
	yold = HenonYStart + HenonStep * j;
	}
    return 0;
    }
	
/**************************************************************************
	Actual Hénon Plot
***************************************************************************/

void	henon(int j, double *yold, double *xold)

    {
    double	temp, xnew, ynew;
    int		p1, p2;

    temp = *yold - *xold * *xold;
    xnew = *xold * cosa - temp * sina;
    ynew = *xold * sina + temp * cosa;
    							// use iteration count as the z axis value
    gManp->OscProcess.ChangeCoordSystem(&xnew, &ynew, &temp, xnew, ynew, (double)j, gManp->CoordSystem);

    p1 = (int)((xnew - gManp->hor) * xscale + 0.5);
    p2 = (int)((gManp->vert + gManp->mandel_width - ynew) * yscale + 0.5);

    if (p1 > 0 && p1 < gManp->xdots && p2 > 0 && p2 < gManp->ydots)
	gManp->Plot.PlotPoint((WORD)p1, (WORD)p2, (DWORD)(j % gManp->threshold));
    *xold = xnew;
    *yold = ynew;
    }
	
