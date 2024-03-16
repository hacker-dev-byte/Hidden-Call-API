/*
MIT License

Copyright (c) 2024 hacker-dev-byte

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "api.h"

LPVOID getAddrFunc(HMODULE module, LPCSTR name)
{
    PIMAGE_DOS_HEADER imgDosHeader = (PIMAGE_DOS_HEADER)module;
    PIMAGE_NT_HEADERS imgNtHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)imgDosHeader + imgDosHeader->e_lfanew);
    PIMAGE_EXPORT_DIRECTORY imgExportDir = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)imgDosHeader + imgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

    LPDWORD rvaAddr = (LPDWORD)((DWORD_PTR)imgDosHeader + imgExportDir->AddressOfFunctions);
    LPDWORD rvaName = (LPDWORD)((DWORD_PTR)imgDosHeader + imgExportDir->AddressOfNames);
    LPWORD rvaOrdinal = (LPWORD)((DWORD_PTR)imgDosHeader + imgExportDir->AddressOfNameOrdinals);

    LPVOID vaAddr = NULL;
    LPSTR vaName = NULL;
    DWORD vaOrdinal = -1;

    for (DWORD i = 0; i < imgExportDir->NumberOfNames - 1; i++)
    {
        vaName = (LPSTR)((DWORD_PTR)imgDosHeader + rvaName[i]);

        if (strcmp(name, vaName) == 0)
        {
            vaOrdinal = (DWORD)rvaOrdinal[i];
            break;
        }
    }

    vaAddr = (LPVOID)((DWORD_PTR)imgDosHeader + rvaAddr[vaOrdinal]);

    return vaAddr;
}

LPVOID call_api(LPCWSTR libFileName, LPCSTR name)
{
#ifndef _WIN64
    PPEB pPeb = (PPEB)__readfsdword(0x30);
#else
    PPEB pPeb = (PPEB)__readgsqword(0x60);
#endif

    HMODULE kernel32 = (HMODULE)(
        (PLDR_DATA_TABLE_ENTRY)(
            (PLDR_DATA_TABLE_ENTRY)(
                (PLDR_DATA_TABLE_ENTRY)
                pPeb->Ldr->InLoadOrderModuleList.Flink
                )->InLoadOrderLinks.Flink
            )->InLoadOrderLinks.Flink
        )->DllBase;

    LOADLIBRARYW ___LoadLibraryW = (LOADLIBRARYW)getAddrFunc(kernel32, (CHAR[13]) { 
        'L', 'o', 'a', 'd', 'L', 'i', 'b', 'r', 'a', 'r', 'y', 'W', '\0' 
    });

    return getAddrFunc(___LoadLibraryW(libFileName), name);
}
