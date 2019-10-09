#pragma once

#ifndef AE_MATH_SYS_TYPES_H
#define AE_MATH_SYS_TYPES_H

#include "floats/float2.h"
#include "floats/float3.h"
#include "floats/float4.h"
#include "floats/float2x2.h"
#include "floats/float3x3.h"
#include "floats/float4x4.h"
#include "floats/float_constants.h"

namespace ae { namespace math {
// *****************************************************************************************************
enum E_TransformationElement
{
	E_TE_PosX,
	E_TE_PosY,
	E_TE_PosZ,
	E_TE_ScaleX,
	E_TE_ScaleY,
	E_TE_ScaleZ,
	E_TE_RotX,
	E_TE_RotY,
	E_TE_RotZ
};
// *****************************************************************************************************
float Deg2Radians(float deg);
float Radians2Deg(float radians);
// *****************************************************************************************************
template<typename T> T Lerp(const T& p0, const T& p1, float blend) {
	AE_ASSERT(0.0f <= blend && blend <= 1.0f && "Wrong blend factor !");
	return (p0*(1.0f - blend)) + (p1*(blend));
}


template<typename T> T Min (T a, T b) {
	return a < b ? a : b;
}

template<typename T> T Max (T a, T b) {
	return a < b ? b : a;
}

// *****************************************************************************************************
// *****************************************************************************************************

// *****************************************************************************************************
// *****************************************************************************************************

template<typename T>
T move(const T& from, const T& to, T speed, T deltaTime)
{
	if(from == to) return from;

	T ret(from);
	if(from < to) {
		ret += speed * deltaTime;
		if(ret > to) ret = to;
	}
	else if(from > to) {
		ret -= speed * deltaTime;
		if(ret < to) ret = to;
	}

	return ret;
}

// *********************************************************************************************************
// *****************************************************************************************************
template<typename T>
struct box3d_t
{
	vector3<T> min;
	vector3<T> max;
	bool IsPointInside(const vector3<T>& pos)	{ return min <= pos && pos <= max; }

};

typedef box3d_t<float> box3d;
typedef box3d_t<ae::u32> box3d_u32;
// *****************************************************************************************************
template<typename T>
struct box2d_t
{
	vector2<T> min;
	vector2<T> max;
};

typedef box2d_t<float> box2d;
typedef box2d_t<ae::u32> box2d_u32;
// *****************************************************************************************************

} } // namespace ae { namespace math {

#include "types.inl"

#endif // #ifdef AE_MATH_SYS_TYPES_H
