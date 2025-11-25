#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

DWORD GetPid(char* targetProcess)
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap && snap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(pe);
        if (Process32First(snap, &pe))
        {
            do
            {
                if (!_stricmp(pe.szExeFile, targetProcess))
                {
                    CloseHandle(snap);
                    return pe.th32ProcessID;
                }
            } while (Process32Next(snap, &pe));
        }
    }
    return 0;
}

#define EXECUTABLE_NAME "cof.exe"

int main()
{
    char* dllpath = DLL_PATH;
    char* processToInject = EXECUTABLE_NAME;
    long pid = 0;

    pid = GetPid(processToInject); 
    if (!pid)
    {
        fputs("Please run Cry Of Fear first\n", stderr);
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
    if (!hProc || hProc == INVALID_HANDLE_VALUE) {
        fputs("Failed opening process", stderr);
        return -1;
    }
    void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (NULL == loc) {
        fputs("Failed allocating memory for dll path", stderr);
        return -1;
    }
    WriteProcessMemory(hProc, loc, dllpath, strlen(dllpath) + 1, 0);
    HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
    if (hThread) {
        CloseHandle(hThread);
    }

    CloseHandle(hProc);
    return 0;
}
