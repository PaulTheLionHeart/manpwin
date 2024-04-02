/*------------------------------------------
   SAVEFILE.CPP --   File Write Functions
  ------------------------------------------*/

#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "manpwin.h"

int     save_file_type;

extern	double	hor;			/* horizontal address */
extern	double	vert;			/* vertical address */
extern	double	mandel_width;		/* width of display */

extern	char	MPGPath[];		// path for MPEG files
extern	char	GIFPath[];		// path for GIF files
extern	char	PNGPath[];		// path for PNG files
extern	char	COLPath[];		// path for COL files
extern	char	MAPPath[];		// path for MAP files
//extern	char	SCIPath[];		// path for SCI files
extern	char	PARPath[];		// path for PAR files
extern	char	KFRPath[];		// path for KFR files

//extern	char	*str_find_ci(char *, char *);
extern	void	close_file(HWND, char *, char *);
extern	void	output_batch(double, double, double, HWND, LPSTR);
extern	int	WriteKallesFile(HWND hwnd, char *filename);
extern	int	save_screen(HWND, char *, short, short, short, short, short);
//extern	short	save_colour(HWND, char *, char *);
extern	int	write_png_file(HWND, char *, char *, char *); 
//extern	int	write_bmp_file(HWND, char *, char *); 
extern	int	write_gif_file(char *, char *);
extern	int	MPEGWrite(char *);
extern	char	*FractData(void);
extern	char	*GenerateTimeString(void);

static OPENFILENAME ofn1;

static char *szParFilter = "Parameter Files (*.PAR)\0*.par\0";
static char *szKfrFilter = "Kalles Parameter Files (*.KFR)\0*.kfr\0";
static char *szPngFilter = "PiNG Files (*.PNG)\0*.png\0";
static char *szGIFFilter = "GIF Files (*.GIF)\0*.gif\0";
static char *szMPGFilter = "MPEG Files (*.MPG)\0*.mpg\0";
static char *szColFilter = "True Colour Files (*.COL)\0*.col\0";
static char *szMAPFilter = "Palette Files (*.MAP)\0*.map\0";

void	SaveFile(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName);

void SaveFileInitialize (HWND hwnd, HINSTANCE hInst)
     {
     ofn1.lStructSize = sizeof(OPENFILENAME);
     ofn1.hwndOwner = hwnd;
     ofn1.hInstance = hInst;
     ofn1.lpstrFilter = szParFilter;
//     ofn1.lpstrFilter = szBitmapExt;
     ofn1.lpstrCustomFilter = NULL;
     ofn1.nMaxCustFilter = 0;
     ofn1.nFilterIndex = 1;
//     ofn1.lpstrFile = (LPSTR)achFileName;
     ofn1.nMaxFile = 128;
     ofn1.lpstrFileTitle = NULL;
     ofn1.nMaxFileTitle = 0;
     ofn1.lpstrInitialDir = NULL;
     ofn1.lpstrTitle = NULL;
     ofn1.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
     ofn1.nFileOffset = 0;
     ofn1.nFileExtension = 0;
     ofn1.lpstrDefExt = (LPSTR)"";
     ofn1.lCustData = 0L;
     ofn1.lpfnHook = NULL;
     ofn1.lpTemplateName = NULL;
     }

DLGPROC SaveMAPFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
//     *lpstrFileName = '\0';
     sprintf(lpstrFileName, "Manp%s", GenerateTimeString());
     ofn1.lpstrInitialDir   	= MAPPath ;
     ofn1.hwndOwner	   	= hwnd ;
     ofn1.lpstrFile	   	= lpstrFileName ;
     ofn1.lpstrFileTitle    	= lpstrTitleName ;
//     ofn1.Flags		   	= OFN_CREATEPROMPT;
     ofn1.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
     ofn1.lpstrFilter = szMAPFilter;

     if (GetSaveFileName (&ofn1) == 0)
         return (DLGPROC)-1;
     save_file_type = FILE_MAP;

#ifdef DEBUG
wsprintf(s, "File Mask = %s", lpstrFileName);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
#endif

     return 0;
     }

DLGPROC SaveColFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
//     *lpstrFileName = '\0';
     sprintf(lpstrFileName, "Manp%s", GenerateTimeString());

     ofn1.lpstrInitialDir   	= COLPath ;
     ofn1.hwndOwner	   	= hwnd ;
     ofn1.lpstrFile	   	= lpstrFileName ;
     ofn1.lpstrFileTitle    	= lpstrTitleName ;
//     ofn1.Flags		   	= OFN_CREATEPROMPT;
     ofn1.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
     ofn1.lpstrFilter = szColFilter;

     if (GetSaveFileName (&ofn1) == 0)
         return (DLGPROC)-1;
     save_file_type = FILE_COL;

#ifdef DEBUG
wsprintf(s, "File Mask = %s", lpstrFileName);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
#endif

     return 0;
     }


DLGPROC SavePNGOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
//     char	s[480];
     char	*fileptr;
//     FILE	*fp;
     char   *pngptr = ".png";
     BOOL	ReturnValue;

//     *lpstrFileName = '\0';
     sprintf(lpstrFileName, "Manp%s", GenerateTimeString());

     ofn1.lpstrInitialDir   	= PNGPath ;
     ofn1.hwndOwner	   	= hwnd ;
     ofn1.lpstrFile	   	= lpstrFileName ;
     ofn1.lpstrFilter		= szPngFilter;
     ofn1.lpstrDefExt		= "png";
     ofn1.nFilterIndex		= 1;
     save_file_type		= FILE_PNG;

     if (ReturnValue = GetSaveFileName(&ofn1) == 0)
	 return (DLGPROC)-1;
     fileptr = lpstrFileName;
     while (*(++fileptr))
	if (*fileptr == '.')	// remove extension
	    {
	    *fileptr = '\0';
	    break;
	    }
	
	save_file_type = FILE_PNG;
	strcat(lpstrFileName, pngptr);

#ifdef DEBUG
wsprintf(s, "File Mask = %s", lpstrFileName);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
#endif

     return 0;
     }

BOOL SaveGIFOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     char	*fileptr;
     char	*gifptr = ".gif";

//     *lpstrFileName = '\0';
     sprintf(lpstrFileName, "Manp%s", GenerateTimeString());

     ofn1.lpstrInitialDir   	= GIFPath ;
     ofn1.hwndOwner	   	= hwnd ;
     ofn1.lpstrFile	   	= lpstrFileName ;
     ofn1.lpstrFilter		= szGIFFilter;
     ofn1.lpstrDefExt		= "gif";
     ofn1.nFilterIndex		= 1;
     save_file_type		= FILE_GIF;

     if (GetSaveFileName (&ofn1) == 0)
         return -1;
     fileptr = lpstrFileName;
     while (*(++fileptr))
	if (*fileptr == '.')	// remove extension
	    {
	    *fileptr = '\0';
	    break;
	    }
	
	save_file_type = FILE_GIF;
	strcat(lpstrFileName, gifptr);

#ifdef DEBUG
wsprintf(s, "File Mask = %s", lpstrFileName);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
#endif

     return 0;
     }

BOOL SaveMPGOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     char	*fileptr;
     char	*mpgptr = ".mpg";

//     *lpstrFileName = '\0';
     sprintf(lpstrFileName, "Manp%s", GenerateTimeString());

     ofn1.lpstrInitialDir   	= MPGPath ;
     ofn1.hwndOwner	   	= hwnd ;
     ofn1.lpstrFile	   	= lpstrFileName ;
     ofn1.lpstrFilter		= szMPGFilter;
     ofn1.lpstrDefExt		= "mpg";
     ofn1.nFilterIndex		= 1;
     save_file_type		= FILE_MPG;

     if (GetSaveFileName (&ofn1) == 0)
         return -1;
     fileptr = lpstrFileName;
     while (*(++fileptr))
	if (*fileptr == '.')	// remove extension
	    {
	    *fileptr = '\0';
	    break;
	    }
	
	save_file_type = FILE_MPG;
	strcat(lpstrFileName, mpgptr);

#ifdef DEBUG
wsprintf(s, "File Mask = %s", lpstrFileName);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
#endif

     return 0;
     }

DLGPROC SaveParOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    //     char	s[480];
    char	*fileptr;
    //     FILE	*fp;
    char   *parptr = ".par";
    //     *lpstrFileName = '\0';
    sprintf(lpstrFileName, "Manp%s", GenerateTimeString());

    ofn1.lpstrInitialDir = PARPath;
    ofn1.hwndOwner = hwnd;
    ofn1.lpstrFile = lpstrFileName;
    ofn1.lpstrFilter = szParFilter;
    ofn1.lpstrDefExt = "par";
    ofn1.nFilterIndex = 1;
    save_file_type = FILE_PAR;

    if (GetSaveFileName(&ofn1) == 0)
	return (DLGPROC)-1;
    fileptr = lpstrFileName;
    while (*(++fileptr))
	if (*fileptr == '.')	// remove extension
	    {
	    *fileptr = '\0';
	    break;
	    }

    save_file_type = FILE_PAR;
    strcat(lpstrFileName, parptr);

#ifdef DEBUG
    wsprintf(s, "File Mask = %s", lpstrFileName);
    MessageBox(hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
#endif

    return 0;
    }

DLGPROC SaveKfrOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    //     char	s[480];
    char	*fileptr;
    //     FILE	*fp;
    char   *kfrptr = ".kfr";
    //     *lpstrFileName = '\0';
    sprintf(lpstrFileName, "Manp%s", GenerateTimeString());

    ofn1.lpstrInitialDir = KFRPath;
    ofn1.hwndOwner = hwnd;
    ofn1.lpstrFile = lpstrFileName;
    ofn1.lpstrFilter = szKfrFilter;
    ofn1.lpstrDefExt = "kfr";
    ofn1.nFilterIndex = 1;
    save_file_type = FILE_KFR;

    if (GetSaveFileName(&ofn1) == 0)
	return (DLGPROC)-1;
    fileptr = lpstrFileName;
    while (*(++fileptr))
	if (*fileptr == '.')	// remove extension
	    {
	    *fileptr = '\0';
	    break;
	    }

    save_file_type = FILE_KFR;
    strcat(lpstrFileName, kfrptr);

#ifdef DEBUG
    wsprintf(s, "File Mask = %s", lpstrFileName);
    MessageBox(hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
#endif

    return 0;
    }
DLGPROC SaveParImageOpenDlg(HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
    {
    char	TimeString[480];
    char	*fileptr;
    //     FILE	*fp;
    char   *parptr = ".par";
    char   *pngptr = ".png";
    //     *lpstrFileName = '\0';
    sprintf(TimeString, "Manp%s", GenerateTimeString());
    strcpy(lpstrFileName, TimeString);

    ofn1.lpstrInitialDir = PARPath;
    ofn1.hwndOwner = hwnd;
    ofn1.lpstrFile = lpstrFileName;
    ofn1.lpstrFilter = szParFilter;
    ofn1.lpstrDefExt = "par";
    ofn1.nFilterIndex = 1;
    save_file_type = FILE_PAR;

    if (GetSaveFileName(&ofn1) == 0)
	return (DLGPROC)-1;
    fileptr = lpstrFileName;
    while (*(++fileptr))
	if (*fileptr == '.')	// remove extension
	    {
	    *fileptr = '\0';
	    break;
	    }

    save_file_type = FILE_PAR;
    strcat(lpstrFileName, parptr);

#ifdef DEBUG
    wsprintf(s, "File Mask = %s", lpstrFileName);
    MessageBox(hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
#endif
    SaveFile(hwnd, lpstrFileName, lpstrTitleName);

    strcpy(lpstrFileName, TimeString);

    ofn1.lpstrInitialDir = PNGPath;
    ofn1.hwndOwner = hwnd;
    ofn1.lpstrFile = lpstrFileName;
    ofn1.lpstrFilter = szPngFilter;
    ofn1.lpstrDefExt = "png";
    ofn1.nFilterIndex = 1;
    save_file_type = FILE_PNG;

    if (GetSaveFileName(&ofn1) == 0)
	return (DLGPROC)-1;
    fileptr = lpstrFileName;
    while (*(++fileptr))
	if (*fileptr == '.')	// remove extension
	    {
	    *fileptr = '\0';
	    break;
	    }

    save_file_type = FILE_PNG;
    strcat(lpstrFileName, pngptr);
    return 0;
    }



void	SaveFile (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)

    { 
    char      s[360];

    switch (save_file_type)
	{
	case FILE_PAR:
	    output_batch(hor, vert, mandel_width, hwnd, lpstrFileName);
	    break;
	case FILE_KFR:
	    WriteKallesFile(hwnd, lpstrFileName);
	    break;
	case FILE_PNG:
	    if (write_png_file(hwnd, lpstrFileName, "ManpWIN", FractData()) < 0)
		{
		wsprintf(s, "Error: Could not write file: <%s>", lpstrFileName);
		MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);
		return;
		}
//	    MessageBeep (0);
	    break;
	case FILE_GIF:
	    if (write_gif_file(lpstrFileName, "ManpWIN") < 0)
		{
		wsprintf(s, "Error: Could not write file: <%s>", lpstrFileName);
		MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);
		return;
		}
//	    MessageBeep (0);
	    break;
	case FILE_MPG:
	    if (MPEGWrite(lpstrFileName) < 0)
		{
		wsprintf(s, "Error: Could not write file: <%s>", lpstrFileName);
		MessageBox (hwnd, s, "ManpWIN", MB_ICONEXCLAMATION | MB_OK);
		MessageBeep (0);
		return;
		}
//	    MessageBeep (0);
	    break;
	default:	
	     wsprintf(s, "Error: File type not Supported: <%s>", lpstrFileName);
	     MessageBox (hwnd, s, "Paul's Fractal Generator", MB_ICONEXCLAMATION | MB_OK);
	}
    }
