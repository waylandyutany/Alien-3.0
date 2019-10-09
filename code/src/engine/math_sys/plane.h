#pragma once

#ifndef AE_MATH_SYS_PLANE_H
#define AE_MATH_SYS_PLANE_H

namespace ae { namespace math {

	typedef float4 plane;
	
	plane CreatePlane(const float3& planePoint, const float3& planeNormal);
	float PlaneDistance(const float3& point, const plane& pln);

} } // namespace ae { namespace math {

#include "plane.inl"

#endif // #ifdef AE_MATH_SYS_PLANE_H
