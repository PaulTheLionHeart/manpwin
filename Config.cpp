/**************************************************************************
	CONFIG.CPP
	Read and interpret config files
**************************************************************************/

#include <windows.h>
#include <direct.h>
#include <stdio.h>
#include <shlobj.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "manpwin.h"
#include "resource.h"
#include "colour.h"

#define MAXLINE			480
#define NONE			0		// opening bitmap loading flags
#define DEFAULT			1
#define NEW			2
#define FILE_ERROR		3

//int	CheckReg(char *, char *);
int	GetManpINIPath(HWND, char *, char *);
//int	GetPGVHelpPath(HWND);
int	GetFilePath(HWND, char *, char *);
int	ReadConfigFile(HWND hwnd, char *filename);

extern	char	*trailing(char *);

	char	WorkingDir[_MAX_PATH];
static	char	ManpINIFoundDir[_MAX_PATH];

static	char	ManpPath[_MAX_PATH] = "";		// path for ManpWin.EXE where needed
	char	GIFPath[_MAX_PATH] = "";		// path for GIF files
	char	MPGPath[_MAX_PATH] = "";		// path for MPG files
	char	PNGPath[_MAX_PATH] = "";		// path for PNG files
	char	ANIMPNGPath[_MAX_PATH] = "";		// path for PNG files for animation
	char	COLPath[_MAX_PATH] = "";		// path for COL files
	char	MAPPath[_MAX_PATH] = "";		// path for MAP files
	char	SCIPath[_MAX_PATH] = "";		// path for SCI files
	char	PARPath[_MAX_PATH] = "";		// path for PAR files
	char	KFRPath[_MAX_PATH] = "";		// path for KFR files
	char	IFSPath[_MAX_PATH] = "";		// path for IFS files
	char	LSYSPath[_MAX_PATH] = "";		// path for LSYS files
	char	FRMPath[_MAX_PATH] = "";		// path for Formula files

	char	PNGFile[_MAX_PATH] = "";		// PNG file
	char	MPGFile[_MAX_PATH] = "";		// MPG file
//	char	GIFFile[_MAX_PATH] = "";		// GIF file
	char	COLFile[_MAX_PATH] = "";		// COL file
	char	MAPFile[_MAX_PATH] = "";		// MAP file
	char	SCIFile[_MAX_PATH] = "";		// SCI file
	char	FracPARFile[_MAX_PATH] = "";		// Fractint PAR file
	char	PARFile[_MAX_PATH] = "";		// PAR file
	char	KFRFile[_MAX_PATH] = "";		// KFR file
	char	IFSFile[_MAX_PATH] = "";		// IFS file
	char	LSYSFile[_MAX_PATH] = "";		// LSYS file
	char	FRMFile[_MAX_PATH] = "";		// Formula file
	char	LSTFile[_MAX_PATH] = "";		// list file for PNG animation frames

extern	char	ScriptFileName[];			// base name for PNG file animation sequence
extern	char	PNGName[];				// base name for PNG file sequence
extern	BOOL	ZoomEdge;				// Zooming process
extern	BOOL	UseFractintPalette;			// standard EGA palette
extern	char	statname[];				// MPEG stat file. If statname[] == '-', stats sent to stdout.
extern	int	NumberThreads;				// Number of threads to be used. If 0, no multi-threading. Used in perturbatio only
extern	CTrueCol    TrueCol;		// palette info
static	char	Extension[25];				// default extension
static	BOOL	UserPath = TRUE;			// use the path to the user data area created when ManpWIN was installed

/**************************************************************************
	Get Pathname of MANPWIN.EXE file
**************************************************************************/

/*
int	GetManpWinFilePath(char *buffer)
    {
    char	*ShortName;

    GetModuleFileName(NULL, buffer, FILENAME_MAX);  // Get path to this program
    if(ShortName = strrchr(buffer, '\\'))
	*++ShortName = '\0';
    return 0;
    }
*/

/**************************************************************************
	Get Pathname for Manpwin.ini
	Try both program path and Windows directory
	Results in global
	 
	ManpINIFoundDir
**************************************************************************/

int	GetManpINIPath(HWND hwnd, char *ManpWinINIFile, char *ManpName)
//int	ReadConfig(HWND hwnd)
    {
    // CalendarReadPath points to a local variable in ReadConfig, only used for debug

    BYTE	status;
    HANDLE	fi;	// handle to ini file
//    char	CalendarWritePath[MAX_PATH];
//    char	ManpWinINIFile[MAX_PATH];

//    strcat(CalendarReadPath, "Calendar.INI");	//IB 2010-07-26	this is different to PgvWritePath, because you insist in including trailing '\' in directories, unlike the rest of Windows

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, ManpINIFoundDir)))
	{
	strcat(ManpINIFoundDir, "\\ManpWIN");
	sprintf(ManpWinINIFile, "%s%s", ManpINIFoundDir, "\\MANPWIN.INI");	// %AppData%\ManpWIN\ManpWIN.ini
	if (!CreateDirectory(ManpINIFoundDir, NULL))
	    {	// Directory Creation failed - probably exists
	    fi = CreateFile(ManpWinINIFile, GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	    if (fi == INVALID_HANDLE_VALUE)	// unable to open %AppData%\ManpWIN\ManpWIN.ini
		{	// try path to this program
		status = ReadConfigFile(hwnd, ManpWinINIFile);
//		strcpy(CalendarINIFile, CalendarWritePath);		// directory to write new ini file
		return status;
		}
	    else    // read & write %AppData%\CALENDAR\Calendar.ini
		{
		CloseHandle(fi);
//		strcpy(CalendarReadPath, CalendarWritePath);	// %AppData%\ManpWIN\ManpWIN.ini
		status = ReadConfigFile(hwnd, ManpWinINIFile);
		return status;
		}
	    }
	else    // %AppData% created - obviously does not contain manpwin.ini
	    {
	    status = ReadConfigFile(hwnd, ManpWinINIFile);
//	    strcat(CalendarWritePath, "\\ManpWIN.INI");	// %AppData%\ManpWIN\ManpWIN.ini
//	    strcpy(CalendarINIFile, CalendarWritePath);		// directory to write new ini file
	    return status;
	    }
	}

    // %AppData% does not exist, probably old OS
    status = ReadConfigFile(hwnd, ManpWinINIFile);
    return status;
    }

/**************************************************************************
	Read and interpret "ManpWin.INI"
**************************************************************************/

int	ReadConfig(HWND hwnd)

    {
    char	buf[MAXLINE];
    char	ConfigFile[MAXLINE];
    short	status;

    *ManpINIFoundDir = '\0';
    GetFilePath(hwnd, WorkingDir, "MANPWIN.EXE");
    strcpy(ManpPath, WorkingDir);			// what is the path for ManpWin.EXE?
    status = GetManpINIPath(hwnd, ConfigFile, "ManpWin.INI");	// find MANPWIN.INI in windows or path

#ifdef DEBUG
    wsprintf(buf, "Path = <%s>", ConfigFile);
    //wsprintf(buf, "c:\\windows\\PGV.INI");
    MessageBox (hwnd, buf, "ReadConfig", MB_ICONEXCLAMATION | MB_OK);
#endif

    if (*ManpINIFoundDir && UserPath)				// user prefers default locations
	{
//	_chdir(ManpPath);

	sprintf(ANIMPNGPath, "%s\\animpng", ManpINIFoundDir);	// path for animated PNG files and LST files
	sprintf(MPGPath, "%s\\mpg", ManpINIFoundDir);		// path for MPG files
	sprintf(GIFPath, "%s\\gif", ManpINIFoundDir);		// path for GIF files
	sprintf(PNGPath, "%s\\png", ManpINIFoundDir);		// path for PNG files
	sprintf(COLPath, "%s\\col", ManpINIFoundDir);		// path for COL files
	sprintf(MAPPath, "%s\\map", ManpINIFoundDir);		// path for MAP files
	sprintf(SCIPath, "%s\\sci", ManpINIFoundDir);		// path for SCI files
	sprintf(PARPath, "%s\\par", ManpINIFoundDir);		// path for PAR files
	sprintf(KFRPath, "%s\\kfr", ManpINIFoundDir);		// path for KFR files
	sprintf(IFSPath, "%s\\ifs", ManpINIFoundDir);		// path for IFS files
	sprintf(LSYSPath, "%s\\lsys", ManpINIFoundDir);		// path for LSYS files
	sprintf(FRMPath, "%s\\frm", ManpINIFoundDir);		// path for formula files
	sprintf(ScriptFileName, "%s\\sci\\Manp", ManpINIFoundDir);
	sprintf(PNGName, "%s\\animpng\\Manp", ManpINIFoundDir);
	}
    else if (*ManpPath)						// use the locations specified in dialogue box
	{
	sprintf(ANIMPNGPath, "%sanimpng", ManpPath);		// path for animated PNG files and LST files
	sprintf(MPGPath, "%smpg", ManpPath);			// path for MPG files
	sprintf(GIFPath, "%sgif", ManpPath);			// path for GIF files
	sprintf(PNGPath, "%spng", ManpPath);			// path for PNG files
	sprintf(COLPath, "%scol", ManpPath);			// path for COL files
	sprintf(MAPPath, "%smap", ManpPath);			// path for MAP files
	sprintf(SCIPath, "%ssci", ManpPath);			// path for SCI files
	sprintf(PARPath, "%spar", ManpPath);			// path for PAR files
	sprintf(PARPath, "%skfr", ManpPath);			// path for KFR files
	sprintf(IFSPath, "%sifs", ManpPath);			// path for IFS files
	sprintf(LSYSPath, "%slsys", ManpPath);			// path for LSYS files
	sprintf(FRMPath, "%sfrm", ManpPath);			// path for formula files
	status = DEFAULT;
	}
    else
	{
	wsprintf(buf, "MANPWIN.INI could not be found in the path: %s\nIf this is not a new installation, then there is a problem with MANPWIN.INI", ConfigFile);
	MessageBox (hwnd, buf, "ReadConfig", MB_ICONEXCLAMATION | MB_OK);
	}

    if (status == FILE_ERROR)
	return -1;
    return 0;
    }

/**************************************************************************
	See if string is in the beginning of buffer and return the offset
**************************************************************************/

int	Strlicmp(const char *buf, const char *string)

    {
    int length;

    length = (int)strlen(string);
    if (_strnicmp(buf, string, length) == 0)
	return length;
    return 0;
    }

/**************************************************************************
	Read and interpret "MANPWIN.INI"
**************************************************************************/

int	ReadConfigFile(HWND hwnd, char *filename)

    {
    char	*t;
    char	buf[MAXLINE];
    FILE	*fptr;
    short	status;
    int		length;

    status = DEFAULT;				// if all else fails, open in the normal way 
    if ((fptr = fopen(filename, "r")) == NULL)
	return FILE_ERROR;				// File not found, open ManpWin in the shareware way

    while (fgets(buf, MAXLINE, fptr))
	{
	if (*buf != ';')						// comment
	    {

#ifdef DEBUG
	    MessageBox (hwnd, buf, "ReadConfig", MB_ICONEXCLAMATION | MB_OK);
#endif

	    if (length = Strlicmp(buf, "PNGFile="))		// Check PNG files
		{
		strcpy(PNGFile, buf + length);
		trailing(PNGFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "COLFile="))		// Check for COL files
		{
		strcpy(COLFile, buf + length);
		trailing(COLFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "MAPFile="))		// Check for MAP files
		{
		strcpy(MAPFile, buf + length);
		trailing(MAPFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "SCIFile="))		// Check for SCI files
		{
		strcpy(SCIFile, buf + length);
		trailing(SCIFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "PNGPath="))		// Check for path to PNG files
		{
		strcpy(PNGPath, buf + length);
		trailing(PNGPath);				// remove trailing spaces or newlines
		t = PNGPath + strlen(PNGPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "GIFPath="))		// Check for path to GIF files
		{
		strcpy(GIFPath, buf + length);
		trailing(GIFPath);				// remove trailing spaces or newlines
		t = GIFPath + strlen(GIFPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "MPGPath="))		// Check for path to MPG files
		{
		strcpy(MPGPath, buf + length);
		trailing(MPGPath);				// remove trailing spaces or newlines
		t = MPGPath + strlen(MPGPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "ANIMPNGPath="))		// Check for path to animation PNG files
		{
		strcpy(ANIMPNGPath, buf + length);
		trailing(ANIMPNGPath);				// remove trailing spaces or newlines
		t = ANIMPNGPath + strlen(ANIMPNGPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "COLPath="))		// Check for path to COL files
		{
		strcpy(COLPath, buf + length);
		trailing(COLPath);				// remove trailing spaces or newlines
		t = COLPath + strlen(COLPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "MAPPath="))		// Check for path to MAP files
		{
		strcpy(MAPPath, buf + length);
		trailing(MAPPath);				// remove trailing spaces or newlines
		t = MAPPath + strlen(MAPPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "SCIPath="))		// Check for path to SCI files
		{
		strcpy(SCIPath, buf + length);
		trailing(SCIPath);				// remove trailing spaces or newlines
		t = SCIPath + strlen(SCIPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "PARPath="))		// Check for path to PAR files
		{
		strcpy(PARPath, buf + length);
		trailing(PARPath);				// remove trailing spaces or newlines
		t = PARPath + strlen(PARPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "KFRPath="))		// Check for path to KFR files
		{
		strcpy(KFRPath, buf + length);
		trailing(KFRPath);				// remove trailing spaces or newlines
		t = KFRPath + strlen(KFRPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "IFSPath="))		// Check for path to IFS files
		{
		strcpy(IFSPath, buf + length);
		trailing(IFSPath);				// remove trailing spaces or newlines
		t = IFSPath + strlen(IFSPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "LSYSPath="))		// Check for path to LSYS files
		{
		strcpy(LSYSPath, buf + length);
		trailing(LSYSPath);				// remove trailing spaces or newlines
		t = LSYSPath + strlen(LSYSPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}
	    if (length = Strlicmp(buf, "FRMPath="))		// Check for path to formula files
		{
		strcpy(FRMPath, buf + length);
		trailing(FRMPath);				// remove trailing spaces or newlines
		t = FRMPath + strlen(FRMPath) - 1;
		if (*t == '\\')					// remove any backslash to the path if required
		    *t = '\0';
		continue;
		}

	    if (length = Strlicmp(buf, "FracPARFile="))		// Check for Fractint PAR files
		{
		strcpy(FracPARFile, buf + length);
		trailing(FracPARFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "PARFile="))		// Check for PAR files
		{
		strcpy(PARFile, buf + length);
		trailing(PARFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "KFRFile="))		// Check for KFR files
		{
		strcpy(KFRFile, buf + length);
		trailing(KFRFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "IFSFile="))		// Check for IFS files
		{
		strcpy(IFSFile, buf + length);
		trailing(IFSFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "LSYSFile="))		// Check for LSYS files
		{
		strcpy(LSYSFile, buf + length);
		trailing(LSYSFile);				// remove trailing spaces or newlines
		continue;
		}
	    if (length = Strlicmp(buf, "FRMFile="))		// Check for formula files
		{
		strcpy(FRMFile, buf + length);
		trailing(FRMFile);				// remove trailing spaces or newlines
		continue;
		}

	    if (length = Strlicmp(buf, "AllowMPEGStats="))	// allow MPEG generator to dump stats into a file
		{
		statname[0] = (*(buf + length) == 'T') ? '\0' : '-';
		continue;
		}

	    if (length = Strlicmp(buf, "ZoomEdge="))		// Do we want to zoom along the edge?
		{
		ZoomEdge = (*(buf + length) == 'F') ? FALSE : TRUE;
		continue;
		}

	    if (length = Strlicmp(buf, "UseFractintPalette="))	// Do we want to zoom along the edge?
		{
		UseFractintPalette = (*(buf + length) == 'F') ? FALSE : TRUE;
		continue;
		}

	    if (length = Strlicmp(buf, "UserPath="))		// Do we want to zoom along the edge?
		{
		UserPath = (*(buf + length) == 'F') ? FALSE : TRUE;
		continue;
		}
	    
	    if (length = Strlicmp(buf, "ThreadNumber="))	// Number of threads in multi-threaded perturbation
		{
		NumberThreads = atoi(buf + length);
		continue;
		}

	    if (length = Strlicmp(buf, "DisplayPalette="))	// Do we want to zoom along the edge?
		{
		TrueCol.DisplayPaletteFlag = (*(buf + length) == 'F') ? FALSE : TRUE;
		continue;
		}
	    }
	}

    fclose(fptr);
    return status;
    }

/**************************************************************************
	Get Pathname for MANPWIN.INI Try both path and WINDOWS directory
**************************************************************************/

/*
int	GetManpINIPath(HWND hwnd, char *buf, char *ManpName)

    {
    BYTE	flag, status;
    char	CurrentDir[_MAX_PATH];

    flag = (_getcwd(CurrentDir, _MAX_PATH) == NULL) ? FALSE : TRUE;

#ifdef DEBUG
    MessageBox (hwnd, WorkingDir, "Working Dir in", MB_ICONEXCLAMATION | MB_OK);
#endif

    sprintf(buf, "%sManpWin.INI", WorkingDir);

#ifdef DEBUG
    MessageBox (hwnd, buf, "PATH Dir", MB_ICONEXCLAMATION | MB_OK);
#endif

    status = ReadConfigFile(hwnd, buf);

#ifdef DEBUG
    MessageBox (hwnd, WorkingDir, "Working Dir out", MB_ICONEXCLAMATION | MB_OK);
#endif

    if (status == FILE_ERROR)			// Search for file in PATH environment variable:
	status = ReadConfigFile(hwnd, "c:\\windows\\MANPWIN.INI");	// backward compatability
    return status;
    }
*/

/**************************************************************************
	Get Pathname of a file
**************************************************************************/

int	GetFilePath(HWND hwnd, char *buffer, char *filename)

    {
    HMODULE	hm;
    char	*p;

    hm = GetModuleHandle(filename);
    GetModuleFileName(hm, buffer, _MAX_PATH - 1);
    p = buffer + strlen(buffer);
    while (*p != '\\' && p > buffer) 
	*p-- = '\0';

#ifdef DEBUG
    MessageBox (hwnd, buffer, filename, MB_ICONEXCLAMATION | MB_OK);
#endif

    return 0;
    }

  
/***************************************************************************
	Write actual ManpWIN.INI Data
***************************************************************************/

void	WriteManpINI(HWND hwnd, FILE *fp)

    {
    fprintf(fp, ";========================================================================\n");
    fprintf(fp, "; Configuration File for Paul's Fractals: © Paul de Leeuw 25/02/2020\n");
    fprintf(fp, ";========================================================================\n");
    fprintf(fp, ";\n");
    fprintf(fp, "; Note the config lines must start at character 0 and are case sensitive\n");
    fprintf(fp, ";\n");
    fprintf(fp, "; Comments are started with a ; character\n");
    fprintf(fp, ";\n");
    fprintf(fp, ";\n");
    fprintf(fp, "; Path for PNG files\n");
    fprintf(fp, "PNGPath=%s\n", PNGPath);
    fprintf(fp, "; Path for GIF files\n");
    fprintf(fp, "GIFPath=%s\n", GIFPath);
    fprintf(fp, "; Path for MPG files\n");
    fprintf(fp, "MPGPath=%s\n", MPGPath);
    fprintf(fp, "; Path for PNG files from animation generator\n");
    fprintf(fp, "ANIMPNGPath=%s\n", ANIMPNGPath);
    fprintf(fp, "; Path for COL files\n");
    fprintf(fp, "COLPath=%s\n", COLPath);
    fprintf(fp, "; Path for MAP files\n");
    fprintf(fp, "MAPPath=%s\n", MAPPath);
    fprintf(fp, "; Path for SCI files\n");
    fprintf(fp, "SCIPath=%s\n", SCIPath);
    fprintf(fp, "; Path for PAR files\n");
    fprintf(fp, "PARPath=%s\n", PARPath);
    fprintf(fp, "; Path for KFR files\n");
    fprintf(fp, "KFRPath=%s\n", KFRPath);
    fprintf(fp, "; Path for IFS files\n");
    fprintf(fp, "IFSPath=%s\n", IFSPath);
    fprintf(fp, "; Path for Lsys files\n");
    fprintf(fp, "LSYSPath=%s\n", LSYSPath);
    fprintf(fp, "; Path for Formula files\n");
    fprintf(fp, "FRMPath=%s\n", FRMPath);
    fprintf(fp, ";\n");

    fprintf(fp, "; PNG file\n");
    fprintf(fp, "PNGFile=%s\n", PNGFile);
    fprintf(fp, "; COL file\n");
    fprintf(fp, "COLFile=%s\n", COLFile);
    fprintf(fp, "; MAP file\n");
    fprintf(fp, "MAPFile=%s\n", MAPFile);
    fprintf(fp, "; SCI file\n");
    fprintf(fp, "SCIFile=%s\n", SCIFile);

    fprintf(fp, "; Fractint PAR file\n");
    fprintf(fp, "FracPARFile=%s\n", FracPARFile);
    fprintf(fp, "; PAR file\n");
    fprintf(fp, "PARFile=%s\n", PARFile);
    fprintf(fp, "; KFR file\n");
    fprintf(fp, "KFRFile=%s\n", KFRFile);
    fprintf(fp, "; IFS file\n");
    fprintf(fp, "IFSFile=%s\n", IFSFile);
    fprintf(fp, "; Lsys file\n");
    fprintf(fp, "LSYSFile=%s\n", LSYSFile);
    fprintf(fp, "; Formula file\n");
    fprintf(fp, "FRMFile=%s\n", FRMFile);
    fprintf(fp, ";\n");
    fprintf(fp, "; Do we want to zoom along the edge?\n");
    fprintf(fp, ";\n");
    fprintf(fp, "ZoomEdge=%s\n", (ZoomEdge) ? "TRUE" : "FALSE");
    fprintf(fp, ";\n");
    fprintf(fp, "; Do we want to use Fractint Palette?\n");
    fprintf(fp, ";\n");
    fprintf(fp, "UseFractintPalette=%s\n", (UseFractintPalette) ? "TRUE" : "FALSE");
    fprintf(fp, ";\n");
    fprintf(fp, "; Allow MPEG generator to dump stats into a stats.txt file\n");
    fprintf(fp, ";\n");
    fprintf(fp, "AllowMPEGStats=%s\n", (statname[0] != '-') ? "TRUE" : "FALSE");
    fprintf(fp, ";\n");
    fprintf(fp, "; Use the default directory structure in user data area\n");
    fprintf(fp, ";\n");
    fprintf(fp, "UserPath=%s\n", (UserPath) ? "TRUE" : "FALSE");
    fprintf(fp, ";\n");
    fprintf(fp, "; Number of threads used in multi-threaded perturbation only. \n");
    fprintf(fp, "; If Number of threads = 0, then non-threaded perturbation is used\n");
    fprintf(fp, ";\n");
    fprintf(fp, "ThreadNumber=%d\n", NumberThreads);
    fprintf(fp, ";\n");
    fprintf(fp, "; Do we display a palette on the right side of the image?\n");
    fprintf(fp, ";\n");
    fprintf(fp, "DisplayPalette=%s\n", (TrueCol.DisplayPaletteFlag) ? "TRUE" : "FALSE");
    fprintf(fp, ";\n");
    }

int	SaveConfig(HWND hwnd)

    {
    FILE	*fp;
    char	s[240];
    char	lpszNewName[MAX_PATH];
/*
    char	*p;
    BOOL	bRC = TRUE;

    if (*ManpINIFoundDir)						// we have read a valid ManpWIN.INI
	{
	strcpy(lpszNewName, ManpINIFoundDir);
	p = lpszNewName;
	while (*(++p))
	    if (*p == '.')						// remove extension
		{
		*p = '\0';
		break;
		}
	strcat(lpszNewName, ".bak");
	bRC = CopyFile(ManpINIFoundDir, lpszNewName, FALSE);	// keep a backup copy
	if (!bRC && GetLastError() != 80)
	    {
	    sprintf(s, "File: %s not found", ManpINIFoundDir);
	    MessageBox (hwnd, s, "Save MANPWIN.INI", MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    }
	}
    else
*/
	sprintf(lpszNewName, "%s\\ManpWIN.INI", ManpINIFoundDir);	// may as well create one

    if ((fp = fopen(lpszNewName, "w")) == NULL)
	{
	wsprintf(s, "Can't open file: %s", lpszNewName);
	MessageBox (hwnd, s, "Save ManpWIN.INI", MB_ICONEXCLAMATION | MB_OK);
	MessageBeep (0);
	}
    else
	{
	WriteManpINI(hwnd, fp);
	fclose(fp);
	}
    return 0;
    }

/**************************************************************************
	Set Up Folder 
**************************************************************************/

void	SetManpWINDir(HWND hwnd, char *path)

    {
    _mkdir(path); 
    }

/**************************************************************************
	Dialog Set Up Folder Structure for ManpWin
**************************************************************************/

DLGPROC FAR PASCAL CreateDirDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
     HWND	hCtrl ;

     switch (message)
	  {
	  case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_MANP, ManpPath);
		SetDlgItemText(hDlg, IDC_USER_DATA, ManpINIFoundDir);
		SetDlgItemText(hDlg, IDC_ANIMPNG, *(ANIMPNGPath) ? ANIMPNGPath : "animpng");
		SetDlgItemText(hDlg, IDC_PNG, *(PNGPath) ? PNGPath : "png");
		SetDlgItemText(hDlg, IDC_COL, *(COLPath) ? COLPath : "col");
		SetDlgItemText(hDlg, IDC_IFSD, *(IFSPath) ? IFSPath : "ifs");
		SetDlgItemText(hDlg, IDC_LSYS, *(LSYSPath) ? LSYSPath : "lsys");
		SetDlgItemText(hDlg, IDC_FRM, *(FRMPath) ? FRMPath : "frm");
		SetDlgItemText(hDlg, IDC_MAP, *(MAPPath) ? MAPPath : "map");
		SetDlgItemText(hDlg, IDC_PAR, *(PARPath) ? PARPath : "par");
		SetDlgItemText(hDlg, IDC_KFR, *(KFRPath) ? KFRPath : "kfr");
		SetDlgItemText(hDlg, IDC_SCI, *(SCIPath) ? SCIPath : "sci");
		SetDlgItemText(hDlg, IDC_GIF, *(GIFPath) ? GIFPath : "gif");
		SetDlgItemText(hDlg, IDC_MPG, *(MPGPath) ? MPGPath : "mpg");
		hCtrl = GetDlgItem (hDlg, IDC_USERPATH);
		SendMessage(hCtrl, BM_SETCHECK, UserPath, 0L);
	        return (DLGPROC)FALSE ;

	  case WM_COMMAND:
	        switch ((int) LOWORD(wParam))
//	        switch (wParam)
		    {
		    case IDOK:
//			_chdir(ManpPath);
			hCtrl = GetDlgItem (hDlg, IDC_USERPATH) ;
			UserPath = (BOOL)SendMessage(hCtrl, BM_GETCHECK, 0, 0L);
//			GetDlgItemText(hDlg, IDC_MANP, ManpPath, _MAX_PATH + 1);
			GetDlgItemText(hDlg, IDC_ANIMPNG, ANIMPNGPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, ANIMPNGPath);
			GetDlgItemText(hDlg, IDC_PNG, PNGPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, PNGPath);
			GetDlgItemText(hDlg, IDC_COL, COLPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, COLPath);
			GetDlgItemText(hDlg, IDC_IFSD, IFSPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, IFSPath);
			GetDlgItemText(hDlg, IDC_LSYS, LSYSPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, LSYSPath);
			GetDlgItemText(hDlg, IDC_FRM, FRMPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, FRMPath);
			GetDlgItemText(hDlg, IDC_MAP, MAPPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, MAPPath);
			GetDlgItemText(hDlg, IDC_PAR, PARPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, PARPath);
			GetDlgItemText(hDlg, IDC_KFR, KFRPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, PARPath);
			GetDlgItemText(hDlg, IDC_SCI, SCIPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, SCIPath );
			GetDlgItemText(hDlg, IDC_GIF, GIFPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, GIFPath );
			GetDlgItemText(hDlg, IDC_MPG, MPGPath, _MAX_PATH + 1);
			SetManpWINDir(hDlg, MPGPath );
			if (UserPath)
			    {
			    sprintf(ScriptFileName, "%s\\SCI\\Manp", ManpINIFoundDir);
			    sprintf(PNGName, "%s\\animpng\\Manp", ManpINIFoundDir);
			    }
			else
			    {
			    sprintf(ScriptFileName, "%s\\SCI\\Manp", ManpPath);
			    sprintf(PNGName, "%s\\animpng\\Manp", ManpPath);
			    }

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

/**************************************************************************
	Generate time string for use in filenames
**************************************************************************/

char	*GenerateTimeString(void)

    {
    __time64_t		long_time;
    static     char	TimeStr[128];
    struct tm		*timeptr;

    _time64(&long_time);             // Get time as 64-bit integer.
    timeptr = _localtime64(&long_time);
    strftime(TimeStr, 128, "-%y%m%d-%H%M%S", timeptr);
    return TimeStr;
    }

/**************************************************************************
	Set Up Filename
**************************************************************************/

void	SetUpFilename(char *Filename, char *Folder, char *AnimType)

    {
    __time64_t long_time;
    char	TimeStr[128];
    struct tm	*timeptr;

    _time64(&long_time);             // Get time as 64-bit integer.
    timeptr = _localtime64(&long_time);
    strftime(TimeStr, 128, "-%y%m%d-%H%M%S", timeptr);
    if (*ManpINIFoundDir && UserPath)				// user prefers default locations
	sprintf(Filename, "%s\\%s\\%s%s", ManpINIFoundDir, Folder, AnimType, GenerateTimeString());
    else if (*ManpPath)						// use the locations specified in dialogue box
	sprintf(Filename, "%s\\%s\\%s", ManpPath, Folder, GenerateTimeString());
    else
	strcpy(Filename, TimeStr);
    }



