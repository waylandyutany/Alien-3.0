#pragma once

#ifndef AG_ENTITIES_PLAYER_H
#define AG_ENTITIES_PLAYER_H

#include "../entity.h"
#include "../../../engine/math_sys/types.h"
#include "../../../engine/render_sys/render_scene/render_camera.h"
#include "../../../engine/physic_sys/scene/actor.h" // I_ContactCallback
#include "../../../engine/physic_sys/scene/collision_objects.h"

namespace ae { namespace physic {
	class C_DynamicActor;
} } // namespace ae { namespace physic {

namespace ae { namespace scene {
	class C_DirectionalLight;
	class C_PointLight;
} } // namespace ae { namespace scene {

namespace ae { namespace audio {
	class C_Sound;
	class C_Listener;
	class C_Context;
} } // namespace ae { namespace render {

namespace ag { namespace entities {
	class C_Player;
// *****************************************************************************************************
// *****************************************************************************************************
	class C_PlayerCollContactsCB : public ae::physic::I_ContactCallback
	{
		NOCOPY_CLASS(C_PlayerCollContactsCB);
		C_Player* m_pPlayer;
		virtual void OnContact(ae::physic::C_Actor* /*pActor*/, const ae::physic::S_Contact* /*contacts*/, ae::u32 /*numContacts*/);
	public:
		C_PlayerCollContactsCB(C_Player* pPlayer) : m_pPlayer(pPlayer) {}
	};

// *****************************************************************************************************
	enum E_CameraType
	{
		E_Camera_FPS,
		E_Camera_Free,
	};
//#define USE_CYLINDER
// *****************************************************************************************************
	class C_Player : public I_Entity
	{
		friend class C_PlayerCollContactsCB;
		C_PlayerCollContactsCB contactsCB;
		ae::render::C_Camera playerCamera;

		float m_PlayerHeight, m_PlayerRadius, playerDistanceToGround;
		ae::math::float3 m_PlayerPos, m_PlayerVel;
#ifdef USE_CYLINDER
		typedef ae::physic::S_Cylinder T_PlayerCollision;
#else
		typedef ae::physic::S_Box T_PlayerCollision;
#endif

		T_PlayerCollision m_PlayerCollision;
		ae::math::float3 m_DirLightDirection0,m_DirLightDirection1;
		float m_DirLightBlend, m_DirLightSpeed;

		ae::audio::C_Context* m_pContext;
		ae::audio::C_Listener* m_pListener;

		ae::audio::C_Sound *m_pBreatingSound, *m_pAmbientSound;

		ae::scene::C_DirectionalLight* m_pDirectionalLight;
		ae::scene::C_PointLight* m_pPointLight;

		ae::physic::C_DynamicActor* m_pDynamicActor;

		void HandleFalling();
		void HandleRotation(float deltaTime);
		void HandleShooting(float deltaTime);
		void HandleCrouch(float deltaTime);
		void HandleMove(float deltaTime);
		void HandleJump(float deltaTime);
		void HandleZoom(float deltaTime);
		void HandleCameraTypeChange();
		void DrawShootingCross();
		bool IsFalling() const;
		bool IsOnGround() const;

		void MovePlayer(const ae::math::float3& moveDir);
		float CalculateStepHeight(float playerHeight) const;
		NOCOPY_CLASS(C_Player);
	public:
		C_Player();
		~C_Player();

		static E_CameraType GetCameraType();
	protected:
		virtual void OnUpdate(float deltaTime);
		virtual void OnAfterUpdate();
	};

} } // namespace ag { namespace entities {

#endif // #ifdef AG_ENTITIES_PLAYER_H
