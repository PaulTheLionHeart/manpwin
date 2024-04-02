// Preview.cpp: implementation of the CPreview class.
//
//////////////////////////////////////////////////////////////////////

#include "Preview.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreview::CPreview()
    {
    OpenDlgFlag = FALSE;			// is image dialog box open?
    }

CPreview::~CPreview()
    {
    PreviewDib.CloseDibPtrs();
    OpenDlgFlag = FALSE;

    }

//---------------------------------------------------------------------
//	Initialise the Preview DIB
//-------------------------------------------------------------------//

int	CPreview::InitPreview(HWND hwnd)
    {
    OpenDlgFlag = TRUE;

    if (PreviewDib.InitDib(PreviewWidth, PreviewHeight, 24) == NULL)
	{
	return -1;
	}

    PreviewDib.ClearDib(WHITE);
    return 0;
    }

//---------------------------------------------------------------------
//	Draw a grid on the Preview DIB
//-------------------------------------------------------------------//

int	CPreview::PreviewGrid(HWND hwnd, int type)

    {
    WORD	i, k, l, bytes_per_pixel;
    long	address, j, m;

    bytes_per_pixel = PreviewDib.BitsPerPixel / 8;
    if (type == VERTLINE || type == BOTHLINE)
	{
	j = bytes_per_pixel * (PreviewWidth / 2);
	for (i = 0; i < PreviewDib.DibHeight; ++i)
	    {
	    address = (long) i * PreviewDib.WidthBytes + j;
	    for (k = 0; k < bytes_per_pixel; ++k)
		*(PreviewDib.DibPixels + address + k) = ~(*(PreviewDib.DibPixels + address + k));
	    }
	}
    if (type == HORLINE || type == BOTHLINE)
	{
	address = (long) (PreviewDib.DibHeight / 2) * PreviewDib.WidthBytes;
	for (i = 0; i < PreviewDib.DibWidth; ++i)
	    {
	    j = bytes_per_pixel * i;
	    for (k = 0; k < bytes_per_pixel; ++k)
		*(PreviewDib.DibPixels + address + (long)k + j) = ~(*(PreviewDib.DibPixels + address + (long)k + j));
	    }
	}
    if (type == GRIDLINES)
	{
	m = bytes_per_pixel * (PreviewWidth / NUMGRIDLINES);
	for (l = 1; l < NUMGRIDLINES; ++l)
	    {
	    j = m * l;
	    for (i = 0; i < PreviewDib.DibHeight; ++i)
		{
		address = (long) i * PreviewDib.WidthBytes + j;
		for (k = 0; k < bytes_per_pixel; ++k)
		    *(PreviewDib.DibPixels + address + k) = ~(*(PreviewDib.DibPixels + address + k));
		}
	    }
	m = (long) (PreviewDib.DibHeight / NUMGRIDLINES) * PreviewDib.WidthBytes;
	for (l = 1; l < NUMGRIDLINES; ++l)
	    {
	    address = m * l;
	    j = 0;
	    for (i = 0; i < PreviewDib.DibWidth; ++i)
		{
		j += bytes_per_pixel;
		for (k = 0; k < bytes_per_pixel; ++k)
		    *(PreviewDib.DibPixels + address + (long)k + j) = ~(*(PreviewDib.DibPixels + address + (long)k + j));
		}
	    }
	}
    return 0;
    }

//---------------------------------------------------------------------
//	Copy Data into preview window
//---------------------------------------------------------------------

int	CPreview::CopyPreview(HWND hwnd, CDib *Dib)

    {
    WORD	i, j;
    RGBTRIPLE *	Source, *tmp;
    RGBTRIPLE *	Dest;

    for (i = 0; i < PreviewHeight; ++i)
	{
	tmp = (RGBTRIPLE *)(Dib->DibPixels + (Dib->WidthBytes * ((DWORD)i * (DWORD)Dib->DibHeight / (DWORD)PreviewHeight)));
	Dest = (RGBTRIPLE *)(PreviewDib.DibPixels + PreviewDib.WidthBytes * (DWORD)i);
	for (j = 0; j < PreviewWidth; ++j)
	    {
	    Source = tmp + ((j * Dib->DibWidth) / PreviewWidth);
	    *Dest = *Source;
	    Dest++;
	    }
	}
    return 0;
    }

//---------------------------------------------------------------------
//	Copy Background to RGB colour
//---------------------------------------------------------------------

int	CPreview::CopyBackground(HWND hwnd, short r, short g, short b)

    {
    PreviewDib.ClearDib(r, g, b);
    ShowWindow (hwnd, SW_SHOWNORMAL);
    return 0;
    }

//---------------------------------------------------------------------
//	Preview
//---------------------------------------------------------------------

int	CPreview::Preview(HWND hwnd)

    {
    HDC	hdc ;

    if (!OpenDlgFlag)
	return 0;
    hdc = GetDC(hwnd);
    StretchDIBits (hdc, HorOffset, VertOffset, 
		    PreviewWidth, PreviewHeight, 0, 0, PreviewWidth, PreviewHeight,
		    (LPSTR)PreviewDib.DibPixels, (LPBITMAPINFO)PreviewDib.pDibInf, DIB_PAL_COLORS, SRCCOPY);
    ShowWindow (hwnd, SW_SHOWNORMAL);

    return 0;
    }

//---------------------------------------------------------------------
//	Give the memory back
//---------------------------------------------------------------------

int	CPreview::ClosePreview(void)
    {
    PreviewDib.CloseDibPtrs();
    OpenDlgFlag = FALSE;
    return 0;
    }

