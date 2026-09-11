// This file is part of Noggit3, licensed under GNU General Public License (version 3).

// Some StormLib forks (e.g. current upstream ladislav-zezula/StormLib) renamed their
// non-Windows GetLastError()/SetLastError() compatibility shims to SErrGetLastError()/
// SErrSetLastError(), while noggit's code (and the StormLib fork it is normally built
// against) still expects the older plain names. This file is only compiled in when
// CMake detects that the linked StormLib does not already provide GetLastError()/
// SetLastError(), and mirrors StormLib's own non-Windows reference implementation
// (includes/FileStream.cpp) so linking succeeds either way.

#include <StormLib.h>

#ifndef STORMLIB_WINDOWS
static thread_local DWORD dwLastError = ERROR_SUCCESS;

DWORD GetLastError()
{
    return dwLastError;
}

void SetLastError(DWORD dwErrCode)
{
    dwLastError = dwErrCode;
}
#endif
