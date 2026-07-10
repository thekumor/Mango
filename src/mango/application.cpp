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

		Text* text = m_Window->AddChild<Text>(L"Hello", { 0.50f, 0.10f }, { 0.25f, 0.10f });
		text->SetFont(&mainFont);

		Button* btn = m_Window->AddChild<Button>(L"Test", { 0.25f, 0.12f }, { 0.38f, 0.30f });
		btn->SetFont(&mainFont);

		MSG msg = { 0 };
		while (GetMessageW(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return EXIT_SUCCESS;
	}

}