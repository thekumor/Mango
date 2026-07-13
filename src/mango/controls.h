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
#include <mango/image.h>

// WinAPI
#include <windows.h>

// STL
#include <string>
#include <vector>

namespace mango
{

	class Font;
	class TextImage;

	class Control
	{
	public:
		Control(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent = nullptr);
		Control();
		virtual ~Control() {};

		template <typename T>
		T* AddChild(const std::wstring& text, Vec2i size, Vec2i pos)
		{
			return dynamic_cast<T*>(m_Children.emplace_back(dynamic_cast<Control*>(new T(text, size, pos, this))));
		}

		Control* GetParent() const { return m_Parent; }
		Vec2i GetSize() const { return m_Size; }
		Vec2i GetPos() const { return m_Pos; }
		TextImage* AddTextImage(const std::wstring& altText, const std::string& path, Vec2i size, Vec2i pos, Control* parent = nullptr);
		void SetFont(Font* font);
		void SetPos(Vec2i pos);
		void SetSize(Vec2i size);

		friend class Window;
		friend class Text;
		friend class Button;
		friend class TextImage;

	protected:
		Control* m_Parent;
		std::vector<Control*> m_Children;
		HWND m_Handle;
		Vec2i m_Size, m_Pos;
		std::wstring m_Text;
	};

	class Text : public virtual Control
	{
	public:
		Text(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent = nullptr);
		Text() = default;
	};

	class Button : public virtual Control
	{
	public:
		Button(const std::wstring& text, Vec2i size, Vec2i pos, Control* parent = nullptr);
		Button() = default;
	};

	class TextImage : public virtual Control
	{
	public:
		TextImage(const std::wstring& altText, const std::string& path, Vec2i size, Vec2i pos, Control* parent = nullptr);
		TextImage() = default;

		static WNDCLASSEXW s_Class;
		static LRESULT s_Procedure(HWND handle, UINT msg, WPARAM wp, LPARAM lp);

	private:
		Image m_Image;
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