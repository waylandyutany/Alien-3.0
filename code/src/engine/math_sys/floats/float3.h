#pragma once

#ifndef AE_MATH_SYS_FLOAT3_H
#define AE_MATH_SYS_FLOAT3_H

#include "float4.h"

namespace ae { namespace math {

	template< typename T >
	struct vector3
	{
		vector3() {}
		vector3(T _x,T _y,T _z) : x(_x), y(_y), z(_z) {}

		vector3(const D3DXVECTOR3& v) : x(v.x), y(v.y), z(v.z) {}

		template<typename  U> vector3(const vector3<U>& v3) : x(v3.x), y(v3.y), z(v3.z) {}
		template<typename  U> vector3(const vector4<U>& v4) : x(v4.x), y(v4.y), z(v4.z) {}

		vector3& operator += (const vector3& a)	{ x += a.x; y += a.y; z += a.z; return *this; }
		vector3& operator -= (const vector3& a)	{ x -= a.x; y -= a.y; z -= a.z; return *this; }
		vector3& operator *= (const vector3& a)	{ x *= a.x; y *= a.y; z *= a.z; return *this; }
		vector3& operator /= (const vector3& a)	{ x /= a.x; y /= a.y; z /= a.z; return *this; }

		vector3& operator += (float f)	{ x += f; y += f; z += f; return *this; }
		vector3& operator -= (float f)	{ x -= f; y -= f; z -= f; return *this; }
		vector3& operator *= (float f)	{ x *= f; y *= f; z *= f; return *this; }
		vector3& operator /= (float f)	{ x /= f; y /= f; z /= f; return *this; }

		friend vector3 operator - (const vector3& a, const vector3& b)	{ return vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
		friend vector3 operator + (const vector3& a, const vector3& b)	{ return vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
		friend vector3 operator / (const vector3& a, const vector3& b)	{ return vector3(a.x / b.x, a.y / b.y, a.z / b.z); }
		friend vector3 operator * (const vector3& a, const vector3& b)	{ return vector3(a.x * b.x, a.y * b.y, a.z * b.z); }

		friend vector3 operator - (const vector3& a, float f)	{ return vector3(a.x - f, a.y - f, a.z - f); }
		friend vector3 operator + (const vector3& a, float f)	{ return vector3(a.x + f, a.y + f, a.z + f); }
		friend vector3 operator / (const vector3& a, float f)	{ return vector3(a.x / f, a.y / f, a.z / f); }
		friend vector3 operator * (const vector3& a, float f)	{ return vector3(a.x * f, a.y * f, a.z * f); }

		friend vector3 operator - (const vector3& a) { return vector3(-a.x, -a.y, -a.z); }

		friend bool	operator <	(const vector3& a, const vector3& b) { return a.x <	 b.x && a.y <  b.y && a.z <  b.z; }
		friend bool	operator <= (const vector3& a, const vector3& b) { return a.x <= b.x && a.y <= b.y && a.z <= b.z; }
		friend bool	operator >	(const vector3& a, const vector3& b) { return a.x >  b.x && a.y >  b.y && a.z >  b.z; }
		friend bool	operator >= (const vector3& a, const vector3& b) { return a.x >= b.x && a.y >= b.y && a.z >= b.z; }

		friend bool operator == (const vector3& a, const vector3& b) { return (a.x == b.x) && (a.y == b.y) && (a.z == b.z); }
		friend bool operator != (const vector3& a, const vector3& b) { return (a.x != b.x) || (a.y != b.y) || (a.z != b.z); }

		friend vector3 Min(const vector3& a, const vector3& b) { return vector3(Min<T>(a.x,b.x), Min<T>(a.y,b.y), Min<T>(a.z,b.z)); }
		friend vector3 Max(const vector3& a, const vector3& b) { return vector3(Max<T>(a.x,b.x), Max<T>(a.y,b.y), Max<T>(a.z,b.z));	}

		operator const T *() const { return reinterpret_cast<const T*>(this); }

		T size() const { return static_cast<T>(sqrtf(x*x + y*y + z*z)); }
		T sizeSquared() const { return static_cast<T>(x*x + y*y + z*z); }

		vector3 normalize() const {
			const T s(size());
			if(s) return vector3(x/s, y/s, z/s);
			return vector3(0,0,0);
		}

		friend vector3 cross(const vector3& a, const vector3& b ) {
			return vector3( (a.y * b.z - a.z * b.y), (a.z * b.x - a.x * b.z), (a.x * b.y - a.y * b.x) );
		}

		friend float dot(const vector3& a, const vector3& b ) {
			return (a.x*b.x + a.y*b.y + a.z*b.z);
		}

		vector3 cross(const vector3& b ) const{
			return vector3( (y * b.z - z * b.y), (z * b.x - x * b.z), (x * b.y - y * b.x) );
		}

		float dot(const vector3& b ) const {
			return (x*b.x + y*b.y + z*b.z);
		}
		T x,y,z;
	};

	typedef vector3<float> float3;
//	typedef vector3<ae::u32> u32_3;

} } // namespace ae { namespace math {

#include "float3.inl"

#endif // #ifdef AE_MATH_SYS_FLOAT3_H
