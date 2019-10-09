#pragma once

#ifndef AE_MATH_SYS_COLLISION_H
#define AE_MATH_SYS_COLLISION_H

#include "separating_axes.h"

#include "collision_types.h"

namespace ae { namespace math {

// Intersection detected when return value ranges between <0 - 1>
float LinePlaneIntersection(const float4& plane, const float3& lineStart, const float3& lineEnd );

// return plane constructed by triangle with given points
float4 TriangleToPlane(const float3& p0, const float3& p1, const float3& p2);

// Return true if given line segment intersect given box
float BoxLineIntersectionFast(const box3d& box, const float3& lineStart, const float3& lineEnd);
bool BoxLineIntersection(const box3d& box, const float3& lineStart, const float3& lineEnd);

// return true if collision exists between given line and triangle, result contains collision time and normal
bool LineToTriangleIntersection(const S_Line& line, const S_Triangle& triangle, S_CollisionResult& result);

void FindClosestPointsBetweenLines(	const ae::math::float3& lineStart0, const ae::math::float3& lineEnd0, 
	const ae::math::float3& lineStart1, const ae::math::float3& lineEnd1,
	ae::math::float3& closestPt0, 
	ae::math::float3& closestPt1);

} } // namespace ae { namespace math {

#include "collision.inl"

#endif // #ifdef AE_MATH_SYS_COLLISION_H
