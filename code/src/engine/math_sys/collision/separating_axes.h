#pragma once

#ifndef AE_MATH_SYS_SEPARATING_AXES_H
#define AE_MATH_SYS_SEPARATING_AXES_H

#include "../types.h"
#include "../collision/collision_types.h"

namespace ae { namespace math {

bool TestSeparatingAxis(const float3& p0,	const float3& p1, const float3& p2,	const float3& line,
								float ProjectedStart, float ProjectedEnd, float ProjectedExtent,
								float& MinIntersectTime, float& MaxIntersectTime, float3& HitNormal	);

bool TestSeparatingAxis( const float3& p0, const float3& p1, const float3& p2, const float3& TriangleEdge,
						 const float3& BoxEdge,	 const float3& Start, const float3& End,
						 const float3& BoxX, const float3& BoxY, const float3& BoxZ, const float3& BoxExtent,
						 float& MinIntersectTime, float& MaxIntersectTime, float3& HitNormal );

bool FindSeparatingAxis( const float3& p0, const float3& p1,	const float3& p2,
						const float3& Start, const float3& End,
						const float3& BoxExtent, const float3& BoxX, const float3& BoxY, const float3& BoxZ,
						float& HitTime,	float3& OutHitNormal );

} } // namespace ae { namespace math {

#include "separating_axes.inl"

#endif // #ifdef AE_MATH_SYS_SEPARATING_AXES_H
