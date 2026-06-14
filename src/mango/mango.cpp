/* ================================================================
*
*	Displays a message.
*
*	#Authors: The Kumor
*
* ================================================================ */

// WinAPI
#include <windows.h>

// Mango
#include <mango/application.h>

#define MANGO_HEIGHT 700
#define MANGO_WIDTH 900

#pragma warning(disable: 28251)
static int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR cmdLine, int cmdShow)
{
	mango::Application app(instance, L"Mango", MANGO_WIDTH, MANGO_HEIGHT);
	return app.Run();
}