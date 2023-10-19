#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

// Function to get the process ID by name
DWORD GetProcessIdByName(const wchar_t* processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(processEntry);
        if (Process32FirstW(snapshot, &processEntry)) {
            do {
                // Check if the process name matches the specified name
                if (_wcsicmp(processEntry.szExeFile, processName) == 0) {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }
    return processId;
}

int main() {
    const wchar_t* processName = L"MyApplication.exe"; // Specify the target process name
    const wchar_t* dllPath = L"MyDll.dll"; // Specify the path to the DLL to be injected

    // Get the process ID of the target process
    DWORD targetProcessId = GetProcessIdByName(processName);

    if (targetProcessId == 0) {
        std::wcerr << L"The target process is not running." << std::endl;
        return 1;
    }

    // Open the target process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);

    if (hProcess == NULL) {
        std::wcerr << L"Failed to open the target process." << std::endl;
        return 1;
    }

    // Allocate memory in the target process for the DLL path
    LPVOID remoteString = VirtualAllocEx(hProcess, NULL, (wcslen(dllPath) + 1) * sizeof(wchar_t), MEM_COMMIT, PAGE_READWRITE);

    if (remoteString == NULL) {
        std::wcerr << L"Failed to allocate memory in the target process." << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Write the DLL path to the allocated memory in the target process
    if (!WriteProcessMemory(hProcess, remoteString, dllPath, (wcslen(dllPath) + 1) * sizeof(wchar_t), NULL)) {
        std::wcerr << L"Failed to write the DLL path to the target process." << std::endl;
        VirtualFreeEx(hProcess, remoteString, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Create a remote thread in the target process to load the DLL
    HANDLE remoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, remoteString, 0, NULL);

    if (remoteThread == NULL) {
        std::wcerr << L"Failed to create a remote thread in the target process." << std::endl;
        VirtualFreeEx(hProcess, remoteString, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Wait for the remote thread to finish and clean up
    WaitForSingleObject(remoteThread, INFINITE);
    CloseHandle(remoteThread);

    VirtualFreeEx(hProcess, remoteString, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}
