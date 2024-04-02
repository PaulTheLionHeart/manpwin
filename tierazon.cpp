////////////////////////////////////////////////////////////////
// Tierazon.cpp a module for the per pixel calculations of Tierazon fractals. 
//
// By: stephen c. ferguson
// http://home1.gte.net/itriazon/
// email: itriazon@gte.net
//
/////////////////////////////////////////////////////
// Updated in Microsoft Visual 'C++' by Paul de Leeuw.
// These are listed in numerical order of the functions.
// Note that CSqr(z) is used in place of z*z and CCube(z) in place of z*z*z for speed
/////////////////////////////////////////////////////



//Restore once we get rid of functions


#include	<math.h>
#include	"manp.h"
#include	"resource.h"
#include	"Fract.h"
#include	"fractalp.h"

static	int	TierazonPtr = 1, TierazonNum = 0;

extern	long	iteration;		// globals for speed for now at least

extern	int	subtype;
extern	double	param[];
extern	double	rqlim;			// bailout level
extern	int	period_level;		// 0 for no periodicity checking

extern	CFract	Fractal;		// current fractal stuff

/**************************************************************************
	General Tierazon Fractal
***************************************************************************/

int	setup_Tierazon(void)
    {
    if (!TierazonNum)	    // we'd better count how many records we have
	{
	while (TierazonSpecific[TierazonNum].name)
	    TierazonNum++;
	}
    return 0;
    }

void	LoadTierazonParams(void) 
    {
    // here is where we can do some specific updates to individual Tierazon fractals
    int	i;

    switch (subtype)
	{
	case 10:					// z=z*z*z+c
	case 11:					// z=z*z*z*z+c
	case 12:					// z=z*z*z*z*z+c
	case 13:					// z=z*z*z*z*z*z+c
	case 26:					// Newton/Mandel, 5th order Newton Mset
	case 27:					// Newton/Mandel, 7th order Newton Mset
	case 28:					// Newton/Mandel, 9th order Newton Mset
	case 29:					// Newton/Mandel, 13th order Newton Mset
	case 30:					// Newton/Mandel, 8th order Newton Mset
	case 31:					// Newton/Mandel, Newton Diamond
	case 32:					// Newton/Mandel, Newton Pentagon
	case 33:					// Newton/Mandel, Newton Hexagon
	case 34:					// Newton/Mandel, Newton Octagon
	case 35:					// Newton/Mandel, 9th order Newton Flower
	case 52:					// Newton/Mandel, 8th order Newton flower
	case 53:					// Newton/Mandel, 6th order Newton Mset
	case 54:					// More Newton Msets, 15th order Newton Mset flower
	case 55:					// More Newton Msets, 4th order Newton's apple
	case 56:					// More Newton Msets, 25th order Newton Mset flower
	case 57:					// More Newton Msets, 38th order Newton Mset flower
	case 58:					// More Newton Msets, 50th order Newton Mset flower
	case 59:					// More Newton Msets, 5th order Newton Mset
	case 84:					// Quartets, t=0; z1=z; z=z*z*z*z-t*t*t*t+c; t=z1
	case 85:					// Quartets, z2=z; z=(z^4)+c; c=z2
	case 96:					// Quartets, z1=z; z=z*z*z*z+z2/2+c;; z2=z1
	case 106:					// More Fractals, 5th order N/Mset
	case 120:					// Flarium 07, Polynomial: z = (z*z+c)^(cn+c)
	case 132:					// Flarium 27, Polynomial: z=z*z*(cn+z)/(cn+z+c)+c
	case 165:					// Flarium 67-69, Newton Variations: z = ((z-(((z^n)-1)/(n*(z^(n-1)))))^2)*c
	case 172:					// Flarium 112-116, Polynomials: z=z^n*c+z*c; Dragon curve variations
	case 177:					// Flarium 145, Polynomial: z=z^2+c [Jaenisch method]
	    Fractal.NumParam = 3;
	    Fractal.ParamName[0] = "Polynomial Degree (>= 2)";
	    Fractal.ParamName[1] = "Real Perturbation of Z(0)";
	    Fractal.ParamName[2] = "Imaginary Perturbation of Z(0)";
	    break;
	}
    if (subtype == 133 || subtype == 177)	// Flarium 28 and 245 don't work well with periodicy checking
	period_level = FALSE;			// no periodicity checking for Tierazon fractals
    for (i = 0; i < 4; i++)
	param[i] = TierazonSpecific[subtype].paramvalue[i];
    rqlim = TierazonSpecific[subtype].rqlim;
    }

int	init_Tierazon(void) 
    {
    return (TierazonSpecific[subtype].per_pixel());
    }

int	do_Tierazon(void) 

    {
    return (TierazonSpecific[subtype].calctype());
    }

/**************************************************************************
	Show Tierazon Fractal
**************************************************************************/

DLGPROC FAR PASCAL SelectTierazonDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

{
static	int	i;
static	int	index;
//		HWND    hCtrl; 

    switch (message) 
	{
        case WM_INITDIALOG:
/*
//	    cycleflag = FALSE;
	    hCtrl = GetDlgItem (hDlg, IDC_COLOURINGOPTION);
	    ShowWindow (hCtrl, SW_HIDE);
	    hCtrl = GetDlgItem (hDlg, IDC_COLOURINGLABEL);
	    ShowWindow (hCtrl, SW_HIDE);
	    hCtrl = GetDlgItem (hDlg, IDC_X);
	    ShowWindow (hCtrl, SW_HIDE);
	    hCtrl = GetDlgItem (hDlg, IDC_Y);
	    ShowWindow (hCtrl, SW_HIDE);
	    hCtrl = GetDlgItem (hDlg, IDC_Z);
	    ShowWindow (hCtrl, SW_HIDE);
	    hCtrl = GetDlgItem (hDlg, IDC_DIM_X);
	    ShowWindow (hCtrl, SW_HIDE);
	    hCtrl = GetDlgItem (hDlg, IDC_DIM_Y);
	    ShowWindow (hCtrl, SW_HIDE);
	    hCtrl = GetDlgItem (hDlg, IDC_DIM_Z);
	    ShowWindow (hCtrl, SW_HIDE);
	    hCtrl = GetDlgItem (hDlg, IDC_LINES);
	    ShowWindow (hCtrl, SW_HIDE);
*/
            SetDlgItemText(hDlg, ID_LISTTITLE, 	"Tierazon");
            for (i = 0; i < TierazonNum; i++) 
                SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) TierazonSpecific[i].name);
            SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)TierazonPtr, 0L);
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
                    TierazonPtr = index;
//		    PreviousLsys_ptr = index;
//		    strcpy(lsys_type, lptr[lsys_ptr]);
		    subtype = TierazonPtr;
                    EndDialog(hDlg, TRUE);
                    return ((DLGPROC)TRUE);
                  
                case IDCANCEL:
                    TierazonPtr = 1;
                    EndDialog(hDlg, FALSE);
                    return (FALSE);
                    
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
			    subtype = TierazonPtr = index;
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

