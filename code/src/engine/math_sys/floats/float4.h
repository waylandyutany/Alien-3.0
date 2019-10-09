#pragma once

#ifndef AE_MATH_SYS_FLOAT4_H
#define AE_MATH_SYS_FLOAT4_H

namespace ae { namespace math {

	template< typename T > struct vector3;

	template< typename T >
	struct vector4
	{
		vector4() {}
		vector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

		template<typename U> vector4(const vector3<U>& v3, float w);

		vector4 operator - (const vector4& v)	{ return vector4(x - v.x, y - v.y, z - v.z, z - v.z); }
		vector4 operator + (const vector4& v)	{ return vector4(x + v.x, y + v.y, z + v.z, z + v.z); }
		vector4 operator / (const vector4& v)	{ return vector4(x / v.x, y / v.y, z / v.z, z / v.z); }
		vector4 operator * (const vector4& v)	{ return vector4(x * v.x, y * v.y, z * v.z, z * v.z); }
		operator const T *() const { return reinterpret_cast<const T*>(this); }

		T x, y, z, w;
	};

	typedef vector4<float> float4;

} } // namespace ae { namespace math {

#include "float4.inl"

#endif // #ifdef AE_MATH_SYS_FLOAT4_H
