#pragma once
#ifndef AE_PHYSIC_SYS_ACTOR_H
#define AE_PHYSIC_SYS_ACTOR_H

namespace ae { namespace physic {

	class C_Actor;
	// *****************************************************************************************************
	// *****************************************************************************************************
	struct S_Contact {
		ae::math::float3 position, normal;
	};
	class I_ContactCallback
	{
	public:
		virtual void OnContact(C_Actor* /*pActor*/, const S_Contact* /*contacts*/, ae::u32 /*numContacts*/) {}
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Actor
	{
		I_ContactCallback* m_pContactCB;
	public:
		C_Actor() : m_pContactCB(0) {}
		void OnContact(C_Actor* pActor, const S_Contact* contacts, ae::u32 numContacts);
		I_ContactCallback* SetContactCB(I_ContactCallback*);
	};

} } // namespace ae { namespace physic {

#include "actor.inl"

#endif // #ifdef AE_PHYSIC_SYS_ACTOR_H
