#pragma once
#include <windows.h>

bool InjectDll(DWORD processId, const std::string& dllPath);
