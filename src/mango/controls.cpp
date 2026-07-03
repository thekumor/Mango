#include "controls.h"

namespace mango
{

	Control::Control(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent) :
		m_Handle(nullptr), m_Parent(parent), m_Size(size), m_Pos(pos), m_Text(text)
	{}

	Control::Control() :
		m_Parent(nullptr), m_Handle(nullptr), m_Size(0, 0), m_Pos(0, 0), m_Text(L"")
	{}

	Window::Window(HINSTANCE instance, const std::wstring& title, Vec2i size)
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
			GetError(L"Window.RegisterClassExW");
		}

		m_Handle = CreateWindowExW(
			0,
			className,
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			size.x,
			size.y,
			nullptr,
			nullptr,
			instance,
			0
		);
		GetError(L"Window.CreateWindowExW");

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

	Text::Text(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent) :
		Control(text, size, pos, parent)
	{
		m_Handle = CreateWindowExW(
			0,
			L"Static",
			static_cast<LPCWSTR>(text.c_str()),
			WS_VISIBLE | (parent ? WS_CHILD : 0),
			pos.x,
			pos.y,
			size.x,
			size.y,
			parent ? parent->m_Handle : 0,
			nullptr,
			GetModuleHandleW(nullptr),
			0
		);
		GetError(L"Text.CreateWindowExW");
	}

	Button::Button(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent) :
		Control(text, size, pos, parent)
	{
		m_Handle = CreateWindowExW(
			0,
			L"Button",
			static_cast<LPCWSTR>(text.c_str()),
			WS_VISIBLE | (parent ? WS_CHILD : 0),
			pos.x,
			pos.y,
			size.x,
			size.y,
			parent ? parent->m_Handle : 0,
			nullptr,
			GetModuleHandleW(nullptr),
			0
		);
		GetError(L"Button.CreateWindowExW");
	}

}
