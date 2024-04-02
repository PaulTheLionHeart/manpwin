/*
    FFT.CPP a module for Exploring Fast Fourier Transform
    
    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<conio.h>
#include	<string.h>
#include	<stdio.h>
#include	<windowsx.h>
#include	"manp.h"
#include	"resource.h"
#include	"fractype.h"
#include	"Plot.h"
#include	"Dib.h"
#include	"preview.h"

#define	TIME_HEIGHT		240		// time domain window.
#define	TIME_WIDTH		400
#define	TIME_HOR_OFFSET		158
#define	TIME_VERT_OFFSET	25

#define	FREQ_HEIGHT		240		// time domain window.
#define	FREQ_WIDTH		395
#define	FREQ_HOR_OFFSET		620
#define	FREQ_VERT_OFFSET	25

#define	MAX_N		1024

extern	int	user_data(HWND);

extern	int	subtype;			// see below
extern	PAINTSTRUCT 	ps;

int	steps;

#define	LEVELS		32		// number of harmonic sliders

#define	SQUARE		'0'
#define	TRIANGLE	'1'
#define	SAWTOOTH	'2'
#define	FULLWAVE	'3'
#define	HALFWAVE	'4'
#define	SINEWAVE	'5'
#define	COSINEWAVE	'6'
#define	IMPULSE		'9'
#define	USER		'U'

#define	FFTREAL		0
#define	FFTIMAG		1
#define	REALIMAG	2
#define	FFTPOWER	3

BYTE	OutputType = REALIMAG;

static	short	level[LEVELS];
//static	double	real[MAX_N], imag[MAX_N];
static	double	real[LEVELS], imag[LEVELS];
//double	*real, *imag;
extern	int	bits_per_pixel, xdots, ydots;

extern	double ctable[];
extern	double stable[];
extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts

extern	CTrueCol    TrueCol;			// palette info
extern	CPlot	Plot;		// image plotting routines 

static	CPreview	TimeScreen, FreqScreen;

////////////////////////////
// Trig tables - refer TRIG.CPP
////////////////////////////

double cosgen(int i)

    {
    return ctable[i];
    }


double singen(int i)

    {
    return stable[i];
    }


/**************************************************************************
	Fast Fourier Transform Subroutine
**************************************************************************/

void	fft(int n, int m)

    {
    double	xt, yt, c, s;
    register int	i, j, k;
    int	l, n1, n2, ie, ia, jt, trig_ptr, temp;

    trig_ptr = MAX_N / n;			/* point to right part of cos & sin */
    n2 = n;
    for (k = 0; k < m; ++k)
	{
//	cursor_at(16, 38);
//	printf("%d", k);
	n1 = n2;
	n2 >>= 1;
	jt = n2 >> 1;
	for (i = 0; i < n; i += n1)
	    {
	    l = i + n2;
	    xt = real[i] - real[l];
	    real[i] += real[l];
	    real[l] = xt;
	    yt = imag[i] - imag[l];
	    imag[i] += imag[l];
	    imag[l] = yt;
	    }
	if (k != m - 1)
	    {
	    ie = n / n1;
	    ia = 0;
	    for (j = 1; j < n2; ++j)
		{
		ia += ie;
		if (j == jt)
		    {
		    for (i = j; i < n; i += n1)
			{
			l = i + n2;
			xt = real[i] - real[l];
			real[i] += real[l];
			yt = imag[i] - imag[l];
			imag[i] += imag[l];
			real[l] = yt;
			imag[l] = - xt;
			}
		    }
		else
		    {
		    temp = ia * trig_ptr;
		    c = cosgen(temp);
		    s = singen(temp);
		    for (i = j; i < n; i += n1)
			{
			l = i + n2;
			xt = real[i] - real[l];
			real[i] += real[l];
			yt = imag[i] - imag[l];
			imag[i] += imag[l];
			real[l] = c * xt + s * yt;
			imag[l] = c * yt - s * xt;
			}
		    }
		}
	    }
	}
				/* unscramble order of samples */
    j = 1;
    for (i = 1; i < n; ++i)
	{
	if (i < j)
	    {
	    xt = real[j - 1];
	    real[j - 1] = real[i - 1];
	    real[i - 1] = xt;
	    yt = imag[j - 1];
	    imag[j - 1] = imag[i - 1];
	    imag[i - 1] = yt;
	    }
	k = n >> 1;
	while (k < j)
	    {
	    j -= k;
	    k >>= 1;
	    }
	j += k;
	}
    }

/**************************************************************************
	Set up screen scaling and other inits
***************************************************************************/

void	InitFFT(char WaveType)

    {
    int	i;

    switch (WaveType)
	{
	case IMPULSE:
	    for (i = 0; i < LEVELS; i++)
		level[i] = -255;
	    level[LEVELS/2] = 255;
	    break;
	case SAWTOOTH:
	    for (i = 0; i < LEVELS; i++)
		level[i] = (i * 510) / LEVELS - 255;
	    break;
	case SQUARE:
	    for (i = 0; i < LEVELS/2; i++)
		level[i] = -255;
	    for (i = LEVELS/2; i < LEVELS; i++)
		level[i] = 255;
	    break;
	case TRIANGLE:
	    for (i = 0; i < LEVELS/2; i++)
		level[i] = (i * 510) / (LEVELS/2) - 255;
	    for (i = LEVELS/2; i < LEVELS; i++)
		level[i] = 765 - (i * 510) / (LEVELS/2);
	    break;
	case FULLWAVE:
	    for (i = 0; i < LEVELS/2; i++)
		level[i] = level[i+LEVELS/2] = (int)(510.0 * sin(2.0*PI*i / LEVELS)) - 255;
	    break;
	case HALFWAVE:
	    for (i = 0; i < LEVELS/2; i++)
		level[i] = (int)(510.0 * sin(2.0*PI*i / LEVELS)) - 255;
	    for (i = LEVELS/2; i < LEVELS; i++)
		level[i] = -255;
	    break;
	case SINEWAVE:
	    for (i = 0; i < LEVELS; i++)
		level[i] = (int)(255.0 * sin(2.0*PI*i / LEVELS));
	    break;
	case COSINEWAVE:
	    for (i = 0; i < LEVELS; i++)
		level[i] = (int)(255.0 * sin(PI/2.0 + 2.0*PI*i / LEVELS));
	    break;
	}

    for (i = 0; i < LEVELS; i++)
	{
	real[i] = (double)level[i] / 255.0;
	imag[i] = 0.0;
//	*(real + i) = (double)level[i] / 255.0;
//	*(imag + i) = 0.0;
	}
    return;
    }

/**************************************************************************
	Draw Preview Fourier Images
***************************************************************************/


int	TimePreview(HWND hwnd, CPlot Plot)

    {
    int	j, jump = TimeScreen.PreviewWidth/LEVELS;
    int	start = jump/2, end = 0;

    memset(TimeScreen.PreviewDib.DibPixels, 0, (WIDTHBYTES((DWORD)TimeScreen.PreviewWidth * (DWORD)bits_per_pixel)) * TimeScreen.PreviewHeight);	// set background to black
    Plot.InitPlot(1000000L, &TrueCol, wpixels, xdots, ydots, TimeScreen.PreviewWidth, TimeScreen.PreviewHeight, TimeScreen.PreviewDib.BitsPerPixel, &TimeScreen.PreviewDib, 0);
    for (j = 1; j < LEVELS; ++j)
	{
	end = start + jump; 
	Plot.genline(start, level[j - 1] * 118 / 255 + 120, end, level[j] * 118 / 255 + 120, 0x0000ff00);
	start = end;
	}
    end = start + jump; 
    Plot.genline(start, level[LEVELS-1]*118/255+120, end, level[0]*118/255+120, 0x0000ff00);
    ShowWindow (hwnd, SW_SHOWNORMAL);
    return 0;
    }

/**************************************************************************
	Draw Preview Fourier Images
***************************************************************************/

int	FreqPreview(HWND hwnd, CPlot Plot)

    {
    int	j, jump = FreqScreen.PreviewWidth/LEVELS;
    int	start = jump/2, end = 0;
    double	rms;

    Plot.InitPlot(1000000L, &TrueCol, wpixels, xdots, ydots, FreqScreen.PreviewWidth, FreqScreen.PreviewHeight, FreqScreen.PreviewDib.BitsPerPixel, &FreqScreen.PreviewDib, 0);
    FreqScreen.PreviewDib.ClearDib(0L);
//memset(FreqScreen.PreviewDib.DibPixels, 127, (WIDTHBYTES((DWORD)PreviewWidth * (DWORD)bits_per_pixel)) * PreviewHeight);	// set background to black
    for (j = 0; j < LEVELS; ++j)
	{
	switch (OutputType)
	    { 
	    case FFTREAL:
		Plot.genline(start, 120, start, (int)(real[j]*30.0)+120, 0x00ffff00);
		break;
	    case FFTIMAG:
		Plot.genline(start, 120, start, (int)(imag[j]*30.0)+120, 0x00ff0000);
		break;
	    case REALIMAG:
		Plot.genline(start, 120, start, (int)(real[j]*30.0)+120, 0x00ffff00);
		Plot.genline(start+2, 120, start+2, (int)(imag[j]*30.0)+120, 0x00ff0000);
		break;
	    case FFTPOWER:
		rms = sqrt(real[j]*real[j]+imag[j]*imag[j]);
		Plot.genline(start, 2, start, (int)(rms*30.0), 0x00ffffff);
		break;
	    }
	start += jump;
	}
    ShowWindow (hwnd, SW_SHOWNORMAL);
    return 0;
    }

/**************************************************************************
	Dialog Control for Displaying Fast Fourier Transforms
**************************************************************************/

DLGPROC FAR PASCAL FastFourierDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     static	HANDLE	hCursor;
     static     char	temp, temp1;
     static     UINT	tempParam, tempParam1;

     short		nCtrlID, nIndex;
     HWND		hwndParent, hCtrl;
     char		s[MAXLINE];

     // set the dimensions and location of the windows
     TimeScreen.HorOffset = TIME_HOR_OFFSET;				// dimensions of time domain window
     TimeScreen.VertOffset = TIME_VERT_OFFSET;
     TimeScreen.PreviewHeight = TIME_HEIGHT;
     TimeScreen.PreviewWidth = TIME_WIDTH;
     FreqScreen.HorOffset = FREQ_HOR_OFFSET;				// dimensions of frequency domain window
     FreqScreen.VertOffset = FREQ_VERT_OFFSET;
     FreqScreen.PreviewHeight = FREQ_HEIGHT;
     FreqScreen.PreviewWidth = FREQ_WIDTH;

     switch (message)
	  {
	  case WM_INITDIALOG:
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
		    case HALFWAVE:
			tempParam = IDC_HALFWAVE;
			break;
		    case USER:
			tempParam = IDC_USER;
			break;
		    default:				// uninitialised
			tempParam = IDC_SQUAREWAVE;
			temp = SQUARE;
			break;
		    }
		CheckRadioButton(hDlg, IDC_SQUAREWAVE, IDC_USER, tempParam);
//		subtype = temp;
		InitFFT(temp);
		for (nCtrlID = IDC_SCROLLBAR1 ; nCtrlID < IDC_SCROLLBAR1 + LEVELS ; nCtrlID++)
		    {
		    nIndex  = nCtrlID - IDC_SCROLLBAR1;
		    hCtrl = GetDlgItem (hDlg, nCtrlID) ;
		    SetScrollRange (hCtrl, SB_CTL, -255, 255, FALSE) ;
		    SetScrollPos   (hCtrl, SB_CTL, -level[nIndex], FALSE) ;
		    SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1, level[nIndex], TRUE) ;
		    }
	        temp1 = OutputType;
	        switch (OutputType)
		    { 
		    case FFTREAL:
			tempParam1 = IDC_FFT_REAL;
			break;
		    case FFTIMAG:
			tempParam1 = IDC_FFT_IMAG;
			break;
		    case REALIMAG:
			tempParam1 = IDC_FFT_REALIMAG;
			break;
		    case FFTPOWER:
			tempParam1 = IDC_FFT_POWER;
			break;
//		    default:				// uninitialised
//			tempParam1 = IDC_FFT_REALIMAG;
//			temp1 = REALIMAG;
//			break;
		    }
		CheckRadioButton(hDlg, IDC_FFT_REAL, IDC_FFT_POWER, tempParam1);
//		OutputType = temp1;
//		SetFocus(GetDlgItem(hDlg, tempParam));
		if (TimeScreen.InitPreview(hDlg) < 0)
		    {
		    wsprintf(s, "Error: No memory for preview");
		    MessageBox (hDlg, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
//		    delete [] real;
//		    delete [] imag;
		    EndDialog (hDlg, FALSE);
		    }
		if (FreqScreen.InitPreview(hDlg) < 0)
		    {
		    wsprintf(s, "Error: No memory for FFT");
		    MessageBox (hDlg, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		    TimeScreen.PreviewDib.CloseDibPtrs();
//		    delete [] real;
//		    delete [] imag;
		    EndDialog (hDlg, FALSE);
		    }
	        temp1 = OutputType;
		InvalidateRect(hDlg,NULL,FALSE);
	        return (DLGPROC)TRUE ;

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
		InvalidateRect(hDlg,NULL,FALSE);
	        return (DLGPROC)TRUE ;

	  case WM_PAINT:
		BeginPaint(hDlg, &ps);
		InitFFT(temp);
		TimePreview(hDlg, Plot);
		TimeScreen.Preview(hDlg);
		fft(32, 5);
	        FreqPreview(hDlg, Plot);
		FreqScreen.Preview(hDlg);
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
		    case IDC_HALFWAVE:
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
			    case IDC_HALFWAVE:
				temp = HALFWAVE;
				break;
			    case IDC_USER:
				temp = USER;
				break;
			    }

			CheckRadioButton(hDlg, IDC_SQUAREWAVE, IDC_USER, (int) LOWORD(wParam));
			subtype = temp;
			InitFFT(temp);
			for (nCtrlID = IDC_SCROLLBAR1 ; nCtrlID < IDC_SCROLLBAR1 + LEVELS ; nCtrlID++)
			    {
			    nIndex  = nCtrlID - IDC_SCROLLBAR1;
			    hCtrl = GetDlgItem (hDlg, nCtrlID) ;
			    SetScrollRange (hCtrl, SB_CTL, -255, 255, FALSE) ;
			    SetScrollPos   (hCtrl, SB_CTL, -level[nIndex], TRUE) ;
			    SetDlgItemInt (hDlg,  nIndex + IDC_LABEL1, level[nIndex], TRUE) ;
			    }
			InvalidateRect(hDlg,NULL,FALSE);
		        return (DLGPROC)TRUE ;

		    case IDC_FFT_REAL:
		    case IDC_FFT_IMAG:
		    case IDC_FFT_REALIMAG:
		    case IDC_FFT_POWER:
		        switch ((int) LOWORD(wParam))
			    {
			    case IDC_FFT_REAL:
				temp1 = FFTREAL;
				break;
			    case IDC_FFT_IMAG:
				temp1 = FFTIMAG;
				break;
			    case IDC_FFT_REALIMAG:
				temp1 = REALIMAG;
				break;
			    case IDC_FFT_POWER:
				temp1 = FFTPOWER;
				break;
			    }

			CheckRadioButton(hDlg, IDC_FFT_REAL, IDC_FFT_POWER, (int) LOWORD(wParam));
			OutputType = temp1;
//			InitFFT(temp);
			InvalidateRect(hDlg,NULL,FALSE);
		        return (DLGPROC)TRUE ;

		    case IDOK:
		    case IDCANCEL:
			TimeScreen.PreviewDib.CloseDibPtrs();
			FreqScreen.PreviewDib.CloseDibPtrs();
//			delete [] real;
//			delete [] imag;
			EndDialog (hDlg, FALSE);
			return (DLGPROC)FALSE;
		   }
		   break;
	    }
      return (DLGPROC)FALSE ;
      }



