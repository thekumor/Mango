/* ================================================================
*
*	Contains main class.
*
*	#Authors: The Kumor
*
* ================================================================ */

// STL
#include <string>
#include <cstdlib>
#include <cstdint>

// WinAPI
#include <windows.h>

namespace mango
{

	class Window
	{
	public:
		Window(HINSTANCE instance, const std::wstring& title, std::int32_t x, std::int32_t y);
		Window() = default;

		static LRESULT s_Procedure(HWND handle, UINT msg, WPARAM wp, LPARAM lp);

	private:
		HINSTANCE m_Instance;
		HWND m_Handle;
		std::wstring m_Title;
		std::int32_t m_X, m_Y;
	};

	class Application
	{
	public:
		Application(HINSTANCE instance, const std::wstring& title, std::int32_t x, std::int32_t y);
		Application() = default;

		int Run();

	private:
		HINSTANCE m_Instance;
		Window m_Window;
	};

}