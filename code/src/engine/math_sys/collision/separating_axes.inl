namespace ae { namespace math {

inline bool TestSeparatingAxis(const float3& p0,	const float3& p1, const float3& p2,	const float3& line,
								float ProjectedStart, float ProjectedEnd, float ProjectedExtent,
								float& MinIntersectTime, float& MaxIntersectTime, float3& HitNormal	)
{
	float	ProjectedDirection = ProjectedEnd - ProjectedStart,
			ProjectedV0 = dot(line , p0),
			ProjectedV1 = dot(line , p1),
			ProjectedV2 = dot(line , p2),
			TriangleMin = Min(ProjectedV0,Min(ProjectedV1,ProjectedV2)) - ProjectedExtent,
			TriangleMax = Max(ProjectedV0,Max(ProjectedV1,ProjectedV2)) + ProjectedExtent;

	// If zero - check vector is perp to test axis, so just check if we start outside. If so, we can't collide.
	if( fabs(ProjectedDirection) < small_float )
	{
		if( ProjectedStart < TriangleMin ||	ProjectedStart > TriangleMax )	return false;
		else return true;
	}
	float OneOverProjectedDirection = 1.f / ProjectedDirection;
	// Moving in a positive direction - enter ConvexMin and exit ConvexMax
	float EntryTime, ExitTime;
	float3 ImpactNormal;
	if(ProjectedDirection > 0.f)
	{
		EntryTime = (TriangleMin - ProjectedStart) * OneOverProjectedDirection;
		ExitTime = (TriangleMax - ProjectedStart) * OneOverProjectedDirection;
		ImpactNormal = -line;
	}
	// Moving in a negative direction - enter ConvexMax and exit ConvexMin
	else
	{
		EntryTime = (TriangleMax - ProjectedStart) * OneOverProjectedDirection;
		ExitTime = (TriangleMin - ProjectedStart) * OneOverProjectedDirection;
		ImpactNormal = line;
	}	
	// See if entry time is further than current furthest entry time. If so, update and remember normal
	if(EntryTime > MinIntersectTime)
	{
		MinIntersectTime = EntryTime;
		HitNormal = ImpactNormal;
	}
	// See if exit time is close than current closest exit time.
	MaxIntersectTime = Min(MaxIntersectTime, ExitTime);
	// If exit is ever before entry - we don't intersect
	if( MaxIntersectTime < MinIntersectTime ) return false;
	// If exit is ever before start of line check - we don't intersect
	if( MaxIntersectTime < 0.f ) return false;
	return true;
}

inline bool TestSeparatingAxis( const float3& p0, const float3& p1, const float3& p2, const float3& TriangleEdge,
						 const float3& BoxEdge,	 const float3& Start, const float3& End,
						 const float3& BoxX, const float3& BoxY, const float3& BoxZ, const float3& BoxExtent,
						 float& MinIntersectTime, float& MaxIntersectTime, float3& HitNormal )
{
	// Separating axis is cross product of box and triangle edges.
	float3	Line = cross(BoxEdge , TriangleEdge);
	// Check separating axis is non-zero. If it is, just don't use this axis.
	if(Line.sizeSquared() < small_float) return true;
	// Calculate extent of box projected onto separating axis.
	float ProjectedExtent = BoxExtent.x * fabs(dot(Line,BoxX)) + BoxExtent.y * fabs(dot(Line, BoxY)) + BoxExtent.z * fabs(dot(Line, BoxZ));
	return TestSeparatingAxis(p0,p1,p2,Line,dot(Line, Start),dot(Line, End),ProjectedExtent,MinIntersectTime,MaxIntersectTime,HitNormal);
}

inline bool FindSeparatingAxis( const float3& p0, const float3& p1,	const float3& p2,
						const float3& Start, const float3& End,
						const float3& BoxExtent, const float3& BoxX, const float3& BoxY, const float3& BoxZ,
						float& HitTime,	float3& OutHitNormal )
{
	float	MinIntersectTime = -max_float,
			MaxIntersectTime = max_float;

	// Calculate normalized edge directions. We normalize here to minimize precision issues when doing cross products later.
	const float3& EdgeDir0 = (p1 - p0).normalize();
	const float3& EdgeDir1 = (p2 - p1).normalize();
	const float3& EdgeDir2 = (p0 - p2).normalize();
	// Used to set the hit normal locally and apply the best normal only upon a full hit
	float3 HitNormal;
	// Box faces. We need to calculate this by crossing edges because BoxX etc are the _edge_ directions - not the faces.
	// The box may be sheared due to non-uniform scaling and rotation so FaceX normal != BoxX edge direction
	if(!TestSeparatingAxis(p0,p1,p2,BoxX,BoxY,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	if(!TestSeparatingAxis(p0,p1,p2,BoxY,BoxZ,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	if(!TestSeparatingAxis(p0,p1,p2,BoxZ,BoxX,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	// Triangle normal.
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir1,EdgeDir0,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal))	return false;
	// Box X edge x triangle edges.
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir0,BoxX,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir1,BoxX,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir2,BoxX,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	// Box Y edge x triangle edges.
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir0,BoxY,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir1,BoxY,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir2,BoxY,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	// Box Z edge x triangle edges.
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir0,BoxZ,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir1,BoxZ,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	if(!TestSeparatingAxis(p0,p1,p2,EdgeDir2,BoxZ,Start,End,BoxX,BoxY,BoxZ,BoxExtent,MinIntersectTime,MaxIntersectTime,HitNormal)) return false;
	// If hit is beyond end of ray, no collision occurs.
	if(MinIntersectTime > HitTime) return false;
	// If hit time is positive- we start outside and hit the hull
	if(MinIntersectTime >= 0.0f)
	{
		HitTime = MinIntersectTime;
		OutHitNormal = HitNormal;
	}
	// If hit time is negative- the entry is behind the start - so we started colliding
	else
	{
		HitTime = 0.f;
		OutHitNormal = -(End - Start).normalize();
	}

	return true;
}

} } // namespace ae { namespace math {
