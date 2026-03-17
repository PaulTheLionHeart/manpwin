///////////////////////////////////////
// SafeStrings.h
// StringBuilder helper used to eliminate legacy strcat usage in ManpWIN
// Instead of constantly recalculating strlen(info) and remaining space, create a small struct that tracks it automatically.
// This removes the danger of shrinking buffer space silently.
// The Helper
//////////////////////////////////////

#include <Windows.h>
#include <stdio.h>

#pragma once

struct StringBuilder
    {
    char*  buf;
    size_t size;
    size_t len;

    StringBuilder(char* b, size_t s)
        : buf(b), size(s), len(b[0] ? strlen(b) : 0) {}

    void append(const char* fmt, ...)
	{
        if (len >= size - 1)
            return;

        va_list args;
        va_start(args, fmt);

        int written = _vsnprintf_s(buf + len, size - len, _TRUNCATE, fmt, args);

        va_end(args);

        if (written > 0)
            len += written;
	}
    };

// alternative clas implementation:

/*
class StringBuilder
    {
    private:
	char*  buf;
	size_t size;
	size_t len;

    public:
	StringBuilder(char* b, size_t s)
	    : buf(b), size(s), len(strlen(b)) {}

	void append(const char* fmt, ...)
	    {
	    if (len >= size - 1)
		return;

	    va_list args;
	    va_start(args, fmt);

	    int written = _vsnprintf_s(buf + len, size - len, _TRUNCATE, fmt, args);

	    va_end(args);

	    if (written > 0)
		len += written;
	    }
    };
*/

#define SAFE_SPRINTF(buf, fmt, ...) \
    _snprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, __VA_ARGS__)

#define SAFE_APPEND(dst, size, src) strncat_s(dst, size, src, _TRUNCATE)
