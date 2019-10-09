namespace ae { namespace physic { namespace physx {

	// *****************************************************************************************************
	// *****************************************************************************************************
	inline void C_DynamicActor::SetSleepingTreshold(float threshHold) {
		m_pDynamic->setSleepThreshold(threshHold);
	}

	inline float C_DynamicActor::GetSleepingTreshold() const {
		return m_pDynamic->getSleepThreshold();
	}

	inline bool C_DynamicActor::IsSleeping() const {
		return m_pDynamic->isSleeping();
	}

	inline void C_DynamicActor::EnableKinematic(bool bEnable) {
		m_pDynamic->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, bEnable);
	}

	inline bool C_DynamicActor::IsKinematic() const	{
		return m_pDynamic->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC ? true : false;
	}

	inline void C_DynamicActor::SetPosition(const ae::math::float3& pos) {
		PxTransform pose((PxVec3&)pos);
		m_pDynamic->setGlobalPose(pose);
	}

	inline void C_DynamicActor::MoveTo(const ae::math::float3& pos)	{
		PxTransform pose((PxVec3&)pos);
		m_pDynamic->setKinematicTarget(pose);
	}

	inline void C_DynamicActor::GetPosition(ae::math::float3& pos) const {
		PxTransform pose = m_pDynamic->getGlobalPose();
		pos.x = pose.p.x;
		pos.y = pose.p.y;
		pos.z = pose.p.z;
	}

	inline void C_DynamicActor::GetTransformation(ae::math::float4x4& matrix) const	{
		PxMat44 mat(m_pDynamic->getGlobalPose());
		memcpy(&matrix, &mat, sizeof(mat));
	}

	inline void C_DynamicActor::SetAngularDamping(float angularDamping) {
		m_pDynamic->setAngularDamping(angularDamping);
	}

	inline void C_DynamicActor::SetLinerDamping(float linearDamping) {
		m_pDynamic->setLinearDamping(linearDamping);
	}

	inline float C_DynamicActor::GetAngularDamping() const {
		return m_pDynamic->getAngularDamping();
	}

	inline float C_DynamicActor::GetLinerDamping() const {
		return m_pDynamic->getLinearDamping();
	}

	inline void C_DynamicActor::SetMaxAngularVelocity(float maxAngularVelocity) {
		m_pDynamic->setMaxAngularVelocity(maxAngularVelocity);
	}

	inline float C_DynamicActor::GetMaxAngularVelocity() const {
		return m_pDynamic->getMaxAngularVelocity();
	}

	inline void C_DynamicActor::SetLinearVelocity(const ae::math::float3& linearVelocity) {
		m_pDynamic->setLinearVelocity((const PxVec3&)linearVelocity);
	}

	inline void C_DynamicActor::SetAngularVelocity(const ae::math::float3& angularVelocity) {
		m_pDynamic->setAngularVelocity((const PxVec3&)angularVelocity);
	}

	inline void C_DynamicActor::GetLinearVelocity(ae::math::float3& linearVelocity) const {
		PxVec3 v(m_pDynamic->getLinearVelocity());
		linearVelocity.x = v.x;
		linearVelocity.y = v.y;
		linearVelocity.z = v.z;
	}

	inline void C_DynamicActor::GetAngularVelocity(ae::math::float3& angularVelocity) const {
		PxVec3 v(m_pDynamic->getAngularVelocity());
		angularVelocity.x = v.x;
		angularVelocity.y = v.y;
		angularVelocity.z = v.z;
	}

	inline void C_DynamicActor::AddForce(const ae::math::float3& force) {
		m_pDynamic->addForce((const PxVec3&)force);
	}

	inline void C_DynamicActor::AddForce(const ae::math::float3& force, const ae::math::float3& pos) {
		PxRigidBodyExt::addForceAtPos(*m_pDynamic, (const PxVec3&)force, (const PxVec3&)pos);
	}

	inline void C_DynamicActor::AddImpulse(const ae::math::float3& force) {
		m_pDynamic->addForce((const PxVec3&)force, PxForceMode::eIMPULSE);
	}

	inline void C_DynamicActor::AddImpulse(const ae::math::float3& force, const ae::math::float3& pos) {
		PxRigidBodyExt::addForceAtPos(*m_pDynamic, (const PxVec3&)force, (const PxVec3&)pos, PxForceMode::eIMPULSE);
	}

} } } // namespace ae { namespace physic { namespace physx {
