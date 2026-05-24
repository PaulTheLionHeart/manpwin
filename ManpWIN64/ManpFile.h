#pragma once

#include <windows.h>

// Encapsulates Win32 dialog state for a specific file type.
// 
// NOTE:
//     File     = full pathname buffer returned by GetOpenFileName()
//     Path     = initial/default directory for this file type
//     Filter   = Win32 filter string
//
// Application-level filename state (e.g. PARFile) is kept separate
// from dialog/UI state intentionally.

struct FileDialogInfo
{
    OPENFILENAME ofn {};

    char Path[_MAX_PATH];
    char File[_MAX_PATH];

    const char* Filter;
};

