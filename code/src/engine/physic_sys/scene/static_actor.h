#pragma once
#ifndef AE_PHYSIC_SYS_STATIC_ACTOR_H
#define AE_PHYSIC_SYS_STATIC_ACTOR_H

#include "actor.h"

#ifdef AE_PHYSX
	#include "../physx/scene/static_actor_physx.h"
#endif // #ifdef AE_PHYSX

namespace ae { namespace scene {
	struct S_SceneNode;
} } // namespace ae { namespace scene {

namespace ae { namespace physic {

	class C_StaticActor : public C_Actor
	{
		T_StaticActorImpl m_Impl;
		ae::scene::S_SceneNode* m_pSceneNode;
		C_StaticActor();
	public:
		C_StaticActor(ae::scene::S_SceneNode* pSceneNode) : m_pSceneNode(pSceneNode) {}

		T_StaticActorImpl& Impl();
		const T_StaticActorImpl& Impl() const;

		const ae::scene::S_SceneNode* SceneNode() const;
		ae::scene::S_SceneNode* SceneNode();
	};

} } // namespace ae { namespace physic {

#include "static_actor.inl"

#endif // #ifdef AE_PHYSIC_SYS_STATIC_ACTOR_H
