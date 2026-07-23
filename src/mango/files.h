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
#include <vector>

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

	class BaseFile
	{
	public:
		BaseFile() = default;

	protected:
		std::wstring m_Path;
	};

	class File : public BaseFile
	{
	public:
		File(const std::wstring& path, FileMode mode);
		File() = default;

		FileMode GetMode() const { return m_Mode; }
		std::wstring GetPath() const { return m_Path; }
		std::optional<WCHAR*> Read();
		bool Write(WCHAR* what, size_t size);
		bool Write(const std::wstring& what);
		bool IsOpen() const { return m_Handle != nullptr; }
		void Open(const std::wstring& path, FileMode mode);
		void Close();

	private:
		HANDLE m_Handle = nullptr;
		FileMode m_Mode = FileMode::None;
	};

	struct FileData
	{
		FileData(HANDLE h, WIN32_FIND_DATAW data);
		FileData() = default;

		friend class Directory;

		const std::wstring GetName() const { return std::wstring(m_Data.cFileName); }

	private:
		HANDLE m_Handle;
		WIN32_FIND_DATAW m_Data;
	};

	class Directory : public BaseFile
	{
	public:
		Directory(const std::wstring& path);
		Directory() = default;

		const std::vector<FileData> GetFiles() const { return m_Files; }
		void Open(const std::wstring& path);
		void Close();

	private:
		std::vector<FileData> m_Files = {};
	};

	using INIDataContainer = std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>>;

	struct INIData
	{
		INIData(const INIDataContainer& container);
		INIData() = default;

		bool ConstructFromFile(File* file);
		void AddField(const std::wstring& section, const std::wstring& field, const std::wstring& value);
		void RemoveField(const std::wstring& section, const std::wstring& field);
		std::vector<std::wstring> Translate() const;
		std::wstring Serialize() const;
		void PushToFile(File* file);
		void Clear();

		INIDataContainer Fields = {};
	};

}