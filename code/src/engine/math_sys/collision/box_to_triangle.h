#pragma once

#ifndef AE_MATH_SYS_COLLISION_BOX_TO_TRIANGLE_H
#define AE_MATH_SYS_COLLISION_BOX_TO_TRIANGLE_H

#include "../types.h"

namespace ae { namespace math {

bool BoxCheckTriangle(const float3& localStart, const float3& localEnd, const float3& extent, const float4x4& worldMatrix, 
	const float3& p0,const float3& p1,const float3& p2, S_CollisionResult& rs)

} } // namespace ae { namespace math {

#include "box_to_triangle.inl"

#endif // #ifdef AE_MATH_SYS_COLLISION_BOX_TO_TRIANGLE_H
