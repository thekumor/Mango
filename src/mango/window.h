/* ================================================================
*
*	Window definition and its functionality.
*
*	#Authors: The Kumor
*
* ================================================================ */

#pragma once

// Mango
#include <mango/controls.h>

// WinAPI
#include <windows.h>

namespace mango
{

	class Window : public Control
	{
	public:
		Window(HINSTANCE instance, const std::wstring& title, Vec2i size);
		Window() = default;

		friend class Application;

		static LRESULT s_Procedure(HWND handle, UINT msg, WPARAM wp, LPARAM lp);

		template <typename T>
		T* AddChild(const std::wstring& text, Vec2f size, Vec2f pos)
		{
			Vec2i actualSize = Vec2i((std::int32_t)(size.X * m_Size.X), (std::int32_t)(size.Y * m_Size.Y));
			Vec2i actualPos = Vec2i((std::int32_t)(pos.X * m_Size.X), (std::int32_t)(pos.Y * m_Size.Y));

			return (T*)Control::AddChild<T>(text, actualSize, actualPos);
		}

	private:
		HINSTANCE m_Instance;
	};

}