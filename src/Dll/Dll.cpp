#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        MessageBoxW(0, L"Hello, World!", L"Hello DLL", MB_ICONINFORMATION);
    }
    return TRUE;
}
