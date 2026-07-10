/* ================================================================
*
*	Contains main class.
*
*	#Authors: The Kumor
*
* ================================================================ */

#pragma once

// Mango
#include <mango/errors.h>
#include <mango/util.h>
#include <mango/controls.h>
#include <mango/window.h>

// WinAPI
#include <windows.h>

// STL
#include <string>
#include <cstdlib>
#include <cstdint>
#include <unordered_map>

namespace mango
{

	class Application
	{
	public:
		Application(HINSTANCE instance, const std::wstring& title, Vec2i size);
		Application() = default;
		~Application();

		int Run();

	private:
		HINSTANCE m_Instance = nullptr;
		std::unordered_map<HWND, Window*> m_Windows = { };
		Window* m_Window = nullptr; // This works with m_Windows
	};

}