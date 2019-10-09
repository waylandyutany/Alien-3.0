#pragma once

#ifndef AE_PHYSIC_SYS_COLLISION_PARAMS_H
#define AE_PHYSIC_SYS_COLLISION_PARAMS_H

#include "../../math_sys/types.h"

namespace ae { namespace physic {

	struct S_Material;
	class C_Actor;
	class C_DynamicActor;
	class C_DynamicActor;
	class C_StaticActor;

	// Input for collision check(sweep)
	struct S_CollisionParams
	{
		ae::math::float3 pos;
		ae::math::float3 direction;
		float distance;
		C_Actor* pActorToIgnore;
	};

	// Output results from collision check
	struct S_CollisionResult
	{
		S_CollisionResult() :	timeToCollision(ae::math::max_float), 
								material(0), 
								collisionNormal(0,0,0),
								collisionContact(0,0,0),
								pDynamicActor(0), 
								pStaticActor(0) { }

		S_Material* material;
		float timeToCollision;
		float distanceToCollision;
		ae::math::float3 collisionNormal;
		ae::math::float3 collisionContact;
		ae::physic::C_DynamicActor* pDynamicActor;
		ae::physic::C_StaticActor* pStaticActor;
	};


} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_COLLISION_PARAMS_H
