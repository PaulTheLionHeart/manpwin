//////////////////////////////////////////////////////////////////////////////////////////
//
// EDITPAL.CPP a module for palette editing functions.
//
//////////////////////////////////////////////////////////////////////////////////////////
// Author:-
//	Paul de Leeuw
//	pdeleeuw at deleeuw dot com dot au	( replace "at" "dot" by the normal characters.)
//	11/12/2007
//	This Class includes code to create a true palette and to modify it
//////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include "manpwin.h"
#include "manp.h"
#include "Dib.h"
#include "EditPal.h"
#include "resource.h"
#include "Preview.h"
#include "colour.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEditPal::CEditPal()
    {
    }

CEditPal::~CEditPal()
    {
    }

/**************************************************************************
	Change the range in proportion to position
**************************************************************************/

void	CEditPal::ChangeRange(int color[], int StartIter, int EndIter, BYTE *TempPal)

    {
    int	i;
    double	v;

    for (i = StartIter; i <= EndIter; ++i)
	{
	v = (double) (i -  StartIter) / (double) (EndIter - StartIter);
	*(TempPal + i * 3 + 0) = (BYTE)(color[0] + (int)(v * (double) (color[3] - color[0])) & 0x00ff);
	*(TempPal + i * 3 + 1) = (BYTE)(color[1] + (int)(v * (double) (color[4] - color[1])) & 0x00ff);
	*(TempPal + i * 3 + 2) = (BYTE)(color[2] + (int)(v * (double) (color[5] - color[2])) & 0x00ff);
	}
    }

/**************************************************************************
	Change the value of start and end values
**************************************************************************/

void	CEditPal::ChangePixel(int color[], int StartIter, int EndIter, BYTE *TempPal)

    {
    *(TempPal + StartIter * 3 + 0) = (BYTE)(color[0] & 0x00ff);
    *(TempPal + StartIter * 3 + 1) = (BYTE)(color[1] & 0x00ff);
    *(TempPal + StartIter * 3 + 2) = (BYTE)(color[2] & 0x00ff);
    *(TempPal + EndIter * 3 + 0) = (BYTE)(color[3] & 0x00ff);
    *(TempPal + EndIter * 3 + 1) = (BYTE)(color[4] & 0x00ff);
    *(TempPal + EndIter * 3 + 2) = (BYTE)(color[5] & 0x00ff);
    }

/**************************************************************************
	Invert all the colours in the range
**************************************************************************/

void	CEditPal::InvertRange(int StartIter, int EndIter, BYTE *TempPal)

    {
    int	i;

    for (i = StartIter; i <= EndIter; ++i)
	{
	*(TempPal + i * 3 + 0) = (BYTE)(~*(TempPal + i * 3 + 0));
	*(TempPal + i * 3 + 1) = (BYTE)(~*(TempPal + i * 3 + 1));
	*(TempPal + i * 3 + 2) = (BYTE)(~*(TempPal + i * 3 + 2));
	}
    }

/**************************************************************************
	Convert to grey scale
**************************************************************************/

void	CEditPal::GreyScaleRange(int StartIter, int EndIter, BYTE *TempPal)

    {
    int	i;
    int	average;

    for (i = StartIter; i <= EndIter; ++i)
	{
	average = (*(TempPal + i * 3 + 0) + *(TempPal + i * 3 + 1) + *(TempPal + i * 3 + 2)) / 3;
	*(TempPal + i * 3 + 0) = (BYTE)(average & 0x00ff);
	*(TempPal + i * 3 + 1) = (BYTE)(average & 0x00ff);
	*(TempPal + i * 3 + 2) = (BYTE)(average & 0x00ff);
	}
    }

/**************************************************************************
	Swap red green and blue for each entry
**************************************************************************/

void	CEditPal::SwapColours(int type, int StartIter, int EndIter, BYTE *TempPal)

    {
    int	  i, k, value;
     
    if (StartIter < 0 || EndIter < 0)					// we don't have a valid rule
	return;
    for (i = StartIter; i <= EndIter; ++i)
	{ 
	k = i * 3;
	switch (type)
	    {
	    case IDC_REDBLUE:
		value = *(TempPal + k); 
		*(TempPal + k) = *(TempPal + k + 2);
		*(TempPal + k + 2) = (BYTE)value;
		break;
	    case IDC_REDGREEN:
		value = *(TempPal + k); 
		*(TempPal + k) = *(TempPal + k + 1);
		*(TempPal + k + 1) = (BYTE)value;
		break;
	    case IDC_GREENBLUE:
		value = *(TempPal + k + 1); 
		*(TempPal + k + 1) = *(TempPal + k + 2);
		*(TempPal + k + 2) = (BYTE)value;
		break;
	    case IDC_ROTATE:
		value = *(TempPal + k); 
		*(TempPal + k) = *(TempPal + k + 1);
		*(TempPal + k + 1) = *(TempPal + k + 2);
		*(TempPal + k + 2) = (BYTE)value;
		break;
	    }
	}
    }

/**************************************************************************
	Char to HEX converter
**************************************************************************/

int	CEditPal::atox(char c)

    {
    if (c >= '0' && c <= '9')
	return (c - '0');
    if (c >= 'A' && c <= 'F')
	return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f')
	return (c - 'a' + 10);
    return -1;
    }

/*---------------------------------------------------------------------
Copy data to Palette Preview
-------------------------------------------------------------------*/

int	CEditPal::CopyPalPreview(HWND hwnd, BYTE *TempPal, long threshold, CPreview *PalPreview)

    {
    WORD	i, j;
    DWORD	address;
    double	MaxPal;
    DWORD	size;
    long	LocalThreshold;

    LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;

    MaxPal = (double)LocalThreshold / (double)PalPreview->PreviewWidth;

    for (i = 0; i < PalPreview->PreviewHeight; ++i)
	{
	address = (WIDTHBYTES((DWORD)PalPreview->PreviewWidth * (DWORD)PalPreview->PreviewDib.BitsPerPixel) * (DWORD)i);
	for (j = 0; j < PalPreview->PreviewWidth; ++j)
	    {
	    size = (DWORD)(MaxPal * (double)j) * 3L;
	    *(PalPreview->PreviewDib.DibPixels + address + (DWORD)j * 3L + 2) = *(TempPal + size + 0);
	    *(PalPreview->PreviewDib.DibPixels + address + (DWORD)j * 3L + 1) = *(TempPal + size + 1);
	    *(PalPreview->PreviewDib.DibPixels + address + (DWORD)j * 3L + 0) = *(TempPal + size + 2);
	    }
	}
    ShowWindow(hwnd, SW_SHOWNORMAL);
    return 0;
    }

/*---------------------------------------------------------------------
Setup colour info
-------------------------------------------------------------------*/

void	CEditPal::SetupValues(HWND hDlg, int color[], int StartIter, int EndIter, BYTE *TempPal, char *StartRGB, char *EndRGB)

    {
    short	nCtrlID;
    HWND	hCtrl;
    char	s[240];

    RedStart = *(TempPal + StartIter * 3 + 0) & 0x00FF;
    GreenStart = *(TempPal + StartIter * 3 + 1) & 0x00FF;
    BlueStart = *(TempPal + StartIter * 3 + 2) & 0x00FF;
    RedEnd = *(TempPal + EndIter * 3 + 0) & 0x00FF;
    GreenEnd = *(TempPal + EndIter * 3 + 1) & 0x00FF;
    BlueEnd = *(TempPal + EndIter * 3 + 2) & 0x00FF;
    sprintf(StartRGB, "%02X%02X%02X", RedStart, GreenStart, BlueStart);
    sprintf(EndRGB, "%02X%02X%02X", RedEnd, GreenEnd, BlueEnd);
    if (SetDlgItemInt(hDlg, IDC_STARTITER, StartIter, TRUE) == 0)
	{
	sprintf(s, "failure type %ld", GetLastError());
	MessageBox(hDlg, s, "StartIter", MB_ICONEXCLAMATION | MB_OK);
	}
    if (SetDlgItemInt(hDlg, IDC_ENDITER, EndIter, TRUE) == 0)
	{
	sprintf(s, "failure type %ld", GetLastError());
	MessageBox(hDlg, s, "EndIter", MB_ICONEXCLAMATION | MB_OK);
	}

    SetDlgItemInt(hDlg, IDC_START_RED, RedStart, TRUE);
    SetDlgItemInt(hDlg, IDC_START_GREEN, GreenStart, TRUE);
    SetDlgItemInt(hDlg, IDC_START_BLUE, BlueStart, TRUE);
    SetDlgItemInt(hDlg, IDC_END_RED, RedEnd, TRUE);
    SetDlgItemInt(hDlg, IDC_END_GREEN, GreenEnd, TRUE);
    SetDlgItemInt(hDlg, IDC_END_BLUE, BlueEnd, TRUE);
    SetDlgItemText(hDlg, IDC_STARTRGB, StartRGB);
    SetDlgItemText(hDlg, IDC_ENDRGB, EndRGB);
    color[0] = RedStart;
    color[1] = GreenStart;
    color[2] = BlueStart;
    color[3] = RedEnd;
    color[4] = GreenEnd;
    color[5] = BlueEnd;
    for (nCtrlID = IDC_STARTREDSCROLL; nCtrlID <= IDC_ENDBLUSCROLL; nCtrlID++)
	{
	hCtrl = GetDlgItem(hDlg, nCtrlID);
	SetScrollRange(hCtrl, SB_CTL, 0, 255, FALSE);
	SetScrollPos(hCtrl, SB_CTL, color[nCtrlID - IDC_STARTREDSCROLL], FALSE);
	}
    InvalidateRect(hDlg, NULL, FALSE);
    }

