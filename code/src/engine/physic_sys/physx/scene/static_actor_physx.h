#pragma once
#ifndef AE_PHYSIC_SYS_ACTORS_PHYSX_H
#define AE_PHYSIC_SYS_ACTORS_PHYSX_H

namespace ae { namespace physic {
	class C_Scene;
	class C_StaticActor;
} } // namespace ae { namespace physic {

namespace ae { namespace physic { namespace physx {

	class C_StaticActor
	{
		friend class C_Scene;
		friend class ae::physic::C_StaticActor;

		PxRigidStatic* m_pStatic;
	};

} } } // namespace ae { namespace physic { namespace physx {

namespace ae { namespace physic {
	typedef physx::C_StaticActor T_StaticActorImpl;
} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_ACTORS_PHYSX_H
