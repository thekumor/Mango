#include "application.h"

namespace mango
{

	Application::Application(HINSTANCE instance, const std::wstring& title, Vec2i size)
		: m_Instance(instance)
	{
		Window* window = new Window(instance, title, size);
		m_Windows[window->m_Handle] = window;

		m_Window = m_Windows[window->m_Handle];
	}

	Application::~Application()
	{
		for (std::unordered_map<HWND, Window*>::iterator it = m_Windows.begin(); it != m_Windows.end(); it++)
			delete it->second;
	}

	int Application::Run()
	{
		Font mainFont(L"Georgia", 24, false, false);
		Font headerFont(L"Georgia", 64, true, false);

		Text* title = m_Window->AddChild<Text>(L"Mango", { 0.50f, 0.10f }, { 0.25f, 0.05f });
		title->SetFont(&headerFont);
		title->SetCentered(true);

		TextImage* img = m_Window->AddTextImage(L"Test", "img/mango.png", { 0.25f, 0.25f }, { 0.75f, 0.25f});

		MSG msg = { 0 };
		while (GetMessageW(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return EXIT_SUCCESS;
	}

}