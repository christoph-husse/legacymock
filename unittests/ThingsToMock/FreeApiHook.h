
/* Copyright (c) 2015 Christoph Husse


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */
//
// Authors: 
//      # Christoph Husse

#ifndef _UNITTEST_MOCK_FREEAPIHOOK_H_
#define _UNITTEST_MOCK_FREEAPIHOOK_H_

#include "citrix/Config.h"
#include <windows.h>

template<typename> class CitrixMockingAccessor;

class Win32Registry {
public:
    static LSTATUS
        APIENTRY
        RegOpenKeyExW (
            HKEY hKey,
            LPCWSTR lpSubKey,
            DWORD ulOptions,
            REGSAM samDesired,
            PHKEY phkResult
        );

    static LSTATUS
        APIENTRY
        RegCloseKey (
            HKEY hKey
        );
};


int myFreeTestFunction(int a);

class FreeApiHook {
public:

// remove the semantics hidden behind it, so that it works with all VS version
#define __Ret_maybenull_
#define __In_
#define __In_opt_
#define __Outptr_
#define __Out_
#define __Inout_
#define __Acquires_exclusive_lock_(p)

    static VOID
    WINAPI
    Sleep(
        __In_ DWORD dwMilliseconds
        );

    static __Ret_maybenull_
    HANDLE
    WINAPI
    CreateEventA(
        __In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
        __In_ BOOL bManualReset,
        __In_ BOOL bInitialState,
        __In_opt_ LPCSTR lpName
        );

    static BOOL
    WINAPI
    GetSecurityDescriptorDacl(
        __In_ PSECURITY_DESCRIPTOR pSecurityDescriptor,
        __Out_ LPBOOL lpbDaclPresent,
        __Outptr_ PACL * pDacl,
        __Out_ LPBOOL lpbDaclDefaulted
        );

    static __Acquires_exclusive_lock_(*SRWLock)
    VOID
    WINAPI
    AcquireSRWLockExclusive(
        __Inout_ PSRWLOCK SRWLock
        );

    static int myFreeTestFunction(int a) { return a * 3; } 
};

#endif