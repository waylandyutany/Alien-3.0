#pragma once
#ifndef AE_PHYSIC_SYS_SCENE_H
#define AE_PHYSIC_SYS_SCENE_H

#ifdef AE_PHYSX
#include "../physx/scene/scene_physx.h"
#include "../physx/scene/static_actor_physx.h"
#include "../physx/scene/dynamic_actor_physx.h"
#endif // #ifdef AE_PHYSX

#include "../../math_sys/types.h"
#include "../resources/resources.h"
#include "collision_params.h"
#include "collision_objects.h"

namespace ae { namespace scene {
	struct S_SceneNode;
} } // namespace ae { namespace scene {

namespace ae { namespace physic {

	class C_Device;
	class C_Scene;

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Scene
	{
		NOCOPY_CLASS(C_Scene);
		friend class C_PhysicSys;

		C_Scene(C_Resources& resources, C_Device& device);
		~C_Scene();

		T_SceneImpl m_Impl;

		C_Device& m_Device;
		C_Resources& m_Resources;
		C_Material m_DefaultMaterial;

		typedef std::deque<C_StaticActor*> T_StaticActors;
		T_StaticActors m_StaticActors;
	public:
		// *****************************************************************************************************
		void AddSceneNode(ae::scene::S_SceneNode*);
		void RemoveSceneNode(ae::scene::S_SceneNode*);
		// *****************************************************************************************************
		C_DynamicActor* CreateDynamicActor	(const S_Cylinder&, const ae::math::float3& pos);
		C_DynamicActor* CreateDynamicActor	(const S_Capsule&, const ae::math::float3& pos);
		C_DynamicActor* CreateDynamicActor	(const S_Box&, const ae::math::float3& pos);
		C_DynamicActor* CreateDynamicActor	(const S_Sphere&, const ae::math::float3& pos);
		void DestroyDynamicActor			(C_DynamicActor*);
		// *****************************************************************************************************
		// ! ! ! result.timeToCollision and result.distanceToCollision may be negative which means that we are inside the collision
		bool LineCollision	(const ae::math::float3& lineStart, const ae::math::float3& lineEnd, S_CollisionResult& result, C_Actor* pIgnore = 0) const;

		bool SweepCollision	(const S_Cylinder&, const S_CollisionParams&, S_CollisionResult&) const;
		bool SweepCollision	(const S_Capsule&, const S_CollisionParams&, S_CollisionResult&) const;
		bool SweepCollision	(const S_Box&, const S_CollisionParams&, S_CollisionResult&) const;
		bool SweepCollision	(const S_Sphere&, const S_CollisionParams&, S_CollisionResult&) const;

		//mPlayerVel = mPlayerVel - normal * Dot(mPlayerVel,nomal)/Dot(normal,normal)
	};
	// *****************************************************************************************************
	// *****************************************************************************************************

} } // namespace ae { namespace physic {

#include "scene.inl"

#endif // #ifdef AE_PHYSIC_SYS_SCENE_H
