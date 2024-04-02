/****************************************************************************

    PROGRAM: Zoom.cpp (From Winfract V2.00)

    PURPOSE: Contains library routines for selecting a region

    FUNCTIONS:

	StartSelection(HWND, POINT, LPRECT, int) - begin selection area
	UpdateSelection(HWND, POINT, LPRECT, int) - update selection area
	EndSelection(POINT, LPRECT) - end selection area
	ClearSelection(HWND, LPRECT, int) - clear selection area

*******************************************************************************/

#include "windows.h"
#include "zoom.h"
#include "manpwin.h"
#include "BigDouble.h"
#include <stdlib.h>
#include <stdio.h>
#include "big.h"

extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address
extern	double	mandel_width;			// width of display
extern	double	yymax;				// vert + width
extern	BOOL	bTrack;				// TRUE if user is selecting a region
extern	BOOL	ZoomEdge;			// Zooming process
extern	int	xdots, ydots;
extern	double	RotationAngle;
extern	double	ScreenRatio;

extern	BOOL	NonStandardImage;

POINT	ZoomClick, ZoomCenter;
static	RECT	oldRect;

int	ClearSelection(HWND, LPRECT, int);
extern	int	calcfracinit(void);
extern	void	SaveUndo(BOOL);
extern	int	BackupVars(void);
extern	int	RestoreVars(void);
//extern	int	analyse_corner(char *);

// Big num declarations **********************************************************
extern	int dec;
extern	BYTE	BigNumFlag;		// True if bignum used
extern	BigDouble   BigHor, BigVert, BigWidth;
// Big num declarations **********************************************************


/****************************************************************************

    FUNCTION: StartSelection(HWND, POINT, LPRECT, int)

    PURPOSE: Begin selection of region

****************************************************************************/

int	StartSelection(HWND hWnd, POINTS ptCurrent, LPRECT lpSelectRect, int fFlags)

    {
    if (lpSelectRect->left != lpSelectRect->right || lpSelectRect->top != lpSelectRect->bottom)
	ClearSelection(hWnd, lpSelectRect, fFlags);

    lpSelectRect->right = ptCurrent.x;
    lpSelectRect->bottom = ptCurrent.y;

    // If you are extending the box, then invert the current rectangle

    if ((fFlags & SL_SPECIAL) == SL_EXTEND)
	ClearSelection(hWnd, lpSelectRect, fFlags);

    // Otherwise, set origin to current location

    else 
	{
	lpSelectRect->left = ptCurrent.x;
	lpSelectRect->top = ptCurrent.y;
	}

    if (!ZoomEdge)
	{
	ZoomCenter.x = ptCurrent.x;
	ZoomCenter.y = ptCurrent.y;
	oldRect.right = ptCurrent.x;
	oldRect.left = ptCurrent.x;
	oldRect.top = ptCurrent.y;
	oldRect.bottom = ptCurrent.y;
	}

    SetCapture(hWnd);
    return 0;
    }

/****************************************************************************

    FUNCTION: UpdateSelection(HWND, POINT, LPRECT, int) - update selection area

    PURPOSE: Update selection

****************************************************************************/

int	UpdateSelection(HWND hWnd, POINTS ptCurrent, LPRECT lpSelectRect, int fFlags)

    {
    HDC hDC;
    short OldROP;
    hDC = GetDC(hWnd);
    int	temp;

		// maintain aspect ratio by only using ptCurrent.y and calculating ptCurrent.x
    ptCurrent.x = (short)(lpSelectRect->left 
		    + (int)((float)(ptCurrent.y - lpSelectRect->top) * (float)xdots / (float)ydots));

    if (ZoomEdge)
	{
	switch (fFlags & SL_TYPE) 
	    {
	    case SL_BOX:
		OldROP = SetROP2(hDC, R2_NOTXORPEN);
//		MoveTo(hDC, lpSelectRect->left, lpSelectRect->top);
		MoveToEx(hDC, lpSelectRect->left, lpSelectRect->top, NULL);
		LineTo(hDC, lpSelectRect->right, lpSelectRect->top);
		LineTo(hDC, lpSelectRect->right, lpSelectRect->bottom);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->bottom);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->top);
		LineTo(hDC, ptCurrent.x, lpSelectRect->top);
		LineTo(hDC, ptCurrent.x, ptCurrent.y);
		LineTo(hDC, lpSelectRect->left, ptCurrent.y);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->top);
		SetROP2(hDC, OldROP);
		break;
        
	    case SL_BLOCK:
		PatBlt(hDC,
		    lpSelectRect->left,
		    lpSelectRect->bottom,
		    lpSelectRect->right - lpSelectRect->left,
		    ptCurrent.y - lpSelectRect->bottom,
		    DSTINVERT);
		PatBlt(hDC,
		    lpSelectRect->right,
		    lpSelectRect->top,
		    ptCurrent.x - lpSelectRect->right,
		    ptCurrent.y - lpSelectRect->top,
	  	    DSTINVERT);
		break;
	    }
	}
    else
	{
	temp = (int)(((float)ptCurrent.y - (float)ZoomCenter.y) / ((float)ydots / (float)xdots));
//	lpSelectRect->left = ptCurrent.x - (ptCurrent.x - ZoomCenter.x) * 2;
//	lpSelectRect->top = ptCurrent.y - (ptCurrent.y - ZoomCenter.y) * 2;
	lpSelectRect->left = ZoomCenter.x - temp;
	lpSelectRect->right = ZoomCenter.x + temp;
	lpSelectRect->top = ptCurrent.y - (ptCurrent.y - ZoomCenter.y) * 2;
	switch (fFlags & SL_TYPE) 
	    {
	    case SL_BOX:
		OldROP = SetROP2(hDC, R2_NOTXORPEN);
//		MoveTo(hDC, lpSelectRect->left, lpSelectRect->top);
		MoveToEx(hDC, lpSelectRect->left, lpSelectRect->top, NULL);
		LineTo(hDC, lpSelectRect->right, lpSelectRect->top);
		LineTo(hDC, lpSelectRect->right, lpSelectRect->bottom);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->bottom);
		LineTo(hDC, lpSelectRect->left, lpSelectRect->top);

		LineTo(hDC, oldRect.right, oldRect.top);
		LineTo(hDC, oldRect.right, oldRect.bottom);
		LineTo(hDC, oldRect.left, oldRect.bottom);
		LineTo(hDC, oldRect.left, oldRect.top);
		SetROP2(hDC, OldROP);
		break;
        
	    case SL_BLOCK:
		PatBlt(hDC,
		    lpSelectRect->left,
		    lpSelectRect->bottom,
		    lpSelectRect->right - lpSelectRect->left,
		    lpSelectRect->top - lpSelectRect->bottom,
		    DSTINVERT);
		PatBlt(hDC,
		    oldRect.left,
		    oldRect.bottom,
		    oldRect.right - oldRect.left,
		    oldRect.top - oldRect.bottom,
		    DSTINVERT);
		break;
	    }

	oldRect.right = lpSelectRect->right;
	oldRect.left = lpSelectRect->left;
	oldRect.top = lpSelectRect->top;
	oldRect.bottom = lpSelectRect->bottom;
	}
    lpSelectRect->right = ptCurrent.x;
    lpSelectRect->bottom = ptCurrent.y;
    ReleaseDC(hWnd, hDC);
    return 0;
    }

/****************************************************************************

    FUNCTION: EndSelection(POINT, LPRECT)

    PURPOSE: End selection of region, release capture of mouse movement

****************************************************************************/

int	EndSelection(POINTS ptCurrent, LPRECT lpSelectRect)

    {
		// maintain aspect ratio by only using ptCurrent.y and calculating ptCurrent.x
    ptCurrent.x = (short)(lpSelectRect->left 
		    + (int)((float)(ptCurrent.y - lpSelectRect->top) * (float)xdots / (float)ydots));

    lpSelectRect->right = ptCurrent.x;
    lpSelectRect->bottom = ptCurrent.y;
    ReleaseCapture();
    return 0;
    }

/****************************************************************************

    FUNCTION: ClearSelection(HWND, LPRECT, int) - clear selection area

    PURPOSE: Clear the current selection

****************************************************************************/

int	ClearSelection(HWND hWnd, LPRECT lpSelectRect, int fFlags)

    {
    HDC hDC;
    short OldROP;

    hDC = GetDC(hWnd);
    switch (fFlags & SL_TYPE) 
	{
	case SL_BOX:
	    OldROP = SetROP2(hDC, R2_NOTXORPEN);
//	    MoveTo(hDC, lpSelectRect->left, lpSelectRect->top);
	    MoveToEx(hDC, lpSelectRect->left, lpSelectRect->top, NULL);
	    LineTo(hDC, lpSelectRect->right, lpSelectRect->top);
	    LineTo(hDC, lpSelectRect->right, lpSelectRect->bottom);
	    LineTo(hDC, lpSelectRect->left, lpSelectRect->bottom);
	    LineTo(hDC, lpSelectRect->left, lpSelectRect->top);
	    SetROP2(hDC, OldROP);
	    break;

	case SL_BLOCK:
	    PatBlt(hDC,
		    lpSelectRect->left,
		    lpSelectRect->top,
		    lpSelectRect->right - lpSelectRect->left,
		    lpSelectRect->bottom - lpSelectRect->top,
		    DSTINVERT);
	    break;
	}
    ReleaseDC(hWnd, hDC);
    return 0;
    }

//*************************************************************************
//
//  FUNCTION   : NormalizeRect(RECT *prc)
//
//  PURPOSE    : If the rectangle coordinates are reversed, swaps them.
//               This is used to make sure that the first coordinate of
//               our rect is the upper left, and the second is lower right.
//
//*************************************************************************


// Macro to swap two values
#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))

void WINAPI NormalizeRect(RECT *prc)
    {
    if (prc->right  < prc->left) SWAP(prc->right,  prc->left);
    if (prc->bottom < prc->top)  SWAP(prc->bottom, prc->top);
    }

/****************************************************************************

    FUNCTION: ZoomIn(HWND, LPRECT, int)

    PURPOSE: Zoom In by setting horizontal, vertical and width

****************************************************************************/

#ifdef	_DEBUG
extern	void	ShowBignum(BigDouble, char *);
#endif

int	ZoomIn(HWND hwnd, RECT *Rect)

    {
    double	hor_factor, vert_factor, width_factor;
    int		ytop, ybottom, xleft, xright;
    BYTE	WasBigNum = FALSE;
    double	xCentreOld, yCentreOld;
    double	xCentreNew, yCentreNew;
    double	SinOrientation = sin(RotationAngle / 57.29577);	// Convert to radians
    double	CosOrientation = cos(RotationAngle / 57.29577);

    // End the selection
    bTrack = FALSE;
    if (abs(Rect->bottom - Rect->top) <= 5 || abs(Rect->right  - Rect->left) <= 5)
	return FALSE; 

    NormalizeRect(Rect);
    SaveUndo(FALSE);					// save original to UNDO
    ytop    = Rect->top;
    ybottom = Rect->bottom;
    xleft   = Rect->left;
    xright  = Rect->right;

    if (NonStandardImage)
	{
	ytop    += GetScrollPos(hwnd, SB_VERT);
	ybottom += GetScrollPos(hwnd, SB_VERT);
	xleft   += GetScrollPos(hwnd, SB_HORZ);
	xright  += GetScrollPos(hwnd, SB_HORZ);
	}
							    // adjust the zoom-box for aspect ratio
    hor_factor = (double) xleft / (double) ydots;
    vert_factor = (double) (ydots - ybottom) / (double) ydots;
    width_factor = (double) (abs(ybottom - ytop)) / (double) ydots;

    if (RotationAngle != 0)		// we have to work out where we are in rotated space
	{
	// let's get the original centre of rotation first
	xCentreOld = hor + mandel_width * ScreenRatio / 2;
	yCentreOld = -vert - mandel_width / 2;
	}
    if (BigNumFlag)
	{
//    ShowBignum(BigWidth, "BigWidth 1");
	WasBigNum = TRUE;
	BigHor = BigHor + BigWidth * hor_factor;
	BigVert = BigVert + BigWidth * vert_factor;
	BigWidth = BigWidth * width_factor;
	mandel_width = mandel_width * width_factor;	// do we really need BigWidth?
//    ShowBignum(BigWidth, "BigWidth 2");
	}
    else
	{
	hor = hor + mandel_width * hor_factor;
	vert = vert + mandel_width * vert_factor;
	mandel_width = mandel_width * width_factor;
	}

    if (RotationAngle != 0)		// we have to work out where we are in rotated space
	{
	double	cx, cy;
	// let's get the new centre of screen 
	cx = hor + mandel_width * ScreenRatio / 2;
	cy = -vert - mandel_width / 2;
	// rotate the new centre around the old centre
	xCentreNew = xCentreOld + CosOrientation * (-cx + xCentreOld) + SinOrientation * (-cy + yCentreOld);
	yCentreNew = yCentreOld + SinOrientation * (-cx + xCentreOld) + CosOrientation * (-cy + yCentreOld);
//    double dbD0r = mr + m_C * (xpos - mr) + m_S * (nYPos - mi);
//    double dbD0i = mi - m_S * (xpos - mr) + m_C * (nYPos - mi);
	// calculate the new values
	hor = xCentreNew - mandel_width * ScreenRatio / 2;
	vert = -yCentreNew - mandel_width / 2;
//	mandel_width *= CosOrientation;
	}

    if (calcfracinit() < 0)				// purely to make sure we have enough precision
	{
	MessageBox (hwnd, "Ran out of resolution", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return FALSE; 
	}

    if (WasBigNum)
	{
    //ShowBignum(BigHor, "BigHor");
    //ShowBignum(BigVert, "BigVert");
    //ShowBignum(BigWidth, "BigWidth 3");
	}

    SaveUndo(TRUE);							// save new to temp UNDO buffer
    return TRUE;
    }

/****************************************************************************

    FUNCTION: ZoomOut(HWND, LPRECT, int)

    PURPOSE: Zoom Out by setting horizontal, vertical and width

****************************************************************************/

int	ZoomOut(HWND hwnd, RECT *Rect)

    {
    double	half_win_aspect, width_factor;
    double	centrex, centrey;				// centre of window
    int	ytop, ybottom, xleft, xright;                                                         
    BYTE	WasBigNum = FALSE;
    BigDouble	Big_temp_hor, Big_temp_vert, Big_temp_width;	// store info if digits change
    BigDouble	Big_centrex, Big_centrey;
							    // End the selection
    bTrack = FALSE;
    if (abs(Rect->bottom - Rect->top ) <= 5 || abs(Rect->right  - Rect->left) <= 5)
	return FALSE; 
    NormalizeRect(Rect);
    SaveUndo(FALSE);					// save original to UNDO
    ytop    = Rect->top;
    ybottom = Rect->bottom;
    xleft   = Rect->left;
    xright  = Rect->right;
						    // adjust the zoom-box for aspect ratio
    half_win_aspect = (double)xdots/(double)ydots * 0.5;
    width_factor = 1.0 / (double) (abs(ybottom - ytop)) * (double) ydots;

    if (BigNumFlag)
	{
	WasBigNum = TRUE;
	Big_centrex = BigHor + (BigWidth * half_win_aspect);		// centre 
	Big_centrey = BigVert + (BigWidth * 0.5);
	BigWidth = BigWidth * width_factor;			// new width
	BigHor = Big_centrex - BigWidth * half_win_aspect;
	BigVert = Big_centrey - BigWidth * 0.5;

	Big_temp_hor = BigHor;					// restore if precision changes
	Big_temp_vert = BigVert;
	Big_temp_width = BigWidth;
	}
    else
	{
	centrex = hor + (mandel_width * half_win_aspect);		// centre 
	centrey = vert + (mandel_width * 0.5);
	mandel_width = mandel_width * width_factor;			// new width
	hor = centrex - mandel_width * half_win_aspect;
	vert = centrey - mandel_width * 0.5;
	}

    if (calcfracinit() < 0)					// purely to make sure we have enough precision
	return FALSE; 
    if (WasBigNum)
	{
	BigHor = Big_temp_hor;
	BigVert = Big_temp_vert;
	BigWidth = Big_temp_width;
	}
    SaveUndo(TRUE);							// save new to temp UNDO buffer
    return TRUE;
    }


