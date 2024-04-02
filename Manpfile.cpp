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
#include "manpwin.h"
#include "colour.h"
#include "fractype.h"
#include "pixel.h"
#include "Plot.h"

extern	int     file_type;
//extern	PAINTSTRUCT 	ps;
extern	RECT 	r;
extern	CTrueCol    TrueCol;
extern	void	DisplayPalette(HWND, BOOL);

extern	char	ANIMPNGPath[];		// path for animated PNG and LST files
extern	char	PNGPath[];		// path for PNG files
extern	char	COLPath[];		// path for COL files
extern	char	MAPPath[];		// path for MAP files
extern	char	SCIPath[];		// path for SCI files
extern	char	PARPath[];		// path for PAR files
extern	char	KFRPath[];		// path for KFR files
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
//extern	char	MPGFile[];		// MPG file
//extern	char	GIFFile[];		// GIF file
extern	char	COLFile[];		// COL file
extern	char	MAPFile[];		// MAP file
extern	char	SCIFile[];		// SCI file

extern	char	WorkingDir[];
extern	int	DataFromPNGFile;	// loaded PNG file?
//extern	char	infile[];

//extern	BYTE	palette_flag;		// set palette
//extern	BYTE	DisplayPaletteFlag;	// Display palette 
extern	CTrueCol    TrueCol;		// palette info
//extern	BYTE	*PalettePtr;		// points to true colour palette
//extern	BYTE	TrueColourFlag;		// Use TrueColour palette generation
extern	long	threshold;		// maximum iterations
extern	WORD	type;			// M=mand, J=Julia 1,2,4->
extern	BOOL	IsPAR;			// are we currently running a PAR file?
extern	BOOL	IsKFR;			// are we currently running a KFR file?

extern	Complex z, q;

static	OPENFILENAME ofnPNG;
static	OPENFILENAME ofnCOL;
static	OPENFILENAME ofnMAP;
static	OPENFILENAME ofnSCI;
static	OPENFILENAME ofnPAR;
static	OPENFILENAME ofnKFR;
static	OPENFILENAME ofnIFS;
static	OPENFILENAME ofnLSYS;
static	OPENFILENAME ofnFRM;
static	OPENFILENAME ofnLST;
//int	GetManpFileType(char *);
char	*str_find_ci(char *, char *);

//char	MAPFileName[255] = "";
//char	PARFileName[255] = "";

static char OldDir[480] = "";

static char *szMAPFilter = "Palette Map Files (*.MAP)\0*.MAP\0";
static char *szColFilter = "True Colour Files (*.COL)\0*.COL\0";
//static char *szFilter = "Parameter Files (*.PAR)\0*.par\0ZSoft Files (*.PCX)\0*.pcx\0";
static char *szPARFilter = "Parameter Files (*.PAR)\0*.par\0";
static char *szPNGFilter = "PiNG Files (*.PNG)\0*.PNG\0";
static char *szLSTFilter = "PNG List Files (*.LST)\0*.LST\0";
static char *szSCIFilter = "Script Files (*.SCI)\0*.SCI\0";
static char *szLSysFilter = "LSys Files (*.L)\0*.L\0";
static char *szFormulaFilter = "Formula Files (*.FRM)\0*.FRM\0";
static char *szFractParFilter = "Fractint PAR Files (*.PAR)\0*.par\0";
static char *szIFSFilter = "IFS Files (*.IFS)\0*.IFS\0";
//static char *szKFRFilter = "Kalles KFR Files (*.KFR)\0*.KFR\0";
static char *szKFRFilter = "Kalles Files (*.KFR,*.KFP)\0*.KFR;*.KFP\0";
static	int	result;

extern	CPlot	Plot;		// image plotting routines 

/***************************************************************************
	Initialise File Open Dialogue Structure
***************************************************************************/

void ViewFileInit (HWND hwnd, OPENFILENAME *ofn, char *path, char *filter)

    {
    ofn->lStructSize		= sizeof (OPENFILENAME) ;
    ofn->hwndOwner	   	= hwnd ;
    ofn->hInstance	   	= NULL ;
    ofn->lpstrFilter		= filter;
    ofn->lpstrCustomFilter 	= NULL ;
    ofn->nMaxCustFilter    	= 0 ;
    ofn->nFilterIndex		= 0 ;
    ofn->lpstrFile	   	= NULL ;	     // Set in Open and Close functions
    ofn->nMaxFile	   	= _MAX_PATH ;
    ofn->lpstrFileTitle    	= NULL ;	     // Set in Open and Close functions
    ofn->nMaxFileTitle		= _MAX_FNAME + _MAX_EXT ;
    ofn->lpstrInitialDir   	= path ;
    ofn->lpstrTitle	   	= NULL ;
    ofn->Flags			= 0L;
//    ofn->Flags		= OFN_CREATEPROMPT; // removed for Windows 10 as it wants to create a new file all the time
    ofn->nFileOffset		= 0 ;
    ofn->nFileExtension    	= 0 ;
    ofn->lpstrDefExt		= "*" ;
    ofn->lCustData	   	= 0L ;
    ofn->lpfnHook	   	= NULL ;
    ofn->lpTemplateName    	= NULL ;
    //strcpy(MAPFileName, "Default Palette MAP");
    }

void ViewFileInitialize (HWND hwnd)

    {
    ViewFileInit(hwnd, &ofnLST, ANIMPNGPath, szLSTFilter);		// note that the LST file is in the animPNG folder
    ViewFileInit(hwnd, &ofnPNG, PNGPath, szPNGFilter);
    ViewFileInit(hwnd, &ofnCOL, COLPath, szColFilter);
    ViewFileInit(hwnd, &ofnMAP, MAPPath, szMAPFilter);
    ViewFileInit(hwnd, &ofnSCI, SCIPath, szSCIFilter);
    ViewFileInit(hwnd, &ofnPAR, PARPath, szPARFilter);
    ViewFileInit(hwnd, &ofnKFR, KFRPath, szKFRFilter);
    ViewFileInit(hwnd, &ofnIFS, IFSPath, szIFSFilter);
    ViewFileInit(hwnd, &ofnLSYS, LSYSPath, szLSysFilter);
    ViewFileInit(hwnd, &ofnFRM, FRMPath, szFormulaFilter);
    }

/***************************************************************************
	Check directory
***************************************************************************/

BOOL CheckDir (HWND hwnd, OPENFILENAME *ofn)
    
    {
    if (strcmp(OldDir, ofn->lpstrInitialDir) == 0)
	return (TRUE);
    else
	{
	strcpy(OldDir, ofn->lpstrInitialDir);
	_chdir(ofn->lpstrInitialDir);
	return (FALSE);
	}
    }

/***************************************************************************
	List File Open Dialogue
***************************************************************************/

DLGPROC LSTFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     char	*pstr;

     if (!CheckDir (hwnd, &ofnLST))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, LSTFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
//	 strcpy(lpstrFileName, infile);         
     ofnLST.lpstrFile	   	= lpstrFileName ;
     ofnLST.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnLST) == 0)
         return (DLGPROC)-1;
     file_type = FILE_LST;
	// Extract the filename from the full pathname
     pstr = lpstrFileName + lstrlen(lpstrFileName) - 1;
     while ((*pstr != '\\') && (*pstr != ':') && (pstr >= lpstrFileName))
	 pstr--;
     pstr++;
     strcpy(LSTFile, pstr);
     return 0;
     }

/***************************************************************************
	Palette Map File Open Dialogue
***************************************************************************/

DLGPROC MAPFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     char	*pstr;

     if (!CheckDir (hwnd, &ofnMAP))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, MAPFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
//	 strcpy(lpstrFileName, infile);         
     ofnMAP.lpstrFile	   	= lpstrFileName ;
     ofnMAP.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnMAP) == 0)
         return (DLGPROC)-1;
     file_type = FILE_MAP;
	// Extract the filename from the full pathname
     pstr = lpstrFileName + lstrlen(lpstrFileName) - 1;
     while ((*pstr != '\\') && (*pstr != ':') && (pstr >= lpstrFileName))
	 pstr--;
     pstr++;
     strcpy(MAPFile, pstr);
     return (DLGPROC)0;
     }

/***************************************************************************
	True Colour File Open Dialogue
***************************************************************************/

DLGPROC ColFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     char	*pstr;

     if (!CheckDir (hwnd, &ofnCOL))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, COLFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
//	 strcpy(lpstrFileName, infile);         
     ofnCOL.lpstrFile	   	= lpstrFileName ;
     ofnCOL.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnCOL) == 0)
         return (DLGPROC)-1;
     file_type = FILE_COL;
	// Extract the filename from the full pathname
     pstr = lpstrFileName + lstrlen(lpstrFileName) - 1;
     while ((*pstr != '\\') && (*pstr != ':') && (pstr >= lpstrFileName))
	 pstr--;
     pstr++;
     strcpy(COLFile, pstr);
     return 0;
     }

/***************************************************************************
	Parameter File Open Dialogue
***************************************************************************/

DLGPROC PARFileOpenDlg (HWND hwnd, char *lpstrFileName, char *lpstrTitleName)
     {
     char	*pstr;

     if (!CheckDir (hwnd, &ofnPAR))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, PARFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
     strcpy(lpstrFileName, PARFile);         
     ofnPAR.lpstrFile	   	= lpstrFileName ;
     ofnPAR.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnPAR) == 0)
         return (DLGPROC)-1;

#ifdef DEBUG
char	s[200];
wsprintf(s, "File Mask = %s, Filetype = %d", lpstrFileName, file_type);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
//file_type = FILE_BMP;
#endif

     file_type = FILE_PAR;
	// Extract the filename from the full pathname
     pstr = lpstrFileName + lstrlen(lpstrFileName) - 1;
     while ((*pstr != '\\') && (*pstr != ':') && (pstr >= lpstrFileName))
	 pstr--;
     pstr++;
     strcpy(PARFile, pstr);
     IsPAR = TRUE;
     IsKFR = FALSE;
     return (DLGPROC)0;
     }

/***************************************************************************
	Kalles Parameter File Open Dialogue
***************************************************************************/

DLGPROC KFRFileOpenDlg(HWND hwnd, char *lpstrFileName, char *lpstrTitleName)
    {
    char	*pstr;

    if (!CheckDir(hwnd, &ofnKFR))			// only load previous file if same directory
	{
	strcpy(lpstrFileName, KFRFile);
	*lpstrTitleName = '\0';
	}
    //     else
    strcpy(lpstrFileName, KFRFile);
    ofnKFR.lpstrFile = lpstrFileName;
    ofnKFR.lpstrFileTitle = lpstrTitleName;
    if (GetOpenFileName(&ofnKFR) == 0)
	return (DLGPROC)-1;

#ifdef DEBUG
    char	s[200];
    wsprintf(s, "File Mask = %s, Filetype = %d", lpstrFileName, file_type);
    MessageBox(hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
    //file_type = FILE_BMP;
#endif

    file_type = FILE_KFR;
    // Extract the filename from the full pathname
    pstr = lpstrFileName + lstrlen(lpstrFileName) - 1;
    while ((*pstr != '\\') && (*pstr != ':') && (pstr >= lpstrFileName))
	pstr--;
    pstr++;
    strcpy(KFRFile, pstr);
    IsKFR = TRUE;
    IsPAR = FALSE;

    return (DLGPROC)0;
    }

/***************************************************************************
	PNG Image File Open Dialogue
***************************************************************************/

DLGPROC PNGFileOpenDlg (HWND hwnd, char *lpstrFileName, char *lpstrTitleName)
     {
     char	*pstr;

     if (!CheckDir (hwnd, &ofnPNG))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, PNGFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
//	 strcpy(lpstrFileName, infile);         
     ofnPNG.lpstrFile	   	= lpstrFileName ;
     ofnPNG.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnPNG) == 0)
         return (DLGPROC)-1;

#ifdef DEBUG
wsprintf(s, "File Mask = %s, Filetype = %d", lpstrFileName, file_type);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
//file_type = FILE_BMP;
#endif

     file_type = FILE_PNG;
	// Extract the filename from the full pathname
     pstr = lpstrFileName + lstrlen(lpstrFileName) - 1;
     while ((*pstr != '\\') && (*pstr != ':') && (pstr >= lpstrFileName))
	 pstr--;
     pstr++;
     strcpy(PNGFile, pstr);
     return 0;
     }

/***************************************************************************
	Animation script File Open Dialogue
***************************************************************************/

DLGPROC SCIFileOpenDlg (HWND hwnd, char *lpstrFileName, char *lpstrTitleName)
     {
     char	*pstr;

     if (!CheckDir (hwnd, &ofnSCI))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, SCIFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
//	 strcpy(lpstrFileName, infile);         
     ofnSCI.lpstrFile	   	= lpstrFileName ;
     ofnSCI.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnSCI) == 0)
         return (DLGPROC)-1;

#ifdef DEBUG
wsprintf(s, "File Mask = %s, Filetype = %d", lpstrFileName, file_type);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
//file_type = FILE_BMP;
#endif

     file_type = FILE_SCI;
	// Extract the filename from the full pathname
     pstr = lpstrFileName + lstrlen(lpstrFileName) - 1;
     while ((*pstr != '\\') && (*pstr != ':') && (pstr >= lpstrFileName))
	 pstr--;
     pstr++;
     strcpy(SCIFile, pstr);
     return 0;
     }

/***************************************************************************
	LSystem File Open Dialogue
***************************************************************************/

DLGPROC LsysFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     if (!CheckDir (hwnd, &ofnLSYS))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, LSYSFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
//     strcpy(lpstrFileName, LSYSFile);         
     ofnLSYS.lpstrFile	   	= lpstrFileName ;
     ofnLSYS.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnLSYS) == 0)
         return (DLGPROC)-1;
     file_type = FILE_LSY;

#ifdef DEBUG
wsprintf(s, "File Mask = %s, Filetype = %d", lpstrFileName, file_type);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
//file_type = FILE_BMP;
#endif

     return 0;
     }

/***************************************************************************
	Formula File Open Dialogue
***************************************************************************/

DLGPROC FormulaFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     if (!CheckDir (hwnd, &ofnFRM))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, FRMFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
     strcpy(lpstrFileName, FRMFile);         
     ofnFRM.lpstrFile	   	= lpstrFileName ;
     ofnFRM.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnFRM) == 0)
         return (DLGPROC)-1;
     file_type = FILE_FRM;

#ifdef DEBUG
wsprintf(s, "File Mask = %s, Filetype = %d", lpstrFileName, file_type);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
//file_type = FILE_BMP;
#endif

     return 0;
     }

/***************************************************************************
	Fractint Par File Open Dialogue
***************************************************************************/

DLGPROC FractintParFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     if (!CheckDir (hwnd, &ofnPAR))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, FracPARFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
     strcpy(lpstrFileName, FracPARFile);         
     ofnPAR.lpstrFile	   	= lpstrFileName ;
     ofnPAR.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnPAR) == 0)
         return (DLGPROC)-1;
     file_type = FILE_FPR;

#ifdef DEBUG
wsprintf(s, "File Mask = %s, Filetype = %d", lpstrFileName, file_type);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
//file_type = FILE_BMP;
#endif

     return 0;
     }

/***************************************************************************
	IFS File Open Dialogue
***************************************************************************/

DLGPROC IFSFileOpenDlg (HWND hwnd, LPSTR lpstrFileName, LPSTR lpstrTitleName)
     {
     if (!CheckDir (hwnd, &ofnIFS))			// only load previous file if same directory
	 {
	 strcpy(lpstrFileName, IFSFile);         
	 *lpstrTitleName = '\0';
	 }
//     else
//	 strcpy(lpstrFileName, infile);         
     ofnIFS.lpstrFile	   	= lpstrFileName ;
     ofnIFS.lpstrFileTitle    	= lpstrTitleName ;
     if (GetOpenFileName (&ofnIFS) == 0)
         return (DLGPROC)-1;
     file_type = FILE_IFS;

#ifdef DEBUG
wsprintf(s, "File Mask = %s, Filetype = %d", lpstrFileName, file_type);
MessageBox (hwnd, s, "FRED", MB_ICONEXCLAMATION | MB_OK);
//file_type = FILE_BMP;
#endif

     return 0;
     }

/***************************************************************************
	Case insensitive version of str_find() which does not corrupt either string
	Find substring s within a null terminated buffer t
	returns pointer to character following in buffer (or NULL)
***************************************************************************/

char *str_find_ci(char *t, char *s)

    {
    register char *w, *v, *u, temp;

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
    int	test;

    i = 0;
    while (fgets(s, 150, fip) && i < MAXTHRESHOLD)
	{
	if ((test = sscanf(s, "%3d %3d %3d", (int *)(TrueCol.PalettePtr + i * 3 + 0), 
						(int *)(TrueCol.PalettePtr + i * 3 + 1),
						(int *)(TrueCol.PalettePtr + i * 3 + 2))) < 3)
	    break;
	i++;
	}
    TrueCol.ColoursInPALFile = i;
    }

short	FilePalette(HWND hwnd, char *infile, char *szAppName)

    {
    char	s[150];
    char	TempFile[MAX_PATH];
    FILE	*fip;

    if (!(fip = fopen(infile, "r")))
	{
	sprintf(TempFile, "%s\\%s", MAPPath, infile);
	if (!(fip = fopen(TempFile, "r")))
	    {
	    wsprintf(s, "Can't Open Map File: <%s> Using default", TempFile);
	    MessageBox (hwnd, s, szAppName, MB_ICONEXCLAMATION | MB_OK);
	    MessageBeep (0);
	    return 0;
	    }
	}

//    SetPalettePointer(PalettePtr);
    ReadTriplets(fip);
    if (TrueCol.Stretch)
	TrueCol.FillPalette(STRETCH, TrueCol.PalettePtr, threshold);
    else
	TrueCol.FillPalette(REPEAT, TrueCol.PalettePtr, threshold);
    fclose(fip);
    if (!DataFromPNGFile)
	{
//	if (type == APOLLONIUS || type == CIRCLES || type == OSCILLATORS || type == FRACTALMAPS || type == SPROTTMAPS || type == SURFACES || type == KNOTS || type == CURVES)
//	    Pix.run_fractal(&z, &q);				// can't refresh 3D so recreate image
//	else
	    Plot.RefreshScreen();
	}
//    if (TrueCol.DisplayPaletteFlag)
	DisplayPalette(hwnd, TrueCol.DisplayPaletteFlag);
    InvalidateRect(hwnd, &r, FALSE);
    DataFromPNGFile = FALSE;
    return 0;
    }

