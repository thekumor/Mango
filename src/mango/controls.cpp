#include "controls.h"

namespace mango
{

	Control::Control(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent) :
		m_Handle(nullptr), m_Parent(parent), m_Size(size), m_Pos(pos), m_Text(text)
	{}

	Control::Control() :
		m_Parent(nullptr), m_Handle(nullptr), m_Size(0, 0), m_Pos(0, 0), m_Text(L"")
	{}

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
