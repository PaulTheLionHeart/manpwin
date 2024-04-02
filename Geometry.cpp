/*
   GEOMETRY.CPP a module for Geometric Shapes Fractals
   Thanks to Marcus Rezende 

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"menu.h"
#include	"anim.h"
#include	"polygon.h"
#include	"plot.h"

#define MIN(a,b) (a <= b ? a : b)
#define MAX(a,b) (a >= b ? a : b)
#define MAXPOINTS 16

extern	long	threshold;
extern	double	mandel_width;			/* width of display */
extern	double	hor;				/* horizontal address */
extern	double	vert;				/* vertical address */
extern	double	ScreenRatio;			// ratio of width / height for the screen
//extern	double	param[];
extern	WORD	type;				// fractal type
extern	int	subtype;			// A - E
extern	int	curpass, totpasses;
//extern	BYTE	*PalettePtr;			// points to true colour palette

static	CPoly	polygon;			// polygon class
extern	int	xdots, ydots;
extern	CPlot	Plot;		// image plotting routines 

static	double	xscale, yscale;
static	int	sides = 7;			// number of sides of polygon or star
static	long	count = 250L;			// number of polygons or stars
static	BOOL	IsStar = TRUE;			// spread colours across palette

/**************************************************************************
	Geometric Shapes Fractal Type Images
***************************************************************************/

int	DoGeometry(void)

    {
    int	    i, j, k;
    double  phase, size, x, y, t;
    double  u[MAXPOINTS], v[MAXPOINTS];
    int	    a[MAXPOINTS], b[MAXPOINTS];
    int	    h = 8;
    int	    offset;
    DWORD   colour;

    srand((unsigned)time(NULL));			// randomize things
    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
    yscale = (double) (ydots - 1) / mandel_width;
    for (i = 0; i < count; i++)
	{
	colour = (count < threshold) ? threshold * (DWORD)i / count : (DWORD)(i % threshold);
	size = h * (1 - sqrt((double)rand() / 32767.0)) / 50;
	x = rand() / 32767.0;
	y = rand() / 32767.0;
	phase= rand() * PI / 32767.0;
	switch (subtype)
	    {
	    case 'A':					
		if (rand() > 16383) 
		    {
		    u[0] = x + size;
		    v[0] = y;
		    u[1] = x;
		    v[1] = y;
		    }
		else
		    {
		    u[0] = x;
		    v[0] = y - size;
		    u[1] = x;
		    v[1] = y;
		    }
		a[0] = (int)((u[0] - hor) * xscale);
		a[1] = (int)((u[1] - hor) * xscale);
		b[0] = (int)((vert + mandel_width - v[0]) * yscale);
		b[1] = (int)((vert + mandel_width - v[1]) * yscale);
		Plot.genline(a[0], b[0], a[1], b[1], colour);
		break;
	    case 'B':					
	    case 'C':					
	    case 'D':					
		t = 2 * PI / sides;
		if (subtype == 'B' || subtype == 'C')
		    offset = 1;
		else
		    offset = (sides % 2) ? sides / 2 : sides / 2 - 1;
		for (j = 0; j < sides; j++)
		    {
		    u[j] = x + size * cos(j * t + phase);
		    v[j] = y + size * sin(j * t + phase);
		    a[j] = (int)((u[j] - hor) * xscale);
		    b[j] = (int)((vert + mandel_width - v[j]) * yscale);
		    }
		if (subtype == 'C')
		    {
		    for (j = 0; j < sides; j++)
			polygon.read(a[j], b[j], j);				// load verticies
		    polygon.init(j, colour);						// initialise vertices and colour
		    polygon.calcs();						// find MAX,MIN
		    polygon.display(Plot);						// fill polygon
		    }
		else
		    {
		    for (j = 0; j < sides; j++)
			{
			k = (j + offset < sides) ? j + offset : j + offset - sides;
			Plot.genline(a[j], b[j], a[k], b[k], colour);
			}
		    }
		break;
	    }
	}
    return 0;
    }

DLGPROC FAR PASCAL GeometryDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static     char	temp;
     static     UINT	tempParam;
     static     WORD	temp_special;
     BOOL		bTrans ;

     switch (message)
	  {
	  case WM_INITDIALOG:
	        temp = subtype;
	        switch (subtype)
		    {
		    case 'A':
			tempParam = IDC_A;
			break;
		    case 'B':
			tempParam = IDC_B;
			break;
		    case 'C':
			tempParam = IDC_C;
			break;
		    case 'D':
			tempParam = IDC_D;
			break;
		    default:				// uninitialised
			tempParam = IDC_A;
			temp = 'A';
			break;
		    }
		CheckRadioButton(hDlg, IDC_A, IDC_F, tempParam);
		SetDlgItemInt(hDlg, IDC_PARAM1, sides, TRUE);
		SetDlgItemInt(hDlg, IDC_PARAM2, count, TRUE);
		SetFocus(GetDlgItem(hDlg, tempParam));
	        return FALSE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_A:
		    case IDC_B:
		    case IDC_C:
		    case IDC_D:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_A:
				temp = 'A';
				break;
			    case IDC_B:
				temp = 'B';
				break;
			    case IDC_C:
				temp = 'C';
				break;
			    case IDC_D:
				temp = 'D';
				break;
			    }

			CheckRadioButton(hDlg, IDC_A, IDC_F, (int) LOWORD(wParam));
		        return (DLGPROC)TRUE ;

		    case IDOK:
			count = GetDlgItemInt(hDlg, IDC_PARAM2, &bTrans, TRUE);
			sides = GetDlgItemInt(hDlg, IDC_PARAM1, &bTrans, TRUE);
			if (sides > MAXPOINTS)
			    sides = MAXPOINTS;
			if (sides < 2)
			    sides = 2;
			subtype = temp;
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
