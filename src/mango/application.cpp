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
		Font subHeaderFont(L"Georgia", 48, true, false);
		Font arrowFont(L"Arial", 24, true, false);

		Text* title = m_Window->AddChild<Text>(L"Mango", { 0.50f, 0.10f }, { 0.25f, 0.00f });
		title->SetFont(&headerFont);
		title->SetCentered(true);

		TextImage* img = m_Window->AddTextImage(L"Test", "img/mango.png", { 0.15f, 0.15f }, { 0.60f, 0.05f});

		Text* series = m_Window->AddChild<Text>(L"Series", { 0.25f, 0.10f }, { 0.05f, 0.02f });
		series->SetFont(&subHeaderFont);
		series->SetCentered(true);

		Button* up = m_Window->AddChild<Button>(L"↑", { 0.04f, 0.05f }, { 0.32f, 0.15f });
		up->SetFont(&arrowFont);

		Button* down = m_Window->AddChild<Button>(L"↓", { 0.04f, 0.05f }, { 0.32f, 0.21f });
		down->SetFont(&arrowFont);

		Directory dataDir(L"data/*");

		for (std::int32_t i = 0; i < 15; i++)
		{
			std::wstring fileName = L"";

			if (dataDir.GetFiles().size() > i)
				fileName = dataDir.GetFiles()[i].GetName();

			Button* text = m_Window->AddChild<Button>(fileName, {0.25f, 0.05f}, {0.05f, 0.05f * i + 0.12f});
			text->SetFont(&mainFont);
		}

		File testFile(L"data/1.ini", FileMode::Read);
		INIData data;
		data.ConstructFromFile(&testFile);

		MSG msg = { 0 };
		while (GetMessageW(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return EXIT_SUCCESS;
	}

}