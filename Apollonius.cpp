/*
   APOLLONIUS.CPP a module for Apollonian Circle Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	<windowsx.h>
#include	"manp.h"
#include	"resource.h"
#include	"Complex.h"
#include	"preview.h"
#include	"fractype.h"
#include	"Plot.h"

#define SQRT3    1.732050807568877193

extern	HWND	GlobalHwnd;				// This is the main windows handle

extern	int	user_data(HWND);

extern	long	threshold;
extern	double	mandel_width;			// width of display 
extern	double	hor;				// horizontal address
extern	double	vert;				// vertical address 
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	int	subtype;			// A - E
extern	int	curpass, totpasses;
extern	WORD	type;				// M=mand, J=Julia 1,2,4-> etc
extern	double	param[];
extern	int	xdots, ydots, bits_per_pixel;
extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts

extern	CTrueCol    TrueCol;			// palette info
extern	CDib    Dib;				// Device Independent Bitmap
extern	CPlot	Plot;				// image plotting routines 

static	double	xscale, yscale;
static	double	shape[2];
static	DWORD   colour;
static	long    count = 10000000;

/**************************************************************************
	Apollonius Fractal Type Images
***************************************************************************/

int	DoApolloniusIFS(void)

    {
    long    n;
    int	    ix,iy;
    double  x=0.2,y=0.3,x1=0,y1=0,r=SQRT3;
    double  a0,b0,f1x,f1y;

    Plot.InitPlot(threshold, &TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib.BitsPerPixel, &Dib, USEPALETTE);
    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;

    count = (long)param[0];
    colour = (DWORD)param[1];
    totpasses = 10;
    srand((unsigned)time(NULL));			// randomise things

    for (n = 0; n < count; n++) 
	{
	if (user_data(GlobalHwnd) == -1)		// user pressed a key?
	    return -1;
	curpass = n * 10 / count;
	a0 = 3 * (1 + r - x) / (pow(1 + r - x,2.0) + y*y) - (1 + r) / (2+r);
	b0 = 3 * y / (pow(1 + r - x,2.0) + y*y);
	f1x = a0 / (a0*a0 + b0*b0);
	f1y = -b0 / (a0*a0 + b0*b0);
	switch (rand()%3) 
	    {
	    case 0:
		x1 = 3 * (1 + r - x) / (pow(1 + r - x,2.0) + y*y) - (1 + r) / (2 + r);
		y1 = 3 * y / (pow(1 + r - x,2.0) + y*y);
		break;
	    case 1:
		x1 = -f1x / 2 - f1y * r / 2;
		y1 = f1x * r / 2 - f1y / 2;
		break;
	    case 2:
		x1 = -f1x / 2 + f1y * r / 2;
		y1 = -f1x * r / 2 - f1y / 2;
		break;
	    }
	if (n < 100)
	    continue;
	ix = (int)((x - hor) * xscale);
	iy = (int)((vert + mandel_width - y) * yscale);

	x = x1;
	y = y1;
	if (ix < 0 || iy < 0 || ix >= xdots || iy >= ydots)
	    continue;
	Plot.PlotPoint((WORD)ix, (WORD)iy, colour);
	}
    return 0;
    }

/*
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg
 xmlns="http://www.w3.org/2000/svg"
 xmlns:xlink="http://www.w3.org/1999/xlink"
 width="100%" height="100%" viewBox="0 0 768 768"
 onload="init(evt)"
><title>Apollonian Fractal</title
><desc>Randomized Apollonian fractal circle packing</desc
><script type="text/ecmascript"><![CDATA[
*/

static	double	minsize = 0.000001;
static	int	CircleColour = 1;
	int	FilledCircle = 1;	// 1 = filled, 2 = unfilled, 3 = 3D
static	BOOL	IsPreview = FALSE;

typedef struct 
    {
    double x;
    double y;
    double r;
    } CIRCLE;		// vector of floats


CIRCLE	*Circle(double x, double y, double r);
void	draw(double x, double y, double r, int colour, CPlot Plot); 

#define MAXCIRCLES 500000

#define	CIRCLE_PREVIEW_HEIGHT	250
#define	CIRCLE_PREVIEW_WIDTH	250
#define	CIRCLE_HOR_OFFSET	124
#define	CIRCLE_VERT_OFFSET	22

static  CIRCLE  queue[MAXCIRCLES][3];
static	int	QueuePtr = 0;
static	short	level[2];
static	CPreview	CircleScreen;
 
extern	PAINTSTRUCT 	ps;
extern	CDib		Dib;				// Device Independent Bitmap

// compute the 4th circle touching 3 circles, each of which touch the other two
CIRCLE	*Kiss(CIRCLE a, CIRCLE b, CIRCLE c, CIRCLE *out, BOOL initial) 
    {
    Complex	temp;

    static	CIRCLE	ret, cc;
    double	k1 = 1 / a.r; 
    Complex	z1; z1.x = a.x; z1.y = a.y; 
    Complex	kz1 = z1 * k1;
    double	k2 = 1 / b.r; 
    Complex	z2; z2.x = b.x; z2.y = b.y; 
    Complex	kz2 = z2 * k2;
    double	k3 = 1 / c.r;
    Complex	z3; z3.x = c.x; z3.y = c.y; 
    Complex	kz3 = z3 * k3;
    double	k4p = k1 + k2 + k3 + 2*sqrt(k1*k2 + k2*k3 + k3*k1);
    double	k4m = k1 + k2 + k3 - 2*sqrt(k1*k2 + k2*k3 + k3*k1);
    temp = kz1 * kz2 + kz2 * kz3 + kz3 * kz1;
    Complex	kz4p = (kz1+kz2+kz3)+(temp.CSqrt()) * 2;
    Complex	kz4m = (kz1+kz2+kz3)-(temp.CSqrt()) * 2;
    double	k4;
    Complex	kz4;
    double	k4b;
    Complex	kz4b;
    if (k4p > k4m) 
	{ 
	k4 = k4p; 
	kz4 = kz4p; 
	k4b = k4m; 
	kz4b = kz4m; 
	}
    else           
	{ 
	k4 = k4m; 
	kz4 = kz4m; 
	k4b = k4p; 
	kz4b = kz4p; 
	}
    cc = *Circle(kz4.x/k4,kz4.y/k4,fabs(1/k4));
    double dx = a.x - cc.x;
    double dy = a.y - cc.y;
    double dr = a.r + cc.r;

    if (fabs(dx * dx + dy *dy - dr * dr) > 0.0001) 
	{
	cc = *Circle(kz4b.x/k4,kz4b.y/k4,fabs(1/k4));
	}
    memcpy(&ret, &cc, sizeof(CIRCLE));
    if (initial) 
	{
	cc = *Circle(kz4b.x/k4b,kz4b.y/k4b,fabs(1/k4b));
	memcpy(out, &cc, sizeof(CIRCLE));
	}
    return &ret;
    }

void	LoadQueue(CIRCLE a, CIRCLE b, CIRCLE c)
    {
    memcpy(&queue[QueuePtr][0], &a, sizeof (CIRCLE));
    memcpy(&queue[QueuePtr][1], &b, sizeof (CIRCLE));
    memcpy(&queue[QueuePtr][2], &c, sizeof (CIRCLE));
   
    if (QueuePtr < MAXCIRCLES - 1)
	QueuePtr++;
    }

// called once on load
int	DoApollonius(void) 
    {
    long    i;
    CIRCLE  b, p, q, cs[2], nc;

    if (IsPreview)
	{
	Plot.InitPlot(threshold, &TrueCol, wpixels, xdots, ydots, CircleScreen.PreviewWidth, CircleScreen.PreviewHeight, CircleScreen.PreviewDib.BitsPerPixel, &CircleScreen.PreviewDib, USEPALETTE);
	xscale = (double) (CircleScreen.PreviewWidth) / 2.2;
	yscale = (double) (CircleScreen.PreviewHeight) / 2.2;
	hor = -0.1;
	vert = -1.9;
	}
    else
	{
	Plot.InitPlot(threshold, &TrueCol, wpixels, xdots, ydots, xdots, ydots, Dib.BitsPerPixel, &Dib, USEPALETTE);
	xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
	yscale = (double) (ydots - 1) / mandel_width;
	}

    if (shape[0] < -1.0)
	shape[0] = -1.0;
    if (shape[0] > 2.66)
	shape[0] = 2.66;
    if (shape[1] < -1.0)
	shape[1] = -1.0;
    if (shape[1] > 1.99)
	shape[1] = 1.99;

    totpasses = 10;
    CircleColour = 1;
    QueuePtr = 0;
    srand((unsigned)time(NULL));
 
  // initial bounding circle
    b = *Circle(0, 0, -1);
 
  // insert two randomly positioned touching circles
    double  tr = 1 - shape[0] / 2;
    double  pa = PI/2 - asin(shape[1] * (1 - tr)/tr);
    double  px = tr * cos(pa);
    double  py = tr * sin(pa);
    double  pr = 1 - tr;
    double  qr = (1 - pr) * (1 - cos(pa + PI/2)) / (1 + pr - (1 - pr) * cos(pa + PI/2));
    double  qx = 0;
    double  qy = qr - 1;

    p = *Circle(px, py, pr);
    q = *Circle(qx, qy, qr);
 
  // a queue to contain triples of touching circles
    cs[0] = *Kiss(b, p, q, &cs[1], TRUE);
    LoadQueue(b, p, cs[0]);
    LoadQueue(b, cs[0], q);
    LoadQueue(cs[0], p, q);
    LoadQueue(b, p, cs[1]);
    LoadQueue(b, cs[1], q);
    LoadQueue(cs[1], p, q);
 
  // add 10000 more circles to the draw queue
  // adding new triples to the compute queue
    draw(b.x, b.y, b.r, CircleColour++, Plot);
    draw(p.x, p.y, p.r, CircleColour++, Plot);
    draw(q.x, q.y, q.r, CircleColour++, Plot);
    draw(cs[0].x, cs[0].y, cs[0].r, CircleColour++, Plot);
    draw(cs[1].x, cs[1].y, cs[1].r, CircleColour++, Plot);

    for (i = 0; i < ((IsPreview) ? 1000 : MAXCIRCLES); i++) 
	{
	CIRCLE	c0, c1, c2;

	if (user_data(GlobalHwnd) == -1)		// user pressed a key?
	    return -1;
	curpass = i * 10 / MAXCIRCLES;
	memcpy(&c0, &queue[i][0], sizeof(CIRCLE));
	memcpy(&c1, &queue[i][1], sizeof(CIRCLE));
	memcpy(&c2, &queue[i][2], sizeof(CIRCLE));
	nc = *Kiss(c0,c1,c2, NULL, FALSE);
	if (nc.r > minsize) 
	    {
	    LoadQueue(nc, c1, c2);
	    LoadQueue(c0, nc, c2);
	    LoadQueue(c0, c1, nc);
	    draw(nc.x, nc.y, nc.r, CircleColour++, Plot);
	    }
	}
    return 0;
    } // init()
 
// circle class
CIRCLE	*Circle(double x, double y, double r) 
    {
    static	CIRCLE	ret;

    ret.x = x;
    ret.y = y;
    ret.r = r;
    return &ret;
    } // Circle()

/*-----------------------------------------
	Draw Sexy Circle
  -----------------------------------------*/

void	draw(double x, double y, double r, int colour, CPlot Plot) 
    {
    int		i, j, radius;
    BYTE	rgb[3];

    i = (int)(((x - hor) + 1) * yscale);
    j = (int)((y + vert + mandel_width - 1) * yscale);
    radius = (int)(fabs(r) * yscale);

    switch (FilledCircle)
	{
	case 1: 
	    Plot.DisplayFilledCircle(i, j, radius, colour%threshold);
	    break;
	case 2: 
	    Plot.DisplayCircle(i, j, radius, colour%threshold);
	    break;
	case 3: 
	    Plot.GetRGB(colour%threshold, rgb);
	    Plot.Display3DCircle((IsPreview) ? &CircleScreen.PreviewDib : &Dib, i, j, radius, rgb);
	    break;
	}
    } 

/**************************************************************************
	Draw Preview CircleImages
***************************************************************************/

int	CirclePreview(HWND hwnd)

    {
    memset(CircleScreen.PreviewDib.DibPixels, 0, (WIDTHBYTES((DWORD)CircleScreen.PreviewWidth * (DWORD)bits_per_pixel)) * CircleScreen.PreviewHeight);	// set background to black
    shape[0] = (double)level[0]/150.0;
    shape[1] = (double)level[1]/100.0;
    IsPreview = TRUE;
    DoApollonius();
    ShowWindow (hwnd, SW_SHOWNORMAL);
    return 0;
    }

DLGPROC FAR PASCAL ApolloniusDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static	HANDLE	hCursor;
     static     char	temp, temp1;
     static     UINT	tempParam, tempParam1;
     short		nCtrlID, nIndex;
     HWND		hwndParent, hCtrl;
     char		s[MAXLINE];

     // set the dimensions and location of the windows
     CircleScreen.HorOffset = CIRCLE_HOR_OFFSET;				// dimensions of preview window
     CircleScreen.VertOffset = CIRCLE_VERT_OFFSET;
     CircleScreen.PreviewHeight = CIRCLE_PREVIEW_HEIGHT;
     CircleScreen.PreviewWidth = CIRCLE_PREVIEW_WIDTH;

     switch (message)
	  {
	  case WM_INITDIALOG:
		level[0] = 127;
		level[1] = 127;
	        switch (FilledCircle)
		    {
		    case 1:
			tempParam = IDC_FILLED;
			break;
		    case 2:
			tempParam = IDC_UNFILLED;
			break;
		    case 3:
			tempParam = IDC_3D;
			break;
		    }
		CheckRadioButton(hDlg, IDC_FILLED, IDC_3D, tempParam);
		for (nCtrlID = IDC_SCROLLBAR1 ; nCtrlID <= IDC_SCROLLBAR2; nCtrlID++)
		    {
		    nIndex  = nCtrlID - IDC_SCROLLBAR1;
		    hCtrl = GetDlgItem (hDlg, nCtrlID) ;
		    SetScrollRange (hCtrl, SB_CTL, 0, 255, FALSE) ;
		    SetScrollPos   (hCtrl, SB_CTL, level[nIndex], FALSE) ;
		    SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1, level[nIndex], TRUE) ;
		    }
		if (CircleScreen.InitPreview(hDlg) < 0)
		    {
		    wsprintf(s, "Error: No memory for preview");
		    MessageBox (hDlg, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		    EndDialog (hDlg, FALSE);
		    }
		InvalidateRect(hDlg,NULL,FALSE);
	        return (DLGPROC)FALSE ;

	  case WM_VSCROLL :

		hCtrl = GET_WM_COMMAND_HWND (wParam, (_int64)lParam);
		if ((nCtrlID = ((WORD)(GetWindowLong (hCtrl, GWL_ID)))) == 0)
		    {
		    wsprintf(s, "Error: Scrollbar fail: <%ld>", GetLastError());
		    MessageBox (hDlg, s, "Paul's Graphics Viewer", MB_ICONEXCLAMATION | MB_OK);
		    }

	       nIndex  = nCtrlID - IDC_SCROLLBAR1;
	       hwndParent = GetParent (hDlg) ;

	       switch ((int) LOWORD(wParam))
		    {
		    case SB_PAGEDOWN :
			 level [nIndex] -= 15 ;        // fall through
		    case SB_LINEDOWN :
			 level [nIndex] = max (0, level [nIndex] - 1) ;
			 break ;
		    case SB_PAGEUP :
			 level [nIndex] += 15 ;        // fall through
		    case SB_LINEUP :
			 level [nIndex] = min (255, level [nIndex] + 1) ;
			 break ;
		    case SB_TOP:
			 level [nIndex] = 255 ;
			 break ;
		    case SB_BOTTOM :
			 level [nIndex] = 0 ;
			 break ;
		    case SB_THUMBPOSITION :
		    case SB_THUMBTRACK :
			 level[nIndex] = 255 - HIWORD (wParam) ;
			 break ;
		    default :
			 return FALSE ;
		    }
	 	SetScrollPos  (hCtrl, SB_CTL, 255 - level [nIndex], TRUE) ;
		SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1, level[nIndex], TRUE) ;
		InvalidateRect(hDlg,NULL,FALSE);
	        return (DLGPROC)TRUE ;

	  case WM_PAINT:
		BeginPaint(hDlg, &ps);
		CirclePreview(hDlg);
		CircleScreen.Preview(hDlg);
		EndPaint(hDlg, &ps);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:

	        switch ((int) LOWORD(wParam))
		    {                 
		    case IDC_FILLED:
		    case IDC_UNFILLED:
		    case IDC_3D:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_FILLED:
				FilledCircle = 1;
				break;
			    case IDC_UNFILLED:
				FilledCircle = 2;
				break;
			    case IDC_3D:
				FilledCircle = 3;
				break;
			    }

			CheckRadioButton(hDlg, IDC_FILLED, IDC_3D, (int) LOWORD(wParam));
			InvalidateRect(hDlg,NULL,FALSE);
		        return (DLGPROC)TRUE ;

		    case IDOK:
			CircleScreen.PreviewDib.CloseDibPtrs();
			IsPreview = FALSE;
//			hor = -1.0; vert = -0.25; mandel_width = 2.5;
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;
		    case IDCANCEL:
			CircleScreen.PreviewDib.CloseDibPtrs();
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }

