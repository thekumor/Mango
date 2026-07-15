#include "files.h"

namespace mango
{

	File::File(const std::wstring& path, FileMode mode)
	{
		Open(path, mode);
	}

	void File::Open(const std::wstring& path, FileMode mode)
	{
		m_Path = path;
		m_Mode = mode;

		DWORD access = 0;
		DWORD disposition = 0;


		if (mode & FileMode::Read)
		{
			access = GENERIC_READ;
			disposition = OPEN_EXISTING;
		}

		if (mode & FileMode::Write)
		{
			access = GENERIC_WRITE;
			disposition = CREATE_NEW;
		}

		if (mode & FileMode::Append)
		{
			disposition = OPEN_EXISTING;
		}

		m_Handle = CreateFileW(
			path.c_str(),
			access,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr, // ?
			disposition,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);
		GetError(L"File.CreateFileW");
	}

	void File::Close()
	{
		CloseHandle(m_Handle);
		m_Path = L"";
		m_Mode = FileMode::None;
	}

	std::optional<WCHAR*> File::Read()
	{
		if (!m_Handle)
			return {};

		// Important!! This only works with UTF-16 encoding.
		// At least with UTF-16 LE.

		DWORD size = GetFileSize(m_Handle, nullptr);
		WCHAR* buffer = new WCHAR[size + 1];

		DWORD bytesRead = 0;

		BOOL read = ReadFile(
			m_Handle,
			buffer,
			size,
			&bytesRead,
			nullptr
		);
		GetError(L"File.ReadFile");

		// delete[] buffer;
		return std::make_optional<WCHAR*>(buffer);
	}

	bool File::Write(WCHAR* what, size_t size)
	{
		if (!m_Handle)
			return false;

		DWORD bytesWritten = 0;

		BOOL write = WriteFile(
			m_Handle,
			what,
			size,
			&bytesWritten,
			nullptr
		);
		GetError(L"File.WriteFile");

		return write == 1 ? true : false; // Weird conversion from C-style BOOL to C++ bool.
	}

	bool File::Write(const std::wstring& what)
	{
		WCHAR* buffer = new WCHAR[what.size()];
		DWORD bytes = what.size() * sizeof(WCHAR);

		std::memcpy(buffer, what.data(), bytes);
		bool success = Write(buffer, bytes);
		delete[] buffer;

		return success;
	}

	FileMode operator|(FileMode lhs, FileMode rhs)
	{
		return static_cast<FileMode>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
	}

	bool operator&(FileMode lhs, FileMode rhs)
	{
		return static_cast<bool>(static_cast<std::uint32_t>(lhs) & static_cast<std::uint32_t>(rhs));
	}

}