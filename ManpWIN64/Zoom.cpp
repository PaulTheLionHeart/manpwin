/*
    Zoom.cpp (From Winfract V2.00)

    PURPOSE: Contains library routines for selecting a region

    FUNCTIONS:

	StartSelection(HWND, POINT, LPRECT, int) - begin selection area
	UpdateSelection(HWND, POINT, LPRECT, int) - update selection area
	EndSelection(POINT, LPRECT) - end selection area
	ClearSelection(HWND, LPRECT, int) - clear selection area

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "windows.h"
#include "zoom.h"
#include "manpwin.h"
#include "manp.h"
#include "BigDouble.h"
#include <stdlib.h>
#include <stdio.h>
#include "big.h"
#include "Arithmetic.h"
#include "FractalMathsTemplate.h"

extern	BOOL	NonStandardImage;

POINT	ZoomClick, ZoomCenter;
static	RECT	oldRect;

int	ClearSelection(HWND, LPRECT, int);
extern	void	SaveUndo(BOOL);


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

    if (!gManp->ZoomEdge)
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
		    + (int)((float)(ptCurrent.y - lpSelectRect->top) * (float)gManp->xdots / (float)gManp->ydots));

    if (gManp->ZoomEdge)
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
	temp = (int)(((float)ptCurrent.y - (float)ZoomCenter.y) / ((float)gManp->ydots / (float)gManp->xdots));
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
		    + (int)((float)(ptCurrent.y - lpSelectRect->top) * (float)gManp->xdots / (float)gManp->ydots));

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
    if (prc->right < prc->left) SWAP(prc->right, prc->left);
    if (prc->bottom < prc->top)  SWAP(prc->bottom, prc->top);
    }

/**************************************************************************
    Update Fractint Inverse Julia six-corner geometry after ZoomIn()

    Map the selected screen area back into the original Fractint
    parallelogram, preserving rotation, skew and horizontal stretching.

    ZoomIn() has already updated hor, vert and mandel_width.
    This routine updates only the six Fractint corners used by
    setup_convert_to_screen().

    The original corners must still be intact when this is called.
**************************************************************************/

void CManp::UpdateFractintIJZoomIn(HWND hwnd, const RECT& Rect)
    {
    RECT r = Rect;
    NormalizeRect(&r);

    if (NonStandardImage)
	{
	int ScrollX = GetScrollPos(hwnd, SB_HORZ);
	int ScrollY = GetScrollPos(hwnd, SB_VERT);

	r.left += ScrollX;
	r.right += ScrollX;
	r.top += ScrollY;
	r.bottom += ScrollY;
	}

    double Width = (double)xdots - 1.0;
    double Height = (double)ydots - 1.0;

    if (Width <= 0.0 || Height <= 0.0 || AspectRatio == 0.0)
	return;

    double CentreX = (r.left + r.right) / 2.0;
    double CentreY = (r.top + r.bottom) / 2.0;

    // Match the scale used by ZoomIn().
    double Scale = (double)(r.bottom - r.top) / (double)ydots;

    // Fractint's original 4:3 horizontal correction.
    double Correction = (4.0 / 3.0) / AspectRatio;

    // Preserve the original three defining corners.
    Complex UpperLeft, LowerLeft, LowerRight;

    UpperLeft.x = xxmin;
    UpperLeft.y = yymax;

    LowerLeft.x = xx3rd;
    LowerLeft.y = yy3rd;

    LowerRight.x = xxmax;
    LowerRight.y = yymin;

    // Convert a displayed screen coordinate back into the
    // original Fractint parallelogram.
    auto ScreenToFractal = [&](double ScreenX, double ScreenY) -> Complex
	{
	double ScreenCentre = Width / 2.0;

	double RawX = ScreenCentre
	    + (ScreenX - ScreenCentre) / Correction;

	double u = RawX / Width;
	double v = ScreenY / Height;

	Complex Point;

	Point.x = UpperLeft.x + u * (LowerRight.x - LowerLeft.x) + v * (LowerLeft.x - UpperLeft.x);
	Point.y = UpperLeft.y + u * (LowerRight.y - LowerLeft.y) + v * (LowerLeft.y - UpperLeft.y);

	return Point;
	};

    // Find the selected centre in Fractint coordinates.
    Complex NewCentre = ScreenToFractal(CentreX, CentreY);

    // Find the centre of the original parallelogram.
    Complex OldCentre;

    OldCentre.x = (UpperLeft.x + LowerRight.x) / 2.0;
    OldCentre.y = (UpperLeft.y + LowerRight.y) / 2.0;

    // Apply uniform scaling to preserve rotation, skew and aspect ratio.
    Complex NewUpperLeft, NewLowerLeft, NewLowerRight;

    NewUpperLeft.x = NewCentre.x + Scale * (UpperLeft.x - OldCentre.x);
    NewUpperLeft.y = NewCentre.y + Scale * (UpperLeft.y - OldCentre.y);

    NewLowerLeft.x = NewCentre.x + Scale * (LowerLeft.x - OldCentre.x);
    NewLowerLeft.y = NewCentre.y + Scale * (LowerLeft.y - OldCentre.y);

    NewLowerRight.x = NewCentre.x + Scale * (LowerRight.x - OldCentre.x);
    NewLowerRight.y = NewCentre.y + Scale * (LowerRight.y - OldCentre.y);

    // Replace the working Fractint corners.
    xxmin = NewUpperLeft.x;
    yymax = NewUpperLeft.y;

    xx3rd = NewLowerLeft.x;
    yy3rd = NewLowerLeft.y;

    xxmax = NewLowerRight.x;
    yymin = NewLowerRight.y;
    }

/**************************************************************************
    Update Fractint Inverse Julia six-corner geometry after ZoomOut()

    Expand the current Fractint viewing parallelogram around its centre,
    preserving rotation, skew and horizontal stretching.

    ZoomOut() has already updated hor, vert and mandel_width.
    This routine updates only the six Fractint corners used by
    setup_convert_to_screen().

    The original corners must still be intact when this is called.
**************************************************************************/

void CManp::UpdateFractintIJZoomOut(HWND hwnd, const RECT& Rect)
    {
    RECT r = Rect;
    NormalizeRect(&r);

    double BoxHeight = (double)(r.bottom - r.top);

    if (BoxHeight <= 5.0)
	return;

    // Match the expansion factor used by ZoomOut().
    double Scale = (double)ydots / BoxHeight;

    // Preserve the current three defining corners.
    Complex UpperLeft, LowerLeft, LowerRight;

    UpperLeft.x = xxmin;
    UpperLeft.y = yymax;

    LowerLeft.x = xx3rd;
    LowerLeft.y = yy3rd;

    LowerRight.x = xxmax;
    LowerRight.y = yymin;

     // Expand the parallelogram uniformly around its centre.
    Complex OldCentre;

    OldCentre.x = (UpperLeft.x + LowerRight.x) / 2.0;
    OldCentre.y = (UpperLeft.y + LowerRight.y) / 2.0;

    Complex NewUpperLeft, NewLowerLeft, NewLowerRight;

    NewUpperLeft.x = OldCentre.x + Scale * (UpperLeft.x - OldCentre.x);
    NewUpperLeft.y = OldCentre.y + Scale * (UpperLeft.y - OldCentre.y);

    NewLowerLeft.x = OldCentre.x + Scale * (LowerLeft.x - OldCentre.x);
    NewLowerLeft.y = OldCentre.y + Scale * (LowerLeft.y - OldCentre.y);

    NewLowerRight.x = OldCentre.x + Scale * (LowerRight.x - OldCentre.x);
    NewLowerRight.y = OldCentre.y + Scale * (LowerRight.y - OldCentre.y);

    // Update the working Fractint corners.
    xxmin = NewUpperLeft.x;
    yymax = NewUpperLeft.y;

    xx3rd = NewLowerLeft.x;
    yy3rd = NewLowerLeft.y;

    xxmax = NewLowerRight.x;
    yymin = NewLowerRight.y;
    }

/****************************************************************************

    FUNCTION: ZoomIn(HWND, LPRECT, int)

    PURPOSE: Zoom In by setting horizontal, vertical and width

****************************************************************************/

#ifdef	_DEBUG
extern	void	ShowBignum(BigDouble, char *);
#endif

int	CManp::ZoomIn(HWND hwnd, RECT *Rect)
    {
    double	width_factor;
    int		ytop, ybottom, xleft, xright;
    BYTE	WasBigNum = FALSE;
    double	CentreXPixel, CentreYPixel;
    Complex	NewCentre;
    BigComplex	NewBigCentre;

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
    width_factor = (double) (abs(ybottom - ytop)) / (double) ydots;
    // A zoom box is selected in screen coordinates.  When the view is rotated,
    // transform the box centre back into fractal coordinates before rebuilding
    // the new view around that point.
    CentreXPixel = ((double)xleft + (double)xright) / 2.0;
    CentreYPixel = ((double)ytop + (double)ybottom) / 2.0;

    if (BigNumFlag)
	{
	BigComplex Point;

	Point.x = BigHor + BigWidth * (CentreXPixel / (double)ydots);
	Point.y = BigVert + BigWidth * ((ydots - CentreYPixel) / (double)ydots);

	if (RotationAngle == NORMAL)
	    NewBigCentre = Point;
	else
	    NewBigCentre = AffineTransformT<BigComplex, BigDouble>(Point, BigViewAffineParams);

	BigWidth = BigWidth * width_factor;
	BigHor = NewBigCentre.x - (BigWidth * AspectRatio) / 2.0;
	BigVert = NewBigCentre.y - BigWidth / 2.0;

	mandel_width = mandel_width * width_factor;
	}
    else
	{
	Complex Point;

	Point.x = hor + mandel_width * CentreXPixel / (double)ydots;
	Point.y = vert + mandel_width * (ydots - CentreYPixel) / (double)ydots;

	if (RotationAngle == NORMAL)
	    NewCentre = Point;
	else
	    NewCentre = AffineTransformT<Complex, double>(Point, ViewAffineParams);

	mandel_width = mandel_width * width_factor;
	hor = NewCentre.x - (mandel_width * AspectRatio) / 2.0;
	vert = NewCentre.y - mandel_width / 2.0;
	}

    if (calcfracinit() < 0)				// purely to make sure we have enough precision
	{
	MessageBox (hwnd, "Ran out of resolution", "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return FALSE; 
	}

    // The view geometry has changed, so rebuild the rotation centre and affine parameters for the new view.
    if (RotationAngle != NORMAL)
	SetViewAffineRotation();

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

int	CManp::ZoomOut(HWND hwnd, RECT *Rect)
    {
    double	half_win_aspect, width_factor;
    double	centrex, centrey;				// centre of window
    int		ytop, ybottom, xleft, xright;                                                         
    BYTE	WasBigNum = FALSE;
    BigDouble	Big_temp_hor, Big_temp_vert, Big_temp_width;	// store info if digits change
    BigDouble	Big_centrex, Big_centrey;
								// End the selection
    bTrack = FALSE;
    if (abs(Rect->bottom - Rect->top ) <= 5 || abs(Rect->right  - Rect->left) <= 5)
	return FALSE; 
    NormalizeRect(Rect);
    SaveUndo(FALSE);						// save original to UNDO
    ytop    = Rect->top;
    ybottom = Rect->bottom;
    xleft   = Rect->left;
    xright  = Rect->right;
								// adjust the zoom-box for aspect ratio
    half_win_aspect = AspectRatio * 0.5;
    width_factor = 1.0 / (double) (abs(ybottom - ytop)) * (double) ydots;

    if (BigNumFlag)
	{
	WasBigNum = TRUE;
	// Zoom out about the current fractal centre.  Rotation does not change the
	// centre itself, only the orientation of the view.
	Big_centrex = BigHor + (BigWidth * half_win_aspect);	// centre 
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
	// Zoom out about the current fractal centre.  Rotation does not change the
	// centre itself, only the orientation of the view.
	centrex = hor + (mandel_width * half_win_aspect);	// centre 
	centrey = vert + (mandel_width * 0.5);
	mandel_width = mandel_width * width_factor;		// new width
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

    // Rebuild rotation state after changing the view size.
    if (RotationAngle != NORMAL)
	SetViewAffineRotation();
	
    SaveUndo(TRUE);						// save new to temp UNDO buffer
    return TRUE;
    }


