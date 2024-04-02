/*
    FIBONACI.CPP a module for Fibonacci Spirals
    
    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "manp.h"
#include "colour.h"
#include "resource.h"
#include "menu.h"
#include "Plot.h"

extern	int	user_data(HWND);

extern	long	threshold;
extern	int	subtype;		/* Q = quad mand, B = bifurcation */
extern	WORD	type;			/* M=mand, J=Julia 1,2,4-> */
extern	int	row, col;
extern	int	xdots, ydots;
extern	CPlot	Plot;		// image plotting routines 

extern	PAINTSTRUCT 	ps;
extern	HDC		hdcMem;		// load picture into memory
extern	RECT 		r;
extern	HWND		PixelHwnd;	// pointer to handle for pixel updating
extern	WORD		delay;

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

    switch (subtype)
	{ 
	case 'I':				// linear
	    radius = (double)temp / (double)threshold;
	    break;
	case 'L':				// log
	    radius = log((double)temp) / log((double)threshold);
	    break;
	case 'S':				// square
	    radius = (double)temp * (double)temp / ((double)threshold * (double)threshold);
	    break;
	case 'E':				// exponential
	    radius = (1.0 / ((double)threshold)) / (1.0 / ((double)temp));
	    break;
	case 'R':				// square root
	    radius = sqrt((double)temp) / sqrt((double)threshold);
	    break;
	default:				// uninitialised - assume linear
	    radius = (double)temp / (double)threshold;
	    break;
	}
                            
    i = (int)(radius * cos(angle / DEG2RAD) * (ydots / 2 - 1)) + xdots / 2;
    j = (int)(radius * sin(angle / DEG2RAD) * (ydots / 2 - 1)) + ydots / 2;
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
	for (j = 0; j < threshold / order; ++j)
	    if ((j + 1) * order + i < threshold)
		Plot.genline(FibArray[j * order + i].x, FibArray[j * order + i].y, 
		    FibArray[(j + 1) * order + i].x, FibArray[(j + 1) * order + i].y, i % 14 + 1);
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

    if (threshold >= FIBMAX)
	threshold = FIBMAX - 1;
    for (j = 0; j < threshold; ++j)
	{
	angle += step;
	if (angle > 360.0)
	    angle -= 360.0;
	PlotSpiral(angle, j);
	}


    for (j = 0; j < threshold; ++j)
	{
	Plot.PlotPoint(FibArray[j].x, FibArray[j].y, FibArray[j].c);
	if (FibDelay(PixelHwnd, delay) < 0)
	    return -1;
	}

    if (Spirals(PixelHwnd, SpiralNo, delay, Plot) < 0)
	return -1;
    return 0;
    }                                                 


/**************************************************************************
	Dialog Control for Cubic Fractal Sub Types
**************************************************************************/

DLGPROC FAR PASCAL FibTypeDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
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
	        temp = subtype;
	        switch (subtype)
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
		SetDlgItemInt(hDlg, IDM_THRESHOLD, threshold, TRUE);
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
		        return (DLGPROC)TRUE ;
/*		        
		    case IDC_FIBSTEPS:
			temp_step = GetDlgItemInt(hDlg, IDC_FIBSTEPS, &bTrans, TRUE);
		        return TRUE ;
*/
		    case IDC_FIBTIME:
			temp_delay = GetDlgItemInt(hDlg, IDC_FIBTIME, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_FIBSPIRALCOUNT:
			temp_SpiralNo = GetDlgItemInt(hDlg, IDC_FIBSPIRALCOUNT, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDM_THRESHOLD:
			temp_threshold = GetDlgItemInt(hDlg, IDM_THRESHOLD, &bTrans, TRUE);
			if (temp_threshold > MAXTHRESHOLD)
			    temp_threshold = MAXTHRESHOLD;
			if (temp_threshold < 1)
			    temp_threshold = 1;
		        return (DLGPROC)TRUE ;

		    case IDOK:
			subtype = temp;
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
			threshold = temp_threshold;
			if (threshold >= MAXTHRESHOLD)
			    threshold = MAXTHRESHOLD;
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
		   break;
	    }
      return FALSE ;
      }
