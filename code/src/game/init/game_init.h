#pragma once

#ifndef AG_GAME_INIT_H
#define AG_GAME_INIT_H

#include "../entities/player/player.h"
#include "../entities/hud/hud.h"
#include "../entities/dynobject/dyn_object.h"

namespace ae { namespace base {
	struct S_Sync;
} } // namespace ae { namespace base {

namespace ag {

	class C_AlienGameInit
	{
		ag::entities::C_Player* m_pPlayer;
		ag::entities::C_HUD* m_pHUD;
		ag::entities::C_DynObject* m_pDynObject;
	public:
		C_AlienGameInit();
		~C_AlienGameInit();

		void Update( float deltaTime );
		void AfterUpdate();
	};

} // namespace ae {

#endif // #ifdef AG_GAME_INIT_H
