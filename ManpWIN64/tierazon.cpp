/*
    Tierazon.cpp - a module for the per pixel calculations of Tierazon fractals. 

    By: stephen c. ferguson
    http://home1.gte.net/itriazon/
    email: itriazon@gte.net

    Updated in Microsoft Visual 'C++' by Paul de Leeuw.
    These are listed in numerical order of the functions.
    Note that CSqr(z) is used in place of z*z and CCube(z) in place of z*z*z for speed

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<math.h>
#include	"manp.h"
#include	"resource.h"
#include	"Fract.h"
#include	"fractalp.h"

static	int	TierazonPtr = 1, TierazonNum = 0;

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

    if (gManp->subtype == 133 || gManp->subtype == 177)	// Flarium 28 and 245 don't work well with periodicy checking
	gManp->period_level = FALSE;			// no periodicity checking for Tierazon fractals
    for (i = 0; i < 4; i++)
	gManp->param[i] = TierazonSpecific[gManp->subtype].paramvalue[i];
    gManp->rqlim = TierazonSpecific[gManp->subtype].rqlim;
    }

int	init_Tierazon(void) 
    {
    return (TierazonSpecific[gManp->subtype].per_pixel());
    }

int	do_Tierazon(void) 

    {
    return (TierazonSpecific[gManp->subtype].calctype());
    }

/**************************************************************************
	Show Tierazon Fractal
**************************************************************************/

INT_PTR CALLBACK SelectTierazonDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static	int	i;
    static	int	index;
//		HWND    hCtrl; 

    switch (message) 
	{
        case WM_INITDIALOG:
            SetDlgItemText(hDlg, ID_LISTTITLE, 	"Tierazon");
            for (i = 0; i < TierazonNum; i++) 
                SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) TierazonSpecific[i].name);
            SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)TierazonPtr, 0L);
            return (TRUE);

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
		    gManp->subtype = TierazonPtr;
                    EndDialog(hDlg, TRUE);
                    return (TRUE);
                  
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
			    gManp->subtype = TierazonPtr = index;
//			    lsys_ptr = index;
//			    strcpy(lsys_type, lptr[lsys_ptr]);
			    EndDialog(hDlg, TRUE);
			    return (TRUE);
                  
			}
	return (TRUE);
	}
    }
return (FALSE);
}

