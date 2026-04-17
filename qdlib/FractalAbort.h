#pragma once
#include <atomic>
#include <exception>

extern std::atomic<bool> gFatalErrorOccurred;

class FractalAbort : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Fractal computation aborted";
    }
};