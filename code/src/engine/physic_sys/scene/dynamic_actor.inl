namespace ae { namespace physic {
	// *****************************************************************************************************
	// *****************************************************************************************************

	inline void C_DynamicActor::SetSleepingTreshold(float threshHold) {
		m_Impl.SetSleepingTreshold(threshHold);
	}

	inline float C_DynamicActor::GetSleepingTreshold() const {
		return m_Impl.GetSleepingTreshold();
	}

	inline bool C_DynamicActor::IsSleeping() const {
		return m_Impl.IsSleeping();
	}

	inline void C_DynamicActor::EnableKinematic(bool bEnable) {
		m_Impl.EnableKinematic(bEnable);
	}

	inline bool C_DynamicActor::IsKinematic() const {
		return m_Impl.IsKinematic();
	}

	inline void C_DynamicActor::SetPosition(const ae::math::float3& pos) {
		m_Impl.SetPosition(pos);
	}

	inline void C_DynamicActor::MoveTo(const ae::math::float3& pos) {
		m_Impl.MoveTo(pos);
	}

	inline void C_DynamicActor::GetPosition(ae::math::float3& pos) const {
		m_Impl.GetPosition(pos);
	}

	inline void C_DynamicActor::GetTransformation(ae::math::float4x4& matrix) const {
		m_Impl.GetTransformation(matrix);
	}

	inline void C_DynamicActor::SetAngularDamping(float angularDamping) {
		m_Impl.SetAngularDamping(angularDamping);
	}

	inline void C_DynamicActor::SetLinerDamping(float linearDamping) {
		m_Impl.SetLinerDamping(linearDamping);
	}

	inline void C_DynamicActor::SetMaxAngularVelocity(float maxAngularVelocity) {
		m_Impl.SetMaxAngularVelocity(maxAngularVelocity);
	}

	inline float C_DynamicActor::GetAngularDamping() const {
		return m_Impl.GetAngularDamping();
	}

	inline float C_DynamicActor::GetLinerDamping() const {
		return m_Impl.GetLinerDamping();
	}

	inline float C_DynamicActor::GetMaxAngularVelocity() const {
		return m_Impl.GetMaxAngularVelocity();
	}

	inline void C_DynamicActor::SetLinearVelocity(const ae::math::float3& linearVelocity) {
		m_Impl.SetLinearVelocity(linearVelocity);
	}

	inline void C_DynamicActor::SetAngularVelocity(const ae::math::float3& angularVelocity) {
		m_Impl.SetAngularVelocity(angularVelocity);
	}

	inline void C_DynamicActor::GetLinearVelocity(ae::math::float3& linearVelocity) const {
		m_Impl.GetLinearVelocity(linearVelocity);
	}

	inline void C_DynamicActor::GetAngularVelocity(ae::math::float3& angularVelocity) const {
		m_Impl.GetAngularVelocity(angularVelocity);
	}

	inline void C_DynamicActor::UpdateShape(const S_Cylinder& shape) {
		m_Impl.UpdateShape(shape);
	}

	inline void C_DynamicActor::UpdateShape(const S_Capsule& shape) {
		m_Impl.UpdateShape(shape);

	}

	inline void C_DynamicActor::UpdateShape(const S_Box& shape) {
		m_Impl.UpdateShape(shape);

	}

	inline void C_DynamicActor::UpdateShape(const S_Sphere& shape) {
		m_Impl.UpdateShape(shape);
	}

	// *****************************************************************************************************
	inline void C_DynamicActor::AddForce(const ae::math::float3& force) {
		m_Impl.AddForce(force);
	}

	inline void C_DynamicActor::AddForce(const ae::math::float3& force, const ae::math::float3& pos) {
		m_Impl.AddForce(force, pos);
	}

	inline void C_DynamicActor::AddImpulse(const ae::math::float3& force) {
		m_Impl.AddImpulse(force);
	}

	inline void C_DynamicActor::AddImpulse(const ae::math::float3& force, const ae::math::float3& pos) {
		m_Impl.AddImpulse(force, pos);
	}
	// *****************************************************************************************************
} } // namespace ae { namespace physic {
