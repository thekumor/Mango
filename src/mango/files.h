/* ================================================================
*
*	Read/write to a file.
*
*	#Authors: The Kumor
*
* ================================================================ */

#pragma once

// Mango
#include <mango/errors.h>

// STL
#include <fstream>
#include <optional>

namespace mango
{
	enum class FileMode : std::uint32_t
	{
		None = 0,
		Read = 1 << 0,
		Write = 1 << 1,
		Append = 1 << 2,
	};
	
	FileMode operator|(FileMode lhs, FileMode rhs);
	bool operator&(FileMode lhs, FileMode rhs);

	class File
	{
	public:
		File(const std::wstring& path, FileMode mode);
		File() = default;

		FileMode GetMode() const { return m_Mode; }
		std::wstring GetPath() const { return m_Path; }
		std::optional<WCHAR*> Read();
		bool Write(WCHAR* what, size_t size);
		bool Write(const std::wstring& what);
		void Open(const std::wstring& path, FileMode mode);
		void Close();

	private:

		std::wstring m_Path;
		HANDLE m_Handle = nullptr;
		FileMode m_Mode = FileMode::None;
	};

}