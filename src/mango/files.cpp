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
			return std::nullopt;

		// Important!! This only works with UTF-16 encoding.
		// At least with UTF-16 LE.

		LARGE_INTEGER size;
		GetFileSizeEx(m_Handle, &size);

		std::size_t numberOfCharacters = size.QuadPart / 2;
		WCHAR* buffer = new WCHAR[numberOfCharacters + 1];

		DWORD bytesRead = 0;

		BOOL read = ReadFile(
			m_Handle,
			buffer,
			size.QuadPart,
			&bytesRead,
			nullptr
		);
		GetError(L"File.ReadFile");

		buffer[numberOfCharacters + 1] = L'\0';

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

	Directory::Directory(const std::wstring& path)
	{
		Open(path);
	}

	void Directory::Open(const std::wstring& path)
	{
		m_Path = path;

		WIN32_FIND_DATAW fileData;
		HANDLE h = FindFirstFileW(path.c_str(), &fileData);
		GetError(L"Directory.FindFirstFileW");

		if (h != INVALID_HANDLE_VALUE)
			do
			{
				if (wcscmp(fileData.cFileName, L".") != 0 && wcscmp(fileData.cFileName, L"..") != 0)
					m_Files.push_back(FileData(h, fileData));
			} while (FindNextFileW(h, &fileData) != 0);
	}

	void Directory::Close()
	{
		m_Path = L"";

		for (auto& k : m_Files)
			CloseHandle(k.m_Handle);

		m_Files.clear();
	}

	FileData::FileData(HANDLE h, WIN32_FIND_DATAW data)
		: m_Handle(h), m_Data(data)
	{}

	INIData::INIData(const INIDataContainer& container)
		: Fields(container)
	{}

	void INIData::AddField(const std::wstring& section, const std::wstring& field, const std::wstring& value)
	{
		for (auto& k : Fields)
		{
			if (k.first == section)
			{
				k.second.push_back(std::pair<std::wstring, std::wstring>(field, value));

				return;
			}
		}

		Fields.push_back(std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>(section, { std::pair<std::wstring, std::wstring>(field, value) }));
	}

	void INIData::RemoveField(const std::wstring& section, const std::wstring& field)
	{
		for (auto& k : Fields)
		{
			if (k.first == section)
			{
				for (std::vector<std::pair<std::wstring, std::wstring>>::iterator it = k.second.begin(); it != k.second.end(); it++)
				{
					if (it->first == field)
					{
						k.second.erase(it);

						return;
					}
				}
			}
		}
	}

	std::vector<std::wstring> INIData::Translate() const
	{
		std::vector<std::wstring> translation = { };

		for (auto& k : Fields)
		{
			translation.push_back(L"[" + k.first + L"]");

			for (auto& l : k.second)
				translation.push_back(l.first + L": " + l.second);
		}

		return translation;
	}

	std::wstring INIData::Serialize() const
	{
		std::vector<std::wstring>& translation = Translate();
		std::wstring str;

		for (auto& k : translation)
			str += k + L"\n";

		return str;
	}

	void INIData::PushToFile(File* file)
	{
		if (file->IsOpen() && (file->GetMode() & FileMode::Write) != 0)
			file->Write(Serialize());
	}

	void INIData::Clear()
	{
		Fields.clear();
	}

	bool INIData::ConstructFromFile(File* file)
	{
		if (file->IsOpen() && (file->GetMode() & FileMode::Read) != 0)
		{
			std::optional<WCHAR*> read = file->Read();

			if (read)
			{
				std::vector<std::wstring> lines = { };

				std::size_t len = 0;
				for (WCHAR* wc = read.value(); *wc != L'\0'; wc++)
				{
					len++;

					bool nextIsZero = *static_cast<WCHAR*>(wc + 1) == L'\0';
					if (*wc == L'\n' || nextIsZero)
					{
						std::wstring line(static_cast<WCHAR*>(wc - len + 1), nextIsZero ? len - 1 : len);
						len = 0;

						lines.push_back(line);
					}
				}

				//delete[] read.value();

				for (auto& k : lines)
				{
					if (k[0] == L'[') // This is a section
					{
						std::wstring name = k.substr(1, k.length() - 4);
						Fields.push_back(std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>(name, { }));
					}
					else
					{
						// Ignore the white spaces, then find the pos of '='.
						std::size_t equalSignPos = k.find(L'=', 0);

						// Could be a comment or an error.
						if (equalSignPos == std::wstring::npos)
						{
							continue;
						}

						std::wstring field, value;

						// First let's go with param name
						for (std::int32_t i = equalSignPos - 1; i >= 0; i--)
						{
							if (k[i] != L' ')
							{
								field = k.substr(0, i + 1);
								break;
							}
						}
						// Second, let's go with value
						for (std::int32_t i = equalSignPos + 1; i < k.length(); i++)
						{
							if (k[i] != L' ')
							{
								value = k.substr(i, k.length() - i - 2);
								break;
							}
						}

						Fields[Fields.size() - 1].second.push_back(std::pair<std::wstring, std::wstring>(field, value));
					}
				}
			}

			return true;
		}

		return false;
	}

}