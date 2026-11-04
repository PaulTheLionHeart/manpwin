/*
    jb.h - Julibrot Fractal Stuff.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#pragma once

//#define	JULIBROT_OPTIMISER  true

// stuff for Julibrot

struct JulibrotPerspective
    {
    double x;
    double y;
    double zx;
    double zy;
    };

struct JulibrotData
    {
    JulibrotPerspective Eye;

    double  FromX;
    double  FromY;
    double  ToX;
    double  ToY;

    int	    Power;
    int	    ZDots;

    double  ZOrigin;
    double  ZDepth;
    double  ScreenDist;
    double  ScreenWidth;
    double  ScreenHeight;

    double  XOffset;
    double  YOffset;
    double  DeltaMX;
    double  DeltaMY;
    double  XPerInch;
    double  YPerInch;
    double  InchPerXDot;
    double  InchPerYDot;
    double  InitZ;

    bool    Greyscale = false;
    };

struct JulibrotDefault
    {
    double FromX;
    double FromY;
    double ToX;
    double ToY;
    };

const JulibrotDefault& GetJulibrotDefault(int Power);

#ifdef JULIBROT_OPTIMISER
struct JulibrotScore
    {
    double Score;
    double Occupancy;
    double DepthSpread;
    double EdgeScore;
    double MeanDepth;
    double DepthEntropy;
    double MidDepthFraction;
    };
#endif

