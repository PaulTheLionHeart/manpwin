/*------------------------------------------
   MANPFILE.CPP --   File Read Functions
  ------------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>
#include <shlwapi.h>
#include "manpwin.h"
#include <dlgs.h>
#include "manp.h"
#include "colour.h"
#include "fractype.h"
#include "pixel.h"
#include "Plot.h"
#include "ManpFile.h"
#include "SafeStrings.h"

#pragma comment(lib, "Shlwapi.lib")

extern	int     file_type;
extern	void	DisplayPalette(HWND, BOOL);

extern	char	ANIMPNGPath[];		// path for animated PNG and LST files
extern	char	PNGPath[];		// path for PNG files
extern	char	COLPath[];		// path for COL files
extern	char	MAPPath[];		// path for MAP files
extern	char	SCIPath[];		// path for SCI files
extern	char	PARPath[];		// path for PAR files
extern	char	KFRPath[];		// path for KFR files
extern	char	FracPARPath[];		// path for Fractint par files
extern	char	IFSPath[];		// path for IFS files
extern	char	LSYSPath[];		// path for LSYS files
extern	char	FRMPath[];		// path for formula files

extern	char	FracPARFile[];		// Fractint PAR file
extern	char	PARFile[];		// PAR file
extern	char	KFRFile[];		// KFR file
extern	char	IFSFile[];		// IFS file
extern	char	LSYSFile[];		// LSYS file
extern	char	FRMFile[];		// Formula file
extern	char	LSTFile[];		// list file for PNG animation frames

extern	char	PNGFile[];		// PNG file
extern	char	COLFile[];		// COL file
extern	char	MAPFile[];		// MAP file
extern	char	SCIFile[];		// SCI file

extern	char	WorkingDir[];

static	FileDialogInfo LSTDialog;
static	FileDialogInfo PNGDialog;
static	FileDialogInfo MAPDialog;
static	FileDialogInfo COLDialog;
static	FileDialogInfo PARDialog;
static	FileDialogInfo FracPARDialog;
static	FileDialogInfo KFRDialog;
static	FileDialogInfo SCIDialog;
static	FileDialogInfo LSYSDialog;
static	FileDialogInfo IFSDialog;
static	FileDialogInfo FRMDialog;

char	*str_find_ci(char *, char *);

static char OldDir[480] = "";

static char *szMAPFilter = "Palette Map Files (*.MAP)\0*.MAP\0";
static char *szColFilter = "True Colour Files (*.COL)\0*.COL\0";
static char *szPARFilter = "Parameter Files (*.PAR)\0*.par\0";
static char *szPNGFilter = "PiNG Files (*.PNG)\0*.PNG\0";
static char *szLSTFilter = "PNG List Files (*.LST)\0*.LST\0";
static char *szSCIFilter = "Script Files (*.SCI)\0*.SCI\0";
static char *szLSysFilter = "LSys Files (*.L)\0*.L\0";
static char *szFormulaFilter = "Formula Files (*.FRM)\0*.FRM\0";
static char *szFractParFilter = "Fractint PAR Files (*.PAR)\0*.par\0";
static char *szIFSFilter = "IFS Files (*.IFS)\0*.IFS\0";
static char *szKFRFilter = "Kalles Files (*.KFR,*.KFP)\0*.KFR;*.KFP\0";

/**************************************************************************
OpenFileHookProc()
```
Explorer-style Open/Save dialog hook procedure.

Purpose:
Modern Windows Explorer dialogs sometimes position the filename
edit control caret at the END of long filenames, causing the
beginning of the filename to be hidden from view.

This hook forces the filename edit control to:
    - move the caret to the beginning
    - scroll the edit control fully left

so the entire filename is immediately visible when the dialog opens.

Notes:
Requires:
    OFN_ENABLEHOOK
    OFN_EXPLORER

Used by both:
    Open dialogs
    Save dialogs

This preserves modern resizable Explorer-style dialogs while
fixing annoying filename edit-field behaviour.
**************************************************************************/

UINT_PTR CALLBACK OpenFileHookProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
    {
    if (msg == WM_NOTIFY)
	{
	OFNOTIFY* pNotify = (OFNOTIFY*)lParam;

	if (pNotify->hdr.code == CDN_INITDONE)
	    {
	    HWND hParent = GetParent(hdlg);
	    HWND hEdit = GetDlgItem(hParent, edt1);

	    if (hEdit)
		{
		SendMessage(hEdit, EM_SETSEL, 0, 0);
		SendMessage(hEdit, EM_SCROLLCARET, 0, 0);
		}
	    }
	}

    return 0;
    }

/***************************************************************************
    Initialise File Open Dialogue Structure
***************************************************************************/

void ViewFileInit (HWND hwnd, OPENFILENAME *ofn, char *path, const char *filter)
    {

    ofn->lStructSize		= sizeof (OPENFILENAME) ;
    ofn->hwndOwner	   	= hwnd ;
    ofn->hInstance	   	= NULL ;
    ofn->lpstrFilter		= filter;
    ofn->lpstrCustomFilter 	= NULL ;
    ofn->nMaxCustFilter    	= 0 ;
    ofn->nFilterIndex		= 1 ;
    ofn->lpstrFile	   	= NULL ;	     // Set in Open and Close functions
    ofn->nMaxFile	   	= _MAX_PATH ;
    ofn->lpstrFileTitle    	= NULL ;	     // Set in Open and Close functions
    ofn->nMaxFileTitle		= _MAX_FNAME + _MAX_EXT ;
    ofn->lpstrInitialDir   	= path ;
    ofn->lpstrTitle	   	= NULL ;
    ofn->Flags			= OFN_EXPLORER | OFN_ENABLEHOOK | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_ENABLESIZING;
    // Possible future experiment:
    // OFN_NOCHANGEDIR
    // We now explicitly track dialog paths.
    ofn->lpfnHook		= OpenFileHookProc;
    ofn->nFileOffset		= 0 ;
    ofn->nFileExtension    	= 0 ;
    ofn->lpstrDefExt		= "*" ;
    ofn->lCustData	   	= 0L ;
    ofn->lpfnHook	   	= NULL ;
    ofn->lpTemplateName    	= NULL ;
    }

void InitFileDialog(HWND hwnd, FileDialogInfo& dlg, const char* path, const char* file, const char* filter)
    {
    strcpy(dlg.Path, path);
    strcpy(dlg.File, file);
    dlg.Filter = filter;
    ViewFileInit(hwnd, &dlg.ofn, dlg.Path, dlg.Filter);
    }

void ViewFileInitialize (HWND hwnd)
    {
    InitFileDialog(hwnd, LSTDialog, ANIMPNGPath, LSTFile, szLSTFilter);
    InitFileDialog(hwnd, PNGDialog, PNGPath, PNGFile, szPNGFilter);
    InitFileDialog(hwnd, MAPDialog, MAPPath, MAPFile, szMAPFilter);
    InitFileDialog(hwnd, COLDialog, COLPath, COLFile, szColFilter);
    InitFileDialog(hwnd, PARDialog, PARPath, PARFile, szPARFilter);
    InitFileDialog(hwnd, FracPARDialog, FracPARPath, FracPARFile, szFractParFilter);
    InitFileDialog(hwnd, KFRDialog, KFRPath, KFRFile, szKFRFilter);
    InitFileDialog(hwnd, SCIDialog, SCIPath, SCIFile, szSCIFilter);
    InitFileDialog(hwnd, LSYSDialog, LSYSPath, LSYSFile, szLSysFilter);
    InitFileDialog(hwnd, IFSDialog, IFSPath, IFSFile, szIFSFilter);
    InitFileDialog(hwnd, FRMDialog, FRMPath, FRMFile, szFormulaFilter);
    }

/***************************************************************************
	File Open Dialogue
***************************************************************************/

int OpenFileDialog(HWND hwnd, FileDialogInfo& dlg, char* titleName)
    {
    dlg.ofn.lpstrFile = dlg.File;
    dlg.ofn.lpstrFileTitle = titleName;
    dlg.ofn.lpstrInitialDir = dlg.Path;
    dlg.ofn.lpstrFilter = dlg.Filter;
    if (GetOpenFileName(&dlg.ofn) == 0)
	return -1;
    strcpy_s(dlg.File, MAX_PATH, PathFindFileName(dlg.ofn.lpstrFile));    
    return 0;
    }

/***************************************************************************
	List File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK LSTFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(LSTDialog.File, LSTFile);
    if (OpenFileDialog(hwnd, LSTDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_LST;
    strcpy_s(LSTFile, MAX_PATH, LSTDialog.File);
    return 0;
    }

/***************************************************************************
	Palette Map File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK MAPFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(MAPDialog.File, MAPFile);
    if (OpenFileDialog(hwnd, MAPDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_MAP;
    strcpy_s(MAPFile, MAX_PATH, MAPDialog.File);
    return 0;
    }

/***************************************************************************
	True Colour File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK ColFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(COLDialog.File, COLFile);
    if (OpenFileDialog(hwnd, COLDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_COL;
    strcpy_s(COLFile, MAX_PATH, COLDialog.File);
    return 0;
    }

/***************************************************************************
	Parameter File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK PARFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy_s(PARDialog.File, MAX_PATH, PARFile);
    if (OpenFileDialog(hwnd, PARDialog,	lpstrTitleName) < 0)
	return -1;

    file_type = FILE_PAR;
    strcpy_s(PARFile, MAX_PATH, PARDialog.File);
    gManp->IsPAR = TRUE;
    gManp->IsKFR = FALSE;
    return 0;
    }

/***************************************************************************
	Kalles Parameter File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK KFRFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(KFRDialog.File, KFRFile);
    if (OpenFileDialog(hwnd, KFRDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_KFR;
    strcpy_s(KFRFile, MAX_PATH, KFRDialog.File);
    gManp->IsPAR = FALSE;
    gManp->IsKFR = TRUE;

    return 0;
    }

/***************************************************************************
	PNG Image File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK PNGFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(PNGDialog.File, PNGFile);
    if (OpenFileDialog(hwnd, PNGDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_PNG;
    strcpy_s(PNGFile, MAX_PATH, PNGDialog.File);
    return 0;
    }

/***************************************************************************
	Animation script File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK SCIFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(SCIDialog.File, SCIFile);
    if (OpenFileDialog(hwnd, SCIDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_SCI;
    strcpy_s(SCIFile, MAX_PATH, SCIDialog.File);
    return 0;
    }

/***************************************************************************
	LSystem File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK LsysFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(LSYSDialog.File, LSYSFile);
    if (OpenFileDialog(hwnd, LSYSDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_LSY;
    strcpy_s(LSYSFile, MAX_PATH, LSYSDialog.File);
    return 0;
    }

/***************************************************************************
	Formula File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK FormulaFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(FRMDialog.File, FRMFile);
    if (OpenFileDialog(hwnd, FRMDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_FRM;
    strcpy_s(FRMFile, MAX_PATH, FRMDialog.File);
    return 0;
    }

/***************************************************************************
	Fractint Par File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK FractintParFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(FracPARDialog.File, FracPARFile);
    if (OpenFileDialog(hwnd, FracPARDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_PAR;
    strcpy_s(FracPARFile, MAX_PATH, FracPARDialog.File);
    gManp->IsPAR = TRUE;
    gManp->IsKFR = FALSE;

    return 0;
    }

/***************************************************************************
	IFS File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK IFSFileOpenDlg(HWND hwnd, char *lpstrTitleName)
    {
    strcpy(IFSDialog.File, IFSFile);
    if (OpenFileDialog(hwnd, IFSDialog, lpstrTitleName) < 0)
	return -1;

    file_type = FILE_IFS;
    strcpy_s(IFSFile, MAX_PATH, IFSDialog.File);
    return 0;
    }

/***************************************************************************
	Case insensitive version of str_find() which does not corrupt either string
	Find substring s within a null terminated buffer t
	returns pointer to character following in buffer (or NULL)
***************************************************************************/

char *str_find_ci(char *t, char *s)
    {
    char *w, *v, *u, temp;

    for (u = t; *u ;u++)
	{
	v = u;		// u points to start of string in buffer
	w = s;
	while (*w)
	    {
	    temp = *v;
	    if (*w != *v++)
		{
		if (islower(*w))
		    {
		    if (tolower(temp) != *w)
			break;
		    }
		else
		    break;
		}
	    w++;
	    }
	if(*w=='\0') 
	    return(v);
	}
    return(NULL);	// not found
    }

/**************************************************************************
	Get Palette Data from File
**************************************************************************/

void	ReadTriplets(FILE *fip)
    {
    long	i;
    char	s[150];
    int		test, r, g, b;

    i = 0;
    while (fgets(s, 150, fip) && i < MAXPALETTE)
	{
	if ((test = sscanf(s, "%3d %3d %3d", &b, &g, &r)) < 3)
	    break;
	else
	    {
	    gManp->TrueCol.PalettePtr[i].rgbtRed = r;
	    gManp->TrueCol.PalettePtr[i].rgbtGreen = g;
	    gManp->TrueCol.PalettePtr[i].rgbtBlue = b;
	    }
	i++;
	}
    gManp->TrueCol.ColoursInPALFile = i;
    }

short	FilePalette(HWND hwnd, char *infile, char *szAppName)
    {
    char	s[150];
    char	TempFile[MAX_PATH];
    FILE	*fip;

    if (!(fip = fopen(infile, "r")))
	{
	SAFE_SPRINTF(TempFile, "%s\\%s", MAPPath, infile);
	if (!(fip = fopen(TempFile, "r")))
	    {
	    _snprintf_s(s, 150, _TRUNCATE, "Can't Open Map File: <%s> Using default", TempFile);
	    MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    return 0;
	    }
	}

//    SetPalettePointer(PalettePtr);
    ReadTriplets(fip);
    if (gManp->TrueCol.Stretch)
	gManp->TrueCol.FillPalette(STRETCH, gManp->TrueCol.PalettePtr, gManp->threshold);
    else
	gManp->TrueCol.FillPalette(REPEAT, gManp->TrueCol.PalettePtr, gManp->threshold);
    fclose(fip);
    if (!gManp->DataFromPNGFile)
	{
	gManp->Plot.RefreshScreen();
	}
//    if (TrueCol.DisplayPaletteFlag)
	DisplayPalette(hwnd, gManp->TrueCol.DisplayPaletteFlag);
    InvalidateRect(hwnd, &gManp->r, FALSE);
    gManp->DataFromPNGFile = FALSE;
    return 0;
    }

