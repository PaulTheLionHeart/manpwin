/*
    TierazonDLG.cpp - Dialogue functions for Tierazon fractals.

    By: stephen c. ferguson
    http://home1.gte.net/itriazon/
    email: itriazon@gte.net

    Updated in Microsoft Visual 'C++' by Paul de Leeuw.
    These are listed in numerical order of the functions.
    Note that CSqr(z) is used in place of z*z and CCube(z) in place of z*z*z for speed

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include "Complex.h"
#include "resource.h"
#include "filter.h"
#include "manpwin.h"
#include "manp.h"
#include "Dib.h"
#include "colour.h"

static	int	ColourPtr = 1, ColourNum = 0;
static	int	FilterPtr = 1, FilterNum = 0;

/**************************************************************************
	Count the number of Tierazon Filters
***************************************************************************/

int	SetupTierazonFilters(void)
    {
    if (!FilterNum)	    // we'd better count how many records we have
	{
	while (TierazonFilter[FilterNum].name)
	    FilterNum++;
	}
    return 0;
    }

/**************************************************************************
	Count the number of Tierazon Colour Methods
***************************************************************************/

int	SetupTierazonColours(void)
    {
    if (!ColourNum)	    // we'd better count how many records we have
	{
	while (TierazonColour[ColourNum].name)
	    ColourNum++;
	}
    return 0;
    }

/**************************************************************************
	Show Tierazon Colour
**************************************************************************/

    INT_PTR CALLBACK SelectColourDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static	int	i;
    static	int	index;
//		HWND    hCtrl; 

    ColourPtr = gManp->ColourMethod;

    switch (message) 
	{
	gManp->RGBFilter = FALSE;
	case WM_INITDIALOG:
	    SetupTierazonFilters();				// count number of filter entries
	    SetupTierazonColours();				// count number of colour method entries
	    SetDlgItemText(hDlg, ID_LISTTITLE, 	"Tierazon Colour");
            for (i = 0; i < ColourNum; i++) 
                SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) TierazonColour[i].name);
            SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)ColourPtr, 0L);
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
                    ColourPtr = index;
		    gManp->ColourMethod = ColourPtr = index;
		    gManp->OutsideMethod = ColourPtr + TIERAZONCOLOURS;
		    gManp->oldcalcmode = gManp->calcmode;
		    							// these plotting modes get the iteration count lost in RGB filters
		    if (gManp->calcmode == 'G' || gManp->calcmode == 'T' || gManp->calcmode == 'B' && TierazonColour[gManp->ColourMethod].calctype != ' ')
			gManp->calcmode = TierazonColour[gManp->ColourMethod].calctype;		// use "simple" plotting modes for filters that change RGB values
		    gManp->RGBFilter = (TierazonColour[gManp->ColourMethod].rgb) ? TRUE : FALSE;
		    gManp->PlotType = (gManp->RGBFilter) ? FILTERPLOT : NOSYM;
		    EndDialog(hDlg, TRUE);
                    return TRUE;
                  
                case IDCANCEL:
                    ColourPtr = 1;
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
			    gManp->ColourMethod = ColourPtr = index;
			    gManp->OutsideMethod = ColourPtr + TIERAZONCOLOURS;
			    gManp->oldcalcmode = gManp->calcmode;
		    							// these plotting modes get the iteration count lost in RGB filters
			    if (gManp->calcmode == 'G' || gManp->calcmode == 'T' || gManp->calcmode == 'B' && TierazonColour[gManp->ColourMethod].calctype != ' ')
				gManp->calcmode = TierazonColour[gManp->ColourMethod].calctype;		// use "simple" plotting modes for filters that change RGB values
			    gManp->RGBFilter = (TierazonColour[gManp->ColourMethod].rgb) ? TRUE : FALSE;
			    gManp->PlotType = (gManp->RGBFilter) ? FILTERPLOT : NOSYM;
			    EndDialog(hDlg, TRUE);
			    return TRUE;
			}
	    return TRUE;
	    }
	}
    return FALSE;
    }

    /**************************************************************************
	    Show Tierazon Filter
    **************************************************************************/

    INT_PTR CALLBACK SelectFilterDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

	{
	static	int	i;
	static	int	index;
	//		HWND    hCtrl; 

	FilterPtr = gManp->FilterType;
	    
	switch (message)
	    {
	    gManp->RGBFilter = FALSE;
	    case WM_INITDIALOG:
		SetupTierazonFilters();				// count number of filter entries
		SetDlgItemText(hDlg, ID_LISTTITLE, "Tierazon Filter");
		for (i = 0; i < FilterNum; i++)
		    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)TierazonFilter[i].name);
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)FilterPtr, 0L);
		return TRUE;

	    case WM_COMMAND:
		switch ((int)LOWORD(wParam))
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
			FilterPtr = index;
			//		    PreviousLsys_ptr = index;
			//		    strcpy(lsys_type, lptr[lsys_ptr]);
			gManp->FilterType = FilterPtr = index;
			gManp->OutsideMethod = FilterPtr + TIERAZONFILTERS;
			gManp->oldcalcmode = gManp->calcmode;
			// these plotting modes get the iteration count lost in RGB filters
			if (gManp->calcmode == 'G' || gManp->calcmode == 'T' || gManp->calcmode == 'B' && TierazonFilter[gManp->FilterType].calctype != ' ')
			    gManp->calcmode = TierazonFilter[gManp->FilterType].calctype;		// use "simple" plotting modes for filters that change RGB values
			gManp->RGBFilter = (TierazonFilter[gManp->FilterType].rgb) ? TRUE : FALSE;
			gManp->PlotType = (gManp->RGBFilter) ? FILTERPLOT : NOSYM;
			EndDialog(hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			FilterPtr = 1;
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
				gManp->FilterType = FilterPtr = index;
				gManp->OutsideMethod = FilterPtr + TIERAZONFILTERS;
				gManp->oldcalcmode = gManp->calcmode;
				// these plotting modes get the iteration count lost in RGB filters
				if (gManp->calcmode == 'G' || gManp->calcmode == 'T' || gManp->calcmode == 'B' && TierazonFilter[gManp->FilterType].calctype != ' ')
				    gManp->calcmode = TierazonFilter[gManp->FilterType].calctype;		// use "simple" plotting modes for filters that change RGB values
				gManp->RGBFilter = (TierazonFilter[gManp->FilterType].rgb) ? TRUE : FALSE;
				gManp->PlotType = (gManp->RGBFilter) ? FILTERPLOT : NOSYM;
    				EndDialog(hDlg, TRUE);
				return TRUE;

			    }
			return TRUE;
		    }
	    }
	return (FALSE);
	}

    /**************************************************************************
	    Show Tierazon Filter
    **************************************************************************/

    INT_PTR CALLBACK SelectFDOptionDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)

	{
	static	int	i;
	static	int	index;

	switch (message)
	    {
	    case WM_INITDIALOG:
		SetDlgItemText(hDlg, ID_LISTTITLE, "Tierazon Filter FD-Options");
		for (i = 0; i < MAXFDOPTIONS; i++)
		    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)TierazonFDOptions[i]);
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)gManp->nFDOption - 1, 0L);
		return TRUE;

	    case WM_COMMAND:
		switch ((int)LOWORD(wParam))
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
			gManp->nFDOption = index + 1;
			gManp->TZfilter.nFDOption = gManp->nFDOption;
			EndDialog(hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
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
				gManp->nFDOption = index + 1;
				gManp->TZfilter.nFDOption = gManp->nFDOption;
				EndDialog(hDlg, TRUE);
				return TRUE;

			    }
			return TRUE;
		    }
	    }
	return FALSE;
	}

