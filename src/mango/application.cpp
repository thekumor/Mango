#include "application.h"

namespace mango
{

	Window::Window(HINSTANCE instance, const std::wstring& title, std::int32_t x, std::int32_t y)
		: m_Instance(instance), m_Title(title), m_X(x), m_Y(y)
	{
		static const wchar_t* className = L"MangoWindow";

		static WNDCLASSEXW apiClass = { 0 };

		// #TODO: Make this more robust.
		if (apiClass.cbSize == 0)
		{
			apiClass.cbSize = sizeof(WNDCLASSEXW);
			apiClass.hInstance = instance;
			apiClass.lpszClassName = className;
			apiClass.cbClsExtra = 0;
			apiClass.hbrBackground = CreateSolidBrush(RGB(100, 100, 120));
			apiClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
			apiClass.lpfnWndProc = Window::s_Procedure;

			RegisterClassExW(&apiClass);
		}

		m_Handle = CreateWindowExW(
			0,
			className,
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			x,
			y,
			nullptr,
			nullptr,
			instance,
			0
		);

		ShowWindow(m_Handle, SW_SHOW);
	}

	LRESULT Window::s_Procedure(HWND handle, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
			case WM_DESTROY:
			{
				PostQuitMessage(EXIT_SUCCESS);
			} break;

			case WM_CLOSE:
			{
				DestroyWindow(handle);
			} break;
		}

		return DefWindowProcW(handle, msg, wp, lp);
	}

	Application::Application(HINSTANCE instance, const std::wstring& title, std::int32_t x, std::int32_t y)
		: m_Instance(instance), m_Window(instance, title, x, y)
	{
	}

	int Application::Run()
	{
		MSG msg = { 0 };
		while (GetMessageW(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return EXIT_SUCCESS;
	}

}