#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "main.h"

namespace Functions
{
	void ShowAllProcesses();
	void ShowProcessModules(const HANDLE process);
	void Inject(const HANDLE process, const std::string &file_path);
};

#endif