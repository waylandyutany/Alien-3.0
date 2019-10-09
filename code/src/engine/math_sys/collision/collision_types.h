#pragma once

#ifndef AE_MATH_SYS_COLLISION_TYPES_H
#define AE_MATH_SYS_COLLISION_TYPES_H

namespace ae { namespace math {

struct S_CollisionResult
{
	float time;
	float3 normal;
};

struct S_Line
{
	S_Line(const float3& _start, const float3& _end) : start(_start), end(_end) {}

	float3 start;
	float3 end;
};

struct S_Ellipsoid
{
	S_Ellipsoid(const float3& _center, const float3& _radius) : center(_center), radius(_radius) {}

	float3 center;
	float3 radius;
};

struct S_Triangle
{
	S_Triangle(const float3& _p0, const float3& _p1, const float3& _p2) : p0(_p0), p1(_p1), p2(_p2) {}
	S_Triangle() {};

	float3 p0, p1, p2;
};

} } // namespace ae { namespace math {

#include "collision_types.inl"

#endif // #ifdef AE_MATH_SYS_COLLISION_TYPES_H
