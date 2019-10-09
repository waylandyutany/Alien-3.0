#pragma once

#ifndef AG_ENTITIES_PROJECTILE_H
#define AG_ENTITIES_PROJECTILE_H

#include "../entity.h"
#include "../../../engine/math_sys/types.h"

#define DRAW_PROJECTILE_TRAIL

namespace ae { namespace physic {
	class C_Actor;
} } // namespace ae { namespace physic {

namespace ag { namespace entities {

#ifdef DRAW_PROJECTILE_TRAIL
	typedef std::vector<ae::math::float3> T_TrailPoints;
	extern T_TrailPoints g_TrailPoints;
#endif // #ifdef DRAW_PROJECTILE_TRAIL

// *****************************************************************************************************
// *****************************************************************************************************
	class C_Projectile : public I_Entity
	{
		NOCOPY_CLASS(C_Projectile);
		ae::physic::C_Actor* m_pParentActor;
		ae::math::float3 m_Position, m_Velocity;
	public:
		C_Projectile(ae::physic::C_Actor* pParentActor, const ae::math::float3& pos, const ae::math::float3& velocity);
		~C_Projectile();
	protected:
		virtual void OnUpdate(float deltaTime);
		virtual void OnAfterUpdate();
	};

} } // namespace ag { namespace entities {

#endif // #ifdef AG_ENTITIES_PROJECTILE_H
