/*------------------------------------------
   SAVEFILE.CPP --   File Write Functions
  ------------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <shlwapi.h>
#include "manpwin.h"
#include "manp.h"
#include "ManpFile.h"

int     save_file_type;

extern	char	MPGPath[];		// path for MPEG files
extern	char	GIFPath[];		// path for GIF files
extern	char	PNGPath[];		// path for PNG files
extern	char	COLPath[];		// path for COL files
extern	char	MAPPath[];		// path for MAP files
//extern	char	SCIPath[];		// path for SCI files
extern	char	PARPath[];		// path for PAR files
extern	char	KFRPath[];		// path for KFR files
extern	char	SVGPath[];		// path for SVG files

//extern	void	close_file(HWND, char *, char *);
extern	void	output_batch(double, double, double, HWND, LPSTR);
extern	int	WriteKallesFile(HWND hwnd, char *filename);
//extern	int	save_screen(HWND, char *, short, short, short, short, short);
extern	int	write_png_file(HWND, char *, char *, char *); 
extern	int	write_gif_file(char *, char *);
extern	int	MPEGWrite(char *);
extern	char	*FractData(void);
extern	char	*GenerateTimeString(void);
extern	int	write_svg_file(HWND, char *, char *, char *);

static char *szParFilter = "Parameter Files (*.PAR)\0*.par\0";
static char *szKfrFilter = "Kalles Parameter Files (*.KFR)\0*.kfr\0";
static char *szPngFilter = "PiNG Files (*.PNG)\0*.png\0";
static char *szGIFFilter = "GIF Files (*.GIF)\0*.gif\0";
static char *szMPGFilter = "MPEG Files (*.MPG)\0*.mpg\0";
static char *szColFilter = "True Colour Files (*.COL)\0*.col\0";
static char *szMAPFilter = "Palette Files (*.MAP)\0*.map\0";
static char *szSvgFilter = "SVG Files (*.SVG)\0*.svg\0";

static FileDialogInfo MAPSaveDialog;
static FileDialogInfo COLSaveDialog;
static FileDialogInfo PARSaveDialog;
static FileDialogInfo KFRSaveDialog;
static FileDialogInfo PNGSaveDialog;
static FileDialogInfo GIFSaveDialog;
static FileDialogInfo MPGSaveDialog;
static FileDialogInfo SVGSaveDialog;

void	SaveFile(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName);

/***************************************************************************
	Initialise File Open Dialogue Structure
***************************************************************************/

static void InitOneSaveDialog(HWND hwnd, HINSTANCE hInst, FileDialogInfo& dlg, const char* path, const char* filter, const char* defExt)
    {
    dlg.ofn = {};

    dlg.ofn.lStructSize = sizeof(OPENFILENAME);
    dlg.ofn.hwndOwner = hwnd;
    dlg.ofn.hInstance = hInst;
    dlg.ofn.lpstrFilter = filter;
    dlg.ofn.lpstrCustomFilter = NULL;
    dlg.ofn.nMaxCustFilter = 0;
    dlg.ofn.nFilterIndex = 1;
    dlg.ofn.lpstrFile = dlg.File;
    dlg.ofn.nMaxFile = MAX_PATH;
    dlg.ofn.lpstrFileTitle = NULL;
    dlg.ofn.nMaxFileTitle = 0;
    dlg.ofn.lpstrInitialDir = path;
    dlg.ofn.lpstrTitle = NULL;
    dlg.ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    // Possible future experiment:
    // OFN_NOCHANGEDIR
    // We now explicitly track dialog paths.
    dlg.ofn.nFileOffset = 0;
    dlg.ofn.nFileExtension = 0;
    dlg.ofn.lpstrDefExt = defExt;
    dlg.ofn.lCustData = 0L;
    dlg.ofn.lpfnHook = NULL;
    dlg.ofn.lpTemplateName = NULL;

    strcpy(dlg.Path, path);
    dlg.Filter = filter;
    }

void SaveFileInitialize (HWND hwnd, HINSTANCE hInst)
    {
    InitOneSaveDialog(hwnd, hInst, MAPSaveDialog, MAPPath, szMAPFilter, "map");
    InitOneSaveDialog(hwnd, hInst, COLSaveDialog, COLPath, szColFilter, "col");
    InitOneSaveDialog(hwnd, hInst, PNGSaveDialog, PNGPath, szPngFilter, "png");
    InitOneSaveDialog(hwnd, hInst, PARSaveDialog, PARPath, szParFilter, "par");
    InitOneSaveDialog(hwnd, hInst, KFRSaveDialog, KFRPath, szKfrFilter, "kfr");
    InitOneSaveDialog(hwnd, hInst, GIFSaveDialog, GIFPath, szGIFFilter, "gif");
    InitOneSaveDialog(hwnd, hInst, MPGSaveDialog, MPGPath, szMPGFilter, "mpg");
    InitOneSaveDialog(hwnd, hInst, SVGSaveDialog, SVGPath, szSvgFilter, "svg");
    }

/***************************************************************************
	MAP File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK SaveMAPFileOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(MAPSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_MAP;
    if (GetSaveFileName(&MAPSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(MAPSaveDialog.File);
    strcat_s(MAPSaveDialog.File, MAX_PATH, ".map");
    strcpy(lpstrFileName, MAPSaveDialog.File);
    return 0;
    }

/***************************************************************************
	True Colour COL File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK SaveColFileOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(COLSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_COL;
    if (GetSaveFileName(&COLSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(COLSaveDialog.File);
    strcat_s(COLSaveDialog.File, MAX_PATH, ".col");
    strcpy(lpstrFileName, COLSaveDialog.File);
    return 0;
    }

/***************************************************************************
	PNG File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK SavePNGOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(PNGSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_PNG;
    if (GetSaveFileName(&PNGSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(PNGSaveDialog.File);
    strcat_s(PNGSaveDialog.File, MAX_PATH, ".png");
    strcpy(lpstrFileName, PNGSaveDialog.File);
    return 0;
    }

/***************************************************************************
	Animated GIF File Open Dialogue
***************************************************************************/

BOOL SaveGIFOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(GIFSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_GIF;
    if (GetSaveFileName(&GIFSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(GIFSaveDialog.File);
    strcat_s(GIFSaveDialog.File, MAX_PATH, ".gif");
    strcpy(lpstrFileName, GIFSaveDialog.File);
    return 0;
    }

/***************************************************************************
	Animated MPEG File Open Dialogue
***************************************************************************/

BOOL SaveMPGOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(MPGSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_MPG;
    if (GetSaveFileName(&MPGSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(MPGSaveDialog.File);
    strcat_s(MPGSaveDialog.File, MAX_PATH, ".mpg");
    strcpy(lpstrFileName, MPGSaveDialog.File);
    return 0;
    }

/***************************************************************************
	Parameter File (PAR) Open Dialogue
***************************************************************************/

INT_PTR CALLBACK SaveParOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(PARSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_PAR;
    if (GetSaveFileName(&PARSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(PARSaveDialog.File);
    strcat_s(PARSaveDialog.File, MAX_PATH, ".par");
    strcpy(lpstrFileName, PARSaveDialog.File);
    return 0;
    }

/***************************************************************************
	Halles Parameter (KFR) File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK SaveKfrOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(KFRSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_KFR;
    if (GetSaveFileName(&KFRSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(KFRSaveDialog.File);
    strcat_s(KFRSaveDialog.File, MAX_PATH, ".kfr");
    strcpy(lpstrFileName, KFRSaveDialog.File);
    return 0;
    }

/***************************************************************************
	Hailstone (SVG) File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK SaveSVGOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(SVGSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_SVG;
    if (GetSaveFileName(&SVGSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(SVGSaveDialog.File);
    strcat_s(SVGSaveDialog.File, MAX_PATH, ".svg");
    strcpy(lpstrFileName, SVGSaveDialog.File);
    return 0;
    }

/***************************************************************************
    Save both PAR and PNG File Open Dialogue
***************************************************************************/

INT_PTR CALLBACK SaveParImageOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    _snprintf_s(PARSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_PAR;
    if (GetSaveFileName(&PARSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(PARSaveDialog.File);
    strcat_s(PARSaveDialog.File, MAX_PATH, ".par");
    SaveFile(hwnd, PARSaveDialog.File, lpstrTitleName);

    _snprintf_s(PNGSaveDialog.File, MAX_PATH, _TRUNCATE, "Manp%s", GenerateTimeString());

    save_file_type = FILE_PNG;
    if (GetSaveFileName(&PNGSaveDialog.ofn) == 0)
	return -1;
    PathRemoveExtension(PNGSaveDialog.File);
    strcat_s(PNGSaveDialog.File, MAX_PATH, ".png");
    strcpy(lpstrFileName, PNGSaveDialog.File);
    return 0;
    }

/***************************************************************************
    Finally we can save the file
***************************************************************************/

void	SaveFile (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    { 
    char      s[360];

    switch (save_file_type)
	{
	case FILE_PAR:
	    output_batch(gManp->hor, gManp->vert, gManp->mandel_width, hwnd, lpstrFileName);
	    break;
	case FILE_KFR:
	    WriteKallesFile(hwnd, lpstrFileName);
	    break;
	case FILE_PNG:
	    if (write_png_file(hwnd, lpstrFileName, "ManpWIN", FractData()) < 0)
		{
		_snprintf_s(s, 360, _TRUNCATE, "Error: Could not write file: <%s>", lpstrFileName);
		MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);
		return;
		}
	    break;
	case FILE_GIF:
	    if (write_gif_file(lpstrFileName, "ManpWIN") < 0)
		{
		_snprintf_s(s, 360, _TRUNCATE, "Error: Could not write file: <%s>", lpstrFileName);
		MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);
		return;
		}
	    break;
	case FILE_MPG:
	    if (MPEGWrite(lpstrFileName) < 0)
		{
		_snprintf_s(s, 360, _TRUNCATE, "Error: Could not write file: <%s>", lpstrFileName);
		MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);
		return;
		}
	    break;
	case FILE_SVG:
	    if (write_svg_file(hwnd, lpstrFileName, "ManpWIN", FractData()) < 0)
		{
		_snprintf_s(s, 360, _TRUNCATE, "Error: Could not write file: <%s>", lpstrFileName);
		MessageBox(hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep(0);
		return;
		}
	    break;
	default:	
	    _snprintf_s(s, 360, _TRUNCATE, "Error: File type not Supported: <%s>", lpstrFileName);
	     MessageBox (hwnd, s, "Paul's Fractal Generator", MB_ICONEXCLAMATION | MB_OK);
	}
    }
