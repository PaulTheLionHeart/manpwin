/*
    COLOUR1.CPP a module for a variety of colour based functions.

    Written in MICROSOFT 'C++' by Paul de Leeuw.
*/

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include "manp.h"
#include "dib.h"
#include "preview.h"
#include "resource.h"
#include "colour.h"
#include "Plot.h"
#include "SafeStrings.h"

#define	PREVIEW_HEIGHT	177
#define	PREVIEW_WIDTH	220
#define	HOR_OFFSET	116
#define	VERT_OFFSET	9

extern	int	StartColourCycling;		// we can start the colour cycling from any point..good for cycling animations
extern	BYTE	default_palette[];		// default VGA colour palette
INT_PTR CALLBACK DisplayRGBDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

//////////////////////////////////////////////////////////////////////
// Build runtime iteration-to-RGB lookup map.
//
// The colour source may be:
//
// 1. Imported MAP palette data
// 2. Procedural COL colour generation
//
// Both sources populate PalettePtr[], which is the
// runtime iteration colour lookup table.
//
//////////////////////////////////////////////////////////////////////
void	InitTrueColourPalette(BYTE RandFlag)
    {
    gManp->TrueCol.BuildIterationColourMap(RandFlag, gManp->threshold, StartColourCycling, gManp->logval);
    }

void	InitIterationColours(BYTE RandFlag)
    {
    gManp->TrueCol.BuildIterationColourMap(RandFlag, gManp->threshold, StartColourCycling, gManp->logval);
    }

/**************************************************************************
	Display palette
**************************************************************************/

void	DisplayPalette(HWND hwnd, BOOL DisplayFlag)
    {
    WORD	i, j;
    DWORD	size;
    double	MaxPal;
    long	LocalThreshold;
    int		tempflags = gManp->Plot.flags;

    if (gManp->OutsideMethod >= TIERAZONFILTERS)
	return;					// many Tierazon filters and colours don't use the palette, so why display it?

    gManp->Plot.flags -= USEWPIXELS;			// we don't want to write to wpixels
    LocalThreshold = (gManp->threshold >= MAXPALETTE) ? MAXPALETTE - 1 : gManp->threshold;

    MaxPal = (double)LocalThreshold / (double)gManp->height;

    for (i = 0; i < gManp->height; ++i)
	{
	if (DisplayFlag)			// display the palette
	    size = (DWORD)(MaxPal * (double) i);
	else if (gManp->type == PERTURBATION || gManp->type == SLOPEFORWARDDIFF || gManp->type == SLOPEDERIVATIVE)
	    return;
	for (j = gManp->width - 15; j < gManp->width; ++j)
	    {
	    if (!DisplayFlag)			// retrieve value from wpixels so we can toggle palette display
		{
		float value = gManp->wpixels[i * gManp->width + j];
		size = (DWORD)value;
		}
	    gManp->Plot.PlotPoint(j, i, size);
	    }
	}
    if (!gManp->bTrack)				// don't splatter scan
	{
	InvalidateRect(hwnd, &gManp->r, FALSE);
	}
    gManp->Plot.flags = tempflags;			// restore flags 
    }

/*---------------------------------------------------------------------
	Display RGB Values
  -------------------------------------------------------------------*/

void	RGBAddress(int x, int y, RGBTRIPLE *colour)
    {
    RGBTRIPLE * Source = (RGBTRIPLE *)(gManp->Dib.DibPixels.data() + (DWORD) (gManp->Dib.DibHeight - y) * gManp->Dib.WidthBytes + x * gManp->Dib.BitsPerPixel / 8);

    *colour = *Source;
    }

/*---------------------------------------------------------------------
	Edit RGB Values
  -------------------------------------------------------------------*/

int	EditRGB(HINSTANCE hInst, POINTS ptCurrent)
    {
    RGBTRIPLE   colour;

    RGBAddress(ptCurrent.x, ptCurrent.y, &colour);
    gManp->BackgroundColour = (DWORD)(colour.rgbtBlue) + (DWORD)(colour.rgbtGreen * 256) + (DWORD)(colour.rgbtRed * 65536);
    DialogBox (hInst, "DisplayRGBDlg", gManp->GlobalHwnd, DisplayRGBDlg);

    return 0;
    }

/*---------------------------------------------------------------------
	Display RGB Values
  -------------------------------------------------------------------*/

int	DisplayRGB(POINTS ptCurrent)
    {
    RGBTRIPLE   colour;
    char	s[80];

    RGBAddress(ptCurrent.x, ptCurrent.y, &colour);
    _snprintf_s(s, 80, _TRUNCATE, "Colour <%02X,%02X,%02X> at %d, %d", colour.rgbtRed, colour.rgbtGreen, colour.rgbtBlue, ptCurrent.x, ptCurrent.y);
    SetWindowText (gManp->GlobalHwnd, s);

    return 0;
    }

/**************************************************************************
	Save Colour Data to File
**************************************************************************/

#define DARKGREY	40

void	SaveTriplets(FILE *fop)
    {
    int	i;
    long LocalThreshold = (gManp->threshold >= MAXPALETTE) ? MAXPALETTE - 1 : gManp->threshold;

    for (i = 0; i < ((LocalThreshold < 256) ? 256 : LocalThreshold); ++i)
	{
	fprintf(fop, "%d %d %d\n",
	    gManp->TrueCol.PalettePtr[i].rgbtBlue,
	    gManp->TrueCol.PalettePtr[i].rgbtGreen,
	    gManp->TrueCol.PalettePtr[i].rgbtRed);
	}
    }

short	save_palette(HWND hwnd, char *outfile, char *szAppName)
    {
    FILE	*fop;
    //int	i;
    long LocalThreshold = (gManp->threshold >= MAXPALETTE) ? MAXPALETTE - 1 : gManp->threshold;

    if (!(fop = fopen(outfile, "w")))
	{
	MessageBox(hwnd, "Can't open colour file", outfile, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	return 0;
	}

    if (LocalThreshold < 256)						// make sure to add entries to the end of the palette map
	{
	RGBTRIPLE   DarkGrey = { DARKGREY, DARKGREY, DARKGREY };
	for (int i = LocalThreshold; i < 256; i++)
	    gManp->TrueCol.PalettePtr[i] = DarkGrey;
	}
    SaveTriplets(fop);
    fclose(fop);
    return 0;
    }

/**************************************************************************
	Get Input Data from File
**************************************************************************/

short	FileReadColours(HWND hwnd, char *infile, char *szAppName)
    {
    char	s[150];
    FILE	*fip;

    if (!(fip = fopen(infile, "r")))
	{
	MessageBox(hwnd, "Can't open colour file", szAppName, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	return 0;
	}
    if (fgets(s, 150, fip))
	{
	gManp->AnalysePalette(s);
	//    TrueColourFlag = TRUE;
	}
    //while (fgets(s, 150, fip))
    //    PaletteToRules(s);
    fclose(fip);
    return 0;
    }

/**************************************************************************
	Point to the appropriate palette
**************************************************************************/

/*
void SetPalettePointer(BYTE *PalettePtr)

{
PalettePtr = TrueCol.TRUE_PALETTE;
}
*/

/**************************************************************************
	Save Colour Data to File
**************************************************************************/

short	save_colour(HWND hwnd, char *outfile, char *szAppName)
    {
    FILE	*fop;

    if (!(fop = fopen(outfile, "w")))
	{
	MessageBox(hwnd, "Can't open colour file", outfile, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	return 0;
	}
    fprintf(fop, "%d %d %d %d %d %d\n",
	gManp->TrueCol.RedStartInt, gManp->TrueCol.GreenStartInt, gManp->TrueCol.BlueStartInt,
	gManp->TrueCol.RedIncInt, gManp->TrueCol.GreenIncInt, gManp->TrueCol.BlueIncInt);
    //OutputPalFile(fop);
    fclose(fop);
    return 0;
    }

/**************************************************************************
	Dialog Control for Setting Inside colour
**************************************************************************/

INT_PTR CALLBACK InsideDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static	    short    color[3];
    HWND	    hwndParent, hCtrl;
    short	    nCtrlID, nIndex;
    char	    s[480];
    static	    CPreview	PreviewColour;

    PreviewColour.HorOffset = HOR_OFFSET;						// dimensions of preview window
    PreviewColour.VertOffset = VERT_OFFSET;
    PreviewColour.PreviewHeight = PREVIEW_HEIGHT;
    PreviewColour.PreviewWidth = PREVIEW_WIDTH;
    switch (message)
	{
	case WM_INITDIALOG:
	    color[0] = gManp->TrueCol.InsideRed;
	    color[1] = gManp->TrueCol.InsideGreen;
	    color[2] = gManp->TrueCol.InsideBlue;
	    for (nCtrlID = 10; nCtrlID < 13; nCtrlID++)
		{
		//		    color[nCtrlID - 10] = 0;
		hCtrl = GetDlgItem(hDlg, nCtrlID);
		SetScrollRange(hCtrl, SB_CTL, 0, 255, FALSE);
		SetScrollPos(hCtrl, SB_CTL, 255 - color[nCtrlID - 10], FALSE);
		SetDlgItemInt(hDlg, nCtrlID + 3, color[nCtrlID - 10], TRUE);
		}
	    PreviewColour.InitPreview(hDlg);
	    PreviewColour.PreviewDib.ClearDib(color[0], color[1], color[2]);
	    PreviewColour.Preview(hDlg);
	    InvalidateRect(hDlg, NULL, FALSE);

	    //		SendMessage (hDlg, WM_HSCROLL, 0, 0L);
	    return TRUE;

	case WM_VSCROLL:

#ifdef	WIN95								// 32 bit code
	    hCtrl = GET_WM_COMMAND_HWND(wParam, (_int64)lParam);
	    if ((nCtrlID = ((WORD)(GetWindowLong(hCtrl, GWL_ID)))) == 0)
		{
		_snprintf_s(s, 480, _TRUNCATE, "Error: Scrollbar fail: <%ld>", GetLastError());
		MessageBox(hDlg, s, "Paul's Graphics Viewer", MB_ICONEXCLAMATION | MB_OK);
		}

#else									// 16 bit code
	    hCtrl = (HWND)(HIWORD(lParam));
	    nCtrlID = GetWindowWord(hCtrl, GWW_ID);
#endif

	    nIndex = nCtrlID - 10;
	    hwndParent = GetParent(hDlg);

	    switch ((int)LOWORD(wParam))
		{
		case SB_PAGEDOWN:
		    color[nIndex] -= 15;        // fall through
		case SB_LINEDOWN:
		    color[nIndex] = max(0, color[nIndex] - 1);
		    break;
		case SB_PAGEUP:
		    color[nIndex] += 15;        // fall through
		case SB_LINEUP:
		    color[nIndex] = min(255, color[nIndex] + 1);
		    break;
		case SB_TOP:
		    color[nIndex] = 255;
		    break;
		case SB_BOTTOM:
		    color[nIndex] = 0;
		    break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
#ifdef	WIN95								// 32 bit code
		    color[nIndex] = 255 - HIWORD(wParam);
#else									// 16 bit code
		    color[nIndex] = LOWORD(lParam);
#endif
		    //			 color [nIndex] = LOWORD (lParam) ;
		    break;
		default:
		    return FALSE;
		}
	    SetScrollPos(hCtrl, SB_CTL, 255 - color[nIndex], TRUE);
	    SetDlgItemInt(hDlg, nCtrlID + 3, color[nIndex], TRUE);
	    PreviewColour.PreviewDib.ClearDib(color[0], color[1], color[2]);
	    PreviewColour.Preview(hDlg);
	    return TRUE;

	case WM_PAINT:
	    BeginPaint(hDlg, &gManp->ps);
	    PreviewColour.PreviewDib.ClearDib(color[0], color[1], color[2]);
	    PreviewColour.Preview(hDlg);
	    EndPaint(hDlg, &gManp->ps);
	    return TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		//	       switch (wParam)
		{
		case IDOK:
		    gManp->TrueCol.InsideRed = color[0];
		    gManp->TrueCol.InsideGreen = color[1];
		    gManp->TrueCol.InsideBlue = color[2];
		    gManp->TrueCol.SetTrueColourPixel(gManp->TrueCol.InsideBlue, gManp->TrueCol.InsideGreen, gManp->TrueCol.InsideRed, gManp->threshold);
		    gManp->Plot.RefreshScreen();			// reload screen values
		    hwndParent = GetParent(hDlg);
		    InvalidateRect(hwndParent, &gManp->r, FALSE);	// force repaint
		    PreviewColour.PreviewDib.CloseDibPtrs();
		    EndDialog(hDlg, 0);
		    return TRUE;
		case IDCANCEL:
		    PreviewColour.PreviewDib.CloseDibPtrs();
		    EndDialog(hDlg, 0);
		    return TRUE;
		}
	    break;
	}
    return FALSE;
    }

INT_PTR CALLBACK DisplayRGBDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    int		r, g, b;
    char	s[20];

    switch (message)
	{
	case WM_INITDIALOG:
	    b = (WORD)(gManp->BackgroundColour & 0xff);
	    g = (WORD)((gManp->BackgroundColour >> 8) & 0xff);
	    r = (WORD)((gManp->BackgroundColour >> 16) & 0xff);
	    SAFE_SPRINTF(s, "%02X%02X%02X", r, g, b);
	    SetDlgItemText(hDlg, IDC_RGB, s);
	    SetDlgItemInt(hDlg, IDC_RGBRED, r, TRUE);
	    SetDlgItemInt(hDlg, IDC_RGBGREEN, g, TRUE);
	    SetDlgItemInt(hDlg, IDC_RGBBLUE, b, TRUE);
	    return TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDCANCEL:
		case IDOK:
		    EndDialog(hDlg, TRUE);
		    return TRUE;
		}
	    break;
	}
    return FALSE;
    }

INT_PTR CALLBACK SetStartRGBDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static	    short    color[3];
    HWND	    hwndParent, hCtrl;
    short	    nCtrlID, nIndex;
    char	    s[480];
    static	    CPreview	PreviewColour;

    PreviewColour.HorOffset = HOR_OFFSET;						// dimensions of preview window
    PreviewColour.VertOffset = VERT_OFFSET;
    PreviewColour.PreviewHeight = PREVIEW_HEIGHT;
    PreviewColour.PreviewWidth = PREVIEW_WIDTH;
    switch (message)
	{
	case WM_INITDIALOG:
	    color[2] = (WORD)(gManp->PrePaletteColour & 0xff);
	    color[1] = (WORD)((gManp->PrePaletteColour >> 8) & 0xff);
	    color[0] = (WORD)((gManp->PrePaletteColour >> 16) & 0xff);
	    for (nCtrlID = 10; nCtrlID < 13; nCtrlID++)
		{
		//		    color[nCtrlID - 10] = 0;
		hCtrl = GetDlgItem(hDlg, nCtrlID);
		SetScrollRange(hCtrl, SB_CTL, 0, 255, FALSE);
		SetScrollPos(hCtrl, SB_CTL, 255 - color[nCtrlID - 10], FALSE);
		SetDlgItemInt(hDlg, nCtrlID + 3, color[nCtrlID - 10], TRUE);
		}
	    PreviewColour.InitPreview(hDlg);
	    PreviewColour.PreviewDib.ClearDib(color[0], color[1], color[2]);
	    PreviewColour.Preview(hDlg);
	    InvalidateRect(hDlg, NULL, FALSE);

	    //		SendMessage (hDlg, WM_HSCROLL, 0, 0L);
	    return TRUE;

	case WM_VSCROLL:

#ifdef	WIN95								// 32 bit code
	    hCtrl = GET_WM_COMMAND_HWND(wParam, (_int64)lParam);
	    if ((nCtrlID = ((WORD)(GetWindowLong(hCtrl, GWL_ID)))) == 0)
		{
		_snprintf_s(s, 480, _TRUNCATE, "Error: Scrollbar fail: <%ld>", GetLastError());
		MessageBox(hDlg, s, "Paul's Graphics Viewer", MB_ICONEXCLAMATION | MB_OK);
		}

#else									// 16 bit code
	    hCtrl = (HWND)(HIWORD(lParam));
	    nCtrlID = GetWindowWord(hCtrl, GWW_ID);
#endif

	    nIndex = nCtrlID - 10;
	    hwndParent = GetParent(hDlg);

	    switch ((int)LOWORD(wParam))
		//	       switch (wParam)
		{
		case SB_PAGEDOWN:
		    color[nIndex] -= 15;        // fall through
		case SB_LINEDOWN:
		    color[nIndex] = max(0, color[nIndex] - 1);
		    break;
		case SB_PAGEUP:
		    color[nIndex] += 15;        // fall through
		case SB_LINEUP:
		    color[nIndex] = min(255, color[nIndex] + 1);
		    break;
		case SB_TOP:
		    color[nIndex] = 255;
		    break;
		case SB_BOTTOM:
		    color[nIndex] = 0;
		    break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
#ifdef	WIN95								// 32 bit code
		    color[nIndex] = 255 - HIWORD(wParam);
#else									// 16 bit code
		    color[nIndex] = LOWORD(lParam);
#endif
		    //			 color [nIndex] = LOWORD (lParam) ;
		    break;
		default:
		    return FALSE;
		}
	    SetScrollPos(hCtrl, SB_CTL, 255 - color[nIndex], TRUE);
	    SetDlgItemInt(hDlg, nCtrlID + 3, color[nIndex], TRUE);
	    PreviewColour.PreviewDib.ClearDib(color[0], color[1], color[2]);
	    PreviewColour.Preview(hDlg);
	    return TRUE;

	case WM_PAINT:
	    BeginPaint(hDlg, &gManp->ps);
	    PreviewColour.PreviewDib.ClearDib(color[0], color[1], color[2]);
	    PreviewColour.Preview(hDlg);
	    EndPaint(hDlg, &gManp->ps);
	    return TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDOK:
		    gManp->PrePaletteColour = ((DWORD)color[0] << 16) | ((DWORD)color[1] << 8) | (DWORD)color[2];
		    if (gManp->type == SLOPEDERIVATIVE || gManp->type == SLOPEFORWARDDIFF)
			gManp->param[15] = (double)gManp->PrePaletteColour;
		    gManp->Plot.RefreshScreen();			// reload screen values
		    hwndParent = GetParent(hDlg);
		    InvalidateRect(hwndParent, &gManp->r, FALSE);	// force repaint
		    PreviewColour.PreviewDib.CloseDibPtrs();
		    EndDialog(hDlg, IDOK);
		    return TRUE;
		case IDCANCEL:
		    PreviewColour.PreviewDib.CloseDibPtrs();
		    EndDialog(hDlg, IDCANCEL);
		    return TRUE;
		}
	    break;
	}
    return FALSE;
    }    

/**************************************************************************
	Dialog Control for Colour Palette Setting
**************************************************************************/

INT_PTR CALLBACK ColourDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    //     static	HANDLE	hCursor;
    static     UINT	tempRedInc;
    static     UINT	tempGreenInc;
    static     UINT	tempBlueInc;
    static     UINT	tempRedStart;
    static     UINT	tempGreenStart;
    static     UINT	tempBlueStart;
    static     UINT	tempRandDiv;
    BOOL	bTrans;
    static	short	color[4];	    // R, G, B + Random Divisor
    static	HWND	hwndParent, hCtrl;
    static	short	nCtrlID, nIndex/*, temp*/;
    char	s[100];

    switch (message)
	{
	case WM_INITDIALOG:
	    SetDlgItemInt(hDlg, IDC_START_RED, gManp->TrueCol.RedStartInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_START_BLUE, gManp->TrueCol.BlueStartInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_START_GREEN, gManp->TrueCol.GreenStartInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_INC_GREEN, gManp->TrueCol.GreenIncInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_INC_RED, gManp->TrueCol.RedIncInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_INC_BLUE, gManp->TrueCol.BlueIncInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_RAND_DIV, gManp->TrueCol.RandomDivisor, TRUE);
	    //		hCtrl = GetDlgItem (hDlg, IDC_GENPAL);
	    //		SendMessage(hCtrl, BM_SETCHECK, TrueColourFlag, 0L);
//	    hCtrl = GetDlgItem(hDlg, IDC_DISPLAYPAL);
//	    SendMessage(hCtrl, BM_SETCHECK, TrueCol.DisplayPaletteFlag, 0L);
	    hCtrl = GetDlgItem(hDlg, IDC_RANDPAL);
	    SendMessage(hCtrl, BM_SETCHECK, gManp->TrueCol.RandomColourFlag, 0L);
	    color[0] = gManp->TrueCol.RedStartInt;
	    color[1] = gManp->TrueCol.GreenStartInt;
	    color[2] = gManp->TrueCol.BlueStartInt;
	    color[3] = gManp->TrueCol.RandomDivisor;
	    gManp->TrueCol.CurrentPaletteMode = PALETTE_CUSTOM_COL;
	    for (nCtrlID = IDC_REDSCROLL; nCtrlID <= IDC_RANDSCROLL; nCtrlID++)
		{
		hCtrl = GetDlgItem(hDlg, nCtrlID);
		SetScrollRange(hCtrl, SB_CTL, 0, 1024, FALSE);
		SetScrollPos(hCtrl, SB_CTL, color[nCtrlID - IDC_REDSCROLL], FALSE);
		}
	    hCtrl = GetDlgItem(hDlg, IDC_CYCLE_START);
	    SetDlgItemInt(hDlg, IDC_CYCLE_START, StartColourCycling, TRUE);
	    return TRUE;

	case WM_HSCROLL:

	    hCtrl = GET_WM_COMMAND_HWND(wParam, (_int64)lParam);
	    if ((nCtrlID = ((WORD)(GetWindowLong(hCtrl, GWL_ID)))) == 0)
		{
		_snprintf_s(s, 480, _TRUNCATE, "Error: Scrollbar fail: <%ld>", GetLastError());
		MessageBox(hDlg, s, "Paul's Graphics Viewer", MB_ICONEXCLAMATION | MB_OK);
		}

	    nIndex = nCtrlID - IDC_REDSCROLL;
	    hwndParent = GetParent(hDlg);

	    switch ((int)LOWORD(wParam))
		{
		case SB_PAGEDOWN:
		    color[nIndex] += 15;        // fall through
		case SB_LINEDOWN:
		    color[nIndex] = min(1024, color[nIndex] + 1);
		    break;
		case SB_PAGEUP:
		    color[nIndex] -= 15;        // fall through
		case SB_LINEUP:
		    color[nIndex] = max(0, color[nIndex] - 1);
		    break;
		case SB_TOP:
		    color[nIndex] = 0;
		    break;
		case SB_BOTTOM:
		    color[nIndex] = 1024;
		    break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
		    color[nIndex] = HIWORD(wParam);
		    break;
		default:
		    return FALSE;
		}
	    SetScrollPos(hCtrl, SB_CTL, color[nIndex], TRUE);
	    switch (nIndex)
		{
		case 0:
		    SetDlgItemInt(hDlg, IDC_START_RED, color[0], TRUE);
		    break;
		case 1:
		    SetDlgItemInt(hDlg, IDC_START_GREEN, color[1], TRUE);
		    break;
		case 2:
		    SetDlgItemInt(hDlg, IDC_START_BLUE, color[2], TRUE);
		    break;
		case 3:
		    SetDlgItemInt(hDlg, IDC_RAND_DIV, color[3], TRUE);
		    break;
		}

	    return TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDC_START_RED:
		    tempRedStart = GetDlgItemInt(hDlg, IDC_START_RED, &bTrans, TRUE);
		    return TRUE;
		case IDC_START_BLUE:
		    tempBlueStart = GetDlgItemInt(hDlg, IDC_START_BLUE, &bTrans, TRUE);
		    return TRUE;
		case IDC_START_GREEN:
		    tempGreenStart = GetDlgItemInt(hDlg, IDC_START_GREEN, &bTrans, TRUE);
		    return TRUE;
		case IDC_INC_GREEN:
		    tempGreenInc = GetDlgItemInt(hDlg, IDC_INC_GREEN, &bTrans, TRUE);
		    return TRUE;
		case IDC_INC_RED:
		    tempRedInc = GetDlgItemInt(hDlg, IDC_INC_RED, &bTrans, TRUE);
		    return TRUE;
		case IDC_INC_BLUE:
		    tempBlueInc = GetDlgItemInt(hDlg, IDC_INC_BLUE, &bTrans, TRUE);
		    return TRUE;
		case IDC_RAND_DIV:
		    tempRandDiv = GetDlgItemInt(hDlg, IDC_RAND_DIV, &bTrans, TRUE);
		    return TRUE;

		case IDM_DEFAULTCOL:
		case IDC_INVERT:
		    if (LOWORD(wParam) == IDM_DEFAULTCOL)
			{
			gManp->TrueCol.RedStartInt = 60;
			gManp->TrueCol.GreenStartInt = 30;
			gManp->TrueCol.BlueStartInt = 120;
			gManp->TrueCol.RedIncInt = 100;
			gManp->TrueCol.GreenIncInt = 200;
			gManp->TrueCol.BlueIncInt = 255;
			}
		    else
			{
			gManp->TrueCol.RedStartInt = (~gManp->TrueCol.RedStartInt & 0x00ff);
			gManp->TrueCol.GreenStartInt = (~gManp->TrueCol.GreenStartInt & 0x00ff);
			gManp->TrueCol.BlueStartInt = (~gManp->TrueCol.BlueStartInt & 0x00ff);
			}
		    SetDlgItemInt(hDlg, IDC_START_RED, gManp->TrueCol.RedStartInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_START_BLUE, gManp->TrueCol.BlueStartInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_START_GREEN, gManp->TrueCol.GreenStartInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_INC_GREEN, gManp->TrueCol.GreenIncInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_INC_RED, gManp->TrueCol.RedIncInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_INC_BLUE, gManp->TrueCol.BlueIncInt, TRUE);
		    color[0] = gManp->TrueCol.RedStartInt;
		    color[1] = gManp->TrueCol.GreenStartInt;
		    color[2] = gManp->TrueCol.BlueStartInt;
		    color[3] = gManp->TrueCol.RandomDivisor;
		    for (nCtrlID = IDC_REDSCROLL; nCtrlID <= IDC_BLUSCROLL; nCtrlID++)
			{
			hCtrl = GetDlgItem(hDlg, nCtrlID);
			SetScrollPos(hCtrl, SB_CTL, color[nCtrlID - IDC_REDSCROLL], FALSE);
			}
		    return TRUE;

		case IDOK:
		    gManp->TrueCol.RedIncInt = tempRedInc;
		    gManp->TrueCol.GreenIncInt = tempGreenInc;
		    gManp->TrueCol.BlueIncInt = tempBlueInc;
		    gManp->TrueCol.RedStartInt = tempRedStart;
		    gManp->TrueCol.GreenStartInt = tempGreenStart;
		    gManp->TrueCol.BlueStartInt = tempBlueStart;
		    gManp->TrueCol.RandomDivisor = tempRandDiv;
		    if (gManp->TrueCol.RandomDivisor < 1)			// no division by 0
			gManp->TrueCol.RandomDivisor = 1;
		    hCtrl = GetDlgItem(hDlg, IDC_RANDPAL);
		    gManp->TrueCol.RandomColourFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
//		    hCtrl = GetDlgItem(hDlg, IDC_DISPLAYPAL);
//		    TrueCol.DisplayPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		    StartColourCycling = GetDlgItemInt(hDlg, IDC_CYCLE_START, &bTrans, TRUE);
//		    long LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
		    while (StartColourCycling < 0)
			StartColourCycling += gManp->TrueCol.LocalThreshold;
		    //			if (TrueColourFlag)
		    {
		    gManp->TrueCol.PalEditFlag = FALSE;
		    }

		    EndDialog(hDlg, TRUE);
		    return TRUE;

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    break;
	}
    return FALSE;
    }





