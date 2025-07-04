/*
   FRACTALMAPS.CPP a module to explore Fractal Maps

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

BOOL	FractalMapColouring = TRUE;

extern	double	param[];
extern	int	curpass, totpasses;
extern	double	iterations;
extern	long	threshold;
extern	int	OscillatorNum;
extern	int	OscPtrArray[];			// array of pointers to specific oscillators or fractal maps
extern	double	dt;				// delta time
extern	double	mandel_width;			// width of display
extern	double	ScreenRatio;			// ratio of width / height for the screen
extern	WORD	type;				// fractal type
extern	int	subtype;
extern	double	hor;				// horizontal address 
extern	double	vert;				// vertical address 
extern	double	VertBias;			// allow vertical stretching of the image
extern	double	zBias;				// allow stretching of the image in the z direction
extern	int	CoordSystem;
extern	double	x_rot;				// angle display plane to x axis
extern	double	y_rot;				// angle display plane to y axis
extern	double	z_rot;				// angle display plane to z axis
extern	ProcessType	OscAnimProc;
extern	RGBTRIPLE	OscBackGround;
extern	BYTE	PerspectiveFlag;		// display using perspective
extern	int	xAxis, yAxis, zAxis;		// numerical values for axes for chaotic oscillators
extern	BOOL	RemoveHiddenPixels;
extern	int	xdots, ydots;

extern	double 	*wpixels;			// an array of doubles holding slope modified iteration counts

extern	CTrueCol	    TrueCol;		// palette info
extern	COscProcess	OscProcess;
extern	CDib	Dib;				// Device Independent Bitmap

extern	HWND	GlobalHwnd;			// This is the main windows handle

void	InitFractalMap(double c1[], int dimensions);
int	DisplayFractalMap(double c1[], DWORD colour, double i, int dimensions, double ColourFactor);

extern	int	user_data(HWND);
extern	void	PlotExtras(void); 
extern	void	PrintOsc(int);			// used for listing oscillator names in d:\temp\OscDump.txt

/**************************************************************************
	2D Circular Chaotic Fractal Map Images
***************************************************************************/

int	Do2DCircular(void)

    {
    double	i, c1[3], xn, yn, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i += dt)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	xn = oldy;
	yn = (1 - (oldx*oldx) - (oldy*oldy))*oldy - oldx;
	c1[0] += xn * dt;
	c1[1] += yn * dt;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	2D-MAP-Modelo - Experimental
	Marcus Rezende november 2020
***************************************************************************/

int	Do2DMAPModeloExperimental(void)

    {
    double	i, c1[3], k, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    k = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx - oldy + k * oldy * cos(1 - oldy);
	c1[1] = sin(oldy + c1[0] + oldx);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	2D Logistic-Sine-Coupling Map for Image Encryption
	Zhongyun Hua, Fan Jin, Binxuan Xu and Hejiao Huang 16 March 2018
	http://www.huazhongyun.cn/wp-content/themes/homepage/Paper_PDF/2D%20Logistic-Sine-Coupling%20Map%20for%20Image%20Encryption.pdf
***************************************************************************/

int	Do2DLogisticSineCouplingMapImageEncryption(void)

    {
    double	i, c1[3], r, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    r = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(PI * (4 * r * oldx * (1 - oldx) + (1 - r) * sin(PI * oldy)));
	c1[1] = sin(PI * (4 * r * oldy * (1 - oldy) + (1 - r) * sin(PI * c1[0])));
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	2D Memristive Hyperchaotic Maps and Application in Secure Communication - model 2D MLM, MTM, 
	Houzhen Li, Zhongyun Hua, Han Bao, Lei Zhu, Mo Chen, Bocheng Bao 2020
	Sent to ManpWIN by Houzhen Li through https://www.researchgate.net/requests/r87275758
***************************************************************************/

#define	MLM	0
#define	MTM	1
#define	SMM	2
#define	MSN	3

int	Do2DMemristiveHyperchaoticMaps(int variety)

    {
    double	i, c1[3], mu, k, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    mu = param[0];
    k = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	switch (variety)
	    {
	    case MLM:
		c1[0] = mu * oldx * (1 - oldx) + k * oldx * cos(oldy);
		break;
	    case MTM:
		c1[0] = (oldx < 0.5) ? mu * oldx + k * oldx * cos(oldy) : mu * (1 - oldx) + k * oldx * cos(oldy);
		break;
	    case SMM:
		c1[0] = mu * oldx + k * oldx * cos(oldy);
		break;
	    case MSN:
		c1[0] = mu * sin(2 * PI * oldx) + k * oldx * cos(oldy);
		break;
	    }
	c1[1] = oldx + oldy;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

int	Do2DMemristiveHyperchaoticMapsMLM(void)
    {
    return	Do2DMemristiveHyperchaoticMaps(MLM);
    }
int	Do2DMemristiveHyperchaoticMapsMTM(void)
    {
    return	Do2DMemristiveHyperchaoticMaps(MTM);
    }
int	Do2DMemristiveHyperchaoticMapsSMM(void)
    {
    return	Do2DMemristiveHyperchaoticMaps(SMM);
    }
int	Do2DMemristiveHyperchaoticMapsMSN(void)
    {
    return	Do2DMemristiveHyperchaoticMaps(MSN);
    }

/**************************************************************************
	2D Logistic-Modulated-Sine-Coupling-Logistic Chaotic Map for Image Encryption
	Hegui Zhu, Yiran Zhao and Yujia Song 21 January 2019
	https://ieeexplore.ieee.org/document/8620223/authors#authors
***************************************************************************/

int	Do2DLogisticModulatedSineCouplingLogisticChaoticMap(void)

    {
    double	i, c1[3], alpha, beta, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    alpha = param[0];
    beta = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = (sin(4.0 * PI * alpha * oldy * (1.0 - oldy)) + beta) * oldx * (1.0 - oldx);
	c1[1] = (sin(4.0 * PI * alpha * c1[0] * (1.0 - c1[0])) + beta) * oldy * (1.0 - oldy);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	2D Rational Chaotic Fractal Map 
***************************************************************************/

int	Do2DRational(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = (-a * oldx) / (1 + (c1[1] * c1[1]));
	c1[1] = oldx + b * c1[1];
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	2D SIMM Map 
***************************************************************************/

int	Do2DSIMMMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * sin(PI * oldy) * sin(b / oldx);
	c1[1] = a * sin(PI * c1[0]) * sin(b / oldy);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	2D Sine Logistic Modulation Map for Image Encryption
	Zhongyun Hua, Yicong Zhou, Chi-Man Pun and C.L. Philip Chen' 15 November 2014
	http://huazhongyun.cn/wp-content/themes/homepage/Paper_PDF/2D%20Sine%20Logistic%20modulation%20map%20for%20image%20encryption.pdf
***************************************************************************/

int	Do2DSineLogisticModulationMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * (sin(PI *oldy) + b) * oldx * (1 - oldx);
	c1[1] = a * a * (sin(PI * c1[0]) + b) * oldy * (1 - oldy);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	A 2D Hyperchaotic Map with Conditional Symmetry and Attractor Growth
	Sixiao Kong, Chunbiao Li, Haibo Jiang, Qiang Lai and Xiaowei Jiang 15 April 2021
	Sent to ManpWIN by Chunbiao Li through https://aip.scitation.org/doi/full/10.1063/5.0043458
***************************************************************************/

int	DoA2DHyperchaoticMapConditionalSymmetryAttractorGrowth(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx + a * sin(oldx) * sin(oldy) + c;
	c1[1] = b * oldx + oldy + d;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Analysis of FIPS 140-2 Test and Chaos-Based Pseudorandom Number Generator
	Lequan Min, Tianyu Chen, and Hongyan Zang
	Proceedings, 5th Chaotic Modeling and Simulation International Conference 2012 - 15 June 2012
	Contacting author: lixiangg@sina.com; furmosi@sina.com
***************************************************************************/

int	DoAnalysisFIPS140(void)

    {
    double	i, c1[3], a, k1, k2, k3, k4, k5, k6, k7, m, n, p, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    ColourFactor = param[1];
    totpasses = 10;
    k1 = 1 - a;
    k2 = 1 + a;
    k3 = 2 * a;
    k4 = 1 + a;
    k5 = 3 * a;
    k6 = 1 - a;
    k7 = a;
    m = 0.5;
    n = sqrt(2.0) / 2.0;
    p = sqrt(2.0);

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = -k1 * oldx * oldy - k2 * oldz + k3 * oldx;
	c1[1] = -k4 * oldx + k5 * oldy;
	c1[2] = -k6 * oldy + k7 * oldz;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Anti-Martin: a Variation of the fractal 'MARTIN' formula in Fractint 20
	Fractint 20 2020
	DOSBox SVN, CPU speed: 3000 cycles, Frameskip 0, Program: FRACTINT (archive.org)
***************************************************************************/

int	DoAntiMartinMap(void)

    {
    double	i, c1[3], a, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldy - cos(oldx);
	c1[1] = a - oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A Color Image Encryption Algorithm based on Hyperchaotic Map and Rubik's Cube-Modified Formula
	Haojie Qiu, Xuemei Xu, Zhaohui Jiang, Kehui Sun and Chuwen Xiao 20 July 2022
	https://doi.org/10.1007/s11071-022-07756-1 - Received directly from Kahui Sun
***************************************************************************/

int	DoAColorImageEncryptionAlgorithmMap(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = cos(a * acos(sin(oldx + oldy)));			// changed the sign.The original was Cos(a* _Acos(Sin(x(1) - x(2))))
	c1[1] = b * sin(PI * (oldx + oldy));
	c1[2] = c * sqrt(sqr(sin(oldx)) + d * sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A Higher Dimensional Chaotic Map with Discrete Memristor - 3D-model 2
	Yuexi Peng, Shaobo He and Kehui Sun 21 November 2020
	Sent to ManpWIN by Kehui Sun although missing a lot of parameters such as 'w', 'b'and 'k' that after a lot of trial I got something
***************************************************************************/

int	DoAHigherDimensionalChaoticMap2(void)

    {
    double	i, c1[3], a, c, w, k, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    c = param[1];
    w = param[2];
    k = param[3];
    b = param[4];
    ColourFactor = param[5];
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
	c1[0] = a * sin(w * oldy) * sin(c / (oldx * (a - b * oldz)));
	c1[1] = a * sin(w * c1[0]) * sin(c / oldx);
	c1[2] = oldz + k * c1[0];
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A Higher Dimensional Chaotic Map with Discrete Memristor - 3D-model 3
	Yuexi Peng, Shaobo He and Kehui Sun 21 November 2020
	Sent to ManpWIN by Kehui Sun although missing a lot of parameters such as 'w', 'b'and 'k' that after a lot of trial I got something
***************************************************************************/

int	DoAHigherDimensionalChaoticMap3(void)

    {
    double	i, c1[3], a, c, w, k, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    c = param[1];
    w = param[2];
    k = param[3];
    b = param[4];
    ColourFactor = param[5];
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
	c1[0] = a * sin(w * oldy) * sin(c / oldx);
	c1[1] = a * sin(w * c1[0]) * sin(c / (oldy * (a - b * oldz)));
	c1[2] = oldz + k * c1[1];
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A Hyperchaotic Map with Grid Sinusoidal Cavity
	Mengyao Yu, Kehui Sun, Wenhao Liu and Shaobo He 20 November 2017
	https://www.sciencedirect.com/science/article/pii/S0960077917304599?via%3Dihub
***************************************************************************/

int	DoAHyperchaoticMap(void)

    {
    double	t, c1[3], a, w, c, h, i, k, s1, s2, r1, r2, r3, ColourFactor;
    int		n;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    n = (int)param[1];
    w = param[2];
    c = param[3];
    k = param[4];
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	h = 0.0;

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	if (n % 2 == 0)
	    {
	    for (i = 1; i <= n; i++)
		{
		s1 = fabs(n / a * oldx + 2 * i - 1) / (n / a * oldx + 2 * i - 1);
		s2 = fabs(n / a * oldx - 2 * i - 1) / (n / a * oldx - 2 * i - 1);
		h = h + s1 + s2;
		}
	    h *= a;
	    }
	else
	    {
	    for (i = 1; i <= n; i++)
		{
		r1 = fabs(n / a * oldx - 2 * i) / (n / a * oldx - 2 * i);
		r2 = fabs(n / a * oldx + 2 * i) / (n / a * oldx + 2 * i);
		h = r1 + r2;
		}
	    r3 = a * fabs(n / a * oldx) / (n / a * oldx);
	    h = a * h + r3;
	    }

	c1[0] = a * sin(w * c1[2]) * sin(c / oldx);
	c1[1] = a * sin(w * c1[0]) * sin(c / oldy);
	c1[2] = a * sin(w * c1[1]) * sin(c / oldz) + h;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	A Lightweight Image Encryption Algorithm Based on Message Passing and Chaotic Map
	Hui Liu, Bo Zhao, Jianwen Zou, Linquan Huang and Yifan Liu 25 Jul 2020
	https://www.hindawi.com/journals/scn/2020/7151836/
***************************************************************************/

int	DoALightweightImageEncryptionAlgorithmMap(void)

    {
    double	t, c1[3], r1, r2, C1, C2, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    r1 = param[0];
    r2 = param[1];
    C1 = param[2];
    C2 = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = r1 * oldx * (1 - oldx) + C1 * oldy * oldy;
	c1[1] = r2 * oldy * (1 - oldy) + C2 * (oldx * oldx + oldx * oldy);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A New Chaotic Attractor from 2D Discrete Mapping Via Border-Collision Period-Doubling Scenario
	ZERAOULIA ELHADJ 12 April 2005
	http://emis.ams.org/journals/HOA/DDNS/2005/3235.pdf
***************************************************************************/

int	DoANewChaoticAttractorDiscreteMapping(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = 1 + a * (fabs(oldx) - oldy * oldy) + oldy;
	c1[1] = b * oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A New Chaotic Map for Secure Transmission
	Hamsa A. Abdullah and Hikmat N. Abdullah June 2018
	https://www.researchgate.net/publication/324603707_A_New_Chaotic_Map_for_Secure_Transmission
***************************************************************************/

int	DoANewChaoticMapSecureTransmission(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = 1 - a * oldx * oldy - oldx * oldx - oldy * oldy;
	c1[1] = oldx;
	c1[2] = oldy - b * oldz;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A New Class of Two-dimensional Chaotic Maps with Closed Curve Fixed Points
	HAIBO JIANG, YANG LIU, ZHOUCHAO WEI and LIPING ZHANG 01 February 2019.
	Received from Research gate
***************************************************************************/

int	DoANewClass2DChaoticMaps(void)

    {
    double	i, c1[3], a, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx + 1.2 * (oldx * oldx + oldy * oldy - 1);
	c1[1] = oldy + a * oldy * (oldx * oldx + oldy * oldy - 1);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A New Class of Three-Dimensional Maps with Hidden Chaotic Dynamics - NF-I1
	Haibo Jiang, Yang Liu, Zhouchao Wei, Liping Zhang July 30, 2016
	https://www.researchgate.net/publication/306108761_A_new_class_of_three-dimensional_maps_with_hidden_chaotic_dynamics
***************************************************************************/

int	DoANewClassThreeDimensionalMaps(int nf)

    {
    double	i, c1[3], cn[3], ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1];
	cn[1] = c1[2];
	switch (nf)
	    {
	    case 1:
		cn[2] = c1[0] - c1[1] * c1[0] + c1[0] * c1[2] + 0.5;
		break;
	    case 2:
		cn[2] = c1[0] + c1[0] * c1[0] + c1[2] * c1[2] - 2 * c1[0] * c1[1] - 0.2;
		break;
	    case 3:
		cn[2] = 1.7 * c1[0] - 0.7 * c1[2] + c1[2] * c1[2] - c1[0] * c1[1] - 0.3;
		break;
	    case 4:
		cn[2] = 1.07 * c1[0] - 0.07 * c1[2] - c1[0] * c1[1] + c1[0] * c1[2] + 0.28;
		break;
	    case 5:
		cn[2] = c1[0] + 0.2 * c1[0] * c1[0] + 0.8 * c1[1] * c1[1] - c1[0] * c1[2] - 0.1;
		break;
	    case 6:
		cn[2] = 0.35 * c1[2] * c1[2] - 0.57 * c1[0] * c1[1] - 1.27;
		break;
	    case 13:
		cn[2] = -c1[2] * c1[2] + 0.7 * c1[0] * c1[1] + c1[0] * c1[2] + 0.7;
		break;
	    case 14:
		cn[2] = -c1[1] - 0.1 * c1[0] * c1[1] + c1[0] * c1[2] + 1.2;
		break;
	    case 16:
		cn[2] = c1[0] + 0.19 * c1[0] * c1[0] - 0.22 * c1[2] * c1[2] + 0.03 * c1[1] * c1[2] + 0.03;
		break;
	    }
	c1[0] = cn[0]; c1[1] = cn[1]; c1[2] = cn[2];
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

int	DoANewClassThreeDimensionalMaps1(void)
    {
    return DoANewClassThreeDimensionalMaps(1);
    }
int	DoANewClassThreeDimensionalMaps2(void)
    {
    return DoANewClassThreeDimensionalMaps(2);
    }
int	DoANewClassThreeDimensionalMaps3(void)
    {
    return DoANewClassThreeDimensionalMaps(3);
    }
int	DoANewClassThreeDimensionalMaps4(void)
    {
    return DoANewClassThreeDimensionalMaps(4);
    }
int	DoANewClassThreeDimensionalMaps5(void)
    {
    return DoANewClassThreeDimensionalMaps(5);
    }
int	DoANewClassThreeDimensionalMaps6(void)
    {
    return DoANewClassThreeDimensionalMaps(6);
    }
int	DoANewClassThreeDimensionalMaps13(void)
    {
    return DoANewClassThreeDimensionalMaps(13);
    }
int	DoANewClassThreeDimensionalMaps14(void)
    {
    return DoANewClassThreeDimensionalMaps(14);
    }
int	DoANewClassThreeDimensionalMaps16(void)
    {
    return DoANewClassThreeDimensionalMaps(16);
    }

/**************************************************************************
	A New Hyperchaotic Map and its Application for Image Encryption
	Hayder Natiq, N.M.G. Al-Saidi, M.R.M. Said and Adem Kilicman 9 January 2018
	Received through Hayder Natiq Kadhim
***************************************************************************/

int	DoANewHyperchaoticMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = 1.0;	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1 / PI / b * sin(1 + a * oldx * oldx - PI * PI * oldy);
	c1[1] = 2 / PI / b * sin(PI * b * oldx);
	c1[2] = sin(oldx) + sin(oldy);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A New Nonlinear Oscillator with Infinite Number of Coexisting Hidden and Self-Excited Attractors - Convoluted
	Yan-Xia Tang, Abdul Jalil M Khalaf, Karthikeyan Rajagopal, Viet-Thanh Pham, Sajad Jafari and Ye Tian 29 March 2018
	https://www.researchgate.net/profile/Abdul_Jalil_M_Khalaf/publication/322509093_A_new_nonlinear_oscillator_with_infinite_number_of_coexisting_hidden_and_self-excited_attractors/links/5ac8d642aca272abdc5f9a26/A-new-nonlinear-oscillator-with-infinite-number-of-coexisting-hidden-and-self-excited-attractors.pdf
***************************************************************************/

int	DoANewNonlinearOscillatorInfiniteNumberMap(void)

    {
    double	i, c1[3], w, azao, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = 1.0;	// z

    w = param[0];
    azao = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldy;
	c1[1] = -sin(0.1 * oldx) + oldy * cos(oldx) + azao * sin(w * i);
	c1[2] = sin(oldx) + sin(oldy);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A New Simple 2D Piecewise Linear Map
	Zeraoulia Elhadj and J. C. Sprott September 7, 2007
***************************************************************************/

int	DoANewSimple2DPiecewiseLinearMap(void)

    {
    double	t, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = 1 - a * fabs(oldy) + b * oldx;
	c1[1] = oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	A Novel Chaos-Based Image Encryption Algorithm using DNA Sequence Operations
	Xiuli Chai, Yiran Chen and Lucie Broyde 18 August 2016
	Sent to ManpWIN by Xiuli Chai
***************************************************************************/

int	DoANovelChaosBasedImageEncryptionAlgorithmMap(void)

    {
    double	t, c1[3], m1, m2, l1, l2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    m1 = param[0];
    m2 = param[1];
    l1 = param[2];
    l2 = param[3];
    ColourFactor = param[4];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = oldx * m1 * (1 - oldx) + l1 * oldy * oldy;
	c1[1] = oldy * m2 * (1 - oldy) + l2 * (oldx * oldx + oldx * oldy);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A Novel Class of Two-Dimensional Chaotic Maps with infinitely Many Coexisting Attractors
	Li-Ping Zhang, Yang Liu, Zhou-ChaoWei, Hai-Bo Jiang and Qin-Sheng Bi 3 April 2020
	ttps://www.researchgate.net/publication/340429058
***************************************************************************/

/*
int	DoANovelClassTwoDimensionalChaoticMaps(void)

    {
    double	t, c1[3], ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    ColourFactor = param[0];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = oldx + 2.7 * oldx * cos(oldy);
	c1[1] = oldx + oldy + 0.1;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }
*/

/**************************************************************************
	A Novel Control Method to Counteract the Dynamical Degradation of a Digital Chaotic Sequence
	Chen Chen, Kehui Suna, Yuexi Peng and Abdulaziz O.A. Alamodi 18 January 2019
	received by the second author
***************************************************************************/

int	DoANovelClassControlMethodCounteractDynamicalDegradation(void)

    {
    double	t, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = a * sin(PI * oldy) * sin(b / oldx);
	c1[1] = a * sin(PI * c1[0]) * sin(b / oldy);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	A Novel Megastable Oscillator MAP with a Strange Structure of Coexisting Attractors: Design, Analysis, and FPGA Implementation
	Kui Zhang, M. D. Vijayakumar, Sajjad Shaukat Jamal, Hayder Natiq, Karthikeyan Rajagopal, Sajad Jafari and Iqtadar Hussain 27 August 2021
	https://www.researchgate.net/publication/354174042
***************************************************************************/

int	DoANovelMegastableOscillatorMAP(void)

    {
    double	t, c1[3], a, w, r, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    w = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	r = sqrt(oldx * oldx + oldy * oldy);
	c1[0] = -0.1 * oldy + oldx * cos(r) / r + a * sin(w * t);
	c1[1] = sin(0.1 * oldx) + oldy * cos(r) / r;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A Secure Image Encryption Scheme based on a Novel 2D Sine.Cosine Cross.Chaotic (SC3) Map
	Bhaskar Mondal, Pratap Kumar Behera and Sugata Gangopadhyay 23 December 2019
' https://www.researchgate.net/publication/338499733
***************************************************************************/

int	DoASecureImageEncryptionSchemeMap(void)

    {
    double	t, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = sin(a / (oldy * oldy * oldy) / 2.0);
	c1[1] = cos(b * acos(oldx));
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	A Two-Dimensional Chaotic Quasi-Conservative Map - Mira - Gumoswski
	Igor Gumowski & Christian Mira early 70s
	http://www.atomosyd.net/spip.php?article98
***************************************************************************/

int	DoA2DimensionalChaoticQuasiConservativeMap(void)

    {
    double	t, c1[3], alpha, beta, mu, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    alpha = param[0];
    mu = param[1];
    beta = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = oldy + mu * oldx + 2 * (1 - mu) * oldx * oldx / (1 + oldx * oldx) + alpha * (1 - beta * oldy * oldy) * oldy;
	c1[1] = -oldx + mu * c1[0] + 2 * (1 - mu) * c1[0] * c1[0] / (1 + c1[0] * c1[0]);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Abundance of Attracting, Repelling and Elliptic Periodic Orbits in Two-Dimensional Reversible Maps - Variation IV
	A. Delshams, S. V. Gonchenko, V. S. Gonchenko, J. T. Lazaro, O. Sten'kin 25 january 2012
	https://arxiv.org/pdf/1201.5357.pdf
***************************************************************************/

int	DoAbundanceAttractingRepellingEllipticPeriodicOrbits(void)

    {
    double	t, c1[3], ee, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    ee = param[0];
    ColourFactor = param[1];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = 1 - 2 * ee * sin(oldx) - ee * sin(oldy);
	c1[1] = 1 - 2 * ee * sin(oldy) - ee * sin(oldx) + ee * sin(oldz);
	c1[2] = 1 - 2 * ee * sin(oldz) - ee * sin(oldy);
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	An Algorithm of Image Encryption Using Logistic and Two-Dimensional Chaotic Economic Maps
	Sameh S. Askar, Abdel A. Karawia, Abdulrahman Al-Khedhairi and Fatemah S. Al-Ammar 9 January 2019
	https://www.researchgate.net/publication/330312314
***************************************************************************/

int	DoAnAlgorithmImageEncryptionUsingLogistic2DimensionalChaoticEconomicMaps(void)

    {
    double	t, c1[3], k, a, b, c, q, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    k = param[0];
    a = param[1];
    b = param[2];
    c = param[3];
    ColourFactor = param[4];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	q = oldx + oldy;
	c1[0] = oldx + k * oldx * oldx * (a - c - b * oldx / q - b * log(q));
	c1[1] = oldy + k * oldy * oldy * (a - c - b * oldy / q - b * log(q));
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	An Amplitude-Controllable 3D Hyperchaotic Map with Homogenous Multistability
	Xuejiao Zhou1, Chunbiao Li1, Yongxin Li, Xu Lu and Tengfei Lei - : goontry@126.com; chunbiaolee@nuist.edu.cn novembro 2020
	https://assets.researchsquare.com/files/rs-411243/v1_stamped.pdf
***************************************************************************/

int	DoAnAmplitudeControllable3DHyperchaoticMap(void)

    {
    double	t, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = oldx + a * oldx * sin(b * oldy);
	c1[1] = oldz;
	c1[2] = c * oldx - oldy - oldz;
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	An Improved Image Encryption Algorithm based on Chaotic System Map
	Shubo Liu, Jing Sun1 and Zhengquan Xu NOVEMBER 2009
	https://www.researchgate.net/publication/220405355
***************************************************************************/

int	DoAnImprovedImageEncryptionAlgorithmMap(void)

    {
    double	t, c1[3], m, n, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    m = param[0];
    n = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = m * oldx * (1 - (oldx));
	c1[1] = n * oldy * (1 - (oldy));
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	An Initial Controlled Double-Scroll Hyperchaotic Map
	Yongxin Li, Chunbiao Li, Sicong Liu, Tengfei Lei and Yicheng Jiang December 21st, 2021
	https://assets.researchsquare.com/files/rs-1177591/v1/b02dd774-2069-488b-b338-e42a11772717.pdf?c=1642485224
***************************************************************************/

int	DoAnInitialControlledDoubleScrollHyperchaoticMap(void)

    {
    double	t, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = -oldx + a * sin(oldx + oldy);
	c1[1] = -oldy + b * sin(oldx);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Application of New 4-D Chaotic Map for Secure IP-Communications
	Belqassim Bouteghrine, CamelTanougast and SaidSadoudi November 2020
	13th Chaotic Modeling and Simulation International Conference
***************************************************************************/

int	DoApplicationNew4DChaoticMap(void)

    {
    double	t, c1[4], a, b, c, d, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z
    c1[3] = param[13];	// w

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	double oldw = c1[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = 1 - a * oldx * oldx + oldy * oldz * oldw;
	c1[1] = 1 - b * oldy * oldy + oldx * oldz * oldw;
	c1[2] = c * oldx *oldy * oldw;
	c1[3] = d * oldx;
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.3.0
	Christian Mira 10 January 2017.
	https://www.researchgate.net/publication/312174305
***************************************************************************/

int	DoAreMathematicsSourceArtisticCreationPropertiesMap430(void)

    {
    double	t, c1[3], a, s, mu, fx, fx2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    mu = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	fx = mu * oldx + 2 * (1 - mu) * oldx * oldx / (1 + oldx * oldx);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + fx;
	fx2 = mu * c1[0] + 2 * (1 - mu) * c1[0] * c1[0] / (1 + c1[0] * c1[0]);
	c1[1] = -oldx + fx2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.4
	Christian Mira 10 January 2017.
	https://www.researchgate.net/publication/312174305
***************************************************************************/

int	DoAreMathematicsSourceArtisticCreationPropertiesMap44(void)

    {
    double	t, c1[3], a, s, mu, fx, fx2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    mu = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	fx = mu * oldx + 2 * (1 - mu) * oldx * oldx / (1 + oldx * oldx * oldx * oldx);
	c1[0] = oldy + a * oldy * (1 - s * (oldy * oldy + 3 * oldx * oldx)) + fx;
	fx2 = mu * c1[0] + 2 * (1 - mu) * c1[0] * c1[0] / (1 + c1[0] * c1[0] * c1[0] * c1[0]);
	c1[1] = -oldx + fx2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.5-1
	Christian Mira 10 January 2017.
	https://www.researchgate.net/publication/312174305
***************************************************************************/

int	DoAreMathematicsSourceArtisticCreationPropertiesMap451(void)

    {
    double	t, c1[3], a, s, mu, fx, fx2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    mu = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	fx = mu * oldx + 2 * (1 - mu) * oldx * oldx * exp(-(oldx * oldx - 1) / 4);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + fx;
	fx2 = mu * c1[0] + 2 * (1 - mu) * c1[0] * c1[0] * exp(-(c1[0] * c1[0] - 1) / 4);
	c1[1] = -oldx + fx2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.5-2
	Christian Mira 10 January 2017.
	https://www.researchgate.net/publication/312174305
***************************************************************************/

int	DoAreMathematicsSourceArtisticCreationPropertiesMap452(void)

    {
    double	t, c1[3], a, s, mu, fx, fx2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    mu = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
//	     mu * x(1) + (1 - mu) * x(1) * x(1) * e ^ (-(x(1) * x(1) - 1) / 4);
	fx = mu * oldx + (1 - mu) * oldx * oldx * exp(-(oldx * oldx - 1) / 4);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + fx;
	fx2 = mu * c1[0] + (1 - mu) * c1[0] * c1[0] * exp(-(c1[0] * c1[0] - 1) / 4);
//	      mu * x(3) + (1 - mu) * x(3) * x(3) * e ^ (-(x(3) * x(3) - 1) / 4);
	c1[1] = -oldx + fx2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Are Mathematics a Source of Artistic Creation … Incursion into Maps Properties - Map 4.5-3
	Christian Mira 10 January 2017.
	https://www.researchgate.net/publication/312174305
***************************************************************************/

int	DoAreMathematicsSourceArtisticCreationPropertiesMap453(void)

    {
    return(DoAreMathematicsSourceArtisticCreationPropertiesMap452());
    }

/**************************************************************************
	Asymmetry Evolvement and Controllability of a Symmetric Hyperchaotic Map
	Sixiao Kong, Chunbiao Li, Haibo Jiang, Yibo Zhao and Yanling Wang 9 June 2021
	https://www.mdpi.com/2073-8994/13/6/1039/htm
***************************************************************************/

int	DoAsymmetryEvolvementControllabilityMap(void)

    {
    double	t, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = sin(a * oldy) - b * sin(oldx) + oldx;
	c1[1] = sin(a * oldx) - b * sin(oldy) + oldy;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Attractor Merging and Amplitude Control of Hyperchaos in a Self-Reproducing Memristive Map
	Yongxin Li, Chunbiao Li, Qing Zhong, Tengfei Lei and Sicong Liu - Apr, 9 2024
	https://www.researchgate.net/publication/379703172....DOI: 10.1142/S0218127424500500
***************************************************************************/

int	DoAttractorMergingAmplitudeControlHyperchaosMap(void)

    {
    double	t, c1[3], a, b, d, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    d = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	d = -d;
	c1[0] = a * oldx * cos(oldy);
	c1[1] = d * fabs(oldx) - b * oldy;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Basin Chaotic fractal map
***************************************************************************/

int	DoBasin(void)

    {
    double	i, c1[3], a, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx * c1[1] + a * oldx - c1[1];
	c1[1] = oldx + c1[1];
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Bifurcations and Chaos in a Three-Dimensional Generalised Hénon Map - I
	Jingjing Zheng, Ziwei Wang, You Li and Jinliang Wang 2018
	https://advancesindifferenceequations.springeropen.com/articles/10.1186/s13662-018-1622-y
***************************************************************************/

int	DoBifurcationsChaos3DHénonMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = a - oldy * oldy + b * oldz;
	c1[1] = oldx;
	c1[2] = oldy;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Bifurcations in the Lozi Map
	V Botella-Soler, J M Castelo, J A Oteo and J Ros 7 jun 2011
	https://arxiv.org/pdf/1102.0034.pdf
***************************************************************************/

int	DoBifurcationsInLoziMap(void)

    {
    double	i, c1[3], a, fa, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	fa = a * fabs(oldx) + (1 - a) * oldx * oldx;
	c1[0] = 1 - 0.8 * fa + oldy;
	c1[1] = -oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Biometric Information Security Based on Double Chaotic Rotating Diffusion
	Zhenlong Man July 2023
	//www.sciencedirect.com/science/article/pii/S0960077923005155.....https://doi.org/10.1016/j.chaos.2023.113614
	Authors contact: manzhenlong@lntu.edu.cn
***************************************************************************/

int	DoBiometricInformationSecurityMap(void)

    {
    double	i, c1[3], t, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    t = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(PI * 4 * t * oldx) + (1 - t) * sin(PI * oldy);
	c1[1] = sin(PI * 4 * t * oldy) + (1 - t) * sin(PI * c1[0]);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Bogdanov Map-Bifurcations, Mode Locking and Chaos in a Dissipative System
	David K. Arrowsmith, Julyan H. E. Cartwright AND Alexis N. Lansbury, 1993
	https://arxiv.org/ftp/chao-dyn/papers/9402/9402006.pdf
**************************************************************************/

int	DoBogdanovMapBifurcations(void)

    {
    double	i, c1[3], k, e, m, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    k = param[0];
    e = param[1];
    m = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[1] = c1[1] + e * c1[1] + k * oldx * (oldx - 1) + m * oldx * oldy;
	c1[0] = oldx + c1[1];
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Bogdanov - Takens Bifurcation
	Bogdanov - 1981
	https://ipfs.io/ipfs/QmXoypizjW3WknFiJnKLwHCnL72vedxjQkDDP1mXWo6uco/wiki/Bogdanov%E2%80%93Takens_bifurcation.html
**************************************************************************/

int	DoBogdanovTakensBifurcationMap(void)

    {
    double	i, c1[3], b1, b2, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    b1 = param[0];
    b2 = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldy;
	c1[1] = b1 + b2 * oldx + oldy * oldy + oldx * oldy;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Boosted Coexisting Chaos in a 2-D Sine Map and Its Hardware Implementation
	Han Bao, Zhongyun Hua, Ning Wang, Lei Zhu, Mo Chen and Bocheng Bao November 18, 2020
	Sent to ManpWIN by Ning Wang through https://www.researchgate.net/requests/r87247005
**************************************************************************/

int	DoBoostedCoexistingChaos(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx + a * sin(oldy);
	c1[1] = oldy + b * sin(oldx) * sin(oldy);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Burgers fractal map Images
***************************************************************************/

int	DoBurgers(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = (1 - a) * oldx - oldy * oldy;
	c1[1] = (1 + b) * oldy + oldx * oldy;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Business Cycle Fractal Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoBusinessCycle(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx + oldy;
	c1[1] = a * oldy - (a + 1) * oldy * oldy * oldy - b * oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Cathala Fractal Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoCathalaMAP(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx + oldy;
	c1[1] = b + oldx * oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaos and Hyperchaos in a Symmetric Coupling of Three Quadratic Maps - I
	Joao C. Xavier and Paulo C. Rech January 25, 2010
	http://epacis.net/jcis/PDF_JCIS/JCIS11-art.24.pdf
***************************************************************************/

int	DoChaosHyperchaosSymmetricCoupling(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = a - oldx * oldx + b * oldy;
	c1[1] = a - oldy * oldy + b * oldz;
	c1[2] = a - oldz * oldz + b * oldx;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaos-Based Engineering Applications with a 6D Memristive Multistable Hyperchaotic System and a 2D SF-SIMM Hyperchaotic Map
	Fei Yu, Shuai Qian, Xi Chen, Yuanyuan Huang, Shuo Cai, Jie Jin and Sichun Du 28 March 2021
	https://www.hindawi.com/journals/complexity/2021/6683284/
***************************************************************************/

int	DoChaosBasedEngineeringApplications(void)

    {
    double	i, c1[3], m, w, n, real, imag, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    m = param[0];
    w = param[1];
    n = param[2];
    real = param[3];
    imag = param[4];
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = m * sin(w * oldy) * sin(n / oldx);
	c1[1] = m * sin(w * c1[0]) * sin(n / oldy);
	c1[2] = sqrt(sqr(real*sin(oldx)) + sqr(imag*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaos, Control, and Synchronisation in Some Fractional-Order Difference Equations
	Amina-Aicha Khennaoui, Adel Ouannas, Samir Bendoukha, Giuseppe Grassi, Xiong Wang, Viet-Thanh Pham and Fawaz E. Alsaadi 2019
	https://www.researchgate.net/publication/336072438
***************************************************************************/

int	DoChaosControlSynchronisation(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = 1 + oldz - a * oldy * oldy;
	c1[1] = 1 + b * oldy - a * oldx * oldx;
	c1[2] = b * oldx;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaos-Hyperchaos Transition Map
	Tomasz Kapitaniak, Yuri Maistrenko and Svitlana Popovych 16 July 1999
	https://www.researchgate.net/publication/12238501_Chaos-hyperchaos_transition
***************************************************************************/

int	DoChaosHyperchaosTransitionMap (void)

    {
    double	i, c1[3], a, epsilon, real, imag, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    epsilon = param[1];
    real = param[2];
    imag = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx * (1 - oldx) + epsilon * (a * oldy * (1 - oldy) - a * oldx * (1 - oldx));
	c1[1] = a * oldy * (1 - oldy) + epsilon * (a * oldx * (1 - oldx) - a * oldy * (1 - oldy));
	c1[2] = sqrt(sqr(real*sin(oldx)) + sqr(imag*cos(oldy)));
	;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaos Synchronisation and Hyperchaos Fractal Map
	Tomasz Kapitaniak 2005
	https://iopscience.iop.org/article/10.1088/1742-6596/23/1/033/pdf
***************************************************************************/

int	DoChaosSynchronisationHyperchaos(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = 1 + oldz - a * oldy * oldy;
	c1[1] = 1 + b * oldy - a * oldx * oldx;
	c1[2] = b * oldx;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaos Synchronisation in Discrete-Time Systems: the F-M Synchronisation - the 3D Hénon Map
	Adel Ouannas, Giuseppe Grassi, Abdulrahman Karouma, Toufik Ziar, Xiong Wang, and Viet–Thanh Pham Jan 31, 2018
	Reference from another paper
***************************************************************************/

int	DoChaosSynchronisation3DHenonMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = -b * oldy;
	c1[1] = oldz + 1 - a * oldy * oldy;
	c1[2] = b * oldy + oldx;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaos Synchronisation in Discrete-Time Systems: the F-M Synchronisation - the Fold Map
	Adel Ouannas, Giuseppe Grassi, Abdulrahman Karouma, Toufik Ziar, Xiong Wang, and Viet–Thanh Pham Jan 31, 2018
	Reference from another paper
***************************************************************************/

int	DoChaosSynchronisationFoldMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = oldy + a * oldx;
	c1[1] = b + oldx * oldx;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaotic Evolution towards Philosophy and Methodology of Chaotic Optimization
	From Determinism and Probability to Chaos
	Yan Pei 12 November 2014
***************************************************************************/

int	DoChaoticEvolutionMap(void)

    {
    double	i, c1[3], a, b, mu, oldx, oldy, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    mu = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = (c1[0] < 0.5 && c1[0] >= 0.0) ? mu * c1[0] : mu * (1.0 - c1[0]);
	oldx = mu * c1[0] * (1.0 - c1[0]);				// here ends the original equations
	oldy = (mu * c1[0] * (1.0 - c1[0]) - c1[1]) / dt;
	c1[0] += oldx * dt;
	c1[1] += oldy * dt;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chaotic Map with No Fixed Points: Entropy, Implementation and Control
	Van Van Huynh, Adel Ouannas, Xiong Wang, Viet-Thanh Pham, Xuan Quynh Nguyen and Fawaz E. Alsaadi 14 March 2019
	https://www.mdpi.com/1099-4300/21/3/279/html
***************************************************************************/

int	DoChaoticMapNoFixedPoints(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx + oldy;
	c1[1] = oldy - a * fabs(oldy) - oldx * oldy + b * oldx * oldx - c * oldy * oldy + d;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chip - taken from Fractint
	Fractint 20 in 2020
	DOSBox SVN, CPU speed: 3000 cycles, Frameskip 0, Program: FRACTINT (archive.org)
***************************************************************************/

int	DoChipMap(void)

    {
    double	i, c1[3], a, b, c, temp, ColourFactor;
    int		MapType;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    MapType = (int)param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	switch (MapType)
	    {
	    case 0:
		temp = oldy - sign(oldx) * cos(sqr(fabs(log(fabs(b * oldx - c))))) * atan(sqr(fabs(log(fabs(c * oldx - b)))));
		break;
	    case 1:
		temp = oldy - sign(oldx) * sin(sqr(fabs(log(fabs(b * oldx - c))))) * atan(sqr(fabs(log(fabs(c * oldx - b)))));
		break;
	    case 2:
		temp = oldy - sign(oldx) * (fabs(sin(oldx) * cos(b) + c - oldx * tan(a + b + c)));
		break;
	    case 3:
		temp = oldy - sign(oldx) * (fabs(cos(oldx) * cos(b) + c - oldx * tan(a + b + c)));
		break;
	    default:
		temp = oldy - sign(oldx) * cos(sqr(fabs(log(fabs(b * oldx - c))))) * atan(sqr(fabs(log(fabs(c * oldx - b)))));
		break;
	    }
	c1[1] = a - oldx;
	c1[0] = temp;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }
/**************************************************************************
	Chirikov Standard Map
	Shaobo He, Kehui Sun and  Huihai Wang 12 February 2019
***************************************************************************/

int	DoChirikovStandardMap(void)

    {
    double	i, c1[3], a, b, k, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    k = param[0];
    a = param[1];
    b = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[1] = oldy + k * sin(oldx);					// MOD 2 * pi  that was an unsuccessful trial, so I removed the MOD operator
	c1[0] = oldx + c1[1];						// MOD 2 * pi
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Chossat-Golubitsky Maps
	18 Mar 2019
	https://github.com/eug/chaotic-maps/commit/619420f0066ecbceade7532576e3359e49d7d0c3
***************************************************************************/

int	DoChossatGolubitskyMaps(void)

    {
    double	i, c1[3], a, b, c, d, aa, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	aa = a * (oldx * oldx + oldy * oldy) + b * oldx * (oldx * oldx - 3 * oldy * oldy) + c;
	c1[0] = aa * oldx + d * (oldx * oldx - oldy * oldy);
	c1[1] = aa * oldy - 2 * d * oldx * oldy;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Classifying and Quantifying Basins of Attraction Map
	J. C. Sprott and Anda Xiong 3 August 2015
	http://sprott.physics.wisc.edu/pubs/paper442.pdf
	the double Hénon is shown in this paper, so Marcus had the idea to change signs at each looping and it worked !!!
***************************************************************************/

int	DoClassifyingQuantifyingBasinsAttraction(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = -c1[0];
	double oldy = -c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx + b * oldy - 0.1 * oldx * oldx * oldx;
	c1[1] = oldx;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Classifying and Quantifying Basins of Attraction - Sine - Sine Map
	J. C. Sprott and Anda Xiong 3 August 2015
	http://sprott.physics.wisc.edu/pubs/paper442.pdf
	the double Hénon is shown in this paper, so Marcus had the idea to change signs at each looping and it worked !!!
***************************************************************************/

int	DoClassifyingQuantifyingBasinsAttractionSineSine(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(oldx) - sin(2 * oldy);
	c1[1] = oldx;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Clifford Chaotic Fractal Map Images
***************************************************************************/

int	DoClifford(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(a*c1[1]) - c * cos(a*oldx);
	c1[1] = sin(b*oldx) - d * cos(b*c1[1]);
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Coexistence of Thread and Sheet Chaotic Attractors for Three-Dimensional Lozi Map
	Rene Lozy 9 June 2023
	https://www.mdpi.com/2673-8716/3/2/19.......https://doi.org/10.3390/dynamics3020019
	Corresponding author: rene.lozi@univ-cotedazur.fr
***************************************************************************/

int	DoCoexistenceThreadSheetChaoticAttractors(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1 - a * fabs(oldx) + oldy;
	c1[1] = b * oldx;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Coexisting Infinite Orbits in an Area-Preserving Lozi Map
	Houzhen Li,Kexin Li, Mo Chen and Bocheng Bao - 3 October 2020
	https://www.mdpi.com/1099-4300/22/10/1119/htm
***************************************************************************/

int	DoCoexistingInfiniteOrbits(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1 - a * fabs(oldx) + oldy;
	c1[1] = b * oldx;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Complex Dynamics and Initial State Effects in a Two-Dimensional Sine-Bounded Memristive Map
	Bocheng Bao, Qianhan Zhao, Xihong Yu, Huagan Wu and Quan Xu 28 June 2023
	https://www.sciencedirect.com/science/article/pii/S0960077923006495?......https://doi.org/10.1016/j.chaos.2023.113748
	Corresponding author: xuquan@cczu.edu.cn
***************************************************************************/

int	DoComplexDynamics2DSineBoundedMemristiveMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(a * oldx / (oldy * oldy - b));
	c1[1] = oldx + oldy;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Complex Dynamics in Generalised Hénon Map
	Meixiang Cai 10 February 2015
	https://www.hindawi.com/journals/ddns/2015/270604/
***************************************************************************/

int	DoComplexDynamicsInGeneralisedHénonMap(void)

    {
    double	i, c1[3], a, b, c, d, e, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    e = param[4];
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1 + a * oldy - b * oldx * oldx + c * oldx * oldy + d * oldy * oldy * oldy;
	c1[1] = oldx + e * oldy;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Complex Dynamics of the Simplest Neuron Model: Singular Chaotic Shilnikov Attractor as Specific Oscillatory Neuron Activity
	Stankevich N.V., Gonchenko A.S., Popova E.S. and Gonchenko S.V. 2023
	http://sgtnd.narod.ru/papers0/2023ChaosSolFrac-3.pdf......
	Authors' email: stankevichnv@mail.ru
***************************************************************************/

int	DoComplexDynamicsSimplestNeuronModelMap(void)

    {
    double	i, c1[3], a, b, c, ii, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ii = param[2];
    c = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx * oldx * exp(oldy - oldx) + ii;
	c1[1] = a * oldy - b * oldx + c;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Complexity and Dynamic Characteristics of a New Discrete-Time Hyperchaotic Model - 2D-HGSM
	Hayder Natiq, Nadia. M. G. Al-Saidi and M.R.M. Said June 2018
    https://www.researchgate.net/publication/325534824
***************************************************************************/

int	DoComplexityDynamicCharacteristicsMap(void)

    {
    double	i, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    c = exp(-b) * b;
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(10.75 * oldx * oldx / 4) - exp(-a * oldy * oldy);
	c1[1] = sin((9.25 * b * oldx) / 2) - c;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Constructing Chaotic Map with Multi-Cavity – Sinusoidal Objects
	Yan Xiao, Kehui Sun and Shaobo He 12 October 2019
	RECEIVED FROM Kehui Sun THROUGHT https://www.researchgate.net/requests/r67692397
***************************************************************************/

int	DoConstructingChaoticMapMultiCavity(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * cos(d * oldy) * cos(c * acos(oldx));
	c1[1] = b * cos(d * c1[0]) * cos(c * acos(oldy));
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Controlling Chaos in Maps with Multiple Strange Attractors
	Hendrik Richter
	https://www.researchgate.net/publication/220265112_Controlling_Chaos_in_Maps_with_Multiple_Strange_attractors
***************************************************************************/

int	DoControllingChaosMapsMultipleStrangeAttractors(void)

    {
    double	i, c1[3], mu, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    mu = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = oldz + mu * oldx * (1 - oldx * oldx) * exp(-oldx * oldx);
	c1[1] = b * oldx;
	c1[2] = b * oldy;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Controlling Hidden Dynamics and Multistability of a Class of Two-Dimensional Maps via Linear Augmentation - Equation XII
	Liping Zhang, Haibo Jiang, Yang Liu, Zhouchao Wei and Qinsheng Bi August 16, 2020
	https://www.researchgate.net/publication/350285338_
***************************************************************************/

int	DoControllingHiddenDynamicsMultistabilityXII(void)

    {
    double	i, c1[3], b, d, e, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    b = param[0];
    d = param[1];
    e = param[2];
    ColourFactor = param[3];
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
	c1[0] = oldy;
	c1[1] = oldx + 0.51 * oldx * oldx + oldy * oldy - 1.51 * oldx * oldy - 0.74 + e * oldz;
	c1[2] = d * oldz - e * (oldy - b);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Controlling Hidden Dynamics and Multistability of a Class of Two-Dimensional Maps via Linear Augmentation - Equation XIV
	Liping Zhang, Haibo Jiang, Yang Liu, Zhouchao Wei and Qinsheng Bi August 16, 2020
	https://www.researchgate.net/publication/350285338_
***************************************************************************/

int	DoControllingHiddenDynamicsMultistabilityXIV(void)

    {
    double	i, c1[3], b, d, e, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    b = param[0];
    d = param[1];
    e = param[2];
    ColourFactor = param[3];
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
	c1[0] = oldy;
	c1[1] = oldx + 0.38 * oldy * oldy - 0.38 * oldx * oldy - 1.6 + e * oldz;
	c1[2] = d * oldz - e * (oldy - b);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Controlling Hidden Dynamics and Multistability of a Class of Two-Dimensional Maps via Linear Augmentation - Equation XVI
	Liping Zhang, Haibo Jiang, Yang Liu, Zhouchao Wei and Qinsheng Bi August 16, 2020
	https://www.researchgate.net/publication/350285338_
***************************************************************************/

int	DoControllingHiddenDynamicsMultistabilityXVI(void)

    {
    double	i, c1[3], b, d, e, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    b = param[0];
    d = param[1];
    e = param[2];
    ColourFactor = param[3];
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
	c1[0] = oldy;
	c1[1] = oldx * 0.6 + 0.49 * oldy * oldy - oldx * oldy - 1.46 + e * oldz;
	c1[2] = d * oldz - e * (oldy - b);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Cubic Hénon Fractal Map
	Meier 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoCubicHenon(void)

    {
    double	i, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx - oldx * oldx * oldx - oldy;
	c1[1] = oldx;
	c1[2] = sqrt(sqr(c*sin(oldx)) + sqr(b*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Delayed Logistic fractal map Images
***************************************************************************/

int	DoDelayedLogistic(void)

    {
    double	i, c1[3], a, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx * (1.0 - c1[1]);
	c1[1] = oldx;
	c1[2] = sqrt(sqr(a*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Design and Hardware Implementation of 4D Memristive Hyperchaotic Map with Rich Initial-Relied and Parameter-Relied Dynamics
	Qiang Lai, Chong-Kun Zhu and Xiao-Wen Zhao 8 August 2024
	https://www.sciencedirect.com/science/article/pii/S0167926024001160.....https://doi.org/10.1016/j.vlsi.2024.102252
	Corrresponding authors: laiqiang87@126.com, 15070749256@163.com, zhaoxiaowen@hfut.edu.cn
***************************************************************************/

int	DoDesignHardwareImplementation4DMemristiveHyperchaoticMap(void)

    {
    double	i, c[4], a, b, c1, c2, k1, k2, k3, ColourFactor;

    c[0] = param[10]; // x
    c[1] = param[11]; // y
    c[2] = param[12]; // z
    c[3] = param[11]; // w

    a = param[0];
    b = param[1];
    c1 = param[2];
    c2 = param[3];
    k1 = param[4];
    k2 = param[5];
    k3 = param[6];
    ColourFactor = param[7];
    totpasses = 10;

    InitFractalMap(c, 4);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c[0];
	double oldy = c[1];
	double oldz = c[2];
	double oldw = c[3];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c[0] = a * cos(oldz) * sin(oldy);
	c[1] = k1 * oldx * cos(oldw) + c1;
	c[2] = b * oldy * sin(oldx) + c2;
	c[3] = k2 * oldw + k3 * oldx;			// I think it should be SIN(PI* ( YN + zn + v ))
	if (DisplayFractalMap(c, ((DWORD)i % threshold), i, 4, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Design New Chaotic Maps Based on Dimension Expansion
	Abdulaziz O A Alamodi, Kehui Sun, Wei Ai, Chen Chen and Dong Peng 25 January 2019
	Sent to me through special deference by Kehui Sun
***************************************************************************/

int	DoDesignNewChaoticMaps(void)

    {
    double	i, c1[5], a, w, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
    c1[3] = param[11]; // u
    c1[4] = param[12]; // v

    a = param[0];
    w = param[1];
    c = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 5);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	double oldu = c1[3];
	double oldv = c1[4];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * sin(w * oldy) * sin(c / oldx);
	c1[1] = a * sin(w * c1[0]) * sin(c / oldy);
	c1[2] = sin(PI * (c1[0] + c1[1] + oldv));
	c1[3] = sin(PI * (c1[0] + c1[2] + oldv));			// I think it should be SIN(PI* ( YN + zn + v ))
	c1[4] = sin(PI * (c1[2] + c1[3] + oldv));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 5, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Dual Hénon Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoDualHenonMap(void)

    {
    double	i, c1[3], a, b, c, d, e, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    e = param[4];
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx + b * oldy - c * oldx * oldx * oldx;
	c1[1] = oldx;
	c1[2] = sqrt(sqr(d*sin(oldx)) + sqr(e*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Duffing 2D oscillator Images
***************************************************************************/

int	DoDuffings(void)

    {
    double	a, e, w, t, h, ColourFactor;
    double	f, g;
    double	i, c1[3], xo, yo, x1, y1;
    double	k1x, k1y, k2x, k2y, k3x, k3y, k4x, k4y;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    ColourFactor = param[1];
    totpasses = 10;

    g = .2; e = 1; f = 7.5; w = 1; t = 0;				// Constant values 
    h = .05; //dt = PI / 100;						// h stepsize and dt time steps
    xo = xdots / 2; yo = ydots / 2; // xscale = 20; yscale = 20;	// Scale values
//    u1 = (int)(x * xscale + xo); v1 = (int)(yo - y * yscale);		// Prevents first line draw 
//    u1 = 1; u = 0; v1 = 0; v = 0;					// Initial line draw points

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	t += dt;
	k1x = c1[1];
	k1y = -g * c1[1] - e * c1[0] * c1[0] * c1[0] + f * cos(w * t) + a * c1[0]; 
	k2x = c1[1] + .5 * h * k1y;
	k2y = -g * (c1[1] + .5 * h * k1y) - e * pow((c1[0] + .5 * h * k1x), 3) + f * cos(w * (t + .5 * dt)) + a * (c1[0] + .5 * h * k1x);
	k3x = c1[1] + .5 * h * k2y;
	k3y = -g * (c1[1] + .5 * h * k2y) - e * pow((c1[0] + .5 * h * k2x), 3) + f * cos(w * (t + .5 * dt)) + a * (c1[0] + .5 * h * k2x); 
	k4x = c1[1] + h * k3y;
	k4y = -g * (c1[1] + h * k3y) - e * pow((c1[0] * h * k3x), 3) + f * cos(w * (t + dt)) + a * (c1[0] * h * k3x);
	y1 = c1[1] + (1.0 / 6.0) * h * (k1y + 2 * k2y + 2 * k3y + k4y);
	x1 = c1[0] + (1.0 / 6.0) * h * (k1x + 2 * k2x + 2 * k3x + k4x);
	c1[0] = x1;
	c1[1] = y1;
	c1[2] = sqrt(sqr(sin(a*x1)) + sqr(cos(a*y1)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Duffing Chaotic fractal map Images
***************************************************************************/

int	DoDuffingChaotic(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = c1[1];
	c1[1] = -b * oldx + a * c1[1] - c1[1] * c1[1] * c1[1];
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Dynamics and Control of the Fractional Forms of Some Rational Chaotic Maps - Chang and Alli Map
	OUANNAS Adel, KHENNAOUI Amina-Aicha, BENDOUKHA Samir, WANG Zhen and PHAM Viet-Thanh 17 February 2019
	Sent to ManpWIN by a J. Tenreiro Machado connection
***************************************************************************/

int	DoDynamicsControlFractionalFormsChangMap(void)

    {
    double	i, c1[3], p, q, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    p = param[0];
    ColourFactor = param[1];
    q = p - 0.001;
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1.0 / (1.0 + oldx * oldx) - p * oldy;
	c1[1] = 1.0 / (0.1 + oldy * oldy) + q * oldx;
	c1[2] = sqrt(sqr(p*sin(oldx)) + sqr(p*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Dynamics and Control of the Fractional Forms of Some Rational Chaotic Maps - Rulkov Map
	OUANNAS Adel, KHENNAOUI Amina-Aicha, BENDOUKHA Samir, WANG Zhen and PHAM Viet-Thanh 17 February 2019
	Sent to ManpWIN by a J. Tenreiro Machado connection
***************************************************************************/

int	DoDynamicsControlFractionalFormsRulkovMap(void)

    {
    double	i, c1[3], sigma, theta, mu, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    sigma = param[0];
    theta = param[1];
    mu = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = mu / (1 + oldx * oldx) + oldy;
	c1[1] = oldy - sigma * oldx - theta;
	c1[2] = sqrt(sqr(mu*sin(oldx)) + sqr(mu*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Dynamics and Control of the Fractional Forms of Some Rational Chaotic Maps - Zeraoulia-Sprott Map
	OUANNAS Adel, KHENNAOUI Amina-Aicha, BENDOUKHA Samir, WANG Zhen and PHAM Viet-Thanh 17 February 2019
	Sent to ManpWIN by a J. Tenreiro Machado connection
***************************************************************************/

int	DoDynamicsControlFractionalFormsZeraculiaMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = -a * oldx / (1 + oldy * oldy);
	c1[1] = oldx + b * oldy;
	c1[2] = sqrt(sqr(a*sin(oldx)) + sqr(b*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Dynamics and Optimisation Control of a Robust Chaotic Map
	HUNLAI LI, KUN QIAN, SHAOBO HE, HONGMIN LI and WEI FENG 2019
	Received from SHAOBO HE through Research Gate
***************************************************************************/

int	DoDynamicsOptimisationControl(void)

    {
    double	i, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldy * oldy;
	c1[1] = b * oldy - c * oldx * oldy;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Dynamics of a Higher Dimensional Fractional-Order Chaotic Map
	Yuexi Peng, Kehui Sun, Dong Peng and Wei Ai 27 March 2019
	Sent directly by the author Kehui Sun
***************************************************************************/

int	DoDynamicsHigherDimensionalFractionalOrderChaoticMap(void)

    {
    double	i, c1[3], a, c, w, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    c = param[1];
    w = param[2];
    ColourFactor = param[3];
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
	c1[0] = a * sin(w * oldz) * sin(c / oldx);				// x
	c1[1] = a * sin(w * oldx) * sin(c / oldy);				// y
	c1[2] = a * sin(w * oldy) * sin(c / oldz);				// z
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Dynamics of a New Multi-Cavity Hyperchaotic Map and Its DSP Implementation
	Yan Xiao, Kehui Sun*, Mengyao Yu and Xuemei Xu November 28, 2018
	https://www.researchgate.net/publication/338177092_Dynamics_of_a_New_Multi-Cavity_Hyperchaotic_Map_and_Its_DSP_Implementation
***************************************************************************/

int	DoDynamicsNewMultiCavityHyperChaoticMap(void)

    {
    double	i, c1[3], a, phi, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    phi = param[1];
    c = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * sin(2 * PI * oldy + phi) * cos(c * acos(oldx));
	c1[1] = a * sin(2 * PI * c1[0] + phi) * cos(c * acos(oldy));
	c1[2] = sqrt(sqr(a*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Dynamical Trapping in the Area-Preserving Hénon Map
	V. M. de Oliveira, D. Ciro and I. L. Caldas 26/July/2019
	https://arxiv.org/pdf/1907.11616.pdf (specifically searched in arxiv.org about maps)
***************************************************************************/

int	DoDynamicalTrappingAreaPreservingHénonMap(void)

    {
    double	t, c1[3], a, b, real, imag, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    real = param[2];
    imag = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = a - oldx * oldx + b * oldy;
	c1[1] = oldx;
	c1[2] = real * cos(oldy) + imag * sin(oldx);
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Dynamic Fractal Map as in Fractint
	https://archive.org/details/frain200
***************************************************************************/

int	DoDynamicFractalMap(void)

    {
    double	t, c1[3], a, b, fx, fy, trig1, trig2, trig3, trig4, reverse, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    trig1 = param[2];
    trig2 = param[3];
    trig3 = param[4];
    trig4 = param[5];
    reverse = param[6];
    ColourFactor = param[7];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	if (reverse == 0.0)
	    {
	    if (trig1 == 0.0 && trig2 == 0.0)
		fx = sin(oldx + a * sin(b * oldx));
	    else if (trig1 == 0.0 && trig2 != 0.0)
		fx = sin(oldx + a * cos(b * oldx));
	    else if (trig1 != 0.0 && trig2 == 0.0)
		fx = cos(oldx + a * sin(b * oldx));
	    else if (trig1 != 0.0 && trig2 != 0.0)
		fx = cos(oldx + a * cos(b * oldx));

	    if (trig3 == 0.0 && trig4 == 0.0)
		fy = sin(oldy + a * sin(b * oldy));
	    else if (trig3 == 0.0 && trig4 != 0.0)
		fy = sin(oldy + a * cos(b * oldy));
	    else if (trig3 != 0.0 && trig4 == 0.0)
		fy = cos(oldy + a * sin(b * oldy));
	    else if (trig3 != 0.0 && trig4 != 0.0)
		fy = cos(oldy + a * cos(b * oldy));
	    }
	else
	    {
	    if (trig1 == 0.0 && trig2 == 0.0)
		fx = sin(oldy + a * sin(b * oldx));
	    else if (trig1 == 0.0 && trig2 != 0.0)
		fx = sin(oldy + a * cos(b * oldx));
	    else if (trig1 != 0.0 && trig2 == 0.0)
		fx = cos(oldy + a * sin(b * oldx));
	    else if (trig1 != 0.0 && trig2 != 0.0)
		fx = cos(oldy + a * cos(b * oldx));

	    if (trig3 == 0.0 && trig4 == 0.0)
		fy = sin(oldx + a * sin(b * oldy));
	    else if (trig3 == 0.0 && trig4 != 0.0)
		fy = sin(oldx + a * cos(b * oldy));
	    else if (trig3 != 0.0 && trig4 == 0.0)
		fy = cos(oldx + a * sin(b * oldy));
	    else if (trig3 != 0.0 && trig4 != 0.0)
		fy = cos(oldx + a * cos(b * oldy));
	    }

	c1[0] = oldy + fx;
	c1[1] = oldx - fy;
	c1[2] = cos(oldy) + sin(oldx);
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Electronically-Implemented Coupled Logistic Maps
	Alexandre L'Her, Pablo Ami1, Nicolas Rubido, Arturo C. Marti and Cecilia Cabeza 4 February 2016
	https://www.researchgate.net/publication/288890242
***************************************************************************/

int	DoElectronicallyImplementedCoupledLogisticMaps(void)

    {
    double	t, c1[3], r1, r2, e, real, imag, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    r1 = param[0];
    r2 = param[1];
    e = param[2];
    real = param[3];
    imag = param[4];
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = (1 - e) * r1 * oldx * (1 - oldx) + e * r2 * oldy * (1 - oldy);
	c1[1] = (1 - e) * r2 * oldy * (1 - oldy) + e * r1 * oldx * (1 - oldx);
	c1[2] = real * cos(oldy) + imag * sin(oldx);
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Elhadj - Sprott Fractal Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoElhadjSprottMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = -a * oldx * oldx + oldy;
	c1[1] = b - fabs(oldx);
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Ergodic Theory of Chaos and Strange Attractors Map
	J.P Eckmann and D. Ruelle July 1985
	https://www.researchgate.net/publication/224043589
***************************************************************************/

int	DoErgodicTheoryMap(void)

    {
    double	i, c1[3], a, b1, b2, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b1 = param[1];
    b2 = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = (a - oldx - b1 * oldy) * oldx;
	c1[1] = (a - b2 * oldx - oldy) * oldy;
	c1[2] = sqrt(sqr(10.0*b1*sin(oldx)) + sqr(10.0*b2*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Existence and Stability of Chimera States in a Minimal System of Phase Oscillators – as a Map
	Mary Thoubaan and Peter Ashwin 24 October 2018
	https://www.researchgate.net/publication/328508380
***************************************************************************/

int	DoExistenceStabilityChimeraStatesMap(void)

    {
    double	i, c1[3], r, beta, g0, g12, g21, g13, g31, g23, g32, v12, v13, v21, v23, v31, v32, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    r = param[0];
    beta = param[1];
    ColourFactor = param[2];
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
	v12 = oldx - oldy;
	v13 = oldx - oldz;
	v21 = oldy - oldx;
	v23 = oldy - oldz;
	v31 = oldz - oldx;
	v32 = oldz - oldy;
	g12 = cos(v12 + beta) + r * sin(2 * v12);
	g21 = cos(v21 + beta) + r * sin(2 * v21);
	g13 = cos(v13 + beta) + r * sin(2 * v13);
	g31 = cos(v31 + beta) + r * sin(2 * v31);
	g23 = cos(v23 + beta) + r * sin(2 * v23);
	g32 = cos(v32 + beta) + r * sin(2 * v32);
	g0 =  cos(beta);
	c1[0] = 2 * g12 + 2 * g13;
	c1[1] = 2 * g21 + g23 + g0;
	c1[2] = 2 * g31 + g32 + g0;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Existence and Stability of Chimera States in a Minimal System of Phase Oscillators – as a 3D Map
	Mary Thoubaan and Peter Ashwin 24 October 2018
	https://www.researchgate.net/publication/328508380
***************************************************************************/

int	DoExistenceStabilityChimeraStates3DMap(void)

    {
    double	i, c1[3], ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

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
	c1[0] = -2 * sin(oldy) + sin(oldx) * sin(oldy);
	c1[1] = 2 * cos(oldy) * cos(oldx) + 5 * cos(oldy) * cos(oldy) * cos(oldx);
	c1[2] = -3 * sin(oldy) + sin(oldx) * sin(oldz);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Explore the Potential of Deep Learning and Hyperchaotic Map in the Meaningful Visual Image Encryption Scheme
	Wei Chen, Yichuan Wang, Yeqiu Xiao and Xinhong Hei 14 June 2023
	https://assets.researchsquare.com/files/rs-1786505/v1_covered.pdf?c=1657814400......DOI: 10.1049/ipr2.12858
	Corresponding author: : chuan@xaut.edu.cn
***************************************************************************/

int	DoExplorePotentialDeepLearningMap(void)

    {
    double	i, r, h, c1[3], ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    r = param[0];
    h = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(r * PI * oldx + h * oldy * cos(PI * oldy));
	c1[1] = sin(h * PI * oldy + r * oldx * cos(PI * oldx));
	c1[2] = sqrt(sqr(sin(r*oldx)) + sqr(cos(h*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Fat Hénon
	M. Hénon
	1976
	https://projecteuclid.org/download/pdf_1/euclid.cmp/1103900150
***************************************************************************/

int	DoFatHenonMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1.0 - a * oldx * oldx + c1[1];
	c1[1] = b * (sqrt(fabs(oldx * c1[1])) + (oldx + c1[1]) / 2.0);
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Feigenbaum Strange fractal map Images
***************************************************************************/

int	DoFeigenbaum(void)

    {
    double	i, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a*(1-b*cos(2*3.141592654*c1[1]))*oldx*(1-oldx);
	c1[1] = fmod(c1[1]+c,1);
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Four-Dimensional Tinkerbelle Chaotic System for Secure Transmission
	Hikmat N. Abdullah and Hamsa A. Abdullah April 2017
	https://www.researchgate.net/publication/324107641
***************************************************************************/

int	DoFourDimensionalTinkerbelle(void)

    {
    double	i, c1[4], a, b, c, d, e, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z
    c1[3] = param[13]; // w

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    e = param[4];
    ColourFactor = param[5];

    InitFractalMap(c1, 4);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);

	c1[0] = oldx * oldx - oldy * oldy + a * oldx + b * oldy;
	c1[1] = 2 * oldx * oldy + c * oldx + d * oldy;
	c1[2] = e * oldx * (1 - oldx * oldx);
	c1[3] = oldy - oldx + oldz;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 4, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Fractal Dream Chaotic Fractal Map
***************************************************************************/

int	DoFractalDream(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(c1[1] * b) + c * sin(oldx*b);
	c1[1] = sin(oldx*a) + d * sin(c1[1] * a);
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Fractional Form of a New Three-Dimensional Generalised Hénon Map
	Lotfi Jouini, Adel Ouannas, Amina-Aicha Khennaoui, Xiong Wang Giuseppe Grassi and Viet-Thanh Pham 2019
	https://www.researchgate.net/publication/331970162_The_fractional_form_of_a_new_three-dimensional_generalized_Henon_map
***************************************************************************/

int	DoFractionalFormNew3DGeneralisedHénonMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = a - oldy * oldy + b * oldz;
	c1[1] = oldx;
	c1[2] = oldy;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Further Study on the Rulkov Neuron: Geometric Control, Circuit Realisation and Chaos-Based Encryption
	Ming Xia1, Chunbiao Li, Yongxin Li1, Peiran Li and Awais khan 31 May 2024
	https://iopscience.iop.org/article/10.1088/1402-4896/ad4e17/pdf.....DOI 10.1088/1402-4896/ad4e17
	Corresponding author: goontry@126.com and chunbiaolee@nuist.edu.cn
***************************************************************************/

int	DoFurtherStudyontheRulkovNeuronMap(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a / (1 + sqr(oldx)) + d * oldy;
	c1[1] = oldy - b * (oldx - c);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Gingerbread Man based on Fractint. Variation 1 - Matryoshka  Presentation
	Mengjiao Wang, Mingyu An, Xinan Zhang and Herbert Ho-Ching Iu 14 January 2019
	https://assets.researchsquare.com/files/rs-1786505/v1_covered.pdf?c=1657814400
***************************************************************************/

int	DoGingerbreadManVariations(void)

    {
    double	i, c1[3], ColourFactor, b, c, d;
    int		MapType;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    MapType = (int)param[0];
    ColourFactor = param[1];
    b = (int)param[2];
    c = (int)param[3];
    d = (int)param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	switch (MapType)
	    {
	    case 0:							// Matryoshka
		c1[0] = 0.9998 - oldy - fabs(oldx);
		c1[1] = exp(sin(oldx));
		break;
	    case 1:							// 1 - y + sin(abs(x))
		c1[0] = 1.0 - oldy + sin(fabs(oldx));
		c1[1] = oldx;
		break;
	    case 2:							// One Inverted Signal
		c1[0] = 1.0 - oldy - fabs(oldx);
		c1[1] = oldx;
		break;
	    case 3:							// Gingerbreadmanakin
		c1[0] = 1.0 - 1.5 * fabs(oldx + d) - b * oldy;
		c1[1] = c * oldx;
		break;
	    default:							// Matryoshka
		c1[0] = 0.9998 - oldy - fabs(oldx);
		c1[1] = exp(sin(oldx));
		break;
	    }
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Gingerbread Woman
	Anonymous and Marcus 2019
***************************************************************************/

int	DoGingerbreadWoman(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1 - oldy + fabs(oldx) ;
	c1[1] = oldx - oldy / 20000;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Gumowski-Mira Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoGumowskiMira(void)

    {
    double	i, c1[3], a, b, c, d, fn, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldy + a * (1 - 0.05 * oldy * oldy) + (b * oldx + 2 * (1 - b) * oldx * oldx / (1 + oldx * oldx));
	fn = b * c1[0] + 2 * (1 - b) * c1[0] * c1[0] / (1 + c1[0] * c1[0]);
	c1[1] = -oldx + fn;
	c1[2] = sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Gumoski-Mira Map - 3 Wings
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoGumowskiMira3Wings(void)

    {
    double	i, c1[3], a, b, c, d, fx, fx1, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	fx = a * oldx - 2 * (1 - a) * oldx * oldx / (1 + oldx * oldx);
	c1[0] = oldy + 0.008 * (1 - 0.05 * oldy * oldy) * oldy + fx;
	fx1 = a * c1[0] - 2 * (1 - a) * c1[0] * c1[0] / (1 + c1[0] * c1[0]);
	c1[1] = -oldx + fx1;
	c1[2] = sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Gumoski-Mira Map - 5 Wings
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoGumowskiMira5Wings(void)

    {
    double	i, c1[3], mu, a, b, fx, fx1, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    mu = param[0];
    a = param[1];
    b = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	fx = mu * oldx + 3 * (1 - mu) * oldx * oldx / (1 + oldx * oldx);
	c1[0] = oldy + 0.008 * (1 - 0.05 * oldy * oldy) * oldy + fx;
	fx1 = mu * c1[0] + 3 * (1 - mu) * c1[0] * c1[0] / (1 + c1[0] * c1[0]);
	c1[1] = -oldx + fx1;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Hénon
	M. Hénon
	1976
	https://projecteuclid.org/download/pdf_1/euclid.cmp/1103900150
***************************************************************************/

int	DoHenonMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1.0 - a * oldx * oldx + c1[1];
	c1[1] = b * oldx;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Hénon Modified Fractal Map
	Marcus Rezende 16 November 2019
	Made at home
***************************************************************************/

int	DoHenonMarcusMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = -a * oldx * oldx + fabs(oldz) + 0.95;
	c1[1] = -b * fabs(oldx);
	c1[2] = b * oldx + fabs(oldy);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Homoclinic Tangencies with Infinitely many Asymptotically Stable Single-Round Periodic Solutions
	S.S. Muni, R.I. McLachlan, and D.J.W. Simpson December 10, 2020
	https://arxiv.org/pdf/2006.01405.pdf
***************************************************************************/

int	DoHomoclinicTangenciesMap(void)

    {
    double	i, c1[3], c2, d5, lambda, sigma, d1, a1, a2, a3, v1, v2, v3, v4, v5, v6, temp, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    c2 = param[0];
    d5 = param[1];
    lambda = param[2];
    sigma = param[3];
    d1 = param[4];
    ColourFactor = param[5];
    totpasses = 10;

    a1 = 3 / (1 - fabs(lambda));	// constant 1
    a2 = (2 * fabs(lambda) + 1) / 3;	// constant 2
    a3 = (2 + fabs(lambda)) / 3;	// constant 3

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	v1 = sqr(a1 * (oldy - a2));					// aux variable 1
	temp = (a1 * (oldy - a2));
	v2 = temp * temp * temp;					// aux variable 2
	v3 = (1 - 3 * v1 + 2 * v2);					// first group of variable
	v4 = (3 * v1 - 2 * v2);						// second group of variables
	v5 = 1 + c2 * (oldy - 1);					// variable affecting XX
	v6 = d1 * oldx + d5 * sqr(oldy - 1);				// variable affecting yy

	if (oldy < a2)
	    {
	    c1[0] = lambda * oldx; c1[1] = sigma * oldy;
	    }
	else if (oldy > a3)
	    {
	    c1[0] = v5; c1[1] = v6;
	    }
	else
	    {
	    c1[0] = (v3 + v4) * lambda * oldx + (v3 - v4) * v5;
	    c1[1] = (v3 + v4) * sigma * oldy + (v3 - v4) * v6;
	    }
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Hopa-Jong Chaotic Fractal Map
***************************************************************************/

int	DoHopaJong(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(a*c1[1])-cos(b*oldx);
	c1[1] = sin(c*oldx)-cos(d*c1[1]);
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Hyperchaotic Attractors of Three-Dimensional Maps and Scenarios of their Appearance-I
	Shykhmamedov A., Karatetskaia E., Kazakov A. and Stankevich N 9 Dec 2020
	https://arxiv.org/pdf/2012.05099.pdf
***************************************************************************/

int	DoHyperchaoticAttractors3DMaps(void)

    {
    double	i, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];
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
	c1[0] = oldy;
	c1[1] = oldz;
	c1[2] = b * oldx + c * oldy + a * oldz - oldy * oldy;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Hyperchaotic Towel Map
	http://www.scholarpedia.org/article/Hyperchaos
***************************************************************************/

int	DoHyperchaoticTowelMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = a * oldx * (1 - oldx) - 0.05 * (oldy + 0.35) * (1 - 2 * oldz);
	c1[1] = 0.1 * ((oldy + 0.35) * (1 + 2 * oldz) - 1) * (1 - 1.9 * oldx);
	c1[2] = 3.78 * oldz * (1 - oldz) + b * oldy;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Hyperchaotic Logistic map for arduino with gameduino shield fractal map Images
***************************************************************************/

int	DoHyperLogistics(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1.0-(a*pow(oldx,2))+b*(c1[1]-oldx);
	c1[1] = 1.0-(a*pow(c1[1],2))+b*(oldx-c1[1]);
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Icons-3D Fractal Map Images
***************************************************************************/

int	DoIcons3D(void)

    {
    double	i, c1[3], alpha, beta, gamma, lambda, omega, ColourFactor;
    double	zzbar, zreal, zimag, za, zb, zn, p;
    int		degree, j;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    alpha = param[0];
    beta = param[1];
    gamma = param[2];
    lambda = param[3];
    degree = (int)param[4];
    omega = param[5];
    ColourFactor = param[6];
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

	zzbar = oldx * oldx + oldy * oldy;
	zreal = oldx;
	zimag = oldy;

	for (j = 1; j <= degree - 2; j++) 
	    {
	    za = zreal * oldx - zimag * oldy;
	    zb = zimag * oldx + zreal * oldy;
	    zreal = za;
	    zimag = zb;
	    }

	zn = oldx * zreal - oldy * zimag;
	p = lambda + alpha * zzbar + beta * zn;
	c1[0] = p * oldx + gamma * zreal - omega * oldy;
	c1[1] = p * oldy - gamma * zimag + omega * oldx;
	c1[2] = zzbar;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Identification of Chaos in Fractional Chaotic Map (Burger's Map)
	Jie Ran 20 May 2020
	https://advancesindifferenceequations.springeropen.com/articles/10.1186/s13662-020-02688-9
***************************************************************************/

int	DoIdentificationChaosFractionalChaoticMapBurgersMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;
//    int		degree, j;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
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
	c1[0] = (1 - a) * oldx - oldy * oldy;
	c1[1] = (1 + b) * oldy + oldx * oldy;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Ikeda Fractal Map Images
***************************************************************************/

int	DoIkeda2D(void)

    {
    double  i, c1[3], xt, temp, sintemp, costemp, a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	temp = c - d / (1.0 + c1[0] * c1[0] + c1[1] * c1[1]);
	sintemp = sin(temp);
	costemp = cos(temp);

	xt = a + b * (c1[0] * costemp - c1[1] * sintemp);
	c1[1] = b * (c1[0] * sintemp + c1[1] * costemp);
	c1[0] = xt;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Image Encryption Technology with Fractional 2D Triangle Function Combination Discrete Chaotic Map Coupled with Menezes-Vanstone Elliptic Curve Cryptosystem
	Zeyu Liu, Tiecheng Xia and Jinbo Wang 23 Apr 2018
	https://www.hindawi.com/journals/ddns/2018/4585083/
***************************************************************************/

int	DoImageEncryptionTechnologyFractional2DTriangleFunction(void)

    {
    double  i, c1[3], ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    ColourFactor = param[0];
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	c1[0] = 1.75 * cos(oldx + oldy);
	c1[1] = 1.75 * sin(oldx * oldy) - 1.15 * sin(c1[0] * c1[1]);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Image Encryption using the Two-dimensional Logistic Chaotic Map
	Yue Wu, Gelan Yang, Huixia Jin and Joseph P. Noonan 2011
	https://www.researchgate.net/profile/Yue-Wu-86/publication/258660674
***************************************************************************/

int	DoImageEncryptionUsing2DLogisticChaoticMap(void)

    {
    double  i, c1[3], r, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    r = param[0];
    ColourFactor = param[1];
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	c1[0] = r * (3 * oldy + 1) * oldx * (1 - oldx);
	c1[1] = r * (3 * c1[0] + 1) * oldy * (1 - oldy);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Influence of Parametric Symmetry on the Dynamics of 3D Sinusoidal Discrete Systems Map
	Karthikeyan Rajagopal, Sathiyadevi Kanagaraj, Christos Volos and Anitha Karthikeyan 23 March 2023
	https://www.researchgate.net/publication/369488107.......https://doi.org/10.3390/sym15040780
	corresponding author: volos@physics.auth.gr
	https://en.wikipedia.org/wiki/Cylindrical_coordinate_system
***************************************************************************/

int	DoInfluenceParametricSymmetryDynamics3DSinusoidalDiscreteSystemsMap(void)

    {
    double  i, c1[3], a, b, c, d, m, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    m = param[4];
    ColourFactor = param[5];
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	c1[0] = oldy;
	c1[1] = sin(oldz);
	c1[2] = a + b * oldx + c * oldy - d * sin(oldz) + m * oldz;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Initial-Switched Boosting Bifurcations in 2D Hyperchaotic Map
	B. C. Bao, H. Z. Li, L. Zhu, X. Zhang and M. Chen 03 March 2020
	https://www.researchgate.net/publication/339635913
***************************************************************************/

int	DoInitialSwitchedBoostingBifurcations(void)

    {
    double  i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	c1[0] = oldx + a * oldx * sin(oldy) + b;
	c1[1] = oldx + oldy;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Kakadu Fractal Map
	Komeil Nosrati and Christos Volos 2001
	http://www.3d-meier.de/ "Darstellung von Attraktoren mit Cinema" 3.8
***************************************************************************/

int	DoKakadu(void)

    {
    double  i, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	c1[0] = oldy * (1.0 + sin(a * oldx)) - b * sqrt(fabs(oldx));
	c1[1] = c - oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Kaneko-I Attractor
	Meier 2001
	http://www.3d-meier.de/ "Darstellung von Attraktoren mit Cinema" 3.9
	The constant d has a great influence on the attractor's appearance. 
	Try different constants (d = 1.00 1.22 1.23 1.32 1.50 1.90 1.96 2.01 2.04).
***************************************************************************/

int	DoKanekoI(void)

    {
    double  i, c1[3], a, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    d = param[1];
    ColourFactor = param[2];
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	c1[0] = a * oldx + (1.0 - a) * (1.0 - d * oldy * oldy);
	c1[1] = oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Kaneko-I Attractor
	Meier 2001
	http://www.3d-meier.de/ "Darstellung von Attraktoren mit Cinema" 3.10
***************************************************************************/

int	DoKanekoII(void)

    {
    double  i, c1[3], a, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    d = param[1];
    ColourFactor = param[2];
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	c1[0] = a * oldx + (1.0 - a) * (1.0 - d * fabs(oldy));
	c1[1] = oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Basin Chaotic fractal map Images
***************************************************************************/

int	DoKnot(void)

    {
    double	i, c1[3], ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

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
	c1[0] = -c1[1];
	c1[1] = pow(oldx,3)-oldx-c1[1];
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Lauwerier Attractor
	Meyer 2001
	http://www.3d-meier.de/ "Darstellung von Attraktoren mit Cinema" 3.7
***************************************************************************/

int	DoLauwerierAttractor(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldy;
	c1[1] = oldx - oldy + a * oldx * oldy - b * oldy * oldy;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Lorenz-2D Chaotic Fractal Map Images
***************************************************************************/

int	DoLorenz2D(void)

    {
    double	i, c1[3], xn, yn, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    ColourFactor = param[0];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i += dt)
	{
	double oldx = c1[0];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	xn = c1[0]-c1[1]-pow(c1[0],3);
	yn = c1[0]-(c1[0]*c1[0])*c1[1];
	c1[0] += xn*dt;
	c1[1] += yn*dt;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Lorenz-3D Fractal Map Images
***************************************************************************/

int	DoLorenz3D(void)

    {
    double	i, c1[3], ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

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
	c1[0] = oldx*oldy-oldz;
	c1[1] = oldx;
	c1[2] = oldy;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Lorenz-84 Chaotic Fractal Map Images
***************************************************************************/

int	DoLorenz84(void)

    {
    double	i, c1[3], xn, yn, zn, a, b, f, g, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    xn = c1[0];
    yn = c1[1];
    zn = c1[2];

    a = param[0];
    b = param[1];
    f = param[2];
    g = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i += dt)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	xn = -a*c1[0]-pow(c1[1],2)-pow(c1[2],2)+a*f;
	yn = -c1[1]+c1[0]*c1[1]-b*c1[0]*c1[2]+g;
	zn = -c1[2]+b*c1[0]*c1[1]+c1[0]*c1[2];
	c1[0] += xn*dt;
	c1[1] += yn*dt;
	c1[2] += zn*dt;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Lorenz and Duffing Like Map
	Marcus Rezende May 2021
***************************************************************************/

int	DoLorenzDuffingAlikeMap(void)

    {
    double	i, c1[3], a, w, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    w = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldy;
	c1[1] = sin(oldy + oldx / sqrt(17.0)) + oldy * cos(oldx) - a * sin(w * i);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Lorenz Discrete-Time Map
	https://www.researchgate.net/publication/282495312
	June 2017
***************************************************************************/

int	DoLorenzDiscreteTimeMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = (1 + a * b) * oldx - b * oldx * oldy;
	c1[1] = (1 - b) * oldy + b * oldx *oldx;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Lozi Fractal map Images
***************************************************************************/

int	DoLozi(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1 - a * fabs(oldx) + b * oldy;
	c1[1] = oldx;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	MacMillan Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoMacMillan(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldy;
	c1[1] = -oldx + 2 * a * oldy / (1 + oldy * oldy) + b * oldy;
	c1[2] = sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Maluquice Craziness Images created by Marcus
***************************************************************************/

int	DoMaluquice(void)

    {
    double	i, c1[3], cn[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1];
	cn[1] = -a * c1[0] - b * c1[0] * c1[0] * c1[0];
	c1[0] += cn[0] * dt;
	c1[1] += cn[1] * dt;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Marotto-Lorenz Fractal Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoMarottoLorenz(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = (1 - a * oldx - b * oldy) * (a * oldx + b * oldy);
	c1[1] = oldx;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Maynard Smith Map
	Meier 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoMaynardSmith(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldy;
	c1[1] = a * oldy + b - oldx * oldx;
	c1[2] = sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Meltzler Map
	Erik Meier
***************************************************************************/

int	DoMeltzlerMap(void)

    {
    double	i, c1[3], a, b, h, ColourFactor;
    bool	xAbs, yAbs;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    h = param[0];
    a = param[1];
    b = param[2];
    xAbs = (param[3] == 1.0);
    yAbs = (param[4] == 1.0);
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = xAbs ? fabs(c1[0]) : c1[0];
	double oldy = yAbs ? fabs(c1[1]) : c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx + h * (oldx - oldx * oldx + oldy);
	c1[1] = oldy + h * (oldy - oldy * oldy + oldx);
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Memristive Rulkov Neuron Model with Magnetic Induction Effects - modified
	Kexin Li, Han Bao, Houzhen Li, Jun Ma, Zhongyun Hua and Bocheng Bao May 31, 2021
***************************************************************************/

int	DoMemristiveRulkovNeuronModelMap(void)

    {
    double	i, c1[3], a, m, e, k, Marcus, fxy, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    m = param[1];
    e = param[2];
    k = param[3];
    Marcus = (param[4] == 0.0) ? 0.0 : 1.0;					// Marcus added anotrher factor
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	fxy = (oldx <= 0.0) ? a / (1 - oldx) + oldy : ((oldx >= a + oldy) ? -1 : a + oldy);
	c1[0] = fxy + k * oldx * tanh(oldz) * ((Marcus) ? oldx : 1.0);
	c1[1] = oldy - m * oldx;
	c1[2] = oldz + e * oldx;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Metz Fractal Map
	Meier 2001
	http://www.3d-meier.de/ "Darstellung von Attraktoren mit Cinema" 3.15
***************************************************************************/

int	DoMetz(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx * exp((1 - sqrt(1 + oldy))/b);
	c1[1] = a * oldx - c1[0];
	c1[2] = sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 08 and 09
	Marcus August 2021
***************************************************************************/

int	DoMiraMapA(bool plus)

    {
    double	t, c1[3], a, s, mu, f2 = 1.0, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    mu = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	if (plus)
	    c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + f2;
	else
	    c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) - f2;
	f2 = mu * c1[0] + (1 - mu) * c1[0] * c1[0] * exp(-(c1[0] * c1[0] - 3) / 4);
	c1[1] = -oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 08
	Marcus August 2021
***************************************************************************/

int	DoMiraMap08(void)

    {
    return (DoMiraMapA(true));
    }

/**************************************************************************
	Mira Map 09
	Marcus August 2021
***************************************************************************/

int	DoMiraMap09(void)

    {
    return (DoMiraMapA(false));
    }

/**************************************************************************
	Mira Map 12 and 13
	Marcus August 2021
***************************************************************************/

int	DoMiraMapB(bool plus)

    {
    double	t, c1[3], a, s, mu, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    mu = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f2 = mu * oldx + 2 * (1 - mu) * oldx * oldx / (1 + oldx * oldx);
	if (plus)
	    c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) - f2;
	else
	    c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + f2;
	c1[1] = -oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 12
	Marcus August 2021
***************************************************************************/

int	DoMiraMap12(void)

    {
    return (DoMiraMapB(true));
    }

/**************************************************************************
	Mira Map 13
	Marcus August 2021
***************************************************************************/

int	DoMiraMap13(void)

    {
    return (DoMiraMapB(false));
    }

/**************************************************************************
	Mira Map 15
	Marcus August 2021
***************************************************************************/

int	DoMiraMap15(void)

    {
    double	t, c1[3], a, s, f1, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f2 = 2 * oldx * oldx / (1 + oldx * oldx * oldx * oldx);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + f2;
	f1 = 2 * c1[0] * c1[0] / (1 + c1[0] * c1[0] * c1[0] * c1[0]);
	c1[1] = -oldx + f1;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 16
	Marcus August 2021
***************************************************************************/

int	DoMiraMap16(void)

    {
    double	t, c1[3], a, s, f1 = 0.0, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f2 = 2 * oldx * oldx / (1 + oldx * oldx * oldx * oldx);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + f2 + f1;
	f1 = 2 * c1[0] * c1[0] / (1 + c1[0] * c1[0] * c1[0] * c1[0]);
	c1[1] = -oldx + f1 - f2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 19
	Marcus August 2021
***************************************************************************/

int	DoMiraMap19(void)

    {
    double	t, c1[3], a, s, f1, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f1 = 2 * oldx * oldx / (1 + oldx * oldx * oldx * oldx);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) - f1 + oldx;
	c1[1] = -oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 21
	Marcus August 2021
***************************************************************************/

int	DoMiraMap21(void)

    {
    double	t, c1[3], a, s, f1, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    ColourFactor = param[2];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f1 = 2 * oldx * oldx / (1 + oldx * oldx * oldx * oldx);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) - f1 / oldx;
	c1[1] = -oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 23
	Marcus August 2021
***************************************************************************/

int	DoMiraMap23(void)

    {
    double	t, c1[3], a, s, m, f1, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    m = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f1 = m * oldx + (2 * (1 - m) * oldx * oldx) / (1 + oldx * oldx);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + f1;
	f2 = m * c1[0] + (2 * (1 - m) * c1[0] * c1[0]) / (1 + c1[0] * c1[0]);
	c1[1] = -oldx + f2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 24
	Marcus August 2021
***************************************************************************/

int	DoMiraMap24(void)

    {
    double	t, c1[3], a, s, m, f1, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    m = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f1 = m * oldx + (2 * (1 - m) * oldx * oldx) / (1 + oldx * oldx);
	c1[0] = oldy + a * (1 - s * oldy * oldy) * oldy * f1;
	f2 = m * c1[0] + (2 * (1 - m) * c1[0] * c1[0]) / (1 + c1[0] * c1[0]);
	c1[1] = -oldx + f2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 25
	Marcus August 2021
***************************************************************************/

int	DoMiraMap25(void)

    {
    double	t, c1[3], a, s, m, f1, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    m = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy);
	f1 = m * c1[0] + (1 - m) * c1[0] * c1[0] * exp((1 - c1[0] * c1[0]) / 4.0);
	c1[1] = -oldx + f1;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 26
	Marcus August 2021
***************************************************************************/

int	DoMiraMap26(void)

    {
    double	t, c1[3], a, s, m, f1, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    m = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f1 = m * oldx + 2 * (1 - m) * oldx * oldx / (1 + oldx * oldx);
	c1[0] = oldy + a * (1 - s * oldy * oldy) * oldy + f1;
	f2 = m * c1[0] + 2 * (1 - m) * c1[0] * c1[0] / (1 + c1[0] * c1[0]);
	c1[1] = -oldx + f2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 29
	Marcus August 2021
***************************************************************************/

int	DoMiraMap29(void)

    {
    double	t, c1[3], a, s, m, f1, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    m = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f1 = m * oldx + 2 * (1 - m) * oldx * oldx / (1 + oldx * oldx);
	c1[0] = oldy + a * oldy * (1 - s * (oldy * oldy + 3 * oldx * oldx)) + f1;
	f2 = m * c1[0] + 2 * (1 - m) * c1[0] * c1[0] / (1 + c1[0] * c1[0]);
	c1[1] = -oldx + f2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map 30
	Marcus August 2021
***************************************************************************/

int	DoMiraMap30(void)

    {
    double	t, c1[3], a, s, m, f1, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    m = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f1 = m * oldy + (1 - m) * oldy * oldy * exp(-(oldy * oldy - 1.0) / 4.0);
	c1[0] = oldy + a * oldy * (1 - s * oldy * oldy) + f1;
	f2 = m * c1[1] + (1 - m) * c1[1] * c1[1] * exp(-(c1[1] * c1[1] - 4.0) / 4.0);
	c1[1] = -oldx + f2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira Map X
	Marcus August 2021
***************************************************************************/

int	DoMiraMapX(void)

    {
    double	t, c1[3], a, s, m, f1, f2, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    a = param[0];
    s = param[1];
    m = param[2];
    ColourFactor = param[3];

    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	f1 = m * oldx + 2 * (1 - m) * oldx * oldx / (1.0 + oldx * oldx);
	c1[0] = oldy + a * (1 - s * oldy * oldy) * oldy + f1;
	f2 = m * c1[0] + 2 * (1 - m) * c1[0] * c1[0] / (1.0 + c1[0] * c1[0]);
	c1[1] = -oldx + f2;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Mira-Gumowski Fractal map Images
***************************************************************************/

int	DoMiraGumowski(void)

    {
    double	i, c1[3], a, c, f, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    f = 0.0;

    a = param[0];
    c = 2.0 * (1.0 - a);
    ColourFactor = param[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = f+1*c1[1];
	f = a*c1[0]+c*c1[0]*c1[0]/(1+c1[0]*c1[0]);
	c1[1] = f-oldx;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(c*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Modifizierte Mira Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoModifizierteMira(void)

    {
    double	i, c1[3], a, b, c, d, fx, fxn, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	fx = a * oldx - (3 - a) / (a + exp(b * oldx));
	c1[0] = b * oldy + fx;
	fxn = a * c1[0] - (3 - a) / (a + exp(b * c1[0]));
	c1[1] = -oldx + fxn;
	c1[2] = 100.0 * (sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy))));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Multicavity Formations and Complexity Modulation in a Hyperchaotic Discrete System
	Shaobo He and Santo Banerjee 20 August 2017 received from the authors
	https://www.researchgate.net/publication/319203388_Multicavity_formations_and_complexity_modulation_in_a_hyperchaotic_discrete_system
***************************************************************************/

int	DoMulticavityFormations(void)

    {
    double	t, c1[3], cn[3], w, k, a, b, n, real, imag, ColourFactor;
    double	h, s1, s2, f, g;
    int		i;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = 1.0;	// z

    w = param[0];
    k = param[1];
    a = param[2];
    b = param[3];
    n = param[4];
    real = param[5];
    imag = param[6];
    ColourFactor = param[7];

    c1[0] = k * PI; c1[1] = k;
    cn[0] = c1[0]; cn[1] = c1[1];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	h = 0.0;
	for (i = 1; i <= n; i++)
	    {
	    s1 = fabs(n / a * c1[0] + 2 * i - 1) / (n / a * c1[0] + 2 * i - 1);
	    s2 = fabs(n / a * c1[0] - 2 * i + 1) / (n / a * c1[0] - 2 * i + 1);
	    h += (s1 + s2);
	    }

	g = a * h;
	cn[0] = a * sin(w * c1[1]) * sin(b / cn[0]);
	f = k * cn[0] * sin(cn[0]);
	cn[1] = a * sin(w * cn[0]) * sin(b / c1[1]) + f + g;
	cn[2] = real * cos(c1[1]) + imag * sin(c1[0]);
	c1[0] = cn[0]; c1[1] = cn[1]; c1[2] = cn[2];
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Multikey Image Encryption Algorithm Based on a High-Complexity Hyperchaotic System
	Ahmed Sh. Ahmed, Hussein A. Salah and Jalal Q.Jameel 29 September 2019.
	https://www.researchgate.net/publication/335881731
***************************************************************************/

int	DoMultikeyImageEncryptionAlgorithm(void)

    {
    double	i, c1[3], r, k, real, imag, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    r = param[0];
    k = param[1];
    real = param[2];
    imag = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 2 * sin(2 * oldy * (1 - oldy)) + sin(21 / (2 / oldx + (k / 2 / PI) * sin(oldx)));
	c1[1] = 21 * oldx * oldx * oldx + sin(21 / (r + (k * oldy + 3) * oldy * (1 - oldy)));
	c1[2] = real * sqrt(sqr(sin(oldx) + imag * sqr(cos(oldy))));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Multistability and Cyclic Attractors in Duopoly Games
	Gian Italo Bischi, Cristiana Mammana and Laura Gardini 30 April 1998
	http://www.mdef.it/uploads/tx_sibibtex/BiMaGaCSF00_01.pdf
***************************************************************************/

int	DoMultistabilityCyclicAttractors(void)

    {
    double	i, c1[3], mu1, mu2, real, imag, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    mu1 = param[0];
    mu2 = param[1];
    real = param[2];
    imag = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = mu1 * oldy * (1 - oldy);
	c1[1] = mu2 * oldx * (1 - oldx);
	c1[2] = real * sqrt(sqr(sin(oldx) + imag * sqr(cos(oldy))));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	New Algorithm of Encryption and Decryption of Images Using Chaotic Mapping
	SMusheer Ahmad and M. Shamsher Alam  2010
	https://scholar.google.com/scholar_lookup?title=A new algorithm of encryption and decryption of images using chaotic mapping&publication_year=2009&author=M. Ahmad&author=M.S. Alam
***************************************************************************/

int	DoNewAlgorithmEncryptionDecryptionMap(void)

    {
    double	i, c[3], m1, m2, c1, c2, real, imag, ColourFactor;

    c[0] = param[10]; // x
    c[1] = param[11]; // y
    c[2] = param[12]; // z

    m1 = param[0];
    m2 = param[1];
    c1 = param[2];
    c2 = param[3];
    real = param[4];
    imag = param[5];
    ColourFactor = param[6];
    totpasses = 10;

    InitFractalMap(c, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c[0];
	double oldy = c[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c[0] = m1 * oldx * (1 - oldx) + c1 * oldy * oldy;
	c[1] = m2 * oldy * (1 - oldy) + c2 * (oldx * oldx + oldx * oldy);
	c[2] = real * sqrt(sqr(sin(c1*oldx) + imag * sqr(cos(c2*oldy))));
	if (DisplayFractalMap(c, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Novel Two Dimensional Fractional-Order Discrete Chaotic Map and its Application to Image Encryption
	Zeyu Liu and Tiecheng Xia 19 July 2017
	https://reader.elsevier.com/reader/sd/pii/S2210832717301618?token=3BDA6F40B8C8036A722561503EE6E2E5EF3C2AA0B802102BA814BCDEABE9C97EB5D2B8BEB16E767C77D52D0ABC5B12F9
***************************************************************************/

int	DoNovel2DFractionalOrderDiscreteChaoticMap(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * cos(oldy);
	c1[1] = b * oldx * sin(oldy);
	c1[2] = 100.0 * (sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy))));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	On a Family of Maps with Multiple Chaotic Attractors
	Hendrik Richter 2008
	https://www.researchgate.net/profile/Hendrik-Richter/publication/222425804
***************************************************************************/

int	DoOnFamilyMaps(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx * (1 - oldx * oldx) * exp(-oldx * oldx) + b * oldy;
	c1[1] = oldx;
	c1[2] = 100.0 * (sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy))));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	On the Dynamics, Control and Synchronisation of Fractional-Order Ikeda Map - modified
	Adel Ouannas, Amina-Aicha Khennaoui, Zaid Odibat, Viet-Thanh Phame and Giuseppe Grassi 1 April 2019
	Sent by one of the authors  Zaid Odibat
***************************************************************************/

int	DoOnDynamicsControlSynchronisationFractionalOrderIkedaMap(void)

    {
    double	t, c1[3], delta, real, imag, phi, ColourFactor, type;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    delta = param[0];
    real = param[1];
    imag = param[2];
    ColourFactor = param[3];
    type = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	phi = 0.4 + type * 6 / (1 + oldx * oldx + oldy * oldy);
	c1[0] = 1 + (oldx * cos(phi) - oldy * sin(phi)) * delta;
	c1[1] = (oldx * sin(phi) + oldy * cos(phi)) * delta - oldy;
	c1[2] = real * cos(oldy) + imag * sin(oldx);
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	On the Three-Dimensional Fractional-Order Henon Map with Lorenz-Like Attractors
	Aicha-Amina Khennao, Adel Ouannas Zaid Odibat, Viet-Thanh Pham and Giuseppe Grassi September 28, 2019
	https://www.researchgate.net/requests/r83560251
***************************************************************************/

int	DoOn3DFractionalOrderHenonMap(void)

    {
    double	t, c1[3], b, m1, m2, ColourFactor, type;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    b = param[0];
    m1 = param[1];
    m2 = param[2];
    ColourFactor = param[3];
    type = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = m1 + b * oldz + m2 * oldy - oldx * oldx;
	c1[1] = oldx;
	c1[2] = oldy;
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Parameter Identification for Discrete Memristive Chaotic Map using Adaptive Differential Evolution Algorithm - I
	Yuexi Peng, Shaobo He and Kehui Sun
	Sent to ManpWIN by Shaobo He through https://www.researchsquare.com/article/rs-720732/v1.pdf
***************************************************************************/

int	DoParameterIdentificationMap(void)

    {
    double	t, c1[3], k, e, ColourFactor;

    c1[0] = param[10];	// x
    c1[1] = param[11];	// y
    c1[2] = param[12];	// z

    k = param[0];
    e = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (t = 0; t < iterations; t++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(t * totpasses / iterations);
	c1[0] = oldx - k * oldx * cos(oldy);
	c1[1] = oldx + e * oldx;
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)t % threshold), t, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Peter de Jong Fractal Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoPeterDeJong(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(a * oldy) - cos(b * oldx);
	c1[1] = sin(c * oldx) - cos(d * oldy);
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Pi Observer Design for a Class of Nondifferentially Flat Systems Map
	UAN PABLO FLORES-FLORES and RAFAEL MARTINEZ-GUERRA 2019
	http://scholar.google.com.br/scholar_url?url=https://www.amcs.uz.zgora.pl/%3Faction%3Ddownload%26pdf%3DAMCS_2019_29_4_3.pdf&hl=pt-BR&sa=X&d=6330806049912570308&scisig=AAGBfm2faj5RVEWld9WiUps14-VbDy-kwQ&nossl=1&oi=scholaralrt
***************************************************************************/

int	DoPiObserverDesign(void)

    {
    double	i, c1[3], g, s, a, u, f, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    g = param[0];
    s = param[1];
    a = param[2];
    u = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	f = exp(6.0 * sin(i));
	c1[0] = oldy;
	c1[1] = g - s * a * u / oldz;
	c1[2] = -s * u - f;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Pickover Fractal Map Images
***************************************************************************/

int	DoPickover(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(a*c1[1])-c1[2]*cos(b*oldx);
	c1[1] = c1[2]*sin(c*oldx)-cos(d*c1[1]);
	c1[2] = sin(oldx);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Polynom Fractal Map Type A Images
***************************************************************************/

int	DoPolynomA(void)

    {
    double	i, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a+oldy-c1[2]*oldy;
	c1[1] = b+c1[2]-oldx*c1[2];
	c1[2] = c+oldx-oldy*oldx;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Polynom Fractal Map Type B Images
***************************************************************************/

int	DoPolynomB(void)

    {
    double	i, c1[3], a, b, c, d, e, f, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    e = param[4];
    f = param[5];
    ColourFactor = param[6];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a+oldy-c1[2]*(b+oldy);
	c1[1] = c+c1[2]-oldx*(d+c1[2]);
	c1[2] = e+oldx-oldy*(f+oldx);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Popcorn 2 Fractal Map
	Meyer 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoPopcorn2(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    double oldx = c1[0];
    double oldy = c1[1];

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * oldx + oldy;
	c1[1] = b + oldx * oldx;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	oldx -= a * sin(oldy + tan(b * oldy));
	oldy -= a * sin(oldx + tan(b * oldx));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Quadruple Two Fractal Map Images
***************************************************************************/

int	DoQuadruple2(void)

    {
    double	i, c1[3], a, b, c, r, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	r = atan(fabs(pow(c * oldx - b, 2)));
	c1[0] = c1[1] - sin(log(fabs(b * oldx - c))) * r;
	c1[1] = a - oldx;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Quadruptwo from Fractint (plus variations)
	https://archive.org/details/frain200
***************************************************************************/

int	DoQuadruple2Fractint(void)

    {
    double	i, c1[3], a, b, c, tmp, ColourFactor;
    int		MapType;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    MapType = (int)param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	switch (MapType)
	    {
	    case 0:
		tmp = oldy - sign(oldx) * sin(log(fabs(b*(oldx)-c))) * atan(sqr(log(fabs(c*oldx - b))));
		break;
	    case 1:
		tmp = oldy - sign(oldx) * sinh(log(fabs(b*(oldx)-c))) * atan(sqr(log(fabs(c*oldx - b))));
		break;
	    case 2:
		tmp = oldy - sign(oldx) * exp(log(fabs(b*(oldx)-c))) * atan(sqr(log(fabs(c*oldx - b))));
		break;
	    case 3:
		tmp = oldy - sign(oldx) * tan(log(fabs(b*(oldx)-c))) * atan(sqr(log(fabs(c*oldx - b))));
		break;
	    default:
		tmp = oldy - sign(oldx) * sin(log(fabs(b*(oldx)-c))) * atan(sqr(log(fabs(c*oldx - b))));
		break;
	    }
	c1[1] = a - oldx;
	c1[0] = tmp;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Robust Chaos of Cubic Polynomial Discrete Maps with Application to Pseudorandom Number Generators
	Dandan Han, Lequan Min, Hongyan Zang, and Xiuping Yang 7 April 2019
	https://www.hindawi.com/journals/mpe/2019/8250903/
***************************************************************************/

int	DoRobustChaosCubicPolynomialDiscreteMaps(void)

    {
    double	i, c1[3], de, ni, nu, p1, p2, p3, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    ni = param[0];
    p2 = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    nu = -27.0 * (1 - sqrt(1.0 - 4.0 / ni));
    de = ni * (4.0 - 18.0 / ni - 4.0 * pow((1.0 - 3.0 / ni), 1.5));
    p1 = nu / de;
    p3 = p2 * p2 / ni / p1;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	double oldz = c1[2];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = p3 * oldx * oldx * oldx + p2 * oldx * oldx + p1 * oldx;
	c1[1] = oldx - 2.7 * sin(oldy) * sin(oldz);				// '2.7' produces more dots than the original '2'
	c1[2] = sin(oldx) + 2 * sin(oldy * oldz);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Robust Compression-Encryption of Images Based on SPIHT Coding and Fractional-Order Discrete-Time Chaotic System
	Mourad Lahdira, Hamid Hamichea, Sarah Kassima, Mohamed Tahanouta, Karim Kemihb and  Sid-Ali Addouchec 14 August 2018
	Sent directly by the author Karim Kemihb
***************************************************************************/

int	DoRobustCompressionEncryptionImagesBasedSPIHTCoding(void)

    {
    double	i, c1[3], alpha1, alpha2, alpha3, a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    alpha1 = param[0];
    alpha2 = param[1];
    alpha3 = param[2];
    a = param[3];
    b = param[4];
    ColourFactor = param[5];
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
	c1[0] = a - oldy * oldy - b * oldz + (alpha1 - 1.0) * oldx;
	c1[1] = oldx + (alpha2 - 1.0) * oldy;
	c1[2] = oldy + (alpha3 - 1.0) * oldz;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Route to Chaos Via Strange Non-Chaotic Attractors by Reshaping Periodic Excitations
	R. Chacon and A. Martinez Garcia-Hoz 22 October 2001
	https://www.researchgate.net/publication/230978460
***************************************************************************/

int	DoRouteChaosViaStrangeNonChaoticAttractors(void)

    {
    double	i, c1[3], a, b, m, w, cnt0m, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    m = param[2];
    w = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cnt0m = cos(w * oldy) + 0.25 * m * (w * oldy - sin(w * oldy) * cos(w * oldy)) * sin(w * oldy);
	c1[0] = sin(oldx) * (a * cnt0m + b);
	c1[1] = fmod((oldy + 2 * PI * w), (2 * PI));
	c1[2] = sqrt(sqr(sin(oldx*1.5)) + sqr(cos(oldy*1.4)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	SF-SIMM High-Dimensional Hyperchaotic Map and its Performance Analysis
	Wenhao Liu, Kehui Sun and Shaobo He 3 June 2017
	https://www.researchgate.net/publication/317935319_SF-SIMM_high-dimensional_hyperchaotic_map_and_its_performance_analysis
***************************************************************************/

int	DoSFSIMMHighDimensionalHyperchaoticMap(void)

    {
    double	i, c1[3], a, w, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    w = param[1];
    c = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a * sin(w * oldy) * sin(c / oldx);
	c1[1] = a * sin(w * c1[0]) * sin(c / oldy);
	c1[2] = sqrt(sqr(sin(oldx*1.5)) + sqr(cos(oldy*1.4)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Shadowing in Hidden Attractors - Equation IV
	N. K. Kamal, V. Varshney, M. D. Shrimali, A. Prasad, N. V. Kuznetsov and G. A. Leonov 5 January 2018
	https://www.researchgate.net/publication/322284119
***************************************************************************/

int	DoShadowingHiddenAttractorsEquationIV(void)

    {
    double	i, c1[3], ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

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
	c1[0] = oldy;
	c1[1] = oldz;
	c1[2] = oldx + 0.2 * oldx * oldx + 0.8 * oldy * oldy - oldx * oldz - 0.1;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Shadowing in Hidden Attractors - Equation V
	N. K. Kamal, V. Varshney, M. D. Shrimali, A. Prasad, N. V. Kuznetsov and G. A. Leonov 5 January 2018
	https://www.researchgate.net/publication/322284119
***************************************************************************/

int	DoShadowingHiddenAttractorsEquationV(void)

    {
    double	i, c1[3], ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

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
	c1[0] = oldy;
	c1[1] = oldz;
	c1[2] = 1.0 - 0.8 * /*fabs*/(oldy) + oldx * (oldx - oldz - 1);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sierpinski Polygon Fractal Map Images
***************************************************************************/

int	DoSierpinskiPoly(void)

    {
    double	i, c1[3], ScaleFactor, x1[12], y1[12], angle, RealAngle, ImagAngle, ColourFactor;
    int		order, r, j;

//    xscale = (double) (xdots - 1) / (mandel_width * ScreenRatio);
//    yscale = (double) (ydots - 1) / mandel_width;

    srand((unsigned)time(NULL));			// randomize things
    ScaleFactor = param[0];
    order = (int)param[1];
    RealAngle = param[2];
    ImagAngle = (int)param[3];
    ColourFactor = param[4];
    if (order < 3)
	order = 3;
    if (order > 10)
	order = 10;

    angle = 2.0 * PI / (double)order;
    for (j = 0; j < order; j++)
	{
	x1[j] = cos(angle * j);
	y1[j] = sin(angle * j);
	}
    c1[0] = x1[1];	// set x = to start coordinates
    c1[1] = y1[1];	// set y = to start coordinates
    c1[2] = 1.0;	// z

    totpasses = 10;
    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	r = (int)((double)rand() / 32768.0 * (double) order) % order;
	c1[0] = (c1[0] - x1[r]) * ScaleFactor + x1[r];
	c1[1] = (c1[1] - y1[r]) * ScaleFactor + y1[r];
	c1[2] = sqrt(sqr(sin(oldx*1.5)) + sqr(cos(oldy*1.4)));
	if (DisplayFractalMap(c1, ((DWORD)(r * 40 + 40)), r * 40 + 40, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Some Open Problems in Chaos Theory and Dynamics - Equation 2
	Zeraoulia Elhadj and J. C. Sprott June 2011
	https://www.researchgate.net/publication/228935174
***************************************************************************/

int	DoSomeOpenProblemsChaosTheory2(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = 1 / (0.1 + oldx * oldx) - a * oldy;
	c1[1] = 1 / (0.1 + oldy * oldy) + b * oldx;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Some Open Problems in Chaos Theory and Dynamics - Equation 3
	Zeraoulia Elhadj and J. C. Sprott June 2011
	https://www.researchgate.net/publication/228935174
***************************************************************************/

int	DoSomeOpenProblemsChaosTheory3(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = -a * oldx / (1 + oldy * oldy);
	c1[1] = oldx + b * oldy;
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Sprott FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/
/*
int	DoSprottFractaLMap12(void)

    {
    double	i, c1[3], a[48], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, FractalMapSpecific[subtype].function);
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
*/

/**************************************************************************
	Sprott FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/
/*
int	DoSprottFractaLMap20(void)

    {
    double	i, c1[3], a[48], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, FractalMapSpecific[subtype].function);
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
*/

/**************************************************************************
	Sprott FractaL Map Generator
	23/09/2019
	Sprott’s book + Marcus
***************************************************************************/

/*
int	DoSprottFractaLMap30(void)

    {
    double	i, c1[3], a[48], ColourFactor;
    char	s[120];
    int		j, length;

    strcpy(s, FractalMapSpecific[subtype].function);
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



//	x(4) = A(1) + A(2) * x(1) + A(3) * x(1) * x(1) + A(4) * x(1) * x(2) + A(5) * x(1) * x(3) + A(6) * x(2) + A(7) * x(2) * x(2) + A(8) * x(2) * x(3) + A(9) * x(3) + A(10) * x(3) * x(3);
//	x(5) = A(11) + A(12) * x(1) + A(13) * x(1) * x(1) + A(14) * x(1) * x(2) + A(15) * x(1) * x(3) + A(16) * x(2) + A(17) * x(2) * x(2) + A(18) * x(2) * x(3) + A(19) * x(3) + A(20) * x(3) * x(3);
//	x(6) = A(21) + A(22) * x(1) + A(23) * x(1) * x(1) + A(24) * x(1) * x(2) + A(25) * x(1) * x(3) + A(26) * x(2) + A(27) * x(2) * x(2) + A(28) * x(2) * x(3) + A(29) * x(3) + A(30) * x(3) * x(3);

	c1[0] = a[0]  + a[1]  * oldx + a[2]  * oldx * oldx + a[3]  * oldx * oldy + a[4]  * oldx * oldz + a[5]  * oldy + a[6]  * oldy * oldy + a[7]  * oldy * oldz + a[8]  * oldz + a[9]  * oldz * oldz;
	c1[1] = a[10] + a[11] * oldx + a[12] * oldx * oldx + a[13] * oldx * oldy + a[14] * oldx * oldz + a[15] * oldy + a[16] * oldy * oldy + a[17] * oldy * oldz + a[18] * oldz + a[19] * oldz * oldz;
	c1[2] = a[20] + a[21] * oldx + a[22] * oldx * oldx + a[23] * oldx * oldy + a[24] * oldx * oldz + a[25] * oldy + a[26] * oldy * oldy + a[27] * oldy * oldz + a[28] * oldz + a[29] * oldz * oldz;
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }
*/

/**************************************************************************
	Square Hénon Fractal Map
	Meier 2001
	http://www.3d-meier.de/ "Darstellung von Attraktoren mit Cinema" 3.11
	This attractor is a bit different from the other attractors. If we carry out the 
	calculation as usual we only get a simple circle. Changing the starting values ​​
	changes the size of the circle. At certain starting values ​​
	we get several circles. If the starting values ​​
	are too big, the points will go to infinity.
	The following figures show the results for different starting values. 
	The constant a is the same for all images (a = 1.524). 
***************************************************************************/

int	DoSquareHenon(void)

    {
    double	i, c1[3], b, c, HenonA, HenonStep, cosa, sina, HenonXStart, HenonYStart, ColourFactor;
    int		j, HenonPoints;

    c1[0] = HenonXStart = param[10];	// x
    c1[1] = HenonYStart = param[11];	// y
    c1[2] = param[12];			// z

    HenonA = param[0];
    HenonPoints = (int)param[1];
    HenonStep = param[2];
    b = param[3];
    c = param[4];
    ColourFactor = param[5];

    cosa = cos(HenonA);
    sina = sin(HenonA);

    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	for (j = 0; j < HenonPoints; ++j)
	    {
	    if ((oldx < MAXREAL && oldx > - MAXREAL) && (oldy < MAXREAL && oldy > -MAXREAL))
		{
		double	temp = oldy - oldx * oldx;
		c1[0] = oldx * cosa - temp * sina;
		c1[1] = oldx * sina + temp * cosa;
		c1[2] = sqrt(sqr(sin(b*oldx)) + sqr(cos(c*oldy)));
		if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
		    break;
		oldx = c1[0];
		oldy = c1[1];
		}
	    }
	c1[0] = HenonXStart + HenonStep * i;
	c1[1] = HenonYStart + HenonStep * i;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Strick Fractal Map
	Meier 2001
	http://www.3d-meier.de/ "Darstellung von Attraktoren mit Cinema" 3.13
***************************************************************************/

int	DoStrick(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);							// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];
	if (user_data(GlobalHwnd) == -1)					// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	if (fabs(oldx) < 0.5)
	    {
	    c1[0] = (2 - a) * oldx - b * oldy;
	    c1[1] = -b * oldx + a * oldy;
	    }
	else
	    {
	    c1[0] = a * oldx - b * oldy + (1-a) * sign(oldx);
	    c1[1] = b * oldx + a * oldy - b * sign(oldx);
	    }
	c1[2] = sqrt(sqr(sin(c*oldx)) + sqr(cos(d*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 2, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Svensson Attractor Fractal Map Images
***************************************************************************/


int	DoSvensson(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = d * sin(a*oldx) - sin(b*oldy);
	c1[1] = c * cos(a*oldx) + cos(b*oldy);
	c1[2] = sqrt(sqr(d * sin(a*oldx)) + sqr(c * cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Swirl from The Pattern Book by Clifford A. Pickover
	Paul D. Bourke 1995
	The Pattern Book : fractals, art, and nature / editor, Clifford A. Pickover.
***************************************************************************/


int	DoSwirlPickover(void)

    {
    double	i, c1[3], a11, a12, a21, a22, real, imag, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a11 = param[0];
    a12 = param[1];
    a21 = param[2];
    a22 = param[3];
    real = param[4];
    imag = param[5];
    ColourFactor = param[6];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sin(a11 * oldy) - cos(a12 * oldx);
	c1[1] = sin(a21 * oldx) - cos(a22 * oldy);
	c1[2] = imag * sin(oldx) + real * cos(oldy);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Symmetric Key Encryption Based on Rotation-Translation Equation
	Borislav Stoyanov and Gyurhan Nedzhibov 2 January 2020
	www.mdpi.com/journal/symmetry.....doi:10.3390/sym12010073
***************************************************************************/


int	DoSymmetricKeyEncryption(void)

    {
    double	i, c1[3], a, b, c, d, tt, real, imag, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    real = param[2];
    imag = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    c = a / 2; d = a;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	tt = c + d / (oldx * oldx + oldy * oldy);
	c1[0] = a + b * (oldx * cos(tt) - oldy * sin(tt));
	c1[1] = b * (oldx * sin(tt) + oldy * cos(tt));
	c1[2] = imag * sin(oldx) + real * cos(oldy);
	if (i > 200)							// first 200 are outliers
	    if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Synchronisation of Chaotic Systems
	Louis M. Pecora and Thomas L. Carroll 16 April 2015
	https://aip.scitation.org/doi/full/10.1063/1.4917383
***************************************************************************/


int	DoSynchronisationChaoticSystemsMap(void)

    {
    double	i, c1[3], a, b, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    ColourFactor = param[3];
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
	c1[0] = a * c1[0] * (1 - c1[0]);
	c1[1] = b * c1[0] * (1 - c1[1]);
	c1[2] = c * c1[1] * (1 - c1[2]);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	The Simplest Case of a Strange Attractor Map
	G.M.Zaslavsky 1986
	http://www.scholarpedia.org/article/Zaslavsky_web_map
***************************************************************************/


int	DoTheSimplestCaseStrangeAttractorsMap(void)

    {
    double	i, c1[3], a, k, q, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    k = param[0];
    q = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	a = 2 * PI / q;
	c1[0] = (oldx + k * sin(oldy)) * cos(a) + oldy * sin(a);
	c1[1] = -(oldx + k * sin(oldy)) * sin(a) + oldy * cos(a);
	c1[2] = sqrt(sqr(sin(oldx)) + sqr(cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Three-Dimensional Hénon-Like Maps and Wild Lorenz-Like Attractors
	S. V. Gonchenko, I. I. Ovsyannikov, C. Simo and D. Turaev February 15, 2005
	https://wwwf.imperial.ac.uk/~dturaev/mypapers/bc2005.pdf
***************************************************************************/

int	DoThreeDimensionalHenonLikeMaps(void)

    {
    double	i, c1[3], cn[3], m1, m2, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    m1 = param[0];
    m2 = param[1];
    b = param[2];
    ColourFactor = param[3];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	cn[0] = c1[1];
	cn[1] = c1[2];
	cn[2] = m1 + b * c1[0] + m2 * c1[1] - c1[2] * c1[2];
	c1[0] = cn[0]; c1[1] = cn[1]; c1[2] = cn[2];
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	ThreePly - Taken from Fractint
	Fractint 2000
	https://archive.org/details/frain200
***************************************************************************/

int	DoThreePlyMap(void)

    {
    double	i, c1[3], a, b, c, temp, ColourFactor;
    int		MapType;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    MapType = (int)param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	switch (MapType)
	    {
	    case 0:
		temp = oldy - sign(oldx) * (fabs(sin(oldx) * cos(b) + c - oldx * sin(a + b + c)));
		break;
	    case 1:
		temp = oldy - sign(oldx) * (fabs(cos(oldx) * tan(b) + c - sin(oldx) * sin(sqrt(fabs(a + b + c)))));
		break;
	    case 2:
		temp = oldy - sign(oldx) * (fabs(sin(oldx) * cos(b) + c - oldx * tan(a + b + c)));
		break;
	    case 3:
		temp = oldy - sign(oldx) * (fabs(cos(oldx) * cos(b) + c - oldx * tan(a + b + c)));
		break;
	    case 4:
		temp = oldy - sign(oldx) * (fabs(cos(sqrt(fabs(oldx))) * tan(b) + c - sin(sqrt(fabs(oldx))) * sin(sqrt(fabs(a + b + c)))));
		break;
	    default:
		temp = oldy - sign(oldx) * (fabs(sin(oldx) * cos(b) + c - oldx * sin(a + b + c)));
		break;
	    }
	c1[1] = a - oldx;
	c1[0] = temp;
	c1[2] = sqrt(sqr(b*sin(oldx)) + sqr(a*cos(oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Tinkerbell fractal map Images
***************************************************************************/

int	DoTinkerbell(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = sqr(oldx)-sqr(c1[1])+a*oldx+b*oldy;
	c1[1] = 2.0*oldx*oldy+c*oldx+d*oldy;
	c1[2] = sqrt(sqr(d * sin(a*oldx)) + sqr(c * cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Torus and Hyperchaos in 3D Lotka-Volterra Map
	Sishu Shankar Muni August 28, 2024
	https://arxiv.org/pdf/2408.15054.....https://doi.org/10.48550/arXiv.2408.15054
	Contacting author: sishushankarmuni@gmail.com
***************************************************************************/

int	DoTorusHyperchaos3DLotkaVolterraMap(void)

    {
    double	i, c1[3], a, b, r, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    r = param[2];
    ColourFactor = param[3];
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
	c1[0] = oldx + r * oldx * (1 - oldx - a * oldy - b * oldz);
	c1[1] = oldy + r * oldx * (1 - b * oldx - oldy - a * oldz);
	c1[2] = oldz + r * oldx * (1 - a * oldx - b * oldy - oldz);
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Transformations of Spike and Burst Oscillations in the Stochastic Rulkov Model
	Irina Bashkirtseva and Lev Ryashko May 2023
	https://www.sciencedirect.com/science/article/abs/pii/S0960077923003156.....https://doi.org/10.1016/j.chaos.2023.113414
	Message to: lev.ryashko@urfu.ru
***************************************************************************/

int	DoTransformationsSpikeBurstOscillations(void)

    {
    double	i, c1[3], a, b, c, s, start, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    s = param[0];
    a = param[1];
    b = param[2];
    c = param[3];
    start = param[4];
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = a / (1 + oldx * oldx) + oldy;
	c1[1] = oldy - s * (oldx + 1);
	c1[2] = sqrt(sqr(s * sin(a*oldx)) + sqr(c * cos(b*oldy)));
	if (i > start)
	    if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
		break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Two-Variable Boosting Bifurcation in a Hyperchaotic Map and Its Hardware Implementation
	Mengjiao Wang, Mingyu An, Xinan Zhang and Herbert Ho-Ching Iu 14 January 2019
	https://assets.researchsquare.com/files/rs-1786505/v1_covered.pdf?c=1657814400
***************************************************************************/

int	DoTwoVariableBoostingBifurcationMap(void)

    {
    double	i, c1[3], a, b, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    ColourFactor = param[2];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = oldx + a * sin(oldy);
	c1[1] = b * sin(oldx);
	c1[2] = sqrt(sqr(sin(a*oldx)) + sqr(cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
    PlotExtras();
    return 0;
    }

/**************************************************************************
	Ushiki fractal map Images
***************************************************************************/

int	DoUshiki(void)

    {
    double	i, c1[3], a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    b = param[1];
    c = param[2];
    d = param[3];
    ColourFactor = param[4];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = (a - oldx - b * oldy) * oldx;
	c1[1] = (c - oldy - d * oldx) * oldy;
	c1[2] = sqrt(sqr(d * sin(a*oldx)) + sqr(c * cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Variety of Strange Pseudohyperbolic Attractors in Three-Dimensional Generalized Hénon Maps
	A.S. Gonchenko and S.V. Gonchenko 19 april 2016
	https://arxiv.org/pdf/1510.02252.pdf
***************************************************************************/

int	DoVarietyStrangePseudohyperbolicAttractors(int variety)

    {
    double	i, c1[3], a, c, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    a = param[0];
    c = param[1];
    ColourFactor = param[2];
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
	c1[0] = oldy;
	c1[1] = oldz;
	switch (variety)
	    {
	    case 0:
		c1[2] = 0.72 * oldx + c * oldy + a * oldz - 1.45 * oldz * oldz + 0.515 * oldy * oldz - oldy * oldy;
		break;
	    case 1:
		c1[2] = 0.5 * oldx + c * oldy + a * oldz - 2.25 * oldz * oldz * oldz - 2 * oldy * oldy * oldy;
		break;
	    case 2:
		c1[2] = 0.7 * oldx + a * oldz + c * oldy - 1.45 * oldz * oldz - oldy * oldy;
		break;
	    case 3:
		c1[2] = 0.5 * oldx + a * oldz + c * oldy - oldy * oldy * oldy;
		break;
	    case 4:
		c1[2] = 0.5 * oldx + a * oldz + c * oldy + 0.5 * oldz * oldz * oldz - 6 * oldz * oldy * oldy + 0.5 * oldy * oldy * oldy;
		break;
	    }
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

int	DoVarietyStrangePseudohyperbolicAttractorsI(void)
    {
    return DoVarietyStrangePseudohyperbolicAttractors(0);
    }

int	DoVarietyStrangePseudohyperbolicAttractorsII(void)
    {
    return DoVarietyStrangePseudohyperbolicAttractors(1);
    }

int	DoVarietyStrangePseudohyperbolicAttractorsIII(void)
    {
    return DoVarietyStrangePseudohyperbolicAttractors(2);
    }

int	DoVarietyStrangePseudohyperbolicAttractorsIV(void)
    {
    return DoVarietyStrangePseudohyperbolicAttractors(3);
    }

int	DoVarietyStrangePseudohyperbolicAttractorsV(void)
    {
    return DoVarietyStrangePseudohyperbolicAttractors(4);
    }


/**************************************************************************
	Wu-Yang Fractal Map
	Meier 2001
	http://www.3d-meier.de/
***************************************************************************/

int	DoWuYang(void)

    {
    double	i, c1[3], r, a, b, c, d, ColourFactor;

    c1[0] = param[10]; // x
    c1[1] = param[11]; // y
    c1[2] = param[12]; // z

    r = param[0];
    a = param[1];
    b = param[2];
    c = param[3];
    d = param[4];
    ColourFactor = param[5];
    totpasses = 10;

    InitFractalMap(c1, 3);						// pass in number of dimensions
    for (i = 0; i < iterations; i++)
	{
	double oldx = c1[0];
	double oldy = c1[1];

	if (user_data(GlobalHwnd) == -1)				// user pressed a key?
	    return -1;
	curpass = (int)(i * totpasses / iterations);
	c1[0] = r * (3 * oldy + 1.0) * oldx * (1.0 - oldx);
	c1[1] = r * (3 * c1[0] + 1.0) * oldy * (1.0 - oldy);
	c1[2] = sqrt(sqr(d * sin(a*oldx)) + sqr(c * cos(b*oldy)));
	if (DisplayFractalMap(c1, ((DWORD)i % threshold), i, 3, ColourFactor) < 0)
	    break;
	}
        PlotExtras();
    return 0;
    }

/**************************************************************************
	Common init Routines
***************************************************************************/

void	InitFractalMap(double c1[], int dimensions)
    {
    OscProcess.InitOscProc(dimensions, mandel_width, ScreenRatio, xdots, ydots, type, subtype, OscAnimProc, &xAxis, &yAxis, &zAxis, BOUNDARY, &Dib, c1, PerspectiveFlag, iterations, OscBackGround, &x_rot, &y_rot, &z_rot, &RemoveHiddenPixels, GlobalHwnd, wpixels, &TrueCol);
    }

/**************************************************************************
	Common Display Routines
***************************************************************************/

int	DisplayFractalMap(double c1[], DWORD colour, double i, int dimensions, double ColourFactor)
    {
    return OscProcess.DisplayOscillator(c1, c1, dt, colour, i, dimensions, FALSE, 1, 1, hor, vert, VertBias, zBias, OscAnimProc, CoordSystem, threshold, ColourFactor, 0);
    }

/**************************************************************************
	Compare function for Fractal Map name sort
**************************************************************************/

int	CompareFracMap(int	*p1, int *p2)

    {
    char    *a1, *a2;

    a1 = FractalMapSpecific[*p1].name;
    a2 = FractalMapSpecific[*p2].name;
    if (*a1 == '\"')			    // ignore " character
	a1 = a1++;
    if (*a2 == '\"')
	a2 = a2++;
    return (_stricmp(a1, a2));
    }

/**************************************************************************
	General Fractal Map
***************************************************************************/

int	setup_FractalMaps(void)
    {
    int	    i;

    OscillatorNum = 0;
			// we'd better count how many records we have
    while (FractalMapSpecific[OscillatorNum].name)
	OscillatorNum++;
    for (i = 0; i < OscillatorNum; i++) 
	OscPtrArray[i] = i;
    qsort((void *)OscPtrArray, (size_t)OscillatorNum, sizeof(OscPtrArray[0]), (int (__cdecl *)(const void *, const void *)) CompareFracMap);
#ifdef	PRINTOSC
    PrintOsc(OscillatorNum);		// used for listing oscillator names in d:\temp\OscDump.txt
#endif
    return 0;
    }

int	DoFractalMaps(void) 

    {
    return (FractalMapSpecific[subtype].calctype());
    }


