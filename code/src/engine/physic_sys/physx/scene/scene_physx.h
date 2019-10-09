#pragma once
#ifndef AE_PHYSIC_SYS_SCENE_PHYSX_H
#define AE_PHYSIC_SYS_SCENE_PHYSX_H

#include "../../../math_sys/types.h"

namespace ae { namespace physic {
	class C_Scene;
	class C_Actor;
	class C_DynamicActor;
	class C_StaticActor;

	struct S_CollisionResult;
	struct S_CollisionParams;

	struct S_Cylinder;
	struct S_Box;
	struct S_Capsule;
	struct S_Sphere;
} } // namespace ae { namespace physic {

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace physic { namespace physx {

	class C_Device;
	class C_Mesh;
	class C_Scene;
	class C_Material;
	class C_DynamicActor;
	class C_StaticActor;
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Scene
	{
		friend class ae::physic::C_Scene;
		NOCOPY_CLASS(C_Scene);

		C_Scene(C_Device&);
		~C_Scene();

		C_Device& m_Device;
		// *****************************************************************************************************
		bool CreateStaticActor(const ae::math::float4x4& transformation, C_StaticActor& actor, void*);
		void DeleteStaticActor(C_StaticActor& actor);
		void AddStaticActorGeometry(C_StaticActor& actor, C_Mesh& mesh, C_Material& material);
		// *****************************************************************************************************
		bool CreateDynamicActor(const PxGeometry&, const PxTransform&, const ae::math::float3&, PxMaterial&, C_DynamicActor& actor, void*);
		bool CreateDynamicActor(const S_Cylinder&, const ae::math::float3& pos, C_Material& material, C_DynamicActor& actor, void*);
		bool CreateDynamicActor(const S_Capsule&, const ae::math::float3& pos, C_Material& material, C_DynamicActor& actor, void*);
		bool CreateDynamicActor(const S_Box&, const ae::math::float3& pos, C_Material& material, C_DynamicActor& actor, void*);
		bool CreateDynamicActor(const S_Sphere&, const ae::math::float3& pos, C_Material& material, C_DynamicActor& actor, void*);
		void DeleteDynamicActor(C_DynamicActor& actor);
		// *****************************************************************************************************
		bool LineCollision(const ae::math::float3& lineStart, const ae::math::float3& lineEnd, ae::physic::S_CollisionResult& result, ae::physic::C_Actor* pIgnore = 0) const;

		bool SweepCollision(const PxGeometry&, const PxTransform&, const S_CollisionParams&, S_CollisionResult&) const;
		bool SweepCollision(const S_Cylinder&, const S_CollisionParams&, S_CollisionResult&) const;
		bool SweepCollision(const S_Capsule&, const S_CollisionParams&, S_CollisionResult&) const;
		bool SweepCollision(const S_Box&, const S_CollisionParams&, S_CollisionResult&) const;
		bool SweepCollision(const S_Sphere&, const S_CollisionParams&, S_CollisionResult&) const;
	public:
		static PxConvexMesh* convexCylinderMesh;
		static PxConvexMesh* convexCylinderCupMesh;
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
} } } // namespace ae { namespace physic { namespace physx {

namespace ae { namespace physic {
	typedef physx::C_Scene T_SceneImpl;
} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_SCENE_PHYSX_H
