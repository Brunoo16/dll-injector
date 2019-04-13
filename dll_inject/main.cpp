#include "main.h"

int main()
{
	std::cout << "Options: 0 - Show all processes | 1 - Show process modules | 2 - Inject DLL." << std::endl;

	std::string input;
	int selected_option = 0;
	DWORD selected_process_id = 0;
	HANDLE process_handle = 0;

	while (1)
	{
		std::getline(std::cin, input);

		std::stringstream stream(input);
		if (!static_cast<bool>(stream >> selected_option))
		{
			selected_option = 0;
		}

		switch (selected_option)
		{
			case 0:
			{
				Functions::ShowAllProcesses();
				break;
			}
			case 1:
			{
				std::cout << "Provide the process ID below:" << std::endl;

				std::getline(std::cin, input);
				stream.clear();
				stream.str(input);

				if (!static_cast<bool>(stream >> selected_process_id))
				{
					selected_process_id = 0;
				}

				process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, selected_process_id);

				if (WaitForSingleObject(process_handle, 0) == WAIT_TIMEOUT)
				{
					Functions::ShowProcessModules(process_handle);
				}
				else
				{
					std::cout << "That process is not running." << std::endl;
				}
				CloseHandle(process_handle);
				break;
			}
			case 2:
			{
				std::cout << "Provide the process ID below:" << std::endl;

				std::getline(std::cin, input);
				stream.clear();
				stream.str(input);

				if (!static_cast<bool>(stream >> selected_process_id))
				{
					selected_process_id = 0;
				}

				process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, selected_process_id);

				if (WaitForSingleObject(process_handle, 0) == WAIT_TIMEOUT)
				{
					std::cout << "Give the dll path below:" << std::endl;
					std::getline(std::cin, input);

					std::ifstream dll_file(input);
					if (dll_file.good())
					{
						Functions::Inject(process_handle, input);
					}
					else
					{
						std::cout << "Invalid path provided. Couldn't find the file." << std::endl;
					}
				}
				else
				{
					std::cout << "That process is not running." << std::endl;
				}
				CloseHandle(process_handle);
				break;
			}
		}
	}
	return 0;
}