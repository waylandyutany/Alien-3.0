#include "StdAfx.h"

#include "projectile.h"
#include "../../../engine/scene_sys/scene_sys.h"
#include "../../../engine/physic_sys/physic_sys.h"
#include "../../../engine/physic_sys/scene/scene.h"
#include "../../../engine/physic_sys/scene/dynamic_actor.h"
#include "../../../engine/physic_sys/scene/collision_params.h"

namespace ag { namespace entities {

#ifdef DRAW_PROJECTILE_TRAIL
	C_Projectile* g_pProjectile = 0;
	T_TrailPoints g_TrailPoints;
	const float g_fTrailPrecision = 0.1f;
	float g_fTrailPrecisionCounter = g_fTrailPrecision;
#endif // #ifdef DRAW_PROJECTILE_TRAIL

C_Projectile::C_Projectile(ae::physic::C_Actor* pParentActor, const ae::math::float3& pos, const ae::math::float3& velocity) :
	m_pParentActor(pParentActor),
	m_Position(pos), m_Velocity(velocity)
{
#ifdef DRAW_PROJECTILE_TRAIL
	g_TrailPoints.clear();
	g_pProjectile = this;
	g_fTrailPrecisionCounter = -0.1f;
#endif // #ifdef DRAW_PROJECTILE_TRAIL
}

C_Projectile::~C_Projectile()
{
}

/*virtual*/ void C_Projectile::OnUpdate(float deltaTime)
{
	// gravity applying
	m_Velocity += ae::math::float3(0,-9.8f,0) * deltaTime;

	ae::math::float3 deltaVelocity = m_Velocity * deltaTime;
	ae::physic::S_CollisionResult collResult;


#ifdef DRAW_PROJECTILE_TRAIL
	const size_t maxTrailPoints = 6400;
	if(g_pProjectile == this && g_TrailPoints.size() < maxTrailPoints)
	{
		g_fTrailPrecisionCounter -= deltaTime;

		if(g_fTrailPrecisionCounter < 0.0f)
		{
			g_fTrailPrecisionCounter = g_fTrailPrecision;
			g_TrailPoints.push_back(m_Position);
		}
	}
#endif // #ifdef DRAW_PROJECTILE_TRAIL

	if(ae::physic::C_PhysicSys::Instance().Scene().LineCollision(m_Position, m_Position + deltaVelocity, collResult, m_pParentActor))
	{
#ifdef DRAW_PROJECTILE_TRAIL
		if(g_pProjectile == this && g_TrailPoints.size() < maxTrailPoints)
			g_TrailPoints.push_back(m_Position + deltaVelocity);
#endif // #ifdef DRAW_PROJECTILE_TRAIL

		if(collResult.pDynamicActor)
			collResult.pDynamicActor->AddImpulse(m_Velocity * 0.1f, collResult.collisionContact);

		SetDead();
	} else {
		m_Position += deltaVelocity;
	}
}

/*virtual*/ void C_Projectile::OnAfterUpdate()
{
}

} } // namespace ag { namespace entities {

