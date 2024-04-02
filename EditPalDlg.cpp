//////////////////////////////////////////////////////////////////////////////////////////
//
// EDITPALDLG.CPP a module for palette editing functions.
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
#include "Plot.h"

extern	BYTE	cycleflag;				// do colour cycling
extern	long	threshold;				// maximum iterations ... called maxiter in FRACTINT
extern	PAINTSTRUCT 	ps;
extern	RECT 	r;

extern	void	DoCaption(HWND, char *);

extern	CTrueCol    TrueCol;			// palette info
extern	CPlot	Plot;		// image plotting routines 

/**************************************************************************
	Dialog Control for Setting Inside colour
**************************************************************************/

DLGPROC FAR PASCAL EditPalDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static int	    color [8] ;
     static int	    StartIter = 0;
     static int	    EndIter = 0;
     static BYTE    *TempPal = NULL;
     static CEditPal EditPal;
     static CPreview PalPreview;
     static char    StartRGB[8];
     static char    EndRGB[8];

     HWND	    hwndParent, hCtrl ;
     int	    nCtrlID, nIndex, swap, PalPtr;
     POINTS	    PalPosition;
     static BOOL    bTrans = FALSE;					// is the mouse button down?
     char	    s[480];

     switch (message)
	  {
	  case WM_INITDIALOG:
		TempPal = new BYTE [MAXTHRESHOLD * 3];			// for maximum palette
		if (TempPal == NULL)
		    {
		    MessageBox(hDlg, "Can't find memory for palette", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		    EndDialog(hDlg, 0);
		    }
		    
		PalPreview.PreviewHeight = PREVIEW_HEIGHT;
		PalPreview.PreviewWidth = PREVIEW_WIDTH;
		PalPreview.HorOffset = HOR_OFFSET;
		PalPreview.VertOffset = VERT_OFFSET;
		cycleflag = FALSE;
		EditPal.LocalThreshold = (threshold >= MAXTHRESHOLD) ? MAXTHRESHOLD - 1 : threshold;
//		memcpy(TempPal, PalettePtr, (TrueColourFlag) ? threshold * 3 : 256 * 3);
		memcpy(TempPal, TrueCol.PalettePtr, EditPal.LocalThreshold * 3);
//		*rules = '\0';
		EditPal.SetupValues(hDlg, color, StartIter, EndIter, TempPal, StartRGB, EndRGB);
		PalPreview.InitPreview(hDlg);
		EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
		PalPreview.Preview(hDlg);
		InvalidateRect(hDlg,NULL,FALSE);
		
//		SendMessage (hDlg, WM_HSCROLL, 0, 0L);
		return (DLGPROC)TRUE ;

	  case WM_HSCROLL :

	       hCtrl = GET_WM_COMMAND_HWND (wParam, (_int64)lParam);
	       if ((nCtrlID = ((WORD)(GetWindowLong (hCtrl, GWL_ID)))) == 0)
		   {
		   wsprintf(s, "Error: Scrollbar fail: <%ld>", GetLastError());
		   MessageBox (hDlg, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		   }

	       nIndex  = nCtrlID - IDC_STARTREDSCROLL;
	       hwndParent = GetParent (hDlg) ;

	       switch ((int) LOWORD(wParam))
		    {
		    case SB_PAGEDOWN :
			 color [nIndex] += 8 ;        // fall through
		    case SB_LINEDOWN :
			 color [nIndex] = min (255, color [nIndex] + 1) ;
			 break ;
		    case SB_PAGEUP :
			 color [nIndex] -= 8 ;        // fall through
		    case SB_LINEUP :
			 color [nIndex] = max (0, color [nIndex] - 1) ;
			 break ;
		    case SB_TOP:
			 color [nIndex] = 0 ;
			 break ;
		    case SB_BOTTOM :
			 color [nIndex] = 255 ;
			 break ;
		    case SB_THUMBPOSITION :
		    case SB_THUMBTRACK :
			 color[nIndex] = HIWORD (wParam) ;
			 break ;
		    default :
			 return FALSE ;
		    }
	 	SetScrollPos  (hCtrl, SB_CTL, color [nIndex], TRUE) ;
		switch (nIndex)
		    {
		    case 0:
			SetDlgItemInt (hDlg,  IDC_START_RED, color [0], TRUE) ;
			sprintf(StartRGB, "%02X%02X%02X", color [0], color [1], color [2]);
			SetDlgItemText(hDlg, IDC_STARTRGB, StartRGB);
			break;
		    case 1:
			SetDlgItemInt (hDlg,  IDC_START_GREEN, color [1], TRUE) ;
			sprintf(StartRGB, "%02X%02X%02X", color [0], color [1], color [2]);
			SetDlgItemText(hDlg, IDC_STARTRGB, StartRGB);
			break;
		    case 2:
			SetDlgItemInt (hDlg,  IDC_START_BLUE, color [2], TRUE) ;
			sprintf(StartRGB, "%02X%02X%02X", color [0], color [1], color [2]);
			SetDlgItemText(hDlg, IDC_STARTRGB, StartRGB);
			break;
		    case 3:
			SetDlgItemInt (hDlg,  IDC_END_RED, color [3], TRUE) ;
			sprintf(EndRGB, "%02X%02X%02X", color [3], color [4], color [5]);
			SetDlgItemText(hDlg, IDC_ENDRGB, EndRGB);
			break;
		    case 4:
			SetDlgItemInt (hDlg,  IDC_END_GREEN, color [4], TRUE) ;
			sprintf(EndRGB, "%02X%02X%02X", color [3], color [4], color [5]);
			SetDlgItemText(hDlg, IDC_ENDRGB, EndRGB);
			break;
		    case 5:
			SetDlgItemInt (hDlg,  IDC_END_BLUE, color [5], TRUE) ;
			sprintf(EndRGB, "%02X%02X%02X", color [3], color [4], color [5]);
			SetDlgItemText(hDlg, IDC_ENDRGB, EndRGB);
			break;
		    }
		EditPal.ChangePixel(color, StartIter, EndIter, TempPal);
		EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
		PalPreview.Preview(hDlg);
	        return (DLGPROC)TRUE ;

	  case WM_PAINT :
		BeginPaint(hDlg, &ps);
		EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
		PalPreview.Preview(hDlg);
		EndPaint(hDlg, &ps);
	        return (DLGPROC)TRUE ;

	  case WM_LBUTTONDOWN:				// message: left mouse button pressed
		PalPosition = MAKEPOINTS(lParam);
		if (PalPosition.x >= HOR_OFFSET && PalPosition.x <= HOR_OFFSET + PREVIEW_WIDTH && PalPosition.y >= VERT_OFFSET 
				&& PalPosition.y <= VERT_OFFSET + PREVIEW_HEIGHT)	// within the palette display
		    {
		    StartIter = (int)((double)(PalPosition.x - HOR_OFFSET) * ((double)EditPal.LocalThreshold) / (double)PREVIEW_WIDTH);
		    bTrans = TRUE;
		    }
	        return (DLGPROC)TRUE ;

	  
	  case WM_MOUSEMOVE:				// message: mouse movement

							// Show the current values if within palette region on the dialogue box

		PalPosition = MAKEPOINTS(lParam);
		if (PalPosition.x >= HOR_OFFSET && PalPosition.x <= HOR_OFFSET + PREVIEW_WIDTH && PalPosition.y >= VERT_OFFSET 
				&& PalPosition.y <= VERT_OFFSET + PREVIEW_HEIGHT)	// within the palette display
		    {
		    PalPtr = (int)((double)(PalPosition.x - HOR_OFFSET) * ((double)EditPal.LocalThreshold) / (double)PREVIEW_WIDTH);
		    sprintf(s, "Palette Position=%d, r=%02X, g=%02X, b=%02X", 
				PalPtr,				
				*(TempPal + PalPtr * 3 + 0),	   
				*(TempPal + PalPtr * 3 + 1), 
				*(TempPal + PalPtr * 3 + 2));
		    DoCaption (hDlg, s);
		    }
		else
		    DoCaption (hDlg, "ManpWin Colour Palette Editing");

	        return (DLGPROC)TRUE ;

	  
	  case WM_LBUTTONUP:				// message: left mouse button released
		PalPosition = MAKEPOINTS(lParam);
		if (PalPosition.x >= HOR_OFFSET && PalPosition.x <= HOR_OFFSET + PREVIEW_WIDTH && PalPosition.y >= VERT_OFFSET 
				&& PalPosition.y <= VERT_OFFSET + PREVIEW_HEIGHT  && bTrans)	// within the palette display
		    {
		    EndIter = (int)((double)(PalPosition.x - HOR_OFFSET) * ((double)EditPal.LocalThreshold) / (double)PREVIEW_WIDTH);
		    if (StartIter > EndIter)						// some silly user sweeps backwards
			{
			swap = StartIter;
			StartIter = EndIter;
			EndIter = swap;
			}

//		    HaveValidRule = TRUE;
		    EditPal.SetupValues(hDlg, color, StartIter, EndIter, TempPal, StartRGB, EndRGB);
//		    if (StartIter == EndIter)						// only one palette entry
//	    		ChangePixel(color);
		    }
		bTrans = FALSE;
		return (DLGPROC)TRUE ;

	  case WM_COMMAND:

		color [0] = GetDlgItemInt (hDlg, IDC_START_RED, &bTrans, TRUE) ;
		color [1] = GetDlgItemInt (hDlg, IDC_START_GREEN, &bTrans, TRUE) ;
		color [2] = GetDlgItemInt (hDlg, IDC_START_BLUE, &bTrans, TRUE) ;
		color [3] = GetDlgItemInt (hDlg, IDC_END_RED, &bTrans, TRUE) ;
		color [4] = GetDlgItemInt (hDlg, IDC_END_GREEN, &bTrans, TRUE) ;
		color [5] = GetDlgItemInt (hDlg, IDC_END_BLUE, &bTrans, TRUE) ;
		StartIter = GetDlgItemInt (hDlg, IDC_STARTITER, &bTrans, TRUE) ;
		EndIter   = GetDlgItemInt (hDlg, IDC_ENDITER, &bTrans, TRUE) ;
//		HaveValidRule = TRUE;

		switch ((int) LOWORD(wParam))
//		switch (wParam)
		    {
		    case IDC_RANGE:
			EditPal.ChangeRange(color, StartIter, EndIter, TempPal);
//			CaptureRules(RANGE, StartIter, EndIter);
			EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
			PalPreview.Preview(hDlg);
			break;
		    case IDC_INVERT:
			EditPal.InvertRange(StartIter, EndIter, TempPal);
//			CaptureRules(INVERT, StartIter, EndIter);
			EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
			PalPreview.Preview(hDlg);
			break;
		    case IDC_GREY:
			EditPal.GreyScaleRange(StartIter, EndIter, TempPal);
//			CaptureRules(GREY, StartIter, EndIter);
			EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
			PalPreview.Preview(hDlg);
			break;

//		    case IDC_ACCEPT:
//			DisplayRules(hDlg);
//			break;

		    case IDC_ACCEPTENTRY:
//			HaveValidRule = TRUE;
			EditPal.RedStart = *(TempPal + StartIter * 3 + 0) & 0x00FF;
			EditPal.GreenStart = *(TempPal + StartIter * 3 + 1) & 0x00FF;
			EditPal.BlueStart = *(TempPal + StartIter * 3 + 2) & 0x00FF;
			EditPal.RedEnd = *(TempPal + EndIter * 3 + 0) & 0x00FF;
			EditPal.GreenEnd = *(TempPal + EndIter * 3 + 1) & 0x00FF;
			EditPal.BlueEnd = *(TempPal + EndIter * 3 + 2) & 0x00FF;
			EditPal.ChangePixel(color, StartIter, EndIter, TempPal);
//			CaptureRules(INDIVIDUAL, StartIter, EndIter);
			break;

		    case IDC_REDBLUE:
		    case IDC_REDGREEN:
		    case IDC_GREENBLUE:
		    case IDC_ROTATE:
			EditPal.SwapColours((int) LOWORD(wParam), StartIter, EndIter, TempPal);
			EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
			PalPreview.Preview(hDlg);
			break;
		    case IDC_STRETCH:
			TrueCol.FillPalette(STRETCH, TempPal, threshold);
			EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
			PalPreview.Preview(hDlg);
			break;
		    case IDC_REPEATPAL:
			TrueCol.FillPalette(REPEAT, TempPal, threshold);
			EditPal.CopyPalPreview(hDlg, TempPal, threshold, &PalPreview);
			PalPreview.Preview(hDlg);
			break;
		    case IDOK:
			EditPal.RedStart = color [0];
			EditPal.GreenStart = color [1];
			EditPal.BlueStart = color [2];
			memcpy(TrueCol.PalettePtr, TempPal, EditPal.LocalThreshold * 3);
			TrueCol.SetTrueColourPixel(EditPal.RedStart, EditPal.GreenStart, EditPal.BlueStart, EditPal.LocalThreshold);
			Plot.RefreshScreen();			// reload screen values
			hwndParent = GetParent (hDlg) ;
    			InvalidateRect(hwndParent, &r, FALSE);	// force repaint
			TrueCol.PalEditFlag = TRUE;
			PalPreview.PreviewDib.CloseDibPtrs();
			if (TempPal) {delete [] TempPal; TempPal = NULL;}
			EndDialog (hDlg, 0);
			return (DLGPROC)TRUE;
		    case IDCANCEL:
			PalPreview.PreviewDib.CloseDibPtrs();
			if (TempPal) { delete[] TempPal; TempPal = NULL; }
			EndDialog (hDlg, 0);
			return (DLGPROC)TRUE;
		    }
	    break;
	    }
      return (DLGPROC)FALSE ;
      }



