/* ================================================================
*
*	Contains 2D vector.
*
*	#Authors: The Kumor
*
* ================================================================ */

#pragma once

// STL
#include <cstdint>

namespace mango
{

	template <typename T>
	struct Vec2
	{
		Vec2(T x, T y)
			: X(x), Y(y)
		{}

		Vec2(const Vec2& other)
			: X(other.x), Y(other.y)
		{}

		union
		{
			T x;
			T X;
		};

		union
		{
			T y;
			T Y;
		};

		Vec2 operator=(const Vec2& other)
		{
			return Vec2(other.x, other.y);
		}

		bool operator==(const Vec2& other)
		{
			return X == other.x && Y == other.y;
		}

		Vec2 operator+(const Vec2& other)
		{
			return Vec2(X + other.x, Y + other.y);
		}

		Vec2 operator-(const Vec2& other)
		{
			return Vec2(X - other.x, Y - other.y);
		}

		Vec2 operator*(const Vec2& other)
		{
			return Vec2(X * other.x, Y * other.y);
		}

		Vec2 operator/(const Vec2& other)
		{
			return Vec2(X / other.x, Y / other.y);
		}
	};

	typedef Vec2<std::int32_t> Vec2i;
	typedef Vec2<std::uint32_t> Vec2u;
	typedef Vec2<float> Vec2f;

}