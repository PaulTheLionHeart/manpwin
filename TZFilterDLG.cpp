/////////////////////////////////////////////////
// rsxcolor.c
/////////////////////////////////////////////////
//
// By: stephen c. ferguson
// http://home1.gte.net/itriazon/
// email: itriazon@gte.net
//
/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


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

int	FilterType = 1;				// data for Tierazon filters
double	dStrands;
BOOL	RGBFilter;				// If true, we use the plotting routine for RGB filters (no plotting of iteration)
int	ColourMethod = 1;			// Tierazon colour methods
BOOL	UseCurrentPalette = TRUE;		// do we use the ManpWIN palette? If false, generate internal filter palette

	int	nFDOption = 4;			// Fractal Dimension option for Tierazon filters
extern	int	InsideMethod;			// inside filter methods
//extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts
extern	BYTE	calcmode;
extern	BYTE	oldcalcmode;			// store values during 3D transformations, filters etc
extern	CDib	Dib;
extern	CTZfilter	TZfilter;		// Tierazon filters
extern	int	PlotType;
extern	int	height, xdots, ydots, width, bits_per_pixel;

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

    DLGPROC FAR PASCAL SelectColourDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

    {
    static	int	i;
    static	int	index;
//		HWND    hCtrl; 

    ColourPtr = ColourMethod;

    switch (message) 
	{
	RGBFilter = FALSE;
	case WM_INITDIALOG:
	    SetupTierazonFilters();				// count number of filter entries
	    SetupTierazonColours();				// count number of colour method entries
	    SetDlgItemText(hDlg, ID_LISTTITLE, 	"Tierazon Colour");
            for (i = 0; i < ColourNum; i++) 
                SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) TierazonColour[i].name);
            SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)ColourPtr, 0L);
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
                    ColourPtr = index;
		    ColourMethod = ColourPtr = index;
		    InsideMethod = ColourPtr + TIERAZONCOLOURS;
		    oldcalcmode = calcmode;
		    							// these plotting modes get the iteration count lost in RGB filters
		    if (calcmode == 'G' || calcmode == 'T' || calcmode == 'B' && TierazonColour[ColourMethod].calctype != ' ')
			calcmode = TierazonColour[ColourMethod].calctype;		// use "simple" plotting modes for filters that change RGB values
		    RGBFilter = (TierazonColour[ColourMethod].rgb) ? TRUE : FALSE;
		    PlotType = (RGBFilter) ? FILTERPLOT : NOSYM;
		    EndDialog(hDlg, TRUE);
                    return ((DLGPROC)TRUE);
                  
                case IDCANCEL:
                    ColourPtr = 1;
                    EndDialog(hDlg, FALSE);
                    return (DLGPROC)(FALSE);
                    
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
			    ColourMethod = ColourPtr = index;
			    InsideMethod = ColourPtr + TIERAZONCOLOURS;
			    oldcalcmode = calcmode;
		    							// these plotting modes get the iteration count lost in RGB filters
			    if (calcmode == 'G' || calcmode == 'T' || calcmode == 'B' && TierazonColour[ColourMethod].calctype != ' ')
				calcmode = TierazonColour[ColourMethod].calctype;		// use "simple" plotting modes for filters that change RGB values
			    RGBFilter = (TierazonColour[ColourMethod].rgb) ? TRUE : FALSE;
			    PlotType = (RGBFilter) ? FILTERPLOT : NOSYM;
			    EndDialog(hDlg, TRUE);
			    return ((DLGPROC)TRUE);
			}
	    return ((DLGPROC)TRUE);
	    }
	}
    return ((DLGPROC)FALSE);
    }

    /**************************************************************************
	    Show Tierazon Filter
    **************************************************************************/

    DLGPROC FAR PASCAL SelectFilterDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

	{
	static	int	i;
	static	int	index;
	//		HWND    hCtrl; 

	FilterPtr = FilterType;
	    
	switch (message)
	    {
	    RGBFilter = FALSE;
	    case WM_INITDIALOG:
		SetupTierazonFilters();				// count number of filter entries
		SetDlgItemText(hDlg, ID_LISTTITLE, "Tierazon Filter");
		for (i = 0; i < FilterNum; i++)
		    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)TierazonFilter[i].name);
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)FilterPtr, 0L);
		return ((DLGPROC)TRUE);

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
			FilterType = FilterPtr = index;
			InsideMethod = FilterPtr + TIERAZONFILTERS;
			oldcalcmode = calcmode;
			// these plotting modes get the iteration count lost in RGB filters
			if (calcmode == 'G' || calcmode == 'T' || calcmode == 'B' && TierazonFilter[FilterType].calctype != ' ')
			    calcmode = TierazonFilter[FilterType].calctype;		// use "simple" plotting modes for filters that change RGB values
			RGBFilter = (TierazonFilter[FilterType].rgb) ? TRUE : FALSE;
			PlotType = (RGBFilter) ? FILTERPLOT : NOSYM;
			EndDialog(hDlg, TRUE);
			return ((DLGPROC)TRUE);

		    case IDCANCEL:
			FilterPtr = 1;
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
				FilterType = FilterPtr = index;
				InsideMethod = FilterPtr + TIERAZONFILTERS;
				oldcalcmode = calcmode;
				// these plotting modes get the iteration count lost in RGB filters
				if (calcmode == 'G' || calcmode == 'T' || calcmode == 'B' && TierazonFilter[FilterType].calctype != ' ')
				    calcmode = TierazonFilter[FilterType].calctype;		// use "simple" plotting modes for filters that change RGB values
				RGBFilter = (TierazonFilter[FilterType].rgb) ? TRUE : FALSE;
				PlotType = (RGBFilter) ? FILTERPLOT : NOSYM;
    				EndDialog(hDlg, TRUE);
				return ((DLGPROC)TRUE);

			    }
			return ((DLGPROC)TRUE);
		    }
	    }
	return (FALSE);
	}

    /**************************************************************************
	    Show Tierazon Filter
    **************************************************************************/

    DLGPROC FAR PASCAL SelectFDOptionDlg(HWND hDlg, unsigned message, WPARAM wParam, LPARAM lParam)

	{
	static	int	i;
	static	int	index;

	switch (message)
	    {
	    case WM_INITDIALOG:
		SetDlgItemText(hDlg, ID_LISTTITLE, "Tierazon Filter FD-Options");
		for (i = 0; i < MAXFDOPTIONS; i++)
		    SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)(LPSTR)TierazonFDOptions[i]);
		SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)nFDOption - 1, 0L);
		return ((DLGPROC)TRUE);

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
			nFDOption = index + 1;
			TZfilter.nFDOption = nFDOption;
			EndDialog(hDlg, TRUE);
			return ((DLGPROC)TRUE);

		    case IDCANCEL:
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
				nFDOption = index + 1;
				TZfilter.nFDOption = nFDOption;
				EndDialog(hDlg, TRUE);
				return ((DLGPROC)TRUE);

			    }
			return ((DLGPROC)TRUE);
		    }
	    }
	return ((DLGPROC)FALSE);
	}

