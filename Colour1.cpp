//////////////////////////////////////////////////////////////////////////////////////////
//
// COLOUR1.CPP a module for a variety of colour based functions.
//
//////////////////////////////////////////////////////////////////////////////////////////
// Author:-
//	Paul de Leeuw
//	pdeleeuw at deleeuw dot com dot au	( replace "at" "dot" by the normal characters.)
//	10/07/2007
//	This Class includes code to create a true palette and to modify it
//////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include "manp.h"
#include "dib.h"
#include "preview.h"
#include "resource.h"
#include "colour.h"
#include "Plot.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;		// This is the main windows handle
extern	CDib	Dib;			// Device Independent Bitmaps
//////////////////////////////////////////////////////////////////////////////////////////////////////

#define	PREVIEW_HEIGHT	177
#define	PREVIEW_WIDTH	220
#define	HOR_OFFSET	116
#define	VERT_OFFSET	9

//extern	short	InsideRed;			// values for r, g, b channels for inside colour
//extern	short	InsideGreen;			
//extern	short	InsideBlue;

extern	DWORD	BackgroundColour;		// set background colour for IFS and L-System fractals
extern	long	threshold;			// maximum iterations ... called maxiter in FRACTINT
extern	int	StartColourCycling;		// we can start the colour cycling from any point..good for cycling animations
extern	int	height, width, bits_per_pixel;

extern	PAINTSTRUCT 	ps;
extern	RECT 	r;
extern	BOOL	bTrack;				// TRUE if user is selecting a region
extern	int	logval;				// log colour map required 
extern	BOOL	UseFractintPalette;		// standard EGA palette
extern	BYTE	default_palette[];		// default VGA colour palette
extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts
extern	WORD	type;				// M=mand, J=Julia 1,2,4->
extern	int	time_to_reinit;			// time to reinitialize?
extern	int	OutsideMethod;			// outside filters

DLGPROC FAR PASCAL DisplayRGBDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam);

extern	void	AnalysePalette(char *);

static	CPreview	PreviewColour;
extern	CTrueCol	TrueCol;		// palette info
extern	CPlot		Plot;			// image plotting routines

//////////////////////////////////////////////////////////////////////
//	Initialise True Colour Palette
//////////////////////////////////////////////////////////////////////

void	InitTrueColourPalette(BYTE RandFlag)

    {
    TrueCol.InitTrueColPal(RandFlag, threshold, StartColourCycling, logval, bits_per_pixel, UseFractintPalette, default_palette);
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
    int		tempflags = Plot.flags;

    if (OutsideMethod >= TIERAZONFILTERS)
	return;					// many Tierazon filters and colours don't use the palette, so why display it?

    Plot.flags -= USEWPIXELS;			// we don't want to write to wpixels
    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;

    MaxPal = (double)LocalThreshold / (double) height;

    for (i = 0; i < height; ++i)
	{
	if (DisplayFlag)			// display the palette
	    size = (DWORD)(MaxPal * (double) i);
	else if (type == PERTURBATION || type == SLOPEFORWARDDIFF || type == SLOPEDERIVATIVE)
	    return;
	for (j = width - 15; j < width; ++j)
	    {
	    if (!DisplayFlag)			// retrieve value from wpixels so we can toggle palette display
		size = (DWORD)(*(wpixels + i * width + j));
	    Plot.PlotPoint(j, i, size);
	    }
	}
    if (!bTrack)				// don't splatter scan
	{
	InvalidateRect(hwnd, &r, FALSE);
	}
    Plot.flags = tempflags;			// restore flags 
    }

/*---------------------------------------------------------------------
	Display RGB Values
  -------------------------------------------------------------------*/

void	RGBAddress(int x, int y, RGBTRIPLE *colour)

    {
    RGBTRIPLE * Source = (RGBTRIPLE *)(Dib.DibPixels + (DWORD) (Dib.DibHeight - y) * Dib.WidthBytes + x * Dib.BitsPerPixel / 8);

    *colour = *Source;
    }

/*---------------------------------------------------------------------
	Edit RGB Values
  -------------------------------------------------------------------*/

int	EditRGB(HINSTANCE hInst, POINTS ptCurrent)

    {
    RGBTRIPLE   colour;

    RGBAddress(ptCurrent.x, ptCurrent.y, &colour);
    BackgroundColour = (DWORD)(colour.rgbtBlue) + (DWORD)(colour.rgbtGreen * 256) + (DWORD)(colour.rgbtRed * 65536);
    DialogBox (hInst, "DisplayRGBDlg", GlobalHwnd, (DLGPROC)DisplayRGBDlg);

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
    wsprintf (s, "Colour <%02X,%02X,%02X> at %d, %d", colour.rgbtRed, colour.rgbtGreen, colour.rgbtBlue, ptCurrent.x, ptCurrent.y);
    SetWindowText (GlobalHwnd, s);

    return 0;
    }

/**************************************************************************
	Save Colour Data to File
**************************************************************************/

#define DARKGREY	40

void	SaveTriplets(FILE *fop)

    {
    int	i;
    long LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;

    for (i = 0; i < ((LocalThreshold < 256) ? 256 : LocalThreshold); ++i)
	{
	fprintf(fop, "%d %d %d\n",
	    *(TrueCol.PalettePtr + i * 3 + 0),
	    *(TrueCol.PalettePtr + i * 3 + 1),
	    *(TrueCol.PalettePtr + i * 3 + 2));
	}
    }

short	save_palette(HWND hwnd, char *outfile, char *szAppName)

    {
    FILE	*fop;
    //int	i;
    long LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;

    if (!(fop = fopen(outfile, "w")))
	{
	MessageBox(hwnd, "Can't open colour file", outfile, MB_ICONEXCLAMATION | MB_OK);
	MessageBeep(0);
	return 0;
	}

    if (LocalThreshold < 256)						// make sure to add entries to the end of the palette map
	memset(TrueCol.PalettePtr + LocalThreshold * 3, DARKGREY, (256 - LocalThreshold) * 3);	// dark grey, not black
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
	AnalysePalette(s);
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
	TrueCol.RedStartInt, TrueCol.GreenStartInt, TrueCol.BlueStartInt,
	TrueCol.RedIncInt, TrueCol.GreenIncInt, TrueCol.BlueIncInt);
    //OutputPalFile(fop);
    fclose(fop);
    return 0;
    }

/**************************************************************************
	Dialog Control for Setting Inside colour
**************************************************************************/

DLGPROC FAR PASCAL InsideDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)
    {
    static short color[3];
    HWND	  hwndParent, hCtrl;
    short	  nCtrlID, nIndex;
    //     BOOL	  bTrans ;
#ifdef	WIN95								// 32 bit code
    char	  s[480];
#endif

    PreviewColour.HorOffset = HOR_OFFSET;						// dimensions of preview window
    PreviewColour.VertOffset = VERT_OFFSET;
    PreviewColour.PreviewHeight = PREVIEW_HEIGHT;
    PreviewColour.PreviewWidth = PREVIEW_WIDTH;
    switch (message)
	{
	case WM_INITDIALOG:
	    color[0] = TrueCol.InsideRed;
	    color[1] = TrueCol.InsideGreen;
	    color[2] = TrueCol.InsideBlue;
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
	    return (DLGPROC)TRUE;

	case WM_VSCROLL:

#ifdef	WIN95								// 32 bit code
	    hCtrl = GET_WM_COMMAND_HWND(wParam, (_int64)lParam);
	    if ((nCtrlID = ((WORD)(GetWindowLong(hCtrl, GWL_ID)))) == 0)
		{
		wsprintf(s, "Error: Scrollbar fail: <%ld>", GetLastError());
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
	    return (DLGPROC)TRUE;

	case WM_PAINT:
	    BeginPaint(hDlg, &ps);
	    PreviewColour.PreviewDib.ClearDib(color[0], color[1], color[2]);
	    PreviewColour.Preview(hDlg);
	    EndPaint(hDlg, &ps);
	    return (DLGPROC)TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		//	       switch (wParam)
		{
		case IDOK:
		    TrueCol.InsideRed = color[0];
		    TrueCol.InsideGreen = color[1];
		    TrueCol.InsideBlue = color[2];
		    TrueCol.SetTrueColourPixel(TrueCol.InsideBlue, TrueCol.InsideGreen, TrueCol.InsideRed, threshold);
		    Plot.RefreshScreen();			// reload screen values
		    hwndParent = GetParent(hDlg);
		    InvalidateRect(hwndParent, &r, FALSE);	// force repaint
		    PreviewColour.PreviewDib.CloseDibPtrs();
		    EndDialog(hDlg, 0);
		    return (DLGPROC)TRUE;
		case IDCANCEL:
		    PreviewColour.PreviewDib.CloseDibPtrs();
		    EndDialog(hDlg, 0);
		    return (DLGPROC)TRUE;
		}
	    break;
	}
    return FALSE;
    }

DLGPROC FAR PASCAL DisplayRGBDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)
    {
    int	r, g, b;
    char	s[20];

    switch (message)
	{
	case WM_INITDIALOG:
	    b = (WORD)(BackgroundColour & 0xff);
	    g = (WORD)((BackgroundColour >> 8) & 0xff);
	    r = (WORD)((BackgroundColour >> 16) & 0xff);
	    sprintf(s, "%02X%02X%02X", r, g, b);
	    SetDlgItemText(hDlg, IDC_RGB, s);
	    SetDlgItemInt(hDlg, IDC_RGBRED, r, TRUE);
	    SetDlgItemInt(hDlg, IDC_RGBGREEN, g, TRUE);
	    SetDlgItemInt(hDlg, IDC_RGBBLUE, b, TRUE);
	    return (DLGPROC)TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDCANCEL:
		case IDOK:
		    EndDialog(hDlg, TRUE);
		    return (DLGPROC)TRUE;
		}
	    break;
	}
    return FALSE;
    }

/**************************************************************************
	Dialog Control for Colour Palette Setting
**************************************************************************/

DLGPROC FAR PASCAL ColourDlg(HWND hDlg, UINT message, UINT wParam, LONG lParam)
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
	    SetDlgItemInt(hDlg, IDC_START_RED, TrueCol.RedStartInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_START_BLUE, TrueCol.BlueStartInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_START_GREEN, TrueCol.GreenStartInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_INC_GREEN, TrueCol.GreenIncInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_INC_RED, TrueCol.RedIncInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_INC_BLUE, TrueCol.BlueIncInt, TRUE);
	    SetDlgItemInt(hDlg, IDC_RAND_DIV, TrueCol.RandomDivisor, TRUE);
	    //		hCtrl = GetDlgItem (hDlg, IDC_GENPAL);
	    //		SendMessage(hCtrl, BM_SETCHECK, TrueColourFlag, 0L);
//	    hCtrl = GetDlgItem(hDlg, IDC_DISPLAYPAL);
//	    SendMessage(hCtrl, BM_SETCHECK, TrueCol.DisplayPaletteFlag, 0L);
	    hCtrl = GetDlgItem(hDlg, IDC_RANDPAL);
	    SendMessage(hCtrl, BM_SETCHECK, TrueCol.RandomColourFlag, 0L);
	    color[0] = TrueCol.RedStartInt;
	    color[1] = TrueCol.GreenStartInt;
	    color[2] = TrueCol.BlueStartInt;
	    color[3] = TrueCol.RandomDivisor;
	    for (nCtrlID = IDC_REDSCROLL; nCtrlID <= IDC_RANDSCROLL; nCtrlID++)
		{
		hCtrl = GetDlgItem(hDlg, nCtrlID);
		SetScrollRange(hCtrl, SB_CTL, 0, 1024, FALSE);
		SetScrollPos(hCtrl, SB_CTL, color[nCtrlID - IDC_REDSCROLL], FALSE);
		}
	    hCtrl = GetDlgItem(hDlg, IDC_CYCLE_START);
	    SetDlgItemInt(hDlg, IDC_CYCLE_START, StartColourCycling, TRUE);
	    return (DLGPROC)TRUE;

	case WM_HSCROLL:

	    hCtrl = GET_WM_COMMAND_HWND(wParam, (_int64)lParam);
	    if ((nCtrlID = ((WORD)(GetWindowLong(hCtrl, GWL_ID)))) == 0)
		{
		wsprintf(s, "Error: Scrollbar fail: <%ld>", GetLastError());
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

	    return (DLGPROC)TRUE;

	case WM_COMMAND:
	    switch ((int)LOWORD(wParam))
		{
		case IDC_START_RED:
		    tempRedStart = GetDlgItemInt(hDlg, IDC_START_RED, &bTrans, TRUE);
		    return (DLGPROC)TRUE;
		case IDC_START_BLUE:
		    tempBlueStart = GetDlgItemInt(hDlg, IDC_START_BLUE, &bTrans, TRUE);
		    return (DLGPROC)TRUE;
		case IDC_START_GREEN:
		    tempGreenStart = GetDlgItemInt(hDlg, IDC_START_GREEN, &bTrans, TRUE);
		    return (DLGPROC)TRUE;
		case IDC_INC_GREEN:
		    tempGreenInc = GetDlgItemInt(hDlg, IDC_INC_GREEN, &bTrans, TRUE);
		    return (DLGPROC)TRUE;
		case IDC_INC_RED:
		    tempRedInc = GetDlgItemInt(hDlg, IDC_INC_RED, &bTrans, TRUE);
		    return (DLGPROC)TRUE;
		case IDC_INC_BLUE:
		    tempBlueInc = GetDlgItemInt(hDlg, IDC_INC_BLUE, &bTrans, TRUE);
		    return (DLGPROC)TRUE;
		case IDC_RAND_DIV:
		    tempRandDiv = GetDlgItemInt(hDlg, IDC_RAND_DIV, &bTrans, TRUE);
		    return (DLGPROC)TRUE;

		case IDM_DEFAULTCOL:
		case IDC_INVERT:
		    if (LOWORD(wParam) == IDM_DEFAULTCOL)
			{
			TrueCol.RedStartInt = 60;
			TrueCol.GreenStartInt = 120;
			TrueCol.BlueStartInt = 30;
			TrueCol.RedIncInt = 100;
			TrueCol.GreenIncInt = 255;
			TrueCol.BlueIncInt = 200;
			}
		    else
			{
			TrueCol.RedStartInt = (~TrueCol.RedStartInt & 0x00ff);
			TrueCol.GreenStartInt = (~TrueCol.GreenStartInt & 0x00ff);
			TrueCol.BlueStartInt = (~TrueCol.BlueStartInt & 0x00ff);
			}
		    SetDlgItemInt(hDlg, IDC_START_RED, TrueCol.RedStartInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_START_BLUE, TrueCol.BlueStartInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_START_GREEN, TrueCol.GreenStartInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_INC_GREEN, TrueCol.GreenIncInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_INC_RED, TrueCol.RedIncInt, TRUE);
		    SetDlgItemInt(hDlg, IDC_INC_BLUE, TrueCol.BlueIncInt, TRUE);
		    color[0] = TrueCol.RedStartInt;
		    color[1] = TrueCol.GreenStartInt;
		    color[2] = TrueCol.BlueStartInt;
		    color[3] = TrueCol.RandomDivisor;
		    for (nCtrlID = IDC_REDSCROLL; nCtrlID <= IDC_BLUSCROLL; nCtrlID++)
			{
			hCtrl = GetDlgItem(hDlg, nCtrlID);
			SetScrollPos(hCtrl, SB_CTL, color[nCtrlID - IDC_REDSCROLL], FALSE);
			}
		    return (DLGPROC)TRUE;

		case IDOK:
		    TrueCol.RedIncInt = tempRedInc;
		    TrueCol.GreenIncInt = tempGreenInc;
		    TrueCol.BlueIncInt = tempBlueInc;
		    TrueCol.RedStartInt = tempRedStart;
		    TrueCol.GreenStartInt = tempGreenStart;
		    TrueCol.BlueStartInt = tempBlueStart;
		    TrueCol.RandomDivisor = tempRandDiv;
		    if (TrueCol.RandomDivisor < 1)			// no division by 0
			TrueCol.RandomDivisor = 1;
		    hCtrl = GetDlgItem(hDlg, IDC_RANDPAL);
		    TrueCol.RandomColourFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
//		    hCtrl = GetDlgItem(hDlg, IDC_DISPLAYPAL);
//		    TrueCol.DisplayPaletteFlag = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		    StartColourCycling = GetDlgItemInt(hDlg, IDC_CYCLE_START, &bTrans, TRUE);
//		    long LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
		    while (StartColourCycling < 0)
			StartColourCycling += TrueCol.LocalThreshold;
		    //			if (TrueColourFlag)
		    {
		    TrueCol.PalEditFlag = FALSE;
		    }

		    EndDialog(hDlg, TRUE);
		    return (DLGPROC)TRUE;

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return (DLGPROC)FALSE;
		}
	    break;
	}
    return (DLGPROC)FALSE;
    }





