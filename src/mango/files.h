#pragma once

// Mango
#include <mango/errors.h>

// STL
#include <fstream>

namespace mango
{

	enum class FileMode : std::ios::openmode
	{
		None = 0,
		Read = std::ios::in,
		Write = std::ios::out,
		Append = std::ios::app
	};

	class File
	{
	public:
		File(const std::wstring& path, FileMode mode);
		File() = default;

		FileMode GetMode() const { return m_Mode; }
		std::wstring GetPath() const { return m_Path; }
		void Open(const std::wstring& path, FileMode mode);
		void Close();

	private:
		std::wstring m_Path;
		HANDLE m_Handle = nullptr;
		FileMode m_Mode = FileMode::None;
	};

}