namespace ae { namespace physic {

	inline bool C_Scene::LineCollision(const ae::math::float3& lineStart, const ae::math::float3& lineEnd, S_CollisionResult& result, C_Actor* pIgnore) const {
		return m_Impl.LineCollision(lineStart, lineEnd, result, pIgnore);
	}

	inline bool C_Scene::SweepCollision(const S_Cylinder& shape, const S_CollisionParams& params, S_CollisionResult& result) const {
		return m_Impl.SweepCollision(shape, params, result);
	}

	inline bool C_Scene::SweepCollision(const S_Capsule& shape, const S_CollisionParams& params, S_CollisionResult& result) const {
		return m_Impl.SweepCollision(shape, params, result);
	}

	inline bool C_Scene::SweepCollision(const S_Box& shape, const S_CollisionParams& params, S_CollisionResult& result) const {
		return m_Impl.SweepCollision(shape, params, result);
	}

	inline bool C_Scene::SweepCollision(const S_Sphere& shape, const S_CollisionParams& params, S_CollisionResult& result) const {
		return m_Impl.SweepCollision(shape, params, result);
	}

} } // namespace ae { namespace physic {
