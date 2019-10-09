
#include "separating_axes.h"

namespace ae { namespace math {

	inline bool BoxCheckTriangle(const float3& localStart, const float3& localEnd, const float3& extent, const float4x4& worldMatrix, 
		const float3& p0,const float3& p1,const float3& p2, S_CollisionResult& rs)
	{

		float HitTime = 1.f;
		float3 HitNormal(0.f,0.f,0.f);
		float3 localBoxX = worldMatrix * float3(1.f,0.f,0.f);
		float3 localBoxY = worldMatrix * float3(0.f,1.f,0.f);
		float3 localBoxZ = worldMatrix * float3(0.f,0.f,1.f);
		// Now check for an intersection using the Separating Axis Theorem
		if(FindSeparatingAxis(p0, p1, p2, localStart, localEnd, extent, localBoxX, localBoxY, localBoxZ, HitTime, HitNormal))
		{
			if (HitTime < rs.time)
			{
				// Store the better time
				rs.time = HitTime;
				// Get the material that was hit
				// Normal will get transformed to world space at end of check
				rs.normal = HitNormal;

				return true;
			}
		}
		return false;
	}

} } // namespace ae { namespace math {
