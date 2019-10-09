#include "StdAfx.h"

#include "player.h"
#include "../hud/hud.h"
#include "../projectiles/projectile.h"

#include "../../../engine/debug_sys/output/outputs.h"

#include "../../../engine/input_sys/input_sys.h"

#include "../../../engine/physic_sys/physic_sys.h"
#include "../../../engine/physic_sys/scene/dynamic_actor.h"
#include "../../../engine/physic_sys/scene/collision_params.h"

#include "../../../engine/render_sys/render_scene/render_scene.h"
#include "../../../engine/render_sys/render_sys.h"

#include "../../../engine/scene_sys/scene_sys.h"
#include "../../../engine/scene_sys/scene/lights.h"

#include "../../../engine/math_sys/types_dbg.h"
#include "../../../engine/_sys/templates.h"

#include "../../../engine/audio_sys/audio_sys.h"
#include "../../../engine/audio_sys/audio_scene/audio_scene.h"
#include "../../../engine/audio_sys/audio_scene/audio_listener.h"
#include "../../../engine/audio_sys/audio_scene/audio_context.h"
#include "../../../engine/audio_sys/audio_scene/audio_sound.h"

namespace ag { namespace entities {
extern C_HUD* g_pHUD;

// *****************************************************************************************************
const float playerWalkSlowSpeed(1.0f);
const float playerWalkSpeed(2.0f);
const float playerRunSpeed(6.0f);
const float playerCrouchSpeed(3.0f);

const float playerEyeDistanceFromTop(0.12f);
const float playerStandHeight(1.8f);
const float playerCrouchHeight(1.0f);
const float playerProneHeight(0.3f);

const float playerStandStepHeight(0.4f);
const float playerCrouchStepHeight(0.2f);
const float playerProneStepHeight(0.15f);

const float m_NoZoomFOV((float)D3DX_PI / 4.0f);
const float m_ZoomFOV(m_NoZoomFOV * 0.5f);
const float m_ZoomSpeed(2.0f);

const float playerRotationSpeed(0.25f);
const float playerJumpAcceleration(2.5);

// *****************************************************************************************************
E_CameraType cameraType = E_Camera_FPS;

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/ void C_PlayerCollContactsCB::OnContact(ae::physic::C_Actor* /*pActor*/, const ae::physic::S_Contact* /*contacts*/, ae::u32 /*numContacts*/)
{
}

// *****************************************************************************************************
// *****************************************************************************************************
E_CameraType C_Player::GetCameraType()
{
	return cameraType;
}

C_Player::C_Player() :
	contactsCB(this),
	m_PlayerHeight(playerStandHeight), 
	m_PlayerRadius(0.3f),
	m_PlayerPos(0.0f, m_PlayerHeight, 0.0f),
	m_PlayerVel(0.0f, 0.0f, 0.0f),
#ifdef USE_CYLINDER
	m_PlayerCollision(m_PlayerHeight, m_PlayerRadius), // capsule,cylinder
#else
	m_PlayerCollision(m_PlayerHeight, m_PlayerRadius * 2.0f, m_PlayerRadius * 2.0f), // box
#endif
	m_DirLightDirection0(-1,-1,1),m_DirLightDirection1(1,-1,1),
	m_DirLightBlend(0.0f), m_DirLightSpeed(1.0f),
	m_pDynamicActor(0)
{
	playerCamera.SetPos(m_PlayerPos);
	playerCamera.SetFOV(m_NoZoomFOV);

	ae::audio::C_AudioScene& audioScene = ae::audio::C_AudioSys::Instance().Scene();
	m_pContext = audioScene.CreateContext(4,8);
	m_pListener = audioScene.CreateListener();
	m_pContext->AddListener(m_pListener);

	m_pBreatingSound = m_pContext->CreateAmbientSound("/resources/default/sounds/breath.sndr");
	m_pAmbientSound = m_pContext->CreateAmbientSound("/resources/default/sounds/forest.sndr");

	m_pBreatingSound->Play(true);
	m_pAmbientSound->Play(true);

	m_pDirectionalLight = ae::scene::C_SceneSys::Inst().Scene().CreateDirectionalLight();
	m_pPointLight = ae::scene::C_SceneSys::Inst().Scene().CreatePointLight();

	m_pDirectionalLight->SetColor(ae::math::float3(0.75f,0.75f,0.75f));

	m_pPointLight->SetColor(ae::math::float3(1,0,0));
	m_pPointLight->SetRadius(16.0f);

	ae::render::C_RenderSys::Instance().GetScene().SetAmbientColor(ae::math::float4(0.5f,0.5f,0.5f,1));

	m_pDynamicActor = ae::physic::C_PhysicSys::Instance().Scene().CreateDynamicActor(m_PlayerCollision, m_PlayerPos);
	m_pDynamicActor->SetMaxAngularVelocity(0.0f);
	m_pDynamicActor->SetAngularDamping(0.0f);
//	m_pDynamicActor->SetContactCB(&contactsCB);
}

C_Player::~C_Player()
{
	ae::physic::C_PhysicSys::Instance().Scene().DestroyDynamicActor(m_pDynamicActor);

	m_pContext->RemoveListener(m_pListener);
	delete m_pListener;
	delete m_pBreatingSound;
	delete m_pAmbientSound;
	delete m_pContext;

	ae::scene::C_SceneSys::Inst().Scene().DestroyDirectionalLight(m_pDirectionalLight);
	ae::scene::C_SceneSys::Inst().Scene().DestroyPointLight(m_pPointLight);
}

/*virtual*/ void C_Player::OnAfterUpdate()
{
	ae::render::C_RenderScene& scene = ae::render::C_RenderSys::Instance().GetScene();
	m_pDynamicActor->GetPosition(m_PlayerPos);
	ae::math::float3 playerEyePos(m_PlayerPos.x,m_PlayerPos.y + m_PlayerHeight*0.5f - playerEyeDistanceFromTop,m_PlayerPos.z);

	D3DXMATRIX viewMatrix;
	playerCamera.SetPos(playerEyePos);
	playerCamera.GetViewMatrix(viewMatrix);

	scene.SetCamera(playerCamera);

	m_pListener->SetPosition(playerCamera.GetPos());
	m_pListener->SetOrientation(playerCamera.GetDir(), playerCamera.GetUp());

	m_pPointLight->SetPosition(playerCamera.GetPos());

	HandleFalling();
}

void C_Player::DrawShootingCross()
{
	const ae::u32 crossHeight = 10;
	const ae::u32 crossWidth = 2;
	const ae::u32 crossHoleSize = 6;
	const ae::math::float4 crossColor(0,1,0,0.8f);

	ae::render::C_RenderScene& scene = ae::render::C_RenderSys::Instance().GetScene();

	ae::u32 screenCenterX, screenCenterY;
	scene.GetScreenDimensions(screenCenterX, screenCenterY);
	screenCenterX >>= 1;
	screenCenterY >>= 1;

	scene.Get2DContext().DrawBox(screenCenterX - (crossWidth >> 1), screenCenterY - crossHeight - (crossHoleSize >> 1), crossWidth, crossHeight, crossColor);
	scene.Get2DContext().DrawBox(screenCenterX - (crossWidth >> 1), screenCenterY + (crossHoleSize >> 1), crossWidth, crossHeight, crossColor);

	scene.Get2DContext().DrawBox(screenCenterX - (crossHoleSize >> 1) - crossHeight, screenCenterY - (crossWidth >> 1), crossHeight, crossWidth, crossColor);
	scene.Get2DContext().DrawBox(screenCenterX + (crossHoleSize >> 1), screenCenterY - (crossWidth >> 1), crossHeight, crossWidth, crossColor);
}

/*virtual*/ void C_Player::OnUpdate(float deltaTime)
{
	HandleCameraTypeChange();
	HandleShooting(deltaTime);
	HandleZoom(deltaTime);
	HandleJump(deltaTime);
	HandleRotation(deltaTime);
	HandleMove(deltaTime);
	HandleCrouch(deltaTime);
	DrawShootingCross();

	// *****************************************************************************************************************
	// moving with light direction
	// *****************************************************************************************************************
	ae::math::float3 lightDir = (m_DirLightDirection0 * m_DirLightBlend) + (m_DirLightDirection1 * (1.0f - m_DirLightBlend));
	m_pDirectionalLight->SetDirection(lightDir);
#if 0
	m_DirLightBlend += m_DirLightSpeed * deltaTime;
	if(m_DirLightBlend < 0.0f) {
		m_DirLightBlend = 0.0f;
		m_DirLightSpeed = 1.0f;
	}
	if(m_DirLightBlend > 1.0f) {
		m_DirLightBlend = 1.0f;
		m_DirLightSpeed = -1.0f;
	}
#endif
}

void C_Player::HandleShooting(float /*deltaTime*/)
{
	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();
	if(input.MouseState(ae::input::E_KS_Key_Pressed, ae::input::E_MB_LEFT))
	{
		ae::math::float3 playerEyePos;
		m_pDynamicActor->GetPosition(playerEyePos);
		playerEyePos = ae::math::float3(playerEyePos.x,playerEyePos.y + m_PlayerHeight*0.5f - playerEyeDistanceFromTop,playerEyePos.z);
		new C_Projectile(m_pDynamicActor, playerEyePos, playerCamera.GetDir() * 400.0f);
	}
}

void C_Player::HandleCameraTypeChange()
{
	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();
	if(input.KeyState(ae::input::E_KS_Key_Pressed, ae::input::E_KC_TAB) ) {
		if(cameraType == E_Camera_FPS)	{
			cameraType = E_Camera_Free;
			m_pDynamicActor->EnableKinematic(true);
		}
		else {
			cameraType = E_Camera_FPS;
			ae::ZeroStruct<>(&m_PlayerVel);
			m_pDynamicActor->EnableKinematic(false);
		}
	}
}

void C_Player::HandleRotation(float /*deltaTime*/)
{
	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();

	ae::math::float3 mouseMove = input.MouseMove();

	if(mouseMove.x != 0.0f || mouseMove.y != 0.0f)
		playerCamera.SetRot(ae::math::float3(playerCamera.GetRot().x + mouseMove.y * playerRotationSpeed, playerCamera.GetRot().y + mouseMove.x * playerRotationSpeed, 0));
}

void C_Player::HandleMove(float deltaTime)
{
	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();

	ae::math::float3 motion(0,0,0);
	if(input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_RIGHT) || input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_D))
		motion += playerCamera.GetRight();
	if(input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_LEFT) || input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_A))
		motion -= playerCamera.GetRight();
	if(input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_UP) || input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_W))
		motion += playerCamera.GetDir();
	if(input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_DOWN) || input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_S))
		motion -= playerCamera.GetDir();

	if(cameraType == E_Camera_FPS)
		motion.y = 0.0f;

	motion = motion.normalize();

	if(cameraType == E_Camera_Free)
		motion *=  10.0f;

	if(input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_LSHIFT))
		m_PlayerVel = motion * playerRunSpeed;
	else if(input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_LALT))
		m_PlayerVel = motion * playerWalkSlowSpeed;
	else 
		m_PlayerVel = motion * playerWalkSpeed;

	if(m_PlayerVel.size() > 0.0f)
	{
		if(cameraType == E_Camera_Free)
			m_pDynamicActor->SetPosition(m_PlayerPos + m_PlayerVel * deltaTime);
		else
		{
			if(!IsFalling()) MovePlayer(m_PlayerVel * deltaTime);
		}
	}
}

void C_Player::MovePlayer(const ae::math::float3& moveDir)
{
	const float playerStepHeight(CalculateStepHeight(m_PlayerHeight));
	// new player collision object that has -m_PlayerStepHeight height
#ifdef USE_CYLINDER
	T_PlayerCollision checkCollision(m_PlayerHeight - playerStepHeight, m_PlayerRadius);
#else
	T_PlayerCollision checkCollision(m_PlayerHeight - playerStepHeight, m_PlayerRadius * 2.0f, m_PlayerRadius * 2.0f);
#endif

	// pos to use as sweep check with adjusted height by m_PlayerStepHeight
	ae::physic::S_CollisionParams params;
	ae::physic::S_CollisionResult collResult;
	params.pos = m_PlayerPos;
	params.pos.y = m_PlayerPos.y + (playerStepHeight * 0.5f);
	params.distance = moveDir.size();
	params.direction = moveDir.normalize();
	params.pActorToIgnore = m_pDynamicActor;

	// first we do sweep check in move direction
	if(ae::physic::C_PhysicSys::Instance().Scene().SweepCollision(checkCollision, params, collResult))
		m_pDynamicActor->SetPosition(m_PlayerPos + moveDir); // in case of collision we move dynamic actor and let physic to do it's job
	else
	{ // in other case we calculate new position with adjusted y
		params.pos += moveDir;
		params.distance = playerStepHeight;
		params.direction = ae::math::float3(0, -1, 0);
		// sweep check to the ground to find out new ground y
		if(!ae::physic::C_PhysicSys::Instance().Scene().SweepCollision(checkCollision, params, collResult))
			m_pDynamicActor->SetPosition(m_PlayerPos + moveDir); // no collision player will fall
		else
		{
			ae::math::float3 newPosition(m_PlayerPos + moveDir);
			newPosition.y = collResult.collisionContact.y + (m_PlayerHeight * 0.5f);
			m_pDynamicActor->SetPosition(newPosition);
		}
	}
}

float C_Player::CalculateStepHeight(float playerHeight) const 
{
	AE_ASSERT(playerProneStepHeight <= playerHeight && playerHeight <= playerStandHeight && "Out of valid height !");
	if(playerHeight < playerCrouchHeight)
		return playerProneStepHeight;
	else if(playerHeight < playerCrouchHeight)
		return playerCrouchStepHeight;
	return playerStandStepHeight;
}

void C_Player::HandleZoom(float deltaTime)
{
	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();

	if(input.MouseState(ae::input::E_KS_Key_Down, ae::input::E_MB_RIGHT))
		playerCamera.SetFOV(ae::math::move<>(playerCamera.GetFOV(), m_ZoomFOV, m_ZoomSpeed, deltaTime));
	else
		playerCamera.SetFOV(ae::math::move<>(playerCamera.GetFOV(), m_NoZoomFOV, m_ZoomSpeed, deltaTime));
}

// *****************************************************************************************************
// *****************************************************************************************************
bool C_Player::IsFalling() const {
	return playerDistanceToGround > playerStandStepHeight ? true : false;
}

bool C_Player::IsOnGround() const {
	return playerDistanceToGround < 0.05f ? true : false;
}

void C_Player::HandleJump(float /*deltaTime*/)
{
	if(cameraType != E_Camera_FPS || !IsOnGround()) return;

	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();
	if(input.KeyState(ae::input::E_KS_Key_Pressed, ae::input::E_KC_SPACE))
		m_pDynamicActor->SetLinearVelocity(ae::math::float3(0,playerJumpAcceleration,0) + m_PlayerVel);
}

// is called after physics has been updated
void C_Player::HandleFalling()
{
	ae::physic::S_CollisionResult collResult;
	ae::physic::S_CollisionParams params;
	m_pDynamicActor->GetPosition(params.pos);
	params.distance = 1.0f;
	params.direction = ae::math::float3(0,-1,0);
	params.pActorToIgnore = m_pDynamicActor;

	bool bColliding = ae::physic::C_PhysicSys::Instance().Scene().SweepCollision(m_PlayerCollision, params, collResult);
	playerDistanceToGround = bColliding ? fabs(params.pos.y - collResult.collisionContact.y) - (m_PlayerHeight *0.5f) : 1.0f;

	C_HUD::S_PlayerState ps;
	ps.distanceToGround = playerDistanceToGround;
	ps.isFalling = IsFalling();
	ps.isOnGround = IsOnGround();

	ps.pos = params.pos;
	ps.pos.y -= m_PlayerHeight * 0.5f;
	ps.groundContact = collResult.collisionContact;

	g_pHUD->SetPlayerState(ps);
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_Player::HandleCrouch(float deltaTime)
{
	if(cameraType != E_Camera_FPS) return;

	float newPlayerHeight;
	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();

	if(input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_LCONTROL))
		newPlayerHeight = ae::math::move<>(m_PlayerHeight, playerProneHeight, playerCrouchSpeed, deltaTime);
	else if(input.KeyState(ae::input::E_KS_Key_Down, ae::input::E_KC_C))
		newPlayerHeight = ae::math::move<>(m_PlayerHeight, playerCrouchHeight, playerCrouchSpeed, deltaTime);
	else
		newPlayerHeight = ae::math::move<>(m_PlayerHeight, playerStandHeight, playerCrouchSpeed, deltaTime);

	if(m_PlayerHeight != newPlayerHeight)
	{
		m_PlayerCollision.height = newPlayerHeight;
		m_pDynamicActor->UpdateShape(m_PlayerCollision);
		m_PlayerHeight = newPlayerHeight;
	}
}

} } // namespace ag { namespace entities {

