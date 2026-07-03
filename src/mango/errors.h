/* ================================================================
*
*	Error management & logging for Win32.
*
*	#Authors: The Kumor
*
* ================================================================ */

#pragma once

// WinAPI
#include <windows.h>

// STL
#include <string>

namespace mango
{
	
	DWORD GetError(const std::wstring& info);

}