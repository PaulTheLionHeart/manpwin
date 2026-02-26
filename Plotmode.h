#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <cmath>
#include <limits>
#include <windows.h>
#include <windowsx.h>

#define	    CHUNK_SIZE				128

#pragma once

enum class YPlotMode
    {
    TopDown = 0,	// 0 .. h-1
    BottomUp,		// h-1 .. 0
    CenterOut,		// center row, then +/-1, +/-2...
    OutsideIn,		// from top+bottom moving toward center your mode 4
    RandomY,		// random permutation of rows
    Interleaved,	// like it says
    BarbarPole,		// progressive bands
    BitReversed,	// as it says
    Golden		// golden ratio
    };

extern YPlotMode currentYMode;

class CPlotmode
    {
    public:
    std::vector<int> generateYOrder(int h, YPlotMode mode);
    private:
    std::vector<int> generateYTopDown(int h);
    std::vector<int> generateYBottomUp(int h);
    std::vector<int> generateYCenterOut(int h);
    std::vector<int> generateYOutsideIn(int h);
    std::vector<int> generateYRandom(int h);
    std::vector<int> generateY_Interleaved(int h);
    std::vector<int> generateY_ProgressiveBands(int h);
    std::vector<int> makeY_BitReversed(int h);
    std::vector<int> makeY_Golden(int h);
    const char* yModeName(YPlotMode m);
    };

/*
enum class PlotMode 
    {
    Scanline = 0,
    Column,
    Tile,
    Spiral,
    Random,
    Tesseral,      // NEW
    Progressive,   // NEW
    SolidGuess     // NEW
    };

extern  PlotMode	currentMode;

class CPlotmode
    {
    public:

    std::vector<std::pair<int, int>> generateScanline(int w, int h);
    std::vector<std::pair<int, int>> generateColumn(int w, int h);
    std::vector<std::pair<int, int>> generateTile(int w, int h);
    std::vector<std::pair<int, int>> generateSpiral(int w, int h);
    std::vector<std::pair<int, int>> generateRandom(int w, int h);
    std::vector<std::pair<int, int>> generateTesseral(int w, int h);     // NEW
    std::vector<std::pair<int, int>> generateProgressive(int w, int h); // NEW
    std::vector<std::pair<int, int>> generateSolidGuess(int w, int h);  // NEW
    char* modeName(PlotMode m);
    };
*/    

