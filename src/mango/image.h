#pragma once

// Mango
#include <mango/util.h>
#include <mango/errors.h>

// STB
#include <stb/stb_image.h>

// STL
#include <string>

namespace mango
{

	class Image
	{
	public:
		Image(const std::string& path);
		Image() = default;
		~Image();

		unsigned char* GetData() const { return m_Data; }
		Vec2i GetSize() const { return m_Size; }

	private:
		std::string m_Path;
		unsigned char* m_Data = nullptr;
		std::int32_t m_Channels = 0;
		Vec2i m_Size = { 0, 0 };
	};

}