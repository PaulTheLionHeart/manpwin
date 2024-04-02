/*
    FOURIER.CPP a module for Fourier Synthesis of signals
    
    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<conio.h>
#include	<string.h>
#include	<stdio.h>
#include	<windowsx.h>
#include	"manp.h"
#include	"resource.h"
#include	"fractype.h"
#include	"colour.h"
#include	"Dib.h"
#include	"preview.h"
#include	"Plot.h"

#define	PREVIEW_HEIGHT	168
#define	PREVIEW_WIDTH	180
#define	HOR_OFFSET	145
#define	VERT_OFFSET	142

#define	DEG2RAD		57.29577951
#define	FOURIERMAX	1000
#define	MAXSTEPS	2500
#define	LEVELS		30		// number of harmonic sliders

#define	SQUARE		'0'
#define	TRIANGLE	'1'
#define	SAWTOOTH	'2'
#define	FULLWAVE	'3'
#define	SINEWAVE	'4'
#define	COSINEWAVE	'5'
#define	IMPULSE		'9'
#define	USER		'U'

struct	fourierstruct
    {    
    double	x;				// starting point for harmonic
    double	y;
    double	magsin;				// magnitude of the sine component of the harmonic
    double	magcos;				// magnitude of the cosine component of the harmonic
    WORD	c;				// colour of harmonic
    }	FourierArray[FOURIERMAX];   

extern	int	user_data(HWND);
//extern	char	*FractData(void);
extern	char	*GenerateMPEGFileName (char *, char *);
extern	char	*GenerateAnimFileName (char *, char *);
extern	void	SetUpFilename(char *Filename, char *Folder, char *AnimType);

extern	long	threshold;
extern	int	subtype;		// see below
extern	WORD	type;			// M=mand, J=Julia 1,2,4-> etc
extern	int	row, col;
extern	PAINTSTRUCT 	ps;
extern	HDC	hdcMem;			// load picture into memory
extern	double 	*wpixels;		// an array of doubles holding slope modified iteration counts
extern	RECT 	r;
extern	HWND	PixelHwnd;		// pointer to handle for pixel updating
extern	POINT	ptSize;			// Stores DIB dimensions
extern	BYTE	cycleflag;		// do colour cycling
extern	int	xdots, ydots, width, height, bits_per_pixel;;

extern	char	PNGName[];		// base name for PNG file sequence
extern	char	ScriptFileName[];	// base name for script file 
extern	char	MPGPath[];		// path for MPEG files
extern	char	MPGFile[];		// MPEG file
extern	char	ANIMPNGPath[];		// path for animated PNG files and LST files
extern	char	PNGFile[];		// PNG file

extern	BOOL	StartImmediately;	// immediate start of animation generation
extern	BOOL	DisplayAnimation;	// allow system to know that we are currently displaying an animation

extern	int	time_to_break;		// time to break out of animation?
extern	int	time_to_quit;		// time to quit?
extern	BOOL	AutoSaveFlag;

extern	BOOL	WritePNGFrames;		// write frames to PNG files
extern	BOOL	WriteMemFrames;		// write frames to memory
extern	BOOL	WritePNGList;		// write PNG filenames to a *lst file
extern	BOOL	WriteMPEGFrames;	// write frames directly to an MPEG file

	WORD	NumHarmonics = 60, steps = 400, delay = 20;
static	double	MagX, MagY;		// magnitude of the fundamental expressed in screen size

int	WaveformArray[MAXSTEPS];   

WORD	WavePtr = 0;			// point to the currently updated location
static	short	level[LEVELS];
BOOL		MovingWave = TRUE;
static	BOOL	first = TRUE;

extern	int	FibDelay(HWND, WORD);
extern	void	ConvertRGB2ASCII(BYTE, BYTE, BYTE, char *);
extern	char	*AnimData(void);
extern	CDib	Dib;			// Device Independent Bitmaps
extern	CTrueCol    TrueCol;		// palette info
extern	CPlot	Plot;		// image plotting routines 

static	CPreview	FourierPreview;

/**************************************************************************
	Write Slider values to string
***************************************************************************/

char	*WriteSliders(void)

    {
    static	char	buffer[LEVELS * 5 + 1];		// 3 digits + sign + ',' per slider and a null at the end
    int	i;
    char	t[6];

    *buffer = '\0';
    if (subtype == USER)
	{
	for (i = 0; i < LEVELS; i++)
	    {
	    sprintf(t, ",%d", level[i]);
	    strcat(buffer, t);;
	    }
	}
    return buffer;
    }

/**************************************************************************
	Write Slider values to string
***************************************************************************/

extern	char	*strtok1(register char *, register const char *);	// required to use different 
									// static variable to main 
									// strtok() in user.c

int	ReadSliders(char *buffer)

    {
    char	seps[]   = ",";
    char	*token;
    int	i;

    if (*buffer == '\0' || subtype != USER)			// nothing to do
	return FALSE;
    i = 0;
    token = strtok1(buffer, seps);
    while (i < LEVELS)
	{
	if (!token)
	    return TRUE;
	level[i++] = atoi(token);
	token = strtok1(NULL, seps);
	}
    return TRUE;
    }

/**************************************************************************
	Plot Harmonics
***************************************************************************/

int	Harmonics(BOOL clear, int TotalFrames, CPlot Plot)

    {
    WORD	i, x1, y1, x2, y2, centrex, centrey;
    double	xold, yold;
    double	xnew, ynew;
    int	test;

    xnew = ynew = xold = yold = 0.0;
    centrey = ydots / 2;
    centrex = xdots / 4;

    for (i = 0; i < NumHarmonics; ++i)
	{
	if (FourierArray[i].magsin == 0.0 && FourierArray[i].magcos == 0.0)		// nothing to do
	    continue;
	xnew += FourierArray[i].x;
	ynew += FourierArray[i].y;
	test = ((int)(MagX * xold) + centrex);
	x1 = (test < 0) ? 0 : (WORD)test;
	test = ((int)(MagY * yold) + centrey);
	y1 = (test < 0) ? 0 : (WORD)test;
	test = ((int)(MagX * xnew) + centrex);
	x2 = (test < 0) ? 0 : (WORD)test;
	test = ((int)(MagY * ynew) + centrey);
	y2 = (test < 0) ? 0 : (WORD)test;
	if (x1 > xdots / 2 - 10)					// make sure we don't write a harmonic into the history area
	    x1 = xdots / 2 - 10;
	if (x2 > xdots / 2 - 10)
	    x2 = xdots / 2 - 10;
	if (x1 < 0)	
	    x1 = 0;
	if (x2 < 0)
	    x2 = 0;
	if (y1 > ydots - 1)						// check boundaries
	    y1 = ydots - 1;
	if (y2 > ydots - 1)
	    y2 = ydots - 1;
	if (y1 < 0)	
	    y1 = 0;
	if (y2 < 0)
	    y2 = 0;
	Plot.genline(x1, (WORD)(ydots - y1), x2, (WORD)(ydots - y2), clear ? 0 : FourierArray[i].c);
	xold = xnew;
	yold = ynew;
	}
    // load waveform array with the current
    if (!clear)
	{
	WaveformArray[WavePtr++] = y2;
	if (WavePtr >= TotalFrames)
	    WavePtr = 0;			// cycle through again
	}
    return 0;
    }

/**************************************************************************
	Set up table of values for the starting point of each harmonic
***************************************************************************/

void	CalculateFourier(double angle)

    {
    int	i;

    for (i = 1; i < NumHarmonics; i++)
	{
	if (FourierArray[i].magsin == 0.0 && FourierArray[i].magcos == 0.0)		// nothing to do
	    continue;
	FourierArray[i].x = -sin(HALF_PI + i * angle) * FourierArray[i].magsin + sin(i * angle) * FourierArray[i].magcos;
	FourierArray[i].y = -cos(HALF_PI + i * angle) * FourierArray[i].magsin + cos(i * angle) * FourierArray[i].magcos;
	FourierArray[i].c = i * (int)threshold / NumHarmonics + 1;
	}
    }

/**************************************************************************
	Set up screen scaling and other inits
***************************************************************************/

void	InitFourier(char WaveType)

    {
    int	i;
    double	sign;

    WavePtr = 0;							// point to the currently updated location
    MagX = (double)xdots / 16.0;
    MagY = (double)ydots / 8.0;
    if (WaveType != USER)						// we don't want to do this if we are using sliders
	{
	for (i = 0; i < FOURIERMAX; i++)
	    {
	    FourierArray[i].magsin = 0.0;
	    FourierArray[i].magcos = 0.0;
	    }

	if (WaveType == IMPULSE || WaveType == SAWTOOTH || WaveType == SQUARE || WaveType == SINEWAVE)
	    FourierArray[1].magsin = 1.0;
	else
	    FourierArray[1].magcos = 1.0;

	sign = (WaveType == IMPULSE || WaveType == SAWTOOTH) ? -1.0 : 1.0;

	if (WaveType == COSINEWAVE)
	    FourierArray[0].magcos = 1.0;				// only fundamental
	if (WaveType == SINEWAVE)
	    FourierArray[0].magsin = 1.0;				// only fundamental
	}

    for (i = 1; i < NumHarmonics; i++)
	{
	switch (WaveType)
	    {
	    case IMPULSE:
		if (i % 2 == 0)					// reject even values
		    continue;
		sign *= -1.0;					// change sign
		FourierArray[i].magsin = sign / (PI * 5.0);	// all the same
		break;
	    case SAWTOOTH:
		sign *= -1.0;					// change sign
		FourierArray[i].magsin = sign / (double)i;
		break;
	    case SQUARE:
		if (i % 2 == 0)					// reject even values
		    continue;
		FourierArray[i].magsin = 1.0 / (double)i;
		break;
	    case TRIANGLE:
		if (i % 2 == 0)					// reject even values
		    continue;
		FourierArray[i].magcos = 1.0 / (double)(i * i);	// n squared
		break;
	    case FULLWAVE:
		if (i == 1)					// reject first harmonic  - no divide by 0
		    {
		    FourierArray[i].magcos = 0.0;
		    continue;
		    }
		if (i % 2 == 1)					// reject odd values
		    continue;
		sign *= -1.0;					// change sign
		    FourierArray[i].magcos = sign * 2.0 / (1.0 - (double)(i * i));
		break;
	    }
	}
    for (i = 0; i < LEVELS; i++)
	level[i] = (i < LEVELS / 2) ?
		    (short)(FourierArray[i + 1].magsin * 255.0) : (short)(FourierArray[i - LEVELS / 2 + 1].magcos * 255.0);
    first = TRUE;						// don't draw artificats when starting up
    }

/**************************************************************************
	Fourier Images
***************************************************************************/

int	FourierStep(HWND hwnd, char *FileName, int TotalFrames, int ThisStep, CPlot Plot)

    {
    static	double	angle;
    int		k, index1, index2;
    static	int	HorPos, OldHorPos;
    BYTE	buffer[MAXHORIZONTAL * 3];
    long	address;
    static	int	colour = 0;

    DWORD	tick;
                     
    user_data(hwnd);
    if (time_to_quit)
	{
	AutoSaveFlag = FALSE;
	return -1;
	}
    tick = GetTickCount();
    while (GetTickCount() < tick + delay);

    OldHorPos = HorPos;
    HorPos = (DWORD)xdots * (DWORD) ThisStep / (DWORD) (TotalFrames * 2) + xdots / 2;
    Harmonics(TRUE, TotalFrames, Plot);		// clear previous vectors
    angle = (TWO_PI / (double)TotalFrames) * (double) ThisStep;
    CalculateFourier(angle);
    Harmonics(FALSE, TotalFrames, Plot);		// write new vectors
    InvalidateRect(hwnd, &r, FALSE);

    index1 = WavePtr - 1;
    index2 = WavePtr - 2;
    if (index1 < 0)
	index1 += TotalFrames;
    if (index2 < 0)
	index2 += TotalFrames;

    if (first)
	first = FALSE;
    else						// don't draw artificats when starting up
	{
	if (MovingWave)
	    {
	    for (k = 0; k < ydots; k++)
		{
		address = WIDTHBYTES((DWORD)width * (DWORD)bits_per_pixel) * k + (xdots / 2 - 3) * 3;
		memcpy(buffer, Dib.DibPixels + address, (xdots / 2) * 3);
		memcpy(Dib.DibPixels + address + 3, buffer, (xdots / 2 - 1) * 3);
		}
	    Plot.genline((WORD)(xdots / 2), (WORD)(ydots - WaveformArray[index2]), 
					    (WORD)(xdots / 2 + 1), (WORD)(ydots - WaveformArray[index1]), colour);
	    }
	else 
	    Plot.genline((WORD)(OldHorPos - 2), (WORD)(ydots - WaveformArray[index2]), 
		(WORD)((HorPos > OldHorPos) ? (HorPos - 2) : OldHorPos),	// remove retrace
		(WORD)(ydots - WaveformArray[index1]), colour);
	}
    if (colour++ >= threshold)
    colour = 1;
    return 0;
    }                                                 

/**************************************************************************
	Fourier Images
***************************************************************************/

int	Fourier(void)

    {
    int	j;

    DisplayAnimation = TRUE;
    time_to_break = FALSE;
    InitFourier(subtype);

    while (time_to_break == FALSE)
	{
	for (j = 0; j < steps; ++j)
	    {
	    if (time_to_break)
		break;
	    if (FourierStep(PixelHwnd, "", steps, j, Plot) < 0)
		return 0;
	    }
	}

    return 0;
    }                                                 

/**************************************************************************
	Draw Preview Fourier Images
***************************************************************************/

int	WavePreview(HWND hwnd, CPlot Plot)

    {
    int	i, j, index;
    double	xnew, ynew;
    double	angle;
    int	test;

    WavePtr = 0;                     
    FourierPreview.PreviewDib.ClearDib(0L);
    //memset(TempDib.DibPixels, 0, (WIDTHBYTES((DWORD)PreviewWidth * (DWORD)bits_per_pixel)) * PreviewHeight);	// set background to black
    for (j = 0; j < FourierPreview.PreviewHeight / 2 + 20; ++j)
	{
	xnew = ynew = 0.0;
	angle = (TWO_PI / (double)(FourierPreview.PreviewHeight / 2)) * (double) j;
	CalculateFourier(angle);
	for (i = 0; i < NumHarmonics; ++i)
	    {
	    if (FourierArray[i].magsin == 0.0 && FourierArray[i].magcos == 0.0)		// nothing to do
		continue;
	    ynew += FourierArray[i].y;
	    }
	test = ((int)((double)FourierPreview.PreviewHeight / 6.0 * ynew) + FourierPreview.PreviewHeight / 2);
	WaveformArray[WavePtr++] = (test < 0) ? 0 : test;
	}

    for (i = 0; i < FourierPreview.PreviewHeight + 2; ++i)
	{
	index = (i < FourierPreview.PreviewHeight / 2 + 1) ? i : i - FourierPreview.PreviewHeight / 2;
	if (i > 0)
	    Plot.genline((WORD)(i + 3), (WORD)WaveformArray[index - 1], (WORD)(i + 4), (WORD)WaveformArray[index], 0x0000ff00);
	}
    ShowWindow (hwnd, SW_SHOWNORMAL);
    return 0;
    }

/**************************************************************************
	Dialog Control for Fourier Analysis
**************************************************************************/

DLGPROC FAR PASCAL FourierTypeDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static	HANDLE	hCursor;
     static     char	temp;
     static     UINT	tempParam;
     static     int	temp_step;
     static     int	temp_delay;
     static     int	temp_Harmonics;
     static     int	temp_steps;
     static     BYTE	*PreviewPix;
     BOOL		bTrans;
     short		nCtrlID, nIndex;
     HWND		hwndParent, hCtrl;
     char		s[MAXLINE];
     static	CPlot	Plot;
     
     FourierPreview.HorOffset = HOR_OFFSET;						// dimensions of preview window
     FourierPreview.VertOffset = VERT_OFFSET;
     FourierPreview.PreviewHeight = PREVIEW_HEIGHT;
     FourierPreview.PreviewWidth = PREVIEW_WIDTH;

     Plot.InitPlot(threshold, &TrueCol, wpixels, xdots, ydots, FourierPreview.PreviewWidth, FourierPreview.PreviewHeight, FourierPreview.PreviewDib.BitsPerPixel, &FourierPreview.PreviewDib, 0);

     switch (message)
	  {
	  case WM_INITDIALOG:
		cycleflag = FALSE;
	        temp = subtype;
	        switch (subtype)
		    { 
		    case SQUARE:
			tempParam = IDC_SQUAREWAVE;
			break;
		    case COSINEWAVE:
			tempParam = IDC_COSWAVE;
			break;
		    case SINEWAVE:
			tempParam = IDC_SINWAVE;
			break;
		    case TRIANGLE:
			tempParam = IDC_TRIANGLE;
			break;
		    case IMPULSE:
			tempParam = IDC_IMPULSETRAIN;
			break;
		    case SAWTOOTH:
			tempParam = IDC_SAWTOOTH;
			break;
		    case FULLWAVE:
			tempParam = IDC_FULLWAVE;
			break;
		    case USER:
			tempParam = IDC_USER;
			break;
		    default:				// uninitialised
			tempParam = IDC_SQUAREWAVE;
			temp = SQUARE;
			subtype = SQUARE;
			break;
		    }

		InitFourier(temp);
		for (nCtrlID = IDC_SCROLLBAR1 ; nCtrlID < IDC_SCROLLBAR1 + LEVELS ; nCtrlID++)
		    {
		    nIndex  = nCtrlID - IDC_SCROLLBAR1;
		    hCtrl = GetDlgItem (hDlg, nCtrlID) ;
		    SetScrollRange (hCtrl, SB_CTL, -255, 255, FALSE) ;
		    SetScrollPos   (hCtrl, SB_CTL, -level[nIndex], FALSE) ;
		    SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1, level[nIndex], TRUE) ;
		    }
		SetDlgItemInt(hDlg, IDC_FIBTIME, delay, TRUE);
		SetDlgItemInt(hDlg, IDM_STEPS, steps, TRUE);
		SetDlgItemInt(hDlg, IDC_HARMONICS, NumHarmonics, TRUE);
		CheckRadioButton(hDlg, IDC_SQUAREWAVE, IDC_USER, tempParam);
		hCtrl = GetDlgItem (hDlg, IDC_MOVINGWAVE);
//		SendMessage(hCtrl, BM_SETCHECK, 1, 0L);
		SendMessage(hCtrl, BM_SETCHECK, MovingWave, 0L);
//		SetFocus(GetDlgItem(hDlg, tempParam));
		if (FourierPreview.InitPreview(hDlg) < 0)
		    {
		    wsprintf(s, "Error: No memory for preview");
		    MessageBox (hDlg, s, "Paul's Graphics Viewer", MB_ICONEXCLAMATION | MB_OK);
		    }
		else
		    {
		    WavePreview(hDlg, Plot);
		    FourierPreview.Preview(hDlg);
		    }
		InvalidateRect(hDlg,NULL,FALSE);
	        return FALSE ;

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
			 level [nIndex] = max (-255, level [nIndex] - 1) ;
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
			 level [nIndex] = -255 ;
			 break ;
		    case SB_THUMBPOSITION :
		    case SB_THUMBTRACK :
			 level[nIndex] = -HIWORD (wParam) ;
			 break ;
		    default :
			 return FALSE ;
		    }
	 	SetScrollPos  (hCtrl, SB_CTL, -level [nIndex], TRUE) ;
		SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1, level[nIndex], TRUE) ;
		CheckRadioButton(hDlg, IDC_SQUAREWAVE, IDC_USER, IDC_USER);
		temp = USER;
		if (nIndex  < LEVELS / 2)					// sine sliders
		    FourierArray[nIndex + 1].magsin = (double)(level[nIndex]) / 255.0;
		else								// cosine sliders
		    FourierArray[nIndex - LEVELS / 2 + 1].magcos = (double)(level[nIndex]) / 255.0;
		InitFourier(temp);
		WavePreview(hDlg, Plot);
		FourierPreview.Preview(hDlg);
		InvalidateRect(hDlg,NULL,FALSE);
	        return (DLGPROC)TRUE ;

	  case WM_PAINT:
		BeginPaint(hDlg, &ps);
		WavePreview(hDlg, Plot);
		FourierPreview.Preview(hDlg);
		EndPaint(hDlg, &ps);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {                 
		    case IDC_TRIANGLE:
		    case IDC_SINWAVE:
		    case IDC_COSWAVE:
		    case IDC_SQUAREWAVE:
		    case IDC_SAWTOOTH:
		    case IDC_FULLWAVE:
		    case IDC_USER:
		    case IDC_IMPULSETRAIN:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_TRIANGLE:
				temp = TRIANGLE;
				break;
			    case IDC_SINWAVE:
				temp = SINEWAVE;
				break;
			    case IDC_COSWAVE:
				temp = COSINEWAVE;
				break;
			    case IDC_IMPULSETRAIN:
				temp = IMPULSE;
				break;
			    case IDC_SQUAREWAVE:
				temp = SQUARE;
				break;
			    case IDC_SAWTOOTH:
				temp = SAWTOOTH;
				break;
			    case IDC_FULLWAVE:
				temp = FULLWAVE;
				break;
			    case IDC_USER:
				temp = USER;
				break;
			    }

			CheckRadioButton(hDlg, IDC_SQUAREWAVE, IDC_USER, (int) LOWORD(wParam));
			InitFourier(temp);
			for (nCtrlID = IDC_SCROLLBAR1 ; nCtrlID < IDC_SCROLLBAR1 + LEVELS ; nCtrlID++)
			    {
			    nIndex  = nCtrlID - IDC_SCROLLBAR1;
			    hCtrl = GetDlgItem (hDlg, nCtrlID) ;
			    SetScrollRange (hCtrl, SB_CTL, -255, 255, FALSE) ;
			    SetScrollPos   (hCtrl, SB_CTL, -level[nIndex], TRUE) ;
			    SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1, level[nIndex], TRUE) ;
			    }
			WavePreview(hDlg, Plot);
			FourierPreview.Preview(hDlg);
			InvalidateRect(hDlg,NULL,FALSE);
		        return(DLGPROC)TRUE ;

		    case IDC_FIBTIME:
			temp_delay = GetDlgItemInt(hDlg, IDC_FIBTIME, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDC_HARMONICS:
			temp_Harmonics = GetDlgItemInt(hDlg, IDC_HARMONICS, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDM_STEPS:
			temp_steps = GetDlgItemInt(hDlg, IDM_STEPS, &bTrans, TRUE);
			if (temp_steps > MAXSTEPS)
			    temp_steps = MAXSTEPS;
			if (temp_steps < 1)
			    temp_steps = 1;
		        return (DLGPROC)TRUE ;

		    case IDOK:
			subtype = temp;
			if (temp_delay >= 0 && temp_delay < 2000)
			    delay = temp_delay;
			else
			    delay = 100;

			if (temp_Harmonics >= 0 && temp_Harmonics < FOURIERMAX)
			    NumHarmonics = temp_Harmonics;
			else
			    NumHarmonics = FOURIERMAX;
			steps = temp_steps;
			if (steps >= MAXSTEPS)
			    steps = MAXSTEPS;
			hCtrl = GetDlgItem (hDlg, IDC_MOVINGWAVE) ;
			MovingWave = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			for (nIndex = 0; nIndex < LEVELS; nIndex++)
			    {
			    if (nIndex  < LEVELS / 2)					// sine sliders
				FourierArray[nIndex + 1].magsin = (double)(level[nIndex]) / 255.0;
			    else								// cosine sliders
				FourierArray[nIndex - LEVELS / 2 + 1].magcos = (double)(level[nIndex]) / 255.0;
			    }

			FourierPreview.PreviewDib.CloseDibPtrs();
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			FourierPreview.PreviewDib.CloseDibPtrs();
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return(DLGPROC)FALSE ;
      }

/**************************************************************************
	Fourier script generator
**************************************************************************/

int	GenFourierScript(HWND hwnd, char *filename) 

    {
    char	s[120];
    FILE	*out;
    int		i, k;
    char	ascii[6];

    if ((out = fopen(filename, "w")) == NULL)
	{
	sprintf(s, "Cannot open output file %s\nDoes Folder exist?", filename);
	MessageBox (hwnd, s, "Animation", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	return -1;
	}

    fprintf(out, "-t%d -s\"%s\" ", threshold, PNGName/*, AnimData()*/);	// add quotes to filename to trap spaces in path
    fprintf(out, " -f%03d%03d%03d%04d%s\n", type, subtype, NumHarmonics, steps, WriteSliders());
    fprintf(out, "%d %d\n", threshold, steps);
    fprintf(out, "Palette=\n");
    for (i = 0, k = 0; i < threshold; i++, k++)
	{
	if (k == 20)							// group into lumps of 20
	    {
	    k = 0;
	    fprintf(out, "\n");
	    }
	ConvertRGB2ASCII(*(TrueCol.PalettePtr + i * 3 + 0), *(TrueCol.PalettePtr + i * 3 + 1), *(TrueCol.PalettePtr + i * 3 + 2), ascii);
	fprintf(out, "%s", ascii);
	}
    fprintf(out, "\n");
    fclose(out);
    return 0;
    }

/**************************************************************************
	Dialog For Setting Up Fourier Animation
**************************************************************************/

DLGPROC FAR PASCAL FourierAnimDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     char		*fileptr;
     HWND		hCtrl;
     BOOL		TempCheck;
     static	char	TempFile[MAX_PATH];

     switch (message)
	  {
	  case WM_INITDIALOG:
		cycleflag = FALSE;
		hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
		SendMessage(hCtrl, BM_SETCHECK, StartImmediately, 0L);
		SetUpFilename(ScriptFileName, "sci", "Fourier");
		SetUpFilename(PNGName, "animpng", "Fourier");
		SetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName);
		SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMPEGFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
		SendMessage(hCtrl, BM_SETCHECK, WriteMemFrames, 0L);
		hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
		SendMessage(hCtrl, BM_SETCHECK, WritePNGList, 0L);
	        return (DLGPROC)TRUE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_WRITEMPEGDIRECT:
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (WriteMPEGFrames)					// no point in writing other forms if direct to MPEG
			    {
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WritePNGFrames = WriteMemFrames = WritePNGList = FALSE;
			    sprintf(MPGFile, "%s", GenerateAnimFileName (MPGPath, PNGName));
			    SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, MPGFile);
			    }
			return (DLGPROC)TRUE;

		    case IDC_WRITEPNGDIRECT:
		    case IDC_WRITEMEMDIRECT:
		    case IDC_WRITEPNGFILELIST:
			hCtrl = GetDlgItem (hDlg, (int) LOWORD(wParam));
			TempCheck = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			if (TempCheck)						// see above
			    {
			    hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			    SendMessage(hCtrl, BM_SETCHECK, FALSE, 0L);
			    WriteMPEGFrames = FALSE;
			    }
//			sprintf(PNGFile, "%s", GenerateAnimFileName (ANIMPNGPath, PNGName));
//			SetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGFile);
			return (DLGPROC)TRUE;

		    case IDOK:
			GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, PNGName, MAX_PATH);
			fileptr = PNGName + strlen(PNGName);
			while (fileptr > PNGName && *fileptr != '.')
			    fileptr--;	
			if (*fileptr == '.')						// remove any extension
			    *fileptr = '\0';
			
			if (WriteMPEGFrames)						// generate MPEG filename
			    {
			    GetDlgItemText(hDlg, IDC_SEQUENCE_NAME, TempFile, MAX_PATH);
			    sprintf(MPGFile, "%s", GenerateMPEGFileName (MPGPath, TempFile));
			    }

			GetDlgItemText(hDlg, IDC_SCRIPT_FILENAME, ScriptFileName, 400);
			fileptr = ScriptFileName + strlen(ScriptFileName);
			while (fileptr > ScriptFileName && *fileptr != '.')
			    fileptr--;							// remove extension
			if (*fileptr == '.')
			    *fileptr = '\0';
			strcat(ScriptFileName, ".sci");
			hCtrl = GetDlgItem (hDlg, IDC_STARTNOW);
			StartImmediately = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGDIRECT);
			WritePNGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMEMDIRECT);
			WriteMemFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEMPEGDIRECT);
			WriteMPEGFrames = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			hCtrl = GetDlgItem (hDlg, IDC_WRITEPNGFILELIST);
			WritePNGList = (BYTE)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
			type = FOURIER;
			GenFourierScript(hDlg, ScriptFileName);
			EndDialog (hDlg, TRUE);
			return (DLGPROC)TRUE;

		    case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE;
      }


