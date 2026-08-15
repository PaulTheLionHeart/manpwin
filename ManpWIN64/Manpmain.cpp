/*
    MANPMAIN.CPP - Main image initlaisaation

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

/**************************************************************************
	MANPMAIN.CPP
	Main graphics file decoder
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <windows.h>
#include <fcntl.h>
#include <vector>
#include <atomic>
#define  VIEWMAIN
#include "manpwin.h"
#include "resource.h"
#include "Dib.h"
#include "Plot.h"
#include "manp.h"
						// routines in this module
void	putline(WORD, BYTE *);

void	GetRealClientRect (HWND, PRECT);
void	SetScrollRanges(HWND);

HPALETTE 	 	hpal = NULL;

extern	void	SetupView(HWND);
extern	void	ClosePtrs(void);
extern	int	CopyPictureToClipboard(HWND);
extern	void	HardStopNow(HWND hwnd, const char* reason);

extern	int     file_type;

extern	HWND	CallingWindowHandle;		// Is ManpWIN called by an external window via WM_COPYDATA message?
extern	int	ReplyUsingDIB;			// 2 for Device Context, 1 for DIB/WM_COPYDATA and 0 for clipboard

extern	int	ThreadCompletionDelay;
extern	int	ThreadEndingDelay;
extern	std::atomic<bool> gStopRequested;	// force early exit

int		fdin;				// FILE descripter
int		display_width, display_height;
int		max_vscroll, max_hscroll;
int		mapentrysize = 0;		// 0 indicates no colormap
WORD		colours;			// colours in the file
DWORD		screen_colours;			// colours in the graphics card
WORD     	iNumColors;    			// Number of colors supported by device
long		eofpos;				// position of end of file   
DWORD		biCompression;			// BMP type compression
BYTE		line_buf[MAXHORIZONTAL * 3];    // true colour = 3 bytes per pixel

BOOL		NonStandardImage;		// has user changed image size?
WORD		NewXdots = 800, NewYdots = 450;	// for non standard image sizes

/**************************************************************************
	Main entry decoder
**************************************************************************/

int	mainview(HWND hwnd, BOOL FileFlag)
    {
    char		*szAppName = "Paul's Fractals";
    HANDLE		hfiledata = (HANDLE)NULL;			
    BYTE	 huge	*filedata = NULL;				// handle to the file data
//    BOOL		FullScreen = FALSE;

    screen_colours = 1L << (GetDeviceCaps(gManp->ps.hdc, BITSPIXEL) * GetDeviceCaps(gManp->ps.hdc, PLANES));
    iNumColors = (WORD) screen_colours;

// get screen metrics
    SystemParametersInfo(SPI_GETWORKAREA, 0, &gManp->WARect, 0);	// let's get usable area including task bar

    gManp->screenx = gManp->WARect.right - gManp->WARect.left;
    gManp->screeny = gManp->WARect.bottom - gManp->WARect.top;

			// Some initial parameters
    if (!FileFlag)					// image from PNG file, so don't splatter height and width
	{
	if (NonStandardImage/* && !IsZoomed(hwnd) == FALSE*/)
	    {
	    gManp->height = NewYdots;
	    gManp->width = NewXdots;
	    }
	else
	    {
	    gManp->height = gManp->screeny - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYMENU) - GetSystemMetrics(SM_CYHSCROLL);
	    gManp->width = gManp->screenx - GetSystemMetrics(SM_CXHSCROLL);
	    gManp->height -= GetSystemMetrics(SM_CYMENU);				    // remember status bar
	    }
	}
//    gManp->AspectRatio = (double)gManp->width / (double)gManp->height;
    
        // Compute the size of the window rectangle based on the given
        // client rectangle size and the window style, then size the window.
         
    GetClientRect(hwnd, &gManp->r);
    gManp->ptSize.x = gManp->width;
    gManp->ptSize.y = gManp->height;

    if (IsZoomed (hwnd))
	SetScrollRanges (hwnd);
    gManp->xdots = gManp->width;
    gManp->ydots = gManp->height;
    gManp->AspectRatio = (double)gManp->xdots / (double)gManp->ydots;

    HardStopNow(hwnd, "image size change");

    if (gManp != nullptr)
	gManp->WaitForAllThreadsToFinish();
    SetupView(hwnd);

    ClosePtrs();							    // ready for next screen

//    gManp->WaitForPixelThreadsToFinish();   // MUST be first
    if (gManp->Dib.InitDib(gManp->width, gManp->height, BITSPERPIXEL) == NULL)
	{
	switch(gManp->Dib.DibErrorCode)
	    {
	    case NODIBMEMORY:
		MessageBox (hwnd, "Can't allocate DIB memory. Using windows default palette", szAppName, MB_ICONEXCLAMATION | MB_OK);
		break;
	    case NOPIXELMEMORY:
		MessageBox (hwnd, "Can't allocate memory for pixels", szAppName, MB_ICONEXCLAMATION | MB_OK);
		break;
	    }
	return -1;
	}
    else
	{
	gManp->wpixels.clear();
	gManp->wpixels.resize((size_t)gManp->xdots * (size_t)gManp->ydots, 0.0f);
	gManp->ClearScreen();
	}
    // reset all the image parameters for the pixel plotting routines
    gManp->Plot.InitPlot(gManp->threshold, &gManp->TrueCol, &gManp->wpixels, gManp->xdots, gManp->ydots, gManp->xdots, gManp->ydots, gManp->Dib.BitsPerPixel, &gManp->Dib, USEPALETTE + USEWPIXELS);
    gStopRequested.store(false, std::memory_order_relaxed);

    return (TRUE);
    }

/**************************************************************************
	Clear Screen 
**************************************************************************/

void	CManp::ClearScreen(void)
    {
    if (Dib.DibPixels.empty())
	{
	OutputDebugStringA("ClearScreen(): Dib not valid\n");
	return;
	}
    std::fill(wpixels.begin(), wpixels.end(), static_cast<float>(0.0));
    memset(Dib.DibPixels.data(), 0, ComputeWidthBytes((DWORD)width, (DWORD)Dib.BitsPerPixel) * (DWORD)height);
    } 

/**************************************************************************
	Send WM_COPYDATA message with Dib 
**************************************************************************/

int	SendCopydataMessage(HWND hwnd, char *szAppName)
    {
    static	COPYDATASTRUCT	FracData;		// structure for sending data back to calling application
    static	BYTE		*MessageArray = NULL;
    char			t[640];

    FracData.dwData = 9L;

    switch (ReplyUsingDIB)
	{
	case 2:				// Device Context version
	    FracData.cbData = sizeof(HDC);
	    FracData.lpData = GetDC(hwnd);
	    break;

	case 1:				// WM_COPYDATA version
	    if (MessageArray)			// memory from last message
		GlobalFree(MessageArray);
	    FracData.cbData = gManp->Dib.pDibInf->bmiHeader.biSize + gManp->Dib.pDibInf->bmiHeader.biSizeImage;
	    if ((MessageArray = (BYTE *)GlobalAlloc(GMEM_FIXED, gManp->Dib.pDibInf->bmiHeader.biSize + gManp->Dib.pDibInf->bmiHeader.biSizeImage + 4096L)) == NULL)
		{
		_snprintf_s(t, 640, _TRUNCATE, "Can't allocate memory for DIB Message");
		MessageBox (hwnd, t, szAppName, MB_ICONEXCLAMATION | MB_OK);
		MessageArray = NULL;
		MessageBeep (0);
		return -1;
		}
	    memcpy(MessageArray, gManp->Dib.pDibInf, gManp->Dib.pDibInf->bmiHeader.biSize);
	    memcpy(MessageArray + gManp->Dib.pDibInf->bmiHeader.biSize, gManp->Dib.DibPixels.data(), gManp->Dib.pDibInf->bmiHeader.biSizeImage);

	    FracData.lpData = MessageArray;

#ifdef DEBUG
	test = (LPBITMAPINFO) FracData.lpData;
	_snprintf_s(s, 200, _TRUNCATE, "Header info\nHeader Size (4 Bytes) = %ld\nWidth (4 Bytes) = %ld\nHeight (4 Bytes) = %ld\nImage Size (4 Bytes) = %ld\nPlanes (2 Bytes) = %d\nBits/pixel (2 Bytes) = %d\nCompression (4 Bytes) = %ld\nXPels (4 Bytes) = %ld\nYPels (4 Bytes) = %ld\nColours used (4 Bytes) = %ld\nColours impotrant (4 Bytes) = %ld\n",
		test->bmiHeader.biSize,
		test->bmiHeader.biWidth,
		test->bmiHeader.biHeight,
		test->bmiHeader.biSizeImage,
		test->bmiHeader.biPlanes,
		test->bmiHeader.biBitCount,
		test->bmiHeader.biCompression,
		test->bmiHeader.biXPelsPerMeter,
		test->bmiHeader.biYPelsPerMeter,
		test->bmiHeader.biClrUsed,
		test->bmiHeader.biClrImportant);    




	MessageBox (hwnd, t, "CopyData", MB_ICONEXCLAMATION | MB_OK);
#endif        
	    break;
	case 0:				// clipboard version
	    FracData.cbData = 0L;
	    FracData.lpData = '\0';
	    if (CopyPictureToClipboard(hwnd) == -1) 
		{
		MessageBox (hwnd, "Error copying image to the clipboard!", szAppName, MB_ICONEXCLAMATION | MB_OK);
		return 0;
		}
	    break;
	}

    SendMessage (CallingWindowHandle, WM_COPYDATA, (WPARAM)hwnd, (LPARAM)&FracData);
    return 0;
    }

/**************************************************************************
	Set line array to Dib memory
**************************************************************************/

void putline(WORD row, BYTE *buf)
    {
    long	diff;
    long	local_width;
    DWORD	i;
    WORD	j;
    BYTE	c;

    if (gManp->Dib.BitsPerPixel == 24)			// swap red and blue
	for (j = 0; j < gManp->width; ++j)                 // the order is reversed to prevent silly colours????
	    {
	    c = *(buf + j * 3);
	    *(buf + j * 3) = *(buf + j * 3 + 2);
	    *(buf + j * 3 + 2) = c;
	    }

    if (gManp->width > MAXHORIZONTAL)
	return;
    local_width = (WORD)(((DWORD)gManp->width * (DWORD)gManp->Dib.BitsPerPixel) / 8);
    if (row < gManp->height && row >= 0)
	{
	i = ((long) (gManp->height - 1 - row) * (long) (local_width + 3 - ((local_width - 1) % 4)));
	if ((i + local_width) >> 16 != i >> 16)			// 64K boundary
	    {
	    diff = (i + local_width) & 0x0000ffff;
	    memcpy(gManp->Dib.DibPixels.data() + i, buf, (size_t)(local_width - diff));
	    memcpy(gManp->Dib.DibPixels.data() + i + local_width - diff, buf + local_width - diff, (size_t)diff);
	    }
	else
  	    memcpy(gManp->Dib.DibPixels.data() + i, buf, (size_t)local_width);
	}
    }

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : GetRealClientRect(HWND hwnd, LPRECT lprc)                  *
 *                                                                          *
 *  PURPOSE    : Calculates the client rectangle taking scrollbars into     *
 *               consideration.                                             *
 *                                                                          *
 ****************************************************************************/
void GetRealClientRect (HWND hwnd, PRECT lprc)

    {
    DWORD dwStyle;

    dwStyle = GetWindowLong (hwnd, GWL_STYLE);
    GetClientRect (hwnd,lprc);

    if (dwStyle & WS_HSCROLL)
        lprc->bottom += GetSystemMetrics (SM_CYHSCROLL);

    if (dwStyle & WS_VSCROLL)
        lprc->right  += GetSystemMetrics (SM_CXVSCROLL);
    }

/****************************************************************************
 *                                                                          *
 *  FUNCTION   : SetScrollRanges(hwnd)                                      *
 *                                                                          *
 *  PURPOSE    :                                                            *
 *                                                                          *
 ****************************************************************************/
void SetScrollRanges(HWND hwnd)

    {
    RECT		rc;
    int		iRangeH, iRangeV, i;
    static	int	iSem = 0;

    if (!iSem)
	{
	iSem++;
	GetRealClientRect (hwnd, &rc);

	for (i = 0; i < 2; i++)
	    {
	    iRangeV = gManp->ptSize.y - rc.bottom;
	    iRangeH = gManp->ptSize.x - rc.right;

	    if (iRangeH < 0) 
		iRangeH = 0;
	    if (iRangeV < 0) 
		iRangeV = 0;

	    if (GetScrollPos(hwnd, SB_VERT) > iRangeV || GetScrollPos(hwnd, SB_HORZ) > iRangeH)
		InvalidateRect (hwnd, NULL, FALSE);
#ifdef _WIN64
	    if (gManp->width > gManp->screenx)
		{
		SetScrollRange(hwnd, SB_VERT, 0, iRangeV, TRUE);
		SetScrollRange(hwnd, SB_HORZ, 0, iRangeH, TRUE);
		}
#else
	    SetScrollRange(hwnd, SB_VERT, 0, iRangeV, TRUE);
	    SetScrollRange(hwnd, SB_HORZ, 0, iRangeH, TRUE);
#endif
	    GetClientRect (hwnd, &rc);
	    }
	iSem--;
	}
    SetScrollPos(hwnd, SB_HORZ, 0, true);
    SetScrollPos(hwnd, SB_VERT, 0, true);
    }

/*-----------------------------------------
	Close all pointers
  -----------------------------------------*/

void	ClosePtrs(void)
    {
    gManp->Dib.CloseDibPtrs();
//  BigNumFree();		// No not here. We only close these if we are leaving ManpWin.
    gManp->wpixels.clear();
    }

/**************************************************************************
	Dialog Control for Image Size
**************************************************************************/

static	BOOL	MaintainAspect = TRUE;

INT_PTR CALLBACK ImageSizeDlg (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
     {
     HWND		hCtrl ;
     BOOL		bTrans ;
     static	UINT	tempX; 
     static     UINT	tempY;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        gManp->cycleflag = FALSE;
		
		SetDlgItemInt(hDlg, IDC_WIDTH, gManp->Dib.DibWidth, TRUE);
		SetDlgItemInt(hDlg, IDC_HEIGHT, gManp->Dib.DibHeight, TRUE);
		SetDlgItemInt(hDlg, IDC_X_SIZE, NewXdots, TRUE);
		SetDlgItemInt(hDlg, IDC_Y_SIZE, NewYdots, TRUE);
		hCtrl = GetDlgItem (hDlg, IDC_MAINTASPECT);
		SendMessage(hCtrl, BM_SETCHECK, MaintainAspect, 0L);
	        return TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_X_SIZE:
			tempX = GetDlgItemInt(hDlg, IDC_X_SIZE, &bTrans, TRUE);
			if (MaintainAspect)
			    SetDlgItemInt(hDlg, IDC_Y_SIZE, (int)(((double) tempX + 0.5) / 1.7777777777), TRUE);
		        return TRUE ;

		    case IDC_Y_SIZE:
			tempY = GetDlgItemInt(hDlg, IDC_Y_SIZE, &bTrans, TRUE);
		        return TRUE ;

		    case IDC_MAINTASPECT:
			hCtrl = GetDlgItem (hDlg, IDC_MAINTASPECT) ;
			MaintainAspect = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return TRUE ;

		    case IDC_FULLSCREEN:
			NonStandardImage = FALSE;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDOK:
			NewXdots = tempX;
			if (NewXdots < 10)
			    NewXdots = 10;
			if (NewXdots >= MAXHORIZONTAL)
			    NewXdots = MAXHORIZONTAL;

			NewYdots = tempY;
			if (NewYdots < 10)
			    NewYdots = 10;
			if (NewYdots >= MAXVERTICAL)
			    NewYdots = MAXVERTICAL;

			gManp->AspectRatio = 16.0 / 9.0;			// force reasonable aspect ratio
			NonStandardImage = TRUE;
			EndDialog (hDlg, TRUE);
			return TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return FALSE;
		   }
	  break;
	  }
      return FALSE ;
      }


                