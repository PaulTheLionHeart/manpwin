/*
    InverseJulia.cpp - a module for Inverse Julia renderer from Fractint 
    
    Written in Microsoft Visual 'C++' by Paul de Leeuw.
*/

#include <windows.h>
#include "OtherFunctions.h"
#include "manp.h"
#include "TransformTemplate.h"

extern	int  setup_convert_to_screen(struct affine *, bool UseFractintCorners);
extern	int	user_data(HWND hwnd);

DWORD lastUpdateTick = GetTickCount();

/****************************************************************
    Per-image setup for the Inverse Julia fractal.

    Called from the fractal database before orbit processing begins.
****************************************************************/

int COtherFunctions::InitInverseJulia()
    {
    Complex root;
    double x, y, r;

    strcpy_s(gManp->InvJuliaStatus, MAXLINE, "Inverse Julia: Initialising...");

    InverseJuliaQueue.clear();

     //Julia constant.
    InverseJuliaC.x = gManp->param[0];
    InverseJuliaC.y = gManp->param[1];

    // Maximum hits per pixel.
    InverseJuliaMaxHits = (int)gManp->param[2];

    if (InverseJuliaMaxHits < 1)
	InverseJuliaMaxHits = 1;

    InverseJuliaRunLength = (int)gManp->param[5];
    if (InverseJuliaRunLength < 1)
	InverseJuliaRunLength = 1024;
    InverseJuliaPower = (int)gManp->param[6];

    switch (InverseJuliaPower)
	{
	case 2:
	    /*
	     * Find a fixed point of:
	     *
	     *      z = z^2 + C
	     *
	     * therefore:
	     *
	     *      z = (1 +/- sqrt(1 - 4C)) / 2
	     */
	    x = 1.0 - 4.0 * InverseJuliaC.x;
	    y = -4.0 * InverseJuliaC.y;

	    r = sqrt(x * x + y * y);

	    root.x = sqrt((r + x) / 2.0);
	    root.y = sqrt((r - x) / 2.0);

	    if (y < 0.0)
		root.y = -root.y;

	    InverseJuliaPoint.x = (1.0 + root.x) / 2.0;
	    InverseJuliaPoint.y = root.y / 2.0;
	    break;

	case 3:
	    {
	    Complex delta;
	    Complex sqrtDelta;
	    Complex u;
	    double denom;
	    double angle;

	    // Solve z^3 - z + C = 0 using Cardano's method
	    delta.x = (InverseJuliaC.x * InverseJuliaC.x - InverseJuliaC.y * InverseJuliaC.y) / 4.0 - 1.0 / 27.0;
	    delta.y = (InverseJuliaC.x * InverseJuliaC.y) / 2.0;

	    // Square root of the discriminant
	    r = sqrt(delta.x * delta.x + delta.y * delta.y);

	    sqrtDelta.x = sqrt((r + delta.x) / 2.0);
	    sqrtDelta.y = sqrt((r - delta.x) / 2.0);

	    if (delta.y < 0.0)
		sqrtDelta.y = -sqrtDelta.y;

	    // First Cardano term
	    x = -InverseJuliaC.x / 2.0 + sqrtDelta.x;
	    y = -InverseJuliaC.y / 2.0 + sqrtDelta.y;

	    r = sqrt(x * x + y * y);
	    angle = atan2(y, x);

	    u.x = pow(r, 1.0 / 3.0) * cos(angle / 3.0);
	    u.y = pow(r, 1.0 / 3.0) * sin(angle / 3.0);

	    // Second term follows from uv = 1/3
	    denom = 3.0 * (u.x * u.x + u.y * u.y);

	    if (denom != 0.0)
		{
		InverseJuliaPoint.x = u.x + u.x / denom;
		InverseJuliaPoint.y = u.y - u.y / denom;
		}
	    else
		InverseJuliaPoint = u;

	    break;
	    }

	case 4:
	    {
	    Complex zn;
	    Complex z2, z3, z4;
	    Complex f, df, delta;
	    double denom;

	    // Solve z^4 - z + C = 0 using Newton iteration.
	    zn.x = 1.0;
	    zn.y = 0.0;

	    for (int n = 0; n < 50; n++)
		{
		z2.x = zn.x * zn.x - zn.y * zn.y;
		z2.y = 2.0 * zn.x * zn.y;

		z3.x = z2.x * zn.x - z2.y * zn.y;
		z3.y = z2.x * zn.y + z2.y * zn.x;

		z4.x = z2.x * z2.x - z2.y * z2.y;
		z4.y = 2.0 * z2.x * z2.y;

		f.x = z4.x - zn.x + InverseJuliaC.x;
		f.y = z4.y - zn.y + InverseJuliaC.y;

		df.x = 4.0 * z3.x - 1.0;
		df.y = 4.0 * z3.y;

		denom = df.x * df.x + df.y * df.y;

		if (denom == 0.0)
		    break;

		delta.x = (f.x * df.x + f.y * df.y) / denom;
		delta.y = (f.y * df.x - f.x * df.y) / denom;

		zn.x -= delta.x;
		zn.y -= delta.y;

		if (delta.x * delta.x + delta.y * delta.y < 1.0e-24)
		    break;
		}

	    InverseJuliaPoint = zn;
	    break;
	    }

	default:
	    // Add a message that this poower doesn't exist
	    break;
	}

    // Fractint six-corner PARs require their original viewing
    // parallelogram. Ordinary IJ views use ManpWIN's rectangle.
    bool UseFractintCorners = gManp->FractintSixCorners;

    if (setup_convert_to_screen(&InverseJuliaScreen, UseFractintCorners) < 0)
	return -1;

    // Seed queue / current point
    InverseJuliaMajor = (InverseJuliaMajorMethod)(int)gManp->param[3];

    if (InverseJuliaMajor < IJ_BREADTH_FIRST ||
	InverseJuliaMajor > IJ_RANDOM_RUN)
	InverseJuliaMajor = IJ_BREADTH_FIRST;

    InverseJuliaMinor = (InverseJuliaMinorMethod)(int)gManp->param[4];

    if (InverseJuliaMinor < IJ_LEFT_FIRST ||
	InverseJuliaMinor > IJ_RIGHT_FIRST)
	InverseJuliaMinor = IJ_LEFT_FIRST;

    // Seed breadth-first and depth-first traversal.
    if (InverseJuliaMajor == IJ_BREADTH_FIRST || InverseJuliaMajor == IJ_DEPTH_FIRST)
	{
	if (InverseJuliaPower == 2)
	    {
	    Complex point1;
	    Complex point2;

	    point1.x = (1.0 + root.x) / 2.0;
	    point1.y = root.y / 2.0;

	    point2.x = (1.0 - root.x) / 2.0;
	    point2.y = -root.y / 2.0;

	    InverseJuliaQueue.push_back(point1);
	    InverseJuliaQueue.push_back(point2);
	    }
	else
	    InverseJuliaQueue.push_back(InverseJuliaPoint);
	}
    return 0;
    }

/****************************************************************
    Orbit routine for the Inverse Julia fractal.

    Traversal modes:
	0 = Breadth first
	1 = Depth first
	2 = Random walk
****************************************************************/

int COtherFunctions::InverseJuliaOrbit()
    {
    Complex z;
    long    ProcessedPoints = 0;
    Complex children[MAXINVERSEJULIAPOWER];
    int     RandomRunLength = 0;
    int     RandomRunMode = 0;
    int     RandomRunChild = 0;

    double  xgap = (gManp->mandel_width * gManp->AspectRatio) / (double)gManp->xdots;
    double  ygap = gManp->mandel_width / (double)gManp->ydots;

    // Limit Inverse Julia point generation to a finite budget.
    // All four modes benefit from additional sampling, especially
    // when rendering detailed images at higher resolutions.
    // Use 64-bit arithmetic to prevent overflow in the calculation.

    const long Multiplier = 16384L;
    const long MaxPoints = (long)min((__int64)gManp->threshold * Multiplier, (__int64)LONG_MAX);

    // Report Inverse Julia progress at approximately 250 ms intervals.
    DWORD lastUpdateTick = GetTickCount();

    /*************************************************************************
	Update Inverse Julia progress approximately every 250 milliseconds.

	Report the number of points processed and progress towards MaxPoints.
	Process pending Windows messages to keep the display responsive and
	allow cancellation. Return false if processing should stop.
    *************************************************************************/

    auto UpdateInverseJuliaStatus = [&]() -> bool
	{
	DWORD now = GetTickCount();

	if (now - lastUpdateTick < 250)
	    return true;

	lastUpdateTick = now;

	int percent = MaxPoints > 0 ? (int)(100.0 * ProcessedPoints / MaxPoints) : 0;
	_snprintf_s(gManp->InvJuliaStatus, MAXLINE, _TRUNCATE, "Inverse Julia: %ld points processed (%d%%)", ProcessedPoints, percent);
	return user_data(NULL) >= 0;
	};

    // Generate the inverse roots for the selected power.
    auto GenerateChildren = [&](const Complex& Parent, Complex *children)
	{
	double x = Parent.x - InverseJuliaC.x;
	double y = Parent.y - InverseJuliaC.y;
	double r = sqrt(x * x + y * y);

	double RootRadius = pow(r, 1.0 / InverseJuliaPower);
	double theta = atan2(y, x);

	for (int k = 0; k < InverseJuliaPower; k++)
	    {
	    double Angle = (theta + TWO_PI * k) / InverseJuliaPower;

	    children[k].x = RootRadius * cos(Angle);
	    children[k].y = RootRadius * sin(Angle);
	    }

	return InverseJuliaPower;
	};

    _snprintf_s(gManp->InvJuliaStatus, MAXLINE, _TRUNCATE, "Inverse Julia: Starting point generation...");
    switch (InverseJuliaMajor)
	{
	// Breadth first
	case IJ_BREADTH_FIRST:
	    {
	    while (!InverseJuliaQueue.empty() && ProcessedPoints < MaxPoints)
		{
		z = InverseJuliaQueue.front();
		InverseJuliaQueue.pop_front();

		Complex PlotPoint = z;

		if (gManp->TransformType == TRANSFORM_AFFINE)
		    PlotPoint = InverseAffineTransformT<Complex, double>(PlotPoint, gManp->AffineParams);

		// Convert fractal coordinate to screen coordinate.
		int j = (int)(InverseJuliaScreen.a * PlotPoint.x + InverseJuliaScreen.b * PlotPoint.y + InverseJuliaScreen.e);
		int i = (int)(InverseJuliaScreen.c * PlotPoint.x + InverseJuliaScreen.d * PlotPoint.y + InverseJuliaScreen.f);

		int ChildCount = GenerateChildren(z, children);

		ProcessedPoints++;
		if (!UpdateInverseJuliaStatus())
		    return -1;

		// If the parent point is off screen, keep one child alive.
		if (j < 0 || j >= gManp->xdots || i < 0 || i >= gManp->ydots)
		    {
		    InverseJuliaQueue.push_back(children[rand() % ChildCount]);
		    continue;
		    }

		// Parent point is visible.
		size_t index = (size_t)i * gManp->xdots + j;
		float hits = gManp->wpixels[index];

		if (hits < InverseJuliaMaxHits)
		    {
		    hits++;
		    gManp->wpixels[index] = hits;

		    Plot.PlotPoint(j, i, (long)hits);

		    // Add all inverse branches.
		    for (int k = 0; k < ChildCount; k++)
			InverseJuliaQueue.push_back(children[k]);
		    }
		}

	    break;
	    }

	// Depth first
	case IJ_DEPTH_FIRST:
	    {
	    while (!InverseJuliaQueue.empty() && ProcessedPoints < MaxPoints)
		{
		// LIFO: remove the newest point from the back.
		z = InverseJuliaQueue.back();
		InverseJuliaQueue.pop_back();

		Complex PlotPoint = z;

		if (gManp->TransformType == TRANSFORM_AFFINE)
		    PlotPoint = InverseAffineTransformT<Complex, double>(PlotPoint, gManp->AffineParams);

		// Convert fractal coordinate to screen coordinate.
		int j = (int)(InverseJuliaScreen.a * PlotPoint.x + InverseJuliaScreen.b * PlotPoint.y + InverseJuliaScreen.e);
		int i = (int)(InverseJuliaScreen.c * PlotPoint.x + InverseJuliaScreen.d * PlotPoint.y + InverseJuliaScreen.f);

		int ChildCount = GenerateChildren(z, children);

		ProcessedPoints++;
		if (!UpdateInverseJuliaStatus())
		    return -1;

		// If the parent point is off screen, keep one child alive.
		if (j < 0 || j >= gManp->xdots || i < 0 || i >= gManp->ydots)
		    {
		    InverseJuliaQueue.push_back(children[rand() % ChildCount]);
		    continue;
		    }

		// Parent point is visible.
		size_t index = (size_t)i * gManp->xdots + j;
		float hits = gManp->wpixels[index];

		if (hits < InverseJuliaMaxHits)
		    {
		    hits++;
		    gManp->wpixels[index] = hits;

		    Plot.PlotPoint(j, i, (long)hits);

		    if (InverseJuliaMinor == IJ_LEFT_FIRST)
			{
			// Fractint push order for left first.
			for (int k = 0; k < ChildCount; k++)
			    InverseJuliaQueue.push_back(children[k]);
			}
		    else
			{
			// Fractint push order for right first.
			for (int k = ChildCount - 1; k >= 0; k--)
			    InverseJuliaQueue.push_back(children[k]);
			}
		    }
		}

	    break;
	    }

	// Random walk
	case IJ_RANDOM_WALK:
	    {
	    z = InverseJuliaPoint;

	    for (long count = 0; count < MaxPoints; count++)
		{
		// Convert fractal coordinate to screen coordinate.
		int j = (int)((z.x - gManp->hor) / xgap);
		int i = (int)((gManp->vert + gManp->mandel_width - z.y) / ygap);

		ProcessedPoints++;
		if (!UpdateInverseJuliaStatus())
		    return -1;

		// Increment hit count.
		if (j >= 0 && j < gManp->xdots && i >= 0 && i < gManp->ydots)
		    {
		    size_t index = (size_t)i * gManp->xdots + j;

		    gManp->wpixels[index]++;

		    Plot.PlotPoint(j, i, (long)gManp->wpixels[index]);
		    }

		int ChildCount = GenerateChildren(z, children);

		// Randomly choose one inverse root.
		z = children[rand() % ChildCount];
		}

	    break;
	    }
	    // Random run
	case IJ_RANDOM_RUN:
	    {
	    z = InverseJuliaPoint;

	    for (long count = 0; count < MaxPoints; count++)
		{
		// Convert fractal coordinate to screen coordinate.
		int j = (int)((z.x - gManp->hor) / xgap);
		int i = (int)((gManp->vert + gManp->mandel_width - z.y) / ygap);

		ProcessedPoints++;
		if (!UpdateInverseJuliaStatus())
		    return -1;

		// Increment hit count.
		if (j >= 0 && j < gManp->xdots && i >= 0 && i < gManp->ydots)
		    {
		    size_t index = (size_t)i * gManp->xdots + j;

		    gManp->wpixels[index]++;
		    Plot.PlotPoint(j, i, (long)gManp->wpixels[index]);
		    }

		int ChildCount = GenerateChildren(z, children);

		if (RandomRunLength-- <= 0)
		    {
		    RandomRunLength = rand() % InverseJuliaRunLength;

		    if (ChildCount == 2)
			{
			// Fractint quadratic modes: left, right or random.
			RandomRunMode = rand() % 3;
			}
		    else
			{
			// Higher powers retain one root for each random run.
			RandomRunChild = rand() % ChildCount;
			}
		    }

		if (ChildCount == 2)
		    {
		    switch (RandomRunMode)
			{
			case 0:
			    z = children[0];
			    break;

			case 1:
			    z = children[1];
			    break;

			case 2:
			default:
			    z = children[rand() % ChildCount];
			    break;
			}
		    }
		else
		    z = children[RandomRunChild];
		}

	    break;
	    }

	default:
	    break;

	}

    return 0;
    }
