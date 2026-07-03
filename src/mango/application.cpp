#include "application.h"

namespace mango
{

	Application::Application(HINSTANCE instance, const std::wstring& title, Vec2i size)
		: m_Instance(instance), m_Window(instance, title, size)
	{
	}

	int Application::Run()
	{
		m_Window.AddChild<Text>(L"Hello", { 250, 30 }, {5, 5});
		m_Window.AddChild<Button>(L"Test", { 250, 30 }, { 40, 40 });

		MSG msg = { 0 };
		while (GetMessageW(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return EXIT_SUCCESS;
	}

}