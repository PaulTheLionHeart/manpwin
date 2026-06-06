/*
    Plotmode.h - interface for the CPlotMode class which are all the plotting modes used in slope and perturbation.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <vector>
#include <random>
#include <atomic>
#include <chrono>
#include <algorithm>
#include <functional>
#include <cmath>
#include <limits>
#include <windows.h>
#include <windowsx.h>

#define	    CHUNK_SIZE				128

#pragma once

enum class PlotMode
    {
    Scanline = 0,
    Column,
    Tile,
    SpiralOut,
    SpiralIn,
    Random,
    Tesseral,
    Progressive,
    SolidGuess
    };

extern  PlotMode	currentMode;

class CPlotMode
    {
    public:

    std::vector<std::pair<int, int>> generateScanline(int w, int h);
    std::vector<std::pair<int, int>> generateColumn(int w, int h);
    std::vector<std::pair<int, int>> generateTile(int w, int h);
    std::vector<std::pair<int, int>> generateSpiralOut(int w, int h);
    std::vector<std::pair<int, int>> generateSpiralIn(int w, int h);
    std::vector<std::pair<int, int>> generateRandom(int w, int h);
    std::vector<std::pair<int, int>> generateTesseral(int w, int h);     // NEW
    std::vector<std::pair<int, int>> generateProgressive(int w, int h); // NEW
    std::vector<std::pair<int, int>> generateSolidGuess(int w, int h);  // NEW
    const  char* modeName(PlotMode m);
    };

void BuildPixelOrder(
    std::vector<std::pair<int, int>>& pixelOrder,
    int& totalPixels,
    int xdots,
    int ydots,
    PlotMode mode);

