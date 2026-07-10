#include "image.h"

namespace mango
{

	Image::Image(const std::string& path)
		: m_Path(path)
	{
		m_Data = stbi_load(
			path.c_str(),
			&m_Size.X,
			&m_Size.Y,
			&m_Channels,
			4
		);

		if (!m_Data)
			ForceError(L"Image.stbi_load");

		for (std::int32_t i = 0; i < m_Size.X * m_Size.Y; i++)
		{
			unsigned char* p = m_Data + i * 4;

			unsigned char r = p[0];
			unsigned char g = p[1];
			unsigned char b = p[2];
			unsigned char a = p[3];

			p[0] = (b * a) / 255;
			p[1] = (g * a) / 255;
			p[2] = (r * a) / 255;
			p[3] = a;
		}
	}

	Image::~Image()
	{
		stbi_image_free(m_Data);
	}

}
