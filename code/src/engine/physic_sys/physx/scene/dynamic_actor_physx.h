#pragma once
#ifndef AE_PHYSIC_SYS_DYNAMIC_ACTOR_PHYSX_H
#define AE_PHYSIC_SYS_DYNAMIC_ACTOR_PHYSX_H

namespace ae { namespace physic {
	class C_Scene;
	class C_DynamicActor;

	struct S_Cylinder;
	struct S_Box;
	struct S_Capsule;
	struct S_Sphere;

} } // namespace ae { namespace physic {

namespace ae { namespace physic { namespace physx {

	class C_DynamicActor
	{
		friend class C_Scene;
		friend class ae::physic::C_DynamicActor;
		PxRigidDynamic* m_pDynamic;
		// *****************************************************************************************************
		void EnableKinematic(bool bEnable);
		bool IsKinematic() const;
		void SetPosition(const ae::math::float3& pos);
		void MoveTo(const ae::math::float3& pos);
		void GetPosition(ae::math::float3& pos) const;
		void GetTransformation(ae::math::float4x4& matrix) const;
		// *****************************************************************************************************
		void SetLinearVelocity(const ae::math::float3& linearVelocity);
		void SetAngularVelocity(const ae::math::float3& angularVelocity);
		void GetLinearVelocity(ae::math::float3& linearVelocity) const;
		void GetAngularVelocity(ae::math::float3& angularVelocity) const;
		// *****************************************************************************************************
		void AddForce(const ae::math::float3& force);
		void AddForce(const ae::math::float3& force, const ae::math::float3& pos);
		void AddImpulse(const ae::math::float3& force);
		void AddImpulse(const ae::math::float3& force, const ae::math::float3& pos);
		// *****************************************************************************************************
		void SetAngularDamping(float angularDamping);
		void SetLinerDamping(float linearDamping);
		void SetMaxAngularVelocity(float maxAngularVelocity);
		// *****************************************************************************************************
		float GetAngularDamping() const;
		float GetLinerDamping() const;
		float GetMaxAngularVelocity() const;
		// *****************************************************************************************************
		void UpdateShape(const PxGeometry&, const PxTransform&);
		void UpdateShape(const S_Cylinder&);
		void UpdateShape(const S_Box&);
		void UpdateShape(const S_Capsule&);
		void UpdateShape(const S_Sphere&);
		// *****************************************************************************************************
		bool IsSleeping() const;
		void SetSleepingTreshold(float threshHold);
		float GetSleepingTreshold() const;
	};

} } } // namespace ae { namespace physic { namespace physx {

#include "dynamic_actor_physx.inl"

namespace ae { namespace physic {
	typedef physx::C_DynamicActor T_DynamicActorImpl;
} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_DYNAMIC_ACTOR_PHYSX_H
