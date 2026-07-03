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
	class Control
	{
	public:
		Control(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent = nullptr);
		Control();

		template <typename T>
		T* AddChild(const std::wstring& text, Vec2i size, Vec2i pos)
		{
			T* child = (T*)m_Children.emplace_back((Control*)new T(text, size, pos, this));
			return child;
		}

		Control* GetParent() const { return m_Parent; }

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

	class Window : public Control
	{
	public:
		Window(HINSTANCE instance, const std::wstring& title, Vec2i size);
		Window() = default;

		static LRESULT s_Procedure(HWND handle, UINT msg, WPARAM wp, LPARAM lp);

	private:
		HINSTANCE m_Instance;
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
}