/*
 * Hailstone.cpp - 2D Hailstone Sequence Visualization Implementation
 * 
 * This module implements the 2D Hailstone sequence with cycle detection,
 * auto-scaling visualization, and CSV export capabilities.
 * 
 * Revision history:
 * 13 Mar 2025  Initial implementation for ManpWIN
 */

#include "Hailstone.h"
#include "plot.h"
#include "manp.h"
#include "resource.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

extern HWND GlobalHwnd;
extern WORD threshold;

/**************************************************************************
    Constructor
**************************************************************************/
CHailstone::CHailstone()
    {
    m_config = HailstoneConfig();
    m_config.preset = HailstonePreset::CURRENT_2D;  // Initialize with default preset
    }

/**************************************************************************
    Destructor
**************************************************************************/
CHailstone::~CHailstone()
    {
    m_points.clear();
    }

/**************************************************************************
    Core Hailstone Rule: Calculate next X coordinate
    Based on selected preset and parity of (x, y)
**************************************************************************/
int CHailstone::NextX(int x, int y)
    {
    switch (m_config.preset)
	{
	case HailstonePreset::CURRENT_2D:
	    // Original implementation
	    if (x % 2 == 0 && y % 2 == 0)
		return x / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return x / 2 + 1;
	    else if (x % 2 != 0 && y % 2 == 0)
		return 3 * x - 1;
	    else
		return 3 * x + 1;

	case HailstonePreset::SIMPLE_COLLATZ:
	    // Simple Collatz on both dimensions
	    if (x % 2 == 0 && y % 2 == 0)
		return x / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return x / 2;
	    else if (x % 2 != 0 && y % 2 == 0)
		return 3 * x + 1;
	    else
		return 3 * x + 1;

	case HailstonePreset::SYMMETRIC:
	    // Symmetric transformation
	    if (x % 2 == 0 && y % 2 == 0)
		return x / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return x / 2;
	    else if (x % 2 != 0 && y % 2 == 0)
		return 3 * x - 1;
	    else
		return 3 * x - 1;

	case HailstonePreset::COORDINATE_SWAP:
	    // Swaps coordinates in odd cases
	    if (x % 2 == 0 && y % 2 == 0)
		return x / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return y;
	    else if (x % 2 != 0 && y % 2 == 0)
		return y / 2;
	    else
		return (x + 1) / 2;

	case HailstonePreset::BOUNDED_GROWTH:
	    // Controlled expansion with 2x instead of 3x
	    if (x % 2 == 0 && y % 2 == 0)
		return x / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return x / 2 + 1;
	    else if (x % 2 != 0 && y % 2 == 0)
		return 2 * x - 1;
	    else
		return 2 * x + 1;

	default:
	    return x / 2; // Fallback
	}
    }

/**************************************************************************
    Core Hailstone Rule: Calculate next Y coordinate
    Based on selected preset and parity of (x, y)
**************************************************************************/
int CHailstone::NextY(int x, int y)
    {
    switch (m_config.preset)
	{
	case HailstonePreset::CURRENT_2D:
	    // Original implementation
	    if (x % 2 == 0 && y % 2 == 0)
		return y / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return 3 * y - 1;
	    else if (x % 2 != 0 && y % 2 == 0)
		return y / 2 - 1;
	    else
		return 3 * y - 3;

	case HailstonePreset::SIMPLE_COLLATZ:
	    // Simple Collatz on both dimensions
	    if (x % 2 == 0 && y % 2 == 0)
		return y / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return 3 * y + 1;
	    else if (x % 2 != 0 && y % 2 == 0)
		return y / 2;
	    else
		return 3 * y + 1;

	case HailstonePreset::SYMMETRIC:
	    // Symmetric transformation
	    if (x % 2 == 0 && y % 2 == 0)
		return y / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return 3 * y - 1;
	    else if (x % 2 != 0 && y % 2 == 0)
		return y / 2;
	    else
		return 3 * y - 1;

	case HailstonePreset::COORDINATE_SWAP:
	    // Swaps coordinates in odd cases
	    if (x % 2 == 0 && y % 2 == 0)
		return y / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return x / 2;
	    else if (x % 2 != 0 && y % 2 == 0)
		return x;
	    else
		return (y + 1) / 2;

	case HailstonePreset::BOUNDED_GROWTH:
	    // Controlled expansion with 2x instead of 3x
	    if (x % 2 == 0 && y % 2 == 0)
		return y / 2;
	    else if (x % 2 == 0 && y % 2 != 0)
		return 2 * y - 1;
	    else if (x % 2 != 0 && y % 2 == 0)
		return y / 2 + 1;
	    else
		return 2 * y + 1;

	default:
	    return y / 2; // Fallback
	}
    }

/**************************************************************************
    Generate Hailstone sequence with cycle detection
**************************************************************************/
bool CHailstone::GenerateSequence(const HailstoneConfig& config)
    {
    m_config = config;
    m_points.clear();
    m_cycleInfo = CycleInfo();

    std::unordered_set<std::pair<int, int>, PairHash> visitedPoints;

    int x = config.startX;
    int y = config.startY;

    // Add starting point
    HailstonePoint pt;
    pt.step = 0;
    pt.x = x;
    pt.y = y;
    m_points.push_back(pt);
    visitedPoints.insert(std::make_pair(x, y));

    // Generate sequence
    for (int i = 1; i <= config.maxIterations; i++)
	{
        // Apply Hailstone rules
        int nextX = NextX(x, y);
        int nextY = NextY(x, y);

        // Create new point
        HailstonePoint newPt;
        newPt.step = i;
        newPt.x = nextX;
        newPt.y = nextY;
        m_points.push_back(newPt);

        x = nextX;
        y = nextY;

        // Cycle detection
        if (config.detectCycles)
	    {
            if (visitedPoints.find(std::make_pair(x, y)) != visitedPoints.end())
		{
                // Found a cycle! Find where it started
                m_cycleInfo.detected = true;
                m_cycleInfo.endStep = i;
                m_cycleInfo.x = x;
                m_cycleInfo.y = y;

                // Find first occurrence
                for (size_t j = 0; j < m_points.size(); j++)
		    {
                    if (m_points[j].x == x && m_points[j].y == y)
			{
                        m_cycleInfo.startStep = m_points[j].step;
                        break;
			}
		    }

                m_cycleInfo.length = m_cycleInfo.endStep - m_cycleInfo.startStep;

                // Log cycle detection
                char msg[256];
		_snprintf_s(msg, 256, _TRUNCATE, "CYCLE DETECTED! Point (%d, %d) repeats at step %d, first seen at step %d. Cycle length: %d",
                    x, y, m_cycleInfo.endStep, m_cycleInfo.startStep, m_cycleInfo.length);
                OutputDebugStringA(msg);
                break;
		}

            visitedPoints.insert(std::make_pair(x, y));
	    }
	}

    return true;
    }

/**************************************************************************
    Calculate bounds of the sequence for auto-scaling
**************************************************************************/
void CHailstone::CalculateBounds(double& minX, double& maxX, double& minY, double& maxY)
    {
    if (m_points.empty())
	{
        minX = maxX = minY = maxY = 0;
        return;
	}

    minX = maxX = m_points[0].x;
    minY = maxY = m_points[0].y;

    // Use first 30% of points or first 50 points for auto-scaling
    size_t boundPoints = min(m_points.size(), (size_t)50);
    if (boundPoints < m_points.size() * 0.3)
        boundPoints = (size_t)(m_points.size() * 0.3);

    for (size_t i = 0; i < boundPoints; i++)
	{
        if (m_points[i].x < minX) minX = m_points[i].x;
        if (m_points[i].x > maxX) maxX = m_points[i].x;
        if (m_points[i].y < minY) minY = m_points[i].y;
        if (m_points[i].y > maxY) maxY = m_points[i].y;
	}

    // Add 15% padding
    double rangeX = maxX - minX;
    double rangeY = maxY - minY;
    
    if (rangeX < 1.0) rangeX = 1.0;
    if (rangeY < 1.0) rangeY = 1.0;

    double paddingX = rangeX * 0.15;
    double paddingY = rangeY * 0.15;

    minX -= paddingX;
    maxX += paddingX;
    minY -= paddingY;
    maxY += paddingY;
}
    
/**************************************************************************
    Calculate nice integer spacing for axis labels
**************************************************************************/
int CHailstone::CalculateNiceIntegerSpacing(int range)
    {
    if (range <= 0) return 1;
    if (range <= 7) return 1;

    // Target ~5-10 tick marks
    int roughSpacing = range / 7;
    if (roughSpacing == 0) return 1;

    // Round to nice numbers: 1, 2, 5, 10, 20, 50, 100, etc.
    int magnitude = (int)pow(10.0, floor(log10((double)roughSpacing)));
    if (magnitude == 0) magnitude = 1;

    int normalized = roughSpacing / magnitude;

    if (normalized <= 1) return magnitude;
    if (normalized <= 2) return 2 * magnitude;
    if (normalized <= 5) return 5 * magnitude;
    return 10 * magnitude;
    }

/**************************************************************************
    Draw a line using RGB color (for axes in medium gray)
**************************************************************************/
void CHailstone::DrawRGBLine(CPlot& plot, int x0, int y0, int x1, int y1, RGBTRIPLE color, int screenWidth, int screenHeight)
    {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
	{
        if (x0 >= 0 && x0 < screenWidth && y0 >= 0 && y0 < screenHeight)
	    {
            plot.OutRGBpoint((WORD)x0, (WORD)y0, color);
	    }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
	    {
            err -= dy;
            x0 += sx;
	    }
        if (e2 < dx)
	    {
            err += dx;
            y0 += sy;
	    }
	}
    }

/**************************************************************************
    Draw axes with integer tick marks
**************************************************************************/
void CHailstone::DrawAxes(CPlot& plot, double minX, double maxX, double minY, double maxY,
                          double centerX, double centerY, float pixelsPerUnitX, float pixelsPerUnitY,
                          int screenWidth, int screenHeight, long threshold)
    {
    if (!m_config.showAxes)
        return;

    int screenCenterX = screenWidth / 2;
    int screenCenterY = screenHeight / 2;

    // Medium gray color RGB(128, 128, 128)
    RGBTRIPLE grayColor;
    grayColor.rgbtRed = 128;
    grayColor.rgbtGreen = 128;
    grayColor.rgbtBlue = 128;

    // Draw X-axis (y=0) if it's within view
    if (minY <= 0 && maxY >= 0)
	{
        int y0 = screenCenterY - (int)((0 - centerY) * pixelsPerUnitY);
        if (y0 >= 0 && y0 < screenHeight)
	    {
            int x1 = screenCenterX + (int)((minX - centerX) * pixelsPerUnitX);
            int x2 = screenCenterX + (int)((maxX - centerX) * pixelsPerUnitX);
            DrawRGBLine(plot, max(0, x1), y0, min(screenWidth - 1, x2), y0, grayColor, screenWidth, screenHeight);

            // Draw tick marks on X-axis
            int tickStart = (int)ceil(minX);
            int tickEnd = (int)floor(maxX);
            for (int tick = tickStart; tick <= tickEnd; tick++)
		{
                int xTick = screenCenterX + (int)((tick - centerX) * pixelsPerUnitX);
                if (xTick >= 0 && xTick < screenWidth && tick != 0)
		    {
                    // Draw tick mark
                    DrawRGBLine(plot, xTick, y0 - 3, xTick, y0 + 3, grayColor, screenWidth, screenHeight);
		    }
		}
	    }
	}

    // Draw Y-axis (x=0) if it's within view
    if (minX <= 0 && maxX >= 0)
	{
        int x0 = screenCenterX + (int)((0 - centerX) * pixelsPerUnitX);
        if (x0 >= 0 && x0 < screenWidth)
	    {
            int y1 = screenCenterY - (int)((maxY - centerY) * pixelsPerUnitY);
            int y2 = screenCenterY - (int)((minY - centerY) * pixelsPerUnitY);
            DrawRGBLine(plot, x0, max(0, y1), x0, min(screenHeight - 1, y2), grayColor, screenWidth, screenHeight);

            // Draw tick marks on Y-axis
            int tickStart = (int)ceil(minY);
            int tickEnd = (int)floor(maxY);
            for (int tick = tickStart; tick <= tickEnd; tick++)
		{
                int yTick = screenCenterY - (int)((tick - centerY) * pixelsPerUnitY);
                if (yTick >= 0 && yTick < screenHeight && tick != 0)
		    {
                    // Draw tick mark
                    DrawRGBLine(plot, x0 - 3, yTick, x0 + 3, yTick, grayColor, screenWidth, screenHeight);
		    }
		}
	    }
	}
    }

/**************************************************************************
    Draw point labels showing step number and coordinates
    For cycle points, show both first visit and return visit labels in BOLD
**************************************************************************/
void CHailstone::DrawPointLabels(HWND hwnd, HDC hdc, double centerX, double centerY,
                                  float pixelsPerUnitX, float pixelsPerUnitY, int screenWidth, int screenHeight)
    {
    if (!m_config.showPointLabels || m_points.empty())
        return;

    int screenCenterX = screenWidth / 2;
    int screenCenterY = screenHeight / 2;

    // Draw labels for each point
    for (size_t i = 0; i < m_points.size(); i++)
	{
        const HailstonePoint& pt = m_points[i];

        // Transform to screen coordinates
        int screenX = screenCenterX + (int)((pt.x - centerX) * pixelsPerUnitX);
        int screenY = screenCenterY - (int)((pt.y - centerY) * pixelsPerUnitY);

        // Skip if outside visible area
        if (screenX < 0 || screenX >= screenWidth || screenY < 0 || screenY >= screenHeight)
            continue;

        char labelText[128];
        bool isCycleLabel = false;  // Flag to determine if this is the cycle completion label

        // Check if this is the cycle point (visited twice)
        if (m_cycleInfo.detected && pt.x == m_cycleInfo.x && pt.y == m_cycleInfo.y)
	    {
            // This point was visited twice - show both visits explicitly with the word "and"
            if ((int)i == m_cycleInfo.startStep)
		{
                // First visit - show both occurrences in BOLD format: (step, x, y) and (step, x, y)
		_snprintf_s(labelText, 128, _TRUNCATE, "(%d, %d, %d) and (%d, %d, %d)",
                        m_cycleInfo.startStep, pt.x, pt.y,
                        m_cycleInfo.endStep, pt.x, pt.y);
                isCycleLabel = true;  // This is the cycle completion label - make it bold
		}
            else if ((int)i == m_cycleInfo.endStep)
		{
                // Second visit - skip since we already labeled it at first visit
                continue;
		}
            else
		{
                // Regular point - single occurrence
		_snprintf_s(labelText, 128, _TRUNCATE, "(%d, %d, %d)", pt.step, pt.x, pt.y);
		}
	    }
        else
	    {
            // Regular point - single occurrence
	    _snprintf_s(labelText, 128, _TRUNCATE, "(%d, %d, %d)", pt.step, pt.x, pt.y);
	    }

        // Setup font for labels - use BOLD for cycle completion label
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        lf.lfHeight = -12;  // Small font for labels
        lf.lfWeight = isCycleLabel ? FW_BOLD : FW_NORMAL;  // Bold if cycle detected
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
        lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
        lf.lfQuality = DEFAULT_QUALITY;
        lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
        strcpy(lf.lfFaceName, "Arial");

        // Draw the label
        RECT textRect;
        textRect.left = screenX + 5;  // Offset from point
        textRect.top = screenY - 15;
        textRect.right = screenX + 200;
        textRect.bottom = screenY + 5;

        extern CDib Dib;
        Dib.Text2Dib(hdc, &textRect, RGB(255, 255, 255), RGB(0, 0, 0), &lf, TRANSPARENT, labelText);
	}
    }

/**************************************************************************
    Draw information overlay in upper left corner
**************************************************************************/
void CHailstone::DrawInfoOverlay(HWND hwnd, HDC hdc, int screenWidth, int screenHeight)
    {
    if (m_points.empty())
        return;

    // Create multi-line info text
//    char infoText[512];
    int lineCount = 0;
    char* lines[10];
    char line0[128], line1[128], line2[128]/*, line3[128], line4[128], line5[128]*/;

    // Line 0: Starting point
    _snprintf_s(line0, 128, _TRUNCATE, "Start: (%d, %d)", m_config.startX, m_config.startY);
    lines[lineCount++] = line0;

    // Line 1: Total iterations
    _snprintf_s(line1, 128, _TRUNCATE, "Iterations: %d", (int)m_points.size());
    lines[lineCount++] = line1;

    // Line 2: Cycle detection status (simple)
    if (m_cycleInfo.detected)
	{
	_snprintf_s(line2, 128, _TRUNCATE, "Cycle: DETECTED");
	}
    else
	{
	_snprintf_s(line2, 128, _TRUNCATE, "Cycle: None");
	}
    lines[lineCount++] = line2;

    // Setup font for info display
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = -14;  // Readable size
    lf.lfWeight = FW_BOLD;  // Bold for visibility
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    strcpy(lf.lfFaceName, "Arial");

    // Draw each line with slight offset
    extern CDib Dib;
    for (int i = 0; i < lineCount; i++)
	{
        RECT textRect;
        textRect.left = 10;
        textRect.top = 10 + (i * 18);  // 18 pixels per line
        textRect.right = 400;
        textRect.bottom = textRect.top + 20;

        Dib.Text2Dib(hdc, &textRect, RGB(255, 0, 255), RGB(0, 0, 0), &lf, TRANSPARENT, lines[i]);
	}
    }

/**************************************************************************
    Draw the Hailstone sequence
**************************************************************************/
void CHailstone::DrawSequence(CPlot& plot, double centerX, double centerY, float pixelsPerUnitX, float pixelsPerUnitY, int screenWidth, int screenHeight, long threshold)
    {
    if (m_points.size() < 2)
	{
        OutputDebugStringA("Hailstone: Not enough points to draw sequence");
        return;
	}

    int screenCenterX = screenWidth / 2;
    int screenCenterY = screenHeight / 2;

    DWORD cycleColor = threshold * 3 / 4;	// High palette index for cycles
    DWORD normalColor = threshold / 2;		// Mid palette index for normal segments

    // Draw lines connecting points
    for (size_t i = 0; i < m_points.size() - 1; i++)
	{
        const HailstonePoint& p1 = m_points[i];
        const HailstonePoint& p2 = m_points[i + 1];

        // Check if this segment is part of the cycle
        bool isInCycle = m_cycleInfo.detected && 
                        (int)i >= m_cycleInfo.startStep && 
                        (int)i < m_cycleInfo.endStep;

        // Transform to screen coordinates
        int x1 = screenCenterX + (int)((p1.x - centerX) * pixelsPerUnitX);
        int y1 = screenCenterY - (int)((p1.y - centerY) * pixelsPerUnitY);
        int x2 = screenCenterX + (int)((p2.x - centerX) * pixelsPerUnitX);
        int y2 = screenCenterY - (int)((p2.y - centerY) * pixelsPerUnitY);

        // Check bounds
        if (x1 < -100 || x1 > screenWidth + 100 || y1 < -100 || y1 > screenHeight + 100)
            continue;
        if (x2 < -100 || x2 > screenWidth + 100 || y2 < -100 || y2 > screenHeight + 100)
            continue;

        DWORD color = isInCycle ? cycleColor : normalColor;
        plot.genline(x1, y1, x2, y2, color);

        // Draw dots at points if enabled
        if (m_config.showDots)
	    {
            int dotRadius = (int)(m_config.dotSize * min(pixelsPerUnitX, pixelsPerUnitY) * 20);
            if (dotRadius < 2) dotRadius = 2;
            if (dotRadius > 5) dotRadius = 5;

            plot.DisplayFilledCircle(x1, y1, dotRadius, color);
	    }
	}

    // Draw last point dot
    if (m_config.showDots && !m_points.empty())
	{
        const HailstonePoint& p = m_points[m_points.size() - 1];
        int x = screenCenterX + (int)((p.x - centerX) * pixelsPerUnitX);
        int y = screenCenterY - (int)((p.y - centerY) * pixelsPerUnitY);

        int dotRadius = (int)(m_config.dotSize * min(pixelsPerUnitX, pixelsPerUnitY) * 20);
        if (dotRadius < 2) dotRadius = 2;
        if (dotRadius > 5) dotRadius = 5;

        DWORD color = m_cycleInfo.detected && (int)m_points.size() - 1 >= m_cycleInfo.startStep 
                     ? cycleColor : normalColor;
        plot.DisplayFilledCircle(x, y, dotRadius, color);
	}
    }

/**************************************************************************
    Main rendering function
**************************************************************************/
int CHailstone::Render(HWND hwnd, CPlot& plot, int screenWidth, int screenHeight, long threshold)
    {
    if (m_points.empty())
        return -1;

    // Calculate bounds for auto-scaling
    double minX, maxX, minY, maxY;
    CalculateBounds(minX, maxX, minY, maxY);

    double centerX = (minX + maxX) / 2.0;
    double centerY = (minY + maxY) / 2.0;
    double dataRangeX = maxX - minX;
    double dataRangeY = maxY - minY;

    // Calculate pixels per unit
    float pixelsPerUnitX = (float)screenWidth / (float)dataRangeX;
    float pixelsPerUnitY = (float)screenHeight / (float)dataRangeY;

    // Draw axes (only if enabled)
    if (m_config.showAxes)
	{
        DrawAxes(plot, minX, maxX, minY, maxY, centerX, centerY, pixelsPerUnitX, pixelsPerUnitY, screenWidth, screenHeight, threshold);
	}

    // Draw sequence
    DrawSequence(plot, centerX, centerY, pixelsPerUnitX, pixelsPerUnitY, screenWidth, screenHeight, threshold);

    // Draw point labels if enabled
    if (m_config.showPointLabels)
	{
        HDC hdc = GetDC(hwnd);
        if (hdc)
	    {
            DrawPointLabels(hwnd, hdc, centerX, centerY, pixelsPerUnitX, pixelsPerUnitY, screenWidth, screenHeight);
            ReleaseDC(hwnd, hdc);
	    }
	}

    return 0;
    }

/**************************************************************************
    Export sequence to CSV file
**************************************************************************/
bool CHailstone::ExportToCSV(const char* filePath)
    {
    std::ofstream file(filePath);
    if (!file.is_open())
        return false;

    // Write header comments (spaces instead of commas in coordinates)
    file << "# Hailstone Sequence (N X Y)\n";
    file << "# Starting point: (0 " << m_config.startX << " " << m_config.startY << ")\n";
    file << "# Total points: " << m_points.size() << "\n";

    if (m_cycleInfo.detected)
	{
        file << "# Cycle Detected: Point (" << m_cycleInfo.endStep << " " 
             << m_cycleInfo.x << " " << m_cycleInfo.y << ")\n";
        file << "# Duplicate of: (" << m_cycleInfo.startStep << " " 
             << m_cycleInfo.x << " " << m_cycleInfo.y << ")\n";
        file << "# Cycle length: " << m_cycleInfo.length << "\n";
	}
    else
	{
        file << "# No cycle detected - stopped at MaxIterations\n";
	}

    file << "#\n";
    file << "N, X, Y\n";

    // Write data rows
    for (const auto& pt : m_points)
	{
        file << pt.step << ", " << pt.x << ", " << pt.y << "\n";
	}

    file.close();
    return true;
    }

/**************************************************************************
    Configuration setter
**************************************************************************/
void CHailstone::SetConfig(const HailstoneConfig& config)
    {
    m_config = config;
    }

/**************************************************************************
    Dialog procedure for Hailstone configuration
**************************************************************************/
INT_PTR CALLBACK CHailstone::HailstoneDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
    static HailstoneConfig* pConfig = nullptr;

    switch (message)
    {
    case WM_INITDIALOG:
        pConfig = (HailstoneConfig*)lParam;
        if (pConfig)
	    {
            SetDlgItemInt(hDlg, IDC_HAILSTONE_STARTX, pConfig->startX, TRUE);
            SetDlgItemInt(hDlg, IDC_HAILSTONE_STARTY, pConfig->startY, TRUE);
            SetDlgItemInt(hDlg, IDC_HAILSTONE_MAXITER, pConfig->maxIterations, FALSE);
            CheckDlgButton(hDlg, IDC_HAILSTONE_DETECTCYCLES, pConfig->detectCycles ? BST_CHECKED : BST_UNCHECKED);
            CheckDlgButton(hDlg, IDC_HAILSTONE_SHOWAXES, pConfig->showAxes ? BST_CHECKED : BST_UNCHECKED);
            CheckDlgButton(hDlg, IDC_HAILSTONE_SHOWLABELS, pConfig->showPointLabels ? BST_CHECKED : BST_UNCHECKED);
            CheckDlgButton(hDlg, IDC_HAILSTONE_SHOWDOTS, pConfig->showDots ? BST_CHECKED : BST_UNCHECKED);
	    }
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
	    {
	    case IDOK:
		if (pConfig)
		    {
		    BOOL translated;
		    pConfig->startX = GetDlgItemInt(hDlg, IDC_HAILSTONE_STARTX, &translated, TRUE);
		    pConfig->startY = GetDlgItemInt(hDlg, IDC_HAILSTONE_STARTY, &translated, TRUE);
		    pConfig->maxIterations = GetDlgItemInt(hDlg, IDC_HAILSTONE_MAXITER, &translated, FALSE);
		    if (pConfig->maxIterations < 10) pConfig->maxIterations = 10;
		    if (pConfig->maxIterations > 500) pConfig->maxIterations = 500;
                
		    pConfig->detectCycles = IsDlgButtonChecked(hDlg, IDC_HAILSTONE_DETECTCYCLES) == BST_CHECKED;
		    pConfig->showAxes = IsDlgButtonChecked(hDlg, IDC_HAILSTONE_SHOWAXES) == BST_CHECKED;
		    pConfig->showPointLabels = IsDlgButtonChecked(hDlg, IDC_HAILSTONE_SHOWLABELS) == BST_CHECKED;
		    pConfig->showDots = IsDlgButtonChecked(hDlg, IDC_HAILSTONE_SHOWDOTS) == BST_CHECKED;
		    }
		EndDialog(hDlg, IDOK);
		return TRUE;

	    case IDCANCEL:
		EndDialog(hDlg, IDCANCEL);
		return TRUE;
	    }
	break;
	}

    return FALSE;
    }
