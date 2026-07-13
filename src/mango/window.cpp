#include "window.h"

namespace mango
{

	Window::Window(HINSTANCE instance, const std::wstring& title, Vec2i size)
		: Control(title, size, { 0, 0 }, nullptr)
	{
		m_Instance = instance;

		static const wchar_t* className = L"MangoWindow";

		static WNDCLASSEXW apiClass = { 0 };

		// #TODO: Make this more robust.
		if (apiClass.cbSize == 0)
		{
			apiClass.cbSize = sizeof(WNDCLASSEXW);
			apiClass.hInstance = instance;
			apiClass.lpszClassName = className;
			apiClass.cbClsExtra = 0;
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

		// #Note(kumor): this could be risky due to use of "this".
		SetWindowLongPtr(m_Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
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

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				RECT rc;
				static HBRUSH s_Brush = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
				GetClientRect(handle, &rc);
				HDC dc = BeginPaint(handle, &ps);

				FillRect(dc, &rc, s_Brush);

				EndPaint(handle, &ps);
			} break;

			case WM_SIZE:
			{
				Vec2i newSize = { (std::int32_t)LOWORD(lp), (std::int32_t)HIWORD(lp) };
				Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));

				if (!window)
					break;

				for (auto& k : window->m_Children)
				{
					// We must avoid using SetPos and SetSize because there's a difference between
					// logical size and actual size, for instance.

					Vec2i controlPos = k->GetPos();
					Vec2i controlSize = k->GetSize();

					Vec2i size = Vec2i((controlSize.X * newSize.x) / window->GetSize().X, (controlSize.Y * newSize.Y) / window->GetSize().Y);
					Vec2i pos = Vec2i((controlPos.X * newSize.x) / window->GetSize().X, (controlPos.Y * newSize.Y) / window->GetSize().Y);

					SetWindowPos(k->m_Handle, nullptr, pos.x, pos.y, size.x, size.y, SWP_NOZORDER);
				}
			} break;
		}

		return DefWindowProcW(handle, msg, wp, lp);
	}

}