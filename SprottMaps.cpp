/*
   SPROTTMAPS.CPP a module to explore Sprott Fractal Maps

   Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include	<stdio.h>
#include	<time.h>
#include	"manp.h"
#include	"Fract.h"
#include	"resource.h"
#include	"fractype.h"
#include	"fractalp.h"
#include	"menu.h"
#include	"anim.h"
#include	"OscProcess.h"

#define		sign(x)		(((x)>0)?1:((x)<0)?-1:0)
#define		MAXREAL		1E+10

extern	double	param[];
extern	int	curpass, totpasses;
extern	double	iterations;
extern	long	threshold;
extern	int	OscillatorNum;
extern	int	OscPtrArray[];			// array of pointers to specific oscillators or fractal maps

extern	HWND	GlobalHwnd;			// This is the main windows handle
extern	int	subtype;

void	InitFractalMap(double c1[], int dimensions);
int	DisplayFractalMap(double c1[], DWORD colour, double i, int dimensions, double ColourFactor);

extern	int	user_data(HWND);
extern	void	PlotExtras(void);
extern	void	PrintOsc(int);			// used for listing oscillator names in d:\temp\OscDump.txt

/**************************************************************************
	Sprott Type E FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapE(void)

    {
    double	i, c1[3], a[48], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 46)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
	c1[0] = a[0] + a[1] * oldx + a[2] * oldx * oldx + a[3] * oldx * oldy + a[4]  * oldy + a[5]  * oldy * oldy;
	c1[1] = a[6] + a[7] * oldx + a[8] * oldx * oldx + a[9] * oldx * oldy + a[10] * oldy + a[11] * oldy * oldy;
	c1[2] = sqrt(sqr(sin(oldx*1.5)) + sqr(cos(oldy*1.4)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type F FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapF(void)

    {
    double	i, c1[3], a[48], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 46)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
	c1[0] = a[0]  + a[1]  * oldx + a[2]  * oldx * oldx + a[3]  * oldx * oldx * oldx + a[4]  * oldx * oldx * oldy + a[5]  * oldx * oldy + a[6]  * oldx * oldy * oldy + a[7]  * oldy + a[8]  * oldy * oldy + a[9]  * oldy * oldy * oldy;
	c1[1] = a[10] + a[11] * oldx + a[12] * oldx * oldx + a[13] * oldx * oldx * oldx + a[14] * oldx * oldx * oldy + a[15] * oldx * oldy + a[16] * oldx * oldy * oldy + a[17] * oldy + a[18] * oldy * oldy + a[19] * oldy * oldy * oldy;
	c1[2] = sqrt(sqr(sin(oldx*1.5)) + sqr(cos(oldy*1.4)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type G FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/


int	DoSprottFractaLMapG(void)

    {
    double	i, c1[3], a[48], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 46)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
	c1[0] = a[0] + a[1] * oldx + a[2] * oldx * oldx + a[3] * oldx * oldx * oldx + a[4] * oldx * oldx * oldx * oldx + a[5] * oldx * oldx * oldx * oldy + a[6] * oldx * oldx * oldy + a[7] * oldx * oldx * oldy * oldy
	    + a[8] * oldx * oldy + a[9] * oldx * oldy * oldy + a[10] * oldx * oldy * oldy * oldy + a[11] * oldy + a[12] * oldy * oldy + a[13] * oldy * oldy * oldy + a[14] * oldy * oldy * oldy * oldy;
	c1[1] = a[15] + a[16] * oldx + a[17] * oldx * oldx + a[18] * oldx * oldx * oldx + a[19] * oldx * oldx * oldx * oldx + a[20] * oldx * oldx * oldx * oldy + a[21] * oldx * oldx * oldy + a[22] * oldx * oldx * oldy * oldy
	    + a[23] * oldx * oldy + a[24] * oldx * oldy * oldy + a[25] * oldx * oldy * oldy * oldy + a[26] * oldy + a[27] * oldy * oldy + a[28] * oldy * oldy * oldy + a[29] * oldy * oldy * oldy * oldy;
	c1[2] = sqrt(sqr(sin(oldx*1.5)) + sqr(cos(oldy*1.4)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type H FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/


int	DoSprottFractaLMapH(void)

    {
    double	i, c1[3], a[64], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 46)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
	c1[0] = a[0] + a[1] * oldx + a[2] * oldx * oldx + a[3] * oldx * oldx * oldx + a[4] * oldx * oldx * oldx * oldx + a[5] * oldx * oldx * oldx * oldx * oldx + a[6] * oldx * oldx * oldx * oldx * oldy + a[7] * oldx * oldx * oldx * oldy 
	    + a[8] * oldx * oldx * oldx * oldy * oldy + a[9] * oldx * oldx * oldy + a[10] * oldx * oldx * oldy * oldy + a[11] * oldx * oldx * oldy * oldy * oldy + a[12] * oldx * oldy + a[13] * oldx * oldy * oldy 
	    + a[14] * oldx * oldy * oldy * oldy + a[15] * oldx * oldy * oldy * oldy * oldy + a[16] * oldy + a[17] * oldy * oldy + a[18] * oldy * oldy * oldy + a[19] * oldy * oldy * oldy * oldy + a[20] * oldy * oldy * oldy * oldy * oldy;
	c1[1] = a[21] + a[22] * oldx + a[23] * oldx * oldx + a[24] * oldx * oldx * oldx + a[25] * oldx * oldx * oldx * oldx + a[26] * oldx * oldx * oldx * oldx * oldx + a[27] * oldx * oldx * oldx * oldx * oldy + a[28] * oldx * oldx * oldx * oldy
	    + a[29] * oldx * oldx * oldx * oldy * oldy + a[30] * oldx * oldx * oldy + a[31] * oldx * oldx * oldy * oldy + a[32] * oldx * oldx * oldy * oldy * oldy + a[33] * oldx * oldy + a[34] * oldx * oldy * oldy
	    + a[35] * oldx * oldy * oldy * oldy + a[36] * oldx * oldy * oldy * oldy * oldy + a[37] * oldy + a[38] * oldy * oldy + a[39] * oldy * oldy * oldy + a[40] * oldy * oldy * oldy * oldy + a[41] * oldy * oldy * oldy * oldy * oldy;
	c1[2] = sqrt(sqr(sin(oldx*1.5)) + sqr(cos(oldy*1.4)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type I FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapI(void)

    {
    double	i, c1[3], a[48], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 46)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
	c1[0] = a[0]  + a[1]  * oldx + a[2]  * oldx * oldx + a[3]  * oldx * oldy + a[4]  * oldx * oldz + a[5]  * oldy + a[6]  * oldy * oldy + a[7]  * oldy * oldz + a[8]  * oldz + a[9]  * oldz * oldz;
	c1[1] = a[10] + a[11] * oldx + a[12] * oldx * oldx + a[13] * oldx * oldy + a[14] * oldx * oldz + a[15] * oldy + a[16] * oldy * oldy + a[17] * oldy * oldz + a[18] * oldz + a[19] * oldz * oldz;
	c1[2] = a[20] + a[21] * oldx + a[22] * oldx * oldx + a[23] * oldx * oldy + a[24] * oldx * oldz + a[25] * oldy + a[26] * oldy * oldy + a[27] * oldy * oldz + a[28] * oldz + a[29] * oldz * oldz;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type J FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapJ(void)

    {
    double	i, c1[3], a[120], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	X = a1 + a2X + a3X2 + a4X3 + a5X2Y + a6X2Z + a7XY + a8XY2 + a9XYZ + a10XZ + a11XZ2 + a12Y + a13Y2 + a14Y3 + a15Y2Z + a16YZ + a17YZ2 + a18Z + a19Z2 + a20Z3
//	Y = a21 + a22X + a23X2 + a24X3 + a25X2Y + a26X2Z + a27XY + a28XY2 + a29XYZ + a30XZ + a31XZ2 + a32Y + a33Y2 + a34Y3 + a35Y2Z + a36YZ + a37YZ2 + a38Z + a39Z2 + a40Z3
//	Z = a41 + a42X + a43X2 + a44X3 + a45X2Y + a46X2Z + a47XY + a48XY2 + a49XYZ + a50XZ + a51XZ2 + a52Y + a53Y2 + a54Y3 + a55Y2Z + a56YZ + a57YZ2 + a58Z + a59Z2 + a60Z3

	c1[0] = a[0] + a[1] * oldx + a[2] * oldx * oldx + a[3] * oldx * oldx * oldx + a[4] * oldx * oldx * oldy + a[5] * oldx * oldx * oldz + a[6] * oldx * oldy + a[7] * oldx * oldy * oldy + a[8] * oldx * oldy * oldz + a[9] * oldx * oldz
	    + a[10] * oldx * oldz * oldz + a[11] * oldy + a[12] * oldy * oldy + a[13] * oldy * oldy * oldy + a[14] * oldy * oldy * oldz + a[15] * oldy * oldz + a[16] * oldy * oldz * oldz + a[17] * oldz + a[18] * oldz * oldz + a[19] * oldz * oldz * oldz;
	c1[1] = a[20] + a[21] * oldx + a[22] * oldx * oldx + a[23] * oldx * oldx * oldx + a[24] * oldx * oldx * oldy + a[25] * oldx * oldx * oldz + a[26] * oldx * oldy + a[27] * oldx * oldy * oldy + a[28] * oldx * oldy * oldz + a[29] * oldx * oldz
	    + a[30] * oldx * oldz * oldz + a[31] * oldy + a[32] * oldy * oldy + a[33] * oldy * oldy * oldy + a[34] * oldy * oldy * oldz + a[35] * oldy * oldz + a[36] * oldy * oldz * oldz + a[37] * oldz + a[38] * oldz * oldz + a[39] * oldz * oldz * oldz;
	c1[2] = a[40] + a[41] * oldx + a[42] * oldx * oldx + a[43] * oldx * oldx * oldx + a[44] * oldx * oldx * oldy + a[45] * oldx * oldx * oldz + a[46] * oldx * oldy + a[47] * oldx * oldy * oldy + a[48] * oldx * oldy * oldz + a[49] * oldx * oldz
	    + a[50] * oldx * oldz * oldz + a[51] * oldy + a[52] * oldy * oldy + a[53] * oldy * oldy * oldy + a[54] * oldy * oldy * oldz + a[55] * oldy * oldz + a[56] * oldy * oldz * oldz + a[57] * oldz + a[58] * oldz * oldz + a[59] * oldz * oldz * oldz;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type M FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapM(void)

    {
    double	i, c1[4], a[120], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
    c1[3] = param[13]; // w

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 4);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	Case M : D = 4, O = 2, M = 60
//	    X = a1  + a2X  + a3X2  + a4XY  + a5XZ  + a6XW  + a7Y +  a8Y2  + a9YZ  + a10YW + a11Z + a12Z2 + a13ZW + a14W + a15W2
//	    Y = a16 + a17X + a18X2 + a19XY + a20XZ + a21XW + a22Y + a23Y2 + a24YZ + a25YW + a26Z + a27Z2 + a28ZW + a29W + a30W2
//	    Z = a31 + a32X + a33X2 + a34XY + a35XZ + a36XW + a37Y + a38Y2 + a39YZ + a40YW + a41Z + a42Z2 + a43ZW + a44W + a45W2
//	    W = a46 + a47X + a48X2 + a49XY + a50XZ + a51XW + a52Y + a53Y2 + a54YZ + a55YW + a56Z + a57Z2 + a58ZW + a59W + a60W2
	c1[0] = a[0]  + a[1]  * oldx + a[2]  * oldx * oldx + a[3]  * oldx * oldy + a[4]  * oldx * oldz + a[5]  * oldx * oldw + a[6]  * oldy + a[7]  * oldy * oldy + a[8]  * oldy * oldz + a[9]  * oldy * oldw + a[10] * oldz + a[11] * oldz * oldz + a[12] * oldz * oldw + a[13] * oldw + a[14] * oldw * oldw;
	c1[1] = a[15] + a[16] * oldx + a[17] * oldx * oldx + a[18] * oldx * oldy + a[19] * oldx * oldz + a[20] * oldx * oldw + a[21] * oldy + a[22] * oldy * oldy + a[23] * oldy * oldz + a[24] * oldy * oldw + a[25] * oldz + a[26] * oldz * oldz + a[27] * oldz * oldw + a[28] * oldw + a[29] * oldw * oldw;
	c1[2] = a[30] + a[31] * oldx + a[32] * oldx * oldx + a[33] * oldx * oldy + a[34] * oldx * oldz + a[35] * oldx * oldw + a[36] * oldy + a[37] * oldy * oldy + a[38] * oldy * oldz + a[39] * oldy * oldw + a[40] * oldz + a[41] * oldz * oldz + a[42] * oldz * oldw + a[43] * oldw + a[44] * oldw * oldw;
	c1[3] = a[45] + a[46] * oldx + a[47] * oldx * oldx + a[48] * oldx * oldy + a[49] * oldx * oldz + a[50] * oldx * oldw + a[51] * oldy + a[52] * oldy * oldy + a[53] * oldy * oldz + a[54] * oldy * oldw + a[55] * oldz + a[56] * oldz * oldz + a[57] * oldz * oldw + a[58] * oldw + a[59] * oldw * oldw;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 4, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type Q FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapQ(void)

    {
    double	i, c1[3], a[48], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 46)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
	c1[0] += 0.1 * (a[0]  + a[1]  * oldx + a[2]  * oldx * oldx + a[3]  * oldx * oldy + a[4]  * oldx * oldz + a[5]  * oldy + a[6]  * oldy * oldy + a[7]  * oldy * oldz + a[8]  * oldz + a[9]  * oldz * oldz);
	c1[1] += 0.1 * (a[10] + a[11] * oldx + a[12] * oldx * oldx + a[13] * oldx * oldy + a[14] * oldx * oldz + a[15] * oldy + a[16] * oldy * oldy + a[17] * oldy * oldz + a[18] * oldz + a[19] * oldz * oldz);
	c1[2] += 0.1 * (a[20] + a[21] * oldx + a[22] * oldx * oldx + a[23] * oldx * oldy + a[24] * oldx * oldz + a[25] * oldy + a[26] * oldy * oldy + a[27] * oldy * oldz + a[28] * oldz + a[29] * oldz * oldz);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type R FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapR(void)

    {
    double	i, c1[3], a[120], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	X = a1 + a2X + a3X2 + a4X3 + a5X2Y + a6X2Z + a7XY + a8XY2 + a9XYZ + a10XZ + a11XZ2 + a12Y + a13Y2 + a14Y3 + a15Y2Z + a16YZ + a17YZ2 + a18Z + a19Z2 + a20Z3
//	Y = a21 + a22X + a23X2 + a24X3 + a25X2Y + a26X2Z + a27XY + a28XY2 + a29XYZ + a30XZ + a31XZ2 + a32Y + a33Y2 + a34Y3 + a35Y2Z + a36YZ + a37YZ2 + a38Z + a39Z2 + a40Z3
//	Z = a41 + a42X + a43X2 + a44X3 + a45X2Y + a46X2Z + a47XY + a48XY2 + a49XYZ + a50XZ + a51XZ2 + a52Y + a53Y2 + a54Y3 + a55Y2Z + a56YZ + a57YZ2 + a58Z + a59Z2 + a60Z3

	c1[0] += 0.1 * (a[0] + a[1] * oldx + a[2] * oldx * oldx + a[3] * oldx * oldx * oldx + a[4] * oldx * oldx * oldy + a[5] * oldx * oldx * oldz + a[6] * oldx * oldy + a[7] * oldx * oldy * oldy + a[8] * oldx * oldy * oldz + a[9] * oldx * oldz
	    + a[10] * oldx * oldz * oldz + a[11] * oldy + a[12] * oldy * oldy + a[13] * oldy * oldy * oldy + a[14] * oldy * oldy * oldz + a[15] * oldy * oldz + a[16] * oldy * oldz * oldz + a[17] * oldz + a[18] * oldz * oldz + a[19] * oldz * oldz * oldz);
	c1[1] += 0.1 * (a[20] + a[21] * oldx + a[22] * oldx * oldx + a[23] * oldx * oldx * oldx + a[24] * oldx * oldx * oldy + a[25] * oldx * oldx * oldz + a[26] * oldx * oldy + a[27] * oldx * oldy * oldy + a[28] * oldx * oldy * oldz + a[29] * oldx * oldz
	    + a[30] * oldx * oldz * oldz + a[31] * oldy + a[32] * oldy * oldy + a[33] * oldy * oldy * oldy + a[34] * oldy * oldy * oldz + a[35] * oldy * oldz + a[36] * oldy * oldz * oldz + a[37] * oldz + a[38] * oldz * oldz + a[39] * oldz * oldz * oldz);
	c1[2] += 0.1 * (a[40] + a[41] * oldx + a[42] * oldx * oldx + a[43] * oldx * oldx * oldx + a[44] * oldx * oldx * oldy + a[45] * oldx * oldx * oldz + a[46] * oldx * oldy + a[47] * oldx * oldy * oldy + a[48] * oldx * oldy * oldz + a[49] * oldx * oldz
	    + a[50] * oldx * oldz * oldz + a[51] * oldy + a[52] * oldy * oldy + a[53] * oldy * oldy * oldy + a[54] * oldy * oldy * oldz + a[55] * oldy * oldz + a[56] * oldy * oldz * oldz + a[57] * oldz + a[58] * oldz * oldz + a[59] * oldz * oldz * oldz);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type U FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapU(void)

    {
    double	i, c1[4], a[120], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
    c1[3] = param[13]; // w

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 4);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	Case M : D = 4, O = 2, M = 60
//	    X = a1  + a2X  + a3X2  + a4XY  + a5XZ  + a6XW  + a7Y +  a8Y2  + a9YZ  + a10YW + a11Z + a12Z2 + a13ZW + a14W + a15W2
//	    Y = a16 + a17X + a18X2 + a19XY + a20XZ + a21XW + a22Y + a23Y2 + a24YZ + a25YW + a26Z + a27Z2 + a28ZW + a29W + a30W2
//	    Z = a31 + a32X + a33X2 + a34XY + a35XZ + a36XW + a37Y + a38Y2 + a39YZ + a40YW + a41Z + a42Z2 + a43ZW + a44W + a45W2
//	    W = a46 + a47X + a48X2 + a49XY + a50XZ + a51XW + a52Y + a53Y2 + a54YZ + a55YW + a56Z + a57Z2 + a58ZW + a59W + a60W2
	c1[0] += 0.1 * (a[0] + a[1] * oldx + a[2] * oldx * oldx + a[3] * oldx * oldy + a[4] * oldx * oldz + a[5] * oldx * oldw + a[6] * oldy + a[7] * oldy * oldy + a[8] * oldy * oldz + a[9] * oldy * oldw + a[10] * oldz + a[11] * oldz * oldz + a[12] * oldz * oldw + a[13] * oldw + a[14] * oldw * oldw);
	c1[1] += 0.1 * (a[15] + a[16] * oldx + a[17] * oldx * oldx + a[18] * oldx * oldy + a[19] * oldx * oldz + a[20] * oldx * oldw + a[21] * oldy + a[22] * oldy * oldy + a[23] * oldy * oldz + a[24] * oldy * oldw + a[25] * oldz + a[26] * oldz * oldz + a[27] * oldz * oldw + a[28] * oldw + a[29] * oldw * oldw);
	c1[2] += 0.1 * (a[30] + a[31] * oldx + a[32] * oldx * oldx + a[33] * oldx * oldy + a[34] * oldx * oldz + a[35] * oldx * oldw + a[36] * oldy + a[37] * oldy * oldy + a[38] * oldy * oldz + a[39] * oldy * oldw + a[40] * oldz + a[41] * oldz * oldz + a[42] * oldz * oldw + a[43] * oldw + a[44] * oldw * oldw);
	c1[3] += 0.1 * (a[45] + a[46] * oldx + a[47] * oldx * oldx + a[48] * oldx * oldy + a[49] * oldx * oldz + a[50] * oldx * oldw + a[51] * oldy + a[52] * oldy * oldy + a[53] * oldy * oldz + a[54] * oldy * oldw + a[55] * oldz + a[56] * oldz * oldz + a[57] * oldz * oldw + a[58] * oldw + a[59] * oldw * oldw);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 4, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type Y FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapY(void)

    {
    double	i, c1[3], a[120], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
//    c1[3] = param[13]; // w

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
//	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	X = a1 + a2X + a3Y + a4 | X | +a5 | Y |
//	Y = a6 + a7X + a8Y + a9 | X | +a10 | Y |
//	Z = X2 + Y2
//	W = (N - 1000) / (NMAX - 1000)	
	c1[0] = a[0] + a[1] * oldx + a[2] * oldy + a[3] * fabs(oldx) + a[4] * fabs(oldy);
	c1[1] = a[5] + a[6] * oldx + a[7] * oldy + a[8] * fabs(oldx) + a[9] * fabs(oldy);
	c1[2] = c1[0] * c1[0] + c1[1] * c1[1];
//	if (iterations != 1000.0)
//	    c1[3] = (i - 1000.0) / (iterations - 1000.0);
//	else
//	    c1[3] = 1.0;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type Z FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMapZ(void)

    {
    double	i, c1[3], a[120], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
//    c1[3] = param[13]; // w

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	//	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	X = a1 + a2X + a3Y  + a4X  AND a5Y  + a6X  OR a7Y
//	Y = a8 + a9X + a10Y + a11X AND a12Y + a13X OR a14Y
//	Z = X2 + Y2
//	W = (N - 1000) / (NMAX - 1000)	
	c1[0] = a[0] + a[1] * oldx + a[2] * oldy + (double)((int)(a[3] * oldx) & (int)(a[4] * oldy)) + (double)((int)(a[5] * oldx) | (int)(a[6] * oldy));
	c1[1] = a[7] + a[8] * oldx + a[9] * oldy + (double)((int)(a[10] * oldx) & (int)(a[11] * oldy)) + (double)((int)(a[12] * oldx) | (int)(a[13] * oldy));
	c1[2] = c1[0] * c1[0] + c1[1] * c1[1];
	//	if (iterations != 1000.0)
	//	    c1[3] = (i - 1000.0) / (iterations - 1000.0);
	//	else
	//	    c1[3] = 1.0;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type [ FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMap1(void)

    {
    double	i, c1[3], a[120], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
//    c1[3] = param[13]; // w

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	//	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	X = a1 + a2X + a3Y  + a4X  AND a5Y  + a6X  OR a7Y
//	Y = a8 + a9X + a10Y + a11X AND a12Y + a13X OR a14Y
//	Z = X2 + Y2
//	W = (N - 1000) / (NMAX - 1000)	
	c1[0] = a[0] + a[1] * oldx + a[2] * oldy + a[3] * pow(fabs(oldx), a[4]) + a[5] * pow(fabs(oldy), a[6]);
	c1[1] = a[7] + a[8] * oldx + a[9] * oldy + a[10] * pow(fabs(oldx), a[11]) + a[12] * pow(fabs(oldy), a[13]);
	c1[2] = c1[0] * c1[0] + c1[1] * c1[1];
	//	if (iterations != 1000.0)
	//	    c1[3] = (i - 1000.0) / (iterations - 1000.0);
	//	else
	//	    c1[3] = 1.0;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type \ FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMap2(void)

    {
    double	i, c1[3], a[120], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
//    c1[3] = param[13]; // w

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	//	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	X = a1 + a2X + a3Y + a4sin(a5X + a6) + a7sin(a8Y + a9)
//	Y = a10 + a11X + a12Y + a13sin(a14X + a15) + a16sin(a17Y + a18)
//	Z = X2 + Y2
//	W = (N - 1000) / (NMAX - 1000)	
	c1[0] = a[0] + a[1] * oldx + a[2] * oldy + a[3] * sin(a[4] * oldx + a[5]) + a[6] * sin(a[7] * oldy + a[8]);
	c1[1] = a[9] + a[10] * oldx + a[11] * oldy + a[12] * sin(a[13] * oldx + a[14]) + a[15] * sin(a[16] * oldy + a[17]);
	c1[2] = c1[0] * c1[0] + c1[1] * c1[1];
	//	if (iterations != 1000.0)
	//	    c1[3] = (i - 1000.0) / (iterations - 1000.0);
	//	else
	//	    c1[3] = 1.0;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

#ifdef ALLOWLASTSPROTT

/**************************************************************************
	Sprott Type ] FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMap3(void)

    {
    double	i, c1[3], a[120], ColourFactor;
    char	s[120];
    int		j, length;
    double	al, sinal, cosal, dum;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
//    c1[3] = param[13]; // w

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    al = TWO_PI / (13.0 + 10.0 * a[5]);
    sinal = sin(al);
    cosal = cos(al);
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	//	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	X = 10a1 + [X + a2sin(a3Y+a4)]cos[2p/(13+10a6)] + Y sin[2p/(13+10a6)]
//	Y = 10a5 - [X + a2sin(a3Y + a4)]sin[2p/(13+10a6)] + Y cos[2p/(13+10a6)]
//	Z = X2 + Y2
//	W = (N - 1000) / (NMAX - 1000)
	dum = oldx + a[1] * sin(a[2] * oldy + a[3]);
	c1[0] = 10.0 * a[0] + dum * cosal + oldy * sinal;
	c1[1] = 10.0 * a[4] + dum * sinal + oldy * cosal;
	c1[2] = c1[0] * c1[0] + c1[1] * c1[1];
	//	if (iterations != 1000.0)
	//	    c1[3] = (i - 1000.0) / (iterations - 1000.0);
	//	else
	//	    c1[3] = 1.0;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott Type ^ FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

int	DoSprottFractaLMap4(void)

    {
    double	i, c1[3], a[120], ColourFactor;
    char	s[120];
    int		j, length;
//    double	TWOPI = 6.28318530717959;

    strcpy(s, SprottMapSpecific[subtype].function);
    length = (int)strlen(s);
    if (length > 64)
	return -1;
    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
//    c1[3] = param[13]; // w

    for (j = 0; j < length; j++)
	a[j] = (double)(*(s + j) - 65.0) / 10.0 - 1.2;
    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	//	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	// THESE ARE THE GENERAL EQUATIONS FOR ALL MAPS FROM SPROTT IN HIS BOOK 'STRANGE ATTRACTORS';
//	X = X + 0.1a1Y
//	Y = Y + 0.1(a2X + a3X3 + a4X2Y + a5XY2 + a6Y + a7Y3 + a8sin Z
//	Z =  [Z + 0.1(a9 + 1.3)] mod 2p
//	W = (N - 1000) / (NMAX - 1000)
	c1[0] = oldx + 0.1 * a[0];
	c1[1] = oldy + 0.1 * (a[1] * oldx + a[2] * oldx * oldx * oldx + a[3] * oldx * oldx * oldy + a[4] * oldx * oldy * oldy + a[5] + oldy + a[6] * oldy * oldy * oldy + a[7] * sin(oldz));
	c1[2] = fmod(oldz + 0.1 * (a[8] + 1.3), TWO_PI);
	//	if (iterations != 1000.0)
	//	    c1[3] = (i - 1000.0) / (iterations - 1000.0);
	//	else
	//	    c1[3] = 1.0;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }





/*
Case ^: D = 4, M = 9
X = X + 0.1a1Y
Y = Y + 0.1(a2X + a3X3 + a4X2Y + a5XY2 + a6Y + a7Y3 + a8sin Z
Z = [Z + 0.1(a9 + 1.3)] mod 2p
*/
#endif

/**************************************************************************
	Compare function for Sprott Map name sort
**************************************************************************/

int	CompareSprottMap(int	*p1, int *p2)

    {
    char    *a1, *a2;

    a1 = SprottMapSpecific[*p1].name;
    a2 = SprottMapSpecific[*p2].name;
    if (*a1 == '\"')			    // ignore " character
	a1 = a1++;
    if (*a2 == '\"')
	a2 = a2++;
    return (strcmp(a1, a2));
//    return (_stricmp(a1, a2));	// note that we don't want to do case insensitive compare because we have charecters like: [ \ ^ ] etc, as attractor names
    }

/**************************************************************************
	General Sprott Map
***************************************************************************/

int	setup_SprottMaps(void)
    {
    int	    i;

    OscillatorNum = 0;
    // we'd better count how many records we have
    while (SprottMapSpecific[OscillatorNum].name)
	OscillatorNum++;
    for (i = 0; i < OscillatorNum; i++)
	OscPtrArray[i] = i;
    qsort((void *)OscPtrArray, (size_t)OscillatorNum, sizeof(OscPtrArray[0]), (int(__cdecl *)(const void *, const void *)) CompareSprottMap);
#ifdef	PRINTOSC
    PrintOsc(OscillatorNum);		// used for listing oscillator names in d:\temp\OscDump.txt
#endif
    return 0;
    }

int	DoSprottMaps(void)

    {
    return (SprottMapSpecific[subtype].calctype());
    }


