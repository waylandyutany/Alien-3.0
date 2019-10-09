namespace ae { namespace math {

	inline plane CreatePlane(const float3& p, const float3& n) {
		return plane(n.x, n.y, n.z, -(n.x*p.x + n.y*p.y + n.z*p.z));
	}

	inline float PlaneDistance(const float3& p, const plane& pl) {
		return fabs(pl.x*p.x + pl.y*p.y + pl.z*p.z + pl.w) / sqrtf(pl.x*pl.x + pl.y*pl.y + pl.z*pl.z);
	}



} } // namespace ae { namespace math {
