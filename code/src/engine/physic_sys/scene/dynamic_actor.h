#pragma once
#ifndef AE_PHYSIC_SYS_DYNAMIC_ACTOR_H
#define AE_PHYSIC_SYS_DYNAMIC_ACTOR_H

#ifdef AE_PHYSX
	#include "../physx/scene/dynamic_actor_physx.h"
#endif // #ifdef AE_PHYSX

#include "actor.h"

namespace ae { namespace physic {

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_DynamicActor : public C_Actor
	{
		friend class C_Scene;
		T_DynamicActorImpl m_Impl;

		C_DynamicActor() {};
		~C_DynamicActor() {};
	public:
		T_DynamicActorImpl& Impl() { return m_Impl; }
		const T_DynamicActorImpl& Impl() const { return m_Impl; }
		void EnableKinematic(bool bEnable);
		bool IsKinematic() const;
		void SetPosition(const ae::math::float3& pos);
		void MoveTo(const ae::math::float3& pos);
		void GetPosition(ae::math::float3& pos) const;
		void GetTransformation(ae::math::float4x4& matrix) const;
		// *****************************************************************************************************
		void SetAngularDamping(float angularDamping);
		void SetLinerDamping(float linearDamping);
		void SetMaxAngularVelocity(float maxAngularVelocity);
		float GetAngularDamping() const;
		float GetLinerDamping() const;
		float GetMaxAngularVelocity() const;
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
		void UpdateShape(const S_Cylinder&);
		void UpdateShape(const S_Capsule&);
		void UpdateShape(const S_Box&);
		void UpdateShape(const S_Sphere&);
		// *****************************************************************************************************
		bool IsSleeping() const;
		void SetSleepingTreshold(float threshHold);
		float GetSleepingTreshold() const;
	};

} } // namespace ae { namespace physic {

#include "dynamic_actor.inl"

#endif // #ifdef AE_PHYSIC_SYS_DYNAMIC_ACTOR_H
