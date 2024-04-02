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
#define  VIEWMAIN
#include "manpwin.h"
#include "resource.h"
#include "Dib.h"
#include "Plot.h"

							// routines in this module
void	putline(WORD, BYTE *);

void	ClearScreen(void);

void	GetRealClientRect (HWND, PRECT);
void	SetScrollRanges(HWND);

HPALETTE 	 	hpal = NULL;
double	 		*wpixels = NULL;		// an array of doubles holding slope modified iteration counts

RECT 	WARect;						// this is the usable screen taking taskbar into account
extern	int	mandel(HWND);
extern	void	SetupView(HWND);
extern	void	ClosePtrs(void);
extern	int	CopyPictureToClipboard(HWND);

extern	int     file_type;
extern	DWORD	dwStyle;			// style of current window
extern	POINT	ptSize;				// Stores DIB dimensions
extern	BYTE	cycleflag;			// do colour cycling

extern	long	threshold;			// maximum iterations
extern	int	logval;				// log colour map starting value
extern	WORD	type;				// M=mand, N=Newton etc
extern	CPlot	Plot;
extern	CTrueCol    TrueCol;			// palette info

extern	WORD	special;			// special colour, phase etc
//extern	BYTE	degree;				// power
extern	HWND	CallingWindowHandle;		// Is ManpWIN called by an external window via WM_COPYDATA message?
extern	int	ReplyUsingDIB;			// 2 for Device Context, 1 for DIB/WM_COPYDATA and 0 for clipboard

int		fdin;				// FILE descripter
int		height, xdots, ydots, width, bits_per_pixel, planes;
int		display_width, display_height;
int		screenx, screeny;
int		caption;			// size of windows caption and scroll bars
int		scroll_width;			// size of horizontal scroll bars
int		max_vscroll, max_hscroll;
int		mapentrysize = 0;		// 0 indicates no colormap
WORD		colours;			// colours in the file
DWORD		screen_colours;			// colours in the graphics card
WORD     	iNumColors;    			// Number of colors supported by device
long		eofpos;				// position of end of file   
double		ScreenRatio;			// ratio of width / height for the screen
PAINTSTRUCT 	ps;
RECT 		r;
HDC		hdcMem;				// load picture into memory
DWORD		biCompression;			// BMP type compression
BYTE		line_buf[MAXHORIZONTAL * 3];    // true colour = 3 bytes per pixel

BOOL		NonStandardImage;		// has user changed image size?
WORD		NewXdots = 800, NewYdots = 450;	// for non standard image sizes

CDib		Dib;				// Device Independent Bitmap

/**************************************************************************
	Main entry decoder
**************************************************************************/

int	mainview(HWND hwnd, BOOL FileFlag)

    {
    char		*szAppName = "Paul's Fractals";
    HANDLE		hfiledata = (HANDLE)NULL;			
    BYTE	 huge	*filedata = NULL;				// handle to the file data
//    BOOL		FullScreen = FALSE;

    screen_colours = 1L << (GetDeviceCaps(ps.hdc, BITSPIXEL) * GetDeviceCaps(ps.hdc, PLANES));
    iNumColors = (WORD) screen_colours;

// get screen metrics
    SystemParametersInfo(SPI_GETWORKAREA, 0, &WARect, 0);	// let's get usable area including task bar

    screenx = WARect.right - WARect.left;
    screeny = WARect.bottom - WARect.top;

			// Some initial parameters
    if (!FileFlag)					// image from PNG file, so don't splatter height and width
	{
	if (NonStandardImage/* && !IsZoomed(hwnd) == FALSE*/)
	    {
	    height = NewYdots;                  
	    width = NewXdots;
	    }
	else
	    {
	    height = screeny - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYMENU) - GetSystemMetrics(SM_CYHSCROLL);                  
	    width = screenx - GetSystemMetrics(SM_CXHSCROLL);
	    height -= GetSystemMetrics(SM_CYMENU);				    // remember status bar
	    }
	}
    ScreenRatio = (double)width / (double)height;
    
        // Compute the size of the window rectangle based on the given
        // client rectangle size and the window style, then size the window.
         
    GetClientRect(hwnd, &r);
    ptSize.x = width;
    ptSize.y = height;

    if (IsZoomed (hwnd))
	SetScrollRanges (hwnd);
    xdots = width;
    ydots = height;

    SetupView(hwnd);

    ClosePtrs();							    // ready for next screen

    if (Dib.InitDib(width, height, bits_per_pixel) == NULL)
	{
	switch(Dib.DibErrorCode)
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
	if (wpixels = new double[width * height])
	    ClearScreen();
	else
	    {
	    Dib.CloseDibPtrs();
//	    throw "Can't allocate DIB memory.";	// IB 2009-05-18 C++ error handling
	    return NULL;
	    }
	}
    memset(wpixels, 0, width * height * sizeof(double));
    // reset all the image parameters for the pixel plotting routines
    Plot.InitPlot(threshold, &TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib.BitsPerPixel, &Dib, USEPALETTE + USEWPIXELS);


#ifdef DEBUG
MessageBox (hwnd, "About to decode file in mainview()!", szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

#ifdef DEBUG
wsprintf(s, "iNumColors = %d, colours = %d", iNumColors, colours);
MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

#ifdef DEBUG
wsprintf(s, "Width = %d, Height = %d, xdots = %d, ydots = %d, order = %d, length = %d", 
						width, height, xdots, ydots, order, length);
MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

#ifdef DEBUG
wsprintf(s, "bits_per_pixel = %d, file_type = %d, iNumColors = %d", bits_per_pixel, file_type, iNumColors);
MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

#ifdef DEBUG
wsprintf(s, "Pic mem -> screen! File colours = %d, screen colours = %ld", colours, screen_colours);
MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

#ifdef DEBUG
MessageBox (hwnd, "Leaving mainview()!", szAppName, MB_ICONEXCLAMATION | MB_OK);
#endif

    return (TRUE);
    }

/**************************************************************************
	Clear Screen 
**************************************************************************/

void	ClearScreen(void)

    {
    memset(wpixels, 0, (DWORD)width * (DWORD)height * sizeof(double));
    memset(Dib.DibPixels, 0, WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel) * (DWORD)height);
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
	    FracData.cbData = Dib.pDibInf->bmiHeader.biSize + Dib.pDibInf->bmiHeader.biSizeImage;
	    if ((MessageArray = (BYTE *)GlobalAlloc(GMEM_FIXED, Dib.pDibInf->bmiHeader.biSize + Dib.pDibInf->bmiHeader.biSizeImage + 4096L)) == NULL)
		{
		wsprintf(t, "Can't allocate memory for DIB Message");
		MessageBox (hwnd, t, szAppName, MB_ICONEXCLAMATION | MB_OK);
		MessageArray = NULL;
		MessageBeep (0);
		return -1;
		}
	    memcpy(MessageArray, Dib.pDibInf, Dib.pDibInf->bmiHeader.biSize);
	    memcpy(MessageArray + Dib.pDibInf->bmiHeader.biSize, Dib.DibPixels,  Dib.pDibInf->bmiHeader.biSizeImage);

	    FracData.lpData = MessageArray;

#ifdef DEBUG
	test = (LPBITMAPINFO) FracData.lpData;
	wsprintf(t, "Header info\nHeader Size (4 Bytes) = %ld\nWidth (4 Bytes) = %ld\nHeight (4 Bytes) = %ld\nImage Size (4 Bytes) = %ld\nPlanes (2 Bytes) = %d\nBits/pixel (2 Bytes) = %d\nCompression (4 Bytes) = %ld\nXPels (4 Bytes) = %ld\nYPels (4 Bytes) = %ld\nColours used (4 Bytes) = %ld\nColours impotrant (4 Bytes) = %ld\n", 
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

    if (bits_per_pixel == 24)			// swap red and blue
	for (j = 0; j < width; ++j)                 // the order is reversed to prevent silly colours????
	    {
	    c = *(buf + j * 3);
	    *(buf + j * 3) = *(buf + j * 3 + 2);
	    *(buf + j * 3 + 2) = c;
	    }

    if (width > MAXHORIZONTAL)
	return;
    local_width = (WORD)(((DWORD) width * (DWORD)bits_per_pixel) / 8);
    if (row < height && row >= 0)
	{
	i = ((long) (height - 1 - row) * (long) (local_width + 3 - ((local_width - 1) % 4)));
	if ((i + local_width) >> 16 != i >> 16)			// 64K boundary
	    {
	    diff = (i + local_width) & 0x0000ffff;
	    memcpy(Dib.DibPixels + i, buf, (size_t)(local_width - diff));
	    memcpy(Dib.DibPixels + i + local_width - diff, buf + local_width - diff, (size_t)diff);
	    }
	else
  	    memcpy(Dib.DibPixels + i, buf, (size_t)local_width);
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
	    iRangeV = ptSize.y - rc.bottom;
	    iRangeH = ptSize.x - rc.right;

	    if (iRangeH < 0) 
		iRangeH = 0;
	    if (iRangeV < 0) 
		iRangeV = 0;

	    if (GetScrollPos(hwnd, SB_VERT) > iRangeV || GetScrollPos(hwnd, SB_HORZ) > iRangeH)
		InvalidateRect (hwnd, NULL, FALSE);
#ifdef _WIN64
	    if (width > screenx)
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
    Dib.CloseDibPtrs();
//  BigNumFree();		// No not here. We only close these if we are leaving ManpWin.
    if (wpixels) {delete wpixels;wpixels = NULL;}
    }

/**************************************************************************
	Dialog Control for Image Size
**************************************************************************/

static	BOOL	MaintainAspect = TRUE;

DLGPROC FAR PASCAL ImageSizeDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     HWND		hCtrl ;
     BOOL		bTrans ;
     static	UINT	tempX; 
     static     UINT	tempY;
     static     UINT	tempParam;

     switch (message)
	  {
	  case WM_INITDIALOG:
		cycleflag = FALSE;
		
		SetDlgItemInt(hDlg, IDC_WIDTH, Dib.DibWidth, TRUE);
		SetDlgItemInt(hDlg, IDC_HEIGHT, Dib.DibHeight, TRUE);
		SetDlgItemInt(hDlg, IDC_X_SIZE, NewXdots, TRUE);
		SetDlgItemInt(hDlg, IDC_Y_SIZE, NewYdots, TRUE);
		hCtrl = GetDlgItem (hDlg, IDC_MAINTASPECT);
		SendMessage(hCtrl, BM_SETCHECK, MaintainAspect, 0L);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDC_X_SIZE:
			tempX = GetDlgItemInt(hDlg, IDC_X_SIZE, &bTrans, TRUE);
			if (MaintainAspect)
			    SetDlgItemInt(hDlg, IDC_Y_SIZE, (int)(((double) tempX + 0.5) / 1.7777777777), TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_Y_SIZE:
			tempY = GetDlgItemInt(hDlg, IDC_Y_SIZE, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_MAINTASPECT:
			hCtrl = GetDlgItem (hDlg, IDC_MAINTASPECT) ;
			MaintainAspect = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
		        return (DLGPROC)TRUE ;

		    case IDC_FULLSCREEN:
			NonStandardImage = FALSE;
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

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

			ScreenRatio = 4.0 / 3.0;			// force reasonable aspect ratio
			NonStandardImage = TRUE;
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
	  break;
	  }
      return (DLGPROC)FALSE ;
      }


                