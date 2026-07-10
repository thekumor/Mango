/* ================================================================
*
*	Contains all controls this app has (like buttons, texts, etc.)
*
*	#Authors: The Kumor
*
* ================================================================ */

#pragma once

// Mango
#include <mango/util.h>
#include <mango/errors.h>

// WinAPI
#include <windows.h>

// STL
#include <string>
#include <vector>

namespace mango
{

	class Font;

	class Control
	{
	public:
		Control(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent = nullptr);
		Control();

		template <typename T>
		T* AddChild(const std::wstring& text, Vec2i size, Vec2i pos)
		{
			return (T*)m_Children.emplace_back((Control*)new T(text, size, pos, this));
		}

		Control* GetParent() const { return m_Parent; }
		Vec2i GetSize() const { return m_Size; }
		Vec2i GetPos() const { return m_Pos; }
		void SetFont(Font* font);
		void SetPos(Vec2i pos);
		void SetSize(Vec2i size);

		friend class Window;
		friend class Text;
		friend class Button;

	protected:
		Control* m_Parent;
		std::vector<Control*> m_Children;
		HWND m_Handle;
		Vec2i m_Size, m_Pos;
		std::wstring m_Text;
	};

	class Text : public Control
	{
	public:
		Text(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent = nullptr);
		Text() = default;
	};

	class Button : public Control
	{
	public:
		Button(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent = nullptr);
		Button() = default;
	};

	class Font
	{
	public:
		Font(const std::wstring& name, std::int32_t size, bool bold = false, bool italic = false);
		Font() = default;

		friend class Control;

	private:
		HFONT m_Handle;
		std::wstring m_Name;
		std::int32_t m_Size;
		bool m_Bold, m_Italic;
	};

}