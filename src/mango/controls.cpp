#include "controls.h"

namespace mango
{

	Control::Control(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent) :
		m_Handle(nullptr), m_Parent(parent), m_Size(size), m_Pos(pos), m_Text(text)
	{}

	Control::Control() :
		m_Parent(nullptr), m_Handle(nullptr), m_Size(0, 0), m_Pos(0, 0), m_Text(L"")
	{}

	TextImage* Control::AddTextImage(const std::wstring& altText, const std::string& path, Vec2i size, Vec2i pos, Control* parent)
	{
		return dynamic_cast<TextImage*>(m_Children.emplace_back(dynamic_cast<Control*>(new TextImage(altText, path, size, pos, this))));
	}

	void Control::SetFont(Font* font)
	{
		SendMessageW(m_Handle, WM_SETFONT, (LPARAM)font->m_Handle, TRUE);
	}

	void Control::SetPos(Vec2i pos)
	{
		m_Pos = pos;
		SetWindowPos(m_Handle, nullptr, pos.x, pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void Control::SetSize(Vec2i size)
	{
		m_Size = size;
		SetWindowPos(m_Handle, nullptr, 0, 0, size.x, size.y, SWP_NOMOVE | SWP_NOZORDER);
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

	WNDCLASSEXW TextImage::s_Class = { 0 };

	LRESULT TextImage::s_Procedure(HWND handle, UINT msg, WPARAM wp, LPARAM lp)
	{
		switch (msg)
		{
			case WM_PAINT:
			{
				TextImage* self = reinterpret_cast<TextImage*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
				if (!self)
					break;

				// Paint image over.
				Vec2i imageSize = self->m_Image.GetSize();

				PAINTSTRUCT ps;
				RECT rc;
				HDC dc = BeginPaint(handle, &ps);
				GetClientRect(handle, &rc);

				//FillRect(dc, &rc, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

				BITMAPINFO bmi = { 0 };
				bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bmi.bmiHeader.biWidth = imageSize.X;
				bmi.bmiHeader.biHeight = -imageSize.Y;
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 32;
				bmi.bmiHeader.biCompression = BI_RGB;

				HDC memDC = CreateCompatibleDC(dc);
				void* bits = nullptr;

				HBITMAP dib = CreateDIBSection(
					dc,
					&bmi,
					DIB_RGB_COLORS,
					&bits,
					nullptr,
					0
				);
				memcpy(bits, self->m_Image.GetData(), imageSize.X * imageSize.Y * 4);

				//void* bits2 = nullptr;
				//StretchDIBits(
				//	dc,
				//	0,
				//	0,
				//	self->GetSize().X,
				//	self->GetSize().Y,
				//	0,
				//	0,
				//	self->m_Image.GetSize().X,
				//	self->m_Image.GetSize().Y,
				//	&bits2,
				//	&bmi,
				//	DIB_RGB_COLORS,
				//	PATPAINT
				//);

				HBITMAP old = reinterpret_cast<HBITMAP>(SelectObject(memDC, dib));

				BLENDFUNCTION bf = {
					AC_SRC_OVER,
					0,
					255,
					AC_SRC_ALPHA
				};

				Vec2i realSize = { rc.right - rc.left, rc.bottom - rc.top };

				AlphaBlend(
					dc,
					0, 0, realSize.X, realSize.Y,
					memDC,
					0, 0, imageSize.X, imageSize.Y,
					bf
				);

				SelectObject(memDC, old);
				DeleteObject(dib);
				DeleteDC(memDC);

			} break;
		}

		return DefWindowProcW(handle, msg, wp, lp);
	}

	TextImage::TextImage(const std::wstring& altText, const std::string& path, Vec2i size, Vec2i pos, Control* parent) :
		Control(altText, size, pos, parent), m_Image(path)
	{
		static const wchar_t* className = L"TextImage";

		// #TODO: Make this more robust.
		if (s_Class.cbSize == 0)
		{
			HINSTANCE instance = reinterpret_cast<HINSTANCE>(GetModuleHandle(nullptr));

			s_Class.cbSize = sizeof(WNDCLASSEXW);
			s_Class.hInstance = instance;
			s_Class.lpszClassName = className;
			s_Class.cbClsExtra = 0;
			s_Class.hCursor = LoadCursorW(nullptr, IDC_ARROW);
			s_Class.lpfnWndProc = TextImage::s_Procedure;

			RegisterClassExW(&s_Class);
			GetError(L"Window.RegisterClassExW");
		}

		m_Handle = CreateWindowExW(
			0,
			className,
			static_cast<LPCWSTR>(altText.c_str()),
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
		GetError(L"TextImage.CreateWindowExW");

		SetWindowLongPtrW(m_Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}

	Font::Font(const std::wstring& name, std::int32_t size, bool bold, bool italic)
		: m_Name(name), m_Size(size), m_Bold(bold), m_Italic(italic)
	{
		m_Handle = CreateFontW(
			size,
			0,
			0,
			0,
			bold ? FW_BOLD : FW_NORMAL,
			italic ? TRUE : FALSE,
			FALSE,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			name.c_str()
		);
		GetError(L"Font.CreateFontW");
	}

}
