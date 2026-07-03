#include "errors.h"

namespace mango
{

	DWORD GetError(const std::wstring& info)
	{
		DWORD error = GetLastError();

		if (error)
		{
			LPWSTR message = 0;
			DWORD size = FormatMessageW(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				0,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				reinterpret_cast<LPWSTR>(&message),
				0,
				nullptr
			);

			LPCWSTR caption = info.c_str();
			MessageBoxW(nullptr, message, caption, IDOK);
		}

		return error;
	}

}