#include "Plotmode.h"

#define min(a,b)    (((a) < (b)) ? (a) : (b))

/*
// ---------------------------------------------------------
// Coordinate generators
// ---------------------------------------------------------
std::vector<std::pair<int, int>> CPlotmode::generateScanline(int w, int h) {
    std::vector<std::pair<int, int>> coords;
    coords.reserve(w * h);
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            coords.emplace_back(x, y);
    return coords;
    }

std::vector<std::pair<int, int>> CPlotmode::generateColumn(int w, int h) {
    std::vector<std::pair<int, int>> coords;
    coords.reserve(w * h);
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            coords.emplace_back(x, y);
    return coords;
    }

std::vector<std::pair<int, int>> CPlotmode::generateTile(int w, int h) {
    const int TILE = 64;
    std::vector<std::pair<int, int>> coords;
    coords.reserve(w * h);
    for (int ty = 0; ty < h; ty += TILE)
        for (int tx = 0; tx < w; tx += TILE)
            for (int y = ty; y < min(ty + TILE, h); y++)
                for (int x = tx; x < min(tx + TILE, w); x++)
                    coords.emplace_back(x, y);
    return coords;
    }

std::vector<std::pair<int, int>> CPlotmode::generateSpiral(int w, int h) {
    std::vector<std::pair<int, int>> coords;
    coords.reserve(w * h);
    int cx = w / 2;
    int cy = h / 2;
    int x = cx, y = cy;
    int dx[4] = { 1,0,-1,0 };
    int dy[4] = { 0,1,0,-1 };
    int steps = 1;
    coords.emplace_back(x, y);
    while ((int)coords.size() < w * h) {
        for (int dir = 0; dir < 4; ++dir) {
            int stepCount = steps;
            for (int s = 0; s < stepCount; s++) {
                x += dx[dir];
                y += dy[dir];
                if (x >= 0 && x < w && y >= 0 && y < h) {
                    coords.emplace_back(x, y);
                    if ((int)coords.size() >= w * h) break;
                    }
                }
            if ((int)coords.size() >= w * h) break;
            if (dir == 1 || dir == 3) steps++;
            }
        }
    return coords;
    }

std::vector<std::pair<int, int>> CPlotmode::generateRandom(int w, int h)
    {
    std::vector<std::pair<int, int>> coords;

    if (w <= 0 || h <= 0)
	return coords;

    // Use size_t and overflow-safe multiply
    const size_t W = (size_t)w;
    const size_t H = (size_t)h;

    if (W > (SIZE_MAX / H))   // overflow guard
	return coords;

    const size_t total = W * H;

    // Optional: also guard against vector max_size
    if (total > coords.max_size())
	return coords;

    coords.reserve(total);

    for (int y = 0; y < h; ++y)
	for (int x = 0; x < w; ++x)
	    coords.emplace_back(x, y);

    // shuffle
    std::mt19937 rng((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::shuffle(coords.begin(), coords.end(), rng);

    return coords;
    }
*/
/*
std::vector<std::pair<int, int>> CPlotmode::generateRandom(int w, int h) {
    std::vector<std::pair<int, int>> coords;
    coords.reserve(w * h);
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++)
            coords.emplace_back(x, y);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(coords.begin(), coords.end(), g);
    return coords;
    }
*/
/*
// ----------------------------------------------------
// Tesseral (Morton/Z-order) generator
// ----------------------------------------------------
std::vector<std::pair<int, int>> CPlotmode::generateTesseral(int w, int h) {
    std::vector<std::pair<int, int>> coords;
    coords.reserve(w * h);

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
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            mortonList.push_back({ x, y, morton2D(x, y) });
            }
        }
    std::sort(mortonList.begin(), mortonList.end(),
        [](auto& a, auto& b) { return std::get<2>(a) < std::get<2>(b); });


    for (auto& tup : mortonList) {
        int x = std::get<0>(tup);
        int y = std::get<1>(tup);
        // unsigned int m = std::get<2>(tup); // only needed for debugging
        coords.emplace_back(x, y);
        }
    return coords;
    }

// ----------------------------------------------------
// Progressive Refinement generator
// ----------------------------------------------------
std::vector<std::pair<int, int>> CPlotmode::generateProgressive(int w, int h) {
    std::vector<std::pair<int, int>> coords;
    coords.reserve(w * h);

    for (int step = 16; step >= 1; step /= 2) { // coarse to fine
        for (int y = 0; y < h; y += step) {
            for (int x = 0; x < w; x += step) {
                coords.emplace_back(x, y);
                }
            }
        }
    return coords;
    }

// ----------------------------------------------------
// Solid Guessing (first cut, simple version)
// ----------------------------------------------------

std::vector<std::pair<int, int>> CPlotmode::generateSolidGuess(int w, int h) {
    std::vector<std::pair<int, int>> coords;
    coords.reserve((size_t)w * h);

    std::vector<char> seen((size_t)w * h, 0);

    struct Rect { int x0, y0, ww, hh; };
    std::vector<Rect> queue;
    queue.reserve(1024);
    queue.push_back({ 0, 0, w, h });

    size_t qi = 0;
    while (qi < queue.size()) {
        Rect r = queue[qi++];

        // center of the rectangle
        int cx = r.x0 + (r.ww >> 1);
        int cy = r.y0 + (r.hh >> 1);
        if (cx >= 0 && cx < w && cy >= 0 && cy < h) {
            int idx = cy * w + cx;
            if (!seen[idx]) {
                seen[idx] = 1;
                coords.emplace_back(cx, cy);
                }
            }

        // subdivide if larger than 1x1
        if (r.ww > 1 || r.hh > 1) {
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
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int idx = y * w + x;
            if (!seen[idx]) {
                seen[idx] = 1;
                coords.emplace_back(x, y);
                }
            }
        }

    return coords;
    }

// ---------------------------------------------------------
// Mode names
// ---------------------------------------------------------
char* CPlotmode::modeName(PlotMode m) {
    switch (m) {
            case PlotMode::Scanline: return "Scanline";
            case PlotMode::Column:   return "Column";
            case PlotMode::Tile:     return "Tile";
            case PlotMode::Spiral:   return "Spiral";
            case PlotMode::Random:   return "Random";
            case PlotMode::Tesseral: return (char*)"Tesseral";
            case PlotMode::Progressive: return (char*)"Progressive";
            case PlotMode::SolidGuess: return (char*)"SolidGuess";
            default: return (char*)"Unknown";
        }
    }
*/

// ----------------------------------------------------
// Y co-ordinate plotting
// ----------------------------------------------------

std::vector<int> CPlotmode::generateYTopDown(int h)
    {
    std::vector<int> rows;
    rows.reserve(h);
    for (int y = h - 1; y >= 0; --y) 
	rows.push_back(y);
    return rows;
    }

std::vector<int> CPlotmode::generateYBottomUp(int h)
    {
    std::vector<int> rows;
    rows.reserve(h);
    for (int y = 0; y < h; ++y) 
	rows.push_back(y);
    return rows;
    }

std::vector<int> CPlotmode::generateYCenterOut(int h)
    {
    std::vector<int> rows;
    rows.reserve(h);

    int mid = (h - 1) / 2;      // works for even/odd
    rows.push_back(mid);

    for (int d = 1; (int)rows.size() < h; ++d)
	{
        int y1 = mid - d;
        int y2 = mid + d;
        if (y1 >= 0) rows.push_back(y1);
        if (y2 < h)  rows.push_back(y2);
	}
    return rows;
}

// Mode 4: Outside-in towards X axis (top/bottom alternating)
std::vector<int> CPlotmode::generateYOutsideIn(int h)
    {
    std::vector<int> rows;
    rows.reserve(h);

    int top = 0;
    int bot = h - 1;
    while (top <= bot)
	{
        rows.push_back(top++);
        if (top <= bot)
            rows.push_back(bot--);
	}
    return rows;
    }

std::vector<int> CPlotmode::generateYRandom(int h)
    {
    std::vector<int> rows;
    rows.reserve(h);
    for (int y = 0; y < h; ++y) rows.push_back(y);

    // thread-local RNG (important: avoids shared state issues)
    thread_local std::mt19937 rng((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count() ^ (unsigned)GetCurrentThreadId());

    std::shuffle(rows.begin(), rows.end(), rng);
    return rows;
    }

// Gives fast global structure, then fills detail.
std::vector<int> CPlotmode::generateY_Interleaved(int h)
    {
    std::vector<int> ys;
    ys.reserve(h);

    // evens
    for (int y = 0; y < h; y += 2)
	ys.push_back(y);

    // odds
    for (int y = 1; y < h; y += 2)
	ys.push_back(y);

    return ys;
    }

// barber's pole
std::vector<int> CPlotmode::generateY_ProgressiveBands(int h)
    {
    std::vector<int> ys;
    ys.reserve(h);
    int bandSize = 32;
    int bands = (h + bandSize - 1) / bandSize;

    for (int row = 0; row < bandSize; ++row)
	{
	for (int b = 0; b < bands; ++b)
	    {
	    int y = b * bandSize + row;
	    if (y < h)
		ys.push_back(y);
	    }
	}
    return ys;
    }

// This is excellent for progressive rendering : you get coarse structure early, then finer detail.
// Concept : reverse the bits of the row index.

static inline uint32_t reverseBits(uint32_t v)
    {
    v = (v >> 16) | (v << 16);
    v = ((v & 0xFF00FF00u) >> 8) | ((v & 0x00FF00FFu) << 8);
    v = ((v & 0xF0F0F0F0u) >> 4) | ((v & 0x0F0F0F0Fu) << 4);
    v = ((v & 0xCCCCCCCCu) >> 2) | ((v & 0x33333333u) << 2);
    v = ((v & 0xAAAAAAAAu) >> 1) | ((v & 0x55555555u) << 1);
    return v;
    }

// This is excellent for progressive rendering : you get coarse structure early, then finer detail.
// Concept : reverse the bits of the row index.
std::vector<int> CPlotmode::makeY_BitReversed(int h)
    {
    std::vector<std::pair<uint32_t, int>> tmp;
    tmp.reserve(h);

    for (int i = 0; i < h; ++i)
	{
	uint32_t r = reverseBits((uint32_t)i);
	tmp.emplace_back(r, i);
	}

    std::sort(tmp.begin(), tmp.end(),
	[](auto& a, auto& b) { return a.first < b.first; });

    std::vector<int> y;
    y.reserve(h);
    for (auto& p : tmp) y.push_back(p.second);
    return y;
    }

// This is a quasi-random walk that fills evenly without clustering.
// Formula: y(n) = floor(frac(n * phi) * h)
// Visually this gives: very smooth “global fill”, no strong striping, great for aesthetic progressive renders
std::vector<int> CPlotmode::makeY_Golden(int h)
    {
    const double phi = 0.6180339887498948482; // golden ratio conjugate

    std::vector<int> y;
    y.reserve(h);

    std::vector<bool> used(h, false);

    for (int i = 0; i < h; ++i)
	{
	int v = (int)(std::fmod(i * phi, 1.0) * h);
	if (!used[v])
	    {
	    used[v] = true;
	    y.push_back(v);
	    }
	}

    // fill any gaps (rare but possible with fp)
    for (int i = 0; i < h; ++i)
	if (!used[i]) y.push_back(i);

    return y;
    }

std::vector<int> CPlotmode::generateYOrder(int h, YPlotMode mode)
    {
    switch (mode)
	{
	case YPlotMode::TopDown:	return generateYTopDown(h);
	case YPlotMode::BottomUp:	return generateYBottomUp(h);
	case YPlotMode::CenterOut:	return generateYCenterOut(h);
	case YPlotMode::OutsideIn:	return generateYOutsideIn(h);
	case YPlotMode::RandomY:	return generateYRandom(h);
	case YPlotMode::Interleaved:	return generateY_Interleaved(h);
	case YPlotMode::BarbarPole:	return generateY_ProgressiveBands(h);
	case YPlotMode::BitReversed:	return makeY_BitReversed(h);
	case YPlotMode::Golden:		return makeY_Golden(h);
	default:                   return generateYTopDown(h);
	}
    }

