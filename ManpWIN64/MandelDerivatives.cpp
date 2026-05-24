/*
    MandelDerivatives.CPP a module for the per pixel calculations of fractals. 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include	<math.h>
#include	"manp.h"
#include	"resource.h"
#include	"Fract.h"
#include	"fractalp.h"

static	int	MandelDerivPtr = 1, MandelDerivNum = 0;

//extern	long	iteration;			// globals for speed for now at least

/**************************************************************************
    Some simple processing
**************************************************************************/

int	setup_MandelDeriv(void)
    {
    if (!MandelDerivNum)	    // we'd better count how many records we have
	{
	while (MandelDerivSpecific[MandelDerivNum].name)
	    MandelDerivNum++;
	}
    return 0;
    }

void	LoadMandelDerivParams(void) 
    {
    // here is where we can do some specific updates to individual Mandelbrot Derivative fractals
    int	i;

    for (i = 0; i < NUMALTERNATIVEPARAM; i++)
	gManp->param[i] = MandelDerivSpecific[gManp->subtype].paramvalue[i];
    gManp->rqlim = MandelDerivSpecific[gManp->subtype].rqlim;
    }

/**************************************************************************
    Show Mandelbrot Derivatives Fractal
**************************************************************************/

INT_PTR CALLBACK SelectMandelDerivDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

{
static	int	i;
static	int	index;
//		HWND    hCtrl; 

    switch (message) 
	{
        case WM_INITDIALOG:
            SetDlgItemText(hDlg, ID_LISTTITLE, 	"Mandelbrot Derivatives");
            for (i = 0; i < MandelDerivNum; i++) 
                SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) MandelDerivSpecific[i].name);
            SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)MandelDerivPtr, 0L);
            return TRUE;

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
//	    switch (wParam)
		{
                case IDOK:
//okay:           
                    index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
                    if (index == LB_ERR) 
			{
                        MessageBox(hDlg, "No Choice selected",
                            "Select From a List", MB_OK | MB_ICONEXCLAMATION);
                        break;
                        }
                    MandelDerivPtr = index;
//		    PreviousLsys_ptr = index;
//		    strcpy(lsys_type, lptr[lsys_ptr]);
		    gManp->subtype = MandelDerivPtr;
                    EndDialog(hDlg, TRUE);
                    return TRUE;
                  
                case IDCANCEL:
                    MandelDerivPtr = 1;
                    EndDialog(hDlg, FALSE);
                    return FALSE;
                    
                case IDM_LSYSTEM:
                    switch (HIWORD(wParam) & 0x0003) 
//                    switch (LOWORD(lParam)) 
			{
                        case LBN_SELCHANGE:
                            index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
                            if (index == LB_ERR)
                                break;
                            break;
                         
                       case LBN_DBLCLK:
//			    goto okay;
			    index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
			    if (index == LB_ERR) 
				{
				MessageBox(hDlg, "No Choice selected",
				    "Select From a List", MB_OK | MB_ICONEXCLAMATION);
				break;
				}
			    gManp->subtype = MandelDerivPtr = index;
//			    lsys_ptr = index;
//			    strcpy(lsys_type, lptr[lsys_ptr]);
			    EndDialog(hDlg, TRUE);
			    return TRUE;
                  
			}
	return TRUE;
	}
    }
return FALSE;
}

