/*
 * Hailstone.h - 2D Hailstone Sequence Visualization
 * 
 * Implements a discrete dynamical system on the integer lattice (Z x Z)
 * with cycle detection, visualization, and CSV export capabilities.
 * 
 * Mathematical Foundation:
 * Given a point (x, y), the next point (x', y') is determined by parity rules:
 *   - (even, even): x' = x/2,     y' = y/2
 *   - (even, odd):  x' = x/2 + 1, y' = 3y - 1
 *   - (odd, even):  x' = 3x - 1,  y' = y/2 - 1
 *   - (odd, odd):   x' = 3x + 1,  y' = 3y - 3
 * 
 * Revision history:
 * 13 Mar 2025  Initial implementation for ManpWIN
 */

 #pragma once

 #ifndef HAILSTONE_H
 #define HAILSTONE_H

 #include <Windows.h>
 #include <vector>
 #include <unordered_set>
 #include <string>

// Enumeration for transformation presets
enum class HailstonePreset
    {
    CURRENT_2D = 0,      // Original implementation
    SIMPLE_COLLATZ = 1,  // Classic Collatz applied to both dimensions
    SYMMETRIC = 2,       // Symmetric variant
    COORDINATE_SWAP = 3, // Swaps coordinates in odd cases
    BOUNDED_GROWTH = 4   // Controlled 2x expansion instead of 3x
    };

// Structure to hold a single point in the sequence
struct HailstonePoint
    {
    int step;       // Iteration number (N)
    int x;          // X coordinate
    int y;          // Y coordinate
    };

// Hash function for unordered_set of coordinate pairs
struct PairHash
    {
    size_t operator()(const std::pair<int, int>& p) const
	{
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
	}
    };

// Configuration for Hailstone visualization
struct HailstoneConfig
    {
    HailstonePreset preset; // Transformation preset to use
    int startX;             // Starting X coordinate
    int startY;             // Starting Y coordinate
    int maxIterations;      // Maximum steps before stopping
    bool detectCycles;      // Enable cycle detection
    bool showAxes;          // Display X and Y axes
    bool showPointLabels;   // Show (N, X, Y) labels at points
    bool showDots;          // Draw dots at segment endpoints
    float lineWidth;        // Thickness of line segments
    float dotSize;          // Size of endpoint dots
    int colorSpread;        // Color variation per iteration
    double scaleFactorX;    // Manual X scaling (0 = auto)
    double scaleFactorY;    // Manual Y scaling (0 = auto)

    // Default constructor
    HailstoneConfig() :
        preset(HailstonePreset::CURRENT_2D),
        startX(-10),
        startY(6),
        maxIterations(150),
        detectCycles(true),
        showAxes(true),
        showPointLabels(true),
        showDots(true),
        lineWidth(0.002f),
        dotSize(0.012f),
        colorSpread(7),
        scaleFactorX(0.0),
        scaleFactorY(0.0)
    {}
    };

enum HailstoneFlags
    {
    HS_SHOW_AXES = 1 << 0,
    HS_SHOW_LABELS = 1 << 1,
    HS_SHOW_DOTS = 1 << 2,
    HS_DETECT_CYCLES = 1 << 3
    };

// Cycle detection information
struct CycleInfo
    {
    bool detected;      // Was a cycle detected?
    int startStep;      // First occurrence of repeated point
    int endStep;        // Second occurrence (cycle detection point)
    int x;              // X coordinate of repeated point
    int y;              // Y coordinate of repeated point
    int length;         // Cycle length (endStep - startStep)

    CycleInfo() : detected(false), startStep(-1), endStep(-1), x(0), y(0), length(0) {}
    };

class CHailstone
{
public:
    CHailstone();
    ~CHailstone();

    // Core Hailstone rules
    int NextX(int x, int y);
    int NextY(int x, int y);

    // Sequence generation
    bool GenerateSequence(const HailstoneConfig& config);
    
    // Visualization
    int Render(HWND hwnd, class CPlot& plot, int screenWidth, int screenHeight, long threshold);
    
    // CSV Export
    bool ExportToCSV(const char* filePath);
    
    // Configuration
    void SetConfig(const HailstoneConfig& config);
    HailstoneConfig GetConfig() const { return m_config; }
    void UpdateDisplayFlags(bool showAxes, bool showPointLabels, bool showDots)
	{
        m_config.showAxes = showAxes;
        m_config.showPointLabels = showPointLabels;
        m_config.showDots = showDots;
	}

    // Access to results
    const std::vector<HailstonePoint>& GetPoints() const { return m_points; }
    const CycleInfo& GetCycleInfo() const { return m_cycleInfo; }

    // Bounds calculation (public for external rendering)
    void CalculateBounds(double& minX, double& maxX, double& minY, double& maxY);

    // UI Dialog
    static INT_PTR CALLBACK HailstoneDlgProc(HWND, UINT, WPARAM, LPARAM);

    // convert parameters for saving and reading fro par files
    void CHailstone::EncodeToParams(double param[], int& subtype);
    void CHailstone::DecodeFromParams(double param[], int subtype);

private:
    HailstoneConfig m_config;
    std::vector<HailstonePoint> m_points;
    CycleInfo m_cycleInfo;

    // Rendering helpers
    void DrawAxes(class CPlot& plot, double minX, double maxX, double minY, double maxY,
                  double centerX, double centerY, float pixelsPerUnitX, float pixelsPerUnitY,
                  int screenWidth, int screenHeight, long threshold);
    void DrawInfoOverlay(HWND hwnd, HDC hdc, int screenWidth, int screenHeight);
    void DrawSequence(class CPlot& plot, double centerX, double centerY,
                      float pixelsPerUnitX, float pixelsPerUnitY,
                      int screenWidth, int screenHeight, long threshold);
    void DrawPointLabels(HWND hwnd, HDC hdc, double centerX, double centerY,
                         float pixelsPerUnitX, float pixelsPerUnitY,
                         int screenWidth, int screenHeight);
    void DrawRGBLine(class CPlot& plot, int x0, int y0, int x1, int y1, RGBTRIPLE color, int screenWidth, int screenHeight);
    int CalculateNiceIntegerSpacing(int range);
};

#endif // HAILSTONE_H
