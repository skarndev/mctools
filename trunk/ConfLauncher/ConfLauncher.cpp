// C++ Standard Library
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "ConfigFile.h"

#ifdef WIN64
#pragma message( " You may want to check the project settings, if everything is right! " ) 
#endif

#ifdef WIN32 || WIN64

// Windows API
#include <Windows.h>

BOOL LoadSeDebugPrivilege()
{
	HANDLE hToken = NULL; // Process token
	LUID Luid = { NULL }; // Locally unique identifier
	TOKEN_PRIVILEGES Privileges = { NULL }; // Process privileges
	BOOL Success = FALSE; // Return value. Assume failure

	__try
	{
		// Open current process token with adjust rights
		if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES
			| TOKEN_QUERY, &hToken)) __leave;
		if (hToken == NULL) __leave;

		// Get the LUID for SE_DEBUG_NAME 
		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid)) __leave;
		if (Luid.LowPart == NULL && Luid.HighPart == NULL) __leave;

		// Set the privileges we need
		Privileges.PrivilegeCount = 1;
		Privileges.Privileges[0].Luid = Luid;
		Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		// Apply the adjusted privileges
		if (!AdjustTokenPrivileges(hToken, FALSE, &Privileges,
			sizeof (Privileges), NULL, NULL)) __leave;

		// Setting SeDebugPrivelige worked
		Success = TRUE;
	}
	__finally
	{
		// Close token handle
		if (hToken != NULL)
			CloseHandle(hToken);
	}

	// Return the result of our 'privelige escalation' attempts
	return Success;
}

bool Write(HANDLE hProcess, int lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, std::string patchname)
{	
	if (!WriteProcessMemory(hProcess,reinterpret_cast<LPVOID>(lpBaseAddress),lpBuffer,nSize,NULL))
	{
		std::string error("Error! Could not patch "); 
		error.append(patchname);
		error.append("!");
		MessageBoxA(0, error.c_str() , "Injection Error", 0);
		return false;
	}
	return true;
}

// Load WoW and inject module
int LoadAndInject()
{
	// Get the full install path of WoW from the registry
	// Take install path and append executable name
//	std::string WoWPath(&InstallPath[0]);
//	WoWPath.append("WoW.exe");
	
	ConfigFile config( "Config.conf" );
	std::string WoWPath;
	config.readInto(WoWPath,"Path");
	
	// Set up structures for CreateProcess
	STARTUPINFO WoWSi;
	ZeroMemory(&WoWSi, sizeof(WoWSi));
	WoWSi.cb = sizeof(WoWSi);
	PROCESS_INFORMATION WoWPi;
	ZeroMemory(&WoWPi, sizeof(WoWPi));

	// Try and create the process from the current directory
	if (!CreateProcess("WoW.exe",NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&WoWSi,&WoWPi))
	{
		// Try and create the process from the install directory
		if (!CreateProcess(WoWPath.c_str(),NULL,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&WoWSi,&WoWPi))
		{
			// Both attempts at process creation failed
			MessageBoxA(0, "Error! Could not find WoW.exe.", "Injection Error", 0);
			return -1;
		}
	}

	// Get SeDebugPrivelege. Not necessary for WoW, but admin is required 
	// for injection to work properly either way, so leaving it here to
	// make it easier to port this to other games that do require elevated
	// privileges.
	if (!LoadSeDebugPrivilege())
	{
		// Attempt at getting SeDebugPrivilege failed.
		MessageBoxA(0, "Error! Could not get SeDebugPrivilege.", "Injection Error", 0);
		return -1;
	}


	if ( !( Write(WoWPi.hProcess, 0x007CC7C5, reinterpret_cast<PBYTE>( "\xEB\x5B" ), 2, "GlueXML_AdditionalScan")
		 && Write(WoWPi.hProcess, 0x0077BB0D, reinterpret_cast<PBYTE>( "\x33\xC0" ), 2, "SignatureCheck")
		 && Write(WoWPi.hProcess, 0x0077BB1C, reinterpret_cast<PBYTE>( "\x33\xC0" ), 2, "SignatureCheck2")
		 && Write(WoWPi.hProcess, 0x006A975B, reinterpret_cast<PBYTE>( "\x90\x90" ), 2, "ModelEdit")
		 && Write(WoWPi.hProcess, 0x00472D9B, reinterpret_cast<PBYTE>( "\x90\x90\x90\x90\x90" ), 5, "CheckSum") ))
	{
		TerminateProcess(WoWPi.hProcess,0);
		return -1;
	}

	
	
	ResumeThread(WoWPi.hThread);

	
/*	Patch(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, name);

	GetPatcher()->AddPatch( gpWoWX->GetFindPattern()->GetAddress( "GlueXML_AdditionalScan" ), reinterpret_cast<PBYTE>( "\xEB\x5B" ), 2, "GlueXML_AdditionalScan" ); 
		// needed, include
		// 8B 4D ? 33 C0 83 F9 ? 77
		// 007CC7C5
		// jmp +0x5B
	
	 
	GetPatcher()->AddPatch( gpWoWX->GetFindPattern()->GetAddress( "SignatureCheck" ), reinterpret_cast<PBYTE>( "\x33\xC0" ), 2, "SignatureCheck" );
		// needed, include
		// 1B C0 5F 83 C0 03 5E 8B E5 5D C3 33 C0 F7 D8 1B C0 5F 83 C0 03 5E 8B E5
		// 0077BB0D
		// xor eax, eax
	GetPatcher()->AddPatch( gpWoWX->GetFindPattern()->GetAddress( "SignatureCheck2" ), reinterpret_cast<PBYTE>( "\x33\xC0" ), 2, "SignatureCheck2" );
		// needed, include
		// 0077BB1C
		// xor eax, eax

	GetPatcher()->AddPatch( gpWoWX->GetFindPattern()->GetAddress( "ModelEdit" ), reinterpret_cast<PBYTE>( "\x90\x90" ), 2, "ModelEdit" );
		// needed, include
		// pattern="8B E5 5D C2 ? ? 8B 4D ? 89 3D ? ? ? ? 89 0D ? ? ? ? 83 3D ? ? ? ? ? 74"
		//   <Add value="1C" />
		// 006A975B
		// nop
		// nop*/

	// Close process and thread handles.
	CloseHandle(WoWPi.hThread);
	CloseHandle(WoWPi.hProcess);

	// Success
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// Load WoW and inject DLL
	int RetVal = LoadAndInject();
	// Exit
	return RetVal;
}

#else

#pragma message( " No support for other OSes yet! " ) 

#endif
