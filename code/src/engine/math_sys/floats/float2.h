#pragma once

#ifndef AE_MATH_SYS_FLOAT2_H
#define AE_MATH_SYS_FLOAT2_H

#include "float3.h"
#include "float4.h"

namespace ae { namespace math {

	template< typename T >
	struct vector2
	{
		vector2() {}
		vector2(T _x,T _y) : x(_x), y(_y) {}

		template<typename  U> vector2(const vector4<U>& v4) : x(static_cast<T>(v4.x)), y(static_cast<T>(v3.y)) {}
		template<typename  U> vector2(const vector3<U>& v3) : x(static_cast<T>(v3.x)), y(static_cast<T>(v4.y)) {}
		template<typename  U> vector2(const vector2<U>& v2) : x(static_cast<T>(v2.x)), y(static_cast<T>(v2.y)) {}

		vector2 operator - (const vector2& v)	{ return vector3(x - v.x, y - v.y);	}
		vector2 operator + (const vector2& v)	{ return vector3(x + v.x, y + v.y);	}
		vector2 operator / (const vector2& v)	{ return vector3(x / v.x, y / v.y);	}
		vector2 operator * (const vector2& v)	{ return vector3(x * v.x, y * v.y);	}

		T x,y;
	};

	typedef vector2<float> float2;
	typedef vector2<ae::u32> float2_u32;

} } // namespace ae { namespace math {

#include "float2.inl"

#endif // #ifdef AE_MATH_SYS_FLOAT2_H
