/*------------------------------------------------
   HTMLHELP.CPP -- Compiled HTML Help
  ------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdio.h>
#include <htmlhelp.h>	// PHD 24/10/2007 tp allow *.CHM files
#include <tchar.h>
#include "manp.h"

//extern	    TCHAR	*FullHelpFile;			// points to full path for help file


/**************************************************************************
	Get Pathname of MANPWIN.EXE file
**************************************************************************/

int	GetManpWINFilePath(char *buffer)
    {
    char	*ShortName;

    GetModuleFileName(NULL, buffer, FILENAME_MAX);  // Get path to this program
    if (ShortName = strrchr(buffer, '\\'))
	*++ShortName = '\0';
    return 0;
    }

HWND	DoHtmlHelp(HWND hwnd, char *HelpFile, UINT uCommand)

    {
    HWND	hwndHelp;
    char	Path[MAX_PATH];
    char	FullHelpFile[MAX_PATH];

    GetManpWINFilePath(Path);
    sprintf(FullHelpFile, "%s%s", Path, HelpFile);
    hwndHelp = HtmlHelp(hwnd, FullHelpFile, uCommand, (DWORD)0);
    if(hwndHelp == 0)
	{
	char    TempString[MAXLINE];

	sprintf(TempString, "Cannot find Help File %s", FullHelpFile);
	MessageBox (hwnd, TempString, "HtmlHelp()", MB_ICONEXCLAMATION | MB_OK);
	}

    return	hwndHelp;
    }