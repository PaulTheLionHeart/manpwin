/* STRANGE ATTRACTOR PROGRAM QuickC Ver 2.0 (c) 1993 by J. C. Sprott */
#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
//#include <graph.h>
#include <math.h>
#include "manp.h"

static	int PREV, OMAX, D, ODE, /*SND, */PJT, TRD, FTH, SAV, T, WID, QM, P, TWOD;
static	int M, I, I1, I2, O, I3, I4, I5, J, WH, FREQ, C4, NC, C, RD, CYAN, BK;
static	int COLR[16];
static	char CODE[515], Q;
static	char FAV[20] = "XDATA.DAT";
static	double NMAX, EPS, TWOPI, SEG, NE, X, Y, Z, W, XE, YE, ZE, WE, LSUM, N, NL;
static	double N1, N2, XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX, WMIN, WMAX, XNEW, YNEW;
static	double ZNEW, WNEW, XP, YP, RAN, XSAVE, YSAVE, ZSAVE, WSAVE, DLX, DLY, DLZ;
static	double DLW, DL2, DF, RS, L, MX, MY, XL, XH, YL, YH, XA, YA, ZA, TT, PT, TIA;
static	double XZ, YZ, DUR, D2MAX, DX, DY, DZ, DW, D2, F, TH, PH, XRT, XLT, HSF, AL;
static	double SINAL, COSAL, DUM, SW, SH;
static	double XS[500], YS[500], ZS[500], WS[500], A[505], V[100], XY[5], XN[5];

void	fun1300(); /* Initialize */
void	fun1500(); /* Set parameters */
void	fun1700(); /* Iterate equations */
void	fun2100(); /* Display results */
void	fun2400(); /* Test results */
void	fun2600(); /* Get coefficients */
void	fun2800(); /* Shuffle random numbers */
void	fun2900(); /* Calculate Lyapunov exponent */
void	fun3100(); /* Resize the screen */
//void	fun3500(); /* Produce sound */
void	fun3600(); /* Respond to user command */
void	fun3900(); /* Calculate fractal dimension */
void	fun4100(); /* Project onto a sphere */
void	DisplayMenuScreen();
void	fun4700(); /* Get dimension and order */
void    fun4900(); /* Save attractor to disk file SA.DIC */
void	fun5000(); /* Plot point on screen */
void	fun5400(); /* Plot background grid */
void	fun5600(); /* Set colors */
void	fun5800(); /* Process evaluation command */
void	fun5900(); /* Save favorite attractors to disk file FAVORITE.DIC */
void	fun6000(); /* Update SA.DIC file */
void	fun6200(); /* Special function definitions */
void	fun6700(); /* Project onto a horizontal cylinder */
void	fun6800(); /* Project onto a vertical cylinder */
void	fun6900(); /* Project onto a torus (unity aspect ratio) */
void	fun7000(); /* Save data */




int	colour;

void	_setpixel_w(double x, double y)
    {
    outpoint((WORD)x, (WORD)y, (DWORD)colour);
    }

int	_getpixel_w(double x, double y)
    {
    return (int)ingetcolor((WORD)x, (WORD)y);
    }

void	_setcolor(int col)
    {
    colour = col;
    }

//union REGS regs;
FILE *F1, *F2, *F3;
int	StrangeAttractor(void)
    {
    int	    count = 0;
    PREV = 5; /* Plot versus fifth previous iterate */
    NMAX = 11000; /* Maximum number of iterations */
    OMAX = 5; /* Maximum order of polynomial */
    D = 2; /* Dimension of system */
    EPS = 0.1; /* Step size for ODE */
    ODE = 0; /* System is map */
//    SND = 0; /* Turn sound off */
    PJT = 0; /* Projection is planar */
    TRD = 1; /* Display third dimension as shadow */
    FTH = 2; /* Display fourth dimension as colors */
    SAV = 0; /* Don’t save any data */
    TWOPI = 6.28318530717959; /* A useful constant (2 pi) */
    srand((unsigned)time(NULL)); /* Reseed random-number generator */
    DisplayMenuScreen();
    T = 1;
    if (Q == 'X') T = 0; /* Exit immediately on command */
    while (T) 
	{
	switch (T) 
	    {
	    case 1: fun1300();	/* Initialize */
	    case 2: fun1500();	/* Set parameters */
	    case 3: fun1700();	/* Iterate equations */
	    case 4: fun2100();	/* Display results */
	    case 5: fun2400();	/* Test results */
	    }
	if (count++ > 1000000)	// just in case
	    break;
	}
//    _clearscreen(_GCLEARSCREEN); /* Erase screen */
//    _setvideomode(_DEFAULTMODE); /* and restore video mode */
    return 0;
    }


void	fun1300() /* Initialize */
    {
//    _setvideomode(_VRES16COLOR); /* Assume VGA graphics */
    WID = 80; /* Number of text columns */
//    _clearscreen(_GCLEARSCREEN);
//    _settextposition(13, WID / 2 - 6);
//    printf("Searching...");
    fun5600(); /* Set colors */
    if (QM == 2) 
	{
	NE = 0;
	fclose(F1);
	F1 = fopen("SA.DIC", "a");
	fclose(F1);
	F1 = fopen("SA.DIC", "r");
	}
    }


void	fun1500() /* Set parameters */
    {
    X = .05; /* Initial condition */
    Y = .05;
    Z = .05;
    W = .05;
    XE = X + .000001;
    YE = Y;
    ZE = Z;
    WE = W;
    fun2600(); /* Get coefficients */
    T = 3;
    P = 0;
    LSUM = 0;
    N = 0;
    NL = 0;
    N1 = 0;
    N2 = 0;
    XMIN = 1000000;
    XMAX = -XMIN;
    YMIN = XMIN;
    YMAX = XMAX;
    ZMIN = XMIN;
    ZMAX = XMAX;
    WMIN = XMIN;
    WMAX = XMAX;
    TWOD = _rotl(1, D);
    }


void	fun1700() /* Iterate equations */
    {
    if (ODE > 1)
	fun6200(); /* Special function */
    else 
	{
	M = 1;
	XY[1] = X;
	XY[2] = Y;
	XY[3] = Z;
	XY[4] = W;
	for (I = 1; I <= D; I++) 
	    {
	    XN[I] = A[M];
	    M = M + 1;
	    for (I1 = 1; I1 <= D; I1++) 
		{
		XN[I] = XN[I] + A[M] * XY[I1];
		M = M + 1;
		for (I2 = I1; I2 <= D; I2++) 
		    {
		    XN[I] = XN[I] + A[M] * XY[I1] * XY[I2];
		    M = M + 1;
		    for (I3 = I2; O > 2 && I3 <= D; I3++) 
			{
			XN[I] = XN[I] + A[M] * XY[I1] * XY[I2] * XY[I3];
			M = M + 1;
			for (I4 = I3; O > 3 && I4 <= D; I4++) 
			    {
			    XN[I] = XN[I] + A[M] * XY[I1] * XY[I2] * XY[I3] * XY[I4];
			    M = M + 1;
			    for (I5 = I4; O > 4 && I5 <= D; I5++) 
				{
				XN[I] = XN[I] + A[M] * XY[I1] * XY[I2] * XY[I3] * XY[I4] * XY[I5];
				M = M + 1;
				}
			    }
			}
		    }
		}
	    if (ODE == 1) XN[I] = XY[I] + EPS * XN[I];
	    }
	XNEW = XN[1];
	YNEW = XN[2];
	ZNEW = XN[3];
	WNEW = XN[4];
	}
    N = N + 1;
    M = M - 1;
    }


void	fun2100() /* Display results */
    {
    if (N >= 100 && N <= 1000) 
	{
	if (X < XMIN) XMIN = X;
	if (X > XMAX) XMAX = X;
	if (Y < YMIN) YMIN = Y;
	if (Y > YMAX) YMAX = Y;
	if (Z < ZMIN) ZMIN = Z;
	if (Z > ZMAX) ZMAX = Z;
	if (W < WMIN) WMIN = W;
	if (W > WMAX) WMAX = W;
	}
    if ((int)N == 1000) 
	fun3100(); /* Resize the screen */
    XS[P] = X;
    YS[P] = Y;
    ZS[P] = Z;
    WS[P] = W;
    P = (P + 1) % 500;
    I = (P + 500 - PREV) % 500;
    if (D == 1) 
	{
	XP = XS[I];
	YP = XNEW;
	}
    else 
	{
	XP = X;
	YP = Y;
	}
    if (N >= 1000 && XP > XL && XP < XH && YP > YL && YP < YH) 
	{
	if (PJT == 1) fun4100(); /* Project onto a sphere */
	if (PJT == 2) fun6700(); /* Project onto a horizontal cylinder */
	if (PJT == 3) fun6800(); /* Project onto a vertical cylinder */
	if (PJT == 4) fun6900(); /* Project onto a torus */
	fun5000(); /* Plot point on screen */
//	if (SND == 1) fun3500(); /* Produce sound */
	}
    }


void	fun2400() /* Test results */
    {
    if (fabs(XNEW) + fabs(YNEW) + fabs(ZNEW) + fabs(WNEW) > 1000000) T = 2;
    if (QM != 2) { /* Speed up evaluation mode */
    fun2900(); /* Calculate Lyapunov exponent */
    fun3900(); /* Calculate fractal dimension */
    if (QM == 0) { /* Skip tests unless in search mode */
    if (N >= NMAX) { /* Strange attractor found */
    T = 2;
    fun4900(); /* Save attractor to disk file SA.DIC */
    }
    if (fabs(XNEW - X) + fabs(YNEW - Y) + fabs(ZNEW - Z) + fabs(WNEW - W) < 0.000001) 
	T = 2;
    if (N > 100 && L < 0.005) 
	T = 2; /* Limit cycle */
    }
    }
//    if (kbhit()) Q = getch(); else Q = 0;
//    if (Q) fun3600(); /* Respond to user command */
    if (SAV > 0) 
	if (N > 1000 && N < 17001) 
	    fun7000(); /* Save data */
    X = XNEW; /* Update value of X */
    Y = YNEW;
    Z = ZNEW;
    W = WNEW;
    }


void	fun2600() /* Get coefficients */
{
if (QM == 2) { /* In evaluate mode */
fgets(CODE, 515, F1);
if (feof(F1)) {
QM = 0;
fun6000(); /* Update SA.DIC file */
}
else {
fun4700(); /* Get dimension and order */
fun5600(); /* Set colors */
}
}
if (QM == 0) { /* In search mode */
O = 2 + (int)floor((OMAX - 1) * (float)rand() / 32768.0);
CODE[0] = 59 + 4 * D + O + 8 * ODE;
if (ODE > 1) CODE[0] = 87 + ODE;
fun4700(); /* Get value of M */
for (I = 1; I <= M; I++) { /* Construct CODE */
fun2800(); /* Shuffle random numbers */
CODE[I] = 65 + (int)floor(25 * RAN);
}
CODE[M + 1] = 0;
}
for (I = 1; I <= M; I++) { /* Convert CODE to coefficient values */
A[I] = (CODE[I] - 77) / 10.0;
}
}

void	fun2800() /* Shuffle random numbers */
    {
    if (V[0] == 0) 
	for (J = 0; J <= 99; J++) 
	    {
	    V[J] = (float)rand() / 32768.0;
	    }
    J = (int)floor(100 * RAN);
    RAN = V[J];
    V[J] = (float)rand() / 32768.0;
    }

void	fun2900() /* Calculate Lyapunov exponent */
    {
    XSAVE = XNEW;
    YSAVE = YNEW;
    ZSAVE = ZNEW;
    WSAVE = WNEW;
    X = XE;
    Y = YE;
    Z = ZE;
    W = WE;
    N = N - 1;
    fun1700(); /* Reiterate equations */
    DLX = XNEW - XSAVE;
    DLY = YNEW - YSAVE;
    DLZ = ZNEW - ZSAVE;
    DLW = WNEW - WSAVE;
    DL2 = DLX * DLX + DLY * DLY + DLZ * DLZ + DLW * DLW;
    if (DL2 > 0) 
	{ /* Check for division by zero */
	DF = 1E12 * DL2;
	RS = 1 / sqrt(DF);
	XE = XSAVE + RS * (XNEW - XSAVE);
	YE = YSAVE + RS * (YNEW - YSAVE);
	ZE = ZSAVE + RS * (ZNEW - ZSAVE);
	WE = WSAVE + RS * (WNEW - WSAVE);
	XNEW = XSAVE;
	YNEW = YSAVE;
	ZNEW = ZSAVE;
	WNEW = WSAVE;
	LSUM = LSUM + log(DF);
	NL = NL + 1;
	L = .721347 * LSUM / NL;
	if (ODE == 1 || ODE == 7) L = L / EPS;
	if (N > 1000 && (int)N % 10 == 0) 
	    {
//	    _settextposition(1, WID - 4);
	    printf("%5.2f", L);
	    }
	}
    }

void	fun3100() // Resize the screen 
{
//_setcolor(WH);
if (D == 1) {
YMIN = XMIN;
YMAX = XMAX;
}
if (XMAX - XMIN < 0.000001) {
XMIN = XMIN - 0.0000005;
XMAX = XMAX + 0.0000005;
}
if (YMAX - YMIN < 0.000001) {
YMIN = YMIN - 0.0000005;
YMAX = YMAX + 0.0000005;
}
if (ZMAX - ZMIN < 0.000001) {
ZMIN = ZMIN - 0.0000005;
ZMAX = ZMAX + 0.0000005;
}
if (WMAX - WMIN < 0.000001) {
WMIN = WMIN - 0.0000005;
WMAX = WMAX + 0.0000005;
}
MX = 0.1 * (XMAX - XMIN);
MY = 0.1 * (YMAX - YMIN);
XL = XMIN - MX;
XH = XMAX + MX;
YL = YMIN - MY;
YH = YMAX + 1.5 * MY;
SW = 640 / (XH - XL);
SH = 480 / (YL - YH);
//_setvieworg((short)(-SW * XL), (short)(-SH * YH));
//_clearscreen(_GCLEARSCREEN);
YH = YH - 0.5 * MY;
XA = (XL + XH) / 2;
YA = (YL + YH) / 2;
if (D > 2) {
ZA = (ZMAX + ZMIN) / 2;
if (TRD == 1) {
//_setcolor(COLR[1]);
//_rectangle_w(_GFILLINTERIOR, SW * XL, SH * YL, SW * XH, SH * YH);

fun5400(); // Plot background grid
}
/*
if (TRD == 4) {
_setcolor(WH);
_rectangle_w(_GFILLINTERIOR, SW * XL, SH * YL, SW * XH, SH * YH);
}
if (TRD == 5) {
_moveto_w(SW * XA, SH * YL);
_lineto_w(SW * XA, SH * YH);
}
if (TRD == 6) {
for (I = 1; I <= 3; I++) {
XP = XL + I * (XH - XL) / 4;
_moveto_w(SW * XP, SH * YL);
_lineto_w(SW * XP, SH * YH);
YP = YL + I * (YH - YL) / 4;
_moveto_w(SW * XL, SH * YP);
_lineto_w(SW * XH, SH * YP);
}
}
}
if (PJT != 1) _rectangle_w(_GBORDER, SW * XL + 1, SH * YL - 1, SW * XH - 1, SH
* YH + 1);
if (PJT == 1 && TRD < 5) _ellipse_w(_GBORDER, SW * XL - SH * (YH - YL) / 6, SH
* YH, SW * XH + SH * (YH - YL) / 6, SH * YL);
TT = 3.1416 / (XMAX - XMIN);
PT = 3.1416 / (YMAX - YMIN);
if (QM == 2) { // In evaluate mode
_settextposition(1, 1);
printf("<Space Bar>: Discard <Enter>: Save");
if (WID >= 80) {
_settextposition(1, 49);
printf("<Esc>: Exit");
_settextposition(1, 69);
printf("%d K left", (int)((filelength(fileno(F1)) - ftell(F1)) /
1024.0));
}}
else {
_settextposition(1, 1);
if (strlen(CODE) < WID - 18)
_outtext(CODE);
else {
printf("%*.*s...", WID - 23, WID - 23, CODE);
}
_settextposition(1, WID - 17);
printf("F =");
_settextposition(1, WID - 7);
printf("L = ");
*/
}
TIA = .05; // Tangent of illumination angle
XZ = -TIA * (XMAX - XMIN) / (ZMAX - ZMIN);
YZ = TIA * (YMAX - YMIN) / (ZMAX - ZMIN);
}


void	fun3500() /* Produce sound */
{
//FREQ = 220 * pow(2, (int)(36 * (XNEW - XL) / (XH - XL)) / 12.0);
//DUR = 1;
//if (D > 1) DUR = pow(2, (int)floor(.5 * (YH - YL) / (YNEW - 9 * YL / 8 + YH / 8)));
/* A sound statement should be placed here */
}
void	fun3600() /* Respond to user command */
{
if (Q > 96) Q = Q - 32; /* Convert to uppercase */
if (QM == 2) fun5800(); /* Process evaluation command */
if (strchr("ACDEHINPRSVX", Q) == 0) 
    DisplayMenuScreen();
if (Q == 'A') {
T = 1;
QM = 0;
}
if (ODE > 1) D = ODE + 5;
if (ODE == 1) D = D + 2;
if (Q == 'C') if (N > 999) N = 999;
if (Q == 'D') {
D = 1 + D % 12;
T = 1;
}
if (D > 6) {
ODE = D - 5;
D = 4;
}
else {
if (D > 4) {
ODE = 1;
D = D - 2;
}
else ODE = 0;
}
if (Q == 'E') {
T = 1;
QM = 2;
}
if (Q == 'H') {
FTH = (FTH + 1) % 3;
T = 3;
if (N > 999) {
N = 999;
fun5600(); /* Set colors */
}
}
if (Q == 'I') {
if (T != 1) {
//_setvideomode(_TEXTC80);
//_settextcolor(15);
//_setbkcolor(1L);
//_clearscreen(_GCLEARSCREEN);
printf("Code? ");
I = 0;
CODE[0] = 0;
do {
//CODE[I] = getche();
if (CODE[I] == 8 && I >= 0) I = I - 2;
if (CODE[I] == 27) {
I = 0;
CODE[I] = 13;
}
}
while (CODE[I++] != 13 && I < 506);
CODE[I - 1] = 0;
if (CODE[0] == 0) {
Q = ' ';
//_clearscreen(_GCLEARSCREEN);
    }
else {
T = 1;
QM = 1;
fun4700();
}
}
}
if (Q == 'N') {
NMAX = 10 * (NMAX - 1000) + 1000;
if (NMAX > 1E10) NMAX = 2000;
}
if (Q == 'P') {
PJT = (PJT + 1) % 5;
T = 3;
if (N > 999) N = 999;
}
if (Q == 'R') {
TRD = (TRD + 1) % 7;
T = 3;
if (N > 999) {
N = 999;
fun5600(); /* Get dimension and order */
}
}
//if (Q == 'S') {
//SND = (SND + 1) % 2;
//T = 3;
//}
    if (Q == 'V') 
	{
	SAV = (SAV + 1) % 5;
	FAV[0] = 87 + SAV % 4;
//	T = 3;
//	if (N > 999) 
	    N = 999;
	}
    if (Q == 'X') 
	T = 0;
    }

void	fun3900() /* Calculate fractal dimension */
    {
    if (N >= 1000) 
	{ /* Wait for transient to settle */
	if ((int)N == 1000) 
	    {
	    D2MAX = pow(XMAX - XMIN, 2);
	    D2MAX = D2MAX + pow(YMAX - YMIN, 2);
	    D2MAX = D2MAX + pow(ZMAX - ZMIN, 2);
	    D2MAX = D2MAX + pow(WMAX - WMIN, 2);
	    }
	J = (P + 1 + (int)floor(480 * (float)rand() / 32768.0)) % 500;
	DX = XNEW - XS[J];
	DY = YNEW - YS[J];
	DZ = ZNEW - ZS[J];
	DW = WNEW - WS[J];
	D2 = DX * DX + DY * DY + DZ * DZ + DW * DW;
	if (D2 < 0.001 * TWOD * D2MAX) 
	    N2 = N2 + 1;
	if (D2 <= 0.00001 * TWOD * D2MAX)   
	    {
	    N1 = N1 + 1;
	    F = 0.434294 * log(N2 / (N1 - .5));
//	    _settextposition(1, WID - 14);
	    printf("%5.2f", F);
	    }
	}
    }

void	fun4100() /* Project onto a sphere */
{
TH = TT * (XMAX - XP);
PH = PT * (YMAX - YP);
XP = XA + 0.36 * (XH - XL) * cos(TH) * sin(PH);
YP = YA + 0.5 * (YH - YL) * cos(PH);
}

void	DisplayMenuScreen()
{
//_setvideomode(_TEXTC80);
//_settextcolor(15);
//_setbkcolor(1L);
//regs.h.ah = 1;
//regs.h.ch = 1;
//regs.h.cl = 0;
//int86(16, &regs, &regs); /* Turn cursor off */
//_clearscreen(_GCLEARSCREEN);
//while (Q == 0 || strchr("AEIX", Q) == 0) 
    {
//_settextposition(1, 27);
printf("STRANGE ATTRACTOR PROGRAM\n");
printf("%26cIBM PC QuickC Version 2.0\n", ' ');
printf("%26c(c) 1993 by J. C. Sprott\n", ' ');
printf("\n");
printf("\n");
printf("%26cA: Search for attractors\n", ' ');
printf("%26cC: Clear screen and restart\n", ' ');
if (ODE > 1) {
printf("%26cD: System is 4-D special map %c \n", ' ', 87 + ODE);}
else {
printf("%26cD: System is %d-D polynomial ", ' ', D);
if (ODE == 1) printf("ODE\n"); else printf("map\n");
}
printf("%26cE: Evaluate attractors\n", ' ');
printf("%26cH: Fourth dimension is ", ' ');
if (FTH == 0) printf("projection\n");
if (FTH == 1) printf("bands \n");
if (FTH == 2) printf("colors \n");
printf("%26cI: Input code from keyboard\n", ' ');
printf("%26cN: Number of iterations is 10^%1.0f\n", ' ', log10(NMAX - 1000));
printf("%26cP: Projection is ", ' ');
if (PJT == 0) printf("planar \n");
if (PJT == 1) printf("spherical\n");
if (PJT == 2) printf("horiz cyl\n");
if (PJT == 3) printf("vert cyl \n");
if (PJT == 4) printf("toroidal \n");
printf("%26cR: Third dimension is ", ' ');
if (TRD == 0) printf("projection\n");
if (TRD == 1) printf("shadow \n");
if (TRD == 2) printf("bands \n");
if (TRD == 3) printf("colors \n");
if (TRD == 4) printf("anaglyph \n");
if (TRD == 5) printf("stereogram\n");
if (TRD == 6) printf("slices \n");
printf("%26cS: Sound is ", ' ');
//if (SND == 0) printf("off\n");
//if (SND == 1) printf("on \n");
printf("%26cV: ", ' ');
//if (SAV == 0) printf("No data will be saved \n");
//if (SAV > 0) printf("%c will be saved in %cDATA.DAT\n", FAV[0], FAV[0]);
printf("%26cX: Exit program", ' ');
//if (kbhit()) Q = getch(); else Q = 0;
if (Q) fun3600(); /* Respond to user command */
}
}
void	fun4700() /* Get dimension and order */
{
D = 1 + (int)floor((double)((CODE[0] - 65) / 4));
if (D > 6) {
ODE = CODE[0] - 87;
D = 4;
fun6200(); /* Special function */
}
else {
if (D > 4) {
D = D - 2;
ODE = 1;
}
else ODE = 0;
O = 2 + (CODE[0] - 65) % 4;
M = 1;
for (I = 1; I <= D; I++) {M = M * (O + I);}
if (D > 2) for (I = 3; I <= D; I++) {M = M / (I - 1);}
}
if (strlen(CODE) != M + 1 && QM == 1) {
printf("\a"); /* Illegal code warning */
while (strlen(CODE) < (size_t)(M + 1)) strcat(CODE, "M");
if (strlen(CODE) > (size_t)(M + 1)) CODE[M + 1] = 0;
}
}
void	fun4900() /* Save attractor to disk file SA.DIC */
{
F1 = fopen("SA.DIC", "a");
fprintf(F1, "%s%5.2f%5.2f\n", CODE, F, L);
fclose(F1);
}
void	fun5000() /* Plot point on screen */
{
C4 = WH;
if (D > 3) {
if (FTH == 1) if ((int)floor(30 * (W - WMIN) / (WMAX - WMIN)) % 2) return;
if (FTH == 2) C4 = 1 + (int)floor(NC * (W - WMIN) / (WMAX - WMIN) + NC) % NC;
}
if (D < 3) { /* Skip 3-D stuff */
_setpixel_w(SW * XP, SH * YP);
return;
}
if (TRD == 0) {
_setcolor(C4);
_setpixel_w(SW * XP, SH * YP);
}
if (TRD == 1) {
if (D > 3 && FTH == 2) {
_setcolor(C4);
_setpixel_w(SW * XP, SH * YP);
}
else {
C = _getpixel_w(SW * XP, SH * YP);
if (C == COLR[2]) {
_setcolor(COLR[3]);
_setpixel_w(SW * XP, SH * YP);}
else {
if (C != COLR[3]) {
_setcolor(COLR[2]);
_setpixel_w(SW * XP, SH * YP);
}
}
}
XP = XP - XZ * (Z - ZMIN);
YP = YP - YZ * (Z - ZMIN);
if (_getpixel_w(SW * XP, SH * YP) == COLR[1]) {
_setcolor(0);
_setpixel_w(SW * XP, SH * YP);
}
}
if (TRD == 2) {
if (D > 3 && FTH == 2 && ((int)floor(15 * (Z - ZMIN) / (ZMAX - ZMIN) + 2) %
2) == 1) {
_setcolor(C4);}
else {
C = COLR[(int)floor(60 * (Z - ZMIN) / (ZMAX - ZMIN) + 4) % 4];
_setcolor(C);
}
_setpixel_w(SW * XP, SH * YP);
}
if (TRD == 3) {
_setcolor(COLR[(int)floor(NC * (Z - ZMIN) / (ZMAX - ZMIN) + NC) % NC]);
_setpixel_w(SW * XP, SH * YP);
}
if (TRD == 4) {
XRT = XP + XZ * (Z - ZA);
C = _getpixel_w(SW * XRT, SH * YP);
if (C == WH) {
_setcolor(RD);
_setpixel_w(SW * XRT, SH * YP);
}
if (C == CYAN) {
_setcolor(BK);
_setpixel_w(SW * XRT, SH * YP);
}
XLT = XP - XZ * (Z - ZA);
C = _getpixel_w(SW * XLT, SH * YP);
if (C == WH) {
_setcolor(CYAN);
_setpixel_w(SW * XLT, SH * YP);
}
if (C == RD) {
_setcolor(BK);
_setpixel_w(SW * XLT, SH * YP);
}
}
if (TRD == 5) {
HSF = 2; /* Horizontal scale factor */
XRT = XA + (XP + XZ * (Z - ZA) - XL) / HSF;
_setcolor(C4);
_setpixel_w(SW * XRT, SH * YP);
XLT = XA + (XP - XZ * (Z - ZA) - XH) / HSF;
_setcolor(C4);
_setpixel_w(SW * XLT, SH * YP);
}
if (TRD == 6) {
DZ = (15 * (Z - ZMIN) / (ZMAX - ZMIN) + 0.5) / 16;
XP = (XP - XL + ((int)floor(16 * DZ) % 4) * (XH - XL)) / 4 + XL;
YP = (YP - YL + (3 - (int)floor(4 * DZ) % 4) * (YH - YL)) / 4 + YL;
_setcolor(C4);
_setpixel_w(SW * XP, SH * YP);
}
}
void	fun5400() /* Plot background grid */
    {
    return;
/*
    _setcolor(0);
    for (I = 0; I <= 15; I++) 
	{	// Draw 15 vertical grid lines
	XP = XMIN + I * (XMAX - XMIN) / 15;
	_moveto_w(SW * XP, SH * YMIN);
	_lineto_w(SW * XP, SH * YMAX);
	}
    for (I = 0; I <= 10; I++) 
	{ // Draw 10 horizontal grid lines
	YP = YMIN + I * (YMAX - YMIN) / 10;
	_moveto_w(SW * XMIN, SH * YP);
	_lineto_w(SW * XMAX, SH * YP);
	}
*/
    }

void	fun5600() /* Set colors */
    {
    NC = 15; /* Number of colors */
    COLR[0] = 0;
    COLR[1] = 8;
    COLR[2] = 7;
    COLR[3] = 15;
    if (TRD == 3 || (D > 3 && FTH == 2 && TRD != 1)) 
	{
	for (I = 0; I <= NC; I++) 
	    COLR[I] = I + 1;
	}
    WH = 15;
    BK = 8;
    RD = 12;
    CYAN = 11;
    }

void	fun5800() /* Process evaluation command */
    {
    if (Q == ' ') 
	{
	T = 2;
	NE = NE + 1;
//	_clearscreen(_GCLEARSCREEN);
	}
    if (Q == 13) 
	{
	T = 2;
	NE = NE + 1;
//	_clearscreen(_GCLEARSCREEN);
	fun5900(); /* Save favorite attractors to disk */
	}
    if (Q == 27) 
	{
//	_clearscreen(_GCLEARSCREEN);
	fun6000(); /* Update SA.DIC file */
	Q = ' ';
	QM = 0;
	}
    else 
	{
	if (strchr("CHNPRVS", Q) == 0) 
	    Q = 0;
	}
    }

void	fun5900() /* Save favorite attractors to disk file FAVORITE.DIC */
    {
    F2 = fopen("FAVORITE.DIC", "a");
    fprintf(F2, CODE);
    fclose(F2);
    }

void	fun6000() /* Update SA.DIC file */
    {
//  _settextposition(11, 9);
//    printf("Evaluation complete\n");
//  _settextposition(12, 8);
//    printf(" %d cases evaluated", (int)NE);
    F2 = fopen("SATEMP.DIC", "w");
    if (QM == 2) fprintf(F2, CODE);
    while (feof(F1) == 0) 
	{
	fgets(CODE, 515, F1);
	if (feof(F1) == 0) 
	    fprintf(F2, CODE);
	}
    fcloseall();
    remove("SA.DIC");
    rename("SATEMP.DIC", "SA.DIC");
    }

void	fun6200() /* Special function definitions */
    {
    ZNEW = X * X + Y * Y; /* Default 3rd and 4th dimension */
    WNEW = (N - 100) / 900;
    if (N > 1000) WNEW = (N - 1000) / (NMAX - 1000);
	if (ODE == 2) 
	    {
	    M = 10;
	    XNEW = A[1] + A[2] * X + A[3] * Y + A[4] * fabs(X) + A[5] * fabs(Y);
	    YNEW = A[6] + A[7] * X + A[8] * Y + A[9] * fabs(X) + A[10] * fabs(Y);
	    }
    if (ODE == 3) 
	{
	M = 14;
	XNEW = A[1] + A[2] * X + A[3] * Y + ((int)(A[4] * X + .5) & (int)(A[5] * Y + .5)) + ((int)(A[6] * X + .5) | (int)(A[7] * Y + .5));
	YNEW = A[8] + A[9] * X + A[10] * Y + ((int)(A[11] * X + .5) & (int)(A[12] * Y + .5)) + ((int)(A[13] * X + .5) | (int)(A[14] * Y + .5));
	}
    if (ODE == 4) 
	{
	M = 14;
	XNEW = A[1] + A[2] * X + A[3] * Y + A[4] * pow(fabs(X), A[5]) + A[6] * pow(fabs(Y), A[7]);
	YNEW = A[8] + A[9] * X + A[10] * Y + A[11] * pow(fabs(X), A[12]) + A[13] * pow(fabs(Y), A[14]);
	}
    if (ODE == 5) 
	{
	M = 18;
	XNEW = A[1] + A[2] * X + A[3] * Y + A[4] * sin(A[5] * X + A[6]) + A[7] * sin(A[8] * Y + A[9]);
	YNEW = A[10] + A[11] * X + A[12] * Y + A[13] * sin(A[14] * X + A[15]) + A[16] * sin(A[17] * Y + A[18]);
	}
    if (ODE == 6) 
	{
	M = 6;
	if (N < 2) 
	    {
	    AL = TWOPI / (13 + 10 * A[6]);
	    SINAL = sin(AL);
	    COSAL = cos(AL);
	    }
	DUM = X + A[2] * sin(A[3] * Y + A[4]);
	XNEW = 10 * A[1] + DUM * COSAL + Y * SINAL;
	YNEW = 10 * A[5] - DUM * SINAL + Y * COSAL;
	}
    if (ODE == 7) 
	{
	M = 9;
	XNEW = X + EPS * A[1] * Y;
	YNEW = Y + EPS * (A[2] * X + A[3] * X * X * X + A[4] * X * X * Y + A[5] * X
	* Y * Y + A[6] * Y + A[7] * Y * Y * Y + A[8] * sin(Z));
	ZNEW = Z + EPS * (A[9] + 1.3);
	if (ZNEW > TWOPI) 
	    ZNEW = ZNEW - TWOPI;
	}
    }

void	fun6700() /* Project onto a horizontal cylinder */
    {
    PH = PT * (YMAX - YP);
    YP = YA + 0.5 * (YH - YL) * cos(PH);
    }

void	fun6800() /* Project onto a vertical cylinder */
    {
    TH = TT * (XMAX - XP);
    XP = XA + 0.5 * (XH - XL) * cos(TH);
    }

void	fun6900() /* Project onto a torus (unity aspect ratio) */
    {
    TH = TT * (XMAX - XP);
    PH = 2 * PT * (YMAX - YP);
    XP = XA + 0.18 * (XH - XL) * (1 + cos(TH)) * sin(PH);
    YP = YA + 0.25 * (YH - YL) * (1 + cos(TH)) * cos(PH);
    }

void	fun7000() /* Save data */
    {
    if ((int)N == 1000) 
	{
	fclose(F3);
	F3 = fopen(FAV, "w");
	}
    if (SAV == 1) 
	DUM = XNEW;
    if (SAV == 2) 
	DUM = YNEW;
    if (SAV == 3) 
	DUM = ZNEW;
    if (SAV == 4) 
	DUM = WNEW;
    fprintf(F3, "%f\n", DUM);
    }


