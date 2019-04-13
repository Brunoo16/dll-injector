#include "functions.h"

namespace Functions
{
	void ShowAllProcesses()
	{
		PROCESSENTRY32 process_struct = { 0 };
		process_struct.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (snapshot_handle == INVALID_HANDLE_VALUE)
		{
			std::cout << "It wasn't possible to show all processes: INVALID_HANDLE_VALUE." << std::endl;
		}
		else
		{
			if (Process32First(snapshot_handle, &process_struct))
			{
				do
				{
					std::wcout << "Process ID: " << process_struct.th32ProcessID << " | Process Name: " << process_struct.szExeFile << std::endl;
				} 
				while (Process32Next(snapshot_handle, &process_struct));
			}
			CloseHandle(snapshot_handle);
		}
	}

	void ShowProcessModules(const HANDLE process)
	{
		WCHAR module_name[MAX_PATH];
		HMODULE module_handle_array[1024];
		DWORD bytes_required;

		if (EnumProcessModules(process, module_handle_array, sizeof(module_handle_array), &bytes_required))
		{
			for (unsigned int i = 0; i < (bytes_required / sizeof(HMODULE)); i++)
			{
				if (GetModuleFileNameEx(process, module_handle_array[i], module_name, sizeof(module_name) / sizeof(WCHAR)))
				{
					std::wcout << "Name: " << module_name << " | Handle: 0x" << std::hex << std::uppercase << reinterpret_cast<DWORD>(module_handle_array[i]) << std::endl;
				}
			}
		}
		else
		{
			std::cout << "It wasn't possible to show the modules: EnumProcessModules failed." << std::endl;
		}
	}

	void Inject(const HANDLE process, const std::string &file_path)
	{
		void* virtual_alloc = VirtualAllocEx(process, NULL, file_path.size() + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		
		if (virtual_alloc)
		{
			if (WriteProcessMemory(process, virtual_alloc, file_path.c_str(), file_path.size() + 1, 0))
			{
				DWORD thread_identifier = 0;
				LPTHREAD_START_ROUTINE load_library = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(LoadLibraryA("kernel32"), "LoadLibraryA"));
								
				if (CreateRemoteThread(process, NULL, 0, load_library, virtual_alloc, 0, &thread_identifier))
				{
					if (thread_identifier)
					{
						std::cout << "DLL injected." << std::endl;
					}
					else
					{
						std::cout << "DLL injection failed: thread_identifier is NULL." << std::endl;
					}
				}
				else
				{
					std::cout << "DLL injection failed: CreateRemoteThread returned NULL." << std::endl;
				}
			}
			else
			{
				std::cout << "DLL injection failed: WriteProcessMemory failed." << std::endl;
			}
		}
	}
};