#include <chrono>
#include <string>
#include <cstdio>
#pragma once

struct HiResTimer
    {
    LARGE_INTEGER freq{};
    LARGE_INTEGER t0{}, t1{};

    HiResTimer() { QueryPerformanceFrequency(&freq); }

    void start() { QueryPerformanceCounter(&t0); }
    double stop()
	{
	QueryPerformanceCounter(&t1);
	return double(t1.QuadPart - t0.QuadPart) / double(freq.QuadPart);
	}
    };

static std::string FormatElapsed(double ms)
    {
    char buf[64];

    if (ms < 1000.0) {
	// < 1 sec -> ms
	sprintf(buf, "%.1f ms", ms);
	}
    else if (ms < 60'000.0) {
	// < 1 min -> seconds
	sprintf(buf, "%.2f s", ms / 1000.0);
	}
    else if (ms < 3'600'000.0) {
	// < 1 hour -> min:sec
	int mins = (int)(ms / 60000.0);
	double secs = (ms - mins * 60000.0) / 1000.0;
	sprintf(buf, "%d m %.1f s", mins, secs);
	}
    else {
	// >= 1 hour -> hr:min
	int hrs = (int)(ms / 3'600'000.0);
	int mins = (int)((ms - hrs * 3'600'000.0) / 60000.0);
	sprintf(buf, "%d h %d m", hrs, mins);
	}

    return std::string(buf);
    }


struct SimpleTimer
    {
    std::chrono::high_resolution_clock::time_point t0;

    void start()
	{
	t0 = std::chrono::high_resolution_clock::now();
	}

    double stop_ms() const
	{
	auto t1 = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<double, std::milli>(t1 - t0).count();
	}
    };