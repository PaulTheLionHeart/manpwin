/*
    jb.cpp - Julibrot Fractal Stuff.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include "OtherFunctions.h"
#include "Manp.h"
#include "resource.h"
#include "fractalp.h"
#include "SafeStrings.h"

extern	int	user_data(HWND hwnd);

static const JulibrotDefault JulibrotDefaults[21] =
    {
    {},		// 0
    {},		// 1

    { -0.830, -0.250, -0.830,  0.250 }, // 2
    { -0.550, -0.220, -0.550,  0.220 },	// 3
    { -1.060, -0.258, -1.060,  0.258 },	// 4
    { -0.780, -0.240, -0.780,  0.240 },	// 5
    { -0.800, -0.324, -0.800,  0.324 },	// 6
    { -0.840, -0.156, -0.840,  0.156 },	// 7
    { -0.840, -0.220, -0.840,  0.220 },	// 8
    { -0.900, -0.250, -0.900,  0.250 },	// 9
    { -0.860, -0.190, -0.860,  0.190 },	// 10
    { -0.960, -0.236, -0.960,  0.236 },	// 11
    { -0.860, -0.420, -0.860,  0.420 },	// 12
    { -1.000, -0.260, -1.000,  0.260 },	// 13
    { -0.880, -0.358, -0.880,  0.358 },	// 14
    { -1.020, -0.294, -1.020,  0.294 },	// 15
    { -0.860, -0.318, -0.860,  0.318 },	// 16
    { -0.980, -0.124, -0.980,  0.124 },	// 17
    { -0.880, -0.208, -0.880,  0.208 },	// 18
    { -0.980, -0.130, -0.980,  0.130 },	// 19
    { -0.920, -0.234, -0.920,  0.234 }	// 20
    };

const JulibrotDefault& GetJulibrotDefault(int Power)
    {
    if (Power < 2)
	Power = 2;
    else if (Power > 20)
	Power = 20;

    return JulibrotDefaults[Power];
    }

#ifdef JULIBROT_OPTIMISER

double COtherFunctions::FindJulibrotDefaultX(int power, double yRange)
    {
    double BestX = -0.58;
    double BestScore = -1.0;

    for (double x = -1.5; x <= 0.0; x += 0.02)
	{
	JulibrotScore Result = ScoreJulibrotSlice(power, x, yRange);

	if (Result.Score > BestScore)
	    {
	    BestScore = Result.Score;
	    BestX = x;
	    }
	}

    return BestX;
    }

double COtherFunctions::FindJulibrotDefaultY(int power, double cx)
    {
    double BestY = 0.25;
    double BestScore = -1.0;

    for (double y = 0.10; y <= 0.40; y += 0.02)
	{
	JulibrotScore Result = ScoreJulibrotSlice(power, cx, y);

	if (Result.Score > BestScore)
	    {
	    BestScore = Result.Score;
	    BestY = y;
	    }
	}

    double Start = BestY - 0.03;
    double End = BestY + 0.03;

    if (Start < 0.02)
	Start = 0.02;

    for (double y = Start; y <= End; y += 0.002)
	{
	JulibrotScore Result = ScoreJulibrotSlice(power, cx, y);

	if (Result.Score > BestScore)
	    {
	    BestScore = Result.Score;
	    BestY = y;
	    }
	}

    return BestY;
    }

JulibrotScore COtherFunctions::ScoreJulibrotSlice(int power, double cx, double yRange)
    {
    const int TestWidth = 80;
    const int TestHeight = 50;
    const int TestZDots = JB.ZDots;
    const long TestThreshold = 250;

    long Hits = 0;
    long Samples = 0;

    double SumDepth = 0.0;
    double SumDepthSquared = 0.0;

    const int DepthBins = 16;
    long DepthHistogram[DepthBins] = {};

    int EdgeTransitions = 0;

    bool PreviousHit = false;
    bool HavePrevious = false;

    // Use a fixed Fractint-style vertical slice while testing cx.
    const double FromX = cx;
    const double ToX = cx;
    const double FromY = -yRange;
    const double ToY = yRange;
    const double DeltaMX = (ToX - FromX) / TestZDots;
    const double DeltaMY = (ToY - FromY) / TestZDots;

    for (int y = 0; y < TestHeight; y++)
	{
	HavePrevious = false;

	double yPixel;

	if (y < (TestHeight >> 1))
	    yPixel = ((double)y - ((TestHeight >> 1) - 1))
	    * (JB.ScreenHeight / TestHeight);
	else
	    yPixel = ((double)y - (TestHeight >> 1))
	    * (JB.ScreenHeight / TestHeight);

	for (int x = 0; x < TestWidth; x++)
	    {
	    double xPixel = -JB.ScreenWidth / 2.0 + (double)x * (JB.ScreenWidth / TestWidth);

	    double jx;
	    double jy;
	    double deltaJX;
	    double deltaJY;

	    JulibrotPerPixel(xPixel, yPixel, jx, jy, deltaJX, deltaJY);

	    deltaJX = (JB.ZDepth / JB.ScreenDist) * (JB.Eye.x - xPixel) * JB.XPerInch / TestZDots;
	    deltaJY = (JB.ZDepth / JB.ScreenDist) * (JB.Eye.y - yPixel) * JB.YPerInch / TestZDots;

	    double mx = FromX;
	    double my = FromY;

	    bool Hit = false;
	    int HitDepth = 0;

	    for (int zpixel = 0; zpixel < TestZDots; zpixel++)
		{
		double zx = jx;
		double zy = jy;

		long iteration;

		for (iteration = 0; iteration < TestThreshold; iteration++)
		    {
		    Complex z;

		    z.x = zx;
		    z.y = zy;

		    Complex p = z.CPolynomial(power);

		    // z = z^Power + c
		    zx = p.x + mx;
		    zy = p.y + my;

		    if (zx * zx + zy * zy > 4.0)
			break;
		    }

		if (iteration == TestThreshold)
		    {
		    Hit = true;
		    HitDepth = zpixel;
		    break;
		    }

		mx += DeltaMX;
		my += DeltaMY;
		jx += deltaJX;
		jy += deltaJY;
		}

	    Samples++;

	    if (Hit)
		{
		Hits++;

		double d = (double)HitDepth / TestZDots;
		SumDepth += d;
		SumDepthSquared += d * d;

		int Bin = HitDepth * DepthBins / TestZDots;

		if (Bin >= DepthBins)
		    Bin = DepthBins - 1;

		DepthHistogram[Bin]++;
		}

	    if (HavePrevious && Hit != PreviousHit)
		EdgeTransitions++;

	    PreviousHit = Hit;
	    HavePrevious = true;
	    }
	}

    JulibrotScore Result = {};

    if (Samples == 0)
	return Result;

    Result.Occupancy = (double)Hits / Samples;

    if (Hits > 1)
	{
	double Mean = SumDepth / Hits;
	double Variance = SumDepthSquared / Hits - Mean * Mean;

	if (Variance < 0.0)
	    Variance = 0.0;

	Result.DepthSpread = sqrt(Variance);
	}

    Result.EdgeScore = (double)EdgeTransitions / Samples;

    // Reject nearly blank or nearly solid candidates.
    if (Result.Occupancy < 0.03 || Result.Occupancy > 0.65) 
	{
	Result.Score = 0.0;
	return Result;
	}

    double OccupancyScore = 1.0 - fabs(Result.Occupancy - 0.30) / 0.30;

    if (OccupancyScore < 0.0)
	OccupancyScore = 0.0;

    double MeanDepth = 0.0;
    double DepthBalance = 0.0;
    double DepthEntropy = 0.0;

    if (Hits > 0)
	{
	for (int i = 0; i < DepthBins; i++)
	    {
	    if (DepthHistogram[i] > 0)
		{
		double p = (double)DepthHistogram[i] / Hits;
		DepthEntropy -= p * log(p);
		}
	    }

	DepthEntropy /= log((double)DepthBins);
	}

    if (Hits > 0)
	{
	MeanDepth = SumDepth / Hits;

	DepthBalance = 1.0 - fabs(MeanDepth - 0.5) / 0.5;

	if (DepthBalance < 0.0)
	    DepthBalance = 0.0;
	}

    // Reject slices which intersect almost entirely at the front.
    if (MeanDepth < 0.20)
	{
	Result.Score = 0.0;
	Result.MeanDepth = MeanDepth;
	Result.DepthEntropy = DepthEntropy;
	return Result;
	}

    long MidDepthHits = 0;

    for (int i = 4; i < 12; i++)
	MidDepthHits += DepthHistogram[i];

    double MidDepthFraction = Hits > 0 ?
	(double)MidDepthHits / Hits : 0.0;

    Result.MidDepthFraction = MidDepthFraction;


    double MidDepthScore =
	1.0 - fabs(MidDepthFraction - 0.50) / 0.50;

    if (MidDepthScore < 0.0)
	MidDepthScore = 0.0;


    Result.Score =
	Result.EdgeScore * 0.10 +
	Result.DepthSpread * 0.15 +
	OccupancyScore * 0.15 +
	DepthBalance * 0.15 +
	DepthEntropy * 0.10 +
	MidDepthScore * 0.35;
    Result.MeanDepth = MeanDepth;
    Result.DepthEntropy = DepthEntropy;

    return Result;
    }

#endif

void COtherFunctions::JulibrotPerPixel(double xPixel, double yPixel, double& jx, double& jy, double& deltaJX, double& deltaJY)
    {
    jx = ((JB.Eye.x - xPixel) * JB.InitZ / JB.ScreenDist - xPixel) * JB.XPerInch;
    jx += JB.XOffset;

    deltaJX = (JB.ZDepth / JB.ScreenDist) * (JB.Eye.x - xPixel) * JB.XPerInch / JB.ZDots;
    jy = ((JB.Eye.y - yPixel) * JB.InitZ / JB.ScreenDist - yPixel) * JB.YPerInch;
    jy += JB.YOffset;

    deltaJY = (JB.ZDepth / JB.ScreenDist) * (JB.Eye.y - yPixel) * JB.YPerInch / JB.ZDots;
    }

int	COtherFunctions::InitJulibrot(double param[], double ScreenRatio)
    {
    JB.Power = (int)param[0];
    JB.FromX = param[1];
    JB.FromY = param[2];
    JB.ToX = param[3];
    JB.ToY = param[4];
    JB.ZDots = (int)(fabs(param[5]));
    JB.ZOrigin = param[6];
    JB.ZDepth = param[7];
    JB.ScreenDist = param[8];
    JB.ScreenWidth = param[9];
    JB.Greyscale = (param[5] < 0.0);

    if (JB.Power < 2)
	JB.Power = 2;
    else if (JB.Power > 20)
	JB.Power = 20;

    const JulibrotDefault& Def = JulibrotDefaults[JB.Power];

    JB.FromX = Def.FromX;
    JB.FromY = Def.FromY;
    JB.ToX = Def.ToX;
    JB.ToY = Def.ToY;

    if (JB.ZDots < 1)
	JB.ZDots = 1;

    JB.ScreenHeight = JB.ScreenWidth / ScreenRatio;
    JB.DeltaMX = (JB.ToX - JB.FromX) / JB.ZDots;
    JB.DeltaMY = (JB.ToY - JB.FromY) / JB.ZDots;

    JB.InitZ = JB.ZOrigin - JB.ZDepth / 2.0;

    const double JulibrotBaseHor = fractalspecific[JULIBROT].hor;
    const double JulibrotBaseVert = fractalspecific[JULIBROT].vert;
    const double JulibrotBaseWidth = fractalspecific[JULIBROT].width;

    double xxmin = JulibrotBaseHor;
    double xxmax = JulibrotBaseHor + JulibrotBaseWidth * ScreenRatio;
    double yymin = JulibrotBaseVert;
    double yymax = JulibrotBaseVert + JulibrotBaseWidth;

    JB.XOffset = (xxmax + xxmin) / 2.0;
    JB.YOffset = (yymax + yymin) / 2.0;

    JB.XPerInch = (xxmin - xxmax) / JB.ScreenWidth;
    JB.YPerInch = (yymax - yymin) / JB.ScreenHeight;

    JB.Eye.x = 0.0;
    JB.Eye.y = 0.0;
    JB.Eye.zx = JB.ScreenDist;
    JB.Eye.zy = JB.ScreenDist;

#ifdef JULIBROT_OPTIMISER

    double BestY = 0.25;
    double BestX = -0.58;

    for (int pass = 0; pass < 4; pass++)
	{
	BestX = FindJulibrotDefaultX(JB.Power, BestY);
	BestY = FindJulibrotDefaultY(JB.Power, BestX);

	char s[160];
	sprintf_s(s,
	    "Julibrot Power %d pass %d: cx = %.6f  yRange = %.6f\n",
	    JB.Power, pass + 1, BestX, BestY);
	OutputDebugStringA(s);
	}

    JulibrotScore FinalResult = ScoreJulibrotSlice(JB.Power, BestX, BestY);

    char s[256];
    sprintf_s(s,
	"Power %d final: X=%.3f Y=%.3f Score=%.6f Occ=%.6f Mean=%.6f Spread=%.6f Entropy=%.6f Mid=%.6f\n",
	JB.Power,
	BestX, BestY,
	FinalResult.Score,
	FinalResult.Occupancy,
	FinalResult.MeanDepth,
	FinalResult.DepthSpread,
	FinalResult.DepthEntropy,
	FinalResult.MidDepthFraction);

    OutputDebugStringA(s);

    JulibrotScore Existing = ScoreJulibrotSlice(5, -0.800, 0.254);

    char s2[256];
    sprintf_s(s2,
	"Power 5 existing: Score=%.6f Occ=%.6f Mean=%.6f Spread=%.6f Entropy=%.6f Mid=%.6f\n",
	Existing.Score,
	Existing.Occupancy,
	Existing.MeanDepth,
	Existing.DepthSpread,
	Existing.DepthEntropy,
	Existing.MidDepthFraction);

    OutputDebugStringA(s2);
#endif

    return 0;
    }

int COtherFunctions::RunJulibrot(int xdots, int ydots, long threshold)
    {
    const double JulibrotBaseHor = fractalspecific[JULIBROT].hor;
    const double JulibrotBaseVert = fractalspecific[JULIBROT].vert;
    const double JulibrotBaseWidth = fractalspecific[JULIBROT].width;

    double BaseXWidth = JulibrotBaseWidth * ScreenRatio;
    double BaseYMax = JulibrotBaseVert + JulibrotBaseWidth;

    double CurrentXMin = hor;
    double CurrentXMax = hor + mandel_width * ScreenRatio;

    double CurrentYMin = vert;
    double CurrentYMax = vert + mandel_width;

    double xPixelMin = -JB.ScreenWidth / 2.0 + (CurrentXMin - JulibrotBaseHor) / BaseXWidth * JB.ScreenWidth;
    double xPixelMax = -JB.ScreenWidth / 2.0 + (CurrentXMax - JulibrotBaseHor) / BaseXWidth * JB.ScreenWidth;
    double yPixelMin = -JB.ScreenHeight / 2.0 + (BaseYMax - CurrentYMax) / JulibrotBaseWidth * JB.ScreenHeight;
    double yPixelMax = -JB.ScreenHeight / 2.0 + (BaseYMax - CurrentYMin) / JulibrotBaseWidth * JB.ScreenHeight;

    JB.InchPerXDot = (xPixelMax - xPixelMin) / xdots;
    JB.InchPerYDot = (yPixelMax - yPixelMin) / ydots;
    const long	TotalPixels = (long)xdots * (long)ydots;
    long	ProcessedPixels = 0;
    DWORD	lastUpdateTick = GetTickCount();

    /**********************************************************************
    Trace one Julibrot ray through the depth samples.
    At each depth, iterate z^Power + c.  If the orbit remains bounded 
    for threshold iterations, plot the pixel using the depth as the colour.
    **********************************************************************/
    auto PlotRay = [&](double xPixel, double yPixel, int col, int row)
	{
	double jx;
	double jy;
	double deltaJX;
	double deltaJY;

	JulibrotPerPixel(xPixel, yPixel, jx, jy, deltaJX, deltaJY);

	double mx = JB.FromX;
	double my = JB.FromY;

	for (int zpixel = 0; zpixel < JB.ZDots; zpixel++)
	    {
	    double zx = jx;
	    double zy = jy;

	    long iteration;

	    for (iteration = 0; iteration < threshold; iteration++)
		{
		// Calculate z^Power.
		Complex z;

		z.x = zx;
		z.y = zy;

		Complex p = z.CPolynomial(JB.Power);

		// z = z^Power + c
		zx = p.x + mx;
		zy = p.y + my;

		if (zx * zx + zy * zy > 4.0)
		    break;
		}

	    // A point which survives all iterations intersects the Julibrot at this depth.
	    if (iteration == threshold)
		{
		DWORD depth = (DWORD)(254L * zpixel / JB.ZDots) + 1;

		if (JB.Greyscale)
		    {
		    BYTE grey = (BYTE)(255 - depth);

		    RGBTRIPLE rgb;
		    rgb.rgbtRed = grey;
		    rgb.rgbtGreen = grey;
		    rgb.rgbtBlue = grey;

		    Plot.OutRGBpoint((WORD)col, (WORD)row, rgb);
		    }
		else
		    Plot.PlotPoint((WORD)col, (WORD)row, depth);
		break;
		}

	    mx += JB.DeltaMX;
	    my += JB.DeltaMY;
	    jx += deltaJX;
	    jy += deltaJY;
	    }
	};

    auto UpdateJulibrotStatus = [&]() -> bool
	{
	DWORD now = GetTickCount();

	if (now - lastUpdateTick < 250)
	    return true;

	lastUpdateTick = now;

	int percent = TotalPixels > 0 ? (int)(100.0 * ProcessedPixels / TotalPixels) : 0;
	_snprintf_s(FractalStatus, MAXLINE, _TRUNCATE, "Julibrot Power %d: %ld pixels processed (%d%%)", JB.Power, ProcessedPixels, percent);
	return user_data(NULL) >= 0;
	};

    for (int ydot = 0; ydot < ydots; ydot++)
	{
	double yPixel = yPixelMin + (double)ydot * JB.InchPerYDot;
	double xPixel = xPixelMin;

	for (int xdot = 0; xdot < xdots; xdot++, xPixel += JB.InchPerXDot)
	    {
	    if (!UpdateJulibrotStatus())
		return -1;

	    PlotRay(xPixel, yPixel, xdot, ydot);
	    ProcessedPixels++;
	    }
	}
    return 0;
    }

/**************************************************************************
	Dialog Box for loading Julibrot parameters
**************************************************************************/

INT_PTR CALLBACK SelectJulibrotParams(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    int	    i, j;
    char    s[NUMPARAM][100];
    char    Bailout[120];

    switch (message)
	{
	case WM_INITDIALOG:
	    SAFE_SPRINTF(Bailout, "%14.14lf", gManp->rqlim);
	    SetDlgItemText(hDlg, IDC_BAILOUT, Bailout);

	    EnableWindow(GetDlgItem(hDlg, IDC_BAILOUT), FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDC_FNLIST1), FALSE);
	    EnableWindow(GetDlgItem(hDlg, IDC_FNLIST2), FALSE);
	    EnableWindow(GetDlgItem(hDlg, ID_FRACPARAM1), TRUE);   // Power stays active

	    SetDlgItemText(hDlg, ID_FRACNAME, gManp->GetFractalName());

	    for (i = 0, j = 0; i < gManp->Fractal.NumParam && i < NUMPARAM; i++, j++)
		{
		SAFE_SPRINTF(s[j], "%f", *gManp->Fractal.ParamValue[j]);
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, gManp->Fractal.ParamName[j]);
		SetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[j]);
		}

	    for (i = gManp->Fractal.NumParam; i < NUMPARAM; i++)
		SetDlgItemText(hDlg, ID_FRACPARTX1 + i, "     N/A");

	    return TRUE;

	case WM_COMMAND:
	    switch (LOWORD(wParam))
		{
		case ID_FRACPARAM1:
		    if (HIWORD(wParam) == EN_CHANGE)
			{
			char PowerString[20];

			GetDlgItemText(hDlg, ID_FRACPARAM1, PowerString, sizeof(PowerString));
			int Power = atoi(PowerString);
			if (Power >= 2 && Power <= 20)
			    {
			    const JulibrotDefault& Def = GetJulibrotDefault(Power);

			    SAFE_SPRINTF(s[0], "%f", Def.FromX);
			    SetDlgItemText(hDlg, ID_FRACPARAM2, s[0]);

			    SAFE_SPRINTF(s[0], "%f", Def.FromY);
			    SetDlgItemText(hDlg, ID_FRACPARAM3, s[0]);

			    SAFE_SPRINTF(s[0], "%f", Def.ToX);
			    SetDlgItemText(hDlg, ID_FRACPARAM4, s[0]);

			    SAFE_SPRINTF(s[0], "%f", Def.ToY);
			    SetDlgItemText(hDlg, ID_FRACPARAM5, s[0]);
			    }
			}
		    return TRUE;

		case IDOK:
		    for (i = 0, j = 0; i < gManp->Fractal.NumParam && i < NUMPARAM; i++, j++)
			{
			GetDlgItemText(hDlg, ID_FRACPARAM1 + i, s[j], 100);
			*gManp->Fractal.ParamValue[j] = atof(s[j]);
			}

		    gManp->cycleflag = FALSE;

		    EndDialog(hDlg, TRUE);
		    return TRUE;

		case IDCANCEL:
		    EndDialog(hDlg, FALSE);
		    return FALSE;
		}
	    }

    return FALSE;
    }