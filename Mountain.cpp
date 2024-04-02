
/////////////////////////////////////////////////////////////////////
// MOUNTAIN.CPP a program to investigate mountain terrain generation.
/////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include "manpwin.h"
#include "manp.h"
#include "Dib.h"
#include "Preview.h"
#include "resource.h"
#include "plot.h"

#ifndef WIN95
#define WIN95	
#endif

#define TRUE		1
#define FALSE		0
#define MAXLEVEL	12
#define SCALEFACTOR	0.04		// assumes a screen of 1024 * 768

#define	WATER_PREVIEW_HEIGHT	170
#define	WATER_PREVIEW_WIDTH	220
#define	WATER_HOR_OFFSET	124
#define	WATER_VERT_OFFSET	22

#define	MOUNT_PREVIEW_HEIGHT	170
#define	MOUNT_PREVIEW_WIDTH	220
#define	MOUNT_HOR_OFFSET	467
#define	MOUNT_VERT_OFFSET	22

static	int	WaterRed = 0, WaterGreen = 0, WaterBlue = 255;
static	int	MountainRed = 98, MountainGreen = 100, MountainBlue = 0;

static	WORD	degree = 8;
extern	HWND	PixelHwnd;		// pointer to handle for pixel updating
extern	double	param[];
extern	PAINTSTRUCT 	ps;
extern	RECT 	r;
extern	BOOL	ShowUnderwaterTerrain = FALSE;
extern	int	xdots, ydots;

extern	CPlot	Plot;		// image plotting routines 
extern	int	user_data(HWND);

//char	test;
static	double	rh, l, vt, xx, yy, zz, d, xp, xb, yp, yb, xt, yt, zt, xo = 0.0;
static	double	x2, y2, z2, x3, y3, z3, w3 = 0.0;
static	double	xs, ys, zs;				/* scale factors */

//double	data[128][64];
static	double	data[1 << (MAXLEVEL + 1)][1 << MAXLEVEL];

static	int	ds, mx, my, ax, ay, bx, by, ib, sk, ex, ey = 0;
static	double	d1, d2 = 0.0;

static	CPreview	WaterPreview, MountainPreview;

double	get_array_data();
void	put_array_data(), xheights(), yheights(), zheights();
void	move(DWORD *), tilt_down(), rotate(), water(int, int, DWORD *);

/**************************************************************************
	Experimental mountain drawing routine
**************************************************************************/

int	mountain()

    {
    int	n;
    DWORD	water_col, land_col, colour;

    Plot.flags = 0;
    water_col = ((WaterRed & 0x00ff) << 16) + ((WaterGreen & 0x00ff) << 8) + (WaterBlue & 0x00ff);
    land_col = ((MountainRed & 0x00ff) << 16) + ((MountainGreen & 0x00ff) << 8) + (MountainBlue & 0x00ff);

    if (degree <= 0)
	degree = 1;
    if (degree > MAXLEVEL)
	degree = MAXLEVEL;

    ds = 2;
    for (n = 0; n < degree; ++n)
	ds += 1 << n;

    mx = ds - 1;
    my = mx >> 1;
    rh = PI / 6.0;
    vt = rh * 1.2;

    srand((unsigned)time(NULL));			// randomize things
    for (n = 0; n < degree; ++n)
	{
	l = 10000.0 / pow(1.8, n); 
	ib = mx / (1 << (n + 1));
	sk = ib << 1;
	xheights();
	yheights();
	zheights();
	}

    colour = land_col;				/* initialise to land */

// note that SCALEFACTOR was defined for 1024 * 768 pixel screen. Let's adjust for any screen
    xs = SCALEFACTOR * (double)xdots / 1024.0;	/* scale factors init */
    ys = SCALEFACTOR * (double)xdots / 1024.0;
    zs = SCALEFACTOR * (double)xdots / 1024.0;

    for (ax = 0; ax <= mx; ++ax)
	{
	xo = -999.0;
	for (ay = 0; ay <= ax; ++ay)
	    {
	    if (user_data(PixelHwnd) == -1)		/* user pressed a key? */
		return(-1);
	    zz = get_array_data();
	    yy = (float)ay / (float)mx * 10000.0;
	    xx = (float)ax / (float)mx * 10000.0 - yy / 2.0;
	    water(water_col, land_col, &colour);	/* plot water */
	    move(&colour);				/* move or plot */
	    }
	}
      
    for (ay = 0; ay <= mx; ++ay)
	{
	xo = -999.0;
	for (ax = ay; ax <= mx; ++ax)
	    {
	    if (user_data(PixelHwnd) == -1)		/* user pressed a key? */
		return(-1);
	    zz = get_array_data();
	    yy = (float)ay / (float)mx * 10000.0;
	    xx = (float)ax / (float)mx * 10000.0 - yy / 2.0;
	    water(water_col, land_col, &colour);	/* plot water */
	    move(&colour);				/* move or plot */
	    }
	}
      
    for (ex = 0; ex <= mx; ++ex)
	{
	xo = -999.0;
	for (ey = 0; ey <= (mx - ex); ++ey)
	    {
	    if (user_data(PixelHwnd) == -1)		/* user pressed a key? */
		return(-1);
	    ax = ex + ey;
	    ay = ey;
	    zz = get_array_data();
	    yy = (float)ay / (float)mx * 10000.0;
	    xx = (float)ax / (float)mx * 10000.0 - yy / 2.0;
	    water(water_col, land_col, &colour);	/* plot water */
	    move(&colour);				/* move or plot */
	    }
	}
    return 0;
    }

/**************************************************************************
	Plot Water
**************************************************************************/

void	water(int water_col, int land_col, DWORD *colour)

    {
    if (xo == -999.0)
	{
	if (zz < 0.0)
	    {
	    *colour = water_col;
	    z2 = zz;
	    if (!ShowUnderwaterTerrain)
		zz = 0.0;
	    }
	else
	    {
	    *colour = land_col;
	    z2 = zz;
	    }
	}
    else
	{
	if (z2 > 0.0 && zz > 0.0)
	    z2 = zz;
	else
	    {
	    if (z2 < 0.0 && zz < 0.0)
		{
		z2 = zz;
		if (!ShowUnderwaterTerrain)
		    zz = 0.0;
		}
	    else
		{
		w3 = zz / (zz - z2);
		x3 = (x2 - xx) * w3 + xx;
		y3 = (y2 - yy) * w3 + yy;
		z3 = 0.0;
		zt = zz;
		yt = yy;
		xt = xx;
		if (zz > 0.0)			/* coming out of water */
		    {
		    zz = z3;
		    yy = y3;
		    xx = x3;
		    move(colour);				/* move or plot */
//		    colour = (zz > 3.0) ? 4 : land_col;
		    *colour = land_col;
		    zz = zt;
		    yy = yt;
		    xx = xt;
		    z2 = zz;
		    }
		else				/* going into water */
		    {
		    zz = z3;
		    yy = y3;
		    xx = x3;
		    move(colour);				/* move or plot */
		    *colour = water_col;
		    zz = 0.0;
		    yy = yt;
		    xx = xt;
		    z2 = zt;
		    }
		}
	    }
	}
    x2 = xx;
    y2 = yy;
    }

/**************************************************************************
	Rotate
**************************************************************************/

void	rotate()

    {
    double	ra, r1, rd;

    if (xx != 0.0)
	{
	ra = atan(yy / xx);
	if (xx < 0.0)
	    ra += PI;
	}
    else if (yy <= 0.0)
	ra = -(PI / 2.0);
    else
	ra = PI / 2.0;
    r1 = ra + rh;
    rd = sqrt(xx * xx + yy * yy);
    xx = rd * cos(r1);
    yy = rd * sin(r1);
    }

/**************************************************************************
	Tilt
**************************************************************************/

void	tilt_down()

    {
    double	ra, r1, rd;

    rd = sqrt(xx * xx + zz * zz);
    if (xx == 0.0)
	ra = PI / 2.0;
    else
	{
	ra = atan(zz / xx);
	if (xx < 0.0)
	    ra += PI;
	}
    r1 = ra - vt;
    xx += rd * cos(r1);
    zz = rd * sin(r1);
    }

/**************************************************************************
	Move or plot to second point
**************************************************************************/

void	move(DWORD *colour)

    {
    char	test;

    xx *= xs;
    yy *= ys;
    zz *= zs;
    rotate();
    tilt_down();
    test = (xo == -999.0) ? 'M' : 'D';
    xp = floor(yy) * 2.25;
    yp = 160.00 - floor(zz) * 2.4;

    if (test ==  'M')
	{
	xb = xp;
	yb = yp;
	xo = xp;
	}

    if (xb < 0) xb = 0;
    if (xb >= xdots) xb = xdots - 1;
    if (yb < 0) yb = 0;
    if (yb >= ydots) yb = ydots - 1;
    if (xp < 0) xp = 0;
    if (xp >= xdots) xp = xdots - 1;
    if (yp < 0) yp = 0;
    if (yp >= ydots) yp = ydots - 1;
    Plot.genline((int)xb, ydots - (int)yb - 1, (int)xp, ydots - (int)yp - 1, *colour);

    xb = xp;
    yb = yp;
    }
	
/**************************************************************************
	Assign heights along x in array
**************************************************************************/
	
void	xheights()

    {
    int	ye, xe = 0;

    for (ye = 0; ye < mx; ye += sk)
	for (xe = ib + ye; xe <= mx; xe += sk)
	    {
	    ax = xe - ib;
	    ay = ye;
	    d1 = get_array_data();
	    ax = xe + ib;
	    d2 = get_array_data();
	    d = (d1 + d2) / 2.0 + ((float)rand() / 32768.0) * l / 2.0 - l / 4.0;
	    ax = xe;
	    ay = ye;
	    put_array_data();
	    }
}
	
/**************************************************************************
	Assign heights along y in array
**************************************************************************/
	
void	yheights()

    {
    int	ye, xe = 0;

    for (xe = mx; xe > 0; xe -= sk)
	for (ye = ib; ye <= xe; ye += sk)
	    {
	    ax = xe;
	    ay = ye + ib; 
	    d1 = get_array_data();
	    ay = ye - ib;
	    d2 = get_array_data();
	    d = (d1 + d2) / 2.0 + ((float)rand() / 32768.0) * l / 2.0 - l / 4.0;
	    ax = xe;
	    ay = ye;
	    put_array_data();
	    }
    }
	
/**************************************************************************
	Assign heights along diagonal in array
**************************************************************************/
	
void	zheights()

    {
    int	ye, xe = 0;

    for (xe = 0; xe < mx; xe += sk)
	for (ye = ib; ye <= mx - xe; ye += sk)
	    {
	    ax = xe + ye - ib;
	    ay = ye - ib;
	    d1 = get_array_data();
	    ax = xe + ye + ib;
	    ay = ye + ib;
	    d2 = get_array_data();
	    ax = xe + ye;
	    ay = ye;
	    d = (d1 + d2) / 2.0 + ((float)rand() / 32768.0) * l / 2.0 - l / 4.0;
	    put_array_data();
	    }
    }

/**************************************************************************
	Return data from array
**************************************************************************/

double	get_array_data()

    {
    if (ay > my)
	{
	by = mx + 1 - ay; 
	bx = mx - ax;
	}
    else
	{
	by = ay; 
	bx = ax;
	}

    return(data[bx][by]);
    }

/**************************************************************************
	Return data to array
**************************************************************************/

void put_array_data()

    {
    if (ay > my)
	{
	by = mx + 1 - ay; 
	bx = mx - ax;
	}
    else
	{
	by = ay; 
	bx = ax;
	}

    data[bx][by] = d;
    }

/**************************************************************************
	Dialog Control for Setting IOnside colour
**************************************************************************/

DLGPROC FAR PASCAL MountainDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static int color [6];
     HWND	  hwndParent, hCtrl ;
     short	  nCtrlID, nIndex;
     BOOL	  bTrans ;
     char	  s[480];

     WaterPreview.HorOffset = WATER_HOR_OFFSET;						// dimensions of preview windows
     WaterPreview.VertOffset = WATER_VERT_OFFSET;
     WaterPreview.PreviewHeight = WATER_PREVIEW_HEIGHT;
     WaterPreview.PreviewWidth = WATER_PREVIEW_WIDTH;
     MountainPreview.HorOffset = MOUNT_HOR_OFFSET;
     MountainPreview.VertOffset = MOUNT_VERT_OFFSET;
     MountainPreview.PreviewHeight = MOUNT_PREVIEW_HEIGHT;
     MountainPreview.PreviewWidth = MOUNT_PREVIEW_WIDTH;
     switch (message)
	  {
	  case WM_INITDIALOG:
	       color [0] = WaterRed;
	       color [1] = WaterGreen;
	       color [2] = WaterBlue;
	       color [3] = MountainRed;
	       color [4] = MountainGreen;
	       color [5] = MountainBlue;
	       for (nCtrlID = IDC_SCROLLBAR1 ; nCtrlID <= IDC_SCROLLBAR6 ; nCtrlID++)
		    { 
		    nIndex  = nCtrlID - IDC_SCROLLBAR1;
		    hCtrl = GetDlgItem (hDlg, nCtrlID) ;
		    SetScrollRange (hCtrl, SB_CTL, 0, 255, FALSE) ;
		    SetScrollPos   (hCtrl, SB_CTL, 255 - color[nIndex], FALSE) ;
		    SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1, color [nIndex], TRUE) ;
		    }
		SetDlgItemInt(hDlg, IDC_MOUNTAINLEVEL, degree, TRUE);
		
		hCtrl = GetDlgItem (hDlg, IDC_SHOWTERRAIN);
		SendMessage(hCtrl, BM_SETCHECK, ShowUnderwaterTerrain, 0L);
		SetFocus(GetDlgItem(hDlg, IDC_MOUNTAINLEVEL));
		WaterPreview.InitPreview(hDlg);
		MountainPreview.InitPreview(hDlg);
		InvalidateRect(hDlg,NULL,FALSE);
		return (DLGPROC)TRUE ;

	  case WM_VSCROLL :
	       hCtrl = (HWND)GET_WM_COMMAND_HWND (wParam, (_int64)lParam);
	       if ((nCtrlID = ((WORD)(GetWindowLong (hCtrl, GWL_ID)))) == 0)
		   {
		   wsprintf(s, "Error: Scrollbar fail: <%ld>", GetLastError());
		   MessageBox  (hDlg, s, "Paul's Graphics Viewer", MB_ICONEXCLAMATION | MB_OK);
		   }

	       nIndex  = nCtrlID - IDC_SCROLLBAR1;
	       hwndParent = GetParent (hDlg) ;

	       switch ((int) LOWORD(wParam))
//	       switch (wParam)
		    {
		    case SB_PAGEDOWN :
			 color [nIndex] -= 15 ;        // fall through
		    case SB_LINEDOWN :
			 color [nIndex] = max (0, color [nIndex] - 1) ;
			 break ;
		    case SB_PAGEUP :
			 color [nIndex] += 15 ;        // fall through
		    case SB_LINEUP :
			 color [nIndex] = min (255, color [nIndex] + 1) ;
			 break ;
		    case SB_TOP:
			 color [nIndex] = 255 ;
			 break ;
		    case SB_BOTTOM :
			 color [nIndex] = 0 ;
			 break ;
		    case SB_THUMBPOSITION :
		    case SB_THUMBTRACK :
#ifdef	WIN95								// 32 bit code
			 color[nIndex] = 255 - HIWORD (wParam) ;
#else									// 16 bit code
			 color[nIndex] = LOWORD (lParam) ;
#endif
//			 color [nIndex] = LOWORD (lParam) ;
			 break ;
		    default :
			 return (DLGPROC)FALSE ;
		    }
	 	SetScrollPos  (hCtrl, SB_CTL,	      255 - color [nIndex], TRUE) ;
	        SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1,  color [nIndex], TRUE) ;
//		PreviewColour.PreviewDib.ClearDib(color [0], color [1], color [2]);
//		PreviewColour.Preview(hDlg);
		InvalidateRect(hDlg,NULL,FALSE);
	        return (DLGPROC)TRUE ;

	  case WM_PAINT :
		BeginPaint(hDlg, &ps);
		WaterPreview.PreviewDib.ClearDib(color [0], color [1], color [2]);
		WaterPreview.Preview(hDlg);
		MountainPreview.PreviewDib.ClearDib(color [3], color [4], color [5]);
		MountainPreview.Preview(hDlg);
		EndPaint(hDlg, &ps);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	       switch ((int) LOWORD(wParam))
//	       switch (wParam)
		    {
		    case IDOK:
			WaterRed = color [0];
			WaterGreen = color [1];
			WaterBlue = color [2];
			MountainRed = color [3];
			MountainGreen = color [4];
			MountainBlue = color [5];
			degree = GetDlgItemInt(hDlg, IDC_MOUNTAINLEVEL, &bTrans, TRUE);
			hwndParent = GetParent (hDlg) ;
    			InvalidateRect(hwndParent, &r, FALSE);	// force repaint
			WaterPreview.PreviewDib.CloseDibPtrs();
			MountainPreview.PreviewDib.CloseDibPtrs();
			hCtrl = GetDlgItem (hDlg, IDC_SHOWTERRAIN) ;
			ShowUnderwaterTerrain = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			EndDialog (hDlg, 1);
			return (DLGPROC)TRUE;
		    case IDCANCEL:
			WaterPreview.PreviewDib.CloseDibPtrs();
			MountainPreview.PreviewDib.CloseDibPtrs();
			EndDialog (hDlg, 0);
			return FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }



