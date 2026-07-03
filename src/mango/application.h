/* ================================================================
*
*	Contains main class.
*
*	#Authors: The Kumor
*
* ================================================================ */

// Mango
#include <mango/errors.h>
#include <mango/util.h>
#include <mango/controls.h>

// WinAPI
#include <windows.h>

// STL
#include <string>
#include <cstdlib>
#include <cstdint>

namespace mango
{

	class Application
	{
	public:
		Application(HINSTANCE instance, const std::wstring& title, Vec2i size);
		Application() = default;

		int Run();

	private:
		HINSTANCE m_Instance;
		Window m_Window;
	};

}