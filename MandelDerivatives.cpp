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

extern	long	iteration;			// globals for speed for now at least

extern	int	subtype;
extern	double	param[];
extern	double	rqlim;			// bailout level

extern	CFract	Fractal;		// current fractal stuff

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

    switch (subtype)
	{
	case 2:
	case 4:
	case 7:
	    Fractal.NumParam = 3;
	    Fractal.ParamName[0] = "Polynomial Degree (>= 2)";
	    Fractal.ParamName[1] = "Real Perturbation of Z(0)";
	    Fractal.ParamName[2] = "Imaginary Perturbation of Z(0)";
	    break;
	case 6:
	    Fractal.NumParam = 4;
	    Fractal.ParamName[0] = "Polynomial Degree (>= 2)";
	    Fractal.ParamName[1] = "Real Perturbation of Z(0)";
	    Fractal.ParamName[2] = "Imaginary Perturbation of Z(0)";
	    Fractal.ParamName[3] = "Rotated? (Only for degree > 2, 1=yes 0=no)";
	    break;
	case 55:
	case 56:
	    Fractal.NumParam = 3;
	    Fractal.ParamName[0] = (subtype == 55) ? "SimonBrot Order (>= 2)" : "SimonBrot2 Order (>= 2)";
	    Fractal.ParamName[1] = "Real Perturbation of Z(0)";
	    Fractal.ParamName[2] = "Imaginary Perturbation of Z(0)";
	    break;
	}

//    if (subtype == 133 || subtype == 177)	// Flarium 28 and 245 don't work well with periodicy checking
//	period_level = FALSE;			// no periodicity checking for Tierazon fractals

    for (i = 0; i < 4; i++)
	param[i] = MandelDerivSpecific[subtype].paramvalue[i];
    rqlim = MandelDerivSpecific[subtype].rqlim;
    }

/**************************************************************************
    Show Mandelbrot Derivatives Fractal
**************************************************************************/

DLGPROC FAR PASCAL SelectMandelDerivDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

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
            return ((DLGPROC)TRUE);

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
		    subtype = MandelDerivPtr;
                    EndDialog(hDlg, TRUE);
                    return ((DLGPROC)TRUE);
                  
                case IDCANCEL:
                    MandelDerivPtr = 1;
                    EndDialog(hDlg, FALSE);
                    return ((DLGPROC)FALSE);
                    
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
			    subtype = MandelDerivPtr = index;
//			    lsys_ptr = index;
//			    strcpy(lsys_type, lptr[lsys_ptr]);
			    EndDialog(hDlg, TRUE);
			    return ((DLGPROC)TRUE);
                  
			}
	return ((DLGPROC)TRUE);
	}
    }
return ((DLGPROC)FALSE);
}

