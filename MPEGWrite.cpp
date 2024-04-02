// MPEGWrite.cpp    interface to ManpWIN for writing MPEG animation of frame sequence
//////////////////////////

#include <windows.h>
#include <stdio.h>
#include <setjmp.h>
#include "manpwin.h"
#include "manp.h"
#include "Dib.h"
#include "Anim.h"
#include "Colour.h"

#define	ENDOFSCRIPT	1

extern	int		DoMPEG(char *, int, int, int);
extern	void		CloseMPEGPtrs(void);	// give the MPEG memory back
extern	int		read_png_file(HWND, char *);
extern	char		*trailing(char *);
extern	BOOL		SaveMPGOpenDlg (HWND, LPSTR, LPSTR);
extern	int		InitScript(HWND, char *, int *);
extern	int		GenerateFractal(HWND, char *, char *, int, int);
extern	void		StatusBarAnimInfo (int, int);
extern	void		CloseScript(void);
extern	void		EndScript(int);
extern	void		InitAnimParamValues(void);
extern	void		UpdateAnimParamValues(void);
extern	void		MovePalette(CTrueCol *TrueCol, int PaletteShift, int threshold);

static	char		*Filenames[MAXANIM];	// animation frame PNG filenames

extern	BYTE		*DibFrames[MAXANIM];	// animation frames in memory
extern	jmp_buf		mark;			// Address for long jump to jump to when an error occurs
extern	char		ErrorMessage[];		// capture the error message for ron
extern	char		LSTFile[];		// list file for PNG animation frames
extern	char		ScriptFileName[];	// filename for the script file
extern	char		ANIMPNGPath[];		// path for PNG files and LST files

extern	AnimStruct	ANIM[];			// holds all the date for each animation frame
extern	int		TotalFrames;		// total number of animation frames
extern	FILE		*outfile, *statfile;	// file descriptors
extern	BOOL		AnimationForward;	// order of file frames
extern	int		file_type;		// if FILE_LST then we are processing a file list of PNG filenames 
	BOOL		RunMPEG = FALSE;	// are we in the middle of generating an MPEG file?
extern	char		MPGPath[];		// path for MPEG files
extern	BOOL		WriteMPEGFrames;	// write frames directly to an MPEG file
//extern	int		time_to_restart;	// time to restart?
extern	int		time_to_break;		// time to break out of animation?
extern	BOOL		AutoSaveFlag;
extern	int		PaletteShift;		// fractional palette addressing
extern	long		threshold;

static	int		SetupAnimationFrameList(char *, char *, int *width, int *height, int *TotalFrames);
static	void		ClosePNGLstPtrs(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////
extern	HWND	GlobalHwnd;			// This is the main windows handle
extern	CDib	Dib;
extern	RECT 	r;
extern	CTrueCol	TrueCol;		// palette info
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************
	Write MPEG File
**************************************************************************/

int	MPEGWrite(char *MPEGFile)
    {
    int	    width, height;

    if (WriteMPEGFrames)			// we write the MPEG Frame after calculating the fractal
	{
	if (InitScript(GlobalHwnd, ScriptFileName, &TotalFrames) < 0)
	    {
	    RunMPEG = FALSE;
	    return -1;
	    }
	width = Dib.DibWidth;
	height = Dib.DibHeight;
	}
    else if (file_type == FILE_LST)		// we are processing a file list of PNG filenames 
	{
	if (SetupAnimationFrameList(LSTFile, MPEGFile, &width, &height, &TotalFrames) < 0)
	    {
	    RunMPEG = FALSE;
	    return -1;
	    }
	}
    else
	{
	width = ANIM[0].animDIB.DibWidth;
	height = ANIM[0].animDIB.DibHeight;
	}

    RunMPEG = TRUE;
    InitAnimParamValues();			// We need to change param values for each frame
    if (setjmp(mark))				// MPEG error handling
	{
	MessageBox (GlobalHwnd, ErrorMessage, "Write MPEG", MB_ICONEXCLAMATION | MB_OK);
	if (outfile)
	    fclose(outfile);
	if (statfile)
	    fclose(statfile);
	if (WriteMPEGFrames)			// we write the MPEG Frame after calculating the fractal
	    CloseScript();
	else if (file_type == FILE_LST)		// we are processing a file list of PNG filenames 
	    ClosePNGLstPtrs();
	else
	    CloseMPEGPtrs();
	RunMPEG = FALSE;
	return -1;
	}
    DoMPEG(MPEGFile, TotalFrames, width, height);
    if (WriteMPEGFrames)			// we write the MPEG Frame after calculating the fractal
	CloseScript();
    else if (file_type == FILE_LST)		// we are processing a file list of PNG filenames 
	ClosePNGLstPtrs();
    RunMPEG = FALSE;
    EndScript(TotalFrames);
    file_type = FILE_PNG;			// restore back to default
    return 0;
    }

/**************************************************************************
	Load the next frame from a Dib
**************************************************************************/

BYTE *LoadFrameDib(int FrameNumber)

    {
    char	    s[240];
    static  BYTE    *ptr;
    int		    status = 0;
    long	    EndIter = TrueCol.FinalThreshold;

    if (WriteMPEGFrames)			// we write the MPEG Frame after calculating the fractal
	{
	UpdateAnimParamValues();		// We need to change param values for each frame
	status = GenerateFractal(GlobalHwnd, " ", ScriptFileName, TotalFrames, FrameNumber);
	if (abs(PaletteShift))
	    MovePalette(&TrueCol, PaletteShift, EndIter);			// move palette for animations
	if (status < 0)
	    {
	    CloseScript();
	    return NULL;
	    }
	else if (status != ENDOFSCRIPT)
	    ptr = Dib.DibPixels;
	}
    else if (file_type == FILE_LST)		// we write the MPEG Frame from a file list of PNG filenames 
	{
	if (read_png_file(GlobalHwnd, Filenames[(AnimationForward) ? FrameNumber : TotalFrames - FrameNumber - 1]) > 0)
	    ptr = Dib.DibPixels;
	else
	    return NULL;			// oops, no DIB available
	}
    else
	ptr = (AnimationForward) ? ANIM[FrameNumber].animDIB.DibPixels : ANIM[TotalFrames - FrameNumber - 1].animDIB.DibPixels;
    sprintf(s, "Writing MPEG Frame %d of %d", FrameNumber + 1, TotalFrames);
//    RefreshScreen();
    InvalidateRect(GlobalHwnd, &r, FALSE);

    SetWindowText(GlobalHwnd, s);			// Show formatted text in the caption bar
    StatusBarAnimInfo (FrameNumber, TotalFrames);
    return ptr;
    }

/**************************************************************************
	Display MPEG warning
**************************************************************************/

void	WarningMPEG(char *text)

    {
    MessageBox (GlobalHwnd, text, "MPEG Warning", MB_ICONEXCLAMATION | MB_OK);
    }

/**************************************************************************
	Load filenames into an array
**************************************************************************/

int	SetupAnimationFrameList(char *LSTFile, char *MPEGFile, int *width, int *height, int *TotalFrames)
    {
    int		i;
    FILE	*fp;					// list file   
    char	s[MAXLINE];
    char	buf[MAXDATALINE]; 
    int		FileCount = 0;
    char	ListFilename[MAX_PATH];

    *width = 0;
    *height = 0;

    sprintf(ListFilename, "%s\\%s", ANIMPNGPath, LSTFile);
    for (i = 0; i < MAXANIM; ++i)
	Filenames[i] = NULL;				// start with a clean slate

    if ((fp = fopen(ListFilename, "r")) == NULL)
	{
	wsprintf(s, "Can't open PNG list file: %s for read", LSTFile);
	MessageBox (GlobalHwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    while (fgets(buf, MAXDATALINE, fp) && FileCount < MAXANIM)
	{
	if (*buf == ';')						// comment
    	    continue;
	if (*buf == '#')						// command
	    {
	    if (strnicmp(buf, "#MPEG=", 6) == 0)			// Okay, we verify it's a manpwin list file
		{
		sscanf(buf + 6, "%d %d", width, height); 	    
		continue;
		}
	    if (strnicmp(buf, "#WriteDirectory=", 16) == 0)		// if blank, overwrite file or change to write dir
		{
		strcpy(MPGPath, trailing(buf + 16));
		if (*(MPGPath + strlen(MPGPath) - 1) != '\\')		// don't expect user to do this!!
		    strcat(MPGPath, "\\");
		continue;
		}
	    if (strnicmp(buf, "#MPEGName=", 10) == 0)
		{
		if ((i= (int)strlen(buf+10)) > 3)				// Do we have a valid filename?
		    strcpy(MPEGFile, trailing(buf + 10));
		else
		    {
		    if (SaveMPGOpenDlg (GlobalHwnd, MPEGFile, "ManpWIN") < 0)
			break;
		    }
		continue;
		}
	    }
	else
	    {
	    if ((Filenames[FileCount] = new char[MAX_PATH]) == NULL)
		{
		wsprintf(s, "Can't get memory for PNG list file");
		MessageBox (GlobalHwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		return -1;
		}

	    strcpy(Filenames[FileCount], trailing(buf));		// remove trailing spaces or newlines
	    FileCount++;
	    }
	}
    *TotalFrames = FileCount;
    fclose(fp);
    if (*width == 0 || *height == 0)
	{
	sprintf(s, "Can't read parameters in PNG list file: %s", LSTFile);
	MessageBox (GlobalHwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    return 0;
    }

/**************************************************************************
	Animate Gif files
**************************************************************************/

void	ClosePNGLstPtrs(void)

    {
    int	i;

    for (i = 0; i < TotalFrames; ++i)	// release all frames
	{
	if (Filenames[i])
	    delete Filenames[i];
	Filenames[i] = NULL;
	}
    file_type = FILE_PNG;			// restore back to default
    }
