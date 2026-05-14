// MPEGWrite.cpp    interface to ManpWIN for writing MPEG animation of frame sequence
//////////////////////////

#include <windows.h>
#include <stdio.h>
#include <setjmp.h>
#include <vector>
#include "manpwin.h"
#include "manp.h"
#include "Dib.h"
//#include "Anim.h"
#include "Colour.h"
#include "SafeStrings.h"

#define	ENDOFSCRIPT	1

extern	int		DoMPEG(char *, int, int, int);
extern	void		CloseMPEGPtrs(void);	// give the MPEG memory back
extern	int		read_png_file(HWND, char *);
extern	char		*trailing(char *);
extern	BOOL		SaveMPGOpenDlg (HWND, LPSTR, LPSTR);

static	char		*Filenames[MAXANIM];	// animation frame PNG filenames

extern	BYTE		*DibFrames[MAXANIM];	// animation frames in memory
extern	jmp_buf		mark;			// Address for long jump to jump to when an error occurs
extern	char		ErrorMessage[];		// capture the error message for ron
extern	char		LSTFile[];		// list file for PNG animation frames
extern	char		ScriptFileName[];	// filename for the script file
extern	char		ANIMPNGPath[];		// path for PNG files and LST files

//extern	std::vector<AnimStruct> ANIM;		// holds all the date for each animation frame
//extern	int		gTotalFrames;		// total number of animation frames
extern	FILE		*outfile, *statfile;	// file descriptors
//extern	BOOL		AnimationForward;	// order of file frames
extern	int		file_type;		// if FILE_LST then we are processing a file list of PNG filenames 
extern	char		MPGPath[];		// path for MPEG files
extern	BOOL		WriteMPEGFrames;	// write frames directly to an MPEG file

static	int		SetupAnimationFrameList(char *, char *, int *width, int *height, int *TotalFrames);
static	void		ClosePNGLstPtrs(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//extern	HWND	GlobalHwnd;			// This is the main windows handle
//////////////////////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************
	Write MPEG File
**************************************************************************/

int	MPEGWrite(char *MPEGFile)
    {
    int	    width, height;

    if (WriteMPEGFrames)			// we write the MPEG Frame after calculating the fractal
	{
	if (gManp->InitScript(gManp->GlobalHwnd, ScriptFileName, &gManp->TotalFrames) < 0)
	    {
	    gManp->RunMPEG = FALSE;
	    return -1;
	    }
	width = gManp->Dib.DibWidth;
	height = gManp->Dib.DibHeight;
	}
    else if (file_type == FILE_LST)		// we are processing a file list of PNG filenames 
	{
	if (SetupAnimationFrameList(LSTFile, MPEGFile, &width, &height, &gManp->TotalFrames) < 0)
	    {
	    gManp->RunMPEG = FALSE;
	    return -1;
	    }
	}
    else
	{
	width = gManp->ANIM[0].width;
	height = gManp->ANIM[0].height;
	}

    gManp->RunMPEG = TRUE;
    gManp->InitAnimParamValues();			// We need to change param values for each frame
    if (setjmp(mark))				// MPEG error handling
	{
	MessageBox (gManp->GlobalHwnd, ErrorMessage, "Write MPEG", MB_ICONEXCLAMATION | MB_OK);
	if (outfile)
	    fclose(outfile);
	if (statfile)
	    fclose(statfile);
	if (WriteMPEGFrames)			// we write the MPEG Frame after calculating the fractal
	    gManp->CloseScript();
	else if (file_type == FILE_LST)		// we are processing a file list of PNG filenames 
	    ClosePNGLstPtrs();
	else
	    CloseMPEGPtrs();
	gManp->RunMPEG = FALSE;
	return -1;
	}
    DoMPEG(MPEGFile, gManp->TotalFrames, width, height);
    if (WriteMPEGFrames)			// we write the MPEG Frame after calculating the fractal
	gManp->CloseScript();
    else if (file_type == FILE_LST)		// we are processing a file list of PNG filenames 
	ClosePNGLstPtrs();
    gManp->RunMPEG = FALSE;
    gManp->EndScript(gManp->TotalFrames);
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
    long	    EndIter = gManp->TrueCol.FinalThreshold;
    double	    delay = 0.0;
    if (WriteMPEGFrames)			// we write the MPEG Frame after calculating the fractal
	{
	gManp->UpdateAnimParamValues();		// We need to change param values for each frame
	status = gManp->GenerateFractal(gManp->GlobalHwnd, " ", ScriptFileName, gManp->TotalFrames, FrameNumber, delay);
	if (abs(gManp->PaletteShift))
	    gManp->TrueCol.MovePalette(&gManp->TrueCol, gManp->PaletteShift, EndIter);			// move palette for animations
	if (status < 0)
	    {
	    gManp->CloseScript();
	    return NULL;
	    }
	else if (status != ENDOFSCRIPT)
	    ptr = gManp->Dib.DibPixels.data();
	}
    else if (file_type == FILE_LST)		// we write the MPEG Frame from a file list of PNG filenames 
	{
	if (read_png_file(gManp->GlobalHwnd, Filenames[(gManp->AnimationForward) ? FrameNumber : gManp->TotalFrames - FrameNumber - 1]) > 0)
	    ptr = gManp->Dib.DibPixels.data();
	else
	    return NULL;			// oops, no DIB available
	}
    else
	{
	const AnimStruct& A = (gManp->AnimationForward) ? gManp->ANIM[FrameNumber] : gManp->ANIM[gManp->TotalFrames - FrameNumber - 1];

	// Rebuild Dib from animation frame
	if (!BuildDibFromAnimFrame(A, gManp->Dib))
	    return NULL;

	ptr = gManp->Dib.DibPixels.data();
	}
//	ptr = (AnimationForward) ? ANIM[FrameNumber].animDIB.DibPixels : ANIM[TotalFrames - FrameNumber - 1].animDIB.DibPixels;
    SAFE_SPRINTF(s, "Writing MPEG Frame %d of %d", FrameNumber + 1, gManp->TotalFrames);
//    RefreshScreen();
    InvalidateRect(gManp->GlobalHwnd, &gManp->r, FALSE);

    SetWindowText(gManp->GlobalHwnd, s);			// Show formatted text in the caption bar
    gManp->StatusBarAnimInfo (FrameNumber, gManp->TotalFrames);
    return ptr;
    }

/**************************************************************************
	Display MPEG warning
**************************************************************************/

void	WarningMPEG(char *text)

    {
    MessageBox (gManp->GlobalHwnd, text, "MPEG Warning", MB_ICONEXCLAMATION | MB_OK);
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

    SAFE_SPRINTF(ListFilename, "%s\\%s", ANIMPNGPath, LSTFile);
    for (i = 0; i < MAXANIM; ++i)
	Filenames[i] = NULL;				// start with a clean slate

    if ((fp = fopen(ListFilename, "r")) == NULL)
	{
	_snprintf_s(s, MAXLINE, _TRUNCATE, "Can't open PNG list file: %s for read", LSTFile);
	MessageBox (gManp->GlobalHwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
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
		    strcat_s(MPGPath, MAX_PATH, "\\");
		continue;
		}
	    if (strnicmp(buf, "#MPEGName=", 10) == 0)
		{
		if ((i= (int)strlen(buf+10)) > 3)				// Do we have a valid filename?
		    strcpy(MPEGFile, trailing(buf + 10));
		else
		    {
		    if (SaveMPGOpenDlg (gManp->GlobalHwnd, MPEGFile, "ManpWIN") < 0)
			break;
		    }
		continue;
		}
	    }
	else
	    {
	    if ((Filenames[FileCount] = new char[MAX_PATH]) == NULL)
		{
		_snprintf_s(s, MAXLINE, _TRUNCATE, "Can't get memory for PNG list file");
		MessageBox (gManp->GlobalHwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
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
	SAFE_SPRINTF(s, "Can't read parameters in PNG list file: %s", LSTFile);
	MessageBox (gManp->GlobalHwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
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

    for (i = 0; i < gManp->TotalFrames; ++i)	// release all frames
	{
	if (Filenames[i])
	    delete Filenames[i];
	Filenames[i] = NULL;
	}
    file_type = FILE_PNG;			// restore back to default
    }
