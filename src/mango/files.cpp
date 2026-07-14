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

		// #Note: For a flag type of variable use an if statement.
		switch (mode)
		{
			case FileMode::Read:
			{
				access = GENERIC_READ;
				disposition = OPEN_EXISTING;
			} break;

			case FileMode::Write:
			{
				access = GENERIC_WRITE;
				disposition = CREATE_ALWAYS;
			} break;
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

}