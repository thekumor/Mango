/* ================================================================
*
*	Displays a message.
*
*	#Authors: The Kumor
*
* ================================================================ */

// Mango
#include <mango/util.h>
#include <mango/application.h>

// WinAPI
#include <windows.h>

#define MANGO_HEIGHT 700
#define MANGO_WIDTH 900

#pragma warning(disable: 28251)
static int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR cmdLine, int cmdShow)
{
	using namespace mango;

	mango::Application app(instance, L"Mango", Vec2i(MANGO_WIDTH, MANGO_HEIGHT));
	return app.Run();
}