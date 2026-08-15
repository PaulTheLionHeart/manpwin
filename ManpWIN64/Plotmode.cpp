/*
    Plotmode.cpp - interface for the CPlotMode class which are all the plotting modes used in slope and perturbation.

    Written in Microsoft Visual C++ by Paul de Leeuw.
*/

#include <algorithm>
#include "Plotmode.h"

PlotMode	currentMode = PlotMode::SpiralOut;

// ---------------------------------------------------------
// Coordinate generators
// ---------------------------------------------------------
std::vector<std::pair<int, int>> CPlotMode::generateScanline(int w, int h) {
    std::vector<std::pair<int, int>> pixelOrder;
    pixelOrder.reserve(w * h);
    pixelOrder.resize(w * h);
    return pixelOrder;
    }

std::vector<std::pair<int, int>> CPlotMode::generateColumn(int w, int h) {
    std::vector<std::pair<int, int>> pixelOrder;
    pixelOrder.reserve(w * h);
    for (int x = 0; x < w; x++)
	for (int y = h - 1; y >= 0; y--)
            pixelOrder.emplace_back(x, y);
    return pixelOrder;
    }

std::vector<std::pair<int, int>> CPlotMode::generateTile(int w, int h)
    {
    const int TILE = 64;
    std::vector<std::pair<int, int>> pixelOrder;
    pixelOrder.reserve(w * h);

    for (int ty = 0; ty < h; ty += TILE)
	for (int tx = 0; tx < w; tx += TILE)
	    for (int y = ty; y < min(ty + TILE, h); y++)
		for (int x = tx; x < min(tx + TILE, w); x++)
		    pixelOrder.emplace_back(x, h - 1 - y);

    return pixelOrder;
    }

std::vector<std::pair<int, int>> CPlotMode::generateSpiralOut(int w, int h) {
    std::vector<std::pair<int, int>> pixelOrder;
    pixelOrder.reserve(w * h);
    int cx = w / 2;
    int cy = h / 2;
    int x = cx, y = cy;
    int dx[4] = { 1,0,-1,0 };
    int dy[4] = { 0,1,0,-1 };
    int steps = 1;
    pixelOrder.emplace_back(x, h - 1 - y);
    while ((int)pixelOrder.size() < w * h)
	{
        for (int dir = 0; dir < 4; ++dir) 
	    {
            int stepCount = steps;
            for (int s = 0; s < stepCount; s++) 
		{
                x += dx[dir];
                y += dy[dir];
                if (x >= 0 && x < w && y >= 0 && y < h) 
		    {
		    pixelOrder.emplace_back(x, h - 1 - y);
                    if ((int)pixelOrder.size() >= w * h) break;
                    }
                }
            if ((int)pixelOrder.size() >= w * h) break;
            if (dir == 1 || dir == 3) steps++;
            }
        }
    return pixelOrder;
    }

std::vector<std::pair<int, int>> CPlotMode::generateSpiralIn(int w, int h)
    {
    auto pixelOrder = generateSpiralOut(w, h);
    std::reverse(pixelOrder.begin(), pixelOrder.end());
    return pixelOrder;
    }

std::vector<std::pair<int, int>> CPlotMode::generateRandom(int w, int h)
    {
    std::vector<std::pair<int, int>> pixelOrder;

    if (w <= 0 || h <= 0)
	return pixelOrder;

    // Use size_t and overflow-safe multiply
    const size_t W = (size_t)w;
    const size_t H = (size_t)h;

    if (W > (SIZE_MAX / H))   // overflow guard
	return pixelOrder;

    const size_t totalPixels = W * H;

    // Optional: also guard against vector max_size
    if (totalPixels > pixelOrder.max_size())
	return pixelOrder;

    pixelOrder.reserve(totalPixels);

    for (int y = 0; y < h; ++y)
	for (int x = 0; x < w; ++x)
	    pixelOrder.emplace_back(x, h - 1 - y);

    // shuffle
    std::mt19937 rng((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::shuffle(pixelOrder.begin(), pixelOrder.end(), rng);

    return pixelOrder;
    }

    // ----------------------------------------------------
// Tesseral (Morton/Z-order) generator
// ----------------------------------------------------
std::vector<std::pair<int, int>> CPlotMode::generateTesseral(int w, int h) {
    std::vector<std::pair<int, int>> pixelOrder;
    pixelOrder.reserve(w * h);

    auto part1By1 = [](unsigned int n) {
        n &= 0x0000ffff;
        n = (n ^ (n << 8)) & 0x00FF00FF;
        n = (n ^ (n << 4)) & 0x0F0F0F0F;
        n = (n ^ (n << 2)) & 0x33333333;
        n = (n ^ (n << 1)) & 0x55555555;
        return n;
        };
    auto morton2D = [&](int x, int y) {
        return (part1By1(y) << 1) | part1By1(x);
        };

    std::vector<std::tuple<int, int, unsigned int>> mortonList;
    mortonList.reserve(w * h);
    for (int y = 0; y < h; ++y) 
	{
	for (int x = 0; x < w; ++x) 
	    {
	    mortonList.push_back({ x, h - 1 - y, morton2D(x, y) });
	    }
	}
    std::sort(mortonList.begin(), mortonList.end(),
        [](auto& a, auto& b) { return std::get<2>(a) < std::get<2>(b); });


    for (auto& tup : mortonList) {
        int x = std::get<0>(tup);
        int y = std::get<1>(tup);
        // unsigned int m = std::get<2>(tup); // only needed for debugging
        pixelOrder.emplace_back(x, y);
        }
    return pixelOrder;
    }

// ----------------------------------------------------
// Progressive Refinement generator
// ----------------------------------------------------
std::vector<std::pair<int, int>> CPlotMode::generateProgressive(int w, int h)
    {
    std::vector<std::pair<int, int>> pixelOrder;
    pixelOrder.reserve((size_t)w * h);

    std::vector<char> seen((size_t)w * h, 0);

    for (int step = 16; step >= 1; step /= 2)
	{
	for (int y = 0; y < h; y += step)
	    {
	    for (int x = 0; x < w; x += step)
		{
		int idx = y * w + x;

		if (!seen[idx])
		    {
		    seen[idx] = 1;
		    pixelOrder.emplace_back(x, h - 1 - y);
		    }
		}
	    }
	}

    return pixelOrder;
    }

// ----------------------------------------------------
// Solid Guessing (first cut, simple version)
// ----------------------------------------------------

std::vector<std::pair<int, int>> CPlotMode::generateSolidGuess(int w, int h)
    {
    std::vector<std::pair<int, int>> pixelOrder;
    pixelOrder.reserve((size_t)w * h);

    std::vector<char> seen((size_t)w * h, 0);

    struct Rect { int x0, y0, ww, hh; };
    std::vector<Rect> queue;
    queue.reserve(1024);
    queue.push_back({ 0, 0, w, h });

    size_t qi = 0;
    while (qi < queue.size()) 
	{
        Rect r = queue[qi++];

        // center of the rectangle
        int cx = r.x0 + (r.ww >> 1);
        int cy = r.y0 + (r.hh >> 1);
        if (cx >= 0 && cx < w && cy >= 0 && cy < h) 
	    {
            int idx = cy * w + cx;
            if (!seen[idx]) 
		{
                seen[idx] = 1;
		pixelOrder.emplace_back(cx, h - 1 - cy);
                }
            }

        // subdivide if larger than 1x1
        if (r.ww > 1 || r.hh > 1) 
	    {
            int w2 = r.ww >> 1;
            int h2 = r.hh >> 1;
            // top-left
            queue.push_back({ r.x0, r.y0, w2 > 0 ? w2 : 1, h2 > 0 ? h2 : 1 });
            // top-right
            queue.push_back({ r.x0 + w2, r.y0, r.ww - w2, h2 > 0 ? h2 : 1 });
            // bottom-left
            queue.push_back({ r.x0, r.y0 + h2, w2 > 0 ? w2 : 1, r.hh - h2 });
            // bottom-right
            queue.push_back({ r.x0 + w2, r.y0 + h2, r.ww - w2, r.hh - h2 });
            }
        }

    // Fill any remaining unseen pixels
    for (int y = 0; y < h; ++y) 
	{
        for (int x = 0; x < w; ++x) 
	    {
            int idx = y * w + x;
            if (!seen[idx]) 
		{
                seen[idx] = 1;
		pixelOrder.emplace_back(x, h - 1 - y);
                }
            }
        }

    return pixelOrder;
    }

// ---------------------------------------------------------
// Mode names
// ---------------------------------------------------------
const char* CPlotMode::modeName(PlotMode m)
    {
    switch (m) 
	{
        case PlotMode::Scanline:    return	"Scanline";
        case PlotMode::Column:	    return	"Column";
        case PlotMode::Tile:	    return	"Tile";
	case PlotMode::SpiralOut:   return	"Spiral Out";
	case PlotMode::SpiralIn:    return	"Spiral In";
	case PlotMode::Random:	    return	"Random";
        case PlotMode::Tesseral:    return	"Tesseral";
        case PlotMode::Progressive: return	"Progressive";
        case PlotMode::SolidGuess:  return	"SolidGuess";
        default:		    return	"Unknown";
        }
    }

// ---------------------------------------------------------
// Choose Plotting Mode
// ---------------------------------------------------------

void BuildPixelOrder(std::vector<std::pair<int, int>>& pixelOrder, int& totalPixels, int xdots, int ydots, PlotMode mode)
    {
    CPlotMode scheduler;

    pixelOrder.clear();

    switch (mode)
	{
	case PlotMode::Scanline:
	    pixelOrder = scheduler.generateScanline(xdots, ydots);
	    break;

	case PlotMode::Column:
	    pixelOrder = scheduler.generateColumn(xdots, ydots);
	    break;

	case PlotMode::Tile:
	    pixelOrder = scheduler.generateTile(xdots, ydots);
	    break;

	case PlotMode::SpiralOut:
	    pixelOrder = scheduler.generateSpiralOut(xdots, ydots);
	    break;

	case PlotMode::SpiralIn:
	    pixelOrder = scheduler.generateSpiralIn(xdots, ydots);
	    break;

	case PlotMode::Random:
	    pixelOrder = scheduler.generateRandom(xdots, ydots);
	    break;

	case PlotMode::Tesseral:
	    pixelOrder = scheduler.generateTesseral(xdots, ydots);
	    break;

	case PlotMode::Progressive:
	    pixelOrder = scheduler.generateProgressive(xdots, ydots);
	    break;

	case PlotMode::SolidGuess:
	    pixelOrder = scheduler.generateSolidGuess(xdots, ydots);
	    break;

	default:
	    break;
	}

    totalPixels = (mode == PlotMode::Scanline) ? xdots * ydots : (int)pixelOrder.size();
    }

