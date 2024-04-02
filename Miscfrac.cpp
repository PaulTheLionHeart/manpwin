/*
Miscellaneous fractal-specific code (formerly in CALCFRAC.C)
*/

#include <windows.h>
#include "Complex.h"
#include "Pixel.h"
#include "resource.h"

extern	BYTE	juliaflag;		//* Julia implementation of fractal 
extern	Complex	z, q, j;
extern	double	rqlim;			// bailout level
extern	WORD	type;			// M=mand, J=Julia 1,2,4-> etc
extern	int	subtype;		// A - E

extern	int	time_to_zoom;			// time to zoom in or out?
extern	int	time_to_restart;		// time to restart?
extern	int	time_to_reinit;			// time to reinitialize?
extern	int	time_to_quit;			// time to quit?
extern	int	time_to_load;			// time to load file?
extern	BOOL	bTrack;				// TRUE if user is selecting a region

extern	CPixel	Pix;

static	Complex	sqr;
static	double	real_imag;		// z_real * z_imag 

int	row, col;
long	color;
int	overflow;
int	save_release;

extern	int user_data(HWND hwnd);

/***************** Some small useful ones ********************/

void FPUsincos(double *Angle, double *Sin, double *Cos)
    {
    *Sin = sin(*Angle);
    *Cos = cos(*Angle);
    }

Complex ComplexPower(Complex x, Complex y)
    {
    Complex z;
    z = x ^ y;
    return z;
    }

int	NullSetup(void)				// sometimes we just don't want to do anything 
    {
    return 1;
    }

/**************************************************************************
    Initialise Mandelbrot
***************************************************************************/

int	init_mandel_df(void)

    {
    if (!juliaflag)
	{
	z.x = q.x + param[0];
	z.y = q.y + param[1];
	}
    sqr = 0;
    real_imag = 0.0;
    return 0;
    }

/**************************************************************************
	Determine count before 'Z' becomes unstable float version
***************************************************************************/

int	do_mandel_df(void)

    {
    sqr.x = z.x * z.x;
    sqr.y = z.y * z.y;
    real_imag = z.x * z.y;
    z.x = q.x + sqr.x - sqr.y;
    z.y = q.y + real_imag + real_imag;
    return ((sqr.x + sqr.y) >= rqlim);
    }

/************ standalone engine for "bifurcation" types ***************/

/***************************************************************/
/* The following code now forms a generalised Fractal Engine   */
/* for Bifurcation fractal typeS.  By rights it now belongs in */
/* CALCFRACT.C, but it's easier for me to leave it here !      */

/* Original code by Phil Wilson, hacked around by Kev Allen.   */

/* Besides generalisation, enhancements include Periodicity    */
/* Checking during the plotting phase (AND halfway through the */
/* filter cycle, if possible, to halve calc times), quicker    */
/* floating-point calculations for the standard Verhulst type, */
/* and new bifurcation types (integer bifurcation, f.p & int   */
/* biflambda - the real equivalent of complex Lambda sets -    */
/* and f.p renditions of bifurcations of r*sin(Pi*p), which    */
/* spurred Mitchel Feigenbaum on to discover his Number).      */

/* To add further types, extend the fractalspecific[] array in */
/* usual way, with Bifurcation as the engine, and the name of  */
/* the routine that calculates the next bifurcation generation */
/* as the "orbitcalc" routine in the fractalspecific[] entry.  */

/* Bifurcation "orbitcalc" routines get called once per screen */
/* pixel column.  They should calculate the next generation    */
/* from the doubles Rate & Population (or the longs lRate &    */
/* lPopulation if they use integer math), placing the result   */
/* back in Population (or lPopulation).  They should return 0  */
/* if all is ok, or any non-zero value if calculation bailout  */
/* is desirable (eg in case of errors, or the series tending   */
/* to infinity).		Have fun !		       */
/***************************************************************/

int	bifurcation(void)
    {
    return	Pix.bifurcation(user_data);
    }

/**************************************************************************
	Get a key or mouse click and process
**************************************************************************/

int user_data(HWND hwnd)

    {
    MSG msg;

    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}

    if (!bTrack && time_to_reinit + time_to_restart + time_to_quit + time_to_zoom + time_to_load)
	return(-1);					  // don't do this if mouse-button is down
    return 0;
    }

/**************************************************************************
	Dialog Control for Plasma Type
**************************************************************************/

DLGPROC FAR PASCAL PlasmaDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static	HANDLE	hCursor;
     char		s[100];
//     BOOL		bTrans ;
     static	UINT	tempPoints; 
     static     char	temp[20];
//     static     char	temp;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        sprintf(s, "%2.2f", param[0]);
		SetDlgItemText(hDlg, IDC_PLASMA_GRAIN, s);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_PLASMA_GRAIN:
			GetDlgItemText(hDlg, IDC_PLASMA_GRAIN, temp, 20);
		        return (DLGPROC)TRUE ;

		    case IDOK:
			sscanf(temp, "%lf", &param[0]);
			if (param[0] < 0.1 || param[0] > 50.0)
			    param[0] = 2.0;
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

/**************************************************************************
	Dialog Control for FrothyBasin Type
**************************************************************************/

DLGPROC FAR PASCAL FrothDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
//     HWND		hCtrl ;
     static	HANDLE	hCursor;
//     short		nCtrlID, filterSum, i, divisor; 
     BOOL		bTrans ;
     static	UINT	tempShading; 
     static     UINT	tempAttractors;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_3_OR_6, (int)param[0], TRUE);
		SetDlgItemInt(hDlg, IDC_SHADING, (int)param[1], TRUE);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_3_OR_6:
			tempAttractors = GetDlgItemInt(hDlg, IDC_3_OR_6, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_SHADING:
			tempShading = GetDlgItemInt(hDlg, IDC_SHADING, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDOK:
			param[0] = (double)tempAttractors;
			if (param[0] != 6.0)		// only 3 or 6 allowed
			    param[0] = 3.0;
			param[1] = (double)tempShading;
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

/**************************************************************************
	Dialog Control for Cellular Automata Type
**************************************************************************/

DLGPROC FAR PASCAL CellularDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
//     HWND		hCtrl ;
     static	HANDLE	hCursor;
//     short		nCtrlID, filterSum, i, divisor; 
     char		s[100];
     BOOL		bTrans ;
     static	UINT	tempInit; 
     static     char	temp[20];
     static     UINT	tempType;
     static     UINT	tempStart;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        sprintf(s, "%2.0f", param[1]);
		SetDlgItemText(hDlg, IDC_CELL_RULE, s);
		SetDlgItemInt(hDlg, IDC_INIT_STRING, (int)param[0], TRUE);
		SetDlgItemInt(hDlg, IDC_CELL_TYPE, (int)param[2], TRUE);
		SetDlgItemInt(hDlg, IDC_START_ROW, (int)param[3], TRUE);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_CELL_RULE:
			GetDlgItemText(hDlg, IDC_CELL_RULE, temp, 20);
		        return (DLGPROC)TRUE ;

		    case IDC_INIT_STRING:
			tempInit = GetDlgItemInt(hDlg, IDC_INIT_STRING, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_START_ROW:
			tempStart = GetDlgItemInt(hDlg, IDC_START_ROW, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_CELL_TYPE:
			tempType = GetDlgItemInt(hDlg, IDC_CELL_TYPE, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDOK:
			sscanf(temp, "%lf", &param[1]);
			param[0] = (double)tempInit;
			param[2] = (double)tempType;
			param[3] = (double)tempStart;
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
	  break;
	  }
      return (DLGPROC)FALSE ;
      }

