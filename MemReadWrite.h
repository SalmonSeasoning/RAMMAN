#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif


#include <Windows.h>
#include <TlHelp32.h>
#include <string>

namespace RAMMAN {

	class MemReadWrite
	{
	public:

		// Constructor
		MemReadWrite() {}

		// Constructor - set new handle
		MemReadWrite(HANDLE newHandle)
		{
			handle = newHandle;
		}

		// Constructor - get handle from process name and set
		MemReadWrite(LPCWCHAR procName)
		{
			HANDLE currentHandle = GetProcessHandleByName(procName);
			if (currentHandle != NULL)
			{
				handle = currentHandle;
			}

		}

		// Constructor - get handle from process ID and set
		MemReadWrite(DWORD procId)
		{
			HANDLE currentHandle = GetProcessHandleById(procId);
			if (currentHandle != NULL)
			{
				handle = currentHandle;
			}
		}
		
		// Destructor - self explanatory
		~MemReadWrite()
		{
			handle = NULL;
		}

		// GetProcessHandleByName - return a handle of the target process found by name
		static HANDLE GetProcessHandleByName(LPCWCHAR procName)
		{
			HWND hwnd = FindWindowW(NULL, procName);
			if (hwnd == NULL) return NULL;
			DWORD procId;
			GetWindowThreadProcessId(hwnd, &procId);
			if (!procId) return NULL;
			HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
			if (pHandle == NULL) return NULL;
			return pHandle;
		}

		// GetProcessHandleByName2 - return a handle of the target process found by name
		static HANDLE GetProcessHandleByName2(LPCWCHAR procName)
		{
			HANDLE hProcId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			DWORD procId;
			PROCESSENTRY32 pEntry;
			pEntry.dwSize = sizeof(pEntry);

			do {
				if (!_wcsicmp(pEntry.szExeFile, procName))
				{
					procId = pEntry.th32ProcessID;
					CloseHandle(hProcId);
					break;
				}
			} while (Process32Next(hProcId, &pEntry));
			if (!procId) return NULL;
			HANDLE newHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
			if (newHandle == NULL) return NULL;
			return newHandle;
		}

		// GetProcessIdByName - return a DWORD ID of the target process found by name
		static DWORD GetProcessIdByName(LPCWCHAR procName)
		{
			HWND hwnd = FindWindowW(NULL, procName);
			if (hwnd == NULL) return NULL;
			DWORD procId;
			GetWindowThreadProcessId(hwnd, &procId);
			if (!procId) return NULL;
			return procId;
		}

		// GetProcessIdByName - return a DWORD ID of the target process found by name
		static DWORD GetProcessIdByName2(LPCWCHAR procName)
		{
			HANDLE hProcId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			DWORD procId;
			PROCESSENTRY32 pEntry;
			pEntry.dwSize = sizeof(pEntry);

			do {
				if (!_wcsicmp(pEntry.szExeFile, procName))
				{
					procId = pEntry.th32ProcessID;
					CloseHandle(hProcId);
					break;
				}
			} while (Process32Next(hProcId, &pEntry));
			if (!procId) return NULL;
			return procId;
		}

		// GetProcessHandleById - return a handle of the target process found by Id
		static HANDLE GetProcessHandleById(DWORD procId)
		{
			HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
			if (pHandle == NULL) return NULL;
			return pHandle;
		}

		// GetModuleAddress - get address of a module
		static LPBYTE* GetModuleAddress(DWORD procId, LPCWCHAR modName)
		{
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
			if (hSnap == INVALID_HANDLE_VALUE) return NULL;
			MODULEENTRY32 mod;
			mod.dwSize = sizeof(mod);
			LPBYTE* modAddr = NULL;
			if (Module32First(hSnap, &mod))
			{
				do
				{
					if (!_wcsicmp(mod.szModule, modName))
					{
						modAddr = (LPBYTE*)mod.modBaseAddr;
						break;
					}
				} while (Module32Next(hSnap, &mod));
			}
			CloseHandle(hSnap);
			return modAddr;
		}

		// StaticReadMemory - Get value from memory
		template <class T>
		T StaticReadMemory(HANDLE reqHandle, DWORD address)
		{
			T _var;
			ReadProcessMemory(reqHandle, (LPBYTE*)address, &_var, sizeof(_var), NULL);
			return _var;
		}
		template <class T>
		T StaticReadMemory(HANDLE reqHandle, LPBYTE* address)
		{
			T _var;
			ReadProcessMemory(reqHandle, address, &_var, sizeof(_var), NULL);
			return _var;
		}

		// StaticWriteMemory - Write value to memory
		template <class T>
		T StaticWriteMemory(HANDLE reqHandle, DWORD address, T value)
		{
			WriteProcessMemory(reqHandle, (LPBYTE*)address, &value, sizeof(value), NULL);
			return value;
		}
		template <class T>
		T StaticWriteMemory(HANDLE reqHandle, LPBYTE* address, T value)
		{
			WriteProcessMemory(reqHandle, address, &value, sizeof(value), NULL);
			return value;
		}

		// ReadMemory - Use set handle, get value from memory
		template <class T>
		T ReadMemory(DWORD address)
		{
			T _var;
			ReadProcessMemory(handle, (LPBYTE*)address, &_var, sizeof(_var), NULL);
			return _var;
		}
		template <class T>
		T ReadMemory(LPBYTE* address)
		{
			T _var;
			ReadProcessMemory(handle, address, &_var, sizeof(_var), NULL);
			return _var;
		}

		// WriteMemory - Use set handle, write value to memory
		template <class T>
		T WriteMemory(DWORD address, T value)
		{
			WriteProcessMemory(handle, (LPBYTE*)address, &value, sizeof(value), NULL);
			return value;
		}
		template <class T>
		T WriteMemory(LPBYTE* address, T value)
		{
			WriteProcessMemory(handle, address, &value, sizeof(value), NULL);
			return value;
		}

		// SetHandle - Set saved handle
		HANDLE SetHandle(HANDLE newHandle)
		{
			handle = newHandle;
			return handle;
		}

		// CallCloseHandleAndClear - Close handle using WINAPI CloseHandle and set to NULL
		VOID CallCloseHandleAndClear()
		{
			CloseHandle(handle);
			handle = NULL;
		}

		// GetHandle - Get current handle
		HANDLE GetHandle()
		{
			return handle;
		}
	private:
		HANDLE handle = NULL;
	};

};