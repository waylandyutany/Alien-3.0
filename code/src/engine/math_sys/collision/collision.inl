namespace ae { namespace math {

inline float LinePlaneIntersection(const float4& p, const float3& l0, const float3& l1 )
{
	return	( p.x*l0.x + p.y*l0.y + p.z*l0.z + p.w) /
			((p.x*(l0.x - l1.x)) + (p.y*(l0.y - l1.y)) + (p.z*(l0.z - l1.z)));
}

inline float4 TriangleToPlane(const float3& p0, const float3& p1, const float3& p2)
{
	ae::math::float3 a(p1 - p0);
	ae::math::float3 b(p2 - p0);
	ae::math::float3 c(cross(a,b));
	ae::math::float3 n(c.normalize());
	return float4(n.x, n.y, n.z, -(n.x*p0.x + n.y*p0.y + n.z*p0.z));
}

inline bool BoxLineIntersection(const box3d& box, const float3& lineStart, const float3& lineEnd)
{
	float t = BoxLineIntersectionFast(box, lineStart, lineEnd);
	if( 0.0f <= t && t <= 1.0f) return true;
	return false;
}

inline float BoxLineIntersectionFast(const box3d& box, const float3& lineStart, const float3& lineEnd)
{
	const float3 dir = (lineEnd - lineStart);
	float3	Time;
	bool	Inside = 1;
	float   faceDir[3] = {1, 1, 1};
	float	t;
	float3  normal;
// ************************************************************
	if(lineStart.x < box.min.x)
		if(dir.x <= 0.0f) return 0;
		else {
			Inside = 0;
			faceDir[0] = -1;
			Time.x = (box.min.x - lineStart.x) / dir.x;
		}
	else if(lineStart.x > box.max.x)
		if(dir.x >= 0.0f) return 0;
		else {
			Inside = 0;
			Time.x = (box.max.x - lineStart.x) / dir.x;
		}
	else Time.x = 0.0f;
// ************************************************************
	if(lineStart.y < box.min.y)
		if(dir.y <= 0.0f) return 0;
		else {
			Inside = 0;
			faceDir[1] = -1;
			Time.y = (box.min.y - lineStart.y) / dir.y;
		}
	else if(lineStart.y > box.max.y)
		if(dir.y >= 0.0f) return 0;
		else {
			Inside = 0;
			Time.y = (box.max.y - lineStart.y) / dir.y;
		}
	else Time.y = 0.0f;

// ************************************************************
	if(lineStart.z < box.min.z)
		if(dir.z <= 0.0f)
			return 0;
		else {
			Inside = 0;
			faceDir[2] = -1;
			Time.z = (box.min.z - lineStart.z) / dir.z;
		}
	else if(lineStart.z > box.max.z)
		if(dir.z >= 0.0f)
			return 0;
		else {
			Inside = 0;
			Time.z = (box.max.z - lineStart.z) / dir.z;
		}
	else Time.z = 0.0f;

// ************************************************************
	if(Inside) {
		normal = float3(0, 0, 1);
		return 0.0f;
	}
	if(Time.y > Time.z) {
		t = Time.y;
		normal = float3(0, faceDir[1], 0);
	}
	else {
		t = Time.z;
		normal = float3(0, 0, faceDir[2]);
	}
	if(Time.x > t) {
		t = Time.x;
		normal = float3(faceDir[0], 0, 0);
	}

	if(t >= 0.0f && t <= 1.0f)	{
		float3 hitLocation = lineStart + dir * t;
		const float treshHold = 0.1f;
		if(	hitLocation.x > box.min.x - treshHold && hitLocation.x < box.max.x + treshHold &&
			hitLocation.y > box.min.y - treshHold && hitLocation.y < box.max.y + treshHold &&
			hitLocation.z > box.min.z - treshHold && hitLocation.z < box.max.z + treshHold)
		{				
			return t;
		}
	}

	return -1.0f;
}

// *****************************************************************************************************
// *****************************************************************************************************

inline bool LineToTriangleIntersection(const S_Line& line, const S_Triangle& triangle, S_CollisionResult& result)
{
	const float3 dir = line.end - line.start;
	const float3 e0 = triangle.p2 - triangle.p0;
	const float3 e1 = triangle.p1 - triangle.p0;
	const float3 p = cross(dir, e1);
	const float d = dot(e0, p);
	if(d < 0.00001f) return false;

	const float3 t = line.start - triangle.p0;
	const float	u = dot(t, p);
	if(u < 0.0f || u > d) return false;

	const float3 q = cross(t, e0);
	const float	v = dot(dir, q);
	if(v < 0.0f || u + v > d)return false;

	const float time = dot(e1, q) / d;
	if(time < 0.0f || time > 1.0f) return false;

	result.normal = cross((triangle.p2 - triangle.p1),(triangle.p1 - triangle.p0)).normalize();
	result.time = dot(triangle.p0 - line.start, result.normal) / dot(result.normal, dir);

	return true;
}

// *****************************************************************************************************
// *****************************************************************************************************

inline void FindClosestPointsBetweenLines(	const ae::math::float3& lineStart0, const ae::math::float3& lineEnd0, 
											const ae::math::float3& lineStart1, const ae::math::float3& lineEnd1,
											ae::math::float3& closestPt0, 
											ae::math::float3& closestPt1)
{
	const ae::math::float3 s1(lineEnd0 - lineStart0), s2(lineEnd1 - lineStart1), s3(lineStart0 - lineStart1);
	const float	dot11(s1.dot(s1)), dot22(s2.dot(s2)), dot12(s1.dot(s2));
	const float	dot13(s1.dot(s3)), dot23(s2.dot(s3)), d(dot11*dot22 - dot12*dot12);
	float d1(d), d2(d), n1, n2;

	if( d < small_float ) { 
		n1 = 0.f; d1 = 1.f;	n2 = dot23;	d2 = dot22;
	} else 	{                
		n1 = (dot12*dot23 - dot22*dot13);
		n2 = (dot11*dot23 - dot12*dot13);
		if( n1 < 0.f ) {
			n1 = 0.f;
			n2 = dot23;
			d2 = dot22;
		} else if( n1 > d1 ) {
			n1 = d1;
			n2 = dot23 + dot12;
			d2 = dot22;
		}
	}

	if( n2 < 0.f ) {           
		n2 = 0.f;
		if( -dot13 < 0.f )
			n1 = 0.f;
		else if( -dot13 > dot11 )
			n1 = d1;
		else {
			n1 = -dot13;
			d1 = dot11;
		}
	}
	else if( n2 > d2 ) {      
		n2 = d2;
		if( (-dot13 + dot12) < 0.f )
			n1 = 0.f;
		else if( (-dot13 + dot12) > dot11 )
			n1 = d1;
		else {
			n1 = (-dot13 + dot12);
			d1 = dot11;
		}
	}

	const float time1 = (fabs(n1) < small_float ? 0.f : n1 / d1);
	const float time2 = (fabs(n2) < small_float ? 0.f : n2 / d2);
	closestPt0 = lineStart0 + s1 * time1;
	closestPt1 = lineStart1 + s2 * time2;
}

} } // namespace ae { namespace math {
