/*
    FIBONACI.CPP - a module for Fibonacci Spirals
    
    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <atomic>
#include "manp.h"
#include "colour.h"
#include "resource.h"
#include "Plot.h"

extern	int	user_data(HWND);

extern	WORD		delay;
extern	std::atomic<bool> gStopRequested;	// force early exit

WORD	SpiralNo;

#define	DEG2RAD	57.29577951
#define	FIBMAX	1000

struct	fibstruct
    {    
    WORD    x;
    WORD    y;
    WORD    c;
    }	FibArray[FIBMAX];

/**************************************************************************
	Fibonacci Spiral Type Images
***************************************************************************/

void	PlotSpiral(double angle, int colour)
    {                                             
    int	i, j;        
    double	radius, temp;

    temp = colour;

    switch (gManp->subtype)
	{ 
	case 'I':				// linear
	    radius = (double)temp / (double)gManp->threshold;
	    break;
	case 'L':				// log
	    radius = log((double)temp) / log((double)gManp->threshold);
	    break;
	case 'S':				// square
	    radius = (double)temp * (double)temp / ((double)gManp->threshold * (double)gManp->threshold);
	    break;
	case 'E':				// exponential
	    radius = (1.0 / ((double)gManp->threshold)) / (1.0 / ((double)temp));
	    break;
	case 'R':				// square root
	    radius = sqrt((double)temp) / sqrt((double)gManp->threshold);
	    break;
	default:				// uninitialised - assume linear
	    radius = (double)temp / (double)gManp->threshold;
	    break;
	}
                            
    i = (int)(radius * cos(angle / DEG2RAD) * (gManp->ydots / 2 - 1)) + gManp->xdots / 2;
    j = (int)(radius * sin(angle / DEG2RAD) * (gManp->ydots / 2 - 1)) + gManp->ydots / 2;
    FibArray[colour].x = i;
    FibArray[colour].y = j;
    FibArray[colour].c = colour % 14 + 1;
    }

/**************************************************************************
	Calculate delay
***************************************************************************/

int	FibDelay(HWND hwnd, WORD MilliSec)
    {
    DWORD	tick;
                     
    tick = GetTickCount();
    while (GetTickCount() < tick + MilliSec);

    if (user_data(hwnd) < 0)
	return -1;
    return 0;
    }

/**************************************************************************
	Plot Spirals
***************************************************************************/

int	Spirals(HWND hwnd, WORD order, WORD delay, CPlot Plot)
    {
    int	i, j;
         
    for (i = 0; i < order; ++i)
	{
	for (j = 0; j < gManp->threshold / order; ++j)
	    if ((j + 1) * order + i < gManp->threshold)
		{
		if (AbortRequested())
		    return 0;
		Plot.genline(FibArray[j * order + i].x, FibArray[j * order + i].y, 
		    FibArray[(j + 1) * order + i].x, FibArray[(j + 1) * order + i].y, i % 14 + 1);
		}
	if (FibDelay(hwnd, delay) < 0)
	    return -1;
	}
    return 0;
    }

/**************************************************************************
	Fibonacci Spiral Type Images
***************************************************************************/

void	initFibonacci(void)
    {
    SpiralNo = 13;
    delay = 10;
    }

/**************************************************************************
	Fibonacci Spiral Type Images
***************************************************************************/

int	Fibonacci(void)
    {
    double	angle, step;
    int	j;

    angle = 0.0;
    step = 360.0 * (2.0 - ((sqrt(5.0) + 1.0) / 2.0));		// Golden Mean ratio in degrees

    if (gManp->threshold >= FIBMAX)
	gManp->threshold = FIBMAX - 1;
    for (j = 0; j < gManp->threshold; ++j)
	{
	if (AbortRequested())
	    return 0;
	angle += step;
	if (angle > 360.0)
	    angle -= 360.0;
	PlotSpiral(angle, j);
	}

    for (j = 0; j < gManp->threshold; ++j)
	{
	gManp->Plot.PlotPoint(FibArray[j].x, FibArray[j].y, FibArray[j].c);
	if (FibDelay(gManp->GlobalHwnd, delay) < 0)
	    return -1;
	}

    if (Spirals(gManp->GlobalHwnd, SpiralNo, delay, gManp->Plot) < 0)
	return -1;
    return 0;
    }                                                 


/**************************************************************************
	Dialog Control for Cubic Fractal Sub Types
**************************************************************************/

INT_PTR CALLBACK FibTypeDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     static	HANDLE	hCursor;
     static     char	temp;
     static     UINT	tempParam;
     static     int	temp_step;
     static     int	temp_delay;
     static     int	temp_SpiralNo;
     static     int	temp_threshold;
     BOOL		bTrans ;
     
     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = gManp->subtype;
	        switch (gManp->subtype)
		    { 
		    case 'I':
			tempParam = IDC_LINEAR;
			break;
		    case 'L':
			tempParam = IDC_LOG;
			break;
		    case 'S':
			tempParam = IDC_SQUARE;
			break;
		    case 'E':
			tempParam = IDC_INVERSE;
			break;
		    case 'R':
			tempParam = IDC_SQUAREROOT;
			break;
		    default:				// uninitialised
			tempParam = IDC_LINEAR;
			temp = 'I';
			break;
		    }
//WORD	delay, SpiralNo;

//		SetDlgItemInt(hDlg, IDC_FIBSTEPS, FibSteps, TRUE);
		SetDlgItemInt(hDlg, IDC_FIBTIME, delay, TRUE);
		SetDlgItemInt(hDlg, IDM_THRESHOLD, gManp->threshold, TRUE);
		SetDlgItemInt(hDlg, IDC_FIBSPIRALCOUNT, SpiralNo, TRUE);
		CheckRadioButton(hDlg, IDC_LINEAR, IDC_SQUAREROOT, tempParam);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return FALSE ;
//	        return TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {                 
		    case IDC_LINEAR:
		    case IDC_LOG:
		    case IDC_SQUARE:
		    case IDC_INVERSE:
		    case IDC_SQUAREROOT:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_LINEAR:
				temp = 'I';
				break;
			    case IDC_LOG:
				temp = 'L';
				break;
			    case IDC_SQUARE:
				temp = 'S';
				break;
			    case IDC_INVERSE:
				temp = 'E';
				break;
			    case IDC_SQUAREROOT:
				temp = 'R';
				break;
			    }

			CheckRadioButton(hDlg, IDC_VERHULST, IDC_QUADMAND, (int) LOWORD(wParam));
		        return TRUE ;
/*		        
		    case IDC_FIBSTEPS:
			temp_step = GetDlgItemInt(hDlg, IDC_FIBSTEPS, &bTrans, TRUE);
		        return TRUE ;
*/
		    case IDC_FIBTIME:
			temp_delay = GetDlgItemInt(hDlg, IDC_FIBTIME, &bTrans, TRUE);
		        return TRUE ;

		    case IDC_FIBSPIRALCOUNT:
			temp_SpiralNo = GetDlgItemInt(hDlg, IDC_FIBSPIRALCOUNT, &bTrans, TRUE);
		        return TRUE ;

		    case IDM_THRESHOLD:
			temp_threshold = GetDlgItemInt(hDlg, IDM_THRESHOLD, &bTrans, TRUE);
			if (temp_threshold > MAXTHRESHOLD)
			    temp_threshold = MAXTHRESHOLD;
			if (temp_threshold < 1)
			    temp_threshold = 1;
		        return TRUE ;

		    case IDOK:
			gManp->subtype = temp;
/*			
			if (temp_step < 0)
			    FibSteps = threshold;
			else
			    FibSteps = temp_step;
*/			    
			if (temp_delay >= 0 && temp_delay < 2000)
			    delay = temp_delay;
			else
			    delay = 100;
			if (temp_SpiralNo >= 0 && temp_SpiralNo < 2000)
			    SpiralNo = temp_SpiralNo;
			else
			    SpiralNo = 55;
			gManp->threshold = temp_threshold;
			if (gManp->threshold >= MAXTHRESHOLD)
			    gManp->threshold = MAXTHRESHOLD;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }
