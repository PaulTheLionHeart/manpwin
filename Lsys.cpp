/*
    LSYS.C a module to investigate Von Koch curves etc. Based on 
    FRACTINT.EXE.
    
    Written in Visual 'C++' by Paul de Leeuw.

    This program is written in "standard" C. Hardware dependant code
    (console drivers & serial I/O) is in separate machine libraries.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "manpwin.h"
#include "resource.h"
#include "manp.h"
#include "Dib.h"
#include "Plot.h"

#define SCREENASPECT	0.75		/* Assumed screen dimensions, y/x */
#define MAXRULES	27		/* this limits rules to 25 */
#define MAXLSYS		800		// Max LSystem in file (Make sure to change this value in parser.cpp)

char	lptr[MAXLSYS][100];
char	lsys_type[MAXLSYS];		// for display
char	lsys_Label[MAXLSYS];		// for display in type selection
int	lsys_ptr = 0;
//int	PreviousLsys_ptr = 0;		// remember last time
int	lsys_num;
//int	level = 2;
DWORD	colour = 15;
DWORD	BackgroundColour = 0L;		// set background colour for IFS and L-System fractals
extern	int	xdots, ydots;

extern	char	LSYSFile[];
extern	BYTE	cycleflag;		// do colour cycling
extern	CDib	Dib;			// Device Independent Bitmaps

void	thinking(HWND, int);

static	double	aspect;			/* aspect ratio of each pixel, ysize/xsize */
static	BYTE	levels = 5;
static	double	sins[50];
static	double	coss[50];
static	int	maxangle, curcolor;
static	double	xpos, ypos, size, realangle;
static	char	counter, angle, reverse;
static	double	Xmin, Xmax, Ymin, Ymax;
static	char	loaded = 0;
static	char	*ruleptrs[MAXRULES];
static	int	endloop;			// ensure a clean exit
int	Lsystem(HWND, char *);
int	LLoad(HWND, char *);

extern	CPlot	Plot;		// image plotting routines 

extern	char	*str_find_ci(char *, char *);
extern	int	user_data(HWND);
extern	void	DisplayFractal(HWND);

void	fastline(const WORD, const WORD, const WORD, const WORD, const DWORD);
static	double	getnumber(char  **str);
static	char	*findsize(HWND, char  *, char **, char);
static	void	findscale(HWND, char  *, char **, char);
static	char	*drawLSys(HWND, char  *, char **, char);
static	int	readLSystemFile(HWND, char *, char *);
static	void	free_rules_mem(void);
static	int	save_rule(char *, char **);

/**************************************************************************
	Get numbers from file
**************************************************************************/

static	double	getnumber(char **str)

    {
    char	numstr[30];
    double	ret;
    int	i, root, inverse;

    root = 0;
    inverse = 0;
    strcpy(numstr, "");
    (*str)++;
    switch (**str)
	{
	case 'q':
	    root = 1;
	    (*str)++;
	    break;
	case 'i':
	    inverse = 1;
	    (*str)++;
	    break;
	}
    switch (**str)
	{
	case 'q':
	    root = 1;
	    (*str)++;
	    break;
	case 'i':
	    inverse = 1;
	    (*str)++;
	    break;
	}
    i = 0;
    while (**str <= '9' && **str >= '0' || **str == '.')
	{
	numstr[i++] = **str;
	(*str)++;
	}
    (*str)--;
    numstr[i] = 0;
    ret = atof(numstr);
    if (root)
	ret = sqrt(ret);
    if (inverse)
	ret = 1 / ret;
    return	ret;
    }

/**************************************************************************
	Get size of picture
**************************************************************************/

static	char	*findsize(HWND hwnd, char *command, char **rules, char depth)

    {
    char	**rulind, tran;
    char	saveang, saverev;
    double	savesize, saverang, savex, savey;

/*
cursor_at(12, 30);
printf("Processing level: %d", depth);
*/
    while ((*command) && (*command != ']'))
	{
	if (!(counter++))
	    thinking(hwnd, depth);
	if (endloop)
	    return ("");
	tran = 0;
	if (depth)
	    {
	    for (rulind = rules; *rulind; rulind++)
		if (**rulind == *command)
		    {
		    tran = 1;
		    findsize(hwnd, (*rulind) + 1, rules, (char)(depth - 1));
		    }
	    }
	if (user_data(hwnd) < 0)
	    {
	    endloop = TRUE;
	    return ("");
	    }
	if (!depth || !tran)
	    {
	    switch (*command)
		{
		case '+':
		    if (reverse)
			{
			if (++angle == maxangle)
			    angle = 0;
			}
		    else
			{
			if (angle)
			    angle--;
			else
			    angle = maxangle - 1;
			}
		    break;
		case '-':
		    if (reverse)
			{
			if (angle)
			    angle--;
			else
			    angle = maxangle - 1;
			}
		    else
			{
			if (++angle == maxangle)
			    angle = 0;
			}
		    break;
		case '/':
		    if (reverse)
			{
			realangle -= getnumber(&command);
			while (realangle < 0.0)
			    realangle += 360.0;
			}
		    else
			{
			realangle += getnumber(&command);
			while (realangle >= 360.0)
			    realangle -= 360.0;
			}
		    break;
		case '\\':
		    if (reverse)
			{
			realangle += getnumber(&command);
			while (realangle >= 360.0)
			    realangle -= 360.0;
			}
		    else
			{
			realangle -= getnumber(&command);
			while (realangle < 0.0)
			    realangle += 360.0;
			}
		    break;
		case '@':
		    size *= getnumber(&command);
		    break;
		case '|':
		    angle += maxangle / 2;
		    angle %= maxangle;
		    break;
		case '!':
		    reverse =! reverse;
		    break;
		case 'd':
		case 'm':
		    xpos += size * aspect * cos(realangle * PI / 180.0);
		    ypos += size * sin(realangle * PI / 180.0);
		    if (xpos > Xmax)
			Xmax = xpos;
		    if (ypos > Ymax)
			Ymax = ypos;
		    if (xpos < Xmin)
			Xmin = xpos;
		    if (ypos < Ymin)
			Ymin = ypos;
		    break;
		case 'g':
		case 'f':
		    xpos += size * coss[angle];
		    ypos += size * sins[angle];
		    if (xpos > Xmax)
			Xmax = xpos;
		    if (ypos > Ymax)
			Ymax = ypos;
		    if (xpos < Xmin)
			Xmin = xpos;
		    if (ypos < Ymin)
			Ymin = ypos;
		    break;
		case '[':
		    saveang = angle;
		    saverev = reverse;
		    savesize = size;
		    saverang = realangle;
		    savex = xpos;
		    savey = ypos;
		    command = findsize(hwnd, command + 1, rules, depth);
		    angle = saveang;
		    reverse = saverev;
		    size = savesize;
		    realangle = saverang;
		    xpos = savex;
		    ypos = savey;
		    break;
		}
	    }
	command++;
	}
    return command;
    }

/**************************************************************************
	Get size of picture
**************************************************************************/

static	void	findscale(HWND hwnd, char *command, char **rules, char depth)

    {
    double	horiz, vert;
    int	i;

    aspect = SCREENASPECT * xdots / ydots;
    for (i = 0; i < maxangle; i++)
	{
	sins[i] = sin(2.0 * (double) i * PI / maxangle);
	coss[i] = aspect * cos(2.0 * (double) i * PI / maxangle);
	}

    angle = reverse = counter = 0;
    xpos = ypos = Xmin = Xmax = Ymax = Ymin = realangle = 0.0;
    size = 1;
    findsize(hwnd, command, rules, depth);
    if (endloop)
	return;
    if (Xmax == Xmin)
	horiz = 1E35;
    else
	horiz = (xdots - 10) / (Xmax - Xmin);
    if (Ymax == Ymin)
	  vert = 1E35;
    else
	  vert = (double) (ydots - 6) / (Ymax - Ymin);
    size = (vert < horiz) ? vert : horiz;
    if (horiz == 1E35)
	xpos = (double) xdots / 2.0;
    else
	xpos = -Xmin * size + 5.0 + ((double) (xdots - 10) - size * (Xmax - Xmin)) / 2.0;
    if (vert == 1E35)
	ypos = (double) ydots / 2.0;
    else
	ypos = -Ymin * size + 3.0 + ((double) (ydots - 6) - size * (Ymax - Ymin)) / 2.0;
    }

/**************************************************************************
	Draw the picture
**************************************************************************/

static	char	*drawLSys(HWND hwnd, char *command, char **rules, char depth)

    {
    char	**rulind, tran;
    WORD	lastx, lasty;
    char	saveang, saverev, savecolor;
    double	savesize, saverang, savex, savey;

    while (*command && *command != ']')
	{
	if (endloop)
	    return ("");
	if (!(counter++))
	    {
	    if (user_data(hwnd) < 0)
		{
		endloop = TRUE;
		return ("");
		}
/*
	    if (getakey() == ESC_CHAR)
		{
		bell(2);
		endloop = TRUE;
		return ("");
		}
*/
	    }

	tran = 0;
	if (depth)
	    {
	    for (rulind = rules; *rulind; rulind++)
		if (**rulind == *command)
		    {
		    tran = 1;
		    drawLSys(hwnd, (*rulind) + 1, rules, (char)(depth - 1));
		    }
	    }
	if (!depth || !tran)
	    {
	    switch (*command)
		{
		case '+':
		    if (reverse)
			{
			if (++angle == maxangle)
			    angle = 0;
			}
		    else
			{
			if (angle)
			    angle--;
			else
			    angle = maxangle - 1;
			}
		    break;
		case '-':
		    if (reverse)
			{
			if (angle)
			    angle--;
			else
			    angle = maxangle - 1;
			}
		    else
			{
			if (++angle == maxangle)
			    angle=0;
			}
		    break;
		case '/':
		    if (reverse)
			{
			realangle -= getnumber(&command);
			while (realangle < 0)
			    realangle += 360;
			}
		    else
			{
			realangle += getnumber(&command);
			while (realangle >= 360)
			    realangle-=360;
			}
		    break;
		case '\\':
		    if (reverse)
			{
			realangle += getnumber(&command);
			while (realangle >= 360)
			    realangle -= 360;
			}
		    else
			{
			realangle -= getnumber(&command);
			while (realangle < 0)
			    realangle+=360;
			}
		    break;
		case '@':
		    size *= getnumber(&command);
		    break;
		case '|':
		    angle += maxangle / 2;
		    angle %= maxangle;
		    break;
		case '!':
		    reverse =! reverse;
		    break;
		case 'd':
		    lastx = (int) xpos;
		    lasty = (int) ypos;
		    xpos += size * aspect * cos(realangle * PI / 180.0);
		    ypos += size * sin(realangle * PI / 180.0);
//		    fastline(lastx, lasty, (WORD) xpos, (WORD) ypos, 15);
		    fastline(lastx, lasty, (WORD) xpos, (WORD) ypos, curcolor);
		    break;
		case 'm':
		    xpos += size * aspect * cos(realangle * PI / 180.0);
		    ypos += size * sin(realangle * PI / 180.0);
		    break;
		case 'g':
		    xpos += size * coss[angle];
		    ypos += size * sins[angle];
		    break;
		case 'f':
		    lastx = (int) xpos;
		    lasty = (int) ypos;
		    xpos += size * coss[angle];
		    ypos += size * sins[angle];
		    fastline(lastx, lasty, (WORD) xpos, (WORD) ypos, curcolor);
//		    fastline(lastx, lasty, (WORD) xpos, (WORD) ypos, 15);
		    break;
		case '[':
		    saveang = angle;
		    saverev = reverse;
		    savesize = size;
		    saverang = realangle;
		    savex = xpos;
		    savey = ypos;
		    savecolor = curcolor;
		    command = drawLSys(hwnd, command + 1, rules, depth);
		    angle = saveang;
		    reverse = saverev;
		    size = savesize;
		    realangle = saverang;
		    xpos = savex;
		    ypos = savey;
		    curcolor = savecolor;
		    break;
		case 'c':
		    curcolor = ((int) getnumber(&command)) % 256;
		    break;
		case '>':
		    curcolor -= (int) getnumber(&command);
		    if ((curcolor &= 255) == 0)
			curcolor = 255;
		    break;
		case '<':
		    curcolor += (int) getnumber(&command);
		    if ((curcolor &= 255) == 0)
			curcolor = 1;
		    break;
		}
	    }
	command++;
	}
    return command;
    }

/**************************************************************************
	Read the L-System file
**************************************************************************/

static	int	readLSystemFile(HWND hwnd, char *str, char *filename)

    {
    int	i;
    char	**rulind;
    char	*tok;
    int	err = 0;
    int	linenum = 0, check = 0;
    char	InLine[164], fixed[164], *word;
    FILE	*fp;
    char	msgbuf[481];				/* enough for 6 full lines */
    char	s[200];

    maxangle = 0;
    for (linenum = 0; linenum < MAXRULES; ++linenum)
	ruleptrs[linenum] = NULL;


    if ((fp = fopen(filename, "r")) == NULL)
	{
	wsprintf(s, "Can't Open LSystem File: <%s>", filename);
	MessageBox (hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    do
	{
	if (fgets(InLine, 160, fp) == 0)
	    {
	    wsprintf(s, "Can't Get LSystem Data: <%s>", filename);
	    MessageBox (hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	    return -1;
	    }
	++linenum;
	}
	while (str_find_ci(InLine, lptr[lsys_ptr]) == 0);

    rulind = &ruleptrs[1];
    msgbuf[0] = 0;

    while(fgets(InLine, 160, fp))		/* Max line length 160 chars */
	{
	linenum++;
	for (i = 0; *(InLine + i); ++i)		/* knock down to lower case */
	    *(InLine + i) = tolower(*(InLine + i));

        
	if ((word = strchr(InLine,';')))	// strip comment
	    *word = 0;
	strlwr(InLine);
      
	word = strtok(InLine," =\011\n");
	if (!word || !strcmp(word, ";;"));
	else if (!strcmp(word, "axiom"))
	    {
	    if ((tok = strtok(NULL, " \011\n")) != NULL)
		save_rule(tok, &ruleptrs[0]);
	    check = 1;
	    }
	else if (!strcmp(word, "angle"))
	    {
	    if ((tok = strtok(NULL, " \011\n")) != NULL)
		maxangle = atoi(tok);
	    else
		maxangle = 0;
	    check = 1;
	    }
	else if (!strcmp(word, "}"))
	     break;
	else if (strlen(word) == 1)
	    {
	    if ((tok = strtok(NULL, " \011\n")) != NULL)
		strcat(strcpy(fixed, word), tok);
	    else
		strcpy(fixed, word);
	    save_rule(fixed, rulind++);
	    check = 1;
	    }
	else if (err < 6)
	    {
	    sprintf(&msgbuf[strlen(msgbuf)], 
				    "Syntax error line %d: %s\n",linenum,word);
	    ++err;
	    }
	if (check)
	    {
	    check = 0;
	    if (word = strtok(NULL, " \011\n"))
		if (err < 6)
		    {
		    sprintf(&msgbuf[strlen(msgbuf)],
			   "Extra text after command line %d: %s\n",linenum, word);
		    ++err;
		    }
	    }
	}
    fclose(fp);
    if (!ruleptrs[0] && err < 6)
	{
	strcat(msgbuf,"Error:  no axiom\n");
	++err;
	}
    if ((maxangle < 3 || maxangle > 50) && err < 6)
	{
	strcat(msgbuf,"Error:  illegal or missing angle\n");
	++err;
	}
    if (err)
	{
	msgbuf[strlen(msgbuf) - 1] = 0;		/* strip trailing \n */
	fprintf(stderr, "\n%s", msgbuf);
	return -1;
	}
    *rulind = NULL;
    return 0;
    }

/**************************************************************************
	Main subroutine
**************************************************************************/

int	Lsystem(HWND hwnd, char *filename)

    {
    char	order;
    HCURSOR  hCursor;

    if ((!loaded) && LLoad(hwnd, filename)) 
	return (-1);
    order = levels;
    if (order <= 0) 
	order = 0;
    endloop = FALSE;
    findscale(hwnd, ruleptrs[0], &ruleptrs[1], order);

    hCursor = LoadCursor(NULL, IDC_WAIT);		// Load hour-glass cursor.
    Dib.ClearDib(BackgroundColour);			// set background colour
    if (!endloop)
	{
	realangle = angle = reverse = 0;
	curcolor = colour;
//	openpl(video_mode);
	drawLSys(hwnd, ruleptrs[0], &ruleptrs[1], order);
//	if (!endloop)
//	    bell(1);
	}
    hCursor = LoadCursor(NULL, IDC_ARROW);		// Load normal cursor.
    free_rules_mem();
//    closepl(!endloop);
    loaded = 0;
    DisplayFractal(hwnd);
    return 0;
    }

/**************************************************************************
	Load L-System Name
**************************************************************************/

int	LLoad(HWND hwnd, char *filename)

    {
    char i;
    if (readLSystemFile(hwnd, lptr[lsys_ptr], filename) < 0)
	{							/* error occurred */
	free_rules_mem();
	loaded = 0;
	return -1;
	}

    for (i = 0; i < maxangle; i++)
	{
	sins[i] = sin(2.0 * (double) i * PI / maxangle);
	coss[i] = aspect * cos(2.0 * (double) i * PI / maxangle);
	}
    loaded = 1;
    return 0;
    }

/**************************************************************************
	Free Rules Memory
**************************************************************************/

static	void	free_rules_mem(void)

    {
    int	i;
    for (i = 0; i < MAXRULES; ++i)
	if (ruleptrs[i])
	    free(ruleptrs[i]);
    }

/**************************************************************************
	Load Rules Memory
**************************************************************************/

static	int	save_rule(char *rule, char **saveptr)

    {
    int	i;
    char	*tmp;

    i = (int)strlen(rule) + 1;
    if ((tmp = (char *)malloc((long) i)) == NULL)
	return -1;
    *saveptr = tmp;
    while(--i >= 0)
	*(tmp++) = *(rule++);
    return 0;
    }

/**************************************************************************
	Display Thinking
**************************************************************************/

void	thinking(HWND hwnd, int count)

    {
    static	int	thinkstate, slow = 0;
    static	char	wheel[] = {'-', '\\', '|', '/'};
    char		s[100];

    if (slow++ % 40 == 0)
	{
	thinkstate = (thinkstate + 1) & 3;
	wsprintf(s, "Thinking. Level = <%d> %c", count, wheel[thinkstate]);
	SetWindowText (hwnd, s);
	}
    }




extern	int	xdots, ydots;
#define dopoint(x,y,c) { if(x < xdots && y < ydots) Plot.PlotPoint(x,y,c); }

/**************************************************************************
	Draw a line from raw x0,y0 to x1,y1
**************************************************************************/

int	MPOverflow;
/*
long	multiply(long a, long b, int offset)

    {
    __int64	c;

    c = (__int64)a * (__int64)b;
    return (long)(((__int64)c) >> offset);
    }
*/
long	divide(long a, long b, int offset)

    {
    __int64	c;

    if (b == 0L)
	MPOverflow = 1;
    c = (__int64)a / b;
    return (long)(((__int64)c) << offset);

    }

void	fastline(const WORD x0, const WORD y0, const WORD x1, const WORD y1, const DWORD col)

    {
    register WORD x, y;
    long	xstep, ystep, xcum = 0L, ycum = 0L;
    int	dx, dy;

    dx = (int) x1 - (int) x0;
    dy = (int) y1 - (int) y0;
    if (dx == 0)						/* vertical line */
	{
	dy = (dy > 0) ? 1 : -1;

	for (y = y0; y != y1; y += dy)
	    dopoint(x0, y, col);
	}
    else if (y0 == y1)					/* horizontal line */
	{
	dx = (dx > 0) ? 1 : -1;
	for (x = x0; x != x1; x += dx)
	    dopoint(x, y0, col);
	}
    else							/* sloping line */
	{ 
	if (abs(dx) >= abs(dy))				/* plot along x axis */
	    {
	    ystep = divide((((long) dy) << 16), dx, 0);
	    if (dy < 0)					/* y goes to left */
		{
		if (ystep > 0L)
		    ystep = - ystep;		/* adjust for wrong sign */
		}
	    else if (ystep < 0L)				/* y goes to right */
		ystep = - ystep;
	    dx = (dx > 0) ? 1 : -1;
	    for (x = x0, y = y0; x != x1; x += dx)
		{
		dopoint(x, y, col);
		ycum += ystep;		/* cumulative motion along y axis */
		y = y0 + (int) (ycum >> 16);
		}
	    }
	else						/* plot along y axis */
	    {
	    xstep = divide((((long) dx) << 16), dy, 0);
	    if (dx < 0)
		{
		if (xstep > 0L)
		    xstep = - xstep;
		}
	    else if (xstep < 0)
		xstep = - xstep;
	    dy = (dy > 0) ? 1 : -1;
    	    for (y = y0, x = x0; y != y1; y += dy)
		{
		dopoint(x, y, col);
		xcum += xstep;
		x = x0 + (WORD)(xcum >> 16);
		}
	    }
	}
    }

/**************************************************************************
	Compare function for L-System name sort
**************************************************************************/

int	compare_lsys(char *p1, char *p2)

    {
    return (_strnicmp(p1, p2, 20));
    }

/**************************************************************************
	Load L_System names from file
**************************************************************************/

int	load_lsystems(HWND hwnd, char *filename)

    {
    FILE	*fp;
    char	s[250];
    int	i, j;

    strcpy(LSYSFile, filename);				// load global filename for LSystem  
    strcpy(lsys_type, "Not Loaded Yet");
    if ((fp = fopen(filename, "r")) == NULL)
	{
	wsprintf(s, "Can't Open LSystem File: <%s>", filename);
	MessageBox (hwnd, s, "MANPWIN", MB_ICONEXCLAMATION | MB_OK);
	return -1;
	}

    lsys_num = 0;
    while (fgets(s, 160, fp) != NULL)
	{
	if (str_find_ci(s, "{") != 0 && str_find_ci(s, "comment") == 0)
	    {
	    for (i = 0; s[i] == ' '; ++i);
	    for (j = 0; s[j]; ++j)
		if (s[j] == '{' || s[j] == '\n' || s[j] == ' ')
		    s[j] = '\0';
	    s[56] = '\0';
	    strcpy(lptr[lsys_num], s + i);
	    if (lsys_num < 64)
		++lsys_num;
	    else
		break;
	    }
	}

//qsort((void *)lptr, (size_t)lsys_num, sizeof(lptr[0]), (int (*)(void *, void *)) compare_lsys);
    qsort((void *)lptr, (size_t)lsys_num, sizeof(lptr[0]), 
				    (int (__cdecl *)(const void *, const void *)) compare_lsys);
    fclose(fp);

    if (lsys_ptr >= lsys_num)
	lsys_ptr = 0;
    strcpy(lsys_Label, "L System");
    return 0;
    }

/**************************************************************************
	Show L_Systems on file
**************************************************************************/

DLGPROC FAR PASCAL SelectFractal(HWND hDlg, UINT message, UINT wParam, LONG lParam)
//BOOL FAR PASCAL SelectFractal(HWND hDlg, unsigned message, WORD wParam, LONG lParam)

    {
    static	int	i;
    static	int	index;

    switch (message) 
	{
        case WM_INITDIALOG:
	    cycleflag = FALSE;
            SetDlgItemText(hDlg, ID_LISTTITLE, 	lsys_Label);
            for (i = 0; i < lsys_num; i++) 
                SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_ADDSTRING, (WPARAM)NULL, (LPARAM) (LPSTR) lptr[i]);
            SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_SETCURSEL, (WPARAM)lsys_ptr, 0L);
            return ((DLGPROC)TRUE);

        case WM_COMMAND:
	    switch ((int) LOWORD(wParam))
//	    switch (wParam)
		{
                case IDOK:
//okay:           
                    index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
                    if (index == LB_ERR) 
			{
                        MessageBox(hDlg, "No Choice selected",
                            "Select From a List", MB_OK | MB_ICONEXCLAMATION);
                        break;
                        }
                    lsys_ptr = index;
//		    PreviousLsys_ptr = index;
		    strcpy(lsys_type, lptr[lsys_ptr]);
                    EndDialog(hDlg, TRUE);
                    return ((DLGPROC)TRUE);
                  
                case IDCANCEL:
                    lsys_ptr = -1;
                    EndDialog(hDlg, FALSE);
                    return (FALSE);
                    
                case IDM_LSYSTEM:
                    switch (HIWORD(wParam) & 0x0003) 
//                    switch (LOWORD(lParam)) 
			{
                        case LBN_SELCHANGE:
                            index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
                            if (index == LB_ERR)
                                break;
                            break;
                         
                       case LBN_DBLCLK:
//			    goto okay;
			    index = (int)SendDlgItemMessage(hDlg, IDM_LSYSTEM, LB_GETCURSEL, 0, 0L);
			    if (index == LB_ERR) 
				{
				MessageBox(hDlg, "No Choice selected",
				    "Select From a List", MB_OK | MB_ICONEXCLAMATION);
				break;
				}
			    lsys_ptr = index;
			    strcpy(lsys_type, lptr[lsys_ptr]);
			    EndDialog(hDlg, TRUE);
			    return ((DLGPROC)TRUE);
                  
			}
	    return ((DLGPROC)TRUE);
	    }
	}
    return ((DLGPROC)FALSE);
    }

/**************************************************************************
	Dialog Control for LSystem Fractal Type
**************************************************************************/

DLGPROC FAR PASCAL LSystemDlg (HWND hDlg, UINT message, UINT wParam, LONG lParam)
     {
//     static	HANDLE	hCursor;
     static     char	temp;
     static     UINT	tempParam;
     static     BYTE	temp_levels;
     BOOL		bTrans ;
     
     switch (message)
	  {
	  case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_DEGREE, levels, TRUE);
	        return (DLGPROC)TRUE ;
//	        return FALSE ;

	  case WM_COMMAND:
//	        switch (wParam)
	        switch ((int) LOWORD(wParam))
		    {
		    case IDC_DEGREE:
			temp_levels = GetDlgItemInt(hDlg, IDC_DEGREE, &bTrans, TRUE);
		        return (DLGPROC)TRUE ;

		    case IDOK:
			if (temp_levels < 1)
			    levels = 1;
			else if (temp_levels > 75)
			    levels = 75;
			else
			    levels = temp_levels;
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

