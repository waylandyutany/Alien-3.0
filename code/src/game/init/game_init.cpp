#include "StdAfx.h"

#include "game_init.h"

#include "../entities/entities.h"

namespace ag {

C_AlienGameInit::C_AlienGameInit()
{
	m_pPlayer = new ag::entities::C_Player();
	m_pHUD = new ag::entities::C_HUD();
	m_pDynObject = new ag::entities::C_DynObject("tv_buildings/models/objects.mesh/barrel.mgeo",ae::math::float3(2,20,2));
}

C_AlienGameInit::~C_AlienGameInit()
{
	delete m_pDynObject;
	delete m_pPlayer;
	delete m_pHUD;
}

void C_AlienGameInit::Update( float deltaTime )
{
	ag::entities::C_Entities::Inst().Update(deltaTime);
}

void C_AlienGameInit::AfterUpdate()
{
	ag::entities::C_Entities::Inst().AfterUpdate();
}

} // namespace ae {
